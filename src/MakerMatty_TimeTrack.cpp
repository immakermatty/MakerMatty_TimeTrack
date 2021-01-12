#ifdef ESP32

#include "MakerMatty_TimeTrack.h"

/**
 * @brief Local (object) timeMs_g tracking
 */
TimeTrack::TimeTrack()
    : m_paused(false)
    , m_memory(0)
    , m_source(nullptr)
{
    setMillis(0);
}

/**
 * @brief Local (object) timeMs_g tracking
 * @param currentTime Current micros when creating this obbject
 */
TimeTrack::TimeTrack(const time_ms initial)
    : m_paused(false)
    , m_memory(0)
    , m_source(nullptr)
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

/**
 * @brief Get current local (object) timeMs_g in miliseconds. 1s = 1000ms
 * @return number of miliseconds based on when setTime_g() was called
 */
const time_ms /*IRAM_ATTR*/ TimeTrack::millis() const
{
    if (m_paused) {
        return (time_ms)(m_memory / 1000LL);
    } else {
        return (time_ms)((sourceMicros() - m_memory) / 1000LL);
    }
}

/**
 * @brief Get current local (object) timeMs_g in microseconds. 1s = 1000000ms
 * @return number of microseconds based on when setMillis() was called
 */
const time_us /*IRAM_ATTR*/ TimeTrack::micros() const
{
    if (m_paused) {
        return (time_us)m_memory;
    } else {
        return (time_us)(sourceMicros() - m_memory);
    }
}

/**
* @brief Set the local (object) timeMs_g in ms. 1s = 1000ms
*/
void TimeTrack::setMillis(const time_ms timestamp)
{
    m_memory = m_paused ? ((int64_t)timestamp * 1000LL) : (sourceMicros() - ((int64_t)timestamp * 1000LL));
}

/**
* @brief Set the local (object) timeMs_g in us. 1s = 1000000us
*/
void TimeTrack::setMicros(const time_us timestamp)
{
    m_memory = m_paused ? (int64_t)timestamp : (sourceMicros() - (int64_t)timestamp);
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
        return (time_ms)(m_freezedAt / 1000LL);
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