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
LIBS:alarm_clock-cache
EELAYER 25 0
EELAYER END
$Descr A4 11693 8268
encoding utf-8
Sheet 5 5
Title ""
Date ""
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
P 1950 1850
F 0 "R14" V 2030 1850 50  0000 C CNN
F 1 "100k" V 1950 1850 50  0000 C CNN
F 2 "" V 1880 1850 30  0000 C CNN
F 3 "" H 1950 1850 30  0000 C CNN
	1    1950 1850
	1    0    0    -1  
$EndComp
$Comp
L 74HC14 U1
U 1 1 557DFDA9
P 2700 2450
F 0 "U1" H 2850 2550 40  0000 C CNN
F 1 "74HC14" H 2900 2350 40  0000 C CNN
F 2 "" H 2700 2450 60  0000 C CNN
F 3 "" H 2700 2450 60  0000 C CNN
	1    2700 2450
	1    0    0    -1  
$EndComp
Wire Wire Line
	1950 2000 1950 2700
Wire Wire Line
	1950 2450 2250 2450
Connection ~ 1950 2450
Wire Wire Line
	1950 3050 1950 3500
Wire Wire Line
	1950 1350 1950 1700
Text HLabel 1950 2700 0    60   Output ~ 0
S1
Text HLabel 1950 3050 0    60   Input ~ 0
S_GND
$Comp
L R R15
U 1 1 557DFEC6
P 4000 1850
F 0 "R15" V 4080 1850 50  0000 C CNN
F 1 "100k" V 4000 1850 50  0000 C CNN
F 2 "" V 3930 1850 30  0000 C CNN
F 3 "" H 4000 1850 30  0000 C CNN
	1    4000 1850
	1    0    0    -1  
$EndComp
Wire Wire Line
	4000 2000 4000 2700
Wire Wire Line
	4000 2450 4300 2450
Connection ~ 4000 2450
Wire Wire Line
	4000 3050 4000 3500
Wire Wire Line
	4000 1350 4000 1700
Text HLabel 4000 2700 0    60   Output ~ 0
S2
Text HLabel 4000 3050 0    60   Input ~ 0
S_GND
$Comp
L R R16
U 1 1 557DFF49
P 6050 1850
F 0 "R16" V 6130 1850 50  0000 C CNN
F 1 "100k" V 6050 1850 50  0000 C CNN
F 2 "" V 5980 1850 30  0000 C CNN
F 3 "" H 6050 1850 30  0000 C CNN
	1    6050 1850
	1    0    0    -1  
$EndComp
Wire Wire Line
	6050 2000 6050 2700
Wire Wire Line
	6050 2450 6350 2450
Connection ~ 6050 2450
Wire Wire Line
	6050 3050 6050 3500
Wire Wire Line
	6050 1350 6050 1700
Text HLabel 6050 2700 0    60   Output ~ 0
S3
Text HLabel 6050 3050 0    60   Input ~ 0
S_GND
$Comp
L R R17
U 1 1 557DFF90
P 7850 1850
F 0 "R17" V 7930 1850 50  0000 C CNN
F 1 "100k" V 7850 1850 50  0000 C CNN
F 2 "" V 7780 1850 30  0000 C CNN
F 3 "" H 7850 1850 30  0000 C CNN
	1    7850 1850
	1    0    0    -1  
$EndComp
Wire Wire Line
	7850 2000 7850 2700
Wire Wire Line
	7850 2450 8150 2450
Connection ~ 7850 2450
Wire Wire Line
	7850 3050 7850 3500
Text HLabel 7850 2700 0    60   Output ~ 0
S4
Text HLabel 7850 3050 0    60   Input ~ 0
S_GND
$Comp
L 74HC14 U1
U 2 1 557DFFAA
P 4750 2450
F 0 "U1" H 4900 2550 40  0000 C CNN
F 1 "74HC14" H 4950 2350 40  0000 C CNN
F 2 "" H 4750 2450 60  0000 C CNN
F 3 "" H 4750 2450 60  0000 C CNN
	2    4750 2450
	1    0    0    -1  
$EndComp
$Comp
L 74HC14 U1
U 3 1 557DFFD9
P 6800 2450
F 0 "U1" H 6950 2550 40  0000 C CNN
F 1 "74HC14" H 7000 2350 40  0000 C CNN
F 2 "" H 6800 2450 60  0000 C CNN
F 3 "" H 6800 2450 60  0000 C CNN
	3    6800 2450
	1    0    0    -1  
$EndComp
$Comp
L 74HC14 U1
U 4 1 557E007A
P 8600 2450
F 0 "U1" H 8750 2550 40  0000 C CNN
F 1 "74HC14" H 8800 2350 40  0000 C CNN
F 2 "" H 8600 2450 60  0000 C CNN
F 3 "" H 8600 2450 60  0000 C CNN
	4    8600 2450
	1    0    0    -1  
$EndComp
Wire Wire Line
	1950 1350 7850 1350
Wire Wire Line
	7850 1350 7850 1700
Wire Wire Line
	7850 3500 1950 3500
$Comp
L GND #PWR012
U 1 1 557E040B
P 5000 3500
F 0 "#PWR012" H 5000 3250 50  0001 C CNN
F 1 "GND" H 5000 3350 50  0000 C CNN
F 2 "" H 5000 3500 60  0000 C CNN
F 3 "" H 5000 3500 60  0000 C CNN
	1    5000 3500
	1    0    0    -1  
$EndComp
$Comp
L VDD #PWR013
U 1 1 557E0427
P 5050 1350
F 0 "#PWR013" H 5050 1200 50  0001 C CNN
F 1 "VDD" H 5050 1500 50  0000 C CNN
F 2 "" H 5050 1350 60  0000 C CNN
F 3 "" H 5050 1350 60  0000 C CNN
	1    5050 1350
	1    0    0    -1  
$EndComp
Wire Wire Line
	3150 2450 3200 2450
Wire Wire Line
	3200 2450 3200 2850
Text HLabel 3200 2850 0    60   Output ~ 0
PC6
Wire Wire Line
	5200 2450 5200 2850
Text HLabel 5200 2850 0    60   Output ~ 0
PC7
Wire Wire Line
	7250 2450 7250 2850
Text HLabel 7250 2850 0    60   Output ~ 0
PC8
Wire Wire Line
	9050 2450 9050 2850
Text HLabel 9050 2850 0    60   Output ~ 0
PC9
$EndSCHEMATC