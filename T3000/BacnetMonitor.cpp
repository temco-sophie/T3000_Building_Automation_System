// BacnetMonitor.cpp : implementation file
//

#include "stdafx.h"
#include "T3000.h"
#include "BacnetMonitor.h"
#include "afxdialogex.h"

#include "CM5/ud_str.h"
#include "Bacnet_Include.h"
#include "globle_function.h"
#include "gloab_define.h"

#include "BacnetGraphic.h"
#include "BacnetWait.h"
#include "BADO/BADO.h"
#define  WM_MONITOR_USER_MESSAGE WM_USER + 902
extern char *ispoint_ex(char *token,int *num_point,byte *var_type, byte *point_type, int *num_panel, int *num_net, int network,unsigned char & sub_panel, byte panel , int *netpresent);
extern char *look_label_ex(int panel,int sub_panel, int point_type, int num, int network);
CBacnetGraphic * GraphicWindow = NULL;
extern CString InputLable[15];
Str_monitor_point m_temp_monitor_data[BAC_MONITOR_COUNT];
// CBacnetMonitor dialog
static int old_monitor_line = -1;
extern int Station_NUM;
extern BacnetWait *WaitDlg;
HANDLE h_read_monitordata_thread = NULL;



IMPLEMENT_DYNAMIC(CBacnetMonitor, CDialogEx)
extern char *ispoint(char *token,int *num_point,byte *var_type, byte *point_type, int *num_panel, int *num_net, int network, byte panel, int *netpresent);
CBacnetMonitor::CBacnetMonitor(CWnd* pParent /*=NULL*/)
	: CDialogEx(CBacnetMonitor::IDD, pParent)
{

}

CBacnetMonitor::~CBacnetMonitor()
{
}

void CBacnetMonitor::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST_MONITOR, m_monitor_list);
	DDX_Control(pDX, IDC_LIST_MONITOR_INPUT, m_monitor_input_list);
	DDX_Control(pDX, IDC_DATETIMEPICKER_MONITOR, m_monitor_time_picker);
}


BEGIN_MESSAGE_MAP(CBacnetMonitor, CDialogEx)
	ON_MESSAGE(WM_HOTKEY,&CBacnetMonitor::OnHotKey)//快捷键消息映射手动加入
	ON_MESSAGE(MY_RESUME_DATA, MonitorMessageCallBack)
	ON_MESSAGE(WM_REFRESH_BAC_MONITOR_LIST,Fresh_Monitor_List)
	ON_MESSAGE(WM_REFRESH_BAC_MONITOR_INPUT_LIST,Fresh_Monitor_Input_List)
	ON_MESSAGE(WM_LIST_ITEM_CHANGED,Fresh_MCallBack_Item)
	ON_NOTIFY(NM_CLICK, IDC_LIST_MONITOR, &CBacnetMonitor::OnNMClickListMonitor)
	ON_MESSAGE(WM_LIST_MONITOR_CHANGED,Fresh_Monitor_Item)
	ON_MESSAGE(WM_LIST_MONITOR_INPUT_CHANGED,Fresh_Monitor_Input_Item)
	ON_WM_TIMER()
	ON_NOTIFY(NM_KILLFOCUS, IDC_DATETIMEPICKER_MONITOR, &CBacnetMonitor::OnNMKillfocusDatetimepickerMonitor)
	ON_BN_CLICKED(IDC_BTN_MONITOR_GRAPHIC, &CBacnetMonitor::OnBnClickedBtnMonitorGraphic)
	ON_NOTIFY(NM_SETFOCUS, IDC_LIST_MONITOR, &CBacnetMonitor::OnNMSetfocusListMonitor)
	ON_NOTIFY(NM_SETFOCUS, IDC_LIST_MONITOR_INPUT, &CBacnetMonitor::OnNMSetfocusListMonitorInput)
	ON_WM_CLOSE()
	ON_MESSAGE(WM_MONITOR_USER_MESSAGE,&CBacnetMonitor::Graphic_Window_Own_Message)


	ON_WM_HELPINFO()
END_MESSAGE_MAP()




LRESULT CBacnetMonitor::Graphic_Window_Own_Message(WPARAM wParam, LPARAM lParam)
{
	int commandtype = wParam ;
	switch(commandtype)
	{
	case MONITOR_MESSAGE_CREATE:
		//break;
		if(GraphicWindow == NULL)
		{
			GraphicWindow = new CBacnetGraphic;
			GraphicWindow->Create(IDD_DIALOG_BACNET_GRAPHIC, this);
			GraphicWindow->ShowWindow(SW_SHOW);
		}
		
		break;
	case MONITOR_MESSAGE_DELETE:

		break;
	default:
		break;
	}

	return 0;
}
// CBacnetMonitor message handlers

LRESULT  CBacnetMonitor::MonitorMessageCallBack(WPARAM wParam, LPARAM lParam)
{
	_MessageInvokeIDInfo *pInvoke =(_MessageInvokeIDInfo *)lParam;
	bool msg_result=WRITE_FAIL;
	msg_result = MKBOOL(wParam);
	CString Show_Results;
	CString temp_cs = pInvoke->task_info;
	if(msg_result)
	{
		Show_Results = temp_cs + _T("Success!");
		SetPaneString(BAC_SHOW_MISSION_RESULTS,Show_Results);
		//MessageBox(_T("Bacnet operation success!"));
	}
	else
	{
		memcpy_s(&m_monitor_data.at(pInvoke->mRow),sizeof(Str_monitor_point),&m_temp_monitor_data[pInvoke->mRow],sizeof(Str_monitor_point));//还原没有改对的值
		PostMessage(WM_REFRESH_BAC_MONITOR_LIST,pInvoke->mRow,REFRESH_ON_ITEM);
		PostMessage(WM_REFRESH_BAC_MONITOR_INPUT_LIST,pInvoke->mRow,REFRESH_ON_ITEM);
		Show_Results = temp_cs + _T("Fail!");
		SetPaneString(BAC_SHOW_MISSION_RESULTS,Show_Results);
		//AfxMessageBox(Show_Results);
		//MessageBox(_T("Bacnet operation fail!"));
	}
	if((pInvoke->mRow%2)==0)	//恢复前景和 背景 颜色;
	{
		if(pInvoke->mRow <= BAC_MONITOR_COUNT)
		m_monitor_list.SetItemBkColor(pInvoke->mRow,pInvoke->mCol,LIST_ITEM_DEFAULT_BKCOLOR,0);
		m_monitor_input_list.SetItemBkColor(pInvoke->mRow,pInvoke->mCol,LIST_ITEM_DEFAULT_BKCOLOR,0);
	}
	else
	{
		if(pInvoke->mRow <= BAC_MONITOR_COUNT)
		m_monitor_list.SetItemBkColor(pInvoke->mRow,pInvoke->mCol,LIST_ITEM_DEFAULT_BKCOLOR_GRAY,0);
		m_monitor_input_list.SetItemBkColor(pInvoke->mRow,pInvoke->mCol,LIST_ITEM_DEFAULT_BKCOLOR_GRAY,0);
	}

	m_monitor_list.RedrawItems(pInvoke->mRow,pInvoke->mRow);
	m_monitor_input_list.RedrawItems(pInvoke->mRow,pInvoke->mRow);


	if(pInvoke)
		delete pInvoke;
	return 0;
}

BOOL CBacnetMonitor::OnInitDialog()
{
	CDialogEx::OnInitDialog();
	old_monitor_line = -1;
	// TODO:  Add extra initialization here
	Initial_List();
	PostMessage(WM_REFRESH_BAC_MONITOR_LIST,NULL,NULL);
	PostMessage(WM_REFRESH_BAC_MONITOR_INPUT_LIST,NULL,NULL);

	hIcon   = AfxGetApp()->LoadIcon(IDI_ICON_REFRESH);

	SetTimer(1,BAC_LIST_REFRESH_TIME,NULL);
	
	//HRESULT hr_con = m_monitor_pCon.CreateInstance(_T("ADODB.Connection"));
	//m_monitor_pRec.CreateInstance(_T("ADODB.Recordset"));

	//HRESULT hr_rs=m_monitor_pRec.CreateInstance(_T("ADODB.Recordset"));
	//if(FAILED(hr_con))
	//{
	//	AfxMessageBox(_T("Load msado12.dll erro"));
	//	return FALSE;
	//}

	//if(FAILED(hr_rs))
	//{
	//	AfxMessageBox(_T("Load msado12.dll erro"));
	//	return FALSE;
	//}



	


//	m_monitor_pCon->Open(g_monitor_db_path.GetString(),_T(""),_T(""),adModeUnknown);

	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}


