
// RelClient.h : PROJECT_NAME Ӧ�ó������ͷ�ļ�
//

#pragma once

#ifndef __AFXWIN_H__
	#error "�ڰ������ļ�֮ǰ������stdafx.h�������� PCH �ļ�"
#endif

#include "resource.h"		// ������


// CRelClientApp:
// �йش����ʵ�֣������ RelClient.cpp
//

class CRelClientApp : public CWinApp
{
public:
	CRelClientApp();

// ��д
public:
	virtual BOOL InitInstance();


// ʵ��

	DECLARE_MESSAGE_MAP()
};

extern CRelClientApp theApp;