#include "em_device.h"
#include "em_cmu.h"
#include "em_gpio.h"
#include "em_system.h"
#include "em_chip.h"    // required for CHIP_Init() function
#include "bsp_trace.h"
#include <stdio.h>

#define CLK 15
#define D0 0
#define D1 1
#define D2 2
#define D3 3
#define A0 4
#define A1 5
#define A2 6
#define A3 7

int _write(int file, const char *ptr, int len)
{
    int x;
    for (x = 0; x < len; x++)
    ITM_SendChar (*ptr++);
    return (len);
}

void setupSWOForPrint(void)
{
  /* Enable GPIO clock. */
  CMU->HFPERCLKEN0 |= CMU_HFPERCLKEN0_GPIO;

  /* Enable Serial wire output pin */
  GPIO->ROUTE |= GPIO_ROUTE_SWOPEN;

#if defined(_EFM32_GIANT_FAMILY) || defined(_EFM32_LEOPARD_FAMILY) || defined(_EFM32_WONDER_FAMILY)
  /* Set location 0 */
  GPIO->ROUTE = (GPIO->ROUTE & ~(_GPIO_ROUTE_SWLOCATION_MASK)) | GPIO_ROUTE_SWLOCATION_LOC0;

  /* Enable output on pin - GPIO Port F, Pin 2 */
  GPIO->P[5].MODEL &= ~(_GPIO_P_MODEL_MODE2_MASK);
  GPIO->P[5].MODEL |= GPIO_P_MODEL_MODE2_PUSHPULL;
#else
  /* Set location 1 */
  GPIO->ROUTE = (GPIO->ROUTE & ~(_GPIO_ROUTE_SWLOCATION_MASK)) |GPIO_ROUTE_SWLOCATION_LOC1;
  /* Enable output on pin */
  GPIO->P[2].MODEH &= ~(_GPIO_P_MODEH_MODE15_MASK);
  GPIO->P[2].MODEH |= GPIO_P_MODEH_MODE15_PUSHPULL;
#endif

  /* Enable debug clock AUXHFRCO */
  CMU->OSCENCMD = CMU_OSCENCMD_AUXHFRCOEN;

  /* Wait until clock is ready */
  while (!(CMU->STATUS & CMU_STATUS_AUXHFRCORDY));

  /* Enable trace in core debug */
  CoreDebug->DEMCR |= CoreDebug_DEMCR_TRCENA_Msk;
  ITM->LAR  = 0xC5ACCE55;
  ITM->TER  = 0x0;
  ITM->TCR  = 0x0;
  TPI->SPPR = 2;
  TPI->ACPR = 0xf;
  ITM->TPR  = 0x0;
  DWT->CTRL = 0x400003FF;
  ITM->TCR  = 0x0001000D;
  TPI->FFCR = 0x00000100;
  ITM->TER  = 0x1;
}

int main(void)
{
	int grades[80],prices[80],previous_prices[80],index=0,predata=0,init=0,ignore=1;
	CHIP_Init();
	CMU_ClockEnable(cmuClock_GPIO, 1);
	  GPIO_DriveModeSet(gpioPortD, gpioDriveModeLowest);

	  GPIO_PinModeSet(gpioPortD, CLK, gpioModeInputPull, 0);
	  GPIO_PinModeSet(gpioPortD, D0, gpioModeInputPull, 0);
	  GPIO_PinModeSet(gpioPortD, D1, gpioModeInputPull, 0);
	  GPIO_PinModeSet(gpioPortD, D2, gpioModeInputPull, 0);
	  GPIO_PinModeSet(gpioPortD, D3, gpioModeInputPull, 0);
	  GPIO_PinModeSet(gpioPortD, A0, gpioModeInputPull, 0);
	  GPIO_PinModeSet(gpioPortD, A1, gpioModeInputPull, 0);
	  GPIO_PinModeSet(gpioPortD, A2, gpioModeInputPull, 0);
	  GPIO_PinModeSet(gpioPortD, A3, gpioModeInputPull, 0);

	setupSWOForPrint();
	printf("Waiting for clock to get HIGH \n");
    while (1)
    {
    	int data=0,address=0;
    	if(GPIO_PinInGet(gpioPortD, CLK)==1){
    	//Reading data bits
    		if((GPIO_PinInGet(gpioPortD, D0))==1)
    			data += 1;
    		if((GPIO_PinInGet(gpioPortD, D1))==1)
    			data += 2;
    		if((GPIO_PinInGet(gpioPortD, D2))==1)
    			data += 4;
    		if((GPIO_PinInGet(gpioPortD, D3))==1)
    			data += 8;
  	   //Reading address bits
    		if((GPIO_PinInGet(gpioPortD, A0))==1)
    			address += 1;
    		if((GPIO_PinInGet(gpioPortD, A1))==1)
    			address += 2;
    		if((GPIO_PinInGet(gpioPortD, A2))==1)
    			address += 4;
    		if((GPIO_PinInGet(gpioPortD, A3))==1)
    			address += 8;
    		if(predata > address){				//finding greater address values
    			if(init==1){					//for comparing from second itiration
    				int final_grades[15]={};
    				int d=0;
    				printf("grades:");
    				for(int a=0;a<=index-1;a++)  //printing grade values
    					printf("%d ",grades[a]);
    				printf("\nprices:");
    				for(int b=0;b<=index-1;b++)	 //printing price values
    					printf("%d ",prices[b]);

    			    printf("\nerror check\n");
    			    for(int c=0;c<=index-1;c++){	//ignore is 0 when all the prices are equal to 15
    			    	if(prices[c]!=15)
    			    		ignore=0;
    			    }
    			    if(ignore==0){					//compare each bit if prices are not equal to 15
    			    ignore=1;
    			    for(int c=0;c<=index-1;c++){
    			    		if(previous_prices[c]!=prices[c]){		//comparing prices with previously stored prices
    			    			if(final_grades[d-1]!=grades[c]){
    			    					final_grades[d]=grades[c];
    			    					d++;
    			    			}
    			    		}
    			    	}
    			    for(int b=0;b<d;b++){
    			    	printf("grade %d:",final_grades[b]);
    			    	for(int final=0;final<=index-1;final++){
    			    			if(final_grades[b]==grades[final])
    			    				printf("%d ",prices[final]);
    			    		}
    			    	printf("\n");
    			    	}
    			    }
    			}
    			if(init==0){						//storing the price at initial state
    				for(int j=0;j<=index-1;j++)
    					previous_prices[j]=prices[j];
    				printf("\nprevious prices:");
    				for(int a=0;a<=index-1;a++)  //printing previous values
    				    printf("%d ",previous_prices[a]);
    				init=1;
    			}
    			index=0;
    		}
    		grades[index]=address;
    		prices[index]=data;
    		predata=address;

    		index++;
    		}
	 }
}
