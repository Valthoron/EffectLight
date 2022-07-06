#include <Adafruit_NeoPixel.h>
#include <ESP8266WiFi.h>
#include <ESP8266mDNS.h>
#include <WiFiUdp.h>
#include "secret.h"

// External functions
void handle_message(const char* buffer, int length);
void program_tick(unsigned long delta);

#if defined ESP12E
#elif defined ESP01
#endif

// Pins
const int PIN_NEOPIXEL = 2;

// Loop
const unsigned long LOOP_INTERVAL = 20;
const unsigned long LOOP_INTERVAL_US = LOOP_INTERVAL * 1000;
const unsigned long COMM_LOOP_INTERVAL = 10;
unsigned int g_iLoopCount = 0;

// NeoPixel
Adafruit_NeoPixel g_Pixels = Adafruit_NeoPixel(48, PIN_NEOPIXEL);

// WiFi
const char* WIFI_SSID = SECRET_WIFI_SSID;
const char* WIFI_PASSWORD = SECRET_WIFI_PASSWORD;
const byte WIFI_BSSID[6] = SECRET_WIFI_BSSID;

// mDNS
const char* HOST_NAME = "effectlight";

// UDP
const unsigned int UDP_PORT = 8888;
WiFiUDP g_Udp;
char g_UdpReceived[256];

void setup()
{
    // Serial monitor
    //Serial.begin(115200);

    // Initialize pixels
    g_Pixels.begin();

    // Initialize WiFi
    WiFi.begin(WIFI_SSID, WIFI_PASSWORD, 0, WIFI_BSSID);

    while (WiFi.status() != WL_CONNECTED)
    {
        delay(500);
    }

    // Initialize mDNS
    MDNS.begin(HOST_NAME);

    // Initialize UDP socket
    g_Udp.begin(UDP_PORT);
}

void loop()
{
    // Keep track of time
    unsigned long tic = micros();

    // Perform communication every COMM_LOOP_INTERVAL loops
    g_iLoopCount++;

    if (g_iLoopCount >= COMM_LOOP_INTERVAL)
    {
        g_iLoopCount = 0;

        // Update mDNS
        MDNS.update();

        // Process UDP messages
        int packetSize = g_Udp.parsePacket();

        if (packetSize)
        {
            int length = g_Udp.read(g_UdpReceived, 255);

            if (length > 0)
            {
                handle_message(g_UdpReceived, length);
            }
        }
    }

    // Run program, if any
    program_tick(LOOP_INTERVAL);

    // Sleep until next loop
    unsigned long toc = micros();
    delayMicroseconds(LOOP_INTERVAL_US - (toc - tic));
}
