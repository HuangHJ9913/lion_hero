#include "mpu6050.h"

//#define THRESHOLD_FOR_LOW_FILTER 1000.0f

volatile MPU6050_REAL_DATA   MPU6050_Real_Data;

uint8_t Receive_buffer[11];
int MPU6050_ReadDone = 0;
float Speed_X,Speed_Y;

float record_for_mid_filter[5] = {0, 0, 0, 0, 0};
enum BOOL{true, false};

/**************************实现函数********************************************
*函数原型:		void MPU6050_Initialize(void)
*功    能:	  MPU6050需要用到USART2进行数据接收，设定USART2的波特率为115200
*******************************************************************************/
//void MPU6050_Initialize(void)
//{
//    USART2_Configuration(115200);  // 初始化USART2
//}

/**************************实现函数********************************************
*函数原型:		void MPU6050_getMotion(float* ax, float* ay, float* az, 
																		 float* wax, float* way, float* waz,
																		 float* wx, float* wy, float* wz)
*功    能:	  读取MPU6050的当前测量值
*描    述:    ax,ay,az:     x,y,z轴方向的加速度
							wax,way,waz:	x,y,z轴方向的角加速度
							wx,wy,wz:			x,y,z轴方向的角度(0-360)
*******************************************************************************/
float Mid_Filter(float current_value)
{
    for(int i = 4; i >= 1; i--)
        record_for_mid_filter[i] = record_for_mid_filter[i - 1];
	  record_for_mid_filter[0] = current_value;
	  
	float temp_record[4];
	for(int i = 4; i >= 0; i--)
		temp_record[i] = record_for_mid_filter[i];
	  
	  int i, j;
	  enum BOOL exchange = false;
	  for(i = 1; i < 5; i++)
	  {
        exchange = false;
			  for(j = 0; j < 5 - i; j++)
			  {
				    if(temp_record[j] > temp_record[j + 1])
						{
						    float tmp = temp_record[j];
							  temp_record[j] = temp_record[j + 1];
							  temp_record[j + 1] = tmp;
							  exchange = true;
						}
				}
				if(!exchange)
				    break;
		}
		return temp_record[2];
}

