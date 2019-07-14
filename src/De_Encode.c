/*
 * de_eccode.c
 *
 *  Created on: 2019年7月11日
 *      Author: shijingwei
 *  编解码程序，将指定数据变成0xFF开头，0xFE结尾的数据，数据内部的0xFF，0xFE替换。
 *  已经测试的数据
 *  1）0x00-0xFF ：测试通过
 *  2）全部是0xFF ：测试通过
 *  3）全部是0xFE ：测试通过
 */




#include "De_Encode.h"
#include "stdio.h"


/*
*Function: encode_buffer
*Description: 编码程序，将指定数据进行编码
*Input:
*  buffer：待编码的数据
*  len：输入缓冲的数据长度
*  outLen:输出缓冲数据长度，如果编码程序发现缓冲大小不够，将返回！ENCODE_SUCCESS
*Output:
*  outBuf:输出数据缓冲，应该比输入缓冲大至少一倍
*  outLen:缓冲的数据长度
Return: ENCODE_SUCCESS 成功，否则失败
*/
int encode_buffer(char * buffer, int len, char *outBuf, int *outLen)
{
	int i=0;
	int index = 0;
	index++;
	for(i=0;i<len;i++)
	{
		if(buffer[i] == TRANS_BASE)
		{
			index++;
			index++;
		}
		else if(buffer[i] == PACK_STOP)
		{
			index++;
			index++;
		}
		else if(buffer[i] == PACK_BEGIN)
		{
			index++;
			index++;
		}
		else
		{
			index++;
		}
	}
	index++;

	if(*outLen <index || outBuf == NULL)
	{
		*outLen = index;
		return !ENCODE_SUCCESS;
	}

	*outLen = index;
	index = 0;
	outBuf[index] = PACK_BEGIN;
	index++;

	for(i=0;i<len;i++)
	{
		if(buffer[i] == TRANS_BASE)
		{
			outBuf[index] = TRANS_BASE;
			index++;
			outBuf[index] = 0x00;
			index++;
		}
		else if(buffer[i] == PACK_STOP)
		{
			outBuf[index] = TRANS_BASE;
			index++;
			outBuf[index] = 0x02;
			index++;
		}
		else if(buffer[i] == PACK_BEGIN)
		{
			outBuf[index] = TRANS_BASE;
			index++;
			outBuf[index] = 0x01;
			index++;
		}
		else
		{
			outBuf[index] = buffer[i];
			index++;
		}
	}
	outBuf[index] = PACK_STOP;
	index++;

	return ENCODE_SUCCESS;
}

/*
*Function: decode_buffer
*Description: 解码程序，将编码数据还原
*Input:
*  buffer：原始数据缓冲
*  len：数据长度
*Output:
*  outBuf:输出数据缓冲,和原始数据一样长即可
*  outLen:解完后的实际长度
*  Return: DECODE_SUCCESS 成功，否则失败
*/

int decode_buffer(char * buffer, int len, char *outBuf,int *outLen)
{
	int i = 0;

	if(buffer[0] != PACK_BEGIN || buffer[len-1] != PACK_STOP)
	{
		//printf("0=%x,1=%x\n",buffer[0],buffer[1]);
		return !DECODE_SUCCESS;
	}

	int count = 0;
	for(i=1;i<len;i++)
	{
		if(buffer[i] == TRANS_BASE)
		{
			if(buffer[i+1] == 0x00)
			{
				//to 0xFD
				count ++;
				i++;
			}
			else if(buffer[i+1] == 0x01)
			{
				//to 0xFF
				count++;
				i++;
			}
			else if(buffer[i+1] == 0x02)
			{
				//to 0xFE
				count++;
				i++;
			}
			else
			{
				//error happend
				count++;
			}
		}
		else
		{
			// keep unchanged
			count++;
		}
	}

	*outLen =count-1;

	count = 0;
	for(i=1;i<len;i++)
	{
		if(buffer[i] == TRANS_BASE)
		{
			if(buffer[i+1] == 0x00)
			{
				//to 0xFD
				outBuf[count] = TRANS_BASE;
				count ++;
				i++;
			}
			else if(buffer[i+1] == 0x01)
			{
				//to 0xFF
				outBuf[count] = PACK_BEGIN;
				count++;
				i++;
			}
			else if(buffer[i+1] == 0x02)
			{
				//to 0xFE
				outBuf[count] = PACK_STOP;
				count++;
				i++;
			}
			else
			{
				//error happend
				outBuf[count] = buffer[i];
				count++;
			}
		}
		else
		{
			// keep unchanged
			outBuf[count] = buffer[i];
			count++;
		}
	}

	return DECODE_SUCCESS;
}
