#pragma once
#pragma pack(push) //�������״̬ 
#pragma pack(1)//�趨Ϊ1�ֽڶ�
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
#pragma pack(pop)//�ָ�����״̬