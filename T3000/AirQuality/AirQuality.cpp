// AirQuality.cpp : implementation file
//

#include "stdafx.h"
#include "../T3000.h"
#include "AirQuality.h"
#include "../globle_function.h"
#include "../MainFrm.h"

#include "../ChartCtrl/ChartLineSerie.h"
#include "../ChartCtrl/ChartPointsSerie.h"
#include "../ChartCtrl/ChartSurfaceSerie.h"
#include "../ChartCtrl/ChartGrid.h"
#include "../ChartCtrl/ChartBarSerie.h"
#include "../ChartCtrl/ChartLabel.h"
#include "../ChartCtrl/ChartAxisLabel.h"
#include "../ChartCtrl/ChartStandardAxis.h"
#include "../ChartCtrl/ChartDateTimeAxis.h"
#include "../ChartCtrl/ChartCrossHairCursor.h"
#include "../ChartCtrl/ChartDragLineCursor.h"

#include <bitset>
static BOOL ifAQdb = TRUE;
static BOOL writedb = TRUE;

CString Str_Display[4]={_T("Scroll"),_T("Temperature"),_T("Humidity"),_T("AirQuanlity")};

#define  WM_AIRQUANITY_FRESH WM_USER + 98562
// CAirQuality
UINT CAirQuality_BackMainUIFresh(LPVOID pParam)
{
	CAirQuality* pdlg = (CAirQuality*)pParam;
	while(1)
	{
		Sleep(2000);
		if(pdlg->IsWindowVisible())
		{	 
			if (is_connect())
			{
				if (pdlg->m_fresh_data)
				{
					if (!pdlg->m_fresh_data)
					{
						TRACE(_T(">>>>Fresh = 0\n"));
						continue;
					}
					TRACE(_T(">>>>Fresh = 1\n"));
					Read_Multi(g_tstat_id,&product_register_value[pdlg->MODBUS_TEMPRATURE_CHIP_F],pdlg->MODBUS_TEMPRATURE_CHIP_F,4,5);
					if (!pdlg->m_fresh_data)
					{
						continue;
					}
					Read_Multi(g_tstat_id,&product_register_value[pdlg->MODBUS_OUTPUT_RANGE],pdlg->MODBUS_OUTPUT_RANGE,4,5);
					if (!pdlg->m_fresh_data)
					{
						continue;
					}
					Read_Multi(g_tstat_id,&product_register_value[pdlg->MODBUS_TEMP_CURRENT_OUTPUT],pdlg->MODBUS_TEMP_CURRENT_OUTPUT,6,5);
					if (!pdlg->m_fresh_data)
					{
						continue;
					}
					::SendMessage(pdlg->m_hWnd,WM_AIRQUANITY_FRESH,0,0);

				}

			}
		}
	}
	return 0;
}
IMPLEMENT_DYNCREATE(CAirQuality, CFormView)

	CAirQuality::CAirQuality()
	: CFormView(CAirQuality::IDD)
{

	m_pFreshBackground=NULL;
	m_fresh_data=TRUE;

	for (int i=0;i<127;i++)
	{
		pSeries[i]=NULL;
	}
}

CAirQuality::~CAirQuality()
{

}

void CAirQuality::DoDataExchange(CDataExchange* pDX)
{
	CFormView::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_EDIT_ADDRESS, m_edit_address);
	DDX_Control(pDX, IDC_EDIT_SN, m_edit_sn);
	DDX_Control(pDX, IDC_EDIT_BAUDRATE, m_edit_baudrate);
	DDX_Control(pDX, IDC_EDIT_FV, m_edit_fv);
	DDX_Control(pDX, IDC_EDIT_HV, m_edit_hv);
	DDX_Control(pDX, IDC_EDIT_MODEL, m_edit_model);
	DDX_Control(pDX, IDC_COMBO_DISPLAY, m_combox_displayer);
	DDX_Control(pDX, IDC_EDIT_BACKLIGHT_TIME_TIME, m_edit_backlightTime);
	DDX_Control(pDX, IDC_EDIT_BACKLIGHT_TIME, m_combox_backlightTime);
	DDX_Control(pDX, IDC_CHECK_USER, m_check_user);
	DDX_Control(pDX, IDC_CHECK_DEFAULT, m_check_default);
	DDX_Control(pDX, IDC_CHARTCTRL, m_ChartCtrl);
	DDX_Control(pDX,IDC_LIST_INPUT_AQ,m_input_list);
	DDX_Control(pDX,IDC_LIST_OUTPUT_AQ,m_output_list);
	DDX_Control(pDX,IDC_LIST_USER,m_user_list);
	DDX_Control(pDX,IDC_LIST_FACTORY,m_factory_list);
	DDX_Control(pDX, IDC_EDIT_CAL, m_edit_calibration);
}

BEGIN_MESSAGE_MAP(CAirQuality, CFormView)
	ON_EN_KILLFOCUS(IDC_EDIT_BACKLIGHT_TIME_TIME, &CAirQuality::OnEnKillfocustempBacklight)
	ON_CBN_SELCHANGE(IDC_EDIT_BACKLIGHT_TIME, &CAirQuality::OnEnChangeBacklight)
	ON_BN_CLICKED(IDC_CHANGE_ID, &CAirQuality::OnBnClickedButton_Change_ID)
	ON_BN_CLICKED(IDC_BUTTON_FACTORY_RESET, &CAirQuality::OnBnClickedButton_Factory_Reset)
	ON_BN_CLICKED(IDD_USER_TABLE_ADD, &CAirQuality::OnBnClickedButton_UserTable_Add)
	ON_BN_CLICKED(IDD_USER_TABLE_DEL, &CAirQuality::OnBnClickedButton_UserTable_Del)
	ON_CBN_SELCHANGE(IDC_COMBO_DISPLAY, &CAirQuality::OnCbnSelchangeComboDisplay)
	ON_BN_CLICKED(IDC_CHECK_USER, &CAirQuality::OnBnClickedUser)
	ON_BN_CLICKED(IDC_CHECK_DEFAULT, &CAirQuality::OnBnClickedDefault)
	ON_MESSAGE(MY_RESUME_DATA, InputMessageCallBack_AQ)
	ON_MESSAGE(WM_REFRESH_BAC_INPUT_LIST,Fresh_Input_List_AQ)
	ON_MESSAGE(WM_LIST_ITEM_CHANGED,Fresh_Input_Item_AQ)
	ON_NOTIFY(NM_CLICK, IDC_LIST_INPUT_AQ, &CAirQuality::OnNMClickList1_AQ)
	ON_NOTIFY(NM_CLICK, IDC_LIST_OUTPUT_AQ, &CAirQuality::OnNMClickList2_AQ)

	ON_NOTIFY(NM_CLICK, IDC_LIST_USER, &CAirQuality::OnNMClickList_User)																			//OnNMClickList_Factory
	 
	ON_WM_DESTROY()
END_MESSAGE_MAP()

BEGIN_EVENTSINK_MAP(CAirQuality, CDialog)
	//ON_EVENT(CAirQuality, IDC_MSFLEXGRID1, DISPID_CLICK, CAirQuality::ClickMsflexgrid1, VTS_NONE)
END_EVENTSINK_MAP()
// CAirQuality diagnostics

#ifdef _DEBUG
void CAirQuality::AssertValid() const
{
	CFormView::AssertValid();
}

#ifndef _WIN32_WCE
void CAirQuality::Dump(CDumpContext& dc) const
{
	CFormView::Dump(dc);
}


#endif
#endif //_DEBUG
// CAirQuality message handlers
void CAirQuality::Fresh()
{
	g_NEED_MULTI_READ = FALSE;
	Read_Multi(g_tstat_id,Default_Value,2000,30);
	Initial_RegisterList();
	ShowAirqualityDialog();

	 
 
	Initial_UserList();
	Initial_FactoryList();
	ShowChecks();
	InitChartData();

	//InitChart();
	// DrawChart();
	if (!g_fresh_Graphic)
	{
		InitChartCtrl();
		DrawerPoint();
		g_fresh_Graphic = TRUE;


	}
	Initial_InputList();
	Initial_OutputList();
	//PostMessage(WM_REFRESH_BAC_INPUT_LIST,0,0);
	if (m_pFreshBackground==NULL)
	{
		m_pFreshBackground = AfxBeginThread(CAirQuality_BackMainUIFresh,this);
	}
}


void CAirQuality::OnCbnSelchangeComboDisplay(){
	int sel=m_combox_displayer.GetCurSel();

	if (product_register_value[MODBUS_SCROLL_MODE]==sel)
	{
		return;
	}
	int ret=write_one(g_tstat_id,MODBUS_SCROLL_MODE,sel);
	if (ret>0)
	{
		product_register_value[MODBUS_SCROLL_MODE]=sel;
	}
	else
	{
		AfxMessageBox(_T("Write Fail"));
	}

	m_combox_displayer.SetCurSel(product_register_value[MODBUS_SCROLL_MODE]);
}

void CAirQuality::OnCbnSelchangeComboBacklight(){

}
 
void CAirQuality::ShowChecks(){
	BOOL is_grad = TRUE;
	if (product_register_value[MODBUS_HUM_CURRENT_DEFAULT] == 1)
	{
		is_grad = FALSE;
	}
	if (is_grad)
	{
		m_check_default.SetCheck(TRUE);
		m_check_user.SetCheck(FALSE);
	} 
	else
	{
		m_check_default.SetCheck(FALSE);
		m_check_user.SetCheck(TRUE);
	}

	Initial_UserList();
}
void CAirQuality::OnBnClickedUser(){


	if (product_register_value[MODBUS_HUM_CURRENT_DEFAULT] == 1)//说明是选中状态
	{

		ShowChecks();
		return;
	}



	int ret=write_one(g_tstat_id,MODBUS_HUM_CURRENT_DEFAULT,1);
	if (ret>0)
	{
		product_register_value[MODBUS_HUM_CURRENT_DEFAULT] = 1;

	}
	else
	{
		AfxMessageBox(_T("Setting Fail!"));
	}
	ShowChecks();
}

void CAirQuality::OnBnClickedDefault(){
	if (product_register_value[MODBUS_HUM_CURRENT_DEFAULT] == 0)//说明是选中状态
	{

		ShowChecks();
		return;
	}



	int ret=write_one(g_tstat_id,MODBUS_HUM_CURRENT_DEFAULT,0);
	if (ret>0)
	{
		product_register_value[MODBUS_HUM_CURRENT_DEFAULT] = 0;

	}
	else
	{
		AfxMessageBox(_T("Setting Fail!"));
	}
	ShowChecks();
}


void CAirQuality::OnDestroy()
{

	if (m_pFreshBackground!=NULL)
	{
		if (WaitForSingleObject(m_pFreshBackground->m_hThread, 1000) != WAIT_OBJECT_0)

			//Sleep(500);//wait for the end of the thread.
			if (m_pFreshBackground!=NULL) 

			{
				if (WaitForSingleObject(m_pFreshBackground->m_hThread, 3000) == WAIT_OBJECT_0)
				{

				}
				else
				{		
					BOOL bRet = TerminateThread(m_pFreshBackground->m_hThread,0);

					m_pFreshBackground=NULL;
				}

			}
	}
	CFormView::OnDestroy();
	// TODO: Add your message handler code here
}
void CAirQuality::OnInitialUpdate()
{
	CFormView::OnInitialUpdate();



}
void CAirQuality::ShowAirqualityDialog()
{
	CString strTemp;
	strTemp.Format(_T("%d"),product_register_value[MODBUS_ADDRESS]);
	m_edit_address.SetWindowTextW(strTemp);
	int SN=product_register_value[MODBUS_SERIALNUMBER_HIWORD+1]*256*256*256+product_register_value[MODBUS_SERIALNUMBER_HIWORD]*256*256+product_register_value[MODBUS_SERIALNUMBER_LOWORD+1]*256+product_register_value[MODBUS_SERIALNUMBER_LOWORD];
	strTemp.Format(_T("%d"),SN);
	m_edit_sn.SetWindowTextW(strTemp);
	strTemp=GetProductName(product_register_value[7]);
	m_edit_model.SetWindowTextW(strTemp);
	float fv=(float)(product_register_value[MODBUS_VERSION_NUMBER_HI]*256+product_register_value[MODBUS_VERSION_NUMBER_LO])/10.0;	
	strTemp.Format(_T("%0.1f"),fv);
	m_edit_fv.SetWindowTextW(strTemp);
	strTemp.Format(_T("%d"),product_register_value[MODBUS_HARDWARE_REV]);
	m_edit_hv.SetWindowTextW(strTemp);
	if (product_register_value[MODBUS_BAUDRATE]==0)
	{
		strTemp=_T("9600");

	} 
	else
	{
		strTemp=_T("19200");
	}
	m_edit_baudrate.SetWindowTextW(strTemp);


	m_combox_displayer.ResetContent();
	if (product_register_value[7]==PM_HUM_R)
	{
		for (int i=0;i<3;i++)
		{
			m_combox_displayer.AddString(Str_Display[i]);
		}
	} 
	else
	{
		for (int i=0;i<4;i++)
		{
			m_combox_displayer.AddString(Str_Display[i]);
		} 
	}

	if (product_register_value[MODBUS_SCROLL_MODE]>=0&&product_register_value[MODBUS_SCROLL_MODE]<4)
	{
		m_combox_displayer.SetCurSel(product_register_value[MODBUS_SCROLL_MODE]);
	}
	else{
		m_combox_displayer.SetCurSel(0);
	}
	strTemp.Format(_T("%d"),product_register_value[MODBUS_HUMIDITY_FREQUENCY]);

	GetDlgItem(IDC_EDIT_FREQUENCE)->SetWindowText(strTemp);

	if (product_register_value[361]==0)
	{
		//strTemp.Format(_T("%d"),(short)product_register_value[361]);

		m_edit_backlightTime.SetWindowTextW(_T("0"));
		m_combox_backlightTime.SetCurSel(0);

	}
	else if (product_register_value[361]==255)
	{

		m_edit_backlightTime.SetWindowTextW(_T("255"));
		m_combox_backlightTime.SetCurSel(1);  

	}
	else if(product_register_value[361]>0&&product_register_value[361]<255)
	{
		strTemp.Format(_T("%d"),(short)product_register_value[361]);
		m_edit_backlightTime.SetWindowTextW(strTemp);
		m_combox_backlightTime.SetCurSel(-1);  
	}

}
 
