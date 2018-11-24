/*
*
* Team Id: 1508
* Author List: Gudapati Nitish, Deeepak S V, Jinesh R, Himadri Poddar
* Filename: NS_Task_1_Sandbox.cpp
* Theme: Nutty Squirrel -- Specific to eYRC
* Functions: forward_wls(unsigned char), left_turn_wls(void), right_turn_wls(void)
*			Square(void), Task_1_1(void), Task_1_2(void)
* Global Variables: left_sensor, centre_sensor, right_sensor
*
*/

#include "NS_Task_1_Sandbox.h"

//variables that store the adc values of the white line sensors
unsigned char left_sensor, centre_sensor, right_sensor;

//variable to store the digital value of the whiteline sensor
unsigned char line_sensor, line_memory;

/*
*
* Function Name: ir_array
* Input: NONE 
* Output: unsigned char -> returns a combined value corresponding to the white line sensor
* Logic: Sets a threshold value to distinguish white and black and sets 1 for black and 0 for white
*		Threshold = 180, if adc_value > 180 => 1 (black), and vice versa
* Example Call: ir_array(); //Returns one of 100, 010, 001, 110, etc.
*
*/
unsigned char ir_array (void)
{
	left_sensor = ADC_Conversion(1);
	centre_sensor = ADC_Conversion(2);
	right_sensor = ADC_Conversion(3);
	line_sensor=(left_sensor>180)*4+(centre_sensor>180)*2+(right_sensor>180);
	//printf("\n %03d", line_sensor);
	return line_sensor;
}

/*
*
* Function Name: line_track
* Input: NONE
* Output: Follows the black line
* Logic: Sets a threshold value to distinguish white and black and sets 1 for black and 0 for white
*		Threshold = 180, if adc_value > 180 => 1 (black), and vice versa
* Example Call: ir_array(); //Returns one of 100, 010, 001, 110, etc.
*
*/
void line_track(void)
{
	if ((ir_array() == 0b010)|| (ir_array() == 0b110)|| (ir_array() == 0b011))
	{
		forward();		velocity(150, 150);
		_delay_ms(1);
	}
	else if (ir_array() == 0b001)
	{
		soft_right();	velocity(150, 150);
		line_memory = 1;
		_delay_ms(1);
	}
	else if (ir_array() == 0b100)
	{
		soft_left();	velocity(150, 150);
		line_memory = 2;
		_delay_ms(1);
	}
	/*else if (ir_array() == 0b011)
	{
		soft_right();	velocity(150, 150);
		line_memory = 1;
		_delay_ms(1);
	}
	else if (ir_array() == 0b110)
	{
		soft_left();	velocity(150, 150);
		line_memory = 2;
		_delay_ms(1);
	}*/
	else if (ir_array() == 0b000)
	{
		if (line_memory == 1)
		{
			soft_right();	velocity(150, 150);
			_delay_ms(1);
		}
		else
		{
			soft_left();	velocity(150, 150);
			_delay_ms(1);
		}
	}
}

/*
*
* Function Name: forward_wls
* Input: node
* Output: void
* Logic: Uses white line sensors to go forward by the number of nodes specified
* Example Call: forward_wls(2); //Goes forward by two nodes
*
*/
void forward_wls(unsigned char node)
{
	int node_count = 0;		//Counter for number of nodes crossed
	while (node_count < node)
	{
		line_track();
		if (ir_array() == 7)
		{
			node_count++;
			for (int i = 0; ir_array() == 7; i++);
			stop();
		}
	}
}

/*
*
* Function Name: left_turn_wls
* Input: void
* Output: void
* Logic: Uses white line sensors to turn left until black line is encountered
* Example Call: left_turn_wls(); //Turns left until black line is encountered
*
*/
void left_turn_wls(void)
{
	while ((ir_array() & 0b010) == 0b000)
	{
		left();		velocity(100, 100);
	}
	stop();
}

/*
*
* Function Name: right_turn_wls
* Input: void
* Output: void
* Logic: Uses white line sensors to turn right until black line is encountered
* Example Call: right_turn_wls(); //Turns right until black line is encountered
*/
void right_turn_wls(void)
{
	while ((ir_array() & 0b010) == 0b000)
	{
		right();	velocity(100, 100);
	}
	stop();
}

