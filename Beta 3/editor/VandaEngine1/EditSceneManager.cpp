//Copyright (C) 2015 Zehne Ziba Co., Ltd.
//This file is licensed and distributed under MIT license

// EditSceneManager.cpp : implementation file
//

#include "stdafx.h"
#include "VandaEngine1.h"
#include "EditSceneManager.h"


// CEditSceneManager dialog

IMPLEMENT_DYNAMIC(CEditSceneManager, CDialog)

CEditSceneManager::CEditSceneManager(CWnd* pParent /*=NULL*/)
	: CDialog(CEditSceneManager::IDD, pParent)
{

}

CEditSceneManager::~CEditSceneManager()
{
}

void CEditSceneManager::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_EDIT_SCENEMANAGER_OBJECT_PER_SPLIT, m_editBoxObjectsPerSplit);
}


BEGIN_MESSAGE_MAP(CEditSceneManager, CDialog)
	ON_EN_CHANGE(IDC_EDIT_SCENEMANAGER_OBJECT_PER_SPLIT, &CEditSceneManager::OnEnChangeEditScenemanagerObjectPerSplit)
	ON_BN_CLICKED(IDOK, &CEditSceneManager::OnBnClickedOk)
END_MESSAGE_MAP()


// CEditSceneManager message handlers

void CEditSceneManager::OnEnChangeEditScenemanagerObjectPerSplit()
{
	m_editBoxObjectsPerSplit.GetWindowTextA( m_strObjectsPerSplit );
	m_iObjectsPerSplit = (CInt)atof( m_strObjectsPerSplit );
}

void CEditSceneManager::OnOK()
{
	// TODO: Add your specialized code here and/or call the base class
	if( m_strObjectsPerSplit.IsEmpty() )
	{
		MessageBox( "Please enter the valid data for all the required fields", "Vanda Engine Error", MB_OK | MB_ICONERROR );
	}
	else
	{
		g_sceneManagerObjectsPerSplit = (CInt)m_iObjectsPerSplit;
		g_updateOctree = CTrue;
		CDialog::OnOK();
	}
}

BOOL CEditSceneManager::OnInitDialog()
{
	CDialog::OnInitDialog();

	m_strObjectsPerSplit.Format( "%d", g_sceneManagerObjectsPerSplit);
	m_editBoxObjectsPerSplit.SetWindowTextA( m_strObjectsPerSplit );
	m_iObjectsPerSplit = g_sceneManagerObjectsPerSplit;

	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}

void CEditSceneManager::OnBnClickedOk()
{
	// TODO: Add your control notification handler code here
	OnOK();
}