void CAirQuality::Initial_UserList(){

	BOOL is_grad = TRUE;
	if (product_register_value[MODBUS_HUM_CURRENT_DEFAULT] == 1)
	{
		is_grad = FALSE;
	}
	int rows = product_register_value[MODBUS_HUM_CAL_NUM];
	if (rows < 0)
	{
		rows = 10;
	}

	CString strTemp;
	m_user_list.ShowWindow(SW_HIDE);
	m_user_list.DeleteAllItems();
	while ( m_user_list.DeleteColumn (0)) ;

	m_user_list.ModifyStyle(0, LVS_SINGLESEL|LVS_REPORT|LVS_SHOWSELALWAYS);
	m_user_list.SetExtendedStyle(m_user_list.GetExtendedStyle()  |LVS_EX_GRIDLINES&(~LVS_EX_FULLROWSELECT));	
	m_user_list.InsertColumn(0, _T("NUM"), 50, ListCtrlEx::Normal, LVCFMT_CENTER, ListCtrlEx::SortByDigit);
	m_user_list.InsertColumn(1, _T("Frequency"), 70, ListCtrlEx::Normal, LVCFMT_CENTER, ListCtrlEx::SortByString);
	m_user_list.InsertColumn(2, _T("Humidity(%)"), 80, ListCtrlEx::EditBox, LVCFMT_CENTER, ListCtrlEx::SortByString);
	g_hwnd_now = this->m_hWnd;

	CString index;
	for (int i=0;i< rows;i++)
	{
		index.Format(_T("%d"),i);
		m_user_list.InsertItem(i,index);
	}

//	CString strTemp;
	for (int row=0;row<rows;row++)
	{
		strTemp.Format(_T("%d"), product_register_value[MODBUS_HUMCOUNT1_H+2*row]);
		m_user_list.SetItemText(row,1,strTemp);
		strTemp.Format(_T("%0.1f"), ((float)((unsigned short)product_register_value[MODBUS_HUMCOUNT1_H+2*row+1]))/10);
		m_user_list.SetItemText(row,2,strTemp);

		if (is_grad)
		{
			m_user_list.SetCellEnabled(row,0,FALSE);
			m_user_list.SetCellEnabled(row,1,FALSE);
		 	m_user_list.SetItemBkColor(row,0,DISABLE_COLOR_CELL);
			m_user_list.SetItemBkColor(row,1,DISABLE_COLOR_CELL);
			m_user_list.SetItemBkColor(row,2,DISABLE_COLOR_CELL); 
		}
		else
		{
			m_user_list.SetCellEnabled(row,0,TRUE);
			m_user_list.SetCellEnabled(row,1,TRUE);
			 m_user_list.SetItemBkColor(row,0,RGB(255,255,255));
			m_user_list.SetItemBkColor(row,1,RGB(255,255,255));
			m_user_list.SetItemBkColor(row,2,RGB(255,255,255)); 

		}
	}
	m_user_list.ShowWindow(TRUE);
}

void CAirQuality::Initial_FactoryList(){
	int rows = Default_Value[3];
	if (rows < 1)
	{
		rows = 10;
	}

	m_factory_list.ShowWindow(SW_HIDE);
	m_factory_list.DeleteAllItems();
	while ( m_factory_list.DeleteColumn (0)) ;

	m_factory_list.ModifyStyle(0, LVS_SINGLESEL|LVS_REPORT|LVS_SHOWSELALWAYS);
	m_factory_list.SetExtendedStyle(m_factory_list.GetExtendedStyle()  |LVS_EX_GRIDLINES&(~LVS_EX_FULLROWSELECT));	
	m_factory_list.InsertColumn(0, _T("NUM"), 50, ListCtrlEx::Normal, LVCFMT_CENTER, ListCtrlEx::SortByDigit);
	m_factory_list.InsertColumn(1, _T("Frequency"), 70, ListCtrlEx::Normal, LVCFMT_CENTER, ListCtrlEx::SortByString);
	m_factory_list.InsertColumn(2, _T("Humidity(%)"), 80, ListCtrlEx::Normal, LVCFMT_CENTER, ListCtrlEx::SortByString);
	g_hwnd_now = this->m_hWnd;

 
	CString index;
	for (int i=0;i< rows;i++)
	{
		index.Format(_T("%d"),i);
		m_factory_list.InsertItem(i,index);
	}
 
	CString strTemp;
	for (int row=0;row<rows;row++)
	{
		strTemp.Format(_T("%d"), Default_Value[4+2*row+1]);
	 
		m_factory_list.SetItemText(row,1,strTemp); 

		strTemp.Format(_T("%0.1f"), ((float)((unsigned short)Default_Value[4+2*row]))/10);
		 
	 	m_factory_list.SetItemText(row,2,strTemp);

		 	m_factory_list.SetCellEnabled(row,0,FALSE);
		m_factory_list.SetCellEnabled(row,1,FALSE);
		m_factory_list.SetItemBkColor(row,0,DISABLE_COLOR_CELL);
		m_factory_list.SetItemBkColor(row,1,DISABLE_COLOR_CELL);
		m_factory_list.SetItemBkColor(row,2,DISABLE_COLOR_CELL);  
		 
	 
	}
  m_factory_list.ShowWindow(TRUE);
}
 
 