BOOL CBacnetMonitor::PreTranslateMessage(MSG* pMsg)
{
	// TODO: Add your specialized code here and/or call the base class
	if(pMsg->message == WM_KEYDOWN  )
	{
		if(pMsg->wParam == VK_RETURN)
		{

			CRect list_rect,win_rect;
			CWnd * window_focus =	GetFocus();
			if(window_focus == NULL)
				return CDialogEx::PreTranslateMessage(pMsg);
			if(GetFocus()->GetDlgCtrlID() == IDC_LIST_MONITOR)
			{
				m_monitor_list.GetWindowRect(list_rect);
				ScreenToClient(&list_rect);
				::GetWindowRect(m_monitor_dlg_hwnd,win_rect);
				m_monitor_list.Set_My_WindowRect(win_rect);
				m_monitor_list.Set_My_ListRect(list_rect);
				m_monitor_list.Get_clicked_mouse_position();
				GetDlgItem(IDC_LIST_MONITOR)->SetFocus();
			}
			else if(GetFocus()->GetDlgCtrlID() == IDC_LIST_MONITOR_INPUT)
			{
				m_monitor_input_list.GetWindowRect(list_rect);
				ScreenToClient(&list_rect);
				::GetWindowRect(m_monitor_dlg_hwnd,win_rect);
				m_monitor_input_list.Set_My_WindowRect(win_rect);
				m_monitor_input_list.Set_My_ListRect(list_rect);
				m_monitor_input_list.Get_clicked_mouse_position();

			}
			else if(GetFocus()->GetDlgCtrlID() == IDC_DATETIMEPICKER_MONITOR)
			{
				GetDlgItem(IDC_LIST_MONITOR)->SetFocus();
			}

			return 1;
		}
	}

	return CDialogEx::PreTranslateMessage(pMsg);
}


void CBacnetMonitor::Initial_List()
{
	//LVS_EX_FULLROWSELECT
	m_monitor_list.ModifyStyle(0, LVS_SINGLESEL|LVS_REPORT|LVS_SHOWSELALWAYS);
	//m_monitor_list.SetExtendedStyle(m_monitor_list.GetExtendedStyle() |LVS_EX_FULLROWSELECT |LVS_EX_GRIDLINES);
	m_monitor_list.SetExtendedStyle(m_monitor_list.GetExtendedStyle()  |LVS_EX_GRIDLINES&(~LVS_EX_FULLROWSELECT));//Not allow full row select.
	m_monitor_list.InsertColumn(MONITOR_NUM, _T("NUM"), 50, ListCtrlEx::CheckBox, LVCFMT_CENTER, ListCtrlEx::SortByDigit);
	m_monitor_list.InsertColumn(MONITOR_LABEL, _T("Label"), 150, ListCtrlEx::EditBox, LVCFMT_CENTER, ListCtrlEx::SortByString);
	m_monitor_list.InsertColumn(MONITOR_INTERVAL, _T("Interval"), 120, ListCtrlEx::Normal, LVCFMT_CENTER, ListCtrlEx::SortByString);
	m_monitor_list.InsertColumn(MONITOR_LENGTH, _T("Length"), 80, ListCtrlEx::EditBox, LVCFMT_CENTER, ListCtrlEx::SortByString);
	//m_monitor_list.InsertColumn(MONITOR_UNITS, _T("Units"), 100, ListCtrlEx::ComboBox, LVCFMT_CENTER, ListCtrlEx::SortByString);
	m_monitor_list.InsertColumn(MONITOR_STATUS, _T("Status"), 100, ListCtrlEx::Normal, LVCFMT_CENTER, ListCtrlEx::SortByString);

	m_monitor_dlg_hwnd = this->m_hWnd;
	g_hwnd_now = m_monitor_dlg_hwnd;

	m_monitor_list.DeleteAllItems();
	for (int i=0;i<(int)m_monitor_data.size();i++)
	{
		CString temp_item,temp_value,temp_cal,temp_filter,temp_status,temp_lable;
		CString temp_des;
		CString temp_units;
		temp_item.Format(_T("%d"),i+1);
		m_monitor_list.InsertItem(i,temp_item);
		m_monitor_list.SetCellEnabled(i,0,0);//禁用num，只是用来显示;
		//if(ListCtrlEx::ComboBox == m_monitor_list.GetColumnType(MONITOR_UNITS))
		//{
		//	ListCtrlEx::CStrList strlist;
		//	strlist.push_back(_T("Minutes"));
		//	strlist.push_back(_T("Hours"));
		//	strlist.push_back(_T("Days"));
		//	m_monitor_list.SetCellStringList(i, MONITOR_UNITS, strlist);
		//}

		for (int x=0;x<MONITOR_COL_NUMBER;x++)
		{
			if((i%2)==0)
				m_monitor_list.SetItemBkColor(i,x,LIST_ITEM_DEFAULT_BKCOLOR);
			else
				m_monitor_list.SetItemBkColor(i,x,LIST_ITEM_DEFAULT_BKCOLOR_GRAY);		
		}

	}
	m_monitor_list.SetCellChecked(0,0,1);//default select the first one;
	m_monitor_input_list.ModifyStyle(0, LVS_SINGLESEL|LVS_REPORT|LVS_SHOWSELALWAYS);
	//m_monitor_list.SetExtendedStyle(m_monitor_list.GetExtendedStyle() |LVS_EX_FULLROWSELECT |LVS_EX_GRIDLINES);
	m_monitor_input_list.SetExtendedStyle(m_monitor_input_list.GetExtendedStyle()  |LVS_EX_GRIDLINES&(~LVS_EX_FULLROWSELECT));//Not allow full row select.
	m_monitor_input_list.InsertColumn(MONITOR_NUM, _T("Num"), 40, ListCtrlEx::Normal, LVCFMT_CENTER, ListCtrlEx::SortByString);
	m_monitor_input_list.InsertColumn(1, _T("Input"), 90, ListCtrlEx::EditBox, LVCFMT_CENTER, ListCtrlEx::SortByString);
	m_monitor_input_list.DeleteAllItems();
	for (int i=0;i<MAX_POINTS_IN_MONITOR;i++)
	{
		CString temp_item;
		temp_item.Format(_T("%d"),i+1);
		m_monitor_input_list.InsertItem(i,temp_item);

		for (int x=0;x<2;x++)
		{
			if((i%2)==0)
				m_monitor_input_list.SetItemBkColor(i,x,LIST_ITEM_DEFAULT_BKCOLOR);
			else
				m_monitor_input_list.SetItemBkColor(i,x,LIST_ITEM_DEFAULT_BKCOLOR_GRAY);		
		}
	}
	m_monitor_input_list.SetWhetherShowBkCol(false);
	//m_monitor_list.SetItemTextColor(2,MONITOR_LABEL,RGB(0,255,0));
}
extern char *look_label(int panel, int point_type, int num, int network);
extern int pointtotext(char *buf,Point_Net *point);
LRESULT CBacnetMonitor::Fresh_Monitor_Input_List(WPARAM wParam,LPARAM lParam)
{
	int temp_input_count = 0;
	int temp_analog_count = 0;
	char q[30];

	for (int i=0;i<MAX_POINTS_IN_MONITOR;i++)
	{
		my_input_info[i].be_record =false; 
		//m_monitor_data.at(monitor_list_line).inputs[i].
		CString temp_des;
		char tempchar[50];
		char *temppoint = NULL;
		byte temp_panel = m_monitor_data.at(monitor_list_line).inputs[i].panel;
		byte temp_point_type = m_monitor_data.at(monitor_list_line).inputs[i].point_type;
		//point type 取的时候减1  ,设备里 out = 1，in = 2 ，var = 3; 取出来是 out = 0; in = 1, var = 2;
		if(temp_point_type > 0)
			temp_point_type = temp_point_type - 1;

		//byte external_high_bit = (m_monitor_data.at(monitor_list_line).inputs[i].point_type & 0xE0)>>5;

		//byte temp_number =  m_monitor_data.at(monitor_list_line).inputs[i].number + external_high_bit*256;
		//	temp_number =temp_number + 1;
		unsigned temp_network = m_monitor_data.at(monitor_list_line).inputs[i].network;
		byte lowbyte_point_type = temp_point_type & 0x1F;	//高3位用于 存放
		//if(temp_point_type > ENUM_AR_DATA)
		//{
		//	m_monitor_input_list.SetItemText(i,1,_T(""));
		//	continue;
		//}
		if(((temp_panel == 0) || (m_monitor_data.at(monitor_list_line).inputs[i].sub_panel == 0)) ||(lowbyte_point_type > ENUM_AR_DATA))
		{
			m_monitor_data.at(monitor_list_line).inputs[i].network = 0;	//发现panel 是0  就说明这个数据是无效的,先设置为初始化值;
			m_monitor_data.at(monitor_list_line).inputs[i].number = 0;
			m_monitor_data.at(monitor_list_line).inputs[i].panel = 0;
			m_monitor_data.at(monitor_list_line).inputs[i].point_type = 0;
			m_monitor_data.at(monitor_list_line).inputs[i].sub_panel = 0;
			m_monitor_input_list.SetItemText(i,1,_T(""));
			continue;
		}
		
	


		//Point_Net temp_point;
		//memcpy(&temp_point,&m_monitor_data.at(monitor_list_line).inputs[i],sizeof(Point_Net));
		//b = code;
		Point_Net temp_point;
		temp_point.network = m_monitor_data.at(monitor_list_line).inputs[i].network;
		//if(m_monitor_data.at(monitor_list_line).inputs[i].number > 0)
			temp_point.number = m_monitor_data.at(monitor_list_line).inputs[i].number ;//- 1;
		//else
		//	temp_point.number = 0;
		//if(m_monitor_data.at(monitor_list_line).inputs[i].panel > 0)
			temp_point.panel = m_monitor_data.at(monitor_list_line).inputs[i].panel;
		//else
		//	temp_point.panel = 0;
		//temp_point.point_type = m_monitor_data.at(monitor_list_line).inputs[i].point_type ;//+ 1;
		temp_point.point_type = temp_point_type ;//temp_point_type;
		temp_point.sub_panel = m_monitor_data.at(monitor_list_line).inputs[i].sub_panel;

		memset(q,0,30);
		pointtotext(q,&temp_point);
		temppoint = q;
		char buf[100];
		memset(buf,0,100);
		if((m_monitor_data.at(monitor_list_line).inputs[i].panel == Station_NUM) && (m_monitor_data.at(monitor_list_line).inputs[i].sub_panel == Station_NUM))
		{
			char *temp_label =NULL;
			byte point_type,var_type;
			int num_point,num_panel,num_net,k;
			unsigned char sub_panel = 0;
			int my_network = 0;
			temp_label = ispoint_ex(q,&num_point,&var_type, &point_type, &num_panel, &num_net, my_network,sub_panel, Station_NUM, &k);
			if(temp_label != NULL)
			{
				strcpy(buf,temp_label);
				temppoint= buf;
			}
			else
				temppoint = NULL;
		}




		//temppoint = look_label(m_monitor_data.at(monitor_list_line).inputs[i].panel,
		//					  m_monitor_data.at(monitor_list_line).inputs[i].point_type,
		//					  m_monitor_data.at(monitor_list_line).inputs[i].number,
		//					  m_monitor_data.at(monitor_list_line).inputs[i].network);
		if(temppoint == NULL)
		{
			m_monitor_input_list.SetItemText(i,1,_T(""));
			continue;
		}
		//if(strlen(temppoint) >9)
		//	memset(tempchar,0,50);
		//else
		//{
			memset(tempchar,0,50);
			memcpy_s(tempchar,50,temppoint,strlen(temppoint));
		//}
		MultiByteToWideChar( CP_ACP, 0, tempchar, (int)strlen(tempchar)+1, 
			temp_des.GetBuffer(MAX_PATH), MAX_PATH );
		temp_des.ReleaseBuffer();

		m_monitor_input_list.SetItemText(i,1,temp_des);

		my_input_info[i].be_record =true; //这一条进行了采样;

	}
	return 0;
}

