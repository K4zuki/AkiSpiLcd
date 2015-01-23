/** this is for SHARP LCD LS027B4DH01
* by Kazuki Yamamoto, or _K4ZUKI_
*/

#include "mbed.h"
#include "AkiSpiLcd.h"
//#include "Ser23K256.h"
extern const uint8_t lcd_line[];

AkiSpiLcd::AkiSpiLcd(PinName mosi, PinName miso, PinName sck, PinName csl, PinName csr)
    :_spi(mosi, miso, sck), _csl(csl), _csr(csr)
{
    _csl=0;
    _csr=1;
    _spi.format(8,0);
    _spi.frequency(10000000);
    _comflag = _modeflag = _clearflag = 0;

//    if(_csr != NULL) {
//        uint8_t data[240];
//        data[0]=0;
//        for(int i=1; i<=240; i++) {
//            data[i]=(uint8_t)lcd_line[i];
//        }
//        ram_write(RAMLINE_BASE,data,240);
//    }
//    uint8_t buffer[4] = {0,0,0,0};
//    ram_write(RAMMODE_BASE,buffer,4);

}

void AkiSpiLcd::cls()
{
    _modeflag=0;
    _clearflag=1;

    _csl=1;
    wait_us(5);

    _spi.write( (_modeflag << 7) | (_comflag << 6) | (_clearflag << 5) );
    _spi.write(0x00);

    wait_us(5);
    _csl=0;

    cominvert();
}

void AkiSpiLcd::cls_ram( int screen )
{
    screen &=1;
    if(screen==SCREEN0) {
        _cls_ram( SCREEN0_BASE );
    } else {
        _cls_ram( SCREEN1_BASE );
    }
}


void AkiSpiLcd::directUpdateSingle(int line, uint8_t* data)
{
    _modeflag=1;
    _clearflag=0;

    _csl=1;
    wait_us(1);

    if(line ==0)line=1;

    _spi.write( (_modeflag << 7) | (_comflag << 6) | (_clearflag << 5) );
    _spi.write((uint8_t)lcd_line[line]);

    for(int i=0; i<50; i++) {
        _spi.write( *(data+i) );
    }
    _spi.write(0x00);
    _spi.write(0x00);

    wait_us(5);
    _csl=0;

    cominvert();
}

void AkiSpiLcd::directUpdateMulti(int line, int length, uint8_t* data)
{
    _modeflag=1;
    _clearflag=0;

    if(line==0)line=1;

    if(length>0) {
        _csl=1;
        wait_us(5);
        for (int j=1; j<=length; j++) {
            _spi.write( (_modeflag << 7) | (_comflag << 6) | (_clearflag << 5) );
            _spi.write((uint8_t)lcd_line[line]);
            for(int i=0; i<50; i++) {
                _spi.write( *(data+(50*j+i)) );//hogepic[50*j+i]
            }
            line+=1;
        }
        _spi.write(0x00);
        _spi.write(0x00);
        wait_us(5);
        _csl=0;
    }
    cominvert();
}

void AkiSpiLcd::cominvert()
{
    _modeflag=0;
    _clearflag=0;

    _csl=1;

    _spi.write( (_modeflag << 7) | (_comflag << 6) | (_clearflag << 5) );
    _spi.write(0x00);
//    wait_us(5);
    _csl=0;
    if(_comflag == 0) {
        _comflag = 1;
    } else {
        _comflag = 0;
    }
}

/** Reads single line (16 + 400 bits = 52 bytes) from a screen
*/
void AkiSpiLcd::ramReadSingleLine(int line, uint8_t* buffer, int screen)
{
    screen &=1;
    if(screen==SCREEN0) {
        screen=SCREEN0_BASE;
    } else {
        screen=SCREEN1_BASE;
    }

    line*=RAMLINE_LENGTH;
    int address=screen+line;
    ram_read(address,buffer,RAMLINE_LENGTH);
}

/** Reads multi lines( (16 + 400) x N bits = 52 x N bytes) from a screen
*/
void AkiSpiLcd::ramReadMultiLine(int line, int length, uint8_t* buffer, int screen)
{
    screen &=1;
    if(screen==SCREEN0) {
        screen=SCREEN0_BASE;
    } else {
        screen=SCREEN1_BASE;
    }
    line*=RAMLINE_LENGTH;
    int address=screen+line;
    ram_read(address,buffer,RAMLINE_LENGTH*length);
}

/** Writes single line (400 bits = 50 bytes) into a screen
*/
void AkiSpiLcd::ramWriteSingleLine(int line, uint8_t* data, int screen)
{
    screen &=1;
    if(screen==SCREEN0) {
        screen=SCREEN0_BASE;
    } else {
        screen=SCREEN1_BASE;
    }
    line--;
    line*=RAMLINE_LENGTH;
    int address=screen+line;
    ram_write(address,data,50);
}

