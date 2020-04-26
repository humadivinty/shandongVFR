
// TestTool_VPRSignalwayDlg.h : 头文件
//

#pragma once
#include "../VPR_SignalwaySDVFR/VPR_SignalwaySDVFR_Commendef.h"


// CTestTool_VPRSignalwayDlg 对话框
class CTestTool_VPRSignalwayDlg : public CDialogEx
{
// 构造
public:
	CTestTool_VPRSignalwayDlg(CWnd* pParent = NULL);	// 标准构造函数

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_TESTTOOL_VPRSIGNALWAY_DIALOG };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支持


// 实现
protected:
	HICON m_hIcon;

	// 生成的消息映射函数
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()

public:
	bool GetItemText(int ItemID, char* buffer, size_t bufSize);
	void ShowMsg(CEdit *pEdit, CString strMsg);
	void ShowMessage(CString strMsg);
	afx_msg void OnBnClickedButtonVlprInit();
	afx_msg void OnBnClickedButtonVlprDeinit();
	afx_msg void OnBnClickedButtonVlprLogin();
	afx_msg void OnBnClickedButtonVlprLogout();
	afx_msg void OnBnClickedButtonVlprSetresultcallback();
	afx_msg void OnBnClickedButtonVlprSetstatuscallback();
	afx_msg void OnBnClickedButtonVlprGetstatus();
	afx_msg void OnBnClickedButtonVlprGetstatusmsg();
	afx_msg void OnBnClickedButtonVlprGetdevinfo();
	afx_msg void OnBnClickedButtonVlprGetversion();
	afx_msg void OnBnClickedButtonVlprSynctime();


	static  void s_CBFun_GetFrontResult(
		int nHandle,
		T_VLPFRONTINFO* pVlpResult,
		void *pUser
		);


	static void s_CBFun_GetBackResult(
		int nHandle,
		T_VLPBACKINFO* pVlpResult,
		void *pUser
		);

	static void s_CBFun_GetDevStatus(int nHandle, int nStatus, void* pUser);

private:
	int m_iHandle;
public:
	afx_msg void OnBnClickedButtonStart();
	afx_msg void OnBnClickedButtonStop();
};
