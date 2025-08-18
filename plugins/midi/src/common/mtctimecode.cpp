/*
  Q Light Controller Plus
  mtctimecode.cpp

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

#include "mtctimecode.h"
#include <QDebug>

MTCTimeCode::MTCTimeCode(QObject* parent)
    : QObject(parent)
    , m_frameRate(FPS_30)
    , m_isComplete(false)
{
    reset();
}

MTCTimeCode::~MTCTimeCode()
{
}

void MTCTimeCode::parseQuarterFrame(uchar data)
{
    // MTC Quarter Frame message format:
    // - Bit 7: Message type (0 = time, 1 = user bits)
    // - Bits 6-4: Message number (0-7)
    // - Bits 3-0: Data nibble
    
    uchar messageType = (data >> 7) & 0x01;
    uchar messageNumber = (data >> 4) & 0x07;
    uchar dataNibble = data & 0x0F;
    
    // We only care about time messages (messageType = 0)
    if (messageType != 0)
        return;
    
    // Store the data nibble
    m_quarterFrameData[messageNumber] = dataNibble;
    m_quarterFrameReceived[messageNumber] = true;
    
    // Check if we have received all 8 quarter frames
    bool allReceived = true;
    for (int i = 0; i < 8; i++)
    {
        if (!m_quarterFrameReceived[i])
        {
            allReceived = false;
            break;
        }
    }
    
    if (allReceived)
    {
        updateTimeCode();
        m_isComplete = true;
        emitTimeCodeChanged();
        
        // Reset for next timecode
        reset();
    }
    
    m_lastUpdateTime = QTime::currentTime();
}

MTCTimeCode::TimeCode MTCTimeCode::currentTimeCode() const
{
    return m_currentTimeCode;
}

quint32 MTCTimeCode::currentTimeMs() const
{
    return m_currentTimeCode.toMilliseconds();
}

int MTCTimeCode::currentBPM() const
{
    return m_currentTimeCode.toBPM();
}

void MTCTimeCode::setFrameRate(FrameRate frameRate)
{
    if (m_frameRate != frameRate)
    {
        m_frameRate = frameRate;
        m_currentTimeCode.frameRate = frameRate;
        emit frameRateChanged(frameRate);
    }
}

MTCTimeCode::FrameRate MTCTimeCode::frameRate() const
{
    return m_frameRate;
}

void MTCTimeCode::reset()
{
    for (int i = 0; i < 8; i++)
    {
        m_quarterFrameData[i] = 0;
        m_quarterFrameReceived[i] = false;
    }
    m_isComplete = false;
}

bool MTCTimeCode::isComplete() const
{
    return m_isComplete;
}

QTime MTCTimeCode::lastUpdateTime() const
{
    return m_lastUpdateTime;
}

void MTCTimeCode::updateTimeCode()
{
    // MTC Quarter Frame order:
    // 0: Frame number LSB (bits 0-3)
    // 1: Frame number MSB (bits 4-5)
    // 2: Seconds LSB (bits 0-3)
    // 3: Seconds MSB (bits 4-5)
    // 4: Minutes LSB (bits 0-3)
    // 5: Minutes MSB (bits 4-5)
    // 6: Hours LSB (bits 0-3)
    // 7: Hours MSB (bit 4) + SMPTE type (bits 5-6) + Frame rate (bit 7)
    
    int frames = m_quarterFrameData[0] | (m_quarterFrameData[1] << 4);
    int seconds = m_quarterFrameData[2] | (m_quarterFrameData[3] << 4);
    int minutes = m_quarterFrameData[4] | (m_quarterFrameData[5] << 4);
    int hours = m_quarterFrameData[6] | (m_quarterFrameData[7] << 4);
    
    // Extract frame rate from the last quarter frame
    uchar frameRateBits = (m_quarterFrameData[7] >> 1) & 0x03;
    FrameRate detectedFrameRate = static_cast<FrameRate>(frameRateBits);
    
    // Update frame rate if it changed
    if (detectedFrameRate != m_frameRate)
    {
        m_frameRate = detectedFrameRate;
        emit frameRateChanged(m_frameRate);
    }
    
    m_currentTimeCode = TimeCode(hours, minutes, seconds, frames, m_frameRate);
}

void MTCTimeCode::emitTimeCodeChanged()
{
    emit timeCodeChanged(m_currentTimeCode);
    
    // Calculate and emit BPM if we have a valid timecode
    int bpm = m_currentTimeCode.toBPM();
    if (bpm > 0)
        emit bpmChanged(bpm);
}

// TimeCode struct methods implementation

quint32 MTCTimeCode::TimeCode::toMilliseconds() const
{
    // Calculate milliseconds based on frame rate
    double frameDuration;
    switch (frameRate)
    {
        case FPS_24:
            frameDuration = 1000.0 / 24.0;
            break;
        case FPS_25:
            frameDuration = 1000.0 / 25.0;
            break;
        case FPS_30:
            frameDuration = 1000.0 / 30.0;
            break;
        case FPS_30DF:
            // Drop frame calculation is more complex, simplified here
            frameDuration = 1000.0 / 30.0;
            break;
        default:
            frameDuration = 1000.0 / 30.0;
            break;
    }
    
    quint32 totalMs = 0;
    totalMs += hours * 3600000;      // hours to ms
    totalMs += minutes * 60000;      // minutes to ms
    totalMs += seconds * 1000;       // seconds to ms
    totalMs += static_cast<quint32>(frames * frameDuration); // frames to ms
    
    return totalMs;
}

MTCTimeCode::TimeCode MTCTimeCode::TimeCode::fromMilliseconds(quint32 ms, FrameRate fr)
{
    TimeCode tc;
    tc.frameRate = fr;
    
    double frameDuration;
    switch (fr)
    {
        case FPS_24:
            frameDuration = 1000.0 / 24.0;
            break;
        case FPS_25:
            frameDuration = 1000.0 / 25.0;
            break;
        case FPS_30:
        case FPS_30DF:
            frameDuration = 1000.0 / 30.0;
            break;
        default:
            frameDuration = 1000.0 / 30.0;
            break;
    }
    
    tc.hours = ms / 3600000;
    ms %= 3600000;
    
    tc.minutes = ms / 60000;
    ms %= 60000;
    
    tc.seconds = ms / 1000;
    ms %= 1000;
    
    tc.frames = static_cast<int>(ms / frameDuration);
    
    return tc;
}

QTime MTCTimeCode::TimeCode::toQTime() const
{
    return QTime(hours, minutes, seconds);
}

MTCTimeCode::TimeCode MTCTimeCode::TimeCode::fromQTime(const QTime& time, FrameRate fr)
{
    return TimeCode(time.hour(), time.minute(), time.second(), 0, fr);
}

int MTCTimeCode::TimeCode::toBPM() const
{
    // Convert timecode to BPM assuming 4/4 time signature
    // This is a simplified calculation - in practice, you'd need to know the musical tempo
    // For now, we'll use a basic conversion based on seconds
    
    if (seconds == 0 && frames == 0)
        return 0; // Can't determine BPM from 0 time
        
    // Calculate total time in seconds
    double totalSeconds = hours * 3600.0 + minutes * 60.0 + seconds + (frames / 30.0);
    
    // Assuming 4/4 time, 4 beats per measure
    // This is a very rough approximation
    if (totalSeconds > 0)
    {
        // Convert to BPM (beats per minute)
        // This is a simplified calculation and may not be accurate for all musical contexts
        return static_cast<int>(240.0 / totalSeconds); // 4 beats * 60 seconds / total time
    }
    
    return 0;
}

MTCTimeCode::TimeCode MTCTimeCode::TimeCode::fromBPM(int bpm, FrameRate fr)
{
    if (bpm <= 0)
        return TimeCode(0, 0, 0, 0, fr);
    
    // Convert BPM to timecode
    // Assuming 4/4 time signature
    double secondsPerBeat = 60.0 / bpm;
    double totalSeconds = secondsPerBeat * 4; // 4 beats per measure
    
    int hours = static_cast<int>(totalSeconds) / 3600;
    int minutes = (static_cast<int>(totalSeconds) % 3600) / 60;
    int seconds = static_cast<int>(totalSeconds) % 60;
    int frames = static_cast<int>((totalSeconds - static_cast<int>(totalSeconds)) * 30);
    
    return TimeCode(hours, minutes, seconds, frames, fr);
}

