
// RelClientDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "RelClient.h"
#include "RelClientDlg.h"
#include "afxdialogex.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// ����Ӧ�ó��򡰹��ڡ��˵���� CAboutDlg �Ի���
CString CRelClientDlg::m_LoginConfirm = L"";

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// �Ի�������
	enum { IDD = IDD_ABOUTBOX };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

// ʵ��
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialogEx(CAboutDlg::IDD)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()


// CRelClientDlg �Ի���




CRelClientDlg::CRelClientDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CRelClientDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CRelClientDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX,IDC_LIST_RECV,m_RecvMessage);
	DDX_Control(pDX,IDC_COMBO_USER,m_UserName);
	DDX_Control(pDX,IDC_LIST_USERNAME,m_LocalUserName);
}

BEGIN_MESSAGE_MAP(CRelClientDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_MESSAGE(WM_RECVDATA,OnRecvData)
	ON_MESSAGE(WM_UPDATECLIENTLIST,OnUpdateClientList)
	ON_MESSAGE(WM_LOCALUSERNAME,OnLocalUserName)
	ON_MESSAGE(WM_CREATEUSER,OnCreateUser)
	ON_MESSAGE(WM_LOGIN,OnLogin)	
	ON_MESSAGE(WM_DISCONNECT,OnDisconnect)
	ON_BN_CLICKED(IDC_BTN_SEND, &CRelClientDlg::OnBnClickedBtnSend)
END_MESSAGE_MAP()


// CRelClientDlg ��Ϣ�������

BOOL CRelClientDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// ��������...���˵�����ӵ�ϵͳ�˵��С�

	// IDM_ABOUTBOX ������ϵͳ���Χ�ڡ�
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

	// ���ô˶Ի����ͼ�ꡣ��Ӧ�ó��������ڲ��ǶԻ���ʱ����ܽ��Զ�
	//  ִ�д˲���
	SetIcon(m_hIcon, TRUE);			// ���ô�ͼ��
	SetIcon(m_hIcon, FALSE);		// ����Сͼ��

	// TODO: �ڴ���Ӷ���ĳ�ʼ������
	//��ʼ��SOCKET
	InitSocket();
	//���������߳�
	HANDLE hThread = CreateThread(NULL,0,RecvProc,(LPVOID)m_sockClient,0,NULL);
	//����������  
	HideWindow();
	//��ʾ��¼����
	ShowLoginWindow();  
	return TRUE;  // ���ǽ��������õ��ؼ������򷵻� TRUE
}

void CRelClientDlg::OnSysCommand(UINT nID, LPARAM lParam)
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

// �����Ի��������С����ť������Ҫ����Ĵ���
//  �����Ƹ�ͼ�ꡣ����ʹ���ĵ�/��ͼģ�͵� MFC Ӧ�ó���
//  �⽫�ɿ���Զ���ɡ�

void CRelClientDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // ���ڻ��Ƶ��豸������

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// ʹͼ���ڹ����������о���
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// ����ͼ��
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

//���û��϶���С������ʱϵͳ���ô˺���ȡ�ù��
//��ʾ��
HCURSOR CRelClientDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

BOOL CRelClientDlg::InitSocket(){

	m_sockClient = socket(AF_INET,SOCK_STREAM,0);
	if (INVALID_SOCKET == m_sockClient){

		MessageBox(L"�׽��ִ���ʧ�ܣ�");
		return FALSE;
	}

	SOCKADDR_IN addrSrv;
	inet_pton(AF_INET,"127.0.0.1",&addrSrv.sin_addr);
	addrSrv.sin_family = AF_INET;
	addrSrv.sin_port = htons(8080);
	//�������������������
	if(SOCKET_ERROR == connect(m_sockClient,(SOCKADDR*)&addrSrv,sizeof(SOCKADDR))){

		MessageBox(L"�����������ʧ�ܣ�");
		return FALSE;
	}
	
	return TRUE;
}

