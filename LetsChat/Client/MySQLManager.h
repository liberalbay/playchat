#pragma once
#define WM_SENDNEWUSERINFO WM_USER+3

struct ConnectionInfo{

	const char* host;           //������ַ
	const char* user;			//�û���
	const char* password;		//����
	const char* database;		//���ݿ���
	unsigned int port;			//�˿ں�
	const char* unix_socket;
	unsigned long clientflag;

	//���캯��������һЩĬ��ֵ
	ConnectionInfo():host("127.0.0.1"),port(3306),unix_socket(NULL),clientflag(0){}
};

class CMySQLManager
{
public:
	CMySQLManager(void);
	~CMySQLManager(void);

public:

	//�������ݿ�
	bool Init(ConnectionInfo &info);

	//�ͷ�����
	bool FreeConnect();

	//��������
	bool InsertData(const char* sql);

	//ɾ������
	bool DeleteData(const char* sql);

	//��������
	bool UpdateData(const char* sql);

	//ִ��sql���(ִ��������û������)
	bool ExecuteSql(const char* sql);

	//��ѯ����
	MYSQL_RES* QueryData(const char* sql);

	//��������
	void PrintQueryRes();

	//��¼��֤
	int IdentifyUser(CString &username,CString &password);


private:
	MYSQL m_mysql;       //mysql���Ӿ��
	MYSQL_RES* m_res;	 //��ѯ�����
};

