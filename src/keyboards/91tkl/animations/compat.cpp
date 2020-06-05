#include "compat.h"

#include "animator.h"
#include "keyboard_properties.h"
#include <QtCore/QDebug>

IS31FL3733_91TKL issi;
IS31FL3733_RGB issi_rgb_upper;
IS31FL3733_RGB issi_rgb_lower;

uint32_t timer_count = 0;

static std::shared_ptr<Animator> g_animator;

void initialize_compat_91tkl(std::shared_ptr<Animator> animator)
{
    g_animator = animator;

    issi.lower = &issi_rgb_lower;
    issi.upper = &issi_rgb_upper;
}

uint16_t timer_read()
{
    return timer_count & 0xFFFF;
}

uint32_t timer_read32()
{
    return timer_count;
}

#define TIMER_DIFF(a, b, max)   ((a) >= (b) ?  (a) - (b) : (max) - (b) + (a))
#define TIMER_DIFF_8(a, b)      TIMER_DIFF(a, b, UINT8_MAX)
#define TIMER_DIFF_16(a, b)     TIMER_DIFF(a, b, UINT16_MAX)
#define TIMER_DIFF_32(a, b)     TIMER_DIFF(a, b, UINT32_MAX)
#define TIMER_DIFF_RAW(a, b)    TIMER_DIFF_8(a, b)

uint16_t timer_elapsed(uint16_t last)
{
    uint32_t t;
    t = timer_count;

    return TIMER_DIFF_16((t & 0xFFFF), last);
}

uint32_t timer_elapsed32(uint32_t last)
{
    uint32_t t;
    t = timer_count;

    return TIMER_DIFF_32(t, last);
}

void system_tick()
{
    timer_count++;
}

static int scaleUpHue(int hue)
{
    return qRound((356.0 / 256.0) * hue);
}

static int scaleDownHue(int hue)
{
    return qRound((256.0 / 360.0) * hue);
}

void is31fl3733_hsv_set_pwm(IS31FL3733_RGB *device, uint8_t x, uint8_t y, HSV color)
{
    int hue = scaleUpHue(color.h);
    g_animator->setColor(y, x, QColor::fromHsv(hue, color.s, color.v));
}

void is31fl3733_hsv_direct_set_pwm(IS31FL3733_RGB *device, uint8_t x, uint8_t y, HSV color)
{
    int hue = scaleUpHue(color.h);
    g_animator->setColor(y, x, QColor::fromHsv(hue, color.s, color.v));
    g_animator->update();
}

RGB is31fl3733_rgb_get_pwm(IS31FL3733_RGB *device, uint8_t x, uint8_t y)
{
    RGB rgb;
    QColor color = g_animator->getColor(y, x);
    rgb.r = color.red();
    rgb.g = color.green();
    rgb.b = color.blue();
    return rgb;
}

void is31fl3733_rgb_set_pwm(IS31FL3733_RGB *device, uint8_t x, uint8_t y, RGB color)
{
    QColor qcolor(color.r, color.g, color.b);
    g_animator->setColor(y, x, qcolor);
}

void is31fl3733_rgb_direct_set_pwm(IS31FL3733_RGB *device, uint8_t x, uint8_t y, RGB color)
{
    QColor qcolor(color.r, color.g, color.b);
    g_animator->setColor(y, x, qcolor);
    g_animator->update();
}

void is31fl3733_91tkl_update(IS31FL3733_91TKL *device)
{
    g_animator->update();
}

void is31fl3733_91tkl_update_led_enable(IS31FL3733_91TKL *device)
{
    g_animator->update();
}

void is31fl3733_91tkl_update_led_pwm(IS31FL3733_91TKL *device)
{
    //qDebug() << __PRETTY_FUNCTION__;
    g_animator->update();
}

void is31fl3733_fill(IS31FL3733 *device, uint8_t brightness)
{
    QColor color(brightness, brightness, brightness);
    for (uint8_t key_row = 0; key_row < MATRIX_ROWS; ++key_row)
    {
        for (uint8_t key_col = 0; key_col < MATRIX_COLS; ++key_col)
        {
            g_animator->setColor(key_row, key_col, color);
        }
    }
}

bool matrix_is_on(uint8_t row, uint8_t col)
{
    return g_animator->isKeyPressed(row, col);
}
