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
//    Ser23K256 _ram(_spi,csr);
    _csl=0;
    _csr=1;
    _spi.format(8,0);
    _spi.frequency(10000000);
    comflag = modeflag = clearflag = 0;
    
    uint8_t data[240];
    for(int i=0; i<240; i++) {
        data[i]=(uint8_t)lcd_line[i];
        /*( ( (i+1) & 0x01 ) << 7 )|
             ( ( (i+1) & 0x02 ) << 5 )|
             ( ( (i+1) & 0x04 ) << 3 )|
             ( ( (i+1) & 0x08 ) << 1 )|
             ( ( (i+1) & 0x10 ) >> 1 )|
             ( ( (i+1) & 0x20 ) >> 3 )|
             ( ( (i+1) & 0x40 ) >> 5 )|
             ( ( (i+1) & 0x80 ) >> 7 );
*/
    }
    ram_write(RAMLINE_BASE,data,240);
    uint8_t buffer[4] = {0,0,0,0};
    ram_write(RAMMODE_BASE,buffer,4);

}

void AkiSpiLcd::cls()
{
    modeflag=0;
    clearflag=1;

    _csl=1;
    wait_us(5);

    _spi.write( (modeflag << 7) | (comflag << 6) | (clearflag << 5) );
    _spi.write(0x00);

    wait_us(5);
    _csl=0;

    if(comflag == 0) {
        comflag = 1;
    } else {
        comflag = 0;
    }
}

void AkiSpiLcd::directUpdateSingle(int line, uint8_t* data)
{
    modeflag=1;
    clearflag=0;

    _csl=1;
    wait_us(1);

    _spi.write( (modeflag << 7) | (comflag << 6) | (clearflag << 5) );
_spi.write((uint8_t)lcd_line[line]);
/*
    _spi.write(
        ( ( (line+1) & 0x01 ) << 7 )|
        ( ( (line+1) & 0x02 ) << 5 )|
        ( ( (line+1) & 0x04 ) << 3 )|
        ( ( (line+1) & 0x08 ) << 1 )|
        ( ( (line+1) & 0x10 ) >> 1 )|
        ( ( (line+1) & 0x20 ) >> 3 )|
        ( ( (line+1) & 0x40 ) >> 5 )|
        ( ( (line+1) & 0x80 ) >> 7 )
    );*/
    
    for(int i=0; i<50; i++) {
        _spi.write( *(data+i) );
    }
    _spi.write(0x00);
    _spi.write(0x00);

    wait_us(5);
    _csl=0;

    if(comflag == 0) {
        comflag = 1;
    } else {
        comflag = 0;
    }
}

