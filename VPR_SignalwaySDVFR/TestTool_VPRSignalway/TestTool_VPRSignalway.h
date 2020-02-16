
// TestTool_VPRSignalway.h : PROJECT_NAME 应用程序的主头文件
//

#pragma once

#ifndef __AFXWIN_H__
	#error "在包含此文件之前包含“stdafx.h”以生成 PCH 文件"
#endif

#include "resource.h"		// 主符号


// CTestTool_VPRSignalwayApp: 
// 有关此类的实现，请参阅 TestTool_VPRSignalway.cpp
//

class CTestTool_VPRSignalwayApp : public CWinApp
{
public:
	CTestTool_VPRSignalwayApp();

// 重写
public:
	virtual BOOL InitInstance();

// 实现

	DECLARE_MESSAGE_MAP()
};

extern CTestTool_VPRSignalwayApp theApp;