#pragma once


// CCreateUser 对话框

class CCreateUser : public CDialogEx
{
	DECLARE_DYNAMIC(CCreateUser)

public:
	CCreateUser(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CCreateUser();

// 对话框数据
	enum { IDD = IDD_DIALOG_CREATEUSER };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()

public:
	CString m_CreateName;
	CString m_CreatePassword;
	CString m_ConfrimPassword;
};
