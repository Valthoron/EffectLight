#include <Adafruit_NeoPixel.h>

// External functions
void run_program(int index);
void stop_program();

// External variables
extern Adafruit_NeoPixel g_Pixels;

enum class MessageHeader : uint8_t
{
    Clear = 0,
    Show,
    UpdateLength,
    Set,
    Fill,
    RunProgram = 100,
    StopProgram
};

void handle_message(const char* buffer, int length)
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
    else if (header == MessageHeader::UpdateLength and length == 3)
    {
        uint16_t stripLength = ((uint16_t)buffer[1] << 8) | ((uint16_t)buffer[2]);
        g_Pixels.updateLength(stripLength);
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
