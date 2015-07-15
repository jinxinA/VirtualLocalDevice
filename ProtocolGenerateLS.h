#pragma once


#pragma pack(push)
#pragma pack(1)
// 接收数据帧结构
struct sLCComReadData8CFrame
{
	BYTE id; // 气象站通讯地址 	唯一 
	BYTE rw; // 仅读(0x03) 
	BYTE len; // 数据域长度，不包括本域 
	WORD dac[9]; // 数据 和 CRC16检验码(低前高后)
};
// 采集仪设置读取帧结构，总长：15
struct sLCReadStations8CConfigFrame
{
	BYTE id; // 气象站通讯地址 	唯一 
	BYTE rw; // 仅读(0x03) 
	BYTE len; // 数据长度 0x0A 	10个数据字节 

	BYTE year; // 年 	0B(2011年)  
	BYTE month; // 月 	08(8月) 
	BYTE week; // 星期 	01(星期1) 
	BYTE day; // 日 	1F (31日) 
	BYTE hour; // 时 	0B(12点/24小时制)
	BYTE minute; // 分 	00(00分钟) 
	BYTE second; // 秒 	00(00秒) 
	BYTE interval; // 存储间隔 	10(分钟) 
	BYTE protocol; // 通讯协议
	BYTE sid; // 00 	保留/地址 最好是先读取系统参数，然后使用读取的设备地址填充该字节

	WORD crc16; // 数据 和 CRC16检验码(低前高后)
};
// 采集仪设置写入帧结构，总长：19
struct sLCWriteStations8CConfigFrame
{
	BYTE id; // 气象站通讯地址 	唯一 
	BYTE rw; // 写(0x10) 	0x10 
	WORD op; // 命令码，0x00 0x10 	高前低后 
	WORD num; // 寄存器数量 ，0x00 0x05 	高前低后 5个字
	BYTE len; // 数据长度 0x0A 	10个数据字节 

	BYTE year; // 年 	0B(2011年)  
	BYTE month; // 月 	08(8月) 
	BYTE week; // 星期 	01(星期1) 
	BYTE day; // 日 	1F (31日) 
	BYTE hour; // 时 	0B(12点/24小时制)
	BYTE minute; // 分 	00(00分钟) 
	BYTE second; // 秒 	00(00秒) 
	BYTE interval; // 存储间隔 	10(分钟) 
	BYTE protocol; // 通讯协议
	BYTE sid; // 00 	保留/地址 最好是先读取系统参数，然后使用读取的设备地址填充该字节

	WORD crc16; // 数据 和 CRC16检验码(低前高后)
};


// 历史数据读取帧结构，总长：249
struct sLCReadHistoryData8CFrame
{
	BYTE id; // 气象站通讯地址 	唯一 
	BYTE op; // 命令码(0x41) 
	WORD num; // 记录数 0x0B 11条记录
	BYTE len; // 数据长度 0xF2 	242个数据字节 

	BYTE data[242]; // 固定11条记录，长度242字节 

	WORD crc16; // 数据 和 CRC16检验码(低前高后)
};

// 历史数据结构，总长：22
struct sLHistoryDataItem8CFrame
{
	BYTE year; // 年 	0B(2000年)
	BYTE month; // 月 	08(8月) 
	BYTE day; // 日 	1F (31日) 
	BYTE hour; // 时 	0B(12点/24小时制)
	BYTE minute; // 分 	00(00分钟) 
	BYTE second; // 秒 	00(00秒) 

	WORD data[8]; // 通道数据
};

#pragma pack(pop)

