#include "project.h"

//*****************************************************************************
//
// The error routine that is called if the driver library encounters an error.
//
//*****************************************************************************

//Create definitions for Port B registers
#define GPIO_PORTB_DATA_R       (*((volatile unsigned long *)0x400053FC))
#define GPIO_PORTB_DIR_R        (*((volatile unsigned long *)0x40005400))
#define GPIO_PORTB_PUR_R        (*((volatile unsigned long *)0x40005510))
#define GPIO_PORTB_DEN_R        (*((volatile unsigned long *)0x4000551C))
#define GPIO_PORTB_CR_R         (*((volatile unsigned long *)0x40005524))
#define GPIO_PORTB_AMSEL_R      (*((volatile unsigned long *)0x40005528))
#define GPIO_PORTB_PCTL_R       (*((volatile unsigned long *)0x4000552C))
#define SYSCTL_RCGC2_R          (*((volatile unsigned long *)0x400FE108))

// State Machine Variables
#define n0 0x40; //0100 0000  Display 0        
#define n1 0x79; //0111 1001  Display 1
#define n2 0x24; //0010 0100  Display 2 
#define n3 0x30; //0011 0000  Display 3
#define n4 0x19; //0001 1001  Display 4
#define n5 0x12; //0001 0010  Display 5
#define n6 0x02; //0000 0010  Display 6
#define n7 0x78; //0111 1000  Display 7
#define n8 0x00; //0000 0000  Display 8
#define n9 0x10; //0001 0000  Display 9


uint16_t bright;
uint16_t number;

void PortB_Init(void);
void Delay(void);
void Delay2(void);
int DistanceToLeds(int a);
int sevenSegDisplay(int b);

#ifdef DEBUG
void
__error__(char *pcFilename, uint32_t ui32Line)
{
}
#endif


void SetupHardware()
{
	UartSetup();
}

int  main(void)
{
ClockSetup();
CAN_Init();
initReceiver();


while(1)
{
	bright = MsgData[0][1];
	number = MsgData[1][1];
}

//bright = MsgData[0][1];

DistanceToLeds(bright);
sevenSegDisplay(number);	

}
void PortB_Init(void){ 
  volatile unsigned long delay;
  SYSCTL_RCGC2_R |= 0x00000002;     // 1) B clock
	delay = 0x00000020;           // reading register adds a delay   
//  delay = SYSCTL_RCGC2_R;           // reading register adds a delay   
  GPIO_PORTB_CR_R = 0x7F;           // allow changes to PF4-0       
  GPIO_PORTB_AMSEL_R = 0x00;        // 3) disable analog function
  GPIO_PORTB_PCTL_R = 0x00000000;   // 4) GPIO clear bit PCTL  
  GPIO_PORTB_DIR_R = 0x7F;          // 5) PB6-PB0 output   
  GPIO_PORTB_AFSEL_R = 0x00;        // 6) no alternate function
  GPIO_PORTB_PUR_R = 0x00;          // enable pullup resistors on PF4,PF0       
  GPIO_PORTB_DEN_R = 0x7F;          // 7) enable digital pins PF4-PF0        
}


int sevenSegDisplay(int num)
{
PortB_Init();        			// Call initialization of port PF0-PF4   
	
	switch(num) {
			case 0: 
				GPIO_PORTB_DATA_R = n0; // Assign value in n0 to output pins 
				break;
			case 1: 
				GPIO_PORTB_DATA_R = n1;
			  break;
			case 2: 
				GPIO_PORTB_DATA_R = n2;
				break;
			case 3: 
				GPIO_PORTB_DATA_R = n3;
				break;
			case 4: 
				GPIO_PORTB_DATA_R = n4;
				break;
			case 5: 
				GPIO_PORTB_DATA_R = n5;
				break;
			case 6: 
				GPIO_PORTB_DATA_R = n6;
				break;
			case 7: 
				GPIO_PORTB_DATA_R = n7;
				break;
			case 8: 
				GPIO_PORTB_DATA_R = n8;
				break;
			case 9: 
				GPIO_PORTB_DATA_R = n9;
				break;
	 }
}


