
// VirtualLocalDeviceDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "VirtualLocalDevice.h"
#include "VirtualLocalDeviceDlg.h"
#include "afxdialogex.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// 用于应用程序“关于”菜单项的 CAboutDlg 对话框

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// 对话框数据
	enum { IDD = IDD_ABOUTBOX };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

// 实现
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialogEx(CAboutDlg::IDD)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()


// CVirtualLocalDeviceDlg 对话框



CVirtualLocalDeviceDlg::CVirtualLocalDeviceDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CVirtualLocalDeviceDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CVirtualLocalDeviceDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CVirtualLocalDeviceDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_CONTROL_RANGE(EN_CHANGE , 11201,12200 , &CVirtualLocalDeviceDlg::OnEditChange)
	ON_WM_TIMER()
	ON_BN_CLICKED(IDCANCEL, &CVirtualLocalDeviceDlg::OnBnClickedCancel)
	ON_NOTIFY(IPN_FIELDCHANGED, IDC_IPADDRESS1, &CVirtualLocalDeviceDlg::OnIpnFieldchangedIpaddress1)
	ON_EN_CHANGE(IDC_EDIT1, &CVirtualLocalDeviceDlg::OnEnChangeEdit1)
	ON_NOTIFY(IPN_FIELDCHANGED, IDC_IPADDRESS2, &CVirtualLocalDeviceDlg::OnIpnFieldchangedIpaddress2)
	ON_EN_CHANGE(IDC_EDIT2, &CVirtualLocalDeviceDlg::OnEnChangeEdit2)
	ON_EN_CHANGE(IDC_EDIT3, &CVirtualLocalDeviceDlg::OnEnChangeEdit3)
	ON_EN_CHANGE(IDC_EDIT4, &CVirtualLocalDeviceDlg::OnEnChangeEdit4)
	ON_CBN_SELCHANGE(IDC_COMBO1, &CVirtualLocalDeviceDlg::OnCbnSelchangeCombo1)
END_MESSAGE_MAP()


void OnLogMsg(CVirtualLocalDeviceDlg* pThis,LPWSTR msg)
{
	CTime tm;
	tm = CTime::GetCurrentTime();
	CString str1;
	str1 = tm.Format(L"%y-%m-%d %H:%M:%S");
	str1 = str1 + L" " + msg;
	int c = ((CListBox*)pThis->GetDlgItem(IDC_LIST1))->GetCount();
	((CListBox*)pThis->GetDlgItem(IDC_LIST1))->AddString(str1);
	((CListBox*)pThis->GetDlgItem(IDC_LIST1))->SetCurSel(c);
}

