#include <stdint.h>
#include <stdbool.h>

#include "efm32gg.h"
int chooseTone(uint32_t button);

int cnt1 = 0;
int cnt2 = 0;
int lisaCnt = 0;
int delay = 0;
int freq = 73;
uint32_t knapp;
uint32_t lights;
uint8_t incrementValue;
uint8_t lydniva = 0;
/* TIMER1 interrupt handler */
void __attribute__ ((interrupt)) TIMER1_IRQHandler() 
{  
  /*
    TODO feed new samples to the DAC
    remember to clear the pending interrupt by writing 1 to TIMER1_IFC
  */ 
	*TIMER1_IFC = 1;
	//TELLESHITT
	/*
	cnt1 = cnt1 + 1;
	if(cnt1 == 20000){
		cnt1 = 0;
		lights = *GPIO_PA_DOUT;
		if(lights < 0xfdff){
			lights = lights + 256;
			*GPIO_PA_DOUT = lights;
		}
		else {
			lights = 0x0000;
			*GPIO_PA_DOUT = lights;
		}	
	}*/
	
	// variables for piano
	/*
	knapp = *GPIO_PC_DIN;
	lights = knapp << 8;
	*GPIO_PA_DOUT = lights; 
	freq = chooseTone(knapp);
	incrementValue = 255/(2*freq);
	*/
	
	// variables for lisa
	
	knapp = LISA[lisaCnt];
	lights = knapp << 8;
	*GPIO_PA_DOUT = lights; 
	freq = chooseTone(knapp);
	incrementValue = 0xFF/(2*freq);


	if(knapp <= 0xFE){ 

	// Sawtooth wave
	
		if (cnt2 < 2*freq){
			*DAC0_CH0DATA = lydniva;
			*DAC0_CH1DATA = lydniva;
			lydniva = lydniva + incrementValue;
		}
		else{
			lydniva = 0;
			cnt2 = 0;			
		}
		
	// Square wave
	/*
		if (cnt2 < freq){
			*DAC0_CH0DATA = 0xff;
		    *DAC0_CH1DATA = 0xff;
		}
		else{
			*DAC0_CH0DATA = 0x00;
		    *DAC0_CH1DATA = 0x00;
		    if (cnt2 >= 2*freq){
		    	cnt2 = 0;
		    }			
		}
	*/

	} //end of first if
	else{
		*DAC0_CH0DATA = 0x00;
		*DAC0_CH1DATA = 0x00;
		if(cnt2 > 40){
			cnt2 = 0;
		}
	}
	cnt2 = cnt2 + 1;
	delay = delay + 1;	

	if(delay > 2000){
		lisaCnt = lisaCnt + 1;
		delay = 0;
		if(lisaCnt > 50){
			lisaCnt = 0;		
		}
	}
	
}
    

	 

void GPIO_HANDLER()
{
		*GPIO_IFC = 0xff;
	  uint32_t a = *GPIO_PC_DIN;
 	  a = a << 8;
		*GPIO_PA_DOUT = a; 
}

/* GPIO even pin interrupt handler */
void __attribute__ ((interrupt)) GPIO_EVEN_IRQHandler() 
{
    /* TODO handle button pressed event, remember to clear pending interrupt */
		*GPIO_IFC = 0xff;
		//GPIO_HANDLER();
}

/* GPIO odd pin interrupt handler */
void __attribute__ ((interrupt)) GPIO_ODD_IRQHandler() 
{
    /* TODO handle button pressed event, remember to clear pending interrupt */
		
		//GPIO_HANDLER();
}

