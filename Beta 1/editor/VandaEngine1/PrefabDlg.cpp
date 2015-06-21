// PrefabDlg.cpp : implementation file
//

#include "stdafx.h"
#include "VandaEngine1.h"
#include "PrefabDlg.h"
#include "afxdialogex.h"


// CPrefabDlg dialog

IMPLEMENT_DYNAMIC(CPrefabDlg, CDialog)

CPrefabDlg::CPrefabDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CPrefabDlg::IDD, pParent)
{
	m_packageIndex = -1;
	m_prefabIndex = -1;
}

CPrefabDlg::~CPrefabDlg()
{
	
	for (int nItem = m_listPrefabPackages.GetItemCount() - 1; nItem >= 0; nItem--)
	{
		m_listPrefabPackages.DeleteItem(nItem);
	}

	for (int nItem = m_listPrefabs.GetItemCount() - 1; nItem >= 0; nItem--)
	{
		m_listPrefabs.DeleteItem(nItem);
	}
}

void CPrefabDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST_PREFABS_PROPERTIES, m_listPrefabProperties);
	DDX_Control(pDX, IDC_LIST_PREFABS_PROJECTS, m_listPrefabPackages);
	DDX_Control(pDX, IDC_LIST_PREFABS, m_listPrefabs);
	DDX_Control(pDX, IDC_SLIDER_PREFAB, m_slideLod);
	DDX_Control(pDX, ID_ADD, m_btnAddPrefab);
	DDX_Control(pDX, ID_OPEN, m_btnOpenPrefab);
	DDX_Control(pDX, ID_RENAME, m_btnRenamePrefab);
	DDX_Control(pDX, ID_DELETE, m_btnDeletePrefab);
	DDX_Control(pDX, ID_INSERT, m_btnInsertPrefab);
	DDX_Control(pDX, ID_NEW_PACKAGE, m_btnNewPrefabPackage);
	DDX_Control(pDX, ID_RENAME_PACKAGE, m_btnRenamePrefabPackage);
	DDX_Control(pDX, ID_DELETE_PACKAGE, m_btnDeletePrefabPackage);
}


BEGIN_MESSAGE_MAP(CPrefabDlg, CDialog)
	ON_BN_CLICKED(ID_INSERT, &CPrefabDlg::OnBnClickedInsert)
	ON_BN_CLICKED(ID_OPEN, &CPrefabDlg::OnBnClickedOpen)
	ON_BN_CLICKED(ID_DELETE, &CPrefabDlg::OnBnClickedDelete)
	ON_BN_CLICKED(IDOK, &CPrefabDlg::OnBnClickedOk)
	ON_BN_CLICKED(ID_ADD, &CPrefabDlg::OnBnClickedNewPrefab)
	ON_BN_CLICKED(ID_RENAME, &CPrefabDlg::OnBnClickedRenamePrefab)
	ON_BN_CLICKED(ID_NEW_PACKAGE, &CPrefabDlg::OnBnClickedNewPackage)
	ON_BN_CLICKED(ID_RENAME_PACKAGE, &CPrefabDlg::OnBnClickedRenamePackage)
	ON_WM_HSCROLL()
	ON_BN_CLICKED(ID_DELETE_PACKAGE, &CPrefabDlg::OnBnClickedDeletePackage)
	ON_NOTIFY(LVN_ITEMCHANGED, IDC_LIST_PREFABS_PROJECTS, &CPrefabDlg::OnLvnItemchangedListPrefabsProjects)
	ON_WM_CLOSE()
END_MESSAGE_MAP()


// CPrefabDlg message handlers


void CPrefabDlg::OnBnClickedInsert()
{
	if (g_currentCameraType == eCAMERA_DEFAULT_PHYSX)
	{
		MessageBox("You can not insert prefabs in Play Mode.Please exit from play mode and try again.", "Vanda Engine Error", MB_OK | MB_ICONINFORMATION);
		return;
	}
	g_importPrefab = CTrue;
	if (g_multipleView->m_enableTimer)
		g_multipleView->EnableTimer(CFalse);
	CBool PhysXCam = CFalse;
	ex_pVandaEngine1Dlg->OnMenuClickedInsertPrefab();
	if (g_multipleView->m_enableTimer)
		g_multipleView->EnableTimer(CTrue);
	g_importPrefab = CFalse;
	if (g_editorMode == eMODE_VSCENE)
		g_multipleView->RenderQueries(CTrue);

	g_multipleView->SetElapsedTimeFromBeginning();

}


void CPrefabDlg::OnBnClickedOpen()
{
	ex_pVandaEngine1Dlg->OnBnClickedBtnOpen();
}

