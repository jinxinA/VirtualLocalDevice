#include "StdAfx.h"
#include "ProtocolGenerateLC.h"
#include "VirtualLocalDeviceDlg.h"
#include "CRC16.h"


void InitStationsConfigBufLC(sStationsConfigBuf& scData)
{
	scData.id = 0;
	CTime tm;
	sLCStationsConfig* psc;
	psc = (sLCStationsConfig*)scData.buf;
	tm = CTime::GetCurrentTime();
	psc->year = tm.GetYear() - 2000;
	psc->month = tm.GetMonth();
	psc->day = tm.GetDay();
	psc->hour = tm.GetHour();
	psc->minute = tm.GetMinute();
	psc->sid = 1;
	psc->language = 0;
	psc->interval = 1;
}
BYTE sendCheckData[] = {0x01,0x03,0x00,0x2F,0x00,0x0E,0x0A,0x22,0x0F,0x01,0x03,0x01,0x01,0x02,0x02,0x01,0x08,0x01,0x06,0x08,0xEF,0x04};
BOOL CheckDataLC(char* buf , int iSize , int& iSendSize , 
	sStationsConfigBuf& scData , CVirtualLocalDeviceDlg* pThis , LPWSTR str)
{
	if( scData.id != 0 )
		InitStationsConfigBufLC(scData);
	CString str1;
	sLiChengComOpFrame* pOP;
	sLCComReadDataFrame* pDF;
	sLCReadHistoryDataFrame* pRHDF;
	sLHistoryDataItemFrame* pHDIF;
	sLCReadStationsConfigFrame* pRSCF;
	sLCWriteStationsConfigFrame* pWSCF;
	pOP = (sLiChengComOpFrame*)buf;
	pDF = (sLCComReadDataFrame*)buf;
	pRSCF = (sLCReadStationsConfigFrame*)buf;
	pWSCF = (sLCWriteStationsConfigFrame*)buf;
	pRHDF = (sLCReadHistoryDataFrame*)buf;
	UINT uOP;
	INT minute;
	int i , k , dLen;
	if ( memcmp(buf,L"LC",4) == 0 ) // 心跳数据
	{
		str1.Format(L"%s收到心跳包。" , str);
		LogMsg( str1.GetBuffer() );
		return FALSE;
	}
	if( pOP->rw == LCCOM_OP_ResetStations )
	{
		str1.Format(L"%s收到设备重置消息。" , str);
		LogMsg( str1.GetBuffer() );
		return FALSE;
	}
	uOP = GetSwapBitWord(pOP->op);
	switch(uOP)
	{
	case LCCOM_OP_RealTimeData:
		if( pOP->rw != LCCOM_RW_OP_Read)
		{
			str1.Format(L"%s采集数据读写标志错误。" , str);
			LogMsg( str1.GetBuffer() );
			break;
		}
		pDF->len = GetSwapBitWord(32);
		for ( i = 0 ; i < 16 ; i++ )
		{
			pDF->dac[i] = rand()%pThis->m_varR[i]+pThis->m_minVal[i];
			WordSwapBit(pDF->dac[i]);
		}
		pDF->dac[16] = crc16((unsigned char*)buf,sizeof(sLCComReadDataFrame)-2);
		WordSwapBit(pDF->dac[16]);
		iSendSize = sizeof(sLCComReadDataFrame);
		str1.Format(L"%s完成采集数据申请。" , str);
		LogMsg( str1.GetBuffer() );
		break;
	case LCCOM_OP_SystemConfig:
		if( pOP->rw == LCCOM_RW_OP_Read )
		{
			pRSCF->len = GetSwapBitWord(8);
			memcpy( &pRSCF->year , &((sLCStationsConfig*)scData.buf)->year , 8 );
			pRSCF->crc16 = crc16((unsigned char*)buf,sizeof(sLCReadStationsConfigFrame)-2);
			WordSwapBit(pRSCF->crc16);
			iSendSize = sizeof(sLCReadStationsConfigFrame);
			str1.Format(L"%s完成读取配置申请。" , str);
			LogMsg( str1.GetBuffer() );
		}
		else if( pOP->rw == LCCOM_RW_OP_Write )
		{
			memcpy( &((sLCStationsConfig*)scData.buf)->year , &pWSCF->year , sizeof(sLCStationsConfig) );
			pDF->dac[0] = GetSwapBitWord(4);
			pDF->dac[1] = crc16( (unsigned char*)buf , sizeof(sLCStationsConfig)-2 );
			WordSwapBit(pDF->dac[1]);
			iSendSize = 8;
			str1.Format(L"%s完成写入配置申请。" , str);
			LogMsg( str1.GetBuffer() );
		}
		else
		{
			str1.Format(L"%s参数设置读写标志错误。" , str);
			LogMsg( str1.GetBuffer() );
		}
		break;
	case LCCOM_OP_ReadHistoryData:
	case LCCOM_OP_HistoryDataReread:
		dLen = scData.bLenFull?28:14;
		pRHDF->fLen = GetSwapBitWord(dLen);
		pRHDF->len = 0x0E04;
		pHDIF = (sLHistoryDataItemFrame*)pRHDF->data;
		for( k = 0 ; k < dLen ; k++ )
		{
			minute = scData.s_minute++;
			pHDIF->minute = minute%60;
			minute /= 60;
			pHDIF->hour = minute%24;
			minute /= 24;
			pHDIF->day = 1+(minute%28);
			minute /= 28;
			pHDIF->month = 1+(minute%12);
			minute /= 12;
			// 仅14年循环
			if( minute != 0 )
				scData.s_minute = 0;
			pHDIF->year = 14;
			for ( i = 0 ; i < 16 ; i++ )
			{
				pHDIF->data[i] = rand()%pThis->m_varR[i]+pThis->m_minVal[i];
				WordSwapBit(pHDIF->data[i]);
			}
			pHDIF++;
		}
		if( scData.bLenFull )
			str1.Format(L"%s完成历史数据下载申请，返回满(28个)数据。" , str);
		else
			str1.Format(L"%s完成历史数据下载申请，返回14个数据。" , str);
		LogMsg( str1.GetBuffer() );
		scData.bLenFull ^= TRUE;
		pRHDF->crc16 = crc16((unsigned char*)buf,1042);
		//pRHDF->crc16++;
		WordSwapBit(pRHDF->crc16);
		iSendSize = 1044;
		break;
	case 0x2F:
		// 读序列号
		iSendSize = sizeof(sendCheckData);
		if( scData.bLenFull )
		{
			memcpy( buf , sendCheckData , iSendSize );
			str1.Format(L"%s完成设备校验申请，返回有效数据。" , str);
			LogMsg( str1.GetBuffer() );
		}
		else
		{
			//memcpy( buf , sendCheckData , iSendSize );
			//iSendSize = 22;
			str1.Format(L"%s完成设备校验申请，返回无效数据。" , str);
			LogMsg( str1.GetBuffer() );
		}
		scData.bLenFull ^= TRUE;

		break;
	}
	if( iSendSize <= 0 )
		return FALSE;
	return TRUE;
}
