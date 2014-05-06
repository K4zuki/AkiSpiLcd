/** this is for SHARP LCD LS027B4DH01
* by Kazuki Yamamoto, or _K4ZUKI_
*/

#include "mbed.h"
#include "AkiSpiLcd.h"

AkiSpiLcd::AkiSpiLcd(PinName mosi,PinName sck, PinName cs, PinName disp)
    :_spi(mosi,NC,sck), _cs(cs), _disp(disp)
{
    _cs=0;
    _spi.format(8,0);
    _spi.frequency(2000000);
    comflag = modeflag = clearflag = 0;
    _disp=1;
}

void AkiSpiLcd::cls()
{
    modeflag=0;
    clearflag=1;

    _cs=1;
    wait_us(5);

    _spi.write( (modeflag << 7) | (comflag << 6) | (clearflag << 5) );
    _spi.write(0x00);

    wait_us(5);
    _cs=0;

    if(comflag == 0) {
        comflag = 1;
    } else {
        comflag = 0;
    }
}

void AkiSpiLcd::updateSingle(int line, uint8_t* data)
{
    modeflag=1;
    clearflag=0;

    _cs=1;
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
    _cs=0;

    if(comflag == 0) {
        comflag = 1;
    } else {
        comflag = 0;
    }
}

void AkiSpiLcd::updateMulti(int line, int length, uint8_t* data)
{
    modeflag=1;
    clearflag=0;

    if(length>0) {
        _cs=1;
        wait_us(5);
        for (int j=0; j<length; j++) {
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
                _spi.write( *(data+(50*j+i)) );//hogepic[50*j+i]
            }
            line+=1;
        }
        _spi.write(0x00);
        _spi.write(0x00);
        wait_us(5);
        _cs=0;
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

    _cs=1;

    _spi.write( (modeflag << 7) | (comflag << 6) | (clearflag << 5) );
    _spi.write(0x00);
    wait_us(5);
    _cs=0;
    if(comflag == 0) {
        comflag = 1;
    } else {
        comflag = 0;
    }
}

void AkiSpiLcd::dispOn(bool disp)
{
    if(disp) {
        _disp=1;
    } else {
        _disp=0;
    }
}
