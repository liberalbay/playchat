
// ClientDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "Client.h"
#include "ClientDlg.h"
#include "afxdialogex.h"
#include "MySQLManager.h"
#include "UserInfo.h"
#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// ����Ӧ�ó��򡰹��ڡ��˵���� CAboutDlg �Ի���

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


// CClientDlg �Ի���




CClientDlg::CClientDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CClientDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CClientDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX,IDC_LIST_RECV,m_RecvMessage);
	DDX_Control(pDX,IDC_COMBO_USER,m_UserName);
}

BEGIN_MESSAGE_MAP(CClientDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_MESSAGE(WM_RECVDATA,OnRecvData)
	ON_MESSAGE(WM_UPDATECLIENTLIST,OnUpdateClientList)
	ON_MESSAGE(WM_DELETEUSER,OnDeleteUser)
	ON_MESSAGE(WM_DISCONNECT,OnDisconnect)
	ON_BN_CLICKED(IDC_BTN_SEND, &CClientDlg::OnBnClickedBtnSend)
	ON_WM_TIMER()
	ON_BN_CLICKED(IDC_BUTTON3, &CClientDlg::OnBnClickedButton3)
	ON_BN_CLICKED(IDC_BUTTON1, &CClientDlg::OnBnClickedButton1)
END_MESSAGE_MAP()


// CClientDlg ��Ϣ�������

BOOL CClientDlg::OnInitDialog()
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

	InitDatabase("root","root","localhost",3306,"clientinfo",NULL,0,info,mysql);   //��ʼ�� �������ݿ�

	if (FALSE == InitSocket()){

		PostQuitMessage(0);      //�׽��ֳ�ʼ��ʧ�ܣ���ֹ����
	}
	
	HANDLE hThreadAccept = CreateThread(NULL,0,AcceptProc,(LPVOID)m_socketSrv,0,NULL);  //�ȴ��ͻ������ӵ����߳�

	return TRUE;  // ���ǽ��������õ��ؼ������򷵻� TRUE
}

void CClientDlg::OnSysCommand(UINT nID, LPARAM lParam)
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

void CClientDlg::OnPaint()
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
HCURSOR CClientDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

BOOL CClientDlg::InitSocket(void)
{
	//�����׽���
	m_socketSrv = socket(AF_INET,SOCK_STREAM,0);
	if (INVALID_SOCKET == m_socketSrv){

		MessageBox(L"�׽��ִ���ʧ�ܣ�");
		return FALSE;
	}

	SOCKADDR_IN addrSrv;
	addrSrv.sin_addr.S_un.S_addr = htonl(INADDR_ANY);
	addrSrv.sin_family = AF_INET;
	addrSrv.sin_port = htons(8080);

	//���׽���
	if (SOCKET_ERROR == bind(m_socketSrv,(SOCKADDR*)&addrSrv,sizeof(SOCKADDR))){

		closesocket(m_socketSrv);
		MessageBox(L"��ʧ�ܣ�");
		return FALSE;
	}

	//����
	if (SOCKET_ERROR == listen(m_socketSrv,SOMAXCONN)){

		MessageBox(L"����ʧ�ܣ�");
		return FALSE;
	}

	return TRUE;
}

