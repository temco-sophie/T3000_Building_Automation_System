// AllNodesDiaolg.cpp : implementation file
//

#include "stdafx.h"
#include "T3000.h"
#include "MainFrm.h"
#include "AllNodesDiaolg.h"
#include "ARDDlg.h"


// CAllNodesDiaolg dialog
#define AN_MAINNAME 1
#define AN_NAME		2
#define AN_SerialID	3
#define AN_FLOORNAME	4
#define	AN_ROOMNAME	5
#define	AN_PRUDUCTNAME	6
#define AN_PRODUCTTYPE	7
#define AN_PRODUCTID	8
#define AN_SCREENID	    9
#define AN_BAUDRATE		10
#define	AN_GRAPHICID	11
#define AN_HDVERSION	12
#define AN_SWVERSION	13
#define AN_EPSIZE		14

IMPLEMENT_DYNAMIC(CAllNodesDiaolg, CDialog)

CAllNodesDiaolg::CAllNodesDiaolg(CWnd* pParent /*=NULL*/)
	: CDialog(CAllNodesDiaolg::IDD, pParent)
{

	m_bChanged=FALSE;
    m_pDialogInfo = NULL;
}

CAllNodesDiaolg::~CAllNodesDiaolg()
{
}

void CAllNodesDiaolg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_MSFLEXGRID1, m_FlexGrid);
	DDX_Control(pDX, IDC_ADDBUTTON, m_AddBtn);
	DDX_Control(pDX, IDC_DELBUTTON, m_delBtn);
	DDX_Control(pDX, IDC_DELALLBUTTON, m_DelBtn);
	DDX_Control(pDX, IDC_EXITBUTTON, m_ExitBtn);
	DDX_Control(pDX, IDC_PRODUCTTYPECOMBO, m_productCombox);
	DDX_Control(pDX, IDC_TEXTEDIT, m_InputTextEdt);
	DDX_Control(pDX, IDC_COMBO1, m_SubLstCombox);
}


BEGIN_MESSAGE_MAP(CAllNodesDiaolg, CDialog)
	ON_BN_CLICKED(IDOK, &CAllNodesDiaolg::OnBnClickedOk)
	ON_BN_CLICKED(IDC_ADDBUTTON, &CAllNodesDiaolg::OnBnClickedAddbutton)
	ON_BN_CLICKED(IDC_DELBUTTON, &CAllNodesDiaolg::OnBnClickedDelbutton)
	ON_BN_CLICKED(IDC_DELALLBUTTON, &CAllNodesDiaolg::OnBnClickedDelallbutton)
	ON_BN_CLICKED(IDC_EXITBUTTON, &CAllNodesDiaolg::OnBnClickedExitbutton)
	ON_BN_CLICKED(IDCANCEL, &CAllNodesDiaolg::OnBnClickedCancel)
	ON_CBN_SELCHANGE(IDC_COMBO1, &CAllNodesDiaolg::OnCbnSelchangeCombo1)
	ON_EN_KILLFOCUS(IDC_TEXTEDIT, &CAllNodesDiaolg::OnEnKillfocusTextedit)
	ON_EN_SETFOCUS(IDC_TEXTEDIT, &CAllNodesDiaolg::OnEnSetfocusTextedit)
	ON_BN_CLICKED(IDC_ARD, &CAllNodesDiaolg::OnBnClickedArd)
    ON_BN_CLICKED(IDC_DELBUTTON_OFFLINE, &CAllNodesDiaolg::OnBnClickedDelbuttonOffline)
END_MESSAGE_MAP()


// CAllNodesDiaolg message handlers

BOOL CAllNodesDiaolg::OnInitDialog()
{
	CDialog::OnInitDialog();
	m_InputTextEdt.ShowWindow(SW_HIDE);
	m_productCombox.ShowWindow(SW_HIDE);
	//GetDlgItem(IDC_ADDBUTTON)->ShowWindow(SW_HIDE);

	CMainFrame* pFrame=(CMainFrame*)(AfxGetApp()->m_pMainWnd);
	m_strMainBuildingName= pFrame->m_strCurMainBuildingName;
	m_strSubNetName= pFrame->m_strCurSubBuldingName;
	int k=0;
	for(UINT i=0;i<pFrame->m_subNetLst.size();i++)
	{
		if(m_strSubNetName.CompareNoCase(pFrame->m_subNetLst.at(i).strBuildingName)==0)
			k=i;
		m_SubLstCombox.AddString(pFrame->m_subNetLst.at(i).strBuildingName);
	}
	m_SubLstCombox.SetCurSel(k);

	m_SubLstCombox.ShowWindow(SW_HIDE);
	GetDlgItem(IDC_STATIC1)->ShowWindow(SW_HIDE);
	
// 	m_pCon.CreateInstance(_T("ADODB.Connection"));
// 	m_pRs.CreateInstance(_T("ADODB.Recordset"));
// 	m_pCon->Open(g_strDatabasefilepath.GetString(),_T(""),_T(""),adModeUnknown);

	ReloadAddBuildingDB();
	return TRUE;  // return TRUE unless you set the focus to a control
}