// 监听并生成随机数据
UINT ListenThreadUDP(LPVOID pm)
{
	CVirtualLocalDeviceDlg* pThis = (CVirtualLocalDeviceDlg*)pm;

	sStationsConfigBuf scData;
	CTime tm;
	//tm = CTime::GetCurrentTime();
	//scData.year = tm.GetYear() - 2000;
	//scData.month = tm.GetMonth();
	//scData.day = tm.GetDay();
	//scData.hour = tm.GetHour();
	//scData.minute = tm.GetMinute();
	//scData.sid = 1;
	//scData.language = 0;
	//scData.interval = 1;
	//static INT s_minute = 1;
	//static BOOL bLenFull = TRUE;

	char buf[1024*32];
	WSADATA        wsd;
	if (WSAStartup(MAKEWORD(2,2), &wsd) != 0)
	{
		TRACE("WSAStartup failed!\n");
		return 0;
	}
	SOCKET  m_hSock;
	m_hSock = socket( AF_INET , SOCK_DGRAM , IPPROTO_UDP );
	if ( m_hSock == INVALID_SOCKET )
	{
		LogMsg(L"UDP创建套接字失败。");
		return 0;
	}
	int             iErrorRe;
	SOCKADDR_IN		sAddress;
	//// 获得主机(本机)地址,并为readdr的零位添零.
	//char  szRecipient[128];
	//gethostname( szRecipient , 128 );
	//hostent *host = gethostbyname( szRecipient );
	//if ( host != NULL )
	//{
	//	memcpy( &sAddress.sin_addr , host->h_addr_list[0] , host->h_length );
	//	memset( sAddress.sin_zero , 0 , sizeof(sAddress.sin_zero) );
	//}
	//else
	//{
	//	LogMsg(L"获取主机地址失败。");
	//	closesocket( m_hSock );
	//	m_hSock = INVALID_SOCKET;
	//	return 0;
	//}
	sAddress.sin_port = htons(4542); // 监听4542端口
	sAddress.sin_family = AF_INET;
	sAddress.sin_addr.S_un.S_addr = htonl( INADDR_ANY );
	iErrorRe = bind( m_hSock , (SOCKADDR*)&sAddress , sizeof(sAddress) );
	if( iErrorRe == SOCKET_ERROR )
	{
		LogMsg(L"UDP绑定4542端口失败。");
		sAddress.sin_port = 0; // 监听任意端口
		iErrorRe = bind( m_hSock , (SOCKADDR*)&sAddress , sizeof(sAddress) );
		if( iErrorRe == SOCKET_ERROR )
		{
			LogMsg(L"UDP绑定端口异常。");
			closesocket( m_hSock );
			m_hSock = INVALID_SOCKET;
			return 0;
		}
	}

	int             iSize;
	int             iReSize;
	int             iSendSize;
	SOCKADDR_IN     sConnectAddr;
	DWORD hb;
	// 本地IP
	sConnectAddr.sin_addr.S_un.S_un_b.s_b1 = 27;
	sConnectAddr.sin_addr.S_un.S_un_b.s_b2 = 54;
	sConnectAddr.sin_addr.S_un.S_un_b.s_b3 = 253;
	sConnectAddr.sin_addr.S_un.S_un_b.s_b4 = 9;
	sConnectAddr.sin_port = htons(45412); // 目标4541端口
	sConnectAddr.sin_family = AF_INET;
	hb = GetTickCount();
	memset( sConnectAddr.sin_zero , 0 , sizeof(sConnectAddr.sin_zero) );
	while(1)
	{
		// 设置超时
		struct timeval tv;
		fd_set readfds;
		FD_ZERO(&readfds);
		FD_SET(m_hSock,&readfds);
		tv.tv_sec=1;
		tv.tv_usec=0;
		select(m_hSock+1,&readfds,NULL,NULL,&tv);
		if(!FD_ISSET(m_hSock,&readfds))
		{
			if( (GetTickCount()-hb) > 10*1000 )
			{
				//sConnectAddr.sin_addr.S_un.S_un_b.s_b1 = 27;
				//sConnectAddr.sin_addr.S_un.S_un_b.s_b2 = 54;
				//sConnectAddr.sin_addr.S_un.S_un_b.s_b3 = 253;
				//sConnectAddr.sin_addr.S_un.S_un_b.s_b4 = 9;
				//sConnectAddr.sin_port = htons(45412); // 目标4541端口
				sConnectAddr.sin_addr.S_un.S_addr = htonl(pThis->m_ip);
				sConnectAddr.sin_port = htons((WORD)pThis->m_port); // 目标4541端口
				sConnectAddr.sin_family = AF_INET;
				iReSize = sendto( m_hSock , (const char*)pThis->m_hb , pThis->m_hbLen , 
					0 , (SOCKADDR*)&sConnectAddr , sizeof(sConnectAddr) );
				hb = GetTickCount();
				memset( sConnectAddr.sin_zero , 0 , sizeof(sConnectAddr.sin_zero) );
			}
			continue;
		}
		iSize = sizeof(sAddress);
		iReSize = recvfrom(m_hSock,buf,1024*32,0,(SOCKADDR*)&sAddress , &iSize);
		if (iReSize<0) // 未收到数据
		{
			//TRACE( "未知短数据。" );
			continue;
		}
		if( pThis->m_protocol == 0 )
		{
			if( !CheckDataLC(buf , iReSize , iSendSize , scData , pThis , L"UDP") )
				continue;
		}
		else if( pThis->m_protocol == 1 )
		{
			if( !CheckDataLS(buf , iReSize , iSendSize , scData , pThis , L"UDP") )
				continue;
		}
		else if( pThis->m_protocol == 2 )
		{
			if( !CheckDataSun(buf , iReSize , iSendSize , scData , pThis , L"UDP") )
				continue;
		}
		else
		{
			if( !CheckDataDebug(buf , iReSize , iSendSize , scData , pThis , L"UDP") )
				continue;
		}
		iReSize = sendto( m_hSock , buf , iSendSize , 
			0 , (SOCKADDR*)&sAddress , sizeof(sAddress) );
		if( iReSize == SOCKET_ERROR && WSAENOTSOCK == WSAGetLastError() )
		{
			LogMsg( L"UDP发送数据失败。" );
		}
	}

	closesocket( m_hSock );

	WSACleanup();
	return 0;
}
// 监听并生成随机数据
UINT ListenThreadTCP(LPVOID pm)
{
	CVirtualLocalDeviceDlg* pThis = (CVirtualLocalDeviceDlg*)pm;

	sStationsConfigBuf scData;
	CTime tm;
	//tm = CTime::GetCurrentTime();
	//scData.year = tm.GetYear() - 2000;
	//scData.month = tm.GetMonth();
	//scData.day = tm.GetDay();
	//scData.hour = tm.GetHour();
	//scData.minute = tm.GetMinute();
	//scData.sid = 1;
	//scData.language = 0;
	//scData.interval = 1;
	//static INT s_minute = 1;
	//static BOOL bLenFull = TRUE;

	char buf[1024*32];
	WSADATA        wsd;
	if (WSAStartup(MAKEWORD(2,2), &wsd) != 0)
	{
		TRACE("WSAStartup failed!\n");
		return 0;
	}
	int             iErrorRe;
	SOCKADDR_IN		sAddress;
	SOCKET  m_hSock;
	m_hSock = socket( AF_INET , SOCK_STREAM , IPPROTO_TCP );
	if ( m_hSock == INVALID_SOCKET )
	{
		iErrorRe = WSAGetLastError();
		LogMsg(L"TCP创建套接字失败。");
		return 0;
	}
	//// 获得主机(本机)地址,并为readdr的零位添零.
	//char  szRecipient[128];
	//gethostname( szRecipient , 128 );
	//hostent *host = gethostbyname( szRecipient );
	//if ( host != NULL )
	//{
	//	memcpy( &sAddress.sin_addr , host->h_addr_list[0] , host->h_length );
	//	memset( sAddress.sin_zero , 0 , sizeof(sAddress.sin_zero) );
	//}
	//else
	//{
	//	LogMsg(L"获取主机地址失败。");
	//	closesocket( m_hSock );
	//	m_hSock = INVALID_SOCKET;
	//	return 0;
	//}
	sAddress.sin_port = htons(4543); // 监听4543端口
	sAddress.sin_family = AF_INET;
	sAddress.sin_addr.S_un.S_addr = htonl( INADDR_ANY );
	iErrorRe = bind( m_hSock , (SOCKADDR*)&sAddress , sizeof(sAddress) );
	if( iErrorRe == SOCKET_ERROR )
	{
		LogMsg(L"TCP绑定4543端口失败。");
		sAddress.sin_port = 0; // 监听任意端口
		iErrorRe = bind( m_hSock , (SOCKADDR*)&sAddress , sizeof(sAddress) );
		if( iErrorRe == SOCKET_ERROR )
		{
			LogMsg(L"TCP绑定端口异常。");
			closesocket( m_hSock );
			m_hSock = INVALID_SOCKET;
			return 0;
		}
	}

	int             iSize;
	int             iReSize;
	int             iSendSize;
	DWORD hb;
	BOOL bConn;
	hb = GetTickCount();
	sAddress.sin_addr.S_un.S_addr = htonl(pThis->m_Cip);
	sAddress.sin_port = htons((WORD)pThis->m_Cport); // 目标4541端口
	sAddress.sin_family = AF_INET;
	memset( sAddress.sin_zero , 0 , sizeof(sAddress.sin_zero) );
	
	iErrorRe = connect( m_hSock , (SOCKADDR*)&sAddress , sizeof(SOCKADDR_IN) );
	if( iErrorRe == SOCKET_ERROR )
		bConn = FALSE;
	else
		bConn = TRUE;
	memset( sAddress.sin_zero , 0 , sizeof(sAddress.sin_zero) );
	while(1)
	{
		if( !bConn )
		{
			// 每秒连接一次
			Sleep(1000);
			sAddress.sin_addr.S_un.S_addr = htonl(pThis->m_Cip);
			sAddress.sin_port = htons((WORD)pThis->m_Cport); // 目标4541端口
			sAddress.sin_family = AF_INET;
			memset( sAddress.sin_zero , 0 , sizeof(sAddress.sin_zero) );
			iErrorRe = connect( m_hSock , (SOCKADDR*)&sAddress , sizeof(SOCKADDR_IN) );
			if( iErrorRe == SOCKET_ERROR )
			{
				iErrorRe = WSAGetLastError();
				bConn = FALSE;
				continue;
			}
			else
				bConn = TRUE;
		}
		// 设置超时
		struct timeval tv;
		fd_set readfds;
		FD_ZERO(&readfds);
		FD_SET(m_hSock,&readfds);
		tv.tv_sec=1;
		tv.tv_usec=0;
		select(m_hSock+1,&readfds,NULL,NULL,&tv);
		if(!FD_ISSET(m_hSock,&readfds))
		{
			if( (GetTickCount()-hb) > 10*1000 )
			{
				//sAddress.sin_addr.S_un.S_addr = htonl(pThis->m_Cip);
				//sAddress.sin_port = htons((WORD)pThis->m_Cport); // 目标4541端口
				//sAddress.sin_family = AF_INET;
				//memset( sAddress.sin_zero , 0 , sizeof(sAddress.sin_zero) );
				iReSize = send( m_hSock , (const char*)pThis->m_Chb , pThis->m_ChbLen , 0 );
				if( iReSize < 0 )
				{
					closesocket( m_hSock );
					sAddress.sin_port = htons(4543); // 监听4543端口
					sAddress.sin_family = AF_INET;
					sAddress.sin_addr.S_un.S_addr = htonl( INADDR_ANY );
					m_hSock = socket( AF_INET , SOCK_STREAM , IPPROTO_TCP );
					iErrorRe = bind( m_hSock , (SOCKADDR*)&sAddress , sizeof(sAddress) );
					LogMsg(L"连接异常。send1");
					bConn = FALSE;
					continue;
				}
				// 等500毫秒防止连续
				Sleep(500);
				// 等1秒写入，防止连续
				//FD_ZERO(&readfds);
				//FD_SET(m_hSock,&readfds);
				//tv.tv_sec=1;
				//tv.tv_usec=0;
				//select(m_hSock+1,NULL,&readfds,NULL,&tv);
				hb = GetTickCount();
			}
			continue;
		}
		iSize = sizeof(sAddress);
		iReSize = recv(m_hSock,buf,1024*32,0);
		if( iReSize <= 0 ) // 未收到数据
		{
			closesocket( m_hSock );
			sAddress.sin_port = htons(4543); // 监听4543端口
			sAddress.sin_family = AF_INET;
			sAddress.sin_addr.S_un.S_addr = htonl( INADDR_ANY );
			m_hSock = socket( AF_INET , SOCK_STREAM , IPPROTO_TCP );
			iErrorRe = bind( m_hSock , (SOCKADDR*)&sAddress , sizeof(sAddress) );
			LogMsg(L"连接异常。recv");
			bConn = FALSE;
			continue;
		}
		if( pThis->m_protocol == 0 )
		{
			if( !CheckDataLC(buf , iReSize , iSendSize , scData , pThis , L"TCP") )
				continue;
		}
		else if( pThis->m_protocol == 1 )
		{
			if( !CheckDataLS(buf , iReSize , iSendSize , scData , pThis , L"TCP") )
				continue;
		}
		else if( pThis->m_protocol == 2 )
		{
			if( !CheckDataSun(buf , iReSize , iSendSize , scData , pThis , L"TCP") )
				continue;
		}
		else
		{
			if( !CheckDataDebug(buf , iReSize , iSendSize , scData , pThis , L"TCP") )
				continue;
		}
		iReSize = send( m_hSock , buf , iSendSize , 0 );
		if (iReSize<0) // 未收到数据
		{
			if( WSAENOTSOCK == WSAGetLastError() )
			{
				LogMsg( L"TCP发送数据失败。" );
			}
			closesocket( m_hSock );
			sAddress.sin_port = htons(4543); // 监听4543端口
			sAddress.sin_family = AF_INET;
			sAddress.sin_addr.S_un.S_addr = htonl( INADDR_ANY );
			m_hSock = socket( AF_INET , SOCK_STREAM , IPPROTO_TCP );
			iErrorRe = bind( m_hSock , (SOCKADDR*)&sAddress , sizeof(sAddress) );
			bConn = FALSE;
			LogMsg(L"连接异常。send2");
			//continue;
		}
	}

	closesocket( m_hSock );

	WSACleanup();
	return 0;
}
// CVirtualLocalDeviceDlg 消息处理程序

