using namespace lx16a;
static SmartServoBus servoBus;
// otestuje funkcnost ramene
void arm_test()
{
    
    printf("servo moved to:%f\n", rkServosGetPosition(1));
    servoBus.set(0, 90_deg, 200.f, 1.f);
    delay(1500);
  
   
}
// nastavi pozici ramene a klepet na chytani kostek
void arm_pickmode()
{
    rkServosSetPosition(1, -90);
    printf("arm in pick mode, angle:%f\n", rkServosGetPosition(1));
    servoBus.set(0, 30_deg, 200.f, 1.f);
    printf("smart servo moved to: %f\n", servoBus.pos(0).deg());
}
// nastavi pozici ramene na jizdu a zavre klepeta
void arm_drivemode()
{
    servoBus.set(0, -30_deg, 100.f, 1.f);
    printf("smart servo moved to: %f\n", servoBus.pos(0).deg());
    rkServosSetPosition(1, -90);
    printf("arm in drive mode, angle:%f\n", rkServosGetPosition(1));
    
}
// nastavi pozici ramene na vylozeni kostky a otevre klepeta
void arm_deliverymode()
{
    rkServosSetPosition(1, 90);
    printf("arm in delivery mode, angle:%f\n", rkServosGetPosition(1));
    servoBus.set(0, -30_deg, 200.f, 1.f);
    printf("smart servo moved to: %f\n", servoBus.pos(0).deg());
}