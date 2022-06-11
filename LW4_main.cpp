#include "DigitalOut.h"
#include "InterruptIn.h"
#include "PinNames.h"
#include "ThisThread.h"
#include "mbed.h"
#include <cstdio>
#include <functional>

DigitalOut oled1(LED1);
DigitalOut oled4(D7);

InterruptIn ibutton(BUTTON1);
static auto sleep_time = 800ms;

int mode = 0; // 0 - LED1 fast blinking; 
              // 1 - LED1 is on, D7 blinking; 
              // 2 - LED1 fast blinking, D7 is on

Ticker toggle_led_ticker;

void led_ticker_led1() // Ticker function for LED1
{
    oled1 = !oled1;
}

void led_ticker_d7() // Ticker function for D7
{
    oled4 = !oled4;
}

void pressed(){
    mode = mode == 2 ? 0 : mode + 1; // Mode change with value check

    if (mode == 0){
        sleep_time = 400ms;
        toggle_led_ticker.detach();

        oled4 = 1;
        toggle_led_ticker.attach(&led_ticker_led1, sleep_time);
    }
    else if (mode == 1){
        sleep_time = 800ms;
        toggle_led_ticker.detach();

        oled1 = 1;
        toggle_led_ticker.attach(&led_ticker_d7, sleep_time);
    }
    else if (mode == 2){
        sleep_time = 200ms;
        toggle_led_ticker.detach();

        oled4 = 0;
        toggle_led_ticker.attach(&led_ticker_led1, sleep_time);
    }
}

void released(){}

// main() runs in its own thread in the OS
int main()
{
    oled4 = 1;
    toggle_led_ticker.attach(&led_ticker_led1, sleep_time);
    ibutton.fall(&released);
    ibutton.rise(&pressed);
    while (true) {
    }
}

