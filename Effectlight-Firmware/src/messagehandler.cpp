#include <Adafruit_NeoPixel.h>
#include <WiFiUdp.h>
#include "program.h"

// External functions
void run_program(int index);
void stop_program();

// External variables
extern Adafruit_NeoPixel g_Pixels;
extern Program* g_Programs[];
extern int g_ProgramCount;

const size_t MAX_NAME_LENGTH = 16;

enum class MessageHeader : uint8_t
{
    Clear = 0,
    Show,
    Set,
    Fill,
    ListPrograms = 100,
    ProgramEntry,
    RunProgram,
    StopProgram
};

void handle_message(WiFiUDP* pUdp, const char* buffer, int length)
{
    if (length < 1)
        return;

    MessageHeader header = (MessageHeader)buffer[0];

    if (header == MessageHeader::Clear)
    {
        g_Pixels.clear();
    }
    else if (header == MessageHeader::Show)
    {
        g_Pixels.show();
    }
    else if (header == MessageHeader::Set and length == 6)
    {
        uint16_t pixel = ((uint16_t)buffer[1] << 8) | ((uint16_t)buffer[2]);
        uint8_t r = buffer[3];
        uint8_t g = buffer[4];
        uint8_t b = buffer[5];
        g_Pixels.setPixelColor(pixel, r, g, b);
    }
    else if (header == MessageHeader::Fill and length == 8)
    {
        uint16_t start = ((uint16_t)buffer[1] << 8) | ((uint16_t)buffer[2]);
        uint16_t count = ((uint16_t)buffer[3] << 8) | ((uint16_t)buffer[4]);
        uint8_t r = buffer[5];
        uint8_t g = buffer[6];
        uint8_t b = buffer[7];
        auto color = g_Pixels.Color(r, g, b);

        g_Pixels.fill(color, start, count);
    }
    else if (header == MessageHeader::ListPrograms)
    {
        char replyBuffer[1 + MAX_NAME_LENGTH];
        replyBuffer[0] = (char)MessageHeader::ProgramEntry;

        for (int i = 0; i < g_ProgramCount; i++)
        {
            const char* name = g_Programs[i]->get_name();

            memset(&replyBuffer[1], 0, MAX_NAME_LENGTH);
            strncpy(&replyBuffer[1], name, min(MAX_NAME_LENGTH, strlen(name)));

            pUdp->beginPacket(pUdp->remoteIP(), pUdp->remotePort());
            pUdp->write(replyBuffer, sizeof(replyBuffer));
            pUdp->endPacket();
        }
    }
    else if (header == MessageHeader::RunProgram and length == 2)
    {
        int programIndex = buffer[1];
        run_program(programIndex);
    }
    else if (header == MessageHeader::StopProgram)
    {
        stop_program();
    }
}
