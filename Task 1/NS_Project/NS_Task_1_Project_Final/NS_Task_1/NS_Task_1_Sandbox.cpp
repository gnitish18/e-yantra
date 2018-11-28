/*
*
* Team Id: 1508
* Author List: Gudapati Nitish, Deepak S V, Jinesh R, Himadri Poddar
* Filename: NS_Task_1_Sandbox.cpp
* Theme: Nutty Squirrel -- Specific to eYRC
* Functions: ir_array(void), line_track(void), F(void), L(void), R(void), forward_wls(unsigned char), left_turn_wls(void), right_turn_wls(void)
*			Square(void), Task_1_1(void), Task_1_2(void)
* Global Variables: line_memory, line_memory_rw, line_track_memory, obstracle_front
*
*/

#include "NS_Task_1_Sandbox.h"

/*
*	Variable name: line_memory
*	To specify preferred turn direction for 'line_track()' in '0b101' case and also set if line tracking is to be done with with 2 or 3 sensors.
*	Possible values:
*		0 - Turn left during '0b101' case in 'line_track()'
*		1 - Turn right during '0b101' case in 'line_track()'
*		2 - Specifies 'line_track()' to track line only with center-right sensors (2 sensors)
*		3 - Specifies 'line_track()' to track line only with center-left sensors (2 sensors)
*/
unsigned char line_memory;

/*
*	Variable name: line_memory_rw
*	Switch variable to allow modification of 'line_memory'
*	Possible values:
*		0 - 'line_memory' set to read-only
*		1 - 'line_memory' set to read-write
*/
unsigned char line_memory_rw = 1;

/*
*	Variable name: line_track_memory
*	Stores the previous turn value, and used to guide the bot when out of the track in 'line_track()'
*	Possible values:
*		0 - stored after left turn
*		1 - stored after right turn
*/
unsigned char line_track_memory = 0;

/*
*	Variable name: obstracle_front
*	Reduces forward distance travelled to align the bot after crossing a node if an independant line (obstracle) is in vicinity (eg. 1st node)
*	Possible values:
*		0 - no independant line in vicinity
*		1 - independant line present in vicinity
*/
unsigned char obstracle_front = 0;

