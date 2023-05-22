using namespace lx16a;
static SmartServoBus servoBus;



// otestuje funkcnost ramene
void test()
{
    rkMotorsSetSpeed(100, 100);
    delay(5000);
    rkMotorsSetSpeed(0, 0);
    rkServosSetPosition(3, -90);
    delay(1000);
    rkServosSetPosition(3, 90);
    delay(1000);
    rkMotorsSetSpeed(-50, -50);
    while (true)
    {
        if (rkButtonIsPressed(BTN_RIGHT))
        {
            break;
        }
    }
    delay(500);
    rkMotorsSetSpeed(0, 0);
    rkMotorsSetSpeed(100, 100);
    delay(5000);
    rkMotorsSetSpeed(0, 0);
}
// nastavi pozici ramene a klepet na chytani kostek
void arm_down()
{
    rkServosSetPosition(1, -90);
    printf("arm in pick mode, angle:%f\n", rkServosGetPosition(1));
    servoBus.set(0, 30_deg, 200.f, 1.f);
    printf("smart servo moved to: %f\n", servoBus.pos(0).deg());
}
// nastavi pozici ramene na jizdu a zavre klepeta
void arm_up()
{
    //servoBus.set(0, -30_deg, 100.f, 1.f);
    //printf("smart servo moved to: %f\n", servoBus.pos(0).deg());
    rkServosSetPosition(2, 0);
    printf("arm in drive mode, angle:%f\n", rkServosGetPosition(1));
    delay(1000);
}
// nastavi pozici ramene na vylozeni kostky a otevre klepeta
void arm_back()
{
    rkServosSetPosition(1, 90);
    printf("arm in delivery mode, angle:%f\n", rkServosGetPosition(1));
    servoBus.set(0, -30_deg, 200.f, 1.f);
    printf("smart servo moved to: %f\n", servoBus.pos(0).deg());
}

