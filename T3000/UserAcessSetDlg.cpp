// UserAcessSetDlg.cpp : implementation file
//

#include "stdafx.h"
#include "T3000.h"
#include "MainFrm.h"
#include "UserAcessSetDlg.h"


// CUserAcessSetDlg dialog

IMPLEMENT_DYNAMIC(CUserAcessSetDlg, CDialog)

CUserAcessSetDlg::CUserAcessSetDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CUserAcessSetDlg::IDD, pParent)
{

}

CUserAcessSetDlg::~CUserAcessSetDlg()
{
}

void CUserAcessSetDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_ADDBUILDING_MSFLEXGRID, m_FlexGrid);
	DDX_Control(pDX, IDC_COMBO1, m_subNetList);
	DDX_Control(pDX, IDC_LEVELSETCOMBO, m_userLeveSetBox);
	//DDX_Control(pDX, IDC_NCPRIVILEGCOMBO, m_ncPrivilegeCombx);
	DDX_Control(pDX, IDC_ADDBUILDING_MSFLEXGRID2, m_FlexGrid2);
	DDX_Control(pDX, IDC_SINGLESETCOMBO, m_singleLevelSetCombox);
}


BEGIN_MESSAGE_MAP(CUserAcessSetDlg, CDialog)
	ON_BN_CLICKED(IDOK, &CUserAcessSetDlg::OnBnClickedOk)
	ON_CBN_SELCHANGE(IDC_COMBO1, &CUserAcessSetDlg::OnCbnSelchangeCombo1)
	ON_CBN_SELCHANGE(IDC_LEVELSETCOMBO, &CUserAcessSetDlg::OnCbnSelchangeLevelsetcombo)
	ON_CBN_KILLFOCUS(IDC_LEVELSETCOMBO, &CUserAcessSetDlg::OnCbnKillfocusLevelsetcombo)
//	ON_CBN_KILLFOCUS(IDC_NCPRIVILEGCOMBO, &CUserAcessSetDlg::OnCbnKillfocusNcprivilegcombo)
//	ON_CBN_SELCHANGE(IDC_NCPRIVILEGCOMBO, &CUserAcessSetDlg::OnCbnSelchangeNcprivilegcombo)
	ON_BN_CLICKED(IDCANCEL, &CUserAcessSetDlg::OnBnClickedCancel)
	ON_CBN_SELCHANGE(IDC_SINGLESETCOMBO, &CUserAcessSetDlg::OnCbnSelchangeSinglesetcombo)
	ON_CBN_KILLFOCUS(IDC_SINGLESETCOMBO, &CUserAcessSetDlg::OnCbnKillfocusSinglesetcombo)
END_MESSAGE_MAP()


// CUserAcessSetDlg message handlers

void CUserAcessSetDlg::OnBnClickedOk()
{
	// TODO: Add your control notification handler code here
	//OnOK();
}

BOOL CUserAcessSetDlg::OnInitDialog()
{
	CDialog::OnInitDialog();
	
	m_singleLevelSetCombox.ShowWindow(SW_HIDE);

	CMainFrame* pFrame=(CMainFrame*)(AfxGetApp()->m_pMainWnd);
	m_strMainBuilding= pFrame->m_strCurMainBuildingName;
	m_strSubNetName= pFrame->m_strCurSubBuldingName;
	int k=0;
	for(UINT i=0;i<pFrame->m_subNetLst.size();i++)
	{
		if(m_strSubNetName.CompareNoCase(pFrame->m_subNetLst.at(i).strBuildingName)==0)
			k=i;
		m_subNetList.AddString(pFrame->m_subNetLst.at(i).strBuildingName);
	}

	if(m_subNetList.GetCount()>0)
		m_subNetList.SetCurSel(k);

	CString strTitle;
	strTitle.Format(_T("%s: User access config dailog"),m_strUserName);
	SetWindowText(strTitle);
	m_userLeveSetBox.ShowWindow(SW_HIDE);

	/*
	m_ncPrivilegeCombx.ResetContent();
	m_ncPrivilegeCombx.AddString(_T("Read & write"));
	m_ncPrivilegeCombx.AddString(_T("Read only"));

*/

	
	InserSingleSetConfig();
	ReloadSingleLevelSetDB();


	InserProductToUserSetDB();
	ReloadUserLevelDB();
	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}

