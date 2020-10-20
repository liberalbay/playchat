#pragma once
#define WM_SENDNEWUSERINFO WM_USER+3

struct ConnectionInfo{

	const char* host;           //主机地址
	const char* user;			//用户名
	const char* password;		//密码
	const char* database;		//数据库名
	unsigned int port;			//端口号
	const char* unix_socket;
	unsigned long clientflag;

	//构造函数，设置一些默认值
	ConnectionInfo():host("127.0.0.1"),port(3306),unix_socket(NULL),clientflag(0){}
};

class CMySQLManager
{
public:
	CMySQLManager(void);
	~CMySQLManager(void);

public:

	//连接数据库
	bool Init(ConnectionInfo &info);

	//释放连接
	bool FreeConnect();

	//增加数据
	bool InsertData(const char* sql);

	//删除数据
	bool DeleteData(const char* sql);

	//更新数据
	bool UpdateData(const char* sql);

	//执行sql语句(执行添加新用户的语句)
	bool ExecuteSql(const char* sql);

	//查询数据
	MYSQL_RES* QueryData(const char* sql);

	//输出结果集
	void PrintQueryRes();

	//登录验证
	int IdentifyUser(CString &username,CString &password);


private:
	MYSQL m_mysql;       //mysql连接句柄
	MYSQL_RES* m_res;	 //查询结果集
};

