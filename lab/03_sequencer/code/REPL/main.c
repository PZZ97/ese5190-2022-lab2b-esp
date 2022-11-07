#include "pico/stdlib.h"
#include <stdio.h>
// #include "neopixel.h"
#include "hardware/pio.h"
#include "ws2812.pio.h"
#include "adafruit_qtpy_rp2040.h" 
#include "hardware/gpio.h"
#include "register.h"

#include <stdio.h>
#include "register.h"


#define E_READ 'r'
#define E_WRITE 'w'

void funcRead(){
    ADDRESS address;
    VALUE readVal;
    scanf("%x",&address);
    readVal = register_read(address);
    printf("\n%x",readVal);
}
void funcWrite(){
    ADDRESS address;
    VALUE writeVal;
    scanf("%x",&address);
    getchar();
    scanf("%x",&writeVal);
    register_write(address,writeVal);
}

// w+address+" "+value
// r+address
// (DON'T NEED THIS IN C code)c+value      color
// (DON'T NEED THIS IN C code)b+value      brightness 
// (DON'T NEED THIS IN C code)#+num        start record num seconds
//
int main() {
    stdio_init_all();
    while(stdio_usb_connected()!=true);
    
    while(1>0){
        event= getchar_timeout_us(0);
        if(event==E_READ){
            funcRead();
        }
        else if(event==E_WRITE){
            funcWrite();
        }
        sleep_ms(10);
    }
    return 0;
}                  
   