//	Initializing default values for North, East, South, West
enum direction { N, E, S, W };
/*
*
*	Variable name: maze
*	Array that stores complete node details of Task 1.2 track and can be used by Dijikstra`s algorithm for path planning
*
*/
//							0		1		2		3		4		5		6		7		8		9		10		11		12		13		14		15		16		17		18		19		20		21		22		23
int maze[24][24][2] = { {	{-1,-1},{-1,-1},{-1,-1},{-1,-1},{-1,-1},{-1,-1},{-1,-1},{-1,-1},{-1,-1},{46,N },{-1,-1},{-1,-1},{-1,-1},{-1,-1},{-1,-1},{-1,-1},{-1,-1},{-1,-1},{-1,-1},{-1,-1},{-1,-1},{-1,-1},{-1,-1},{-1,-1}},	//0
						{	{-1,-1},{-1,-1},{-1,-1},{88,N },{-1,-1},{-1,-1},{-1,-1},{-1,-1},{-1,-1},{-1,-1},{-1,-1},{-1,-1},{-1,-1},{-1,-1},{-1,-1},{-1,-1},{-1,-1},{-1,-1},{-1,-1},{-1,-1},{-1,-1},{-1,-1},{-1,-1},{-1,-1}},	//1
						{	{-1,-1},{-1,-1},{-1,-1},{-1,-1},{-1,-1},{-1,-1},{-1,-1},{-1,-1},{88,N },{-1,-1},{-1,-1},{-1,-1},{-1,-1},{-1,-1},{-1,-1},{-1,-1},{-1,-1},{-1,-1},{-1,-1},{-1,-1},{-1,-1},{-1,-1},{-1,-1},{-1,-1}},	//2
						{	{-1,-1},{88,S },{-1,-1},{-1,-1},{91,E },{-1,-1},{-1,-1},{-1,-1},{-1,-1},{-1,-1},{-1,-1},{-1,-1},{-1,-1},{240,N},{-1,-1},{-1,-1},{-1,-1},{-1,-1},{-1,-1},{-1,-1},{-1,-1},{-1,-1},{-1,-1},{-1,-1}},	//3
						{	{-1,-1},{-1,-1},{-1,-1},{91,W },{-1,-1},{53,E },{-1,-1},{-1,-1},{-1,-1},{-1,-1},{-1,-1},{-1,-1},{-1,-1},{-1,-1},{-1,-1},{-1,-1},{-1,-1},{-1,-1},{-1,-1},{-1,-1},{-1,-1},{-1,-1},{-1,-1},{-1,-1}},	//4
						{	{-1,-1},{-1,-1},{-1,-1},{-1,-1},{53,W },{-1,-1},{420,S},{-1,-1},{-1,-1},{-1,-1},{103,N},{-1,-1},{-1,-1},{-1,-1},{-1,-1},{-1,-1},{-1,-1},{-1,-1},{-1,-1},{-1,-1},{-1,-1},{-1,-1},{-1,-1},{-1,-1}},	//5
						{	{-1,-1},{-1,-1},{-1,-1},{-1,-1},{-1,-1},{420,S},{-1,-1},{53,E },{-1,-1},{-1,-1},{-1,-1},{103,N},{-1,-1},{-1,-1},{-1,-1},{-1,-1},{-1,-1},{-1,-1},{-1,-1},{-1,-1},{-1,-1},{-1,-1},{-1,-1},{-1,-1}},	//6
						{	{-1,-1},{-1,-1},{-1,-1},{-1,-1},{-1,-1},{-1,-1},{53,W },{-1,-1},{91,E },{-1,-1},{-1,-1},{-1,-1},{-1,-1},{-1,-1},{-1,-1},{-1,-1},{-1,-1},{-1,-1},{-1,-1},{-1,-1},{-1,-1},{-1,-1},{-1,-1},{-1,-1}},	//7
						{	{-1,-1},{-1,-1},{88,S },{-1,-1},{-1,-1},{-1,-1},{-1,-1},{91,W },{-1,-1},{-1,-1},{-1,-1},{-1,-1},{-1,-1},{-1,-1},{240,N},{-1,-1},{-1,-1},{-1,-1},{-1,-1},{-1,-1},{-1,-1},{-1,-1},{-1,-1},{-1,-1}},	//8
						{	{46,S },{-1,-1},{-1,-1},{-1,-1},{-1,-1},{-1,-1},{-1,-1},{-1,-1},{-1,-1},{-1,-1},{-1,-1},{-1,-1},{67,N },{-1,-1},{-1,-1},{-1,-1},{-1,-1},{-1,-1},{-1,-1},{-1,-1},{-1,-1},{-1,-1},{-1,-1},{-1,-1}},	//9
						{	{-1,-1},{-1,-1},{-1,-1},{-1,-1},{-1,-1},{103,W},{-1,-1},{-1,-1},{-1,-1},{-1,-1},{-1,-1},{-1,-1},{107,E},{-1,-1},{-1,-1},{223,N},{-1,-1},{-1,-1},{-1,-1},{-1,-1},{-1,-1},{-1,-1},{-1,-1},{-1,-1}},	//10
						{	{-1,-1},{-1,-1},{-1,-1},{-1,-1},{-1,-1},{-1,-1},{103,E},{-1,-1},{-1,-1},{-1,-1},{-1,-1},{-1,-1},{107,W},{-1,-1},{-1,-1},{-1,-1},{223,N},{-1,-1},{-1,-1},{-1,-1},{-1,-1},{-1,-1},{-1,-1},{-1,-1}},	//11
						{	{-1,-1},{-1,-1},{-1,-1},{-1,-1},{-1,-1},{-1,-1},{-1,-1},{-1,-1},{-1,-1},{67,S },{107,W},{107,E},{-1,-1},{-1,-1},{-1,-1},{-1,-1},{-1,-1},{-1,-1},{-1,-1},{-1,-1},{290,N},{-1,-1},{-1,-1},{-1,-1}},	//12
						{	{-1,-1},{-1,-1},{-1,-1},{240,S},{-1,-1},{-1,-1},{-1,-1},{-1,-1},{-1,-1},{-1,-1},{-1,-1},{-1,-1},{-1,-1},{-1,-1},{-1,-1},{126,E},{-1,-1},{248,N},{-1,-1},{-1,-1},{-1,-1},{-1,-1},{-1,-1},{-1,-1}},	//13
						{	{-1,-1},{-1,-1},{-1,-1},{-1,-1},{-1,-1},{-1,-1},{-1,-1},{-1,-1},{240,S},{-1,-1},{-1,-1},{-1,-1},{-1,-1},{-1,-1},{-1,-1},{-1,-1},{126,W},{-1,-1},{-1,-1},{-1,-1},{-1,-1},{-1,-1},{-1,-1},{248,N}},	//14
						{	{-1,-1},{-1,-1},{-1,-1},{-1,-1},{-1,-1},{-1,-1},{-1,-1},{-1,-1},{-1,-1},{-1,-1},{223,S},{-1,-1},{-1,-1},{126,W},{-1,-1},{-1,-1},{-1,-1},{-1,-1},{132,N},{-1,-1},{-1,-1},{-1,-1},{-1,-1},{-1,-1}},	//15
						{	{-1,-1},{-1,-1},{-1,-1},{-1,-1},{-1,-1},{-1,-1},{-1,-1},{-1,-1},{-1,-1},{-1,-1},{-1,-1},{223,S},{-1,-1},{-1,-1},{126,E},{-1,-1},{-1,-1},{-1,-1},{-1,-1},{-1,-1},{-1,-1},{-1,-1},{132,N},{-1,-1}},	//16
						{	{-1,-1},{-1,-1},{-1,-1},{-1,-1},{-1,-1},{-1,-1},{-1,-1},{-1,-1},{-1,-1},{-1,-1},{-1,-1},{-1,-1},{-1,-1},{248,S},{-1,-1},{-1,-1},{-1,-1},{-1,-1},{92,E },{-1,-1},{-1,-1},{-1,-1},{-1,-1},{-1,-1}},	//17
						{	{-1,-1},{-1,-1},{-1,-1},{-1,-1},{-1,-1},{-1,-1},{-1,-1},{-1,-1},{-1,-1},{-1,-1},{-1,-1},{-1,-1},{-1,-1},{-1,-1},{-1,-1},{132,S},{-1,-1},{92,W },{-1,-1},{92,E },{-1,-1},{-1,-1},{-1,-1},{-1,-1}},	//18
						{	{-1,-1},{-1,-1},{-1,-1},{-1,-1},{-1,-1},{-1,-1},{-1,-1},{-1,-1},{-1,-1},{-1,-1},{-1,-1},{-1,-1},{-1,-1},{-1,-1},{-1,-1},{-1,-1},{-1,-1},{-1,-1},{92,W },{-1,-1},{92,E },{-1,-1},{-1,-1},{-1,-1}},	//19
						{	{-1,-1},{-1,-1},{-1,-1},{-1,-1},{-1,-1},{-1,-1},{-1,-1},{-1,-1},{-1,-1},{-1,-1},{-1,-1},{-1,-1},{290,S},{-1,-1},{-1,-1},{-1,-1},{-1,-1},{-1,-1},{-1,-1},{92,W },{-1,-1},{92,E },{-1,-1},{-1,-1}},	//20
						{	{-1,-1},{-1,-1},{-1,-1},{-1,-1},{-1,-1},{-1,-1},{-1,-1},{-1,-1},{-1,-1},{-1,-1},{-1,-1},{-1,-1},{-1,-1},{-1,-1},{-1,-1},{-1,-1},{-1,-1},{-1,-1},{-1,-1},{-1,-1},{92,W },{-1,-1},{92,E },{-1,-1}},	//21
						{	{-1,-1},{-1,-1},{-1,-1},{-1,-1},{-1,-1},{-1,-1},{-1,-1},{-1,-1},{-1,-1},{-1,-1},{-1,-1},{-1,-1},{-1,-1},{-1,-1},{-1,-1},{-1,-1},{132,S},{-1,-1},{-1,-1},{-1,-1},{-1,-1},{92,W },{-1,-1},{92,E }},	//22
						{	{-1,-1},{-1,-1},{-1,-1},{-1,-1},{-1,-1},{-1,-1},{-1,-1},{-1,-1},{-1,-1},{-1,-1},{-1,-1},{-1,-1},{-1,-1},{-1,-1},{248,S},{-1,-1},{-1,-1},{-1,-1},{-1,-1},{-1,-1},{-1,-1},{-1,-1},{92,W },{-1,-1}},	//23
					  };