void CUserAcessSetDlg::SetConfigUserName(CString strUserName)
{
	m_strUserName=strUserName;
}
void CUserAcessSetDlg::InserProductToUserSetDB()
{
	_ConnectionPtr m_pConTmp;
 	_RecordsetPtr m_pRsTemp;
	m_pConTmp.CreateInstance("ADODB.Connection");
	m_pRsTemp.CreateInstance("ADODB.Recordset");
	
	m_pConTmp->Open(g_strDatabasefilepath.GetString(),"","",adModeUnknown);

//	m_pRsTemp->Open((_variant_t)(_T("select * from user_level where username = '"+m_strUserName+"'")),_variant_t((IDispatch *)m_pConTmp,true),adOpenStatic,adLockOptimistic,adCmdText);	

	CString strSql;
	strSql.Format(_T("select * from user_level where MainBuilding_Name='%s' and Building_Name='%s'and username='%s'"),m_strMainBuilding,m_strSubNetName,m_strUserName);	
	//m_pRsTemp->Open((_variant_t)(strSql),adOpenStatic,adLockOptimistic,adCmdText);	
	m_pRsTemp->Open((_variant_t)(strSql),_variant_t((IDispatch *)m_pConTmp,true),adOpenStatic,adLockOptimistic,adCmdText);	
	USERLEVEL UserLevelV;
	_variant_t temp_variant;
	CString strTemp;
	int nTemp;
	m_UserLevelLst.clear();
	while(VARIANT_FALSE==m_pRsTemp->EndOfFile)
	{
		
		temp_variant=m_pRsTemp->GetCollect("username");//
		if(temp_variant.vt!=VT_NULL)
			strTemp=temp_variant;
		else
			strTemp=_T("");
		UserLevelV.strUserName=strTemp;
		
		temp_variant=m_pRsTemp->GetCollect("serial_number");//
		if(temp_variant.vt!=VT_NULL)
			nTemp=temp_variant;
		else
			nTemp=0;
		UserLevelV.nSerialNumber=nTemp;

		m_UserLevelLst.push_back(UserLevelV);
		m_pRsTemp->MoveNext();
	}

	/*m_pCon.CreateInstance("ADODB.Connection");
	m_pRs.CreateInstance("ADODB.Recordset");
	m_pCon->Open(g_strDatabasefilepath.GetString(),"","",adModeUnknown);*/
	CBADO bado;
	bado.SetDBPath(g_strCurBuildingDatabasefilePath);
	bado.OnInitADOConn(); 

	VERYPRODCT veryProduct;
	//strSql=_T("select * from ALL_NODE where ");///MainBuilding_Name
	strSql.Format(_T("select * from ALL_NODE where MainBuilding_Name='%s' and Building_Name='%s'"),m_strMainBuilding,m_strSubNetName);

//	m_pRs->Open((_variant_t)strSql,_variant_t((IDispatch *)m_pCon,true),adOpenStatic,adLockOptimistic,adCmdText);	
	 bado.m_pRecordset=bado.OpenRecordset(strSql);
	m_VeryProdctLst.clear();
	while(VARIANT_FALSE==bado.m_pRecordset->EndOfFile)
	{
		//veryProduct.strMainBuildingName=m_pRs->GetCollect("MainBuilding_Name");//
		temp_variant=bado.m_pRecordset->GetCollect("MainBuilding_Name");//
		if(temp_variant.vt!=VT_NULL)
			strTemp=temp_variant;
		else
			strTemp=_T("");
		veryProduct.strMainBuildingName=strTemp;
		
		//veryProduct.strBuildingName=m_pRs->GetCollect("Building_Name");//
		temp_variant=bado.m_pRecordset->GetCollect("Building_Name");//
		if(temp_variant.vt!=VT_NULL)
			strTemp=temp_variant;
		else
			strTemp=_T("");
		veryProduct.strBuildingName=strTemp;

	//	veryProduct.nSerialNumber=m_pRs->GetCollect("Serial_ID");
		temp_variant=bado.m_pRecordset->GetCollect("Serial_ID");//
		if(temp_variant.vt!=VT_NULL)
			strTemp=temp_variant;
		else
			strTemp=_T("");
		veryProduct.nSerialNumber=_wtol(strTemp);

		//veryProduct.ProductID=m_pRs->GetCollect("Product_ID");
		temp_variant=bado.m_pRecordset->GetCollect("Product_ID");//
		if(temp_variant.vt!=VT_NULL)
			strTemp=temp_variant;
		else
			strTemp=_T("");
		veryProduct.ProductID=_wtoi(strTemp);

		//veryProduct.product_class_id=m_pRs->GetCollect("Product_class_ID");
		temp_variant=bado.m_pRecordset->GetCollect("Product_class_ID");//
		if(temp_variant.vt!=VT_NULL)
			strTemp=temp_variant;
		else
			strTemp=_T("");
		veryProduct.product_class_id=_wtoi(strTemp);

		//veryProduct.strFloorName=m_pRs->GetCollect("Floor_name");
		temp_variant=bado.m_pRecordset->GetCollect("Floor_name");//
		if(temp_variant.vt!=VT_NULL)
			strTemp=temp_variant;
		else
			strTemp=_T("");
		veryProduct.strFloorName=strTemp;

	//	veryProduct.strRoomName=m_pRs->GetCollect("Room_name");
		temp_variant=bado.m_pRecordset->GetCollect("Room_name");//
		if(temp_variant.vt!=VT_NULL)
			strTemp=temp_variant;
		else
			strTemp=_T("");
		veryProduct.strRoomName=strTemp;
		m_VeryProdctLst.push_back(veryProduct);
		bado.m_pRecordset->MoveNext();
	}
	int nVerySerialNum;
	int nUserlevelSerial;
	CString strUserName;
	for(UINT i=0;i<m_VeryProdctLst.size();i++)
	{
		nVerySerialNum=m_VeryProdctLst.at(i).nSerialNumber;
		BOOL bFind=FALSE;
		for(UINT j=0;j<m_UserLevelLst.size();j++)
		{
			nUserlevelSerial=m_UserLevelLst.at(j).nSerialNumber;
			if(nVerySerialNum==nUserlevelSerial)
			{
	
				bFind=TRUE;
				break;
			}
			//else
			//{
			//	bFind=FALSE;
		
			//}
		}
			
		if(!bFind)//insert to userlevel table.
		{

			try
			{


			strSql.Format(_T("insert into user_level values('%s','%s',%i,'%i','%s','%s','%s','%i','%i','%i','%i','%i','%i','%i','%i',%i)"),
			m_VeryProdctLst.at(i).strMainBuildingName,
			m_VeryProdctLst.at(i).strBuildingName,
			m_VeryProdctLst.at(i).nSerialNumber,
			m_VeryProdctLst.at(i).ProductID,
			m_strUserName,
			m_VeryProdctLst.at(i).strFloorName,
			m_VeryProdctLst.at(i).strRoomName,
			0,
			0,
			0,
			0,
			0,
			0,
			0,
			0,
			0
			);				
			m_pConTmp->Execute(strSql.GetString(),NULL,adCmdText);
			}
			catch(_com_error *e)
			{
				AfxMessageBox(e->ErrorMessage());
			}
		}
		
	
		
	}
	if(m_pRs->State) 
	m_pRs->Close(); 
	if(m_pCon->State)
	m_pCon->Close(); 

	if(m_pRsTemp->State) 
	m_pRsTemp->Close(); 
	if(m_pConTmp->State)
	m_pConTmp->Close(); 

}
void CUserAcessSetDlg::OnCbnSelchangeCombo1()
{
	CString strSelect;
	int nIdext=m_subNetList.GetCurSel();
	if(nIdext>=0)
	{
		m_subNetList.GetLBText(nIdext,strSelect);
		m_strSubNetName=strSelect;

		InserProductToUserSetDB();
		ReloadUserLevelDB();
	}
	if(nIdext>=0)
	{
		//m_subNetList.GetLBText(nIdext,strSelect);
	//	m_strSubNetName=strSelect;
		InserSingleSetConfig();
		ReloadSingleLevelSetDB();
	}

}
void CUserAcessSetDlg::ReloadUserLevelDB()
{
	m_FlexGrid.Clear();
	m_FlexGrid.put_Cols(13);	
	m_FlexGrid.put_TextMatrix(0,1,_T("Grid 2"));
	m_FlexGrid.put_TextMatrix(0,1,_T("User"));
	m_FlexGrid.put_TextMatrix(0,2,_T("Serial"));
	m_FlexGrid.put_TextMatrix(0,3,_T("ID"));
	m_FlexGrid.put_TextMatrix(0,4,_T("Floor"));
	m_FlexGrid.put_TextMatrix(0,5,_T("Room"));
	m_FlexGrid.put_TextMatrix(0,6,_T("Main Screen"));
	m_FlexGrid.put_TextMatrix(0,7,_T("Parameter"));
	m_FlexGrid.put_TextMatrix(0,8,_T("Output "));
//	m_FlexGrid.put_TextMatrix(0,9,_T("Network Ctr"));
	m_FlexGrid.put_TextMatrix(0,9,_T("Graphic"));	
//	m_FlexGrid.put_TextMatrix(0,10,_T("Building Set"));
	m_FlexGrid.put_TextMatrix(0,10,_T("Burn Hex"));
	m_FlexGrid.put_TextMatrix(0,11,_T("Load Config"));
	m_FlexGrid.put_TextMatrix(0,12,_T("All Screen"));
		
	m_FlexGrid.put_ColWidth(0,700);
	m_FlexGrid.put_ColWidth(1,700);
	m_FlexGrid.put_ColWidth(2,700);
	m_FlexGrid.put_ColWidth(3,1000);
	m_FlexGrid.put_ColWidth(4,1000);

	m_FlexGrid.put_ColWidth(5,1000);
	m_FlexGrid.put_ColWidth(6,1100);
	m_FlexGrid.put_ColWidth(7,1100);
	m_FlexGrid.put_ColWidth(8,1100);
	m_FlexGrid.put_ColWidth(9,1100);
	m_FlexGrid.put_ColWidth(10,1100);
	m_FlexGrid.put_ColWidth(11,1100);
	m_FlexGrid.put_ColWidth(12,1100);



	for(int i=0;i<13;i++)
	{
		m_FlexGrid.put_ColAlignment(i,4);
	}


	_ConnectionPtr m_pConTmp;
	_RecordsetPtr m_pRsTemp;
	m_pConTmp.CreateInstance("ADODB.Connection");
	m_pRsTemp.CreateInstance("ADODB.Recordset");

	m_pConTmp->Open(g_strDatabasefilepath.GetString(),"","",adModeUnknown);
	CString strSql;

	//m_pRsTemp->Open((_variant_t)("select * from user_level where username = '"+m_strUserName+"'"),_variant_t((IDispatch *)m_pConTmp,true),adOpenStatic,adLockOptimistic,adCmdText);	
	//	strSql.Format(_T("select * from user_level where MainBuilding_Name='%s' and Building_Name='%s'"),m_strMainBuilding,m_strSubNetName);
	strSql.Format(_T("select * from user_level where MainBuilding_Name='%s' and Building_Name='%s' and username='%s'"),m_strMainBuilding,m_strSubNetName,m_strUserName);

	m_pRsTemp->Open((_variant_t)(strSql),_variant_t((IDispatch *)m_pConTmp,true),adOpenStatic,adLockOptimistic,adCmdText);	
	USERLEVEL UserLevelV;
	int temp_row=0;
	int nTemp;
	CString strTemp;
	_variant_t temp_variant;

	m_FlexGrid.put_Rows(m_pRsTemp->RecordCount+1);

	for(int i=1;i<m_pRsTemp->RecordCount+1;i++)
	{	
		for(int k=0;k<=12;k++)
		{
			if (i%2==0)
			{
				m_FlexGrid.put_Row(i);m_FlexGrid.put_Col(k);m_FlexGrid.put_CellBackColor(COLOR_CELL);
			}
		}
	}

	while(VARIANT_FALSE==m_pRsTemp->EndOfFile)
	{
		++temp_row;
		temp_variant=m_pRsTemp->GetCollect("MainBuilding_Name");//
		if(temp_variant.vt!=VT_NULL)
			strTemp=temp_variant;
		else
			strTemp=_T("");
		UserLevelV.strMainBuildingName=strTemp;

		temp_variant=m_pRsTemp->GetCollect("Building_Name");//
		if(temp_variant.vt!=VT_NULL)
			strTemp=temp_variant;
		else
			strTemp=_T("");
		UserLevelV.strSubBuildingName=strTemp;

		temp_variant=m_pRsTemp->GetCollect("username");//
		if(temp_variant.vt!=VT_NULL)
			strTemp=temp_variant;
		else
			strTemp=_T("");
		UserLevelV.strUserName=strTemp;

		temp_variant=m_pRsTemp->GetCollect("serial_number");//
		if(temp_variant.vt!=VT_NULL)
			nTemp=temp_variant;
		else
			nTemp=0;
		UserLevelV.nSerialNumber=nTemp;

		temp_variant=m_pRsTemp->GetCollect("product_id");//
		if(temp_variant.vt!=VT_NULL)
			nTemp=temp_variant;
		else
			nTemp=0;
		UserLevelV.ProductID=nTemp;

		temp_variant=m_pRsTemp->GetCollect("floorname");//
		if(temp_variant.vt!=VT_NULL)
			strTemp=temp_variant;
		else
			strTemp=_T("");
		UserLevelV.strFloorName=strTemp;

		temp_variant=m_pRsTemp->GetCollect("roomname");//
		if(temp_variant.vt!=VT_NULL)
			strTemp=temp_variant;
		else
			strTemp=_T("");
		UserLevelV.strRoomName=strTemp;

		temp_variant=m_pRsTemp->GetCollect("mainscreen_level");//
		if(temp_variant.vt!=VT_NULL)
			nTemp=temp_variant;
		else
			nTemp=0;
		UserLevelV.MainLevel=nTemp;

		temp_variant=m_pRsTemp->GetCollect("parameter_level");//
		if(temp_variant.vt!=VT_NULL)
			strTemp=temp_variant;
		else
			nTemp=0;
		UserLevelV.ParamerLevel=nTemp;

		temp_variant=m_pRsTemp->GetCollect("outputtable_level");//
		if(temp_variant.vt!=VT_NULL)
			nTemp=temp_variant;
		else
			nTemp=0;
		UserLevelV.Outputtable_level=nTemp;

		temp_variant=m_pRsTemp->GetCollect("graphic_level");//
		if(temp_variant.vt!=VT_NULL)
			nTemp=temp_variant;
		else
			nTemp=0;
		UserLevelV.Graphic_level=nTemp;

		temp_variant=m_pRsTemp->GetCollect("burnhex_level");//
		if(temp_variant.vt!=VT_NULL)
			nTemp=temp_variant;
		else
			nTemp=0;
		UserLevelV.Burnhex_level=nTemp;

		temp_variant=m_pRsTemp->GetCollect("loadconfig_level");//
		if(temp_variant.vt!=VT_NULL)
			nTemp=temp_variant;
		else
			nTemp=0;
		UserLevelV.Loadconfig_level=nTemp;

		/*
		temp_variant=m_pRsTemp->GetCollect("building_level");//
		if(temp_variant.vt!=VT_NULL)
			nTemp=temp_variant;
		else
			nTemp=0;
		UserLevelV.Buildingset_level=nTemp;
		*/

		temp_variant=m_pRsTemp->GetCollect("allscreen_level");//
		if(temp_variant.vt!=VT_NULL)
			nTemp=temp_variant;
		else
			nTemp=0;
		UserLevelV.Allscreen_level=nTemp;

		
		if(UserLevelV.Allscreen_level==1)
		{
			

			for(int k=0;k<12;k++)
			{
			
				m_FlexGrid.put_Row(temp_row);m_FlexGrid.put_Col(k);m_FlexGrid.put_CellBackColor(RGB(215,215,215));
			}
		}
		else
		{
				for(int k=0;k<=12;k++)
				{
					if (temp_row%2==0)
					{
						m_FlexGrid.put_Row(temp_row);m_FlexGrid.put_Col(k);m_FlexGrid.put_CellBackColor(COLOR_CELL);
					}
					else
					{
						m_FlexGrid.put_Row(temp_row);m_FlexGrid.put_Col(k);m_FlexGrid.put_CellBackColor(RGB(255,255,240));

					}
				}
		}
		

		m_pRsTemp->MoveNext();


		m_FlexGrid.put_TextMatrix(temp_row,1,UserLevelV.strUserName);
		CString strTemp;;
		strTemp.Format(_T("%d"),UserLevelV.nSerialNumber);
	
		m_FlexGrid.put_TextMatrix(temp_row,2,strTemp);
		strTemp.Format(_T("%d"),UserLevelV.ProductID);
		m_FlexGrid.put_TextMatrix(temp_row,3,strTemp);
		m_FlexGrid.put_TextMatrix(temp_row,4,UserLevelV.strFloorName);
		m_FlexGrid.put_TextMatrix(temp_row,5,UserLevelV.strRoomName);
		switch (UserLevelV.MainLevel)
		{
			case 0:strTemp=_T("Read & write");break;
			case 1:strTemp=_T("Read only");break;
			default:strTemp=_T("Read & write");
		}
		m_FlexGrid.put_TextMatrix(temp_row,6,strTemp);
		switch (UserLevelV.ParamerLevel)
		{
			case 0:strTemp=_T("Read & write");break;
			case 1:strTemp=_T("Read only");break;
			default:strTemp=_T("Read & write");
		}
		m_FlexGrid.put_TextMatrix(temp_row,7,strTemp);
		
		switch (UserLevelV.Outputtable_level)
		{
			case 0:strTemp=_T("Read & write");break;
			case 1:strTemp=_T("Read only");break;
			default:strTemp=_T("Read & write");
		}
		m_FlexGrid.put_TextMatrix(temp_row,8,strTemp);
		switch (UserLevelV.Graphic_level)
		{
			case 0:strTemp=_T("Read & write");break;
			case 1:strTemp=_T("Read only");break;
			default:strTemp=_T("Read & write");
		}
		m_FlexGrid.put_TextMatrix(temp_row,9,strTemp);

		/*
		switch (UserLevelV.Buildingset_level)
		{
			case 0:strTemp=_T("Enable");break;
			case 1:strTemp=_T("Unenable");break;
			default:strTemp=_T("Enable");
		}
		m_FlexGrid.put_TextMatrix(temp_row,10,strTemp);
		*/



		switch (UserLevelV.Burnhex_level)
		{
			case 0:strTemp=_T("Enable");break;
			case 1:strTemp=_T("Unenable");break;
			default:strTemp=_T("Enable");
		}
		m_FlexGrid.put_TextMatrix(temp_row,10,strTemp);
		switch (UserLevelV.Loadconfig_level)
		{
			case 0:strTemp=_T("Enable");break;
			case 1:strTemp=_T("Unenable");break;
			default:strTemp=_T("Enable");
		}
		m_FlexGrid.put_TextMatrix(temp_row,11,strTemp);
		
		switch (UserLevelV.Allscreen_level)
		{
			case 0:strTemp=_T("Enable");break;
			case 1:strTemp=_T("Unenable");break;
			default:strTemp=_T("Enable");
		}
		m_FlexGrid.put_TextMatrix(temp_row,12,strTemp);
	}


	if(m_pRsTemp->State) 
		m_pRsTemp->Close(); 
	if(m_pConTmp->State)
		m_pConTmp->Close(); 
}BEGIN_EVENTSINK_MAP(CUserAcessSetDlg, CDialog)
ON_EVENT(CUserAcessSetDlg, IDC_ADDBUILDING_MSFLEXGRID, DISPID_CLICK, CUserAcessSetDlg::ClickAddbuildingMsflexgrid, VTS_NONE)
ON_EVENT(CUserAcessSetDlg, IDC_ADDBUILDING_MSFLEXGRID2, DISPID_CLICK, CUserAcessSetDlg::ClickAddbuildingMsflexgrid2, VTS_NONE)
END_EVENTSINK_MAP()

