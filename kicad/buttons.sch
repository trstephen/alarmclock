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
Sheet 2 5
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
L R R14
U 1 1 557DFC47
P 2000 2800
F 0 "R14" V 2080 2800 50  0000 C CNN
F 1 "100k" V 2000 2800 50  0000 C CNN
F 2 "" V 1930 2800 30  0000 C CNN
F 3 "" H 2000 2800 30  0000 C CNN
	1    2000 2800
	1    0    0    -1  
$EndComp
$Comp
L R R15
U 1 1 557DFEC6
P 4050 2800
F 0 "R15" V 4130 2800 50  0000 C CNN
F 1 "100k" V 4050 2800 50  0000 C CNN
F 2 "" V 3980 2800 30  0000 C CNN
F 3 "" H 4050 2800 30  0000 C CNN
	1    4050 2800
	1    0    0    -1  
$EndComp
$Comp
L R R16
U 1 1 557DFF49
P 6100 2800
F 0 "R16" V 6180 2800 50  0000 C CNN
F 1 "100k" V 6100 2800 50  0000 C CNN
F 2 "" V 6030 2800 30  0000 C CNN
F 3 "" H 6100 2800 30  0000 C CNN
	1    6100 2800
	1    0    0    -1  
$EndComp
$Comp
L R R17
U 1 1 557DFF90
P 7900 2800
F 0 "R17" V 7980 2800 50  0000 C CNN
F 1 "100k" V 7900 2800 50  0000 C CNN
F 2 "" V 7830 2800 30  0000 C CNN
F 3 "" H 7900 2800 30  0000 C CNN
	1    7900 2800
	1    0    0    -1  
$EndComp
$Comp
L 74HC14 U1
U 2 1 557DFFAA
P 4800 3400
F 0 "U1" H 4950 3500 40  0000 C CNN
F 1 "74HC14" H 5000 3300 40  0000 C CNN
F 2 "" H 4800 3400 60  0000 C CNN
F 3 "" H 4800 3400 60  0000 C CNN
	2    4800 3400
	1    0    0    -1  
$EndComp
$Comp
L 74HC14 U1
U 3 1 557DFFD9
P 6850 3400
F 0 "U1" H 7000 3500 40  0000 C CNN
F 1 "74HC14" H 7050 3300 40  0000 C CNN
F 2 "" H 6850 3400 60  0000 C CNN
F 3 "" H 6850 3400 60  0000 C CNN
	3    6850 3400
	1    0    0    -1  
$EndComp
$Comp
L 74HC14 U1
U 4 1 557E007A
P 8650 3400
F 0 "U1" H 8800 3500 40  0000 C CNN
F 1 "74HC14" H 8850 3300 40  0000 C CNN
F 2 "" H 8650 3400 60  0000 C CNN
F 3 "" H 8650 3400 60  0000 C CNN
	4    8650 3400
	1    0    0    -1  
$EndComp
Text GLabel 3250 3800 0    60   Output ~ 0
PC6
Text GLabel 5250 3800 0    60   Output ~ 0
PC7
Text GLabel 7300 3800 0    60   Output ~ 0
PC8
Text GLabel 9100 3800 0    60   Output ~ 0
PC9
$Comp
L 74HC14 U1
U 6 1 557E2E40
P 1200 3450
F 0 "U1" H 1350 3550 40  0000 C CNN
F 1 "74HC14" H 1400 3350 40  0000 C CNN
F 2 "" H 1200 3450 60  0000 C CNN
F 3 "" H 1200 3450 60  0000 C CNN
	6    1200 3450
	1    0    0    -1  
$EndComp
Text GLabel 6250 5300 2    60   Output ~ 0
S_GND
$Comp
L 74HC14 U1
U 1 1 557DFDA9
P 2750 3400
F 0 "U1" H 2900 3500 40  0000 C CNN
F 1 "74HC14" H 2950 3300 40  0000 C CNN
F 2 "" H 2750 3400 60  0000 C CNN
F 3 "" H 2750 3400 60  0000 C CNN
	1    2750 3400
	1    0    0    -1  
$EndComp
NoConn ~ 1650 3450
$Comp
L 74HC14 U1
U 5 1 557E2DE0
P 10450 3350
F 0 "U1" H 10600 3450 40  0000 C CNN
F 1 "74HC14" H 10650 3250 40  0000 C CNN
F 2 "" H 10450 3350 60  0000 C CNN
F 3 "" H 10450 3350 60  0000 C CNN
	5    10450 3350
	1    0    0    -1  
$EndComp
NoConn ~ 750  3450
Wire Wire Line
	2000 3400 2300 3400
Connection ~ 2000 3400
Wire Wire Line
	4050 3400 4350 3400
