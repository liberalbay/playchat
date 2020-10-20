// CreateUser.cpp : 实现文件
//

#include "stdafx.h"
#include "RelClient.h"
#include "CreateUser.h"
#include "afxdialogex.h"


// CCreateUser 对话框

IMPLEMENT_DYNAMIC(CCreateUser, CDialogEx)

CCreateUser::CCreateUser(CWnd* pParent /*=NULL*/)
	: CDialogEx(CCreateUser::IDD, pParent)
{

}

CCreateUser::~CCreateUser()
{
}

void CCreateUser::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX,IDC_EDIT_CREATENAME,m_CreateName);
	DDX_Text(pDX,IDC_EDIT_PASSWORD,m_CreatePassword);
	DDX_Text(pDX,IDC_EDIT_CONPASSWORD,m_ConfrimPassword);
}


BEGIN_MESSAGE_MAP(CCreateUser, CDialogEx)
END_MESSAGE_MAP()


// CCreateUser 消息处理程序
