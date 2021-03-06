// ImageTreeCtrl.cpp : 实现文件
//

#include "stdafx.h"
#include "T3000.h"
#include "ImageTreeCtrl.h"

#include "MainFrm.h"
#include "globle_function.h"
// CImageTreeCtrl
enum ECmdHandler {
	ID_RENAME = 1,
	ID_DELETE,
	ID_ADD_SIBLING,
	ID_ADD_CHILD,
	ID_ADD_ROOT,
	ID_SORT_LEVEL,
	ID_SORT_LEVELANDBELOW,
	ID_SORT_BY_CONNECTION,
	ID_SORT_BY_FLOOR,
	ID_MAX_CMD
};

enum ERightDragHandler {
	ID_DRAG_COPY = 15,
	ID_DRAG_MOVE,
	ID_DRAG_CANCEL,

	ID_MAX_DRH
};
 DWORD WINAPI _Background_Write_Name(LPVOID pParam){
  CImageTreeCtrl* dlg=(CImageTreeCtrl*)(pParam);
     CMainFrame* pFrame=(CMainFrame*)(AfxGetApp()->m_pMainWnd);
     for (int i=0;i< pFrame->m_product.size();i++){
         if (dlg->m_hSelItem == pFrame->m_product.at(i).product_item)
         {  
              CString strIPAddress;
              int IPPort=0;
              int ComPort=0;
              int brandrate = 0;
              int ModbusID=0;
             CString strSql,temp_serial;
            int sn=pFrame->m_product.at(i).serial_number;
            temp_serial.Format(_T("%d"),sn);
            int  int_product_type = pFrame->m_product.at(i).product_class_id;
            
            if (int_product_type == PM_TSTAT6||int_product_type == PM_TSTAT7||int_product_type == PM_HUMTEMPSENSOR||int_product_type ==PM_AirQuality||int_product_type ==PM_HUM_R)
            {
                int communicationType = pFrame->m_product.at(i).protocol;  
                ModbusID = pFrame->m_product.at(i).product_id;
                SetCommunicationType(communicationType);
                if (communicationType==0)
                {
                    
                    ComPort =  pFrame->m_product.at(i).ncomport;
                    brandrate = pFrame->m_product.at(i).baudrate;
                   
                    if (open_com(ComPort))
                    {
                        Change_BaudRate(brandrate);
                        
                        char cTemp1[16];
                        memset(cTemp1,0,16);
                        WideCharToMultiByte( CP_ACP, 0, dlg->m_name_new.GetBuffer(), -1, cTemp1, 16, NULL, NULL ); 
                        unsigned char Databuffer[16];
                        memcpy_s(Databuffer,16,cTemp1,16);
                        if (Write_Multi(ModbusID,Databuffer,715,16,10)>0)
                        {
                             WritePrivateProfileStringW(temp_serial,_T("WriteFlag"),_T("0"),g_achive_device_name_path);
                        }
                        
                        
                        close_com();

                    }
                } 
                else
                {
                    strIPAddress = pFrame->m_product.at(i).BuildingInfo.strIp;
                    IPPort = _wtoi(pFrame->m_product.at(i).BuildingInfo.strIpPort);
                    if (Open_Socket2(strIPAddress,IPPort))
                    {
                        if(dlg->m_name_new.GetLength()> 17)	//长度不能大于结构体定义的长度;
                        {
                            dlg->m_name_new.Delete(16,dlg->m_name_new.GetLength()-16);
                        }

                        char cTemp1[16];
                        memset(cTemp1,0,16);
                        WideCharToMultiByte( CP_ACP, 0, dlg->m_name_new.GetBuffer(), -1, cTemp1, 16, NULL, NULL ); 
                        unsigned char Databuffer[16];
                        memcpy_s(Databuffer,16,cTemp1,16);
                        if (Write_Multi(ModbusID,Databuffer,715,16,10)>0)
                        {
                             
                                 
                                WritePrivateProfileStringW(temp_serial,_T("WriteFlag"),_T("0"),g_achive_device_name_path);
                           
                        }
                    }
                      
                }

                
            } 

            break;
         }
         }

         return 1;
 }
IMPLEMENT_DYNAMIC(CImageTreeCtrl, CTreeCtrl)
CImageTreeCtrl::CImageTreeCtrl()
{
	m_nSubnetItemData = 9000;
	m_nFloorItemData = 1000;
	m_nRoomItemData = 2000;
	m_nDeviceItemData = 3000;
	m_Keymap[VK_F2][false][false] = &CImageTreeCtrl::DoEditLabel;
    m_Keymap[VK_DELETE][false][false] = &CImageTreeCtrl::DoDeleteItem;
	//m_Keymap[VK_INSERT][true ][false] = &CImageTreeCtrl::DoInsertChild;
	//m_Keymap[VK_INSERT][false][true ] = &CImageTreeCtrl::DoInsertRoot;
	//m_Keymap[VK_INSERT][false][false] = &CImageTreeCtrl::DoInsertSibling;
	//m_Keymap[VK_DELETE][false][false] = &CImageTreeCtrl::DoDeleteItem;
	//m_Keymap['S'      ][true ][false] = &CImageTreeCtrl::DoSortCurrentLevel;
	//m_Keymap['S'      ][true ][true ] = &CImageTreeCtrl::DoSortCurrentLevelAndBelow;

	m_Commandmap[ID_RENAME]		        = &CImageTreeCtrl::DoEditLabel;
	//m_Commandmap[ID_ADD_CHILD]          = &CImageTreeCtrl::DoInsertChild;
	//m_Commandmap[ID_ADD_ROOT]           = &CImageTreeCtrl::DoInsertRoot;
	//m_Commandmap[ID_ADD_SIBLING]        = &CImageTreeCtrl::DoInsertSibling;
	m_Commandmap[ID_DELETE]             = &CImageTreeCtrl::DoDeleteItem;
	//m_Commandmap[ID_SORT_LEVEL]         = &CImageTreeCtrl::DoSortCurrentLevel;
	//m_Commandmap[ID_SORT_LEVELANDBELOW] = &CImageTreeCtrl::DoSortCurrentLevelAndBelow;

	m_Commandmap[ID_SORT_BY_CONNECTION]		     = &CImageTreeCtrl::SortByConnection;
	m_Commandmap[ID_SORT_BY_FLOOR]		        = &CImageTreeCtrl::SortByFloor;


	old_hItem = NULL;
	m_serial_number = 0;
}

CImageTreeCtrl::~CImageTreeCtrl()
{
}


BEGIN_MESSAGE_MAP(CImageTreeCtrl, CTreeCtrl)
	ON_WM_PAINT()
	ON_WM_CREATE()
	ON_WM_CONTEXTMENU()
	ON_NOTIFY_REFLECT(TVN_BEGINLABELEDIT, OnBeginlabeledit)
	ON_NOTIFY_REFLECT(TVN_ENDLABELEDIT, OnEndlabeledit)
	ON_NOTIFY_REFLECT(NM_RCLICK, OnRclick)
	ON_COMMAND_RANGE(ID_RENAME, ID_MAX_CMD-1, OnContextCmd)
	ON_WM_KILLFOCUS()
