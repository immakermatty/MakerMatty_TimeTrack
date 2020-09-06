#ifdef ESP32

#include "MakerMatty_TimeTrack.h"

/**
 * @brief Local (object) timeMs_g tracking
 */
TimeTrack::TimeTrack()
    : paused_(false)
{
    setTime(0);
}

/**
 * @brief Local (object) timeMs_g tracking
 * @param currentTime Current timeUs when creating this obbject
 */
TimeTrack::TimeTrack(const time_ms initial)
    : paused_(false)
{
    setTime(initial);
}

/**
 * @brief Get current local (object) timeMs_g in miliseconds. 1s = 1000ms
 * @return number of miliseconds based on when setTime_g() was called
 */
const time_ms /*IRAM_ATTR*/ TimeTrack::time() const
{
    return paused_ ? (time_ms)(memory_ / 1000LL) : (time_ms)((esp_timer_get_time() - memory_) / 1000LL);
}

/**
 * @brief Get current local (object) timeMs_g in microseconds. 1s = 1000000ms
 * @return number of microseconds based on when setTime() was called
 */
const time_us /*IRAM_ATTR*/ TimeTrack::timeUs() const
{
    return paused_ ? (time_us)memory_ : (time_us)(esp_timer_get_time() - memory_);
}

/**
 * @brief Pauses the local (object) time.
 */
void TimeTrack::pause()
{
    if (!paused_) {
        paused_ = true;
        memory_ = esp_timer_get_time() - memory_;
    }
}

/**
 * @brief Unpauses the local (object) time.
 */
void TimeTrack::unpause()
{
    if (paused_) {
        paused_ = false;
        memory_ = esp_timer_get_time() - memory_;
    }
}

/**
* @brief Set the local (object) timeMs_g in ms. 1s = 1000ms
*/
void TimeTrack::setTime(const time_ms current)
{
    memory_ = paused_ ? ((int64_t)current * 1000LL) : (esp_timer_get_time() - ((int64_t)current * 1000LL));
}

/**
* @brief Set the local (object) timeMs_g in us. 1s = 1000000us
*/
void TimeTrack::setTimeUs(const time_us current)
{
    memory_ = paused_ ? (int64_t)current : (esp_timer_get_time() - (int64_t)current);
}

#endif