/*
*	Variable name: priority_node
*	1D array that stores details of the priority_node (node to be branched) to be used in Dijikstra`s algorithm
*/
int priority_node[3];

/*
*	Variable name: priority_queue
*	Acts as priority queue in Dijikstra`s algorithm and stores details of corresponding nodes
*/
int priority_queue[24][3];

/*
*	Variable name: node_pile
*	Holds the stack of previous priority nodes and helps in creation of final path
*/
int node_pile[24][3];

/*
*	Variable name: path
*	Array to store node no.s of final path
*/
char path[24];

/*
*	Variable name: path_dir
*	Array to store directions for bot to reach end node
*/
char path_dir[24];

/*
*	Variable name: prev_node
*	Stores the previous travelled node number
*/
char prev_node = 0;

/*
*	Variable name: curr_node
*	Stores the current node number
*/
char curr_node = 0;

/*
*	Variable name: next_node
*	Stores the next node number to be travelled
*/
char next_node = 0;

// colour of the nut picked
int nut_color;

// number of nuts picked
int nuts_picked = 0;

// tell the occupancy status of the drop locations
unsigned char occupied[4] = { 0,0,0,0 };

#define confidence_max 20			//max value of confidence in software debouncing
#define confidence_thresh 14		//threshold for dececion in software debouncing

/* The colours are given specific numerical codes*/
#define clear 0
#define red 1
#define green 2
#define brown 3