Str_monitor_point temp_monitor_data_analog;
Str_monitor_point temp_monitor_data_digital;
void CBacnetMonitor::Set_Input_Range_And_count()
{
	int temp_input_count = 0;
	int temp_analog_count = 0;
	int temp_digital_count = 0;
	memset(&temp_monitor_data_analog,0,sizeof(Str_monitor_point));
	memset(&temp_monitor_data_digital,0,sizeof(Str_monitor_point));
	//ZeroMemory(&temp_monitor_data_analog,0,sizeof(Str_monitor_point));
	//ZeroMemory(&temp_monitor_data_digital,0,sizeof(Str_monitor_point));

	for (int i=0;i<MAX_POINTS_IN_MONITOR;i++)
	{
		byte temp_sub_panel = m_monitor_data.at(monitor_list_line).inputs[i].sub_panel;
		byte temp_panel = m_monitor_data.at(monitor_list_line).inputs[i].panel;
		byte temp_point_type = m_monitor_data.at(monitor_list_line).inputs[i].point_type;
		byte temp_number = 0;
		if(m_monitor_data.at(monitor_list_line).inputs[i].number > 0)
			temp_number =  m_monitor_data.at(monitor_list_line).inputs[i].number ;//- 1;

		unsigned temp_network = m_monitor_data.at(monitor_list_line).inputs[i].network;
		if((temp_panel == 0) || (temp_sub_panel == 0))
			continue;

		//如果不是，就说明是远程的点;
		if((temp_sub_panel!=Station_NUM) || (temp_panel != Station_NUM))
		{
			temp_input_count ++;
			temp_analog_count ++;
			temp_monitor_data_analog.inputs[temp_analog_count - 1].panel =  temp_panel;
			temp_monitor_data_analog.inputs[temp_analog_count - 1].point_type = temp_point_type;
			temp_monitor_data_analog.inputs[temp_analog_count - 1].number = temp_number ;
			temp_monitor_data_analog.inputs[temp_analog_count - 1].network = temp_network;
			temp_monitor_data_analog.inputs[temp_analog_count - 1].sub_panel = temp_sub_panel;
			temp_monitor_data_analog.range[temp_analog_count - 1] = 0;
			continue;
		}

		if( temp_point_type == ENUM_OUT + 1)
		{
			temp_input_count ++;
			if(m_Output_data.at(temp_number).digital_analog == 1)
			{
				temp_analog_count ++;

				temp_monitor_data_analog.inputs[temp_analog_count - 1].panel =  temp_panel;
				temp_monitor_data_analog.inputs[temp_analog_count - 1].point_type = temp_point_type;
				temp_monitor_data_analog.inputs[temp_analog_count - 1].number = temp_number ;
				temp_monitor_data_analog.inputs[temp_analog_count - 1].network = temp_network;
				temp_monitor_data_analog.inputs[temp_analog_count - 1].sub_panel = temp_sub_panel;
			}
			else
			{
				temp_digital_count ++;
				temp_monitor_data_digital.inputs[temp_digital_count - 1].panel =  temp_panel;
				temp_monitor_data_digital.inputs[temp_digital_count - 1].point_type = temp_point_type;
				temp_monitor_data_digital.inputs[temp_digital_count - 1].number = temp_number ;
				temp_monitor_data_digital.inputs[temp_digital_count - 1].network = temp_network;
				temp_monitor_data_digital.inputs[temp_digital_count - 1].sub_panel = temp_sub_panel;
			}
				temp_monitor_data_analog.range[temp_analog_count - 1] = m_Output_data.at(temp_number).range;
		}
		else if(temp_point_type == ENUM_IN + 1)
		{
			temp_input_count++;
			if(m_Input_data.at(temp_number).digital_analog == 1)
			{
				temp_analog_count ++;
				temp_monitor_data_analog.inputs[temp_analog_count - 1].panel =  temp_panel;
				temp_monitor_data_analog.inputs[temp_analog_count - 1].point_type = temp_point_type;
				temp_monitor_data_analog.inputs[temp_analog_count - 1].number = temp_number ;
				temp_monitor_data_analog.inputs[temp_analog_count - 1].network = temp_network;
				temp_monitor_data_analog.inputs[temp_analog_count - 1].sub_panel = temp_sub_panel;
			}
			else
			{
				temp_digital_count ++;
				temp_monitor_data_digital.inputs[temp_digital_count - 1].panel =  temp_panel;
				temp_monitor_data_digital.inputs[temp_digital_count - 1].point_type = temp_point_type;
				temp_monitor_data_digital.inputs[temp_digital_count - 1].number = temp_number ;
				temp_monitor_data_digital.inputs[temp_digital_count - 1].network = temp_network;
				temp_monitor_data_digital.inputs[temp_digital_count - 1].sub_panel = temp_sub_panel;
			}
			temp_monitor_data_analog.range[temp_analog_count - 1] = m_Input_data.at(temp_number).range;
		}
		else if(temp_point_type == ENUM_VAR + 1)
		{
			temp_input_count++;
			if(m_Variable_data.at(temp_number).digital_analog == 1)
			{
				temp_analog_count ++;
				temp_monitor_data_analog.inputs[temp_analog_count - 1].panel =  temp_panel;
				temp_monitor_data_analog.inputs[temp_analog_count - 1].point_type = temp_point_type;
				temp_monitor_data_analog.inputs[temp_analog_count - 1].number = temp_number ;
				temp_monitor_data_analog.inputs[temp_analog_count - 1].network = temp_network;
				temp_monitor_data_analog.inputs[temp_analog_count - 1].sub_panel = temp_sub_panel;
			}
			else
			{
				temp_digital_count ++;
				temp_monitor_data_digital.inputs[temp_digital_count - 1].panel =  temp_panel;
				temp_monitor_data_digital.inputs[temp_digital_count - 1].point_type = temp_point_type;
				temp_monitor_data_digital.inputs[temp_digital_count - 1].number = temp_number ;
				temp_monitor_data_digital.inputs[temp_digital_count - 1].network = temp_network;
				temp_monitor_data_digital.inputs[temp_digital_count - 1].sub_panel = temp_sub_panel;
			}
			temp_monitor_data_analog.range[temp_analog_count - 1] = m_Variable_data.at(temp_number).range;
		}

	}
	int test1 = sizeof(Point_Net);
	memset(m_monitor_data.at(monitor_list_line).inputs,0,MAX_POINTS_IN_MONITOR * sizeof(Point_Net));
	memcpy_s(m_monitor_data.at(monitor_list_line).inputs,temp_analog_count*sizeof(Point_Net),temp_monitor_data_analog.inputs,temp_analog_count*sizeof(Point_Net));
	memcpy_s(&m_monitor_data.at(monitor_list_line).inputs[temp_analog_count]  ,temp_digital_count * sizeof(Point_Net),&temp_monitor_data_digital.inputs[0],temp_digital_count * sizeof(Point_Net));

#if 0
	temp_input_count = 0;
	temp_analog_count = 0;

	for (int i=0;i<MAX_POINTS_IN_MONITOR;i++)
	{
		my_input_info[i].be_record =false;	//刚进来的时候全部都清空;如果满足条件就设置为已经采样;
		//m_monitor_data.at(monitor_list_line).inputs[i].
		CString temp_des;
		char tempchar[50];
		char *temppoint = NULL;
		byte temp_panel = m_monitor_data.at(monitor_list_line).inputs[i].panel;
		byte temp_point_type = m_monitor_data.at(monitor_list_line).inputs[i].point_type & 0x1F;//point_type= (point->point_type ) & 0x1F;
		byte temp_number =  m_monitor_data.at(monitor_list_line).inputs[i].number ;
		byte temp_network = m_monitor_data.at(monitor_list_line).inputs[i].network;
		byte temp_sub_panel =  m_monitor_data.at(monitor_list_line).inputs[i].sub_panel;
		if((temp_panel == 0) ||	(temp_sub_panel == 0) )
			continue;
		if((m_monitor_data.at(monitor_list_line).inputs[i].sub_panel != Station_NUM) || (m_monitor_data.at(monitor_list_line).inputs[i].panel != Station_NUM))
		{
			temp_input_count ++;
			temp_analog_count ++;
			continue;
		}

		temppoint = look_label_ex(m_monitor_data.at(monitor_list_line).inputs[i].panel,
			m_monitor_data.at(monitor_list_line).inputs[i].sub_panel,
			temp_point_type - 1,
			m_monitor_data.at(monitor_list_line).inputs[i].number + 1,
			m_monitor_data.at(monitor_list_line).inputs[i].network);
		//temppoint = look_label(m_monitor_data.at(monitor_list_line).inputs[i].panel,
		//	m_monitor_data.at(monitor_list_line).inputs[i].point_type,
		//	m_monitor_data.at(monitor_list_line).inputs[i].number,
		//	m_monitor_data.at(monitor_list_line).inputs[i].network);
		
		if(temppoint == NULL)
		{
			temp_input_count ++;
			temp_analog_count ++;
			continue;
		}
		if(strlen(temppoint) >9)
		{
			memset(tempchar,0,50);
		}
		else
		{
			memset(tempchar,0,50);
			memcpy_s(tempchar,50,temppoint,strlen(temppoint));
		}
		MultiByteToWideChar( CP_ACP, 0, tempchar, (int)strlen(tempchar)+1, 
			temp_des.GetBuffer(MAX_PATH), MAX_PATH );
		temp_des.ReleaseBuffer();

		my_input_info[i].be_record =true; //这一条进行了采样;

		if( temp_point_type == ENUM_OUT +1)
		{
			m_monitor_data.at(monitor_list_line).range[i] = m_Output_data.at(temp_number).range;
			temp_input_count ++;
			if(m_Output_data.at(temp_number).digital_analog == 1)
				temp_analog_count ++;
		}
		else if(temp_point_type == ENUM_IN +1)
		{
			m_monitor_data.at(monitor_list_line).range[i] = m_Input_data.at(temp_number).range;
			temp_input_count++;
			if(m_Input_data.at(temp_number).digital_analog == 1)
				temp_analog_count ++;
		}
		else if(temp_point_type == ENUM_VAR +1)
		{
			m_monitor_data.at(monitor_list_line).range[i] = m_Variable_data.at(temp_number).range;
			temp_input_count++;
			if(m_Variable_data.at(temp_number).digital_analog == 1)
				temp_analog_count ++;
		}

	}
	m_monitor_data.at(monitor_list_line).num_inputs = temp_input_count;
	m_monitor_data.at(monitor_list_line).an_inputs = temp_analog_count;
	CString TEST_CS;
	TEST_CS.Format(_T("Input item = %d , Analog item = %d"),temp_input_count , temp_analog_count);
	DFTrace(TEST_CS);
#endif
	return ;
}


