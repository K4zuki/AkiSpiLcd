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
/** this is base class for Memory LCDs from SHARP or JDI
* by Kazuki Yamamoto, or _K4ZUKI_
*/

#ifndef __MEMORYLCD_H__  // NOLINT
#define __MEMORYLCD_H__

#include "mbed.h"  // NOLINT

class LCD_MODE {
 public:
  static const uint8_t COM_INVERT = 0;
  static const uint8_t CLEAR_SCREEN = 0;
  static const uint8_t UPDATE = 0;
};

// class MemoryLCD {
//  public:
//   // enum LCD_MODE { COM_INVERT, CLEAR_SCREEN, UPDATE };
//
//   /** Constructor
//   * @param mosi SPI data output from mbed
//   * @param mosi SPI data input from slave
//   * @param sck SPI clock output from mbed
//   * @param csl chip select input for LCD
//   */
//   MemoryLCD(PinName mosi, PinName miso, PinName sck, PinName csl);
//
//   /** Clear screen
//   */
//   void cls();
//
//   /** Writes single line(400 bits = 50 bytes)
//   * @param line line number
//   * @param *data pointer to data
//   */
//   void directUpdateSingle(int line, uint8_t *data);
//
//   /** Writes multi lines
//   * @param line line number
//   * @param length number of line to write
//   * @param *data pointer to data
//   */
//   void directUpdateMulti(int startline, int length, uint8_t *data);
//
//   /** Inverting internal COM signal
//   */
//   void cominvert();
//
//  private:
//   int _comflag;
//   SPI _spi;
//   DigitalOut _csl;
// };
#endif  // __MEMORYLCD_H__ //NOLINT
