#pragma once

#include "Resource.h"           //IDD_DIALOG_USERINFOδ�����ı�ʶ��  ���#include "Resource.h" ���
// CUserInfo �Ի���

#define WM_DELETEUSER WM_USER+3

class CUserInfo : public CDialogEx
{
	DECLARE_DYNAMIC(CUserInfo)

public:
	CUserInfo(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CUserInfo();

// �Ի�������
	enum { IDD = IDD_DIALOG_USERINFO };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()

public:
	CListCtrl m_DisUserInfo;

public:
	void InitUserTable(void);
	void DisplayUserMes(CString str);
	afx_msg void OnBnClickedBtnDelete();
};
