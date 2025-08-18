/*
  Q Light Controller Plus
  show.h

  Copyright (c) Massimo Callegari

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

#ifndef SHOW_H
#define SHOW_H

#include <QMutex>
#include <QList>
#include <QSet>

#include "function.h"
#include "track.h"

// Simple TimeCode struct for MTC support
struct TimeCode
{
    int hours;
    int minutes;
    int seconds;
    int frames;
    int frameRate; // 0=24fps, 1=25fps, 2=30fps, 3=30fps drop frame
    
    TimeCode() : hours(0), minutes(0), seconds(0), frames(0), frameRate(2) {}
    TimeCode(int h, int m, int s, int f, int fr = 2) 
        : hours(h), minutes(m), seconds(s), frames(f), frameRate(fr) {}
    
    // Convert to milliseconds
    quint32 toMilliseconds() const;
};

class QXmlStreamReader;
class ShowRunner;

/** @addtogroup engine_functions Functions
 * @{
 */

class Show : public Function
{
    Q_OBJECT
    Q_DISABLE_COPY(Show)

    /*********************************************************************
     * Initialization
     *********************************************************************/
public:
    Show(Doc* doc);
    virtual ~Show();

    /** @reimp */
    QIcon getIcon() const;

    /** @reimp */
    quint32 totalDuration();

    /*********************************************************************
     * Copying
     *********************************************************************/
public:
    /** @reimp */
    Function* createCopy(Doc* doc, bool addToDoc = true);

    /** Copy the contents for this function from another function */
    bool copyFrom(const Function* function);

    /*********************************************************************
     * Time division
     *********************************************************************/
public:
    enum TimeDivision
    {
        Time = 0,
        BPM_4_4,
        BPM_3_4,
        BPM_2_4,
        MTC,
        Invalid
    };
    Q_ENUM(TimeDivision)

    /** Set the show time division type (Time, BPM) */
    void setTimeDivision(Show::TimeDivision type, int BPM);

    Show::TimeDivision timeDivisionType();
    void setTimeDivisionType(Show::TimeDivision type);
    int beatsDivision();

    int timeDivisionBPM();
    void setTimeDivisionBPM(int BPM);

    static QString tempoToString(Show::TimeDivision type);
    static Show::TimeDivision stringToTempo(QString tempo);

    // MTC support
    void setMTCEnabled(bool enabled);
    bool isMTCEnabled() const;
    void setMTCTimeCode(const TimeCode& timeCode);
    TimeCode currentMTCTimeCode() const;
    quint32 currentMTCTimeMs() const;

private:
    TimeDivision m_timeDivisionType;
    int m_timeDivisionBPM;
    bool m_mtcEnabled;
    TimeCode m_currentMTCTimeCode;

    /*********************************************************************
     * Tracks
     *********************************************************************/
public:
    /**
     * Add a track to this show. If the track is already a
     * member of the show, this call fails.
     *
     * @param id The track to add
     * @return true if successful, otherwise false
     */
    bool addTrack(Track *track, quint32 id = Track::invalidId());

    /**
     * Remove a track from this show. If the track is not a
     * member of the show, this call fails.
     *
     * @param id The track to remove
     * @return true if successful, otherwise false
     */
    bool removeTrack(quint32 id);

    /** Get a track by id */
    Track *track(quint32 id) const;

    /** Get a reference to a Track from the provided Scene ID */
    Track *getTrackFromSceneID(quint32 id);

    /** Get a reference to a Track from the provided ShowFunction ID */
    Track *getTrackFromShowFunctionID(quint32 id);

    /** Get the number of tracks in the Show */
    int getTracksCount();

    /** Move a track ID up or down */
    void moveTrack(Track *track, int direction);

    /** Get a list of available tracks */
    QList <Track*> tracks() const;

private:
    /** Create a new track ID */
    quint32 createTrackId();

protected:
    /** Map of the available tracks coupled by ID */
    QMap <quint32,Track*> m_tracks;

    /** Latest assigned track ID */
    quint32 m_latestTrackId;

    /*********************************************************************
     * Show Functions
     *********************************************************************/
public:
    /** Get a unique ID for the creation of a new ShowFunction */
    quint32 getLatestShowFunctionId();

    /** Get a reference to a ShowFunction from the provided uinique ID */
    ShowFunction *showFunction(quint32 id);

protected:
    /** Latest assigned unique ShowFunction ID */
    quint32 m_latestShowFunctionID;

    /*********************************************************************
     * Save & Load
     *********************************************************************/
public:
    /** Save function's contents to an XML document */
    bool saveXML(QXmlStreamWriter *doc);

    /** Load function's contents from an XML document */
    bool loadXML(QXmlStreamReader &root);

    /** @reimp */
    void postLoad();

public:
    /** @reimp */
    bool contains(quint32 functionId);

    /** @reimp */
    QList<quint32> components();

    /*********************************************************************
     * Running
     *********************************************************************/
public:
    /** @reimp */
    void preRun(MasterTimer* timer);

    /** @reimp */
    void setPause(bool enable);

    /** @reimp */
    void write(MasterTimer* timer, QList<Universe*> universes);

    /** @reimp */
    void postRun(MasterTimer* timer, QList<Universe*> universes);

protected slots:
    /** Called whenever one of this function's child functions stops */
    void slotChildStopped(quint32 fid);

signals:
    void timeChanged(quint32);
    void showFinished();

protected:
    ShowRunner *m_runner;
    /** Number of currently running children */
    QSet <quint32> m_runningChildren;

    /*************************************************************************
     * Attributes
     *************************************************************************/
public:
    /** @reimp */
    int adjustAttribute(qreal fraction, int attributeId = 0);
};

/** @} */

#endif
