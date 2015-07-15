#include "StdAfx.h"
#include "ProtocolGenerateLC.h"
#include "ProtocolGenerateSun.h"
#include "VirtualLocalDeviceDlg.h"
#include "CRC16.h"



void InitStationsConfigBufSun(sStationsConfigBuf& scData)
{
	scData.id = 2;
}

BOOL CheckDataSun(char* buf , int iSize , int& iSendSize , 
	sStationsConfigBuf& scData , CVirtualLocalDeviceDlg* pThis , LPWSTR str)
{
	if( scData.id != 2 )
		InitStationsConfigBufSun(scData);
	// 读数据
	if( iSize == 6 && memcmp(buf , "#030CG" , 6) == 0 )
	{
		CStringA strA = "030C001016280601120000000000000000011f800080008000000080000000000026bf00b900000000000000b1001E001E0000000000008600GG";
		iSendSize = strA.GetLength();
		memcpy(buf , strA.GetBuffer() , iSendSize);
		LogMsg( L"读数据请求" );
		return TRUE;
	}
	// 写时间
	else if( iSize == 21 && memcmp(buf , "#010A" , 5) == 0 && memcmp(&buf[19] , "GG" , 2) == 0 )
	{
		LogMsg( L"写时间请求" );
	}
	return FALSE;
}