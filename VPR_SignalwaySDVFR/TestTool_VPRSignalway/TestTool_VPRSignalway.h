
// TestTool_VPRSignalway.h : PROJECT_NAME Ӧ�ó������ͷ�ļ�
//

#pragma once

#ifndef __AFXWIN_H__
	#error "�ڰ������ļ�֮ǰ������stdafx.h�������� PCH �ļ�"
#endif

#include "resource.h"		// ������


// CTestTool_VPRSignalwayApp: 
// �йش����ʵ�֣������ TestTool_VPRSignalway.cpp
//

class CTestTool_VPRSignalwayApp : public CWinApp
{
public:
	CTestTool_VPRSignalwayApp();

// ��д
public:
	virtual BOOL InitInstance();

// ʵ��

	DECLARE_MESSAGE_MAP()
};

extern CTestTool_VPRSignalwayApp theApp;