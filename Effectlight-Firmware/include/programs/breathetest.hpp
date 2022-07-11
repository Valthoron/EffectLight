#pragma once

#include <Adafruit_NeoPixel.h>
#include "program.h"

// External variables
extern Adafruit_NeoPixel g_Pixels;

class Program_BreatheTest : public Program
{
    const uint32_t color = g_Pixels.Color(0, 255, 0);

    const char* get_name() { return "Breathe test"; }

    void tick(unsigned long step, unsigned long time, unsigned long delta)
    {
        int progress = time % 1000;
        uint8_t brightness;

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
};
