#include "robotka.h"

const int speed = 30;
float ticksToMm = 3.62; // prepocet z tiku v enkoderech na mm 
byte state = 1;
float rozpeti_kol = 165;
float g_US = 1;


void forward(int mm){ // 
    rkConfig cfg;
    rkSetup(cfg); 
        rkMotorsDrive(mm*ticksToMm, mm*ticksToMm, speed);
}

void turn(int degrees){ // + doprava - doleva
    rkConfig cfg;
    rkSetup(cfg);
        rkMotorsDrive(3.141*rozpeti_kol*degrees/360*ticksToMm, -3.141*rozpeti_kol*degrees/360*ticksToMm, speed);
}

void encodery(){
    Serial.printf("L: %f, R: %f",rkMotorsGetPositionLeft(), rkMotorsGetPositionRight() );
    // rkMotorsGetPositionLeft();
    // rkMotorsGetPositionRight();

}
void Skoback(){
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

void Sko(){
    forward(850);
    turn(90);
    forward(450);
    turn(90);
    forward(500);
    turn(-90);
    forward(500);
    turn(-90);
    forward(1000); 
}

void update_sensors(){
    g_US = rkUltraMeasure(1);
    printf("n/ g_US: %f /n", g_US);
    std::cout << " " << std::endl;
}

void setup() {
    rkConfig cfg;
    // Upravte nastavení, například:
    // cfg.motor_max_power_pct = 30; // limit výkonu motorů na 30%
    rkSetup(cfg);   
           
    Serial.begin(115200);
            
            
    while(true){
        if(rkButtonIsPressed(BTN_UP)){
                break;
        }
    }
    Sko();
    turn(180);
    Skoback();
}