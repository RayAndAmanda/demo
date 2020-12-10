#pragma once
#include "ips.h"
#include "afxtempl.h"
typedef  struct  
{
	char strIP[128];
	int iChanIndex;
	UINT netport;
	char devicename[32];
	int bVic;
	void *ownpara;
	void *otherpara;
}NET_VIDEO_DATA;
enum YuntaiPara
{
    YUNTAI_START,
    YUNTAI_STOP,
    YUNTAI_UP,
    YUNTAI_DOWN,
    YUNTAI_LEFT,
    YUNTAI_RIGHT,
    YUNTAI_BIG,
    YUNTAI_SMALL,
    YUNTAI_NULL
};
enum PlayBackState
{
    PLAYBACK_START,
    PLAYBACK_FAST,
    PLAYBACK_SLOW,
    PLAYBACK_STOP,
    PLAYBACK_PAUSE,
    PLAYBACK_RESUME,
	PLAYBACK_SETPOS
};
extern CArray<NET_VIDEO_DATA*,NET_VIDEO_DATA*> m_VideoData;				//some cam may need re-open all opened window
int GetIpsIndex(char* devicename);
void CopyDataCheck(HWND hWnd,COPYDATASTRUCT *tmpstr);
void senddata(NET_VIDEO_DATA *userpara,ULONG *pBufYuv,int width,int height,ULONG *ulLineSize);
void SdSrcInfo2Cli(int MsgID , char *msg);