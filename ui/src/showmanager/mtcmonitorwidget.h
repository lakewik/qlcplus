/*
  Q Light Controller Plus
  mtcmonitorwidget.h

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

#ifndef MTCMONITORWIDGET_H
#define MTCMONITORWIDGET_H

#include <QWidget>
#include <QLabel>
#include <QPushButton>
#include <QCheckBox>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QGroupBox>

class Show;

class MTCMonitorWidget : public QWidget
{
    Q_OBJECT

public:
    MTCMonitorWidget(QWidget* parent = nullptr);
    ~MTCMonitorWidget();

    void setShow(Show* show);
    void updateMTCStatus(bool enabled);
    void updateMTCTimeCode(const QString& timeStr);
    void updateMTCBPM(int bpm);

signals:
    void mtcEnabledChanged(bool enabled);

private slots:
    void slotMTCCheckboxChanged(bool checked);

private:
    void setupUI();

private:
    Show* m_show;
    
    QGroupBox* m_mtcGroup;
    QCheckBox* m_mtcEnabledCheck;
    QLabel* m_mtcStatusLabel;
    QLabel* m_mtcTimeLabel;
    QLabel* m_mtcBPMLabel;
    QPushButton* m_mtcSyncButton;
};

#endif // MTCMONITORWIDGET_H

