/* Copyright (c) 2016 Kazuki Yamamoto <k.yamamoto.08136891@gmail.com>
Permission is hereby granted, free of charge, to any person obtaining
a copy of this software and associated documentation files
(the "Software"), to deal in the Software without restriction,
including without limitation the rights to use, copy, modify, merge,
publish, distribute, sublicense, and/or sell copies of the Software,
and to permit persons to whom the Software is furnished to do so,
subject to the following conditions:
The above copyright notice and this permission notice shall be
included in all copies or substantial portions of the Software.
THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY
CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT,
TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE
SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
*/
/** this is for SHARP LCD LS027B4DH01
* by Kazuki Yamamoto, or _K4ZUKI_
*/

#ifndef __AKISPILCD_H__  // NOLINT
#define __AKISPILCD_H__  // NOLINT

#include "MemoryLcd.h"
#include "Ser23K256.h"
#include "mbed.h"  //NOLINT

/** \class AkiSpiLcd
 * \brief mbed library for SHARP LCD LS027B4DH01
 *
 * Example:
 * @code
 * #include "mbed.h"
 * #include "AkiSpiLcd.h"
 *
 * AkiSpiLcd LCD(MOSI_, MISO_, SCK_, D2, D5);
 * extern const uint8_t hogepic[];
 * int main()
 * {
 *
 *   wait_ms(1);
 *   LCD.cls();
 *   LCD.directUpdateSingle(10,(uint8_t*)(hogepic+2000));
 *   LCD.directUpdateMulti(100,(240-100),(uint8_t*)(hogepic));
 *
 *   while(1) {
 *       for(int i=0; i<240; i++) {
 *           LCD.directUpdateMulti(i,(240-i),(uint8_t*)(hogepic));
 *           LCD.directUpdateMulti(0,(i),(uint8_t*)(hogepic+50*(240-i)));
 *       }
 *   }
 * }
 * @endcode
 */

// #define SCREEN0 0
// #define SCREEN1 1
// #define LINE_LENGTH 50
// #define RAMLINE_LENGTH 52

const uint8_t lcd_line[256] = {
    0x00, 0x80, 0x40, 0xC0, 0x20, 0xA0, 0x60, 0xE0, 0x10, 0x90, 0x50, 0xD0,
    0x30, 0xB0, 0x70, 0xF0, 0x08, 0x88, 0x48, 0xC8, 0x28, 0xA8, 0x68, 0xE8,
    0x18, 0x98, 0x58, 0xD8, 0x38, 0xB8, 0x78, 0xF8, 0x04, 0x84, 0x44, 0xC4,
    0x24, 0xA4, 0x64, 0xE4, 0x14, 0x94, 0x54, 0xD4, 0x34, 0xB4, 0x74, 0xF4,
    0x0C, 0x8C, 0x4C, 0xCC, 0x2C, 0xAC, 0x6C, 0xEC, 0x1C, 0x9C, 0x5C, 0xDC,
    0x3C, 0xBC, 0x7C, 0xFC, 0x02, 0x82, 0x42, 0xC2, 0x22, 0xA2, 0x62, 0xE2,
    0x12, 0x92, 0x52, 0xD2, 0x32, 0xB2, 0x72, 0xF2, 0x0A, 0x8A, 0x4A, 0xCA,
    0x2A, 0xAA, 0x6A, 0xEA, 0x1A, 0x9A, 0x5A, 0xDA, 0x3A, 0xBA, 0x7A, 0xFA,
    0x06, 0x86, 0x46, 0xC6, 0x26, 0xA6, 0x66, 0xE6, 0x16, 0x96, 0x56, 0xD6,
    0x36, 0xB6, 0x76, 0xF6, 0x0E, 0x8E, 0x4E, 0xCE, 0x2E, 0xAE, 0x6E, 0xEE,
    0x1E, 0x9E, 0x5E, 0xDE, 0x3E, 0xBE, 0x7E, 0xFE, 0x01, 0x81, 0x41, 0xC1,
    0x21, 0xA1, 0x61, 0xE1, 0x11, 0x91, 0x51, 0xD1, 0x31, 0xB1, 0x71, 0xF1,
    0x09, 0x89, 0x49, 0xC9, 0x29, 0xA9, 0x69, 0xE9, 0x19, 0x99, 0x59, 0xD9,
    0x39, 0xB9, 0x79, 0xF9, 0x05, 0x85, 0x45, 0xC5, 0x25, 0xA5, 0x65, 0xE5,
    0x15, 0x95, 0x55, 0xD5, 0x35, 0xB5, 0x75, 0xF5, 0x0D, 0x8D, 0x4D, 0xCD,
    0x2D, 0xAD, 0x6D, 0xED, 0x1D, 0x9D, 0x5D, 0xDD, 0x3D, 0xBD, 0x7D, 0xFD,
    0x03, 0x83, 0x43, 0xC3, 0x23, 0xA3, 0x63, 0xE3, 0x13, 0x93, 0x53, 0xD3,
    0x33, 0xB3, 0x73, 0xF3, 0x0B, 0x8B, 0x4B, 0xCB, 0x2B, 0xAB, 0x6B, 0xEB,
    0x1B, 0x9B, 0x5B, 0xDB, 0x3B, 0xBB, 0x7B, 0xFB, 0x07, 0x87, 0x47, 0xC7,
    0x27, 0xA7, 0x67, 0xE7, 0x17, 0x97, 0x57, 0xD7, 0x37, 0xB7, 0x77, 0xF7,
    0x0F,
};

