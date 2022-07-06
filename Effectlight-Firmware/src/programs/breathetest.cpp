#include <Adafruit_NeoPixel.h>

// External variables
extern Adafruit_NeoPixel g_Pixels;

const uint32_t color = g_Pixels.Color(0, 255, 0);

void program_breathetest(unsigned long step, unsigned long time, unsigned long delta)
{
    unsigned long progress = time % 1000;
    uint32_t brightness;

    if (progress < 500)
    {
        brightness = progress / 2;
    }
    else
    {
        brightness = (1000 - progress) / 2;
    }

    g_Pixels.fill(color);
    g_Pixels.setBrightness(brightness);
    g_Pixels.show();
}
