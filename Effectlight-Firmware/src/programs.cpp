#include <Adafruit_NeoPixel.h>
#include "program.h"
#include "programs/blinktest.hpp"
#include "programs/breathetest.hpp"
#include "programs/police.hpp"
#include "programs/road.hpp"

// List of programs
Program* g_Programs[] = {
    new Program_BlinkTest(),
    new Program_BreatheTest(),
    new Program_Police(),
    new Program_Road()
};

int g_ProgramCount = sizeof(g_Programs) / sizeof(Program);

// External variables
extern Adafruit_NeoPixel g_Pixels;

bool g_bProgramRunning;
Program* g_pActiveProgram = nullptr;

unsigned long g_iStep = 0;
unsigned long g_iTime = 0;

void run_program(int index)
{
    if ((index < 0) or (index >= g_ProgramCount))
        return;

    g_pActiveProgram = g_Programs[index];
    g_pActiveProgram->init();
    g_iTime = 0;
    g_bProgramRunning = true;
}

void stop_program()
{
    g_bProgramRunning = false;
}

void program_tick(unsigned long delta)
{
    if (g_pActiveProgram == nullptr)
        return;

    if (not g_bProgramRunning)
        return;

    g_iStep++;
    g_iTime += delta;

    g_pActiveProgram->tick(g_iStep, g_iTime, delta);
}
