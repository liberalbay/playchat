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

	//��ʼ��mysql,����mysql
	mysql_init(&m_mysql);

	//����ʧ��
	if(!(mysql_real_connect(&m_mysql,info.host,info.user,info.password,info.database,info.port,info.unix_socket,info.clientflag))){

		return false;
	}

	return true;
}

bool CMySQLManager::FreeConnect(){

	//�ͷ���Դ
	mysql_free_result(m_res);
	mysql_close(&m_mysql);

	return false;
}

// ִ��sql���, �������ӡ�ɾ������������
bool CMySQLManager::ExecuteSql(const char* sql){

	if(mysql_query(&m_mysql,sql)){

		CString errorReason;
		errorReason = mysql_error(&m_mysql);
		::MessageBox(AfxGetApp()->m_pMainWnd->m_hWnd,errorReason,L"����",MB_OK);

		return false;
	}
	else{

		::MessageBox(AfxGetApp()->m_pMainWnd->m_hWnd,L"�����û������ɹ��� ~",L"",MB_OK);
	}

	return true;
}

//��ѯ����
MYSQL_RES* CMySQLManager::QueryData(const char* sql){

	if (mysql_query(&m_mysql, sql)){

		CString errorReason;
		errorReason = mysql_error(&m_mysql);
		::MessageBox(AfxGetApp()->m_pMainWnd->m_hWnd,errorReason,L"��ѯ����",MB_OK);
		return nullptr;
	}
	else{

		//::MessageBox(AfxGetApp()->m_pMainWnd->m_hWnd,L"��ѯ�ɹ�",L"",MB_OK);
	}

	// �洢��ѯ���
	m_res = mysql_store_result(&m_mysql);

	return m_res;
}

// ���������
void CMySQLManager::PrintQueryRes(){

	HWND hWnd = ::FindWindow(NULL,_T("�û���Ϣ"));
	CUserInfo *pWnd = (CUserInfo*)CWnd::FromHandle(hWnd);
	pWnd->m_DisUserInfo.DeleteAllItems();    //�����ٴδ򿪶Ի�����ظ���������

	if (nullptr == m_res || NULL == m_res){

		return;
	}

	// ��ȡ����
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