int DistanceToLeds(int n)
{
uint16_t brightness;
	
	SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOD);
  brightness = n;	
	
	  if (brightness > 50)
	 {
		 GPIOPinTypeGPIOOutput(GPIO_PORTD_BASE, GPIO_PIN_0);
		 GPIOPinWrite(GPIO_PORTD_BASE, GPIO_PIN_0, 0xF);
	 }
	 else if (brightness < 50 && brightness > 35)
	 {
		 GPIOPinTypeGPIOOutput(GPIO_PORTD_BASE, GPIO_PIN_0);
		 GPIOPinWrite(GPIO_PORTD_BASE, GPIO_PIN_0, 0xF);
		 GPIOPinTypeGPIOOutput(GPIO_PORTD_BASE, GPIO_PIN_1);
		 GPIOPinWrite(GPIO_PORTD_BASE, GPIO_PIN_1, 0xF);
	 }
	 else if (brightness < 35 && brightness > 20)
	 {
		 GPIOPinTypeGPIOOutput(GPIO_PORTD_BASE, GPIO_PIN_0);
		 GPIOPinWrite(GPIO_PORTD_BASE, GPIO_PIN_0, 0xF);
		 GPIOPinTypeGPIOOutput(GPIO_PORTD_BASE, GPIO_PIN_1);
		 GPIOPinWrite(GPIO_PORTD_BASE, GPIO_PIN_1, 0xF);
		 GPIOPinTypeGPIOOutput(GPIO_PORTD_BASE, GPIO_PIN_2);
		 GPIOPinWrite(GPIO_PORTD_BASE, GPIO_PIN_2, 0xF);
	 }
	 else if (brightness < 20 && brightness > 10)
	 {
		 GPIOPinTypeGPIOOutput(GPIO_PORTD_BASE, GPIO_PIN_0);
		 GPIOPinWrite(GPIO_PORTD_BASE, GPIO_PIN_0, 0xF);
		 GPIOPinTypeGPIOOutput(GPIO_PORTD_BASE, GPIO_PIN_1);
		 GPIOPinWrite(GPIO_PORTD_BASE, GPIO_PIN_1, 0xF);
		 GPIOPinTypeGPIOOutput(GPIO_PORTD_BASE, GPIO_PIN_2);
		 GPIOPinWrite(GPIO_PORTD_BASE, GPIO_PIN_2, 0xF);
		 GPIOPinTypeGPIOOutput(GPIO_PORTD_BASE, GPIO_PIN_3);
		 GPIOPinWrite(GPIO_PORTD_BASE, GPIO_PIN_3, 0xF);
	 }
	 else if (brightness < 10 && brightness >= 0)
	 {
		 GPIOPinTypeGPIOOutput(GPIO_PORTD_BASE, GPIO_PIN_0);
		 GPIOPinWrite(GPIO_PORTD_BASE, GPIO_PIN_0, 0xF);
		 GPIOPinTypeGPIOOutput(GPIO_PORTD_BASE, GPIO_PIN_1);
		 GPIOPinWrite(GPIO_PORTD_BASE, GPIO_PIN_1, 0xF);
		 GPIOPinTypeGPIOOutput(GPIO_PORTD_BASE, GPIO_PIN_2);
		 GPIOPinWrite(GPIO_PORTD_BASE, GPIO_PIN_2, 0xF);
		 GPIOPinTypeGPIOOutput(GPIO_PORTD_BASE, GPIO_PIN_3);
		 GPIOPinWrite(GPIO_PORTD_BASE, GPIO_PIN_3, 0xF);
		 GPIOPinTypeGPIOOutput(GPIO_PORTD_BASE, GPIO_PIN_6);
		 GPIOPinWrite(GPIO_PORTD_BASE, GPIO_PIN_6, 0xF);
	 }
}