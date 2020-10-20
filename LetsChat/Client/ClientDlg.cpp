
// ClientDlg.cpp : 实现文件
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


// 用于应用程序“关于”菜单项的 CAboutDlg 对话框

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// 对话框数据
	enum { IDD = IDD_ABOUTBOX };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

// 实现
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


// CClientDlg 对话框




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


// CClientDlg 消息处理程序

BOOL CClientDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// 将“关于...”菜单项添加到系统菜单中。

	// IDM_ABOUTBOX 必须在系统命令范围内。
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

	// 设置此对话框的图标。当应用程序主窗口不是对话框时，框架将自动
	//  执行此操作
	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标

	// TODO: 在此添加额外的初始化代码

	InitDatabase("root","root","localhost",3306,"clientinfo",NULL,0,info,mysql);   //初始化 连接数据库

	if (FALSE == InitSocket()){

		PostQuitMessage(0);      //套接字初始化失败，终止程序
	}
	
	HANDLE hThreadAccept = CreateThread(NULL,0,AcceptProc,(LPVOID)m_socketSrv,0,NULL);  //等待客户端连接的子线程

	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
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

// 如果向对话框添加最小化按钮，则需要下面的代码
//  来绘制该图标。对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。

void CClientDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 用于绘制的设备上下文

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// 使图标在工作区矩形中居中
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// 绘制图标
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

//当用户拖动最小化窗口时系统调用此函数取得光标
//显示。
HCURSOR CClientDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

BOOL CClientDlg::InitSocket(void)
{
	//创建套接字
	m_socketSrv = socket(AF_INET,SOCK_STREAM,0);
	if (INVALID_SOCKET == m_socketSrv){

		MessageBox(L"套接字创建失败！");
		return FALSE;
	}

	SOCKADDR_IN addrSrv;
	addrSrv.sin_addr.S_un.S_addr = htonl(INADDR_ANY);
	addrSrv.sin_family = AF_INET;
	addrSrv.sin_port = htons(8080);

	//绑定套接字
	if (SOCKET_ERROR == bind(m_socketSrv,(SOCKADDR*)&addrSrv,sizeof(SOCKADDR))){

		closesocket(m_socketSrv);
		MessageBox(L"绑定失败！");
		return FALSE;
	}

	//监听
	if (SOCKET_ERROR == listen(m_socketSrv,SOMAXCONN)){

		MessageBox(L"监听失败！");
		return FALSE;
	}

	return TRUE;
}

DWORD WINAPI CClientDlg::AcceptProc(LPVOID lpParameter){

	int len = sizeof(SOCKADDR);
	for (int i = 1; m_transferSock.size() <= 100; i++){

		SOCKET sockConn = accept(SOCKET(lpParameter),(SOCKADDR*)&addrClient,&len);
		if (INVALID_SOCKET == sockConn){

			::MessageBox(AfxGetApp()->m_pMainWnd->m_hWnd,L"客户端连接失败",L"警告",MB_OK);
			continue;
		}

		//获取客户端IP
		strcpy_s(ip,inet_ntoa(addrClient.sin_addr));
		CString m_ip;
		m_ip = ip;   //char*转为CString
		CString Client_name;
		Client_name.Format(L"Client%d(%s)",i,(CStringW)m_ip);   //为每一个请求连接的客户机命名
		//将Client_name转为char*存在ip中
		int n = 0;
		for(; n < Client_name.GetLength(); n++){

			ip[n] = Client_name.GetAt(n);   
		}
		ip[n] = '\0';       //在ip中存储了客户机名和ip地址

		m_transferSock[Client_name] = sockConn;  //更新用户列表

		//这是为了给客户端更新用户列表
		TRANSFER_MSG sendbuf;//定义发送结构体
		memset(sendbuf.content,0,sizeof(sendbuf.content));//清空缓存
		memset(sendbuf.type,0,sizeof(sendbuf.type));//清空缓存
		memcpy(sendbuf.type, "add", sizeof(sendbuf.type));//将发送的信息定义为add
		n = 0; 
		CString str;
		//将其他客户端连接的信息发送给当前的客户端
		for (map<CString,SOCKET>::iterator it = m_transferSock.begin(); it != m_transferSock.end(); it++){

			if ((*it).first != Client_name){

				TRANSFER_MSG sendclient;
				memset(sendclient.type,0,sizeof(sendclient.type));
				memset(sendclient.content,0,sizeof(sendclient.content));
				memcpy(sendclient.type,"add",sizeof(sendclient.type));
				memcpy(sendclient.content,ip,sizeof(sendclient.content));
				send((*it).second, (char*)&sendclient, strlen(sendclient.content) + 32, 0);
			}

			if (n == 0){    //将第一个请求连接的客户机名存储在str中

				str.Format(L"%s",(CStringW)it->first);
			}else{          //将第二个请求连接的客户机名存储在第一个客户机名的后面，用逗号相隔

				str.Format(L"%s,%s",(CStringW)it->first,(CStringW)str);
			}
			n++;
		}
		n = 0;
		for(; n < str.GetLength(); n++){

			sendbuf.content[n] = str.GetAt(n);
		}
		sendbuf.content[n] = '\0';
		send(sockConn,(char *)&sendbuf,strlen(sendbuf.content)+32,0);   //给客户机发送其他客户机连接的信息

		::SendMessage(AfxGetApp()->m_pMainWnd->m_hWnd,WM_UPDATECLIENTLIST,(WPARAM)ip,1);  //更新服务器的用户列表
		HANDLE hThreadRecv = CreateThread(NULL,0,RecvProc,(LPVOID)sockConn,0,NULL);  //开辟接收信息的线程
	}

	return 0;
}

