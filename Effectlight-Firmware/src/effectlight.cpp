#include <Adafruit_NeoPixel.h>
#include <ESP8266WiFi.h>
#include <ESP8266mDNS.h>
#include <WiFiUdp.h>
#include "secret.h"

// External functions
void handle_message(WiFiUDP* pUdp, const char* buffer, int length);
void program_tick(unsigned long delta);

#if defined ESP12E
#define PIN_NEOPIXEL_PLATFORM D8
#elif defined ESP01
#define PIN_NEOPIXEL_PLATFORM 3
#endif

// Pins
const int PIN_NEOPIXEL = PIN_NEOPIXEL_PLATFORM;

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
#ifdef SECRET_WIFI_BSSID
const byte WIFI_BSSID[6] = SECRET_WIFI_BSSID;
#endif

// mDNS
const char* HOST_NAME = "effectlight";

// UDP
const unsigned int UDP_PORT = 8888;
WiFiUDP g_Udp;
char g_UdpReceived[256];

void setup()
{
    delay(500);

    // Serial monitor
    //Serial.begin(115200);

    const uint32_t green = g_Pixels.Color(0, 100, 0);
    const uint32_t yellow = g_Pixels.Color(80, 80, 0);

    // Initialize pixels
    g_Pixels.begin();
    g_Pixels.clear();
    g_Pixels.show();

    delay(500);

    // Initialize WiFi
#ifdef SECRET_WIFI_BSSID
    WiFi.begin(WIFI_SSID, WIFI_PASSWORD, 0, WIFI_BSSID);
#else
    WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
#endif
    g_Pixels.setPixelColor(0, yellow);
    g_Pixels.show();

    while (WiFi.status() != WL_CONNECTED)
    {
        delay(500);
    }

    g_Pixels.setPixelColor(0, green);
    g_Pixels.show();

    // Initialize mDNS
    MDNS.begin(HOST_NAME);
    g_Pixels.setPixelColor(1, green);
    g_Pixels.show();

    // Initialize UDP socket
    g_Udp.begin(UDP_PORT);
    g_Pixels.setPixelColor(2, green);
    g_Pixels.show();

    delay(1000);
    g_Pixels.clear();
    g_Pixels.show();
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
                handle_message(&g_Udp, g_UdpReceived, length);
            }
        }
    }

    // Run program, if any
    program_tick(LOOP_INTERVAL);

    // Sleep until next loop
    unsigned long toc = micros();
    unsigned long delay = max(LOOP_INTERVAL_US - (toc - tic), 1UL);
    delayMicroseconds(delay);
}
