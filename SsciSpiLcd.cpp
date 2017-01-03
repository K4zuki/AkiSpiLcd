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

#include "SsciSpiLcd.h"
#include "Ser23K256.h"
#include "mbed.h"  //NOLINT
extern const uint8_t lcd_line[];

SsciSpiLcd::SsciSpiLcd(PinName mosi, PinName miso, PinName sck, PinName csl,
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

  _colorflag = SsciLCD_MODE::UPDATE_4COLOR;
  _comflag = 0;
  _mux = 4;
}

void SsciSpiLcd::cls() {
  _csl = 1;

  _spi.write(SsciLCD_MODE::CLEAR_SCREEN | (_comflag << 6));
  _spi.write(0x00);

  _csl = 0;

  cominvert();
}

void SsciSpiLcd::set_color(int color) {
  if ((color & 1) == 1) {
    _colorflag = SsciLCD_MODE::UPDATE_MONO;
    _mux = 1;
  } else {
    _colorflag = SsciLCD_MODE::UPDATE_4COLOR;
    _mux = 4;
  }
}

int SsciSpiLcd::get_color(void) { return (_colorflag >> 4) & 0x01; }

void SsciSpiLcd::cls_ram(int screen) {
  screen &= 1;
  if (screen == SsciLCD_MODE::SCREEN0) {
    _cls_ram(SsciLCD_MODE::SCREEN0_BASE);
  } else {
    _cls_ram(SsciLCD_MODE::SCREEN1_BASE);
  }
}

uint8_t SsciSpiLcd::_generate_line(int line) { return line + 1; }

void SsciSpiLcd::directUpdateSingle(int line, uint8_t *data) {
  _csl = 1;
  if (line == 0) line = 1;

  _spi.write(_colorflag | (_comflag << 6));
  _spi.write(_generate_line(line));

  for (int i = 0; i < SsciLCD_MODE::LINE_LENGTH * _mux; i++) {
    _spi.write(*(data + i));
  }
  _spi.write(0x00);
  _spi.write(0x00);

  _csl = 0;

  cominvert();
}

void SsciSpiLcd::directUpdateMulti(int line, int length, uint8_t *data) {
  if (line == 0) line = 1;

  if (length > 0) {
    _csl = 1;

    for (int j = 1; j <= length; j++) {
      _spi.write(_colorflag | (_comflag << 6));
      _spi.write(_generate_line(line));
      for (int i = 0; i < SsciLCD_MODE::LINE_LENGTH * _mux; i++) {
        _spi.write(*(data + (SsciLCD_MODE::LINE_LENGTH * _mux * j + i)));
      }
      line += 1;
    }
    _spi.write(0x00);
    _spi.write(0x00);

    _csl = 0;
  }
  cominvert();
}

