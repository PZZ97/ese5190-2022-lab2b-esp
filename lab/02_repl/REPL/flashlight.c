#include "pico/stdlib.h"
#include <stdio.h>
#include "neopixel.h"
#include "hardware/gpio.h"
#include "register.h"

#define QTPY_BOOT_PIN 21

// Function as a mask to change the register
ADDRESS boot_pin_address;
VALUE full_gpio_register_value;
VALUE pin_21_selection_mask;
VALUE selected_pin_state;
VALUE shifted_pin_21_state;

// sio base is 0xd0000000
// gpio input offset is 0x00000004
// full register address is 0xd0000004

typedef struct {
    uint32_t last_serial_byte;
    uint32_t button_is_pressed;
    uint32_t light_color;
} Flashlight; 

void render_to_console(Flashlight status) {
    // adjust console window height to match 'frame size'
    for (uint8_t i=0; i<10; i++) { 
        printf("\n");
    }
    printf("button_is_pressed:  0x%08x\n",   status.button_is_pressed);
    printf("light_color:        0x%08x\n",   status.light_color);
}  
enum e_state{
INPUT=1,
READ_MEM,
WRITE_MEM,
OUTPUT
};
enum e_event{
GET_READ=1,
GET_WRITE, 
DISPLAY,
WAIT_INPUT
};
//
uint8_t event=WAIT_INPUT;
ADDRESS address;
VALUE value;
void funcInput(){
    char input[4];  //"w0x" or "r0x"  NO SPACE ACCEPT
    printf("for read: r0x12345678\t\tfor write: w0x12345678 0x12345678\n");
    scanf("%3s",input);
    scanf("%x",&address);
    if(input[0]=='r'){
        event= GET_READ;
    }
    else if(input[0]=='w'){
        char buf[3]={0};
        scanf("%2s",input);
        scanf("%x",&value);
        event = GET_WRITE;
    }
    return;
}
void funcRead(){
    // printf("\nfuncRead address:%x",address);
    value = register_read(address);
    event = DISPLAY;
}
void funcWrite(){
    // printf("\nfuncWrite address:%x,",address);
    // printf("\t value=%x",value);
    register_write(address,value);
    event=DISPLAY;
}
void funcDisplay(){
    // printf("\nDisplay value=%x\n",value);
    event = WAIT_INPUT;
}
typedef struct state_table{
    uint8_t curState;
    void (*actFunc)();
    uint8_t triggerEvent;
    uint8_t nextState;
}t_state_table;
t_state_table __table[6]={
    /* curState,triggerEvent,actFunc,nextState*/
    {INPUT,     funcInput,    WAIT_INPUT,   INPUT},
    {INPUT,     funcRead,    GET_READ,   READ_MEM},
    {INPUT,     funcWrite,    GET_WRITE,  WRITE_MEM},
    {READ_MEM,  funcDisplay,     DISPLAY,    OUTPUT},
    {WRITE_MEM, funcDisplay,    DISPLAY,    OUTPUT},
    {OUTPUT,    funcInput,  WAIT_INPUT, INPUT  }
};

typedef struct state_machine{
    uint8_t curState;
    t_state_table *fsmTable;
}fsm;

void register_FSM(fsm* stateMachine,uint8_t* size){
    stateMachine->curState=OUTPUT;
    stateMachine->fsmTable=__table;
    *size = sizeof(__table)/sizeof(t_state_table);
    // printf("size=%d",*size);
}
void scheduler(uint8_t* cur_state,uint8_t event_, fsm* stateMachine,uint8_t size){
    // void (*func)();
    for(uint8_t i=0;i<size;i++){
        if(*cur_state==stateMachine->fsmTable[i].curState&&event_==stateMachine->fsmTable[i].triggerEvent){
            *cur_state=stateMachine->fsmTable[i].nextState;
            stateMachine->fsmTable[i].actFunc();

            // func();
            break;
        }
    }
    return ;

}


// int main() {
//     stdio_init_all();

//     fsm stateMachine;
//     uint8_t size;
//     register_FSM(&stateMachine,&size);
//     uint8_t currentState=INPUT;
//     while(1>0){
//         scheduler(&currentState,event, & stateMachine,size);
//         sleep_ms(10); 
//     }
//     return 0;
// }                  
   


int main() {

    stdio_init_all();
    gpio_init(QTPY_BOOT_PIN);
    gpio_set_dir(QTPY_BOOT_PIN, GPIO_IN);
    neopixel_init();

    fsm stateMachine;
    uint8_t size;
    register_FSM(&stateMachine,&size);
    uint8_t currentState=INPUT;
    while(1>0){
        scheduler(&currentState,event, & stateMachine,size);
        sleep_ms(10); 
    }
    return 0;
}                  
