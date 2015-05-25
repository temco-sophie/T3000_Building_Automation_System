// ConnectionSetup.cpp : implementation file
//

#include "stdafx.h"
#include "Modbus Poll.h"
#include "ConnectionSetup.h"
#include "afxdialogex.h"



#include "global_variable_extern.h"
#include "globle_function.h"
#include "ModbusDllforVC.h"

// CConnectionSetup dialog

IMPLEMENT_DYNAMIC(CConnectionSetup, CDialogEx)

CConnectionSetup::CConnectionSetup(CWnd* pParent /*=NULL*/)
	: CDialogEx(CConnectionSetup::IDD, pParent)
{
	m_Commication_Type=0;
}

CConnectionSetup::~CConnectionSetup()
{
}

void CConnectionSetup::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_COMBOBOX_BRADRATE, m_combox_bradrate);
	DDX_Control(pDX, IDC_COMBOBOX_CONNECTION_TYPE, m_combox_connection_type);
	DDX_Control(pDX, IDC_COMBOBOX_COMPORT, m_combox_comport);
	DDX_Control(pDX, IDC_COMBOBOX_IPADDRESS, m_combox_ipaddress);
	DDX_Control(pDX, IDC_EDIT_BETWEEN_TIME, m_edit_between_time);
	DDX_Control(pDX, IDC_EDIT_CONNECT_TIMEOUT, m_edit_connect_timeout);
	DDX_Control(pDX, IDC_EDIT_PORT, m_edit_port);
	DDX_Control(pDX, IDC_EDIT_TIMEOUT, m_edit_response_timeout);
	DDX_Control(pDX, IDC_COMBO_BRANDRATE, m_combox_brandrate);
}
void CConnectionSetup::OnInitUI(){
    Read_Config();

    CString StrTemp;
	m_combox_connection_type.ResetContent();
	m_combox_connection_type.AddString(_T("Serial Port"));
	m_combox_connection_type.AddString(_T("Modbus TCP/IP"));
	m_combox_connection_type.SetCurSel(m_Commication_Type);

	//m_combox_comport.SetWindowText(m_comport);

	m_combox_bradrate.ResetContent();
	m_combox_bradrate.AddString(_T("9600"));
	m_combox_bradrate.AddString(_T("19200"));
	
	if (m_bradrate==19200)
	{
	m_combox_bradrate.SetCurSel(1);
	} 
	else
	{
	m_combox_bradrate.SetCurSel(1);
	}
	
	GetSerialComPortNumber1(m_szComm);
	m_combox_comport.ResetContent();
	for (int i=0;i<(int)m_szComm.size();i++)
	{
		m_combox_comport.AddString(m_szComm[i]);
	}

	m_combox_comport.SetWindowText(m_comport);
	m_combox_ipaddress.SetWindowText(m_ipaddress);

	StrTemp.Format(_T("%d"),m_ipport);
	m_edit_port.SetWindowText(StrTemp);

	StrTemp.Format(_T("%d"),m_connect_timeout);
	m_edit_connect_timeout.SetWindowText(StrTemp);

	StrTemp.Format(_T("%d"),m_between_time);
	m_edit_between_time.SetWindowText(StrTemp);

	StrTemp.Format(_T("%d"),m_response_timeout);
	m_edit_response_timeout.SetWindowText(StrTemp);

	Fresh_UI();
	//从文件中填充配置

}
void CConnectionSetup::Fresh_UI(){
     
	if (m_Commication_Type==0)//串口通信
	{
		GetDlgItem(IDC_COMBOBOX_COMPORT)->EnableWindow(TRUE);
		GetDlgItem(IDC_COMBOBOX_BRADRATE)->EnableWindow(TRUE);
		GetDlgItem(IDC_COMBOBOX_IPADDRESS)->EnableWindow(FALSE);
		GetDlgItem(IDC_EDIT_PORT)->EnableWindow(FALSE);
		GetDlgItem(IDC_EDIT_CONNECT_TIMEOUT)->EnableWindow(FALSE);
	} 
	else
	{
		   GetDlgItem(IDC_COMBOBOX_COMPORT)->EnableWindow(FALSE);
		   GetDlgItem(IDC_COMBOBOX_BRADRATE)->EnableWindow(FALSE);

			
		   GetDlgItem(IDC_COMBOBOX_IPADDRESS)->EnableWindow(TRUE);
		   GetDlgItem(IDC_EDIT_PORT)->EnableWindow(TRUE);
		   GetDlgItem(IDC_EDIT_CONNECT_TIMEOUT)->EnableWindow(TRUE);		
	}
}

BOOL CConnectionSetup::OnInitDialog(){
	CDialogEx::OnInitDialog();
	 

    OnInitUI();
	return TRUE;
}