void CAllNodesDiaolg::OnBnClickedOk()
{
	// TODO: Add your control notification handler code here
	//OnOK();
	/*
	*/

}


void CAllNodesDiaolg::SetBuildingMainName(CString strBuildName)
{
	m_strMainBuildingName=strBuildName;
}




void CAllNodesDiaolg::OnBnClickedDelallbutton()
{

	CBADO bado;
	bado.SetDBPath(g_strCurBuildingDatabasefilePath);
	bado.OnInitADOConn(); 

	CString strSql;
	strSql=_T("delete * from ALL_NODE where Custom is null or Custom = '0'");
	CString strTemp;
	strTemp.Format(_T("Are you sure to delete all the sub node(s)"));
	if(AfxMessageBox(strTemp,MB_OKCANCEL)==IDOK)
	{
		try
		{


			bado.m_pConnection->Execute(strSql.GetString(),NULL,adCmdText);	
		}
		catch(_com_error *e)
		{
			AfxMessageBox(e->ErrorMessage());
		}
	}
	bado.CloseConn();
	ReloadAddBuildingDB();
	m_bChanged=TRUE;
}

void CAllNodesDiaolg::OnBnClickedExitbutton()
{
	CAllNodesDiaolg::OnCancel();
	
	if (m_bChanged)
	{
		CMainFrame* pFrame=(CMainFrame*)(AfxGetApp()->m_pMainWnd);
		::PostMessage(pFrame->m_hWnd, WM_MYMSG_REFRESHBUILDING,0,0);
	}
}

void CAllNodesDiaolg::OnBnClickedCancel()
{
	// TODO: Add your control notification handler code here
	OnCancel();
}
BEGIN_EVENTSINK_MAP(CAllNodesDiaolg, CDialog)
	ON_EVENT(CAllNodesDiaolg, IDC_MSFLEXGRID1, DISPID_CLICK, CAllNodesDiaolg::ClickMsflexgrid1, VTS_NONE)
END_EVENTSINK_MAP()