void CAirQuality::Initial_RegisterList(){
#if 1
	MODBUS_SERIALNUMBER_LOWORD	=	0	;
	MODBUS_SERIALNUMBER_HIWORD	=	2	;
	MODBUS_VERSION_NUMBER_LO	=	4	;
	MODBUS_VERSION_NUMBER_HI	=	5	;
	MODBUS_ADDRESS	=	6	;
	MODBUS_PRODUCT_MODEL	=	7	;
	MODBUS_HARDWARE_REV	=	8	;
	MODBUS_PIC_VERSION	=	9	;
	MODBUS_HUM_PIC_VERSION	=	10	;
	MODBUS_ADDRESS_PLUG_N_PLAY	=	11	;
	MODBUS_BASE_ADDRESS 	=	15	;
	MODBUS_UPDATE_STATUS	=	16	;
	MODBUS_UPDATE_PTR_HI	=	17	;
	MODBUS_UPDATE_PTR_LO	=	18	;
	MODBUS_SCRATCHPAD_ADDRESS	=	19	;
	MODBUS_TEMPRATURE_CHIP_F	=	100	;
	MODBUS_TEMPRATURE_CHIP 	=	101	;
	MODBUS_HUM            	=	102	;
	MODBUS_AQ            	=	103	;
	MODBUS_CALIBRATION_TEMP    	=	104	;
	MODBUS_CALIBRATION_AQ 	=	105	;
	MODBUS_CALIBRATION_HUM 	=	106	;
	MODBUS_TEMP_SELECT	=	111	;
	MODBUS_DAC_OFFSET	=	112	;
	MODBUS_DELAY	=	113	;
	MODBUS_SEQUENCE	=	114	;
	MODBUS_DEGC_OR_F 	=	121	;
	MODBUS_POWERUP_MODE  	=	122	;
	MODBUS_AUTO_ONLY 	=	123	;
	MODBUS_OUTPUT_RANGE	=	180	;
	MODBUS_FACTORY_DEFAULTS     	=	181	;
	MODBUS_EXTERNAL_SENSOR_0	=	182	;
	MODBUS_EXTERNAL_SENSOR_1	=	183	;
	MODBUS_INFO_BYTE	=	184	;
	MODBUS_BAUDRATE    	=	185	;
	MODBUS_HUM_FILTER	=	186	;
	MODBUS_AQ_FILTER	=	187	;
	MODBUS_DIGITAL_IN1	=	188	;
	MODBUS_CYCLING_DELAY 	=	190	;
	MODBUS_CHANGOVER_DELAY	=	191	;
	MODBUS_OVERRIDE_TIMER	=	192	;
	MODBUS_OVERRIDE_TIMER_LEFT	=	193	;
	MODBUS_FILTER	=	194	;
	MODBUS_HEAT_COOL_CONFIG	=	195	;
	MODBUS_INTERNAL_TEMP_IC	=	196	;
	MODBUS_INTERNAL_THERMISTOR	=	197	;
	MODBUS_CALIBRATION_INTERNAL_THERMISTOR	=	217	;
	MODBUS_CALIBRATION_ANALOG_IN2	=	218	;
	MODBUS_INPUT1_SELECT                          	=	219	;
	MODBUS_UNIVERSAL_NIGHTSET                  	=	220	;
	MODBUS_HEAT_ORIGINAL_TABLE                 	=	221	;
	MODBUS_COOL_ORIGINAL_TABLE                 	=	222	;
	MODBUS_PID2_MODE_OPERATION	=	223	;
	MODBUS_VALVE_TRAVEL_TIME 	=	224	;
	MODBUS_VALVE_PERCENT	=	225	;
	MODBUS_INTERLOCK_OUTPUT1	=	226	;
	MODBUS_INTERLOCK_OUTPUT2	=	227	;
	MODBUS_INTERLOCK_OUTPUT3	=	228	;
	MODBUS_INTERLOCK_OUTPUT4	=	229	;
	MODBUS_INTERLOCK_OUTPUT5	=	230	;
	MODBUS_INTERLOCK_OUTPUT6	=	231	;
	MODBUS_INTERLOCK_OUTPUT7	=	232	;
	MODBUS_SETPOINT_INCREASE	=	233	;
	MODBUS_FREEZE_TEMP_SETPOINT	=	234	;
	MODBUS_FREEZE_DELAY_ON	=	235	;
	MODBUS_FREEZE_DELAY_OFF	=	236	;
	MODBUS_ANALOG1_FUNCTION	=	237	;
	MODBUS_ANALOG2_FUNCTION	=	238	;
	MODBUS_DIGITAL1_FUNCTION	=	239	;
	MODBUS_DISPLAY_HUNDRED	=	240	;
	MODBUS_DISPLAY_TEN	=	241	;
	MODBUS_DISPLAY_DIGITAL	=	242	;
	MODBUS_DISPLAY_STATUS	=	243	;
	MODBUS_INPUT_MANU_ENABLE	=	244	;
	MODBUS_OUTPUT_MANU_ENABLE	=	245	;
	MODBUS_DIGITAL_INPUT	=	246	;
	MODBUS_DEAD_MASTER	=	247	;
	MODBUS_ROUND_DISPLAY	=	248	;
	MODBUS_MIN_ADDRESS 	=	249	;
	MODBUS_MAX_ADDRESS	=	250	;
	MODBUS_FAN_GRIDPOINT	=	251	;
	MODBUS_MODE_GRIDPOINT	=	252	;
	MODBUS_HOLD_GRIDPOINT	=	253	;
	MODBUS_CONFIGURATION	=	254	;
	MODBUS_UTIL_MODE	=	255	;
	MODBUS_EEPROM_SIZE	=	256	;
	MODBUS_TIMER_SELECT	=	257	;
	MODBUS_OUTPUT1_FUNCTION	=	258	;
	MODBUS_ROTATION_OUT2	=	259	;
	MODBUS_ROTATION_OUT3	=	260	;
	MODBUS_ROTATION_OUT4	=	261	;
	MODBUS_ROTATION_OUT5	=	262	;
	MODBUS_OUTPUT2_FUNCTION	=	263	;
	MODBUS_OUTPUT3_FUNCTION	=	264	;
	MODBUS_OUTPUT4_FUNCTION	=	265	;
	MODBUS_OUTPUT5_FUNCTION	=	266	;
	MODBUS_PIR_STAGE	=	267	;
	MODBUS_AIR_CALIBRATION_POINT4	=	300	;
	MODBUS_AIR_CALIBRATION_POINT1	=	301	;
	MODBUS_AIR_CALIBRATION_POINT2	=	302	;
	MODBUS_AIR_CALIBRATION_POINT3	=	303	;
	MODUBS_HUMIDITY_RH 	=	304	;
	MODBUS_HUMIDITY_FREQUENCY	=	305	;
	MODBUS_HUM_PIC_UPDATE	=	306	;
	MODBUS_HUM_CAL_NUM 	=	307	;
	MODBUS_HUM_CAL_EREASE 	=	308	;
	MODBUS_HUM_CURRENT_DEFAULT 	=	309	;
	MODBUS_HUM_PICDATAOK 	=	310	;
	MODBUS_HUM_OFFSETFLAG  	=	311	;
	MODBUS_HUMCOUNT1_H   	=	312	;
	MODBUS_HUMRH1_H	=	313	;
	MODBUS_HUMCOUNT2_H 	=	314	;
	MODBUS_HUMRH2_H	=	315	;
	MODBUS_HUMCOUNT3_H 	=	316	;
	MODBUS_HUMRH3_H	=	317	;
	MODBUS_HUMCOUNT4_H 	=	318	;
	MODBUS_HUMRH4_H	=	319	;
	MODBUS_HUMCOUNT5_H 	=	320	;
	MODBUS_HUMRH5_H	=	321	;
	MODBUS_HUMCOUNT6_H 	=	322	;
	MODBUS_HUMRH6_H	=	323	;
	MODBUS_HUMCOUNT7_H 	=	324	;
	MODBUS_HUMRH7_H	=	325	;
	MODBUS_HUMCOUNT8_H 	=	326	;
	MODBUS_HUMRH8_H	=	327	;
	MODBUS_HUMCOUNT9_H 	=	328	;
	MODBUS_HUMRH9_H	=	329	;
	MODBUS_HUMCOUNT10_H 	=	330	;
	MODBUS_HUMRH10_H	=	331	;
	MODBUS_TEM_RANGE_LOWER  	=	332	;
	MODBUS_TEM_RANGE_UPPER	=	333	;
	MODBUS_HUM_RANGE_LOWER	=	334	;
	MODBUS_HUM_RANGE_UPPER	=	335	;
	MODBUS_AQ_RANGE_LOWER	=	336	;
	MODBUS_AQ_RANGE_UPPER 	=	337	;
	MODBUS_TEMP_CURRENT_OUTPUT      	=	338	;
	MODBUS_HUM_CURRENT_OUTPUT	=	339	;
	MODBUS_AQ_CURRENT_OUTPUT	=	340	;
	MODBUS_TEMP_VOLTAGE_OUTPUT	=	341	;
	MODBUS_HUM_VOLTAGE_OUTPUT	=	342	;
	MODBUS_AQ_VOLTAGE_OUTPUT  	=	343	;
	MODBUS_CALIBRATION_MODE  	=	344	;
	MODBUS_SCROLL_MODE	=	345	;
	MODBUS_AQ_LEVEL0	=	346	;
	MODBUS_AQ_LEVEL1	=	347	;
	MODBUS_AQ_LEVEL2	=	348	;
	MODBUS_OUTPUT_MODE	=	349	;
	MODBUS_TEMP_MANU_OUTPUT	=	350	;
	MODBUS_HUM_MANU_OUTPUT	=	351	;
	MODBUS_AQ_MANU_OUTPUT	=	352	;
	MODBUS_HUM_CALIBRATION_LOCKX    	=	353	;
	MODBUS_HUM_CALIBRATION_LOCKY 	=	354	;
#endif

	T3Register temp;
	CADO m_ado;
	m_ado.OnInitADOConn();
#if 1
	if (!m_ado.IsHaveTable(m_ado,_T("AirQuanlity_Reglist")))
	{
		return;
	}
	CString SQL = _T("select * from AirQuanlity_Reglist");
	m_ado.m_pRecordset = m_ado.OpenRecordset(SQL);
	_variant_t vartemp;
	while(!m_ado.m_pRecordset->EndOfFile)
	{
		temp.regID=m_ado.m_pRecordset->GetCollect(_T("RegID"));
		vartemp =m_ado.m_pRecordset->GetCollect(_T("RegName"));
		if (vartemp.vt==VT_NULL)
			temp.regName=_T("");
		else
			temp.regName =vartemp;
		m_ado.m_pRecordset->MoveNext();
		m_vecT3Register.push_back(temp);
	}
	m_ado.CloseRecordset();
	m_ado.CloseConn();
#endif 

	MODBUS_SERIALNUMBER_LOWORD	=	Get_RegID(_T("	MODBUS_SERIALNUMBER_LOWORD	"))	;
	MODBUS_SERIALNUMBER_HIWORD	=	Get_RegID(_T("	MODBUS_SERIALNUMBER_HIWORD	"))	;
	MODBUS_VERSION_NUMBER_LO	=	Get_RegID(_T("	MODBUS_VERSION_NUMBER_LO	"))	;
	MODBUS_VERSION_NUMBER_HI	=	Get_RegID(_T("	MODBUS_VERSION_NUMBER_HI	"))	;
	MODBUS_ADDRESS	=	Get_RegID(_T("	MODBUS_ADDRESS	"))	;
	MODBUS_PRODUCT_MODEL	=	Get_RegID(_T("	MODBUS_PRODUCT_MODEL	"))	;
	MODBUS_HARDWARE_REV	=	Get_RegID(_T("	MODBUS_HARDWARE_REV	"))	;
	MODBUS_PIC_VERSION	=	Get_RegID(_T("	MODBUS_PIC_VERSION	"))	;
	MODBUS_HUM_PIC_VERSION	=	Get_RegID(_T("	MODBUS_HUM_PIC_VERSION	"))	;
	MODBUS_ADDRESS_PLUG_N_PLAY	=	Get_RegID(_T("	MODBUS_ADDRESS_PLUG_N_PLAY	"))	;
	MODBUS_BASE_ADDRESS 	=	Get_RegID(_T("	MODBUS_BASE_ADDRESS 	"))	;
	MODBUS_UPDATE_STATUS	=	Get_RegID(_T("	MODBUS_UPDATE_STATUS	"))	;
	MODBUS_UPDATE_PTR_HI	=	Get_RegID(_T("	MODBUS_UPDATE_PTR_HI	"))	;
	MODBUS_UPDATE_PTR_LO	=	Get_RegID(_T("	MODBUS_UPDATE_PTR_LO	"))	;
	MODBUS_SCRATCHPAD_ADDRESS	=	Get_RegID(_T("	MODBUS_SCRATCHPAD_ADDRESS	"))	;
	MODBUS_TEMPRATURE_CHIP_F	=	Get_RegID(_T("	MODBUS_TEMPRATURE_CHIP_F	"))	;
	MODBUS_TEMPRATURE_CHIP 	=	Get_RegID(_T("	MODBUS_TEMPRATURE_CHIP 	"))	;
	MODBUS_HUM            	=	Get_RegID(_T("	MODBUS_HUM            	"))	;
	MODBUS_AQ            	=	Get_RegID(_T("	MODBUS_AQ            	"))	;
	MODBUS_CALIBRATION_TEMP    	=	Get_RegID(_T("	MODBUS_CALIBRATION_TEMP    	"))	;
	MODBUS_CALIBRATION_AQ 	=	Get_RegID(_T("	MODBUS_CALIBRATION_AQ 	"))	;
	MODBUS_CALIBRATION_HUM 	=	Get_RegID(_T("	MODBUS_CALIBRATION_HUM 	"))	;
	MODBUS_TEMP_SELECT	=	Get_RegID(_T("	MODBUS_TEMP_SELECT	"))	;
	MODBUS_DAC_OFFSET	=	Get_RegID(_T("	MODBUS_DAC_OFFSET	"))	;
	MODBUS_DELAY	=	Get_RegID(_T("	MODBUS_DELAY	"))	;
	MODBUS_SEQUENCE	=	Get_RegID(_T("	MODBUS_SEQUENCE	"))	;
	MODBUS_DEGC_OR_F 	=	Get_RegID(_T("	MODBUS_DEGC_OR_F 	"))	;
	MODBUS_POWERUP_MODE  	=	Get_RegID(_T("	MODBUS_POWERUP_MODE  	"))	;
	MODBUS_AUTO_ONLY 	=	Get_RegID(_T("	MODBUS_AUTO_ONLY 	"))	;
	MODBUS_OUTPUT_RANGE	=	Get_RegID(_T("	MODBUS_OUTPUT_RANGE	"))	;
	MODBUS_FACTORY_DEFAULTS     	=	Get_RegID(_T("	MODBUS_FACTORY_DEFAULTS     	"))	;
	MODBUS_EXTERNAL_SENSOR_0	=	Get_RegID(_T("	MODBUS_EXTERNAL_SENSOR_0	"))	;
	MODBUS_EXTERNAL_SENSOR_1	=	Get_RegID(_T("	MODBUS_EXTERNAL_SENSOR_1	"))	;
	MODBUS_INFO_BYTE	=	Get_RegID(_T("	MODBUS_INFO_BYTE	"))	;
	MODBUS_BAUDRATE    	=	Get_RegID(_T("	MODBUS_BAUDRATE    	"))	;
	MODBUS_HUM_FILTER	=	Get_RegID(_T("	MODBUS_HUM_FILTER	"))	;
	MODBUS_AQ_FILTER	=	Get_RegID(_T("	MODBUS_AQ_FILTER	"))	;
	MODBUS_DIGITAL_IN1	=	Get_RegID(_T("	MODBUS_DIGITAL_IN1	"))	;
	MODBUS_CYCLING_DELAY 	=	Get_RegID(_T("	MODBUS_CYCLING_DELAY 	"))	;
	MODBUS_CHANGOVER_DELAY	=	Get_RegID(_T("	MODBUS_CHANGOVER_DELAY	"))	;
	MODBUS_OVERRIDE_TIMER	=	Get_RegID(_T("	MODBUS_OVERRIDE_TIMER	"))	;
	MODBUS_OVERRIDE_TIMER_LEFT	=	Get_RegID(_T("	MODBUS_OVERRIDE_TIMER_LEFT	"))	;
	MODBUS_FILTER	=	Get_RegID(_T("	MODBUS_FILTER	"))	;
	MODBUS_HEAT_COOL_CONFIG	=	Get_RegID(_T("	MODBUS_HEAT_COOL_CONFIG	"))	;
	MODBUS_INTERNAL_TEMP_IC	=	Get_RegID(_T("	MODBUS_INTERNAL_TEMP_IC	"))	;
	MODBUS_INTERNAL_THERMISTOR	=	Get_RegID(_T("	MODBUS_INTERNAL_THERMISTOR	"))	;
	MODBUS_CALIBRATION_INTERNAL_THERMISTOR	=	Get_RegID(_T("	MODBUS_CALIBRATION_INTERNAL_THERMISTOR	"))	;
	MODBUS_CALIBRATION_ANALOG_IN2	=	Get_RegID(_T("	MODBUS_CALIBRATION_ANALOG_IN2	"))	;
	MODBUS_INPUT1_SELECT                          	=	Get_RegID(_T("	MODBUS_INPUT1_SELECT                          	"))	;
	MODBUS_UNIVERSAL_NIGHTSET                  	=	Get_RegID(_T("	MODBUS_UNIVERSAL_NIGHTSET                  	"))	;
	MODBUS_HEAT_ORIGINAL_TABLE                 	=	Get_RegID(_T("	MODBUS_HEAT_ORIGINAL_TABLE                 	"))	;
	MODBUS_COOL_ORIGINAL_TABLE                 	=	Get_RegID(_T("	MODBUS_COOL_ORIGINAL_TABLE                 	"))	;
	MODBUS_PID2_MODE_OPERATION	=	Get_RegID(_T("	MODBUS_PID2_MODE_OPERATION	"))	;
	MODBUS_VALVE_TRAVEL_TIME 	=	Get_RegID(_T("	MODBUS_VALVE_TRAVEL_TIME 	"))	;
	MODBUS_VALVE_PERCENT	=	Get_RegID(_T("	MODBUS_VALVE_PERCENT	"))	;
	MODBUS_INTERLOCK_OUTPUT1	=	Get_RegID(_T("	MODBUS_INTERLOCK_OUTPUT1	"))	;
	MODBUS_INTERLOCK_OUTPUT2	=	Get_RegID(_T("	MODBUS_INTERLOCK_OUTPUT2	"))	;
	MODBUS_INTERLOCK_OUTPUT3	=	Get_RegID(_T("	MODBUS_INTERLOCK_OUTPUT3	"))	;
	MODBUS_INTERLOCK_OUTPUT4	=	Get_RegID(_T("	MODBUS_INTERLOCK_OUTPUT4	"))	;
	MODBUS_INTERLOCK_OUTPUT5	=	Get_RegID(_T("	MODBUS_INTERLOCK_OUTPUT5	"))	;
	MODBUS_INTERLOCK_OUTPUT6	=	Get_RegID(_T("	MODBUS_INTERLOCK_OUTPUT6	"))	;
	MODBUS_INTERLOCK_OUTPUT7	=	Get_RegID(_T("	MODBUS_INTERLOCK_OUTPUT7	"))	;
	MODBUS_SETPOINT_INCREASE	=	Get_RegID(_T("	MODBUS_SETPOINT_INCREASE	"))	;
	MODBUS_FREEZE_TEMP_SETPOINT	=	Get_RegID(_T("	MODBUS_FREEZE_TEMP_SETPOINT	"))	;
	MODBUS_FREEZE_DELAY_ON	=	Get_RegID(_T("	MODBUS_FREEZE_DELAY_ON	"))	;
	MODBUS_FREEZE_DELAY_OFF	=	Get_RegID(_T("	MODBUS_FREEZE_DELAY_OFF	"))	;
	MODBUS_ANALOG1_FUNCTION	=	Get_RegID(_T("	MODBUS_ANALOG1_FUNCTION	"))	;
	MODBUS_ANALOG2_FUNCTION	=	Get_RegID(_T("	MODBUS_ANALOG2_FUNCTION	"))	;
	MODBUS_DIGITAL1_FUNCTION	=	Get_RegID(_T("	MODBUS_DIGITAL1_FUNCTION	"))	;
	MODBUS_DISPLAY_HUNDRED	=	Get_RegID(_T("	MODBUS_DISPLAY_HUNDRED	"))	;
	MODBUS_DISPLAY_TEN	=	Get_RegID(_T("	MODBUS_DISPLAY_TEN	"))	;
	MODBUS_DISPLAY_DIGITAL	=	Get_RegID(_T("	MODBUS_DISPLAY_DIGITAL	"))	;
	MODBUS_DISPLAY_STATUS	=	Get_RegID(_T("	MODBUS_DISPLAY_STATUS	"))	;
	MODBUS_INPUT_MANU_ENABLE	=	Get_RegID(_T("	MODBUS_INPUT_MANU_ENABLE	"))	;
	MODBUS_OUTPUT_MANU_ENABLE	=	Get_RegID(_T("	MODBUS_OUTPUT_MANU_ENABLE	"))	;
	MODBUS_DIGITAL_INPUT	=	Get_RegID(_T("	MODBUS_DIGITAL_INPUT	"))	;
	MODBUS_DEAD_MASTER	=	Get_RegID(_T("	MODBUS_DEAD_MASTER	"))	;
	MODBUS_ROUND_DISPLAY	=	Get_RegID(_T("	MODBUS_ROUND_DISPLAY	"))	;
	MODBUS_MIN_ADDRESS 	=	Get_RegID(_T("	MODBUS_MIN_ADDRESS 	"))	;
	MODBUS_MAX_ADDRESS	=	Get_RegID(_T("	MODBUS_MAX_ADDRESS	"))	;
	MODBUS_FAN_GRIDPOINT	=	Get_RegID(_T("	MODBUS_FAN_GRIDPOINT	"))	;
	MODBUS_MODE_GRIDPOINT	=	Get_RegID(_T("	MODBUS_MODE_GRIDPOINT	"))	;
	MODBUS_HOLD_GRIDPOINT	=	Get_RegID(_T("	MODBUS_HOLD_GRIDPOINT	"))	;
	MODBUS_CONFIGURATION	=	Get_RegID(_T("	MODBUS_CONFIGURATION	"))	;
	MODBUS_UTIL_MODE	=	Get_RegID(_T("	MODBUS_UTIL_MODE	"))	;
	MODBUS_EEPROM_SIZE	=	Get_RegID(_T("	MODBUS_EEPROM_SIZE	"))	;
	MODBUS_TIMER_SELECT	=	Get_RegID(_T("	MODBUS_TIMER_SELECT	"))	;
	MODBUS_OUTPUT1_FUNCTION	=	Get_RegID(_T("	MODBUS_OUTPUT1_FUNCTION	"))	;
	MODBUS_ROTATION_OUT2	=	Get_RegID(_T("	MODBUS_ROTATION_OUT2	"))	;
	MODBUS_ROTATION_OUT3	=	Get_RegID(_T("	MODBUS_ROTATION_OUT3	"))	;
	MODBUS_ROTATION_OUT4	=	Get_RegID(_T("	MODBUS_ROTATION_OUT4	"))	;
	MODBUS_ROTATION_OUT5	=	Get_RegID(_T("	MODBUS_ROTATION_OUT5	"))	;
	MODBUS_OUTPUT2_FUNCTION	=	Get_RegID(_T("	MODBUS_OUTPUT2_FUNCTION	"))	;
	MODBUS_OUTPUT3_FUNCTION	=	Get_RegID(_T("	MODBUS_OUTPUT3_FUNCTION	"))	;
	MODBUS_OUTPUT4_FUNCTION	=	Get_RegID(_T("	MODBUS_OUTPUT4_FUNCTION	"))	;
	MODBUS_OUTPUT5_FUNCTION	=	Get_RegID(_T("	MODBUS_OUTPUT5_FUNCTION	"))	;
	MODBUS_PIR_STAGE	=	Get_RegID(_T("	MODBUS_PIR_STAGE	"))	;
	MODBUS_AIR_CALIBRATION_POINT4	=	Get_RegID(_T("	MODBUS_AIR_CALIBRATION_POINT4	"))	;
	MODBUS_AIR_CALIBRATION_POINT1	=	Get_RegID(_T("	MODBUS_AIR_CALIBRATION_POINT1	"))	;
	MODBUS_AIR_CALIBRATION_POINT2	=	Get_RegID(_T("	MODBUS_AIR_CALIBRATION_POINT2	"))	;
	MODBUS_AIR_CALIBRATION_POINT3	=	Get_RegID(_T("	MODBUS_AIR_CALIBRATION_POINT3	"))	;
	MODUBS_HUMIDITY_RH 	=	Get_RegID(_T("	MODUBS_HUMIDITY_RH 	"))	;
	MODBUS_HUMIDITY_FREQUENCY	=	Get_RegID(_T("	MODBUS_HUMIDITY_FREQUENCY	"))	;
	MODBUS_HUM_PIC_UPDATE	=	Get_RegID(_T("	MODBUS_HUM_PIC_UPDATE	"))	;
	MODBUS_HUM_CAL_NUM 	=	Get_RegID(_T("	MODBUS_HUM_CAL_NUM 	"))	;
	MODBUS_HUM_CAL_EREASE 	=	Get_RegID(_T("	MODBUS_HUM_CAL_EREASE 	"))	;
	MODBUS_HUM_CURRENT_DEFAULT 	=	Get_RegID(_T("	MODBUS_HUM_CURRENT_DEFAULT 	"))	;
	MODBUS_HUM_PICDATAOK 	=	Get_RegID(_T("	MODBUS_HUM_PICDATAOK 	"))	;
	MODBUS_HUM_OFFSETFLAG  	=	Get_RegID(_T("	MODBUS_HUM_OFFSETFLAG  	"))	;
	MODBUS_HUMCOUNT1_H   	=	Get_RegID(_T("	MODBUS_HUMCOUNT1_H   	"))	;
	MODBUS_HUMRH1_H	=	Get_RegID(_T("	MODBUS_HUMRH1_H	"))	;
	MODBUS_HUMCOUNT2_H 	=	Get_RegID(_T("	MODBUS_HUMCOUNT2_H 	"))	;
	MODBUS_HUMRH2_H	=	Get_RegID(_T("	MODBUS_HUMRH2_H	"))	;
	MODBUS_HUMCOUNT3_H 	=	Get_RegID(_T("	MODBUS_HUMCOUNT3_H 	"))	;
	MODBUS_HUMRH3_H	=	Get_RegID(_T("	MODBUS_HUMRH3_H	"))	;
	MODBUS_HUMCOUNT4_H 	=	Get_RegID(_T("	MODBUS_HUMCOUNT4_H 	"))	;
	MODBUS_HUMRH4_H	=	Get_RegID(_T("	MODBUS_HUMRH4_H	"))	;
	MODBUS_HUMCOUNT5_H 	=	Get_RegID(_T("	MODBUS_HUMCOUNT5_H 	"))	;
	MODBUS_HUMRH5_H	=	Get_RegID(_T("	MODBUS_HUMRH5_H	"))	;
	MODBUS_HUMCOUNT6_H 	=	Get_RegID(_T("	MODBUS_HUMCOUNT6_H 	"))	;
	MODBUS_HUMRH6_H	=	Get_RegID(_T("	MODBUS_HUMRH6_H	"))	;
	MODBUS_HUMCOUNT7_H 	=	Get_RegID(_T("	MODBUS_HUMCOUNT7_H 	"))	;
	MODBUS_HUMRH7_H	=	Get_RegID(_T("	MODBUS_HUMRH7_H	"))	;
	MODBUS_HUMCOUNT8_H 	=	Get_RegID(_T("	MODBUS_HUMCOUNT8_H 	"))	;
	MODBUS_HUMRH8_H	=	Get_RegID(_T("	MODBUS_HUMRH8_H	"))	;
	MODBUS_HUMCOUNT9_H 	=	Get_RegID(_T("	MODBUS_HUMCOUNT9_H 	"))	;
	MODBUS_HUMRH9_H	=	Get_RegID(_T("	MODBUS_HUMRH9_H	"))	;
	MODBUS_HUMCOUNT10_H 	=	Get_RegID(_T("	MODBUS_HUMCOUNT10_H 	"))	;
	MODBUS_HUMRH10_H	=	Get_RegID(_T("	MODBUS_HUMRH10_H	"))	;
	MODBUS_TEM_RANGE_LOWER  	=	Get_RegID(_T("	MODBUS_TEM_RANGE_LOWER  	"))	;
	MODBUS_TEM_RANGE_UPPER	=	Get_RegID(_T("	MODBUS_TEM_RANGE_UPPER	"))	;
	MODBUS_HUM_RANGE_LOWER	=	Get_RegID(_T("	MODBUS_HUM_RANGE_LOWER	"))	;
	MODBUS_HUM_RANGE_UPPER	=	Get_RegID(_T("	MODBUS_HUM_RANGE_UPPER	"))	;
	MODBUS_AQ_RANGE_LOWER	=	Get_RegID(_T("	MODBUS_AQ_RANGE_LOWER	"))	;
	MODBUS_AQ_RANGE_UPPER 	=	Get_RegID(_T("	MODBUS_AQ_RANGE_UPPER 	"))	;
	MODBUS_TEMP_CURRENT_OUTPUT      	=	Get_RegID(_T("	MODBUS_TEMP_CURRENT_OUTPUT      	"))	;
	MODBUS_HUM_CURRENT_OUTPUT	=	Get_RegID(_T("	MODBUS_HUM_CURRENT_OUTPUT	"))	;
	MODBUS_AQ_CURRENT_OUTPUT	=	Get_RegID(_T("	MODBUS_AQ_CURRENT_OUTPUT	"))	;
	MODBUS_TEMP_VOLTAGE_OUTPUT	=	Get_RegID(_T("	MODBUS_TEMP_VOLTAGE_OUTPUT	"))	;
	MODBUS_HUM_VOLTAGE_OUTPUT	=	Get_RegID(_T("	MODBUS_HUM_VOLTAGE_OUTPUT	"))	;
	MODBUS_AQ_VOLTAGE_OUTPUT  	=	Get_RegID(_T("	MODBUS_AQ_VOLTAGE_OUTPUT  	"))	;
	MODBUS_CALIBRATION_MODE  	=	Get_RegID(_T("	MODBUS_CALIBRATION_MODE  	"))	;
	MODBUS_SCROLL_MODE	=	Get_RegID(_T("	MODBUS_SCROLL_MODE	"))	;
	MODBUS_AQ_LEVEL0	=	Get_RegID(_T("	MODBUS_AQ_LEVEL0	"))	;
	MODBUS_AQ_LEVEL1	=	Get_RegID(_T("	MODBUS_AQ_LEVEL1	"))	;
	MODBUS_AQ_LEVEL2	=	Get_RegID(_T("	MODBUS_AQ_LEVEL2	"))	;
	MODBUS_OUTPUT_MODE	=	Get_RegID(_T("	MODBUS_OUTPUT_MODE	"))	;
	MODBUS_TEMP_MANU_OUTPUT	=	Get_RegID(_T("	MODBUS_TEMP_MANU_OUTPUT	"))	;
	MODBUS_HUM_MANU_OUTPUT	=	Get_RegID(_T("	MODBUS_HUM_MANU_OUTPUT	"))	;
	MODBUS_AQ_MANU_OUTPUT	=	Get_RegID(_T("	MODBUS_AQ_MANU_OUTPUT	"))	;
	MODBUS_HUM_CALIBRATION_LOCKX    	=	Get_RegID(_T("	MODBUS_HUM_CALIBRATION_LOCKX    	"))	;
	MODBUS_HUM_CALIBRATION_LOCKY 	=	Get_RegID(_T("	MODBUS_HUM_CALIBRATION_LOCKY 	"))	;
}	  
int  CAirQuality::Get_RegID(CString Name)
{
	Name.TrimLeft();
	Name.TrimRight();
	int regid=-1;
	vector<T3Register>::iterator iter;
	for (iter=m_vecT3Register.begin();iter!=m_vecT3Register.end();iter++)
	{
		if (iter->regName==Name)
		{
			regid=iter->regID;
			break;
		}
	}
	return regid;
}



