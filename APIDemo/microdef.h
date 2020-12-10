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

	int RdWidth,RdHeight;				//回显窗口的宽高
	USHORT rdtablehbuf[960];				//每次两个字节
	USHORT rdtablevbuf[540];				//每次两行
	BYTE rdbuf[320*2];				//回显窗口的宽*2
	UINT framenum;

	BYTE *sepbuf;

	char clinum;
	CLIENTINFO m_arrClientInfo[100];
	USHORT FirstData[100];
	
	int  RealBack;

	HANDLE hShareMem;
	LPBYTE pcMap;
	
}DSP_VIDEO_DATA;
