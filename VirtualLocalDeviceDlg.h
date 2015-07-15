
// VirtualLocalDeviceDlg.h : ͷ�ļ�
//

#pragma once

#include "Resource.h"

// CVirtualLocalDeviceDlg �Ի���
class CVirtualLocalDeviceDlg : public CDialogEx
{
// ����
public:
	CVirtualLocalDeviceDlg(CWnd* pParent = NULL);	// ��׼���캯��

// �Ի�������
	enum { IDD = IDD_VIRTUALLOCALDEVICE_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV ֧��


// ʵ��
protected:
	HICON m_hIcon;
	CEdit m_num[32];
	CStatic m_text[32];
	// ���ɵ���Ϣӳ�亯��
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	DWORD m_ip;
	DWORD m_port;
	DWORD m_hbLen;
	BYTE m_hb[256];
	DWORD m_Cip;
	DWORD m_Cport;
	DWORD m_ChbLen;
	BYTE m_Chb[256];
	INT m_protocol;

	INT m_minVal[16];
	INT m_varR[16];
	BOOL m_bStart;
	BOOL m_bExit;
	afx_msg void OnEditChange(UINT id);
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg void OnBnClickedCancel();
	afx_msg void OnIpnFieldchangedIpaddress1(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnEnChangeEdit1();
	afx_msg void OnIpnFieldchangedIpaddress2(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnEnChangeEdit2();
	afx_msg void OnEnChangeEdit3();
	afx_msg void OnEnChangeEdit4();
	afx_msg void OnCbnSelchangeCombo1();
};

#include "ProtocolGenerateLC.h"
#include "ProtocolGenerateLS.h"

void OnLogMsg(CVirtualLocalDeviceDlg* pThis,LPWSTR msg);
#define LogMsg(msg) OnLogMsg(pThis,msg)

BOOL CheckDataLC(char* buf , int iSize , int& iSendSize , 
	sStationsConfigBuf& scData , CVirtualLocalDeviceDlg* pThis , LPWSTR str);
BOOL CheckDataLS(char* buf , int iSize , int& iSendSize , 
	sStationsConfigBuf& scData , CVirtualLocalDeviceDlg* pThis , LPWSTR str);
BOOL CheckDataSun(char* buf , int iSize , int& iSendSize , 
	sStationsConfigBuf& scData , CVirtualLocalDeviceDlg* pThis , LPWSTR str);
BOOL CheckDataDebug(char* buf , int iSize , int& iSendSize , 
	sStationsConfigBuf& scData , CVirtualLocalDeviceDlg* pThis , LPWSTR str);