void AkiSpiLcd::directUpdateMulti(int line, int length, uint8_t* data)
{
    modeflag=1;
    clearflag=0;

    if(length>0) {
        _csl=1;
        wait_us(5);
        for (int j=0; j<length; j++) {
            _spi.write( (modeflag << 7) | (comflag << 6) | (clearflag << 5) );
            _spi.write((uint8_t)lcd_line[line]);
/*            _spi.write(
                ( ( (line+1) & 0x01 ) << 7 )|
                ( ( (line+1) & 0x02 ) << 5 )|
                ( ( (line+1) & 0x04 ) << 3 )|
                ( ( (line+1) & 0x08 ) << 1 )|
                ( ( (line+1) & 0x10 ) >> 1 )|
                ( ( (line+1) & 0x20 ) >> 3 )|
                ( ( (line+1) & 0x40 ) >> 5 )|
                ( ( (line+1) & 0x80 ) >> 7 )
            );
*/
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
    if(comflag == 0) {
        comflag = 1;
    } else {
        comflag = 0;
    }
}

void AkiSpiLcd::cominvert()
{
    modeflag=0;
    clearflag=0;

    _csl=1;

    _spi.write( (modeflag << 7) | (comflag << 6) | (clearflag << 5) );
    _spi.write(0x00);
    wait_us(5);
    _csl=0;
    if(comflag == 0) {
        comflag = 1;
    } else {
        comflag = 0;
    }
}
/*
void AkiSpiLcd::dispOn(bool disp)
{
    if(disp) {
        _csr=1;
    } else {
        _csr=0;
    }
}
*/

/** Reads single line (400 bits = 50 bytes) from a screen
*/
void AkiSpiLcd::ramReadSingle(int line, uint8_t* buffer, int screen)
{
    screen &=1;
    if(screen==SCREEN0) {
        screen=SCREEN0_BASE;
    } else {
        screen=SCREEN1_BASE;
    }
//    uint8_t buffer[50];
    line*=50;
    int address=screen+line;
    ram_read(address,buffer,50);
    /*
    _csr=0; //select VRAM
    _spi.write(0x03);
    _spi.write(address>>8);
    _spi.write(address&0xff);
    for(int i=0;i<50;i++){
        *(buffer+i)=_spi.write(0xaa);
    }
    _csr=1;
    */
//    return buffer;
}

/** Reads multi lines(400 x N bits = 50 x N bytes) from a screen
*/
void AkiSpiLcd::ramReadMulti(int line, int length, uint8_t* buffer, int screen)
{
    screen &=1;
    if(screen==SCREEN0) {
        screen=SCREEN0_BASE;
    } else {
        screen=SCREEN1_BASE;
    }
    line*=50;
    int address=screen+line;
//    for(int j=0;j<length;j++){
    ram_read(address,buffer,50*length);
//    }
    /*
    _csr=0; //select VRAM
    _spi.write(0x03);
    _spi.write(address>>8);
    _spi.write(address&0xff);
    for(int j=0;j<length;j++){
        for(int i=0;i<50;i++){
            *(buffer+i)=_spi.write(0xaa);
        }
    }
    _csr=1;
    */
//    return buffer;
}

/** Writes single line (400 bits = 50 bytes) into a screen
*/
void AkiSpiLcd::ramWriteSingle(int line, uint8_t* data, int screen)
{
    screen &=1;
    if(screen==SCREEN0) {
        screen=SCREEN0_BASE;
    } else {
        screen=SCREEN1_BASE;
    }
    line*=50;
    int address=screen+line;
    ram_write(address,data,50);

    _csr=0; //select VRAM
    _spi.write(0x02);
    _spi.write(address>>8);
    _spi.write(address&0xff);
    for(int i=0; i<50; i++) {
        _spi.write(*(data+i));
    }
    _csr=1;
}

/** Writes multi lines(400 x N bits = 50 x N bytes) into a screen
*/
void AkiSpiLcd::ramWriteMulti(int line, int length, uint8_t* data, int screen)
{
    screen &=1;
    if(screen==SCREEN0) {
        screen=SCREEN0_BASE;
    } else {
        screen=SCREEN1_BASE;
    }
    line*=50;

    int address=screen+line;
    ram_write(address,data,50*length);
    /*
    _csr=0; //select VRAM
    _spi.write(0x02);
    _spi.write(address>>8);
    _spi.write(address&0xff);
    for(int j=0;j<length;j++){
        for(int i=0;i<50;i++){
            _spi.write(*(data+j*50+i));
        }
    }
    _csr=1;
    */
}

/** copies whole data in screen into LCD
*/
void AkiSpiLcd::ram2lcd(int startline, int length, int screen)
{
    screen &=1;
    if(screen==SCREEN0) {
        screen=SCREEN0_BASE;
    } else {
        screen=SCREEN1_BASE;
    }
//    screen<<=7;

    int address=screen+length*50;
    uint8_t dummy[50];
    /*
    _csr=0; //select VRAM
    _spi.write(0x03);
    _spi.write(address>>8);
    _spi.write(address&0xff);
    */
    ram_write(RAMMODE_BASE,(modeflag << 7) | (comflag << 6) | (clearflag << 5));
    _csl=1;
    for(int j=0; j<length; j++) {
        ram_read(RAMMODE_BASE);
        ram_read(RAMLINE_BASE+startline);
        ram_read(address+50*j,dummy,50);
        ram_read(RAMMODE_BASE+2,dummy,2);
        wait_us(5);
        _csl=0;
        if(comflag == 0) {
            comflag = 1;
        } else {
            comflag = 0;
        }
    }
    _csl=0;
}

uint8_t AkiSpiLcd::ram_read(int address)
{
    ram_prepareCommand(READ, address);
    int result = _spi.write(0);
    ram_deselect();
    return (uint8_t) result;
}

void AkiSpiLcd::ram_read(int address, uint8_t * buffer, int count)
{
    ram_writeStatus(SEQUENTIAL_MODE);
    ram_prepareCommand(READ, address);
    for (int i = 0; i < count; i++) {
        buffer[i] = _spi.write(0);
    }
    ram_deselect();
    ram_writeStatus(BYTE_MODE);
}

void AkiSpiLcd::ram_write(int address, uint8_t byte)
{
    ram_prepareCommand(WRITE, address);
    _spi.write(byte);
    ram_deselect();
}

void AkiSpiLcd::ram_write(int address, uint8_t * buffer, int count)
{
    ram_writeStatus(SEQUENTIAL_MODE);
    ram_prepareCommand(WRITE, address);
    for (int i = 0; i < count; i++) {
        _spi.write(buffer[i]);
    }
    ram_deselect();
    ram_writeStatus(BYTE_MODE);
}

uint8_t AkiSpiLcd::ram_readStatus()
{
    ram_select();
    _spi.write(READ_STATUS);
    uint8_t result = (uint8_t) _spi.write(0);
    ram_deselect();
    return result;
}

void AkiSpiLcd::ram_writeStatus(uint8_t status)
{
    ram_select();
    _spi.write(WRITE_STATUS);
    _spi.write(status);
    ram_deselect();
}

void AkiSpiLcd::ram_prepareCommand(uint8_t command, int address)
{
    ram_select();
    _spi.write(command);
    _spi.write(address >> 8);
    _spi.write(address & 0xFF);
}

void AkiSpiLcd::ram_select()
{
    _csr=0;
}

void AkiSpiLcd::ram_deselect()
{
    _csr=1;
}