void MPU6050_getMotion(float* ax, float* ay, float* az, 
											 float* wax, float* way, float* waz,
											 float* wx, float* wy, float* wz) 
{
	if(MPU6050_ReadDone == 1)
	{
		if(Receive_buffer[0] == 0x55)  
		{  
			float tmp_Gyro_Z;
			switch(Receive_buffer[1])
			{
				case 0x51:
					
					MPU6050_Real_Data.Accel_X = ((short)(Receive_buffer[3]<<8 | Receive_buffer[2]))/32768.0*16*9.8;
					MPU6050_Real_Data.Accel_Y = ((short)(Receive_buffer[5]<<8 | Receive_buffer[4]))/32768.0*16*9.8;
					MPU6050_Real_Data.Accel_Z = ((short)(Receive_buffer[7]<<8 | Receive_buffer[6]))/32768.0*16*9.8;
					break;
				case 0x52:
					MPU6050_Real_Data.Gyro_X = ((short)(Receive_buffer[3]<<8 | Receive_buffer[2]))/32768.0*2000;
					MPU6050_Real_Data.Gyro_Y = ((short)(Receive_buffer[5]<<8 | Receive_buffer[4]))/32768.0*2000;
					tmp_Gyro_Z = ((short)(Receive_buffer[7]<<8 | Receive_buffer[6]))/32768.0*2000;
				
				  MPU6050_Real_Data.Gyro_Z = tmp_Gyro_Z;//Mid_Filter(tmp_Gyro_Z);
					break;
				case 0x53:
					MPU6050_Real_Data.Mag_X = ((short)(Receive_buffer[3]<<8 | Receive_buffer[2]))/32768.0*180;
					MPU6050_Real_Data.Mag_Y = ((short)(Receive_buffer[5]<<8 | Receive_buffer[4]))/32768.0*180;
					MPU6050_Real_Data.Mag_Z = ((short)(Receive_buffer[7]<<8 | Receive_buffer[6]))/32768.0*180;
					break;
			} 
			
			// ax和ay的特殊操作：g太小，用其他方向的量进行补偿
			*ax  = MPU6050_Real_Data.Accel_X + 9.8f*(MPU6050_Real_Data.Mag_Y/180*3.14159f);
			*ay  = MPU6050_Real_Data.Accel_Y - 9.8f*(MPU6050_Real_Data.Mag_X/180*3.14159f);
			*az  = MPU6050_Real_Data.Accel_Z;
			*wax = MPU6050_Real_Data.Gyro_X;
			*way = MPU6050_Real_Data.Gyro_Y;
			*waz = MPU6050_Real_Data.Gyro_Z;
			*wx  = MPU6050_Real_Data.Mag_X;
			*wy  = MPU6050_Real_Data.Mag_Y;
			*wz  = MPU6050_Real_Data.Mag_Z;
		} 
		else
		{
			*ax  = MPU6050_Real_Data.Accel_X + 9.8f*(MPU6050_Real_Data.Mag_Y/180*3.14159f);
			*ay  = MPU6050_Real_Data.Accel_Y - 9.8f*(MPU6050_Real_Data.Mag_X/180*3.14159f);
			*az  = MPU6050_Real_Data.Accel_Z;
			*wax = MPU6050_Real_Data.Gyro_X;
			*way = MPU6050_Real_Data.Gyro_Y;
			*waz = MPU6050_Real_Data.Gyro_Z;
			*wx  = MPU6050_Real_Data.Mag_X;
			*wy  = MPU6050_Real_Data.Mag_Y;
			*wz  = MPU6050_Real_Data.Mag_Z;
		}
		MPU6050_ReadDone = 0;
	}
}
/**************************实现函数********************************************
*函数原型:		MPU6050_getlastMotion(float* ax, float* ay, float* az,
																	  float* wax, float* way,float* waz,
																		float* wx, float* wy, float* wz)
*功    能:	  读取MPU6050的最新测量值
*******************************************************************************/
void MPU6050_getlastMotion(float* ax, float* ay, float* az,
													 float* wax, float* way,float* waz,
													 float* wx, float* wy, float* wz)
{
	*ax  = MPU6050_Real_Data.Accel_X + 9.8f*(MPU6050_Real_Data.Mag_Y/180*3.14159f);
	*ay  = MPU6050_Real_Data.Accel_Y - 9.8f*(MPU6050_Real_Data.Mag_X/180*3.14159f);
	*az  = MPU6050_Real_Data.Accel_Z;
	*wax = MPU6050_Real_Data.Gyro_X;
	*way = MPU6050_Real_Data.Gyro_Y;
	*waz = MPU6050_Real_Data.Gyro_Z;
	*wx  = MPU6050_Real_Data.Mag_X;
	*wy  = MPU6050_Real_Data.Mag_Y;
	*wz  = MPU6050_Real_Data.Mag_Z;
}

/**************************????********************************************
*????:		void MPU6050_getSpeed(float* Spx,float* Spy,float* Acc_x,float* Acc_y) 
*?  ?:	  ??????x?y??????
尚未整理：积分得到速度的方法
*******************************************************************************/
void MPU6050_getSpeed(float* Spx,float* Spy,float* Acc_x,float* Acc_y,float* last_Acc_x,float* last_Acc_y)
{
	if(*Acc_x<0.15f&&*Acc_x>-0.15f)
	{
		*Spx += 0;
  	*Spy += 0;
	}
	else
	{
		*Spx += * Acc_x* 0.00305f;
		*Spy += * Acc_y* 0.00305f;
		Speed_X=*Spx;
		Speed_Y=*Spy;
	}
}
void MPU6050_getlastSpeed(float* Spx,float* Spy)
{
	*Spx=Speed_X;
	*Spy=Speed_Y;
}
/**************************????********************************************
*????:		void MPU6050_getDisplacement(float* Disx,float* Disy,float* Spx,float* Spy) 
*?  ?:	  ??????x?y??????
尚未整理：积分得到距离的方法
*******************************************************************************/
float Dis_x,Dis_y;
void MPU6050_getDisplacement(float* Disx,float* Disy,float* Spx,float* Spy)
{
	*Disx += *Spx*0.003f;
	*Disy += *Spy*0.003f;
	Dis_x=*Disx;
	Dis_y=*Disy;
}
void MPU6050_getlastDisplacement(float* Disx,float* Disy)
{
	*Disx=Dis_x;
	*Disy=Dis_y;
}