void CAllNodesDiaolg::ClickMsflexgrid1()
{
	long lRow,lCol;
	lRow = m_FlexGrid.get_RowSel();//获取点击的行号	
	lCol = m_FlexGrid.get_ColSel(); //获取点击的列号
	if(lRow==0)
		return;
	CRect rect;
	m_FlexGrid.GetWindowRect(rect); //获取表格控件的窗口矩形
	ScreenToClient(rect); //转换为客户区矩形	
	// MSFlexGrid控件的函数的长度单位是"缇(twips)"，
	//需要将其转化为像素，1440缇= 1英寸
	CDC* pDC =GetDC();
	//计算象素点和缇的转换比例
	int nTwipsPerDotX = 1440 / pDC->GetDeviceCaps(LOGPIXELSX) ;
	int nTwipsPerDotY = 1440 / pDC->GetDeviceCaps(LOGPIXELSY) ;
	//计算选中格的左上角的坐标(象素为单位)
	long y = m_FlexGrid.get_RowPos(lRow)/nTwipsPerDotY;
	long x = m_FlexGrid.get_ColPos(lCol)/nTwipsPerDotX;
	//计算选中格的尺寸(象素为单位)。加1是实际调试中，发现加1后效果更好
	long width = m_FlexGrid.get_ColWidth(lCol)/nTwipsPerDotX+1;
	long height = m_FlexGrid.get_RowHeight(lRow)/nTwipsPerDotY+1;
	//形成选中个所在的矩形区域
	CRect rc(x,y,x+width,y+height);
	//转换成相对对话框的坐标
	rc.OffsetRect(rect.left+1,rect.top+1);
	//获取选中格的文本信息	
	CString strValue = m_FlexGrid.get_TextMatrix(lRow,lCol);
	m_nCurRow=lRow;
	m_nCurCol=lCol;

	if(m_nCurCol==1)
	{
		if(m_nCurRow<m_FlexGrid.get_Rows()-1)
			return;
	}
// 	if(m_nCurCol==AN_PRODUCTTYPE)
// 	{
// 		if(m_nCurRow<m_FlexGrid.get_Rows()-1)
// 			return;
// 		m_productCombox.ResetContent();
// 		m_productCombox.AddString(_T("Tstat"));
// 		m_productCombox.AddString(_T("NC"));
// 		m_productCombox.ShowWindow(SW_SHOW);//显示控件		
// 		m_productCombox.SetWindowText(strValue); //显示文本
// 		m_productCombox.SelectString(-1,strValue); //内容全选。方便直接修改		this line must before setfocus
// 		m_productCombox.SetFocus(); //获取焦点
// 		m_productCombox.MoveWindow(rc); //移动到选中格的位置，覆盖		
// 	}
//	if(m_nCurCol==AN_PRUDUCTNAME||m_nCurCol==AN_ROOMNAME||m_nCurCol==AN_FLOORNAME)
	if(m_nCurCol > 0 && m_nCurCol != AN_GRAPHICID  && m_nCurCol != AN_MAINNAME)
	{
		m_InputTextEdt.ShowWindow(SW_SHOW);

		m_InputTextEdt.SetWindowText(strValue); //显示文本
	//	m_InputTextEdt(-1,strValue); //内容全选。方便直接修改		this line must before setfocus
		
		m_InputTextEdt.SetFocus();
		int nLenth=strValue.GetLength();
		m_InputTextEdt.SetSel(0,nLenth); 
		m_InputTextEdt.SetFocus(); //获取焦点
		m_InputTextEdt.MoveWindow(rc); //移动到选中格的位置，覆盖	

	}
	if(m_nCurCol==AN_GRAPHICID)
	{
		CString strFilter = _T("bmp file|*.bmp|jpg file|*.jpg|png file|*.png|tif file|*.tif|all file|*.*||");
		CFileDialog dlg(true,_T("Open image file"),NULL,OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT | OFN_EXPLORER,strFilter);
		if(IDOK==dlg.DoModal())
		{
			//=program_path+"image\\"+short_name;

			CString strImgFilePathName=dlg.GetPathName();
			CString strImgFileName=dlg.GetFileName();
			CString strDestFileName=g_strImgeFolder+strImgFileName;

			CopyFile(strImgFilePathName,strDestFileName,FALSE);
			m_FlexGrid.put_TextMatrix(m_nCurRow,m_nCurCol,strImgFileName);
			//update recorder;
			CBADO bado;
			bado.SetDBPath(g_strCurBuildingDatabasefilePath);
			bado.OnInitADOConn(); 

			if(m_nCurRow<m_FlexGrid.get_Rows())
			{
				CString strSerial=m_FlexGrid.get_TextMatrix(m_nCurRow,AN_SerialID);
				if(strSerial.IsEmpty())
					return;
				try
				{


				CString strSql;
				strSql.Format(_T("update ALL_NODE set Background_imgID ='%s' where Serial_ID = '%s' "/*and Building_Name = '%s'*/),strImgFileName,strSerial/*,m_strSubNetName*/);
				bado.m_pConnection->Execute(strSql.GetString(),NULL,adCmdText);
				m_bChanged=TRUE;
				g_strImagePathName=strImgFileName;			
				}
				catch(_com_error *e)
				{
					AfxMessageBox(e->ErrorMessage());
				}
			}
			bado.CloseConn();
		}
	}
}

void CAllNodesDiaolg::OnCbnSelchangeCombo1()
{
	CString strSelect;
	int nIdext=m_SubLstCombox.GetCurSel();
	if(nIdext>=0)
	{
		m_SubLstCombox.GetLBText(nIdext,strSelect);
		m_strSubNetName=strSelect;
		ReloadAddBuildingDB();
        m_bChanged=TRUE;
	}
}

