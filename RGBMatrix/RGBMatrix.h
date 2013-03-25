#ifndef RGBMATRIX_H
#define RGBMATRIX_H

#define CMD_FRAME 0x1
#define CMD_PIXEL 0x2
#define CMD_CHAR_5X7 0x3
#define CMD_DIGIT_3X5 0x4
#define CMD_DIGIT_3X7 0x5
#define CMD_DRAW 0x6
#define CMD_CLEAR 0x7

class CFrameBuffer
{
public:
    CFrameBuffer() { Clear(); }
    void SetPixel(uint8_t x, uint8_t y, uint8_t r, uint8_t g, uint8_t b)
    {
        if ((x & 1)) {
            x >>= 1;
            framebuffer[0][y][x] = (g >> 4) | (framebuffer[0][y][x] & 0xF0);
            framebuffer[1][y][x] = (r >> 4) | (framebuffer[1][y][x] & 0xF0);
            framebuffer[2][y][x] = (b >> 4) | (framebuffer[2][y][x] & 0xF0);
        } else {
            x >>= 1;
            framebuffer[0][y][x] = (g & 0xF0) | (framebuffer[0][y][x] & 0x0F);
            framebuffer[1][y][x] = (r & 0xF0) | (framebuffer[1][y][x] & 0x0F);
            framebuffer[2][y][x] = (b & 0xF0) | (framebuffer[2][y][x] & 0x0F);
        }
    }
    void ClearPixel(uint8_t x, uint8_t y)
    {
        if ((x & 1)) {
            x >>= 1;
            framebuffer[0][y][x] &= 0xF0;
            framebuffer[1][y][x] &= 0xF0;
            framebuffer[2][y][x] &= 0xF0;
        } else {
            x >>= 1;
            framebuffer[0][y][x] &= 0x0F;
            framebuffer[1][y][x] &= 0x0F;
            framebuffer[2][y][x] &= 0x0F;
        }
    }
    void Clear()
    {
        memset(framebuffer, 0, sizeof(framebuffer));
    }
    uint8_t framebuffer[3][8][4];
};

class CRGBMatrixI2C
{
public:
    void Init(uint8_t addr)
    {
        m_addr = addr;
        Wire.begin();
    }
    void SetFrame(CFrameBuffer& frame)
    {
        uint8_t* pixels = (uint8_t*)frame.framebuffer;
        // send command
        Wire.beginTransmission(m_addr);
        Wire.write((uint8_t)CMD_FRAME);
        Wire.endTransmission();
        // send R plane
        Wire.beginTransmission(m_addr);
        Wire.write(pixels, 32);
        Wire.endTransmission();
        // send G plane
        Wire.beginTransmission(m_addr);
        Wire.write(pixels + 32, 32);
        Wire.endTransmission();
        // send B plane
        Wire.beginTransmission(m_addr);
        Wire.write(pixels + 64, 32);
        Wire.endTransmission();
    }
    void SetPixel(uint8_t x, uint8_t y, uint8_t r, uint8_t g, uint8_t b)
    {
        Wire.beginTransmission(m_addr);
        Wire.write((uint8_t)CMD_PIXEL);
        Wire.write((x & 0xf) | (y << 4));
        Wire.write(r);
        Wire.write(g);
        Wire.write(b);
        Wire.endTransmission();
    }
    void ShowChar5x7(char c, char bias_x, char bias_y, uint8_t r, uint8_t g, uint8_t b)
    {
        Wire.beginTransmission(m_addr);
        Wire.write((uint8_t)CMD_CHAR_5X7);
        Wire.write(c);
        Wire.write(bias_x);
        Wire.write(bias_y);
        Wire.write(r);
        Wire.write(g);
        Wire.write(b);
        Wire.endTransmission();
    }
    void ShowDigit3x5(byte n, char bias_x, char bias_y, uint8_t r, uint8_t g, uint8_t b)
    {
        Wire.beginTransmission(m_addr);
        Wire.write((uint8_t)CMD_DIGIT_3X5);
        Wire.write(n);
        Wire.write(bias_x);
        Wire.write(bias_y);
        Wire.write(r);
        Wire.write(g);
        Wire.write(b);
        Wire.endTransmission();
    }
    void ShowDigit3x7(byte n, char bias_x, char bias_y, uint8_t r, uint8_t g, uint8_t b)
    {
        Wire.beginTransmission(m_addr);
        Wire.write((uint8_t)CMD_DIGIT_3X7);
        Wire.write(n);
        Wire.write(bias_x);
        Wire.write(bias_y);
        Wire.write(r);
        Wire.write(g);
        Wire.write(b);
        Wire.endTransmission();
    }
    void Draw()
    {
        Wire.beginTransmission(m_addr);
        Wire.write((uint8_t)CMD_DRAW);
        Wire.endTransmission();
    }

    void Clear()
    {
        Wire.beginTransmission(m_addr);
        Wire.write((uint8_t)CMD_CLEAR);
        Wire.endTransmission();
    }
private:
    uint8_t m_addr;
};

class CRGBMatrixSerial
{
public:
    void Init(byte addr)
    {
        m_addr = addr;
        Serial.begin(115200);
    }
    void SetFrame(CFrameBuffer& frame)
    {
        uint8_t* pixels = (uint8_t*)frame.framebuffer;
        // send command
        Serial.write(m_addr | CMD_FRAME);
        // send R/G/B plane
        for (byte i = 0; i < 96; i++)
            Serial.write(pixels[i]);
    }
    void SetPixel(uint8_t x, uint8_t y, uint8_t r, uint8_t g, uint8_t b)
    {
        Serial.write(m_addr | CMD_PIXEL);
        Serial.write((x & 0xf) | (y << 4));
        Serial.write(r);
        Serial.write(g);
        Serial.write(b);
    }
    void ShowChar5x7(char c, char bias_x, char bias_y, uint8_t r, uint8_t g, uint8_t b)
    {
        Serial.write(m_addr | CMD_CHAR_5X7);
        Serial.write(c);
        Serial.write(bias_x);
        Serial.write(bias_y);
        Serial.write(r);
        Serial.write(g);
        Serial.write(b);
    }
    void ShowDigit3x5(byte n, char bias_x, char bias_y, uint8_t r, uint8_t g, uint8_t b)
    {
        Serial.write(m_addr | CMD_DIGIT_3X5);
        Serial.write(n);
        Serial.write(bias_x);
        Serial.write(bias_y);
        Serial.write(r);
        Serial.write(g);
        Serial.write(b);
    }
    void ShowDigit3x7(byte n, char bias_x, char bias_y, uint8_t r, uint8_t g, uint8_t b)
    {
        Serial.write(m_addr | CMD_DIGIT_3X7);
        Serial.write(n);
        Serial.write(bias_x);
        Serial.write(bias_y);
        Serial.write(r);
        Serial.write(g);
        Serial.write(b);
    }
    void Draw()
    {
        Serial.write(m_addr | CMD_DRAW);
    }

    void Clear()
    {
        Serial.write(m_addr | CMD_CLEAR);
    }
private:
    uint8_t m_addr;
};

#endif
