# LCD812L-B Arduino Library

[![PlatformIO CI](https://github.com/adrianaxente/LCD812L-B/actions/workflows/platformio.yml/badge.svg)](https://github.com/adrianaxente/LCD812L-B/actions/workflows/platformio.yml)

An Arduino library for controlling the 812L-B LCD display module.

## Description

This library provides a simple interface for printing text, numbers, and other data to the 812L-B LCD display. The 812L-B is a 10-digit, 7-segment display that communicates using a simple serial interface.

This library handles the low-level communication with the display, allowing you to easily display data without needing to understand the underlying protocol.

## Features

- Print strings, integers, and floating-point numbers.
- Control the alignment of the displayed text (left or right padded).
- Clear the display.
- Show the time (if the module supports it).
- A built-in timer function.

## Installation

### Arduino IDE

1.  Download the latest release of the library from the [releases page](https://github.com/adrianaxente/LCD812L-B/releases).
2.  In the Arduino IDE, go to `Sketch` > `Include Library` > `Add .ZIP Library...` and select the downloaded file.
3.  You can now use the library in your sketches by including the header file: `#include <LCD812L-B.h>`.

### PlatformIO

1.  Add the library to your `platformio.ini` file:

    ```ini
    lib_deps = adrianaxente/LCD812L-B
    ```

2.  Include the library in your code: `#include <LCD812L-B.h>`.

## Getting Started

Here's a simple example of how to use the library:

```cpp
#include <Arduino.h>
#include <LCD812L-B.h>

// Define the pins connected to the LCD
#define CLK_PIN 2
#define DATA_PIN 3
#define HKS_PIN 4

LCD821LB lcd;

void setup() {
  // Initialize the LCD
  lcd.begin(CLK_PIN, DATA_PIN, HKS_PIN);
  
  // Clear the display
  lcd.clear();
}

void loop() {
  // Print a string
  lcd.print("43llO");
  delay(2000);

  // Print an integer
  lcd.print((signed long long)1234567890);
  delay(2000);

  // Print a double
  lcd.print(3.14159);
  delay(2000);
  
  // Print a right-padded string
  lcd.print("F41l", true);
  delay(2000);
}
```

## API

### `void begin(int clk_p, int data_p, int hks_p)`

Initializes the library with the pins connected to the LCD.

- `clk_p`: The clock pin.
- `data_p`: The data pin.
- `hks_p`: The HKS (Hook Switch) pin.

### `void clear()`

Clears the display.

### `void print(const char *str, bool leftPadded = false)`

Prints a string to the display.

- `str`: The string to print.
- `leftPadded`: If `true`, the string will be right-aligned. Otherwise, it will be left-aligned.

### `void print(signed long long value, bool leftPadded = false)`

Prints a signed long long to the display.

- `value`: The number to print.
- `leftPadded`: If `true`, the number will be right-aligned.

### `void print(double value, byte precision = 3, bool leftPadded = false)`

Prints a double to the display.

- `value`: The number to print.
- `precision`: The number of decimal places to display.
- `leftPadded`: If `true`, the number will be right-aligned.

### `void showClock()`

Switches the display to clock mode. The exact behavior depends on the LCD module.

### `void startTimer()`

Starts a timer or stopwatch. The exact behavior depends on the LCD module.

## Supported Characters

The display can show the following characters:

- `0`, `1`, `2`, `3`, `4`, `5`, `6`, `7`, `8`, `9`
- `F`
- `,`
- `l`
- `P`
- `-`
- ` ` (space)

Any unsupported character will be displayed as `F`.

## Contributing

Contributions are welcome! If you find a bug or have a feature request, please open an issue on the [GitHub repository](https://github.com/adrianaxente/LCD812L-B/issues).

## License

This library is licensed under the MIT License. See the [LICENSE](LICENSE) file for more information.
