#pragma once

#include "msflexgrid1.h"
#include "MbPollOptions.h"
#include "MbpExterns.h"

#define WM_MBPOLL_CLOSED (WM_USER + 555)
// CMbPoll dialog

class CMbPoll : public CDialog
{
	DECLARE_DYNAMIC(CMbPoll)

public:
	CMbPoll(CWnd* pParent = NULL);   // standard constructor
	virtual ~CMbPoll();

// Dialog Data
	enum { IDD = IDD_MBPOLL };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL Create(UINT nIDTemplate, CWnd* pParentWnd = NULL);
	afx_msg void OnNcDestroy();
	virtual void PostNcDestroy();
	CWnd* mParent;
//	afx_msg void OnBnClickedCancel();
	afx_msg void OnBnClickedClose();
//	afx_msg void OnSetupOptions();
	CMbPollOptions mbPollOptions;
	CStatic ctrlLabel1;
	UINT_PTR timer1;
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	CEdit ctrlEditGrid1;
	CString strGrid1;
	CMsflexgrid ctrlGrid1;
	CMsflexgrid ctrlGrid2;
	CMsflexgrid ctrlGrid3;
	CMsflexgrid ctrlGrid4;
	CMsflexgrid ctrlGrid5;
	void ClickGrid(CMsflexgrid &grid);
	void ClickGrid1();
	void EnterCellGrid1();
	void SelChangeGrid1();
	void DblClickGrid1();
	void ClickGrid2();
	void EnterCellGrid2();
	void SelChangeGrid2();
	void DblClickGrid2();
	void ClickGrid3();
	void EnterCellGrid3();
	void SelChangeGrid3();
	void DblClickGrid3();
	void ClickGrid4();
	void EnterCellGrid4();
	void SelChangeGrid4();
	void DblClickGrid4();
	void ClickGrid5();
	void EnterCellGrid5();
	void SelChangeGrid5();
	void DblClickGrid5();

	DECLARE_EVENTSINK_MAP()

	afx_msg void OnKillfocusEditForGrid();

	int currentRow;
	int currentCol;
	int curGridForEdit;
	int noOfGridsVisible;

	virtual void OnOK();
	virtual void OnCancel();
	virtual BOOL OnInitDialog();
	afx_msg void OnSelchangeComboNoofgrids();
	CComboBox comboNoOfGrids;
	int valNoOfGrids;
	void displayGrids(int number);
	afx_msg void OnFileExit();
	void OnMbPollDestroyWindow();
	afx_msg void OnClickedButtonConfig1();
	afx_msg void OnClickedButtonConfig2();
	afx_msg void OnClickedButtonConfig3();
	afx_msg void OnClickedButtonConfig4();
	afx_msg void OnClickedButtonConfig5();
	CButton btnConfig1;
	CButton btnConfig2;
	CButton btnConfig3;
	CButton btnConfig4;
	CButton btnConfig5;
	void updateStaticConfig(int configNo);
	CStatic staticConfig1;
	CStatic staticConfig2;
	CStatic staticConfig3;
	CStatic staticConfig4;
	CStatic staticConfig5;
	void updateDataSharingVariables(int configNo);
	CButton btnStartStop1;
	CButton btnStartStop2;
	CButton btnStartStop3;
	CButton btnStartStop4;
	CButton btnStartStop5;
	afx_msg void OnClickedButtonStartStop1();
	afx_msg void OnClickedButtonStartStop2();
	afx_msg void OnClickedButtonStartStop3();
	afx_msg void OnClickedButtonStartStop4();
	afx_msg void OnClickedButtonStartStop5();
	void OnClickedButtonStartStop(CMsflexgrid &grid);
	afx_msg void OnClickedCheckTapData();
	CButton checkTapData;
};