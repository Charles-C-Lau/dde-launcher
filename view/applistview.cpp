#include "applistview.h"
#include "global_util/constants.h"
#include "global_util/calculate_util.h"
#include "model/appslistmodel.h"

#include <QDebug>
#include <QWheelEvent>
#include <QTimer>
#include <QDrag>
#include <QMimeData>
#include <QtGlobal>

#include <QDrag>

AppsManager *AppListView::m_appManager = nullptr;
CalculateUtil *AppListView::m_calcUtil = nullptr;

AppListView::AppListView(QWidget *parent) :
    QListView(parent),
    m_dropThresholdTimer(new QTimer(this))
{
    if (!m_appManager)
        m_appManager = AppsManager::instance(this);
    if (!m_calcUtil)
        m_calcUtil = CalculateUtil::instance(this);

    m_dropThresholdTimer->setInterval(100);
    m_dropThresholdTimer->setSingleShot(true);

    viewport()->installEventFilter(this);
    viewport()->setAcceptDrops(true);
    setUniformItemSizes(true);

    setMouseTracking(true);
    setAcceptDrops(true);
    setDragEnabled(true);
    setWrapping(true);
    setFocusPolicy(Qt::NoFocus);
    setDragDropMode(QAbstractItemView::DragDrop);
    setMovement(QListView::Free);
    setFlow(QListView::LeftToRight);
    setLayoutMode(QListView::Batched);
    setResizeMode(QListView::Adjust);
    setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    setFrameStyle(QFrame::NoFrame);

    setStyleSheet("background-color:transparent;");

    // update item spacing
    connect(m_calcUtil, &CalculateUtil::layoutChanged, [this] {setSpacing(m_calcUtil->appItemSpacing());});
    connect(m_dropThresholdTimer, &QTimer::timeout, this, &AppListView::dropSwap);
}

const QModelIndex AppListView::indexAt(const int index) const
{
    return model()->index(index, 0, QModelIndex());
}

int AppListView::indexYOffset(const QModelIndex &index) const
{
    return rectForIndex(index).top();
}

void AppListView::enterEvent(QEvent *e)
{
    QListView::enterEvent(e);
}

void AppListView::dropEvent(QDropEvent *e)
{
    Q_UNUSED(e)

    m_enableDropInside = true;
}

void AppListView::mousePressEvent(QMouseEvent *e)
{
    if (e->buttons() == Qt::RightButton) {
        QPoint rightClickPoint = mapToGlobal(e->pos());

        const QModelIndex &clickedIndex = QListView::indexAt(e->pos());
        if (clickedIndex.isValid())
            emit popupMenuRequested(rightClickPoint, clickedIndex);
    }

    if (e->buttons() == Qt::LeftButton)
        m_dragStartPos = e->pos();

    QListView::mousePressEvent(e);
}

void AppListView::dragEnterEvent(QDragEnterEvent *e)
{
    e->accept();
}

void AppListView::dragMoveEvent(QDragMoveEvent *e)
{
//    if (!m_isDragging) {
//        const QModelIndex &beDragedIndex = QListView::indexAt(e->pos());
//        qDebug() << "beDragedIndex name:"
//                 << beDragedIndex.data(AppsListModel::AppKeyRole).toString();

//        emit appBeDraged(beDragedIndex);
//        m_isDragging = true;
//    } else {
//        return;
//    }
    const QModelIndex dropIndex = QListView::indexAt(e->pos());
    if (dropIndex.isValid())
        m_dropToPos = dropIndex.row();

    m_dropThresholdTimer->start();
}

void AppListView::dragLeaveEvent(QDragLeaveEvent *e)
{
//    m_isDragging = false;
    Q_UNUSED(e);

    m_dropThresholdTimer->stop();
}

void AppListView::mouseMoveEvent(QMouseEvent *e)
{
    // disable default drag
    setState(NoState);

    QListView::mouseMoveEvent(e);

    if (e->buttons() != Qt::LeftButton)
        return;

    if (qAbs(e->pos().x() - m_dragStartPos.x()) > DLauncher::DRAG_THRESHOLD ||
        qAbs(e->pos().y() - m_dragStartPos.y()) > DLauncher::DRAG_THRESHOLD)
        startDrag(QListView::indexAt(e->pos()));
}

void AppListView::mouseReleaseEvent(QMouseEvent *e)
{
    // request main frame hide when click invalid area
    if (e->button() != Qt::LeftButton)
        return;

    const QModelIndex index = QListView::indexAt(e->pos());
    if (!index.isValid())
        emit clicked(index);

    QListView::mouseReleaseEvent(e);
}

void AppListView::resizeEvent(QResizeEvent *e)
{
    QListView::resizeEvent(e);
}

void AppListView::wheelEvent(QWheelEvent *e)
{
    e->ignore();
}

void AppListView::currentChanged(const QModelIndex &current, const QModelIndex &previous)
{
    QListView::currentChanged(current, previous);
}

void AppListView::startDrag(const QModelIndex &index)
{
    if (!index.isValid())
        return;

    AppsListModel *listModel = qobject_cast<AppsListModel *>(model());
    if (!listModel)
        return;

    const QModelIndex &dragIndex = index;
    const QString appKey = index.data(AppsListModel::AppKeyRole).toString();
    const QPixmap pixmap = index.data(AppsListModel::AppIconRole).value<QPixmap>();

    QDrag *drag = new QDrag(this);
    drag->setMimeData(model()->mimeData(QModelIndexList() << dragIndex));
    drag->setPixmap(pixmap.scaled(DLauncher::APP_DRAG_ICON_SIZE, DLauncher::APP_DRAG_ICON_SIZE, Qt::IgnoreAspectRatio, Qt::SmoothTransformation));
    drag->setHotSpot(QPoint(DLauncher::APP_DRAG_ICON_SIZE / 2, DLauncher::APP_DRAG_ICON_SIZE / 2));

    // request remove current item.
    if (listModel->category() == AppsListModel::All)
    {
        m_dropToPos = index.row();
        listModel->setDragingIndex(index);
    }

    drag->exec(Qt::MoveAction);

    if (listModel->category() != AppsListModel::All)
        return;

    if (m_enableDropInside)
        listModel->dropSwap(m_dropToPos);
    else
        listModel->dropSwap(indexAt(m_dragStartPos).row());
    listModel->clearDragingIndex();
    m_enableDropInside = false;
}

bool AppListView::eventFilter(QObject *o, QEvent *e)
{
    if (o == viewport() && e->type() == QEvent::Paint)
        fitToContent();

    return false;
}

void AppListView::fitToContent()
{
    if (width() == contentsRect().width() && height() == contentsSize().height())
        return;
    int tmpHeight = qMax(contentsSize().height(), 1);
//    qDebug() << "contentsRect:" << contentsRect().width() << contentsSize().height();
    setFixedWidth(contentsRect().width());
    setFixedHeight(tmpHeight);
}

void AppListView::dropSwap()
{
    AppsListModel *listModel = qobject_cast<AppsListModel *>(model());
    if (!listModel)
        return;

    listModel->dropSwap(m_dropToPos);
}
