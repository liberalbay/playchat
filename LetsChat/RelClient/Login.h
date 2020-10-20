#pragma once

#define WM_CREATEUSER WM_USER+4

// CLogin 对话框

class CLogin : public CDialogEx
{
	DECLARE_DYNAMIC(CLogin)

public:
	CLogin(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CLogin();

// 对话框数据
	enum { IDD = IDD_LOGIN_DIALOG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()

public:
	CString m_LoginUser;
	CString m_LoginPassword;
	CString m_NewUserName;
	CString m_NewUserPassword;
	afx_msg void OnBnClickedBtnCreateuser();
};