void CAllNodesDiaolg::OnEnKillfocusTextedit()
{
	// TODO: Add your control notification handler code here
	if(m_nCurRow==0)
		return;
	if(m_FlexGrid.get_RowSel()==0)
		return;
	m_InputTextEdt.ShowWindow(SW_HIDE);
	CString strName;
	m_InputTextEdt.GetWindowText(strName);

	CBADO bado;
	bado.SetDBPath(g_strCurBuildingDatabasefilePath);
	bado.OnInitADOConn(); 

	if (m_nCurRow < m_FlexGrid.get_Rows()-1)
	{
		if(m_nCurCol==AN_PRUDUCTNAME)
		{
			CString strSerial=m_FlexGrid.get_TextMatrix(m_nCurRow,AN_SerialID);
			if(strSerial.IsEmpty())
				return;
			CString strSql;
			try
			{


			strSql.Format(_T("update ALL_NODE set Product_name ='%s' where Serial_ID = '%s' and Building_Name = '%s'"),strName,strSerial,m_strSubNetName);
			bado.m_pConnection->Execute(strSql.GetString(),NULL,adCmdText);		
			}
			catch(_com_error *e)
			{
				AfxMessageBox(e->ErrorMessage());
			}
			ReloadAddBuildingDB();
		}
		if(m_nCurCol==AN_ROOMNAME)
		{
			CString strSerial=m_FlexGrid.get_TextMatrix(m_nCurRow,AN_SerialID);
			if(strSerial.IsEmpty())
				return;
			CString strSql;
			bado.OnInitADOConn(); 
			try
			{


			strSql.Format(_T("update ALL_NODE set Room_name ='%s' where Serial_ID = '%s' and Building_Name = '%s'"),strName,strSerial,m_strSubNetName);
			bado.m_pConnection->Execute(strSql.GetString(),NULL,adCmdText);	
			}
			catch(_com_error *e)
			{
				AfxMessageBox(e->ErrorMessage());
			}
			  bado.CloseConn();
			ReloadAddBuildingDB();
		}
		if(m_nCurCol==AN_FLOORNAME)
		{
			CString strSerial=m_FlexGrid.get_TextMatrix(m_nCurRow,AN_SerialID);
			if(strSerial.IsEmpty())
				return;
			try
			{


			CString strSql;

			strSql.Format(_T("update ALL_NODE set Floor_name ='%s' where Serial_ID = '%s' and Building_Name = '%s'"),strName,strSerial,m_strSubNetName);
			bado.m_pConnection->Execute(strSql.GetString(),NULL,adCmdText);		
			}
			catch(_com_error *e)
			{
				AfxMessageBox(e->ErrorMessage());
			}
			  bado.CloseConn();
			ReloadAddBuildingDB();
		}
	}
	else // 最后一行，用来增加
	{
		//if(m_nCurCol==AN_PRODUCTTYPE)
		{
			m_FlexGrid.put_TextMatrix(m_nCurRow, m_nCurCol, strName);
		}

	}
 
}

void CAllNodesDiaolg::OnEnSetfocusTextedit()
{
	// TODO: Add your control notification handler code here
}

BOOL CAllNodesDiaolg::PreTranslateMessage(MSG* pMsg)
{
	// TODO: Add your specialized code here and/or call the base class
	if(pMsg->wParam == VK_RETURN )
	{ 
		GetDlgItem(IDC_EXITBUTTON)->SetFocus();
		return true; 
	} 
	return CDialog::PreTranslateMessage(pMsg);
}


