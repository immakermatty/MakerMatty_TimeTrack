#ifdef ESP32

#include "MakerMatty_TimeTrack.h"


// const TimeTrack* m_source;
// int64_t m_memory;
// bool m_paused;


/**
 * @brief Local (object) timeMs_g tracking
 */
TimeTrack::TimeTrack()
    : m_source(nullptr)
    , m_timeJumpCb(nullptr)
    , m_memory(0)
    , m_paused(false)
{
    setMillis(0);
}

/**
 * @brief Local (object) timeMs_g tracking
 * @param currentTime Current micros when creating this obbject
 */
TimeTrack::TimeTrack(const time_ms initial)
    : m_source(nullptr)
    , m_timeJumpCb(nullptr)
    , m_memory(0)
    , m_paused(false)
{
    setMillis(initial);
}

// /**
//  * @brief Copy contructor
//  * @param other Other TimeTrack object to copy
//  */
// TimeTrack::TimeTrack(const TimeTrack& other)
//     : m_paused(other.m_paused)
//     , m_memory(other.m_memory)
// {
// }

// /**
//  * @brief Copy assignment
//  * @param other Other TimeTrack object to copy
//  */
// TimeTrack& TimeTrack::operator=(TimeTrack other) noexcept
// {
//     m_paused = other.m_paused;
//     m_memory = other.m_memory;
//     return *this;
// }

void TimeTrack::attach(const TimeTrack& other)
{
    m_source = &other;
}

void TimeTrack::detach()
{
    m_source = nullptr;
}

// const timeline_ms /*IRAM_ATTR*/ TimeTrack::timeline() const
// {
//     if (m_paused) {
//         return timeline_ms((m_memory / 1000LL));
//     } else {
//         return timeline_ms(((sourceMicros() - m_memory) / 1000LL));
//     }
// }

/**
 * @brief Get current local (object) timeMs_g in miliseconds. 1s = 1000ms
 * @return number of miliseconds based on when setTime_g() was called
 */
const time_ms /*IRAM_ATTR*/ TimeTrack::millis() const
{
    if (m_paused) {
        return time_ms((m_memory / 1000LL));
    } else {
        return time_ms(((sourceMicros() - m_memory) / 1000LL));
    }
}

const uint32_t TimeTrack::millis_u32() const
{
    if (m_paused) {
        return uint32_t((m_memory / 1000LL));
    } else {
        return uint32_t(((sourceMicros() - m_memory) / 1000LL));
    }
}

const int32_t TimeTrack::millis_i32() const
{
    if (m_paused) {
        return int32_t((m_memory / 1000LL));
    } else {
        return int32_t(((sourceMicros() - m_memory) / 1000LL));
    }
}

/**
 * @brief Get current local (object) timeMs_g in microseconds. 1s = 1000000ms
 * @return number of microseconds based on when setMillis() was called
 */
const time_us /*IRAM_ATTR*/ TimeTrack::micros() const
{
    if (m_paused) {
        return time_us(m_memory);
    } else {
        return time_us((sourceMicros() - m_memory));
    }
}

/**
 * @brief Set the local (object) timeMs_g in ms. 1s = 1000ms
 */
void TimeTrack::setMillis(const time_ms timestamp, const time_ms transition)
{
    const int64_t memory = m_paused ? (time_us(timestamp * 1000LL)) : (sourceMicros() - (time_us(timestamp * 1000LL)));

    const time_us delta = time_us(m_paused ? memory - m_memory : m_memory - memory);
    m_memory = memory;

    onTimeJump(delta);

    if (m_timeJumpCb) {
        (*m_timeJumpCb)(delta);
    }
}

/**
 * @brief Set the local (object) timeMs_g in ms. 1s = 1000ms
 */
void TimeTrack::setMillis(const uint32_t timestamp, const time_ms transition)
{
    const int64_t memory = m_paused ? (time_us(timestamp * 1000LL)) : (sourceMicros() - (time_us(timestamp * 1000LL)));

    const time_us delta = time_us(m_paused ? memory - m_memory : m_memory - memory);
    m_memory = memory;

    onTimeJump(delta);

    if (m_timeJumpCb) {
        (*m_timeJumpCb)(delta);
    }
}


