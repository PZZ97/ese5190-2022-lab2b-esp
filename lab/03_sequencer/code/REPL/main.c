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
#define E_READIO    '>'
#define E_SETPIN 'p'

// default read pin
int Pin;

void funcRead(){
    ADDRESS address;
    VALUE readVal;
    scanf("%x",&address);
    readVal = register_read(address);
    printf("v%08x",readVal);
}
void funcWrite(){
    ADDRESS address;
    VALUE writeVal;
    scanf("%08x",&address);
    scanf("%08x",&writeVal);
    register_write(address,writeVal);
    printf("g%08x\n",writeVal);
    // printf("m%08x",address);
}

// neopixel_set_rgb()
void funcRGB(){
    VALUE rgbVal;
    scanf("%x",&rgbVal);
    uint32_t grb = ((rgbVal & 0xFF0000) >> 8) | ((rgbVal & 0x00FF00) << 8) | (rgbVal & 0x0000FF);
    pio_sm_put_blocking(pio0, 0, grb << 8u);
    printf("v%08x",rgbVal);
}
void funcInitGpio(){
    scanf("%d",&Pin);
    // gpio_set_dir(Pin, GPIO_IN);
}
void funcGpioRead(){
    int pin=0;  // must use type int (not uint_8) and %d in scanf
    scanf("%d",&pin);
    printf("o%d",gpio_get(pin));
}
void funcGpioReadDefault(){
    // gpio_get(Pin);
    printf("o%d",gpio_get(pin));
}
// w+address+" "+value
// r+address
// c+value      color   // PIO function
// 
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
    uint8_t ioreadFlag=0;
    while(1>0){
        event= getchar_timeout_us(0);
        if(event==E_READ){
            neopixel_set_rgb(0x000000ff);   //blue
            funcRead();
        }
        else if(event==E_WRITE){
            neopixel_set_rgb(0x0000ff00);   //green
            funcWrite();
        }
        else if(event==E_RGB){
            neopixel_set_rgb(0x00ff0000);   // red
            funcRGB();
        }
        else if(event==E_READIO){
            neopixel_set_rgb(0x00000f0f);   // 
            // if(ioreadFlag++==0)
                // funcGpioRead();
            funcGpioReadDefault();
        }
        else if(event ==E_SETPIN){
            funcInitGpio();
        }


        sleep_ms(10);
    }
    return 0;
}                  
   