void CAllNodesDiaolg::OnBnClickedAddbutton()
{
	// TODO: Add your control notification handler code here
	//AfxMessageBox(_T("Not realized!"));

	CString strSql;
	int nMaxRowIndext=m_FlexGrid.get_Rows()-1;

	//CString strSBuildingNameTemp;
	//strSBuildingNameTemp.Format(_T("Subnet_%d"), nMaxRowIndext);	

	// main building name
	CString strMainBuildName=m_FlexGrid.get_TextMatrix(nMaxRowIndext, AN_MAINNAME);
	if(strMainBuildName.IsEmpty())
	{
		strMainBuildName = m_strMainBuildingName;
		//AfxMessageBox(_T("Input the building info in the last row of grid, the Main Building can not be empty."));
		//return ;
	}
	// sub net name
	CString strSubBuildingName=m_FlexGrid.get_TextMatrix(nMaxRowIndext,AN_NAME);
	if(strSubBuildingName.IsEmpty())
	{
		strSubBuildingName = m_strSubNetName;
	}
	// serial ID
	CString strSID;
	strSID=m_FlexGrid.get_TextMatrix(nMaxRowIndext,AN_SerialID);
	if(strSID.IsEmpty())
	{
		AfxMessageBox(_T("The Serial ID can not be empty, please input."));
		return ;
	}
	// Floor name
	CString strFloorName;
	strFloorName=m_FlexGrid.get_TextMatrix(nMaxRowIndext,AN_FLOORNAME);
	if(strFloorName.IsEmpty())
	{
		strFloorName.Format(_T("Floor_%d"), nMaxRowIndext);
	}
	// room name
	CString strRoomName;
	strRoomName=m_FlexGrid.get_TextMatrix(nMaxRowIndext,AN_ROOMNAME);
	if(strRoomName.IsEmpty())
	{
		strRoomName.Format(_T("Room_%d"), nMaxRowIndext);
	}

	// PRODUCT NAME
	CString strProName;
	strProName=m_FlexGrid.get_TextMatrix(nMaxRowIndext, AN_PRUDUCTNAME);
	if(strProName.IsEmpty())
	{
		strProName=_T("New device");
	}


	// product type 
	CString strProType;
	strProType=m_FlexGrid.get_TextMatrix(nMaxRowIndext, AN_PRODUCTTYPE);
	if(strProType.IsEmpty())
	{
		AfxMessageBox(_T("The Product type can not be empty, please input."));
		return ;
	}
	
	// product ID
	CString strProID;
	strProID=m_FlexGrid.get_TextMatrix(nMaxRowIndext, AN_PRODUCTID);
	if(strProID.IsEmpty())
	{
		AfxMessageBox(_T("The Product ID can not be empty, please input."));
		return ;
	}


	// SCREEN ID
	CString strScreenID;
	strScreenID=m_FlexGrid.get_TextMatrix(nMaxRowIndext, AN_SCREENID);
	if(strScreenID.IsEmpty())
	{		
		strScreenID = _T("Screen(S:") + strSID +_T("--") + strProID + _T(")");
	}

	// baud rate 
	CString strBaudrate;
	strBaudrate=m_FlexGrid.get_TextMatrix(nMaxRowIndext, AN_BAUDRATE);
	if(strBaudrate.IsEmpty())
	{		
		int nType = _wtoi(strProType);
		if ( nType == 100 )  // NC
		{
			strBaudrate = _T("192.168.0.3");
		}
		else
		{
			strBaudrate = _T("19200");
		}		
	}

	// GRAPHIC ID 
	CString strGraphicID;
	strGraphicID=m_FlexGrid.get_TextMatrix(nMaxRowIndext, AN_GRAPHICID);
	if(strGraphicID.IsEmpty())
	{		
		strGraphicID = _T("Clicking here to add a image");
	}
	
	// Hard VERSION
	CString strHdVersion;
	strHdVersion=m_FlexGrid.get_TextMatrix(nMaxRowIndext, AN_HDVERSION);
	if(strHdVersion.IsEmpty())
	{		
		strHdVersion = _T("0");
	}
	// soft VERSION
	CString strStVersion;
	strStVersion=m_FlexGrid.get_TextMatrix(nMaxRowIndext, AN_SWVERSION);
	if(strStVersion.IsEmpty())
	{		
		strStVersion = _T("0");
	}

	// EP Size
	CString strEPSize;
	strEPSize=m_FlexGrid.get_TextMatrix(nMaxRowIndext, AN_EPSIZE);
	if(strEPSize.IsEmpty())
	{		
		strEPSize = _T("0");
	}

	CString strCom = _T("0");
	CBADO bado;
	bado.SetDBPath(g_strCurBuildingDatabasefilePath);
	bado.OnInitADOConn(); 

	strSql.Format(_T("insert into ALL_NODE (MainBuilding_Name,Building_Name,Serial_ID,Floor_name,Room_name,Product_name,Product_class_ID,Product_ID,Screen_Name,Bautrate,Background_imgID,Hardware_Ver,Software_Ver,Com_Port,EPsize) values('"+strMainBuildName+"','"+strSubBuildingName +"','"+strSID+"','"+strFloorName+"','"+strRoomName+"','"+strProName+"','"+strProType+"','"+strProID+"','"+strScreenID+"','"+strBaudrate+"','"+strGraphicID+"','"+strHdVersion+"','"+strStVersion+"','"+strCom+"','"+strEPSize+"')"));
	//new nc//strSql.Format(_T("insert into ALL_NODE (MainBuilding_Name,Building_Name,Serial_ID,Floor_name,Room_name,Product_name,Product_class_ID,Product_ID,Screen_Name,Bautrate,Background_imgID,Hardware_Ver,Software_Ver,Com_Port,EPsize,Mainnet_info) values('"+strMainBuildName+"','"+strSubBuildingName +"','"+strSID+"','"+strFloorName+"','"+strRoomName+"','"+strProName+"','"+strProType+"','"+strProID+"','"+strScreenID+"','"+strBaudrate+"','"+strGraphicID+"','"+strHdVersion+"','"+strStVersion+"','"+strCom+"','"+strEPSize+"','"+strMainnetInfo+"')"));
	try
	{



	TRACE(strSql);
	
	bado.m_pConnection->Execute(strSql.GetString(),NULL,adCmdText);
	}
	catch(_com_error *e)
	{
		AfxMessageBox(e->ErrorMessage());
	}
	bado.CloseConn();
	ReloadAddBuildingDB();


}

void CAllNodesDiaolg::OnBnClickedArd()
{
	 CARDDlg dlg;
	if (dlg.DoModal()==IDOK)
	{
     OnInitDialog();
     m_bChanged=TRUE;
	} 
	 
	  
}

