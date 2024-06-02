# sparkypi satellite

Control LPD433 devices like switch sockets or doorbells via a microcontroller board like the Arduino over UART or USB.

This is the firmware for the microcontroller board, intended to be used in conjunction with e.g. [this](https://crates.io/crates/libsparkypi), but it also works with the serial monitor in the Arduino IDE alone.

## How to compile and upload to the board

Compiling is pretty straightforward. It can be done with the [Arduino IDE 2](https://github.com/arduino/arduino-ide). Alternatively you can of course give the [arduino-cli](https://github.com/arduino/arduino-cli) a try.

1. Clone the repository to a local directory:

    `git clone https://github.com/elkasztano/sparkypi-satellite`

1. Launch the Arduino IDE 2 and open `sparkypi-satellite.ino` in the directory.

1. Make sure your microcontroller board is properly connected, and compile and upload the code from the IDE.

    * _Note:_ If you are working on Linux and have an Arduino Nano Every, you might want to set the Baud rate of the serial monitor to 1200 before uploading, otherwise it might not work.

## What it does

If you send a bit sequence along with additional information in csv format over a serial connection to the microcontroller board, it will create an actual radio signal out of the given information and transmit it over a connected radio module like the `FS1000A`. That way you can for example control mains powered devices via a radio controlled switch socket. The radio signal properties must be formatted in the following way:

`bit-sequence,pulse-length,repeats,short-factor,long-factor,sync-bit,sync-gap`

### bit-sequence

The actual data that is going to be transmitted. Must be an ASCII character sequence. A `'0'` will obviously be interpreted as binary zero, and `'1'` will result in a binary one. Any other character will represent a so-called sync bit.

### pulse-length

This represents the smallest time unit of the radio signal in microseconds. Will be multiplied by `short-factor` and `long-factor` to create the representations of `0` and `1`.

### repeats

Usually LPD433 radio signals must be repeated several times to increase the probability of getting registered by their respective target devices. This number should be as low as reasonably achievable.

### short-factor, long-factor

The representations of `0` and `1` will be generated using the following method:

`pulse-length * short-factor` µsecs HIGH followed by `pulse-length * long-factor` µsecs LOW will be interpreted as `0`.

`pulse-length * long-factor` µsecs HIGH followed by `pulse-length * short-factor` µsecs LOW will be interpreted as `1`.

### sync-bit, sync-gap

In order to stand out from noise and to mark the beginning of each radio transmission so-called sync-bits and sync-gaps must be put at the beginning.

### Example

`s001100100101100101010110,175,5,1,3,1,31`

The sequence `001100100101100101010110` preceded by a sync-bit/sync-gap combination will be transmitted `5` times. A binary 1 is represented by `175 * 3` microseconds HIGH followed by `175 * 1` microseconds LOW, and a binary 0 is represented by `175 * 1` microseconds HIGH followed by `175 * 3` microseconds LOW. The duration of the sync bit and sync gap is calculated accordingly. When the transmission has fully completed, the microcontroller board will give a feedback in json format via serial port for debugging purposes.

The above example has been tested with a simple radio controlled door bell.

### Bonus

The microcontroller board can catch a certain ASCII character sequence at the beginning of the line in order to do something completely different. For example, if you send the sequence `blink` via serial connection to the board, a connected LED will blink and nothing more will happen. If the sequence `reqswitch` is registered, the state of pins 5 and 6 will be sent back over the serial port.

Of course, you can also include your own code. For example, the microcontroller board could read a sensor and send the reading back via serial port after reading a certain keyword.

## Wiring

The following wiring is recommended if the constants in the source code are left unchanged:

* `FS1000A` Data → D10
* `FS1000A` VCC → 3V3
* `FS1000A` GND → GND

Optional:

* RGB-LED Red → D2
* RGB-LED Green → D3
* RGB-LED Blue → D4
* RGB-LED GND → GND

Connecting an LED module like for example the `KY-016` is optional. However, a visual feedback can sometimes be very useful.

Optional:

* Switch 1 → D5
* Switch 2 → D6

Reading the states of pin 5 and 6 is also fully optional. This functionality was added for testing purposes. If you need more information on how to connect a switch to a pin, please refer to [this](https://docs.arduino.cc/built-in-examples/basics/DigitalReadSerial/).

If you don't need the additional functionality at all you can comment out the corresponding preprocessor directives (`#define` ...) in `sparky.h`.

## Baud rate

The Baud rate is set to 115200 by default. It can easily be changed in the source code.

## References

[https://www.arduino.cc/reference/en/libraries/rc-switch/](https://www.arduino.cc/reference/en/libraries/rc-switch/)

[https://en.wikipedia.org/wiki/LPD433](https://en.wikipedia.org/wiki/LPD433)