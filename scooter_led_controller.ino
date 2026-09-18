#define F_CPU 16000000UL

#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include <stdint.h>


#define WS_PORT PORTB
#define WS_DDR  DDRB

#define WS_PIN_1 PB0
#define WS_PIN_2 PB2

#define LED_COUNT_PER_SIDE 15

void adc_init(void)
{
    // Select ADC3 (PB3)
    ADMUX |= (1 << MUX1) | (1 << MUX0);

    // Enable ADC
    // ADC clock = 16 MHz / 128 = 125 kHz
    ADCSRA =
        (1 << ADEN)  | (1 << ADPS2) | (1 << ADPS1) | (1 << ADPS0);

    // Disable digital input on PB3
    DIDR0 |= (1 << ADC3D);
}


uint16_t adc_read(void)
{
    // Start ADC conversion
    ADCSRA |= (1 << ADSC);

    // Wait until conversion is finished
    while (ADCSRA & (1 << ADSC))
    {
    }

    // Return ADC value: 0–1023
    return ADC;
}


// ============================================================
// WS2812B - SEND ONE BIT
// ============================================================
//
// WS2812B uses different timing for:
//s
// 1 = longer HIGH(800ns), shorter LOW(450ns)
// 0 = shorter HIGH(400ns), longer LOW(850ns)
//
// The timing-critical part is written in Assembly, ensuring very accurate time control

void sendBit(uint8_t bit, uint8_t pin){
  if(pin == WS_PIN_1){
    if (bit)
    {
        // Send logical 1

        asm volatile(
            "sbi %[port], 0\n\t"

            // HIGH time
            "nop\n\t"
            "nop\n\t"
            "nop\n\t"
            "nop\n\t"
            "nop\n\t"
            "nop\n\t"
            "nop\n\t"
            "nop\n\t"

            // LOW starts here
            "cbi %[port], 0\n\t"

            // LOW time
            "nop\n\t"
            "nop\n\t"
            "nop\n\t"
            "nop\n\t"
            "nop\n\t"

            :
            : [port] "I" (_SFR_IO_ADDR(WS_PORT))
        );
    }
    else
    {
        // Send logical 0

        asm volatile(
            "sbi %[port], 0\n\t" // Set bit in I/O Register

            // Short HIGH time
            "nop\n\t" // No operation
            "nop\n\t"

            // LOW starts here
            "cbi %[port], 0\n\t" // Clear bit in I/O Register

            // Long LOW time
            "nop\n\t"
            "nop\n\t"
            "nop\n\t"
            "nop\n\t"
            "nop\n\t"
            "nop\n\t"
            "nop\n\t"
            "nop\n\t"
            "nop\n\t"
            "nop\n\t"

            :
            : [port] "I" (_SFR_IO_ADDR(WS_PORT)) // Get I/O address of PORTB register
        );
    }
  }
  else{
    if (bit)
    {
        // Send logical 1

        asm volatile(
            "sbi %[port], 2\n\t"

            // HIGH time
            "nop\n\t"
            "nop\n\t"
            "nop\n\t"
            "nop\n\t"
            "nop\n\t"
            "nop\n\t"
            "nop\n\t"
            "nop\n\t"

            // LOW starts here
            "cbi %[port], 2\n\t"

            // LOW time
            "nop\n\t"
            "nop\n\t"
            "nop\n\t"
            "nop\n\t"
            "nop\n\t"

            :
            : [port] "I" (_SFR_IO_ADDR(WS_PORT))
        );
    }
    else
    {
        // Send logical 0

        asm volatile(
            "sbi %[port], 2\n\t" // Set bit in I/O Register

            // Short HIGH time
            "nop\n\t" // No operation
            "nop\n\t"

            // LOW starts here
            "cbi %[port], 2\n\t" // Clear bit in I/O Register

            // Long LOW time
            "nop\n\t"
            "nop\n\t"
            "nop\n\t"
            "nop\n\t"
            "nop\n\t"
            "nop\n\t"
            "nop\n\t"
            "nop\n\t"
            "nop\n\t"
            "nop\n\t"

            :
            : [port] "I" (_SFR_IO_ADDR(WS_PORT)) // Get I/O address of PORTB register
        );
    }
  }
}


// Send a byte of data to WS2812B

void sendByte(uint8_t data, uint8_t pin)
{
    for (uint8_t i = 0; i < 8; i++)
    {
        // Send the highest bit
        sendBit(data & 0x80, pin);

        // Move the next bit into the highest position
        data = data << 1;
    }
}



// WS2812B color order is GRB, not RGB, so we add the sendLED function just to make it more user-friendly

void sendLED(uint8_t red, uint8_t green, uint8_t blue, uint8_t pin)
{
    sendByte(green, pin);
    sendByte(red, pin);
    sendByte(blue, pin);
}



// brightness = 0   -> LEDs off
// brightness = 255 -> full brightness

void setAllLEDs(uint8_t brightness)
{
    uint8_t red   = 255;
    uint8_t green = 20;
    uint8_t blue  = 147;

    // Scaling according to brightness
    // 255 * brightness / 256

    uint8_t r = ((uint16_t)red   * brightness) >> 8;
    uint8_t g = ((uint16_t)green * brightness) >> 8;
    uint8_t b = ((uint16_t)blue  * brightness) >> 8;


    // Disable interrupts because WS2812B timing must not be interrupted
    cli();

    for (uint8_t i = 0; i < LED_COUNT_PER_SIDE; i++){
        sendLED(r, g, b, WS_PIN_1);
    }

    for (uint8_t i = 0; i < LED_COUNT_PER_SIDE; i++){
      sendLED(r, g, b, WS_PIN_2);
    }

    // Start interrupts again
    sei();


    // WS2812B latch
    _delay_us(100);
}



int main(void)
{

    WS_DDR |= (1 << WS_PIN_1) | (1 << WS_PIN_2);
    WS_PORT &= ~((1 << WS_PIN_1) | (1 << WS_PIN_2));

    adc_init();


    // Main loop
    while (1)
    {
        // Read potentiometer
        uint16_t adc_value = adc_read();


        // Convert 10-bit ADC value to 8-bit brightness
        uint8_t brightness = adc_value >> 2; // Shifting right by 2 basically divides the 10-bit value by 2²(4), so we will get an 8-bit value


        // Update LEDs
        setAllLEDs(brightness);
    }
}