void CAirQuality::OnEnKillfocustempBacklight(){
	m_fresh_data=FALSE;
	CString strTemp;
	m_edit_backlightTime.GetWindowText(strTemp);
	if(strTemp.IsEmpty())
	{m_fresh_data=TRUE;  return;}
	int backlighttime=-1;

	backlighttime=_wtoi(strTemp);

	if (backlighttime>=0&&backlighttime<=255)
	{

	}
	else
	{
		AfxMessageBox(_T("Please input the value>=0&&<=255"));
		return;
	}
	if(product_register_value[361]==_wtoi(strTemp))	//Add this to judge weather this value need to change.
	{ m_fresh_data=TRUE; return;}

	unsigned short TempShort=(unsigned short)_wtoi(strTemp);
	int ret=write_one(g_tstat_id,361,TempShort);
	if (ret>0)
	{
		product_register_value[361]=TempShort;
		ShowAirqualityDialog();
	}
	else
	{
		AfxMessageBox(_T("Write Fail!"));
	}
	m_fresh_data=TRUE;
}

void CAirQuality::OnEnChangeBacklight(){
	int sel = m_combox_backlightTime.GetCurSel();
	if (0 == sel)
	{
		m_edit_backlightTime.SetWindowTextW(_T("0"));
	}

	if (1 == sel)
	{
		m_edit_backlightTime.SetWindowTextW(_T("255"));
	}
	OnEnKillfocustempBacklight();

}







#include "../EreaseDlg.h"
void CAirQuality::OnBnClickedButton_Change_ID(){
	CEreaseDlg Dlg;
	Dlg.DoModal();
}

void CAirQuality::OnBnClickedButton_Factory_Reset(){


	if(AfxMessageBox(_T(" This will reset the module to the factory defaults,Are you sure to reset it ?"))==IDOK)
	{
		int ret = write_one(g_tstat_id,311,1);
		if (ret > 0 )
		{
			AfxMessageBox(_T("Factory Reset Successful!"));

			unsigned short Data[20];
			ret = Read_Multi(g_tstat_id,Data,MODBUS_HUMCOUNT1_H,20);
			if (ret>0)
			{
				for (int i = 0 ; i < 20 ; i++)
				{
					product_register_value[MODBUS_HUMCOUNT1_H + i] = Data[ i ];
				}
			}
			Initial_UserList(); 

		}
	}

}


void CAirQuality::OnBnClickedButton_UserTable_Add(){
	if (product_register_value[MODBUS_HUM_CAL_NUM] >= 10)
	{
		AfxMessageBox(_T("Can't Add,Current Point can't be more than 10!"));
		return;
	}
	int ret = write_one(g_tstat_id,MODBUS_HUM_CAL_NUM,product_register_value[MODBUS_HUM_CAL_NUM]+1);
	if (ret>0)
	{
		++product_register_value[MODBUS_HUM_CAL_NUM];
	}
	else
	{
		AfxMessageBox(_T("Add Fail,Try again!"));
		return;
	}

	unsigned short Data[20];
	ret = Read_Multi(g_tstat_id,Data,MODBUS_HUMCOUNT1_H,20);
	if (ret>0)
	{
		for (int i = 0 ; i < 20 ; i++)
		{
			product_register_value[MODBUS_HUMCOUNT1_H + i] = Data[ i ];
		}
	}
	Initial_UserList();
	InitChartData();
	//InitChartCtrl();
	UpdatePoint();
}
void CAirQuality::OnBnClickedButton_UserTable_Del(){
	if (product_register_value[MODBUS_HUM_CAL_NUM] <= 0)
	{
		AfxMessageBox(_T("Can't Add,Current Point can't be less than 0!"));
		return;
	}
	int ret = write_one(g_tstat_id,MODBUS_HUM_CAL_NUM,product_register_value[MODBUS_HUM_CAL_NUM]-1);
	if (ret>0)
	{
		--product_register_value[MODBUS_HUM_CAL_NUM];

		int RegAddress;
		//int row =m_sensor_calibrate_table.get_Rows();
		int row =m_user_list.GetItemCount();
		row -= 1;//末尾行
		RegAddress=MODBUS_HUMCOUNT1_H+2*row+1;
		int ret=write_one(g_tstat_id,RegAddress,0);


	}
	else
	{
		AfxMessageBox(_T("Add Fail,Try again!"));
		return;
	}

	unsigned short Data[20];
	ret = Read_Multi(g_tstat_id,Data,MODBUS_HUMCOUNT1_H,20);
	if (ret>0)
	{
		for (int i = 0 ; i < 20 ; i++)
		{
			product_register_value[MODBUS_HUMCOUNT1_H + i] = Data[ i ];
		}
	}
	Initial_UserList();
	InitChartData();
 
	UpdatePoint();


}

LRESULT CAirQuality::WindowProc(UINT message, WPARAM wParam, LPARAM lParam)
{

	// 
	if(WM_AIRQUANITY_FRESH==message){
		 Fresh_Input_Output();
	}
	if (MY_FRESH_DRAW_GRAPHIC==message)
	{
		UpdatePoint();
	}
	return CFormView::WindowProc(message, wParam, lParam);
}

BOOL CAirQuality::PreTranslateMessage(MSG* pMsg)
{
	// TODO: Add your specialized code here and/or call the base class
	if(pMsg->message == WM_KEYDOWN  )
	{
		if(pMsg->wParam == VK_RETURN)
		{
			//CWnd *temp_focus=GetFocus();	//Maurice require ,click enter and the cursor still in this edit or combobox.
			////GetDlgItem(IDC_REFRESHBUTTON)->SetFocus();
			//temp_focus->OnKillFocus();
			CWnd *temp_focus=GetFocus();	//Maurice require ,click enter and the cursor still in this edit or combobox.
			GetDlgItem(IDC_EDIT1_TEST)->SetFocus();
			temp_focus->SetFocus();

			return 1;
		}
	}

	return CFormView::PreTranslateMessage(pMsg);
}


