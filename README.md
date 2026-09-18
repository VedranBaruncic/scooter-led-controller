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

## Hardware

## Pinout
