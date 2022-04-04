#include <Keypad.h>
#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include "KeyConfig.h"

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 32
#define OLED_RESET 4

#define SERIAL_CTR 12
#define TRANSMIT_MODE HIGH
#define RECEIVER_MODE LOW

const int KEYPAD_ROW = 4;
const int KEYPAD_COLUMN = 6;
const char KEYS[KEYPAD_ROW][KEYPAD_COLUMN] = {
    {(char)65, (char)66, (char)67, (char)68, (char)69, (char)70},
    {(char)71, (char)72, (char)73, (char)74, (char)75, (char)76},
    {(char)77, (char)78, (char)79, (char)80, (char)81, (char)82},
    {(char)83, (char)84, (char)85, (char)86, (char)87, (char)88}};
const char baseIndex = 'A';
   
byte ROW_PINS[KEYPAD_ROW] = {2, 3, 4, 5};
byte COLUMN_PINS[KEYPAD_COLUMN] = {6, 7, 8, 9, 10, 11};

Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);
Keypad keypad(makeKeymap(KEYS), ROW_PINS, COLUMN_PINS, KEYPAD_ROW, KEYPAD_COLUMN);

// Cool down time when no key is pressed
const long SCREENSAVER_TIME = 1500;
long lastPressedTime = 0;

void setup()
{
    Serial.begin(9600);

    // Set the TTL-to-485 interface to Transmitter mode
    pinMode(SERIAL_CTR, OUTPUT);
    digitalWrite(SERIAL_CTR, TRANSMIT_MODE);

    if (!display.begin(SSD1306_SWITCHCAPVCC, 0x3C))
    {
        for (;;)
            ;
    }
    display.clearDisplay();
    display.display();

    display.setTextSize(1);
    display.setTextColor(SSD1306_WHITE);
    display.setCursor(30, 0);
    display.print("INITIALIZING");

    display.display();
    delay(2000);
}

void loop()
{
    char key = keypad.getKey();
    if (key != NO_KEY)
    {
        String command = getCommandFromKey(key);
        printKeyInScreen(command);
        Serial.write(0x02);
        Serial.print(command);
        lastPressedTime = millis();
    }
    else
    {
        if (millis() - lastPressedTime > SCREENSAVER_TIME)
        {
            clearDisplay();
        }
    }
}

String getCommandFromKey(char key)
{
    int index = (int) key - (int) baseIndex;
    int row = (int)index / 6;
    int column = (int)index % 6;
    return COMMAND_LIST[row][column];
}

void clearDisplay()
{
    display.clearDisplay();
    display.display();
}

void printKeyInScreen(String key)
{
    clearDisplay();
    display.setTextSize(4);
    display.setTextColor(SSD1306_WHITE);
    display.setCursor(50, 0);
    display.print(key);
    display.display();
}
