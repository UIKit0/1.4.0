#pragma once
#include "afxcmn.h"
#include "afxwin.h"
#include "PrefabNameDlg.h"

// CPrefabDlg dialog

class CPrefabDlg : public CDialog
{
	DECLARE_DYNAMIC(CPrefabDlg)

public:
	CPrefabDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~CPrefabDlg();

// Dialog Data
	enum { IDD = IDD_DIALOG_PREFAB };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	std::string m_selectedPackageName;
	std::string m_selectedPrefabName;

	// //shows prefabs made in game engine
	afx_msg void OnBnClickedInsert();
	afx_msg void OnBnClickedOpen();
	afx_msg void OnBnClickedDelete();
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedNewPrefab();
	afx_msg void OnBnClickedRenamePrefab();
	CListCtrl m_listPrefabPackages;
	CListCtrl m_listPrefabs;
	CImageList m_prefabListImage;
	CImageList m_packageListImage;
	CListCtrl m_listPrefabProperties;
	CSliderCtrl m_slideLod;
	CButton m_btnAddPrefab;
	CButton m_btnOpenPrefab;
	CButton m_btnRenamePrefab;
	CButton m_btnDeletePrefab;
	CButton m_btnInsertPrefab;
	CButton m_btnNewPrefabPackage;
	CButton m_btnRenamePrefabPackage;
	CPrefabNameDlg* m_prefabNameDlg;
	afx_msg void OnBnClickedNewPackage();
	virtual BOOL OnInitDialog();
	afx_msg void OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	CVoid InsertItemToPackageList(CChar* packageName);
	CVoid InsertItemToPrefabList(CChar* prefabList);
	CInt m_packageIndex;
	CInt m_prefabIndex;

	afx_msg void OnBnClickedRenamePackage();
	afx_msg void OnBnClickedDeletePackage();
	afx_msg void OnLvnItemchangedListPrefabsProjects(NMHDR *pNMHDR, LRESULT *pResult);
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	CVoid InserItemToPropertiesList(char * properties);
	CButton m_btnDeletePrefabPackage;
};
