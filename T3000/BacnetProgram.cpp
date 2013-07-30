// BacnetProgram.cpp : implementation file
//

#include "stdafx.h"
#include "T3000.h"
#include "BacnetProgram.h"
#include "afxdialogex.h"

#include "afxdialogex.h"
#include "CM5/ud_str.h"
#include "Bacnet_Include.h"
#include "globle_function.h"


// CBacnetProgram dialog

IMPLEMENT_DYNAMIC(CBacnetProgram, CDialogEx)

CBacnetProgram::CBacnetProgram(CWnd* pParent /*=NULL*/)
	: CDialogEx(CBacnetProgram::IDD, pParent)
{

}

CBacnetProgram::~CBacnetProgram()
{
}

void CBacnetProgram::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST_PROGRAM, m_program_list);
}


BEGIN_MESSAGE_MAP(CBacnetProgram, CDialogEx)
	ON_MESSAGE(WM_REFRESH_BAC_PROGRAM_LIST,Fresh_Program_List)
	ON_BN_CLICKED(IDC_BUTTON_PROGRAM_READ, &CBacnetProgram::OnBnClickedButtonProgramRead)
	ON_BN_CLICKED(IDC_BUTTON_PROGRAM_APPLY, &CBacnetProgram::OnBnClickedButtonProgramApply)
	ON_MESSAGE(MY_RESUME_DATA, ProgramResumeMessageCallBack)
END_MESSAGE_MAP()


// CBacnetProgram message handlers


BOOL CBacnetProgram::OnInitDialog()
{
	CDialogEx::OnInitDialog();
	Initial_List();
	g_invoke_id = GetPrivateData(1234,READPROGRAM_T3000,0,5,sizeof(Str_program_point));
	Post_Invoke_ID_Monitor_Thread(MY_INVOKE_ID,g_invoke_id,this->m_hWnd);
	// TODO:  Add extra initialization here

	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}

void CBacnetProgram::Initial_List()
{
	m_program_list.ModifyStyle(0, LVS_SINGLESEL|LVS_REPORT|LVS_SHOWSELALWAYS);
	m_program_list.SetExtendedStyle(m_program_list.GetExtendedStyle() |LVS_EX_FULLROWSELECT |LVS_EX_GRIDLINES);

	m_program_list.InsertColumn(0, _T("Num"), 50, ListCtrlEx::Normal, LVCFMT_CENTER, ListCtrlEx::SortByDigit);
	m_program_list.InsertColumn(1, _T("Full Lable"), 150, ListCtrlEx::EditBox, LVCFMT_CENTER, ListCtrlEx::SortByString);
	m_program_list.InsertColumn(2, _T("Status"), 100, ListCtrlEx::ComboBox, LVCFMT_CENTER, ListCtrlEx::SortByString);
	m_program_list.InsertColumn(3, _T("Auto/Manual"), 100, ListCtrlEx::ComboBox, LVCFMT_CENTER, ListCtrlEx::SortByString);
	m_program_list.InsertColumn(4, _T("Size"), 80, ListCtrlEx::Normal, LVCFMT_CENTER, ListCtrlEx::SortByString);
	m_program_list.InsertColumn(5, _T("Run Status"), 100, ListCtrlEx::Normal, LVCFMT_CENTER, ListCtrlEx::SortByString);
	m_program_list.InsertColumn(6, _T("Label"), 100, ListCtrlEx::EditBox, LVCFMT_CENTER, ListCtrlEx::SortByString);
	m_pragram_dlg_hwnd = this->m_hWnd;
	g_hwnd_now = m_pragram_dlg_hwnd;
}


void CBacnetProgram::OnBnClickedButtonProgramRead()
{
	// TODO: Add your control notification handler code here
	g_invoke_id = GetPrivateData(1234,READPROGRAM_T3000,0,5,sizeof(Str_program_point));
	Post_Invoke_ID_Monitor_Thread(MY_INVOKE_ID,g_invoke_id,this->m_hWnd);
}

