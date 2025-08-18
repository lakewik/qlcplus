/*
  Q Light Controller Plus
  mtctimecode.h

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

#ifndef MTCTIMECODE_H
#define MTCTIMECODE_H

#include <QObject>
#include <QTime>

enum FrameRate
{
    FPS_24 = 0,    // 24 fps (film)
    FPS_25 = 1,    // 25 fps (PAL)
    FPS_30 = 2,    // 30 fps (NTSC)
    FPS_30DF = 3   // 30 fps drop frame (NTSC)
};

struct TimeCode
{
    int hours;
    int minutes;
    int seconds;
    int frames;
    FrameRate frameRate;
    
    TimeCode() : hours(0), minutes(0), seconds(0), frames(0), frameRate(FPS_30) {}
    TimeCode(int h, int m, int s, int f, FrameRate fr = FPS_30) 
        : hours(h), minutes(m), seconds(s), frames(f), frameRate(fr) {}
    
    // Convert to milliseconds
    quint32 toMilliseconds() const;
    
    // Convert from milliseconds
    static TimeCode fromMilliseconds(quint32 ms, FrameRate fr = FPS_30);
    
    // Convert to QTime
    QTime toQTime() const;
    
    // Convert from QTime
    static TimeCode fromQTime(const QTime& time, FrameRate fr = FPS_30);
    
    // Convert to BPM (assuming 4/4 time signature)
    int toBPM() const;
    
    // Convert from BPM
    static TimeCode fromBPM(int bpm, FrameRate fr = FPS_30);
};

class MTCTimeCode : public QObject
{
    Q_OBJECT

public:
    MTCTimeCode(QObject* parent = nullptr);
    virtual ~MTCTimeCode();

    // Parse MIDI MTC Quarter Frame messages
    void parseQuarterFrame(uchar data);
    
    // Get the current timecode
    TimeCode currentTimeCode() const;
    
    // Get the current time in milliseconds
    quint32 currentTimeMs() const;
    
    // Get the current BPM
    int currentBPM() const;
    
    // Set frame rate
    void setFrameRate(FrameRate frameRate);
    FrameRate frameRate() const;
    
    // Reset the timecode parser
    void reset();
    
    // Check if we have received a complete timecode
    bool isComplete() const;
    
    // Get the last update time
    QTime lastUpdateTime() const;

signals:
    void timeCodeChanged(const ::TimeCode& timeCode);
    void bpmChanged(int bpm);
    void frameRateChanged(::FrameRate frameRate);

private:
    void updateTimeCode();
    void emitTimeCodeChanged();
    
    // MTC Quarter Frame data storage
    uchar m_quarterFrameData[8];
    bool m_quarterFrameReceived[8];
    
    // Current timecode
    TimeCode m_currentTimeCode;
    
    // Last update time
    QTime m_lastUpdateTime;
    
    // Frame rate
    FrameRate m_frameRate;
    
    // Complete flag
    bool m_isComplete;
};

#endif // MTCTIMECODE_H