END_MESSAGE_MAP()
void CImageTreeCtrl::OnContextCmd(UINT id) {
	HTREEITEM hCur = GetSelectedItem();
	method fnc = m_Commandmap[id];
	if(fnc) {
		(this->*fnc)(hCur);
		return;
	}
	ASSERT(false);
}
bool CImageTreeCtrl::DoEditLabel(HTREEITEM hItem) 
{
	m_hSelItem=hItem;
	return hItem ? (EditLabel(hItem) != 0) : false;
}

bool CImageTreeCtrl::SortByConnection(HTREEITEM hItem) 
{
	if(product_sort_way != SORT_BY_CONNECTION)
	{
		WritePrivateProfileStringW(_T("Setting"),_T("ProductSort"),_T("1"),g_cstring_ini_path);
		product_sort_way = SORT_BY_CONNECTION;
		CMainFrame* pFrame=(CMainFrame*)(AfxGetApp()->m_pMainWnd);
		::PostMessage(pFrame->m_hWnd,WM_MYMSG_REFRESHBUILDING,0,0);
	}

	return true;
}

bool CImageTreeCtrl::SortByFloor(HTREEITEM hItem) 
{
	if(product_sort_way != SORT_BY_BUILDING_FLOOR)
	{
		WritePrivateProfileStringW(_T("Setting"),_T("ProductSort"),_T("2"),g_cstring_ini_path);
		product_sort_way = SORT_BY_BUILDING_FLOOR;
		CMainFrame* pFrame=(CMainFrame*)(AfxGetApp()->m_pMainWnd);
		::PostMessage(pFrame->m_hWnd,WM_MYMSG_REFRESHBUILDING,0,0);
	}
	return true;
}

