#pragma once

#define WM_CREATEUSER WM_USER+4

// CLogin �Ի���

class CLogin : public CDialogEx
{
	DECLARE_DYNAMIC(CLogin)

public:
	CLogin(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CLogin();

// �Ի�������
	enum { IDD = IDD_LOGIN_DIALOG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()

public:
	CString m_LoginUser;
	CString m_LoginPassword;
	CString m_NewUserName;
	CString m_NewUserPassword;
	afx_msg void OnBnClickedBtnCreateuser();
};
