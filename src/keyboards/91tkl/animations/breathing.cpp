
#include "../animations/breathing.h"

#include "../animations/animation_utils.h"

#ifdef DEBUG_ANIMATION
#include "../animations/debug.h"
#else
#include "../animations/nodebug.h"
#endif

uint8_t animation_frame;

void set_animation_breathing()
{
	dprintf("breathing\r\n");

    animation.delay_in_ms = 50;
    animation.duration_in_ms = 0;

    animation.animationStart = &breathing_animation_start;
    animation.animationStop = &breathing_animation_stop;
    animation.animationLoop = 0;
    animation.animation_typematrix_row = 0;
}

void breathing_animation_start()
{
    animation_frame = 1;

    animation_prepare(true);

    /*TODO: implement
    issi.setAutoFramePlayConfig(0, 1, 40);
    issi.setAutoFramePlayMode(0);
    issi.setBreathConfig(7, 7, 2);
    issi.setBreathMode(true);
    */

    /*
    issi.enableLed(0, 1, animation_frame);
    issi.setLedBrightness(14, 255, animation_frame);
    issi.setAutoFramePlayConfig(0,1,40);
    issi.setAutoFramePlayMode(animation_frame);
    issi.setBreathConfig(7, 7, 2);
    issi.setBreathMode(1);
    */
}

void breathing_animation_stop()
{
	/*TODO: implement
    issi.setBreathMode(false);
    issi.setPictureMode();
    */

    animation_postpare();
}
