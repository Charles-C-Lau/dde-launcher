
#include "navigationwidget.h"
#include "global_util/constants.h"

#include <QVBoxLayout>
#include <QDebug>

DWIDGET_USE_NAMESPACE

NavigationWidget::NavigationWidget(QWidget *parent) :
    QWidget(parent),
    m_toggleModeBtn(new DImageButton(this)),
    m_categoryGroup(new QButtonGroup(this)),
    m_internetBtn(new CategoryButton(AppsListModel::Internet, this)),
    m_chatBtn(new CategoryButton(AppsListModel::Chat, this)),
    m_musicBtn(new CategoryButton(AppsListModel::Music, this)),
    m_videoBtn(new CategoryButton(AppsListModel::Video, this)),
    m_graphicsBtn(new CategoryButton(AppsListModel::Graphics, this)),
    m_gameBtn(new CategoryButton(AppsListModel::Game, this)),
    m_officeBtn(new CategoryButton(AppsListModel::Office, this)),
    m_readingBtn(new CategoryButton(AppsListModel::Reading, this)),
    m_developmentBtn(new CategoryButton(AppsListModel::Development, this)),
    m_systemBtn(new CategoryButton(AppsListModel::System, this)),
    m_othersBtn(new CategoryButton(AppsListModel::Others, this))

{
    setFixedWidth(DLauncher::NAVIGATION_WIDGET_WIDTH);

    initUI();
    initConnection();
}

void NavigationWidget::setButtonsVisible(const bool visible)
{
    m_internetBtn->setVisible(visible);
    m_chatBtn->setVisible(visible);
    m_musicBtn->setVisible(visible);
    m_videoBtn->setVisible(visible);
    m_graphicsBtn->setVisible(visible);
    m_gameBtn->setVisible(visible);
    m_officeBtn->setVisible(visible);
    m_readingBtn->setVisible(visible);
    m_developmentBtn->setVisible(visible);
    m_systemBtn->setVisible(visible);
    m_othersBtn->setVisible(visible);
}

void NavigationWidget::setCurrentCategory(const AppsListModel::AppCategory category)
{
    CategoryButton *btn = button(category);

    if (btn)
        btn->setChecked(true);
}

void NavigationWidget::refershCategoryVisible(const AppsListModel::AppCategory category, const int appNums)
{
    QAbstractButton *btn = button(category);

    if (btn)
        btn->setVisible(appNums);
}

void NavigationWidget::initUI()
{
    m_toggleModeBtn->setNormalPic(":/skin/images/category_10.svg");
    m_toggleModeBtn->setHoverPic(":/skin/images/category_50.svg");
    m_toggleModeBtn->setPressPic(":/skin/images/category_100.svg");
    m_toggleModeBtn->setStyleSheet("margin:50 0 0 30px;");

    m_categoryGroup->addButton(m_internetBtn);
    m_categoryGroup->addButton(m_chatBtn);
    m_categoryGroup->addButton(m_musicBtn);
    m_categoryGroup->addButton(m_videoBtn);
    m_categoryGroup->addButton(m_graphicsBtn);
    m_categoryGroup->addButton(m_gameBtn);
    m_categoryGroup->addButton(m_officeBtn);
    m_categoryGroup->addButton(m_readingBtn);
    m_categoryGroup->addButton(m_developmentBtn);
    m_categoryGroup->addButton(m_systemBtn);
    m_categoryGroup->addButton(m_othersBtn);

    m_internetBtn->setChecked(true);

    QVBoxLayout *mainLayout = new QVBoxLayout;
    mainLayout->addWidget(m_toggleModeBtn);
    mainLayout->addStretch();
    mainLayout->addWidget(m_internetBtn);
    mainLayout->addWidget(m_chatBtn);
    mainLayout->addWidget(m_musicBtn);
    mainLayout->addWidget(m_videoBtn);
    mainLayout->addWidget(m_graphicsBtn);
    mainLayout->addWidget(m_gameBtn);
    mainLayout->addWidget(m_officeBtn);
    mainLayout->addWidget(m_readingBtn);
    mainLayout->addWidget(m_developmentBtn);
    mainLayout->addWidget(m_systemBtn);
    mainLayout->addWidget(m_othersBtn);
    mainLayout->addStretch();
    mainLayout->setSpacing(0);
    mainLayout->setMargin(0);

    setLayout(mainLayout);
}

void NavigationWidget::initConnection()
{
    connect(m_internetBtn, &CategoryButton::clicked, this, &NavigationWidget::buttonClicked);
    connect(m_musicBtn, &CategoryButton::clicked, this, &NavigationWidget::buttonClicked);
    connect(m_chatBtn, &CategoryButton::clicked, this, &NavigationWidget::buttonClicked);
    connect(m_videoBtn, &CategoryButton::clicked, this, &NavigationWidget::buttonClicked);
    connect(m_graphicsBtn, &CategoryButton::clicked, this, &NavigationWidget::buttonClicked);
    connect(m_gameBtn, &CategoryButton::clicked, this, &NavigationWidget::buttonClicked);
    connect(m_officeBtn, &CategoryButton::clicked, this, &NavigationWidget::buttonClicked);
    connect(m_readingBtn, &CategoryButton::clicked, this, &NavigationWidget::buttonClicked);
    connect(m_developmentBtn, &CategoryButton::clicked, this, &NavigationWidget::buttonClicked);
    connect(m_systemBtn, &CategoryButton::clicked, this, &NavigationWidget::buttonClicked);
    connect(m_othersBtn, &CategoryButton::clicked, this, &NavigationWidget::buttonClicked);

    connect(m_toggleModeBtn, &DImageButton::clicked, this, &NavigationWidget::toggleModeClicked);
}

void NavigationWidget::buttonClicked()
{
    CategoryButton *btn = qobject_cast<CategoryButton *>(sender());
    if (!btn)
        return;

    emit scrollToCategory(btn->category());
}

CategoryButton *NavigationWidget::button(const AppsListModel::AppCategory category)
{
    switch (category)
    {
    case AppsListModel::Internet:       return m_internetBtn;
    case AppsListModel::Chat:           return m_chatBtn;
    case AppsListModel::Music:          return m_musicBtn;
    case AppsListModel::Video:          return m_videoBtn;
    case AppsListModel::Graphics:       return m_graphicsBtn;
    case AppsListModel::Game:           return m_gameBtn;
    case AppsListModel::Office:         return m_officeBtn;
    case AppsListModel::Reading:        return m_readingBtn;
    case AppsListModel::Development:    return m_developmentBtn;
    case AppsListModel::System:         return m_systemBtn;
    case AppsListModel::Others:         return m_othersBtn;
    default:;
    }

    return nullptr;
}
