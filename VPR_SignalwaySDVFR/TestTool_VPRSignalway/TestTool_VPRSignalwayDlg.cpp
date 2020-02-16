
// TestTool_VPRSignalwayDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "TestTool_VPRSignalway.h"
#include "TestTool_VPRSignalwayDlg.h"
#include "afxdialogex.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// ����Ӧ�ó��򡰹��ڡ��˵���� CAboutDlg �Ի���

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// �Ի�������
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ABOUTBOX };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

// ʵ��
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


// CTestTool_VPRSignalwayDlg �Ի���



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
END_MESSAGE_MAP()


// CTestTool_VPRSignalwayDlg ��Ϣ�������

BOOL CTestTool_VPRSignalwayDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// ��������...���˵�����ӵ�ϵͳ�˵��С�

	// IDM_ABOUTBOX ������ϵͳ���Χ�ڡ�
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

	// ���ô˶Ի����ͼ�ꡣ  ��Ӧ�ó��������ڲ��ǶԻ���ʱ����ܽ��Զ�
	//  ִ�д˲���
	SetIcon(m_hIcon, TRUE);			// ���ô�ͼ��
	SetIcon(m_hIcon, FALSE);		// ����Сͼ��

	// TODO: �ڴ���Ӷ���ĳ�ʼ������

	return TRUE;  // ���ǽ��������õ��ؼ������򷵻� TRUE
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

// �����Ի��������С����ť������Ҫ����Ĵ���
//  �����Ƹ�ͼ�ꡣ  ����ʹ���ĵ�/��ͼģ�͵� MFC Ӧ�ó���
//  �⽫�ɿ���Զ���ɡ�

void CTestTool_VPRSignalwayDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // ���ڻ��Ƶ��豸������

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// ʹͼ���ڹ����������о���
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// ����ͼ��
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

//���û��϶���С������ʱϵͳ���ô˺���ȡ�ù��
//��ʾ��
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