LRESULT CBacnetMonitor::Fresh_Monitor_Input_Item(WPARAM wParam,LPARAM lParam)
{
	int Changed_Item = (int)wParam;
	int Changed_SubItem = (int)lParam;

	CString temp_task_info;
	CString New_CString =  m_monitor_input_list.GetItemText(Changed_Item,Changed_SubItem);

	char *label=NULL/*,test*/;
	byte point_type,var_type;
	int k,num_net,num_point,num_panel;
	var_type = num_point = point_type = 0;
	k=0;
	memcpy_s(&m_temp_monitor_data[monitor_list_line],sizeof(Str_monitor_point),&m_monitor_data.at(monitor_list_line),sizeof(Str_monitor_point));

	New_CString.Trim();
	if(New_CString.IsEmpty())
	{
		m_monitor_data.at(monitor_list_line).inputs[Changed_Item].network = 1;//目前不知道network 怎么处理;
		m_monitor_data.at(monitor_list_line).inputs[Changed_Item].number = 0;
		m_monitor_data.at(monitor_list_line).inputs[Changed_Item].panel = 0;
		m_monitor_data.at(monitor_list_line).inputs[Changed_Item].point_type = 0;
		Set_Input_Range_And_count();	//update the monitor structure analog and totol input number;
		int cmp_ret = memcmp(&m_temp_monitor_data[monitor_list_line],&m_monitor_data.at(monitor_list_line),sizeof(Str_monitor_point));
		if(cmp_ret!=0)
		{
			m_monitor_input_list.SetItemBkColor(Changed_Item,Changed_SubItem,LIST_ITEM_CHANGED_BKCOLOR);
			temp_task_info.Format(_T("Write Monitor List Item%d .Changed to empty "),monitor_list_line + 1);
			Post_Write_Message(g_bac_instance,WRITEMONITOR_T3000,monitor_list_line,monitor_list_line,sizeof(Str_monitor_point),m_monitor_dlg_hwnd,temp_task_info,Changed_Item,Changed_SubItem);
		}
		PostMessage(WM_REFRESH_BAC_MONITOR_INPUT_LIST,NULL,NULL);
		return 0;
	}

	char cTemp1[255];
	memset(cTemp1,0,255);
	WideCharToMultiByte( CP_ACP, 0, New_CString, -1, cTemp1, 255, NULL, NULL );

	//目前只支持在本Panel下面寻找 各个Label 和值;
	//label=ispoint(cTemp1,&num_point,&var_type,&point_type,&num_panel,&num_net,0/*my_network*/,Station_NUM,&k);
	unsigned char sub_panel_number = 0;
	 label=ispoint_ex(cTemp1,&num_point,&var_type,&point_type,&num_panel,&num_net,0,sub_panel_number,Station_NUM,&k);
	if(label!=NULL)
	{
		m_monitor_data.at(monitor_list_line).inputs[Changed_Item].network = 1;//目前不知道network 怎么处理;
		if(num_point > 0)
			num_point = num_point - 1;
		//point type 存的时候加1  ,原始 out = 0，in = 1 ，var = 2; 存进去是 out = 1; in = 2, var = 3;
		m_monitor_data.at(monitor_list_line).inputs[Changed_Item].number = num_point;
		m_monitor_data.at(monitor_list_line).inputs[Changed_Item].panel = num_panel;
		m_monitor_data.at(monitor_list_line).inputs[Changed_Item].sub_panel = sub_panel_number;
		m_monitor_data.at(monitor_list_line).inputs[Changed_Item].point_type = point_type + 1;
		char temp_input[250];
		memset(temp_input,0,250);
		strcpy(temp_input,label);

		CString temp_monitor_input;
		temp_monitor_input.Empty();
		MultiByteToWideChar( CP_ACP, 0, temp_input, (int)strlen(temp_input)+1, 
			temp_monitor_input.GetBuffer(MAX_PATH), MAX_PATH );
		temp_monitor_input.ReleaseBuffer();
		


		Set_Input_Range_And_count();	//update the monitor structure analog and totol input number;
		m_monitor_input_list.SetItemText(Changed_Item,1,temp_monitor_input);
		//这里应该是 monitor_list_line，就是要改变的多少项 而不是右边的input label的项目;
		int cmp_ret = memcmp(&m_temp_monitor_data[monitor_list_line],&m_monitor_data.at(monitor_list_line),sizeof(Str_monitor_point));
		if(cmp_ret!=0)
		{
			m_monitor_input_list.SetItemBkColor(Changed_Item,Changed_SubItem,LIST_ITEM_CHANGED_BKCOLOR);
		temp_task_info.Format(_T("Write Monitor List Item%d .Changed to \"%s\" "),monitor_list_line + 1,temp_monitor_input);
		Post_Write_Message(g_bac_instance,WRITEMONITOR_T3000,monitor_list_line,monitor_list_line,sizeof(Str_monitor_point),m_monitor_dlg_hwnd,temp_task_info,Changed_Item,Changed_SubItem);
		}
	}
	else
	{
		SetPaneString(BAC_SHOW_MISSION_RESULTS,_T("Error!Can't find the point!"));
	}
	PostMessage(WM_REFRESH_BAC_MONITOR_INPUT_LIST,NULL,NULL);
	return 0;
}
LRESULT CBacnetMonitor::Fresh_Monitor_Item(WPARAM wParam,LPARAM lParam)
{
	int cmp_ret ;//compare if match it will 0;
	int Changed_Item = (int)wParam;
	int Changed_SubItem = (int)lParam;

	CString temp_task_info;
	CString New_CString =  m_monitor_list.GetItemText(Changed_Item,Changed_SubItem);
	//先保存 原来的值，等结束的时候来比对，看是否有改变，有改变就进行写动作;
	memcpy_s(&m_temp_monitor_data[Changed_Item],sizeof(Str_monitor_point),&m_monitor_data.at(Changed_Item),sizeof(Str_monitor_point));


	if(Changed_SubItem == MONITOR_LABEL)
	{
		if(New_CString.GetLength()>= STR_MONITOR_LABEL_LENGTH)	//长度不能大于结构体定义的长度;
		{
			MessageBox(_T("Length can not greater than 8"),_T("Warning"),MB_OK | MB_ICONINFORMATION);
			PostMessage(WM_REFRESH_BAC_PROGRAM_LIST,NULL,NULL);
			return 0;
		}
		New_CString.MakeUpper();
		if(Check_Label_Exsit(New_CString))
		{
			PostMessage(WM_REFRESH_BAC_MONITOR_LIST,Changed_Item,REFRESH_ON_ITEM);
			return 0;
		}
		char cTemp1[255];
		memset(cTemp1,0,255);
		WideCharToMultiByte( CP_ACP, 0, New_CString, -1, cTemp1, 255, NULL, NULL );
		memcpy_s(m_monitor_data.at(Changed_Item).label,STR_IN_LABEL,cTemp1,STR_IN_LABEL);
	}
	else if(Changed_SubItem == MONITOR_LENGTH)
	{
		int temp_value = _wtoi(New_CString);
		if(temp_value>255)
			return 0;
		m_monitor_data.at(Changed_Item).max_time_length = temp_value;
	}
	//else if(Changed_SubItem == MONITOR_UNITS)
	//{
		//if(New_CString.CompareNoCase(_T("Days"))==0)
		//{
		//	m_monitor_data.at(Changed_Item).unit = 2;
		//}
		//else if(New_CString.CompareNoCase(_T("Hours"))==0)
		//{
		//	m_monitor_data.at(Changed_Item).unit = 1;
		//}
		//else if(New_CString.CompareNoCase(_T("Minutes"))==0)
		//{
		//	m_monitor_data.at(Changed_Item).unit = 0;
		//}
	//}

	cmp_ret = memcmp(&m_temp_monitor_data[Changed_Item],&m_monitor_data.at(Changed_Item),sizeof(Str_monitor_point));
	if(cmp_ret!=0)
	{
		m_monitor_list.SetItemBkColor(Changed_Item,Changed_SubItem,LIST_ITEM_CHANGED_BKCOLOR);
		temp_task_info.Format(_T("Write Monitor List Item%d .Changed to \"%s\" "),Changed_Item + 1,New_CString);
		Post_Write_Message(g_bac_instance,WRITEMONITOR_T3000,Changed_Item,Changed_Item,sizeof(Str_monitor_point),m_monitor_dlg_hwnd,temp_task_info,Changed_Item,Changed_SubItem);
	}
	return 0;
}
LRESULT CBacnetMonitor::Fresh_Monitor_List(WPARAM wParam,LPARAM lParam)
{
	int Fresh_Item;
	int isFreshOne = (int)lParam;
	if(isFreshOne == REFRESH_ON_ITEM)
	{
		Fresh_Item = (int)wParam;
	}
	else
	{
		if(m_monitor_list.IsDataNewer((char *)&m_monitor_data.at(0),sizeof(Str_monitor_point) * BAC_MONITOR_COUNT))
		{
			//避免list 刷新时闪烁;在没有数据变动的情况下不刷新List;
			m_monitor_list.SetListData((char *)&m_monitor_data.at(0),sizeof(Str_monitor_point) * BAC_MONITOR_COUNT);
		}
		else
		{
			return 0;
		}
	}

	for (int i=0;i<(int)m_monitor_data.size();i++)
	{
		CString temp_des;
		CString temp_time;
		CString temp_length;
		CString temp_units;
		char temp_char[50];

		if(isFreshOne)
		{
			i = Fresh_Item;
		}

		MultiByteToWideChar( CP_ACP, 0, (char *)m_monitor_data.at(i).label, (int)strlen((char *)m_monitor_data.at(i).label)+1, 
			temp_des.GetBuffer(MAX_PATH), MAX_PATH );
		temp_des.ReleaseBuffer();
		m_monitor_list.SetItemText(i,MONITOR_LABEL,temp_des);

		int temp_interval_time;
		temp_interval_time = m_monitor_data.at(i).hour_interval_time * 60 * 60 +
							 m_monitor_data.at(i).minute_interval_time * 60 +
							 m_monitor_data.at(i).second_interval_time;
		intervaltotextfull(temp_char,temp_interval_time,0,0);
		MultiByteToWideChar( CP_ACP, 0, temp_char, (int)strlen(temp_char)+1, 
			temp_time.GetBuffer(MAX_PATH), MAX_PATH );
		temp_time.ReleaseBuffer();
		m_monitor_list.SetItemText(i,MONITOR_INTERVAL,temp_time);

		temp_length.Format(_T("%d"),m_monitor_data.at(i).max_time_length);
		m_monitor_list.SetItemText(i,MONITOR_LENGTH,temp_length);

		//if(m_monitor_data.at(i).unit == 2)
		//{
		//	m_monitor_list.SetItemText(i,MONITOR_UNITS,_T("Days"));
		//}
		//else if(m_monitor_data.at(i).unit == 1)
		//{
		//	m_monitor_list.SetItemText(i,MONITOR_UNITS,_T("Hours"));
		//}
		//else if(m_monitor_data.at(i).unit == 0)
		//{
		//	m_monitor_list.SetItemText(i,MONITOR_UNITS,_T("Minutes"));
		//}

		if(m_monitor_data.at(i).status == 1)
		{
			m_monitor_list.SetItemText(i,MONITOR_STATUS,_T("ON"));
		}
		else if(m_monitor_data.at(i).status == 0)
		{
			m_monitor_list.SetItemText(i,MONITOR_STATUS,_T("OFF"));
		}

		if(isFreshOne)
		{
			break;
		}

	}
	return 0;
}
//对于一个对话框里面有多个List 只能通过确认焦点 来发送Changed 消息;
LRESULT CBacnetMonitor::Fresh_MCallBack_Item(WPARAM wParam,LPARAM lParam)
{
	//if (!::IsWindow(m_hWnd))
	//{
	//	return 0;
	//}
	CWnd * window_focus =	GetFocus();
	if(window_focus == NULL)
		return 0;
	if(GetFocus()->GetDlgCtrlID() == IDC_LIST_MONITOR)
	{
		PostMessage(WM_LIST_MONITOR_CHANGED,wParam,lParam);

	}
	else if(GetFocus()->GetDlgCtrlID() == IDC_LIST_MONITOR_INPUT)
	{
		PostMessage(WM_LIST_MONITOR_INPUT_CHANGED,wParam,lParam);

	}

	return 0;
}