CString GetAppPath()
{
	CString tstr1 , tstr2 ;
	int i;
	GetModuleFileName(theApp.m_hInstance,tstr1.GetBuffer(MAX_PATH),MAX_PATH);
	tstr2 = tstr1.GetBuffer();
	for( i = tstr2.GetLength()-1 ; i >= 0 ; i-- )
		if( tstr2[i] == _T('\\') )
		{
			tstr1 = tstr2.Left( i );
			break;
		}
		return tstr1;
}
BOOL CVirtualLocalDeviceDlg::OnInitDialog()
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

	// 设置此对话框的图标。当应用程序主窗口不是对话框时，框架将自动
	//  执行此操作
	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标

	m_bStart = FALSE;
	CString strFile = GetAppPath()+L"\\config.ini";
	CString str1,str2;
	LPWSTR pbuf = str2.GetBuffer(256);
	for( int i = 0 ; i < 16 ; i++ )
	{
		m_num[i*2].Create( WS_CHILD | WS_VISIBLE | WS_TABSTOP | WS_BORDER | ES_AUTOHSCROLL,
			CRect(100, i*25, 200, i*25+20), this, 11201 + i*2 );
		m_num[i*2].SetFont(GetFont());
		str1.Format(L"V%d_0",i);
		GetPrivateProfileString (L"VLD",str1,L"0",pbuf,255,strFile);
		m_num[i*2].SetWindowText(pbuf);
		str1.Format(L"数据%i最小值",i);
		//m_text[i*2].SetWindowText(str1);
		m_text[i*2].Create(str1.GetBuffer() , WS_VISIBLE   ,
			CRect(10, i*25, 100, i*25+20), this, 1201 + i*2 );
		m_text[i*2].SetFont(GetFont());
		m_minVal[i] = _tstoi(pbuf);

		m_num[i*2+1].Create( WS_CHILD | WS_VISIBLE | WS_TABSTOP | WS_BORDER | ES_AUTOHSCROLL,
			CRect(300, i*25, 400,  i*25+20), this, 11201 + i*2+1 );
		m_num[i*2+1].SetFont(GetFont());
		str1.Format(L"V%d_1",i);
		GetPrivateProfileString (L"VLD",str1,L"1000",pbuf,255,strFile);
		m_num[i*2+1].SetWindowText(pbuf);
		str1.Format(L"数据%i范围值",i);
		//m_text[i*2+1].SetWindowText(str1);
		m_text[i*2+1].Create(str1.GetBuffer() , WS_VISIBLE   ,
			CRect(210, i*25, 300,  i*25+20), this, 1201 + i*2+1 );
		m_text[i*2+1].SetFont(GetFont());
		m_varR[i] = _tstoi(pbuf);
	}
	GetPrivateProfileString(L"VLD",L"IP",L"2130706433",pbuf,255,strFile);
	m_ip = (DWORD)_tstoi(pbuf);
	CIPAddressCtrl* pIPCtrl = (CIPAddressCtrl*)GetDlgItem(IDC_IPADDRESS1);
	pIPCtrl->SetAddress(m_ip);

	GetPrivateProfileString(L"VLD",L"PORT",L"45412",pbuf,255,strFile);
	m_port = (DWORD)_tstoi(pbuf);
	GetDlgItem(IDC_EDIT1)->SetWindowText(pbuf);

	GetPrivateProfileString(L"VLDC",L"IP",L"2130706433",pbuf,255,strFile);
	m_Cip = (DWORD)_tstoi(pbuf);
	pIPCtrl = (CIPAddressCtrl*)GetDlgItem(IDC_IPADDRESS2);
	pIPCtrl->SetAddress(m_Cip);

	GetPrivateProfileString(L"VLDC",L"PORT",L"45413",pbuf,255,strFile);
	m_Cport = (DWORD)_tstoi(pbuf);
	GetDlgItem(IDC_EDIT2)->SetWindowText(pbuf);

	GetPrivateProfileString(L"VLD",L"HB",L"{12345678901234567890{",pbuf,255,strFile);
	str1 = pbuf;
	m_hbLen = str1.GetLength();
	WideCharToMultiByte(CP_ACP, 0, str1.GetBuffer(), -1, (LPSTR)m_hb, 256 , NULL , NULL); 
	GetDlgItem(IDC_EDIT3)->SetWindowText(str1);

	GetPrivateProfileString(L"VLDC",L"HB",L"{12345678901234567890{",pbuf,255,strFile);
	str1 = pbuf;
	m_ChbLen = str1.GetLength();
	WideCharToMultiByte(CP_ACP, 0, str1.GetBuffer(), -1, (LPSTR)m_Chb, 256 , NULL , NULL); 
	GetDlgItem(IDC_EDIT4)->SetWindowText(str1);

	CComboBox* pcb;
	GetPrivateProfileString(L"VLD",L"PROTOCOL",L"0",pbuf,255,strFile);
	m_protocol = (INT)_tstoi(pbuf);
	pcb = (CComboBox*)GetDlgItem(IDC_COMBO1);
	pcb->AddString(L"利诚自动气象站");
	pcb->AddString(L"利诚便携式采集仪");
	pcb->AddString(L"阳光采集仪");
	pcb->AddString(L"BUG测试");
	if( m_protocol >= 0 && m_protocol <= 3 )
		pcb->SetCurSel(m_protocol);
	else
		pcb->SetCurSel(0);

	m_bStart= TRUE;
	m_bExit = FALSE;
	AfxBeginThread( ListenThreadUDP , this );
	AfxBeginThread( ListenThreadTCP , this );
	SetTimer( 100 , 5000 , NULL );

	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