void SsciSpiLcd::cominvert() {
  _csl = 1;

  _spi.write(SsciLCD_MODE::COM_INVERT | (_comflag << 6));
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
void SsciSpiLcd::ramReadSingleLine(int line, uint8_t *buffer, int screen) {
  screen &= 1;
  if (screen == SsciLCD_MODE::SCREEN0) {
    screen = SsciLCD_MODE::SCREEN0_BASE;
  } else {
    screen = SsciLCD_MODE::SCREEN1_BASE;
  }

  line *= SsciLCD_MODE::RAMLINE_LENGTH;
  int address = screen + line;
  _mem.read(address, buffer, SsciLCD_MODE::RAMLINE_LENGTH);
}

/** Reads multi lines( (16 + 400) x N bits = 52 x N bytes) from a screen
*/
void SsciSpiLcd::ramReadMultiLine(int line, int length, uint8_t *buffer,
                                  int screen) {
  screen &= 1;
  if (screen == SsciLCD_MODE::SCREEN0) {
    screen = SsciLCD_MODE::SCREEN0_BASE;
  } else {
    screen = SsciLCD_MODE::SCREEN1_BASE;
  }
  line *= SsciLCD_MODE::RAMLINE_LENGTH;
  int address = screen + line;
  _mem.read(address, buffer, SsciLCD_MODE::RAMLINE_LENGTH * length);
}

/** Writes single line (400 bits = 50 bytes) into a screen
*/
void SsciSpiLcd::ramWriteSingleLine(int line, uint8_t *data, int screen) {
  screen &= 1;
  if (screen == SsciLCD_MODE::SCREEN0) {
    screen = SsciLCD_MODE::SCREEN0_BASE;
  } else {
    screen = SsciLCD_MODE::SCREEN1_BASE;
  }
  line--;
  line *= SsciLCD_MODE::RAMLINE_LENGTH;
  int address = screen + line;
  _mem.write(address, data, SsciLCD_MODE::LINE_LENGTH);
}

/** Writes multi lines(400 x N bits = 50 x N bytes) into a screen
*/
void SsciSpiLcd::ramWriteMultiLine(int line, int length, uint8_t *data,
                                   int screen) {
  screen &= 1;
  if (screen == SsciLCD_MODE::SCREEN0) {
    screen = SsciLCD_MODE::SCREEN0_BASE;
  } else {
    screen = SsciLCD_MODE::SCREEN1_BASE;
  }
  //    line--;
  //    line*=RAMLINE_LENGTH;

  int address = screen + line * SsciLCD_MODE::RAMLINE_LENGTH;
  _ram_writeStatus(SEQUENTIAL_MODE);  // SEQUENTIAL_MODE predifined in Ser23K256
  _ram_prepareCommand(WRITE, address);  // WRITE predifined in Ser23K256
  for (int i = 0; i < length; i++) {
    _spi.write(SsciLCD_MODE::UPDATE_4COLOR | (_comflag << 6));
    _spi.write(_generate_line(line));
    for (int j = 0; j < SsciLCD_MODE::LINE_LENGTH; j++) {
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
void SsciSpiLcd::ram2lcd(int startline, int length, int screen) {
  screen &= 1;
  if (screen == SsciLCD_MODE::SCREEN0) {
    screen = SsciLCD_MODE::SCREEN0_BASE;
  } else {
    screen = SsciLCD_MODE::SCREEN1_BASE;
  }
  if (startline == 0) startline = 1;

  if (length > 0) {
    int address = screen + startline * SsciLCD_MODE::RAMLINE_LENGTH;

    _ram_writeStatus(
        SEQUENTIAL_MODE);  // SEQUENTIAL_MODE predifined in Ser23K256
    _ram_prepareCommand(READ, address);  // predifined in Ser23K256
    _spi.format(16, 0);
    _csl = 1;

    for (int j = 0; j <= length; j++) {
      for (int k = 0; k < SsciLCD_MODE::RAMLINE_LENGTH; k += 4) {
        _spi.write(0x55de);
        _spi.write(0xadaa);
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
void SsciSpiLcd::ram2lcd(int screen) {
  uint8_t lineBuffer[SsciLCD_MODE::RAMLINE_LENGTH];
  for (int y = 0; y < SsciLCD_MODE::LCD_HEIGHT; y++) {
    _mem.read(y * SsciLCD_MODE::RAMLINE_LENGTH + 2, lineBuffer,
              SsciLCD_MODE::RAMLINE_LENGTH);
    directUpdateSingle(y + 1, lineBuffer);
  }
}

void SsciSpiLcd::_ram_writeStatus(uint8_t status) {
  _csr = 0;
  _spi.write(WRITE_STATUS);  // predifined in Ser23K256
  _spi.write(status);
  _csr = 1;
}

void SsciSpiLcd::_ram_prepareCommand(uint8_t command, int address) {
  _csr = 0;
  _spi.write(command);
  _spi.write(address >> 8);
  _spi.write(address & 0xFF);
}

void SsciSpiLcd::_cls_ram(int address) {
  _ram_writeStatus(SEQUENTIAL_MODE);  // SEQUENTIAL_MODE predifined in Ser23K256
  _ram_prepareCommand(WRITE, address);  // predifined in Ser23K256
  for (int i = 1; i <= (SsciLCD_MODE::LCD_HEIGHT); i++) {
    _spi.write(SsciLCD_MODE::UPDATE_4COLOR | (_comflag << 6));
    _spi.write(_generate_line(i));
    for (int j = 0; j < SsciLCD_MODE::LINE_LENGTH; j++) {
      _spi.write(0x00);
    }
  }
  _csr = 1;
  _ram_writeStatus(BYTE_MODE);  // predifined in Ser23K256
}
