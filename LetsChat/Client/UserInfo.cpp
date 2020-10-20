// UserInfo.cpp : 实现文件
//

#include "stdafx.h"
#include "afxdialogex.h"
#include "UserInfo.h"


// CUserInfo 对话框

IMPLEMENT_DYNAMIC(CUserInfo, CDialogEx)

CUserInfo::CUserInfo(CWnd* pParent /*=NULL*/)
	: CDialogEx(CUserInfo::IDD, pParent)
{

}

CUserInfo::~CUserInfo()
{
}

void CUserInfo::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX,IDC_LIST_USERINFO,m_DisUserInfo);
}


BEGIN_MESSAGE_MAP(CUserInfo, CDialogEx)
	ON_BN_CLICKED(ID_BTN_DELETE, &CUserInfo::OnBnClickedBtnDelete)
END_MESSAGE_MAP()


// CUserInfo 消息处理程序

void CUserInfo::InitUserTable(void){

	LONG lStyle;
	lStyle = GetWindowLong(m_DisUserInfo.m_hWnd, GWL_STYLE);//获取当前窗口style
	lStyle &= ~LVS_TYPEMASK; //清除显示方式位	
	lStyle |= LVS_REPORT; //设置style
	lStyle |= LVS_SINGLESEL;//单选模式
	SetWindowLong(m_DisUserInfo.m_hWnd, GWL_STYLE, lStyle);//设置style

	DWORD dwStyle = m_DisUserInfo.GetExtendedStyle();
	dwStyle |= LVS_EX_FULLROWSELECT;//选中某行使整行高亮（只适用与report风格的listctrl）
	dwStyle |= LVS_EX_GRIDLINES;//网格线（只适用与report风格的listctrl）
	//dwStyle |= LVS_EX_CHECKBOXES;//item前生成checkbox控件
	m_DisUserInfo.SetExtendedStyle(dwStyle); //设置扩展风格

	//初始化列;
	m_DisUserInfo.InsertColumn(0,_T("序号"),LVCFMT_LEFT,40,-1);
	m_DisUserInfo.InsertColumn(1,_T("用户名"),LVCFMT_LEFT,100,-1);
	m_DisUserInfo.InsertColumn(2,_T("密码"),LVCFMT_LEFT,100,-1);
	m_DisUserInfo.InsertColumn(3,_T("客户机信息"),LVCFMT_LEFT,200,-1);
}

void CUserInfo::DisplayUserMes(CString str){

	CString getstr;
	int nCount = 0;
	int nRow = 0;
	while (AfxExtractSubString(getstr,str,nCount++,',')){

		if (nCount < 6){

			if(nCount == 1){

				nRow = m_DisUserInfo.InsertItem(0,getstr);//插入行
			}
			else{

				m_DisUserInfo.SetItemText(nRow, nCount - 1, getstr);//设置数据
			}
		}
	}
}


void CUserInfo::OnBnClickedBtnDelete(){
	
	CString str;
	int index = m_DisUserInfo.GetSelectionMark();
	str = m_DisUserInfo.GetItemText(index,1);
	if (index != -1){

		m_DisUserInfo.DeleteItem(index);
	}

	HWND hmainwindow;
	hmainwindow = ::FindWindow(NULL,L"Server");
	USES_CONVERSION;
	char *send = T2A(str);
	::SendMessage(hmainwindow,WM_DELETEUSER,WPARAM(send),0);
}
