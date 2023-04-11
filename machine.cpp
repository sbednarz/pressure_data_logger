#include "machine.h"

Machine::Machine(Storage *s) {
    storage = s;
}

Machine::~Machine() {
}

void Machine::begin(void) {
    state = IDLE;
    interval = 1000;
    pinMode(LED_RED_PIN, OUTPUT);
    pinMode(LED_GREEN_PIN, OUTPUT);
    Wire.begin();
    mpr = new SparkFun_MicroPressure();
    mytime = 0;
    mypressure = 0;
    mydatasetname = "";

    if(!mpr->begin())
    {
        Serial.println("# sensor error: cannot connect to MicroPressure sensor.");
        while(1);
    }
    Serial.println("Machine created");
}

void Machine::led_off(void){
  digitalWrite(LED_GREEN_PIN, LOW);
  digitalWrite(LED_RED_PIN, LOW);
}

void Machine::red_led_on(void){
  digitalWrite(LED_GREEN_PIN, LOW);
  digitalWrite(LED_RED_PIN, HIGH);
}

void Machine::green_led_on(void){
  digitalWrite(LED_GREEN_PIN, HIGH);
  digitalWrite(LED_RED_PIN, LOW);
}


void Machine::start(const char* timestamp) {
    if (state == IDLE) {
        state = WORKING;
        mytime = 0;
        tick = 0;
        storage->new_dataset(timestamp);
        mydatasetname = storage->get_datasetname();
    }
}
void Machine::stop(void) {
    if (state == WORKING) {
        state = IDLE;
        mytime = 0;
        tick = 0;
        storage->close_dataset();
        mydatasetname = "";
    }
}




void Machine::run(void) {

    unsigned long now = millis();
    if (now < last_run + interval) {
        return;
    }
    last_run = now;
    tick++;


    mypressure = mpr->readPressure(PA);

    if (state == IDLE) {
        sprintf(mydata, "%.1f %.0f %s", mytime, mypressure, mydatasetname);
        Serial.println(mydata);

    }

    if (state == WORKING) {
        mytime = mytime + interval/1000;
        sprintf(mydata, "%.1f %.0f %s", mytime, mypressure, mydatasetname);
        Serial.println(mydata);
        char buff[80];
        sprintf(buff, "%.1f %.0f", mytime, mypressure);
        storage->add_row(buff);

        (tick%2)?green_led_on():led_off();
    }


}




