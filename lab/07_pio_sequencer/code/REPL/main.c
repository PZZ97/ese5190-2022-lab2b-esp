#include "pico/stdlib.h"
#include <stdio.h>
#include "neopixel.h"
// #include "adafruit_qtpy_rp2040.h" 
#include "hardware/gpio.h"
#include "register.h"
#include "ws2812.pio.h" // pio0
#include "hardware/dma.h"
#define QTPY_BOOT_PIN 21

#define E_READ 'r'
#define E_WRITE 'w'
#define E_RGB   'c'
#define E_READIO    '<'
#define E_SETPIN 'p'
#define E_WRITEIO '>'
// default read pin
int g_Pin;

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
    // printf("g%08x\n",writeVal);
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
void funcSetPin(){
    scanf("%d",&g_Pin);
    // gpio_set_dir(g_Pin, GPIO_IN);
}
// void funcGpioRead(){
//     int pin=0;  // must use type int (not uint_8) and %d in scanf
//     scanf("%d",&pin);
//     int res = gpio_get(pin);
//     printf("o%d",res);
//     if(res==1)
//         neopixel_set_rgb(0x00000f0f);   // 
//     else
//         neopixel_set_rgb(0x00000000);   // 
// }
void funcGpioReadDefault(){
    // gpio_get(g_Pin);
    // printf("o%d",gpio_get(g_Pin));
    uint32_t read;

    gpio_pio_read(&read,g_Pin);
    printf("o%d",read);
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
void PIO_init(PIO pio, uint sm, uint pin_base,  float div) {
    // uint16_t capture_prog_instr = pio_encode_in(pio_pins, 1);   /*  => $ IN pio_pins pin_count
    //                                                                         (pio_pins=0, source =PINS)
    //                                                                         pin_count=2, shift 2bits to ISR 
    //                                                                         datasheet 3.2.3.2 ISR  */
    // struct pio_program capture_prog = {
    //         .instructions = &capture_prog_instr,
    //         .length = 1,
    //         .origin = -1
    // };
    // uint offset = pio_add_program(pio, &capture_prog);                  /*  Create the pio program */

    // Configure state machine to loop over this `in` instruction forever,
    // with autopush enabled.
    pio_sm_config c = pio_get_default_sm_config();
    sm_config_set_in_pins(&c, pin_base);
    // sm_config_set_wrap(&c, offset, offset);
    sm_config_set_clkdiv(&c, div);
    // Note that we may push at a < 32 bit threshold if pin_count does not
    // divide 32. We are using shift-to-right, so the sample data ends up
    // left-justified in the FIFO in this case, with some zeroes at the LSBs.
                                                /* push_threshold */
    int push_threshold_bit =1;
    sm_config_set_in_shift(&c, true, true, push_threshold_bit);
    sm_config_set_fifo_join(&c, PIO_FIFO_JOIN_RX);
    // pio_sm_init(pio, sm, offset, &c);
}

// void gpio_pio_read(PIO pio, uint sm, uint dma_chan, uint32_t *read_buf,uint32_t pin) {
void gpio_pio_read(uint32_t *read_buf,uint32_t pin) {
    gpio_init(pin);
    gpio_set_dir(pin, GPIO_IN);
    PIO pio = pio0;
    uint sm = 0;
    uint dma_chan = 0;
    PIO_init(pio,sm,pin,1.f);

    pio_sm_clear_fifos(pio, sm);
    pio_sm_restart(pio, sm);

    dma_channel_config c = dma_channel_get_default_config(dma_chan);
    channel_config_set_read_increment(&c, false);
    channel_config_set_write_increment(&c, false);
    channel_config_set_dreq(&c, pio_get_dreq(pio, sm, false));  //Return the DREQ to use for pacing transfers to/from a particular state machine FIFO.
                                                    // bool is_tx= false
    dma_channel_configure(dma_chan, &c,
        read_buf,        // Destination pointer
        &pio->rxf[sm],      // Source pointer
        1, // Number of transfers
        true                // Start immediately
    );
    pio_sm_exec(pio, sm,  pio_encode_in(pio_pins, 1)); /* stall until the first up edge */
}
int main() {
    stdio_init_all();
    while(stdio_usb_connected()!=true);

    // gpio_init(QTPY_BOOT_PIN);
    // gpio_set_dir(QTPY_BOOT_PIN, GPIO_IN);
    /////////////////////////////////////////////////////

    /////////////////////////////////////////////////////
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
        // sleep_ms(10);
    }
    return 0;
}                  
   
