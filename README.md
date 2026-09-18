# Scooter LED Controller

## Overview
The Scooter LED Controller is a personal embedded-systems project designed to control a 30-LED WS2812B lighting system installed on the deck of a scooter.

The project is primarily a learning and engineering project rather than a solution to a specific real-world problem. It was built to gain practical experience with AVR microcontrollers, ADCs, timing-critical communication, and low-level firmware development.

The controller is based on the ATtiny45 microcontroller and allows the user to turn the LEDs on or off and adjust their brightness using a potentiometer. The WS2812B communication is implemented directly using AVR instructions and inline assembly rather than relying on a high-level LED library. This approach was chosen to both keep the firmware lightweight for the ATtiny45's limited memory and to provide precise control over the timing required by the WS2812B protocol.

ATtiny45 specifications: 
- 4 KB flash 
- 256 B SRAM 
- 256 B EEPROM

## Features
- 30 x WS2812B LEDs arranged as two 15-LED strips
- Two independent WS2812B data lines
- Potentiometer-controlled brightness
- 10-bit ADC input mapped to an 8-bit brightness value
- Direct AVR register manipulation
- Custom WS2812B communication using inline AVR assembly
- Solid-color LED output in the current firmware
  
## Hardware
### LED Configuration
The lighting system consists of two 0.5m WS2812B strips, each containing 15 LEDs at a density of 30 LEDs/m. The two strips are installed along the sides of the scooter deck, resulting in 30 LEDs in total.

The choice of 30 LEDs was based on the intended use of the system. Higher LED densities can make a significant visual difference when LED strips are viewed directly, such as in room lighting. However, for this project the LEDs are mounted underneath the scooter and are primarily intended to illuminate the ground while riding. Therefore, increasing the density beyond 30 LEDs/m would provide very small visual benefits while increasing power consumption and system complexity.

Each strip has its own dedicated data line from the ATtiny45. The current firmware sends the same solid color to both strips, but keeping the data lines independent leaves the system open to future improvements, like patterns, animations, or independent control of the two sides without requiring changes to the physical LED wiring.

## Pinout
