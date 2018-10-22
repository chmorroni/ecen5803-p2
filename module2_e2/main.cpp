#include "mbed.h"

#define SPEAKER_PIN (PB_10)
#define FREQ_POT_PIN (PA_0)
#define AMP_POT_PIN (PA_1)

#define THRESHOLD (0.05f)
#define PWM_SET_INTERVAL_S (0.1f)
#define POT_PRINT_INTERVAL_S (1.0f)
#define MIN_PERIOD_US (125)
#define MAX_PERIOD_US (1500)

PwmOut speaker(SPEAKER_PIN);
AnalogIn freq_pot(FREQ_POT_PIN);
AnalogIn amp_pot(AMP_POT_PIN);

Ticker tick;
Serial pc(SERIAL_TX, SERIAL_RX);

uint32_t calc_period_us(float pot_val)
{
    return (MAX_PERIOD_US - MIN_PERIOD_US) * pot_val - MIN_PERIOD_US;
}

void pwm_set()
{
    static float freq_pot_val_last = 0, amp_pot_val_last = 0;
    
    float freq_pot_val = freq_pot.read();
    float amp_pot_val = amp_pot.read();
    
    if( abs(freq_pot_val - freq_pot_val_last) > THRESHOLD )
    {
        float period = calc_period_us(freq_pot_val);
        speaker.period_us(period);
    }
        
    if( abs(amp_pot_val - amp_pot_val_last) > THRESHOLD )
    {
        speaker.write(amp_pot_val);
    }
    
    freq_pot_val_last = freq_pot_val;
    amp_pot_val_last = amp_pot_val;
}

void pot_print()
{
    float freq_pot_val = freq_pot.read();
    float amp_pot_val = amp_pot.read();
    pc.printf("Frequency pot: %.6f, Amplitude pot: %.6f\r\n", freq_pot_val, amp_pot_val);
}

int main()
{
    tick.attach(pwm_set, PWM_SET_INTERVAL_S);
    tick.attach(pot_print, POT_PRINT_INTERVAL_S);
    
    while(1);
}
