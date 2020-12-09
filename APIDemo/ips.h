#pragma once
#pragma pack(push) //保存对齐状态 
#pragma pack(1)//设定为1字节对
typedef struct
{
	char devicename[32];
	char displayn[16];
	char manufn[16];
	char username[32];
	char password[32];
	char dns[128];
	int tm;
	char ipadd[128];
	int port;
	char rtspcmd[128];
	int fmt;
}IPvs;
extern CArray<IPvs,IPvs&> arrIPvs;
#pragma pack(pop)//恢复对齐状态