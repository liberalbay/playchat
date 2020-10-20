
// RelClientDlg.h : ͷ�ļ�
//
#include "Login.h"
#pragma once

#define WM_RECVDATA WM_USER+1
#define WM_UPDATECLIENTLIST WM_USER+2
#define WM_LOCALUSERNAME WM_USER+3
#define WM_CREATEUSER WM_USER+4
#define WM_LOGIN WM_USER+5
#define WM_DISCONNECT WM_USER+6

// CRelClientDlg �Ի���
class CRelClientDlg : public CDialogEx
{
// ����
public:
	CRelClientDlg(CWnd* pParent = NULL);	// ��׼���캯��

// �Ի�������
	enum { IDD = IDD_RELCLIENT_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV ֧��


// ʵ��
protected:
	HICON m_hIcon;

	// ���ɵ���Ϣӳ�亯��
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg LRESULT OnRecvData(WPARAM wParam,LPARAM lParam);
	afx_msg LRESULT OnUpdateClientList(WPARAM wParam,LPARAM lParam);
	afx_msg LRESULT OnLocalUserName(WPARAM wParam,LPARAM lParam);
	afx_msg LRESULT OnCreateUser(WPARAM wParam,LPARAM lParam);
	afx_msg LRESULT OnLogin(WPARAM wParam,LPARAM lParam);
	afx_msg LRESULT OnDisconnect(WPARAM wParam,LPARAM lParam);
	DECLARE_MESSAGE_MAP()
private:
	SOCKET m_sockClient;
	BOOL InitSocket(void);
	static DWORD WINAPI RecvProc(LPVOID lpParameter);
	CListBox m_RecvMessage;
	CListBox m_LocalUserName;
	CComboBox m_UserName;
	WINDOWPLACEMENT hwp;
	CLogin m_Logindlg;
	CString m_LoginUsername;
	CString m_LoginPassword;
	static CString m_LoginConfirm;
	static struct TRANSFER_MSG{

		char type[32];
		char content[1024];
	};
	
	
private:
	afx_msg void OnBnClickedBtnSend();
	void HideWindow(void);
	void ShowWindow(void);
	void ShowLoginWindow(void);
};