Connection ~ 4050 3400
Wire Wire Line
	6100 3400 6400 3400
Connection ~ 6100 3400
Wire Wire Line
	7900 3400 8200 3400
Connection ~ 7900 3400
Wire Wire Line
	3200 3400 3250 3400
Wire Wire Line
	3250 3400 3250 3800
Wire Wire Line
	5250 3400 5250 3800
Wire Wire Line
	7300 3400 7300 3800
Wire Wire Line
	9100 3400 9100 3800
Connection ~ 2000 2300
Connection ~ 4850 4450
Connection ~ 4650 4450
Connection ~ 5100 2300
Connection ~ 6100 3500
Connection ~ 7900 3550
Connection ~ 7900 2300
Connection ~ 4750 4450
Connection ~ 6800 4450
Connection ~ 7900 4450
Connection ~ 5650 4450
Wire Wire Line
	6100 3500 6350 3500
Wire Wire Line
	7900 3550 8200 3550
Connection ~ 6100 4450
Connection ~ 4200 4450
Wire Wire Line
	2000 3550 2300 3550
Connection ~ 5600 4450
Connection ~ 4050 2300
Text GLabel 5750 5950 3    60   Input ~ 0
GND_102
Connection ~ 6100 2300
Connection ~ 2700 4450
Wire Wire Line
	1150 2300 8600 2300
Wire Wire Line
	10400 2300 10400 3250
Wire Wire Line
	2700 3300 2700 2300
Connection ~ 2700 2300
Wire Wire Line
	4750 3300 4750 2300
Connection ~ 4750 2300
Wire Wire Line
	6800 3300 6800 2300
Connection ~ 6800 2300
Wire Wire Line
	8600 2300 8600 3300
Text GLabel 6450 1850 0    60   Input ~ 0
5V_203
Wire Wire Line
	6450 1850 6500 1850
Wire Wire Line
	6500 1850 6500 2300
Connection ~ 6500 2300
Wire Wire Line
	2700 3500 2700 4450
Connection ~ 2750 4450
Wire Wire Line
	4750 4450 4750 3500
Wire Wire Line
	6800 4450 6800 3500
Wire Wire Line
	8600 4450 8600 3500
Text GLabel 2300 3550 2    60   Output ~ 0
S1
Text GLabel 4300 3600 2    60   Output ~ 0
S2
Wire Wire Line
	4050 3600 4300 3600
Text GLabel 6350 3500 2    60   Output ~ 0
S3
Text GLabel 8200 3550 2    60   Output ~ 0
S4
Wire Wire Line
	7900 2550 7900 2300
Wire Wire Line
	7900 3050 7900 3550
Wire Wire Line
	6100 3050 6100 3500
Wire Wire Line
	6100 2550 6100 2300
Wire Wire Line
	4050 2550 4050 2300
Wire Wire Line
	4050 3050 4050 3600
Wire Wire Line
	2000 3050 2000 3550
Wire Wire Line
	2000 2550 2000 2300
$Comp
L R R18
U 1 1 557F4624
P 9750 2700
F 0 "R18" V 9830 2700 50  0000 C CNN
F 1 "100k" V 9750 2700 50  0000 C CNN
F 2 "" V 9680 2700 30  0000 C CNN
F 3 "" H 9750 2700 30  0000 C CNN
	1    9750 2700
	1    0    0    -1  
$EndComp
Wire Wire Line
	9750 2300 9750 2450
Wire Wire Line
	8550 2300 9750 2300
Connection ~ 8550 2300
Wire Wire Line
	9750 2950 9750 3650
Wire Wire Line
	9750 3350 10000 3350
Wire Wire Line
	9750 3650 9900 3650
Connection ~ 9750 3350
Wire Wire Line
	10400 2300 9700 2300
Connection ~ 9700 2300
Text GLabel 11000 3700 0    60   Output ~ 0
PC10
Wire Wire Line
	10900 3350 11000 3350
Wire Wire Line
	11000 3350 11000 3700
Text GLabel 9900 3650 2    60   Output ~ 0
S5
Wire Wire Line
	1150 4450 10400 4450
Connection ~ 10400 4450
Wire Wire Line
	10400 4450 10400 3450
Wire Wire Line
	1150 3350 1150 2300
Wire Wire Line
	1150 3550 1150 4450
Wire Wire Line
	5750 4450 5750 5950
Connection ~ 5750 4450
Wire Wire Line
	5750 5300 6250 5300
Connection ~ 5750 5300
Text Notes 5900 5100 0    60   ~ 0
Return ground line from buttons
Text Notes 5950 6100 0    60   ~ 0
To board ground
Text Notes 1250 3800 0    60   ~ 0
Unused IC element
Text Notes 5150 4000 0    60   ~ 0
To switches on front panel
$EndSCHEMATC
