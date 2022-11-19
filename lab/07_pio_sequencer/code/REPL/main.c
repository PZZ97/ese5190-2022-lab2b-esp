#include "pico/stdlib.h"
#include <stdio.h>
#include "neopixel.h"
// #include "adafruit_qtpy_rp2040.h" 
#include "hardware/gpio.h"
#include "register.h"
#include "ws2812.pio.h" // pio0
#include "pio_i2c.h"
#define QTPY_BOOT_PIN 21
#define SDA 22
#define SCL 23

#define E_READ 'r'
#define E_WRITE 'w'
#define E_RGB   'c'
#define E_READIO    '<'
#define E_SETPIN 'p'
#define E_WRITEIO '>'
#define E_I2C_W 'o'
#define E_I2C_R 'i'
// default read pin
int g_Pin;
PIO pio = pio0;
uint sm = 0;


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
}

// neopixel_set_rgb()
void funcRGB(){
    VALUE rgbVal;
    scanf("%x",&rgbVal);
    uint32_t grb = ((rgbVal & 0xFF0000) >> 8) | ((rgbVal & 0x00FF00) << 8) | (rgbVal & 0x0000FF);
    pio_sm_put_blocking(pio0, 0, grb << 8u);
    printf("v%08x",rgbVal);
}
void funcSetPin(){
    scanf("%d",&g_Pin);
    // gpio_set_dir(g_Pin, GPIO_IN);
}
void funcGpioRead(){
    int pin=0;  // must use type int (not uint_8) and %d in scanf
    scanf("%d",&pin);
    int res = gpio_get(pin);
    printf("o%d",res);
    if(res==1)
        neopixel_set_rgb(0x00000f0f);   // 
    else
        neopixel_set_rgb(0x00000000);   // 
}
void funcGpioReadDefault(){
    // gpio_get(g_Pin);
    printf("o%d",gpio_get(g_Pin));
}
void funcPIOWrite(){
    char value= 0xff;
    value=getchar_timeout_us(0);
    // scanf("%d",&value);  
    if(value=='1'){
        neopixel_set_rgb(0x000000ff);  
    }
    else if(value=='0')
        neopixel_set_rgb(0x0000ff00);  
    // printf("%c",value);
}
void i2cwrite(){
    uint8_t address;    //i2c address 7bits
    uint8_t writeVal;
    scanf("%08x",&address);
    scanf("%08x",&writeVal);
    pio_i2c_write_blocking(pio, sm, address,& writeVal, 1);
}
void i2cread(){
    uint8_t address;    //i2c address 7bits
    uint8_t readlen;
    scanf("%08x",&address);
    scanf("%08x",&readlen);
    int ret;
    uint8_t* rxbuf=(uint8*)malloc(sizeof(uint8)*readlen);
    ret= pio_i2c_read_blocking( pio, sm, address, rxbuf, readlen) ;
    for(int i=0;i<readlen;i++){
        printf("%d,",rxbuf[i]);
    }
    free(rxbuf);

}

int main() {
    stdio_init_all();
    while(stdio_usb_connected()!=true);


    uint offset = pio_add_program(pio, &i2c_program);
    i2c_program_init(pio, sm, offset, SDA, SCL);

 
    gpio_init(QTPY_BOOT_PIN);
    gpio_set_dir(QTPY_BOOT_PIN, GPIO_IN);
    neopixel_init();
    uint8_t event=0;
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
            funcGpioReadDefault();
        }
        else if(event ==E_SETPIN){
            funcSetPin();
        }
        else if(event==E_WRITEIO){
            funcPIOWrite();
        }
        else if(event == E_I2C_W){
            i2cwrite();
        }
        else if(event == E_I2C_R){
            i2cread();
        }
        // sleep_ms(10);
    }
    return 0;
}      