class AkiLCD_MODE : public LCD_MODE {
 public:
  static const uint8_t COM_INVERT = 0x00;    // 0-X-0-XXXXX
  static const uint8_t CLEAR_SCREEN = 0x20;  // 0-X-1-XXXXX
  static const uint8_t UPDATE = 0x80;        // 1-X-0-XXXXX

  static const uint8_t SCREEN0 = 0;
  static const uint8_t SCREEN1 = 1;
  static const uint8_t SCREEN0_BASE = 0x0000;
  static const uint8_t SCREEN1_BASE = 0x4000;

  static const uint8_t LINE_LENGTH = 50;
  static const uint8_t RAMLINE_LENGTH = 52;

  static const uint8_t LCD_HEIGHT = 240;
};

class AkiSpiLcd : public MemoryLCD {
 public:
  /** \enum BASE_ADDR
      \brief base address list for 23K256
      @param SCREEN0_BASE = 0x0000,
      @param SCREEN1_BASE = 0x3000,
      @param RAMLINE_BASE = 0x6000,
  */
  // enum BASE_ADDR {
  //   SCREEN0_BASE = 0x0000,
  //   SCREEN1_BASE = 0x4000,
  // };

  // enum LCD_MODE {
  //   COM_INVERT = 0x00,     // 0-X-0-0-0-0-XX
  //   UPDATE_4COLOR = 0x90,  // 1-X-0-1-0-0-XX
  //   UPDATE_MONO = 0x88,    // 1-X-0-0-1-0-XX
  //   UPDATE_3COLOR = 0x80,  // 1-X-0-0-0-0-XX
  //   CLEAR_SCREEN = 0x20,   // 0-X-1-0-0-0-XX
  //   BLINK_BLACK = 0x10,    // 0-X-0-1-0-0-XX
  //   BLINK_WHITE = 0x18,    // 0-X-0-1-1-0-XX
  //   BLINK_INVERT = 0x14,   // 0-X-0-1-0-1-XX
  //   BLINK_STOP = COM_INVERT
  // };

  /** Constructor
  * @param mosi SPI data output from mbed
  * @param mosi SPI data input from slave
  * @param sck SPI clock output from mbed
  * @param csl HIGH-active chip select input for LCD
  * @param csr LOW-active chip select input for SRAM
  */
  AkiSpiLcd(PinName mosi, PinName miso, PinName sck, PinName csl, PinName csr);

  // AkiSpiLcd(PinName mosi, PinName miso, PinName sck, PinName csl, PinName
  // disp);
  // AkiSpiLcd(PinName mosi, PinName miso, PinName sck, PinName csl, PinName
  // csr,
  //           PinName disp);

  /** Clear screen
  */
  virtual void cls();

  /** Update coloring setting
  * @param color 0 for 4bit color mode(4bits/dot), 1 for monochrome
  * mode(1bit/dot)
  */
  // void set_color(int color);