// void CAirQuality::ClickMsflexgrid1()
// { 
// 
// 
// 	m_sensor_calibrate_table.SetFocus();
// 	long lRow,lCol;
// 	lRow = m_sensor_calibrate_table.get_RowSel();//获取点击的行号	
// 	lCol = m_sensor_calibrate_table.get_ColSel(); //获取点击的列号
// 	if(lRow>m_sensor_calibrate_table.get_Rows()) //如果点击区超过最大行号，则点击是无效的
// 		return;
// 	if(lRow == 0) //如果点击标题行，也无效
// 		return;
// 	CRect rect;
// 	m_sensor_calibrate_table.GetWindowRect(rect); //获取表格控件的窗口矩形
// 	ScreenToClient(rect); //转换为客户区矩形	
// 	// MSFlexGrid控件的函数的长度单位是"缇(twips)"，
// 	//需要将其转化为像素，1440缇= 1英寸
// 	CDC* pDC =GetDC();
// 	//计算象素点和缇的转换比例
// 	int nTwipsPerDotX = 1440 / pDC->GetDeviceCaps(LOGPIXELSX) ;
// 	int nTwipsPerDotY = 1440 / pDC->GetDeviceCaps(LOGPIXELSY) ;
// 	//计算选中格的左上角的坐标(象素为单位)
// 	long y = m_sensor_calibrate_table.get_RowPos(lRow)/nTwipsPerDotY;
// 	long x = m_sensor_calibrate_table.get_ColPos(lCol)/nTwipsPerDotX;
// 	//计算选中格的尺寸(象素为单位)。加1是实际调试中，发现加1后效果更好
// 	long width = m_sensor_calibrate_table.get_ColWidth(lCol)/nTwipsPerDotX+1;
// 	long height = m_sensor_calibrate_table.get_RowHeight(lRow)/nTwipsPerDotY+1;
// 	//形成选中个所在的矩形区域
// 	CRect rc(x,y,x+width,y+height);
// 	//转换成相对对话框的坐标
// 	rc.OffsetRect(rect.left+1,rect.top+1);
// 	//获取选中格的文本信息
// 	CString strValue = m_sensor_calibrate_table.get_TextMatrix(lRow,lCol);
// 	m_nCurRow=lRow;
// 	m_nCurCol=lCol;
// 
// 	if(0!=lCol&&0!=lRow)
// 	{
// 
// 		if(DISABLE_COLOR_CELL==m_sensor_calibrate_table.get_CellBackColor())
// 			return;
// 		if (1==lCol)
// 		{
// 			return;
// 		}
// 		m_edit_calibration.ShowWindow(SW_SHOW);	
// 		m_edit_calibration.SetWindowText(strValue);
// 		m_edit_calibration.MoveWindow(rc); //移动到选中格的位置，覆盖
// 		m_edit_calibration.BringWindowToTop();
// 		m_edit_calibration.SetFocus(); //获取焦点
// 
// 	}
// }

void CAirQuality::InitChartData(){
	m_Xmax=0;
	m_Xmin=0;
	m_Ymax=0;
	m_Ymin=0;
	m_Draw_User=3;
	int dataline = 0;
	Registers_Infor temp;
	RegPoint temppoint;  
	g_vectRegisters.clear();
	int rows = product_register_value[MODBUS_HUM_CAL_NUM];
	if (rows == 1)
	{
		m_Draw_User=1;
		m_offset = (short)product_register_value[MODBUS_CALIBRATION_HUM] - 500;
	}
	else if(rows == 0)
	{
		m_Draw_User=0;
	}
	else{
		m_Draw_User =3;
	}
	if (m_Draw_User == 1)
	{

		rows = Default_Value[3];
		temp.Reg_Name=_T("Factory Table Line");
		temp.Point_Color = RGB(0,100,255);
		for (int row=1;row<=rows;row++)
		{
			Factory_Xdata[row-1]=(short)Default_Value[4+2*row-1];
			temppoint.Time_offset=(short)Default_Value[4+2*row-1];
			Factory_Ydata[row-1]=(short)Default_Value[4+2*(row-1)];

			temppoint.Value_offset =(short)Default_Value[4+2*(row-1)];

			temp.Reg_Point.push_back(temppoint);
		}
		g_vectRegisters.push_back(temp);
		temp.Reg_Point.clear();

		m_Xmax = Factory_Xdata[0];
		m_Xmin = Factory_Xdata[0];
		m_Ymax = Factory_Ydata[0];
		m_Ymin = Factory_Ydata[0];

		for (int row=0;row<rows;row++)
		{
			if (m_Xmax < Factory_Xdata[row])
			{
				m_Xmax = Factory_Xdata[row];
			}

			if (m_Xmin > Factory_Xdata[row])
			{
				m_Xmin = Factory_Xdata[row];
			}

			if (m_Ymax < Factory_Ydata[row])
			{
				m_Ymax = Factory_Ydata[row];
			}

			if (m_Ymin > Factory_Ydata[row])
			{
				m_Ymin = Factory_Ydata[row];
			}

		}

		temp.Reg_Name=_T("User Table Line");
		temp.Point_Color = RGB(255,0,0);

		for (int i=0;i<rows;i++)
		{
			User_Xdata[i]=(short)Factory_Xdata[i];
			temppoint.Time_offset =(short)Factory_Xdata[i]   ;
			User_Ydata[i]=(short)Factory_Ydata[i]  + m_offset;//
			temppoint.Value_offset =(short)Factory_Ydata[i] + m_offset;

			temp.Reg_Point.push_back(temppoint);
		}
		g_vectRegisters.push_back(temp);

		for (int row=0;row<rows;row++)
		{

			if (m_Ymax < User_Ydata[row])
			{
				m_Ymax = User_Ydata[row];
			}

			if (m_Ymin > User_Ydata[row])
			{
				m_Ymin = User_Ydata[row];
			}

		}

	}

	else if (m_Draw_User == 0)
	{
		rows = Default_Value[3];
		temp.Reg_Name=_T("Factory Table Line");
		temp.Point_Color = RGB(0,100,255);
		for (int row=1;row<=rows;row++)
		{
			Factory_Xdata[row-1]=Default_Value[4+2*row-1];
			temppoint.Time_offset=Default_Value[4+2*row-1];
			Factory_Ydata[row-1]=Default_Value[4+2*(row-1)];

			temppoint.Value_offset = Default_Value[4+2*(row-1)];

			temp.Reg_Point.push_back(temppoint);
		}
		g_vectRegisters.push_back(temp);
		temp.Reg_Point.clear();

		m_Xmax = Factory_Xdata[0];
		m_Xmin = Factory_Xdata[0];
		m_Ymax = Factory_Ydata[0];
		m_Ymin = Factory_Ydata[0];

		for (int row=0;row<rows;row++)
		{
			if (m_Xmax < Factory_Xdata[row])
			{
				m_Xmax = Factory_Xdata[row];
			}

			if (m_Xmin > Factory_Xdata[row])
			{
				m_Xmin = Factory_Xdata[row];
			}

			if (m_Ymax < Factory_Ydata[row])
			{
				m_Ymax = Factory_Ydata[row];
			}

			if (m_Ymin > Factory_Ydata[row])
			{
				m_Ymin = Factory_Ydata[row];
			}

		}
	}
	else	//
	{
		temp.Reg_Name=_T("User Table Line");
		temp.Point_Color = RGB(255,0,0);
		for (int row=1;row<=rows;row++) 
		{
			if (product_register_value[MODBUS_HUMCOUNT1_H+2*(row-1)] == 0)// Frequence is zero ,continue
			{
				continue;
			}

			User_Xdata[row-1]=product_register_value[MODBUS_HUMCOUNT1_H+2*(row-1)];

			temppoint.Time_offset = product_register_value[MODBUS_HUMCOUNT1_H+2*(row-1)];
			User_Ydata[row-1]=product_register_value[MODBUS_HUMCOUNT1_H+2*row-1];
			temppoint.Value_offset = product_register_value[MODBUS_HUMCOUNT1_H+2*row-1];

			temp.Reg_Point.push_back(temppoint);
			dataline++;
		}

		g_vectRegisters.push_back(temp);
		temp.Reg_Point.clear();
		m_Xmax = User_Xdata[0];
		m_Xmin = User_Xdata[0];
		m_Ymax = User_Ydata[0];
		m_Ymin = User_Ydata[0];


		for (int row=0;row<dataline;row++)
		{
			if (User_Xdata[row] == 0)
			{
				continue;
			}
			if (m_Xmax < User_Xdata[row])
			{
				m_Xmax = User_Xdata[row];
			}

			if (m_Xmin > User_Xdata[row])
			{
				m_Xmin = User_Xdata[row];
			}

			if (m_Ymax < User_Ydata[row])
			{
				m_Ymax = User_Ydata[row];
			}

			if (m_Ymin > User_Ydata[row])
			{
				m_Ymin = User_Ydata[row];
			}

		}


		temp.Reg_Name=_T("Factory Table Line");
		temp.Point_Color = RGB(0,100,255);

		rows = Default_Value[3];
		dataline = 0;
		for (int row=1;row<=rows;row++)
		{
			if (Default_Value[4+2*row-1] == 0)
			{
				continue;
			}
			Factory_Xdata[row-1]=Default_Value[4+2*row-1];
			temppoint.Time_offset=Default_Value[4+2*row-1];
			Factory_Ydata[row-1]=Default_Value[4+2*(row-1)];
			temppoint.Value_offset =Default_Value[4+2*(row-1)];

			temp.Reg_Point.push_back(temppoint);
			dataline++;
		}
		g_vectRegisters.push_back(temp);
		temp.Reg_Point.clear();
		for (int row=0;row<dataline;row++)
		{
			if (Factory_Xdata[row] ==0)
			{
				continue;
			}
			if (m_Xmax < Factory_Xdata[row])
			{
				m_Xmax = Factory_Xdata[row];
			}

			if (m_Xmin > Factory_Xdata[row])
			{
				m_Xmin = Factory_Xdata[row];
			}

			if (m_Ymax < Factory_Ydata[row])
			{
				m_Ymax = Factory_Ydata[row];
			}

			if (m_Ymin > Factory_Ydata[row])
			{
				m_Ymin = Factory_Ydata[row];
			}

		}


	}

	while (m_Xmax%10 != 0)
	{
		m_Xmax++;
	}
	while (m_Xmin%10 != 0)
	{
		m_Xmin--;
	}

	while (m_Ymax%10 != 0)
	{
		m_Ymax++;
	}
	while (m_Ymin%10 != 0)
	{
		m_Ymin--;
	}



}



void CAirQuality::InitChartCtrl(){
	CChartTitle* pTitle = m_ChartCtrl.GetTitle();
	pTitle->RemoveAll();
	pTitle->AddString(_T("Frequency Humidity Graph"));
	pTitle->SetColor(RGB(255,0,0));

	CChartStandardAxis* pBottomAxis = 
		m_ChartCtrl.CreateStandardAxis(CChartCtrl::BottomAxis);
	pBottomAxis->SetMinMax(m_Xmin, m_Xmax);

	CChartStandardAxis* pLeftAxis =
		m_ChartCtrl.CreateStandardAxis(CChartCtrl::LeftAxis);
	//pLeftAxis->SetMinMax(m_Ymin, m_Ymax);
	pLeftAxis->SetMinMax(0, 1000);
	CChartStandardAxis* pTopAxis =
		m_ChartCtrl.CreateStandardAxis(CChartCtrl::TopAxis);
	pTopAxis->SetMinMax(m_Xmin, m_Xmax);

	CChartStandardAxis* pRightAxis =
		m_ChartCtrl.CreateStandardAxis(CChartCtrl::RightAxis);
	//pRightAxis->SetMinMax(m_Ymin, m_Ymax);
	pRightAxis->SetMinMax(0, 1000);
	m_ChartCtrl.GetLegend()->SetVisible(true);
	m_ChartCtrl.RefreshCtrl();
}

void CAirQuality::DrawerPoint(){
	int Type=0;

	//
	for (int i=0;i<g_vectRegisters.size();i++)
	{
		bool bSecondHorizAxis =true;
		bool bSecondVertAxis =true;




		switch (Type)
		{
			// Line series
		case 0:
			{
				bool bSecondHorizAxis = TRUE;
				bool bSecondVertAxis = TRUE;
				CChartLineSerie* pLineSeries = m_ChartCtrl.CreateLineSerie(bSecondHorizAxis, bSecondVertAxis);
				pSeries[i] = pLineSeries;


				/*
				实心;下划线;点;下划线-点;下划线-点-点;
				*/
				pLineSeries->SetWidth(3);

				pLineSeries->SetPenStyle(0);
			}
			break;

			// Points series
		case 1:
			{
				bool bSecondHorizAxis = TRUE;
				bool bSecondVertAxis = TRUE;
				// 				CChartPointsSerie* pPointsSeries = m_ChartCtrl.CreatePointsSerie(bSecondHorizAxis, bSecondVertAxis);
				// 				pSeries = pPointsSeries;
				// 
				// 				CPointsPropDialog PointsPropDlg;
				// 				PointsPropDlg.DoModal();
				// 
				// 				switch (PointsPropDlg.m_iPointsType)
				// 				{
				// 				case 0:
				// 					pPointsSeries->SetPointType(CChartPointsSerie::ptEllipse);
				// 					break;
				// 
				// 				case 1:
				// 					pPointsSeries->SetPointType(CChartPointsSerie::ptRectangle);
				// 					break;
				// 
				// 				case 2:
				// 					pPointsSeries->SetPointType(CChartPointsSerie::ptTriangle);
				// 					break;
				// 				}
				// 
				// 				pPointsSeries->SetPointSize(PointsPropDlg.m_iPointsWidth,PointsPropDlg.m_iPointsHeight);

				CChartPointsSerie* pPointsSeries = m_ChartCtrl.CreatePointsSerie(bSecondHorizAxis, bSecondVertAxis);
				pSeries[i] = pPointsSeries;
				pPointsSeries->SetPointType(CChartPointsSerie::ptRectangle);
				pPointsSeries->SetPointSize(5,5);

			}
			break;

			// Surface series
		case 2:
			{
				bool bSecondHorizAxis = TRUE;
				bool bSecondVertAxis = TRUE;
				CChartSurfaceSerie* pSurfSeries = m_ChartCtrl.CreateSurfaceSerie(bSecondHorizAxis, bSecondVertAxis);
				pSeries[i] = pSurfSeries;

				// 				CSurfacePropDialog SurfPropDlg;
				// 				SurfPropDlg.DoModal();
				int m_FillStyle=0;
				switch (m_FillStyle)
				{
				case 0:
					pSurfSeries->SetFillStyle(CChartSurfaceSerie::fsSolid);
					break;
				case 1:
					pSurfSeries->SetFillStyle(CChartSurfaceSerie::fsHatchDownDiag);
					break;
				case 2:
					pSurfSeries->SetFillStyle(CChartSurfaceSerie::fsHatchUpDiag);
					break;
				case 3:
					pSurfSeries->SetFillStyle(CChartSurfaceSerie::fsHatchCross);
					break;
				case 4:
					pSurfSeries->SetFillStyle(CChartSurfaceSerie::fsHatchDiagCross);
					break;
				case 5:
					pSurfSeries->SetFillStyle(CChartSurfaceSerie::fsHatchHorizontal);
					break;
				case 6:
					pSurfSeries->SetFillStyle(CChartSurfaceSerie::fsHatchVertical);
					break;
				}

				if (TRUE)
					pSurfSeries->SetHorizontal(true);
				else
					pSurfSeries->SetHorizontal(false);
			}
			break;
		}


		TChartString Name=g_vectRegisters[i].Reg_Name.GetBuffer(0);

		pSeries[i]->SetName(Name);
		pSeries[i]->SetColor(g_vectRegisters[i].Point_Color);

		int NumberPoints =g_vectRegisters[i].Reg_Point.size();


		double* XValues = new double[NumberPoints];
		double* YValues = new double[NumberPoints];





		for (int j=0;j<NumberPoints;j++)
		{
			RegPoint tempPoint=g_vectRegisters[i].Reg_Point[j];
			XValues[j] =tempPoint.Time_offset;
			YValues[j] =tempPoint.Value_offset ;
			CString str;
			str.Format(_T("\nx=%0.1f y=%0.1f\n"),XValues[j],YValues[j]);
			TRACE(str);
		}

		pSeries[i]->SetPoints(XValues,YValues,NumberPoints);


		delete[] XValues;
		delete[] YValues;
		m_ChartCtrl.RefreshCtrl();


	}

}