void CPrefabDlg::OnBnClickedDelete()
{
	int nSelected = -1;
	POSITION p = m_listPrefabPackages.GetFirstSelectedItemPosition();
	while (p)
	{
		nSelected = m_listPrefabPackages.GetNextSelectedItem(p);
	}
	TCHAR szBuffer1[1024]; //package
	TCHAR szBuffer2[1024]; //prefab

	if (nSelected >= 0)
	{ 
		DWORD cchBuf(1024);
		LVITEM lvi;
		lvi.iItem = nSelected;
		lvi.iSubItem = 0;
		lvi.mask = LVIF_TEXT;
		lvi.pszText = szBuffer1;
		lvi.cchTextMax = cchBuf;
		m_listPrefabPackages.GetItem(&lvi);
		m_selectedPackageName = szBuffer1;
	}
	else
	{
		MessageBox("Please select a package!", "Vanda Engine Error", MB_OK | MB_ICONINFORMATION);
		return;
	}

	nSelected = -1;
	p = m_listPrefabs.GetFirstSelectedItemPosition();
	while (p)
	{
		nSelected = m_listPrefabs.GetNextSelectedItem(p);
	}
	if (nSelected >= 0)
	{
		CInt packageIndex = -1;
		for (CUInt i = 0; i < g_prefabPackagesAndNames.size(); i++)
		{
			if (Cmp(g_prefabPackagesAndNames[i].front().c_str(), m_selectedPackageName.c_str()))
			{
				packageIndex = i;
				break;
			}
		}

		DWORD cchBuf(1024);
		LVITEM lvi;
		lvi.iItem = nSelected;
		lvi.iSubItem = 0;
		lvi.mask = LVIF_TEXT | LVIF_IMAGE;
		lvi.pszText = szBuffer2;
		lvi.cchTextMax = cchBuf;
		m_listPrefabs.GetItem(&lvi);

		m_selectedPrefabName = szBuffer2;

		CChar currentPackageAndPrefabName[MAX_NAME_SIZE];
		sprintf(currentPackageAndPrefabName, "%s%s%s", szBuffer1, "_", szBuffer2);
		if (Cmp(g_currentPackageAndPrefabName, currentPackageAndPrefabName))
		{
			MessageBox("This prefab is already open. Please close current prefab scene and try again.", "Vanda Engine Error", MB_OK | MB_ICONERROR);
			return;
		}

		for (CUInt i = 0; i < g_projects.size(); i++)
		{
			for (CUInt j = 0; j < g_projects[i]->m_sceneNames.size(); j++)
			{
				//find pkg file
				CChar pkgPath[MAX_NAME_SIZE];
				CChar sceneWithoutDot[MAX_NAME_SIZE];
				Cpy(sceneWithoutDot, g_projects[i]->m_sceneNames[j].c_str());
				GetWithoutDot(sceneWithoutDot);
				sprintf(pkgPath, "%s%s%s%s%s", g_projectsPath, g_projects[i]->m_name, "/", sceneWithoutDot, "/packages.pkg");
				//copy package names
				FILE *PackageFilePtr;
				PackageFilePtr = fopen(pkgPath, "rb");
				if (!PackageFilePtr)
				{
					CChar temp[MAX_NAME_SIZE];
					sprintf(temp, "%s%s%s", "Couldn't open the file '", pkgPath, "' to save data");
					MessageBox(temp, "Vanda Engine Error", MB_OK);
					ReleaseCapture();
					PrintInfo("Delete failed.", COLOR_RED);
					return;
				}
				CUInt size = -1;
				fread(&size, sizeof(CUInt), 1, PackageFilePtr);
				for (CUInt k = 0; k < size; k++)
				{
					CChar name[MAX_NAME_SIZE];
					CChar package_name[MAX_NAME_SIZE];
					CChar prefab_name[MAX_NAME_SIZE];
					//write prefab data
					fread(name, sizeof(CChar), MAX_NAME_SIZE, PackageFilePtr);
					fread(package_name, sizeof(CChar), MAX_NAME_SIZE, PackageFilePtr);
					fread(prefab_name, sizeof(CChar), MAX_NAME_SIZE, PackageFilePtr);

					if (Cmp(prefab_name, m_selectedPrefabName.c_str()))
					{
						CChar preafbName[MAX_NAME_SIZE];
						sprintf(preafbName, "This prefab is used by %s/%s.\nYou can not delete this prefab.", g_projects[i]->m_name, g_projects[i]->m_sceneNames[j].c_str());
						MessageBox(preafbName, "Vanda Engine Error", MB_OK | MB_ICONERROR);
						fclose(PackageFilePtr);
						return;
					}
				}
				fclose(PackageFilePtr);
			}
		}

		if (MessageBox("Are you sure you want to delete this prefab?", "Warning", MB_YESNO) == IDYES)
		{
			//remove prefab directory
			CChar PackagePath[MAX_NAME_SIZE];
			HRESULT doc_result_package = SHGetFolderPath(NULL, CSIDL_PERSONAL, NULL, SHGFP_TYPE_CURRENT, PackagePath);
			if (doc_result_package != S_OK)
			{
				PrintInfo("\nCouldn't get the documents folder to write data", COLOR_RED);
				return;
			}
			else
			{
				Append(PackagePath, "/Vanda/Packages/");
			}
			CChar path[MAX_NAME_SIZE];
			sprintf(path, "%s%s%s%s", PackagePath, szBuffer1, "/", szBuffer2);
			RemoveAllFilesAndFoldersInDirectory(path);
			for (CUInt i = 0; i < g_prefabPackagesAndNames[packageIndex].size(); i++)
			{
				if (i == 0) continue;
				if (Cmp(g_prefabPackagesAndNames[packageIndex][i].c_str(), m_selectedPrefabName.c_str()))
				{
					g_prefabPackagesAndNames[packageIndex].erase(g_prefabPackagesAndNames[packageIndex].begin() + i);
				}
			}

			m_prefabIndex = -1;
			//delete all items of prefab list
			for (int nItem = m_listPrefabs.GetItemCount() - 1; nItem >= 0; nItem--)
			{
				m_listPrefabs.DeleteItem(nItem);
			}

			for (CUInt i = 0; i < g_prefabPackagesAndNames[packageIndex].size(); i++)
			{
				if (i == 0) continue;
				CChar str[MAX_NAME_SIZE];
				Cpy(str, g_prefabPackagesAndNames[packageIndex][i].c_str());
				InsertItemToPrefabList(str);
			}
			m_listPrefabs.UpdateWindow();
		}
	}
	else
	{
		MessageBox("Please select a prefab!", "Vanda Engine Error", MB_OK | MB_ICONINFORMATION);
		return;
	}
}

