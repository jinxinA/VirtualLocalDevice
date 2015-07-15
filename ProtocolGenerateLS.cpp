#include "StdAfx.h"
#include "ProtocolGenerateLC.h"
#include "ProtocolGenerateLS.h"
#include "VirtualLocalDeviceDlg.h"
#include "CRC16.h"

// ѹ��BCD��ת��ֵ
inline BYTE CBCD2Byte(BYTE cbcd)
{
	return (cbcd&0x0F) + ((cbcd&0xF0)>>4)*10;
}
// ��ֵתѹ��BCD��
inline BYTE Byte2CBCD(BYTE bit)
{
	return (bit%10) + (bit/10)*16;
}
void InitStationsConfigBufLS(sStationsConfigBuf& scData)
{
	scData.id = 1;
	CTime tm;
	sLCReadStations8CConfigFrame* psc;
	psc = (sLCReadStations8CConfigFrame*)scData.buf;
	tm = CTime::GetCurrentTime();
	psc->year = Byte2CBCD(tm.GetYear() - 2000);
	psc->month = Byte2CBCD(tm.GetMonth());
	psc->day = Byte2CBCD(tm.GetDay());
	psc->hour = Byte2CBCD(tm.GetHour());
	psc->minute = Byte2CBCD(tm.GetMinute());
	psc->second = Byte2CBCD(tm.GetSecond());
	psc->week = tm.GetDayOfWeek()-1;
	psc->sid = 1;
	psc->protocol = 1;
	psc->interval = 1;
}

BOOL CheckDataLS(char* buf , int iSize , int& iSendSize , 
	sStationsConfigBuf& scData , CVirtualLocalDeviceDlg* pThis , LPWSTR str)
{
	if( scData.id != 1 )
		InitStationsConfigBufLS(scData);
	CString str1;
	sLiChengComOpFrame* pOP;
	sLCComReadData8CFrame* pDF;
	sLCComReadDataFrame* pDF2;
	sLCReadHistoryData8CFrame* pRHDF;
	sLHistoryDataItem8CFrame* pHDIF;
	sLCReadStations8CConfigFrame* pRSCF;
	sLCWriteStations8CConfigFrame* pWSCF;
	pOP = (sLiChengComOpFrame*)buf;
	pDF = (sLCComReadData8CFrame*)buf;
	pDF2 = (sLCComReadDataFrame*)buf;
	pRSCF = (sLCReadStations8CConfigFrame*)buf;
	pWSCF = (sLCWriteStations8CConfigFrame*)buf;
	pRHDF = (sLCReadHistoryData8CFrame*)buf;
	UINT uOP;
	INT minute;
	int i , k , dLen;
	if ( memcmp(buf,L"LC",4) == 0 ) // ��������
	{
		str1.Format(L"%s�յ���������" , str);
		LogMsg( str1.GetBuffer() );
		return FALSE;
	}
	if( pOP->rw == LCCOM_OP_ResetStations )
	{
		str1.Format(L"%s�յ��豸������Ϣ��" , str);
		LogMsg( str1.GetBuffer() );
		return FALSE;
	}
	uOP = GetSwapBitWord(pOP->op);

	if( pOP->rw == LCCOM_RW_OP_Read && pOP->op == 0x0000 )
	{
		pDF->len = 16;
		for ( i = 0 ; i < 8 ; i++ )
		{
			pDF->dac[i] = rand()%pThis->m_varR[i]+pThis->m_minVal[i];
			WordSwapBit(pDF->dac[i]);
		}
		pDF->dac[8] = crc16((unsigned char*)buf,19);
		WordSwapBit(pDF->dac[8]);
		iSendSize = 21;
		str1.Format(L"%s��ɲɼ��������롣" , str);
		LogMsg( str1.GetBuffer() );
	}
	else if( pOP->op == 0x1000 )
	{
		if( pOP->rw == LCCOM_RW_OP_Read )
		{
			pRSCF->len = 0x0A;
			memcpy( &pRSCF->year , &((sLCReadStations8CConfigFrame*)scData.buf)->year , 10 );
			pRSCF->crc16 = crc16((unsigned char*)buf,13);
			WordSwapBit(pRSCF->crc16);
			iSendSize = 15;
			str1.Format(L"%s��ɶ�ȡ�������롣" , str);
			LogMsg( str1.GetBuffer() );
		}
		else if( pOP->rw == LCCOM_RW_OP_Write )
		{
			memcpy( &((sLCReadStations8CConfigFrame*)scData.buf)->year , &pWSCF->year , 10 );

			pDF2->dac[1] = crc16( (unsigned char*)buf , 6 );
			WordSwapBit(pDF2->dac[1]);
			iSendSize = 8;
			str1.Format(L"%s���д���������롣" , str);
			LogMsg( str1.GetBuffer() );
		}
		else
		{
			str1.Format(L"%s�������ö�д��־����" , str);
			LogMsg( str1.GetBuffer() );
		}
	}
	else if( (pOP->rw == 0x41 || pOP->rw == 0x42) && pOP->op == 0x0000 )
	{ // ??????????
		dLen = scData.bLenFull?11:5;
		pRHDF->num = GetSwapBitWord(dLen);
		pRHDF->len = 0xF2;
		pHDIF = (sLHistoryDataItem8CFrame*)pRHDF->data;
		for( k = 0 ; k < dLen ; k++ )
		{
			minute = scData.s_minute++;
			pHDIF->minute = Byte2CBCD(minute%60);
			minute /= 60;
			pHDIF->hour = Byte2CBCD(minute%24);
			minute /= 24;
			pHDIF->day = Byte2CBCD(1+(minute%28));
			minute /= 28;
			pHDIF->month = Byte2CBCD(1+(minute%12));
			minute /= 12;
			pHDIF->second = Byte2CBCD(rand()%60);
			// ��14��ѭ��
			if( minute != 0 )
				scData.s_minute = 0;
			pHDIF->year = Byte2CBCD(14);
			for ( i = 0 ; i < 16 ; i++ )
			{
				pHDIF->data[i] = rand()%pThis->m_varR[i]+pThis->m_minVal[i];
				WordSwapBit(pHDIF->data[i]);
			}
			pHDIF++;
		}
		if( scData.bLenFull )
			str1.Format(L"%s�����ʷ�����������룬������(11��)���ݡ�" , str);
		else
			str1.Format(L"%s�����ʷ�����������룬����5�����ݡ�" , str);
		LogMsg( str1.GetBuffer() );
		scData.bLenFull ^= TRUE;
		pRHDF->crc16 = crc16((unsigned char*)buf,247);
		//pRHDF->crc16++;
		WordSwapBit(pRHDF->crc16);
		iSendSize = 249;
	}

	if( iSendSize <= 0 )
		return FALSE;
	return TRUE;
}