DWORD WINAPI CRelClientDlg::RecvProc(LPVOID lpParameter){

	while(TRUE){

		TRANSFER_MSG recvmsg;
		char recvbuf[1024] = {0};
		int retval;
		retval = recv((SOCKET)lpParameter,recvbuf,sizeof(recvbuf),0);
		memcpy(&recvmsg,recvbuf,sizeof(recvmsg));

		if (SOCKET_ERROR == retval){

			::MessageBox(AfxGetApp()->m_pMainWnd->m_hWnd,L"�������ѹرգ�",L"����",MB_OK);
			exit(0);
			break;
		}
		else if((CStringW)recvmsg.type == "add"){

			CString str;
			str = recvmsg.content;
			int nCount = 0; 
			int	i = 0;
			CString getstr;
			while (AfxExtractSubString(getstr,str,nCount++,',')){

				//CStringתΪchar*
				USES_CONVERSION;
				char *buf = T2A(getstr.GetBuffer(getstr.GetLength()));
				getstr.ReleaseBuffer(getstr.GetLength());

				if (0 == i){

					::SendMessage(AfxGetApp()->m_pMainWnd->m_hWnd,WM_LOCALUSERNAME,(WPARAM)buf,0);
				}
				i++;

				::SendMessage(AfxGetApp()->m_pMainWnd->m_hWnd,WM_UPDATECLIENTLIST,(WPARAM)buf,0);
				
			}
		}
		else if ((CStringW)recvmsg.type == "login"){

			m_LoginConfirm = recvmsg.content;
			::SendMessage(AfxGetApp()->m_pMainWnd->m_hWnd,WM_LOGIN,NULL,NULL);
		}
		else if ((CStringW)recvmsg.type == "delete"){

			::SendMessage(AfxGetApp()->m_pMainWnd->m_hWnd,WM_DISCONNECT,(WPARAM)recvmsg.content,NULL);
		}
		else if ((CStringW)recvmsg.type == "Admin"){

			::SendMessage(AfxGetApp()->m_pMainWnd->m_hWnd,WM_RECVDATA,(WPARAM)recvmsg.content,1);
		}
		else{

			::SendMessage(AfxGetApp()->m_pMainWnd->m_hWnd,WM_RECVDATA,(WPARAM)recvmsg.content,NULL);
		}
	}

	return 0;
}

LRESULT CRelClientDlg::OnRecvData(WPARAM wParam,LPARAM lParam){

	if (lParam == 1){

		CString str;
		USES_CONVERSION;
		str = A2T((char*)wParam);
		str.Format(L"admin��%s",str);
		m_RecvMessage.AddString(str);
	}
	else{

		int n = 0;
		CString str,getstr,sendername,context;
		USES_CONVERSION;
		str = A2T((char*)wParam);

		AfxExtractSubString(sendername,str,0,',');
		n = str.Find(',');
		str = str.Right(str.GetLength()-n-1);

		str.Format(L"%s��%s",sendername,str);
		m_RecvMessage.AddString(str);
	}

	return 0;
}

LRESULT CRelClientDlg::OnUpdateClientList(WPARAM wParam,LPARAM lParam){

	CString str;
	str.Format(L"%s",(CStringW)(char*)wParam);
	m_UserName.AddString(str);

	CString myName;
	int n = 0;
	n = m_LocalUserName.GetTextLen(n);
	m_LocalUserName.GetText(0,myName.GetBuffer(n));
	myName.ReleaseBuffer();
	int nItem = 0;
	while((nItem = m_UserName.FindString(nItem,myName)) != CB_ERR){

		m_UserName.DeleteString(nItem);
	}

	static int i = 0;
	if(i == 0){

		m_UserName.AddString(L"����Ա");
		i++;
	}

	return 0;
}

LRESULT CRelClientDlg::OnLocalUserName(WPARAM wParam,LPARAM lParam){

	CString str;
	USES_CONVERSION;
	str = A2T((char*)wParam);
	if(m_LocalUserName.GetCount() == 0){

		m_LocalUserName.AddString(str);
	}
	
	return 0;
}

LRESULT CRelClientDlg::OnDisconnect(WPARAM wParam,LPARAM lParam){

	CString str;
	str.Format(L"%s",(CStringW)(char*)wParam);
	int index = m_UserName.FindString(-1,str);
	m_UserName.DeleteString(index);
	
	return 0;
}

