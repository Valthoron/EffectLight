#pragma once

#include <Adafruit_NeoPixel.h>
#include "program.h"

// External variables
extern Adafruit_NeoPixel g_Pixels;

class Program_Road : public Program
{
    const char* get_name() { return "Road"; }

    void tick(unsigned long step, unsigned long time, unsigned long delta)
    {
    }
};
