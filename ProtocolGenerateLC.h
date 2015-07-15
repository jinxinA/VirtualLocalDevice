#pragma once

#define LCCOM_OP_RealTimeData  0x00   // 实时环境参数数据  , 通用类型  , 仅读 
#define LCCOM_OP_SystemConfig  0x20   // 系统参数配置  , 通用类型  , 读写（参数中为读）
#define LCCOM_OP_ReadHistoryData  0x37   // 读历史数据  , 通用类型  , 仅读 
#define LCCOM_OP_HistoryDataReread  0x38   // 历史数据重读  , 通用类型  , 仅读 
#define LCCOM_OP_GPRSLoginPassword  0x61   // 配置登录GPRS服务器密码  , 需支持  ,读写（参数中为读）
#define LCCOM_OP_ResetStations  0x80   // 复位气象站  , 通用类型  , 仅写 

#define LCCOM_OP_SystemConfigWrite  0x0100   // 系统参数配置写入（LCCOM_OP_SystemConfig）  , 通用类型  , 写 
#define LCCOM_OP_GPRSLoginPasswordWrite  0x0200   // 配置登录GPRS服务器密码  , 需支持  , 写 

#define LCCOM_RW_OP_Read  0x03   // 读(0x03)/写(0x10)操作码 读
#define LCCOM_RW_OP_Write  0x10   // 读(0x03)/写(0x10)操作码 写

#define WordSwapBit(A)  A=((((WORD)(A)&0xff00) >> 8 ) | (((WORD)(A)&0x00ff) << 8))
#define GetSwapBitWord(A)  ((((WORD)(A)&0xff00) >> 8 ) | (((WORD)(A)&0x00ff) << 8))

#pragma pack(push)
#pragma pack(1)
// 发送命令帧结构
struct sLiChengComOpFrame
{
	BYTE id; // 气象站通讯地址 	唯一 
	BYTE rw; // 读(0x03)/写(0x10)操作码 	2选1 
	WORD op; // 命令码，前1字节必须为0 	00 xx 
	WORD len; // 数据域长度，不包括本域 
	WORD dac[9]; // 数据 和 CRC16检验码(低前高后)
};

// 接收数据帧结构
struct sLCComReadDataFrame
{
	BYTE id; // 气象站通讯地址 	唯一 
	BYTE rw; // 仅读(0x03) 
	WORD len; // 数据域长度，不包括本域 
	WORD dac[17]; // 数据 和 CRC16检验码(低前高后)
};

// 采集仪设置读取帧结构
struct sLCReadStationsConfigFrame
{
	BYTE id; // 气象站通讯地址 	唯一 
	BYTE rw; // 仅读(0x03) 
	WORD len; // 数据域长度，不包括本域 

	BYTE year; // 年 	0B(2011年)  
	BYTE month; // 月 	08(8月) 
	BYTE day; // 日 	1F (31日) 
	BYTE hour; // 时 	0B(12点/24小时制)
	BYTE minute; // 分 	00(00分钟) 
	BYTE interval; // 存储间隔 	10(分钟) 
	BYTE language; // 语言类型0/1	1-英语/0-中文 
	BYTE sid; // 设备地址 	01(0x01-0xFE) 

	WORD crc16; // 数据 和 CRC16检验码(低前高后)
};
// 采集仪设置写入帧结构
struct sLCWriteStationsConfigFrame
{
	BYTE id; // 气象站通讯地址 	唯一 
	BYTE rw; // 写(0x10) 	0x10 
	WORD op; // 命令码，0x00 0x20 	高前低后 
	WORD regc; // 0x00 0x04 	高前低后 
	BYTE len; // 数据长度 0x08 	8个数据字节 

	BYTE year; // 年 	0B(2011年)  
	BYTE month; // 月 	08(8月) 
	BYTE day; // 日 	1F (31日) 
	BYTE hour; // 时 	0B(12点/24小时制)
	BYTE minute; // 分 	00(00分钟) 
	BYTE interval; // 存储间隔 	10(分钟) 
	BYTE language; // 语言类型0/1	1-英语/0-中文 
	BYTE sid; // 00 	保留/地址 最好是先读取系统参数，然后使用读取的设备地址填充该字节

	WORD crc16; // 数据 和 CRC16检验码(低前高后)
};

// 采集仪参数
struct sLCStationsConfig
{
	BYTE year; // 年 	0B(2011年)  
	BYTE month; // 月 	08(8月) 
	BYTE day; // 日 	1F (31日) 
	BYTE hour; // 时 	0B(12点/24小时制)
	BYTE minute; // 分 	00(00分钟) 
	BYTE interval; // 存储间隔 	10(分钟) 
	BYTE language; // 语言类型0/1	1-英语/0-中文 
	BYTE sid; // 00 	保留/地址 最好是先读取系统参数，然后使用读取的设备地址填充该字节
};

// 历史数据读取帧结构
struct sLCReadHistoryDataFrame
{
	BYTE id; // 气象站通讯地址 	唯一 
	BYTE rw; // 仅读(0x03) 
	WORD len; // 数据域长度，0x04 0x0E 	高前低后 
	WORD fLen; // 有效数据帧数 ，0x00 0x1C 	高前低后  

	BYTE data[1036]; // 历史数据包 	分成N帧 

	WORD crc16; // 数据 和 CRC16检验码(低前高后)
};

// 历史数据结构
struct sLHistoryDataItemFrame
{
	BYTE year; // 年 	0B(2000年)  
	BYTE month; // 月 	08(8月) 
	BYTE day; // 日 	1F (31日) 
	BYTE hour; // 时 	0B(12点/24小时制)
	BYTE minute; // 分 	00(00分钟) 

	WORD data[16]; // 通道数据
};

// 读取登录密码帧结构
struct sLCReadPassWordFrame
{
	BYTE id; // 气象站通讯地址 	唯一 
	BYTE rw; // 仅读(0x03) 
	WORD op; // 命令码，0x00 0x61 	高前低后 
	WORD len; // 数据域长度，0x00 0x04 	高前低后 

	BYTE pw[4]; // 4位密码数据 	从左到右 

	WORD crc16; // 数据 和 CRC16检验码(低前高后)
};

// 过程变量结构
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