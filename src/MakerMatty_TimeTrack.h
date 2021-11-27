/** 
 * Author	: @makermatty (maker.matejsuchanek.cz)
 * Date		: 15-6-2020
 */

#ifdef ESP32

#ifndef _MM_TIMETRACK_h
#define _MM_TIMETRACK_h

#include <Arduino.h>

typedef int32_t time_ms;
#define TIME_MS_MAX INT32_MAX
#define TIME_MS_MIN INT32_MIN
#define PRITIMEMS PRId32

typedef int64_t time_us;
#define TIME_US_MAX INT64_MAX
#define TIME_US_MIN INT64_MIN
#define PRITIMEUS PRId64

#define TIMELINE_MS_MAX INT32_MAX
#define TIMELINE_MS_MIN 0
#define PRITIMELINEMS PRId32

struct timeline_ms {

    timeline_ms()
        : m_timestamp() {};
    timeline_ms(const time_ms timestamp)
        : m_timestamp(timestamp & 0x7fffffff) {};
    timeline_ms(const timeline_ms& other)
        : m_timestamp(other.m_timestamp) {};

    inline timeline_ms& operator=(const timeline_ms& other)
    {
        m_timestamp = other.m_timestamp;
        return *this;
    }

    inline timeline_ms& operator=(const time_ms timestamp)
    {
        m_timestamp = timestamp & 0x7fffffff;
        return *this;
    }

    inline operator time_ms() const
    {
        return m_timestamp;
    }

private:
    time_ms m_timestamp;
};

class TimeTrack {
public:
    TimeTrack();
    TimeTrack(const time_ms initial);

    // TimeTrack(const TimeTrack& other); // copy contructor
    // TimeTrack& TimeTrack::operator=(TimeTrack other) noexcept;

    void attach(const TimeTrack& source);
    void detach();

    const timeline_ms timeline() const;
    const time_ms millis() const;
    const time_us micros() const;
    void setMillis(const time_ms timestamp);
    void setMicros(const time_us timestamp);
    void adjustMillis(const time_ms delta);
    void adjustMicros(const time_us delta);
    void pause();
    void unpause();

    typedef void (*TimeJumpCallback)(const time_us delta);
    void onTimeJump(TimeJumpCallback cb);

    void sync(const TimeTrack& source);
    void sync(const time_ms timestamp);
    void sync(const time_us timestamp);

    /**
	 * @brief Is the timer paused?
	 * @returns true if it's paused, false if it's not paused.
	 */
    inline const bool paused() const { return m_paused; }; //getter

private:
    inline const time_us sourceMicros() const;

    const TimeTrack* m_source;
    const TimeJumpCallback m_timeJumpCb;

    time_us m_memory;
    bool m_paused;
};

class FreezableTimeTrack : public TimeTrack {

    FreezableTimeTrack();

    const time_ms millis() const;
    const time_us micros() const;
    void freeze();
    void unfreeze();

    /**
	 * @brief Is the timer freezed?
	 * @returns true if it's freezed, false if it's not freezed.
	 */
    inline const bool freezed() const { return m_freezed; }; //getter

private:
    bool m_freezed;
    time_us m_freezedAt;
};

typedef TimeTrack MakerMatty_TimeTrack;

#endif
#endif