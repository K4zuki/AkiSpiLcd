/** this is for SHARP LCD LS027B4DH01
* by Kazuki Yamamoto, or _K4ZUKI_
*/

#ifndef __AKISPILCD_H__
#define __AKISPILCD_H

#include "mbed.h"
#include "Ser23K256.h"
/** AkiSpiLcd
 * mbed library for SHARP LCD LS027B4DH01
 *
 * Example:
 * @code
 * #include "mbed.h"
 * #include "AkiSpiLcd.h"
 *
 * AkiSpiLcd LCD(MOSI_, SCK_, D2, D5);
 * extern const uint8_t hogepic[];
 * int main()
 * {
 * 
 *   wait_ms(1);
 *   LCD.cls();
 *   LCD.updateSingle(10,(uint8_t*)(hogepic+2000));
 *   LCD.updateMulti(100,(240-100),(uint8_t*)(hogepic));
 *
 *   while(1) {
 *       for(int i=0; i<240; i++) {
 *           LCD.updateMulti(i,(240-i),(uint8_t*)(hogepic));
 *           LCD.updateMulti(0,(i),(uint8_t*)(hogepic+50*(240-i)));
 *       }
 *   }
 * }
 * @endcode
 */
#define RAMLINE_BASE 0x6000
#define RAMMODE_BASE 0x6100
#define SCREEN0_BASE 0x0000
#define SCREEN1_BASE 0x3000
 
class AkiSpiLcd
{
public:
    /** Constructor
    * @param mosi SPI data input
    * @param mosi SPI data output
    * @param sck SPI clock input
    * @param cs HIGH-active chip enable input
    * @param disp HIGH-active display enable input
    */
    AkiSpiLcd(PinName mosi, PinName miso, PinName sck, PinName csl, PinName csr);

    /** Clear screen
    */
    void cls();

    /** Writes single line(400 bits = 50 bytes)
    * @param line line number(1-240)
    * @param *data pointer to data
    */
    void directUpdateSingle(int line, uint8_t* data);

    /** Writes multi lines(400 x N bits = 50 x N bytes)
    * @param line line number(1-240)
    * @param length number of line to write
    * @param *data pointer to data
    */
    void directUpdateMulti(int startline, int length, uint8_t* data);

    /** Inverting internal COM signal
    */
    void cominvert();
    
    /** Reads single line (400 bits = 50 bytes) from a screen
    */
    void ramReadSingle(int line, uint8_t* buffer, int screen);

    /** Reads multi lines(400 x N bits = 50 x N bytes) from a screen
    */
    void ramReadMulti(int startline, int length, uint8_t* buffer, int screen);
    
    /** Writes single line (400 bits = 50 bytes) into a screen
    */
    void ramWriteSingle(int line, uint8_t* data, int screen);

    /** Writes multi lines(400 x N bits = 50 x N bytes) into a screen
    */
    void ramWriteMulti(int startline, int length, uint8_t* data, int screen);
    
    /** copies whole data in screen into LCD
    */
    void ram2lcd(int startline, int length, int screen);

//    /** Enables/disables display. internal memory will not flushed
//    * @param disp true = display is on / false = display is off
//    */
//    void dispOn(bool disp);

private:
    Ser23K256::Ser23K256 *_ram;
    int comflag;
    int modeflag;
    int clearflag;
    SPI _spi;
    DigitalOut _csl;
    DigitalOut _csr;
};
#endif
