/*
 * mpu6050.C
 *
 *  Created on: Sep 19, 2024
 *      Author: Muhammad Haris Mujeeb
 */

#include "mpu6050.h"



void MPU6050_check_conn(MPU6050_t *mpu_instance) {
	uint8_t who_am_i;

	HAL_StatusTypeDef status;
	status = HAL_I2C_Mem_Read(mpu_instance->i2c_handle, mpu_instance->mpu6050_addr, 0x75, I2C_MEMADD_SIZE_8BIT, &who_am_i, 1, 1000);
	if (status == HAL_OK) {
		if (who_am_i == mpu_instance->who_am_i_addr>> 1) {
		  // MPU6050 is connected
		  for (int i = 0; i < 3; i++){
			  HAL_GPIO_WritePin(LED_GREEN_GPIO_Port, LED_GREEN_Pin, GPIO_PIN_SET);
			  HAL_Delay(100);
			  HAL_GPIO_WritePin(LED_GREEN_GPIO_Port, LED_GREEN_Pin, GPIO_PIN_RESET);
			  HAL_Delay(100);
		  }
    } else {
	  // Handle the error, e.g., toggle an error LED

///////// only for testing !!! remove it.
for (int i = 0; i < 3; i++){
  HAL_GPIO_WritePin(LED_GREEN_GPIO_Port, LED_GREEN_Pin, GPIO_PIN_SET);
  HAL_Delay(250);
  HAL_GPIO_WritePin(LED_GREEN_GPIO_Port, LED_GREEN_Pin, GPIO_PIN_RESET);
  HAL_Delay(250);
}
//////////
	}
  } else {
	  // Handle the error, e.g., toggle an error LED
///////// only for testing !!! remove it.
for (int i = 0; i < 5; i++){
  HAL_GPIO_WritePin(LED_GREEN_GPIO_Port, LED_GREEN_Pin, GPIO_PIN_SET);
  HAL_Delay(250);
  HAL_GPIO_WritePin(LED_GREEN_GPIO_Port, LED_GREEN_Pin, GPIO_PIN_RESET);
  HAL_Delay(250);
}
///////////////////
  }
}



void MPU6050_init( MPU6050_t *mpu_instance){
	mpu_instance->mpu6050_addr = (0x68 << 1);
	mpu_instance->who_am_i_addr = 0x75;
	mpu_instance->pwr_mgmt_1_addr = 0x6B;
	mpu_instance->smprt_div_addr = 0x19;
	mpu_instance->cfg_addr = 0x1A;
	mpu_instance->accel_cfg_addr = 0x1B;
	mpu_instance->gyro_cfg_addr = 0x1C;
	mpu_instance->accel_data_addr = 0x3B;
	mpu_instance->gyro_data_addr = 0x43;

	MPU6050_check_conn(mpu_instance);

	uint8_t data;
	// Power Management Register 0X6B we should write all 0's to wake the sensor up
    data = 0;
    HAL_I2C_Mem_Write(mpu_instance->i2c_handle, mpu_instance->mpu6050_addr, mpu_instance->pwr_mgmt_1_addr, 1, &data, 1, 1000);

	// Set accelerometer configuration in ACCEL_CONFIG Register
	data = 0x00; // XA_ST=0,YA_ST=0,ZA_ST=0, FS_SEL=0 -> ± 2g
	HAL_I2C_Mem_Write(mpu_instance->i2c_handle, mpu_instance->mpu6050_addr, mpu_instance->accel_cfg_addr, 1, &data, 1, 1000);	//


	// Set Gyroscopic configuration in GYRO_CONFIG Register
	data = 0x00; // XG_ST=0,YG_ST=0,ZG_ST=0, FS_SEL=0 -> ± 250 deg/s
	HAL_I2C_Mem_Write(mpu_instance->i2c_handle, mpu_instance->mpu6050_addr, mpu_instance->gyro_cfg_addr, 1, &data, 1, 1000);	//
}

void MPU6050_read_accel(MPU6050_t *mpu_instance){
	uint8_t size_of_Rec_Data = 6;
	uint8_t Rec_Data[size_of_Rec_Data];
	HAL_I2C_Mem_Read(mpu_instance->i2c_handle, mpu_instance->mpu6050_addr, mpu_instance->accel_data_addr, 1, Rec_Data, size_of_Rec_Data, 1000);
	mpu_instance->Accel_X_RAW = (int16_t)(Rec_Data[0] << 8 | Rec_Data[1] );
	mpu_instance->Accel_Y_RAW = (int16_t)(Rec_Data[2] << 8 | Rec_Data[3] );
	mpu_instance->Accel_Z_RAW = (int16_t)(Rec_Data[4] << 8 | Rec_Data[5] );

	// As we have selected the Full-Scale range of ± 2g, for which the sensitivity is 16384 LSB /g, we have to divide the RAW values by 16384 to get the values in grams (g).
	mpu_instance->Ax = (float)mpu_instance->Accel_X_RAW/16384.0;
	mpu_instance->Ay = (float)mpu_instance->Accel_Y_RAW/16384.0;
	mpu_instance->Az = (float)mpu_instance->Accel_Z_RAW/16384.0;

}


void MPU6050_read_gyro(MPU6050_t *mpu_instance){
	uint8_t size_of_Rec_Data = 6;
	uint8_t Rec_Data[size_of_Rec_Data];
	HAL_I2C_Mem_Read(mpu_instance->i2c_handle, mpu_instance->mpu6050_addr, mpu_instance->gyro_data_addr, 1, Rec_Data, size_of_Rec_Data, 1000);
	mpu_instance->Gyro_X_RAW = (int16_t)(Rec_Data[0] << 8 | Rec_Data[1] );
	mpu_instance->Gyro_Y_RAW = (int16_t)(Rec_Data[2] << 8 | Rec_Data[3] );
	mpu_instance->Gyro_Z_RAW = (int16_t)(Rec_Data[4] << 8 | Rec_Data[5] );

	// As we have selected the Full-Scale range of ± 2g, for which the sensitivity is 16384 LSB /g, we have to divide the RAW values by 16384 to get the values in grams (g).
	mpu_instance->Gx = (float)mpu_instance->Gyro_X_RAW/16384.0;
	mpu_instance->Gy = (float)mpu_instance->Gyro_Y_RAW/16384.0;
	mpu_instance->Gz = (float)mpu_instance->Gyro_Z_RAW/16384.0;
}
