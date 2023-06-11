
#include "i2c_6050.h"
#include "timer.h"

static uint8_t CTR_MPU6050_BUF[8];
static float mpuData[4];
static uint16_t a;

void I2C_MPU6050_Setup (void)
{
	i2c_setup(GPIOB, GPIO_Pin_10, GPIO_Pin_11, RCC_APB2Periph_GPIOB);
}

void I2C_MPU6050_Init (void)
{
	i2c_init();
	CTR_Single_Write(MPU6050_ADDR, MPU6050_SMPLRT_DIV, 0x00);
  CTR_Single_Write(MPU6050_ADDR, MPU6050_CONFIG, 0x00);
  CTR_Single_Write(MPU6050_ADDR, MPU6050_GYRO_CONFIG, 0x08);
  CTR_Single_Write(MPU6050_ADDR, MPU6050_ACCEL_CONFIG, 0x02);
  CTR_Single_Write(MPU6050_ADDR, MPU6050_PWR_MGMT_1, 0x01);
}
float* CTR_READ_ACCEL_MPU3050(void)
{
	
	CTR_MPU6050_BUF[0] = CTR_Single_Read(MPU6050_ADDR,ACCEL_XOUT_H);
	CTR_MPU6050_BUF[1] = CTR_Single_Read(MPU6050_ADDR,ACCEL_XOUT_L);
	a =(uint16_t)(CTR_MPU6050_BUF[0] <<8)|CTR_MPU6050_BUF[1];
	mpuData[0] = a / 16384.0;
	
	CTR_MPU6050_BUF[2] = CTR_Single_Read(MPU6050_ADDR,ACCEL_YOUT_H);
	CTR_MPU6050_BUF[3] = CTR_Single_Read(MPU6050_ADDR,ACCEL_YOUT_L);
	a =	(uint16_t)(CTR_MPU6050_BUF[2] <<8)|CTR_MPU6050_BUF[3];
	mpuData[1] = a /16384.0;
	
	CTR_MPU6050_BUF[4] = CTR_Single_Read(MPU6050_ADDR,ACCEL_ZOUT_H);
	CTR_MPU6050_BUF[5] = CTR_Single_Read(MPU6050_ADDR,ACCEL_ZOUT_L);
	a =	(uint16_t)(CTR_MPU6050_BUF[4] <<8)|CTR_MPU6050_BUF[5];
	mpuData[2] =  a/16384.0;
	
	return mpuData;
}

