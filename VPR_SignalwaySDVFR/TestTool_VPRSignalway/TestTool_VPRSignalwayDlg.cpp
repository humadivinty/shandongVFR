
// TestTool_VPRSignalwayDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "TestTool_VPRSignalway.h"
#include "TestTool_VPRSignalwayDlg.h"
#include "afxdialogex.h"
#include <string>
#include <direct.h>
#include <Dbghelp.h>
#pragma comment(lib, "Dbghelp.lib")

#include "../VPR_SignalwaySDVFR/VPR_SignalwaySDVFR.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

#ifdef DEBUG
#pragma comment(lib, "../Debug/VPR_SignalwaySDVFR.lib")
#else
#pragma comment(lib, "../release/VPR_SignalwaySDVFR.lib")
#endif


// 用于应用程序“关于”菜单项的 CAboutDlg 对话框
char g_chCurrentPath[256];

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ABOUTBOX };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

// 实现
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialogEx(IDD_ABOUTBOX)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()


// CTestTool_VPRSignalwayDlg 对话框



CTestTool_VPRSignalwayDlg::CTestTool_VPRSignalwayDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_TESTTOOL_VPRSIGNALWAY_DIALOG, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CTestTool_VPRSignalwayDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CTestTool_VPRSignalwayDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BUTTON_VLPR_Init, &CTestTool_VPRSignalwayDlg::OnBnClickedButtonVlprInit)
	ON_BN_CLICKED(IDC_BUTTON_VLPR_Deinit, &CTestTool_VPRSignalwayDlg::OnBnClickedButtonVlprDeinit)
	ON_BN_CLICKED(IDC_BUTTON_VLPR_Login, &CTestTool_VPRSignalwayDlg::OnBnClickedButtonVlprLogin)
	ON_BN_CLICKED(IDC_BUTTON_VLPR_Logout, &CTestTool_VPRSignalwayDlg::OnBnClickedButtonVlprLogout)
	ON_BN_CLICKED(IDC_BUTTON_VLPR_SetResultCallBack, &CTestTool_VPRSignalwayDlg::OnBnClickedButtonVlprSetresultcallback)
	ON_BN_CLICKED(IDC_BUTTON_VLPR_SetStatusCallBack, &CTestTool_VPRSignalwayDlg::OnBnClickedButtonVlprSetstatuscallback)
	ON_BN_CLICKED(IDC_BUTTON_VLPR_GetStatus, &CTestTool_VPRSignalwayDlg::OnBnClickedButtonVlprGetstatus)
	ON_BN_CLICKED(IDC_BUTTON_VLPR_GetStatusMsg, &CTestTool_VPRSignalwayDlg::OnBnClickedButtonVlprGetstatusmsg)
	ON_BN_CLICKED(IDC_BUTTON_VLPR_GetDevInfo, &CTestTool_VPRSignalwayDlg::OnBnClickedButtonVlprGetdevinfo)
	ON_BN_CLICKED(IDC_BUTTON_VLPR_GetVersion, &CTestTool_VPRSignalwayDlg::OnBnClickedButtonVlprGetversion)
	ON_BN_CLICKED(IDC_BUTTON_VLPR_SyncTime, &CTestTool_VPRSignalwayDlg::OnBnClickedButtonVlprSynctime)
END_MESSAGE_MAP()


// CTestTool_VPRSignalwayDlg 消息处理程序

BOOL CTestTool_VPRSignalwayDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// 将“关于...”菜单项添加到系统菜单中。

	// IDM_ABOUTBOX 必须在系统命令范围内。
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// 设置此对话框的图标。  当应用程序主窗口不是对话框时，框架将自动
	//  执行此操作
	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标

	// TODO: 在此添加额外的初始化代码
	m_iHandle = 0;
	memset(g_chCurrentPath, '\0', sizeof(g_chCurrentPath));
	_getcwd(g_chCurrentPath, sizeof(g_chCurrentPath));
	strcat(g_chCurrentPath, "\\resultBuffer\\");

	MakeSureDirectoryPathExists(g_chCurrentPath);
	GetDlgItem(IDC_IPADDRESS1)->SetWindowTextA("172.18.111.76");

	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

void CTestTool_VPRSignalwayDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialogEx::OnSysCommand(nID, lParam);
	}
}

// 如果向对话框添加最小化按钮，则需要下面的代码
//  来绘制该图标。  对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。