void CUserAcessSetDlg::ClickAddbuildingMsflexgrid()
{
	long lRow,lCol;
	lRow = m_FlexGrid.get_RowSel();//获取点击的行号	
	lCol = m_FlexGrid.get_ColSel(); //获取点击的列号

	m_nCurRow=lRow;
	m_nCurCol=lCol;

	m_FlexGrid.put_Row(m_nCurRow);m_FlexGrid.put_Col(m_nCurCol);
	if(RGB(215,215,215)==m_FlexGrid.get_CellBackColor())
		return;

	CRect rect;
	m_FlexGrid.GetWindowRect(rect); //获取表格控件的窗口矩形
	ScreenToClient(rect); //转换为客户区矩形	
	CDC* pDC =GetDC();

	int nTwipsPerDotX = 1440 / pDC->GetDeviceCaps(LOGPIXELSX) ;
	int nTwipsPerDotY = 1440 / pDC->GetDeviceCaps(LOGPIXELSY) ;
	//计算选中格的左上角的坐标(象素为单位)
	long y = m_FlexGrid.get_RowPos(lRow)/nTwipsPerDotY;
	long x = m_FlexGrid.get_ColPos(lCol)/nTwipsPerDotX;
	//计算选中格的尺寸(象素为单位)。加1是实际调试中，发现加1后效果更好
	long width = m_FlexGrid.get_ColWidth(lCol)/nTwipsPerDotX+1;
	long height = m_FlexGrid.get_RowHeight(lRow)/nTwipsPerDotY+1;
	//形成选中个所在的矩形区域
	CRect rcCell(x,y,x+width,y+height);
	//转换成相对对话框的坐标
	rcCell.OffsetRect(rect.left+1,rect.top+1);
	ReleaseDC(pDC);
	CString strValue = m_FlexGrid.get_TextMatrix(lRow,lCol);
	if(lCol==6||lCol==7||lCol==8)
	{
		
		m_userLeveSetBox.ResetContent();
		m_userLeveSetBox.AddString(_T("Read & write"));
		m_userLeveSetBox.AddString(_T("Read only"));
		m_userLeveSetBox.MoveWindow(&rcCell,1);
		m_userLeveSetBox.ShowWindow(SW_SHOW);
		m_userLeveSetBox.BringWindowToTop();
		m_userLeveSetBox.SetFocus();
	}
	if(lCol==9||lCol==11||lCol==12||lCol==10)
	{	
		m_userLeveSetBox.ResetContent();
		m_userLeveSetBox.AddString(_T("Enable"));
		m_userLeveSetBox.AddString(_T("Unenable"));
		m_userLeveSetBox.MoveWindow(&rcCell,1);
		m_userLeveSetBox.ShowWindow(SW_SHOW);
		m_userLeveSetBox.BringWindowToTop();
		m_userLeveSetBox.SetFocus();
	}
}

