
// TestTool_VPRSignalwayDlg.h : ͷ�ļ�
//

#pragma once


// CTestTool_VPRSignalwayDlg �Ի���
class CTestTool_VPRSignalwayDlg : public CDialogEx
{
// ����
public:
	CTestTool_VPRSignalwayDlg(CWnd* pParent = NULL);	// ��׼���캯��

// �Ի�������
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_TESTTOOL_VPRSIGNALWAY_DIALOG };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV ֧��


// ʵ��
protected:
	HICON m_hIcon;

	// ���ɵ���Ϣӳ�亯��
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()

public:
	bool GetItemText(int ItemID, char* buffer, size_t bufSize);
	void ShowMsg(CEdit *pEdit, CString strMsg);
	void ShowMessage(CString strMsg);
};
