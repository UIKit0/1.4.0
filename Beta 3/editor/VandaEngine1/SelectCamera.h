//Copyright (C) 2015 Zehne Ziba Co., Ltd.
//This file is licensed and distributed under MIT license

#pragma once
#include "afxcmn.h"


// CSelectCamera dialog

class CSelectCamera : public CDialog
{
	DECLARE_DYNAMIC(CSelectCamera)

public:
	CSelectCamera(CWnd* pParent = NULL);   // standard constructor
	virtual ~CSelectCamera();

// Dialog Data
	enum { IDD = IDD_DIALOG_SELECT_CAMERA };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	CInt cameraObjectIndex;
	CVoid InserItemToCameraList( CChar * objectName );
	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
public:
	CListCtrl m_listBoxCameraObjects;
public:
	afx_msg void OnBnClickedOk();
public:
	virtual INT_PTR DoModal();
	afx_msg void OnBnClickedAcivateAndRender();
};