void CBacnetMonitor::OnNMClickListMonitor(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	CString temp_task_info;
	// TODO: Add your control notification handler code here
	DWORD dwPos=GetMessagePos();//Get which line is click by user.Set the check box, when user enter Insert it will jump to program dialog
	CPoint point( LOWORD(dwPos), HIWORD(dwPos));
	m_monitor_list.ScreenToClient(&point);
	LVHITTESTINFO lvinfo;
	lvinfo.pt=point;
	lvinfo.flags=LVHT_ABOVE;
	m_monitor_list.Set_Edit(true);
	int nItem=m_monitor_list.SubItemHitTest(&lvinfo);
	if(nItem!=-1)
	{
		m_monitor_list.SetCellChecked(nItem,0,1); 
		monitor_list_line = nItem;	//记录点击的是第几项;
		if(old_monitor_line != monitor_list_line) //避免反复刷新右边的 INPUT list;
		{
			PostMessage(WM_REFRESH_BAC_MONITOR_INPUT_LIST,NULL,NULL);
			old_monitor_line = monitor_list_line;
		}
		
		for (int i=0;i<m_monitor_list.GetItemCount();++i)
		{
			if(i == nItem)
				continue;
			m_monitor_list.SetCellChecked(i,0,FALSE);
		}
	}
	long lRow,lCol;
	lRow = lvinfo.iItem;
	lCol = lvinfo.iSubItem;
	if(lRow>m_monitor_list.GetItemCount()) //如果点击区超过最大行号，则点击是无效的
		return;
	if(lRow<0)
		return;

	memcpy_s(&m_temp_monitor_data[lRow],sizeof(Str_monitor_point),&m_monitor_data.at(lRow),sizeof(Str_monitor_point));

	if(lCol == MONITOR_STATUS)
	{
		m_monitor_list.Set_Edit(false);
		if(m_monitor_data.at(lRow).status == 0)
		{
			m_monitor_data.at(lRow).status = 1;
			m_monitor_list.SetItemText(lRow,MONITOR_STATUS,_T("ON"));
			temp_task_info.Format(_T("Write Monitor List Item%d .Changed to \"ON\" "),lRow + 1);
		}
		else
		{
			m_monitor_data.at(lRow).status = 0;
			m_monitor_list.SetItemText(lRow,MONITOR_STATUS,_T("OFF"));
			temp_task_info.Format(_T("Write Monitor List Item%d .Changed to \"OFF\" "),lRow + 1);
		}
		int cmp_ret = memcmp(&m_temp_monitor_data[lRow],&m_monitor_data.at(lRow),sizeof(Str_monitor_point));
		if(cmp_ret!=0)
		{
			m_monitor_list.SetItemBkColor(lRow,lCol,LIST_ITEM_CHANGED_BKCOLOR);
			Post_Write_Message(g_bac_instance,WRITEMONITOR_T3000,lRow,lRow,sizeof(Str_monitor_point),m_monitor_dlg_hwnd,temp_task_info,lRow,lCol);
		}
	}
	else if(lCol == MONITOR_INTERVAL)
	{
		m_row = lRow;
		m_col = lCol;

		m_monitor_list.Set_Edit(false);
		m_monitor_time_picker.ShowWindow(SW_SHOW);
		CRect list_rect,win_rect;
		m_monitor_list.GetWindowRect(list_rect);
		GetWindowRect(win_rect);
		CRect myrect;
		m_monitor_list.GetSubItemRect(lRow,lCol,LVIR_BOUNDS,myrect);


		myrect.left = myrect.left + list_rect.left - win_rect.left + 1;
		myrect.right = myrect.right + list_rect.left - win_rect.left + 1;
		myrect.top = myrect.top + 10;
		myrect.bottom = myrect.bottom + 12;

		myrect.top = myrect.top + 20;
		myrect.bottom = myrect.bottom + 20;
		m_monitor_time_picker.BringWindowToTop();
		m_monitor_time_picker.MoveWindow(myrect);


		CString Temp_CString =  m_monitor_list.GetItemText(lRow,lCol);

		CStringArray TEMPCS;
		int temp_hour,temp_minute,temp_second;
		SplitCStringA(TEMPCS, Temp_CString, _T(":"));

		if((int)TEMPCS.GetSize() <3)
		{
			temp_hour = 0;
			temp_minute = 0;
			temp_second = 0;
			CTime TimeTemp(2013,1,1,temp_hour,temp_minute,0);
			m_monitor_time_picker.SetFormat(_T("HH:mm"));
			m_monitor_time_picker.SetTime(&TimeTemp);
			m_monitor_time_picker.SetFocus();
		}
		else
		{
			temp_hour = _wtoi(TEMPCS.GetAt(0));
			temp_minute = _wtoi(TEMPCS.GetAt(1));
			temp_second = _wtoi(TEMPCS.GetAt(2));
			if(temp_hour >=24)
				temp_hour = 0;
			if(temp_minute >=60)
				temp_minute = 0;
			if(temp_second >=60)
				temp_second = 15;
			CTime TimeTemp(2013,1,1,temp_hour,temp_minute,temp_second);
			m_monitor_time_picker.SetFormat(_T("HH:mm:ss"));
			m_monitor_time_picker.SetTime(&TimeTemp);
			m_monitor_time_picker.SetFocus();
		}
		m_monitor_list.SetItemText(lRow,lCol,_T(""));
		m_monitor_time_picker.Invalidate();
		SetTimer(2,100,NULL);
	}


	
	*pResult = 0;
}


