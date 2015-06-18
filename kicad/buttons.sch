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
Date "18 jun 2015"
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
Text GLabel 3200 2850 0    60   Output ~ 0
PC6
Text GLabel 5200 2850 0    60   Output ~ 0
PC7
Text GLabel 7250 2850 0    60   Output ~ 0
PC8
Text GLabel 9050 2850 0    60   Output ~ 0
PC9
$Comp
L 74HC14 U1
U 6 1 557E2E40
P 3750 5150
F 0 "U1" H 3900 5250 40  0000 C CNN
F 1 "74HC14" H 3950 5050 40  0000 C CNN
F 2 "" H 3750 5150 60  0000 C CNN
F 3 "" H 3750 5150 60  0000 C CNN
	6    3750 5150
	1    0    0    -1  
$EndComp
Text GLabel 6800 4100 2    60   Output ~ 0
S_GND
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
NoConn ~ 4200 5150
$Comp
L 74HC14 U1
U 5 1 557E2DE0
P 10400 2400
F 0 "U1" H 10550 2500 40  0000 C CNN
F 1 "74HC14" H 10600 2300 40  0000 C CNN
F 2 "" H 10400 2400 60  0000 C CNN
F 3 "" H 10400 2400 60  0000 C CNN
	5    10400 2400
	1    0    0    -1  
$EndComp
NoConn ~ 3300 5150
Wire Wire Line
	1950 2450 2250 2450
Connection ~ 1950 2450
Wire Wire Line
	4000 2450 4300 2450
Connection ~ 4000 2450
Wire Wire Line
	6050 2450 6350 2450
Connection ~ 6050 2450
Wire Wire Line
	7850 2450 8150 2450
Connection ~ 7850 2450
Wire Wire Line
	3150 2450 3200 2450
Wire Wire Line
	3200 2450 3200 2850
Wire Wire Line
	5200 2450 5200 2850
Wire Wire Line
	7250 2450 7250 2850
Wire Wire Line
	9050 2450 9050 2850
Connection ~ 1950 1350
Connection ~ 4800 3500
Connection ~ 4600 3500
Connection ~ 5050 1350
Connection ~ 6050 2550
Connection ~ 7850 2600
Connection ~ 7850 1350
Wire Wire Line
	2600 3500 2700 3500
Connection ~ 4700 3500
Connection ~ 6750 3500
Connection ~ 7850 3500
Connection ~ 5600 3500
Wire Wire Line
	6050 2550 6300 2550
Wire Wire Line
	7850 2600 8150 2600
Connection ~ 6050 3500
Connection ~ 4150 3500
Wire Wire Line
	1950 2600 2250 2600
Wire Wire Line
	4700 4000 6800 4000
Wire Wire Line
	6800 4000 6800 4100
Wire Wire Line
	5600 4000 5600 3950
Connection ~ 5600 3950
Wire Wire Line
	5550 3500 5550 3950
Wire Wire Line
	5550 3950 5600 3950
Connection ~ 5550 3500
Connection ~ 4000 1350
Wire Wire Line
	5800 4000 5800 4500
Connection ~ 5800 4000
Text GLabel 5800 4500 0    60   Input ~ 0
GND_102
Connection ~ 6050 1350
Connection ~ 2650 3500
Connection ~ 5600 4000
Wire Wire Line
	1250 1350 8550 1350
Wire Wire Line
	3700 4800 1250 4800
Wire Wire Line
	3700 5050 3700 4800
Wire Wire Line
	1250 4800 1250 1350
Wire Wire Line
	10350 1350 10350 2300
Connection ~ 1250 3950
Wire Wire Line
	2650 2350 2650 1350
Connection ~ 2650 1350
Wire Wire Line
	4700 2350 4700 1350
Connection ~ 4700 1350
Wire Wire Line
	6750 2350 6750 1350
Connection ~ 6750 1350
Wire Wire Line
	8550 1350 8550 2350
Text GLabel 6400 900  0    60   Input ~ 0
5V_203
Wire Wire Line
	6400 900  6450 900 
Wire Wire Line
	6450 900  6450 1350
Connection ~ 6450 1350
Wire Wire Line
	3700 5250 3700 5500
Wire Wire Line
	3700 5500 4700 5500
Wire Wire Line
	4700 5500 4700 4000
Wire Wire Line
	10350 3900 10350 2500
Connection ~ 4700 4600
Wire Wire Line
	2650 2550 2650 3500
Connection ~ 2600 3500
Connection ~ 2700 3500
Wire Wire Line
	4700 2550 4700 3500
Wire Wire Line
	6750 2550 6750 3500
Wire Wire Line
	8550 3500 8550 2550
Wire Wire Line
	6700 3900 10350 3900
Wire Wire Line
	6700 3900 6700 4000
Connection ~ 6700 4000
Text GLabel 2250 2600 2    60   Output ~ 0
S1
Text GLabel 4250 2650 2    60   Output ~ 0
S2
Wire Wire Line
	4000 2650 4250 2650
Text GLabel 6300 2550 2    60   Output ~ 0
S3
Text GLabel 8150 2600 2    60   Output ~ 0
S4
Wire Wire Line
	1950 3500 8550 3500
Wire Wire Line
	7850 1600 7850 1350
Wire Wire Line
	7850 2100 7850 2600
Wire Wire Line
	6050 2100 6050 2550
Wire Wire Line
	6050 1600 6050 1350
Wire Wire Line
	4000 1600 4000 1350
Wire Wire Line
	4000 2100 4000 2650
Wire Wire Line
	1950 2100 1950 2600
Wire Wire Line
	1950 1600 1950 1350
$Comp
L R R18
U 1 1 557F4624
P 9700 1750
F 0 "R18" V 9780 1750 50  0000 C CNN
F 1 "100k" V 9700 1750 50  0000 C CNN
F 2 "" V 9630 1750 30  0000 C CNN
F 3 "" H 9700 1750 30  0000 C CNN
	1    9700 1750
	1    0    0    -1  
$EndComp
Wire Wire Line
	9700 1500 9700 1350
Wire Wire Line
	9700 1350 8500 1350
Connection ~ 8500 1350
Wire Wire Line
	9700 2000 9700 2700
Wire Wire Line
	9700 2400 9950 2400
Wire Wire Line
	9700 2700 9850 2700
Connection ~ 9700 2400
Wire Wire Line
	10350 1350 9650 1350
Connection ~ 9650 1350
Text GLabel 10950 2750 0    60   Output ~ 0
PC10
Wire Wire Line
	10850 2400 10950 2400
Wire Wire Line
	10950 2400 10950 2750
Text GLabel 9850 2700 2    60   Output ~ 0
S5
Connection ~ 6250 4000
$EndSCHEMATC