void CPrefabDlg::OnBnClickedOk()
{
	// TODO: Add your control notification handler code here
	CDialog::OnOK();
}


void CPrefabDlg::OnBnClickedNewPrefab()
{
	int nSelected = -1;
	POSITION p = m_listPrefabPackages.GetFirstSelectedItemPosition();
	while (p)
	{
		nSelected = m_listPrefabPackages.GetNextSelectedItem(p);
	}
	if (nSelected >= 0)
	{
		TCHAR szBuffer[1024];
		DWORD cchBuf(1024);
		LVITEM lvi;
		lvi.iItem = nSelected;
		lvi.iSubItem = 0;
		lvi.mask = LVIF_TEXT;
		lvi.pszText = szBuffer;
		lvi.cchTextMax = cchBuf;
		m_listPrefabPackages.GetItem(&lvi);
		m_selectedPackageName = szBuffer;
	}
	else
	{
		MessageBox("Please select a package!", "Vanda Engine Error", MB_OK | MB_ICONINFORMATION);
		return;
	}


	m_prefabNameDlg = CNew(CPrefabNameDlg);
	m_prefabNameDlg->SetInitialData(m_selectedPackageName, "\n", CFalse);
	if (IDOK == m_prefabNameDlg->DoModal())
		InsertItemToPrefabList(m_prefabNameDlg->GetNewName());

	CDelete(m_prefabNameDlg);
}


