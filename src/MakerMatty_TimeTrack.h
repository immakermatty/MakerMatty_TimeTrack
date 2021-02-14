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
#define PRITIMEMS PRId32

typedef int64_t time_us;
#define TIME_US_MAX __INT64_MAX__
#define TIME_US_MIN __INT64_MIN__
#define PRITIMEUS PRId64

class TimeTrack {
public:
    TimeTrack();
    TimeTrack(const time_ms initial);

    // TimeTrack(const TimeTrack& other); // copy contructor
    // TimeTrack& TimeTrack::operator=(TimeTrack other) noexcept;

    void attach(const TimeTrack& source);
    void detach();

    const time_ms millis() const;
    const time_us micros() const;
    void setMillis(const time_ms timestamp);
    void setMicros(const time_us timestamp);
    void pause();
    void unpause();

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

    int64_t m_memory;
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
    int64_t m_freezedAt;
};

typedef TimeTrack MakerMatty_TimeTrack;

#endif
#endif