BOOL CImageTreeCtrl::UpdateDataToDB_Floor(){
	
    
    CADO ado;
	ado.OnInitADOConn();

	CBADO bado;
	bado.SetDBPath(g_strCurBuildingDatabasefilePath);
	bado.OnInitADOConn(); 


	::CoInitialize(NULL);
	try 
	{
		////////////////////////////////////////////////////////////////////////////////////////////
		//获取数据库名称及路径
		/////////////////////////////////////////////////////////////////////////////////////////////////
		//连接数据库
	    
		CString strSql;   BOOL is_exist=FALSE;	  CString str_temp;
		switch (m_level)
		{
		case 0:  //Subnet
			{


				//strSql.Format(_T("select * from Building where Main_BuildingName = '%s'"),m_strMainBuildingName);
				strSql.Format(_T("select * from Building order by Main_BuildingName"));
				//m_pRs->Open((_variant_t)strSql,_variant_t((IDispatch *)m_pCon,true),adOpenStatic,adLockOptimistic,adCmdText);
				ado.m_pRecordset=ado.OpenRecordset(strSql);
				while(VARIANT_FALSE==ado.m_pRecordset->EndOfFile)
				{	   	
				str_temp.Empty();
				str_temp=ado.m_pRecordset->GetCollect("Building_Name");
				if (str_temp.Compare(m_name_new)==0)
				{	   
					is_exist=TRUE;
					break;
				}			
				ado.m_pRecordset->MoveNext();
				}
				//ado.m_pRecordset->Close();
				ado.CloseRecordset();
				if (!is_exist)	 //更新的名字在数据库中查找不到的
				{
					////////////先更新Building表/////////////////////
					//CString strSql;

// 					strSql.Format(_T("delete * from Building_ALL where Building_Name = '%s' "),m_name_old);
// 					ado.m_pConnection->Execute(strSql.GetString(),NULL,adCmdText
					 strSql.Format(_T("update Building_ALL set Building_Name='%s' where Building_Name='%s' "),m_name_new,m_name_old);

					ado.m_pConnection->Execute(strSql.GetString(),NULL,adCmdText);

					strSql.Format(_T("update Building set Building_Name='%s',Main_BuildingName='%s' where Building_Name='%s'"),m_name_new,m_name_new,m_name_old);
					//MessageBox(strSql);
					ado.m_pConnection->Execute(strSql.GetString(),NULL,adCmdText);


					strSql.Format(_T("select * from ALL_NODE where Building_Name='%s' order by Building_Name"),m_name_old);
				 
				   bado.m_pRecordset=bado.OpenRecordset(strSql);
					while(VARIANT_FALSE== bado.m_pRecordset->EndOfFile)
					{
						strSql.Format(_T("update ALL_NODE set Building_Name='%s',MainBuilding_Name='%s' where Building_Name='%s'"),m_name_new,m_name_new,m_name_old);
						bado.m_pConnection->Execute(strSql.GetString(),NULL,adCmdText);
						bado.m_pRecordset->MoveNext();
					}

					//m_pRs->Close();
					bado.CloseRecordset();
				
				}
				else
				{

					return FALSE;
				}

				break;	
			}
		case 1:		//Floor
			{	 
			CString subnetname;
			     strSql=_T("select * from Building where Default_SubBuilding=-1");
				// m_pRs->Open((_variant_t)strSql,_variant_t((IDispatch *)m_pCon,true),adOpenStatic,adLockOptimistic,adCmdText);
				 ado.m_pRecordset=ado.OpenRecordset(strSql);
				 while(VARIANT_FALSE== ado.m_pRecordset->EndOfFile)
				 {	   	
				  subnetname.Empty();
				 subnetname= ado.m_pRecordset->GetCollect("Building_Name");	
				  ado.m_pRecordset->MoveNext();
				 }
				// m_pRs->Close();
				ado.CloseRecordset();
// 				HTREEITEM parentnode=GetParentItem(m_hSelItem);
// 
// 				CString subnetname=GetItemText(parentnode);//m_pRs->GetCollect("Building_Name");

				//  m_pRs->Close();
				strSql.Format(_T("select * from ALL_NODE where Building_Name='%s' and Floor_name='%s' order by Building_Name"),subnetname,m_name_new);
				//m_pRs->Open((_variant_t)strSql,_variant_t((IDispatch *)m_pCon,true),adOpenStatic,adLockOptimistic,adCmdText);
				 bado.m_pRecordset=bado.OpenRecordset(strSql);
				while(VARIANT_FALSE==bado.m_pRecordset->EndOfFile)
				{	   	 str_temp.Empty();
				str_temp=bado.m_pRecordset->GetCollect("Floor_name");
				if (str_temp.Compare(m_name_new)==0)
				{is_exist=TRUE;
				break;
				}			
				bado.m_pRecordset->MoveNext();
				}
				//m_pRs->Close();
				bado.CloseRecordset();
				/*if (is_exist)
				{
					return FALSE;
				} 
				else*/
				{  
				strSql.Format(_T("select * from ALL_NODE where Building_Name='%s' and Floor_name='%s' order by Building_Name"),subnetname,m_name_old);
				//m_pRs->Open((_variant_t)strSql,_variant_t((IDispatch *)m_pCon,true),adOpenStatic,adLockOptimistic,adCmdText);
				bado.m_pRecordset=bado.OpenRecordset(strSql);
				while(VARIANT_FALSE==bado.m_pRecordset->EndOfFile)
				{
					strSql.Format(_T("update ALL_NODE set Floor_name='%s' where Floor_name='%s'"),m_name_new,m_name_old);
					bado.m_pConnection->Execute(strSql.GetString(),NULL,adCmdText);
					bado.m_pRecordset->MoveNext();
				}
				//m_pRs->Close();
				bado.CloseRecordset(); 
				}

				 
				break;
			}
		case 2:		//Room
			{
				//Subnet
				HTREEITEM root=GetRootItem();
				CString subnetname=GetItemText(root);
				//Floor
				HTREEITEM parentnode=GetParentItem(m_hSelItem);
				CString Floorname=GetItemText(parentnode);
				/*strSql.Format(_T("select * from ALL_NODE where Building_Name='%s' and Floor_name='%s' and Room_name='%s' order by Building_Name"),subnetname,Floorname,m_name_new);
				m_pRs->Open((_variant_t)strSql,_variant_t((IDispatch *)m_pCon,true),adOpenStatic,adLockOptimistic,adCmdText);

				while(VARIANT_FALSE==m_pRs->EndOfFile)
				{	   	 str_temp.Empty();
				str_temp=m_pRs->GetCollect("Room_name");
				if (str_temp.Compare(m_name_new)==0)
				{	   is_exist=TRUE;
				break;
				}			
				m_pRs->MoveNext();
				}
				m_pRs->Close();


				if (is_exist)
				{
					return FALSE;
				} 
				else*/
				{
				strSql.Format(_T("select * from ALL_NODE where Building_Name='%s' and Floor_name='%s' and Room_name='%s' order by Building_Name"),subnetname,Floorname,m_name_old);
				//m_pRs->Open((_variant_t)strSql,_variant_t((IDispatch *)m_pCon,true),adOpenStatic,adLockOptimistic,adCmdText);
				bado.m_pRecordset=bado.OpenRecordset(strSql);
				while(VARIANT_FALSE==bado.m_pRecordset->EndOfFile)
				{
					strSql.Format(_T("update ALL_NODE set Room_name='%s' where Room_name='%s'"),m_name_new,m_name_old);
					bado.m_pConnection->Execute(strSql.GetString(),NULL,adCmdText);
					bado.m_pRecordset->MoveNext();
				}
				//m_pRs->Close();
				bado.CloseRecordset();
				 
				}


				break;
			}
		case 3:		//Device Name Leaf
			{
             #if 1
                if (m_name_old.CompareNoCase(m_name_new)==0)
                {
                    return FALSE;
                }
                CMainFrame* pFrame=(CMainFrame*)(AfxGetApp()->m_pMainWnd);
                for (int i=0;i< pFrame->m_product.size();i++){
                    if (m_hSelItem == pFrame->m_product.at(i).product_item)
                    {     

                        CString strIPAddress;
                        int IPPort=0;
                        int ComPort=0;
                        int brandrate = 0;
                        int ModbusID=0;
                        CString strSql,temp_serial;
                        int sn=pFrame->m_product.at(i).serial_number;
                        temp_serial.Format(_T("%d"),sn);
                        int  int_product_type = pFrame->m_product.at(i).product_class_id;


                        if((int_product_type == PM_MINIPANEL) || (int_product_type == PM_CM5) )//||(int_product_type == PM_TSTAT6)
                        {
                            WritePrivateProfileStringW(temp_serial,_T("NewName"),m_name_new,g_achive_device_name_path);
                            WritePrivateProfileStringW(temp_serial,_T("WriteFlag"),_T("1"),g_achive_device_name_path);
                        }
                       
                        if (FALSE)    //
                        {
                            int communicationType = pFrame->m_product.at(i).protocol;  
                            SetCommunicationType(communicationType); 
							ModbusID = pFrame->m_product.at(i).product_id;
                            if (communicationType==0)
                            {
                                ComPort =  pFrame->m_product.at(i).ncomport;
                                brandrate = pFrame->m_product.at(i).baudrate;
                               

                                if (open_com(ComPort))
                                {
                                    Change_BaudRate(brandrate);
                                    if(m_name_new.GetLength()> 17)	//长度不能大于结构体定义的长度;
                                    {
                                        m_name_new.Delete(16,m_name_new.GetLength()-16);
                                    }

                                    char cTemp1[16];
                                    memset(cTemp1,0,16);
                                    WideCharToMultiByte( CP_ACP, 0, m_name_new.GetBuffer(), -1, cTemp1, 16, NULL, NULL ); 
                                    unsigned char Databuffer[16];
                                    memcpy_s(Databuffer,16,cTemp1,16);
                                    if (Write_Multi(ModbusID,Databuffer,715,16,10)<0)
                                    {
                                        if( int_product_type == PM_TSTAT6) 
                                        {
                                            WritePrivateProfileStringW(temp_serial,_T("NewName"),m_name_new,g_achive_device_name_path);
                                            WritePrivateProfileStringW(temp_serial,_T("WriteFlag"),_T("1"),g_achive_device_name_path);
                                        }
                                    }


                                    close_com();

                                }
                            } 
                            else
                            {
                                strIPAddress = pFrame->m_product.at(i).BuildingInfo.strIp;
                                IPPort = _wtoi(pFrame->m_product.at(i).BuildingInfo.strIpPort);
                                if (Open_Socket2(strIPAddress,IPPort))
                                {
                                    if(m_name_new.GetLength()> 17)	//长度不能大于结构体定义的长度;
                                    {
                                        m_name_new.Delete(16,m_name_new.GetLength()-16);
                                    }

                                    char cTemp1[16];
                                    memset(cTemp1,0,16);
                                    WideCharToMultiByte( CP_ACP, 0, m_name_new.GetBuffer(), -1, cTemp1, 16, NULL, NULL ); 
                                    unsigned char Databuffer[16];
                                    memcpy_s(Databuffer,16,cTemp1,16);
                                    if (Write_Multi(ModbusID,Databuffer,715,16,10)<0)
                                    {
                                        if( int_product_type == PM_TSTAT6) 
                                        {
                                            WritePrivateProfileStringW(temp_serial,_T("NewName"),m_name_new,g_achive_device_name_path);
                                            WritePrivateProfileStringW(temp_serial,_T("WriteFlag"),_T("1"),g_achive_device_name_path);
                                        }
                                    }
                                }                              

                            }
                        }            
                        CBADO bado;
                        bado.SetDBPath(g_strCurBuildingDatabasefilePath);
                        bado.OnInitADOConn(); 
                        strSql.Format(_T("update ALL_NODE set Product_name='%s' where Product_name='%s' and Serial_ID='%s'"),m_name_new,m_name_old,temp_serial);
                        bado.m_pConnection->Execute(strSql.GetString(),NULL,adCmdText);
                        bado.CloseConn();
                        
                        if (int_product_type == PM_TSTAT6 || int_product_type == PM_TSTAT7)
                        {
                            if(m_name_new.GetLength()> 16)	//长度不能大于结构体定义的长度;
                            {
                                m_name_new.Delete(16,m_name_new.GetLength()-16);
                            }
                            WritePrivateProfileStringW(temp_serial,_T("NewName"),m_name_new,g_achive_device_name_path);
                            WritePrivateProfileStringW(temp_serial,_T("WriteFlag"),_T("1"),g_achive_device_name_path);
                            CreateThread(NULL,NULL,_Background_Write_Name,this,NULL,0); 
                        }

                        return TRUE;
                    }
                }
            #endif

				break;
			}
		}
	}
	catch(_com_error e)
	{
		/* AfxMessageBox(e.Description());*/
		//MessageBox(m_name_new+_T("  has been here\n Please change another name!"));
		bado.CloseConn();
		return FALSE;
		//m_pCon->Close();
	}
	//m_pCon->Close();  
	bado.CloseConn();
	CMainFrame* pFrame=(CMainFrame*)(AfxGetApp()->m_pMainWnd);
	::PostMessage(pFrame->m_hWnd, WM_MYMSG_REFRESHBUILDING,0,0);

	return TRUE;
}
BOOL CImageTreeCtrl::UpdateDataToDB_Connect(){
       if (m_level==0)
       {
           return UpdateDataToDB_Floor();
       }
       if (m_name_old.CompareNoCase(m_name_new)==0)
       {
          return FALSE;
       }
    CMainFrame* pFrame=(CMainFrame*)(AfxGetApp()->m_pMainWnd);
    for (int i=0;i< pFrame->m_product.size();i++){
      if (m_hSelItem == pFrame->m_product.at(i).product_item)
      {     
          
              CString strIPAddress;
              int IPPort=0;
              int ComPort=0;
              int brandrate = 0;
              int ModbusID=0;
             CString strSql,temp_serial;
            int sn=pFrame->m_product.at(i).serial_number;
            temp_serial.Format(_T("%d"),sn);
            int  int_product_type = pFrame->m_product.at(i).product_class_id;

            /* if( int_product_type == PM_TSTAT6) 
            {
            WritePrivateProfileStringW(temp_serial,_T("NewName"),m_name_new,g_achive_device_name_path);
            WritePrivateProfileStringW(temp_serial,_T("WriteFlag"),_T("1"),g_achive_device_name_path);
            }
            return TRUE;*/
            if((int_product_type == PM_MINIPANEL) || (int_product_type == PM_CM5) )//  ||(int_product_type == PM_TSTAT6 )
            {
                WritePrivateProfileStringW(temp_serial,_T("NewName"),m_name_new,g_achive_device_name_path);
                WritePrivateProfileStringW(temp_serial,_T("WriteFlag"),_T("1"),g_achive_device_name_path);
            }
            
             // int_product_type == PM_TSTAT6
            if (FALSE)
            {
                int communicationType = pFrame->m_product.at(i).protocol;  
                ModbusID = pFrame->m_product.at(i).product_id;
                SetCommunicationType(communicationType);
                if (communicationType==0)
                {
                    
                    ComPort =  pFrame->m_product.at(i).ncomport;
                    brandrate = pFrame->m_product.at(i).baudrate;
                   
                    if (open_com(ComPort))
                    {
                        Change_BaudRate(brandrate);
                        
                        char cTemp1[16];
                        memset(cTemp1,0,16);
                        WideCharToMultiByte( CP_ACP, 0, m_name_new.GetBuffer(), -1, cTemp1, 16, NULL, NULL ); 
                        unsigned char Databuffer[16];
                        memcpy_s(Databuffer,16,cTemp1,16);
                        if (Write_Multi(ModbusID,Databuffer,715,16,10)<0)
                        {
                            
                        }
                        
                        
                        close_com();

                    }
                } 
                else
                {
                    strIPAddress = pFrame->m_product.at(i).BuildingInfo.strIp;
                    IPPort = _wtoi(pFrame->m_product.at(i).BuildingInfo.strIpPort);
                    if (Open_Socket2(strIPAddress,IPPort))
                    {
                        if(m_name_new.GetLength()> 17)	//长度不能大于结构体定义的长度;
                        {
                            m_name_new.Delete(16,m_name_new.GetLength()-16);
                        }

                        char cTemp1[16];
                        memset(cTemp1,0,16);
                        WideCharToMultiByte( CP_ACP, 0, m_name_new.GetBuffer(), -1, cTemp1, 16, NULL, NULL ); 
                        unsigned char Databuffer[16];
                        memcpy_s(Databuffer,16,cTemp1,16);
                        if (Write_Multi(ModbusID,Databuffer,715,16,10)<0)
                        {
                            if( int_product_type == PM_TSTAT6) 
                            {
                                WritePrivateProfileStringW(temp_serial,_T("NewName"),m_name_new,g_achive_device_name_path);
                                WritePrivateProfileStringW(temp_serial,_T("WriteFlag"),_T("1"),g_achive_device_name_path);
                            }
                        }
                    }
                      
                }
            }
            CBADO bado;
            bado.SetDBPath(g_strCurBuildingDatabasefilePath);
            bado.OnInitADOConn(); 
            strSql.Format(_T("update ALL_NODE set Product_name='%s' where Product_name='%s' and Serial_ID='%s'"),m_name_new,m_name_old,temp_serial);
            bado.m_pConnection->Execute(strSql.GetString(),NULL,adCmdText);
            bado.CloseConn();

            if( int_product_type == PM_TSTAT6 || int_product_type == PM_TSTAT7) 
            {

                if(m_name_new.GetLength()> 16)	//长度不能大于结构体定义的长度;
                {
                    m_name_new.Delete(16,m_name_new.GetLength()-16);
                }
                WritePrivateProfileStringW(temp_serial,_T("NewName"),m_name_new,g_achive_device_name_path);
                WritePrivateProfileStringW(temp_serial,_T("WriteFlag"),_T("1"),g_achive_device_name_path);
                CreateThread(NULL,NULL,_Background_Write_Name,this,NULL,0);
            }
            return TRUE;
      }
    }
    
    return FALSE; 
}
bool CImageTreeCtrl::DoDeleteItem(HTREEITEM hItem)
{
         m_level=get_item_level(hItem);
         m_name_old=GetItemText(hItem);
    CBADO bado;
    bado.SetDBPath(g_strCurBuildingDatabasefilePath);
    bado.OnInitADOConn(); 


    ::CoInitialize(NULL);
    try 
    {
        ////////////////////////////////////////////////////////////////////////////////////////////
        //获取数据库名称及路径
        /////////////////////////////////////////////////////////////////////////////////////////////////
        //连接数据库

        CString strSql;   BOOL is_exist=FALSE;	  CString str_temp;
        if(m_level >= 2)
        {
            HTREEITEM root=GetRootItem();
            CString subnetname=GetItemText(root);
            //Room
            HTREEITEM parentnode=GetParentItem(hItem);
            CString Roomname=GetItemText(parentnode);
            //Floor
            HTREEITEM floornode=GetParentItem(parentnode);
            CString Floorname=GetItemText(floornode);
             
            

                strSql.Format(_T("select * from ALL_NODE where  Product_name='%s' order by Building_Name"),m_name_old);
                //m_pRs->Open((_variant_t)strSql,_variant_t((IDispatch *)m_pCon,true),adOpenStatic,adLockOptimistic,adCmdText);
                bado.m_pRecordset=bado.OpenRecordset(strSql);
                while(VARIANT_FALSE==bado.m_pRecordset->EndOfFile)
                {
                    CString temp_serial;
                  
                    temp_serial = bado.m_pRecordset->GetCollect("Serial_ID");
                
                    strSql.Format(_T("delete * from ALL_NODE  where  Serial_ID='%s'"),temp_serial);
                    bado.m_pConnection->Execute(strSql.GetString(),NULL,adCmdText);
                    bado.m_pRecordset->MoveNext();
                }
                
                bado.CloseRecordset();
              
        }
        
        }
        catch(_com_error e)
        {
             
            bado.CloseConn();
            return FALSE;
            //m_pCon->Close();
        }
        //m_pCon->Close();  
        bado.CloseConn();
        CMainFrame* pFrame=(CMainFrame*)(AfxGetApp()->m_pMainWnd);
        ::PostMessage(pFrame->m_hWnd, WM_MYMSG_REFRESHBUILDING,0,0);
    return true;
}
// CImageTreeCtrl 消息处理程序
bool CImageTreeCtrl::HandleKeyDown(WPARAM wParam, LPARAM lParam) {
	bool bCtrl = (::GetKeyState(VK_CONTROL) & 0x8000) != 0;
	bool bShift = (::GetKeyState(VK_SHIFT) & 0x8000) != 0;
	  m_hSelItem = GetSelectedItem();
	m_level=get_item_level(m_hSelItem);
	m_name_old=GetItemText(m_hSelItem);
	if(GetEditControl() == 0) {
		method fnc = m_Keymap[int(wParam)][bCtrl][bShift];
		if(fnc)
			return (this->*fnc)(m_hSelItem);
	}

	// under some circumstances we need to check some more keys
	switch(int(wParam)) {
	case VK_ESCAPE:
		/*if(m_pDragData) {
			DragStop();
			return true;
		}*/
		/*FALLTHRU*/

	case VK_RETURN:
		if(GetEditControl() != 0) {
			GetEditControl()->SendMessage(WM_KEYDOWN, wParam, lParam);

			return true;
		}
		break;

	default:
		break;
	}

	return false;
}
BOOL CImageTreeCtrl::PreTranslateMessage(MSG* pMsg) {
	switch(pMsg->message) {
	case WM_KEYDOWN:
		if(HandleKeyDown(pMsg->wParam, pMsg->lParam))
			return true;
		break;

	default:
		break;
	}
	return CTreeCtrl::PreTranslateMessage(pMsg);
}
void CImageTreeCtrl::OnBeginlabeledit(NMHDR* pNMHDR, LRESULT* pResult) 
{
	TV_DISPINFO* pTVDispInfo = (TV_DISPINFO*)pNMHDR;
	*pResult = 1;

	if(CanEditLabel(pTVDispInfo->item.hItem))
		*pResult = 0;
}
void CImageTreeCtrl::OnEndlabeledit(NMHDR* pNMHDR, LRESULT* pResult) 
{
	TV_DISPINFO* pTVDispInfo = (TV_DISPINFO*)pNMHDR;
	TVITEM & item = pTVDispInfo->item;
	*pResult = 1;

	if(item.pszText && CanSetLabelText(item)) {
	    if (m_name_new.IsEmpty())
	    {
		item.pszText=m_name_old.GetBuffer();
		SetItem(&item);
		*pResult = 0;
		 return;
	    }
	    
         if (product_sort_way == SORT_BY_BUILDING_FLOOR )
        {  
            if(!UpdateDataToDB_Floor()){
                item.pszText=m_name_old.GetBuffer();
            }
        }
        else if (product_sort_way ==SORT_BY_CONNECTION)
        {
             if(!UpdateDataToDB_Connect()){
               item.pszText=m_name_old.GetBuffer();
             }
        } 
		else
		{
		item.pszText=m_name_old.GetBuffer();
		}
		SetItem(&item);
		*pResult = 0;
	}
}
bool CImageTreeCtrl::CanEditLabel(HTREEITEM hItem) {
	//TRACE1(_T("CEditTreeCtrl::CanEditLabel('%s')\n"), LPCTSTR(GetItemText(hItem)));
	return true;
}
bool CImageTreeCtrl::CanSetLabelText(TVITEM & item) {
	//TRACE1(_T("CEditTreeCtrl::CanSetLabelText('%s')\n"), item.pszText);
	m_name_new=item.pszText;
	return true;
}
BOOL CImageTreeCtrl::SetItemImage(HTREEITEM hItem, int nImage, int nSelectedImage)
{
	//tree0412 屏蔽下面原有的
#if 0
	BOOL bReturn=CTreeCtrl::SetItemImage( hItem, nImage, nSelectedImage );

	HTREEITEM hParentItem;//,hBrotherItem;	
	static int dd=0;
	//查找子节点，没有就结束
	hParentItem=GetParentItem(hItem);
	int brother_nImage,brother_nSelectedImage;
	if(hParentItem)
	{	 
//Subnet,Floor_xx,Room_xx 只有当全部设备没有连接时才显示“没连接图标”，否则不改变这三个图标
#if 0  //LSC
		//have parentitem 
		if(!is_connection_by_image(nImage,nSelectedImage))
		{//this is disconnection
			GetItemImage(hParentItem,brother_nImage,brother_nSelectedImage);
			if(is_connection_by_image(brother_nImage,brother_nSelectedImage))
				SetItemImage(hParentItem,6,brother_nSelectedImage);
			return bReturn;////////////////////unconnection return ;
		}	

		/////////////prev
		hBrotherItem=GetPrevSiblingItem(hItem);
		if(hBrotherItem)
			GetItemImage(hBrotherItem,brother_nImage,brother_nSelectedImage);
		while(hBrotherItem)
		{
			if(!is_connection_by_image(brother_nImage,brother_nSelectedImage))
			{//one brother is disconnection
				GetItemImage(hParentItem,brother_nImage,brother_nSelectedImage);
				if(is_connection_by_image(brother_nImage,brother_nSelectedImage))
					SetItemImage(hParentItem,6,brother_nSelectedImage);
				return bReturn;////////////////////unconnection return ;
			}			
			hBrotherItem=GetPrevSiblingItem(hBrotherItem);
			if(hBrotherItem)
				GetItemImage(hBrotherItem,brother_nImage,brother_nSelectedImage);
		}	
		/////////////next
		hBrotherItem=GetNextSiblingItem(hItem);
		if(hBrotherItem)
			GetItemImage(hBrotherItem,brother_nImage,brother_nSelectedImage);
		while(hBrotherItem)
		{
			if(!is_connection_by_image(brother_nImage,brother_nSelectedImage))
			{//one brother is disconnection
				GetItemImage(hParentItem,brother_nImage,brother_nSelectedImage);
				if(is_connection_by_image(brother_nImage,brother_nSelectedImage))
					SetItemImage(hParentItem,6,brother_nSelectedImage);
				return bReturn;////////////////////unconnection return ;
			}
			hBrotherItem=GetNextSiblingItem(hBrotherItem);
			if(hBrotherItem)
				GetItemImage(hBrotherItem,brother_nImage,brother_nSelectedImage);
		}		
#endif
		//every brother is connection
//		GetItemImage(hParentItem,brother_nImage,brother_nSelectedImage);
// 		if(!is_connection_by_image(brother_nImage,brother_nSelectedImage))
// 			SetItemImage(hParentItem,brother_nSelectedImage,brother_nSelectedImage);
	}
	return bReturn;
#endif
//tree0412

//tree0412
	BOOL bReturn=CTreeCtrl::SetItemImage( hItem, nImage, nSelectedImage );
	return bReturn;
//tree0412
}
int CImageTreeCtrl::get_item_level(HTREEITEM hItem)
{//return value 0,
	int r_v=0;
	HTREEITEM hParentItem=hItem;
	do{
		hParentItem=GetParentItem(hParentItem);
		r_v++;
	}while(hParentItem!=NULL);
	r_v--;//^-^	
	return r_v;
}

