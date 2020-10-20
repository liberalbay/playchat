#pragma once

#include "Resource.h"           //IDD_DIALOG_USERINFO未声明的标识符  添加#include "Resource.h" 解决
// CUserInfo 对话框

#define WM_DELETEUSER WM_USER+3

class CUserInfo : public CDialogEx
{
	DECLARE_DYNAMIC(CUserInfo)

public:
	CUserInfo(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CUserInfo();

// 对话框数据
	enum { IDD = IDD_DIALOG_USERINFO };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()

public:
	CListCtrl m_DisUserInfo;

public:
	void InitUserTable(void);
	void DisplayUserMes(CString str);
	afx_msg void OnBnClickedBtnDelete();
};
