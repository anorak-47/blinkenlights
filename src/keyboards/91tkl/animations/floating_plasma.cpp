
#include "../animations/floating_plasma.h"

#include "utils.h"

#include "../animations/animation_utils.h"
#include "../animations/plasma_color_lut.h"
#include "../animations/sinus_lut.h"
#ifdef __AVR__
#include "../key_led_map.h"
#include "config.h"
#else
#include "../animations/compat.h"
#include "../animations/key_led_map.h"
#endif
#include <stdlib.h>

#define DEBUG_ANIMATION

#ifdef DEBUG_ANIMATION
#include "../animations/debug.h"
#else
#include "../animations/nodebug.h"
#endif

static animation_options plasma_option = animation_option_variant_1;
static uint16_t plasmacounter = 0;

void floating_plasma_animation_loop(void)
{
    uint16_t color;

    RGB rgb;
    rgb.r = animation.rgb.r;
    rgb.g = animation.rgb.g;
    rgb.b = animation.rgb.b;

    for (uint8_t y = 0; y < MATRIX_ROWS; ++y)
    {
        for (uint8_t x = 0; x < MATRIX_COLS; ++x)
        {
            uint8_t temp1 = (x<<4) + plasmacounter;
            uint8_t temp2 = (y<<5) + plasmacounter;
            uint8_t temp3 = ((x<<4) + (y<<4)) + (plasmacounter>>1);
            uint8_t temp4 = (((x*x)<<3) + ((y*y)<<3)) / (x+y+1);

            if (plasma_option | animation_option_variant_1)
            {
                color =
                    (((
                            sin_lut[temp1]
                          + sin_lut[temp2]
                          + sin_lut[temp3]
                          + sin_lut[temp4]
                          ) >> 4) + plasmacounter) % 256*3;
            }
            else
            {
                color =
                        (((
                                sin_lut[temp1]
                              + sin_lut[temp2]
                              + sin_lut[temp3]
                              + sin_lut[temp4]
                              + (plasmacounter << 2)
                        ) >> 4) ) % (256*3);
            }

            rgb.r = PlasmaColorSpace[color*3];
            rgb.g = PlasmaColorSpace[color*3+1];
            rgb.b = PlasmaColorSpace[color*3+2];

            draw_keymatrix_rgb_pixel(&issi, y, x, rgb);
        }
    }

    is31fl3733_91tkl_update_led_pwm(&issi);
    plasmacounter++;
}

/*
void floating_plasma_typematrix_row(uint8_t row_number, matrix_row_t row)
{
}
*/

void floating_plasma_animation_start(void)
{
    animation_prepare(true);
    plasmacounter = 0;
}

void floating_plasma_animation_stop(void)
{
    animation_postpare();
}

void set_animation_floating_plasma()
{
    dprintf("floating plasma\n");

    animation.delay_in_ms = FPS_TO_DELAY(25);
    animation.duration_in_ms = 0;

    animation.animationStart = &floating_plasma_animation_start;
    animation.animationStop = &floating_plasma_animation_stop;
    animation.animationLoop = &floating_plasma_animation_loop;
    //animation.animation_typematrix_row = &floating_plasma_typematrix_row;
    animation.animation_typematrix_row = 0;
}

uint16_t get_animation_options_floating_plasma()
{
    return (animation_option_variant_1 | animation_option_variant_2);
}

void set_animation_option_floating_plasma(animation_options option)
{
    plasma_option = option;
}

animation_options get_animation_option_floating_plasma()
{
    return plasma_option;
}
