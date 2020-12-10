// SubDlgInit.cpp : implementation file
//

#include "stdafx.h"
#include "APIDemo.h"
#include "SubDlgInit.h"
#include "HttpUtillib.h"
#include "HttpHeader.h"
#include "iostream"
#include "sstream"
#include<fstream>
#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif
///添加s200东东
#include "DspLib.h"
#include "S200Interface.h"
/////////////////////////////////////////////////////////////////////////////
// CSubDlgInit dialog
#include<json\json.h>

#ifdef _DEBUG
#pragma   comment   (lib,   "json_vc71_libmtd.lib")  

#else
#pragma   comment   (lib,   "json_vc71_libmt.lib")  
#endif


bool g_GerScrOK=false;
std::vector <TreeNode> g_TreeNodeList;
std::vector<CameraInfo> g_CameraInfoList;
std::vector<DeviceGroup> g_DeviceGroupList;
Unit_query_t myQuery;  //移动外面变成全局变量
//(POST)获取所有树编码treeCode  
string getAllTreeCode(Unit_query_t query)
{
	std::stringstream  ss;
	ss << "https://" << query.artemisIp << ":" << query.artemisPort
		<< "/artemis/api/resource/v1/unit/getAllTreeCode";
	map<string, string> headers;
	//（必填的Http头）根据期望的Response内容类型设置
	headers.insert(std::make_pair(HttpHeader::HTTP_HEADER_ACCEPT, "application/json"));
	headers.insert(std::make_pair(HttpHeader::HTTP_HEADER_CONTENT_TYPE, "application/json;charset=UTF-8"));
	list<string> signHeaderPrefixList;
	char strBody[1024] = { 0 };
	sprintf_s(strBody, 1024, "{}");
	return HttpPost(ss.str(), headers, strBody, query.appKey, query.appSecret, 5, signHeaderPrefixList);;
}

///


/*（Post）根据分页参数获取所有监控点资源*/
string getCamerasByTreeCode(Unit_query_t query,std::string &treecode,int PageNo,int PageSize)
{
	std::stringstream  ss;
	ss << "https://" << query.artemisIp << ":" << query.artemisPort
		<< "/artemis/api/resource/v1/cameras";
	map<string, string> headers;
	//（必填的Http头）根据期望的Response内容类型设置
	headers.insert(std::make_pair(HttpHeader::HTTP_HEADER_ACCEPT, "application/json"));
	headers.insert(std::make_pair(HttpHeader::HTTP_HEADER_CONTENT_TYPE, "application/json;charset=UTF-8"));
	list<string> signHeaderPrefixList;
	char strBody[1024] = { 0 };
	sprintf_s(strBody, 1024, "{\"pageNo\":%d,\"pageSize\":%d,\"treeCode\":\"%s\"}", PageNo, PageSize,treecode.c_str());
	return HttpPost(ss.str(), headers, strBody, query.appKey, query.appSecret, 5, signHeaderPrefixList);

}
/********************************************/

/*根据分页参数获取所有分组信息*/
string getDeviceGroupListByTreeCode(Unit_query_t query, std::string &treecode, int PageNo, int PageSize)
{
	std::stringstream  ss;
	ss << "https://" << query.artemisIp << ":" << query.artemisPort
		<< "/artemis/api/resource/v1/regions";
	map<string, string> headers;
	//（必填的Http头）根据期望的Response内容类型设置
	headers.insert(std::make_pair(HttpHeader::HTTP_HEADER_ACCEPT, "application/json"));
	headers.insert(std::make_pair(HttpHeader::HTTP_HEADER_CONTENT_TYPE, "application/json;charset=UTF-8"));
	list<string> signHeaderPrefixList;
	char strBody[1024] = { 0 };
	sprintf_s(strBody, 1024, "{\"pageNo\":%d,\"pageSize\":%d,\"treeCode\":\"%s\"}", PageNo, PageSize, treecode.c_str());
	return HttpPost(ss.str(), headers, strBody, query.appKey, query.appSecret, 5, signHeaderPrefixList);
}
/********************************************/


//add 
string resDecodeUrlJson(std::string &strUrls)
{
	/*{
	"code": "0",
	"msg" : "success",
	"data" : {
	"url": "rtsp://ip:port/BSwvVkAUrG6XAMhIEeIMYb66A84s"
	}
	}*/
	string res_url;
	Json::Reader reader;                                    //解析json用Json::Reader
	Json::Value value;                                        //可以代表任意类型
	if (reader.parse(strUrls, value))
	{
		Json::Value &dataObject = value["data"];
		res_url = dataObject["url"].asString();
		//g_pMainDlg->ShowMsg(CString(camerainfo.name.c_str()));
		//g_pMainDlg->ShowMsg(CString(camerainfo.url.c_str()));
	}

	return res_url;
}