void CTestTool_VPRSignalwayDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 用于绘制的设备上下文

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// 使图标在工作区矩形中居中
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// 绘制图标
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

//当用户拖动最小化窗口时系统调用此函数取得光标
//显示。
HCURSOR CTestTool_VPRSignalwayDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

bool CTestTool_VPRSignalwayDlg::GetItemText(int ItemID, char* buffer, size_t bufSize)
{
	CString strTemp;
	GetDlgItem(ItemID)->GetWindowText(strTemp);
	if (strTemp.GetLength() < bufSize)
	{
		//sprintf(buffer, "%s", strTemp.GetBuffer());
		sprintf_s(buffer, bufSize, "%s", strTemp.GetBuffer());
		strTemp.ReleaseBuffer();
		return true;
	}
	return false;
}

void CTestTool_VPRSignalwayDlg::ShowMsg(CEdit *pEdit, CString strMsg)
{
	if (pEdit == NULL)
	{
		return;
	}

	CTime  time = CTime::GetCurrentTime();
	CString strTmp;
	pEdit->GetWindowText(strTmp);
	if (strTmp.IsEmpty() || strTmp.GetLength() > 4096)
	{
		strTmp = time.Format(_T("[%Y-%m-%d %H:%M:%S] "));
		strTmp += strMsg;
		pEdit->SetWindowText(strTmp);
		return;
	}

	strTmp += _T("\r\n");
	strTmp += time.Format(_T("[%Y-%m-%d %H:%M:%S] "));
	strTmp += strMsg;
	pEdit->SetWindowText(strTmp);

	if (pEdit != NULL)
	{
		pEdit->LineScroll(pEdit->GetLineCount() - 1);
	}
}

void CTestTool_VPRSignalwayDlg::ShowMessage(CString strMsg)
{
	CEdit *pEdit = (CEdit*)GetDlgItem(IDC_EDIT_MSG);
	ShowMsg(pEdit, strMsg);
}



void CTestTool_VPRSignalwayDlg::OnBnClickedButtonVlprInit()
{
	// TODO:  在此添加控件通知处理程序代码
	char chLog[256] = {0};
	int iRet = VLPR_Init();
	sprintf(chLog, "VLPR_Init = %d", iRet);
	ShowMessage(chLog);
}


void CTestTool_VPRSignalwayDlg::OnBnClickedButtonVlprDeinit()
{
	// TODO:  在此添加控件通知处理程序代码
	char chLog[256] = { 0 };
	int iRet = VLPR_Deinit();
	sprintf(chLog, "VLPR_Deinit = %d", iRet);
	ShowMessage(chLog);
}


void CTestTool_VPRSignalwayDlg::OnBnClickedButtonVlprLogin()
{
	// TODO:  在此添加控件通知处理程序代码
	char chBuffer[256] = {0};
	GetItemText(IDC_IPADDRESS1, chBuffer, sizeof(chBuffer));

	char chSparas[256] = {0};
	sprintf(chSparas, "%s, 8000, admin, password", chBuffer);

	char chLog[256] = { 0 };
	m_iHandle = VLPR_Login(1, chSparas);
	sprintf(chLog, "VLPR_Login(1, %s) = %d", chSparas, m_iHandle );
	ShowMessage(chLog);
}


void CTestTool_VPRSignalwayDlg::OnBnClickedButtonVlprLogout()
{
	// TODO:  在此添加控件通知处理程序代码
	char chLog[256] = { 0 };
	int iRet = VLPR_Logout(m_iHandle);
	sprintf(chLog, "VLPR_Logout(%d) = %d", m_iHandle, iRet);
	ShowMessage(chLog);
}


void CTestTool_VPRSignalwayDlg::OnBnClickedButtonVlprSetresultcallback()
{
	// TODO:  在此添加控件通知处理程序代码
	char chLog[256] = { 0 };
	int iRet = VLPR_SetResultCallBack(m_iHandle, s_CBFun_GetFrontResult, s_CBFun_GetBackResult, this);
	sprintf(chLog, "VLPR_SetResultCallBack(%d) = %d", m_iHandle, iRet);
	ShowMessage(chLog);
}