DWORD WINAPI CClientDlg::AcceptProc(LPVOID lpParameter){

	int len = sizeof(SOCKADDR);
	for (int i = 1; m_transferSock.size() <= 100; i++){

		SOCKET sockConn = accept(SOCKET(lpParameter),(SOCKADDR*)&addrClient,&len);
		if (INVALID_SOCKET == sockConn){

			::MessageBox(AfxGetApp()->m_pMainWnd->m_hWnd,L"�ͻ�������ʧ��",L"����",MB_OK);
			continue;
		}

		//��ȡ�ͻ���IP
		strcpy_s(ip,inet_ntoa(addrClient.sin_addr));
		CString m_ip;
		m_ip = ip;   //char*תΪCString
		CString Client_name;
		Client_name.Format(L"Client%d(%s)",i,(CStringW)m_ip);   //Ϊÿһ���������ӵĿͻ�������
		//��Client_nameתΪchar*����ip��
		int n = 0;
		for(; n < Client_name.GetLength(); n++){

			ip[n] = Client_name.GetAt(n);   
		}
		ip[n] = '\0';       //��ip�д洢�˿ͻ�������ip��ַ

		m_transferSock[Client_name] = sockConn;  //�����û��б�

		//����Ϊ�˸��ͻ��˸����û��б�
		TRANSFER_MSG sendbuf;//���巢�ͽṹ��
		memset(sendbuf.content,0,sizeof(sendbuf.content));//��ջ���
		memset(sendbuf.type,0,sizeof(sendbuf.type));//��ջ���
		memcpy(sendbuf.type, "add", sizeof(sendbuf.type));//�����͵���Ϣ����Ϊadd
		n = 0; 
		CString str;
		//�������ͻ������ӵ���Ϣ���͸���ǰ�Ŀͻ���
		for (map<CString,SOCKET>::iterator it = m_transferSock.begin(); it != m_transferSock.end(); it++){

			if ((*it).first != Client_name){

				TRANSFER_MSG sendclient;
				memset(sendclient.type,0,sizeof(sendclient.type));
				memset(sendclient.content,0,sizeof(sendclient.content));
				memcpy(sendclient.type,"add",sizeof(sendclient.type));
				memcpy(sendclient.content,ip,sizeof(sendclient.content));
				send((*it).second, (char*)&sendclient, strlen(sendclient.content) + 32, 0);
			}

			if (n == 0){    //����һ���������ӵĿͻ������洢��str��

				str.Format(L"%s",(CStringW)it->first);
			}else{          //���ڶ����������ӵĿͻ������洢�ڵ�һ���ͻ������ĺ��棬�ö������

				str.Format(L"%s,%s",(CStringW)it->first,(CStringW)str);
			}
			n++;
		}
		n = 0;
		for(; n < str.GetLength(); n++){

			sendbuf.content[n] = str.GetAt(n);
		}
		sendbuf.content[n] = '\0';
		send(sockConn,(char *)&sendbuf,strlen(sendbuf.content)+32,0);   //���ͻ������������ͻ������ӵ���Ϣ

		::SendMessage(AfxGetApp()->m_pMainWnd->m_hWnd,WM_UPDATECLIENTLIST,(WPARAM)ip,1);  //���·��������û��б�
		HANDLE hThreadRecv = CreateThread(NULL,0,RecvProc,(LPVOID)sockConn,0,NULL);  //���ٽ�����Ϣ���߳�
	}

	return 0;
}

