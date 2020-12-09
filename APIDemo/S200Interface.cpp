#include "stdafx.h"
#include<stdio.h>
#include "DspLib.h"
#include <string>
#include <vector>

#include "S200Interface.h"

using namespace std; 

#ifdef _DEBUG
#pragma   comment   (lib,   "DspLibd.lib")  

#else
#pragma   comment   (lib,   "DspLib.lib")  
#endif

#define  S200INTERFACE
#define SAFE_DELETE(obj)		if(obj){delete obj; obj = NULL;}
#define SAFE_M_DELETE(obj)		if(obj){delete []obj; obj = NULL;}


 typedef struct
{
	unsigned long seq;
}OWN_PARA;

  

S200INTERFACE bool DisConnect()
{
	return true;
}
S200INTERFACE void PlayBackSig(NET_VIDEO_DATA *netvid, int act, SYSTEMTIME &sts, SYSTEMTIME &ste)
{
//	GetIpsIndex("TEST");
}
S200INTERFACE void ChangeYuntai(NET_VIDEO_DATA *netvid, int yuntaipara, int StartStop)
{
	//SdSrcInfo2Cli(3, _T("乌鲁木齐,-1"));
}
S200INTERFACE char GetSigVic(void)
{
	return 0;
}
void GetSrcS200()
{
}

S200INTERFACE void PlatformLogin(IPvs &ipvs)
{
	printf("enter and leave PlatfornLogin\n");

	
}


