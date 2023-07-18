
#include <Arduino.h>
#include <Wire.h>
#define ADDRESS_FIRST_PCF 0x38
#define ADDRESS_SECOND_PCF 0xBC

uint64_t img = 0x415536141c141408;

const uint64_t VID[] = {0x0000000000000007, 0x0000000000000e00, 0x00000000001c0000, 0x0000000038000000,
                        0x0000007000000000, 0x0000e00000000000, 0x0070000000000000, 0x3800000000000000,
                        0x001c000000000000, 0x00000e0000000000, 0x0000000700000000, 0x000000000e000000,
                        0x00000000001c0000, 0x0000000000003800, 0x0000000000000070, 0x000000000000e000,
                        0x0000000000700000, 0x0000000038000000, 0x0000001c00000000, 0x00000e0000000000,
                        0x0007000000000000, 0x0e00000000000000, 0x001c000000000000, 0x0000380000000000,
                        0x0000007000000000, 0x00000000e0000000};
const int VID_LEN = sizeof(VID) / 8;

void matrix_int()
{
    Wire.beginTransmission(ADDRESS_FIRST_PCF);

    Wire.write(0x00);
    Wire.endTransmission(); // stop transmitting

    Wire.beginTransmission(ADDRESS_SECOND_PCF);

    Wire.write(0x00);
    Wire.endTransmission(); // stop transmitting
}

void matrix_setLED(char col, char raw)
{
    Wire.beginTransmission(ADDRESS_FIRST_PCF);

    Wire.write(0xff & ~(0x01 << raw));
    Wire.endTransmission(); // stop transmitting

    Wire.beginTransmission(ADDRESS_SECOND_PCF);

    Wire.write(0x01 << col);
    Wire.endTransmission(); // stop transmitting
}
void matrix_setROW(char col, char raw)
{
    Wire.beginTransmission(ADDRESS_SECOND_PCF);

    Wire.write(0x01 << col);
    Wire.endTransmission(); // stop transmitting

    Wire.beginTransmission(ADDRESS_FIRST_PCF);

    Wire.write(0xff & ~(raw));
    Wire.endTransmission(); // stop transmitting
}
void matrix_set_frame(uint64_t *raw)
{
    uint8_t *ptr = (uint8_t *)raw;
    for (char i = 0; i < 8; i++)
    {
        if (*ptr == 0x00)
        {
            ptr++;
            continue;
        }
        matrix_setROW(i, (uint8_t)*ptr++);
        delay(100); // debug
    }
}

void matrix_set_vid(uint64_t *frame, uint64_t frames)
{
    for (uint64_t i = 0; i < frames; i++)
    {
        delay(100); // debug

        matrix_set_frame(&VID[i]);
    }
}

void setup()
{
    Wire.begin();
    matrix_int();
}

void loop()
{
    matrix_set_vid(VID, VID_LEN);
}
