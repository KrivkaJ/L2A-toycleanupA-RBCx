#include <Arduino.h>
#include "SmartServoBus.hpp"
#include "robotka.h"
#include "arm_commands.h"


// Funkce setup se zavolá vždy po startu robota.
const int speed = 60;
float ticksToMm = 3.62; // prepocet z tiku v enkoderech na mm
byte state = 1;
float wheel_diameter = 162;
float g_US = 1;

void forward(int mm) { //

    rkMotorsDrive(mm * ticksToMm, mm * ticksToMm, speed);
}

// + doprava - doleva
void turn(int degrees) { 
    rkMotorsDrive(3.141 * wheel_diameter * degrees / 360 * ticksToMm, -3.141 * wheel_diameter * degrees / 360 * ticksToMm, speed);
}

void curve(int radius, byte new_state, bool right){
    int sR = radius/radius * speed;
    int sL = (radius + wheel_diameter) / radius * speed;
    if (right)
    {
    rkMotorsDriveRightAsync(ticksToMm * radius * PI, sR, [&, new_state](){state = new_state;});
    rkMotorsDriveLeftAsync(ticksToMm * (radius + wheel_diameter) * PI, sL);
    }
    else{
    rkMotorsDriveLeftAsync(ticksToMm * radius * PI, sR, [&, new_state](){state = new_state;});
    rkMotorsDriveRightAsync(ticksToMm * (radius + wheel_diameter) * PI, sL);
    }
}
void curve_back(int radius, byte new_state, bool right){
    int sR = radius/radius * speed;
    int sL = (radius - wheel_diameter) / radius * speed;
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

void Sko() {
    forward(500);
    curve(150, 1, true);
    forward(300);
    curve(150, 1, false);
    forward(1000);
}

void update_sensors() {
    g_US = rkUltraMeasure(1);
    printf("n/ g_US: %f /n", g_US);
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

void setup() {
    rkConfig cfg;
    // Upravte nastavení, například:
    // cfg.motor_max_power_pct = 30; // limit výkonu motorů na 30%
    rkSetup(cfg);
    servoBus.begin(1, UART_NUM_1, GPIO_NUM_27);
    servoBus.setAutoStop(0, true);

    while(true){
        if(rkButtonIsPressed(BTN_UP)){
                break;
        }
    }

    while(true)
    {
        printf("state= %u /n", state);
        switch (state)
        {
        case 1:
            state = 2;
            //forward(500);
            curve_back(150, 3, true);
            //state = 3;
            break;
        case 3:
            state = 4;
            curve(150, 5, true);
            break;
        case 5:
            state = 6;
            forward(50);
            state = 7;
            break;
        case 7:
            state = 8;
            curve(150, 9, false);
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
            curve(-150, 17, false);
            break;
        case 17:
            state = 18;
            forward(-50);
            state = 19;
            break;
        case 19:
            state = 20;
            curve(-150, 21, true);
            break;
        case 21:
            state = 22;
            forward(-500);
            state = 23;
            break;     
        }
    }
        
        //rkServosSetPosition(1, 90);x
        //rkServosSetPosition(2, 0);
        //delay(2000);
        //while (true)
        //{       
        //servoBus.set(0, 240_deg, 200.f, 1.f);
        //delay(2000);
        //servoBus.set(0, 0_deg, 200.f, 1.f);
        //delay(2000);
        //}
}