void CAirQuality::UpdatePoint(){

	m_ChartCtrl.RemoveAllSeries();
	//m_ChartCtrl.RefreshCtrl();
	m_ChartCtrl.GetLeftAxis()->SetMinMax(m_Ymin,m_Ymax);
	m_ChartCtrl.GetRightAxis()->SetMinMax(m_Ymin,m_Ymax);
	m_ChartCtrl.GetBottomAxis()->SetMinMax(m_Xmin,m_Xmax);
	m_ChartCtrl.GetTopAxis()->SetMinMax(m_Xmin,m_Xmax);

	m_ChartCtrl.RefreshCtrl();
	int Type=0;
	for (int i=0;i<g_vectRegisters.size();i++)
	{
		bool bSecondHorizAxis =true;
		bool bSecondVertAxis =true;

		switch (Type)
		{
			// Line series
		case 0:
			{
				bool bSecondHorizAxis = TRUE;
				bool bSecondVertAxis = TRUE;
				CChartLineSerie* pLineSeries = m_ChartCtrl.CreateLineSerie(bSecondHorizAxis, bSecondVertAxis);
				pSeries[i] = pLineSeries;


				/*
				实心;下划线;点;下划线-点;下划线-点-点;
				*/ 
				pLineSeries->SetWidth(3);
				pLineSeries->SetPenStyle(0);
			}
			break;

			// Points series
		case 1:
			{
				bool bSecondHorizAxis = TRUE;
				bool bSecondVertAxis = TRUE;
				// 				CChartPointsSerie* pPointsSeries = m_ChartCtrl.CreatePointsSerie(bSecond								   0HorizAxis, bSecondVertAxis);
				// 				pSeries = pPointsSeries;
				// 
				// 				CPointsPropDialog PointsPropDlg;
				// 				PointsPropDlg.DoModal();
				// 
				// 				switch (PointsPropDlg.m_iPointsType)
				// 				{
				// 				case 0:
				// 					pPointsSeries->SetPointType(CChartPointsSerie::ptEllipse);
				// 					break;
				// 
				// 				case 1:
				// 					pPointsSeries->SetPointType(CChartPointsSerie::ptRectangle);
				// 					break;
				// 
				// 				case 2:
				// 					pPointsSeries->SetPointType(CChartPointsSerie::ptTriangle);
				// 					break;
				// 				}
				// 
				// 				pPointsSeries->SetPointSize(PointsPropDlg.m_iPointsWidth,PointsPropDlg.m_iPointsHeight);

				CChartPointsSerie* pPointsSeries = m_ChartCtrl.CreatePointsSerie(bSecondHorizAxis, bSecondVertAxis);
				pSeries[i] = pPointsSeries;
				pPointsSeries->SetPointType(CChartPointsSerie::ptRectangle);
				pPointsSeries->SetPointSize(5,5);

			}
			break;

			// Surface series
		case 2:
			{
				bool bSecondHorizAxis = TRUE;
				bool bSecondVertAxis = TRUE;
				CChartSurfaceSerie* pSurfSeries = m_ChartCtrl.CreateSurfaceSerie(bSecondHorizAxis, bSecondVertAxis);
				pSeries[i] = pSurfSeries;

				// 				CSurfacePropDialog SurfPropDlg;
				// 				SurfPropDlg.DoModal();
				int m_FillStyle=0;
				switch (m_FillStyle)
				{
				case 0:
					pSurfSeries->SetFillStyle(CChartSurfaceSerie::fsSolid);
					break;
				case 1:
					pSurfSeries->SetFillStyle(CChartSurfaceSerie::fsHatchDownDiag);
					break;
				case 2:
					pSurfSeries->SetFillStyle(CChartSurfaceSerie::fsHatchUpDiag);
					break;
				case 3:
					pSurfSeries->SetFillStyle(CChartSurfaceSerie::fsHatchCross);
					break;
				case 4:
					pSurfSeries->SetFillStyle(CChartSurfaceSerie::fsHatchDiagCross);
					break;
				case 5:
					pSurfSeries->SetFillStyle(CChartSurfaceSerie::fsHatchHorizontal);
					break;
				case 6:
					pSurfSeries->SetFillStyle(CChartSurfaceSerie::fsHatchVertical);
					break;
				}

				if (TRUE)
					pSurfSeries->SetHorizontal(true);
				else
					pSurfSeries->SetHorizontal(false);
			}
			break;
		}

		TChartString Name=g_vectRegisters[i].Reg_Name.GetBuffer(0);
		pSeries[i]->SetName(Name);
		pSeries[i]->SetColor(g_vectRegisters[i].Point_Color);

		int NumberPoints =g_vectRegisters[i].Reg_Point.size();


		double* XValues = new double[NumberPoints];
		double* YValues = new double[NumberPoints];

		for (int j=0;j<NumberPoints;j++)
		{
			RegPoint tempPoint=g_vectRegisters[i].Reg_Point[j];
			XValues[j] =tempPoint.Time_offset;
			YValues[j] =tempPoint.Value_offset;
		}

		pSeries[i]->SetPoints(XValues,YValues,NumberPoints);
		delete[] XValues;
		delete[] YValues;
		m_ChartCtrl.RefreshCtrl();

	}

}


void CAirQuality::Initial_InputList(){
	CString strTemp;
	m_input_list.ShowWindow(SW_HIDE);
	m_input_list.DeleteAllItems();
	while ( m_input_list.DeleteColumn (0)) ;

	m_input_list.ModifyStyle(0, LVS_SINGLESEL|LVS_REPORT|LVS_SHOWSELALWAYS);
	m_input_list.SetExtendedStyle(m_input_list.GetExtendedStyle()  |LVS_EX_GRIDLINES&(~LVS_EX_FULLROWSELECT));	
	m_input_list.InsertColumn(0, _T("NUM"), 50, ListCtrlEx::Normal, LVCFMT_CENTER, ListCtrlEx::SortByDigit);
	m_input_list.InsertColumn(1, _T("Full Label"), 70, ListCtrlEx::Normal, LVCFMT_CENTER, ListCtrlEx::SortByString);
	m_input_list.InsertColumn(2, _T("Auto/Manual"), 80, ListCtrlEx::Normal, LVCFMT_CENTER, ListCtrlEx::SortByString);
	m_input_list.InsertColumn(3, _T("Value"), 50, ListCtrlEx::EditBox, LVCFMT_CENTER, ListCtrlEx::SortByString);
	m_input_list.InsertColumn(4, _T("Range"), 60, ListCtrlEx::Normal, LVCFMT_CENTER, ListCtrlEx::SortByString);
	m_input_list.InsertColumn(5, _T("Filter"), 60, ListCtrlEx::EditBox, LVCFMT_CENTER, ListCtrlEx::SortByString);
	g_hwnd_now = this->m_hWnd;

	m_input_list.InsertItem(0,_T("1"));
	m_input_list.InsertItem(1,_T("2"));

	m_input_list.InsertItem(2,_T("3"));

	m_input_list.SetItemText(0,1,_T("Temperature"));
	m_input_list.SetItemText(1,1,_T("Humidity"));

	bitset<16> AM(product_register_value[349]);

	if (!AM[0])
	{
		m_input_list.SetItemText(0,2,Global_String_AUTO);

		if(product_register_value[MODBUS_DEGC_OR_F]==0)
		{
			strTemp.Format(_T("%0.1f"),((float)((short)product_register_value[MODBUS_TEMPRATURE_CHIP]))/10.0);
			m_input_list.SetItemText(0,3,strTemp);
			m_input_list.SetItemText(0,4,Global_String_C);
		} 
		else
		{
			strTemp.Format(_T("%0.1f"),((float)((short)product_register_value[MODBUS_TEMPRATURE_CHIP_F]))/10.0);
			m_input_list.SetItemText(0,3,strTemp);
			m_input_list.SetItemText(0,4,Glocal_String_F);
		}
	}
	else
	{
		m_input_list.SetItemText(0,2,Global_String_MANUAL);

		strTemp.Format(_T("%0.1f"),((float)((short)product_register_value[350]))/10.0);
		m_input_list.SetItemText(0,3,strTemp);
		if(product_register_value[MODBUS_DEGC_OR_F]==0)
		{

			m_input_list.SetItemText(0,4,Global_String_C);
		} 
		else
		{

			m_input_list.SetItemText(0,4,Glocal_String_F);
		}


	}

	if (!AM[1])
	{
		m_input_list.SetItemText(1,2,Global_String_AUTO);
		strTemp.Format(_T("%0.1f"),(float)(short)product_register_value[MODBUS_HUM]/10.0);
		m_input_list.SetItemText(1,3,strTemp);
	}
	else
	{
		m_input_list.SetItemText(1,2,Global_String_MANUAL);
		strTemp.Format(_T("%0.1f"),(float)(short)product_register_value[351]/10.0);
		m_input_list.SetItemText(1,3,strTemp);
	}

	strTemp.Format(_T("%d"),product_register_value[MODBUS_OVERRIDE_TIMER_LEFT]);
	m_input_list.SetItemText(0,5,strTemp);
	strTemp.Format(_T("%d"),product_register_value[MODBUS_HUM_FILTER]);
	m_input_list.SetItemText(1,5,strTemp);

	if (product_register_value[7]!=PM_HUM_R)
	{
		m_input_list.SetItemText(2,1,_T("Air Quanlity"));
		if (!AM[2])
		{
			m_input_list.SetItemText(2,2,Global_String_AUTO);

			strTemp.Format(_T("%d"),product_register_value[MODBUS_AQ]);
			m_input_list.SetItemText(2,3,strTemp);

		} 
		else
		{
			m_input_list.SetItemText(2,2,Global_String_MANUAL);

			strTemp.Format(_T("%d"),product_register_value[352]);
			m_input_list.SetItemText(2,3,strTemp);

		}
		strTemp.Format(_T("%d"),product_register_value[MODBUS_AQ_FILTER]);	 
		m_input_list.SetItemText(2,5,strTemp );

	}

	m_input_list.ShowWindow(SW_SHOW);

}


