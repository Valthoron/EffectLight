#include <Adafruit_NeoPixel.h>

// List of programs
void program_blinktest(unsigned long step, unsigned long time, unsigned long delta);
void program_breathetest(unsigned long step, unsigned long time, unsigned long delta);
void program_police(unsigned long step, unsigned long time, unsigned long delta);

// External variables
extern Adafruit_NeoPixel g_Pixels;

bool g_bProgramRunning;
void (*active_program)(unsigned long, unsigned long, unsigned long) = nullptr;

unsigned long g_iStep = 0;
unsigned long g_iTime = 0;

void run_program(int index)
{
    switch (index)
    {
    case 0: active_program = program_blinktest; break;
    case 1: active_program = program_breathetest; break;
    case 2: active_program = program_police; break;
    default: active_program = nullptr; break;
    }

    if (active_program == nullptr)
        return;

    g_iTime = 0;
    g_bProgramRunning = true;
}

void stop_program()
{
    g_bProgramRunning = false;
}

void program_tick(unsigned long delta)
{
    if (active_program == nullptr)
        return;

    if (not g_bProgramRunning)
        return;

    g_iStep++;
    g_iTime += delta;

    active_program(g_iStep, g_iTime, delta);
}
