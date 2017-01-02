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
* (c) Kazuki Yamamoto, or _K4ZUKI_
*/

#include "AkiSpiLcd.h"
#include "Ser23K256.h"
#include "mbed.h"  //NOLINT
extern const uint8_t lcd_line[];

AkiSpiLcd::AkiSpiLcd(PinName mosi, PinName miso, PinName sck, PinName csl,
                     PinName csr)
    : MemoryLCD(mosi, miso, sck, csl),
      _spi(mosi, miso, sck),
      _mem(_spi, csr),
      _csl(csl),
      _csr(csr) {
  _csl = 0;
  _csr = 1;
  _spi.format(8, 0);
  _spi.frequency(1000000);

  _colorflag = AkiLCD_MODE::UPDATE;
  _comflag = 0;
}

void AkiSpiLcd::cls() {
  _csl = 1;

  _spi.write(AkiLCD_MODE::CLEAR_SCREEN | (_comflag << 6));
  _spi.write(0x00);

  _csl = 0;

  cominvert();
}

// void AkiSpiLcd::set_color(int color) {
//   if ((color & 1) == 1) {
//     _colorflag = UPDATE_MONO;
//   } else {
//     _colorflag = UPDATE_4COLOR;
//   }
// }
// int AkiSpiLcd::get_color(void) { return (_colorflag >> 4) & 0x01; }

void AkiSpiLcd::cls_ram(int screen) {
  screen &= 1;
  if (screen == AkiLCD_MODE::SCREEN0) {
    _cls_ram(AkiLCD_MODE::SCREEN0_BASE);
  } else {
    _cls_ram(AkiLCD_MODE::SCREEN1_BASE);
  }
}

uint8_t AkiSpiLcd::_generate_line(int line) { return lcd_line[line]; }

void AkiSpiLcd::directUpdateSingle(int line, uint8_t *data) {
  _csl = 1;
  if (line == 0) line = 1;

  _spi.write(AkiLCD_MODE::UPDATE | (_comflag << 6));
  _spi.write(_generate_line(line));

  for (int i = 0; i < AkiLCD_MODE::LINE_LENGTH; i++) {
    _spi.write(*(data + i));
  }
  _spi.write(0x00);
  _spi.write(0x00);

  _csl = 0;

  cominvert();
}

void AkiSpiLcd::directUpdateMulti(int line, int length, uint8_t *data) {
  if (line == 0) line = 1;

  if (length > 0) {
    _csl = 1;

    for (int j = 1; j <= length; j++) {
      _spi.write(AkiLCD_MODE::UPDATE | (_comflag << 6));
      _spi.write(_generate_line(line));
      for (int i = 0; i < AkiLCD_MODE::LINE_LENGTH; i++) {
        _spi.write(*(data + (AkiLCD_MODE::LINE_LENGTH * j + i)));
      }
      line += 1;
    }
    _spi.write(0x00);
    _spi.write(0x00);

    _csl = 0;
  }
  cominvert();
}

void AkiSpiLcd::cominvert() {
  _csl = 1;

  _spi.write(AkiLCD_MODE::COM_INVERT | (_comflag << 6));
  _spi.write(0x00);

  _csl = 0;
  if (_comflag == 0) {
    _comflag = 1;
  } else {
    _comflag = 0;
  }
}

/** Reads single line (16 + 400 bits = 52 bytes) from a screen
*/
void AkiSpiLcd::ramReadSingleLine(int line, uint8_t *buffer, int screen) {
  screen &= 1;
  if (screen == AkiLCD_MODE::SCREEN0) {
    screen = AkiLCD_MODE::SCREEN0_BASE;
  } else {
    screen = AkiLCD_MODE::SCREEN1_BASE;
  }

  line *= AkiLCD_MODE::RAMLINE_LENGTH;
  int address = screen + line;
  _mem.read(address, buffer, AkiLCD_MODE::RAMLINE_LENGTH);
}

/** Reads multi lines( (16 + 400) x N bits = 52 x N bytes) from a screen
*/
void AkiSpiLcd::ramReadMultiLine(int line, int length, uint8_t *buffer,
                                 int screen) {
  screen &= 1;
  if (screen == AkiLCD_MODE::SCREEN0) {
    screen = AkiLCD_MODE::SCREEN0_BASE;
  } else {
    screen = AkiLCD_MODE::SCREEN1_BASE;
  }
  line *= AkiLCD_MODE::RAMLINE_LENGTH;
  int address = screen + line;
  _mem.read(address, buffer, AkiLCD_MODE::RAMLINE_LENGTH * length);
}

