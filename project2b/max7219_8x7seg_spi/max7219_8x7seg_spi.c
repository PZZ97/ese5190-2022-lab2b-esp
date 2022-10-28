/**
 * Copyright (c) 2022 Raspberry Pi (Trading) Ltd.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include <stdio.h>
#include <string.h>
#include "pico/stdlib.h"
#include "pico/binary_info.h"
#include "hardware/spi.h"

/* Example code to talk to a Max7219 driving an 8 digit 7 segment display via SPI

   NOTE: The device is driven at 5v, but SPI communications are at 3v3

   Connections on Raspberry Pi Pico board and a generic Max7219 board, other
   boards may vary.

   * GPIO 17 (pin 22) Chip select -> CS on Max7219 board
   * GPIO 18 (pin 24) SCK/spi0_sclk -> CLK on Max7219 board
   * GPIO 19 (pin 25) MOSI/spi0_tx -> DIN on Max7219 board
   * 5v (pin 40) -> VCC on Max7219 board
   * GND (pin 38)  -> GND on Max7219 board

   Note: SPI devices can have a number of different naming schemes for pins. See
   the Wikipedia page at https://en.wikipedia.org/wiki/Serial_Peripheral_Interface
   for variations.

*/

// This defines how many Max7219 modules we have cascaded together, in this case, just the one.
#define NUM_MODULES 1
//  Register Address Map    [11:8]
const uint8_t CMD_NOOP = 0x00;
const uint8_t CMD_DIGIT0 = 0x01; // Goes up to 8, for each line
const uint8_t CMD_DECODEMODE = 0x09;    // select decode mode
const uint8_t CMD_BRIGHTNESS = 0x0A;    // Brightness
const uint8_t CMD_SCANLIMIT = 0X0B;     // How many lines to display
const uint8_t CMD_SHUTDOWN = 0X0C;      // wheather goes into shutdown mode (lower power consumption)
const uint8_t CMD_DISPLAYTEST = 0X0D;


# define PICO_DEFAULT_SPI_SCK_PIN 6 //SCLK
# define PICO_DEFAULT_SPI_TX_PIN 3
# define PICO_DEFAULT_SPI_CSN_PIN 24    //A0


// #ifdef PICO_DEFAULT_SPI_CSN_PIN
static inline void cs_select() {
    asm volatile("nop \n nop \n nop");
    gpio_put(PICO_DEFAULT_SPI_CSN_PIN, 0);  // Active low
    asm volatile("nop \n nop \n nop");
}

static inline void cs_deselect() {
    asm volatile("nop \n nop \n nop");
    gpio_put(PICO_DEFAULT_SPI_CSN_PIN, 1);
    asm volatile("nop \n nop \n nop");
}
// #endif
# define spi_default spi0
#if defined(spi_default) && defined(PICO_DEFAULT_SPI_CSN_PIN)
uint8_t smile[8]=   {0x3C,0x42,0xA5,0x81,0xA5,0x99,0x42,0x3C};
static void write_register(uint8_t reg, uint8_t data) {
    uint8_t buf[2];
    buf[0] = reg;
    buf[1] = data;
    cs_select();
    spi_write_blocking(spi_default,buf, 2);
    cs_deselect();
    sleep_ms(1);
}

static void write_register_all(uint8_t reg, uint8_t data) {
    uint8_t buf[2];
    buf[0] = reg;
    buf[1] = data;
    cs_select();
    for (int i = 0; i< NUM_MODULES;i++) {
        spi_write_blocking(spi_default, buf, 2);
    }
    cs_deselect();
}
#endif
// board data wroten to 8*8
uint8_t board[8]={0}

// The maximum lenght of snake =64
uint8_t snake[64]={9};
void generate_new(){
    uint8_t x=rand()%8;
    uint8_t y=rand()%8;
    while(board[x]&(0x01<<y)==1){
        x=rand()%8;
        y=rand()%8;
    } 
    board[x]|=0x01<<y;
}


void update()
{
    for(int i=0;i<8;i++){
        write_register(CMD_DIGIT0+i,smile[i]);
    }   
}


void clear()
{
    for (int i=0;i<8;i++) {
        write_register_all(CMD_DIGIT0 + i, 0);
    }
}



int main() {
    stdio_init_all();

    // printf("Hello, max7219! Drawing things on a 8 x 7 segment display since 2022...\n");

    // This example will use SPI0 at 10MHz.
    // spi_init(spi_default, 10 * 1000 * 1000);
    spi_init(spi_default, 10  * 1000);
    gpio_set_function(PICO_DEFAULT_SPI_SCK_PIN, GPIO_FUNC_SPI);
    gpio_set_function(PICO_DEFAULT_SPI_TX_PIN, GPIO_FUNC_SPI);

    // Make the SPI pins available to picotool
    bi_decl(bi_2pins_with_func(PICO_DEFAULT_SPI_TX_PIN, PICO_DEFAULT_SPI_SCK_PIN, GPIO_FUNC_SPI));

    // Chip select is active-low, so we'll initialise it to a driven-high state
    gpio_init(PICO_DEFAULT_SPI_CSN_PIN);
    gpio_set_dir(PICO_DEFAULT_SPI_CSN_PIN, GPIO_OUT);
    gpio_put(PICO_DEFAULT_SPI_CSN_PIN, 1);

    // Make the CS pin available to picotool
    bi_decl(bi_1pin_with_name(PICO_DEFAULT_SPI_CSN_PIN, "SPI CS"));

    // Send init sequence to device

    write_register_all(CMD_SHUTDOWN, 0);    // 0: shutdown, 1: disable shutdown
    write_register_all(CMD_DISPLAYTEST, 0); 
    write_register_all(CMD_SCANLIMIT, 7);   // 
    write_register_all(CMD_DECODEMODE, 0);
    write_register_all(CMD_SHUTDOWN, 1);
    write_register_all(CMD_BRIGHTNESS, 1);

    clear();
    while (true) {
        
        update();
        sleep_ms(500);
    }

    return 0;

}
