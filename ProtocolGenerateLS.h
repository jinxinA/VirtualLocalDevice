#pragma once


#pragma pack(push)
#pragma pack(1)
// ��������֡�ṹ
struct sLCComReadData8CFrame
{
	BYTE id; // ����վͨѶ��ַ 	Ψһ 
	BYTE rw; // ����(0x03) 
	BYTE len; // �����򳤶ȣ����������� 
	WORD dac[9]; // ���� �� CRC16������(��ǰ�ߺ�)
};
// �ɼ������ö�ȡ֡�ṹ���ܳ���15
struct sLCReadStations8CConfigFrame
{
	BYTE id; // ����վͨѶ��ַ 	Ψһ 
	BYTE rw; // ����(0x03) 
	BYTE len; // ���ݳ��� 0x0A 	10�������ֽ� 

	BYTE year; // �� 	0B(2011��)  
	BYTE month; // �� 	08(8��) 
	BYTE week; // ���� 	01(����1) 
	BYTE day; // �� 	1F (31��) 
	BYTE hour; // ʱ 	0B(12��/24Сʱ��)
	BYTE minute; // �� 	00(00����) 
	BYTE second; // �� 	00(00��) 
	BYTE interval; // �洢��� 	10(����) 
	BYTE protocol; // ͨѶЭ��
	BYTE sid; // 00 	����/��ַ ������ȶ�ȡϵͳ������Ȼ��ʹ�ö�ȡ���豸��ַ�����ֽ�

	WORD crc16; // ���� �� CRC16������(��ǰ�ߺ�)
};
// �ɼ�������д��֡�ṹ���ܳ���19
struct sLCWriteStations8CConfigFrame
{
	BYTE id; // ����վͨѶ��ַ 	Ψһ 
	BYTE rw; // д(0x10) 	0x10 
	WORD op; // �����룬0x00 0x10 	��ǰ�ͺ� 
	WORD num; // �Ĵ������� ��0x00 0x05 	��ǰ�ͺ� 5����
	BYTE len; // ���ݳ��� 0x0A 	10�������ֽ� 

	BYTE year; // �� 	0B(2011��)  
	BYTE month; // �� 	08(8��) 
	BYTE week; // ���� 	01(����1) 
	BYTE day; // �� 	1F (31��) 
	BYTE hour; // ʱ 	0B(12��/24Сʱ��)
	BYTE minute; // �� 	00(00����) 
	BYTE second; // �� 	00(00��) 
	BYTE interval; // �洢��� 	10(����) 
	BYTE protocol; // ͨѶЭ��
	BYTE sid; // 00 	����/��ַ ������ȶ�ȡϵͳ������Ȼ��ʹ�ö�ȡ���豸��ַ�����ֽ�

	WORD crc16; // ���� �� CRC16������(��ǰ�ߺ�)
};


// ��ʷ���ݶ�ȡ֡�ṹ���ܳ���249
struct sLCReadHistoryData8CFrame
{
	BYTE id; // ����վͨѶ��ַ 	Ψһ 
	BYTE op; // ������(0x41) 
	WORD num; // ��¼�� 0x0B 11����¼
	BYTE len; // ���ݳ��� 0xF2 	242�������ֽ� 

	BYTE data[242]; // �̶�11����¼������242�ֽ� 

	WORD crc16; // ���� �� CRC16������(��ǰ�ߺ�)
};

// ��ʷ���ݽṹ���ܳ���22
struct sLHistoryDataItem8CFrame
{
	BYTE year; // �� 	0B(2000��)
	BYTE month; // �� 	08(8��) 
	BYTE day; // �� 	1F (31��) 
	BYTE hour; // ʱ 	0B(12��/24Сʱ��)
	BYTE minute; // �� 	00(00����) 
	BYTE second; // �� 	00(00��) 

	WORD data[8]; // ͨ������
};

#pragma pack(pop)

