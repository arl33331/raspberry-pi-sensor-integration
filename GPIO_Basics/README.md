
# GPIO Basics – Raspberry Pi 4

This directory contains four introductory GPIO programs written in C using the libgpiod library.

## Hardware and Software

- Board: Raspberry Pi 4 Model B (4 GB)
- Operating System: Raspberry Pi OS 64-bit
- Programming Language: C
- Compiler: GCC
- GPIO Library: libgpiod

## Programs

| Source File | Description | GPIO |
|---|---|---|
| gpio_led_on.c | Turns an LED ON | GPIO17 |
| gpio_led_off.c | Turns an LED OFF | GPIO17 |
| gpio_led_blink.c | Blinks an LED | GPIO17 |
| gpio_read_input.c | Reads an external digital input | GPIO27 |

## Compilation

Compile using GCC and link with libgpiod:

```bash
gcc -Wall -Wextra gpio_led_on.c -o gpio_led_on -lgpiod
```

The same procedure applies to the other programs, replacing the source and executable names.

## Learning Objectives

1. Configure a GPIO pin as an output.
2. Set a GPIO output HIGH or LOW.
3. Generate a blinking signal.
4. Configure a GPIO pin as an input.
5. Read the digital state of an external input.

## Project Notes

These programs are introductory examples for learning direct GPIO control in C without using Arduino libraries.