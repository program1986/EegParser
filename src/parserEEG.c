/*
 * parserEEG.c
 *
 *  Created on: 2019��7��9��
 *      Author: Administrator
 */

#include "ThinkGearStreamParser.h"
#include "De_Encode.h"
#include "stdio.h"
#include "eeg_def.h"
#include "crc16.h"
#include "eeg_tools.h"
#include <time.h>
#include "string.h"


EEG_OutBuffer eeg_outbuffer;
ThinkGearStreamParser thinkGearStreamParser;

FILE *raw;
int mm;
int raw_receive;
int power_receive;
int raw_count;


//打印脑电数据
void printBlinks( unsigned char extendedCodeLevel,
                  unsigned char code,
                  unsigned char valueLength,
                  const unsigned char *value,
                  void *customData ) {


	if ( extendedCodeLevel == 0 ) {
        switch ( code ) {
            case ( 0x16 ):
				sendEEGData(code ,(unsigned char)1,(unsigned char*)value);
				//eeg_outbuffer.blink = value[0] & 0xFF;
                break;
            case (0x04):
				sendEEGData(code ,(unsigned char)1,(unsigned char*)value);
            	//eeg_outbuffer.attention = value[0] & 0xFF;
            	break;
            case ( 0x05 ):
				sendEEGData(code ,(unsigned char)1,(unsigned char*)value);
				//eeg_outbuffer.meditation = value[0] & 0xFF;
            	break;
            case ( 0x02 ):
				sendEEGData(code ,(unsigned char)1,(unsigned char*)value);
				//eeg_outbuffer.poorSignal = value[0] & 0xFF;
                break;
            case (0x83):
				sendEEGData(code ,(unsigned char)valueLength,(unsigned char*)value);
				//eegPower[j] = ((uint32_t)value[i] << 16) | ((uint32_t)value[i+1] << 8) | (uint32_t)value[i+2];

                break;
            case ( 0x80 ):
				if (mm%10==0)
				{
					sendEEGData(code ,(unsigned char)2,(unsigned char*)value);
					//fprintf( raw,"%d\n", (short) (( value[0] << 8 ) | value[1]));
				}
            	mm++;
                break;
            default:

                break;
        }
    }
}

FILE *fp_blink;
FILE *fp_attention;
FILE *fp_meditation;
FILE *fp_poorSignal;
FILE *fp_powerSignal;
FILE *fp_raw;

char filename_blink[128];
char filename_attention[128];
char filename_meditation[128];
char filename_poorSignal[128];
char filename_powerSignal[128];
char filename_raw[128];

typedef unsigned int uint32_t;

//获取时间,并返回字符串
void getTime(char *retTime)
{
    time_t timep;
    struct tm *p;
    time (&timep);
    p=gmtime(&timep);

    sprintf(retTime,"%0d-%02d-%02d %02d:%02d:%02d",
    		1900+p->tm_year,
			p->tm_mon,
			p->tm_mday,
			8+p->tm_hour,
			p->tm_min,
			p->tm_sec);
}




//打开所有的文件用于记录

FILE  *openAllFile(char *prex,char *prompt,char *type)
{

	char filename[128];
	char currentTime[255];
	FILE *file;

	getTime(currentTime);

	//眨眼
	strcpy(filename,prex);
	strcat(filename,"_");
	strcat(filename,type);
	strcat(filename, ".csv");


	file = fopen(filename, "w+" );
	if (file==NULL)
	{
		printf(type);
		printf(" record file open error!\n");
		exit(0);
	}

	fprintf(file,"%s %s.record at %s\r\n", type,prompt,currentTime);
	return file;

}