void CPrefabDlg::OnBnClickedRenamePrefab()
{
	TCHAR szBuffer1[1024]; //package
	TCHAR szBuffer2[1024]; //prefab

	int nSelected = -1;
	POSITION p = m_listPrefabPackages.GetFirstSelectedItemPosition();
	while (p)
	{
		nSelected = m_listPrefabPackages.GetNextSelectedItem(p);
	}
	if (nSelected >= 0)
	{
		DWORD cchBuf(1024);
		LVITEM lvi;
		lvi.iItem = nSelected;
		lvi.iSubItem = 0;
		lvi.mask = LVIF_TEXT;
		lvi.pszText = szBuffer1;
		lvi.cchTextMax = cchBuf;
		m_listPrefabPackages.GetItem(&lvi);
		m_selectedPackageName = szBuffer1;
	}
	else
	{
		MessageBox("Please select a package!", "Vanda Engine Error", MB_OK | MB_ICONINFORMATION);
		return;
	}

	nSelected = -1;
	p = m_listPrefabs.GetFirstSelectedItemPosition();
	while (p)
	{
		nSelected = m_listPrefabs.GetNextSelectedItem(p);
	}
	if (nSelected >= 0)
	{
		DWORD cchBuf(1024);
		LVITEM lvi;
		lvi.iItem = nSelected;
		lvi.iSubItem = 0;
		lvi.mask = LVIF_TEXT | LVIF_IMAGE;
		lvi.pszText = szBuffer2;
		lvi.cchTextMax = cchBuf;
		m_listPrefabs.GetItem(&lvi);

		m_selectedPrefabName = szBuffer2;

		CChar currentPackageAndPrefabName[MAX_NAME_SIZE];
		sprintf(currentPackageAndPrefabName, "%s%s%s", szBuffer1, "_", szBuffer2);
		if (Cmp(g_currentPackageAndPrefabName, currentPackageAndPrefabName))
		{
			MessageBox("This prefab is already open. Please close current prefab scene and try again.", "Vanda Engine Error", MB_OK | MB_ICONERROR);
			return;
		}
		for (CUInt i = 0; i < g_projects.size(); i++)
		{
			for (CUInt j = 0; j < g_projects[i]->m_sceneNames.size(); j++)
			{
				//find pkg file
				CChar pkgPath[MAX_NAME_SIZE];
				CChar sceneWithoutDot[MAX_NAME_SIZE];
				Cpy(sceneWithoutDot, g_projects[i]->m_sceneNames[j].c_str());
				GetWithoutDot(sceneWithoutDot);
				sprintf(pkgPath, "%s%s%s%s%s", g_projectsPath, g_projects[i]->m_name, "/", sceneWithoutDot, "/packages.pkg");
				//copy package names
				FILE *PackageFilePtr;
				PackageFilePtr = fopen(pkgPath, "rb");
				if (!PackageFilePtr)
				{
					CChar temp[MAX_NAME_SIZE];
					sprintf(temp, "%s%s%s", "Couldn't open the file '", pkgPath, "' to save data");
					MessageBox(temp, "Vanda Engine Error", MB_OK);
					ReleaseCapture();
					PrintInfo("Delete failed.", COLOR_RED);
					return;
				}
				CUInt size = -1;
				fread(&size, sizeof(CUInt), 1, PackageFilePtr);
				for (CUInt k = 0; k < size; k++)
				{
					CChar name[MAX_NAME_SIZE];
					CChar package_name[MAX_NAME_SIZE];
					CChar prefab_name[MAX_NAME_SIZE];
					//write prefab data
					fread(name, sizeof(CChar), MAX_NAME_SIZE, PackageFilePtr);
					fread(package_name, sizeof(CChar), MAX_NAME_SIZE, PackageFilePtr);
					fread(prefab_name, sizeof(CChar), MAX_NAME_SIZE, PackageFilePtr);

					if (Cmp(prefab_name, m_selectedPrefabName.c_str()))
					{
						CChar preafbName[MAX_NAME_SIZE];
						sprintf(preafbName, "This prefab is used by %s/%s.\nYou can not delete this prefab.", g_projects[i]->m_name, g_projects[i]->m_sceneNames[j].c_str());
						MessageBox(preafbName, "Vanda Engine Error", MB_OK | MB_ICONERROR);
						fclose(PackageFilePtr);
						return;
					}
				}
				fclose(PackageFilePtr);
			}
		}

	}
	else
	{
		MessageBox("Please select an item!", "Vanda Engine Error", MB_OK | MB_ICONINFORMATION);
		return;
	}

	m_prefabNameDlg = CNew(CPrefabNameDlg);
	m_prefabNameDlg->SetInitialData(m_selectedPackageName, m_selectedPrefabName, CFalse, CTrue);
	if (IDOK == m_prefabNameDlg->DoModal())
	{
		CChar PackagePath[MAX_NAME_SIZE];
		HRESULT doc_result_package = SHGetFolderPath(NULL, CSIDL_PERSONAL, NULL, SHGFP_TYPE_CURRENT, PackagePath);
		if (doc_result_package != S_OK)
		{
			PrintInfo("\nCouldn't get the documents folder to write data", COLOR_RED);
			return;
		}
		else
		{
			Append(PackagePath, "/Vanda/Packages/");
		}
		CChar old_path[MAX_NAME_SIZE];
		CChar new_path[MAX_NAME_SIZE];
		sprintf(old_path, "%s%s%s%s", PackagePath, szBuffer1, "/", szBuffer2);
		sprintf(new_path, "%s%s%s%s", PackagePath, szBuffer1, "/", m_prefabNameDlg->GetNewName());
		rename(old_path, new_path);

		//rename vpf files
		CBool foundTarget = CFalse;
		for (CUInt i = 0; i < g_prefabPackagesAndNames.size(); i++)
		{
			if (Cmp(g_prefabPackagesAndNames[i].front().c_str(), szBuffer1))
			{
				for (CUInt j = 0; j < g_prefabPackagesAndNames[i].size(); j++)
				{
					if (j == 0) continue;
					if (Cmp(g_prefabPackagesAndNames[i][j].c_str(), m_prefabNameDlg->GetNewName()))
					{
						CChar vpfOldPath[MAX_NAME_SIZE];
						sprintf(vpfOldPath, "%s%s%s%s%s%s%s%s%s", PackagePath, g_prefabPackagesAndNames[i].front().c_str(), "/", g_prefabPackagesAndNames[i][j].c_str(), "/", g_prefabPackagesAndNames[i].front().c_str(), "_", szBuffer2, ".vpf");
						CChar vpfNewPath[MAX_NAME_SIZE];
						sprintf(vpfNewPath, "%s%s%s%s%s%s%s%s%s", PackagePath, g_prefabPackagesAndNames[i].front().c_str(), "/", g_prefabPackagesAndNames[i][j].c_str(), "/", g_prefabPackagesAndNames[i].front().c_str(), "_", g_prefabPackagesAndNames[i][j].c_str(), ".vpf");
						rename(vpfOldPath, vpfNewPath);

						foundTarget = CTrue;
						break;
					}
				}
			}
			if (foundTarget)
				break;
		}
		m_listPrefabs.SetItemText(nSelected, 0, m_prefabNameDlg->GetNewName());
	}
	CDelete(m_prefabNameDlg);
}

