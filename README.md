# PES-Final-Project
PES-Final-Project for PES, ECEN-5813, Fall-2021.

## Table of Contents
* [Description](#description)
* [Build](#build)
* [Author](#author)

## Description
Headlamp Beam Color Assitance System is a feature where headlamps vary color based on the angle of motion of the car. Headlamp beam colour assistance system changes the colour of the headlamp beam based on the PITCH and ROLL angles of the vehicle. 
When the vehicle is moving from a higher elevation to a lower level, the low beam is updated with a low wavelength colour. When the vehicle is moving up from a lower elevation point, a higher beam is required, hence for a sample project a higher wavelength colour is replaced.

Similarly, for the case of roll left and right different colours are updated. In the following proposed solution, I used a KL25Z and the on-board accelerometer(MMA8451Q). 
This feature of headlamp beam colour assistance is initialized by command line interface or by pressing a button-switch. Reducing the scale of the device by limiting the parameters to angles or positions, 
the direction of the vehicle and the colour to be updated, the conditional table is developed.

#### Technology :

1) Implemented UART using circular buffers for communication of terminal to board and vice-versa.
2) Implemented I2C for accelerometer data transfer.
3) Implemented Interrupts for GPIO switches and PWM for LED color
4) State machine implemeted for direction defenition and interactive command terminal with different commands.

#### Design Information:

Command-Position-Color Table:
![Command-Position-Color Table](https://github.com/PradyumnaGudluru/PES-Final-Project/blob/main/ProjectFinal/files/Design_Info_Table.PNG)

THRESHOLD : 35(degrees). 

Command Authentication function flow chart:
![Function Flow Chart](https://github.com/PradyumnaGudluru/PES-Final-Project/blob/main/ProjectFinal/files/Design_Info_Flow_Chart.png)

State Machine for color determination:
![State Machine](https://github.com/PradyumnaGudluru/PES-Final-Project/blob/main/ProjectFinal/files/Design_Info_State_Machine.png)

#### File Information :

cb_fifo.c/.h : Has the code for circular buffer with 256 bytes.
cbfifo_testcase.c/ .h : Testcases as given by Howdy Pierce.
commandprocessor.c/ .h : Command processor terminal command analysis with functioning of all the commands
i2c.c/ .h : Handles read, write functioning of I2C module.
led_tmp.c/ .h : Initialization and working of TPM functionality for the switching ON and OFF of LED.
mma8451.c/ .h : Functioning of the accelerometer, calculation of roll and pitch.
msec_timers.c/ .h : Functioning of the systick timer as per the lecture slides of Howdy Pierce
Project.c : Main function, calling application and init functionalities.
statemachine.c/ .h : Functioning of the main application algorithm and working of LED switching as per the design values.
switch.c/ .h : Functioning of switches with GPIO interrupts enabled.
sysclock.c/ .h : As per the function developed by Howdy Pierce.
uart.c/ .h : Has the code for UART initialization and terminal calling function.

#### Command Options:
1) author : Print the author
2) display : Print the roll and pitch values
3) help : Print all commands
4) init : initiates the headlamp beam assistance feature
5) calib : calibrates the angle roll and pitch

#### TeraTerm settings:
Speed: 38400
Data : 8 bit
Stop bits : 2 bit

#### Testing Procedure:

1) CBFIFO tested using automated tests (as per the test cases given by Howdy Pierce)
2) Manual testing done as per the testcase_sheet functionality.
	Testcase Sheet : https://github.com/PradyumnaGudluru/PES-Final-Project/blob/main/ProjectFinal/files/Testcase_Sheet.xlsx
3) The testing procedure as per the following video:
	a) Run the project on MCUXpresso
	b) help - command information visible
	c) calib - roll and pitch value calculated
	d) init - feature enabled
	e) Rotate the board for different roll and pitch values, considered corner cases.
	f) Switch the feature OFF by using GPIO port D PIN 4 interrupt (Switch 2).
	g) Initiating the feature ON by using GPIO port D PIN 3 interrupt (Switch 1).
	h) By pressing the Switch 1, printing the roll and pitch values.
	
	Video : https://drive.google.com/file/d/1BtZKkdK8tYoAmR4jaVgACcY08QV5nCG0/view?usp=sharing

#### Project Proposal :
	https://github.com/PradyumnaGudluru/PES-Final-Project/blob/main/ProjectFinal/files/PES_CourseProject_Proposal.pdf
	
#### References:
1)	Lecture slides of Howdy Pierce.
2)	https://github.com/alexander-g-dean/ESF/tree/master/NXP/
3)  Embedded Systems Fundamentals with ARm Cortex-M based Microcontrollers by Alexander G. Dean
4) Making Embedded Systems Design Patterns for Great Software-by-Elicia White
5) MMA8451Q datasheet
6) KL25Z manual/ datasheet

URL: https://github.com/PradyumnaGudluru/PES-Final-Project
  
## Build
* The compilation steps are as follows:
1.	Launch MCUXpresso_IDEv11.2.0
2.	Import the cloned project in the project window.
3.	Clean project.
4.	Build the project.
5.	Run the project by flashing the code on to the KL25Z board.

  ## Author
 * Pradyumna Gudluru