/*
*
* Function Name: Square
* Input: void
* Output: void
* Logic: Use this function to make the robot trace a square path on the arena
* Example Call: Square();
*/
void Square(void)
{
	forward();	velocity(230, 230);		//moves forward for 1 sec with ~90% dutycycle
	_delay_ms(1000);
	right();	velocity(230, 230);		//turns 90 deg right
	_delay_ms(500);
	forward();	velocity(230, 230);		//moves forward for 1 sec with ~90% dutycycle
	_delay_ms(1000);
	right();	velocity(230, 230);		//turns 90 deg right
	_delay_ms(500);
	forward();	velocity(230, 230);		//moves forward for 1 sec with ~90% dutycycle
	_delay_ms(1000);
	right();	velocity(230, 230);		//turns 90 deg right
	_delay_ms(500);
	forward();	velocity(230, 230);		//moves forward for 1 sec with ~90% dutycycle
	_delay_ms(1000);
	right();	velocity(230, 230);		//turns 90 deg right
	_delay_ms(500);
	stop();
}

void Test()
{
	printf("\n %d", ir_array());
}

/*
*
* Function Name: Task_1_1
* Input: void
* Output: void
* Logic: Use this function to encapsulate your Task 1.1 logic
* Example Call: Task_1_1();
*/
void Task_1_1(void)
{
	_delay_ms(10);
	{
		forward_wls(1);	velocity(150, 150);
		forward();		velocity(150, 150);
		_delay_ms(250);
	}
	{
		right();		velocity(150, 150);
		_delay_ms(600);
		right_turn_wls();
	}
	{
		forward_wls(1); velocity(150, 150);
		forward();		velocity(150, 150);
		_delay_ms(300);
	}
	{
		left();			velocity(150, 150);
		_delay_ms(400);
		left_turn_wls();
	}
	{
		forward_wls(1);
		forward();		velocity(150, 150);
		_delay_ms(250);
	}
	{
		left();			velocity(150, 150);
		_delay_ms(300);
		left_turn_wls();
	}
	{
		forward_wls(1);
		forward();		velocity(150, 150);
		_delay_ms(300);
	}
	{
		left();			velocity(150, 150);
		_delay_ms(300);
		left_turn_wls();
	}
	{
		forward_wls(1);
		forward();		velocity(150, 150);
		_delay_ms(300);
	}
	{
		right();		velocity(150, 150);
		_delay_ms(300);
		right_turn_wls();
	}
	{
		forward_wls(3);
		forward();		velocity(150, 150);
		_delay_ms(300);
	}
	{
		right();		velocity(150, 150);
		_delay_ms(500);
		right_turn_wls();
	}
	{
		forward_wls(1);
		forward();		velocity(150, 150);
		_delay_ms(300);
	}
	{
		left();			velocity(150, 150);
		_delay_ms(300);
		left_turn_wls();
	}
	{
		forward_wls(1);
		forward();		velocity(150, 150);
		_delay_ms(300);
	}
	{
		left();			velocity(150, 150);
		_delay_ms(300);
		left_turn_wls();
	}
	{
		forward_wls(1);
		forward();		velocity(150, 150);
		_delay_ms(300);
	}
	{
		left();			velocity(150, 150);
		_delay_ms(300);
		left_turn_wls();
	}
	{
		forward();		velocity(150, 150);
		_delay_ms(300);
		forward_wls(1);
		forward();		velocity(150, 150);
		_delay_ms(300);
	}
	{
		right();		velocity(150, 150);
		_delay_ms(300);
		right_turn_wls();
	}
	{
		forward_wls(1);
		forward();		velocity(150, 150);
		_delay_ms(300);
	}
	stop();
	while (1);
}

/*
*
* Function Name: Task_1_2
* Input: void
* Output: void
* Logic: Use this function to encapsulate your Task 1.2 logic
* Example Call: Task_1_2();
*/
void Task_1_2(void)
{

}
