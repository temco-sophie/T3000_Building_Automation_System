// BacnetRemotePoint.cpp : implementation file
//

#include "stdafx.h"
#include "T3000.h"
#include "BacnetRemotePoint.h"
#include "afxdialogex.h"


#include "CM5/ud_str.h"
#include "Bacnet_Include.h"
#include "globle_function.h"
#include "gloab_define.h"
#include "MainFrm.h"
// CBacnetRemotePoint dialog

IMPLEMENT_DYNAMIC(CBacnetRemotePoint, CDialogEx)

CBacnetRemotePoint::CBacnetRemotePoint(CWnd* pParent /*=NULL*/)
	: CDialogEx(CBacnetRemotePoint::IDD, pParent)
{

}

CBacnetRemotePoint::~CBacnetRemotePoint()
{
}

void CBacnetRemotePoint::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST_REMOTE_POINT, m_remote_point_list);
}


BEGIN_MESSAGE_MAP(CBacnetRemotePoint, CDialogEx)
	//ON_MESSAGE(MY_RESUME_DATA, RemotePointCallBack)
	ON_MESSAGE(WM_REFRESH_BAC_REMOTE_POINT_LIST,Fresh_Remote_List)	
	//ON_MESSAGE(WM_LIST_ITEM_CHANGED,Fresh_Remote_Item)
	ON_WM_CLOSE()
	ON_WM_TIMER()
END_MESSAGE_MAP()


// CBacnetRemotePoint message handlers


BOOL CBacnetRemotePoint::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	Initial_List();
	//	RegisterHotKey(GetSafeHwnd(),KEY_INSERT,NULL,VK_INSERT);//F2键
	SetTimer(1,BAC_LIST_REFRESH_TIME,NULL);

	// TODO:  Add extra initialization here

	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}


void CBacnetRemotePoint::Initial_List()
{

	m_remote_point_list.ModifyStyle(0, LVS_SINGLESEL|LVS_REPORT|LVS_SHOWSELALWAYS);
	m_remote_point_list.SetExtendedStyle(m_remote_point_list.GetExtendedStyle()  |LVS_EX_GRIDLINES&(~LVS_EX_FULLROWSELECT));//Not allow full row select.
	m_remote_point_list.InsertColumn(REMOTE_NUMBER, _T("Item"), 50, ListCtrlEx::Normal, LVCFMT_LEFT, ListCtrlEx::SortByDigit);
	m_remote_point_list.InsertColumn(REMOTE_DEVICE_ID, _T("Device ID"), 100, ListCtrlEx::Normal, LVCFMT_LEFT, ListCtrlEx::SortByString);
	m_remote_point_list.InsertColumn(REMOTE_REG, _T("Point number"), 100, ListCtrlEx::Normal, LVCFMT_LEFT, ListCtrlEx::SortByString);
	m_remote_point_list.InsertColumn(REMOTE_VALUE, _T("Value"), 120, ListCtrlEx::Normal, LVCFMT_LEFT, ListCtrlEx::SortByString);
	m_remote_point_list.InsertColumn(REMOTE_DEVICE_STATUS, _T("Status"), 100, ListCtrlEx::Normal, LVCFMT_LEFT, ListCtrlEx::SortByString);
	m_remote_point_list.InsertColumn(REMOTE_DESCRIPTION, _T("Description"), 200, ListCtrlEx::Normal, LVCFMT_LEFT, ListCtrlEx::SortByString);
	
	m_remote_point_hwnd = this->m_hWnd;
	//g_hwnd_now = m_screen_dlg_hwnd;

	CRect list_rect,win_rect;
	m_remote_point_list.GetWindowRect(list_rect);
	ScreenToClient(&list_rect);
	::GetWindowRect(m_screen_dlg_hwnd,win_rect);
	m_remote_point_list.Set_My_WindowRect(win_rect);
	m_remote_point_list.Set_My_ListRect(list_rect);


	m_remote_point_list.DeleteAllItems();
	for (int i=0;i<(int)m_remote_point_data.size();i++)
	{
		CString temp_item,temp_value,temp_cal,temp_filter,temp_status,temp_lable;
		CString temp_des;
		CString temp_units;
		temp_item.Format(_T("%d"),i+1);
		m_remote_point_list.InsertItem(i,temp_item);
		m_remote_point_list.SetCellEnabled(i,REMOTE_NUMBER,0);
		m_remote_point_list.SetCellChecked(0,0,1);// default will set the first one checked


		for (int x=0;x<REMOTE_COL_NUMBER;x++)
		{
			if((i%2)==0)
				m_remote_point_list.SetItemBkColor(i,x,LIST_ITEM_DEFAULT_BKCOLOR);
			else
				m_remote_point_list.SetItemBkColor(i,x,LIST_ITEM_DEFAULT_BKCOLOR_GRAY);		
		}

	}
}