/** Writes single line (400 bits = 50 bytes) into a screen
*/
void AkiSpiLcd::ramWriteSingleLine(int line, uint8_t *data, int screen) {
  screen &= 1;
  if (screen == AkiLCD_MODE::SCREEN0) {
    screen = AkiLCD_MODE::SCREEN0_BASE;
  } else {
    screen = AkiLCD_MODE::SCREEN1_BASE;
  }
  line--;
  line *= AkiLCD_MODE::RAMLINE_LENGTH;
  int address = screen + line;
  _mem.write(address, data, AkiLCD_MODE::LINE_LENGTH);
}

/** Writes multi lines(400 x N bits = 50 x N bytes) into a screen
*/
void AkiSpiLcd::ramWriteMultiLine(int line, int length, uint8_t *data,
                                  int screen) {
  screen &= 1;
  if (screen == AkiLCD_MODE::SCREEN0) {
    screen = AkiLCD_MODE::SCREEN0_BASE;
  } else {
    screen = AkiLCD_MODE::SCREEN1_BASE;
  }
  //    line--;
  //    line*=RAMLINE_LENGTH;

  int address = screen + line * AkiLCD_MODE::RAMLINE_LENGTH;
  _ram_writeStatus(SEQUENTIAL_MODE);  // SEQUENTIAL_MODE predifined in Ser23K256
  _ram_prepareCommand(WRITE, address);  // WRITE predifined in Ser23K256
  for (int i = 0; i < length; i++) {
    _spi.write(AkiLCD_MODE::UPDATE | (_comflag << 6));
    _spi.write(_generate_line(line));
    for (int j = 0; j < AkiLCD_MODE::LINE_LENGTH; j++) {
      _spi.write(*data);
      data++;
    }
    line++;
  }
  _csr = 1;
  _ram_writeStatus(BYTE_MODE);  // predifined in Ser23K256
}

/** copies whole data in screen into LCD
*/
void AkiSpiLcd::ram2lcd(int startline, int length, int screen) {
  screen &= 1;
  if (screen == AkiLCD_MODE::SCREEN0) {
    screen = AkiLCD_MODE::SCREEN0_BASE;
  } else {
    screen = AkiLCD_MODE::SCREEN1_BASE;
  }
  if (startline == 0) startline = 1;

  if (length > 0) {
    int address = screen + startline * AkiLCD_MODE::RAMLINE_LENGTH;
    int dummy = 0;

    _ram_writeStatus(
        SEQUENTIAL_MODE);  // SEQUENTIAL_MODE predifined in Ser23K256
    _ram_prepareCommand(READ, address);  // predifined in Ser23K256
    _spi.format(16, 0);
    _csl = 1;

    for (int j = 0; j <= length; j++) {
      for (int k = 0; k < AkiLCD_MODE::RAMLINE_LENGTH; k += 4) {
        dummy = _spi.write(0x55de);
        dummy = _spi.write(0xadaa);
      }
    }
  }
  _spi.write(0xdead);
  _csl = 0;
  _csr = 1;
  _spi.format(8, 0);
  cominvert();
}

/** copies whole data in screen into LCD
*/
void AkiSpiLcd::ram2lcd(int screen) {
  uint8_t lineBuffer[AkiLCD_MODE::RAMLINE_LENGTH];
  for (int y = 0; y < AkiLCD_MODE::LCD_HEIGHT; y++) {
    _mem.read(y * AkiLCD_MODE::RAMLINE_LENGTH + 2, lineBuffer,
              AkiLCD_MODE::RAMLINE_LENGTH);
    directUpdateSingle(y + 1, lineBuffer);
  }
}

void AkiSpiLcd::_ram_writeStatus(uint8_t status) {
  _csr = 0;
  _spi.write(WRITE_STATUS);  // predifined in Ser23K256
  _spi.write(status);
  _csr = 1;
}

void AkiSpiLcd::_ram_prepareCommand(uint8_t command, int address) {
  _csr = 0;
  _spi.write(command);
  _spi.write(address >> 8);
  _spi.write(address & 0xFF);
}

void AkiSpiLcd::_cls_ram(int address) {
  _ram_writeStatus(SEQUENTIAL_MODE);  // SEQUENTIAL_MODE predifined in Ser23K256
  _ram_prepareCommand(WRITE, address);  // predifined in Ser23K256
  for (int i = 1; i <= (AkiLCD_MODE::LCD_HEIGHT); i++) {
    _spi.write(AkiLCD_MODE::UPDATE | (_comflag << 6));
    _spi.write(_generate_line(i));
    for (int j = 0; j < AkiLCD_MODE::LINE_LENGTH; j++) {
      _spi.write(0x00);
    }
  }
  _csr = 1;
  _ram_writeStatus(BYTE_MODE);  // predifined in Ser23K256
}