S200INTERFACE void GetSrcInfo(IPvs &ipvs)
{
	printf("enter GerSrcInfo\n");
	///
	{//公司测试
		//string msg1 = "0";
		//char msg[1024];
		//char msg2[1024];
		//msg1 += ",0";
		//SdSrcInfo2Cli(3, (char*)msg1.c_str()); // 发3 表示发送根节点
		//sprintf(msg, "%s,%d,%s,%s", "0", \
		//	1, "分组1", "abc");
		//SdSrcInfo2Cli(1, msg);					//1 发送分组信息
		//sprintf(msg2, "%s,%d,%s,%s,%d","abc",1,"Camera_01", "90ad77d8057c43dab140b77361606927", 1);
		//SdSrcInfo2Cli(0, msg2);               //0 发送设备信息
		//SdSrcInfo2Cli(2, "0");             ///发2 表示发送结束 
	}
	//printf("leave GerSrcInfo\n");
	//
	while (!g_GerScrOK)
	{
		Sleep(1000);
	}
	std::vector<TreeNode>::iterator TreeIt = g_TreeNodeList.begin();
	for (; TreeIt != g_TreeNodeList.end(); ++TreeIt)
	{//发送根节点信息
		string msg1 = TreeIt->treeCode;
		
		msg1 += ",0";
		//CString str;
		//str.Format(_T("%s"), msg1.c_str());
		SdSrcInfo2Cli(3, (char*)msg1.c_str()); // 发3 表示发送根节点

		
	}
	std::vector<DeviceGroup>::iterator Groupit = g_DeviceGroupList.begin();
	for (; Groupit != g_DeviceGroupList.end(); ++Groupit)
	{
		//发送分组信息
		char msg[1024];
	//	//{
	//	"indexCode": "b93e8685049b47ddbfe8b512c4c95608",
	//		"treeCode" : "0",
	//		"name" : "杭州市",
	//		"parentIndexCode" : "-1"
	//},
	//{
	//	"indexCode": "330108",
	//	"treeCode" : "0",
	//	"name" : "330108",
	//	"parentIndexCode" : "b93e8685049b47ddbfe8b512c4c95608"
	//},
	//{
	//	"indexCode": "190604011451064790",
	//	"treeCode" : "0",
	//	"name" : "车载设备",
	//	"parentIndexCode" : "330108"
	//},
	//{
	//	"indexCode": "efeef8d45acc4149addf13a5cf8368e9",
	//	"treeCode" : "0",
	//	"name" : "本地设备",
	//	"parentIndexCode" : "330108"
	//}
		//这里发送分组份两种情况
		//1,parentIndexCode == -1 ,说明父节点为根节点
		//2,parentIndexCode != -1 ,则发送真正的父节点
		if (Groupit->parentIndexCode == "-1")
		{
			sprintf(msg, "%s,%d,%s,%s", Groupit->treeCode.c_str(),\
				1, Groupit->name.c_str(), Groupit->indexCode.c_str());
		}
		else
		{
			sprintf(msg, "%s,%d,%s,%s", Groupit->parentIndexCode.c_str(), \
				1, Groupit->name.c_str(), Groupit->indexCode.c_str());
		}
		
		
		SdSrcInfo2Cli(1, msg);					//1 发送分组信息
	}

	std::vector<CameraInfo>::iterator Camerait = g_CameraInfoList.begin();
	for (; Camerait != g_CameraInfoList.end(); ++Camerait)
	{//发送摄像机信息
		char msg[1024];
		sprintf(msg, "%s,%d,%s,%s,%d", Camerait->unitIndexCode.c_str(), 1, \
			Camerait->name.c_str(), Camerait->cameraIndexCode.c_str(), Camerait->status);
		printf("SdSrcInfo2Cli0=%s\n", msg);
		SdSrcInfo2Cli(0, msg);               //0 发送设备信息
	}
	
	SdSrcInfo2Cli(2, "0");             ///发2 表示发送结束
	printf("leave  GerSrcInfo\n");
}
//YUV数据回调回调函数定义
void CALLBACK DecodedDataCallback1(VIDEO_INT64 i64PlayHandle, const char* pDataArray,
	int iDataLen, int iWidth, int iHeight, int iFrameType, int iTimeStamp, void* pUserData)
{
	//// YUV数据回调
// i64PlayHandle：预览或回放接口返回的句柄
// pDataArray：YUV数据数组（需当成二进制数据来解析）
// iDataLen：流数据长度
// iWidth：图像宽度
// iHeight：图像高度
// iFrameType：图像YUV类型（目前为YV12，值为3）
// iTimeStamp：时间戳
// pUserData：用户数据
	printf("enter DecodedDataCallback1\n");
	NET_VIDEO_DATA *sinChan = (NET_VIDEO_DATA *)(pUserData);
	ULONG pYuvBuf[4];
	
	pYuvBuf[0] = (ULONG)pDataArray;
	pYuvBuf[1] = (ULONG)(pDataArray+ iWidth*iHeight);
	pYuvBuf[2] = (ULONG)(pDataArray + (iWidth/2)*(iHeight/2)+ iWidth*iHeight);
	senddata(sinChan, pYuvBuf, iWidth, iHeight, NULL);
}
void CALLBACK MsgCallBack1(VIDEO_INT64 i64PlayHandle, int iMsg, void* pUserData)
{

}
//实时播放回调函数定义
void CALLBACK StreamCallBack1(VIDEO_INT64 i64PlayHandle, int iStreamDataType,
	const char* pDataArray, int iDataLen, void* pUserData)
{
}
BOOL StartRealPlayS200(const char* path, void* netid)
{
	printf(" StartRealPlayS200+++++++++++++\r\n");
	std::string cameraIndex = path;
	printf("cameraIndex is:%s\n", cameraIndex.c_str());
	std::string Url;
	
	/*std::vector<CameraInfo>::iterator it = g_CameraInfoList.begin();
	for (; it != g_CameraInfoList.end(); ++it)
	{
		if (it->cameraIndexCode == cameraIndex)
		{
			Url = it->url;
			break;
		}
	}*/
	Url = FindPreviewUrlByCameraindex(cameraIndex);
	printf("this camera url is:%s\n", Url.c_str());
	USERDATA stUserData;
	stUserData.ctype = RealPlay;
	strcpy_s(stUserData.msg, AnsiToUtf8("实时").c_str());
	VIDEO_PLAY_REQ stReq;
	stReq.fnStream = StreamCallBack1;
	stReq.fnDecodedStream = DecodedDataCallback1;
	stReq.fnMsg = MsgCallBack1;
	//stReq.pUserData = (void*)&stUserData;
	stReq.pUserData = (void*)netid;
	stReq.iHardWareDecode = 0;
	int PlayHandle = Video_StartPreview(Url.c_str(),
			NULL,
			&stReq);
	if (PlayHandle>0)
	{  ///开窗成功后需要保存这个窗口句柄
		NET_VIDEO_DATA *pnet = (NET_VIDEO_DATA *)(netid);

		if (pnet)
		{
			OWN_PARA* ownp = (OWN_PARA*)pnet->ownpara;
			if (ownp)
			{
				ownp = (OWN_PARA*)malloc(sizeof(OWN_PARA));

				ownp->seq = PlayHandle;
				pnet->ownpara = ownp;

			}
		}
	}

	return TRUE;
}

S200INTERFACE int LogAndOpen(NET_VIDEO_DATA *netvid)
{
	printf("enter LogAndOpen+++++++++++++\r\n");
	if (netvid)
	{
		std::string cameraIndexCode = netvid->strIP;
		OWN_PARA *op = NULL;
		op = (OWN_PARA*)malloc(sizeof(OWN_PARA));
		memset(op, 0, sizeof(OWN_PARA));
		netvid->ownpara = op;
		StartRealPlayS200(netvid->strIP, netvid);


	}
	
	printf("leave LogAndOpen+++++++++++++\r\n");
 	return 1;
}
S200INTERFACE void CloseSig(NET_VIDEO_DATA *netvid)
{
	printf("IVS_Demo_PlatformCloseSig++++++++++++\r\n");


	OWN_PARA *op = (OWN_PARA *)(netvid->ownpara);
	if (op->seq)
	{
		int iRet = -1;

		iRet = Video_StopPreview(op->seq);
		
		if (iRet >= 0)
		{
			printf("IVS_Demo_Platform CloseSig success!-----------\r\n");
		}
	}
	if (op)
	{//删除关闭窗口的句柄
		SAFE_DELETE(op)
	}


}