void CVirtualLocalDeviceDlg::OnSysCommand(UINT nID, LPARAM lParam)
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
//  来绘制该图标。对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。

void CVirtualLocalDeviceDlg::OnPaint()
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
HCURSOR CVirtualLocalDeviceDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}


// 数据更改
void CVirtualLocalDeviceDlg::OnEditChange(UINT id)
{
	if (!m_bStart)
		return;
	CString str,str2;
	GetDlgItem(id)->GetWindowText(str);
	int i;
	id -= 11201;
	CString strFile = GetAppPath()+L"\\config.ini";
	if( (id % 2) == 0 )
	{
		m_minVal[id/2] = _tstoi(str.GetBuffer());
		str.Format(L"%d",m_minVal[id/2]);
		str2.Format(L"V%d_0",id/2);
		WritePrivateProfileString(L"VLD",str2,str,strFile);
	}
	else
	{
		i = _tstoi(str.GetBuffer());
		if (i<=0)
		{
			i = 1;
		}
		str.Format(L"%d",i);
		str2.Format(L"V%d_1",id/2);
		WritePrivateProfileString(L"VLD",str2,str,strFile);
		m_varR[id/2] = i;
	}
}

void CVirtualLocalDeviceDlg::OnTimer(UINT_PTR nIDEvent)
{
	if( nIDEvent == 100 )
	{
		int c = ((CListBox*)GetDlgItem(IDC_LIST1))->GetCount();
		if( c > 1000 )
			((CListBox*)GetDlgItem(IDC_LIST1))->ResetContent();
	}

	CDialogEx::OnTimer(nIDEvent);
}


