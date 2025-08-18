/*
  Q Light Controller Plus
  mtctest.cpp

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

#include <QTest>
#include <QSignalSpy>

#include "mtctimecode.h"

class MTCTest : public QObject
{
    Q_OBJECT

private slots:
    void testTimeCodeCreation();
    void testTimeCodeConversion();
    void testMTCQuarterFrameParsing();
    void testFrameRateDetection();
    void testBPMCalculation();
};

void MTCTest::testTimeCodeCreation()
{
    MTCTimeCode::TimeCode tc(1, 30, 45, 12, MTCTimeCode::FPS_30);
    
    QCOMPARE(tc.hours, 1);
    QCOMPARE(tc.minutes, 30);
    QCOMPARE(tc.seconds, 45);
    QCOMPARE(tc.frames, 12);
    QCOMPARE(tc.frameRate, MTCTimeCode::FPS_30);
}

void MTCTest::testTimeCodeConversion()
{
    MTCTimeCode::TimeCode tc(0, 1, 0, 0, MTCTimeCode::FPS_30);
    quint32 ms = tc.toMilliseconds();
    
    // 1 minute = 60 seconds = 60000 milliseconds
    QCOMPARE(ms, 60000u);
    
    // Test conversion back
    MTCTimeCode::TimeCode converted = MTCTimeCode::TimeCode::fromMilliseconds(ms, MTCTimeCode::FPS_30);
    QCOMPARE(converted.minutes, 1);
    QCOMPARE(converted.seconds, 0);
    QCOMPARE(converted.frames, 0);
}

void MTCTest::testMTCQuarterFrameParsing()
{
    MTCTimeCode mtc;
    QSignalSpy timeCodeSpy(&mtc, &MTCTimeCode::timeCodeChanged);
    
    // Send MTC Quarter Frame messages for 00:01:00:00 (1 minute)
    // Frame LSB
    mtc.parseQuarterFrame(0x00);
    // Frame MSB
    mtc.parseQuarterFrame(0x10);
    // Seconds LSB
    mtc.parseQuarterFrame(0x20);
    // Seconds MSB
    mtc.parseQuarterFrame(0x30);
    // Minutes LSB
    mtc.parseQuarterFrame(0x40);
    // Minutes MSB
    mtc.parseQuarterFrame(0x50);
    // Hours LSB
    mtc.parseQuarterFrame(0x60);
    // Hours MSB + Frame Rate
    mtc.parseQuarterFrame(0x70);
    
    QCOMPARE(timeCodeSpy.count(), 1);
    
    MTCTimeCode::TimeCode received = timeCodeSpy.first().at(0).value<MTCTimeCode::TimeCode>();
    QCOMPARE(received.hours, 0);
    QCOMPARE(received.minutes, 1);
    QCOMPARE(received.seconds, 0);
    QCOMPARE(received.frames, 0);
}

void MTCTest::testFrameRateDetection()
{
    MTCTimeCode mtc;
    
    // Test frame rate detection from MTC data
    mtc.parseQuarterFrame(0x70); // Hours MSB + Frame Rate (FPS_30)
    
    QCOMPARE(mtc.frameRate(), MTCTimeCode::FPS_30);
}

void MTCTest::testBPMCalculation()
{
    // Test BPM calculation (this is a simplified test)
    MTCTimeCode::TimeCode tc(0, 0, 1, 0, MTCTimeCode::FPS_30);
    
    // 1 second should give us a reasonable BPM
    int bpm = tc.toBPM();
    QVERIFY(bpm > 0);
}

QTEST_MAIN(MTCTest)

#include "mtctest.moc"
