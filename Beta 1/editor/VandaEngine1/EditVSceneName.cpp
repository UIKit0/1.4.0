// EditVSceneName.cpp : implementation file
//

#include "stdafx.h"
#include "VandaEngine1.h"
#include "EditVSceneName.h"
#include "afxdialogex.h"


// CEditVSceneName dialog

IMPLEMENT_DYNAMIC(CEditVSceneName, CDialog)

CEditVSceneName::CEditVSceneName(CWnd* pParent /*=NULL*/)
	: CDialog(CEditVSceneName::IDD, pParent)
{
	m_save = CTrue;
	Cpy(m_name, "\n");
	Cpy(m_selectedName, "\n");
}

CEditVSceneName::~CEditVSceneName()
{
}

void CEditVSceneName::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_PREFAB_NAME, m_editName);
}


BEGIN_MESSAGE_MAP(CEditVSceneName, CDialog)
	ON_BN_CLICKED(IDOK, &CEditVSceneName::OnBnClickedOk)
	ON_EN_CHANGE(IDC_PREFAB_NAME, &CEditVSceneName::OnEnChangePrefabName)
END_MESSAGE_MAP()


// CEditVSceneName message handlers


void CEditVSceneName::OnBnClickedOk()
{
	if (m_strName.IsEmpty())
	{
		MessageBox("Please choose a name", "Vanda Engine Error", MB_OK | MB_ICONERROR);
		return;
	}
	CBool foundTarget = CFalse;
	Cpy(m_name, (LPCTSTR)m_strName);
	Append(m_name, ".vin");

	CChar selectedName[MAX_NAME_SIZE];
	Cpy(selectedName, m_selectedName);
	Append(selectedName, ".vin");
	for (CUInt i = 0; i < g_VSceneNamesOfCurrentProject.size(); i++)
	{
		if (!m_save)
		{
			if (!Cmp(g_VSceneNamesOfCurrentProject[i].c_str(), selectedName))
			{
				if (Cmp(m_name, g_VSceneNamesOfCurrentProject[i].c_str()))
				{
					foundTarget = CTrue;
					break;
				}
			}
		}
		else
		{
			if (Cmp(m_name, g_VSceneNamesOfCurrentProject[i].c_str()))
			{
				foundTarget = CTrue;
					break;
			}
		}
	}
	if (foundTarget)
	{
		if (m_save)
		{
			if (MessageBox("This VScene already exits. Do you want to overwrite it?", "Vanda Engine Report", MB_OK | MB_ICONINFORMATION | MB_YESNO) == IDYES)
				CDialog::OnOK();
			else
				return;
		}
		else
		{
			MessageBox("This VScene name already exits. Please choose another name.", "Vanda Engine Error", MB_OK | MB_ICONERROR);
			return;
		}
	}
	CDialog::OnOK();
}

void CEditVSceneName::SetInitialData(CBool save, CChar* selName)
{
	m_save = save;
	if (selName)
		Cpy(m_selectedName, selName);
}


void CEditVSceneName::OnEnChangePrefabName()
{
	m_editName.GetWindowTextA(m_strName);
}

CChar* CEditVSceneName::GetName()
{
	return m_name;
}

BOOL CEditVSceneName::OnInitDialog()
{
	CDialog::OnInitDialog();
	if (!Cmp(m_selectedName, "\n"))
		m_editName.SetWindowTextA(m_selectedName);
	
	return TRUE;  // return TRUE unless you set the focus to a control
}
