/*
  Q Light Controller
  midiinputdevice.cpp

  Copyright (c) Heikki Junnila

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
#include "midiinputdevice.h"
#include "mtctimecode.h"

MidiInputDevice::MidiInputDevice(const QVariant& uid, const QString& name, QObject* parent)
    : MidiDevice(uid, name, Input, parent)
    , m_mtcTimeCode(new MTCTimeCode(this))
    , m_mtcEnabled(false)
{
    //qDebug() << Q_FUNC_INFO;
    
    // Connect MTC signals
    connect(m_mtcTimeCode, &MTCTimeCode::timeCodeChanged,
            this, &MidiInputDevice::mtcTimeCodeChanged);
    connect(m_mtcTimeCode, &MTCTimeCode::bpmChanged,
            this, &MidiInputDevice::mtcBPMChanged);
}

MidiInputDevice::~MidiInputDevice()
{
    //qDebug() << Q_FUNC_INFO;
}

void MidiInputDevice::emitValueChanged(uint channel, uchar value)
{
    emit valueChanged(uid(), channel, value);
}

void MidiInputDevice::enableMTC(bool enable)
{
    m_mtcEnabled = enable;
    if (enable)
    {
        m_mtcTimeCode->reset();
        qDebug() << "MTC enabled for device:" << name();
    }
    else
    {
        qDebug() << "MTC disabled for device:" << name();
    }
}

bool MidiInputDevice::isMTCEnabled() const
{
    return m_mtcEnabled;
}

void MidiInputDevice::processMTCQuarterFrame(uchar data)
{
    if (m_mtcEnabled)
    {
        m_mtcTimeCode->parseQuarterFrame(data);
    }
}

MTCTimeCode* MidiInputDevice::mtcTimeCode() const
{
    return m_mtcTimeCode;
}
