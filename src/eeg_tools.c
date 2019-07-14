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



//浼犺緭缂撳啿
char inSendbuffer[30];
char outSendbuffer[48];


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
*Function: sendData
*Description: 鍙戦�佽剳鐢垫暟鎹�
*Input:
*  code锛氭暟鎹爣蹇�
*  len: 鏁版嵁闀垮害
*  buffer锛氭暟鎹紦鍐诧紝ID闀垮害10瀛楄妭
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


/*
*Function: sendEEGPower
*Description: 鍙戦�乤lpha锛宐eta绛夋尝褰㈢殑鑳介噺鍊硷紝鏁版嵁24瀛楄妭
*Input:
*  power锛歱ower鍊硷紝24瀛楄妭
*Output:
*鏃�
*/

void sendEEGPower(unsigned char code ,unsigned char len,unsigned char *power)
{
	int i;

	inSendbuffer[0]=code;
	inSendbuffer[1]=len;

	for (i=0;i<=len;i++)
	{
		inSendbuffer[i+2]=power[i];
	}
	len = len+2;
	//璁＄畻CRC

	unsigned short crc16=crc(power,len);
	inSendbuffer[len+1]=(crc16&0xFF00)>>8;
	inSendbuffer[len+2]=(crc16&0xFF);

	len = len+2;

	encode_buffer(inSendbuffer, len, outSendbuffer,&len);
#ifdef SIMULATION

	fwrite(outSendbuffer,len,1,simulationFile);

#else

#endif

}





#endif /* SRC_EEG_TOOLS_C_ */
