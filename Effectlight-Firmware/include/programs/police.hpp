#pragma once

#include <Adafruit_NeoPixel.h>
#include "program.h"

// External variables
extern Adafruit_NeoPixel g_Pixels;

class Program_Police : public Program
{
    const char* get_name() { return "Police car"; }

    void tick(unsigned long step, unsigned long time, unsigned long delta)
    {
        int progress = time % 750;
        uint8_t blue = 0;
        uint8_t red = 0;

        if (progress < 250)
        {
            blue = progress;
            red = 0;
        }
        else if (progress < 500)
        {
            blue = 500 - progress;
            red = progress - 250;
        }
        else if (progress < 750)
        {
            blue = 0;
            red = 750 - progress;
        }

        uint32_t leftColor = g_Pixels.Color(0, 0, blue);
        uint32_t rightColor = g_Pixels.Color(red, 0, 0);

        for (int row = 0; row < 6; row++)
        {
            if (row % 2)
            {
                g_Pixels.fill(leftColor, (row * 8), 4);
                g_Pixels.fill(rightColor, (row * 8) + 4, 4);
            }
            else
            {
                g_Pixels.fill(rightColor, (row * 8), 4);
                g_Pixels.fill(leftColor, (row * 8) + 4, 4);
            }
        }

        g_Pixels.show();
    }
};