//打印脑电数据
void printEEGDate( unsigned char extendedCodeLevel,
                  unsigned char code,
                  unsigned char valueLength,
                  const unsigned char *value,
                  void *customData ) {


	int j;
	if ( extendedCodeLevel == 0 ) {
        switch ( code ) {
            case ( 0x16 ):
				fprintf(fp_blink,"%d\n",(unsigned char)(value[0] & 0xFF));
	            break;
            case (0x04):
				fprintf(fp_attention,"%d\n",(unsigned char)(value[0] & 0xFF));
				break;
            case ( 0x05 ):
				fprintf(fp_meditation,"%d\n",(unsigned char)(value[0] & 0xFF));
				break;
            case ( 0x02 ):
				fprintf(fp_poorSignal,"%d\n",(unsigned char)(value[0] & 0xFF));
				break;
            case (0x83):

				for (j=0;j<valueLength;j=j+3)
				{
					fprintf(fp_powerSignal,"%d,",((uint32_t)value[j] << 16) | ((uint32_t)value[j+1] << 8) | (uint32_t)value[j+2]);
				}
            	fprintf(fp_powerSignal,"\n");
                break;
            case ( 0x80 ):
				fprintf(fp_raw,"%d\n",(short) (( value[0] << 8 ) | value[1]));
                break;
            default:
                break;
        }
    }
}

int main(int argc ,char *argv[])
{
	unsigned char streamByte;
	ThinkGearStreamParser parser;

	printf("EEG Data Transform...\r\n");

	THINKGEAR_initParser( &parser, PARSER_TYPE_PACKETS,
			printEEGDate, NULL );

	//根据argv1提供的名字建立几个文件
	fp_blink=openAllFile(argv[1],argv[2],"blink");
	fp_attention=openAllFile(argv[1],argv[2],"attention");
	fp_meditation=openAllFile(argv[1],argv[2],"meditation");
	fp_poorSignal=openAllFile(argv[1],argv[2],"poorSignal");
	fp_powerSignal=openAllFile(argv[1],argv[2],"powerSignal");
	fprintf(fp_powerSignal,"%s\n","0.5~2.7,3.5~6.75,7.5~9.25,10~11.75,13~16.75,18~29.75,31~39.75,41~49.75");
	fp_raw=openAllFile(argv[1],argv[2],"raw");


	//打开原始数据文件
	FILE *stream = fopen(argv[3], "rb" );
	while ( 1 ) {
		int x=fread( &streamByte, 1, 1, stream );
		if (x!=1) break;
		THINKGEAR_parseByte( &parser, streamByte );

	}

	fclose(stream);

	//关闭所有输出文件
	fclose(fp_blink);
	fclose(fp_attention);
	fclose(fp_meditation);
	fclose(fp_poorSignal);
	fclose(fp_powerSignal);
	fclose(fp_raw);

	printf("EEG Data Transform complete!\r\n");

}

int sendData()
{
	eeg_outbuffer.crc16[0]=0;
	eeg_outbuffer.crc16[1]=0;

	int rlen = sizeof(eeg_outbuffer)*2;
	char outbuffer[sizeof(eeg_outbuffer)*2];

	unsigned short crc16=crc((unsigned char *)&eeg_outbuffer,sizeof(eeg_outbuffer));

	eeg_outbuffer.crc16[0]=(crc16 & 0xFF00)>>8;
	eeg_outbuffer.crc16[1]=(crc16 & 0xFF);

	printf("CRC=%x\n",eeg_outbuffer.crc16[0]);
	encode_buffer((char*)&eeg_outbuffer, rlen/2, outbuffer,&rlen);

	fwrite(outbuffer,rlen,1,raw);

	return 0;
}



