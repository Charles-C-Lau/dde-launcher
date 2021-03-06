/*
 * Copyright (C) 2017 ~ 2018 Deepin Technology Co., Ltd.
 *
 * Author:     sbw <sbw@sbw.so>
 *
 * Maintainer: sbw <sbw@sbw.so>
 *             kirigaya <kirigaya@mkacg.com>
 *             Hualet <mr.asianwang@gmail.com>
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include "navigationwidget.h"
#include "src/global_util/constants.h"

#include <QVBoxLayout>
#include <QDebug>

NavigationWidget::NavigationWidget(QWidget *parent) :
    QFrame(parent),
    m_calcUtil(CalculateUtil::instance()),
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

void NavigationWidget::setCategoryTextVisible(const bool visible, const bool animation)
{
    m_internetBtn->setTextVisible(visible, animation);
    m_chatBtn->setTextVisible(visible, animation);
    m_musicBtn->setTextVisible(visible, animation);
    m_videoBtn->setTextVisible(visible, animation);
    m_graphicsBtn->setTextVisible(visible, animation);
    m_gameBtn->setTextVisible(visible, animation);
    m_officeBtn->setTextVisible(visible, animation);
    m_readingBtn->setTextVisible(visible, animation);
    m_developmentBtn->setTextVisible(visible, animation);
    m_systemBtn->setTextVisible(visible, animation);
    m_othersBtn->setTextVisible(visible, animation);
}

QLabel *NavigationWidget::categoryTextLabel(const AppsListModel::AppCategory category) const
{
    CategoryButton *btn = button(category);

    return btn ? btn->textLabel() : nullptr;
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
    mainLayout->addSpacing(30);
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
}

void NavigationWidget::buttonClicked()
{
    CategoryButton *btn = qobject_cast<CategoryButton *>(sender());
    if (!btn)
        return;

    emit scrollToCategory(btn->category());
}

CategoryButton *NavigationWidget::button(const AppsListModel::AppCategory category) const
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

qreal NavigationWidget::zoomLevel() const
{
    return m_zoomLevel;
}

void NavigationWidget::setZoomLevel(const qreal &zoomLevel)
{
    if (m_zoomLevel != zoomLevel) {
        m_zoomLevel = zoomLevel;

        m_internetBtn->setZoomLevel(zoomLevel);
        m_chatBtn->setZoomLevel(zoomLevel);
        m_musicBtn->setZoomLevel(zoomLevel);
        m_videoBtn->setZoomLevel(zoomLevel);
        m_graphicsBtn->setZoomLevel(zoomLevel);
        m_gameBtn->setZoomLevel(zoomLevel);
        m_officeBtn->setZoomLevel(zoomLevel);
        m_readingBtn->setZoomLevel(zoomLevel);
        m_developmentBtn->setZoomLevel(zoomLevel);
        m_systemBtn->setZoomLevel(zoomLevel);
        m_othersBtn->setZoomLevel(zoomLevel);
    }
}

void NavigationWidget::enterEvent(QEvent *e)
{
    QFrame::enterEvent(e);

    QPropertyAnimation * ani = new QPropertyAnimation(this, "zoomLevel");
    ani->setDuration(300);
    ani->setStartValue(zoomLevel());
    ani->setEndValue(1.2);
    ani->start(QPropertyAnimation::DeleteWhenStopped);

    emit mouseEntered();
}

void NavigationWidget::leaveEvent(QEvent *e)
{
    QFrame::leaveEvent(e);

    QPropertyAnimation * ani = new QPropertyAnimation(this, "zoomLevel");
    ani->setDuration(300);
    ani->setEndValue(zoomLevel());
    ani->setEndValue(1);
    ani->start(QPropertyAnimation::DeleteWhenStopped);
}

void NavigationWidget::showEvent(QShowEvent *e)
{
    QFrame::showEvent(e);

    QTimer::singleShot(0, this, &NavigationWidget::raise);
}
