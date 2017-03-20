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
}

float readVdd() {
    return 1.20 * 4096.0 / adc_read(ADC1, 17);
}

float readCputemp( vdd ) {
    // following 1.43 and 0.0043 parameters come from F103 datasheet - ch. 5.9.13
    // and need to be calibrated for every chip (large fab parameters variance)
    float tempr;
    tempr = (1.43 - (vdd / 4096.0 * adc_read(ADC1, 16))) / 0.0043 + 25.0;

    return tempr
}

float readAdc( inputPin ) {
    return adc_read( ADC1, inputPin ) * 3.3 / 4096.0;
}

void loop() {

    digitalWrite( led, !digitalRead( led ) );

    char c = Serial.read();

    if( c == 'm' ) {
        float tempr, vdd, ph, temp, humidity;

        vdd = readVdd();
        tempr = readCputemp( vdd );
        ph = readAdc( PIN_PH );
        temp = readAdc( PIN_TEMP );
        humidity = readAdc( PIN_TEMP );
        
        Serial.print( temp );
        Serial.print( ',' );
        Serial.print( humidity );
        Serial.print( ',' );
        Serial.print( ph );
        Serial.print( ',' );
        Serial.print( vdd );
        Serial.print( ',' );
        Serial.println( tempr );

    } else {
        Serial.print( "Unknown command: " );
        Serial.println( c );
    }
}

