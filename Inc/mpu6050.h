/*
 * mpu6050.h
 *
 *  Created on: Sep 19, 2024
 *      Author: harri
 */

#ifndef INC_MPU6050_H_
#define INC_MPU6050_H_

#include "main.h"

typedef struct {
	I2C_HandleTypeDef *i2c_handle;
	uint8_t mpu6050_addr;
	uint8_t who_am_i_addr;
	uint8_t pwr_mgmt_1_addr;
	uint8_t smprt_div_addr;
	uint8_t cfg_addr;
	uint8_t accel_cfg_addr;
	uint8_t gyro_cfg_addr;
	uint8_t accel_data_addr;
	uint8_t gyro_data_addr;
	int16_t Accel_X_RAW;
	int16_t Accel_Y_RAW;
	int16_t Accel_Z_RAW;
	float Ax;
	float Ay;
	float Az;
	int16_t Gyro_X_RAW;
	int16_t Gyro_Y_RAW;
	int16_t Gyro_Z_RAW;
	float Gx;
	float Gy;
	float Gz;

} MPU6050_t;


void MPU6050_init( MPU6050_t *mpu_instance);
void MPU6050_read_accel(MPU6050_t *mpu_instance);
void MPU6050_read_gyro(MPU6050_t *mpu_instance);
#endif /* INC_MPU6050_H_ */
