#include <Arduino.h>
#include <Wire.h>
#include <RGBMatrix.h>

#define I2C_ADDR 0x62

CFrameBuffer fb;
CRGBMatrixI2C matrix;

void ShowColor(byte r, byte g, byte b)
{
    byte i,j;
    for (i = 0;i<8;i++) {
        for(j = 0;j<8;j++) {
          fb.SetPixel(i, j, r, g, b);
        }
    }
    matrix.SetFrame(fb);
}

void setup()
{
    matrix.Init(I2C_ADDR);

    ShowColor(255, 0, 0);
    delay(500);
    ShowColor(0, 255, 0);
    delay(500);
    ShowColor(0, 0, 255);
    delay(500);
    ShowColor(255, 255, 0);
    delay(500);
    ShowColor(0, 255, 255);
    delay(500);
    ShowColor(255, 0, 255);
    delay(500);

    // set pixel directly
    for (byte y = 0; y < 8; y++) {
        for (byte x = 0; x < 8; x++) {
            matrix.SetPixel(x, y, 31 + y * 32, 0, 0);
            matrix.Draw();
            delay(50);
        }
    }
    delay(500);

    // set pixel in local frame buffer and push to module
    fb.Clear();
    for (byte y = 0; y < 8; y++) {
        for (byte x = 0; x < 8; x++) {
            fb.SetPixel(x, y, 0, 31 + y * 32, 0);
            matrix.SetFrame(fb);
            delay(50);
        }
    }
    delay(500);
    matrix.Clear();
}

void loop()
{
    for (byte n = '0'; n <= '9'; n++) {
        matrix.Clear();
        matrix.ShowChar5x7(n, 2, -1, 0, 255, 255);
        matrix.Draw();
        delay(250);
    }
    for (byte n = 0; n <= 99; n++) {
        matrix.Clear();
        matrix.ShowDigit3x5(n / 10, 0, 0, 0, 255, 255);
        matrix.ShowDigit3x5(n % 10, 4, 0, 0, 255, 255);
        matrix.Draw();
        delay(100);
    }
    for (byte n = 0; n <= 99; n++) {
        matrix.Clear();
        matrix.ShowDigit3x7(n / 10, 0, 0, 0, 255, 255);
        matrix.ShowDigit3x7(n % 10, 4, 0, 0, 255, 255);
        matrix.Draw();
        delay(100);
    }
}
