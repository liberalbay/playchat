// UserInfo.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "afxdialogex.h"
#include "UserInfo.h"


// CUserInfo �Ի���

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


// CUserInfo ��Ϣ�������

void CUserInfo::InitUserTable(void){

	LONG lStyle;
	lStyle = GetWindowLong(m_DisUserInfo.m_hWnd, GWL_STYLE);//��ȡ��ǰ����style
	lStyle &= ~LVS_TYPEMASK; //�����ʾ��ʽλ	
	lStyle |= LVS_REPORT; //����style
	lStyle |= LVS_SINGLESEL;//��ѡģʽ
	SetWindowLong(m_DisUserInfo.m_hWnd, GWL_STYLE, lStyle);//����style

	DWORD dwStyle = m_DisUserInfo.GetExtendedStyle();
	dwStyle |= LVS_EX_FULLROWSELECT;//ѡ��ĳ��ʹ���и�����ֻ������report����listctrl��
	dwStyle |= LVS_EX_GRIDLINES;//�����ߣ�ֻ������report����listctrl��
	//dwStyle |= LVS_EX_CHECKBOXES;//itemǰ����checkbox�ؼ�
	m_DisUserInfo.SetExtendedStyle(dwStyle); //������չ���

	//��ʼ����;
	m_DisUserInfo.InsertColumn(0,_T("���"),LVCFMT_LEFT,40,-1);
	m_DisUserInfo.InsertColumn(1,_T("�û���"),LVCFMT_LEFT,100,-1);
	m_DisUserInfo.InsertColumn(2,_T("����"),LVCFMT_LEFT,100,-1);
	m_DisUserInfo.InsertColumn(3,_T("�ͻ�����Ϣ"),LVCFMT_LEFT,200,-1);
}

void CUserInfo::DisplayUserMes(CString str){

	CString getstr;
	int nCount = 0;
	int nRow = 0;
	while (AfxExtractSubString(getstr,str,nCount++,',')){

		if (nCount < 6){

			if(nCount == 1){

				nRow = m_DisUserInfo.InsertItem(0,getstr);//������
			}
			else{

				m_DisUserInfo.SetItemText(nRow, nCount - 1, getstr);//��������
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