void CConnectionSetup::Read_Config(){
	CFileFind fFind;
	if(!fFind.FindFile(g_configfile_path))
	{
		WritePrivateProfileStringW(_T("MBPOLL_Setting"),_T("Connection Type"),_T("0"),g_configfile_path);

		WritePrivateProfileStringW(_T("MBPOLL_Setting"),_T("COM Port"),_T("COM1"),g_configfile_path);
		WritePrivateProfileStringW(_T("MBPOLL_Setting"),_T("COM_Port"),_T("1"),g_configfile_path);
		WritePrivateProfileStringW(_T("MBPOLL_Setting"),_T("Baudrate"),_T("19200"),g_configfile_path);


		WritePrivateProfileStringW(_T("MBPOLL_Setting"),_T("IP Address"),_T("127.0.0.1"),g_configfile_path);
		WritePrivateProfileStringW(_T("MBPOLL_Setting"),_T("IP Port"),_T("6001"),g_configfile_path);

		WritePrivateProfileStringW(_T("MBPOLL_Setting"),_T("Response Timeout"),_T("1000"),g_configfile_path);
		WritePrivateProfileStringW(_T("MBPOLL_Setting"),_T("Delay Between Time"),_T("1000"),g_configfile_path);

		WritePrivateProfileStringW(_T("MBPOLL_Setting"),_T("Connect Timeout"),_T("1000"),g_configfile_path);
	}

	m_bradrate=GetPrivateProfileInt(_T("MBPOLL_Setting"),_T("Baudrate"),19200,g_configfile_path);
	m_Commication_Type=GetPrivateProfileInt(_T("MBPOLL_Setting"),_T("Connection Type"),2,g_configfile_path);
	GetPrivateProfileString(_T("MBPOLL_Setting"),_T("COM Port"),_T("COM1"),m_comport.GetBuffer(MAX_PATH),MAX_PATH,g_configfile_path);

	m_between_time=GetPrivateProfileInt(_T("MBPOLL_Setting"),_T("Delay Between Time"),1000,g_configfile_path);
	m_response_timeout=GetPrivateProfileInt(_T("MBPOLL_Setting"),_T("Response Timeout"),1000,g_configfile_path);

	GetPrivateProfileString(_T("MBPOLL_Setting"),_T("IP Address"),_T("127.0.0.1"),m_ipaddress.GetBuffer(MAX_PATH),MAX_PATH,g_configfile_path);
	m_ipport=GetPrivateProfileInt(_T("MBPOLL_Setting"),_T("IP Port"),6001,g_configfile_path);
	m_connect_timeout=GetPrivateProfileInt(_T("MBPOLL_Setting"),_T("Connect Timeout"),1000,g_configfile_path);
	
	
}

BEGIN_MESSAGE_MAP(CConnectionSetup, CDialogEx)
	ON_CBN_SELCHANGE(IDC_COMBOBOX_CONNECTION_TYPE, &CConnectionSetup::OnCbnSelchangeComboboxConnectionType)
	ON_CBN_SELCHANGE(IDC_COMBOBOX_COMPORT, &CConnectionSetup::OnCbnSelchangeComboboxComport)
	ON_BN_CLICKED(IDOK, &CConnectionSetup::OnBnClickedOk)
END_MESSAGE_MAP()


// CConnectionSetup message handlers


void CConnectionSetup::OnCbnSelchangeComboboxConnectionType()
{
	 m_Commication_Type=m_combox_connection_type.GetCurSel();
	 Fresh_UI();
}


void CConnectionSetup::OnCbnSelchangeComboboxComport()
{

}

void CConnectionSetup::Write_Config(){
	 
	CString Connect_Type;
	Connect_Type.Format(_T("%d"),m_combox_connection_type.GetCurSel());
	CString COM_Port;
	m_combox_comport.GetWindowText(COM_Port);
	CString Num_com_port=COM_Port.Mid(3);

	CString bradrate;
	m_combox_bradrate.GetWindowText(bradrate);

	CString ipaddress;
	m_combox_ipaddress.GetWindowText(ipaddress);

	CString ipport;
	m_edit_port.GetWindowText(ipport);

	CString BT;
	m_edit_between_time.GetWindowText(BT);

	CString RT;
	m_edit_response_timeout.GetWindowText(RT);

	CString CT;
	m_edit_connect_timeout.GetWindowText(CT);

		WritePrivateProfileStringW(_T("MBPOLL_Setting"),_T("Connection Type"),Connect_Type,g_configfile_path);

		WritePrivateProfileStringW(_T("MBPOLL_Setting"),_T("COM Port"),COM_Port,g_configfile_path);

		WritePrivateProfileStringW(_T("MBPOLL_Setting"),_T("COM_Port"),Num_com_port,g_configfile_path);

		WritePrivateProfileStringW(_T("MBPOLL_Setting"),_T("Baudrate"),bradrate,g_configfile_path);


		WritePrivateProfileStringW(_T("MBPOLL_Setting"),_T("IP Address"),ipaddress,g_configfile_path);
		WritePrivateProfileStringW(_T("MBPOLL_Setting"),_T("IP Port"),ipport,g_configfile_path);

		WritePrivateProfileStringW(_T("MBPOLL_Setting"),_T("Delay Between Time"),BT,g_configfile_path);
		WritePrivateProfileStringW(_T("MBPOLL_Setting"),_T("Response Timeout"),RT,g_configfile_path);
		WritePrivateProfileStringW(_T("MBPOLL_Setting"),_T(" Connect Timeout"),CT,g_configfile_path);


		  m_bradrate=_wtoi(bradrate);
		  m_Commication_Type=_wtoi(Connect_Type);
		  m_comport=COM_Port;
		  m_ipaddress=ipaddress;
		  m_ipport=_wtoi(ipport);
		  m_connect_timeout=_wtoi(CT);
		
		 


	 
}
void CConnectionSetup::OnBnClickedOk()
{
	Write_Config();
	CDialogEx::OnOK();
}
