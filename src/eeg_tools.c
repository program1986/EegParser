/*
 * eeg_tools.c
 *
 *  Created on: 2019骞�7鏈�12鏃�
 *      Author: shijingwei
 */

#ifndef SRC_EEG_TOOLS_C_
#define SRC_EEG_TOOLS_C_

#include  "crc16.h"
#include  "stdlib.h"
#include  "stdio.h"
#include  "eeg_tools.h"
#include  "De_Encode.h"



//输入输出缓冲
char inSendbuffer[30];
char outSendbuffer[48];


//调试是使用
#ifdef __SIMULATION
FILE *simulationFile;

void openSimulationFile(char *filename)
{
	simulationFile = fopen(filename,"wb");
}

void closeSimulationFile()
{
	fclose(simulationFile);
}


#endif




/*
*Function: sendEEGData
*Description: 发送脑电数据
*Input:
*  code：数据代码
*  len: 后续数据长度
*  buffer：要发送的数据，
*Output:
*鏃�
*/
void sendEEGData(unsigned char code ,unsigned char len,unsigned char *buffer)
{
	int i;
	int rlen=48;

	inSendbuffer[0]=code;
	inSendbuffer[1]=len;

	for (i=0;i<len;i++)
	{
		inSendbuffer[i+2]=buffer[i];
	}
	len = len+2;
	//璁＄畻CRC
	unsigned short crc16=crc(inSendbuffer,len);
	inSendbuffer[len]=(crc16&0xFF00)>>8;
	inSendbuffer[len+1]=(crc16&0xFF);
	len = len+2;

	encode_buffer(inSendbuffer, len, outSendbuffer,&rlen);

#ifdef __SIMULATION

	fwrite(outSendbuffer,rlen,1,simulationFile);

#else

#endif



}





#endif /* SRC_EEG_TOOLS_C_ */
