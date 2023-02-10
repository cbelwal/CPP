// PingtesterDlg.h : header file
//

#if !defined(AFX_PINGTESTERDLG_H__A2A1C9A7_90D6_11D4_8A69_0050DA8ECDC4__INCLUDED_)
#define AFX_PINGTESTERDLG_H__A2A1C9A7_90D6_11D4_8A69_0050DA8ECDC4__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

/////////////////////////////////////////////////////////////////////////////
// CPingtesterDlg dialog

class CPingtesterDlg : public CDialog
{
// Construction
public:
	void ConnectServer();
	void OnTimer();
	CPingtesterDlg(CWnd* pParent = NULL);	// standard constructor

// Dialog Data
	//{{AFX_DATA(CPingtesterDlg)
	enum { IDD = IDD_PINGTESTER_DIALOG };
	CString	m_filepath;
	CString	m_ipaddr;
	long	m_port;
	CString	m_msgstatus;
	CString	m_msgtime;
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CPingtesterDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	//{{AFX_MSG(CPingtesterDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg void OnTimer(UINT nIDEvent);
	afx_msg void OnStart();
	afx_msg void OnStop();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_PINGTESTERDLG_H__A2A1C9A7_90D6_11D4_8A69_0050DA8ECDC4__INCLUDED_)
