#pragma once

#include <Adafruit_NeoPixel.h>
#include "program.h"

// External variables
extern Adafruit_NeoPixel g_Pixels;

class Program_Road : public Program
{
private:
    void fill_column(int column, uint32_t color)
    {
        for (int row = 0; row < 6; row++)
        {
            if (row % 2)
                g_Pixels.setPixelColor((row * 8) + column, color);
            else
                g_Pixels.setPixelColor((row * 8) + 8 - column, color);
        }
    }

public:
    const char* get_name() { return "Road"; }

    void tick(unsigned long step, unsigned long time, unsigned long delta)
    {
        int progress = time % 1200;
        int position = progress - 200;

        for (int col = 0; col < 8; col++)
        {
            int distance = abs((col * 100) - position);
            uint8_t brightness = max(200 - distance, 0) * 5 / 4; // 0-200 scaled to 0-250
            uint32_t color = g_Pixels.ColorHSV(9466, 255, brightness); // 52 degree hue, orange-yellow
            fill_column(col, color);
        }

        g_Pixels.show();
    }
};