void CRelClientDlg::OnBnClickedBtnSend()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	TRANSFER_MSG sendbuf;
	CString strSend;
	CString myName;
	CString displayself;
	//��ô���������
	GetDlgItemText(IDC_EDIT_SEND,strSend);
	displayself = strSend;
	//��÷�����Ϣ�Ŀͻ���IP
	int n = 0;
	n = m_LocalUserName.GetTextLen(n);
	m_LocalUserName.GetText(0,myName.GetBuffer(n));
	myName.ReleaseBuffer();

	strSend.Format(L"%s,%s",myName,strSend);
	USES_CONVERSION;
	char *pStrSend = T2A(strSend);
	memset(sendbuf.type,0,sizeof(sendbuf.type));
	memset(sendbuf.content,0,sizeof(sendbuf.content));
	//��佫Ҫ���͵Ľṹ��
	CString ClientIp;
	m_UserName.GetLBText(m_UserName.GetCurSel(),ClientIp);
	char *pIP = T2A(ClientIp.GetBuffer(ClientIp.GetLength()));
	ClientIp.ReleaseBuffer(ClientIp.GetLength());
	memcpy(sendbuf.content,pStrSend,sizeof(sendbuf.content));
	memcpy(sendbuf.type,pIP,sizeof(sendbuf.type));
	//���Լ����͵���ϢҲ��ʾ����Ϣ����
	CString strMysend;
	strMysend.Format(L"��--->%s:%s",ClientIp,(CStringW)displayself);
	m_RecvMessage.AddString(strMysend);
	
	if (ClientIp == L"����Ա"){

		TRANSFER_MSG sendadmin;
		memset(sendadmin.type,0,sizeof(sendadmin.type));
		memset(sendadmin.content,0,sizeof(sendadmin.content));
		memcpy(sendadmin.type,"Admin",sizeof(sendadmin.type));
		memcpy(sendadmin.content,pStrSend,sizeof(sendadmin.content));
		send(m_sockClient,(char*)&sendadmin,strlen(sendadmin.content) + 32,0);
	}
	else{

		//��������
		if(SOCKET_ERROR == send(m_sockClient,(char*)&sendbuf,strlen(sendbuf.content) + 32,0)){

			MessageBox(L"����ʧ�ܣ�");
		}
	}
	//��շ��ͱ༭�������
	SetDlgItemText(IDC_EDIT_SEND,L"");
}

void CRelClientDlg::HideWindow(void){

	CenterWindow();
	GetWindowPlacement(&hwp);
	ModifyStyleEx(WS_EX_APPWINDOW,WS_EX_TOOLWINDOW);
	WINDOWPLACEMENT wp;
	wp.length = sizeof(WINDOWPLACEMENT);
	wp.flags = WPF_RESTORETOMAXIMIZED;
	wp.showCmd = SW_HIDE;
	SetWindowPlacement(&wp);
}

void CRelClientDlg::ShowWindow(void){

	ModifyStyleEx(WS_EX_TOOLWINDOW,WS_EX_APPWINDOW); 
	SetWindowPlacement(&hwp); 
}

void CRelClientDlg::ShowLoginWindow(void){

	this->CenterWindow();
	int nReturn = m_Logindlg.DoModal();
	if (nReturn == IDCANCEL){

		PostQuitMessage(0); 
	}
	if (nReturn == IDOK){

		TRANSFER_MSG sendbuf;
		CString strsend;
		memset(sendbuf.type,0,sizeof(sendbuf.type));
		memset(sendbuf.content,0,sizeof(sendbuf.content));
		m_LoginUsername = m_Logindlg.m_LoginUser;
		m_LoginPassword = m_Logindlg.m_LoginPassword;
		strsend.Format(L"%s,%s",m_LoginUsername,m_LoginPassword);
		USES_CONVERSION;
		char *pStrSend = T2A(strsend);
		memcpy(sendbuf.type,"login",sizeof(sendbuf.type));
		memcpy(sendbuf.content,pStrSend,sizeof(sendbuf.content));
		send(m_sockClient,(char*)&sendbuf,strlen(sendbuf.content) + 32,0);
	}
}

LRESULT CRelClientDlg::OnCreateUser(WPARAM wParam,LPARAM lParam){

	TRANSFER_MSG createuserbuf;
	CString strSend;
	memset(createuserbuf.type,0,sizeof(createuserbuf.type));
	memset(createuserbuf.content,0,sizeof(createuserbuf.content));
	strSend.Format(L"%s,%s",m_Logindlg.m_NewUserName,m_Logindlg.m_NewUserPassword);
	USES_CONVERSION;
	char *pStrSend = T2A(strSend);
	memcpy(createuserbuf.type,"newuser",sizeof(createuserbuf.type));
	memcpy(createuserbuf.content,pStrSend,sizeof(createuserbuf.type));

	if(SOCKET_ERROR == send(m_sockClient,(char*)&createuserbuf,strlen(createuserbuf.content) + 32,0)){

		MessageBox(L"�û�����ʧ�ܣ�");
	}else{

		MessageBox(L"��ϲ����ע��ɹ�~");
	}

	return 0;
}

LRESULT CRelClientDlg::OnLogin(WPARAM wParam,LPARAM lParam){

	if (m_LoginConfirm == "confirm"){

		ShowWindow();
	}
	if (m_LoginConfirm == "errorpassword"){

		::MessageBox(NULL,L"�������",L"��¼ʧ��",MB_OK);
		ShowLoginWindow();
	}
	if(m_LoginConfirm == "unknownuser"){

		::MessageBox(NULL,L"�û������ڣ�",L"��¼ʧ��",MB_OK);
		ShowLoginWindow();
	}
	
	return 0;
}