void CVirtualLocalDeviceDlg::OnBnClickedCancel()
{
	//if( AfxMessageBox(L"确认退出程序？",MB_OKCANCEL) != IDOK )
	//	return;
	m_bExit = TRUE;
	// TODO: 在此添加控件通知处理程序代码
	CDialogEx::OnCancel();
}


void CVirtualLocalDeviceDlg::OnIpnFieldchangedIpaddress1(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMIPADDRESS pIPAddr = reinterpret_cast<LPNMIPADDRESS>(pNMHDR);
	// TODO: 在此添加控件通知处理程序代码
	CIPAddressCtrl* pIPCtrl = (CIPAddressCtrl*)GetDlgItem(IDC_IPADDRESS1);
	pIPCtrl->GetAddress(m_ip);
	CString strFile = GetAppPath()+L"\\config.ini";
	CString str;
	str.Format(L"%d",m_ip);
	WritePrivateProfileString(L"VLD",L"IP",str,strFile);
	*pResult = 0;
}


void CVirtualLocalDeviceDlg::OnEnChangeEdit1()
{
	CString str,str2;
	GetDlgItem(IDC_EDIT1)->GetWindowText(str);
	m_port = _tstoi(str.GetBuffer());
	CString strFile = GetAppPath()+L"\\config.ini";
	str.Format(L"%d",m_port);
	WritePrivateProfileString(L"VLD",L"PORT",str,strFile);
}