void CPrefabDlg::OnBnClickedNewPackage()
{
	m_prefabNameDlg = CNew(CPrefabNameDlg);
	m_prefabNameDlg->SetInitialData("\n", "\n", CTrue);
	if( IDOK == m_prefabNameDlg->DoModal() )
		InsertItemToPackageList(m_prefabNameDlg->GetNewName());
	CDelete(m_prefabNameDlg);
}

void CPrefabDlg::OnBnClickedRenamePackage()
{
	int nSelected = -1;
	TCHAR szBuffer[1024];

	POSITION p = m_listPrefabPackages.GetFirstSelectedItemPosition();
	while (p)
	{
		nSelected = m_listPrefabPackages.GetNextSelectedItem(p);
	}
	if (nSelected >= 0)
	{
		DWORD cchBuf(1024);
		LVITEM lvi;
		lvi.iItem = nSelected;
		lvi.iSubItem = 0;
		lvi.mask = LVIF_TEXT;
		lvi.pszText = szBuffer;
		lvi.cchTextMax = cchBuf;
		m_listPrefabPackages.GetItem(&lvi);
		m_selectedPackageName = szBuffer;
	}
	else
	{
		MessageBox("Please select an item!", "Vanda Engine Error", MB_OK | MB_ICONINFORMATION);
		return;
	}
	CChar currentPackageName[MAX_NAME_SIZE];
	sprintf(currentPackageName, "%s", szBuffer);
	if (Cmp(g_currentPackageName, currentPackageName))
	{
		MessageBox("current open prefab belongs to selected package. Please close current prefab scene and try again.", "Vanda Engine Error", MB_OK | MB_ICONERROR);
		return;
	}

	//then fill it with the VScenes of the selected project
	for (CUInt i = 0; i < g_projects.size(); i++)
	{
		for (CUInt j = 0; j < g_projects[i]->m_sceneNames.size(); j++)
		{
			//find pkg file
			CChar pkgPath[MAX_NAME_SIZE];
			CChar sceneWithoutDot[MAX_NAME_SIZE];
			Cpy(sceneWithoutDot, g_projects[i]->m_sceneNames[j].c_str());
			GetWithoutDot(sceneWithoutDot);
			sprintf(pkgPath, "%s%s%s%s%s", g_projectsPath, g_projects[i]->m_name, "/", sceneWithoutDot, "/packages.pkg");
			//copy package names
			FILE *PackageFilePtr;
			PackageFilePtr = fopen(pkgPath, "rb");
			if (!PackageFilePtr)
			{
				CChar temp[MAX_NAME_SIZE];
				sprintf(temp, "%s%s%s", "Couldn't open the file '", pkgPath, "' to save data");
				MessageBox(temp, "Vanda Engine Error", MB_OK);
				ReleaseCapture();
				PrintInfo("Rename failed.", COLOR_RED);
				return;
			}
			CUInt size = -1;
			fread(&size, sizeof(CUInt), 1, PackageFilePtr);
			for (CUInt k = 0; k < size; k++)
			{
				CChar name[MAX_NAME_SIZE];
				CChar package_name[MAX_NAME_SIZE];
				CChar prefab_name[MAX_NAME_SIZE];
				//write prefab data
				fread(name, sizeof(CChar), MAX_NAME_SIZE, PackageFilePtr);
				fread(package_name, sizeof(CChar), MAX_NAME_SIZE, PackageFilePtr);
				fread(prefab_name, sizeof(CChar), MAX_NAME_SIZE, PackageFilePtr);

				if (Cmp(package_name, m_selectedPackageName.c_str()))
				{
					CChar packageName[MAX_NAME_SIZE];
					sprintf(packageName, "This package is used by %s/%s.\nYou can not rename this package.", g_projects[i]->m_name, g_projects[i]->m_sceneNames[j].c_str());
					MessageBox(packageName, "Vanda Engine Error", MB_OK | MB_ICONERROR);
					fclose(PackageFilePtr);
					return;
				}
			}
			fclose(PackageFilePtr);
		}
	}

	m_prefabNameDlg = CNew(CPrefabNameDlg);
	m_prefabNameDlg->SetInitialData(m_selectedPackageName, "\n", CTrue, CTrue);
	if (IDOK == m_prefabNameDlg->DoModal())
	{
		m_listPrefabPackages.SetItemText(nSelected, 0, m_prefabNameDlg->GetNewName());

		CChar PackagePath[MAX_NAME_SIZE];
		HRESULT doc_result_package = SHGetFolderPath(NULL, CSIDL_PERSONAL, NULL, SHGFP_TYPE_CURRENT, PackagePath);
		if (doc_result_package != S_OK)
		{
			PrintInfo("\nCouldn't get the documents folder to write data", COLOR_RED);
			return;
		}
		else
		{
			Append(PackagePath, "/Vanda/Packages/");
		}
		CChar old_path[MAX_NAME_SIZE];
		CChar new_path[MAX_NAME_SIZE];
		sprintf(old_path, "%s%s", PackagePath, szBuffer);
		sprintf(new_path, "%s%s", PackagePath, m_prefabNameDlg->GetNewName());
		rename(old_path, new_path);

		//rename vpf files
		for (CUInt i = 0; i < g_prefabPackagesAndNames.size(); i++)
		{
			if (Cmp(g_prefabPackagesAndNames[i].front().c_str(), m_prefabNameDlg->GetNewName()))
			{
				for (CUInt j = 0; j < g_prefabPackagesAndNames[i].size(); j++)
				{
					if (j == 0)continue;
					CChar vpfOldPath[MAX_NAME_SIZE];
					sprintf(vpfOldPath, "%s%s%s%s%s%s%s%s%s", PackagePath, g_prefabPackagesAndNames[i].front().c_str(), "/", g_prefabPackagesAndNames[i][j].c_str(), "/", szBuffer, "_", g_prefabPackagesAndNames[i][j].c_str(), ".vpf");
					CChar vpfNewPath[MAX_NAME_SIZE];
					sprintf(vpfNewPath, "%s%s%s%s%s%s%s%s%s", PackagePath, g_prefabPackagesAndNames[i].front().c_str(), "/", g_prefabPackagesAndNames[i][j].c_str(), "/", g_prefabPackagesAndNames[i].front().c_str(), "_", g_prefabPackagesAndNames[i][j].c_str(), ".vpf");
					rename(vpfOldPath, vpfNewPath);
				}
				break;
			}
		}

	}
	CDelete(m_prefabNameDlg);
}