/*
*
* Function Name: ir_array
* Input: NONE
* Output: unsigned char -> returns a combined value corresponding to the white line sensor data (only three LSBs or least significant bits of the char is used)
* Logic: Sets a threshold value to distinguish white and black and sets 1 for black and 0 for white
*		Threshold = 180, if adc_value > 180 => 1 (black), and vice versa
* Example Call: ir_array(); //Returns one of 0(0b000), 1(0b001), 2(0b010), ....., 7(0b111).
*
*/
unsigned char ir_array(void)
{
	unsigned char left_sensor, centre_sensor, right_sensor;									//variables that store the adc values of the white line sensors
	left_sensor = ADC_Conversion(1);
	centre_sensor = ADC_Conversion(2);
	right_sensor = ADC_Conversion(3);
	return ((left_sensor > 180) * 4 + (centre_sensor > 180) * 2 + (right_sensor > 180));	//converting adc values into a single decimal number based on the binary value
}

/*
*
* Function Name: line_track
* Input: NONE
* Output: Follows the black line with either two or three sensors based on 'line_memory' value
* Logic:
*	When tracking with three sensors:
*		If, centre sensor is on line => it goes straight, left sensor on line => slight right, right sensor on line => slight left
*		If two consecutive sensors are on the line, correspondingly it goes straight
*	When tracking with two sensors: (This is done to avoid conflict at certain nodes where the line is thick on one side)
*		Based on the 'line_memory' value, it tracks using left-centre or right-centre sensors
*	When it is out of line, then it moves based on the previous movement stored in 'line_track_memory'
*	When a node is detected (all three sensors are on black) , the control exits the line track function (software debouncing is done here)
*Example Call: line_track();
*
*/
void line_track(void)
{
	int confidence = 0;								// counter variable for software debounce 
	while (1)
	{
		velocity(250, 250);
		if (ir_array() == 0b010)					// only centre sensor on black line
		{
			if (line_memory == 2)					// line track with right-centre sensors 
			{
				forward(); velocity(125, 250);		// curve left
			}
			else if (line_memory == 3)				// line track with left-centre sensors
			{
				forward(); velocity(250, 125);		// curve right
			}
			else
				forward();							// when tracking with all sensors
		}
		else if (ir_array() == 0b001)				// only right sensor on black	
		{
			if (line_memory == 2)					// line track with right-centre sensors 
			{
				forward(); velocity(250, 125);		// curve right
			}
			else
				soft_right();
			line_track_memory = 1;					// store the movement 
		}
		else if (ir_array() == 0b100)				// only left sensor on black	
		{
			if (line_memory == 3)					// line track with left-centre sensors
			{
				forward(); velocity(125, 250);		// curve left
			}
			else
				soft_left();
			line_track_memory = 0;					// store the movement 
		}
		else if (ir_array() == 0b011)				// right-centre sensor on black	
		{
			if (line_memory == 3)					// line track with left-centre sensors
				soft_right();
			else
				forward();
			line_track_memory = 1;					// store the movement 
		}
		else if (ir_array() == 0b110)				// left-centre sensor on black	
		{
			if (line_memory == 2)					// line track with right-centre sensors 
				soft_left();
			else
				forward();
			line_track_memory = 0;					// store the movement 
		}
		else if (ir_array() == 0b101)				// rare case of left-right sensors
		{
			if (line_memory == 0)
				soft_left();						//line_memory = 0 => if previous was movement right_turn_wls, right is given priority
			else if (line_memory == 1)
				soft_right();						//line_memory = 1 => if previous was movement left_turn_wls, left is given priority
			else if (line_memory == 2)				// line track with right-centre sensors 
			{
				left();
			}
			else if (line_memory == 3)				// line track with left-centre sensors
			{
				right();
			}
		}
		else if (ir_array() == 0)					// out of line
		{
			if (line_track_memory == 0)
				soft_left();
			else if (line_track_memory == 1)
				soft_right();
		}
		else										// node detected
		{
			stop();
			for (int i = 0; i < 10; i++)			// Confirms node detection via software debouncing
			{
				if (ir_array() == 7)
				{
					confidence++;
				}
				_delay_ms(1);
			}
			if (confidence > 5)
				break;								// breaks from loop (eventually function) after confirmation
			confidence = 0;
		}
		_delay_ms(1);
	}
}


