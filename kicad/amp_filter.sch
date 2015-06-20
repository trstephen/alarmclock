EESchema Schematic File Version 2
LIBS:power
LIBS:device
LIBS:transistors
LIBS:conn
LIBS:linear
LIBS:regul
LIBS:74xx
LIBS:cmos4000
LIBS:adc-dac
LIBS:memory
LIBS:xilinx
LIBS:special
LIBS:microcontrollers
LIBS:dsp
LIBS:microchip
LIBS:analog_switches
LIBS:motorola
LIBS:texas
LIBS:intel
LIBS:audio
LIBS:interface
LIBS:digital-audio
LIBS:philips
LIBS:display
LIBS:cypress
LIBS:siliconi
LIBS:opto
LIBS:atmel
LIBS:contrib
LIBS:valves
LIBS:ece
LIBS:alarm_clock-cache
EELAYER 27 0
EELAYER END
$Descr A4 11693 8268
encoding utf-8
Sheet 3 5
Title ""
Date "19 jun 2015"
Rev ""
Comp ""
Comment1 ""
Comment2 ""
Comment3 ""
Comment4 ""
$EndDescr
$Comp
L LM386 Audio2
U 1 1 557B9F18
P 5450 3700
F 0 "Audio2" H 5700 3900 60  0000 C CNN
F 1 "LM386" H 5600 4000 60  0000 C CNN
F 2 "" H 5450 3700 60  0000 C CNN
F 3 "" H 5450 3700 60  0000 C CNN
	1    5450 3700
	1    0    0    -1  
$EndComp
$Comp
L R R12
U 1 1 557BA3B3
P 4700 4650
F 0 "R12" H 4900 4650 50  0000 C CNN
F 1 "10k" V 4700 4650 50  0000 C CNN
F 2 "" V 4630 4650 30  0000 C CNN
F 3 "" H 4700 4650 30  0000 C CNN
	1    4700 4650
	0    -1   -1   0   
$EndComp
$Comp
L R R11
U 1 1 557BBDA2
P 4500 3300
F 0 "R11" V 4580 3300 50  0000 C CNN
F 1 "10k" V 4500 3300 50  0000 C CNN
F 2 "" V 4430 3300 30  0000 C CNN
F 3 "" H 4500 3300 30  0000 C CNN
	1    4500 3300
	0    1    1    0   
$EndComp
$Comp
L C C5
U 1 1 557BC0F4
P 4000 2650
F 0 "C5" H 4050 2850 50  0000 L CNN
F 1 "0.01uF" H 4025 2550 50  0000 L CNN
F 2 "" H 4038 2500 30  0000 C CNN
F 3 "" H 4000 2650 60  0000 C CNN
	1    4000 2650
	1    0    0    -1  
$EndComp
$Comp
L CP1 C2
U 1 1 557BC4F9
P 6250 2950
F 0 "C2" V 6400 2350 50  0000 L CNN
F 1 "100uF" V 6100 2850 50  0000 L CNN
F 2 "" H 6250 2950 60  0000 C CNN
F 3 "" H 6250 2950 60  0000 C CNN
	1    6250 2950
	0    -1   -1   0   
$EndComp
Text Label 5550 4100 0    60   ~ 0
NoConnection
$Comp
L R R13
U 1 1 557BC882
P 6550 4050
F 0 "R13" H 6350 3950 50  0000 C CNN
F 1 "10" V 6550 4050 50  0000 C CNN
F 2 "" V 6480 4050 30  0000 C CNN
F 3 "" H 6550 4050 30  0000 C CNN
	1    6550 4050
	-1   0    0    1   
$EndComp
$Comp
L CP1 C4
U 1 1 557BCB2B
P 7050 3700
F 0 "C4" V 7250 3300 50  0000 L CNN
F 1 "220uF" V 6900 3600 50  0000 L CNN
F 2 "" H 7050 3700 60  0000 C CNN
F 3 "" H 7050 3700 60  0000 C CNN
	1    7050 3700
	0    -1   -1   0   
$EndComp
$Comp
L C C3
U 1 1 557BCEB2
P 6550 4750
F 0 "C3" H 6575 4850 50  0000 L CNN
F 1 ".047uF" H 6575 4650 50  0000 L CNN
F 2 "" H 6588 4600 30  0000 C CNN
F 3 "" H 6550 4750 60  0000 C CNN
	1    6550 4750
	1    0    0    -1  
$EndComp
Text Notes 3900 4850 0    60   ~ 0
High: off\nLow: on
Text Notes 3550 2350 0    60   ~ 0
Audio in\n
Text GLabel 4250 4650 0    60   Input ~ 0
PD6
Text Notes 3850 4550 0    60   ~ 0
Audio disable
Text GLabel 4000 3050 0    60   Output ~ 0
VOL+
Text GLabel 4150 3300 0    60   Input ~ 0
VOL_SEL
Text GLabel 4000 3550 0    60   Output ~ 0
VOL-
Text Notes 3650 3850 1    60   ~ 0
To volume potentiometer
Text GLabel 7550 3700 2    60   Output ~ 0
SPKR+
Text GLabel 7550 3900 2    60   Input ~ 0
SPKR-
Text Notes 8050 4050 1    60   ~ 0
To speaker
Text GLabel 3850 2400 0    60   Output ~ 0
AudioIn
NoConn ~ 5550 4100
Text GLabel 5900 2300 0    60   Output ~ 0
5V_203
Text GLabel 6200 5750 3    60   Input ~ 0
GND_103
$Comp
L +5V #PWR01
U 1 1 557EBF37
P 6100 2450
F 0 "#PWR01" H 6100 2300 50  0001 C CNN
F 1 "+5V" H 6100 2590 50  0000 C CNN
F 2 "" H 6100 2450 60  0000 C CNN
F 3 "" H 6100 2450 60  0000 C CNN
	1    6100 2450
	0    1    1    0   
