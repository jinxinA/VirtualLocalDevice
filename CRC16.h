//---------------------------------------------------------------------------
#pragma once


unsigned short CRC16(unsigned char *puchMsg,unsigned short  usDataLen);
//WORD crc16(unsigned char *addr, int num, WORD crc = 0xFFFF);
WORD crc16(unsigned char *buf,WORD len, WORD crc = 0xFFFF);
BOOL GetBufStr( PWSTR reStr , PBYTE buf , DWORD len ); 
BOOL GetStrBuf( PBYTE reBuf , PWSTR str , DWORD len ); 
//---------------------------------------------------------------------------