void CImageTreeCtrl::turn_item_image(HTREEITEM hItem,bool state)
{
//tree0412  //屏蔽下面原有的
#if 0
	int brother_nImage,brother_nSelectedImage;
	GetItemImage(hItem,brother_nImage,brother_nSelectedImage);
	
	HTREEITEM hselItem = GetSelectedItem();
 	if (state)  // online
	{
		int nCurImage = brother_nImage == 6 ? brother_nSelectedImage : brother_nImage;
	
		SetItemImage(hItem, nCurImage, nCurImage);		
	
	}
	else // offline
	{
		if (hselItem == hItem) //sel
		{
			SetItemImage(hItem, brother_nImage, 6);		
		}
		else  // unselected
		{
			SetItemImage(hItem, 6, brother_nImage);	
		}
		
	}

	/*
	if(brother_nImage==0 && brother_nSelectedImage==0 && state==false)						
		SetItemImage(hItem, 6, brother_nSelectedImage);								
	else if(brother_nImage==6 && brother_nSelectedImage==0 && state==true)							
		SetItemImage(hItem,brother_nImage-6,brother_nSelectedImage);						

	if(brother_nImage==1 && brother_nSelectedImage==1 && state==false)						
		SetItemImage(hItem,6,brother_nSelectedImage);					
	else if(brother_nImage==6 && brother_nSelectedImage==1 && state==true)						
		SetItemImage(hItem,brother_nImage-5,brother_nSelectedImage);					

// 	if(brother_nImage==2 && brother_nSelectedImage==2 && state==false)						
// 		SetItemImage(hItem,6,brother_nSelectedImage);					
// 	else if(brother_nImage==6 && brother_nSelectedImage==2 && state==true)						
// 		SetItemImage(hItem,brother_nImage-4,brother_nSelectedImage);					
	if(brother_nImage==2 && brother_nSelectedImage==2 && state==false)						
		SetItemImage(hItem,6,brother_nSelectedImage);					
	else if(brother_nImage==6 && brother_nSelectedImage==2 && state==true)						
		SetItemImage(hItem,brother_nImage-4,brother_nSelectedImage);					

	if(brother_nImage==3 && brother_nSelectedImage==3 && state==false)						
		SetItemImage(hItem,6,brother_nSelectedImage);					
	else if(brother_nImage==6 && brother_nSelectedImage==3 && state==true)						
		SetItemImage(hItem,brother_nImage-3,brother_nSelectedImage);					

	if(brother_nImage==4 && brother_nSelectedImage==4 && state==false)						
		SetItemImage(hItem,6,brother_nSelectedImage);					
	else if(brother_nImage==6 && brother_nSelectedImage==4 && state==true)						
		SetItemImage(hItem,brother_nImage-2,brother_nSelectedImage);					

	if(brother_nImage==5 && brother_nSelectedImage==5 && state==false)						
		SetItemImage(hItem,6,brother_nSelectedImage);					
	else if(brother_nImage==6 && brother_nSelectedImage==5 && state==true)						
		SetItemImage(hItem,brother_nImage-1,brother_nSelectedImage);	
	//	*/
#endif
//tree0412

//tree0412
	int brother_nImage,brother_nSelectedImage;
	GetItemImage(hItem,brother_nImage,brother_nSelectedImage);

// 	if(state == false)&&(brother_nImage!= 7)
// 		SetItemImage(hItem,brother_nImage+1,brother_nSelectedImage+1);
// 	else
// 		SetItemImage(hItem,brother_nImage-1,brother_nSelectedImage-1);
	/*
	switch(brother_nImage)
	{
	case 0:
	case 1:
	case 2:
	case 3:
	case 4:
	case 5:	
		break;
	case 6:
		if(state == false)
		{
			brother_nImage++;
			brother_nSelectedImage++;
		}
		break;
	case 7:
		if(state == true)
		{
			brother_nImage--;
			brother_nSelectedImage--;
		}
		break;
	case 8:
		if(state == false)
		{
			brother_nImage++;
			brother_nSelectedImage++;
		}
		break;
	case 9:
		if(state == true)
		{
			brother_nImage--;
			brother_nSelectedImage--;
		}
		break;
	case 10:
		if(state == false)
		{
			brother_nImage++;
			brother_nSelectedImage++;
		}
		break;
	case 11:
		if(state == true)
		{
			brother_nImage--;
			brother_nSelectedImage--;
		}
		break;
	case 12:
		if(state == false)
		{
			brother_nImage++;
			brother_nSelectedImage++;
		}
		break;
	case 13:
		if(state == true)
		{
			brother_nImage--;
			brother_nSelectedImage--;
		}
		break;
	case 14:
		if(state == false)
		{
			brother_nImage++;
			brother_nSelectedImage++;
		}
		break;
	case 15:
		if(state == true)
		{
			brother_nImage--;
			brother_nSelectedImage--;
		}
		break;
	case 16:
		if(state == false)
		{
			brother_nImage++;
			brother_nSelectedImage++;
		}
		break;
	case 17:
		if(state == true)
		{
			brother_nImage--;
			brother_nSelectedImage--;
		}
		break;

	}
	*/

	switch(brother_nImage)
	{
	case 0:
	case 1:
	case 2:
	case 3:
	case 4:
	case 5:	
		break;
	case 6:
	case 8:
	case 10:
	case 12:
	case 14:
	case 16:
    case PM_PM5E:
	case 18:
	case 20:
		if(state == false)
		{
			brother_nImage++;
			brother_nSelectedImage++;
		}
	break;
		case 7:
	case 9:
	case 11:
	case 13:
	case 15:
	case 17:
	case 19:
	case 21:
		if(state == true)
		{
			brother_nImage--;
			brother_nSelectedImage--;
		}
		break;
	case 22:
	   if (state == false)
	   {
		   brother_nImage++;
		   brother_nSelectedImage++;
	   }
	break;

	}
	SetItemImage(hItem,brother_nImage,brother_nSelectedImage);


//tree0412


}