void CUserAcessSetDlg::OnCbnSelchangeLevelsetcombo()
{
	
	if(m_nCurRow==0)
		return;
	if(m_nCurCol==0)
		return;
	CString strValue;
	strValue=m_FlexGrid.get_TextMatrix(m_nCurRow,m_nCurCol);
	CString strSelect;
	int nIdext=m_userLeveSetBox.GetCurSel();
	if(nIdext>=0)
	{
		m_userLeveSetBox.GetLBText(nIdext,strSelect);
		if(strSelect.CompareNoCase(strValue)==0)
		{
			return;
		}
		else
		{
			int nNewValue=nIdext;
			m_FlexGrid.put_TextMatrix(m_nCurRow,m_nCurCol,strSelect);

			CString strField;
			if(m_nCurCol==6)
				strField="mainscreen_level";
			if(m_nCurCol==7)
				strField="parameter_level";
			if(m_nCurCol==8)
				strField="outputtable_level";
			if(m_nCurCol==9)
				strField="graphic_level";

			//if(m_nCurCol==10)
				//	strField="building_level";
			if(m_nCurCol==10)
				strField="burnhex_level";
			if(m_nCurCol==11)
				strField="loadconfig_level";
			if(m_nCurCol==12)
				strField="allscreen_level";

			CString strSerial=m_FlexGrid.get_TextMatrix(m_nCurRow,2);
			strSerial.TrimLeft();
			strSerial.TrimRight();
			int nSerial=_wtol(strSerial);
			try
			{

			_ConnectionPtr m_pConTmp;
			_RecordsetPtr m_pRsTemp;
			m_pConTmp.CreateInstance("ADODB.Connection");
			m_pRsTemp.CreateInstance("ADODB.Recordset");
			m_pConTmp->Open(g_strDatabasefilepath.GetString(),"","",adModeUnknown);
			CString strSql;

			strSql.Format(_T("update user_level set "+strField+" = %i where serial_number = %i and username ='%s' and MainBuilding_Name='%s' and Building_Name='%s'"),nNewValue,nSerial,m_strUserName,m_strMainBuilding,m_strSubNetName);
			m_pConTmp->Execute(strSql.GetString(),NULL,adCmdText);

			if(m_pConTmp->State)
				m_pConTmp->Close();
			}
			catch(_com_error *e)
			{
				AfxMessageBox(e->ErrorMessage());
			}	 

		}
	}

	if(m_nCurCol==12)
	{
		if(nIdext==0)//enable
		{
			for(int k=0;k<=12;k++)
				{
					if (m_nCurRow%2==0)
					{
						m_FlexGrid.put_Row(m_nCurRow);m_FlexGrid.put_Col(k);m_FlexGrid.put_CellBackColor(COLOR_CELL);
					}
					else
					{
						m_FlexGrid.put_Row(m_nCurRow);m_FlexGrid.put_Col(k);m_FlexGrid.put_CellBackColor(RGB(255,255,240));

					}
				}
		}
		else//unenable
		{
			for(int k=0;k<12;k++)
			{
			
				m_FlexGrid.put_Row(m_nCurRow);m_FlexGrid.put_Col(k);m_FlexGrid.put_CellBackColor(RGB(215,215,215));
			}
		}
	}
	//ReloadUserLevelDB();
}