//end
/*根据摄像头id获取摄像头url*/

std::string FindPreviewUrlByCameraindex(std::string cameraIndexCode)
{
	std::stringstream  ss;
	ss << "https://" << myQuery.artemisIp << ":" << myQuery.artemisPort
		<< "/artemis/api/video/v1/cameras/previewURLs";
	map<string, string> headers;
	//（必填的Http头）根据期望的Response内容类型设置
	headers.insert(std::make_pair(HttpHeader::HTTP_HEADER_ACCEPT, "application/json"));
	headers.insert(std::make_pair(HttpHeader::HTTP_HEADER_CONTENT_TYPE, "application/json;charset=UTF-8"));
	list<string> signHeaderPrefixList;
	char strBody[1024] = { 0 };
	sprintf_s(strBody, 1024, "{\"cameraIndexCode\":\"%s\"}", cameraIndexCode.c_str());
	string res_pos= HttpPost(ss.str(), headers, strBody, myQuery.appKey, myQuery.appSecret, 5, signHeaderPrefixList);
	return resDecodeUrlJson(res_pos);

}











/*Post）根据监控点编号获取预览取流url*/
string FindPreviewUrlByIndexcode(Unit_query_t query,CameraInfo &camerainfo)
{
	std::stringstream  ss;
	ss << "https://" << query.artemisIp << ":" << query.artemisPort
		<< "/artemis/api/video/v1/cameras/previewURLs";
	map<string, string> headers;
	//（必填的Http头）根据期望的Response内容类型设置
	headers.insert(std::make_pair(HttpHeader::HTTP_HEADER_ACCEPT, "application/json"));
	headers.insert(std::make_pair(HttpHeader::HTTP_HEADER_CONTENT_TYPE, "application/json;charset=UTF-8"));
	list<string> signHeaderPrefixList;
	char strBody[1024] = { 0 };
	sprintf_s(strBody, 1024, "{\"cameraIndexCode\":\"%s\"}", camerainfo.cameraIndexCode.c_str());
	return HttpPost(ss.str(), headers, strBody, query.appKey, query.appSecret, 5, signHeaderPrefixList);
}
/********************************************/
CSubDlgInit::CSubDlgInit(CWnd* pParent /*=NULL*/)
: CDialog(CSubDlgInit::IDD, pParent)
, isInitialize(FALSE)
{
	//{{AFX_DATA_INIT(CSubDlgInit)
	//}}AFX_DATA_INIT
}


void CSubDlgInit::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CSubDlgInit)
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CSubDlgInit, CDialog)
	//{{AFX_MSG_MAP(CSubDlgInit)
	ON_BN_CLICKED(IDC_BUTTON_INIT, OnButtonInit)
	ON_BN_CLICKED(IDC_BUTTON_UNINIT, OnButtonUninit)
	//}}AFX_MSG_MAP
	ON_WM_COPYDATA()
	
	ON_BN_CLICKED(IDC_BUTTON1, &CSubDlgInit::OnBnClickedButton1)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CSubDlgInit message handlers
void DecodeTreeJson(std::string &strTrees)
{
	///获取所有树编码
	//{返回码
	//	"code": "0",
	//		"msg" : "success",
	//		"data" : {
	//		"total": 2,
	//			"list" : [
	//		{
	//			"treeCode": "0",
	//				"treeName" : "国标目录",
	//				"capabilitySet" : "{\"CAMERA\":\"@motiontrack@remote_emer@\"}",
	//				"intelligentSet" : " {\"CAMERA\":\"@alarm@\"}"
	//		}
	//			]
	//	}
	//}
	Json::Reader reader;                                    //解析json用Json::Reader
	Json::Value value;                                        //可以代表任意类型
	if (reader.parse(strTrees, value))
	{
		Json::Value &dataObject = value["data"];
		int total = dataObject["total"].asInt();
		Json::Value &lists = dataObject["list"];
		for (int i = 0; i < lists.size(); i++)
		{
			CString temp;
			Json::Value &list = lists[i];
			TreeNode treenode;
			treenode.treeCode = list["treeCode"].asString();
			
			temp = list["treeName"].asCString();
			treenode.treeName=Utf8toAnsi(temp.GetBuffer());
			g_TreeNodeList.push_back(treenode);
		}
	}
}
void DecodeCameraJson(std::string &strCameras,std::string &strTreeCode,int &totalCamera )
{
	/*{
		"code": "0",
			"msg" : "success",
			"data" : {
			"total": 1,
				"pageSize" : 20,
				"pageNo" : 1,
				"list" : [
			{
				"cameraIndexCode": "90ad77d8057c43dab140b77361606927",
					"gbIndexCode" : "12000000051210000000",
					"name" : "Camera_01",
			}]
		}
	}*/
	//test
	//totalCamera = 21;
	//
	Json::Reader reader;                                    //解析json用Json::Reader
	Json::Value value; //可以代表任意类型

	if (reader.parse(strCameras, value))
	{
		Json::Value &dataObject = value["data"];
		totalCamera = dataObject["total"].asInt();
		Json::Value &lists = dataObject["list"];
		for (int i = 0; i < lists.size(); i++)
		{
			CString temp ;

			Json::Value &list = lists[i];
			CameraInfo camerainfo;
			camerainfo.cameraIndexCode = list["cameraIndexCode"].asString();
			temp = list["cameraTypeName"].asCString();
			camerainfo.cameraTypeName= Utf8toAnsi(temp.GetBuffer());
			temp = list["name"].asCString();
			camerainfo.name = Utf8toAnsi(temp.GetBuffer());
			camerainfo.cameraType= list["cameraType"].asInt();
			camerainfo.treeCode = strTreeCode;
			camerainfo.status= list["status"].asInt();
			camerainfo.unitIndexCode = list["unitIndexCode"].asString();
			camerainfo.deviceIndexCode = list["deviceIndexCode"].asString();
			
			g_CameraInfoList.push_back(camerainfo);
		}
	}
}



