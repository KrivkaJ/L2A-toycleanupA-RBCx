#include <Arduino.h>
#include "SmartServoBus.hpp"
#include "robotka.h"
#include "Adafruit_TCS34725.h"

byte state = 1;


// Funkce setup se zavolá vždy po startu robota.
float g_US = 1;

typedef enum {
    RED,
    GREEN,
    BLUE
}RGB;

RGB rgb_value;
// pokud se nepovede neco inicializovat (RGB senzor), program se zasekne v teto funkci
void trap()
{
    Serial.println("trap\n");
    while(1);
}

static const uint8_t TCS_led_pin = 2;
static const uint8_t TCS_SDA_pin = 21;
static const uint8_t TCS_SCL_pin = 22;

Adafruit_TCS34725 tcs = Adafruit_TCS34725(TCS34725_INTEGRATIONTIME_50MS, TCS34725_GAIN_1X);

#include "arm_commands.h"
#include "motors_commands.h"

void encodery() {
    Serial.printf("L: %f, R: %f\n", rkMotorsGetPositionLeft(), rkMotorsGetPositionRight());
    // rkMotorsGetPositionLeft();
    // rkMotorsGetPositionRight();
}

void update_sensors() {
    g_US = rkUltraMeasure(1);
    printf(" g_US: %f \n", g_US);
    //std::cout << " " << std::endl;
}

RGB get_rgb(){
    float r, g, b;
    tcs.getRGB(&r, &g, &b);
    int red[10], green[10], blue[10];
    for (size_t i = 0; i < 10; i++)
    {
        tcs.getRGB(&r, &g, &b);
        red[i] = r;
        green[i] = g;
        blue[i] = b;
        delay(100);
    }
    int sum_red = 0, sum_green = 0, sum_blue = 0;
    for (size_t i = 0; i < 10; i++)
    {
        sum_red += red[i];
        sum_green += green[i];
        sum_blue += blue[i];
    }
    byte red_avg = sum_red / 10;
    byte green_avg = sum_green / 10;
    byte blue_avg = sum_blue / 10;

    if (red_avg > green_avg && red_avg > blue_avg)
    {
        rgb_value = RED;
        return RED;
    }
    else if (blue_avg > red_avg && blue_avg > green_avg)
    {
        rgb_value = BLUE;
        return BLUE;
    }
    else{
        rgb_value = GREEN;
        return GREEN;
    }
}

void setup() {
    rkConfig cfg;
    // Upravte nastavení, například:
    // cfg.motor_max_power_pct = 30; // limit výkonu motorů na 30%
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

    while(true){
        if(rkButtonIsPressed(BTN_UP)){
                break;
        }
    }
printf("batery percent: %u\n", rkBatteryPercent());
    /////////////////////////////////////
    while (false)
    {
        // printf("state= %u \n", state);
        delay(20);
        switch (state)
        {
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
            // jizda doprostred hriste
            forward(550);
            turn(-90);
            back_button();
            // jizda pro kostku
            arm_down();
            go_for_brick();
            //tady se rozhodne na jakou barvu robot pojede
            rgb_value = get_rgb();
            if (rgb_value = RED)
            {
                go_to_red();
            }
            else if (rgb_value = GREEN)
            {
                go_to_green();
            }
            else
            {
                go_to_blue();
            }
            // jizda zpet ke zdi nakonec eska
            forward(220);
            turn(93);
            back_button();
            forward(50);
            turn(75);
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
            curve(150, 90, 13, true);
            break;
        case 13:
            state = 14;
            // musi se dopocitat
            curve(150, 180, 15, true);
            break;
        case 15:
            state = 16;
            forward(500);
            state = 17;
            break;
        }
    }
    while(true){
        //rgb_value = get_rgb();
        //printf("barva: %d \n", rgb_value);
        //delay(1000);
        //servoBus.set(0, 240_deg, 200.f, 1.f);
        //delay(2000);   
        for (size_t i = 0; i < 3; i++)
        {
        printf("smart servo moved to: %f\n", servoBus.pos(0).deg());
        servoBus.set(0, 160_deg, 200.f, 1.f);
        delay(2000);
        }
        for (size_t i = 0; i < 3; i++)
        {
        printf("smart servo moved to: %f\n", servoBus.pos(0).deg());
        servoBus.set(0, 115_deg, 200.f, 1.f);
        delay(2000);
        }
    }
}
void loop() {
    // precteni a poslani RGB
    float r, g, b;
    tcs.getRGB(&r, &g, &b);
    Serial.print(uint8_t(r)); Serial.print(", "); Serial.print(uint8_t(g)); Serial.print(", "); Serial.print(uint8_t(b));Serial.print(" || "); 
    uint16_t red, green, blue, clear;
    tcs.getRawData(&red, &green, &blue, &clear);
    Serial.print(red); Serial.print(", "); Serial.print(green); Serial.print(", "); Serial.print(blue); Serial.print(", "); 
    Serial.print(clear); Serial.print("\n "); 
    delay(1000);
}