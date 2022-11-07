#include "pico/stdlib.h"
#include <stdio.h>
#include "neopixel.h"
// #include "adafruit_qtpy_rp2040.h" 
#include "hardware/gpio.h"
#include "register.h"
#include "ws2812.pio.h" // pio0
#define QTPY_BOOT_PIN 21

#define E_READ 'r'
#define E_WRITE 'w'
#define E_RGB   'c'

void funcRead(){
    ADDRESS address;
    VALUE readVal;
    scanf("%x",&address);
    readVal = register_read(address);
    printf("v%x",readVal);
}
void funcWrite(){
    ADDRESS address;
    VALUE writeVal;
    scanf("%x",&address);
    getchar();
    scanf("%x",&writeVal);
    register_write(address,writeVal);
    printf("v%x",writeVal);
}

// neopixel_set_rgb()
void funcRGB(){
    VALUE rgbVal;
    scanf("%x",&rgbVal);
    uint32_t grb = ((rgbVal & 0xFF0000) >> 8) | ((rgbVal & 0x00FF00) << 8) | (rgbVal & 0x0000FF);
    pio_sm_put_blocking(pio0, 0, grb << 8u);
    printf("v%x",rgbVal);
}

// w+address+" "+value
// r+address
// c+value      color
// (DON'T NEED THIS IN C code)b+value      brightness 
// (DON'T NEED THIS IN C code)#+num        start record num seconds
//
int main() {
    stdio_init_all();
    while(stdio_usb_connected()!=true);

    gpio_init(QTPY_BOOT_PIN);
    gpio_set_dir(QTPY_BOOT_PIN, GPIO_IN);
    neopixel_init();
    uint8_t event=0;
    while(1>0){
        event= getchar_timeout_us(0);
        if(event==E_READ){
            funcRead();
        }
        else if(event==E_WRITE){
            funcWrite();
        }
        else if(event==E_RGB){
            funcRGB();
        }
        sleep_ms(10);
    }
    return 0;
}                  
   
