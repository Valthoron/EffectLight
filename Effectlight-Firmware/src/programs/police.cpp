#include <Adafruit_NeoPixel.h>

// External variables
extern Adafruit_NeoPixel g_Pixels;

void program_police(unsigned long step, unsigned long time, unsigned long delta)
{
    unsigned long progress = time % 1500;
    uint8_t blue;
    uint8_t red;

    if (progress < 500)
    {
        blue = progress / 2;
        red = 0;
    }
    else if (progress < 1000)
    {
        blue = (1000 - progress) / 2;
        red = (progress - 500) / 2;
    }
    else if (progress < 1500)
    {
        blue = 0;
        red = (1500 - progress) / 2;
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