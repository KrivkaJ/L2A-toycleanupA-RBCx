#include "Adafruit_TCS34725.h"
#include "SmartServoBus.hpp"
#include "robotka.h"
#include <Arduino.h>

byte state = 1;
unsigned long startTime = 0;
const unsigned long FinalTime = 250000;

// Funkce setup se zavolá vždy po startu robota.
float g_US = 1;
float k = 0;
byte brick_count = 5;

// pokud se nepovede neco inicializovat (RGB senzor), program se zasekne v teto funkci
void trap() {
    Serial.println("trap\n");
    while (1)
        ;
}

static const uint8_t TCS_led_pin = 2;
static const uint8_t TCS_SDA_pin = 21;
static const uint8_t TCS_SCL_pin = 22;

Adafruit_TCS34725 tcs = Adafruit_TCS34725(TCS34725_INTEGRATIONTIME_50MS, TCS34725_GAIN_1X);

#include "arm_commands.h"
#include "motors_commands.h"
#include "sensors_commands.h"

void setup() {
    rkConfig cfg;
    // Upravte nastavení, například:
    // cfg.motor_max_power_pct = 30; // limit výkonu motorů na 30%
    cfg.motor_max_power_pct = 100;
    cfg.motor_max_ticks_per_second = 8000;
    cfg.motor_max_acceleration = 100000;
    rkSetup(cfg);
    servoBus.begin(1, UART_NUM_1, GPIO_NUM_27);
    servoBus.setAutoStop(0, true);

    Serial.begin(115200);
    Serial.println("start");

    // inicializace RGB
    pinMode(TCS_SDA_pin, PULLUP);
    pinMode(TCS_SCL_pin, PULLUP);
    Wire1.begin(TCS_SDA_pin, TCS_SCL_pin, 100000);
    if (!tcs.begin(TCS34725_ADDRESS, &Wire1)) {
        Serial.println("Can not connect to the RGB sensor");
        trap();
    }
    pinMode(TCS_led_pin, GPIO_MODE_OUTPUT);
    digitalWrite(TCS_led_pin, 1);
    Serial.println("Starting main loop\n");

    //start tlacitko pro kalibraci klepet
    while (true) {
        if (rkButtonIsPressed(BTN_UP)) {
            break;
        }
    }
    // zhasnuti ledek
    for (byte i = 0; i < 8; i++) {
        rkSmartLedsRGB(i, 0, 0, 0);
    }
    //kalibrace klepet
    for (size_t i = 0; i < 3; i++) {
        servoBus.set(0, 140_deg, 200.f, 1.f);
        delay(1000);
        printf("smart servo moved to: %f\n", servoBus.pos(0).deg());
    }
    // start tlacitko
    while (true) {
        if (rkButtonIsPressed(BTN_UP)) {
            break;
        }
    }
    // zapise startovni cas
    startTime = millis();
    printf("startTime: %lu\n", startTime);
    // vypise procenta baterky
    printf("batery percent: %u\n", rkBatteryPercent());
    /////////////////////////////////////
    while (true) {
        // printf("state= %u \n", state);
        delay(20);
        switch (state) {
            case 1:
                state = 2;
                // jizda eskem
                arm_up();
                back_button();
                forward(500);
                state = 3;
                break;
            case 3:
                state = 4;
                // jizda eskem
                curve(150, 180, 5, true);
                break;
            case 5:
                state = 6;
                // jizda eskem
                forward(50);
                state = 7;
                break;
            case 7:
                state = 8;
                // jizda eskem
                curve(150, 150, 9, false);
                break;
            case 9:
                state = 10;
                forward(1750);
                // otocka do hriste
                turn_by_wall();
                for (byte i = 0; i < 5; i++) {
                    // jizda doprostred hriste
                    forward(900 - (k * 100));
                    k++;
                    if (k > 6)
                    {
                        k = 0.5;
                    }
                    if(k = 0){
                        turn(-100);
                    }
                    else{
                    turn(-90);
                    }
                    back_button();
                    // jizda pro kostku
                    //zapnuti ledek
                    for (byte i = 0; i < 8; i++) {
                        rkSmartLedsRGB(i, 255, 255, 255);
                    }
                    arm_down();
                    bool brick;
                    brick = go_for_brick();
                    if (brick) {
                        klepeta_close();
                        arm_up();
                        rkMotorsSetSpeed(-100, -100);
                        //tady se rozhodne na jakou barvu robot pojede
                        rgb_value = rgb_get();
                            if (rgb_value == RED) {
                                go_to_red();
                            } 
                            else if (rgb_value == GREEN) {
                                go_to_green();
                            } 
                            else {
                                go_to_blue();
                            }
                            rkLedAll(false);
                        // jizda zpet ke zdi nakonec eska
                        back_button();
                    } else {
                        i--;
                        k++;
                        back_button();
                        forward(150);
                        turn(90);
                        back_button();
                    }
                    if ((i = 4) && ((millis() - startTime) < FinalTime)) {
                            i--;
                        }
                }
                forward(50);
                turn(80);
                back_button();
                // cesta z rohu eskem zpet
                forward(100);
                turn_by_wall();
                back_button();
                forward(300);
                state = 11;
                break;
            case 11:
                state = 12;
                // musi se dopocitat
                curve(190, 90, 13, true);
                break;
            case 13:
                state = 14;
                forward(100);
                // musi se dopocitat
                curve(140, 180, 15, false);
                break;
            case 15:
                state = 16;
                forward(600);
                state = 17;
                break;
            case 69:
                state = 70;
                back_button();
                break;
        }
    }
    for (byte i = 0; i < 8; i++) {
        rkSmartLedsRGB(i, 255, 255, 255);
    }
    while (true) {
        go_for_brick();
    }
    for (byte i = 0; i < 8; i++) {
        rkSmartLedsRGB(i, 255, 255, 255);
    }
}
void loop() {
    // precteni a poslani RGB
    float r, g, b;
    tcs.getRGB(&r, &g, &b);
    Serial.print(uint8_t(r));
    Serial.print(", ");
    Serial.print(uint8_t(g));
    Serial.print(", ");
    Serial.print(uint8_t(b));
    Serial.print(" || ");
    uint16_t red, green, blue, clear;
    tcs.getRawData(&red, &green, &blue, &clear);
    Serial.print(red);
    Serial.print(", ");
    Serial.print(green);
    Serial.print(", ");
    Serial.print(blue);
    Serial.print(", ");
    Serial.print(clear);
    Serial.print("\n ");
    delay(1000);
}