#pragma once
#include<iostream>
#include <vector>


class CSubDlgInit : public CDialog
{
public:
	CSubDlgInit(CWnd* pParent = NULL);   // standard constructor
	enum
	{
		IDD = IDD_DLG_SUB_INIT
	};

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual BOOL OnInitDialog();
	afx_msg void OnButtonInit();
	afx_msg void OnButtonUninit();

	DECLARE_MESSAGE_MAP()

public:
	BOOL isInitialize;
	afx_msg BOOL OnCopyData(CWnd* pWnd, COPYDATASTRUCT* pCopyDataStruct);
	
};
