/*
 * egg_def.h
 *
 *  Created on: 2019年7月11日
 *      Author: shijingwei
 */

#ifndef SRC_EEG_DEF_H_
#define SRC_EEG_DEF_H_

#ifdef __cplusplus
extern "C" {
#endif

#pragma pack(push)
#pragma pack(1)

typedef struct _EEG_OutBuffer{

	char deviceID[10];     //设备ID
	char rawData[100];     //脑电原始数据
	char eggPower[24];     //alpha，beta等波形的能量值
	unsigned char blink;   //眨眼
	char meditation;       //冥想
	char attention;        //注意力
	char poorSignal;       //信号状态
	char resieve[10];      //保留
	char crc16[2];         //CRC16


} EEG_OutBuffer;

#pragma pack(pop)

#ifdef __cplusplus
}  /* extern "C" */
#endif

#endif /* SRC_EEG_DEF_H_ */
