typedef enum {
    RED,
    GREEN,
    BLUE
}RGB;

RGB rgb_value;

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

RGB rgb_get(){
    float r, g, b;
    tcs.getRGB(&r, &g, &b);
    int red[10], green[10], blue[10];
    for (size_t i = 0; i < 10; i++)
    {
        tcs.getRGB(&r, &g, &b);
        red[i] = r;
        green[i] = g;
        blue[i] = b;
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
        printf("RED\n");
        return RED;
    }
    else if (blue_avg > red_avg && blue_avg > green_avg)
    {
        rgb_value = BLUE;
        printf("BLUE\n");
        return BLUE;
    }
    else{
        rgb_value = GREEN;
        printf("GREEN\n");
        return GREEN;
    }
}