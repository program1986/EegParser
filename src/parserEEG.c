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

EEG_OutBuffer eeg_outbuffer;
ThinkGearStreamParser thinkGearStreamParser;

FILE *raw;
int mm;
int raw_receive;
int power_receive;
int raw_count;


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