DWORD WINAPI CClientDlg::RecvProc(LPVOID lpParameter){

	char deleteIP[100];     //�������ߵĿͻ�����
	strcpy_s(deleteIP,ip);

	while(TRUE){

		TRANSFER_MSG recvmsg;
		char recvbuf[1024] = {0};
		int recvd = recv((SOCKET)lpParameter,recvbuf,sizeof(recvbuf),0);
		memcpy(&recvmsg,recvbuf,sizeof(recvmsg));
		
		SOCKET transfersock;
		CString ClientName;

		if (recvd == -1){   //�ͻ������� recv�����ͻ᷵��-1

			::SendMessage(AfxGetApp()->m_pMainWnd->m_hWnd,WM_DISCONNECT,(WPARAM)deleteIP,NULL);

			for(map<CString,SOCKET>::iterator it = m_transferSock.begin(); it != m_transferSock.end(); it++){

				if(it->first == (CStringW)deleteIP){

					m_transferSock.erase(it);    //�����ߵĿͻ��˴��б���ɾ��
					break;
				}
			}

			//���������ͻ��˴˿ͻ��������� ��Ϊ�˿�map���Ѿ����������ߵĿͻ�����Ϣ �еĶ������ߵĿͻ��� ���ԾͿ�����������
			for(map<CString,SOCKET>::iterator it = m_transferSock.begin(); it != m_transferSock.end(); it++){

				TRANSFER_MSG sendbuf;
				memset(sendbuf.type,0,sizeof(sendbuf.type));
				memset(sendbuf.content,0,sizeof(sendbuf.content));
				memcpy(sendbuf.type,"delete",sizeof(sendbuf.type));
				memcpy(sendbuf.content,deleteIP,sizeof(sendbuf.type));
				send(it->second,(char*)&sendbuf,strlen(sendbuf.content) + 32,0);
			}
			break;
		}

		if ((CStringW)recvmsg.type == "newuser"){

			int n = 0;
			CString str,username,basicinfo;
			str = recvmsg.content;                    //str����password
			AfxExtractSubString(username,str,0,',');
			n = str.Find(',');
			str = str.Right(str.GetLength()-n-1);     //��abc,123��ɾ������֮ǰ������
			basicinfo = ip;

			CreateNewusers(str,username,basicinfo);
		}
		else if((CStringW)recvmsg.type == "login"){

			transfersock = SOCKET(lpParameter);
			TRANSFER_MSG sendbuf;
			CString strsend;
			memset(sendbuf.type,0,sizeof(sendbuf.type));
			memset(sendbuf.content,0,sizeof(sendbuf.content));
			memcpy(sendbuf.type,"login",sizeof(sendbuf.type));

			int n = 0;
			CString str,username,password;
			str = recvmsg.content;
			AfxExtractSubString(username,str,0,',');
			n = str.Find(',');
			str = str.Right(str.GetLength()-n-1);
			password = str;
			if (1 == mysql.IdentifyUser(username,password)){

				memcpy(sendbuf.content,"confirm",sizeof(sendbuf.type));
				send(transfersock,(char*)&sendbuf,strlen(sendbuf.content) + 32,0);
			}
			if(-1 == mysql.IdentifyUser(username,password)){

				memcpy(sendbuf.content,"errorpassword",sizeof(sendbuf.type));
				send(transfersock,(char*)&sendbuf,strlen(sendbuf.content) + 32,0);
			}
			if(0 == mysql.IdentifyUser(username,password)){

				memcpy(sendbuf.content,"unknownuser",sizeof(sendbuf.type));
				send(transfersock,(char*)&sendbuf,strlen(sendbuf.content) + 32,0);
			}
			
		}
		else if((CStringW)recvmsg.type == "Admin"){

			::SendMessage(AfxGetApp()->m_pMainWnd->m_hWnd,WM_RECVDATA,0,LPARAM(recvmsg.content));
		}
		else{
			      //����������ת���ӿͻ��˷��͸���һ���ͻ��˵���Ϣ
			for (map<CString,SOCKET>::iterator it = m_transferSock.begin(); it != m_transferSock.end(); it++){

				if (it->first == (CString)recvmsg.type){

					transfersock = it->second;
					break;
				}
			}

			send(transfersock,(char*)&recvmsg,strlen(recvmsg.content) + 32,0);
		}
	}

	return 0;
}

LRESULT CClientDlg::OnRecvData(WPARAM wParam,LPARAM lParam){

	//ȡ�����յ�������
	int n = 0;
	CString str,sendername;
	USES_CONVERSION;
	str = A2T((char*)lParam);
	AfxExtractSubString(sendername,str,0,',');
	n = str.Find(',');
	str = str.Right(str.GetLength()-n-1);
	str.Format(L"%s��%s",sendername,str);
	m_RecvMessage.AddString(str);
	return 0;
}

LRESULT CClientDlg::OnUpdateClientList(WPARAM wParam,LPARAM lParam){

	CString str;
	str = (char*)wParam;
	m_UserName.AddString(str);
	static int i = 0;
	if(i == 0){

		m_UserName.AddString(L"������");
		i++;
	}
	
	return 0;
}

LRESULT CClientDlg::OnDisconnect(WPARAM wParam,LPARAM lParam){

	CString str;
	str.Format(L"%s",(CStringW)(char*)wParam);
	int index = m_UserName.FindString(-1,str);
	m_UserName.DeleteString(index);
	
	return 0;
}