void CAllNodesDiaolg::ReloadAddBuildingDB()
{
    m_FlexGrid.Clear();

    m_FlexGrid.put_TextMatrix(0,0,_T(""));
    m_FlexGrid.put_ColWidth(0,400);

    m_FlexGrid.put_TextMatrix(0,AN_MAINNAME,_T("Main Building"));
    m_FlexGrid.put_ColWidth(AN_MAINNAME,800);

    m_FlexGrid.put_TextMatrix(0,AN_NAME,_T("Sub Net"));
    m_FlexGrid.put_ColWidth(AN_NAME,800);

    m_FlexGrid.put_TextMatrix(0,AN_SerialID,_T("Serial ID"));
    m_FlexGrid.put_ColWidth(AN_SerialID,800);

    m_FlexGrid.put_TextMatrix(0,AN_FLOORNAME,_T("Floor"));
    m_FlexGrid.put_ColWidth(AN_FLOORNAME,800);

    m_FlexGrid.put_TextMatrix(0,AN_ROOMNAME,_T("Room"));
    m_FlexGrid.put_ColWidth(AN_ROOMNAME,800);

    m_FlexGrid.put_TextMatrix(0,AN_PRUDUCTNAME,_T("Product Name"));
    m_FlexGrid.put_ColWidth(AN_PRUDUCTNAME,1300);

    m_FlexGrid.put_TextMatrix(0,AN_PRODUCTTYPE,_T("Product ID"));
    m_FlexGrid.put_ColWidth(AN_PRODUCTTYPE,1300);

    m_FlexGrid.put_TextMatrix(0,AN_PRODUCTID,_T("Address"));
    m_FlexGrid.put_ColWidth(AN_PRODUCTID,700);

    m_FlexGrid.put_TextMatrix(0,AN_SCREENID,_T("Screen ID"));
    m_FlexGrid.put_ColWidth(AN_SCREENID,2000);

    m_FlexGrid.put_TextMatrix(0,AN_BAUDRATE,_T("Baud Rate"));
    m_FlexGrid.put_ColWidth(AN_BAUDRATE,1300);

    m_FlexGrid.put_TextMatrix(0,AN_GRAPHICID,_T("Graphic Name"));
    m_FlexGrid.put_ColWidth(AN_GRAPHICID,1800);

    m_FlexGrid.put_TextMatrix(0,AN_HDVERSION,_T("HardW_Ver"));
    m_FlexGrid.put_ColWidth(AN_HDVERSION,1300);

    m_FlexGrid.put_TextMatrix(0,AN_SWVERSION,_T("SoftW_Ver"));
    m_FlexGrid.put_ColWidth(AN_SWVERSION,1300);
    m_FlexGrid.put_TextMatrix(0,AN_EPSIZE,_T("EPSize"));
    m_FlexGrid.put_ColWidth(AN_EPSIZE,800);

    CBADO bado;
    bado.SetDBPath(g_strCurBuildingDatabasefilePath);
    bado.OnInitADOConn(); 

    CString strSql;
    strSql.Format(_T("select * from ALL_NODE where MainBuilding_Name = '%s' ORDER BY Product_ID ASC"),m_strMainBuildingName);
    //m_pRs->Open((_variant_t)strSql,_variant_t((IDispatch *)m_pCon,true),adOpenStatic,adLockOptimistic,adCmdText);		
    bado.m_pRecordset=bado.OpenRecordset(strSql);	
    int recordcount= bado.GetRecordCount(bado.m_pRecordset);
    m_FlexGrid.put_Rows(recordcount+2);	
    int temp_row=0;
    CString str_temp;
    str_temp.Empty();
    _variant_t temp_variant;
    while(VARIANT_FALSE==bado.m_pRecordset->EndOfFile)
    {	

        //m_FlexGrid.put_TextMatrix(temp_row,AN_PRODUCTTYPE,m_strID);

        ++temp_row;
        CString strIndex; strIndex.Format(_T("%d"), temp_row);
        m_FlexGrid.put_TextMatrix(temp_row,0,strIndex);

        m_FlexGrid.put_TextMatrix(temp_row,AN_MAINNAME,m_strMainBuildingName);
        m_strSubNetName=bado.m_pRecordset->GetCollect("Building_Name");//
        m_FlexGrid.put_TextMatrix(temp_row,AN_NAME,m_strSubNetName);
        temp_variant=bado.m_pRecordset->GetCollect("Serial_ID");//
        if(temp_variant.vt!=VT_NULL)
            m_strID=temp_variant;
        else
            m_strID=_T("");
        m_FlexGrid.put_TextMatrix(temp_row,AN_SerialID,m_strID);

        temp_variant=bado.m_pRecordset->GetCollect("Floor_name");//
        if(temp_variant.vt!=VT_NULL)
            m_strID=temp_variant;
        else
            m_strID=_T("");
        m_FlexGrid.put_TextMatrix(temp_row,AN_FLOORNAME,m_strID);

        temp_variant=bado.m_pRecordset->GetCollect("Room_name");//
        if(temp_variant.vt!=VT_NULL)
            m_strID=temp_variant;
        else
            m_strID=_T("");
        m_FlexGrid.put_TextMatrix(temp_row,AN_ROOMNAME,m_strID);

        temp_variant=bado.m_pRecordset->GetCollect("Product_name");//
        if(temp_variant.vt!=VT_NULL)
            m_strID=temp_variant;
        else
            m_strID=_T("");
        m_FlexGrid.put_TextMatrix(temp_row,AN_PRUDUCTNAME,m_strID);

        temp_variant=bado.m_pRecordset->GetCollect("Product_class_ID");//
        if(temp_variant.vt!=VT_NULL)
            m_strID=temp_variant;
        else
            m_strID=_T("");
        m_FlexGrid.put_TextMatrix(temp_row,AN_PRODUCTTYPE,m_strID);

        temp_variant=bado.m_pRecordset->GetCollect("Product_ID");//
        if(temp_variant.vt!=VT_NULL)
            m_strID=temp_variant;
        else
            m_strID=_T("");
        m_FlexGrid.put_TextMatrix(temp_row,AN_PRODUCTID,m_strID);

        temp_variant=bado.m_pRecordset->GetCollect("Screen_Name");//
        if(temp_variant.vt!=VT_NULL)
            m_strID=temp_variant;
        else
            m_strID=_T("");
        m_FlexGrid.put_TextMatrix(temp_row,AN_SCREENID,m_strID);



        temp_variant=bado.m_pRecordset->GetCollect("Bautrate");//
        if(temp_variant.vt!=VT_NULL)
            m_strID=temp_variant;
        else
            m_strID=_T("");
        m_FlexGrid.put_TextMatrix(temp_row,AN_BAUDRATE,m_strID);

        temp_variant=bado.m_pRecordset->GetCollect("Background_imgID");//
        if(temp_variant.vt!=VT_NULL)
            m_strID=temp_variant;
        else
            m_strID=_T("");
        m_FlexGrid.put_TextMatrix(temp_row,AN_GRAPHICID,m_strID);


        temp_variant=bado.m_pRecordset->GetCollect("Hardware_Ver");//
        if(temp_variant.vt!=VT_NULL)
            m_strID=temp_variant;
        else
            m_strID=_T("");
        m_FlexGrid.put_TextMatrix(temp_row,AN_HDVERSION,m_strID);


        temp_variant=bado.m_pRecordset->GetCollect("Software_Ver");//
        if(temp_variant.vt!=VT_NULL)
            m_strID=temp_variant;
        else
            m_strID=_T("");
        m_FlexGrid.put_TextMatrix(temp_row,AN_SWVERSION,m_strID);

        temp_variant=bado.m_pRecordset->GetCollect("EPsize");//
        if(temp_variant.vt!=VT_NULL)
            m_strID=temp_variant;
        else
            m_strID=_T("");
        m_FlexGrid.put_TextMatrix(temp_row,AN_EPSIZE,m_strID);
        bado.m_pRecordset->MoveNext();
    }
    //m_pRs->Close();
    bado.CloseRecordset();
    bado.CloseConn();
}
void CAllNodesDiaolg::OnBnClickedDelbutton()
{
    if(m_nCurRow==0||m_nCurRow==m_FlexGrid.get_Rows()-1)
    {
        AfxMessageBox(_T("Please select a item first!"));
        return;
    }
    CString strText;
    strText=m_FlexGrid.get_TextMatrix(m_nCurRow,AN_SerialID);
    if(strText.IsEmpty())
    {
        AfxMessageBox(_T("No item Selected!"));
    }
    CBADO bado;
    bado.SetDBPath(g_strCurBuildingDatabasefilePath);
    bado.OnInitADOConn(); 

    CString strhw_version;
    strhw_version = m_FlexGrid.get_TextMatrix(m_nCurRow,AN_HDVERSION);
    CString strSql;
    strSql.Format(_T("delete * from ALL_NODE where Serial_ID ='%s' and Hardware_Ver = '%s'"),strText,strhw_version);
    CString strTemp;
    strTemp.Format(_T("Are you sure to delete thise item"));
    if(AfxMessageBox(strTemp,MB_OKCANCEL)==IDOK)
    {
        try
        {


            bado.m_pConnection->Execute(strSql.GetString(),NULL,adCmdText);	
        }
        catch(_com_error *e)
        {
            AfxMessageBox(e->ErrorMessage());
        }
    }
    bado.CloseConn();
    ReloadAddBuildingDB();
    m_bChanged=TRUE;

}
void CAllNodesDiaolg::OnBnClickedDelbuttonOffline()
{


    if(m_pDialogInfo==NULL)
    {
        m_pDialogInfo = new CDialogInfo;
        m_pDialogInfo->Create(IDD_DIALOG_INFO,this);
        m_pDialogInfo->ShowWindow(SW_SHOW); 
        m_pDialogInfo->CenterWindow();
        m_pDialogInfo->GetDlgItem(IDC_STATIC_INFO)->SetWindowText(_T(""));
    }

    CBADO bado;
    bado.SetDBPath(g_strCurBuildingDatabasefilePath);
    bado.OnInitADOConn(); 
    CString strSql;
    strSql.Format(_T("select * from ALL_NODE where MainBuilding_Name = '%s' ORDER BY Product_ID ASC"),m_strMainBuildingName);
    //m_pRs->Open((_variant_t)strSql,_variant_t((IDispatch *)m_pCon,true),adOpenStatic,adLockOptimistic,adCmdText);		
    bado.m_pRecordset=bado.OpenRecordset(strSql);	
    int recordcount= bado.GetRecordCount(bado.m_pRecordset);
    //m_FlexGrid.put_Rows(recordcount+2);	
    int temp_row=0;
    CString str_temp;
    str_temp.Empty();
    _variant_t temp_variant;
    CString Serial_Number;
    CString Protocol;
    CString Bautrate;
    CString Com_Port;
    CString Product_ID;
     BOOL Is_Online = FALSE;

    while(VARIANT_FALSE==bado.m_pRecordset->EndOfFile)
    {
        temp_variant=bado.m_pRecordset->GetCollect("Protocol");//
        if(temp_variant.vt!=VT_NULL)
            Protocol=temp_variant;
        else
            Protocol=_T("");

        temp_variant=bado.m_pRecordset->GetCollect("Serial_ID");//
        if(temp_variant.vt!=VT_NULL)
            Serial_Number=temp_variant;
        else
            Serial_Number=_T("");
            str_temp.Format(_T("Checking Serial Number:%s"),Serial_Number);
        if (m_pDialogInfo->IsWindowVisible())
        {
            m_pDialogInfo->GetDlgItem(IDC_STATIC_INFO)->SetWindowText(str_temp);
           /* m_pDialogInfo->ShowWindow(SW_SHOW);*/
        }

        temp_variant=bado.m_pRecordset->GetCollect("Com_Port");//
        if(temp_variant.vt!=VT_NULL)
            Com_Port=temp_variant;
        else
            Com_Port=_T(""); 

        temp_variant=bado.m_pRecordset->GetCollect("Bautrate");//
        if(temp_variant.vt!=VT_NULL)
            Bautrate=temp_variant;
        else
            Bautrate=_T("");

        temp_variant=bado.m_pRecordset->GetCollect("Product_ID");//
        if(temp_variant.vt!=VT_NULL)
            Product_ID=temp_variant;
        else
            Product_ID=_T("");

             close_com();

           int modbusid = _wtoi(Product_ID);

            if (Protocol.CompareNoCase(L"0")==0)
            {
                 int comport = _wtoi(Com_Port);
                 int brandrate = _wtoi(Bautrate);
                 
                if(open_com(comport))
                {
                    Change_BaudRate(brandrate);
                      
                      SetCommunicationType(0);
                    
                    int ret = read_one(modbusid,7);
                    if (ret>0)
                    {
                      Is_Online = TRUE;  
                    }
                    else
                    {
                       Is_Online = FALSE;
                    }
                }     
                       
            }
            else
            {
                int port = _wtoi(Com_Port);
                if (Open_Socket2(Bautrate,port))
                {
                    SetCommunicationType(1);
                    int ret = read_one(modbusid,7);
                    if (ret>0)
                    {
                        Is_Online = TRUE;  
                    }
                    else
                    {
                        Is_Online = FALSE;
                    } 
                }
                  
            }

            if (!Is_Online)
            {
                   strSql.Format(_T("delete * from ALL_NODE where Serial_ID ='%s'"),Serial_Number);
                   try
                   {


                       bado.m_pConnection->Execute(strSql.GetString(),NULL,adCmdText);	
                   }
                   catch(_com_error *e)
                   {
                       AfxMessageBox(e->ErrorMessage());
                   }
                    m_bChanged=TRUE;
            }

            bado.m_pRecordset->MoveNext();	
    }

    bado.CloseRecordset();
    bado.CloseConn();


    ReloadAddBuildingDB();


    if (m_pDialogInfo!=NULL)
    {
        delete m_pDialogInfo;
        m_pDialogInfo = NULL;
    }
}