void CTestTool_VPRSignalwayDlg::OnBnClickedButtonVlprSetstatuscallback()
{
	// TODO:  在此添加控件通知处理程序代码
	char chLog[256] = { 0 };
	int nTimeInvl = 10;
	int iRet = VLPR_SetStatusCallBack(m_iHandle, nTimeInvl, s_CBFun_GetDevStatus, this);
	sprintf(chLog, "VLPR_SetStatusCallBack(%d, %d) = %d", m_iHandle, nTimeInvl, iRet);
	ShowMessage(chLog);
}


void CTestTool_VPRSignalwayDlg::OnBnClickedButtonVlprGetstatus()
{
	// TODO:  在此添加控件通知处理程序代码
	char chLog[256] = { 0 };
	int iStatus = 0;
	int iRet = VLPR_GetStatus(m_iHandle, &iStatus);
	sprintf(chLog, "VLPR_GetStatus(%d) = %d, iStatus = %d", m_iHandle, iRet, iStatus);
	ShowMessage(chLog);
}


void CTestTool_VPRSignalwayDlg::OnBnClickedButtonVlprGetstatusmsg()
{
	// TODO:  在此添加控件通知处理程序代码
	char chBuf[256] = {0};
	GetItemText(IDC_EDIT_StaCode, chBuf, sizeof(chBuf));
	int nStatusCode = atoi(chBuf);

	char chStatusMsg[256] = {0};
	int nStatusMsgLen = sizeof(chStatusMsg);
	int iRet = VLPR_GetStatusMsg(nStatusCode, chStatusMsg, nStatusMsgLen);
	char chLog[256] = { 0 };
	sprintf(chLog, "VLPR_GetStatusMsg(%d) = %d, msg = %s", nStatusCode, iRet, chStatusMsg);
	ShowMessage(chLog);

}


void CTestTool_VPRSignalwayDlg::OnBnClickedButtonVlprGetdevinfo()
{
	// TODO:  在此添加控件通知处理程序代码

	char chCompany[256] = {0};
	int iCompanyLength = sizeof(chCompany);

	char chDevInfo[256] = {0};
	int iDevLength = sizeof(chDevInfo);

	int iRet = VLPR_GetDevInfo(
		chCompany,
		iCompanyLength,
		chDevInfo,
		iDevLength
		);

	char chLog[256] = { 0 };
	sprintf(chLog, "VLPR_GetDevInfo() = %d, chCompany = %s, devInfo = %s", iRet, chCompany, chDevInfo);
	ShowMessage(chLog);
}


void CTestTool_VPRSignalwayDlg::OnBnClickedButtonVlprGetversion()
{
	// TODO:  在此添加控件通知处理程序代码
	char chDevVersio[256] = { 0 };
	int iDevVersioLength = sizeof(chDevVersio);

	char chAPIVersion[256] = { 0 };
	int iAPIVersionLength = sizeof(chAPIVersion);

	int iRet = VLPR_GetVersion(
		chDevVersio,
		iDevVersioLength,
		chAPIVersion,
		iAPIVersionLength
		);

	char chLog[256] = { 0 };
	sprintf(chLog, "VLPR_GetVersion() = %d, DevVersio = %s, APIVersion = %s", iRet, chDevVersio, chAPIVersion);
	ShowMessage(chLog);
}


void CTestTool_VPRSignalwayDlg::OnBnClickedButtonVlprSynctime()
{
	// TODO:  在此添加控件通知处理程序代码
	char chTime[256] = {0};
	GetItemText(IDC_EDIT_TIME, chTime, sizeof(chTime));
	std::string strTime(chTime);
	if (strTime.length() != 14)
	{
		MessageBox("time format is invalid.");
		return;
	}

	int iRet = VLPR_SyncTime(m_iHandle, chTime);
	char chLog[256] = { 0 };
	sprintf(chLog, "VLPR_SyncTime(%d, %s) = %d ", m_iHandle, chTime, iRet);
	ShowMessage(chLog);
}