void CBacnetMonitor::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: Add your message handler code here and/or call default
	switch(nIDEvent)
	{
	case 1:

		if(this->IsWindowVisible() && (bac_select_device_online))
		{
		PostMessage(WM_REFRESH_BAC_MONITOR_LIST,NULL,NULL);
		PostMessage(WM_REFRESH_BAC_MONITOR_INPUT_LIST,NULL,NULL);
		Post_Refresh_Message(g_bac_instance,READMONITOR_T3000,0,BAC_MONITOR_COUNT - 1,sizeof(Str_monitor_point),BAC_MONITOR_GROUP);
		}
		break;
	case 2:
		KillTimer(2);
		m_monitor_time_picker.Invalidate();
		break;
	}
	CDialogEx::OnTimer(nIDEvent);
}


void CBacnetMonitor::OnNMKillfocusDatetimepickerMonitor(NMHDR *pNMHDR, LRESULT *pResult)
{
	// TODO: Add your control notification handler code here
	*pResult = 0;

	memcpy_s(&m_temp_monitor_data[m_row],sizeof(Str_monitor_point),&m_monitor_data.at(m_row),sizeof(Str_monitor_point));

	CTime temp_time;CString temp_cs;
	int chour,cmin,csecond;
	m_monitor_time_picker.GetTime(temp_time);
	chour = temp_time.GetHour();
	cmin = temp_time.GetMinute();
	csecond = temp_time.GetSecond();


	if((chour == 0) &&(cmin	==0) &&(csecond == 0))
	{
		temp_cs.Empty();
	}
	else
		temp_cs.Format(_T("%02d:%02d:%02d"),chour,cmin,csecond);
	m_monitor_list.SetItemText(m_row,m_col,temp_cs);

	m_monitor_time_picker.ShowWindow(SW_HIDE);

	m_monitor_data.at(m_row).hour_interval_time = chour;
	m_monitor_data.at(m_row).minute_interval_time = cmin;
	m_monitor_data.at(m_row).second_interval_time = csecond;


	CString temp_task_info;

	int cmp_ret = memcmp(&m_temp_monitor_data[m_row],&m_monitor_data.at(m_row),sizeof(Str_monitor_point));
	if(cmp_ret!=0)
	{
		m_monitor_list.SetItemBkColor(m_row,m_col,LIST_ITEM_CHANGED_BKCOLOR);
	temp_task_info.Format(_T("Write Monitor Interval Time Item%d .Changed Time to \"%s\" "),m_row + 1,temp_cs);
	Post_Write_Message(g_bac_instance,WRITEMONITOR_T3000,m_row,m_row,sizeof(Str_monitor_point),m_monitor_dlg_hwnd,temp_task_info,m_row,m_col);
	}

}


