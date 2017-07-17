/*
Copyright (c) 2016 Kazuki Yamamoto <k.yamamoto.08136891@gmail.com>
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
#include "MemoryLcd.h"

MemoryLCD::MemoryLCD(PinName mosi, PinName miso, PinName sck, PinName csl)
    : _spi(mosi, miso, sck), _csl(csl) {
  _spi.format(8, 0);
  _spi.frequency(1000000);
}
//
// void MemoryLCD::cls() {
//   _csl = 1;
//
//   _spi.write(LCD_MODE::CLEAR_SCREEN | (_comflag << 6));
//   _spi.write(0x00);
//
//   _csl = 0;
//
//   cominvert();
// }
//
// void MemoryLCD::directUpdateSingle(int line, uint8_t *data) {
//   _csl = 1;
//   if (line == 0) line = 1;
//
//   _spi.write(LCD_MODE::UPDATE | (_comflag << 6));
//   _spi.write(line);
//
//   for (int i = 0; i < 50; i++) {
//     _spi.write(*(data + i));
//   }
//   _spi.write(0x00);
//   _spi.write(0x00);
//
//   _csl = 0;
//
//   cominvert();
// }
//
// void MemoryLCD::directUpdateMulti(int line, int length, uint8_t *data) {
//   _modeflag = 1;
//   _clearflag = 0;
//
//   if (line == 0) line = 1;
//
//   if (length > 0) {
//     _csl = 1;
//
//     for (int j = 1; j <= length; j++) {
//       _spi.write(LCD_MODE::UPDATE | (_comflag << 6));
//       _spi.write(line);
//       for (int i = 0; i < 50; i++) {
//         _spi.write(*(data + (50 * j + i)));  // hogepic[50*j+i]
//       }
//       line += 1;
//     }
//     _spi.write(0x00);
//     _spi.write(0x00);
//
//     _csl = 0;
//   }
//   cominvert();
// }
//
// void MemoryLCD::cominvert() {
//   _csl = 1;
//
//   _spi.write(LCD_MODE::COM_INVERT | (_comflag << 6));
//   _spi.write(0x00);
//
//   _csl = 0;
//   if (_comflag == 0) {
//     _comflag = 1;
//   } else {
//     _comflag = 0;
//   }
// }
