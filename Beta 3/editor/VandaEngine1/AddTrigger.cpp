// AddTrigger.cpp : implementation file
//

#include "stdafx.h"
#include "VandaEngine1.h"
#include "AddTrigger.h"
#include "afxdialogex.h"


// CAddTrigger dialog

IMPLEMENT_DYNAMIC(CAddTrigger, CDialog)

CAddTrigger::CAddTrigger(CWnd* pParent /*=NULL*/)
	: CDialog(CAddTrigger::IDD, pParent)
{
	m_triggerType = eTRIGGER_BOX;
	m_trigger = NULL;
}

CAddTrigger::~CAddTrigger()
{
}

void CAddTrigger::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_COMB_TRIGGER, m_comboTrigger);
	DDX_Control(pDX, IDC_EDIT_TRIGGER, m_editBoxTrigger);
}


BEGIN_MESSAGE_MAP(CAddTrigger, CDialog)
	ON_BN_CLICKED(IDOK, &CAddTrigger::OnBnClickedOk)
	ON_CBN_SELCHANGE(IDC_COMB_TRIGGER, &CAddTrigger::OnCbnSelchangeCombTrigger)
	ON_EN_CHANGE(IDC_EDIT_TRIGGER, &CAddTrigger::OnEnChangeEditTrigger)
END_MESSAGE_MAP()


// CAddTrigger message handlers


