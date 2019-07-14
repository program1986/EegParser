/*
 * de_encode.h
 *
 *  Created on: 2019年7月11日
 *      Author: shijingwei
 */

#ifndef SRC_DE_ENCODE_H_
#define SRC_DE_ENCODE_H_

#define		PACK_BEGIN		(char)0xFF
#define		PACK_STOP		(char)0xFE
#define		TRANS_BASE		(char)0xFD
#define     ENCODE_SUCCESS  0
#define     DECODE_SUCCESS  0

#ifdef __cplusplus
extern "C" {
#endif

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
int encode_buffer(char * buffer, int len, char *outBuf, int *outLen);

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

int decode_buffer(char * buffer, int len, char *outBuf,int *outLen);




#ifdef __cplusplus
}  /* extern "C" */
#endif



#endif /* SRC_DE_ENCODE_H_ */
