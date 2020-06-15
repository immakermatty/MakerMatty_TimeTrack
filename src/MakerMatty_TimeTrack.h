/** 
 * Author	: @makermatty (maker.matejsuchanek.cz)
 * Date		: 15-6-2020
 */

#ifdef ESP32

#ifndef _MM_TIMETRACK_h
#define _MM_TIMETRACK_h

#include <Arduino.h>

typedef int32_t time_ms;
#define TIME_MS_MAX __INT32_MAX__
#define TIME_MS_MIN __INT32_MIN__

typedef int64_t time_us;
#define TIME_US_MAX __INT64_MAX__
#define TIME_US_MIN __INT64_MIN__

class TimeTrack {
public:
    TimeTrack();
    TimeTrack(time_ms initial);

    time_ms time();
    time_us timeUs();
    void setTime(time_ms current);
    void setTimeUs(time_us current);
    void pause();
    void unpause();

    /**
	 * @brief Is the timer paused_?
	 * @true if it's paused_, false if it's unpaused.
	 */
    inline const bool paused() const { return paused_; }; //getter

private:
    bool paused_;
    int64_t memory_;
};

typedef TimeTrack MakerMatty_TimeTrack;

#endif
#endif