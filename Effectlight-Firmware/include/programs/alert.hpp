#pragma once

#include <Adafruit_NeoPixel.h>
#include "program.h"

// External variables
extern Adafruit_NeoPixel g_Pixels;

class Program_Alert : public Program
{
    const char* get_name() { return "Alert"; }

    void tick(unsigned long step, unsigned long time, unsigned long delta)
    {
        int progress = time % 1000;
        uint8_t brightness = 0;

        if (progress < 250)
        {
            brightness = progress;
        }
        else if (progress < 500)
        {
            brightness = 500 - progress;
        }
        else if (progress < 750)
        {
            brightness = 0;
        }

        uint32_t color = g_Pixels.Color(100, 75, 0);

        g_Pixels.fill(color);
        g_Pixels.setBrightness(brightness);
        g_Pixels.show();
    }
};
