# alarmclock
### What is CENG 299?
CENG 299 is a second year project course at the University of Victoria. Students execute the full design cycle to create an alarm clock with mp3 audio playback capabilities. A [STM32F407VG](http://www.st.com/web/catalog/mmc/FM141/SC1169/SS1577/LN11/PF252140) microcontroller is responsible for handling I/O, decoding the mp3 and generating the audio output. A custom-designed PCB filters and amplifies the audio output from the STM and contains the circuitry for driving the 7-segment display and cleaning up button input.

### What's in the repo?
**Schematics**

Detail the four main components of the circuit design:
* I/O
* Audio amplification and filtering
* External button connections
* 7-Segment display controller

The files will open natively in  [KiCad](http://www.kicad-pcb.org/display/KICAD/KiCad+EDA+Software+Suite).

Printable schematics, with some helpful notes, are [here](alarmclock/blob/master/kicad/schematic.pdf).

*TODO*: Include files for the PCB.

**Code**

A skeleton was provided that handles the USB On-The-Go communication and audio decoding. The bulk of the project code is in:
```
main.c
state_machine.c
buttons.c
clock_display.c
```
The included `.coproj` file will open the project in [CoIDE](http://www.coocox.org/software/coide.php) but the project should work wherever the [GCC-ARM](https://launchpad.net/gcc-arm-embedded) compiler is used.

**Known Issues**

* Errors on the schematic
  * PA4 missing on I/O sheet
  * PD8, PD10 connected to wrong pin
* mp3 playback
  * Only works with 128kb/s encoding
  * Stops playback after ~2min and needs to be hard reset before playback can continue
