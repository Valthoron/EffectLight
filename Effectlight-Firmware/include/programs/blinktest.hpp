#pragma once

#include <Adafruit_NeoPixel.h>
#include "program.h"

// External variables
extern Adafruit_NeoPixel g_Pixels;

class Program_BlinkTest : public Program
{
    const uint32_t color = g_Pixels.Color(0, 255, 0);

    const char* get_name() { return "Blink test"; }

    void tick(unsigned long step, unsigned long time, unsigned long delta)
    {
        if ((time % 1000) < 500)
            g_Pixels.fill(color);
        else
            g_Pixels.clear();

        g_Pixels.show();
    }
};