void CAirQuality::Initial_OutputList(){

	CString strTemp;
	m_output_list.ShowWindow(SW_HIDE);
	m_output_list.DeleteAllItems();
	while ( m_output_list.DeleteColumn (0)) ;

	m_output_list.ModifyStyle(0, LVS_SINGLESEL|LVS_REPORT|LVS_SHOWSELALWAYS);
	m_output_list.SetExtendedStyle(m_output_list.GetExtendedStyle()  |LVS_EX_GRIDLINES&(~LVS_EX_FULLROWSELECT));	
	m_output_list.InsertColumn(0, _T("NUM"), 50, ListCtrlEx::Normal, LVCFMT_CENTER, ListCtrlEx::SortByDigit);
	m_output_list.InsertColumn(1, _T("Full Label"), 70, ListCtrlEx::Normal, LVCFMT_CENTER, ListCtrlEx::SortByString);
	m_output_list.InsertColumn(2, _T("Value"), 50, ListCtrlEx::Normal, LVCFMT_CENTER, ListCtrlEx::SortByString);
	m_output_list.InsertColumn(3, _T("Range"), 50, ListCtrlEx::Normal, LVCFMT_CENTER, ListCtrlEx::SortByString);
	m_output_list.InsertColumn(4, _T("Min Out Scale"), 80, ListCtrlEx::EditBox, LVCFMT_CENTER, ListCtrlEx::SortByString);
	m_output_list.InsertColumn(5, _T("Max Out Scale"), 100, ListCtrlEx::EditBox, LVCFMT_CENTER, ListCtrlEx::SortByString);


	g_hwnd_now = this->m_hWnd;
	m_output_list.InsertItem(0,_T("1"));
	m_output_list.InsertItem(1,_T("2"));
	m_output_list.InsertItem(2,_T("3"));
	m_output_list.SetItemText(0,1,_T("Temperature"));
	m_output_list.SetItemText(1,1,_T("Humidity"));

	if (product_register_value[7]!=PM_HUM_R)
	{
		m_output_list.SetItemText(2,1,_T("Air Quanlity"));
	}

	strTemp.Format(_T("%0.1f"),(float)(short)product_register_value[MODBUS_TEM_RANGE_LOWER]/10.0);
	m_output_list.SetItemText(0,4,strTemp);

	strTemp.Format(_T("%0.1f"),(float)(short)product_register_value[MODBUS_TEM_RANGE_UPPER]/10.0);
	m_output_list.SetItemText(0,5,strTemp);
	strTemp.Format(_T("%0.1f"),(float)(short)product_register_value[MODBUS_HUM_RANGE_LOWER]/10.0);
	m_output_list.SetItemText(1,4,strTemp);
	strTemp.Format(_T("%0.1f"),(float)(short)product_register_value[MODBUS_HUM_RANGE_UPPER]/10.0);
	m_output_list.SetItemText(1,5,strTemp);

	int output_range=product_register_value[MODBUS_OUTPUT_RANGE];
	if (output_range==1){
		strTemp=_T("0-10v");
	}
	else if (output_range==2)
	{
		strTemp=_T("0-5v");
	}
	else if (output_range==3)
	{
		strTemp=_T("4-20mA");
	}
	else
	{
		strTemp=_T("");
	}
	m_output_list.SetItemText(0,3,strTemp);
	m_output_list.SetItemText(1,3,strTemp);
	if (product_register_value[7]!=PM_HUM_R)
	{
		m_output_list.SetItemText(2,3,strTemp);
	}
	if (output_range==1||output_range==2)
	{
		strTemp.Format(_T("%0.2f V"),(float)(short)product_register_value[MODBUS_TEMP_VOLTAGE_OUTPUT]/100.0);
		m_output_list.SetItemText(0,2,strTemp); 
		strTemp.Format(_T("%0.2f V"),(float)(short)product_register_value[MODBUS_HUM_VOLTAGE_OUTPUT]/100.0);
		m_output_list.SetItemText(1,2,strTemp);  

	} 
	else
	{
		strTemp.Format(_T("%0.1f mA"),(float)(short)product_register_value[MODBUS_TEMP_CURRENT_OUTPUT]/10.0);
		m_output_list.SetItemText(0,2,strTemp); 
		strTemp.Format(_T("%0.1f mA"),(float)(short)product_register_value[MODBUS_HUM_CURRENT_OUTPUT]/10.0);
		m_output_list.SetItemText(1,2,strTemp); 

	}
	if (product_register_value[7]!=PM_HUM_R)
	{
		strTemp.Format(_T("%d"),product_register_value[MODBUS_AQ_RANGE_LOWER]);
		m_output_list.SetItemText(2,4,strTemp);
		strTemp.Format(_T("%d"),product_register_value[MODBUS_AQ_RANGE_UPPER]);
		m_output_list.SetItemText(2,5,strTemp);

		if (output_range==1||output_range==2)
		{
			strTemp.Format(_T("%0.2f V"),(float)(short)product_register_value[MODBUS_AQ_VOLTAGE_OUTPUT]/100.0);
			m_output_list.SetItemText(2,2,strTemp); 
		}
		else
		{
			strTemp.Format(_T("%0.1f mA"),(float)(short)product_register_value[MODBUS_AQ_CURRENT_OUTPUT]/10.0);
			m_output_list.SetItemText(2,2,strTemp); 
		}
	}


	m_output_list.ShowWindow(SW_SHOW);
}


LRESULT  CAirQuality::InputMessageCallBack_AQ(WPARAM wParam, LPARAM lParam){

	return TRUE;
}

LRESULT CAirQuality::Fresh_Input_List_AQ(WPARAM wParam,LPARAM lParam){
	CString strTemp;
	if (m_list_type == 0)
	{
		m_input_list.SetItemText(0,1,_T("Temperature"));
		m_input_list.SetItemText(1,1,_T("Humidity"));

		bitset<16> AM(product_register_value[349]);

		if (!AM[0])
		{
			m_input_list.SetItemText(0,2,Global_String_AUTO);

			if(product_register_value[MODBUS_DEGC_OR_F]==0)
			{
				strTemp.Format(_T("%0.1f"),((float)((short)product_register_value[MODBUS_TEMPRATURE_CHIP]))/10.0);
				m_input_list.SetItemText(0,3,strTemp);
				m_input_list.SetItemText(0,4,Global_String_C);
			} 
			else
			{
				strTemp.Format(_T("%0.1f"),((float)((short)product_register_value[MODBUS_TEMPRATURE_CHIP_F]))/10.0);
				m_input_list.SetItemText(0,3,strTemp);
				m_input_list.SetItemText(0,4,Glocal_String_F);
			}
		}
		else
		{
			m_input_list.SetItemText(0,2,Global_String_MANUAL);

			strTemp.Format(_T("%0.1f"),((float)((short)product_register_value[350]))/10.0);
			m_input_list.SetItemText(0,3,strTemp);
			if(product_register_value[MODBUS_DEGC_OR_F]==0)
			{

				m_input_list.SetItemText(0,4,Global_String_C);
			} 
			else
			{

				m_input_list.SetItemText(0,4,Glocal_String_F);
			}


		}

		if (!AM[1])
		{
			m_input_list.SetItemText(1,2,Global_String_AUTO);
			strTemp.Format(_T("%0.1f"),(float)(short)product_register_value[MODBUS_HUM]/10.0);
			m_input_list.SetItemText(1,3,strTemp);
		}
		else
		{
			m_input_list.SetItemText(1,2,Global_String_MANUAL);
			strTemp.Format(_T("%0.1f"),(float)(short)product_register_value[351]/10.0);
			m_input_list.SetItemText(1,3,strTemp);
		}

		strTemp.Format(_T("%d"),product_register_value[MODBUS_OVERRIDE_TIMER_LEFT]);
		m_input_list.SetItemText(0,5,strTemp);
		strTemp.Format(_T("%d"),product_register_value[MODBUS_HUM_FILTER]);
		m_input_list.SetItemText(1,5,strTemp);

		if (product_register_value[7]!=PM_HUM_R)
		{
			m_input_list.SetItemText(2,1,_T("Air Quanlity"));
			if (!AM[2])
			{
				m_input_list.SetItemText(2,2,Global_String_AUTO);
				strTemp.Format(_T("%d"),product_register_value[MODBUS_AQ]);
				m_input_list.SetItemText(2,3,strTemp);
			} 
			else
			{
				m_input_list.SetItemText(2,2,Global_String_MANUAL);
				strTemp.Format(_T("%d"),product_register_value[352]);
				m_input_list.SetItemText(2,3,strTemp);
			}
			strTemp.Format(_T("%d"),product_register_value[MODBUS_AQ_FILTER]);	 
			m_input_list.SetItemText(2,5,strTemp );

		}
	} 
	else if (m_list_type == 1)
	{
		m_output_list.SetItemText(0,1,_T("Temperature"));
		m_output_list.SetItemText(1,1,_T("Humidity"));

		if (product_register_value[7]!=PM_HUM_R)
		{
			m_output_list.SetItemText(2,1,_T("Air Quanlity"));
		}

		strTemp.Format(_T("%0.1f"),(float)(short)product_register_value[MODBUS_TEM_RANGE_LOWER]/10.0);
		m_output_list.SetItemText(0,4,strTemp);
		strTemp.Format(_T("%0.1f"),(float)(short)product_register_value[MODBUS_TEM_RANGE_UPPER]/10.0);
		m_output_list.SetItemText(0,5,strTemp);
		strTemp.Format(_T("%0.1f"),(float)(short)product_register_value[MODBUS_HUM_RANGE_LOWER]/10.0);
		m_output_list.SetItemText(1,4,strTemp);
		strTemp.Format(_T("%0.1f"),(float)(short)product_register_value[MODBUS_HUM_RANGE_UPPER]/10.0);
		m_output_list.SetItemText(1,5,strTemp);

		int output_range=product_register_value[MODBUS_OUTPUT_RANGE];
		if (output_range==1){
			strTemp=_T("0-10v");
		}
		else if (output_range==2)
		{
			strTemp=_T("0-5v");
		}
		else if (output_range==3)
		{
			strTemp=_T("4-20mA");
		}
		else
		{
			strTemp=_T("");
		}
		m_output_list.SetItemText(0,3,strTemp);
		m_output_list.SetItemText(1,3,strTemp);
		if (product_register_value[7]!=PM_HUM_R)
		{
			m_output_list.SetItemText(2,3,strTemp);
		}
		if (output_range==1||output_range==2)
		{
			strTemp.Format(_T("%0.2f V"),(float)(short)product_register_value[MODBUS_TEMP_VOLTAGE_OUTPUT]/100.0);
			m_output_list.SetItemText(0,2,strTemp); 
			strTemp.Format(_T("%0.2f V"),(float)(short)product_register_value[MODBUS_HUM_VOLTAGE_OUTPUT]/100.0);
			m_output_list.SetItemText(1,2,strTemp);  

		} 
		else
		{
			strTemp.Format(_T("%0.1f mA"),(float)(short)product_register_value[MODBUS_TEMP_CURRENT_OUTPUT]/10.0);
			m_output_list.SetItemText(0,2,strTemp); 
			strTemp.Format(_T("%0.1f mA"),(float)(short)product_register_value[MODBUS_HUM_CURRENT_OUTPUT]/10.0);
			m_output_list.SetItemText(1,2,strTemp); 

		}
		if (product_register_value[7]!=PM_HUM_R)
		{
			strTemp.Format(_T("%d"),product_register_value[MODBUS_AQ_RANGE_LOWER]);
			m_output_list.SetItemText(2,4,strTemp);
			strTemp.Format(_T("%d"),product_register_value[MODBUS_AQ_RANGE_UPPER]);
			m_output_list.SetItemText(2,5,strTemp);

			if (output_range==1||output_range==2)
			{
				strTemp.Format(_T("%0.2f V"),(float)(short)product_register_value[MODBUS_AQ_VOLTAGE_OUTPUT]/100.0);
				m_output_list.SetItemText(2,2,strTemp); 
			}
			else
			{
				strTemp.Format(_T("%0.1f mA"),(float)(short)product_register_value[MODBUS_AQ_CURRENT_OUTPUT]/10.0);
				m_output_list.SetItemText(2,2,strTemp); 
			}
		}
	}




	return 0;
}