BOOL CBacnetRemotePoint::PreTranslateMessage(MSG* pMsg)
{
	// TODO: Add your specialized code here and/or call the base class

	return CDialogEx::PreTranslateMessage(pMsg);
}


void CBacnetRemotePoint::OnCancel()
{
	// TODO: Add your specialized code here and/or call the base class
	::PostMessage(BacNet_hwd,WM_DELETE_NEW_MESSAGE_DLG,DELETE_WINDOW_MSG,0);
	//CDialogEx::OnCancel();
}


void CBacnetRemotePoint::OnClose()
{
	// TODO: Add your message handler code here and/or call default

	CDialogEx::OnClose();
}



LRESULT CBacnetRemotePoint::Fresh_Remote_List(WPARAM wParam,LPARAM lParam)
{
	int Fresh_Item;
	int isFreshOne = (int)lParam;
	if(isFreshOne == REFRESH_ON_ITEM)
	{
		Fresh_Item = (int)wParam;
	}
	else
	{
		if(m_remote_point_list.IsDataNewer((char *)&m_remote_point_data.at(0),sizeof(Str_remote_point) * BAC_REMOTE_POINT_COUNT))
		{
			//避免list 刷新时闪烁;在没有数据变动的情况下不刷新List;
			m_remote_point_list.SetListData((char *)&m_remote_point_data.at(0),sizeof(Str_remote_point) * BAC_REMOTE_POINT_COUNT);
		}
		else
		{
			return 0;
		}
	}

	for (int i=0;i<(int)m_remote_point_data.size();i++)
	{
		CString temp_device_id;
		CString temp_reg_number;
		CString temp_reg_value;
		CString temp_status;
		unsigned char dev_id = 0;
		unsigned short dev_reg = 0;
		int  dev_value = 0;

		if(isFreshOne)
		{
			i = Fresh_Item;
		}

		if(m_remote_point_data.at(i).point.sub_panel == 0)
		{
			m_remote_point_list.SetItemText(i,REMOTE_DEVICE_ID,_T(""));
			m_remote_point_list.SetItemText(i,REMOTE_REG,_T(""));
			m_remote_point_list.SetItemText(i,REMOTE_VALUE,_T(""));
			m_remote_point_list.SetItemText(i,REMOTE_DEVICE_STATUS,_T(""));
			m_remote_point_list.SetItemText(i,REMOTE_DESCRIPTION,_T(""));
			continue;
		}


		temp_device_id.Format(_T("%u"),m_remote_point_data.at(i).point.sub_panel);
		unsigned char high_3bit ;
		high_3bit = (unsigned char)((( m_remote_point_data.at(i).point.point_type ) & 0xE0) >> 5);
		dev_reg = high_3bit * 256 + m_remote_point_data.at(i).point.number;
		temp_reg_number.Format(_T("%u"),dev_reg);

		temp_reg_value.Format(_T("%d"),(m_remote_point_data.at(i).point_value));

		if(m_remote_point_data.at(i).device_online)
			temp_status.Format(_T("OK"));
		else
			temp_status.Format(_T("-"));

		if(dev_reg == 0)
		{
			m_remote_point_list.SetItemText(i,REMOTE_DEVICE_ID,_T(""));
			m_remote_point_list.SetItemText(i,REMOTE_REG,_T(""));
			m_remote_point_list.SetItemText(i,REMOTE_VALUE,_T(""));
			m_remote_point_list.SetItemText(i,REMOTE_DEVICE_STATUS,_T(""));
			m_remote_point_list.SetItemText(i,REMOTE_DESCRIPTION,_T(""));
			continue;
		}


		

		m_remote_point_list.SetItemText(i,REMOTE_DEVICE_ID,temp_device_id);
		m_remote_point_list.SetItemText(i,REMOTE_REG,temp_reg_number);
		m_remote_point_list.SetItemText(i,REMOTE_VALUE,temp_reg_value);
		m_remote_point_list.SetItemText(i,REMOTE_DEVICE_STATUS,temp_status);

		if(dev_reg >= (sizeof(TSTAT_5ABCDFG_LED_ADDRESS)/sizeof(TSTAT_5ABCDFG_LED_ADDRESS[0])))
		{
			m_remote_point_list.SetItemText(i,REMOTE_DEVICE_ID,_T(""));
			m_remote_point_list.SetItemText(i,REMOTE_REG,_T(""));
			m_remote_point_list.SetItemText(i,REMOTE_VALUE,_T(""));
			m_remote_point_list.SetItemText(i,REMOTE_DEVICE_STATUS,_T(""));
			m_remote_point_list.SetItemText(i,REMOTE_DESCRIPTION,_T(""));
			continue;
		}
		//AddressMap T3_8AI8AO[292];//PM_T3IOA
		//AddressMap T3_8AI16O[292];PM_T3AI16O
		//	AddressMap T3_32AI[292];PM_T332AI
		//	AddressMap T3_Performance[292];
		//AddressMap T3_4AO[292];PM_T34AO
		//	AddressMap T3_6CT[292];PM_T36CT
		//	//AddressMap T3_28IN[292];
		//	AddressMap T3_RTD[292];PM_T3PT10
		CString temp_description;
		unsigned char nFlag = m_remote_point_data.at(i).product_id;
		if((nFlag == PM_TSTAT6) || (nFlag == PM_TSTAT7)|| (nFlag == PM_TSTAT5i)|| (nFlag == PM_TSTAT8) )
		{
			MultiByteToWideChar( CP_ACP, 0, (char *)TSTAT_6_ADDRESS[dev_reg].AddressName,(int)strlen((char *)TSTAT_6_ADDRESS[dev_reg].AddressName)+1, temp_description.GetBuffer(MAX_PATH), MAX_PATH );
			temp_description.ReleaseBuffer();	
			
		}
		else if((nFlag == PM_TSTATRUNAR)||(nFlag == PM_TSTAT5E)||(nFlag == PM_PM5E) || (nFlag == PM_TSTAT5H)||(nFlag==PM_TSTAT5G))
		{
			MultiByteToWideChar( CP_ACP, 0, (char *)TSTAT_5EH_LCD_ADDRESS[dev_reg].AddressName,(int)strlen((char *)TSTAT_5EH_LCD_ADDRESS[dev_reg].AddressName)+1, temp_description.GetBuffer(MAX_PATH), MAX_PATH );
			temp_description.ReleaseBuffer();
		}
		else if((nFlag == PM_TSTAT5A) ||(nFlag == PM_TSTAT5B) ||
			(nFlag ==PM_TSTAT5C ) || (nFlag == PM_TSTAT5D) || (nFlag == PM_TSTAT5F))
		{
			MultiByteToWideChar( CP_ACP, 0, (char *)TSTAT_5ABCDFG_LED_ADDRESS[dev_reg].AddressName,(int)strlen((char *)TSTAT_5ABCDFG_LED_ADDRESS[dev_reg].AddressName)+1, temp_description.GetBuffer(MAX_PATH), MAX_PATH );
			temp_description.ReleaseBuffer();
		}
		else if(nFlag == PM_T3IOA)
		{
			if(dev_reg >= T3_REG_TOTAL_COUNT)
				continue;
			MultiByteToWideChar( CP_ACP, 0, (char *)T3_8AI8AO[dev_reg].AddressName,(int)strlen((char *)T3_8AI8AO[dev_reg].AddressName)+1, temp_description.GetBuffer(MAX_PATH), MAX_PATH );
			temp_description.ReleaseBuffer();
		}
		else if(nFlag == PM_T38AI16O)
		{
			if(dev_reg >= T3_REG_TOTAL_COUNT)
				continue;
			MultiByteToWideChar( CP_ACP, 0, (char *)T3_8AI16O[dev_reg].AddressName,(int)strlen((char *)T3_8AI16O[dev_reg].AddressName)+1, temp_description.GetBuffer(MAX_PATH), MAX_PATH );
			temp_description.ReleaseBuffer();
		}
		else if(nFlag == PM_T332AI)
		{
			if(dev_reg >= T3_REG_TOTAL_COUNT)
				continue;
			MultiByteToWideChar( CP_ACP, 0, (char *)T3_32AI[dev_reg].AddressName,(int)strlen((char *)T3_32AI[dev_reg].AddressName)+1, temp_description.GetBuffer(MAX_PATH), MAX_PATH );
			temp_description.ReleaseBuffer();
		}
		else if(nFlag == PM_T34AO)
		{
			if(dev_reg >= T3_REG_TOTAL_COUNT)
				continue;
			MultiByteToWideChar( CP_ACP, 0, (char *)T3_4AO[dev_reg].AddressName,(int)strlen((char *)T3_4AO[dev_reg].AddressName)+1, temp_description.GetBuffer(MAX_PATH), MAX_PATH );
			temp_description.ReleaseBuffer();
		}
		else if(nFlag == PM_T36CT)
		{
			if(dev_reg >= T3_REG_TOTAL_COUNT)
				continue;
			MultiByteToWideChar( CP_ACP, 0, (char *)T3_6CT[dev_reg].AddressName,(int)strlen((char *)T3_6CT[dev_reg].AddressName)+1, temp_description.GetBuffer(MAX_PATH), MAX_PATH );
			temp_description.ReleaseBuffer();
		}
		else if(nFlag == PM_T3PT10)
		{
			if(dev_reg >= T3_REG_TOTAL_COUNT)
				continue;
			MultiByteToWideChar( CP_ACP, 0, (char *)T3_RTD[dev_reg].AddressName,(int)strlen((char *)T3_RTD[dev_reg].AddressName)+1, temp_description.GetBuffer(MAX_PATH), MAX_PATH );
			temp_description.ReleaseBuffer();
		}
		else if(nFlag == PM_T38I13O)
		{
			if(dev_reg >= T3_REG_TOTAL_COUNT)
				continue;
			MultiByteToWideChar( CP_ACP, 0, (char *)T3_8I13O[dev_reg].AddressName,(int)strlen((char *)T3_8I13O[dev_reg].AddressName)+1, temp_description.GetBuffer(MAX_PATH), MAX_PATH );
			temp_description.ReleaseBuffer();
		}
		else
		{
			temp_description.Empty();
		}
		CString cs_temp_1;
		if(!temp_description.IsEmpty())
		{
			if(temp_description.GetLength() > 7)
			{
				cs_temp_1 = temp_description.Left(7);
				if(cs_temp_1.CompareNoCase(_T("MODBUS_")) == 0)
					temp_description = temp_description.Mid(7);
			}
		}
		//MODBUS_
		m_remote_point_list.SetItemText(i,REMOTE_DESCRIPTION,temp_description);

		if(isFreshOne)
		{
			break;
		}
	}
	return 0;
}


void CBacnetRemotePoint::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: Add your message handler code here and/or call default
	switch(nIDEvent)
	{
	case 1:
		{
			if((this->IsWindowVisible()) && (Gsm_communication == false) )	//GSM连接时不要刷新;
			{
				PostMessage(WM_REFRESH_BAC_REMOTE_POINT_LIST,NULL,NULL);
				if(bac_select_device_online)
					Post_Refresh_Message(g_bac_instance,READ_REMOTE_POINT,0,BAC_REMOTE_POINT_COUNT - 1,sizeof(Str_remote_point),BAC_REMOTE_POINT_GROUP);
			}
		}
		break;
	case 2:	//在更改某一列之后要在读取此列的值，并刷新此列;
		{
			if(this->IsWindowVisible())
				PostMessage(WM_REFRESH_BAC_OUTPUT_LIST,NULL,NULL);
			KillTimer(2);
		}
		break;
	}

	CDialogEx::OnTimer(nIDEvent);
}




