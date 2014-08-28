/** this is for SHARP LCD LS027B4DH01
* by Kazuki Yamamoto, or _K4ZUKI_
*/

#include "mbed.h"
#include "AkiSpiLcd.h"

AkiSpiLcd::AkiSpiLcd(PinName mosi, PinName miso, PinName sck, PinName csl, PinName csr)
    :_spi(mosi, miso, sck), _csl(csl), _csr(csr)
{
    _ram= new Ser23K256::Ser23K256(_spi,csr);
    _csl=0;
    _csr=1;
    _spi.format(8,0);
    _spi.frequency(2000000);
    comflag = modeflag = clearflag = 0;

    int data=0;
    for(int i=0;i<240;i++){
        data=( ( i & 0x01 ) << 7 )|
            ( ( i & 0x02 ) << 5 )|
            ( ( i & 0x04 ) << 3 )|
            ( ( i & 0x08 ) << 1 )|
            ( ( i & 0x10 ) >> 1 )|
            ( ( i & 0x20 ) >> 3 )|
            ( ( i & 0x40 ) >> 5 )|
            ( ( i & 0x80 ) >> 7 );

        _ram->write(RAMLINE_BASE+i,(uint8_t)data);
    }
        _ram->write(RAMMODE_BASE,0x00000000);

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
    wait_us(5);

    _spi.write( (modeflag << 7) | (comflag << 6) | (clearflag << 5) );

    _spi.write(
        ( ( (line+1) & 0x01 ) << 7 )|
        ( ( (line+1) & 0x02 ) << 5 )|
        ( ( (line+1) & 0x04 ) << 3 )|
        ( ( (line+1) & 0x08 ) << 1 )|
        ( ( (line+1) & 0x10 ) >> 1 )|
        ( ( (line+1) & 0x20 ) >> 3 )|
        ( ( (line+1) & 0x40 ) >> 5 )|
        ( ( (line+1) & 0x80 ) >> 7 )
    );
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
            _spi.write(
                ( ( (line) & 0x01 ) << 7 )|
                ( ( (line) & 0x02 ) << 5 )|
                ( ( (line) & 0x04 ) << 3 )|
                ( ( (line) & 0x08 ) << 1 )|
                ( ( (line) & 0x10 ) >> 1 )|
                ( ( (line) & 0x20 ) >> 3 )|
                ( ( (line) & 0x40 ) >> 5 )|
                ( ( (line) & 0x80 ) >> 7 )
            );

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
void AkiSpiLcd::ramReadSingle(int line, uint8_t* buffer, int screen){
    screen &=1;
    if(screen==0){
        screen=SCREEN0_BASE;
    }else{
        screen=SCREEN1_BASE;
    }
//    uint8_t buffer[50];
    line*=50;
    int address=screen+line;
    _ram->read(address,buffer,50);
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
void AkiSpiLcd::ramReadMulti(int line, int length, uint8_t* buffer, int screen){
    screen &=1;
    if(screen==0){
        screen=SCREEN0_BASE;
    }else{
        screen=SCREEN1_BASE;
    }
    line*=50;
    int address=screen+line;
//    for(int j=0;j<length;j++){
    _ram->read(address,buffer,50*length);
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
void AkiSpiLcd::ramWriteSingle(int line, uint8_t* data, int screen){
    screen &=1;
    if(screen==0){
        screen=SCREEN0_BASE;
    }else{
        screen=SCREEN1_BASE;
    }
    line*=50;
    int address=screen+line;
    _ram->write(address,data,50);
    
    _csr=0; //select VRAM
    _spi.write(0x02);
    _spi.write(address>>8);
    _spi.write(address&0xff);
    for(int i=0;i<50;i++){
        _spi.write(*(data+i));
    }
    _csr=1;
}

    /** Writes multi lines(400 x N bits = 50 x N bytes) into a screen
    */
void AkiSpiLcd::ramWriteMulti(int line, int length, uint8_t* data, int screen){
    screen &=1;
    if(screen==0){
        screen=SCREEN0_BASE;
    }else{
        screen=SCREEN1_BASE;
    }
    line*=50;
    
    int address=screen+line;
    _ram->write(address,data,50*length);
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
void AkiSpiLcd::ram2lcd(int startline, int length, int screen){
    screen &=1;
    if(screen==0){
        screen=SCREEN0_BASE;
    }else{
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
    for(int j=0;j<240;j++){
        _ram->write(RAMMODE_BASE,(modeflag << 7) | (comflag << 6) | (clearflag << 5));
        _csl=1;
        _ram->read(RAMMODE_BASE);
        _ram->read(RAMLINE_BASE+startline);
        _ram->read(address+50*j,dummy,50);
        _ram->read(RAMMODE_BASE+2,dummy,2);
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