void CPrefabDlg::OnBnClickedDeletePackage()
{
	int nSelected = -1;
	POSITION p = m_listPrefabPackages.GetFirstSelectedItemPosition();
	while (p)
	{
		nSelected = m_listPrefabPackages.GetNextSelectedItem(p);
	}
	if (nSelected >= 0)
	{
		for (CUInt i = 0; i < g_projects.size(); i++)
		{
			for (CUInt j = 0; j < g_projects[i]->m_sceneNames.size(); j++)
			{
				//find pkg file
				CChar pkgPath[MAX_NAME_SIZE];
				CChar sceneWithoutDot[MAX_NAME_SIZE];
				Cpy(sceneWithoutDot, g_projects[i]->m_sceneNames[j].c_str());
				GetWithoutDot(sceneWithoutDot);
				sprintf(pkgPath, "%s%s%s%s%s", g_projectsPath, g_projects[i]->m_name, "/", sceneWithoutDot, "/packages.pkg");
				//copy package names
				FILE *PackageFilePtr;
				PackageFilePtr = fopen(pkgPath, "rb");
				if (!PackageFilePtr)
				{
					CChar temp[MAX_NAME_SIZE];
					sprintf(temp, "%s%s%s", "Couldn't open the file '", pkgPath, "' to save data");
					MessageBox(temp, "Vanda Engine Error", MB_OK);
					ReleaseCapture();
					PrintInfo("Delete failed.", COLOR_RED);
					return;
				}
				CUInt size = -1;
				fread(&size, sizeof(CUInt), 1, PackageFilePtr);
				for (CUInt k = 0; k < size; k++)
				{
					CChar name[MAX_NAME_SIZE];
					CChar package_name[MAX_NAME_SIZE];
					CChar prefab_name[MAX_NAME_SIZE];
					//write prefab data
					fread(name, sizeof(CChar), MAX_NAME_SIZE, PackageFilePtr);
					fread(package_name, sizeof(CChar), MAX_NAME_SIZE, PackageFilePtr);
					fread(prefab_name, sizeof(CChar), MAX_NAME_SIZE, PackageFilePtr);

					if (Cmp(package_name, m_selectedPackageName.c_str()))
					{
						CChar packageName[MAX_NAME_SIZE];
						sprintf(packageName, "This package is used by %s/%s.\nYou can not delete this package.", g_projects[i]->m_name, g_projects[i]->m_sceneNames[j].c_str());
						MessageBox(packageName, "Vanda Engine Error", MB_OK | MB_ICONERROR);
						fclose(PackageFilePtr);
						return;
					}
				}
				fclose(PackageFilePtr);
			}
		}

		TCHAR szBuffer[1024];
		DWORD cchBuf(1024);
		LVITEM lvi;
		lvi.iItem = nSelected;
		lvi.iSubItem = 0;
		lvi.mask = LVIF_TEXT;
		lvi.pszText = szBuffer;
		lvi.cchTextMax = cchBuf;
		m_listPrefabPackages.GetItem(&lvi);

		m_selectedPackageName = szBuffer;

		CChar currentPackageName[MAX_NAME_SIZE];
		sprintf(currentPackageName, "%s", szBuffer);
		if (Cmp(g_currentPackageName, currentPackageName))
		{
			MessageBox("current open prefab belongs to selected package. Please close current prefab scene and try again.", "Vanda Engine Error", MB_OK | MB_ICONERROR);
			return;
		}

		if (MessageBox("Are you sure you want to delete this package?", "Warning", MB_YESNO) == IDYES)
		{
			CChar PackagePath[MAX_NAME_SIZE];
			HRESULT doc_result_package = SHGetFolderPath(NULL, CSIDL_PERSONAL, NULL, SHGFP_TYPE_CURRENT, PackagePath);
			if (doc_result_package != S_OK)
			{
				PrintInfo("\nCouldn't get the documents folder to write data", COLOR_RED);
				return;
			}
			else
			{
				Append(PackagePath, "/Vanda/Packages/");
			}
			CChar path[MAX_NAME_SIZE];
			sprintf(path, "%s%s", PackagePath, szBuffer);

			RemoveAllFilesAndFoldersInDirectory(path);
			//delete items from std::vector
			for (CUInt i = 0; i < g_prefabPackagesAndNames.size(); i++)
			{
				if (Cmp(g_prefabPackagesAndNames[i].front().c_str(), m_selectedPackageName.c_str()))
				{
					g_prefabPackagesAndNames[i].clear();
					g_prefabPackagesAndNames.erase(g_prefabPackagesAndNames.begin() + i);
					break;
				}
			}

			m_listPrefabPackages.DeleteItem(nSelected);
			m_listPrefabPackages.UpdateWindow();
			m_packageIndex--;
		}
	}
	else
	{
		MessageBox("Please select an item!", "Vanda Engine Error", MB_OK | MB_ICONINFORMATION);
		return;
	}
}

