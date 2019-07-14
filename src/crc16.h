/*
 * crc16.h
 *
 *  Created on: 2019年7月11日
 *      Author: shijingwei
 */

#ifndef SRC_CRC16_H_
#define SRC_CRC16_H_

#ifdef __cplusplus
extern "C" {
#endif


/**
@ brief ：CRC 校验
@ input ：puchMsg ：待校验数据域 usDataLen：校验数据长度
@output ：校验值
**/
unsigned short crc(unsigned char *puchMsg,unsigned short usDataLen);


#ifdef __cplusplus
}  /* extern "C" */
#endif
#endif /* SRC_CRC16_H_ */
