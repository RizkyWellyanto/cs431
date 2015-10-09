#include "Debouncer.h"

void button_init(Debouncer * button)
{
    button->samples[NUM_SAMPLES] = {0};
    button->sampleIdx = 0;
    button->debouncedValue = 0;
    button->stable = 0;
}

void button_read(Debouncer * button, uint8_t Port_Reading)
{
    
    button->samples[button->sampleIdx] = Port_Reading;
    
    if (++button->sampleIdx >= NUM_SAMPLES - 1)
        button->sampleIdx = 0;
}

uint8_t button_debounced(Debouncer* button) 
{
    uint8_t previousDebouncedValue = button->debouncedValue;
    button->stable = 1;
    uint8_t i = 1;
    for(; i < NUM_SAMPLES; ++i)
    {
        if (button->samples[0] ^ button->samples[i] == 1)
        {
            button->stable = 0;
            return previousDebouncedValue;
        }
    }
    
    return button->debouncedValue = button->samples[0];
}