BOOL CImageTreeCtrl::is_connection_by_image(int nImage,int nSelectedImage)
{//return value :true is connection,false is unconnection
//tree0412 //屏蔽下面原有的
#if 0
	if (nImage == 6)		
		return false;	
	else		
		return true;
#endif
//tree0412

//tree0412
	switch(nImage)
	{
	case 0:
	case 1:
	case 2:
	case 3:
	case 4:
	case 5:	
	case 6:
	case 8:
	case 10:
	case 12:
	case 14:
	case 16:
    case PM_PM5E:
		return true;
	case 7:
	case 9:
	case 11:		
	case 13:		
	case 15:	
	case 17:
		return false;
	}

//tree0412
	return false;
}
int CImageTreeCtrl::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CTreeCtrl::OnCreate(lpCreateStruct) == -1)
		return -1;



	// TODO:  Add your specialized creation code here

	return 0;
}
bool CImageTreeCtrl::CanInsertItem(HTREEITEM hItem) {
	TRACE(_T("CEditTreeCtrl::CanInsertItem('%s')\n"), (hItem && hItem != TVI_ROOT) ? LPCTSTR(GetItemText(hItem)) : _T("<Root>"));
	return true;
}
void CImageTreeCtrl::OnContextMenu(CWnd* /*pWnd*/, CPoint point)
{
	DisplayContextMenu(point);
}

