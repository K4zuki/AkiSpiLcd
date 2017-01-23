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
/** this is for JDI LCD LPM013M126A
* by Kazuki Yamamoto, or _K4ZUKI_
*/

#ifndef __SSCISPILCD_H__  // NOLINT
#define __SSCISPILCD_H__  // NOLINT

#include "MemoryLcd.h"
#include "Ser23K256.h"
#include "mbed.h"  //NOLINT

/** \class SsciSpiLcd
 * \brief mbed library for SHARP LCD LS027B4DH01
 *
 * Example:
 * @code
 * #include "mbed.h"
 * #include "SsciSpiLcd.h"
 *
 * SsciSpiLcd LCD(MOSI_, MISO_, SCK_, D2, D5);
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

class SsciLCD_MODE : public LCD_MODE {
 public:
  static const uint8_t COM_INVERT = 0x00;     // 0-X-0-0-0-0-XX
  static const uint8_t UPDATE_4COLOR = 0x90;  // 1-X-0-1-0-0-XX
  static const uint8_t UPDATE_MONO = 0x88;    // 1-X-0-0-1-0-XX
  static const uint8_t UPDATE_3COLOR = 0x80;  // 1-X-0-0-0-0-XX
  static const uint8_t CLEAR_SCREEN = 0x20;   // 0-X-1-0-0-0-XX
  static const uint8_t BLINK_BLACK = 0x10;    // 0-X-0-1-0-0-XX
  static const uint8_t BLINK_WHITE = 0x18;    // 0-X-0-1-1-0-XX
  static const uint8_t BLINK_INVERT = 0x14;   // 0-X-0-1-0-1-XX
  static const uint8_t BLINK_STOP = COM_INVERT;

  static const uint8_t SCREEN0 = 0;
  static const uint8_t SCREEN1 = 1;
  static const uint16_t SCREEN0_BASE = 0x0000;
  static const uint16_t SCREEN1_BASE = 0x4000;

  static const uint8_t LINE_LENGTH = 22;
  static const uint8_t RAMLINE_LENGTH = 90;

  static const uint8_t LCD_HEIGHT = 176;
};

class SsciSpiLcd : public MemoryLCD {
 public:
  /** Constructor
  * @param mosi SPI data output from mbed
  * @param mosi SPI data input from slave
  * @param sck SPI clock output from mbed
  * @param csl HIGH-active chip select input for LCD
  * @param csr LOW-active chip select input for SRAM
  */
  SsciSpiLcd(PinName mosi, PinName miso, PinName sck, PinName csl, PinName csr);

  /** Clear screen
  */
  virtual void cls();

  /** Update coloring setting
  * @param color 0 for 4bit color mode(4bits/dot), 1 for monochrome
  * mode(1bit/dot)
  */
  void set_color(int color);

  /** Gets current coloring setting
  * @return 0 for 4bit color mode(4bits/dot), 1 for monochrome mode(1bit/dot)
  */
  int get_color(void);

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
  int _comflag;
  int _colorflag;
  int _mux;
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
