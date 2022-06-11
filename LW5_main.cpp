#include "DigitalOut.h"
#include "PinNames.h"
#include "ThisThread.h"
#include "mbed.h"

DigitalOut oled1(LED1); // Internal LED (LED1)
DigitalOut oled4(D7);   // External LED (D7)

typedef struct {
    int in_mode;  // Mode for internal LED (LED1): 
                  // 0 is off, 1 is on; 
    int out_mode; // Mode for external LED (D7): 
                  // 0 is on, 1 is off.
} message_t;

MemoryPool<message_t, 16> mpool;
Queue<message_t, 16> queue;
Thread thread;

void write_message(message_t* message, int in_mode, int out_mode)
{
    message->in_mode = in_mode;
    message->out_mode = out_mode;
}

void send_thread(void){
    uint32_t i = 0;

    while (true) {
        i++; // Thread counter
        
        message_t* message = mpool.alloc();
        rest = i % 4;

        // Thread build message by Gray's code
        if (rest == 0) {      // LED1 is off, D7 if off
            write_message(message, 0, 1);
        }
        else if (rest == 1) { // LED1 is on, D7 is off
            write_message(message, 1, 1);
        }
        else if (rest == 2) { // LED1 if on, D7 if on
            write_message(message, 1, 0);
        }
        else {                // LED1 is off, D7 is on
            write_message(message, 0, 0);
        }

        queue.put(message);
        ThisThread::sleep_for(1000);
    }
}

// main() runs in its own thread in the OS
int main()
{
    thread.start(callback(send_thread));
    
    while (true) {
        osEvent evt = queue.get();

        if (evt.status == osEventMessage) {
            message_t* message = (message_t*)evt.value.p;

            // On|off LED1 and D7 by 2'nd thread's message signals
            oled1 = message->in_mode;
            oled4 = message->out_mode;

            mpool.free(message);
        }
    }
}

