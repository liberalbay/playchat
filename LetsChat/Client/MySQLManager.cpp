#include "StdAfx.h"
#include "MySQLManager.h"
#include "UserInfo.h"

CMySQLManager::CMySQLManager(void)
{
}


CMySQLManager::~CMySQLManager(void)
{
}

bool CMySQLManager::Init(ConnectionInfo &info){

	//初始化mysql,连接mysql
	mysql_init(&m_mysql);

	//连接失败
	if(!(mysql_real_connect(&m_mysql,info.host,info.user,info.password,info.database,info.port,info.unix_socket,info.clientflag))){

		return false;
	}

	return true;
}

bool CMySQLManager::FreeConnect(){

	//释放资源
	mysql_free_result(m_res);
	mysql_close(&m_mysql);

	return false;
}

// 执行sql语句, 包括增加、删除、更新数据
bool CMySQLManager::ExecuteSql(const char* sql){

	if(mysql_query(&m_mysql,sql)){

		CString errorReason;
		errorReason = mysql_error(&m_mysql);
		::MessageBox(AfxGetApp()->m_pMainWnd->m_hWnd,errorReason,L"警告",MB_OK);

		return false;
	}
	else{

		::MessageBox(AfxGetApp()->m_pMainWnd->m_hWnd,L"有新用户创建成功啦 ~",L"",MB_OK);
	}

	return true;
}

//查询数据
MYSQL_RES* CMySQLManager::QueryData(const char* sql){

	if (mysql_query(&m_mysql, sql)){

		CString errorReason;
		errorReason = mysql_error(&m_mysql);
		::MessageBox(AfxGetApp()->m_pMainWnd->m_hWnd,errorReason,L"查询警告",MB_OK);
		return nullptr;
	}
	else{

		//::MessageBox(AfxGetApp()->m_pMainWnd->m_hWnd,L"查询成功",L"",MB_OK);
	}

	// 存储查询结果
	m_res = mysql_store_result(&m_mysql);

	return m_res;
}

// 遍历结果集
void CMySQLManager::PrintQueryRes(){

	HWND hWnd = ::FindWindow(NULL,_T("用户信息"));
	CUserInfo *pWnd = (CUserInfo*)CWnd::FromHandle(hWnd);
	pWnd->m_DisUserInfo.DeleteAllItems();    //避免再次打开对话框而重复插入数据

	if (nullptr == m_res || NULL == m_res){

		return;
	}

	// 获取列数
	int columns = mysql_num_fields(m_res);
	MYSQL_ROW row;
	while ((row = mysql_fetch_row(m_res)) != NULL){

		CString UserInfo;

		for (int i = 0; i < columns; ++i){

			char *a = row[i];
			char buffer[100];
			sprintf_s(buffer,"%s,",a);
			UserInfo += buffer;
		}
		pWnd->DisplayUserMes(UserInfo);
	}
}

int CMySQLManager::IdentifyUser(CString &username,CString &password){

	CString passwordconfirm;
	CString str;
	str.Format(L"select password from userinfo where username = '%s'",username);
	USES_CONVERSION;
	const char* command = T2A(str);
	QueryData(command);

	int columns = mysql_num_fields(m_res);
	int rows = mysql_num_rows(m_res);
	MYSQL_ROW row;
	while ((row = mysql_fetch_row(m_res)) != NULL){

		for (int i = 0; i < columns; ++i){

			char *a = row[i];
			passwordconfirm = a;
		}
	}

	if (rows == 0){

		return 0;
	}

	if (password == passwordconfirm){

		return 1;
	}else
		return -1;
}

bool CMySQLManager::DeleteData(const char* sql){

	if(mysql_query(&m_mysql,sql)){

		return false;
	}else
		return true;
}