void CVirtualLocalDeviceDlg::OnIpnFieldchangedIpaddress2(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMIPADDRESS pIPAddr = reinterpret_cast<LPNMIPADDRESS>(pNMHDR);
	// TODO: 在此添加控件通知处理程序代码
	CIPAddressCtrl* pIPCtrl = (CIPAddressCtrl*)GetDlgItem(IDC_IPADDRESS2);
	pIPCtrl->GetAddress(m_Cip);
	CString strFile = GetAppPath()+L"\\config.ini";
	CString str;
	str.Format(L"%d",m_Cip);
	WritePrivateProfileString(L"VLDC",L"IP",str,strFile);
	*pResult = 0;
}


void CVirtualLocalDeviceDlg::OnEnChangeEdit2()
{
	CString str,str2;
	GetDlgItem(IDC_EDIT2)->GetWindowText(str);
	m_Cport = _tstoi(str.GetBuffer());
	CString strFile = GetAppPath()+L"\\config.ini";
	str.Format(L"%d",m_Cport);
	WritePrivateProfileString(L"VLDC",L"PORT",str,strFile);
}


void CVirtualLocalDeviceDlg::OnEnChangeEdit3()
{
	CString str;
	GetDlgItem(IDC_EDIT3)->GetWindowText(str);
	m_hbLen = str.GetLength();
	WideCharToMultiByte(CP_ACP, 0, str.GetBuffer(), -1, (LPSTR)m_hb, 256 , NULL , NULL); 
	CString strFile = GetAppPath()+L"\\config.ini";
	WritePrivateProfileString(L"VLD",L"HB",str,strFile);
}


void CVirtualLocalDeviceDlg::OnEnChangeEdit4()
{
	CString str;
	GetDlgItem(IDC_EDIT4)->GetWindowText(str);
	m_ChbLen = str.GetLength();
	WideCharToMultiByte(CP_ACP, 0, str.GetBuffer(), -1, (LPSTR)m_Chb, 256 , NULL , NULL); 
	CString strFile = GetAppPath()+L"\\config.ini";
	WritePrivateProfileString(L"VLDC",L"HB",str,strFile);
}

// 协议选择
void CVirtualLocalDeviceDlg::OnCbnSelchangeCombo1()
{
	m_protocol = ((CComboBox*)GetDlgItem(IDC_COMBO1))->GetCurSel();
	CString strFile = GetAppPath()+L"\\config.ini";
	CString str;
	str.Format(L"%i",m_protocol);
	WritePrivateProfileString(L"VLD",L"PROTOCOL",str,strFile);
}