void CClientDlg::OnBnClickedBtnSend(){

	// TODO: �ڴ���ӿؼ�֪ͨ����������
	CString strSend;
	GetDlgItemText(IDC_EDIT_SEND,strSend);
	USES_CONVERSION;
	char *pStrSend = T2A(strSend);
	TRANSFER_MSG sendbuf;
	memset(sendbuf.type,0,sizeof(sendbuf.type));
	memset(sendbuf.content,0,sizeof(sendbuf.content));
	memcpy(sendbuf.type,"Admin",sizeof(sendbuf.type));
	memcpy(sendbuf.content,pStrSend,sizeof(sendbuf.content));

	SOCKET selectsocket;
	CString selectip;
	m_UserName.GetLBText(m_UserName.GetCurSel(),selectip);
	selectsocket = m_transferSock[selectip];

	//���Լ����͵���ϢҲ��ʾ����Ϣ����
	CString strMysend,strSelect;
	m_UserName.GetLBText(m_UserName.GetCurSel(),strSelect);
	strMysend.Format(L"��--->%s��%s",strSelect,(CStringW)strSend);
	m_RecvMessage.AddString(strMysend);

	//��������
	if (selectip == L"������"){

		for (map<CString,SOCKET>::iterator it = m_transferSock.begin(); it != m_transferSock.end(); it++){

			if(SOCKET_ERROR == send((*it).second,(char*)&sendbuf,strlen(sendbuf.content) + 32,0)){

				//MessageBox(L"����ʧ�ܣ�");
			}
		}
	}else{

		if(SOCKET_ERROR == send(selectsocket,(char*)&sendbuf,strlen(sendbuf.content) + 32,0)){

			MessageBox(L"����ʧ�ܣ�");
		}
	}

	//��շ��ͱ༭�������
	SetDlgItemText(IDC_EDIT_SEND,L"");
}

void CClientDlg::InitDatabase(const char* user,const char* password,const char* host, 
							  unsigned int port,const char* database,
							  char* unix_socket,unsigned long clientflag,ConnectionInfo &info,CMySQLManager &mysql){

	info.user = user;
	info.password = password;
	info.host = host;
	info.port = port;
	info.database = database;
	info.unix_socket = unix_socket;
	info.clientflag = clientflag;

	if (!mysql.Init(info)){

		::MessageBox(AfxGetApp()->m_pMainWnd->m_hWnd,L"���ݿ�����ʧ��",L"����",MB_OK);
	}

}

void CClientDlg::CreateNewusers(const CString Password, const CString Username, CString IP){

	CString str;
	str.Format(L"insert into userinfo (password,username,ip,submission_date) values ('%s','%s','%s',NOW())",Password,Username,IP);
	USES_CONVERSION;
	const char* command = T2A(str);
	mysql.ExecuteSql(command);
}


void CClientDlg::OnBnClickedButton3(){    //�鿴�����û���ť
	
	static int i = 0;
	if (i == 0){

		m_userinfo.Create(IDD_DIALOG_USERINFO,this);
		m_userinfo.InitUserTable();
		i++;
	}

	m_userinfo.ShowWindow(SW_SHOW);

	const char* sql4 = "select * from userinfo";
	mysql.QueryData(sql4);
	mysql.PrintQueryRes();
}
	
LRESULT CClientDlg::OnDeleteUser(WPARAM wParam,LPARAM lParam){

	CString str,deletestr;
	USES_CONVERSION;
	str = A2T((char*)wParam);
	deletestr.Format(L"delete from userinfo where username = '%s'",str);
	const char* command = T2A(deletestr);

	if(str != L""){

		if(mysql.DeleteData(command)){

			MessageBox(L"ɾ���ɹ���");
		}
	}
	
	return 0;
}

void CClientDlg::OnBnClickedButton1(){     //�Ͽ����Ӱ�ť
	
	SOCKET disconnectsock;
	CString disconnectip;
	m_UserName.GetLBText(m_UserName.GetCurSel(),disconnectip);

	if (disconnectip == "������"){

		for (map<CString,SOCKET>::iterator it = m_transferSock.begin(); it != m_transferSock.end(); it++){

			closesocket(it->second);
		}
	}
	else{

		disconnectsock = m_transferSock[disconnectip];
		closesocket(disconnectsock);
	}
	GetDlgItem(IDC_COMBO_USER)->SetWindowText(L"");
}
