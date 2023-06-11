#include "stm32f10x.h"
#include "timer.h"
#include "interrupt.h"
#include "i2c.h"
#include "i2c_lcd.h"
#include <stdio.h>
#include "i2c_6050.h"
//khoi tao bien cho chuong trinh
uint32_t i = 0;
uint32_t i2 = 0;
static uint8_t Trang_thai;
char CTR_datasend[32];
float* mpuData ;
//doc cam bien
		int stepCount = 0;
		int previousState = 0;
    int currentState = 0;
    float thresholdHigh = 3.0;  
    float thresholdLow = 0.1;
		
//thuat toan buoc chay khai bao
float accelX;
float accelY;
float accelZ;
float accelX2;
float accelY2;
float accelZ2;
float magnitude;
int couter6050=0;
int main(void) {
  GPIO_InitTypeDef gpioInit;
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE);
	gpioInit.GPIO_Mode = GPIO_Mode_Out_PP;
	gpioInit.GPIO_Pin = GPIO_Pin_15|GPIO_Pin_14;
	gpioInit.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOC, &gpioInit);

	delay_init();
	
	interrupt_init(RCC_APB2Periph_GPIOB, GPIO_Pin_0, GPIO_PortSourceGPIOB, GPIO_PinSource0, 
	EXTI_Line0, EXTI0_IRQn, 0x01);
	
	interrupt_init(RCC_APB2Periph_GPIOB, GPIO_Pin_1, GPIO_PortSourceGPIOB, GPIO_PinSource1, 
	EXTI_Line1, EXTI1_IRQn, 0x00);
	
	I2C_LCD_Setup();
	I2C_LCD_Init();
	I2C_LCD_BackLight(1);
	I2C_LCD_Clear();
	sprintf(CTR_datasend,"Bat dau");
	I2C_LCD_Puts(CTR_datasend);
	
	I2C_MPU6050_Setup() ;
	I2C_MPU6050_Init() ;
	
	while(1){
		Trang_thai = read_interrupt();
		
		if(Trang_thai==1){
		i2=0;
		GPIO_ResetBits(GPIOC, GPIO_Pin_14);
			
		//-------------------------------------
			couter6050++;
			if ( couter6050 ==1 ){
			I2C_MPU6050_Setup() ;
        // doc du lieu gia toc
				
        mpuData = CTR_READ_ACCEL_MPU3050();
         accelX = mpuData[0];
				accelY = mpuData[1];
         accelZ = mpuData[2];
				}
			if ( couter6050 > 1&& couter6050 <=25){
			I2C_MPU6050_Setup() ;
        // doc du lieu gia toc
        mpuData = CTR_READ_ACCEL_MPU3050();
         accelX = (mpuData[0]+ accelX)/2;
				 accelY = (mpuData[1] + accelY)/2;
         accelZ = (mpuData[2] + accelZ)/2;
				}
				if ( couter6050 > 25&& couter6050<50){
				I2C_MPU6050_Setup() ;
         // doc du lieu gia toc
        mpuData = CTR_READ_ACCEL_MPU3050();
         accelX2 = (mpuData[0]+ accelX2)/2;
				 accelY2 = (mpuData[1] + accelY2)/2;
         accelZ2= (mpuData[2] + accelZ2)/2;
				}
			if (couter6050== 99){
				couter6050=0;
				I2C_MPU6050_Setup() ;
        mpuData = CTR_READ_ACCEL_MPU3050();
         accelX2 = (mpuData[0]+ accelX2)/2;
				 accelY2 = (mpuData[1] + accelY2)/2;
         accelZ2= (mpuData[2] + accelZ2)/2;
        // tinh toan do lon gia toc
        magnitude = sqrt((accelX2-accelX) * (accelX2-accelX) + (accelY2-accelY) * (accelY2-accelY) + (accelZ2-accelZ)*(accelZ2-accelZ));
        
        // cap nhat trang thai va dem
        if (currentState == 0 && magnitude > thresholdHigh) {
            currentState = 1;
        } else if (currentState == 1 && magnitude < thresholdLow) {
            currentState = 0;
            stepCount++;
					previousState = currentState;
        }
				}
        
        
		//-------------------------------------
		i++;
		Delay_Ms(1);
		
			
		if (i == 1){	
				GPIO_SetBits(GPIOC, GPIO_Pin_15);
				I2C_LCD_Setup();
				I2C_LCD_Clear();
				sprintf(CTR_datasend,"Buoc chan : %d", stepCount);
				I2C_LCD_Puts(CTR_datasend);
				
			}				
		if (i == 1000){	
				GPIO_ResetBits(GPIOC, GPIO_Pin_15);
				I2C_LCD_Setup();
				I2C_LCD_Clear();
				sprintf(CTR_datasend,"Buoc chan : %d", stepCount);
				I2C_LCD_Puts(CTR_datasend);
				
			}
		if ( i ==2000 ){
			GPIO_SetBits(GPIOC, GPIO_Pin_15);
		 i=0;
			}
		}
		else if(Trang_thai==2){
			
			
			GPIO_ResetBits(GPIOC, GPIO_Pin_15);
			i2++;
			i=0;
			Delay_Ms(1);
			if (i2 == 1){
				GPIO_SetBits(GPIOC, GPIO_Pin_14);
				I2C_LCD_Setup();
				I2C_LCD_Clear();
				sprintf(CTR_datasend,"Buoc chan : %d", stepCount);
				I2C_LCD_Puts(CTR_datasend);
				I2C_LCD_NewLine();
				sprintf(CTR_datasend,"DungChuongTrinh");
				I2C_LCD_Puts(CTR_datasend);;
		}
			if ( i2 ==1000 ){
				
				GPIO_SetBits(GPIOC, GPIO_Pin_14);
				I2C_LCD_Setup();
				I2C_LCD_Clear();
				sprintf(CTR_datasend,"Buoc chan : %d", stepCount);
				I2C_LCD_Puts(CTR_datasend);
				I2C_LCD_NewLine();
				sprintf(CTR_datasend,"DungChuongTrinh");
				I2C_LCD_Puts(CTR_datasend);;
				}
			if (i2 == 2000){
				GPIO_ResetBits(GPIOC, GPIO_Pin_14);
				
				i2=0;
		}
		
		}else{
			GPIO_SetBits(GPIOC, GPIO_Pin_14);
			GPIO_ResetBits(GPIOC, GPIO_Pin_15);
		}
		
	}
}