void DecodeUrlJson(std::string &strUrls,CameraInfo &camerainfo)
{
	/*{
		"code": "0",
			"msg" : "success",
			"data" : {
			"url": "rtsp://ip:port/BSwvVkAUrG6XAMhIEeIMYb66A84s"
		}
	}*/
	Json::Reader reader;                                    //解析json用Json::Reader
	Json::Value value;                                        //可以代表任意类型
	if (reader.parse(strUrls, value))
	{
		Json::Value &dataObject = value["data"];
		camerainfo.url = dataObject["url"].asString();
		g_pMainDlg->ShowMsg(CString(camerainfo.name.c_str()));
		g_pMainDlg->ShowMsg(CString(camerainfo.url.c_str()));
	}
}
void DecodeDeviceGroupJson(std::string &strGroups,int& totalGroup)
{
	Json::Reader reader;                                    //解析json用Json::Reader
	Json::Value value; //可以代表任意类型

	if (reader.parse(strGroups, value))
	{
		Json::Value &dataObject = value["data"];
		totalGroup = dataObject["total"].asInt();
		Json::Value &lists = dataObject["list"];
		for (int i = 0; i < lists.size(); i++)
		{
			CString temp;
			Json::Value &list = lists[i];
			DeviceGroup devicegroup;
			devicegroup.indexCode = list["indexCode"].asString();
			temp = list["name"].asCString();
			devicegroup.name = Utf8toAnsi(temp.GetBuffer());
			
			devicegroup.treeCode = list["treeCode"].asString();
			devicegroup.parentIndexCode = list["parentIndexCode"].asString();
			g_DeviceGroupList.push_back(devicegroup);
			
		}
	}
}
UINT GetHttpInfoThread(LPVOID lparam)
{
	/*CString UserInfo_path(".\\UserInfo.ini");

	
	
	char key[256] = { 0 };
	char secret[256] = { 0 };
	char artIP[256] = { 0 };
	char artPort[256] = { 0 };
	GetPrivateProfileString("UserInfo", "appKey", "", key, 256, UserInfo_path);
	GetPrivateProfileString("UserInfo", "appSecret", "", secret, 256, UserInfo_path);
	GetPrivateProfileString("UserInfo", "artemisIp", "", artIP, 256, UserInfo_path);
	myQuery.artemisPort = GetPrivateProfileInt("UserInfo", "artemisPort", 0, UserInfo_path);
	myQuery.appKey = key;
	myQuery.appSecret = secret;
	myQuery.artemisIp = artIP;*/
	//20201210 这里有问题如果不启动线程就不能初始化
	///获取所有树编码

	std::string strTrees = getAllTreeCode(myQuery);
	//test data
	//std::string strTrees="{\"code\":true,\"data\":{\"total\":2,\"list\":[{\"treeCode\":\"8\",\"treeName\":\"trao\"},{\"treeCode\":\"9\",\"treeName\":\"ray\"},{\"treeCode\":\"12\",\"treeName\":\"alex\"}]}},\"token\":\"000000 - 000\"}";
	DecodeTreeJson(strTrees);
	
	
	for (std::vector<TreeNode>::iterator it = g_TreeNodeList.begin();
		it != g_TreeNodeList.end();
		++it)
	{
		//根据树编码和分页参数获取所有监控点资源编号
		int CameraPageNo = 1;
		const int PageSize = 1000;
		int totalCamera = 0;
		int totalDeviceGroup = 0;
		int DeviceGroupPageNo = 1;
	L1:
		totalCamera = 0;
		std::string strCameras = getCamerasByTreeCode(myQuery, it->treeCode, CameraPageNo, PageSize);
		DecodeCameraJson(strCameras, it->treeCode, totalCamera);
		if (CameraPageNo*PageSize < totalCamera)
		{
			CameraPageNo++;
			goto L1;
		}

	L2:
		totalDeviceGroup = 0;
		std::string strDeviceGroup = getDeviceGroupListByTreeCode(myQuery, it->treeCode, DeviceGroupPageNo, PageSize);
		DecodeDeviceGroupJson(strDeviceGroup, totalDeviceGroup);
		if (DeviceGroupPageNo*PageSize < totalDeviceGroup)
		{
			DeviceGroupPageNo++;
			goto L2;
		}
	}
	CString camera_size;
	camera_size.Format("all camera size:%d", g_CameraInfoList.size());
	
	g_pMainDlg->ShowMsg(camera_size);
	//这里不用获取全部url,节省时间
	//for (std::vector<CameraInfo>::iterator it = g_CameraInfoList.begin();
	//	it != g_CameraInfoList.end();
	//	++it)
	//{//根据监控点编号获取视频预览url
	//	std::string strUrl = FindPreviewUrlByIndexcode(myQuery, *it);
	//	DecodeUrlJson(strUrl, *it);
	//}

	//test send
	/*
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
	
		if (Groupit->parentIndexCode == "-1")
		{
			sprintf(msg, "%s,%d,%s,%s", Groupit->treeCode.c_str(), \
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
*/
	g_GerScrOK = true;
	printf("g_GerScrOK\n");

	return 0;
}
BOOL CSubDlgInit::OnInitDialog()
{
	CDialog::OnInitDialog();
	int iRet = VIDEO_ERR_FAIL;

	iRet = Video_Init(NULL);
	if (VIDEO_ERR_SUCCESS == iRet)
	{
		isInitialize = TRUE;
	}
	else
	{
		isInitialize = FALSE;
	}

	g_pMainDlg->PrintCallMsg("Video_Init()", iRet);
	CString UserInfo_path(".\\UserInfo.ini");
	char key[256] = { 0 };
	char secret[256] = { 0 };
	char artIP[256] = { 0 };
	char artPort[256] = { 0 };
	GetPrivateProfileString("UserInfo", "appKey", "", key, 256, UserInfo_path);
	GetPrivateProfileString("UserInfo", "appSecret", "", secret, 256, UserInfo_path);
	GetPrivateProfileString("UserInfo", "artemisIp", "", artIP, 256, UserInfo_path);
	myQuery.artemisPort = GetPrivateProfileInt("UserInfo", "artemisPort", 0, UserInfo_path);
	myQuery.appKey = key;
	myQuery.appSecret = secret;
	myQuery.artemisIp = artIP;
	//判断d:\exe2014\controlsvr.exe是否存在。
	std::fstream _file;
	_file.open("d:\\exe2014\\controlsvr.exe", ios::in);
	if (_file)
	{
		//g_pMainDlg->ShowMsg("开始预览...");
		AfxBeginThread(GetHttpInfoThread, this);
		_file.close();
	}
	else
	{
		g_GerScrOK = true;
	}

	
	

	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}

void CSubDlgInit::OnButtonInit()
{
	
}

void CSubDlgInit::OnButtonUninit()
{
	int iRet = VIDEO_ERR_FAIL;

	iRet = Video_Fini();
	if (VIDEO_ERR_SUCCESS == iRet)
	{
		isInitialize = FALSE;
	}

	g_pMainDlg->PrintCallMsg("Video_Fini()", iRet);
}

BOOL CSubDlgInit::OnCopyData(CWnd* pWnd, COPYDATASTRUCT* pCopyDataStruct)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	CopyDataCheck(m_hWnd, pCopyDataStruct);
	return CDialog::OnCopyData(pWnd, pCopyDataStruct);
}




void CSubDlgInit::OnBnClickedButton1()
{

	// TODO: 在此添加控件通知处理程序代码
	CString csUrl;
	GetDlgItemText(IDC_EDIT1, csUrl);
	string Url = FindPreviewUrlByCameraindex(csUrl.GetString());
	printf("this camera url is:%s\n", Url.c_str());
	csUrl = "this camera url is:";
	csUrl += Url.c_str();
	g_pMainDlg->ShowMsg(csUrl);

}