BOOL CPrefabDlg::OnInitDialog()
{
	CDialog::OnInitDialog();
	ModifyStyle(WS_SYSMENU, 0);

	if (g_editorMode == eMODE_PREFAB)
	{
		m_btnAddPrefab.ShowWindow(SW_SHOW);
		m_btnOpenPrefab.ShowWindow(SW_SHOW);
		m_btnDeletePrefab.ShowWindow(SW_SHOW);
		m_btnRenamePrefab.ShowWindow(SW_SHOW);

		m_btnNewPrefabPackage.ShowWindow(SW_SHOW);
		m_btnRenamePrefabPackage.ShowWindow(SW_SHOW);
		m_btnDeletePrefabPackage.ShowWindow(SW_SHOW);

		m_btnInsertPrefab.ShowWindow(SW_HIDE);
	}
	else
	{
		m_btnAddPrefab.ShowWindow(SW_HIDE);
		m_btnOpenPrefab.ShowWindow(SW_HIDE);
		m_btnDeletePrefab.ShowWindow(SW_HIDE);
		m_btnRenamePrefab.ShowWindow(SW_HIDE);

		m_btnNewPrefabPackage.ShowWindow(SW_HIDE);
		m_btnRenamePrefabPackage.ShowWindow(SW_HIDE);
		m_btnDeletePrefabPackage.ShowWindow(SW_HIDE);

		m_btnInsertPrefab.ShowWindow(SW_SHOW);

	}
	m_slideLod.SetRange(0, 2);
	CBitmap cBmp;
	CBitmap* cBmpMask = NULL;
	m_packageListImage.Create(36, 36, ILC_COLOR24, 1, 1);

	cBmp.LoadBitmap(IDB_BITMAP_DEFAULT_PACKAGE);
	m_packageListImage.Add(&cBmp, cBmpMask);
	cBmp.DeleteObject();

	RECT tempRect;
	m_listPrefabPackages.GetClientRect(&tempRect);
	m_listPrefabPackages.SetImageList(&m_packageListImage, LVSIL_SMALL);
	m_listPrefabPackages.InsertColumn(0, "Packages", LVCFMT_CENTER | LVS_SHOWSELALWAYS, (tempRect.right - tempRect.left) * 80 / 100);
	m_listPrefabPackages.ShowWindow(SW_SHOW);
	m_listPrefabPackages.UpdateWindow();

	m_prefabListImage.Create(80, 80, ILC_COLOR24, 1, 1);

	cBmp.LoadBitmap(IDB_BITMAP_DEFAULT_VPF);
	m_prefabListImage.Add(&cBmp, cBmpMask);
	cBmp.DeleteObject();

	m_listPrefabs.SetImageList(&m_prefabListImage, LVSIL_NORMAL);

	m_listPrefabs.GetClientRect(&tempRect);
	m_listPrefabs.InsertColumn(0, "Prefabs", LVCFMT_LEFT | LVS_SHOWSELALWAYS, (tempRect.right - tempRect.left) * 80 / 100);
	m_listPrefabs.ShowWindow(SW_SHOW);
	m_listPrefabs.UpdateWindow();

	//insert items
	for (CUInt i = 0; i < g_prefabPackagesAndNames.size(); i++)
	{
		CChar str[MAX_NAME_SIZE];
		Cpy(str, g_prefabPackagesAndNames[i].front().c_str());
		InsertItemToPackageList(str);
	}
	RECT rect;
	m_listPrefabProperties.GetClientRect(&rect);
	m_listPrefabProperties.InsertColumn(0, "Parameters", LVCFMT_LEFT, rect.right - rect.left, 1);
	m_listPrefabProperties.SetExtendedStyle(LVS_EX_INFOTIP | LVS_EX_ONECLICKACTIVATE | LVS_EX_LABELTIP);
	InserItemToPropertiesList("Vertices");
	InserItemToPropertiesList("Textures");

	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}

