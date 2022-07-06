#include <Adafruit_NeoPixel.h>

// External variables
extern Adafruit_NeoPixel g_Pixels;

const uint32_t color = g_Pixels.Color(0, 255, 0);

void program_blinktest(unsigned long step, unsigned long time, unsigned long delta)
{
    if ((time % 1000) < 500)
        g_Pixels.fill(color);
    else
        g_Pixels.clear();

    g_Pixels.show();
}
