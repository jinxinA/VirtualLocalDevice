#include "StdAfx.h"
#include "ProtocolGenerateLC.h"
#include "ProtocolGenerateDebug.h"
#include "VirtualLocalDeviceDlg.h"
#include "CRC16.h"



void InitStationsConfigBufDebug(sStationsConfigBuf& scData)
{
	scData.id = 100;
}

BOOL CheckDataDebug(char* buf , int iSize , int& iSendSize , 
	sStationsConfigBuf& scData , CVirtualLocalDeviceDlg* pThis , LPWSTR str)
{
	if( scData.id != 100 )
		InitStationsConfigBufDebug(scData);
	CString str1 , strBuf;
	LPWSTR pstr;
	scData.s_minute++;
	switch( scData.s_minute )
	{
	case 1:
		iSendSize = iSize;
		pstr = strBuf.GetBuffer(iSize*3+1);
		GetBufStr(pstr , (PBYTE)buf , iSize);
		pstr[iSize*3] = 0;
		str1 = L"原数据返回：";
		str1 += pstr;
		LogMsg( str1.GetBuffer() );
		break;
	default:
		scData.s_minute = 0;
	}
	return TRUE;
}