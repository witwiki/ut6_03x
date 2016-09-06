/**

UTAustinX: UT.6.03x Embedded Systems 
Lab 6: Branching Functions Delays

@author: Vikram Udyawer
@organization: Vikasa Studios
@date: 6th September 2016
@summary: 
				When SW1 and SW2 are pressed -> LED has a steady blue
				When SW1 is pressed only -> Blue LED flashes for 100 ms
				Runs on TI Tiva LaunchPad (EK-TM4C123GXL)
				This lab uses hardware already built into the LaunchPad.


**/


// built-in connection: PF0 connected to negative logic momentary switch, SW2
// built-in connection: PF1 connected to red LED
// built-in connection: PF2 connected to blue LED
// built-in connection: PF3 connected to green LED
// built-in connection: PF4 connected to negative logic momentary switch, SW1

/** Pre-processor definitions  **/
#include "TExaS.h"																																//	header for the Lab Grader

//	Declarations to access port registers using variables 
#define GPIO_PORTF_DATA_R       (*((volatile unsigned long *)0x400253FC))
#define GPIO_PORTF_DIR_R        (*((volatile unsigned long *)0x40025400))
#define GPIO_PORTF_AFSEL_R      (*((volatile unsigned long *)0x40025420))
#define GPIO_PORTF_PUR_R        (*((volatile unsigned long *)0x40025510))
#define GPIO_PORTF_DEN_R        (*((volatile unsigned long *)0x4002551C))
#define GPIO_PORTF_AMSEL_R      (*((volatile unsigned long *)0x40025528))
#define GPIO_PORTF_PCTL_R       (*((volatile unsigned long *)0x4002552C))
#define SYSCTL_RCGC2_R          (*((volatile unsigned long *)0x400FE108))
#define SYSCTL_RCGC2_GPIOF      0x00000020  // port F Clock Gating Control

//	Declarations to access port registers using variables made for this Lab
#define GPIO_PORTF_LOCK_R       (*((volatile unsigned long *)0x40025520))
#define GPIO_PORTF_CR_R         (*((volatile unsigned long *)0x40025524))

// basic functions defined at end of startup.s
void DisableInterrupts(void); // Disable interrupts
void EnableInterrupts(void);  // Enable interrupts

// Function prototypes
void PortF_Init(void);

int main(void){ unsigned long volatile delay;
  TExaS_Init(SW_PIN_PF4, LED_PIN_PF2);  // activate grader and set system clock to 80 MHz
  // initialization goes here
	PortF_Init();									//	Call
	EnableInterrupts();           // enable interrupts for the grader
  while(1){
    // body goes here	
		In = GPIO_PORTF_DATA_R&0x10;	// clear PF4
  }	
}

void PortF_Init(void){ 
	volatile unsigned long delay;
	SYSCTL_RCGC2_R |= SYSCTL_RCGC2_GPIOF;     // 1) We first activate the clock for Port F
  delay = SYSCTL_RCGC2_R;           				// allow time for clock to start
  GPIO_PORTF_LOCK_R = 0x4C4F434B;   				// 2) unlock GPIO Port F
  GPIO_PORTF_CR_R = 0x1F;           // allow changes to PF4-0
  // only PF0 needs to be unlocked, other bits can't be locked
  GPIO_PORTF_AMSEL_R = 0x00;        // 3) disable analog on PF
  GPIO_PORTF_PCTL_R = 0x00000000;   // 4) PCTL GPIO on PF4-0
  GPIO_PORTF_DIR_R = 0x0E;          // 5) PF4,PF0 in, PF3-1 out
  GPIO_PORTF_AFSEL_R = 0x00;        // 6) disable alt funct on PF7-0
  GPIO_PORTF_PUR_R = 0x11;          // enable pull-up on PF0 and PF4
  GPIO_PORTF_DEN_R = 0x1F;          // 7) enable digital I/O on PF4-0
}