$EndComp
$Comp
L GND #PWR02
U 1 1 557EC04F
P 5950 5900
F 0 "#PWR02" H 5950 5650 50  0001 C CNN
F 1 "GND" H 5950 5750 50  0000 C CNN
F 2 "" H 5950 5900 60  0000 C CNN
F 3 "" H 5950 5900 60  0000 C CNN
	1    5950 5900
	1    0    0    -1  
$EndComp
Wire Wire Line
	5450 4400 5450 4100
Wire Wire Line
	4950 3300 4950 3600
Connection ~ 4800 3300
Wire Wire Line
	4000 3550 4000 4150
Wire Wire Line
	4000 2400 3850 2400
Wire Wire Line
	5350 3300 5350 3200
Wire Wire Line
	5350 3200 5900 3200
Wire Wire Line
	5900 3200 5900 2300
Wire Wire Line
	6450 2950 8450 2950
Connection ~ 6550 3700
Wire Wire Line
	7550 5450 7550 3900
Wire Wire Line
	5450 5450 5450 4800
Wire Wire Line
	5350 4100 5000 4100
Wire Wire Line
	5000 4100 5000 5450
Wire Wire Line
	4800 2050 3200 2050
Wire Wire Line
	3200 2050 3200 5450
Wire Wire Line
	4000 4150 3500 4150
Wire Wire Line
	3500 4150 3500 5450
Wire Wire Line
	8450 2950 8450 5450
Wire Wire Line
	8450 5450 3200 5450
Connection ~ 7550 5450
Connection ~ 6550 5450
Connection ~ 3500 5450
Connection ~ 5000 5450
Wire Wire Line
	4950 3800 4350 3800
Wire Wire Line
	4350 3800 4350 5950
Connection ~ 4350 5450
Connection ~ 5450 5450
Wire Wire Line
	6200 5750 5950 5750
Wire Wire Line
	5950 5450 5950 5900
Connection ~ 5950 5450
Wire Wire Line
	5900 2450 6100 2450
Connection ~ 5900 2450
Connection ~ 5950 5750
Connection ~ 4000 2450
Wire Wire Line
	4000 2450 4000 2450
Wire Wire Line
	4000 2850 4000 2850
Connection ~ 4000 2850
Wire Wire Line
	4000 2850 4000 3050
Wire Wire Line
	4000 2450 4000 2400
Text GLabel 3900 5950 0    60   Output ~ 0
AudioJackGrnd
Wire Wire Line
	4350 5950 3900 5950
$Comp
L 2N7000 M1
U 1 1 55809FE3
P 5350 4600
F 0 "M1" H 5550 4675 50  0000 L CNN
F 1 "2N7000" H 5550 4600 50  0000 L CNN
F 2 "TO-92" H 5550 4525 50  0000 L CIN
F 3 "~" H 5350 4600 50  0000 L CNN
	1    5350 4600
	1    0    0    -1  
$EndComp
Text GLabel 4150 1700 2    60   Input ~ 0
AudioIn
Text GLabel 3450 1900 0    60   Output ~ 0
PA4
Text GLabel 3350 1500 0    60   Output ~ 0
AudiojackOut
$Comp
L R R19
U 1 1 55847DF0
P 3700 1500
F 0 "R19" V 3780 1500 40  0000 C CNN
F 1 "R" V 3707 1501 40  0000 C CNN
F 2 "~" V 3630 1500 30  0000 C CNN
F 3 "~" H 3700 1500 30  0000 C CNN
	1    3700 1500
	0    1    1    0   
$EndComp
Wire Wire Line
	3350 1500 3450 1500
Wire Wire Line
	3950 1900 3950 1500
Wire Wire Line
	3950 1700 4150 1700
Wire Wire Line
	3950 1900 3450 1900
Connection ~ 3950 1700
Wire Wire Line
	6050 2950 5900 2950
Connection ~ 5900 2950
$Comp
L C C1
U 1 1 557BC2CC
P 4800 2950
F 0 "C1" H 4825 3050 50  0000 L CNN
F 1 "800pF" H 4825 2850 50  0000 L CNN
F 2 "" H 4838 2800 30  0000 C CNN
F 3 "" H 4800 2950 60  0000 C CNN
	1    4800 2950
	1    0    0    -1  
$EndComp
Wire Wire Line
	4800 3300 4800 3150
Wire Wire Line
	4800 2750 4800 2050
Wire Wire Line
	4750 3300 4950 3300
Wire Wire Line
	4250 3300 4150 3300
Wire Wire Line
	5150 4650 4950 4650
Wire Wire Line
	4450 4650 4250 4650
Wire Wire Line
	6550 3700 6550 3800
Wire Wire Line
	6550 4300 6550 4550
Wire Wire Line
	6550 4950 6550 5450
Wire Wire Line
	5950 3700 6850 3700
Wire Wire Line
	7250 3700 7550 3700
Text Notes 3800 1710 2    60   ~ 0
Auxiliary jack to audio in
$EndSCHEMATC