void CAddTrigger::OnBnClickedOk()
{
	if (m_strTriggerName.IsEmpty())
	{
		MessageBox("Please select a name for new trigger", "Vanda Engine Error", MB_OK | MB_ICONINFORMATION);
		return;
	}
	CChar name[MAX_NAME_SIZE];
	Cpy(name, (LPCTSTR)m_strTriggerName);

	CChar oldName[MAX_NAME_SIZE];
	Cpy(oldName, "\n");
	if (m_trigger)
		Cpy(oldName, m_trigger->GetName());
	for (std::vector<std::string>::iterator it = g_engineObjectNames.begin(); it != g_engineObjectNames.end(); it++)
	{
		if (!Cmp(oldName, (*it).c_str()) && Cmp((LPCSTR)name, (*it).c_str()))
		{
			MessageBox("This name already exists. Please select another name!", "Vanda Engine Error", MB_OK | MB_ICONERROR);
			return;
		}
	}
	CTrigger* new_trigger = NULL;
	CVec3f translation;
	CVec4f rotation;
	CVec3f scaling;
	CChar m_enterScript[MAX_NAME_SIZE];
	CChar m_exitScript[MAX_NAME_SIZE];

	if (m_trigger)
	{
		new_trigger = m_trigger;
		translation = m_trigger->GetInstancePrefab()->GetTranslate();
		rotation = m_trigger->GetInstancePrefab()->GetRotate();
		scaling = m_trigger->GetInstancePrefab()->GetScale();
		Cpy(m_enterScript, m_trigger->GetInstancePrefab()->GetScene(0)->m_instanceGeometries[0]->m_enterScript);
		Cpy(m_exitScript, m_trigger->GetInstancePrefab()->GetScene(0)->m_instanceGeometries[0]->m_exitScript);

	}
	else
		new_trigger = CNew(CTrigger);
	new_trigger->SetName(name);

	CChar packageName[MAX_NAME_SIZE];
	CChar prefabName[MAX_NAME_SIZE];

	Cpy(packageName, "Vanda_Basics");
	
	if (m_triggerType == eTRIGGER_BOX)
	{
		Cpy(prefabName, "Box_Trigger");
		new_trigger->SetTriggerType(eTRIGGER_BOX);
	}
	else
	{
		Cpy(prefabName, "Sphere_Trigger");
		new_trigger->SetTriggerType(eTRIGGER_SPHERE);
	}

	g_importPrefab = CTrue;
	if (g_multipleView->m_enableTimer)
		g_multipleView->EnableTimer(CFalse);
	CBool PhysXCam = CFalse;
	CBool selected = CFalse;
	if (m_trigger)
	{
		g_arrowPosition = translation;
		g_arrowRotate.x = rotation.x; g_arrowRotate.y = rotation.y; g_arrowRotate.z = rotation.z;
		g_arrowScale = scaling;
		if (g_selectedName == m_trigger->GetInstancePrefab()->GetNameIndex())
			selected = CTrue;
		m_trigger->Destroy();

	}
	else
	{
		g_arrowPosition.x = g_arrowPosition.y = g_arrowPosition.z = 0.0f;
		g_arrowRotate.x = g_arrowRotate.y = g_arrowRotate.z = 0.0f;
		g_arrowScale.x = g_arrowScale.y = g_arrowScale.z = 1.0f;
	}
	ex_pVandaEngine1Dlg->m_dlgPrefabs = CNew(CPrefabDlg);
	ex_pDlgPrefabs = ex_pVandaEngine1Dlg->m_dlgPrefabs;
	ex_pVandaEngine1Dlg->OnMenuClickedInsertPrefab(NULL, packageName, prefabName);
	new_trigger->SetInstancePrefab(g_instancePrefab[g_instancePrefab.size() - 1]); //last element
	new_trigger->GetInstancePrefab()->SetIsTrigger(CTrue);
	CDelete(ex_pVandaEngine1Dlg->m_dlgPrefabs);
	if (g_multipleView->m_enableTimer)
		g_multipleView->EnableTimer(CTrue);
	g_importPrefab = CFalse;
	if (g_editorMode == eMODE_VSCENE)
		g_multipleView->RenderQueries(CTrue);

	if (selected)
	{
		g_selectedName = new_trigger->GetInstancePrefab()->GetNameIndex();
	}
	if (m_trigger)
	{
		new_trigger->GetInstancePrefab()->SetTranslate(translation);
		new_trigger->GetInstancePrefab()->SetRotate(rotation);
		new_trigger->GetInstancePrefab()->SetScale(scaling);
		new_trigger->GetInstancePrefab()->UpdateBoundingBox(CTrue);
		new_trigger->GetInstancePrefab()->CalculateDistance();

		CScene* scene = new_trigger->GetInstancePrefab()->GetScene(0);
		for (CUInt i = 0; i < scene->m_instanceGeometries.size(); i++)
		{
			Cpy(scene->m_instanceGeometries[i]->m_enterScript, m_enterScript);
			Cpy(scene->m_instanceGeometries[i]->m_exitScript, m_exitScript);

			if (!Cmp(scene->m_instanceGeometries[i]->m_enterScript, "\n"))
				scene->m_instanceGeometries[i]->m_hasEnterScript = CTrue;
			else
				scene->m_instanceGeometries[i]->m_hasEnterScript = CFalse;

			if (!Cmp(scene->m_instanceGeometries[i]->m_exitScript, "\n"))
				scene->m_instanceGeometries[i]->m_hasExitScript = CTrue;
			else
				scene->m_instanceGeometries[i]->m_hasExitScript = CFalse;

		}
		new_trigger->GetInstancePrefab()->SetEnterScript(m_enterScript);
		new_trigger->GetInstancePrefab()->SetExitScript(m_exitScript);
	}
	else
	{
		g_triggers.push_back(new_trigger);
		ex_pVandaEngine1Dlg->InserItemToEngineObjectList(new_trigger->GetName(), eENGINEOBJECTLIST_TRIGGER);
		g_engineObjectNames.push_back(new_trigger->GetName());
	}
	g_multipleView->SetElapsedTimeFromBeginning();

	CDialog::OnOK();
}


BOOL CAddTrigger::OnInitDialog()
{
	CDialog::OnInitDialog();

	m_comboTrigger.InsertString(0, "Box");
	m_comboTrigger.InsertString(1, "Sphere");
	if (m_trigger)
	{
		m_triggerType = m_trigger->GetTriggerType();
		m_comboTrigger.SetCurSel(m_triggerType);
	}
	else
		m_comboTrigger.SetCurSel(0);

	m_comboTrigger.UpdateWindow();

	if (m_trigger)
	{
		m_editBoxTrigger.SetWindowTextA(m_trigger->GetName());
	}

	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}


void CAddTrigger::OnCbnSelchangeCombTrigger()
{
	CInt curSel = m_comboTrigger.GetCurSel();
	switch (curSel)
	{
	case eTRIGGER_BOX: //box
		m_triggerType = eTRIGGER_BOX;
		break;
	case eTRIGGER_SPHERE: //sphere
		m_triggerType = eTRIGGER_SPHERE;
		break;
	default:
		break;
	}
}

void CAddTrigger::OnEnChangeEditTrigger()
{
	m_editBoxTrigger.GetWindowTextA(m_strTriggerName);
}

CVoid CAddTrigger::Init(CTrigger* trigger)
{
	m_trigger = trigger;
}