int main23(int argc ,char *argv[])
{
	unsigned char streamByte;
	printf("start2...\n");

	EEG_OutBuffer eeg_outbuffer;
	ThinkGearStreamParser parser;
	THINKGEAR_initParser( &parser, PARSER_TYPE_PACKETS,
		                            printBlinks, NULL );
	FILE *stream = fopen( "D:/EEG/sin.dat", "rb" );
	//raw = fopen( "/Users/shijingwei/aa/data/transdata.dat", "wb" );
	openSimulationFile("D:/EEG/transdata.dat");

	int count=0;
	while ( 1 ) {

			int x=fread( &streamByte, 1, 1, stream );
			if (x!=1) break;
		    THINKGEAR_parseByte( &parser, streamByte );
		    count++;
		    //printf("end...%d\r\n",count);

		}

		fclose(stream);
		closeSimulationFile();
		printf("end...%d\r\n",count);

		return 0;


}
int main3(int argc ,char *argv[])
{
	FILE *stream = fopen( "/Users/shijingwei/aa/data/transdata.dat", "rb" );
	unsigned char inbuffer[250];
	unsigned char outbuffer[250];
	EEG_OutBuffer eeg_outbuffer;
	int count;
	char streamByte;
	printf("start...\n");
	count =0;
	while ( 1 )
	{
		int x=fread( &streamByte, 1, 1, stream );
		if (x!=1) break;

		switch (streamByte)
		{
			case PACK_BEGIN:
				//printf("bbbb\n");
				inbuffer[count++]=streamByte;
				break;
			case PACK_STOP:
			{
			//printf("aaaaa\n");
			inbuffer[count++]=streamByte;
			int len=sizeof(eeg_outbuffer);
			decode_buffer(inbuffer, count, (char *)&eeg_outbuffer,&len);
			printf("len=%d",len);
			unsigned short crc16_o=((unsigned short)eeg_outbuffer.crc16[0])<<8|eeg_outbuffer.crc16[1];
			eeg_outbuffer.crc16[0]=0;
			eeg_outbuffer.crc16[1]=0;
			unsigned short crc16_c=crc((unsigned char *)&eeg_outbuffer,sizeof(eeg_outbuffer));

			printf("CRC0=%x,CRCC=%x\n",crc16_o,crc16_c);
			if (crc16_o==crc16_c)
			{
				printf("CRC OK!\n");
			}
			else
			{
				printf("date--------------!\n");

				for (count=0;count<sizeof(eeg_outbuffer);count++)
				{

					printf("%x ",((unsigned char*)(&eeg_outbuffer)+count)[count]);

				}
				printf("date end--------------!\n");
			}
			count =0;
			break;
			}
			default:
				inbuffer[count++]=streamByte;
				break;
		}


	}
	printf("end...\n");


}

int main1(int argc ,char *argv[])
{

	EEG_OutBuffer eeg_outbuffer;
	ThinkGearStreamParser parser;
	THINKGEAR_initParser( &parser, PARSER_TYPE_PACKETS,
	                            printBlinks, NULL );
	FILE *stream = fopen( "/Users/shijingwei/aa/data/data.DAT", "rb" );
	raw = fopen( "/Users/shijingwei/aa/data/transdata.dat", "wb" );

	raw_receive =0;
	power_receive =0;
	raw_count =0;
	unsigned char streamByte;
	printf("starting...\r\n");
	int count=0;
	while ( 1 ) {

		printf("end =%d\r\n",count);
		int x=fread( &streamByte, 1, 1, stream );
		if (x!=1) break;
	    THINKGEAR_parseByte( &parser, streamByte );
	    printf("end =%d\r\n",count);
	    count++;
	}

	fclose(stream);
	fclose(raw);
	printf("end =%d\r\n",count);
	return 0;


	/*
	char buffer[1204];
	char outbuffer[1204];

	int i;
	int rlen;
	for (i=0;i<254;i++)
	{
		buffer[i]=(char)0xfE;

	}
	buffer[254]=(char)0xBB;

	printf("----------1-----------------\n");
	for (i=0;i<255;i++)
	{
		printf("%x ",(unsigned char)buffer[i]);

	}
	printf("\n----------2-----------------\n");

	rlen =512;
	encode_buffer(buffer,255,outbuffer,&rlen);
	for (i=0;i<rlen;i++)
	{
		printf("%x ",(unsigned char)outbuffer[i]);

	}

	printf("\n----------3-----------------\n");

	decode_buffer(outbuffer, rlen, buffer,&rlen);

	for (i=0;i<rlen;i++)
	{
			printf("%x ",(unsigned char)buffer[i]);

	}
	*/

}
