const int speed = 100;
const float ticksToMm = 3.62; // prepocet z tiku v enkoderech na mm
const float wheel_diameter = 162;

//jizda rovne o zadanou vzdalenost 
void forward(int mm)
{
    rkMotorsDrive(mm * ticksToMm, mm * ticksToMm, speed);
}

//zatoceni na miste o zadany pocet stupnu
// + doprava - doleva
void turn(int degrees)
{
    rkMotorsDrive(3.141 * wheel_diameter * degrees / 360 * ticksToMm, -3.141 * wheel_diameter * degrees / 360 * ticksToMm, speed);
}

//oblouk
// tue=doprava, false=doleva
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

//oblouk dozadu 
void curve_back(int radius, bool right)
{
    int sR = radius / radius * 40;
    int sL = (radius + wheel_diameter) / radius * 40;
    printf("sR: %i, sL: %i\n", sR, sL);
    if (right)
    {
        rkMotorsDriveRightAsync(-ticksToMm * radius * PI, sR);
        rkMotorsDriveLeftAsync(-ticksToMm * (radius + wheel_diameter) * PI, sL);
    }
    else
    {
        rkMotorsDriveLeftAsync(-ticksToMm * radius * PI, sR);
        rkMotorsDriveRightAsync(-ticksToMm * (radius + wheel_diameter) * PI, sL);
    }
}

// robot couva dokud nenarazi na zadni tlacitko
void back_button()
{
    rkMotorsSetSpeed(-100, -100);
    while (true)
    {
        if (rkButtonIsPressed(BTN_RIGHT))
        {
            break;
        }
    }
    delay(500);
    rkMotorsSetSpeed(0, 0);
}

// otocka robota ode zdi o 90 stupnu doleva
void turn_by_wall()
{
    rkServosSetPosition(3, -90);
    delay(1000);
    rkServosSetPosition(3, 90);
    delay(1000);
    rkServosSetPosition(3, -90);
    delay(1000);
    rkServosSetPosition(3, 90);
    delay(1000);
    back_button();
}

uint16_t red, green, blue, clear;

void go_for_brick(){
    byte timer = 0;
    rkMotorsSetSpeed(50, 50);
    while((clear < 200) && (timer <= 15)){
        timer += 1;
        tcs.getRawData(&red, &green, &blue, &clear);
        printf("clear: %hu, timer: %hhu\n", clear, timer);
        delay(200);
    }
    rkMotorsSetSpeed(0, 0);
    if (clear < 200)
    {
        printf("found by rgb senzor\n");
    }
    else
    {
        printf("stoped by timer\n");
    }
    
}
//robot jede na cervene pole
void go_to_red(){
//sevre klepeta
arm_up();
back_button();
forward(150);
turn(-90);
forward(-450);//doladit
turn(90);
back_button();
arm_back();
forward(30);
klepeta_open();
arm_up();
forward(200);
turn(-90);
forward(500);
turn(180);
}

void go_to_green(){
//sevre klepeta
arm_up();
back_button();
arm_back();
forward(30);
klepeta_open();
arm_up();
forward(200);
turn(-90);
}


void go_to_blue(){
//sevre klepeta
arm_up();
back_button();
forward(150);
turn(90);
forward(-450);//doladit
turn(-90);
back_button();
arm_back();
forward(30);
klepeta_open();
arm_up();
forward(200);
turn(-90);
}
