#include "robotka.h"

// Funkce setup se zavolá vždy po startu robota.
const int speed = 60;
float ticksToMm = 3.62; // prepocet z tiku v enkoderech na mm
byte state = 1;
float wheel_diameter = 162;
float g_US = 1;

void forward(int mm) { //

    rkMotorsDrive(mm * ticksToMm, mm * ticksToMm, speed);
}

void turn(int degrees) { // + doprava - doleva
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

    while(true){
        if(rkButtonIsPressed(BTN_UP)){
                break;
        }
    }

    //while(true)
    //{
    //    printf("state= %u /n", state);
    //    switch (state)
    //    {
    //    case 1:
    //        state = 2;
    //        forward(500);
    //        state = 3;
    //        break;
    //    case 3:
    //        state = 4;
    //        curve(150, 5, true);
    //        break;
    //    case 5:
    //        state = 6;
    //        forward(50);
    //        state = 7;
    //        break;
    //    case 7:
    //        state = 8;
    //        curve(150, 9, false);
    //        break;
    //    case 9:
    //        state = 10;
    //        forward(1000);
    //        state = 11;
    //        break;
    //    }
    //}
        

    while(true){
        //rkServosSetPosition(1, 90);x
        rkServosSetPosition(2,90);
        delay(2000);
        //rkServosSetPosition(1, -120);
        rkServosSetPosition(2, -120);
        delay(2000);
    }
}
