// ClientDlg.h : 头文件
#pragma once

#define WM_RECVDATA WM_USER+1
#define WM_UPDATECLIENTLIST WM_USER+2
#define WM_DELETEUSER WM_USER+3
#define WM_DISCONNECT WM_USER+4

static map<CString,SOCKET> m_transferSock;
static char ip[100] = {0};
static SOCKADDR_IN addrClient;
static CMySQLManager mysql;


// CClientDlg 对话框
class CClientDlg : public CDialogEx
{
// 构造
public:
	CClientDlg(CWnd* pParent = NULL);	// 标准构造函数

// 对话框数据
	enum { IDD = IDD_CLIENT_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支持


// 实现
protected:
	HICON m_hIcon;

	// 生成的消息映射函数
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg LRESULT OnRecvData(WPARAM wParam,LPARAM lParam);
	afx_msg LRESULT OnUpdateClientList(WPARAM wParam,LPARAM lParam);
	afx_msg LRESULT OnDeleteUser(WPARAM wParam,LPARAM lParam);
	afx_msg LRESULT OnDisconnect(WPARAM wParam,LPARAM lParam);
	DECLARE_MESSAGE_MAP()
private:
	SOCKET m_socketSrv;
	BOOL InitSocket(void);
	static DWORD WINAPI AcceptProc(LPVOID lpParameter);
	static DWORD WINAPI RecvProc(LPVOID lpParameter);
	CListBox m_RecvMessage;
	CComboBox m_UserName;
	static struct TRANSFER_MSG{

		char type[32];
		char content[1024];
	};
	ConnectionInfo info;
	MYSQL m_sqlCon;
	CUserInfo m_userinfo;
	
private:
	afx_msg void OnBnClickedBtnSend();
	afx_msg void OnBnClickedButton3();
	afx_msg void OnBnClickedButton1();
	
private:
	static void InitDatabase(const char* user,const char* password,const char* host, 
						unsigned int port,const char* database,
						char* unix_socket,unsigned long clientflag,ConnectionInfo &info,CMySQLManager &mysql);

	static void CreateNewusers(const CString Password, const CString Username,const CString IP);
};

