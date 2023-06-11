
#include "interrupt.h"
#include "timer.h"
static uint8_t Trang_thai = 0;
void EXTI0_IRQHandler()
{	
		if ((EXTI_GetITStatus(EXTI_Line0) != RESET))
    {
			
       switch (Trang_thai)
        {
            case 0:
            
									Trang_thai =1;
									Delay_Ms(200);
                break;

            case 1:
               
									Trang_thai =2;
									Delay_Ms(200);
                break;
						
						case 2:
                
									Trang_thai =1;
									Delay_Ms(200);		
                break;
            default:
							Trang_thai =1;
						Delay_Ms(200);
                break;
						
				}
				
				EXTI_ClearITPendingBit(EXTI_Line0);
			
		}
		
}

 
void EXTI1_IRQHandler()
{
					
  
		if (EXTI_GetITStatus(EXTI_Line1) != RESET)
    {
        NVIC_SystemReset();
			
        EXTI_ClearITPendingBit(EXTI_Line1);
    }
}

void interrupt_init(uint32_t RCC_APB2Periph_GPIOX ,uint32_t GPIO_Pin_X, uint32_t GPIO_PortSourceGPIOX, 
	uint32_t GPIO_PinSourceX, uint32_t EXTI_LineX, uint32_t EXTIX_IRQn,uint8_t NVIC_IRQChannelPreemptionPriorityX)
{
    GPIO_InitTypeDef GPIO_InitStructure;
		EXTI_InitTypeDef EXTI_InitStructure;
    NVIC_InitTypeDef NVIC_InitStructure;

    // Buoc 1: bat clock
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOX, ENABLE);

    // cau hinh gpio
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_X;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPD;
		GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOB, &GPIO_InitStructure);

   // cau hinh ngat ngoai
		RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE);
	
    GPIO_EXTILineConfig(GPIO_PortSourceGPIOX, GPIO_PinSourceX);
		EXTI_InitStructure.EXTI_Line = EXTI_LineX;
		EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
    EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising;
    EXTI_InitStructure.EXTI_LineCmd = ENABLE;
    EXTI_Init(&EXTI_InitStructure);
   
    // cai dat muc do uu tien
    NVIC_InitStructure.NVIC_IRQChannel = EXTIX_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = NVIC_IRQChannelPreemptionPriorityX;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x00;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);
}
uint8_t read_interrupt ()
{
	return Trang_thai;
}

