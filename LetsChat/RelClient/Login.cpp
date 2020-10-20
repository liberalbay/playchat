// Login.cpp : 实现文件
//

#include "stdafx.h"
#include "RelClient.h"
#include "Login.h"
#include "afxdialogex.h"
#include "CreateUser.h"

// CLogin 对话框

IMPLEMENT_DYNAMIC(CLogin, CDialogEx)

CLogin::CLogin(CWnd* pParent /*=NULL*/)
	: CDialogEx(CLogin::IDD, pParent)
{

}

CLogin::~CLogin()
{
}

void CLogin::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX,IDC_EDIT_USER,m_LoginUser);
	DDX_Text(pDX, IDC_EDIT_PASSWORD, m_LoginPassword);
}


BEGIN_MESSAGE_MAP(CLogin, CDialogEx)
	ON_BN_CLICKED(IDC_BTN_CREATEUSER, &CLogin::OnBnClickedBtnCreateuser)
END_MESSAGE_MAP()


void CLogin::OnBnClickedBtnCreateuser(){
	
	CCreateUser createuser;
	int nReturn = createuser.DoModal();
	if (nReturn == IDCANCEL){

		//
	}
	if (nReturn == IDOK){

		m_NewUserName = createuser.m_CreateName;
		m_NewUserPassword = createuser.m_ConfrimPassword;
		CString password = createuser.m_CreatePassword;

		if (m_NewUserName == L""){

			MessageBox(L"用户名不能为空！");

			return;
		}
		if (password == L""){

			MessageBox(L"密码不能为空！");

			return;
		}
		if (m_NewUserPassword != password){

			MessageBox(L"密码不一致！");

			return;
		}

		HWND hmainwindow;
		if (!(hmainwindow = ::FindWindow(NULL,L"Client"))){

			MessageBox(L"Error!");
		}
		::SendMessage(hmainwindow,WM_CREATEUSER,0,0);
	}
}