bool CImageTreeCtrl::CanDeleteItem(HTREEITEM hItem) {
	TRACE(_T("CEditTreeCtrl::CanDeleteItem('%s')\n"), LPCTSTR(GetItemText(hItem)));
	return true;
}
HTREEITEM CImageTreeCtrl::InsertSubnetItem(LPTVINSERTSTRUCT lpInsertStruct)
{
	HTREEITEM hti = InsertItem(lpInsertStruct);
	SetItemData(hti,m_nSubnetItemData++);
	return hti;
}

// add a floor node
HTREEITEM CImageTreeCtrl::InsertFloorItem(LPTVINSERTSTRUCT lpInsertStruct)
{
	HTREEITEM hti = InsertItem(lpInsertStruct);
	SetItemData(hti,m_nFloorItemData++);
	return hti;
}
// add a room node 
HTREEITEM CImageTreeCtrl::InsertRoomItem(LPTVINSERTSTRUCT lpInsertStruct)
{
	HTREEITEM hti = InsertItem(lpInsertStruct);
	SetItemData(hti,m_nRoomItemData++);
	return hti;
}
// add a device node
HTREEITEM CImageTreeCtrl::InsertDeviceItem(LPTVINSERTSTRUCT lpInsertStruct)
{
	HTREEITEM hti = InsertItem(lpInsertStruct);
	SetItemData(hti,m_nDeviceItemData++);
	return hti;
}

