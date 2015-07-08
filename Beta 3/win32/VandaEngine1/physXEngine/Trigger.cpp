//Copyright (C) 2015 Zehne Ziba Co., Ltd.
//This file is licensed and distributed under MIT license

#include "stdafx.h"
#include "Trigger.h"
#include "../GraphicsEngine/Scene.h"
#include "../Main.h"
CTrigger::CTrigger()
{
}

CTrigger::~CTrigger()
{
	Destroy();
}

CVoid CTrigger::Destroy()
{
	CBool foundTarget = CFalse;
	for (CUInt i = 0; i < g_instancePrefab.size(); i++)
	{
		if (Cmp(g_instancePrefab[i]->GetName(), m_instancePrefab->GetName()))
		{
			foundTarget = CTrue;
			break;
		}
	}
	if (!foundTarget)
		return;

	CPrefab* dstPrefab = NULL;
	dstPrefab = m_instancePrefab->GetPrefab();
	//remove instance from prefab
	for (CUInt j = 0; j < dstPrefab->GetNumInstances(); j++)
	{
		CBool foundTarget = CFalse;
		if (Cmp(dstPrefab->GetInstance(j)->GetName(), m_instancePrefab->GetName()))
		{
			dstPrefab->RemoveInstance(j);
			foundTarget = CTrue;
		}
		if (foundTarget)
			break;
	}
	for (CUInt k = 0; k < 3; k++)
	{
		if (m_instancePrefab->GetScene(k))
		{
			CScene* scene = m_instancePrefab->GetScene(k);
			g_main->RemoveSelectedScene(scene->GetName(), scene->GetDocURI());

			//CScene* scene = m_instancePrefab->GetScene(k);
			//for (CUInt i = 0; i < g_scene.size(); i++)
			//{
			//	CBool condition = CFalse;
			//	condition = Cmp(g_scene[i]->m_pureFileName, scene->GetName()) && Cmp(g_scene[i]->GetDocURI(), scene->GetDocURI());
			//	if (condition)
			//	{
			//		//delete the scene
			//		CDelete(g_scene[i]);
			//		//delete the vector that holds the scene
			//		g_scene.erase(g_scene.begin() + i);
			//		break;
			//	}
			//}
		}
	}

	for (CUInt i = 0; i < g_instancePrefab.size(); i++)
	{
		if (Cmp(g_instancePrefab[i]->GetName(), m_instancePrefab->GetName()))
		{
			g_instancePrefab.erase(g_instancePrefab.begin() + i);
			break;
		}
	}
	CDelete(m_instancePrefab);

	if (dstPrefab->GetNumInstances() == 0)
	{
		//now remove the prefab
		for (CUInt k = 0; k < g_prefab.size(); k++)
		{
			if (Cmp(dstPrefab->GetName(), g_prefab[k]->GetName()))
			{
				CDelete(g_prefab[k]);
				g_prefab.erase(g_prefab.begin() + k);
				break;
			}
		}
	}


}

CVoid CTrigger::SetName(CChar* name)
{
	Cpy(m_name, name);
}

CChar* CTrigger::GetName()
{
	return m_name;
}

CVoid CTrigger::SetTriggerType(CTriggerType triggerType)
{
	m_triggerType = triggerType;
}

CVoid CTrigger::SetInstancePrefab(CInstancePrefab* instancePrefab)
{
	m_instancePrefab = instancePrefab;
}

CTriggerType CTrigger::GetTriggerType()
{
	return m_triggerType;
}

CInstancePrefab* CTrigger::GetInstancePrefab()
{
	return m_instancePrefab;
}