/*
*
* Function Name: back_track
* Input: NONE
* Output: Follows the black line with either two or three sensors based on 'line_memory' value
* Logic:
*	When an obstracle is detected, the bot back-tracks the line without a U-turn.
*	This is done to avoid miscount of nodes when the blocks are at critical positions.
*	When tracking with three sensors:
*		If, centre sensor is on line => it goes straight, left sensor on line => slight right, right sensor on line => slight left
*		If two consecutive sensors are on the line, correspondingly it goes straight
*	When tracking with two sensors: (This is done to avoid conflict at certain nodes where the line is thick on one side)
*		Based on the 'line_memory' value, it tracks using left-centre or right-centre sensors
*	When it is out of line, then it moves based on the previous movement stored in 'line_track_memory'
*	When a node is detected (all three sensors are on black) , the control exits the line track function (software debouncing is done here)
*Example Call: back_track();
*
*/
void back_track(void)
{
	int confidence = 0;								// counter variable for software debounce 
	while (1)
	{
		// Type the algorithm here


		// Software debouncing
												// node detected
		{
			stop();
			for (int i = 0; i < 10; i++)			// Confirms node detection via software debouncing
			{
				if (ir_array() == 7)
				{
					confidence++;
				}
				_delay_ms(1);
			}
			if (confidence > 5)
				break;								// breaks from loop (eventually function) after confirmation
			confidence = 0;
		}
		_delay_ms(1);
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
	while (node-- > 0)									// runs for 'node' number of times (number of nodes)
	{
		int confidence = 0;								// counter variable for software debounce 
		forward(); velocity(250, 250);
		while (1)
		{
			for (int i = 0; i < confidence_max; i++)	// To confirm when out of a node via software debouncing
			{
				if (ir_array() != 0b111)
					confidence++;						// Increment 'confidence' when past node 
				if (ir_array() == 0b011)				// left sensor out of line
				{
					forward(); velocity(250, 125);		// curve right
				}
				if (ir_array() == 0b110)				// right sensor out of line
				{
					forward(); velocity(125, 250);		// curve left
				}
			}
			if (confidence > confidence_thresh)
				break;									// confirms and breaks out of the loop
			_delay_ms(1);
			confidence = 0;								// resets confidence value
		}
		forward();	velocity(150, 150);
		if (obstracle_front == 1)
			_delay_ms(63);								// to align the centre of the bot close to the node and prevent false detection when there is an independant black line (obstracle) in the front (eg. first node)
		else
			_delay_ms(100);								// to align the centre of the bot on the node to ease turning
		stop();
	}
}

/*
*
* Function Name: left_turn_wls
* Input: int
* Output: void
* Logic: Uses white line sensors to turn left until black line is encountered, for specified no. of times as parameter
* Example Call: left_turn_wls(1); //Turns left once until black line is encountered
*
*/
void left_turn_wls(int count)
{
	while (count-- > 0)
	{
		int confidence = 0;									// counter variable for software debounce
		left();		velocity(150, 150);
		_delay_ms(120);
		for (int i = 0; i < confidence_max; i++)			// confirms if left sensor is on black line
		{
			if ((ir_array() & 0b100) == 0b100)
				confidence++;
		}
		if (confidence > confidence_thresh)
		{
			left();		velocity(150, 150);
			while (1)										// if left sensor is on black, turn left until it is out of the black line
			{
				confidence = 0;								// resets 'confidence' variable
				for (int i = 0; i < confidence_max; i++)	// confirms if out of line via software debouncing
				{
					if ((ir_array() & 0b100) == 0)
						confidence++;
				}
				if (confidence > confidence_thresh)
					break;									// confirms and breaks out of the loop
			}
		}
		while (1)
		{
			left();											// Turn left until left ir sensor detects a black line
			if (line_memory == 3)
				velocity(0, 100);							// soft left at critical left turns (eg. last before node)
			else
				velocity(100, 100);
			confidence = 0;
			for (int i = 0; i < confidence_max; i++)		// confirms if black line is detected by left sensor
			{
				if ((ir_array() & 0b100) == 0b100)
					confidence++;
			}
			if (confidence > confidence_thresh)				// confirms and breaks out of the loop
				break;
		}
		_delay_ms(20);
		stop();
		_delay_ms(100);										// to stabilize after a turn
		if (line_memory_rw)
			line_memory = 0;								//	updates 'line_memory' to set line track preference to left turn at 0b101 case
	}
}

/*
*
* Function Name: right_turn_wls
* Input: int
* Output: void
* Logic: Uses white line sensors to turn right until black line is encountered, for specified no. of times as parameter
* Example Call: right_turn_wls(1); //Turns right once until black line is encountered
*
*/
void right_turn_wls(int count)
{
	while (count-- > 0)
	{
		int confidence = 0;									// counter variable for software debounce
		right();		velocity(150, 150);
		_delay_ms(120);
		for (int i = 0; i < confidence_max; i++)			// confirms if right sensor is on black line
		{
			if ((ir_array() & 0b001) == 0b001)
				confidence++;
		}
		if (confidence > confidence_thresh)
		{
			right();
			while (1)										// if right sensor is on black, turn right until it is out of the black line
			{
				confidence = 0;								// resets 'confidence' variable
				for (int i = 0; i < confidence_max; i++)	// confirms if out of line via software debouncing
				{
					if ((ir_array() & 1) == 0)
						confidence++;
				}
				if (confidence > confidence_thresh)
					break;									// confirms and breaks out of the loop
			}
		}
		while (1)
		{
			right();		velocity(100, 100);				// Turn right until right ir sensor detects a black line
			if (line_memory == 2)
				velocity(0, 100);							// soft right at critical right turns
			else
				velocity(100, 100);
			confidence = 0;
			for (int i = 0; i < confidence_max; i++)		// confirms if black line is detected by right sensor
			{
				if ((ir_array() & 1) == 1)
					confidence++;
			}
			if (confidence > confidence_thresh)				// confirms and breaks out of the loop
				break;
		}
		_delay_ms(20);
		stop();
		_delay_ms(100);										// to stabilize after a turn
		if (line_memory_rw)
			line_memory = 1;								//	updates 'line_memory' to set line track preference to right turn at 0b101 case
	}
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
}

/*
*
* Function Name: F
* Input: void
* Output: void
* Logic: Use this function to cross a node, track the line and stop at the next node
* Example Call: F();
*
*/
void F()
{
	forward_wls(1);
	line_track();
}

/*
*
* Function Name: L
* Input: void
* Output: void
* Logic: Use this function to cross a node, then turns left for specified no. of times and finally stop at the next node
* Example Call: L();
*
*/
void L(int count)
{
	forward_wls(1);
	left_turn_wls(count);
	line_track();
}

/*
*
* Function Name: R
* Input: int
* Output: void
* Logic: Use this function to cross a node, then turns right for specified no. of times and finally stop at the next node
* Example Call: R();
*
*/
void R(int count)
{
	forward_wls(1);
	right_turn_wls(count);
	line_track();
}

/*
*
* Function Name: align
* Input: void
* Output: void
* Logic: Use this function to cross a align the bot parallel to the line
* Example Call: align();
*
*/
void align()
{
	while (1)
	{
		if (ir_array() == 0b001 || ir_array() == 0b011)
		{
			right();	velocity(25, 25);
		}
		else if (ir_array() == 0b100 || ir_array() == 0b110)
		{
			left();		velocity(25, 25);
		}
		if (ir_array() == 0b010)
		{
			stop();												//stop and break after aligned
			break;
		}
	}
	stop();
}

/*
*
* Function Name: filter_color
* Input: void
* Output: int
* Logic: The function filters the colour and returns the corresponding colour code for the colour of the block
* Example Call: filter_color();
*
*/
int filter_color()
{
	int r = 0, g = 0, b = 0;														// variables to store r, g, b values
	filter_red();
	r = color_sensor_pulse_count;													// gets r value
	filter_green();
	g = color_sensor_pulse_count;													// gets g value
	filter_blue();
	b = color_sensor_pulse_count;													// gets b value
	if (r > 3000 && g < 1000 && b < 1000)
		return red;																	// determines if red
	else if (r < 1000 && g>3000 && b < 1500)
		return green;																// determines if green
	else if (r > 2000 && r < 4000 && g > 1000 && g < 3000 && b > 500 && b < 2000)
		return brown;																// determines if brown
	return clear;																	// determines if none of the above colours
}

/*
*
* Function Name: pick_nut
* Input: void
* Output: int
* Logic: The function pick the nut after checking if it is a nut
* Example Call: pick_nut();
*
*/
int pick_nut()
{
	forward_wls(1);
	printf("\n\n %d \t %d \t %d\n\n",curr_node,prev_node, maze[curr_node][prev_node][1]);
	if (maze[curr_node][prev_node][1] == E)
	{
		right();	velocity(150, 150);
		_delay_ms(100);
		right_turn_wls(1);
	}
	else if (maze[curr_node][prev_node][1] == W)
	{
		left();		velocity(150, 150);
		_delay_ms(100);
		left_turn_wls(1);
	}
	align();								// aligns the bot
	//while (ADC_Conversion(4) > 60)			// moves bot to the proximity of the object
	//{
	//	forward();
	//}
	_delay_ms(100);
	stop();
	if (filter_color() == red)				// picks red nut
	{
		pick();
		return red;
	}
	else if (filter_color() == green)		// picks green nut
	{
		pick();
		return green;
	}
	else if (filter_color() == brown)		// does not pick obstacles
	{
		return brown;
	}
	return clear;							// no object to pick
}

/*
*
* Function Name: place_nut
* Input: void
* Output: void
* Logic: The function place the nut in the location
* Example Call: place_nut();
*
*/
void place_nut()
{
	forward_wls(1);
	//printf("\n\n %d \t %d \t %d\n\n", curr_node, prev_node, maze[curr_node][prev_node][1]);
	if (maze[curr_node][prev_node][1] == W)
		right_turn_wls(1);
	else if (maze[curr_node][prev_node][1] == E)
		left_turn_wls(1);
	align();
	stop();								// aligns the bot
	place();								// places the nut in the location
	nuts_picked += 1;
}

/*
* Function Name: generate_path
* Input: char,char
* Output: void
* Logic: Generates the shortest/fastest path using Djikstra`s algorithm between the inputed start and end nodes and stores nodes in path[] & path directions in path_dir[]
* Example Call: generate_path(2,13);
*/
void generate_path(char start_node, char end_node)
{
	int i_pq = 0, swap_pq[3], pq_node_repeat, i_np = 0, i_path = 1, swap_path;
	
	//priority_node = start_node
	priority_node[0] = start_node;	priority_node[1] = 0;	priority_node[2] = 0;
	do {
		//Dead End Cases elimination
		if ((start_node == 9 && end_node == 0) || start_node == end_node) {
			priority_node[1] = start_node;
			break;
		}
		//Creating/adding to priority_queue
		for (int i = 0; i < 24; i++)
		{
			if (maze[priority_node[0]][i][0] != -1 && i != priority_node[1]) {
				pq_node_repeat = 0;
				for (int j = 0; j < i_pq; j++)
					if (priority_queue[j][0] == i) {
						pq_node_repeat = 1;
						if (priority_node[2] + maze[priority_node[0]][i][0] < priority_queue[j][2]) {
							priority_queue[j][2] = priority_node[2] + maze[priority_node[0]][i][0];
							priority_queue[j][1] = priority_node[0];
						}
						break;
					}
				for (int j = 0; j < i_np; j++)
					if (node_pile[j][0] == i) {
						pq_node_repeat = 1;
						break;
					}
				if (pq_node_repeat == 0) {
					priority_queue[i_pq][0] = i;	priority_queue[i_pq][1] = priority_node[0];		priority_queue[i_pq][2] = priority_node[2] + maze[priority_node[0]][i][0];
					i_pq += 1;
				}
			}
		}
		priority_queue[i_pq][0] = -1;

		//Sorting priority_queue
		for (int i = 0; i < i_pq; i++) {
			for (int j = i + 1; j < i_pq; j++) {
				if (priority_queue[j][2] < priority_queue[i][2]) {
					for (int k = 0; k < 3; k++) {
						swap_pq[k] = priority_queue[i][k];
						priority_queue[i][k] = priority_queue[j][k];
						priority_queue[j][k] = swap_pq[k];
					}
				}
			}
		}

		//Add priority_node to node_pile
		for (int i = 0; i < 3; i++)
			node_pile[i_np][i] = priority_node[i];
		i_np += 1;

		//Update priority_node
		for (int i = 0; i < 3; i++)
			priority_node[i] = priority_queue[0][i];
		//Move priority_queue by 1 step
		for (int i = 1; i < 24 && priority_queue[i][0] != -1; i++)
			for (int k = 0; k < 3; k++)
				priority_queue[i - 1][k] = priority_queue[i][k];
		priority_queue[--i_pq][0] = -1;
	} while (priority_node[0] != end_node);		//Djikstra`s Algorithm stops only when the end node reaches the top of the priority queue

	//Cration of path matrix by backtracking
	path[0] = end_node;
	path[1] = priority_node[1];
	while (1)
	{
		if (path[1] == start_node)
			break;
		if (node_pile[--i_np][0] == path[i_path])
		{
			path[++i_path] = node_pile[i_np][1];
			if (path[i_path] == start_node)
				break;
		}
	}
	
	//Reversing path matrix to ascending order of path
	for (int i = 0; i <= i_path / 2; i++) {
		swap_path = path[i];
		path[i] = path[i_path - i];
		path[i_path - i] = swap_path;
	}
	path[i_path + 1] = -1;

	if (path[0] == path[1])
	{
		path[1] = -1;
	}
}

void orient(int apr_dir)
{
	printf("\n###########################apr_dir=%d#####################\n", apr_dir);
	char sd_complete,sd_else;	//flags for same direction case
	for (int index = 0; path[index+1] != -1; index++)
	{
		curr_node = path[index];
		if (index)
			prev_node = path[index - 1];
		next_node = path[index + 1];
		printf("\n %d \t %d \t %d", curr_node, prev_node, next_node);
		// target direction
		int tar_dir;
		if (!(apr_dir != -1 && index == 0 ))
			apr_dir = maze[curr_node][prev_node][1];
		printf("\t%d", apr_dir);
		tar_dir = maze[curr_node][next_node][1];
		if (apr_dir - tar_dir == 1 || apr_dir - tar_dir == -3)
		{
			R(1);
		}
		else if (apr_dir - tar_dir == -1 || apr_dir - tar_dir == 3)
		{
			L(1);
		}
		else if (apr_dir - tar_dir == 2 || apr_dir - tar_dir == -2)
		{
			F();
		}
		else
		{
			if (apr_dir == N)
			{
				sd_complete = 0; sd_else = 1;
				for(int j=0;j<24;j++)
					if (maze[curr_node][j][1] == W) {
						sd_complete = 1;
						break;
					}
				if (!sd_complete) {
					R(1); sd_else = 0;
				}
				if (sd_complete) {
					sd_complete = 0;
					for (int j = 0; j < 24; j++)
						if (maze[curr_node][j][1] == E) {
							sd_complete = 1;
							break;
						}
					if (!sd_complete) {
						L(1); sd_else = 0;
					}
				}
				if (sd_else)
					R(2);
			}
			else if (apr_dir == E)
			{

			}
			else if (apr_dir == W)
			{

			}
			else
			{
				sd_complete = 0; sd_else = 1;
				printf("\nIN\n");
				for (int j = 0; j < 24; j++)
					if (maze[curr_node][j][1] == W) {
						sd_complete = 1;
						break;
					}
				if (!sd_complete) {
					L(1); sd_else = 0;
				}
				if (sd_complete) {
					sd_complete = 0;
					for (int j = 0; j < 24; j++)
						if (maze[curr_node][j][1] == E) {
							sd_complete = 1;
							break;
						}
					if (!sd_complete) {
						R(1); sd_else = 0;
					}
				}
				if (sd_else)
					L(2);
				printf("\nOUT\n");
			}
		}
	}
	if (path[1] != -1)
	{
		prev_node = curr_node;
		curr_node = next_node;
	}
}

/*
*
* Function Name: Task_1_1
* Input: void
* Output: void
* Logic: Use this function to encapsulate the Task 1.1 logic
* Example Call: Task_1_1();
*/
void Task_1_1(void)
{
	_delay_ms(10);								// Wait for the microcontroller to start running this code
	line_memory = 3;							// Set left-centre line tracking
	line_memory_rw = 0;							// Set read-only mode for 'line_memory'
	line_track();
	obstracle_front = 1;						// Reduces distance travelled to align the bot after crossing a node to avoid false tracking of independant line present in vicinity
	R(1);
	obstracle_front = 0;						// Sets distance travelled to align the bot after crossing a node to default value
	line_memory_rw = 1;							// Set read-write mode for 'line_memory'
	L(1);	L(1);	L(1);	R(1);	F();
	line_memory = 3;							// Set left-centre line tracking
	line_memory_rw = 0;							// Set read-only mode for 'line_memory'
	F();
	line_memory_rw = 1;							// Set read-write mode for 'line_memory'
	R(1);	L(1);	L(1);
	line_memory = 3;							// Set left-centre line tracking
	line_memory_rw = 0;							// Set read-only mode for 'line_memory'
	L(1);
	line_memory_rw = 1;							// Set read-write mode for 'line_memory'
	R(1);
	stop();
	_delay_ms(3000);							// Display for 3 secs after completion
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
	stop();
	_delay_ms(2000);
	stop();
	// switch variable to check if placed
	int toggle = 0;

	line_track();
	curr_node = 9;
	prev_node = 0;

	for (int i = 17; i <= 23; i++)
	{
		if (i == 20)
			continue;
		generate_path(curr_node, i);
		if (!toggle)
			orient(-1);
		else
			orient(N);
		toggle = 0;
		nut_color = pick_nut();
		printf("\n************ %d ***********\n ", nut_color);
		if (nut_color == red)
		{
			if (!occupied[0])
			{
				generate_path(i, 1);
				occupied[0] = 1;
			}
			else
			{
				generate_path(i, 4);
				occupied[1] = 1;
			}
			orient(S);
			place_nut();
			toggle = 1;
		}
		else if (nut_color == green)
		{
			if (!occupied[2])
			{
				generate_path(i, 7);
				occupied[2] = 1;
			}
			else
			{
				generate_path(i, 2);
				occupied[3] = 1;
			}
			orient(S);
			place_nut();
			toggle = 1;
		}
		else if (nut_color == clear)
		{
			if (i == 23)
				break;
		//	if (i != 19)
				generate_path(i, i + 1);
		//	else
		//		generate_path(i, i + 2);
			orient(S);
		}
		if (nuts_picked == 4)
			break;
	}

	generate_path(curr_node, 0);
	orient(N);
	left();	velocity(230, 230);
	_delay_ms(1000);
	/*pick_nut();
	_delay_ms(10000);
	place_nut();*/
	/*
	for (int k = 0; k < 24; k++)
	{
		for (int l = 0; l < 24; l++)
		{
			generate_path(k, l);
			printf("\n");
			for (int i = 0; path[i] != -1; i++)
				printf("%d\t", path[i]);
		}
	}*/
}