BOOL CImageTreeCtrl::Retofline( HTREEITEM hItem )//tree0412
{
	int brother_nImage,brother_nSelectedImage;
	GetItemImage(hItem,brother_nImage,brother_nSelectedImage);
	return is_connection_by_image(brother_nImage,brother_nSelectedImage);



}


#pragma region color_function
//This function add by Fance ,used for when some click ,the device will change color
//and user will know which one is selected
//add time  2014 01 06
//添加设置字体、颜色、粗体的函数
	void CImageTreeCtrl::SetItemFont(HTREEITEM hItem, LOGFONT& logfont)
{
	Color_Font cf;
	if( !m_mapColorFont.Lookup( hItem, cf ) )
		cf.color = (COLORREF)-1;
	cf.logfont = logfont;
	m_mapColorFont[hItem] = cf;
}

void CImageTreeCtrl::SetItemBold(HTREEITEM hItem, BOOL bBold)
{
	SetItemState( hItem, bBold ? TVIS_BOLD: 0, TVIS_BOLD );
}

void CImageTreeCtrl::SetSelectSerialNumber(unsigned int nserial)
{
	m_serial_number = nserial;
}

unsigned int CImageTreeCtrl::GetSelectSerialNumber()
{
	return m_serial_number;
}


void CImageTreeCtrl::SetSelectItem(HTREEITEM hItem)
{
	if(old_hItem != hItem)
	{
		if(old_hItem!=NULL)
		{
			SetItemBold(old_hItem,0);
			SetItemColor( old_hItem, RGB(0,0,0));
		}
		old_hItem = hItem;
	}

	SetItemBold(hItem,1);
	SetItemColor( hItem, RGB(255,0,0));
}

void CImageTreeCtrl::SetItemColor(HTREEITEM hItem, COLORREF color)
{
	Color_Font cf;
	if( !m_mapColorFont.Lookup( hItem, cf ) )
		cf.logfont.lfFaceName[0] = '/0';
	cf.color = color;
	m_mapColorFont[hItem] = cf;
	Invalidate();
}

BOOL CImageTreeCtrl::GetItemFont(HTREEITEM hItem, LOGFONT * plogfont)
{
	Color_Font cf;
	if( !m_mapColorFont.Lookup( hItem, cf ) )
		return FALSE;
	if( cf.logfont.lfFaceName[0] == '/0' ) 
		return FALSE;
	*plogfont = cf.logfont;
	return TRUE;

}

BOOL CImageTreeCtrl::GetItemBold(HTREEITEM hItem)
{
	return GetItemState( hItem, TVIS_BOLD ) & TVIS_BOLD;
}

COLORREF CImageTreeCtrl::GetItemColor(HTREEITEM hItem)
{
	// Returns (COLORREF)-1 if color was not set
	Color_Font cf;
	if( !m_mapColorFont.Lookup( hItem, cf ) )
		return (COLORREF)-1;
	return cf.color;

}



// void CImageTreeCtrl::FillBkGroudColor(HTREEITEM& hSelItem)
// {
// 	CRect rc;
// 	GetItemRect(hSelItem, &rc,TRUE);
// 	CDC* pDC = GetDC();
// 
// 	CBrush brush;
// 	LOGBRUSH lb;
// 	lb.lbColor = GetSysColor(COLOR_HIGHLIGHT);
// 	lb.lbStyle = BS_SOLID;
// 	brush.CreateBrushIndirect(&lb);
// 
// 	pDC->SetBkMode(OPAQUE);
// 	pDC->FillRect(&rc, &brush);
// }


//添加WM_PAINT 信息监控，重载OnPaint()函数来实现绘制