void CAirQuality::OnNMClickList1_AQ(NMHDR *pNMHDR, LRESULT *pResult){
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);

	CString temp_cstring;
	long lRow,lCol;
	m_input_list.Set_Edit(true);
	DWORD dwPos=GetMessagePos();//Get which line is click by user.Set the check box, when user enter Insert it will jump to program dialog
	CPoint point( LOWORD(dwPos), HIWORD(dwPos));
	m_input_list.ScreenToClient(&point);
	LVHITTESTINFO lvinfo;
	lvinfo.pt=point;
	lvinfo.flags=LVHT_ABOVE;
	int nItem=m_input_list.SubItemHitTest(&lvinfo);

	lRow = lvinfo.iItem;
	lCol = lvinfo.iSubItem;


	if(lRow>m_input_list.GetItemCount()) //如果点击区超过最大行号，则点击是无效的
		return;
	if(lRow<0)
		return;
	CString New_CString;
	CString temp_task_info;

	CString temp1;
	CStringArray temparray;

	if (lCol == 2)	//	A/M
	{
		bitset<16> AM(product_register_value[349]);
		if(AM[lRow])
		{
			AM[lRow]=FALSE;
		}
		else
		{
			AM[lRow]=TRUE;
		}
		int AM_Int = AM.to_ulong();
		if (product_register_value[349]!=AM_Int)
		{
			int ret = write_one(g_tstat_id,349,AM_Int);
			if (ret> 0)
			{
				product_register_value[349]=AM_Int;
				PostMessage(WM_REFRESH_BAC_INPUT_LIST,0,0);
			}
		}

	}
	if (lCol == 4)
	{
		if (lRow == 0)
		{
			if(product_register_value[MODBUS_DEGC_OR_F]==0)
			{
				int ret = write_one(g_tstat_id,MODBUS_DEGC_OR_F,1);
				if (ret> 0)
				{
					product_register_value[MODBUS_DEGC_OR_F]=1;
				}
				PostMessage(WM_REFRESH_BAC_INPUT_LIST,0,0);
			}
			else
			{
				int ret = write_one(g_tstat_id,MODBUS_DEGC_OR_F,0);
				if (ret> 0)
				{
					product_register_value[MODBUS_DEGC_OR_F]=0;
				}
				PostMessage(WM_REFRESH_BAC_INPUT_LIST,0,0);
			}	
		}
	}

	m_list_type = 0;
}
void CAirQuality::OnNMClickList2_AQ(NMHDR *pNMHDR, LRESULT *pResult){
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);

	CString temp_cstring;
	 
	m_input_list.Set_Edit(true);
	DWORD dwPos=GetMessagePos();//Get which line is click by user.Set the check box, when user enter Insert it will jump to program dialog
	CPoint point( LOWORD(dwPos), HIWORD(dwPos));
	m_output_list.ScreenToClient(&point);
	LVHITTESTINFO lvinfo;
	lvinfo.pt=point;
	lvinfo.flags=LVHT_ABOVE;
	int nItem=m_output_list.SubItemHitTest(&lvinfo);

	m_list_type = 1;




}
void CAirQuality::OnNMClickList_User(NMHDR *pNMHDR, LRESULT *pResult){
   LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
  
   CString temp_cstring;
   long lRow,lCol;
   m_factory_list.Set_Edit(true);
   DWORD dwPos=GetMessagePos();//Get which line is click by user.Set the check box, when user enter Insert it will jump to program dialog
   CPoint point( LOWORD(dwPos), HIWORD(dwPos));
   m_factory_list.ScreenToClient(&point);
   LVHITTESTINFO lvinfo;
   lvinfo.pt=point;
   lvinfo.flags=LVHT_ABOVE;
   int nItem=m_factory_list.SubItemHitTest(&lvinfo);

   lRow = lvinfo.iItem;
   lCol = lvinfo.iSubItem;


   if(lRow>m_factory_list.GetItemCount()) //如果点击区超过最大行号，则点击是无效的
	   return;
   if(lRow<0)
	   return;
	if (lCol == 2)
	{
		if (product_register_value[MODBUS_HUM_CURRENT_DEFAULT] == 1)
		{
			m_list_type = 3; 
		}
		else
		{
			m_factory_list.Set_Edit(false);
			return;
		}
	}
	 
}
LRESULT CAirQuality::Fresh_Input_Item_AQ(WPARAM wParam,LPARAM lParam){
	int cmp_ret ;//compare if match it will 0;
	int Changed_Item = (int)wParam;
	int Changed_SubItem = (int)lParam;  
	CString temp_task_info;
	if (m_list_type == 0)
	{
		CString New_CString =  m_input_list.GetItemText(Changed_Item,Changed_SubItem);
		CString cstemp_value;

		//Value 
		if (Changed_SubItem == 3)//Value
		{
			int ItemValue = (int)(_wtof(New_CString)*10);
			int IntValue = _wtoi(New_CString);
			bitset<16> AM(product_register_value[349]);
			if (!AM[Changed_Item])//Auto
			{

				if (Changed_Item == 0)//
				{
					if (product_register_value[MODBUS_DEGC_OR_F]==0)
					{
						if (product_register_value[MODBUS_TEMPRATURE_CHIP]!=ItemValue)
						{
							int ret = write_one(g_tstat_id,MODBUS_TEMPRATURE_CHIP,ItemValue);
							if (ret> 0)
							{
								product_register_value[MODBUS_TEMPRATURE_CHIP]=ItemValue;
							}
						}
					}
					else
					{
						if (product_register_value[MODBUS_TEMPRATURE_CHIP_F]!=ItemValue)
						{
							int ret = write_one(g_tstat_id,MODBUS_TEMPRATURE_CHIP_F,ItemValue);
							if (ret> 0)
							{
								product_register_value[MODBUS_TEMPRATURE_CHIP_F]=ItemValue;
							}
						}
					}

				}

				if (Changed_Item == 1)
				{
					if (product_register_value[MODBUS_HUM]!=IntValue)
					{
						int ret = write_one(g_tstat_id,MODBUS_HUM,IntValue);
						if (ret> 0)
						{
							product_register_value[MODBUS_HUM]=IntValue;
						}
					}
				}

				if (Changed_Item == 2)
				{
					if (product_register_value[MODBUS_AQ]!=ItemValue)
					{
						int ret = write_one(g_tstat_id,MODBUS_AQ,ItemValue);
						if (ret> 0)
						{
							product_register_value[MODBUS_AQ]=ItemValue;
						}
					}
				}
			}
			else//Manual
			{
				if (Changed_Item == 0)
				{
					if (product_register_value[350]!=ItemValue)
					{
						int ret = write_one(g_tstat_id,350,ItemValue);
						if (ret> 0)
						{
							product_register_value[350]=ItemValue;
						}
					}
				}

				if (Changed_Item == 1)
				{
					if (product_register_value[351]!=ItemValue)
					{
						int ret = write_one(g_tstat_id,351,ItemValue);
						if (ret> 0)
						{
							product_register_value[351]=ItemValue;
						}
					}
				}

				if (Changed_Item == 2)
				{
					if (product_register_value[352]!=IntValue)
					{
						int ret = write_one(g_tstat_id,352,IntValue);
						if (ret> 0)
						{
							product_register_value[352]=IntValue;
						}
					}
				}
			}
		}
		//Filter
		if (Changed_SubItem == 5)
		{
			int ItemValue = _wtoi(New_CString);

			if (Changed_Item == 0)
			{
				if (product_register_value[MODBUS_OVERRIDE_TIMER_LEFT]!=ItemValue)
				{
					int ret = write_one(g_tstat_id,MODBUS_OVERRIDE_TIMER_LEFT,ItemValue);
					if (ret> 0)
					{
						product_register_value[MODBUS_OVERRIDE_TIMER_LEFT]=ItemValue;
					}
				}

			}
			if (Changed_Item == 1)
			{
				if (product_register_value[MODBUS_HUM_FILTER]!=ItemValue)
				{
					int ret = write_one(g_tstat_id,MODBUS_HUM_FILTER,ItemValue);
					if (ret> 0)
					{
						product_register_value[MODBUS_HUM_FILTER]=ItemValue;
					}
				}

			}
			if (Changed_Item == 2)
			{
				if (product_register_value[MODBUS_AQ_FILTER]!=ItemValue)
				{
					int ret = write_one(g_tstat_id,MODBUS_AQ_FILTER,ItemValue);
					if (ret> 0)
					{
						product_register_value[MODBUS_AQ_FILTER]=ItemValue;
					}
				}

			}
		}

		PostMessage(WM_REFRESH_BAC_INPUT_LIST,0,0);
	} 
	else if(m_list_type == 2)
	{	
		CString New_CString =  m_output_list.GetItemText(Changed_Item,Changed_SubItem);
		int ItemValue = (int)(_wtof(New_CString)*10);
		if (Changed_SubItem == 4)
		{
			if (Changed_Item == 0)	  //MODBUS_TEM_RANGE_LOWER
			{
				if (product_register_value[MODBUS_TEM_RANGE_LOWER]!=ItemValue)
				{
					int ret = write_one(g_tstat_id,MODBUS_TEM_RANGE_LOWER,ItemValue);
					if (ret> 0)
					{
						product_register_value[MODBUS_TEM_RANGE_LOWER]=ItemValue;
					}
				}
			}
			if (Changed_Item == 1)
			{
				if (product_register_value[MODBUS_HUM_RANGE_LOWER]!=ItemValue)
				{
					int ret = write_one(g_tstat_id,MODBUS_HUM_RANGE_LOWER,ItemValue);
					if (ret> 0)
					{
						product_register_value[MODBUS_HUM_RANGE_LOWER]=ItemValue;
					}
				}
			}
			if (Changed_Item == 2)
			{
				if (product_register_value[MODBUS_AQ_RANGE_LOWER]!=ItemValue)
				{
					int ret = write_one(g_tstat_id,MODBUS_AQ_RANGE_LOWER,ItemValue);
					if (ret> 0)
					{
						product_register_value[MODBUS_AQ_RANGE_LOWER]=ItemValue;
					}
				}
			}
		}

		if (Changed_SubItem == 5)
		{
			if (Changed_Item == 0)	  //MODBUS_TEM_RANGE_LOWER
			{
				if (product_register_value[MODBUS_TEM_RANGE_UPPER]!=ItemValue)
				{
					int ret = write_one(g_tstat_id,MODBUS_TEM_RANGE_UPPER,ItemValue);
					if (ret> 0)
					{
						product_register_value[MODBUS_TEM_RANGE_UPPER]=ItemValue;
					}
				}
			}
			if (Changed_Item == 1)
			{
				if (product_register_value[MODBUS_HUM_RANGE_UPPER]!=ItemValue)
				{
					int ret = write_one(g_tstat_id,MODBUS_HUM_RANGE_UPPER,ItemValue);
					if (ret> 0)
					{
						product_register_value[MODBUS_HUM_RANGE_UPPER]=ItemValue;
					}
				}
			}
			if (Changed_Item == 2)
			{
				if (product_register_value[MODBUS_AQ_RANGE_UPPER]!=ItemValue)
				{
					int ret = write_one(g_tstat_id,MODBUS_AQ_RANGE_UPPER,ItemValue);
					if (ret> 0)
					{
						product_register_value[MODBUS_AQ_RANGE_UPPER]=ItemValue;
					}
				}
			}
		}
	}
	else
	{
		if (Changed_SubItem == 2)
		{
			m_fresh_data=FALSE;
			CString New_CString =  m_user_list.GetItemText(Changed_Item,Changed_SubItem);
			int ItemValue = (int)(_wtof(New_CString)*10);
			int RegAddress;

		 
			 
			if(New_CString.IsEmpty())
			{
				m_fresh_data=TRUE;
				Initial_UserList();  
				return FALSE;
			}
			 
			 
			 
			RegAddress=MODBUS_HUMCOUNT1_H+2*Changed_Item+1;
			 
			if(product_register_value[RegAddress]==ItemValue)	//Add this to judge weather this value need to change.
			{
				m_fresh_data=TRUE;  
				Initial_UserList();  
				return FALSE;
			}

			int ret=write_one(g_tstat_id,RegAddress,ItemValue);
			if (ret>0)
			{
				product_register_value[RegAddress]=ItemValue;		 
			}
			else
			{
				AfxMessageBox(_T("Write Fail!"));
				m_user_list.Set_Edit(false);
				return false;
			}

			m_fresh_data=TRUE;
			unsigned short Data[20];
			ret = Read_Multi(g_tstat_id,Data,MODBUS_HUMCOUNT1_H,20);
			if (ret>0)
			{
				for (int i = 0 ; i < 20 ; i++)
				{
					product_register_value[MODBUS_HUMCOUNT1_H + i] = Data[ i ];
				}
			}
			Initial_UserList(); 

			InitChartData();
			UpdatePoint();
		}
	}


	return 0;
}

void CAirQuality::Fresh_Input_Output()
{		  
	CString strTemp;
	m_input_list.SetItemText(0,1,_T("Temperature"));
	m_input_list.SetItemText(1,1,_T("Humidity"));

	bitset<16> AM(product_register_value[349]);

	if (!AM[0])
	{
		m_input_list.SetItemText(0,2,Global_String_AUTO);

		if(product_register_value[MODBUS_DEGC_OR_F]==0)
		{
			strTemp.Format(_T("%0.1f"),((float)((short)product_register_value[MODBUS_TEMPRATURE_CHIP]))/10.0);
			m_input_list.SetItemText(0,3,strTemp);
			m_input_list.SetItemText(0,4,Global_String_C);
		} 
		else
		{
			strTemp.Format(_T("%0.1f"),((float)((short)product_register_value[MODBUS_TEMPRATURE_CHIP_F]))/10.0);
			m_input_list.SetItemText(0,3,strTemp);
			m_input_list.SetItemText(0,4,Glocal_String_F);
		}
	}
	else
	{
		m_input_list.SetItemText(0,2,Global_String_MANUAL);

		strTemp.Format(_T("%0.1f"),((float)((short)product_register_value[350]))/10.0);
		m_input_list.SetItemText(0,3,strTemp);
		if(product_register_value[MODBUS_DEGC_OR_F]==0)
		{

			m_input_list.SetItemText(0,4,Global_String_C);
		} 
		else
		{

			m_input_list.SetItemText(0,4,Glocal_String_F);
		}


	}

	if (!AM[1])
	{
		m_input_list.SetItemText(1,2,Global_String_AUTO);
		strTemp.Format(_T("%0.1f"),(float)(short)product_register_value[MODBUS_HUM]/10.0);
		m_input_list.SetItemText(1,3,strTemp);
	}
	else
	{
		m_input_list.SetItemText(1,2,Global_String_MANUAL);
		strTemp.Format(_T("%0.1f"),(float)(short)product_register_value[351]/10.0);
		m_input_list.SetItemText(1,3,strTemp);
	}

	strTemp.Format(_T("%d"),product_register_value[MODBUS_OVERRIDE_TIMER_LEFT]);
	m_input_list.SetItemText(0,5,strTemp);
	strTemp.Format(_T("%d"),product_register_value[MODBUS_HUM_FILTER]);
	m_input_list.SetItemText(1,5,strTemp);

	if (product_register_value[7]!=PM_HUM_R)
	{
		m_input_list.SetItemText(2,1,_T("Air Quanlity"));
		if (!AM[2])
		{
			m_input_list.SetItemText(2,2,Global_String_AUTO);
			strTemp.Format(_T("%d"),product_register_value[MODBUS_AQ]);
			m_input_list.SetItemText(2,3,strTemp);
		} 
		else
		{
			m_input_list.SetItemText(2,2,Global_String_MANUAL);
			strTemp.Format(_T("%d"),product_register_value[352]);
			m_input_list.SetItemText(2,3,strTemp);
		}
		strTemp.Format(_T("%d"),product_register_value[MODBUS_AQ_FILTER]);	 
		m_input_list.SetItemText(2,5,strTemp );

	}



	m_output_list.SetItemText(0,1,_T("Temperature"));
	m_output_list.SetItemText(1,1,_T("Humidity"));

	if (product_register_value[7]!=PM_HUM_R)
	{
		m_output_list.SetItemText(2,1,_T("Air Quanlity"));
	}

	strTemp.Format(_T("%0.1f"),(float)(short)product_register_value[MODBUS_TEM_RANGE_LOWER]/10.0);
	m_output_list.SetItemText(0,4,strTemp);
	strTemp.Format(_T("%0.1f"),(float)(short)product_register_value[MODBUS_TEM_RANGE_UPPER]/10.0);
	m_output_list.SetItemText(0,5,strTemp);
	strTemp.Format(_T("%0.1f"),(float)(short)product_register_value[MODBUS_HUM_RANGE_LOWER]/10.0);
	m_output_list.SetItemText(1,4,strTemp);
	strTemp.Format(_T("%0.1f"),(float)(short)product_register_value[MODBUS_HUM_RANGE_UPPER]/10.0);
	m_output_list.SetItemText(1,5,strTemp);

	int output_range=product_register_value[MODBUS_OUTPUT_RANGE];
	if (output_range==1){
		strTemp=_T("0-10v");
	}
	else if (output_range==2)
	{
		strTemp=_T("0-5v");
	}
	else if (output_range==3)
	{
		strTemp=_T("4-20mA");
	}
	else
	{
		strTemp=_T("");
	}
	m_output_list.SetItemText(0,3,strTemp);
	m_output_list.SetItemText(1,3,strTemp);
	if (product_register_value[7]!=PM_HUM_R)
	{
		m_output_list.SetItemText(2,3,strTemp);
	}
	if (output_range==1||output_range==2)
	{
		strTemp.Format(_T("%0.2f V"),(float)(short)product_register_value[MODBUS_TEMP_VOLTAGE_OUTPUT]/100.0);
		m_output_list.SetItemText(0,2,strTemp); 
		strTemp.Format(_T("%0.2f V"),(float)(short)product_register_value[MODBUS_HUM_VOLTAGE_OUTPUT]/100.0);
		m_output_list.SetItemText(1,2,strTemp);  

	} 
	else
	{
		strTemp.Format(_T("%0.1f mA"),(float)(short)product_register_value[MODBUS_TEMP_CURRENT_OUTPUT]/10.0);
		m_output_list.SetItemText(0,2,strTemp); 
		strTemp.Format(_T("%0.1f mA"),(float)(short)product_register_value[MODBUS_HUM_CURRENT_OUTPUT]/10.0);
		m_output_list.SetItemText(1,2,strTemp); 

	}
	if (product_register_value[7]!=PM_HUM_R)
	{
		strTemp.Format(_T("%d"),product_register_value[MODBUS_AQ_RANGE_LOWER]);
		m_output_list.SetItemText(2,4,strTemp);
		strTemp.Format(_T("%d"),product_register_value[MODBUS_AQ_RANGE_UPPER]);
		m_output_list.SetItemText(2,5,strTemp);

		if (output_range==1||output_range==2)
		{
			strTemp.Format(_T("%0.2f V"),(float)(short)product_register_value[MODBUS_AQ_VOLTAGE_OUTPUT]/100.0);
			m_output_list.SetItemText(2,2,strTemp); 
		}
		else
		{
			strTemp.Format(_T("%0.1f mA"),(float)(short)product_register_value[MODBUS_AQ_CURRENT_OUTPUT]/10.0);
			m_output_list.SetItemText(2,2,strTemp); 
		}
	}
}