void CTestTool_VPRSignalwayDlg::s_CBFun_GetFrontResult(
	int nHandle,
	T_VLPFRONTINFO* pVlpResult,
	void *pUser)
{
	if (pVlpResult == NULL)
	{
		return;
	}
	CTestTool_VPRSignalwayDlg* pDlg = (CTestTool_VPRSignalwayDlg*)pUser;

	char chFileName[MAX_PATH] = { 0 };
	sprintf(chFileName, "%s%s_front.txt", g_chCurrentPath, pVlpResult->vlpTime);

	FILE* pFile = NULL;
	pFile = fopen(chFileName, "wb");
	if (pFile)
	{
		fprintf(pFile, "vlpInfoSize = %d\n", pVlpResult->vlpInfoSize);
		fprintf(pFile, "vehNo = %d\n", pVlpResult->vehNo);
		fprintf(pFile, "vlpColor = %d%d\n", pVlpResult->vlpColor[0], pVlpResult->vlpColor[1]);
		fprintf(pFile, "vlpText = %s\n", pVlpResult->vlpText);
		fprintf(pFile, "vlpTime = %s\n", pVlpResult->vlpTime);
		fprintf(pFile, "vlpReliability = %u\n", pVlpResult->vlpReliability);
		fprintf(pFile, "imageFile[0] = %s\n", pVlpResult->imageFile[0]);
		fprintf(pFile, "imageFile[1] = %s\n", pVlpResult->imageFile[1]);
		fprintf(pFile, "imageFile[2] = %s\n", pVlpResult->imageFile[2]);

		fclose(pFile);
		pFile = NULL;
	}

	char chLog[256] = { 0 };
	sprintf(chLog, "GetFrontResult(%d), NO= %s, time= %s ", 
		nHandle, 
		pVlpResult->vlpText,
		pVlpResult->vlpTime);
	if (pDlg)
	{
		pDlg->ShowMessage(chLog);
	}	
}

void CTestTool_VPRSignalwayDlg::s_CBFun_GetBackResult(int nHandle, 
	T_VLPBACKINFO* pVlpResult, 
	void *pUser)
{
	if (pVlpResult == NULL)
	{
		return;
	}
	CTestTool_VPRSignalwayDlg* pDlg = (CTestTool_VPRSignalwayDlg*)pUser;

	char chFileName[MAX_PATH] = {0};
	sprintf(chFileName, "%s%s_back.txt",g_chCurrentPath, pVlpResult->vlpBackTime);

	FILE* pFile = NULL;
	pFile = fopen(chFileName, "wb");
	if (pFile)
	{
		fprintf(pFile, "vlpInfoSize = %d\n", pVlpResult->vlpInfoSize);
		fprintf(pFile, "vehNo = %d\n", pVlpResult->vehNo);
		fprintf(pFile, "vlpBackColor = %d%d\n", pVlpResult->vlpBackColor[0], pVlpResult->vlpBackColor[1]);
		fprintf(pFile, "vlpBackText = %s\n", pVlpResult->vlpBackText);
		fprintf(pFile, "vlpBackTime = %s\n", pVlpResult->vlpBackTime);
		fprintf(pFile, "vlpCarClass = %d\n", pVlpResult->vlpCarClass);
		fprintf(pFile, "vehLength = %d\n", pVlpResult->vehLength);
		fprintf(pFile, "vehWidth = %d\n", pVlpResult->vehWidth);
		fprintf(pFile, "vehHigh = %d\n", pVlpResult->vehHigh);
		fprintf(pFile, "vehAxis = %d\n", pVlpResult->vehAxis);
		fprintf(pFile, "vlpReliability = %u\n", pVlpResult->vlpReliability);
		fprintf(pFile, "imageFile[0] = %s\n", pVlpResult->imageFile[0]);
		fprintf(pFile, "imageFile[1] = %s\n", pVlpResult->imageFile[1]);
		fprintf(pFile, "imageFile[2] = %s\n", pVlpResult->imageFile[2]);
		fprintf(pFile, "imageFile[3] = %s\n", pVlpResult->imageFile[3]);
		fprintf(pFile, "imageFile[4] = %s\n", pVlpResult->imageFile[4]);

		fclose(pFile);
		pFile = NULL;
	}

	char chLog[256] = { 0 };
	sprintf(chLog, "GetBackResult(%d), NO= %s, time= %s ",
		nHandle,
		pVlpResult->vlpBackText,
		pVlpResult->vlpBackTime);
	if (pDlg)
	{
		pDlg->ShowMessage(chLog);
	}
}

void CTestTool_VPRSignalwayDlg::s_CBFun_GetDevStatus(int nHandle, int nStatus, void* pUser)
{
	CTestTool_VPRSignalwayDlg* pDlg = (CTestTool_VPRSignalwayDlg*)pUser;

	char chLog[256] = { 0 };
	sprintf(chLog, "DevStatusCallback(%d), status = %d ", nHandle, nStatus);
	if (pDlg)
	{
		pDlg->ShowMessage(chLog);
	}
}
