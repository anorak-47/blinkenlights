#ifndef COMPAT_H
#define COMPAT_H

#include "utils.h"
#include <inttypes.h>
#include <memory>
#include "color.h"

#define MATRIX_ROWS 6
#define MATRIX_COLS 17

typedef uint32_t matrix_row_t;

struct IS31FL3733Device
{
    /// Address on I2C bus.
    uint8_t address;
    /*
    /// Configuration register
    uint8_t cr;
    /// Global Current Control value. Iout = (840 / Rext) * (GCC / 256). Rext = 20 kOhm, typically.
    uint8_t gcc;
    // This device is the master
    bool is_master;
    /// LED matrix enabled.
    uint8_t leds[IS31FL3733_LED_ENABLE_SIZE];
    /// LED matrix brightness.
    uint8_t pwm[IS31FL3733_LED_PWM_SIZE];
    /// LED matrix mask.
    uint8_t mask[IS31FL3733_LED_ENABLE_SIZE];
    /// Pointer to I2C write data to register function.
    uint8_t (*pfn_i2c_write_reg)(uint8_t i2c_addr, uint8_t reg_addr, uint8_t *buffer, uint8_t count);
    /// Pointer to I2C read data from register function.
    uint8_t (*pfn_i2c_read_reg)(uint8_t i2c_addr, uint8_t reg_addr, uint8_t *buffer, uint8_t count);
    /// Pointer to I2C write byte to register function.
    uint8_t (*pfn_i2c_write_reg8)(uint8_t i2c_addr, uint8_t reg_addr, uint8_t data);
    /// Pointer to I2C read byte from register function.
    uint8_t (*pfn_i2c_read_reg8)(uint8_t i2c_addr, uint8_t reg_addr, uint8_t *data);
    /// Hardware enable (SDB)
    void (*pfn_hardware_enable)(bool enabled);
    /// Hardware I2C reset (IICRSET)
    void (*pfn_iic_reset)(void);
    */
};

typedef struct IS31FL3733Device IS31FL3733;

struct IS31FL3733_RGB_Device
{
    IS31FL3733 *device;

    union {
        struct
        {
            uint8_t r;
            uint8_t g;
            uint8_t b;
        };
        uint8_t color[3];
    } offsets;
};

typedef struct IS31FL3733_RGB_Device IS31FL3733_RGB;

struct IS31FL3733_RGB_91TKL
{
        IS31FL3733_RGB *upper;
        IS31FL3733_RGB *lower;
};

typedef struct IS31FL3733_RGB_91TKL IS31FL3733_91TKL;

extern IS31FL3733_91TKL issi;

/// Set LED brightness level.
void is31fl3733_hsv_set_pwm(IS31FL3733_RGB *device, uint8_t x, uint8_t y, HSV color);
/// Set LED brightness level.
void is31fl3733_hsv_direct_set_pwm(IS31FL3733_RGB *device, uint8_t x, uint8_t y, HSV color);

/// Get LED brightness level.
RGB is31fl3733_rgb_get_pwm(IS31FL3733_RGB *device, uint8_t x, uint8_t y);

/// Set LED brightness level.
void is31fl3733_rgb_set_pwm(IS31FL3733_RGB *device, uint8_t x, uint8_t y, RGB color);
/// Set LED brightness level.
void is31fl3733_rgb_direct_set_pwm(IS31FL3733_RGB *device, uint8_t x, uint8_t y, RGB color);

/// Update LED matrix with internal buffer values.
void is31fl3733_91tkl_update(IS31FL3733_91TKL *device);
/// Update LED matrix LED enable/disable states with internal buffer values.
void is31fl3733_91tkl_update_led_enable(IS31FL3733_91TKL *device);
/// Update LED matrix LED brightness values with internal buffer values.
void is31fl3733_91tkl_update_led_pwm(IS31FL3733_91TKL *device);

/// Set brightness level for all LEDs.
void is31fl3733_fill(IS31FL3733 *device, uint8_t brightness);

bool matrix_is_on(uint8_t row, uint8_t col);

uint16_t timer_read();
uint32_t timer_read32();

uint16_t timer_elapsed(uint16_t last);
uint32_t timer_elapsed32(uint32_t last);

class Animator;
void initialize_compat_91tkl(std::shared_ptr<Animator> animator);
void system_tick();

#endif // COMPAT_H
