/*
 * Copyright (C) 2017 ~ 2018 Deepin Technology Co., Ltd.
 *
 * Author:     rekols <rekols@foxmail.com>
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

#include "miniframeswitchbtn.h"
#include "../windowedframe.h"
#include "../global_util/util.h"

#include <QHBoxLayout>

MiniFrameSwitchBtn::MiniFrameSwitchBtn(QWidget *parent)
    : QPushButton(parent)
    , m_textLabel(new QLabel)
    , m_enterIcon(new QLabel)
{
    QPixmap enterPixmap = renderSVG(":/widgets/images/enter_details_normal.svg", QSize(20, 20));
    QPixmap allPixmap = renderSVG(":/widgets/images/all.svg", QSize(24, 24));

    m_enterIcon->setFixedSize(20, 20);
    m_enterIcon->setPixmap(enterPixmap);
    m_enterIcon->setVisible(false);

    QLabel *allIconLabel = new QLabel;
    allIconLabel->setPixmap(allPixmap);
    allIconLabel->setFixedSize(24, 24);

    QHBoxLayout *mainLayout = new QHBoxLayout;
    setLayout(mainLayout);
    setObjectName("MiniFrameSwitchBtn");
    setFixedHeight(36);
    setCheckable(true);

    mainLayout->addSpacing(10);
    mainLayout->addWidget(allIconLabel);
    mainLayout->addSpacing(12);
    mainLayout->addWidget(m_textLabel);
    mainLayout->addWidget(m_enterIcon);
    mainLayout->addSpacing(10);
    mainLayout->setMargin(0);
    mainLayout->setSpacing(0);

    connect(this, &QPushButton::clicked, [=] { setChecked(false); });
}

MiniFrameSwitchBtn::~MiniFrameSwitchBtn()
{
}

void MiniFrameSwitchBtn::updateStatus(int status)
{
    if (status == WindowedFrame::All) {
        m_textLabel->setText(tr("All Categories"));
        m_enterIcon->setVisible(true);
    } else {
        m_textLabel->setText(tr("Back"));
        m_enterIcon->setVisible(false);
    }
}