/** Writes multi lines(400 x N bits = 50 x N bytes) into a screen
*/
void AkiSpiLcd::ramWriteMultiLine(int line, int length, uint8_t* data, int screen)
{
    _modeflag=1;
    _clearflag=0;
    screen &=1;
    if(screen==SCREEN0) {
        screen=SCREEN0_BASE;
    } else {
        screen=SCREEN1_BASE;
    }
//    line--;
//    line*=RAMLINE_LENGTH;

    int address=screen+line*RAMLINE_LENGTH;
    _ram_writeStatus(SEQUENTIAL_MODE);
    _ram_prepareCommand(WRITE, address);
    for(int i=0; i<length; i++) {
        _spi.write( (_modeflag << 7) | (_comflag << 6) | (_clearflag << 5) );
        _spi.write( (uint8_t)lcd_line[line] );
        for(int j = 0; j < LINE_LENGTH; j++) {
            _spi.write(*data);
            data++;
        }
        line++;
    }
    _ram_deselect();
    _ram_writeStatus(BYTE_MODE);
//    ram_write(address,data,50*length);
}

/** copies whole data in screen into LCD
*/
void AkiSpiLcd::ram2lcd(int startline, int length, int screen)
{
    _modeflag=1;
    _clearflag=0;
    screen &=1;
    if(screen==SCREEN0) {
        screen=SCREEN0_BASE;
    } else {
        screen=SCREEN1_BASE;
    }
    if(startline==0)startline=1;

    if(length>0) {

        int address=screen+startline*RAMLINE_LENGTH;
//    uint8_t dummy[RAMLINE_LENGTH+2];
        int dummy=0;

        _ram_writeStatus(SEQUENTIAL_MODE);
        _ram_prepareCommand(READ,address);
_spi.format(16,0);
        _csl=1;
//wait_us(5);
        
        for(int j = 0; j <= length; j++) {
//        _csl=1;
            for(int k = 0; k < RAMLINE_LENGTH; k+=4) {
                dummy = _spi.write(0x55de);
//                dummy = _spi.write(0xde);
                dummy = _spi.write(0xadaa);
//            dummy = _spi.write(0xbe);
//            dummy = _spi.write(0xaf);
//                dummy = _spi.write(0xaa);
            }
//        _csl = 0;
        }
    }
    _spi.write(0xdead);
//wait_us(5);
    _csl = 0;
//    _spi.write(0xde);
//    _spi.write(0xad);
    _ram_deselect();
_spi.format(8,0);
    cominvert();
}

/** copies whole data in screen into LCD
*/
void AkiSpiLcd::ram2lcd(int screen)
{
    uint8_t lineBuffer[RAMLINE_LENGTH];
    for (int y = 0; y < 240; y++) {
        ram_read(y * RAMLINE_LENGTH+2,lineBuffer,RAMLINE_LENGTH);
        directUpdateSingle(y + 1,lineBuffer);
    }
}
uint8_t AkiSpiLcd::ram_read(int address)
{
    _ram_prepareCommand(READ, address);
    int result = _spi.write(0);
    _ram_deselect();
    return (uint8_t) result;
}

void AkiSpiLcd::ram_read(int address, uint8_t * buffer, int count)
{
    _ram_writeStatus(SEQUENTIAL_MODE);
    _ram_prepareCommand(READ, address);
    for (int i = 0; i < count; i++) {
        buffer[i] = _spi.write(0x00);

    }
    _ram_deselect();
    _ram_writeStatus(BYTE_MODE);
}

void AkiSpiLcd::ram_write(int address, uint8_t byte)
{
    _ram_prepareCommand(WRITE, address);
    _spi.write(byte);
    _ram_deselect();
}

void AkiSpiLcd::ram_write(int address, uint8_t * buffer, int count)
{
    _ram_writeStatus(SEQUENTIAL_MODE);
    _ram_prepareCommand(WRITE, address);
    for (int i = 0; i < count; i++) {
        _spi.write(buffer[i]);
    }
    _ram_deselect();
    _ram_writeStatus(BYTE_MODE);
}

uint8_t AkiSpiLcd::ram_readStatus()
{
    _ram_select();
    _spi.write(READ_STATUS);
    uint8_t result = (uint8_t) _spi.write(0);
    _ram_deselect();
    return result;
}

void AkiSpiLcd::_ram_writeStatus(uint8_t status)
{
    _ram_select();
    _spi.write(WRITE_STATUS);
    _spi.write(status);
    _ram_deselect();
}

void AkiSpiLcd::_ram_prepareCommand(uint8_t command, int address)
{
    _ram_select();
    _spi.write(command);
    _spi.write(address >> 8);
    _spi.write(address & 0xFF);
}

void AkiSpiLcd::_ram_select()
{
    _csr = 0;
}

void AkiSpiLcd::_ram_deselect()
{
    _csr = 1;
}

void AkiSpiLcd::_cls_ram( int address )
{
    _modeflag=1;
    _clearflag=0;
    _ram_writeStatus(SEQUENTIAL_MODE);
    _ram_prepareCommand(WRITE, address);
    for (int i = 1; i <= (240); i++) {
        _spi.write( (_modeflag << 7) | (_comflag << 6) | (_clearflag << 5) );
        _spi.write( (uint8_t)lcd_line[i] );
        for(int j = 0; j < 50; j++) {
            _spi.write(0x00);
        }

    }
    _ram_deselect();
    _ram_writeStatus(BYTE_MODE);
}