void CBacnetMonitor::OnBnClickedBtnMonitorGraphic()
{
	// TODO: Add your control notification handler code here

	for (int i=0;i<14;i++)
	{
		InputLable[i] =	m_monitor_input_list.GetItemText(i,1);	
	}
	if(WaitDlg==NULL)
	{
		WaitDlg = new BacnetWait((int)BAC_WAIT_READ_MONITOR_DATA);

		WaitDlg->Create(IDD_DIALOG_BACNET_WAIT,this);
		WaitDlg->ShowWindow(SW_SHOW);

		RECT RECT_SET1;
		::GetClientRect(BacNet_hwd,&RECT_SET1);
		ClientToScreen(&RECT_SET1);
		WaitDlg->MoveWindow(RECT_SET1.left + 50,RECT_SET1.bottom - 30,800,20);
	}

	if(h_read_monitordata_thread==NULL)
	{
		h_read_monitordata_thread =CreateThread(NULL,NULL,Readmonitorthreadfun,this,NULL, NULL);
	}

}

bool read_monitordata(int digtal_or_analog)
{
	timestart = time(NULL);
	MonitorUpdateData temp;
	memset(&temp,0,sizeof(MonitorUpdateData));
	//temp.nsize = 10000;
	//timesec1970 = (unsigned long)time(NULL);
	//temp.oldest_time =timesec1970 - 43200;
	//temp.most_recent_time = timestart + 43200;


	//unsigned long nTime = temp.oldest_time;   //nTime类型为unsigned long
	//unsigned long nTime1 = temp.most_recent_time;  //nTime类型为unsigned long
	//CTime objTime(nTime);   //CTime类型变量直接获取时间值
	//CTime objTime1(nTime1);   //CTime类型变量直接获取时间值
	////string strTime;    //格式化string变量
	////strTime.Format("d-d-d d:d:d", objTime.GetYear(), objTime.GetMonth(), objTime.GetDay(),objTime.GetHour(), objTime.GetMinute(), objTime.GetSecond());

	//g_Print.Format(_T("Read Start Time %d-%d-%d %d:%d:%d   %d"), objTime.GetYear(), objTime.GetMonth(), objTime.GetDay(),objTime.GetHour(), objTime.GetMinute(), objTime.GetSecond(),temp.oldest_time);
	//DFTrace(g_Print);
	//g_Print.Format(_T("Read End Time %d-%d-%d %d:%d:%d   %d"), objTime1.GetYear(), objTime1.GetMonth(), objTime1.GetDay(),objTime1.GetHour(), objTime1.GetMinute(), objTime1.GetSecond(),temp.most_recent_time);
	//DFTrace(g_Print);
	int ret = -2;
	int send_count = 0;
	//int time_count = 0;//用于动态计时只要有回复继续下一包;.
	int recieve_flag = 0;
	CString cs_my_temp;
	cs_my_temp = _T("");
	if(WaitDlg)
	{
		WaitDlg->Set_Show_String(cs_my_temp);
		WaitDlg->Set_Pos(0);
	}
	else
		return false;

	m_monitor_head.total_seg = 0;
	m_monitor_head.seg_index = 0;
	for (int i=0;i<5;i++)
	{
		recieve_flag = false;
		ret = GetMonitorBlockData(g_bac_instance,READMONITORDATA_T3000,monitor_list_line,digtal_or_analog,0,0,&temp);	
		if(ret < 0)
		{
			Sleep(SEND_COMMAND_DELAY_TIME);
			continue;
		}
		for(int time_loop=0;time_loop<2000;time_loop++)
		{
			Sleep(1);
			if(tsm_invoke_id_free(ret))
			{
				recieve_flag = true;
				break;
			}
		} 
		if(recieve_flag)
		{
			break;
		}
		else if((recieve_flag == false) &&( i == 5))
		{
			return false;
		}
	}

	CString temp_db_ini_folder;
	temp_db_ini_folder = g_achive_folder + _T("\\MonitorIndex.ini");
	CString temp_serial;
	CString temp_monitor_index;
	if(digtal_or_analog == BAC_UNITS_ANALOG)
		temp_monitor_index.Format(_T("Index_%d"),monitor_list_line);
	else
		temp_monitor_index.Format(_T("Digital_Index_%d"),monitor_list_line);

	temp_serial.Format(_T("%u"),g_selected_serialnumber);
	int temp_index = GetPrivateProfileInt(temp_serial,temp_monitor_index,-1,temp_db_ini_folder) + 1;


	for (int read_index= temp_index ;read_index<m_monitor_head.total_seg;read_index++)
	{
		cs_my_temp.Format(_T("%d / %d"),read_index - temp_index,m_monitor_head.total_seg - temp_index);
		WaitDlg->Set_Show_String(cs_my_temp);
		int pos = ((read_index - temp_index) * 100 ) / (m_monitor_head.total_seg - temp_index);
		if(pos > 100)
			pos = 100;
		WaitDlg->Set_Pos(pos);
		for (int i=0;i<5;i++)
		{
			recieve_flag = false;
			ret = GetMonitorBlockData(g_bac_instance,READMONITORDATA_T3000,monitor_list_line,digtal_or_analog,m_monitor_head.total_seg,read_index,&temp);	
			if(ret < 0)
			{
				Sleep(SEND_COMMAND_DELAY_TIME);
				continue;
			}
			for(int time_loop=0;time_loop<1500;time_loop++)
			{
				Sleep(1);
				if(tsm_invoke_id_free(ret))
				{
					recieve_flag = true;
					break;
				}
			} 
			if(recieve_flag)
			{
				break;
			}
			else if((recieve_flag == false) &&( i == 5))
			{
				Sleep(1);
				return false;
				//goto readendthread;
			}
		}
		if(recieve_flag)
		{
			continue;
		}
		else
		{
			return false;
		}

	}

	return true;
}



DWORD WINAPI  CBacnetMonitor::Readmonitorthreadfun(LPVOID lpVoid)
{
	//Write_Config_Info
	CBacnetMonitor *pParent = (CBacnetMonitor *)lpVoid;

	if(read_monitordata(BAC_UNITS_ANALOG))
	{
		if(read_monitordata(BAC_UNITS_DIGITAL))
			pParent->PostMessage(WM_MONITOR_USER_MESSAGE,MONITOR_MESSAGE_CREATE,0);
	}
readendthread:
	
	if(WaitDlg)
	{
		WaitDlg->Set_Pos(100);
		WaitDlg = NULL;
	}
	h_read_monitordata_thread = NULL;
	return 0;


}





void CBacnetMonitor::OnNMSetfocusListMonitor(NMHDR *pNMHDR, LRESULT *pResult)
{
	// TODO: Add your control notification handler code here
	//Fance
	//两个list 要只有一个 的select 处于显示状态;
	int my_raw=0;
	int my_col=0;
	m_monitor_input_list.Get_Selected_Item(my_raw,my_col);

	m_monitor_list.SetWhetherShowBkCol(true);
	m_monitor_input_list.SetWhetherShowBkCol(false);

	if((my_raw%2)==0)
		m_monitor_input_list.SetItemBkColor(my_raw,my_col,LIST_ITEM_DEFAULT_BKCOLOR,0);
	else
		m_monitor_input_list.SetItemBkColor(my_raw,my_col,LIST_ITEM_DEFAULT_BKCOLOR_GRAY,0);	
	m_monitor_input_list.RedrawItems(my_raw,my_raw);

	m_monitor_list.Get_Selected_Item(my_raw,my_col);//重新在获取那个是select
	m_monitor_list.SetItemBkColor(my_raw,my_col,LIST_ITEM_SELECTED,0);
	m_monitor_list.RedrawItems(my_raw,my_raw);



	*pResult = 0;
}


void CBacnetMonitor::OnNMSetfocusListMonitorInput(NMHDR *pNMHDR, LRESULT *pResult)
{
	// TODO: Add your control notification handler code here

	int my_raw=0;
	int my_col=0;
	m_monitor_list.SetWhetherShowBkCol(false);
	m_monitor_input_list.SetWhetherShowBkCol(true);
	m_monitor_list.Get_Selected_Item(my_raw,my_col);
	TRACE(_T("Kill focus monitor\r\n"));
	if((my_raw%2)==0)
		m_monitor_list.SetItemBkColor(my_raw,my_col,LIST_ITEM_DEFAULT_BKCOLOR,1);
	else
		m_monitor_list.SetItemBkColor(my_raw,my_col,LIST_ITEM_DEFAULT_BKCOLOR_GRAY,1);	

	m_monitor_list.RedrawItems(my_raw,my_raw);

	m_monitor_input_list.Get_Selected_Item(my_raw,my_col);//重新在获取那个是select
	m_monitor_input_list.SetItemBkColor(my_raw,my_col,LIST_ITEM_SELECTED,0);
	m_monitor_input_list.RedrawItems(my_raw,my_raw);


	*pResult = 0;
}