void CPrefabDlg::OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar)
{
	// TODO: Add your message handler code here and/or call default
	CInt nSliderPos = m_slideLod.GetPos();
	switch (nSliderPos)
	{
	case 0:
		break;
	case 1:
		break;
	case 2:
		break;
	}

	CDialog::OnHScroll(nSBCode, nPos, pScrollBar);
}

CVoid CPrefabDlg::InsertItemToPackageList(CChar* packageName)
{
	m_packageIndex++;
	int index = m_packageIndex;
	LVITEM lvItem;
	lvItem.mask = LVIF_TEXT | LVIF_IMAGE;
	lvItem.iImage = 0;
	lvItem.iItem = index;
	lvItem.iSubItem = 0;
	lvItem.pszText = packageName;
	m_listPrefabPackages.InsertItem(&lvItem);

	m_listPrefabPackages.SetExtendedStyle(LVS_EX_INFOTIP | LVS_EX_ONECLICKACTIVATE);
	m_listPrefabPackages.SetItemState(index, LVIS_SELECTED, LVIS_SELECTED | LVIS_FOCUSED);
	m_listPrefabPackages.SetSelectionMark(index);
	m_listPrefabPackages.EnsureVisible(index, FALSE);
	m_listPrefabPackages.UpdateWindow();
}

CVoid CPrefabDlg::InsertItemToPrefabList(CChar* prefabName)
{
	m_prefabIndex++;
	int index = m_prefabIndex;
	LVITEM lvItem;
	lvItem.mask = LVIF_TEXT | LVIF_IMAGE;
	lvItem.iImage = 0;
	lvItem.iItem = index;
	lvItem.iSubItem = 0;
	lvItem.pszText = prefabName;
	m_listPrefabs.InsertItem(&lvItem);

	m_listPrefabs.SetExtendedStyle(LVS_EX_INFOTIP | LVS_EX_ONECLICKACTIVATE);
	m_listPrefabs.SetItemState(index, LVIS_SELECTED, LVIS_SELECTED | LVIS_FOCUSED);
	m_listPrefabs.SetSelectionMark(index);
	m_listPrefabs.EnsureVisible(index, FALSE);
	m_listPrefabs.UpdateWindow();
}

void CPrefabDlg::OnLvnItemchangedListPrefabsProjects(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMLISTVIEW pNMLV = reinterpret_cast<LPNMLISTVIEW>(pNMHDR);
	//remove the current contents of the prefab list
	for (int nItem = m_listPrefabs.GetItemCount() - 1; nItem >= 0; nItem--)
	{
		m_listPrefabs.DeleteItem(nItem);
	}
	m_listPrefabs.UpdateWindow();
	m_prefabIndex = -1; //reset prefab index

	int nSelected = -1;
	POSITION p = m_listPrefabPackages.GetFirstSelectedItemPosition();
	while (p)
	{
		nSelected = m_listPrefabPackages.GetNextSelectedItem(p);
	}
	if (nSelected >= 0)
	{
		TCHAR szBuffer[1024];
		DWORD cchBuf(1024);
		LVITEM lvi;
		lvi.iItem = nSelected;
		lvi.iSubItem = 0;
		lvi.mask = LVIF_TEXT;
		lvi.pszText = szBuffer;
		lvi.cchTextMax = cchBuf;
		m_listPrefabPackages.GetItem(&lvi);
		m_selectedPackageName = szBuffer;
	}
	else
	{
		return;
	}
	CInt index = -1;
	for (CInt i = 0; i < (CInt)g_prefabPackagesAndNames.size(); i++)
	{
		if (Cmp(g_prefabPackagesAndNames[i].front().c_str(), m_selectedPackageName.c_str()))
		{
			index = i;
			break;
		}
	}
	if (index != -1)
	{
		for (CUInt i = 0; i < g_prefabPackagesAndNames[index].size(); i++)
		{
			if (i == 0) continue;
			CChar str[MAX_NAME_SIZE];
			Cpy(str, g_prefabPackagesAndNames[index][i].c_str());
			InsertItemToPrefabList(str);
		}
	}
	*pResult = 0;
}


BOOL CPrefabDlg::PreTranslateMessage(MSG* pMsg)
{
	// TODO: Add your specialized code here and/or call the base class
	if (pMsg->wParam == VK_ESCAPE)
	{
		OnBnClickedOk();
		return TRUE;
	}
	return CDialog::PreTranslateMessage(pMsg);
}


CVoid CPrefabDlg::InserItemToPropertiesList(char * properties)
{
	static CInt index = 0;
	LVITEM lvItem;
	lvItem.mask = LVIF_TEXT;
	lvItem.iItem = index;
	lvItem.iSubItem = 0;
	lvItem.pszText = properties;
	m_listPrefabProperties.InsertItem(&lvItem);
	m_listPrefabProperties.SetItemState(index, LVIS_SELECTED, LVIS_SELECTED | LVIS_FOCUSED);
	m_listPrefabProperties.EnsureVisible(0, FALSE);
	index++;
}
