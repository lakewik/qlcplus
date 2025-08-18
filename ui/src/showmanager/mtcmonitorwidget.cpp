/*
  Q Light Controller Plus
  mtcmonitorwidget.cpp

  Copyright (c) 2024 QLC+ Contributors

  Licensed under the Apache License, Version 2.0 (the "License");
  you may not use this file except in compliance with the License.
  You may obtain a copy of the License at

      http://www.apache.org/licenses/LICENSE-2.0.txt

  Unless required by applicable law or agreed to in writing, software
  distributed under the License is distributed on an "AS IS" BASIS,
  WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
  See the License for the specific language governing permissions and
  limitations under the License.
*/

#include <QDebug>
#include "mtcmonitorwidget.h"
#include "show.h"

MTCMonitorWidget::MTCMonitorWidget(QWidget* parent)
    : QWidget(parent)
    , m_show(nullptr)
{
    setupUI();
}

MTCMonitorWidget::~MTCMonitorWidget()
{
}

void MTCMonitorWidget::setupUI()
{
    QVBoxLayout* mainLayout = new QVBoxLayout(this);
    mainLayout->setContentsMargins(5, 5, 5, 5);
    mainLayout->setSpacing(5);

    m_mtcGroup = new QGroupBox(tr("MIDI Time Code (MTC)"));
    mainLayout->addWidget(m_mtcGroup);

    QVBoxLayout* groupLayout = new QVBoxLayout(m_mtcGroup);
    groupLayout->setSpacing(5);

    // MTC Enable checkbox
    m_mtcEnabledCheck = new QCheckBox(tr("Enable MTC synchronization"));
    groupLayout->addWidget(m_mtcEnabledCheck);
    connect(m_mtcEnabledCheck, SIGNAL(toggled(bool)), 
            this, SLOT(slotMTCCheckboxChanged(bool)));

    // MTC Status
    QHBoxLayout* statusLayout = new QHBoxLayout();
    statusLayout->addWidget(new QLabel(tr("Status:")));
    m_mtcStatusLabel = new QLabel(tr("Off"));
    m_mtcStatusLabel->setStyleSheet("QLabel { color: red; font-weight: bold; }");
    statusLayout->addWidget(m_mtcStatusLabel);
    statusLayout->addStretch();
    groupLayout->addLayout(statusLayout);

    // MTC Time
    QHBoxLayout* timeLayout = new QHBoxLayout();
    timeLayout->addWidget(new QLabel(tr("Time:")));
    m_mtcTimeLabel = new QLabel("00:00:00:00");
    m_mtcTimeLabel->setStyleSheet("QLabel { color: blue; font-weight: bold; font-family: monospace; }");
    timeLayout->addWidget(m_mtcTimeLabel);
    timeLayout->addStretch();
    groupLayout->addLayout(timeLayout);

    // MTC BPM
    QHBoxLayout* bpmLayout = new QHBoxLayout();
    bpmLayout->addWidget(new QLabel(tr("BPM:")));
    m_mtcBPMLabel = new QLabel("--");
    m_mtcBPMLabel->setStyleSheet("QLabel { color: green; font-weight: bold; }");
    bpmLayout->addWidget(m_mtcBPMLabel);
    bpmLayout->addStretch();
    groupLayout->addLayout(bpmLayout);

    // Sync button
    m_mtcSyncButton = new QPushButton(tr("Sync to MTC"));
    m_mtcSyncButton->setEnabled(false);
    groupLayout->addWidget(m_mtcSyncButton);

    groupLayout->addStretch();
}

void MTCMonitorWidget::setShow(Show* show)
{
    m_show = show;
    if (m_show != nullptr)
    {
        m_mtcEnabledCheck->setChecked(m_show->isMTCEnabled());
        m_mtcSyncButton->setEnabled(m_show->isMTCEnabled());
    }
}

void MTCMonitorWidget::updateMTCStatus(bool enabled)
{
    if (enabled)
    {
        m_mtcStatusLabel->setText(tr("On"));
        m_mtcStatusLabel->setStyleSheet("QLabel { color: green; font-weight: bold; }");
        m_mtcSyncButton->setEnabled(true);
    }
    else
    {
        m_mtcStatusLabel->setText(tr("Off"));
        m_mtcStatusLabel->setStyleSheet("QLabel { color: red; font-weight: bold; }");
        m_mtcSyncButton->setEnabled(false);
    }
}

void MTCMonitorWidget::updateMTCTimeCode(const QString& timeStr)
{
    m_mtcTimeLabel->setText(timeStr);
}

void MTCMonitorWidget::updateMTCBPM(int bpm)
{
    if (bpm > 0)
        m_mtcBPMLabel->setText(QString::number(bpm));
    else
        m_mtcBPMLabel->setText("--");
}

void MTCMonitorWidget::slotMTCCheckboxChanged(bool checked)
{
    if (m_show != nullptr)
    {
        m_show->setMTCEnabled(checked);
        emit mtcEnabledChanged(checked);
    }
}

