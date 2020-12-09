
// testDspLibDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "testDspLib.h"
#include "testDspLibDlg.h"
#include "afxdialogex.h"
///添加s200东东
//#include "DspLib.h"
//#include "S200Interface.h"
#include "../include/json/json.h"
#include "../include/json/reader.h"
#include "../include/json/autolink.h"
#include "../include/json/config.h"
#include "../include/json/features.h"
#include "../include/json/forwards.h"
#include "../include/json/value.h"
#include "../include/json/writer.h"
#ifdef _DEBUG
#define new DEBUG_NEW
#endif

#ifdef _DEBUG
#pragma   comment   (lib,   "json_vc71_libmtd.lib")  

#else
#pragma   comment   (lib,   "json_vc71_libmt.lib")  
#endif


// 用于应用程序“关于”菜单项的 CAboutDlg 对话框

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ABOUTBOX };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

// 实现
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialogEx(IDD_ABOUTBOX)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()


// CtestDspLibDlg 对话框



CtestDspLibDlg::CtestDspLibDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_TESTDSPLIB_DIALOG, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CtestDspLibDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CtestDspLibDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_WM_COPYDATA()
END_MESSAGE_MAP()


// CtestDspLibDlg 消息处理程序
typedef struct
{
	int id;
	std::string name;
}user;
std::vector<user> g_users;
BOOL CtestDspLibDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// 将“关于...”菜单项添加到系统菜单中。

	// IDM_ABOUTBOX 必须在系统命令范围内。
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// 设置此对话框的图标。  当应用程序主窗口不是对话框时，框架将自动
	//  执行此操作
	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标

	// TODO: 在此添加额外的初始化代码
	const char* str = "{\"uploadid\": \"UP000000\",\"code\": 100,\"msg\": \"\",\"files\": \"\"}";

	//Json::Reader reader;
	//Json::Value root;
	//if (reader.parse(str, root))  // reader将Json字符串解析到root，root将包含Json里所有子元素  
	//{
	//	std::string upload_id = root["uploadid"].asString();  // 访问节点，upload_id = "UP000000"  
	//	int code = root["code"].asInt();    // 访问节点，code = 100 
	//}

	std::string strRtn = "{\"success\":true,\"user\":[{\"id\":8,\"username\":\"trao\"},{\"id\":9,\"username\":\"ray\"},{\"id\":12,\"username\":\"alex\"}]},\"token\":\"000000 - 000\"}";
	std::string strToken;
	Json::Reader reader;                                    //解析json用Json::Reader
	Json::Value value;                                        //可以代表任意类型
	if (reader.parse(strRtn.c_str(), strRtn.c_str() + strRtn.size(), value))
	{
		strToken = value["token"].asString();

		Json::Value &TempUsers = value["user0"];
		for (int i = 0; i < TempUsers.size(); i++)
		{
			Json::Value &TempUser = TempUsers[i];
			user User;
			User.id = TempUser["id"].asInt();
			User.name = TempUser["username"].asString();
			g_users.push_back(User);
		}
		
		
	
	}
	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

void CtestDspLibDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialogEx::OnSysCommand(nID, lParam);
	}
}

// 如果向对话框添加最小化按钮，则需要下面的代码
//  来绘制该图标。  对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。

void CtestDspLibDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 用于绘制的设备上下文

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// 使图标在工作区矩形中居中
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// 绘制图标
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

//当用户拖动最小化窗口时系统调用此函数取得光标
//显示。
HCURSOR CtestDspLibDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}



BOOL CtestDspLibDlg::OnCopyData(CWnd* pWnd, COPYDATASTRUCT* pCopyDataStruct)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	//CopyDataCheck(m_hWnd, pCopyDataStruct);
	return CDialogEx::OnCopyData(pWnd, pCopyDataStruct);
}
