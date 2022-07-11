#pragma once

#include <Adafruit_NeoPixel.h>
#include <random>
#include "program.h"

// External variables
extern Adafruit_NeoPixel g_Pixels;

class Program_Television : public Program
{
private:
    std::default_random_engine generator;
    int mode;
    int remainingTime;

    // Constant mode
    uint16_t hue_c;

    // Flickering mode
    uint16_t hue_f;
    uint8_t saturation_f;
    uint8_t brightness_f;
    int remainingSteps_f;
    int deltaBrightness_f;

    // Ramp mode
    uint8_t red_r, green_r, blue_r;
    uint8_t deltaRed_r, deltaGreen_r, deltaBlue_r;

    void init_constant()
    {
        hue_c = (hue_c + std::uniform_int_distribution(-8000, 10000)(generator)) % 65536;
        uint8_t saturation = std::uniform_int_distribution(10, 250)(generator);
        uint8_t brightness = std::uniform_int_distribution(10, 150)(generator);
        uint32_t color = g_Pixels.ColorHSV(hue_c, saturation, brightness);
        g_Pixels.fill(color);
        g_Pixels.show();
    }

    void tick_constant(unsigned long step, unsigned long time, unsigned long delta)
    {
        // nop
    }

    void init_flickering()
    {
        hue_f = std::uniform_int_distribution(0, 65536)(generator);
        saturation_f = std::uniform_int_distribution(0, 250)(generator);
        brightness_f = std::uniform_int_distribution(0, 250)(generator);
        remainingSteps_f = 0;
    }

    void tick_flickering(unsigned long step, unsigned long time, unsigned long delta)
    {
        if (remainingSteps_f <= 0)
        {
            int remaining = std::uniform_int_distribution(50, 250)(generator);
            remainingSteps_f = remaining / delta;

            uint8_t target = std::uniform_int_distribution(0, 250)(generator);
            deltaBrightness_f = (target - brightness_f) / remainingSteps_f;
        }

        brightness_f += deltaBrightness_f;
        uint32_t color = g_Pixels.ColorHSV(hue_f, saturation_f, brightness_f);
        g_Pixels.fill(color);
        g_Pixels.show();

        remainingSteps_f--;
    }

    void init_ramp()
    {

    }

    void tick_ramp(unsigned long step, unsigned long time, unsigned long delta)
    {

    }

public:
    const char* get_name() { return "Television"; }

    void init()
    {
        std::random_device rd;
        generator = std::default_random_engine(rd());

        mode = 0;
        remainingTime = 0;

        hue_c = 0;
    }

    void tick(unsigned long step, unsigned long time, unsigned long delta)
    {
        if (remainingTime <= 0)
        {
            // Pick new mode
            mode = std::uniform_int_distribution(1, 3)(generator);
            remainingTime = std::uniform_int_distribution(250, 2500)(generator) + std::uniform_int_distribution(250, 2500)(generator);

            switch (mode)
            {
            case 1:
                init_flickering();
                break;
            case 2:
                init_ramp();
                break;
            default:
                init_constant();
                break;
            }
        }

        switch (mode)
        {
        case 1:
            tick_flickering(step, time, delta);
            break;
        case 2:
            tick_ramp(step, time, delta);
            break;
        default:
            tick_constant(step, time, delta);
            break;
        }

        remainingTime -= delta;
    }
};
