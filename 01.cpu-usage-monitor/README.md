# CPU Usage Monitor

Hardware utility build on top of UNO, used for binary led cpu usage.

## Breadboard layout

![Schematics](https://raw.githubusercontent.com/sleeperor/breadboards/master/01.cpu-usage-monitor/schematics_bb.png)

## Software

Software is divided in two parts: Python script for sending CPU usage data over Serial, and Arduino sketch for UNO board.

### PC Python Script

`read_stats.py` script reads data from /proc/stat and sends over Serial port (/dev/ttyACM0).

### Arduino Sketch

Arduino code reads data from Serial and displays in binary format using 74HC595 Shift Register and 8 red LEDs.