DWORD WINAPI CClientDlg::RecvProc(LPVOID lpParameter){

	char deleteIP[100];     //定义离线的客户端名
	strcpy_s(deleteIP,ip);

	while(TRUE){

		TRANSFER_MSG recvmsg;
		char recvbuf[1024] = {0};
		int recvd = recv((SOCKET)lpParameter,recvbuf,sizeof(recvbuf),0);
		memcpy(&recvmsg,recvbuf,sizeof(recvmsg));
		
		SOCKET transfersock;
		CString ClientName;

		if (recvd == -1){   //客户端离线 recv函数就会返回-1

			::SendMessage(AfxGetApp()->m_pMainWnd->m_hWnd,WM_DISCONNECT,(WPARAM)deleteIP,NULL);

			for(map<CString,SOCKET>::iterator it = m_transferSock.begin(); it != m_transferSock.end(); it++){

				if(it->first == (CStringW)deleteIP){

					m_transferSock.erase(it);    //将离线的客户端从列表中删除
					break;
				}
			}

			//告诉其他客户端此客户端已离线 因为此刻map中已经不再有离线的客户端信息 有的都是在线的客户端 所以就可以整个遍历
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
			str = recvmsg.content;                    //str就是password
			AfxExtractSubString(username,str,0,',');
			n = str.Find(',');
			str = str.Right(str.GetLength()-n-1);     //（abc,123）删除逗号之前的内容
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
			      //经过服务器转发从客户端发送给另一个客户端的消息
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

	//取出接收到的数据
	int n = 0;
	CString str,sendername;
	USES_CONVERSION;
	str = A2T((char*)lParam);
	AfxExtractSubString(sendername,str,0,',');
	n = str.Find(',');
	str = str.Right(str.GetLength()-n-1);
	str.Format(L"%s：%s",sendername,str);
	m_RecvMessage.AddString(str);
	return 0;
}

LRESULT CClientDlg::OnUpdateClientList(WPARAM wParam,LPARAM lParam){

	CString str;
	str = (char*)wParam;
	m_UserName.AddString(str);
	static int i = 0;
	if(i == 0){

		m_UserName.AddString(L"所有人");
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

	// TODO: 在此添加控件通知处理程序代码
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

	//将自己发送的消息也显示在消息框中
	CString strMysend,strSelect;
	m_UserName.GetLBText(m_UserName.GetCurSel(),strSelect);
	strMysend.Format(L"我--->%s：%s",strSelect,(CStringW)strSend);
	m_RecvMessage.AddString(strMysend);

	//发送数据
	if (selectip == L"所有人"){

		for (map<CString,SOCKET>::iterator it = m_transferSock.begin(); it != m_transferSock.end(); it++){

			if(SOCKET_ERROR == send((*it).second,(char*)&sendbuf,strlen(sendbuf.content) + 32,0)){

				//MessageBox(L"发送失败！");
			}
		}
	}else{

		if(SOCKET_ERROR == send(selectsocket,(char*)&sendbuf,strlen(sendbuf.content) + 32,0)){

			MessageBox(L"发送失败！");
		}
	}

	//清空发送编辑框的内容
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

		::MessageBox(AfxGetApp()->m_pMainWnd->m_hWnd,L"数据库连接失败",L"警告",MB_OK);
	}

}

void CClientDlg::CreateNewusers(const CString Password, const CString Username, CString IP){

	CString str;
	str.Format(L"insert into userinfo (password,username,ip,submission_date) values ('%s','%s','%s',NOW())",Password,Username,IP);
	USES_CONVERSION;
	const char* command = T2A(str);
	mysql.ExecuteSql(command);
}


void CClientDlg::OnBnClickedButton3(){    //查看所有用户按钮
	
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

			MessageBox(L"删除成功！");
		}
	}
	
	return 0;
}

void CClientDlg::OnBnClickedButton1(){     //断开连接按钮
	
	SOCKET disconnectsock;
	CString disconnectip;
	m_UserName.GetLBText(m_UserName.GetCurSel(),disconnectip);

	if (disconnectip == "所有人"){

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
