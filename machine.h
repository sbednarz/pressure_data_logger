#ifndef MACHINE_H_
#define MACHINE_H_

#include <arduino.h>
#include <wire.h>
#include "SparkFun_MicroPressure.h"


enum State { IDLE, WORKING};

#define LED_RED_PIN 2
#define LED_GREEN_PIN 15

#include "storage.h"

class Machine {
    
    public:

    Machine(Storage *);
    ~Machine();
    void begin(void);
    void start(const char*);
    void stop(void);
    void run(void);
    State get_state(void);

    void led_off(void);
    void red_led_on(void);
    void green_led_on(void);

    float get_time(void) { return mytime; }
    float get_pressure(void) { return mypressure; }
    const char* get_datasetname(void) { return mydatasetname; }
    const char* get_data(void) { return mydata; }
    const char* get_status(void) { return state==IDLE?"STATUS_IDLE":"STATUS_WORKING"; }


    SparkFun_MicroPressure *mpr;
    Storage *storage;
    

    private:
    unsigned long last_run;
    State state;
    int interval;
    unsigned long tick;
    
    float mytime;
    float mypressure;
    const char * mydatasetname;
    char mydata[100];


};

#endif