LRESULT CBacnetProgram::Fresh_Program_List(WPARAM wParam,LPARAM lParam)
{
	// Str_in_point Get_Str_in_Point(int index);

	m_program_list.DeleteAllItems();
	
	for (int i=0;i<(int)m_Program_data.size();i++)
	{
		CString temp_item,temp_value,temp_cal,temp_filter,temp_status,temp_lable;
		CString temp_des;
		CString temp_units;
		temp_item.Format(_T("%d"),i+1);
		m_program_list.InsertItem(i,temp_item);
		if(ListCtrlEx::ComboBox == m_program_list.GetColumnType(PROGRAM_AUTO_MANUAL))
		{
			ListCtrlEx::CStrList strlist;
			strlist.push_back(Auto_Manual[0]);
			strlist.push_back(Auto_Manual[1]);
			m_program_list.SetCellStringList(i, PROGRAM_AUTO_MANUAL, strlist);
		}

		if(ListCtrlEx::ComboBox == m_program_list.GetColumnType(PROGRAM_STATUS))
		{
			ListCtrlEx::CStrList strlist;
			strlist.push_back(ProgramStatus[0]);
			strlist.push_back(ProgramStatus[1]);
			m_program_list.SetCellStringList(i, PROGRAM_STATUS, strlist);
		}

		MultiByteToWideChar( CP_ACP, 0, (char *)m_Program_data.at(i).description, (int)strlen((char *)m_Program_data.at(i).description)+1, 
			temp_des.GetBuffer(MAX_PATH), MAX_PATH );
		temp_des.ReleaseBuffer();
		m_program_list.SetItemText(i,PROGRAM_FULL_LABLE,temp_des);


		if(m_Program_data.at(i).auto_manual==0)
			m_program_list.SetItemText(i,PROGRAM_AUTO_MANUAL,_T("Auto"));
		else
			m_program_list.SetItemText(i,PROGRAM_AUTO_MANUAL,_T("Manual"));

		if(m_Program_data.at(i).on_off==0)
			m_program_list.SetItemText(i,PROGRAM_STATUS,ProgramStatus[0]);
		else
			m_program_list.SetItemText(i,PROGRAM_STATUS,ProgramStatus[1]);

		temp_value.Format(_T("%d"),m_Program_data.at(i).bytes);
		m_program_list.SetItemText(i,PROGRAM_SIZE,temp_value);

		if(m_Program_data.at(i).com_prg==0)
			m_program_list.SetItemText(i,PROGRAM_RUN_STATUS,_T("Normal"));
		else
			m_program_list.SetItemText(i,PROGRAM_RUN_STATUS,_T("Abnormal"));


		CString temp_des2;
		MultiByteToWideChar( CP_ACP, 0, (char *)m_Program_data.at(i).label, (int)strlen((char *)m_Program_data.at(i).label)+1, 
			temp_des2.GetBuffer(MAX_PATH), MAX_PATH );
		temp_des2.ReleaseBuffer();
		m_program_list.SetItemText(i,PROGRAM_LABEL,temp_des2);

	}


	//MessageBox("1");
	return 0;
}


void CBacnetProgram::OnBnClickedButtonProgramApply()
{
	// TODO: Add your control notification handler code here
	for (int i=0;i<(int)m_Program_data.size();i++)
	{
		CString cs_temp=m_program_list.GetItemText(i,PROGRAM_FULL_LABLE);
		char cTemp[255];
		memset(cTemp,0,255);
		WideCharToMultiByte( CP_ACP, 0, cs_temp.GetBuffer(), -1, cTemp, 255, NULL, NULL );
		memcpy_s(m_Program_data.at(i).description,STR_PROGRAM_DESCRIPTION_LENGTH,cTemp,STR_PROGRAM_DESCRIPTION_LENGTH);

		cs_temp=m_program_list.GetItemText(i,PROGRAM_STATUS);
		if(cs_temp.CompareNoCase(_T("OFF"))==0)
		{
			m_Program_data.at(i).auto_manual=0;
		}
		else
		{
			m_Program_data.at(i).auto_manual=1;
		}

		cs_temp=m_program_list.GetItemText(i,PROGRAM_AUTO_MANUAL);
		if(cs_temp.CompareNoCase(_T("Auto"))==0)
		{
			m_Program_data.at(i).auto_manual=0;
		}
		else
		{
			m_Program_data.at(i).auto_manual=1;
		}

		cs_temp=m_program_list.GetItemText(i,PROGRAM_LABEL);
		char cTemp1[255];
		memset(cTemp1,0,255);
		WideCharToMultiByte( CP_ACP, 0, cs_temp.GetBuffer(), -1, cTemp1, 255, NULL, NULL );
		memcpy_s(m_Program_data.at(i).label,STR_PROGRAM_LABEL_LENGTH,cTemp1,STR_PROGRAM_LABEL_LENGTH);


	}

		g_invoke_id =WritePrivateData(1234,WRITEPROGRAM_T3000,0,(int)m_Program_data.size()-1,sizeof(Str_program_point));
	Post_Invoke_ID_Monitor_Thread(MY_INVOKE_ID,g_invoke_id,this->m_hWnd);
}

LRESULT  CBacnetProgram::ProgramResumeMessageCallBack(WPARAM wParam, LPARAM lParam)
{
	_MessageInvokeIDInfo *pInvoke =(_MessageInvokeIDInfo *)lParam;
	bool msg_result=WRITE_FAIL;
	msg_result = MKBOOL(wParam);
	if(msg_result)
	{
		SetPaneString(0,_T("Bacnet operation success!"));
		//MessageBox(_T("Bacnet operation success!"));
	}
	else
	{
		SetPaneString(0,_T("Bacnet operation fail!"));
		MessageBox(_T("Bacnet operation fail!"));
	}
	if(pInvoke)
		delete pInvoke;
	return 0;
}