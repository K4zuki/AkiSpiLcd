/*
this is for SHARP LCD LS027B4DH01
by Kazuki Yamamoto, or _K4ZUKI_
*/

#ifndef __AKISPILCD_H__
#define __AKISPILCD_H
//#include "mbed.h"

class AkiSpiLcd
{
public:
    /** Constructor
    * @param mosi SPI data input
    * @param sck SPI clock input
    * @param cs HIGH-active chip enable input
    * @param disp HIGH-active display enable input
    */
    AkiSpiLcd(PinName mosi,PinName sck, PinName cs, PinName disp);

    /** Clear screen
    */
    void cls();

    /** Writes single line(400 bits = 50 bytes)
    * @param line line number(1-240)
    * @param *data pointer to data
    */
    void updateSingle(int line, uint8_t* data);

    /** Writes multi lines(400 x N bits = 50 x N bytes)
    * @param line line number(1-240)
    * @param length number of line to write
    * @param *data pointer to data
    */
    void updateMulti(int line, int length, uint8_t* data);

    /** Inverting internal COM signal
    */
    void cominvert();

    /** Enables/disables display. internal memory will not flushed
    * @param disp true = display is on / false = display is off
    */
    void dispOn(bool disp);
    
private:
    int comflag;
    int modeflag;
    int clearflag;
    SPI _spi;
    DigitalOut _cs;
    DigitalOut _disp;
};
#endif
