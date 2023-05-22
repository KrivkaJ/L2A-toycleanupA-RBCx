#include <Arduino.h>
#include "SmartServoBus.hpp"
#include "robotka.h"
#include "arm_commands.h"
#include "Adafruit_TCS34725.h"


// Funkce setup se zavolá vždy po startu robota.
const int speed = 60;
float ticksToMm = 3.62; // prepocet z tiku v enkoderech na mm
byte state = 1;
float wheel_diameter = 162;
float g_US = 1;

typedef enum {
    RED,
    GREEN,
    BLUE
}RGB;
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


void forward(int mm) { //

    rkMotorsDrive(mm * ticksToMm, mm * ticksToMm, speed);
}

// + doprava - doleva
void turn(int degrees) { 
    rkMotorsDrive(3.141 * wheel_diameter * degrees / 360 * ticksToMm, -3.141 * wheel_diameter * degrees / 360 * ticksToMm, speed);
}

void curve(int radius, int degrees, byte new_state, bool right){
    int sR = radius/radius * 40;
    int sL = (radius + wheel_diameter) / radius * 40;
    printf("sR: %i, sL: %i\n", sR, sL);
    if (right)
    {
    rkMotorsDriveRightAsync((ticksToMm * radius * PI * degrees)/180, sR, [&, new_state](){state = new_state;});
    rkMotorsDriveLeftAsync((ticksToMm * (radius + wheel_diameter) * PI * degrees)/180, sL);
    }
    else{
    rkMotorsDriveLeftAsync((ticksToMm * radius * PI * degrees)/180, sR, [&, new_state](){state = new_state;});
    rkMotorsDriveRightAsync((ticksToMm * (radius + wheel_diameter) * PI * degrees)/180, sL);
    }
}
void curve_back(int radius, byte new_state, bool right){
    int sR = radius/radius * 40;
    int sL = (radius + wheel_diameter) / radius * 40;
    printf("sR: %i, sL: %i\n", sR, sL);
    if (right)
    {
    rkMotorsDriveRightAsync(-ticksToMm * radius * PI, sR, [&, new_state](){state = new_state;});
    rkMotorsDriveLeftAsync(-ticksToMm * (radius + wheel_diameter) * PI, sL);
    }
    else{
    rkMotorsDriveLeftAsync(-ticksToMm * radius * PI, sR, [&, new_state](){state = new_state;});
    rkMotorsDriveRightAsync(-ticksToMm * (radius + wheel_diameter) * PI, sL);
    }
}

void encodery() {
    Serial.printf("L: %f, R: %f", rkMotorsGetPositionLeft(), rkMotorsGetPositionRight());
    // rkMotorsGetPositionLeft();
    // rkMotorsGetPositionRight();
}
void Skoback() {
    forward(1000);
    turn(90);
    forward(450);
    turn(90);
    forward(500);
    turn(-90);
    forward(400);
    turn(-90);
    forward(850);
}

void update_sensors() {
    g_US = rkUltraMeasure(1);
    printf(" g_US: %f \n", g_US);
    //std::cout << " " << std::endl;
}
void Sko_reversed() {
    forward(1000);
    turn(90);
    forward(500);
    turn(90);
    forward(500);
    turn(-90);
    forward(450);
    turn(-90);
    forward(850);
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
        delay(200);
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
        return RED;
    }
    else if (blue_avg > red_avg && blue_avg > green_avg)
    {
        return BLUE;
    }
    else{
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

    while(false)
    {
        //printf("state= %u \n", state);
        delay(20);
        switch (state)
        {
        case 1:
            state = 2;
            forward(500);
            state = 3;
            break;
        case 3:
            state = 4;
            curve(150, 180, 5, true);
            break;
        case 5:
            state = 6;
            forward(50);
            state = 7;
            break;
        case 7:
            state = 8;
            curve(150, 150, 9, false);
            break;
        case 9:
            state = 10;
            forward(1000);
            state = 11;
            break;
        case 11:
            state = 12;
            state = 13;
            break;
        case 13:
            state = 14;
            forward(-1000);
            state = 15;
            break;
        case 15:
            state = 16;
            curve_back(150, 17, false);
            break;
        case 17:
            state = 18;
            forward(-50);
            state = 19;
            break;
        case 19:
            state = 20;
            curve_back(130, 21, true);
            break;
        case 21:
            state = 22;
            forward(-500);
            state = 23;
            break;     
        case 23:
            state = 24;
            rkServosSetPosition(3,0);
            delay(1000);
            state = 25;
            break;
        case 26:
            state = 27;
            rkServosSetPosition(3, 90);
            delay(1000);
            state = 28;
            break;
        case 28:
            state = 29;
            rkServosSetPosition(3, -90);
            delay(1000);
            state = 30;
            break;
        }
    }

    int rgb_value;
    while(true){
        rgb_value = get_rgb();
        printf("barva: %d \n", rgb_value);
        delay(1000);
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