  /** Gets current coloring setting
  * @return 0 for 4bit color mode(4bits/dot), 1 for monochrome mode(1bit/dot)
  */
  // int get_color(void);

  /** Clear screen of SRAM
  * @param screen screen number (0 or 1)
  */
  void cls_ram(int screen);

  /** place a dot pixel
  * @param x x position
  * @param y y position
  */
  //    virtual void pixel(int x, int y, int colour);
  //    virtual int width() = 0;
  //    virtual int height() = 0;

  /** Writes single line(400 bits = 50 bytes)
  * @param line line number(1-240)
  * @param *data pointer to data
  */
  virtual void directUpdateSingle(int line, uint8_t *data);

  /** Writes multi lines(400 x N bits = 50 x N bytes)
  * @param line line number(1-240)
  * @param length number of line to write
  * @param *data pointer to data
  */
  virtual void directUpdateMulti(int startline, int length, uint8_t *data);

  /** Inverting internal COM signal
  */
  virtual void cominvert();

  /** Reads single line (400 bits = 50 bytes) from a screen and writes into
  * buffer
  * @param line line number(1-240)
  * @param buffer pointer to buffer(50 bytes)
  * @param screen screen to read from(0 or 1)
  */
  void ramReadSingleLine(int line, uint8_t *buffer, int screen);

  /** Reads multi lines(400 x N bits = 50 x N bytes) from a screen
  * @param startline starting line number(1-240)
  * @param length number of line to read
  * @param *buffer pointer to buffer
  * @param screen screen to read from(0 or 1)
  */
  void ramReadMultiLine(int startline, int length, uint8_t *buffer, int screen);

  /** Writes single line (400 bits = 50 bytes) into a screen
  * @param line line number(1-240)
  * @param *data pointer to data to write(50 bytes)
  * @param screen screen to read from(0 or 1)
  */
  void ramWriteSingleLine(int line, uint8_t *data, int screen);

  /** Writes multi lines(400 x N bits = 50 x N bytes) into a screen
  * @param startline starting line number(1-240)
  * @param length number of line to read
  * @param *data pointer to data to write
  * @param screen screen to read from(0 or 1)
  */
  void ramWriteMultiLine(int startline, int length, uint8_t *data, int screen);

  /** copies whole data in screen into LCD
  * @param startline starting line number(1-240)
  * @param length number of line to read
  * @param screen screen to copy (0 or 1)
  */
  void ram2lcd(int startline, int length, int screen);

  /** copies whole data in screen into LCD
  * @param screen screen to copy (0 or 1)
  */
  void ram2lcd(int screen);

  /** read a byte from SRAM (copied from Ser23K256)
  * @param address    The address to read from
  * @return the character at that address
  */
  uint8_t ram_read(int address);

  /** read multiple bytes from SRAM into a buffer (copied from Ser23K256)
  * @param address    The SRAM address to read from
  * @param buffer     The buffer to read into (must be big enough!)
  * @param count      The number of bytes to read
  */
  void ram_read(int address, uint8_t *buffer, int count);

  /** write a byte to SRAM (copied from Ser23K256)
  * @param address    The SRAM address to write to
  * @param byte       The byte to write there
  */
  void ram_write(int address, uint8_t byte);

  /** write multiple bytes to SRAM from a buffer (copied from Ser23K256)
  * @param address    The SRAM address write to
  * @param buffer     The buffer to write from
  * @param count      The number of bytes to write
  */
  void ram_write(int address, uint8_t *buffer, int count);

 private:
  // Ser23K256 _ram;

  // enum RAM_MODE { BYTE_MODE = 0x00, SEQUENTIAL_MODE = 0x40 };
  //
  // enum RAM_COMMAND {
  //   READ = 0x03,
  //   WRITE = 0x02,
  //   READ_STATUS = 0x05,  // called RDSR in datasheet
  //   WRITE_STATUS = 0x01  // called WRSR in datasheet
  // };

  int _comflag;
  int _colorflag;
  SPI _spi;
  Ser23K256 _mem;
  DigitalOut _csl;
  DigitalOut _csr;

  virtual uint8_t _generate_line(int line);
  void _cls_ram(int address);
  uint8_t ram_readStatus();
  void _ram_writeStatus(uint8_t status);
  void _ram_prepareCommand(uint8_t command, int address);
};
#endif  // NOLINT
