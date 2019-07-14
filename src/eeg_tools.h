/*
 * eeg_tools.h
 *
 *  Created on: 2019年7月12日
 *      Author: shijingwei
 */

#ifndef SRC_EEG_TOOLS_H_
#define SRC_EEG_TOOLS_H_

#define __SIMULATION

#ifdef __cplusplus
extern "C" {
#endif

#ifdef __SIMULATION
FILE *simulationFile;

void openSimulationFile(char *filename);
void closeSimulationFile();

#endif

/*
*Function: sendData
*Description: 发送脑电数据
*Input:
*  code：数据标志
*  len: 数据长度
*  buffer：数据缓冲，ID长度10字节
*Output:
*无
*/
void sendEEGData(unsigned char code ,unsigned char len,unsigned char *buffer);

/*
*Function: sendDeviceID
*Description: 发送设备ID
*Input:
*  IDBuffer：ID的数据Buffer，ID长度10字节
*Output:
*无
*/
void sendDeviceID(unsigned char code ,unsigned char len,unsigned char *IDBuffer);

/*
*Function: sendRawData
*Description: 发送原始数据
*Input:
*  data：原始数据，2字节
*Output:
*无
*/
void sendRawData(unsigned char code ,unsigned char len,unsigned char *data);

/*
*Function: sendEEGPower
*Description: 发送alpha，beta等波形的能量值，数据24字节
*Input:
*  power：power值，24字节
*Output:
*无
*/
void sendEEGPower(unsigned char code ,unsigned char len,unsigned char *power);

/*
*Function: sendBlink
*Description: 发送眨眼信号
*Input:
*  blink：眨眼信号
*Output:
*无
*/
void  sendBlink(unsigned char code ,unsigned char len,unsigned char blink);


/*
*Function: sendMeditation
*Description: 发送冥想信号
*Input:
*  meditation：冥想信号
*Output:
*无
*/

void  sendMeditation (unsigned char code ,unsigned char len,unsigned char meditation);


/*
*Function: sendAttention
*Description: 发送注意力信号
*Input:
*  attention：注意力信号
*Output:
*无
*/
void  sendAttention(unsigned char code ,unsigned char len,unsigned char attention);


/*
*Function: sendPoorSignal
*Description: 发送信号状态
*Input:
*  poorSignal：信号状态
*Output:
*无
*/
void  sendPoorSignal (unsigned char code ,unsigned char len,unsigned char poorSignal);
;

#ifdef __cplusplus
}  /* extern "C" */
#endif



#endif /* SRC_EEG_TOOLS_H_ */
