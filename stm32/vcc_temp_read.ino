const uint8_t led = PC13;

enum { LED_ON=0, LED_OFF=1 }; // for Blue Pill .. active low led

// Reading Vdd and Temperature Sensor
// Pito 8/2016
// Temperature sensor at ADC16, VREFINT at ADC17
// BluePill and Maple Mini

void setup_vdd_tempr_sensor() {
    adc_reg_map *regs = ADC1->regs;
    regs->CR2 |= ADC_CR2_TSVREFE;    // enable VREFINT and Temperature sensor
    // sample rate for VREFINT ADC channel and for Temperature sensor
    regs->SMPR1 |=  (0b111 << 18);  // sample rate temperature
    regs->SMPR1 |=  (0b111 << 21);  // sample rate vrefint
    adc_calibrate(ADC1);
}

void setup(){
    setup_vdd_tempr_sensor();
    Serial.begin(115200);
    pinMode(led,OUTPUT);
    Serial.println("\r\nPrint Internal VCC Voltage");
}

void loop() {
    float tempr, vdd;

    digitalWrite(led,LED_ON);
    delay(50);
    // reading Vdd by utilising the internal 1.20V VREF
    vdd = 1.20 * 4096.0 / adc_read(ADC1, 17);
    // following 1.43 and 0.0043 parameters come from F103 datasheet - ch. 5.9.13
    // and need to be calibrated for every chip (large fab parameters variance)
    tempr = (1.43 - (vdd / 4096.0 * adc_read(ADC1, 16))) / 0.0043 + 25.0;
    Serial.print("Vdd=  ");
    Serial.print(vdd);
    Serial.println(" V");
    Serial.print("Temp= ");
    Serial.print(tempr);
    Serial.println(" C");

    digitalWrite(led,LED_OFF);
    delay(500);
}

