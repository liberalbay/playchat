#pragma once


// CCreateUser �Ի���

class CCreateUser : public CDialogEx
{
	DECLARE_DYNAMIC(CCreateUser)

public:
	CCreateUser(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CCreateUser();

// �Ի�������
	enum { IDD = IDD_DIALOG_CREATEUSER };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()

public:
	CString m_CreateName;
	CString m_CreatePassword;
	CString m_ConfrimPassword;
};
