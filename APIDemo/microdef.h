#pragma once
#include "execmd.h"
#include "DspLib.h"

typedef  struct  
{
	ULONG   ulIP;
	RECT rcWinReal;
	int left;
	int top;
	int cwidth;
	int cheight;
}CLIENTINFO;

typedef  struct  
{
	unsigned int WinId;
	int width;
	int height;
	RECT rcWinReal;
	RECT setrcOffset;
	SOCKET sndskt;
	sockaddr_in addr;

	int RdWidth,RdHeight;				//���Դ��ڵĿ��
	USHORT rdtablehbuf[960];				//ÿ�������ֽ�
	USHORT rdtablevbuf[540];				//ÿ������
	BYTE rdbuf[320*2];				//���Դ��ڵĿ�*2
	UINT framenum;

	BYTE *sepbuf;

	char clinum;
	CLIENTINFO m_arrClientInfo[100];
	USHORT FirstData[100];
	
	int  RealBack;

	HANDLE hShareMem;
	LPBYTE pcMap;
	
}DSP_VIDEO_DATA;