void CImageTreeCtrl::OnPaint()
{
try
{
	//获取当前绘制对象的DC
	CPaintDC dc(this);

	// 使用将要绘制的对象的DC创建一个memory DC
	//memory device context的概念：是在内存中创建一个结构来反映一个显示（屏幕区域、窗口、
	//打印机等）的表面。可以用来先在内存中准备好要显示的图像，从而实现双缓存，提高刷新
	//速度减少刷新时产生的闪烁。
	CDC memDC;
	//从当前DC创建内存对象
	memDC.CreateCompatibleDC( &dc );

	//定义CRect对象，用来确定区域
	CRect rcClip, rcClient;
	//获取当前对象的边界区域
	dc.GetClipBox( &rcClip );
	//获取当前对象的用户区域
	GetClientRect(&rcClient);

	// Select a compatible bitmap into the memory DC
	//创建一个bmp文件，作为memDC的内容
	//该文件的大小与用于区域相同
	CBitmap bitmap;
	bitmap.CreateCompatibleBitmap( &dc, rcClient.Width(), rcClient.Height() );
	memDC.SelectObject( &bitmap );

	// Set clip region to be same as that in paint DC
	//通过对象的边界区域创建CRgn对象
	CRgn rgn;
	rgn.CreateRectRgnIndirect( &rcClip );

	//rcClip.bottom = rcClip.bottom + 10;

	memDC.SelectClipRgn(&rgn);
	rgn.DeleteObject();

	// First let the control do its default drawing.
	//首先让控件自己进行默认的绘制，绘制到内存中
	CWnd::DefWindowProc( WM_PAINT, (WPARAM)memDC.m_hDC, 0 );

	//获取树状控件的第一个节点
	HTREEITEM hItem = GetFirstVisibleItem();

	//遍历这棵树
	int n = GetVisibleCount()+1;
	while( hItem && n--)
	{
		CRect rect;

		// Do not meddle with selected items or drop highlighted items
		//不对选中的节点和实行拖放功能的节点进行操作
		UINT selflag = TVIS_DROPHILITED;// | TVIS_SELECTED;

		//定义字体、颜色
		Color_Font cf;


		//设置字体
		if ( !(GetItemState( hItem, selflag ) & selflag )
			&& m_mapColorFont.Lookup( hItem, cf ))
		{
			CFont *pFontDC;
			CFont fontDC;
			LOGFONT logfont;

			if( cf.logfont.lfFaceName[0] != '/0' )
			{
				//用户定义了字体
				logfont = cf.logfont;
			}
			else
			{
				// 用户没有定义，使用系统字体
				CFont *pFont = GetFont();
				pFont->GetLogFont( &logfont );
			}

			//用户是否设定节点为加粗
			if( GetItemBold( hItem ) )
				logfont.lfWeight = 700;
			//创建字体
			fontDC.CreateFontIndirect( &logfont );
			pFontDC = memDC.SelectObject( &fontDC );

			//设置字体颜色
			if( cf.color != (COLORREF)-1 )
				memDC.SetTextColor( cf.color );

			//获取节点文字
			CString sItem = GetItemText( hItem );

			//获取节点区域
			GetItemRect( hItem, &rect, TRUE );
			//rect.bottom = rect.bottom + 2;
			//设置背景色为系统色
			memDC.FillSolidRect(&rect,GetSysColor( COLOR_WINDOW ));//clr);

			memDC.SetBkColor( GetSysColor( COLOR_WINDOW ) );
			//向内存中的图片写入内容,为该节点的内容
			memDC.TextOut( rect.left+2, rect.top+1, sItem );

			memDC.SelectObject( pFontDC );
		}
		hItem = GetNextVisibleItem( hItem );
	}
	

	//TRACE(_T("Fresh...ImageTreeCtrl\r\n"));
	dc.BitBlt( rcClip.left, rcClip.top, rcClip.Width(), rcClip.Height(), &memDC,
		rcClip.left, rcClip.top, SRCCOPY );
		memDC.DeleteDC();
		bitmap.DeleteObject();
}

catch (CException* e)
{
 TRACE(_T("Exception...ImageTreeCtrl\r\n"));
return;
}

	

}




void CImageTreeCtrl::OnKillFocus(CWnd* pNewWnd)
{
//	PostMessage(WM_PAINT,NULL,NULL);
	CTreeCtrl::OnKillFocus(pNewWnd);

	// TODO: Add your message handler code here
}



void CImageTreeCtrl::DisplayContextOtherMenu(CPoint & point) {
		CPoint pt(point);
		ScreenToClient(&pt);
		UINT flags;
		HTREEITEM hItem = HitTest(pt, &flags);
		bool bOnItem = (flags & TVHT_ONITEM) != 0;


		CMenu menu;
		VERIFY(menu.CreatePopupMenu());

		VERIFY(menu.AppendMenu(MF_STRING, ID_SORT_BY_CONNECTION, _T("Sort By Connection")));
		VERIFY(menu.AppendMenu(MF_STRING, ID_SORT_BY_FLOOR, _T("Sort By Floor")));
		

		if(menu.GetMenuItemCount() > 0)
			menu.TrackPopupMenu(TPM_LEFTALIGN, point.x, point.y, this);
}

void CImageTreeCtrl::DisplayContextMenu(CPoint & point) {
	CPoint pt(point);
	ScreenToClient(&pt);
	UINT flags;
	HTREEITEM hItem = HitTest(pt, &flags);
	bool bOnItem = (flags & TVHT_ONITEM) != 0;

// 	CMenu add;
// 	VERIFY(add.CreatePopupMenu());
// 	if(bOnItem) {
// 		if(CanInsertItem(GetParentItem(hItem)))
// 			VERIFY(add.AppendMenu(MF_STRING, ID_ADD_SIBLING, _T("New Sibling\tINS")));
// 		if(CanInsertItem(hItem))
// 			VERIFY(add.AppendMenu(MF_STRING, ID_ADD_CHILD, _T("New Child Item\tCtrl+INS")));
// 	}
// 	if(CanInsertItem(0))
// 		VERIFY(add.AppendMenu(MF_STRING, ID_ADD_ROOT, _T("New Root Item\tShift+INS")));

// 	CMenu sort;
// 	VERIFY(sort.CreatePopupMenu());
// 	VERIFY(sort.AppendMenu(MF_STRING, ID_SORT_LEVEL, _T("Current Level\tCtrl+S")));
// 	VERIFY(sort.AppendMenu(MF_STRING, ID_SORT_LEVELANDBELOW, _T("Current Level And Below\tCtrl+Shift+S")));

	CMenu menu;
	VERIFY(menu.CreatePopupMenu());
	if(bOnItem) 
	{
		if(CanEditLabel(hItem))
		{
			VERIFY(menu.AppendMenu(MF_STRING, ID_RENAME, _T("Rename\tF2")));
            VERIFY(menu.AppendMenu(MF_STRING, ID_DELETE, _T("Delete\tDel")));
			VERIFY(menu.AppendMenu(MF_STRING, ID_SORT_BY_CONNECTION, _T("Sort By Connection")));
			VERIFY(menu.AppendMenu(MF_STRING, ID_SORT_BY_FLOOR, _T("Sort By Floor")));
		}
// 		if(CanDeleteItem(hItem))
// 			VERIFY(menu.AppendMenu(MF_STRING, ID_DELETE, _T("Delete\tDEL")));
	}
// 	if(add.GetMenuItemCount() > 0)
// 		VERIFY(menu.AppendMenu(MF_POPUP, UINT(add.GetSafeHmenu()), _T("Add")));
// 	if(bOnItem) {
// 		if(menu.GetMenuItemCount() > 0)
// 			VERIFY(menu.AppendMenu(MF_SEPARATOR));
// 		VERIFY(menu.AppendMenu(MF_POPUP, UINT(sort.GetSafeHmenu()), _T("Sort")));
// 	}

	//ExtendContextMenu(menu);

	// maybe the menu is empty...
	if(menu.GetMenuItemCount() > 0)
		menu.TrackPopupMenu(TPM_LEFTALIGN, point.x, point.y, this);
}
void CImageTreeCtrl::OnRclick(NMHDR* pNMHDR, LRESULT* pResult) 
{
	*pResult = 1;
	UINT flags;
	CPoint point;
	GetCursorPos(&point);
	ScreenToClient(&point);
	HTREEITEM hItem = HitTest(point, &flags);
	if(hItem != NULL)
	{
		if(hItem && (flags & TVHT_ONITEM) && !(flags & TVHT_ONITEMRIGHT))
			SelectItem(hItem);
		ClientToScreen(&point);
		DisplayContextMenu(point);
	}
	else
	{
		ClientToScreen(&point);
		DisplayContextOtherMenu(point);
	}

}
#pragma endregion