void CUserAcessSetDlg::OnCbnKillfocusLevelsetcombo()
{
	m_userLeveSetBox.ShowWindow(SW_HIDE);
}


/*

void CUserAcessSetDlg::OnCbnSelchangeNcprivilegcombo()
{
	_ConnectionPtr m_pConTmp;
	m_pConTmp.CreateInstance("ADODB.Connection");
	m_pConTmp->Open(g_strDatabasefilepath.GetString(),"","",adModeUnknown);
	CString strSql;
	int nSel=0;
	nSel=m_ncPrivilegeCombx.GetCurSel();

	if(nSel==1)
	{
		 strSql.Format(_T("update NcEnable set NC_ENABLE = 0"));
	
	}
	else
	{
		 strSql.Format(_T("update NcEnable set NC_ENABLE = -1"));
	
	}
	m_pConTmp->Execute(strSql.GetString(),NULL,adCmdText);
	if(m_pConTmp->State) 
		m_pConTmp->Close(); 
}

*/
void CUserAcessSetDlg::OnBnClickedCancel()
{
	// TODO: Add your control notification handler code here
	OnCancel();
}

void CUserAcessSetDlg::ClickAddbuildingMsflexgrid2()
{
	long lRow,lCol;
	lRow = m_FlexGrid2.get_RowSel();//获取点击的行号	
	lCol = m_FlexGrid2.get_ColSel(); //获取点击的列号

	
	//if(RGB(215,215,215)==m_FlexGrid.get_CellBackColor())
	//	return;
	if(lRow==0)
		return;
	m_nCurRow2=lRow;
	m_nCurCol2=lCol;

	CRect rect;
	m_FlexGrid2.GetWindowRect(rect); //获取表格控件的窗口矩形
	ScreenToClient(rect); //转换为客户区矩形	
	CDC* pDC =GetDC();

	int nTwipsPerDotX = 1440 / pDC->GetDeviceCaps(LOGPIXELSX) ;
	int nTwipsPerDotY = 1440 / pDC->GetDeviceCaps(LOGPIXELSY) ;
	//计算选中格的左上角的坐标(象素为单位)
	long y = m_FlexGrid2.get_RowPos(lRow)/nTwipsPerDotY;
	long x = m_FlexGrid2.get_ColPos(lCol)/nTwipsPerDotX;
	//计算选中格的尺寸(象素为单位)。加1是实际调试中，发现加1后效果更好
	long width = m_FlexGrid2.get_ColWidth(lCol)/nTwipsPerDotX+1;
	long height = m_FlexGrid2.get_RowHeight(lRow)/nTwipsPerDotY+1;
	//形成选中个所在的矩形区域
	CRect rcCell(x,y,x+width,y+height);
	//转换成相对对话框的坐标
	rcCell.OffsetRect(rect.left+1,rect.top+1);
	ReleaseDC(pDC);
	CString strValue = m_FlexGrid2.get_TextMatrix(lRow,lCol);
	if(lCol==2||lCol==3)
	{
		
		m_singleLevelSetCombox.ResetContent();
		m_singleLevelSetCombox.AddString(_T("Read & write"));
		m_singleLevelSetCombox.AddString(_T("Read only"));
		m_singleLevelSetCombox.MoveWindow(&rcCell,1);
		m_singleLevelSetCombox.ShowWindow(SW_SHOW);
		m_singleLevelSetCombox.BringWindowToTop();
		m_singleLevelSetCombox.SetFocus();
	}
}
void CUserAcessSetDlg::InserSingleSetConfig()
{

	_ConnectionPtr m_pConTmp;
	_RecordsetPtr m_pRsTemp;
	m_pConTmp.CreateInstance("ADODB.Connection");
	m_pRsTemp.CreateInstance("ADODB.Recordset");

	m_pConTmp->Open(g_strDatabasefilepath.GetString(),"","",adModeUnknown);
	CString strSql;
	strSql.Format(_T("select * from UserLevelSingleSet where MainBuilding_Name='%s' and Building_Name='%s' and username='%s'"),m_strMainBuilding,m_strSubNetName,m_strUserName);
	m_pRsTemp->Open((_variant_t)(strSql),_variant_t((IDispatch *)m_pConTmp,true),adOpenStatic,adLockOptimistic,adCmdText);	
	_variant_t temp_variant;
	BOOL b_useLogin=false;

	try
	{

	if(m_pRsTemp->GetRecordCount()<=0)
	{
		strSql.Format(_T("insert into UserLevelSingleSet values('%s','%s','%s',%i,%i)"),m_strMainBuilding,m_strSubNetName,m_strUserName,0,0);		
		m_pConTmp->Execute(strSql.GetString(),NULL,adCmdText);
	}
	if(m_pConTmp->State) 
		m_pConTmp->Close(); 
	if(m_pRsTemp->State) 
		m_pRsTemp->Close(); 
	}
	catch(_com_error *e)
	{
		AfxMessageBox(e->ErrorMessage());
	}



}
void CUserAcessSetDlg::ReloadSingleLevelSetDB()
{
	m_FlexGrid2.Clear();
	m_FlexGrid2.put_Cols(4);	
	m_FlexGrid2.put_TextMatrix(0,1,_T("Grid 1"));
	m_FlexGrid2.put_TextMatrix(0,1,_T("User"));
	m_FlexGrid2.put_TextMatrix(0,2,_T("Network Controller"));
	m_FlexGrid2.put_TextMatrix(0,3,_T("Database"));

	m_FlexGrid2.put_ColWidth(0,1000);
	m_FlexGrid2.put_ColWidth(1,1000);
	m_FlexGrid2.put_ColWidth(2,1500);
	m_FlexGrid2.put_ColWidth(3,1000);
	for(int i=0;i<4;i++)
	{
		m_FlexGrid2.put_ColAlignment(i,4);
	}

	_ConnectionPtr m_pConTmp;
	_RecordsetPtr m_pRsTemp;
	m_pConTmp.CreateInstance("ADODB.Connection");
	m_pRsTemp.CreateInstance("ADODB.Recordset");

	m_pConTmp->Open(g_strDatabasefilepath.GetString(),"","",adModeUnknown);
	CString strSql;
	strSql.Format(_T("select * from UserLevelSingleSet where MainBuilding_Name='%s' and Building_Name='%s' and username='%s'"),m_strMainBuilding,m_strSubNetName,m_strUserName);
	m_pRsTemp->Open((_variant_t)(strSql),_variant_t((IDispatch *)m_pConTmp,true),adOpenStatic,adLockOptimistic,adCmdText);	
	_variant_t temp_variant;
	BOOL b_useLogin=false;
	int temp_row=0;
	CString strTemp;
	int nTemp;
	while(VARIANT_FALSE==m_pRsTemp->EndOfFile)
	{

		++temp_row;
		
		temp_variant=m_pRsTemp->GetCollect("username");//
		if(temp_variant.vt!=VT_NULL)
			strTemp=temp_variant;
		else
			strTemp=_T("");
		m_FlexGrid2.put_TextMatrix(temp_row,1,strTemp);

		temp_variant=m_pRsTemp->GetCollect("networkcontroller");//
		if(temp_variant.vt!=VT_NULL)
			nTemp=temp_variant;
		else
			nTemp=0;
		switch (nTemp)
		{
			case 0:strTemp=_T("Read & write");break;
			case 1:strTemp=_T("Read only");break;
			default:strTemp=_T("Read & write");
		}
		m_FlexGrid2.put_TextMatrix(temp_row,2,strTemp);

		temp_variant=m_pRsTemp->GetCollect("database_limition");//
		if(temp_variant.vt!=VT_NULL)
			nTemp=temp_variant;
		else
			nTemp=0;
		switch (nTemp)
		{
			case 0:strTemp=_T("Read & write");break;
			case 1:strTemp=_T("Read only");break;
			default:strTemp=_T("Read & write");
		}
		m_FlexGrid2.put_TextMatrix(temp_row,3,strTemp);

		m_pRsTemp->MoveNext();
	}
	if(m_pConTmp->State) 
		m_pConTmp->Close(); 
	if(m_pRsTemp->State) 
		m_pRsTemp->Close(); 
}
void CUserAcessSetDlg::OnCbnSelchangeSinglesetcombo()
{
	
	if(m_nCurRow2==0)
		return;
	if(m_nCurCol2==0)
		return;
	CString strValue;
	strValue=m_FlexGrid2.get_TextMatrix(m_nCurRow2,m_nCurCol2);
	CString strSelect;
	int nIdext=m_singleLevelSetCombox.GetCurSel();
	if(nIdext>=0)
	{
		m_singleLevelSetCombox.GetLBText(nIdext,strSelect);
		if(strSelect.CompareNoCase(strValue)==0)
		{
			return;
		}
		else
		{
			int nNewValue=nIdext;
			m_FlexGrid2.put_TextMatrix(m_nCurRow2,m_nCurCol2,strSelect);

			CString strField;
			if(m_nCurCol2==2)
				strField="networkcontroller";
			if(m_nCurCol2==3)
				strField="database_limition";
	
			try
			{

	
			_ConnectionPtr m_pConTmp;
			_RecordsetPtr m_pRsTemp;
			m_pConTmp.CreateInstance("ADODB.Connection");
			m_pRsTemp.CreateInstance("ADODB.Recordset");
			m_pConTmp->Open(g_strDatabasefilepath.GetString(),"","",adModeUnknown);
			CString strSql;

			strSql.Format(_T("update UserLevelSingleSet set "+strField+" = %i where username ='%s' and MainBuilding_Name='%s' and Building_Name='%s'"),nNewValue,m_strUserName,m_strMainBuilding,m_strSubNetName);
			m_pConTmp->Execute(strSql.GetString(),NULL,adCmdText);

			if(m_pConTmp->State)
				m_pConTmp->Close(); 
			}
			catch(_com_error *e)
			{
				AfxMessageBox(e->ErrorMessage());
			}

		}
	}
	
}

void CUserAcessSetDlg::OnCbnKillfocusSinglesetcombo()
{
	m_singleLevelSetCombox.ShowWindow(SW_HIDE);
}
