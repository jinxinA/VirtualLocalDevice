#pragma once

#define LCCOM_OP_RealTimeData  0x00   // ʵʱ������������  , ͨ������  , ���� 
#define LCCOM_OP_SystemConfig  0x20   // ϵͳ��������  , ͨ������  , ��д��������Ϊ����
#define LCCOM_OP_ReadHistoryData  0x37   // ����ʷ����  , ͨ������  , ���� 
#define LCCOM_OP_HistoryDataReread  0x38   // ��ʷ�����ض�  , ͨ������  , ���� 
#define LCCOM_OP_GPRSLoginPassword  0x61   // ���õ�¼GPRS����������  , ��֧��  ,��д��������Ϊ����
#define LCCOM_OP_ResetStations  0x80   // ��λ����վ  , ͨ������  , ��д 

#define LCCOM_OP_SystemConfigWrite  0x0100   // ϵͳ��������д�루LCCOM_OP_SystemConfig��  , ͨ������  , д 
#define LCCOM_OP_GPRSLoginPasswordWrite  0x0200   // ���õ�¼GPRS����������  , ��֧��  , д 

#define LCCOM_RW_OP_Read  0x03   // ��(0x03)/д(0x10)������ ��
#define LCCOM_RW_OP_Write  0x10   // ��(0x03)/д(0x10)������ д

#define WordSwapBit(A)  A=((((WORD)(A)&0xff00) >> 8 ) | (((WORD)(A)&0x00ff) << 8))
#define GetSwapBitWord(A)  ((((WORD)(A)&0xff00) >> 8 ) | (((WORD)(A)&0x00ff) << 8))

#pragma pack(push)
#pragma pack(1)
// ��������֡�ṹ
struct sLiChengComOpFrame
{
	BYTE id; // ����վͨѶ��ַ 	Ψһ 
	BYTE rw; // ��(0x03)/д(0x10)������ 	2ѡ1 
	WORD op; // �����룬ǰ1�ֽڱ���Ϊ0 	00 xx 
	WORD len; // �����򳤶ȣ����������� 
	WORD dac[9]; // ���� �� CRC16������(��ǰ�ߺ�)
};

// ��������֡�ṹ
struct sLCComReadDataFrame
{
	BYTE id; // ����վͨѶ��ַ 	Ψһ 
	BYTE rw; // ����(0x03) 
	WORD len; // �����򳤶ȣ����������� 
	WORD dac[17]; // ���� �� CRC16������(��ǰ�ߺ�)
};

// �ɼ������ö�ȡ֡�ṹ
struct sLCReadStationsConfigFrame
{
	BYTE id; // ����վͨѶ��ַ 	Ψһ 
	BYTE rw; // ����(0x03) 
	WORD len; // �����򳤶ȣ����������� 

	BYTE year; // �� 	0B(2011��)  
	BYTE month; // �� 	08(8��) 
	BYTE day; // �� 	1F (31��) 
	BYTE hour; // ʱ 	0B(12��/24Сʱ��)
	BYTE minute; // �� 	00(00����) 
	BYTE interval; // �洢��� 	10(����) 
	BYTE language; // ��������0/1	1-Ӣ��/0-���� 
	BYTE sid; // �豸��ַ 	01(0x01-0xFE) 

	WORD crc16; // ���� �� CRC16������(��ǰ�ߺ�)
};
// �ɼ�������д��֡�ṹ
struct sLCWriteStationsConfigFrame
{
	BYTE id; // ����վͨѶ��ַ 	Ψһ 
	BYTE rw; // д(0x10) 	0x10 
	WORD op; // �����룬0x00 0x20 	��ǰ�ͺ� 
	WORD regc; // 0x00 0x04 	��ǰ�ͺ� 
	BYTE len; // ���ݳ��� 0x08 	8�������ֽ� 

	BYTE year; // �� 	0B(2011��)  
	BYTE month; // �� 	08(8��) 
	BYTE day; // �� 	1F (31��) 
	BYTE hour; // ʱ 	0B(12��/24Сʱ��)
	BYTE minute; // �� 	00(00����) 
	BYTE interval; // �洢��� 	10(����) 
	BYTE language; // ��������0/1	1-Ӣ��/0-���� 
	BYTE sid; // 00 	����/��ַ ������ȶ�ȡϵͳ������Ȼ��ʹ�ö�ȡ���豸��ַ�����ֽ�

	WORD crc16; // ���� �� CRC16������(��ǰ�ߺ�)
};

// �ɼ��ǲ���
struct sLCStationsConfig
{
	BYTE year; // �� 	0B(2011��)  
	BYTE month; // �� 	08(8��) 
	BYTE day; // �� 	1F (31��) 
	BYTE hour; // ʱ 	0B(12��/24Сʱ��)
	BYTE minute; // �� 	00(00����) 
	BYTE interval; // �洢��� 	10(����) 
	BYTE language; // ��������0/1	1-Ӣ��/0-���� 
	BYTE sid; // 00 	����/��ַ ������ȶ�ȡϵͳ������Ȼ��ʹ�ö�ȡ���豸��ַ�����ֽ�
};

// ��ʷ���ݶ�ȡ֡�ṹ
struct sLCReadHistoryDataFrame
{
	BYTE id; // ����վͨѶ��ַ 	Ψһ 
	BYTE rw; // ����(0x03) 
	WORD len; // �����򳤶ȣ�0x04 0x0E 	��ǰ�ͺ� 
	WORD fLen; // ��Ч����֡�� ��0x00 0x1C 	��ǰ�ͺ�  

	BYTE data[1036]; // ��ʷ���ݰ� 	�ֳ�N֡ 

	WORD crc16; // ���� �� CRC16������(��ǰ�ߺ�)
};

// ��ʷ���ݽṹ
struct sLHistoryDataItemFrame
{
	BYTE year; // �� 	0B(2000��)  
	BYTE month; // �� 	08(8��) 
	BYTE day; // �� 	1F (31��) 
	BYTE hour; // ʱ 	0B(12��/24Сʱ��)
	BYTE minute; // �� 	00(00����) 

	WORD data[16]; // ͨ������
};

// ��ȡ��¼����֡�ṹ
struct sLCReadPassWordFrame
{
	BYTE id; // ����վͨѶ��ַ 	Ψһ 
	BYTE rw; // ����(0x03) 
	WORD op; // �����룬0x00 0x61 	��ǰ�ͺ� 
	WORD len; // �����򳤶ȣ�0x00 0x04 	��ǰ�ͺ� 

	BYTE pw[4]; // 4λ�������� 	������ 

	WORD crc16; // ���� �� CRC16������(��ǰ�ߺ�)
};

// ���̱����ṹ
struct sStationsConfigBuf
{
	INT id;
	INT s_minute;
	BOOL bLenFull;
	BYTE buf[244];
	sStationsConfigBuf()
	{
		id = -1;
		s_minute = 0;
		bLenFull = FALSE;
	}
};
#pragma pack(pop)