// PingtesterDlg.cpp : implementation file
//

#include "stdafx.h"
#include "Pingtester.h"
#include "PingtesterDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CAboutDlg dialog used for App About

class CAboutDlg : public CDialog
{
public:
	CAboutDlg();

// Dialog Data
	//{{AFX_DATA(CAboutDlg)
	enum { IDD = IDD_ABOUTBOX };
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CAboutDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	//{{AFX_MSG(CAboutDlg)
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
{
	//{{AFX_DATA_INIT(CAboutDlg)
	//}}AFX_DATA_INIT
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CAboutDlg)
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
	//{{AFX_MSG_MAP(CAboutDlg)
		// No message handlers
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CPingtesterDlg dialog

CPingtesterDlg::CPingtesterDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CPingtesterDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CPingtesterDlg)
	m_filepath = _T("");
	m_ipaddr = _T("");
	m_port = 0;
	m_msgstatus = _T("");
	m_msgtime = _T("");
	//}}AFX_DATA_INIT
	// Note that LoadIcon does not require a subsequent DestroyIcon in Win32
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CPingtesterDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CPingtesterDlg)
	DDX_Text(pDX, IDC_FILEPATH, m_filepath);
	DDX_Text(pDX, IDC_IPADDR, m_ipaddr);
	DDX_Text(pDX, IDC_PORT, m_port);
	DDV_MinMaxLong(pDX, m_port, 1, 65536);
	DDX_Text(pDX, IDC_MSGSTATUS, m_msgstatus);
	DDX_Text(pDX, IDC_MSGDATETIME, m_msgtime);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CPingtesterDlg, CDialog)
	//{{AFX_MSG_MAP(CPingtesterDlg)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_WM_TIMER()
	ON_BN_CLICKED(IDSTART, OnStart)
	ON_BN_CLICKED(IDC_STOP, OnStop)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CPingtesterDlg message handlers

BOOL CPingtesterDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// Add "About..." menu item to system menu.

	// IDM_ABOUTBOX must be in the system command range.
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		CString strAboutMenu;
		strAboutMenu.LoadString(IDS_ABOUTBOX);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon
	
	// TODO: Add extra initialization here
	m_filepath="c:\\NetworkPing.log";
	m_port=80;
	m_ipaddr="202.9.128.25";
    m_msgtime="Ready";
		
	m_msgstatus = "Not Connected";
	UpdateData(false);
	// Open File

	
	return TRUE;  // return TRUE  unless you set the focus to a control
	
}

void CPingtesterDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialog::OnSysCommand(nID, lParam);
	}
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CPingtesterDlg::OnPaint() 
{
	if (IsIconic())
	{
		CPaintDC dc(this); // device context for painting

		SendMessage(WM_ICONERASEBKGND, (WPARAM) dc.GetSafeHdc(), 0);

		// Center icon in client rectangle
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// Draw the icon
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialog::OnPaint();
	}
}

// The system calls this to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CPingtesterDlg::OnQueryDragIcon()
{
	return (HCURSOR) m_hIcon;
}

//DEL void CPingtesterDlg::OnOK() 
//DEL {
//DEL 	// TODO: Add extra validation here
//DEL 	MessageBox("Ok Im in, Inittailing Timer");
//DEL 	SetTimer(1,150,NULL);
//DEL }


void CPingtesterDlg::ConnectServer()
{
	/*int id = 0,printLoop=0;
	int numberChar=40;
	Ctring strDisplayText;*/
	
    KillTimer(1); // We dont want this operation to be hampered by timer
	
	int iMessageID;
	//MessageBox("Trying to create socket !");

	CSocket theSocket;
	
	// File Init
	FILE *fp;
	fp = fopen(m_filepath,"a");


	if(!theSocket.Create())
	{
		
		MessageBox("Socket Creation Error !");
		exit(0);
	}
	

	m_msgstatus="Connecting ...";
	UpdateData(false);
	
	iMessageID = theSocket.Connect(m_ipaddr,m_port);

	if (iMessageID != 0 )
	{

			
		CTime theTime = CTime::GetCurrentTime();
	    CString s = theTime.Format( "%H:%M:%S hours" );
	
	    fprintf(fp,"Successful connection at time: %s \n",s);
		
		m_msgstatus="Link or server up";
	    UpdateData(false);
	}
	
	else
	{

			// Log to File
	
		   CTime theTime = CTime::GetCurrentTime();
	       CString s = theTime.Format( "%H:%M:%S hours" );
	
		   fprintf(fp,"FAILED connection at time: %s \n",s);
		   m_msgstatus="Link or server down";
	       UpdateData(false);

	}

	theSocket.Close();
    fclose(fp); // Close File
	SetTimer(1,5000,NULL);	

}

void CPingtesterDlg::OnTimer(UINT nIDEvent) 
{
	// TODO: Add your message handler code here and/or call default
//		MessageBox("Ok buds timer called !");
	ConnectServer();
	CDialog::OnTimer(nIDEvent);
}

void CPingtesterDlg::OnStart() 
{
	// TODO: Add your control notification handler code here

 	
	// Get value of information

	UpdateData(true);

	// Get time for status / file
	
	CTime theTime = CTime::GetCurrentTime();
	CString s = theTime.Format( "%H:%M:%S hours" );
	
	m_msgtime="Started log at " + s;
    UpdateData(false);

	UpdateData(TRUE);

	// Initialize File

	m_filepath.Replace("/","//");


	FILE *fp;
	fp = fopen(m_filepath,"w");
    
	if (fp==NULL)
	{ m_msgtime = "Log file creation error";
	  m_msgstatus = "Check path, disk space";
	  UpdateData(false);
	}
	else
	{
	
	fprintf(fp,"Network Connection Log. Copyright (c) 2000 ETH Inc.\n");
	fprintf(fp,"<----START----------------------------------------- %s\n",s);
	fclose(fp);
    
	
	
	ConnectServer();
	}
}

void CPingtesterDlg::OnStop() 
{
	// TODO: Add your control notification handler code here
	KillTimer(1);
	m_msgtime = "Ready";
	m_msgstatus = "Not Connected";
	UpdateData(false);
}