void CBacnetMonitor::OnClose()
{
	// TODO: Add your message handler code here and/or call default
	KillTimer(1);
	m_monitor_dlg_hwnd = NULL;
	::PostMessage(BacNet_hwd,WM_DELETE_NEW_MESSAGE_DLG,DELETE_WINDOW_MSG,0);
	CDialogEx::OnClose();
}

void CBacnetMonitor::OnCancel()
{
	// TODO: Add your specialized code here and/or call the base class
//	KillTimer(1);
//	m_monitor_dlg_hwnd = NULL;
	::PostMessage(BacNet_hwd,WM_DELETE_NEW_MESSAGE_DLG,DELETE_WINDOW_MSG,0);
//	CDialogEx::OnCancel();
}

void CBacnetMonitor::Reg_Hotkey()
{
	RegisterHotKey(GetSafeHwnd(),KEY_INSERT,NULL,VK_INSERT);//Insert键
}

void CBacnetMonitor::Unreg_Hotkey()
{
	UnregisterHotKey(GetSafeHwnd(),KEY_INSERT);
}

//This function code by Fance du for receive the monitor data;
int handle_read_monitordata_ex(char *npoint,int nlength)
{
	char * my_temp_point = npoint;
	char * temp_print = my_temp_point;


	temp_print = my_temp_point;

	//CString n_temp_print;
	//n_temp_print.Format(_T("Monitor Rx (%03d): "),nlength);
	//CString temp_char;
	//for (int i = 0; i< nlength ; i++)
	//{
	//	temp_char.Format(_T("%02x"),(unsigned char)*temp_print);
	//	temp_char.MakeUpper();
	//	temp_print ++;
	//	n_temp_print = n_temp_print + temp_char + _T(" ");
	//}
	//DFTrace(n_temp_print);


	bool analog_data = true;//1为Analog;


	m_monitor_head.total_seg =  (unsigned char)my_temp_point[1]<<8 | (unsigned char)my_temp_point[0];
	my_temp_point = my_temp_point + 2;
	m_monitor_head.command = *(my_temp_point++);
	m_monitor_head.index = *(my_temp_point++);
	m_monitor_head.type = *(my_temp_point++);
	m_monitor_head.conm_args.nsize = ((unsigned char)my_temp_point[0])<<24 | ((unsigned char)my_temp_point[1]<<16) | ((unsigned char)my_temp_point[2])<<8 | ((unsigned char)my_temp_point[3]);
	my_temp_point = my_temp_point + 4;
	m_monitor_head.conm_args.oldest_time = ((unsigned char)my_temp_point[0])<<24 | ((unsigned char)my_temp_point[1]<<16) | ((unsigned char)my_temp_point[2])<<8 | ((unsigned char)my_temp_point[3]);
	my_temp_point = my_temp_point + 4;
	m_monitor_head.conm_args.most_recent_time = ((unsigned char)my_temp_point[0])<<24 | ((unsigned char)my_temp_point[1]<<16) | ((unsigned char)my_temp_point[2])<<8 | ((unsigned char)my_temp_point[3]);
	my_temp_point = my_temp_point + 4;
	m_monitor_head.special = *(my_temp_point++);
	//m_monitor_head.total_seg = (unsigned char)my_temp_point[1]<<8 | (unsigned char)my_temp_point[0];
	//my_temp_point = my_temp_point + 2;
	m_monitor_head.seg_index = (unsigned char)my_temp_point[1]<<8 | (unsigned char)my_temp_point[0]; 
	my_temp_point = my_temp_point + 2;

	if(nlength!= 420)	//每包必发420个字节;
		return 0;
	if(m_monitor_head.type == BAC_UNITS_ANALOG)
		analog_data = true;
	else if(m_monitor_head.type == BAC_UNITS_DIGITAL)
		analog_data = false;
	else
		return 0;

	CString temp_db_ini_folder;
	temp_db_ini_folder = g_achive_folder + _T("\\MonitorIndex.ini");
	CString temp_serial;
	CString temp_monitor_index;
	if(analog_data)
		temp_monitor_index.Format(_T("Index_%d"),monitor_list_line);
	else
		temp_monitor_index.Format(_T("Digital_Index_%d"),monitor_list_line);
	temp_serial.Format(_T("%u"),g_selected_serialnumber);
	int temp_index = GetPrivateProfileInt(temp_serial,temp_monitor_index,-1,temp_db_ini_folder);
	if(temp_index >= m_monitor_head.seg_index)
		return 1;
	if(temp_index >= m_monitor_head.total_seg)	//如果数据库里面的 index 已经比设备里面的 多，就说明已经读过了;不需要再往数据库里面存了;
		return 1;

	CBADO monitor_bado;
	monitor_bado.SetDBPath(g_achive_monitor_datatbase_path);	//暂时不创建新数据库
	monitor_bado.OnInitADOConn(); 
	int loop_count = 400/(sizeof(Str_mon_element));
	for (int i=0;i<loop_count;i++)
	{
		Str_mon_element temp_data;
		temp_data.index = *(my_temp_point++); 
		temp_data.time = ((unsigned char)my_temp_point[0])<<24 | ((unsigned char)my_temp_point[1]<<16) | ((unsigned char)my_temp_point[2])<<8 | ((unsigned char)my_temp_point[3]);
		my_temp_point = my_temp_point + 4;
		temp_data.point.number = *(my_temp_point++); 
		temp_data.point.point_type = *(my_temp_point++); 

		temp_data.point.panel = *(my_temp_point++); 
		temp_data.point.sub_panel = *(my_temp_point++); 
		temp_data.point.network = 1;my_temp_point ++; //*(my_temp_point++);  目前不考虑 多个net 的情况 ;
		
		temp_data.value = ((unsigned char)my_temp_point[0])<<24 | ((unsigned char)my_temp_point[1]<<16) | ((unsigned char)my_temp_point[2])<<8 | ((unsigned char)my_temp_point[3]);
		my_temp_point = my_temp_point + 4;

		temp_data.mark =  (unsigned char)my_temp_point[1]<<8 | (unsigned char)my_temp_point[0];
		my_temp_point = my_temp_point + 2;
		if(monitor_list_line != temp_data.index)
			continue;
		if(temp_data.mark != 0x0A0D)	//0d0a
			continue;
		if(temp_data.time == 0) //说明后面是无用的数据;填充的是0
			continue;

		CString temp_type;
		temp_type.Format(_T("%u_%u_%u_%u_%u"),temp_data.point.number,temp_data.point.point_type,temp_data.point.panel,temp_data.point.sub_panel,temp_data.point.network);
	
		CString strSql;
		strSql.Format(_T("insert into MonitorData values(%i,%i,'%s',%i,%i,%i)"),g_selected_serialnumber,screen_list_line,temp_type,temp_data.value,temp_data.time , analog_data);
		monitor_bado.m_pConnection->Execute(strSql.GetString(),NULL,adCmdText);	

	}
	CString temp_write_index;
	temp_write_index.Format(_T("%d"),m_monitor_head.seg_index);
	WritePrivateProfileStringW(temp_serial,temp_monitor_index,temp_write_index,temp_db_ini_folder);
	monitor_bado.CloseConn();



	return 1;

}

// CBacnetProgramEdit message handlers
LRESULT CBacnetMonitor::OnHotKey(WPARAM wParam,LPARAM lParam)
{
	if (wParam==KEY_INSERT)
	{
		OnBnClickedBtnMonitorGraphic();
	}
	return 0;
}

BOOL CBacnetMonitor::OnHelpInfo(HELPINFO* pHelpInfo)
{ 

	if (g_protocol==PROTOCOL_BACNET_IP){
		HWND hWnd;

		if(pHelpInfo->dwContextId > 0) hWnd = ::HtmlHelp((HWND)pHelpInfo->hItemHandle,theApp.m_szHelpFile, HH_HELP_CONTEXT, pHelpInfo->dwContextId);
		else
			hWnd =  ::HtmlHelp((HWND)pHelpInfo->hItemHandle, theApp.m_szHelpFile,HH_HELP_CONTEXT, IDH_TOPIC_7_DATA);
		return (hWnd != NULL);
	}
	else{
		::HtmlHelp(NULL, theApp.m_szHelpFile, HH_HELP_CONTEXT, IDH_TOPIC_OVERVIEW);
	}

	return CDialogEx::OnHelpInfo(pHelpInfo);
}