/**
 * @brief Set the local (object) timeMs_g in us. 1s = 1000000us
 */
void TimeTrack::setMicros(const time_us timestamp, const time_ms transition)
{
    const int64_t memory = m_paused ? time_us(timestamp) : (sourceMicros() - time_us(timestamp));

    const time_us delta = time_us(m_paused ? memory - m_memory : m_memory - memory);
    m_memory = memory;

    onTimeJump(delta);

    if (m_timeJumpCb) {
        (*m_timeJumpCb)(delta);
    }
}

/**
 * @brief Add miliseconds to the clock
 */
void TimeTrack::adjustMillis(const time_ms delta, const time_ms transition)
{
    const time_us delta_us = time_us(delta) * 1000LL;

    m_memory = m_memory + delta_us;

    onTimeJump(delta_us);

    if (m_timeJumpCb) {
        (*m_timeJumpCb)(delta_us);
    }
}

/**
 * @brief  Add microseconds to the clock
 */
void TimeTrack::adjustMicros(const time_us delta, const time_ms transition)
{
    m_memory = m_memory + time_us(delta);

    onTimeJump(delta);

    if (m_timeJumpCb) {
        (*m_timeJumpCb)(delta);
    }
}

/**
 * @brief Pauses the local (object) millis.
 */
void TimeTrack::pause()
{
    if (!m_paused) {
        m_memory = sourceMicros() - m_memory;
        m_paused = true;
    }
}

/**
 * @brief Unpauses the local (object) millis.
 */
void TimeTrack::unpause()
{
    if (m_paused) {
        m_memory = sourceMicros() - m_memory;
        m_paused = false;
    }
}

/**
 * @brief Synchronizes time with source TimeTrack. The time is not linked in any way tho
 */
void TimeTrack::sync(const TimeTrack& source)
{
    this->setMicros(source.micros());
}

// /**
//  * @brief Synchronizes time with source TimeTrack. The time is not linked in any way tho
//  */
// void TimeTrack::sync(const time_ms timestamp)
// {
//     this->setMillis(timestamp);
// }

// /**
//  * @brief Synchronizes time with source TimeTrack. The time is not linked in any way tho
//  */
// void TimeTrack::sync(const time_us timestamp)
// {
//     this->setMicros(timestamp);
// }

void TimeTrack::onTimeJump(TimeJumpCallback cb)
{
    m_timeJumpCb = cb;
}

const time_us TimeTrack::sourceMicros() const
{
    return (m_source != nullptr ? m_source->micros() : esp_timer_get_time());
}

//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////

FreezableTimeTrack::FreezableTimeTrack()
    : TimeTrack()
    , m_freezed(false)
    , m_freezedAt(0) {};

/**
 * @brief Get current local (object) timeMs_g in miliseconds. 1s = 1000ms
 * @return number of miliseconds based on when setTime_g() was called
 */
const time_ms /*IRAM_ATTR*/ FreezableTimeTrack::millis() const
{
    if (m_freezed) {
        return time_ms((m_freezedAt / 1000LL));
    } else {
        return TimeTrack::millis();
    }
}

/**
 * @brief Get current local (object) timeMs_g in microseconds. 1s = 1000000ms
 * @return number of microseconds based on when setMillis() was called
 */
const time_us /*IRAM_ATTR*/ FreezableTimeTrack::micros() const
{
    if (m_freezed) {
        return (time_us)m_freezedAt;
    } else {
        return TimeTrack::micros();
    }
}

/**
 * @brief Freezes the local (object) millis.
 */
void FreezableTimeTrack::freeze()
{
    m_freezed = true;
    m_freezedAt = TimeTrack::micros();
}

/**
 * @brief Freezes the local (object) millis.
 */
void FreezableTimeTrack::unfreeze()
{
    m_freezed = false;
}

#endif