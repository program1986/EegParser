/*
 * bld_decode.c
 *
 *  Created on: 2019年7月12日
 *      Author: shijingwei
 */


#include "stdio.h"

//传输缓冲
unsigned char inSendbuffer[48];
unsigned char outSendbuffer[30];
#define		PACK_BEGIN		(char)0xFF
#define		PACK_STOP		(char)0xFE

void extractData(unsigned char *data,int len)
{
	int rlen=30;



	decode_buffer(data, len, outSendbuffer,&rlen);
	//检查校验和

	unsigned short crc16_1=crc((unsigned char *)outSendbuffer,rlen-2);
	unsigned short crc16_2=(unsigned short)(outSendbuffer[rlen-2]<<8)|(unsigned short)(outSendbuffer[rlen-1]);

	if (crc16_1!=crc16_2)
	{
		printf("CRC CHECK ERROR!\n");
		//exit(0);
	}
	else
	{
		switch (outSendbuffer[0])
		{
			//眨眼
			case ( 0x16 ):
				printf("%d\n", outSendbuffer[2] & 0xFF);
			    break;
		    //注意力
			case (0x04):
				printf("%d\n", outSendbuffer[2] & 0xFF);
				break;
		    //冥想
			case ( 0x05 ):
				printf("%d\n", outSendbuffer[2] & 0xFF);
				break;
		    //信号质量 0 是最好，200最不好
			case ( 0x02 ):
				printf("%d\n", outSendbuffer[2] & 0xFF);
				break;
		    //能量值
			case (0x83):
				//sendEEGData(code ,(unsigned char)valueLength,(unsigned char*)value);
				//eegPower[j] = ((uint32_t)value[i] << 16) | ((uint32_t)value[i+1] << 8) | (uint32_t)value[i+2];
			//原始数据
			case 0x80:
				printf("%d\n", (short) ((outSendbuffer[2] << 8 ) | outSendbuffer[3]));
				break;
		}

	}




}

int main33(int argc ,char *argv[])
{
	printf("start3...\n");
	FILE *stream = fopen( "D:/EEG/transdata.dat", "rb" );
	unsigned char streamByte;
	int count;
	count=0;
	while ( 1 )
	{
		int x=fread( &streamByte, 1, 1, stream );
		switch ((char)streamByte)
		{

		case PACK_BEGIN:
			inSendbuffer[0]=PACK_BEGIN;
			count=1;

			break;
		case PACK_STOP:
			inSendbuffer[count++]=PACK_STOP;
			extractData(inSendbuffer,count);
			count=0;

			break;
		default:

			inSendbuffer[count++]=streamByte;
			break;

		}
		if (x!=1) break;

	}

	fclose(stream);
	printf("end...%d\r\n");

}
