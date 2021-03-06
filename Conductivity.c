#include    "smartpanipipe.h"

void configure_ADC(){
    COND_SIGNAL_CONFIG = 1;
    OpenADC(ADC_FOSC_16&ADC_RIGHT_JUST&ADC_0_TAD, ADC_CH0&ADC_INT_OFF&ADC_REF_VDD_VSS, 0b1110);
}

unsigned int get_ADC_value(void){
    unsigned int max;
    COND_POWER=1;
    delay_1s(2);
    ConvertADC();                               // start conversion
    while(BusyADC());                           //wait while conversion takes place

    max=ReadADC();
    for (int i=0; i<10000; i++){
        ConvertADC();                           // start conversion
        while(BusyADC());                       //wait while conversion takes place
        unsigned int new = ReadADC();
        if (new >= max){
            max=new;
        }
        delay_1us(1);
    }
    COND_POWER=0;
    delay_1ms(50);
    return max;
}

void getConductivity(){
    float scaled_value;                                             // variable for delay adc scaling
    unsigned int peak_value;                                        // variable for scaled delay value
    float net_voltage;                                     // variable for net voltage

    configure_ADC();                                                // function call to configure adc
    peak_value = get_ADC_value();                                   // gets 10 bit adc value
    scaled_value = (float) peak_value*(float) (5.08f/1023f);        // adc scaled value now scaled and ready
    net_voltage = (scaled_value - 2.46);
    Conductivity = (CELL_CONSTANT*100/(net_voltage - Y_INTERCEPT))/95;       // calculate conductivity value
}