//Copyright (C) 2015 Zehne Ziba Co., Ltd.
//This file is licensed and distributed under MIT license

// VandaEngine1Dlg.cpp : implementation file
//

#include "stdafx.h"
#include "VandaEngine1.h"
#include "VandaEngine1Dlg.h"
#include "GraphicsEngine/Animation.h"
//#include "graphicsEngine\\imagelib.h"
#include "physxengine\\nx.h"
#include <windows.h>
#include <shlobj.h>
#include "Common/Prefab.h"
#pragma comment(lib, "shfolder.lib")

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

//Edition.MaxVersion.MinVersion.BugFixes;
//RTI.1.2.0
CInt g_maxVersion = 1;
CInt g_minVersion = 2;
CInt g_bugFixesVersion = 0;
CChar g_edition[MAX_NAME_SIZE] = "RTIBasic";
CBool g_useOriginalPathOfDAETextures = CFalse;
CBool g_updateTextureViaEditor = CFalse;
CChar g_currentVSceneName[MAX_NAME_SIZE]; //save functions
std::vector<std::string> g_VSceneNamesOfCurrentProject; //save functions
std::vector<std::string> g_VSceneNamesOfCurrentProjectToBePublished; //save functions
std::vector<std::string> g_allPrefabNames; //save functions
std::string g_firstSceneNameToBeLoaded; //save functions
CChar g_projectsPath[MAX_NAME_SIZE];
CChar g_currentProjectPath[MAX_NAME_SIZE];
CScene* g_currentScene = NULL;
CInstancePrefab* g_currentInstancePrefab = NULL;
CChar g_currentPackageAndPrefabName[MAX_NAME_SIZE]; //save functions
CChar g_currentPackageName[MAX_NAME_SIZE]; //save functions
CChar g_currentPrefabName[MAX_NAME_SIZE]; //save functions
CChar g_currentPackagePath[MAX_NAME_SIZE];
CChar g_currentPrefabAndSceneName[MAX_NAME_SIZE]; //save functions
CChar g_currentInstancePrefabName[MAX_NAME_SIZE]; //save functions
CTFormType g_currentTransformType = eCTranslate;
CBool g_shareGeometriesBetweenScenes = CTrue;
CBool g_openNewPrefabFromList = CFalse;
CChar g_currentPassword[MAX_NAME_SIZE];
CRichEditCtrl *ex_pRichEdit = NULL; // extern
CRichEditCtrl *ex_pRichEditScript = NULL; //extern
CVandaEngine1Dlg* ex_pVandaEngine1Dlg = NULL;
CPrefabDlg* ex_pDlgPrefabs = NULL;
CRichEditCtrl* ex_pStaticSelectedObject = NULL;

CButton* ex_pBtnPrevAnim = NULL;
CButton* ex_pBtnPlayAnim = NULL;
CButton* ex_pBtnPauseAnim = NULL;
CButton* ex_pBtnNextAnim = NULL;
CButton* ex_pBtnTimerPlay = NULL;
CButton* ex_pBtnTimerPause = NULL;
CButton* ex_pBtnTestModeActive = NULL;
CButton* ex_pBtnTestModeDeactive = NULL;
CButton* ex_pBtnMaterialEditor = NULL;
CButton* ex_pBtnPhysXEditor = NULL;
CButton* ex_pBtnScriptEditor = NULL;
CMenu *ex_pMenu = NULL; //extern

CEditMaterial *ex_pEditMaterial = NULL; //extern
CEditPhysX *ex_pEditPhysX = NULL; //extern
CAddScript* ex_pAddScript = NULL;

CBool g_vandaDemo = CFalse;
CBool g_importCOLLADA = CFalse;
CBool g_openVINFile = CFalse;
CChar g_fileNameInCommandLine[MAX_NAME_SIZE];
std::vector<COpenALSoundBuffer*>g_soundBuffers;
CRender g_render; //extern
std::vector<CScene*> g_scene;
std::vector<CPrefab*> g_prefab;
std::vector<CGeometry *> g_geometries;
std::vector<CInstancePrefab*> g_instancePrefab;
CBool g_importPrefab = CFalse;
CScene* g_translateArrow = NULL;
CScene* g_scaleArrow = NULL;
CScene* g_rotateArrow = NULL;
CScene* g_negativeZArrow = NULL;
CVec3f g_arrowPosition;
CVec3f g_arrowRotate;
CVec3f g_arrowScale;
CBool g_showArrow = CFalse;
CScene* g_centerArrowScene = NULL;
COpenGLUtility g_glUtil;
CInt numErrors, numWarnings, totalErrors, totalWarnings;
CImage* g_soundImg = NULL;
CImage* g_pointLightImg = NULL;
CImage* g_directionalLightImg = NULL;
CImage* g_spotLightImg = NULL;
CImage* g_pointLightCOLLADAImg = NULL;
CImage* g_directionalLightCOLLADAImg = NULL;
CImage* g_spotLightCOLLADAImg = NULL;
CImage* g_skyImg = NULL;
CImage* g_waterImg = NULL;
CImage* g_cameraImg = NULL;
CVec4f g_defaultDirectionalLight;
CColor4f g_globalAmbientColor;
CBool g_useGlobalAmbientColor = CFalse;
std::vector<CInstanceLight*> g_engineLights;
std::vector<CTrigger*> g_triggers;
std::vector<CWater*> g_engineWaters;
std::vector<CInstanceCamera*> g_cameraInstances;
std::vector<CImage*> g_images;
std::vector<CImage*>g_waterImages; // This variable holds the information of water images of Engine
std::vector<CStaticSound*>g_engineStaticSounds;
COpenALSystem* g_soundSystem = NULL;
std::vector<std::string> g_engineObjectNames;
CShadowMap * g_dynamicShadowMap = NULL;
COctree* g_octree;
CMultipleWindows *g_multipleView;
CBool g_updateOctree;
CFloat g_elapsedTime;
GLuint g_shaderType;
CUpdateCamera *g_camera;
CSkyDome *g_skyDome = NULL;
NxExtendedVec3 g_characterPos;
CVec3f g_cameraPitchYawTilt;
CVec3f g_cameraInstancePos; //default free camera
CVec2f g_cameraInstancePanTilt; //default free camera
CBool g_useOldRenderingStyle = CFalse;
CMenuVariables g_menu;
CShadowProperties g_shadowProperties;
CPhysXProperties g_physXProperties;
CDOFProperties g_dofProperties;
CFogProperties g_fogProperties;
CBloomProperties g_bloomProperties;
CLightProperties g_lightProperties;
CPathProperties g_pathProperties;
CExtraTexturesNamingConventions g_extraTexturesNamingConventions;
COptions g_options;
std::vector<CProjects*> g_projects;
CPolygonMode g_polygonMode = ePOLYGON_FILL;
CBool g_firstPass;
CInstanceLight* g_currentInstanceLight = NULL;
CMaterialChannels g_materialChannels = eCHANNELS_ALL;
CInt g_sceneManagerObjectsPerSplit = 15;
CLuaState g_lua;
CBool g_testScript = CFalse;
CBool g_renderShadow = CFalse;

CEditorMode g_editorMode = eMODE_VSCENE;

CBool g_clickedOpen = CFalse;
CBool g_clickedNew = CFalse;

CCameraType g_currentCameraType = eCAMERA_DEFAULT_FREE;
std::vector<std::vector<std::string>> g_prefabPackagesAndNames;
CBloom* g_bloom = NULL;
CExternalPhysX*  g_externalPhysX = NULL;
CSceneBanner g_sceneBanner;
CInt g_width;
CInt g_height;
// CAboutDlg dialog used for App About

DWORD __stdcall MEditStreamOutCallback(DWORD dwCookie, LPBYTE pbBuff, LONG cb, LONG *pcb)
{
	CString sThisWrite;
	sThisWrite.GetBufferSetLength(cb);

	CString *psBuffer = (CString *)dwCookie;
	
	for (int i=0;i<cb;i++) {
		sThisWrite.SetAt(i,*(pbBuff+i));
	}

	*psBuffer += sThisWrite;

	*pcb = sThisWrite.GetLength();
	sThisWrite.ReleaseBuffer();
	return 0;
}

DWORD __stdcall MEditStreamInCallback(DWORD dwCookie, LPBYTE pbBuff, LONG cb, LONG *pcb)
{
	CString *psBuffer = (CString *)dwCookie;

	if (cb < psBuffer->GetLength()) cb = psBuffer->GetLength();

	for (int i=0;i<cb;i++)
	{
		*(pbBuff+i) = psBuffer->GetAt(i);
	}

	*pcb = cb;

	*psBuffer = psBuffer->Mid(cb);

	return 0;
}

CVoid SetDialogData3(CBool selected, CInstancePrefab* instancePrefab)
{
	//selected object
	COLORREF color = COLOR_WHITE;
	CHARFORMAT cf;
	cf.dwMask = CFM_COLOR/* | CFM_SIZE*/;
	cf.dwEffects = NULL;
	cf.crTextColor = color;

	ex_pStaticSelectedObject->SetWindowTextA("\n");
	CInt nSel = ex_pStaticSelectedObject->GetWindowTextLength();
	ex_pStaticSelectedObject->SetSel(nSel, nSel);

	ex_pStaticSelectedObject->SetSelectionCharFormat(cf);
	if (selected)
	{
		ex_pStaticSelectedObject->ReplaceSel(instancePrefab->GetName());
		g_showArrow = CTrue;

		if (g_currentTransformType == eCTranslate)
		{
			CFloat val;
			CChar temp1[MAX_NAME_SIZE];
			val = roundf(instancePrefab->GetTranslate().x * 100) / 100;
			sprintf(temp1, "%.02f", val);
			ex_pVandaEngine1Dlg->m_editX.SetWindowTextA(temp1);

			CChar temp2[MAX_NAME_SIZE];
			val = roundf(instancePrefab->GetTranslate().y * 100) / 100;
			sprintf(temp2, "%.02f", val);
			ex_pVandaEngine1Dlg->m_editY.SetWindowTextA(temp2);

			CChar temp3[MAX_NAME_SIZE];
			val = roundf(instancePrefab->GetTranslate().z * 100) / 100;
			sprintf(temp3, "%.02f", val);
			ex_pVandaEngine1Dlg->m_editZ.SetWindowTextA(temp3);
		}
		else if (g_currentTransformType == eCRotate)
		{
			CFloat val;
			CChar temp1[MAX_NAME_SIZE];
			val = roundf(instancePrefab->GetRotate().x * 100) / 100;
			sprintf(temp1, "%.02f", val);
			ex_pVandaEngine1Dlg->m_editX.SetWindowTextA(temp1);

			CChar temp2[MAX_NAME_SIZE];
			val = roundf(instancePrefab->GetRotate().y * 100) / 100;
			sprintf(temp2, "%.02f", val);
			ex_pVandaEngine1Dlg->m_editY.SetWindowTextA(temp2);

			CChar temp3[MAX_NAME_SIZE];
			val = roundf(instancePrefab->GetRotate().z * 100) / 100;
			sprintf(temp3, "%.02f", val);
			ex_pVandaEngine1Dlg->m_editZ.SetWindowTextA(temp3);
		}
		else if (g_currentTransformType == eCScale)
		{
			CFloat val;
			CChar temp1[MAX_NAME_SIZE];
			val = roundf(instancePrefab->GetScale().x * 100) / 100;
			sprintf(temp1, "%.02f", val);
			ex_pVandaEngine1Dlg->m_editX.SetWindowTextA(temp1);

			CChar temp2[MAX_NAME_SIZE];
			val = roundf(instancePrefab->GetScale().y * 100) / 100;
			sprintf(temp2, "%.02f", val);
			ex_pVandaEngine1Dlg->m_editY.SetWindowTextA(temp2);

			CChar temp3[MAX_NAME_SIZE];
			val = roundf(instancePrefab->GetScale().z * 100) / 100;
			sprintf(temp3, "%.02f", val);
			ex_pVandaEngine1Dlg->m_editZ.SetWindowTextA(temp3);
		}
	}
	else
	{
		ex_pStaticSelectedObject->ReplaceSel("Nothing Selected");
		ex_pVandaEngine1Dlg->m_editX.SetWindowTextA("\n");
		ex_pVandaEngine1Dlg->m_editY.SetWindowTextA("\n");
		ex_pVandaEngine1Dlg->m_editZ.SetWindowTextA("\n");

		g_showArrow = CFalse;
	}

}

CVoid SetDialogData2( CBool selected, CPolyGroup* group, CBool engineObject, CBool showDialog )
{
	if (g_editorMode == eMODE_VSCENE) return;

	if( engineObject )
		g_showArrow = CTrue;
	else
		g_showArrow = CFalse;

	if( selected )
	{
		//selected object
		COLORREF color = COLOR_WHITE;
		CHARFORMAT cf;
		cf.dwMask = CFM_COLOR/* | CFM_SIZE*/;
		cf.dwEffects = NULL;
		cf.crTextColor = color;

		ex_pStaticSelectedObject->SetWindowTextA( "\n" );
		CInt nSel = ex_pStaticSelectedObject->GetWindowTextLength();
		ex_pStaticSelectedObject->SetSel(nSel, nSel);

		ex_pStaticSelectedObject->SetSelectionCharFormat(cf);
		ex_pStaticSelectedObject->ReplaceSel( group->m_materialName );

		///////

		if( g_render.GetSelectedScene()->m_hasAnimation )
		{
			g_render.GetSelectedScene()->SetCurrentClipIndex();

			if( g_render.GetSelectedScene()->m_animationStatus == eANIM_PLAY )
			{
				ex_pBtnPlayAnim->EnableWindow( FALSE );
				ex_pBtnPlayAnim->ShowWindow( SW_HIDE );
				ex_pBtnPauseAnim->EnableWindow( TRUE );
				ex_pBtnPauseAnim->ShowWindow( SW_SHOW );
			}
			else
			{
				ex_pBtnPlayAnim->EnableWindow( TRUE );
				ex_pBtnPlayAnim->ShowWindow( SW_SHOW );
				ex_pBtnPauseAnim->EnableWindow( FALSE );
				ex_pBtnPauseAnim->ShowWindow( SW_HIDE );
			}
		}
		else
		{
			ex_pBtnPlayAnim->EnableWindow( FALSE );
			ex_pBtnPlayAnim->ShowWindow( SW_HIDE );
			ex_pBtnPauseAnim->EnableWindow( FALSE );
			ex_pBtnPauseAnim->ShowWindow( SW_SHOW );
			ex_pBtnPrevAnim->EnableWindow( FALSE );
			ex_pBtnNextAnim->EnableWindow( FALSE );
		}

		if( showDialog )
		{
			//material editor
			ex_pBtnMaterialEditor->EnableWindow( TRUE );
			ex_pMenu->EnableMenuItem( ID_TOOLS_MATERIALEDITOR, MF_ENABLED );

			ex_pBtnPhysXEditor->EnableWindow( FALSE );
			ex_pMenu->EnableMenuItem( ID_TOOLS_PHYSXEDITOR, MF_DISABLED );

			ex_pEditMaterial->SetNormalBtnState( CTrue );
			ex_pEditMaterial->SetDirtBtnState( CTrue );
			ex_pEditMaterial->SetDiffuseBtnState( CTrue );
			ex_pEditMaterial->SetGlossBtnState( CTrue );

			ex_pEditMaterial->SetEditBoxNormalMapName( GetAfterPath(group->m_strNormalMap) );
			ex_pEditMaterial->SetEditBoxDirtMapName( GetAfterPath(group->m_strDirtMap) );
			ex_pEditMaterial->SetEditBoxDiffuseName( GetAfterPath(group->m_strDiffuse) );
			ex_pEditMaterial->SetEditBoxGlossMapName( GetAfterPath(group->m_strGlossMap) );

			ex_pEditMaterial->SetEditBoxBias( group->m_parallaxMapBias );
			ex_pEditMaterial->SetEditBoxScale( group->m_parallaxMapScale );

			if( Cmp( group->m_strDirtMap, "\n" ) == CFalse )
				ex_pEditMaterial->SetRemoveDirtBtnState( CTrue );
			else
				ex_pEditMaterial->SetRemoveDirtBtnState( CFalse );

			if( Cmp( group->m_strNormalMap, "\n" ) == CFalse )
				ex_pEditMaterial->SetRemoveNormalBtnState( CTrue );
			else
				ex_pEditMaterial->SetRemoveNormalBtnState( CFalse );

			if( Cmp( group->m_strDiffuse, "\n" ) == CFalse )
				ex_pEditMaterial->SetRemoveDiffuseBtnState( CTrue );
			else
				ex_pEditMaterial->SetRemoveDiffuseBtnState( CFalse );

			if( Cmp( group->m_strGlossMap, "\n" ) == CFalse )
				ex_pEditMaterial->SetRemoveGlossBtnState( CTrue );
			else
				ex_pEditMaterial->SetRemoveGlossBtnState( CFalse );

		} //if showDialog
	}
	else
	{
		//material editor

		ex_pBtnMaterialEditor->EnableWindow( FALSE );
		ex_pMenu->EnableMenuItem( ID_TOOLS_MATERIALEDITOR,  MF_DISABLED | MF_GRAYED );

		ex_pBtnPhysXEditor->EnableWindow( FALSE );
		ex_pMenu->EnableMenuItem( ID_TOOLS_PHYSXEDITOR,  MF_DISABLED | MF_GRAYED );

		//ex_pBtnScriptEditor->EnableWindow( FALSE );
		//ex_pMenu->EnableMenuItem( ID_TOOLS_SCRIPTMANAGER, MF_DISABLED | MF_GRAYED );

		ex_pEditMaterial->SetNormalBtnState( CFalse );
		ex_pEditMaterial->SetDirtBtnState( CFalse );
		ex_pEditMaterial->SetDiffuseBtnState( CFalse );
		ex_pEditMaterial->SetGlossBtnState( CFalse );

		ex_pEditMaterial->SetEditBoxNormalMapName( "\n" );
		ex_pEditMaterial->SetEditBoxDirtMapName( "\n" );
		ex_pEditMaterial->SetEditBoxDiffuseName( "\n" );
		ex_pEditMaterial->SetEditBoxGlossMapName( "\n" );

		ex_pEditMaterial->SetRemoveDirtBtnState( CFalse );
		ex_pEditMaterial->SetRemoveNormalBtnState( CFalse );
		ex_pEditMaterial->SetRemoveDiffuseBtnState( CFalse );
		ex_pEditMaterial->SetRemoveGlossBtnState( CFalse );
		/////

		ex_pBtnPlayAnim->EnableWindow( FALSE );
		ex_pBtnPlayAnim->ShowWindow( SW_HIDE );
		ex_pBtnPauseAnim->EnableWindow( FALSE );
		ex_pBtnPauseAnim->ShowWindow( SW_SHOW );
		ex_pBtnPrevAnim->EnableWindow( FALSE );
		ex_pBtnNextAnim->EnableWindow( FALSE );

		ex_pStaticSelectedObject->SetWindowTextA( "Nothing Selected" );
	}
}

CVoid SetDialogData( CBool selected, CInstanceGeometry* instanceGeo, CGeometry* geo, CBool engineObject, CBool showDialog )
{
	if (g_editorMode == eMODE_VSCENE) return;
	if( engineObject )
		g_showArrow = CTrue;
	else
		g_showArrow = CFalse;

	if( selected )
	{
		//set cull face check mark of the main dialog's menu
		if( geo->m_cullFaces )
			CheckCullFace( eCULLFACETYPEFORMENU_CHECKED );
		else
			CheckCullFace( eCULLFACETYPEFORMENU_UNCHECKED );
	}
	else
		CheckCullFace( eCULLFACETYPEFORMENU_GRAYED );

	if( selected )
	{
		//selected object
		COLORREF color = COLOR_WHITE;
		CHARFORMAT cf;
		cf.dwMask = CFM_COLOR;
		cf.dwEffects = NULL;
		cf.crTextColor = color;

		ex_pStaticSelectedObject->SetWindowTextA( "\n" );
		CInt nSel = ex_pStaticSelectedObject->GetWindowTextLength();
		ex_pStaticSelectedObject->SetSel(nSel, nSel);

		ex_pStaticSelectedObject->SetSelectionCharFormat(cf);
		ex_pStaticSelectedObject->ReplaceSel( geo->GetName() );

		////////
		if( g_render.GetSelectedScene()->m_hasAnimation )
		{
			g_render.GetSelectedScene()->SetCurrentClipIndex();
			if( g_render.GetSelectedScene()->m_animationStatus == eANIM_PLAY )
			{
				ex_pBtnPlayAnim->EnableWindow( FALSE );
				ex_pBtnPlayAnim->ShowWindow( SW_HIDE );
				ex_pBtnPauseAnim->EnableWindow( TRUE );
				ex_pBtnPauseAnim->ShowWindow( SW_SHOW );
			}
			else
			{
				ex_pBtnPlayAnim->EnableWindow( TRUE );
				ex_pBtnPlayAnim->ShowWindow( SW_SHOW );
				ex_pBtnPauseAnim->EnableWindow( FALSE );
				ex_pBtnPauseAnim->ShowWindow( SW_HIDE );
			}
		}
		else
		{
			ex_pBtnPlayAnim->EnableWindow( FALSE );
			ex_pBtnPlayAnim->ShowWindow( SW_HIDE );
			ex_pBtnPauseAnim->EnableWindow( FALSE );
			ex_pBtnPauseAnim->ShowWindow( SW_SHOW );
			ex_pBtnPrevAnim->EnableWindow( FALSE );
			ex_pBtnNextAnim->EnableWindow( FALSE );
		}

		if( showDialog )
		{
			//material editor
			ex_pBtnMaterialEditor->EnableWindow( TRUE );
			ex_pMenu->EnableMenuItem( ID_TOOLS_MATERIALEDITOR, MF_ENABLED );

			ex_pBtnPhysXEditor->EnableWindow( TRUE );
			ex_pMenu->EnableMenuItem( ID_TOOLS_PHYSXEDITOR, MF_ENABLED );

			//if( instanceGeo->m_isTrigger )
			//{
			//	ex_pBtnScriptEditor->EnableWindow( TRUE );
			//	ex_pMenu->EnableMenuItem( ID_TOOLS_SCRIPTMANAGER, MF_ENABLED );
			//	ex_pAddScript->SetInstanceGeo( instanceGeo );
			//}
			//else
			//{
			//	ex_pBtnScriptEditor->EnableWindow( FALSE );
			//	ex_pMenu->EnableMenuItem( ID_TOOLS_SCRIPTMANAGER, MF_DISABLED | MF_GRAYED );
			//}

			ex_pEditMaterial->SetNormalBtnState( CTrue );
			ex_pEditMaterial->SetDirtBtnState( CTrue );
			ex_pEditMaterial->SetDiffuseBtnState( CTrue );
			ex_pEditMaterial->SetGlossBtnState( CTrue );

			ex_pEditMaterial->SetEditBoxNormalMapName( GetAfterPath(geo->m_strNormalMap) );
			ex_pEditMaterial->SetEditBoxDirtMapName( GetAfterPath(geo->m_strDirtMap) );
			ex_pEditMaterial->SetEditBoxDiffuseName( GetAfterPath(geo->m_strDiffuse) );
			ex_pEditMaterial->SetEditBoxGlossMapName( GetAfterPath(geo->m_strGlossMap) );

			ex_pEditMaterial->SetEditBoxBias( geo->m_parallaxMapBias );
			ex_pEditMaterial->SetEditBoxScale( geo->m_parallaxMapScale );
			ex_pEditPhysX->SetPhysX( instanceGeo );

			if( Cmp( geo->m_strDirtMap, "\n" ) == CFalse )
				ex_pEditMaterial->SetRemoveDirtBtnState( CTrue );
			else
				ex_pEditMaterial->SetRemoveDirtBtnState( CFalse );

			if( Cmp( geo->m_strNormalMap, "\n" ) == CFalse )
				ex_pEditMaterial->SetRemoveNormalBtnState( CTrue );
			else
				ex_pEditMaterial->SetRemoveNormalBtnState( CFalse );

			if( Cmp( geo->m_strDiffuse, "\n" ) == CFalse )
				ex_pEditMaterial->SetRemoveDiffuseBtnState( CTrue );
			else
				ex_pEditMaterial->SetRemoveDiffuseBtnState( CFalse );

			if( Cmp( geo->m_strGlossMap, "\n" ) == CFalse )
				ex_pEditMaterial->SetRemoveGlossBtnState( CTrue );
			else
				ex_pEditMaterial->SetRemoveGlossBtnState( CFalse );

		} //if showDialog
	}
	else
	{
		//material editor
		ex_pBtnMaterialEditor->EnableWindow( FALSE );
		ex_pMenu->EnableMenuItem( ID_TOOLS_MATERIALEDITOR, MF_DISABLED | MF_GRAYED );

		ex_pBtnPhysXEditor->EnableWindow( FALSE );
		ex_pMenu->EnableMenuItem( ID_TOOLS_PHYSXEDITOR, MF_DISABLED | MF_GRAYED );

		//ex_pBtnScriptEditor->EnableWindow( FALSE );
		//ex_pMenu->EnableMenuItem( ID_TOOLS_SCRIPTMANAGER, MF_DISABLED | MF_GRAYED );

		ex_pEditMaterial->SetNormalBtnState( CFalse );
		ex_pEditMaterial->SetDirtBtnState( CFalse );
		ex_pEditMaterial->SetDiffuseBtnState( CFalse );
		ex_pEditMaterial->SetGlossBtnState( CFalse );

		ex_pEditMaterial->SetEditBoxNormalMapName( "\n" );
		ex_pEditMaterial->SetEditBoxDirtMapName( "\n" );
		ex_pEditMaterial->SetEditBoxDiffuseName( "\n" );
		ex_pEditMaterial->SetEditBoxGlossMapName( "\n" );

		ex_pEditMaterial->SetRemoveDirtBtnState( CFalse );
		ex_pEditMaterial->SetRemoveNormalBtnState( CFalse );
		ex_pEditMaterial->SetRemoveDiffuseBtnState( CFalse );
		ex_pEditMaterial->SetRemoveGlossBtnState( CFalse );

		ex_pEditPhysX->SetPhysX( NULL );

		///////////////////////////

		ex_pBtnPlayAnim->EnableWindow( FALSE );
		ex_pBtnPlayAnim->ShowWindow( SW_HIDE );
		ex_pBtnPauseAnim->EnableWindow( FALSE );
		ex_pBtnPauseAnim->ShowWindow( SW_SHOW );
		ex_pBtnPrevAnim->EnableWindow( FALSE );
		ex_pBtnNextAnim->EnableWindow( FALSE );

		ex_pStaticSelectedObject->SetWindowTextA( "Nothing Selected" );
	}
}

CVoid CheckCullFace( CCullFaceTypeForMenu type )
{
	if( type == eCULLFACETYPEFORMENU_CHECKED )
	{
		ex_pMenu->EnableMenuItem( ID_TOOLS_CULLFACES, MF_ENABLED );
		ex_pMenu->CheckMenuItem( ID_TOOLS_CULLFACES, MF_CHECKED );
	}
	else if( type == eCULLFACETYPEFORMENU_UNCHECKED )
	{
		ex_pMenu->EnableMenuItem( ID_TOOLS_CULLFACES, MF_ENABLED );
		ex_pMenu->CheckMenuItem( ID_TOOLS_CULLFACES, MF_UNCHECKED );
	}
	else if( type == eCULLFACETYPEFORMENU_GRAYED )
	{
		ex_pMenu->CheckMenuItem( ID_TOOLS_CULLFACES, MF_UNCHECKED );
		ex_pMenu->EnableMenuItem( ID_TOOLS_CULLFACES, MF_GRAYED );
	}
}


CVoid PrintShaderLog( CString strText )
{
	CHARFORMAT2 cf;
	cf.dwMask = CFM_COLOR/* | CFM_SIZE*/;
	cf.dwEffects = NULL;
	cf.crTextColor = COLOR_RED;

	CInt nSel = ex_pRichEdit->GetWindowTextLength();
	ex_pRichEdit->SetSel(nSel, nSel);

	ex_pRichEdit->SetSelectionCharFormat(cf);
	ex_pRichEdit->ReplaceSel(strText.GetString());
	ex_pRichEdit->UpdateData(TRUE);
	ex_pRichEdit->UpdateWindow();
}

CVoid PrintInfo( CString strText, COLORREF color, CBool resetCounter )
{
	if( g_reportInfo )
	{
		static CInt reportCounter = 1;
		if( resetCounter )
			reportCounter = 0;
		CChar tempName[MAX_NAME_SIZE];
		Cpy( tempName, strText.GetString() );
		CChar* str = tempName;
		CChar str2 [ MAX_NAME_SIZE ];


		if( CmpIn( str, "\n" ) )
		{
			reportCounter++;
			str += sizeof( CChar); //skip first endline
			sprintf(str2, "\n%i> %s", reportCounter, str);
		}
		else
		{
			sprintf(str2, "%s", str);
		}

		CHARFORMAT2 cf;
		cf.dwMask = CFM_COLOR | CFM_FACE;
		cf.dwEffects = NULL;
		cf.crTextColor = color;
		Cpy(cf.szFaceName,"Consolas");

		CInt nSel = ex_pRichEdit->GetWindowTextLength();
		ex_pRichEdit->SetSel(nSel, nSel);

		ex_pRichEdit->SetSelectionCharFormat(cf);
		ex_pRichEdit->ReplaceSel(str2);
		ex_pRichEdit->UpdateData(TRUE);
		ex_pRichEdit->UpdateWindow();
	}
}



CVoid PrintScriptInfo( CString strText, COLORREF color )
{
	//CRect rect;
	//ex_pRichEdit->GetWindowRect(&rect);
	static CInt reportCounter = 1;
	CChar temp[MAX_NAME_SIZE];
	if( reportCounter != 1 )
		sprintf( temp, "\n%i> %s", reportCounter, strText.GetString() );
	else
		sprintf( temp, "%i> %s", reportCounter, strText.GetString() );

	CHARFORMAT2 cf;
	cf.dwMask = CFM_COLOR | CFM_FACE;
	cf.dwEffects = NULL;
	cf.crTextColor = color;
	Cpy(cf.szFaceName,"Consolas");
	//cf.yHeight = CInt( rect.bottom - rect.top ) * 10.5 ;

	CInt nSel = ex_pRichEditScript->GetWindowTextLength();
	ex_pRichEditScript->SetSel(nSel, nSel);

	ex_pRichEditScript->SetSelectionCharFormat(cf);
	ex_pRichEditScript->ReplaceSel(temp);
	ex_pRichEditScript->UpdateData(TRUE);
	ex_pRichEditScript->UpdateWindow();

	reportCounter++;
}

CGeometry *GetGeometryFromScenes(const CChar * name, const CChar * DocURI)
{
	for (CUInt i = 0; i<g_geometries.size(); i++)
	{
		if (ICmp(g_geometries[i]->GetName(), name) &&
			ICmp(g_geometries[i]->GetDocURI(), DocURI))
		{
			CChar temp[MAX_NAME_SIZE];
			sprintf(temp, "%s%s%s%s", "\nGeometry '", g_geometries[i]->GetName(), g_geometries[i]->GetDocURI(), "' already exists.");
			PrintInfo(temp, COLOR_YELLOW);
			return g_geometries[i];
		}
	}
	return NULL;
}

CPrefab* GetPrefab(CChar* name)
{
	if (name == NULL) return NULL;
	for (CUInt i = 0; i < g_prefab.size(); i++)
	{
		if (Cmp(g_prefab[i]->GetName(), name))
			return g_prefab[i];
	}
	return NULL;
}

// Definition: relative pixel = 1 pixel at 96 DPI and scaled based on actual DPI.
class CDPI
{
public:
    CDPI() : _fInitialized(false), _dpiX(96), _dpiY(96), _resX(1024), _resY(768) { }
    
    // Get screen DPI.
    int GetDPIX() { _Init(); return _dpiX; }
    int GetDPIY() { _Init(); return _dpiY; }

	int GetResX() { _Init(); return _resX; }
	int GetResY() { _Init(); return _resY; }

    // Convert between raw pixels and relative pixels.
    int ScaleX(int x) { _Init(); return MulDiv(x, _dpiX, 96); }
    int ScaleY(int y) { _Init(); return MulDiv(y, _dpiY, 96); }
    int UnscaleX(int x) { _Init(); return MulDiv(x, 96, _dpiX); }
    int UnscaleY(int y) { _Init(); return MulDiv(y, 96, _dpiY); }

    // Determine the screen dimensions in relative pixels.
    int ScaledScreenWidth() { return _ScaledSystemMetricX(SM_CXSCREEN); }
    int ScaledScreenHeight() { return _ScaledSystemMetricY(SM_CYSCREEN); }

    // Scale rectangle from raw pixels to relative pixels.
    void ScaleRect(__inout RECT *pRect)
    {
        pRect->left = ScaleX(pRect->left);
        pRect->right = ScaleX(pRect->right);
        pRect->top = ScaleY(pRect->top);
        pRect->bottom = ScaleY(pRect->bottom);
    }
    // Determine if screen resolution meets minimum requirements in relative
    // pixels.
    bool IsResolutionAtLeast(int cxMin, int cyMin) 
    { 
        return (ScaledScreenWidth() >= cxMin) && (ScaledScreenHeight() >= cyMin); 
    }

    // Convert a point size (1/72 of an inch) to raw pixels.
    int PointsToPixels(int pt) { _Init(); return MulDiv(pt, _dpiY, 72); }

    // Invalidate any cached metrics.
    void Invalidate() { _fInitialized = false; }

private:
    void _Init()
    {
        if (!_fInitialized)
        {
            HDC hdc = GetDC(NULL);
            if (hdc)
            {
                _dpiX = GetDeviceCaps(hdc, HORZSIZE );
                _dpiY = GetDeviceCaps(hdc, LOGPIXELSY);

				_resX = GetDeviceCaps(hdc, HORZRES );
				_resY = GetDeviceCaps(hdc, VERTRES );
                ReleaseDC(NULL, hdc);
            }
            _fInitialized = true;
        }
    }

    int _ScaledSystemMetricX(int nIndex) 
    { 
        _Init(); 
        return MulDiv(GetSystemMetrics(nIndex), 96, _dpiX); 
    }

    int _ScaledSystemMetricY(int nIndex) 
    { 
        _Init(); 
        return MulDiv(GetSystemMetrics(nIndex), 96, _dpiY); 
    }
private:
    bool _fInitialized;

    int _dpiX;
    int _dpiY;

	int _resX;
	int _resY;
};

class CAboutDlg : public CDialog
{
public:
	CAboutDlg();

// Dialog Data
	enum { IDD = IDD_ABOUTBOX };

	protected:
	virtual CVoid DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

// Implementation
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
{
}

CVoid CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
END_MESSAGE_MAP()


// CVandaEngine1Dlg dialog


CVandaEngine1Dlg::CVandaEngine1Dlg(CWnd* pParent /*=NULL*/)
	: CDialog(CVandaEngine1Dlg::IDD, pParent)
{

	SetCapture();
	SetCursor( LoadCursorFromFile( "Assets/Engine/Icons/progress.ani") );

	m_pToolTip = NULL;
	m_hIcon = AfxGetApp()->LoadIcon(IDI_VANDAENGINE);

	ex_pRichEdit = &m_rich;

	ex_pStaticSelectedObject = &m_staticSelectedObject;

	//Initialize material editor
	ex_pEditMaterial = m_dlgEditMaterial = CNew( CEditMaterial );
	ex_pEditPhysX = m_dlgEditPhysX = CNew( CEditPhysX );
	ex_pAddScript = m_dlgAddScript = CNew (CAddScript);

	ex_pBtnPrevAnim = &m_mainBtnPrevAnim;
	ex_pBtnNextAnim = &m_mainBtnNextAnim;
	ex_pBtnPlayAnim = &m_mainBtnPlayAnim;
	ex_pBtnPauseAnim = &m_mainBtnPauseAnim;
	ex_pBtnTestModeActive = &m_mainBtnTestActive;
	ex_pBtnTestModeDeactive = &m_mainBtnTestDeactive;
	ex_pBtnMaterialEditor = &m_mainBtnMaterial;
	ex_pBtnPhysXEditor = &m_mainBtnPhysXEditor;
	ex_pBtnScriptEditor = &m_mainBtnScriptManager;
	ex_pBtnTimerPlay = &m_mainBtnTimer;
	ex_pBtnTimerPause = &m_mainBtnTimerPause;
	m_savePathName = "\n";
	Cpy(m_strNewPackageName, "\n");
	Cpy(m_strNewPrefabName, "\n");
	Cpy(m_strNewPackageAndPrefabName, "\n");
	Cpy(m_strNewPassword, "\n");
	m_horizontalSizeOfLeftCulomn = 0;
	m_horizontalSizeOfRightCulomn = 0;

	m_engineObjectListIndex = -1;
	m_sceneListIndex = -1;
	m_physXElementListIndex = -1;

	g_useGlobalAmbientColor = CFalse;
	g_globalAmbientColor.r = g_globalAmbientColor.g = g_globalAmbientColor.b = 0.5f;
	g_globalAmbientColor.a = 1.0f;
	m_askRemoveScene = CTrue;
}

CVandaEngine1Dlg::~CVandaEngine1Dlg()
{
	g_VSceneNamesOfCurrentProject.clear();
	g_VSceneNamesOfCurrentProjectToBePublished.clear();
	g_allPrefabNames.clear();
	for (CUInt i = 0; i < g_prefabPackagesAndNames.size(); i++)
	{
		for (CUInt j = 0; j < g_prefabPackagesAndNames[i].size(); j++)
		{
			g_prefabPackagesAndNames[i].clear();
		}
	}
	g_prefabPackagesAndNames.clear();

	for (CUInt i = 0; i < g_triggers.size(); i++)
	{
		CDelete(g_triggers[i]);
	}
	g_triggers.clear();

	//clear prefabs
	for (CUInt i = 0; i < g_prefab.size(); i++)
	{
		CDelete(g_prefab[i]);
	}
	g_prefab.clear();

	for (CUInt i = 0; i < g_instancePrefab.size(); i++)
	{
		CDelete(g_instancePrefab[i]);
	}
	g_instancePrefab.clear();

	if( g_multipleView->m_nx->m_hasScene )
	{
		CDelete(g_externalPhysX);
	}
	//delete material editor
	CDelete( m_dlgEditMaterial );
	CDelete( m_dlgEditPhysX );
	CDelete( m_dlgAddScript );
	CDelete( m_pToolTip );
	//Delete ambient sound
	CDelete( g_multipleView );
	ex_pRichEdit = NULL;
	for( std::vector<CScene*>::iterator it = g_scene.begin(); it != g_scene.end(); it++ )
	{
		CDelete( *it );
	}
	//Clear the vctor objects
	g_scene.clear();
	g_render.Destroy();

	CDelete( g_skyDome );

	for( std::vector<CInstanceLight*>::iterator it = g_engineLights.begin(); it != g_engineLights.end(); it++ )
	{
		CDelete( (*it)->m_abstractLight );
		CDelete( *it );
	}
	g_engineLights.clear();

	for( std::vector<CWater*>::iterator it = g_engineWaters.begin(); it != g_engineWaters.end(); it++ )
	{
		CDelete( *it );
	}
	g_engineWaters.clear();

	for( std::vector<CStaticSound*>::iterator it = g_engineStaticSounds.begin(); it != g_engineStaticSounds.end(); it++ )
	{
		CDelete( *it );
	}
	if( g_engineStaticSounds.size() > 0 )
		g_engineStaticSounds.clear();

	if( g_engineObjectNames.size() > 0 )
		g_engineObjectNames.clear();
	//delete the static sound buffers
	for( std::vector<COpenALSoundBuffer*>::iterator it = g_soundBuffers.begin(); it != g_soundBuffers.end(); it++ )
	{
		CDelete( *it );
	}
	g_soundBuffers.clear();
	CDelete( g_translateArrow );
	CDelete( g_negativeZArrow );
	CDelete (g_scaleArrow);
	CDelete(g_rotateArrow);
	CDelete( g_centerArrowScene );
	for( std::vector<CImage*>::iterator it = g_images.begin(); it != g_images.end(); it++ )
	{
		CDelete( *it );
	}
	if( g_images.size() > 0 )
		g_images.clear();

	CDelete( g_bloom );

	for( std::vector<CImage*>::iterator it = g_waterImages.begin(); it != g_waterImages.end(); it++ )
	{
		CDelete( *it );
	}
	if( g_waterImages.size() > 0 )
		g_waterImages.clear();

	//Delete all items from the list boxes
	for (int nItem = m_listBoxScenes.GetItemCount()-1; nItem >= 0 ;nItem-- )
	{
		m_listBoxScenes.DeleteItem(nItem);
	}
	for (int nItem = m_listBoxObjects.GetItemCount(); nItem >= 0 ;nItem-- )
	{
		m_listBoxObjects.DeleteItem(nItem);
	}

	for( CUInt j = 0; j < g_projects.size(); j++ )
	{
		g_projects.erase(g_projects.begin() + j);
	}
	g_projects.clear();

}

CVoid CVandaEngine1Dlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_RICHEDIT21, m_rich);
	DDX_Control(pDX, IDC_LIST_SCENES, m_listBoxScenes);
	DDX_Control(pDX, IDC_LIST_OBJECTS, m_listBoxObjects);
	DDX_Control(pDX, IDC_BTN_REMOVE_SCENE, m_btnRemoveScene);
	DDX_Control(pDX, IDC_BTN_REMOVE_OBJECT, m_btnRemoveObject);
	DDX_Control(pDX, IDC_LIST_ENGINE_OBJECTS, m_listBoxEngineObjects);
	DDX_Control(pDX, IDC_BTN_REMOVE_ENGINE_OBJECT, m_btnRemoveEngineObject);
	DDX_Control(pDX, IDC_BTN_ENGINE_OBJECT_PROPERTIES, m_btnEngineObjectProperties);
	DDX_Control(pDX, IDC_BTN_NEW, m_mainBtnNew);
	DDX_Control(pDX, IDC_BTN_SAVE, m_mainBtnSave);
	DDX_Control(pDX, IDC_BTN_COLLADA, m_mainBtnImportCollada);
	//DDX_Control(pDX, IDC_BTN_PHYSX, m_mainBtnImportPhysX);
	DDX_Control(pDX, IDC_BTN_ANIM_PREV, m_mainBtnPrevAnim);
	DDX_Control(pDX, IDC_BTN_OPEN, m_mainBtnOpen);
	DDX_Control(pDX, IDC_BTN_SAVEAS, m_mainBtnSaveAs);
	DDX_Control(pDX, IDC_BTN_ANIM_PLAY, m_mainBtnPlayAnim);
	DDX_Control(pDX, IDC_BTN_ANIM_NEXT, m_mainBtnNextAnim);
	DDX_Control(pDX, IDC_BTN_TIMER, m_mainBtnTimer);
	DDX_Control(pDX, IDC_BTN_MATERIAL, m_mainBtnMaterial);
	DDX_Control(pDX, IDC_BTN_ANIM_PAUSE, m_mainBtnPauseAnim);
	DDX_Control(pDX, IDC_BTN_WEB, m_mainBtnWeb);
	DDX_Control(pDX, IDC_BTN_FACEBOOK, m_mainBtnFacebook);
	DDX_Control(pDX, IDC_STATIC_SELECTEDOBJECT, m_staticSelectedObject);
	DDX_Control(pDX, IDC_BTN_SKY, m_mainBtnSky);
	DDX_Control(pDX, IDC_BTN_WATER, m_mainBtnWater);
	DDX_Control(pDX, IDC_BTN_STATICSOUND, m_mainBtnStaticSound);
	DDX_Control(pDX, IDC_BTN_AMBIENTSOUND, m_mainBtnAmbientSound);
	DDX_Control(pDX, IDC_BTN_PLAYER, m_mainBtnPlayer);
	DDX_Control(pDX, IDC_BTN_LIGHT, m_mainBtnLight);
	DDX_Control(pDX, IDC_BTN_CONSOLE, m_mainBtnConsole);
	DDX_Control(pDX, IDC_LIST_PHYSX_ELEMENTS, m_listBoxPhysXElements);
	DDX_Control(pDX, IDC_BTN_REMOVE_PHYSX, m_btnRemovePhysX);
	DDX_Control(pDX, IDC_BTN_VANDA_TEXT, m_btnVandaText);
	DDX_Control(pDX, IDC_BTN_TWITTER, m_mainBtnTwitter);
	DDX_Control(pDX, IDC_BTN_YOUTUBE, m_mainBtnYoutube);
	DDX_Control(pDX, IDC_BTN_PUBLISH_SOLUTION, m_mainBtnPublishSolution);
	DDX_Control(pDX, IDC_BTN_SCRIPT_MANAGER, m_mainBtnScriptManager);
	DDX_Control(pDX, IDC_BTN_CAMERA_RENDERING_MANAGER, m_mainBtnCameraAndRenderingManager);
	DDX_Control(pDX, IDC_BTN_VANDA_VERSION, m_btnVandaVersion);
	DDX_Control(pDX, IDC_BTN_SCENE_PROPERTIES, m_btnSceneProperties);
	DDX_Control(pDX, IDC_BTN_PHYSX_EDITOR, m_mainBtnPhysXEditor);
	DDX_Control(pDX, IDC_BTN_PREFABS, m_mainBtnPrefab);
	DDX_Control(pDX, IDC_BTN_TRANSLATE, m_mainBtnTranslate);
	DDX_Control(pDX, IDC_BTN_ROTATE, m_mainBtnRotate);
	DDX_Control(pDX, IDC_BTN_SCALE, m_mainBtnScale);
	DDX_Control(pDX, IDC_BTN_TIMER_PAUSE, m_mainBtnTimerPause);
	DDX_Control(pDX, IDC_BTN_PLAY_ACTIVE, m_mainBtnTestActive);
	DDX_Control(pDX, IDC_BTN_PLAY_DEACTIVE, m_mainBtnTestDeactive);
	DDX_Control(pDX, IDC_EDIT_X, m_editX);
	DDX_Control(pDX, IDC_EDIT_Y, m_editY);
	DDX_Control(pDX, IDC_EDIT_Z, m_editZ);
	DDX_Control(pDX, IDC_BTN_WATER_ATTACH, m_mainBtnWaterAttach);
	DDX_Control(pDX, IDC_BTN_TRIGGER, m_mainBtnTrigger);
}

BEGIN_MESSAGE_MAP(CVandaEngine1Dlg, CDialog)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	//}}AFX_MSG_MAP
//	ON_WM_CLOSE()
	ON_WM_ERASEBKGND()

	ON_BN_CLICKED(IDC_BTN_REMOVE_SCENE, &CVandaEngine1Dlg::OnBnClickedBtnRemoveScene)
	ON_BN_CLICKED(IDC_BTN_REMOVE_OBJECT, &CVandaEngine1Dlg::OnBnClickedBtnRemoveObject)
	ON_WM_CTLCOLOR()
	ON_BN_CLICKED(IDC_BTN_REMOVE_ENGINE_OBJECT, &CVandaEngine1Dlg::OnBnClickedBtnRemoveEngineObject)
	ON_BN_CLICKED(IDC_BTN_ENGINE_OBJECT_PROPERTIES, &CVandaEngine1Dlg::OnBnClickedBtnEngineObjectProperties)
	ON_WM_CLOSE()
//	ON_NOTIFY(NM_RETURN, IDC_LIST_SCENES, &CVandaEngine1Dlg::OnNMReturnListScenes)
ON_BN_CLICKED(IDC_BTN_NEW, &CVandaEngine1Dlg::OnBnClickedBtnNew)
ON_BN_CLICKED(IDC_BTN_SAVE, &CVandaEngine1Dlg::OnBnClickedBtnSave)
ON_BN_CLICKED(IDC_BTN_COLLADA, &CVandaEngine1Dlg::OnBnClickedBtnCollada)
//ON_BN_CLICKED(IDC_BTN_PHYSX, &CVandaEngine1Dlg::OnBnClickedBtnPhysx) removed in version 1.4 or later
ON_BN_CLICKED(IDC_BTN_ANIM_PREV, &CVandaEngine1Dlg::OnBnClickedBtnAnimPrev)
ON_BN_CLICKED(IDC_BTN_OPEN, &CVandaEngine1Dlg::OnBnClickedBtnOpen)
ON_BN_CLICKED(IDC_BTN_SAVEAS, &CVandaEngine1Dlg::OnBnClickedBtnSaveas)
ON_BN_CLICKED(IDC_BTN_ANIM_NEXT, &CVandaEngine1Dlg::OnBnClickedBtnAnimNext)
ON_BN_CLICKED(IDC_BTN_ANIM_PLAY, &CVandaEngine1Dlg::OnBnClickedBtnAnimPlay)
ON_WM_SETCURSOR()
ON_WM_TIMER()
ON_BN_CLICKED(IDC_BTN_TIMER, &CVandaEngine1Dlg::OnBnClickedBtnTimer)
ON_BN_CLICKED(IDC_BTN_ANIM_PAUSE, &CVandaEngine1Dlg::OnBnClickedBtnAnimPause)
ON_BN_CLICKED(IDC_BTN_WEB, &CVandaEngine1Dlg::OnBnClickedBtnWeb)
ON_BN_CLICKED(IDC_BTN_FACEBOOK, &CVandaEngine1Dlg::OnBnClickedBtnFacebook)
ON_BN_CLICKED(IDC_BTN_MATERIAL, &CVandaEngine1Dlg::OnBnClickedBtnMaterial)
ON_BN_CLICKED(IDC_BTN_LIGHT, &CVandaEngine1Dlg::OnBnClickedBtnLight)
ON_BN_CLICKED(IDC_BTN_WATER, &CVandaEngine1Dlg::OnBnClickedBtnWater)
ON_BN_CLICKED(IDC_BTN_AMBIENTSOUND, &CVandaEngine1Dlg::OnBnClickedBtnAmbientsound)
ON_BN_CLICKED(IDC_BTN_STATICSOUND, &CVandaEngine1Dlg::OnBnClickedBtnStaticsound)
ON_BN_CLICKED(IDC_BTN_SKY, &CVandaEngine1Dlg::OnBnClickedBtnSky)
ON_BN_CLICKED(IDC_BTN_PLAYER, &CVandaEngine1Dlg::OnBnClickedBtnPlayer)
ON_BN_CLICKED(IDC_BTN_CONSOLE, &CVandaEngine1Dlg::OnBnClickedBtnConsole)
ON_NOTIFY(LVN_ITEMCHANGED, IDC_LIST_SCENES, &CVandaEngine1Dlg::OnLvnItemchangedListScenes)
ON_NOTIFY(LVN_ITEMCHANGED, IDC_LIST_ENGINE_OBJECTS, &CVandaEngine1Dlg::OnLvnItemchangedListEngineObjects)
ON_BN_CLICKED(IDC_BTN_REMOVE_PHYSX, &CVandaEngine1Dlg::OnBnClickedBtnRemovePhysx)
ON_BN_CLICKED(IDC_BTN_TWITTER, &CVandaEngine1Dlg::OnBnClickedBtnTwitter)
ON_BN_CLICKED(IDC_BTN_YOUTUBE, &CVandaEngine1Dlg::OnBnClickedBtnYoutube)
ON_NOTIFY(LVN_ITEMCHANGED, IDC_LIST_PHYSX_ELEMENTS, &CVandaEngine1Dlg::OnLvnItemchangedListPhysxElements)
ON_BN_CLICKED(IDC_BTN_PUBLISH_SOLUTION, &CVandaEngine1Dlg::OnBnClickedBtnPublishSolution)
ON_BN_CLICKED(IDC_BTN_SCRIPT_MANAGER, &CVandaEngine1Dlg::OnBnClickedBtnScriptManager)
ON_BN_CLICKED(IDC_BTN_CAMERA_RENDERING_MANAGER, &CVandaEngine1Dlg::OnBnClickedBtnCameraRenderingManager)
ON_BN_CLICKED(IDC_BTN_SCENE_PROPERTIES, &CVandaEngine1Dlg::OnBnClickedBtnSceneProperties)
ON_BN_CLICKED(IDC_BTN_PHYSX_EDITOR, &CVandaEngine1Dlg::OnBnClickedBtnPhysxEditor)
ON_WM_NCLBUTTONDBLCLK()
ON_BN_CLICKED(IDC_BTN_PREFABS, &CVandaEngine1Dlg::OnBnClickedBtnPrefabs)
ON_BN_CLICKED(IDC_BTN_TIMER_PAUSE, &CVandaEngine1Dlg::OnBnClickedBtnTimerPause)
ON_BN_CLICKED(IDC_BTN_PLAY_ACTIVE, &CVandaEngine1Dlg::OnBnClickedBtnPlayActive)
ON_BN_CLICKED(IDC_BTN_PLAY_DEACTIVE, &CVandaEngine1Dlg::OnBnClickedBtnPlayDeactive)
ON_BN_CLICKED(IDC_BTN_TRANSLATE, &CVandaEngine1Dlg::OnBnClickedBtnTranslate)
ON_BN_CLICKED(IDC_BTN_ROTATE, &CVandaEngine1Dlg::OnBnClickedBtnRotate)
ON_BN_CLICKED(IDC_BTN_SCALE, &CVandaEngine1Dlg::OnBnClickedBtnScale)
ON_EN_CHANGE(IDC_EDIT_X, &CVandaEngine1Dlg::OnEnChangeEditX)
ON_EN_CHANGE(IDC_EDIT_Y, &CVandaEngine1Dlg::OnEnChangeEditY)
ON_EN_CHANGE(IDC_EDIT_Z, &CVandaEngine1Dlg::OnEnChangeEditZ)
ON_BN_CLICKED(IDC_BTN_WATER_ATTACH, &CVandaEngine1Dlg::OnBnClickedBtnWaterAttach)
ON_BN_CLICKED(IDC_BTN_TRIGGER, &CVandaEngine1Dlg::OnBnClickedBtnTrigger)
END_MESSAGE_MAP()


// CVandaEngine1Dlg message handlers

BOOL CVandaEngine1Dlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// Add "About..." menu item to system menu.

	// IDM_ABOUTBOX must be in the system command range.
	//ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	//ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		pSysMenu->RemoveMenu( SC_MOVE, MF_BYCOMMAND );
		pSysMenu->RemoveMenu( SC_SIZE, MF_BYCOMMAND );
		pSysMenu->RemoveMenu( SC_RESTORE, MF_BYCOMMAND );
	//	CString strAboutMenu;
	//	strAboutMenu.LoadString(IDS_ABOUTBOX);
	//	if (!strAboutMenu.IsEmpty())
	//	{
	//		pSysMenu->AppendMenu(MF_SEPARATOR);
	//		pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
	//	}
	}

	m_pToolTip = new CToolTipCtrl;
	if(!m_pToolTip->Create(this))
	{
	   TRACE("Unable To create ToolTip\n");
	   return TRUE;
	}

	m_pToolTip->AddTool(&m_mainBtnNew, "New scene");
	m_pToolTip->AddTool(&m_mainBtnOpen, "Open scene");
	m_pToolTip->AddTool(&m_mainBtnSave, "Save");
	m_pToolTip->AddTool(&m_mainBtnSaveAs, "Save As");
	m_pToolTip->AddTool(&m_mainBtnImportCollada, "Import COLLADA");
	//m_pToolTip->AddTool(&m_mainBtnImportPhysX, "Import PhysX"); removed in version 1.4 or later
	m_pToolTip->AddTool(&m_mainBtnPublishSolution, "Publish Solution");
	m_pToolTip->AddTool(&m_mainBtnScriptManager, "Script Manager");
	m_pToolTip->AddTool(&m_mainBtnCameraAndRenderingManager, "Camera And Rendering");
	m_pToolTip->AddTool(&m_mainBtnWaterAttach, "Water Attachment");
	m_pToolTip->AddTool(&m_mainBtnTimer, "Enable/Disable Timer");
	m_pToolTip->AddTool(&m_mainBtnMaterial, "Open Material Editor");
	m_pToolTip->AddTool(&m_mainBtnPhysXEditor, "Open Physics Editor");
	m_pToolTip->AddTool(&m_mainBtnNextAnim, "Next Animation");
	m_pToolTip->AddTool(&m_mainBtnPrevAnim, "Previous Animation");
	m_pToolTip->AddTool(&m_mainBtnPlayAnim, "Play Animation");
	m_pToolTip->AddTool(&m_mainBtnPauseAnim, "Pause Animation");
	m_pToolTip->AddTool(&m_mainBtnPrefab, "Open Prefab Editor");
	m_pToolTip->AddTool(&m_mainBtnTranslate, "Translate");
	m_pToolTip->AddTool(&m_mainBtnRotate, "Rotate");
	m_pToolTip->AddTool(&m_mainBtnScale, "Scale");
	m_pToolTip->AddTool(&m_mainBtnTestActive, "Enable/Disable Play Mode");
	m_pToolTip->AddTool(&m_mainBtnTestDeactive, "Enable/Disable Play Mode");
	m_pToolTip->AddTool(&m_mainBtnConsole, "Console Window");
	m_pToolTip->AddTool(&m_mainBtnWeb, "www.vandaengine.org");
	m_pToolTip->AddTool(&m_mainBtnFacebook, "www.facebook.com/VandaEngine");
	m_pToolTip->AddTool(&m_mainBtnTwitter, "www.twitter.com/Vanda_Engine");
	m_pToolTip->AddTool(&m_mainBtnYoutube, "www.youtube.com/VandaEngine");

	m_pToolTip->AddTool(&m_mainBtnSky, "Create Sky");
	m_pToolTip->AddTool(&m_mainBtnWater, "Create Water");
	m_pToolTip->AddTool(&m_mainBtnAmbientSound, "Create Ambient Sound");
	m_pToolTip->AddTool(&m_mainBtnStaticSound, "Create Static Sound");
	m_pToolTip->AddTool(&m_mainBtnPlayer, "Create Player");
	m_pToolTip->AddTool(&m_mainBtnLight, "Create Light");
	m_pToolTip->AddTool(&m_mainBtnTrigger, "Create Trigger");

	m_pToolTip->Activate(TRUE);

	m_brush.CreateSolidBrush(RGB(255, 255, 255));
	//load the menu here
	CMenu menu;
	menu.LoadMenu (IDR_MENU1);
	SetMenu(&menu);
	menu.Detach();
	
	ex_pMenu = GetMenu();

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon

	SetWindowText(_T("Vanda Engine 1.4"));

	// TODO: Add extra initialization here
	ShowWindow( SW_SHOWMAXIMIZED );
	UpdateWindow();

	CRect windowRect;
	GetClientRect( &windowRect );

	CRect rcRect;

	CInt fivePercent;
	CInt previousRight;
	CInt previousLeft;
	m_startLeftButtons = 6;
	//Initialize main *New* button here
	CFloat aspectRatio = fabs( CFloat( windowRect.bottom - windowRect.top ) / CFloat( windowRect.right - windowRect.left ) );
	fivePercent = ( (5 * ( windowRect.right - windowRect.left ) / 100) * aspectRatio);
	m_horizontalPointFivePercent = CInt(0.5 * (windowRect.right - windowRect.left) / 100);

	rcRect.left = CInt( 1 * ( windowRect.right - windowRect.left ) / 100);
	rcRect.right = rcRect.left + fivePercent;
	rcRect.top = CInt( ( m_startLeftButtons - 5.5)  * ( windowRect.bottom - windowRect.top ) / 100 );
	rcRect.bottom = rcRect.top + (5 * ( windowRect.bottom - windowRect.top ) / 100 );
	//ScreenToClient( &rcRect );
	m_mainBtnNew.MoveWindow( rcRect );
	m_mainBtnNew.LoadBitmaps( IDB_BITMAP_NEW_UP, IDB_BITMAP_NEW_DOWN, IDB_BITMAP_NEW_FOCUS );
	m_mainBtnNew.ShowWindow( SW_SHOW );
	m_mainBtnNew.UpdateWindow();

	//Initialize main *Open* button here
	previousRight = rcRect.right;
	rcRect.left = previousRight + m_horizontalPointFivePercent;
	rcRect.right = rcRect.left + fivePercent;

	m_mainBtnOpen.MoveWindow( rcRect );
	m_mainBtnOpen.LoadBitmaps(IDB_BITMAP_OPEN_UP, IDB_BITMAP_OPEN_DOWN, IDB_BITMAP_OPEN_FOCUS, IDB_BITMAP_OPEN_DISABLE);
	m_mainBtnOpen.ShowWindow( SW_SHOW );
	m_mainBtnOpen.UpdateWindow();

	//Initialize main *Import COLLADA* button here

	//previousRight = rcRect.right;
	//rcRect.left = previousRight + m_horizontalPointFivePercent;
	//rcRect.right = rcRect.left + fivePercent;

	//m_mainBtnImportCollada.MoveWindow( rcRect );
	m_mainBtnImportCollada.LoadBitmaps( IDB_BITMAP_COLLADA_UP, IDB_BITMAP_COLLADA_DOWN, IDB_BITMAP_COLLADA_FOCUS );
	m_mainBtnImportCollada.ShowWindow( SW_HIDE );
	m_mainBtnImportCollada.UpdateWindow();

	//Initialize main *Import PhysX* button here
	//removed in version 1.4 or latebur
	//previousRight = rcRect.right;
	//rcRect.left = previousRight + m_horizontalPointFivePercent;
	//rcRect.right = rcRect.left + fivePercent;

	//m_mainBtnImportPhysX.MoveWindow( rcRect );
	//m_mainBtnImportPhysX.LoadBitmaps( IDB_BITMAP_PHYSX_UP, IDB_BITMAP_PHYSX_DOWN, IDB_BITMAP_PHYSX_FOCUS );
	//m_mainBtnImportPhysX.ShowWindow( SW_SHOW );
	//m_mainBtnImportPhysX.UpdateWindow();

	//Initialize main *Save* button here

	previousRight = rcRect.right;
	rcRect.left = previousRight + m_horizontalPointFivePercent;
	rcRect.right = rcRect.left + fivePercent;

	m_mainBtnSave.MoveWindow( rcRect );
	m_mainBtnSave.LoadBitmaps( IDB_BITMAP_SAVE_UP, IDB_BITMAP_SAVE_DOWN, IDB_BITMAP_SAVE_FOCUS, IDB_BITMAP_SAVE_DISABLE );
	m_mainBtnSave.ShowWindow( SW_SHOW );
	m_mainBtnSave.UpdateWindow();

	//Initialize main *Save As* button here
	previousRight = rcRect.right;
	rcRect.left = previousRight + m_horizontalPointFivePercent;
	rcRect.right = rcRect.left + fivePercent;

	m_mainBtnSaveAs.MoveWindow( rcRect );
	m_mainBtnSaveAs.LoadBitmaps( IDB_BITMAP_SAVEAS_UP, IDB_BITMAP_SAVEAS_DOWN, IDB_BITMAP_SAVEAS_FOCUS, IDB_BITMAP_SAVEAS_DISABLE );
	m_mainBtnSaveAs.ShowWindow( SW_SHOW );
	m_mainBtnSaveAs.UpdateWindow();
	
	//Initialize main *Translate* button here
	previousRight = rcRect.right;
	rcRect.left = previousRight + m_horizontalPointFivePercent;
	rcRect.right = rcRect.left + fivePercent;

	m_mainBtnTranslate.MoveWindow(rcRect);
	m_mainBtnTranslate.LoadBitmaps(IDB_BITMAP_TRANSLATE_UP, IDB_BITMAP_TRANSLATE_DOWN, IDB_BITMAP_TRANSLATE_FOCUS, IDB_BITMAP_TRANSLATE_DISABLE);
	m_mainBtnTranslate.ShowWindow(SW_SHOW);
	m_mainBtnTranslate.UpdateWindow();

	//Initialize main *Rotate* button here
	previousRight = rcRect.right;
	rcRect.left = previousRight + m_horizontalPointFivePercent;
	rcRect.right = rcRect.left + fivePercent;

	m_mainBtnRotate.MoveWindow(rcRect);
	m_mainBtnRotate.LoadBitmaps(IDB_BITMAP_ROTATE_UP, IDB_BITMAP_ROTATE_DOWN, IDB_BITMAP_ROTATE_FOCUS, IDB_BITMAP_ROTATE_DISABLE);
	m_mainBtnRotate.ShowWindow(SW_SHOW);
	m_mainBtnRotate.UpdateWindow();

	//Initialize main *Scale* button here
	previousRight = rcRect.right;
	rcRect.left = previousRight + m_horizontalPointFivePercent;
	rcRect.right = rcRect.left + fivePercent;

	m_mainBtnScale.MoveWindow(rcRect);
	m_mainBtnScale.LoadBitmaps(IDB_BITMAP_SCALE_UP, IDB_BITMAP_SCALE_DOWN, IDB_BITMAP_SCALE_FOCUS, IDB_BITMAP_SCALE_DISABLE);
	m_mainBtnScale.ShowWindow(SW_SHOW);
	m_mainBtnScale.UpdateWindow();

	//Initialize main *Prefab* button here
	previousRight = rcRect.right;
	rcRect.left = previousRight + m_horizontalPointFivePercent;
	rcRect.right = rcRect.left + fivePercent;

	m_mainBtnPrefab.MoveWindow(rcRect);
	m_mainBtnPrefab.LoadBitmaps(IDB_BITMAP_PREFAB_UP, IDB_BITMAP_PREFAB_DOWN, IDB_BITMAP_PREFAB_FOCUS, IDB_BITMAP_PREFAB_DISABLE);
	m_mainBtnPrefab.ShowWindow(SW_SHOW);
	m_mainBtnPrefab.UpdateWindow();

	//Initialize main *Publish* button here
	previousRight = rcRect.right;
	rcRect.left = previousRight + m_horizontalPointFivePercent;
	rcRect.right = rcRect.left + fivePercent;

	m_mainBtnPublishSolution.MoveWindow( rcRect );
	m_mainBtnPublishSolution.LoadBitmaps(IDB_BITMAP_PUBLISH_UP, IDB_BITMAP_PUBLISH_DOWN, IDB_BITMAP_PUBLISH_FOCUS, IDB_BITMAP_PUBLISH_DISABLE);
	m_mainBtnPublishSolution.ShowWindow( SW_SHOW );
	m_mainBtnPublishSolution.UpdateWindow();

	//Initialize main *Camera And Rendering* button here
	previousRight = rcRect.right;
	rcRect.left = previousRight + m_horizontalPointFivePercent;
	rcRect.right = rcRect.left + fivePercent;

	m_mainBtnCameraAndRenderingManager.MoveWindow( rcRect );
	m_mainBtnCameraAndRenderingManager.LoadBitmaps( IDB_BITMAP_CAMERA_UP, IDB_BITMAP_CAMERA_DOWN, IDB_BITMAP_CAMERA_FOCUS );
	m_mainBtnCameraAndRenderingManager.ShowWindow( SW_SHOW );
	m_mainBtnCameraAndRenderingManager.UpdateWindow();

	//Initialize main *Camera And Rendering* button here
	previousRight = rcRect.right;
	rcRect.left = previousRight + m_horizontalPointFivePercent;
	rcRect.right = rcRect.left + fivePercent;

	m_mainBtnWaterAttach.MoveWindow(rcRect);
	m_mainBtnWaterAttach.LoadBitmaps(IDB_BITMAP_WATER_ATTACH_UP, IDB_BITMAP_WATER_ATTACH_DOWN, IDB_BITMAP_WATER_ATTACH_FOCUS);
	m_mainBtnWaterAttach.ShowWindow(SW_SHOW);
	m_mainBtnWaterAttach.UpdateWindow();

	//Initialize main *Script* button here
	previousRight = rcRect.right;
	rcRect.left = previousRight + m_horizontalPointFivePercent;
	rcRect.right = rcRect.left + fivePercent;

	m_mainBtnScriptManager.MoveWindow(rcRect);
	m_mainBtnScriptManager.LoadBitmaps(IDB_BITMAP_SCRIPT_UP, IDB_BITMAP_SCRIPT_DOWN, IDB_BITMAP_SCRIPT_FOCUS, IDB_BITMAP_SCRIPT_DISABLE);
	m_mainBtnScriptManager.ShowWindow(SW_SHOW);
	m_mainBtnScriptManager.UpdateWindow();


	//Initialize main *Timer* button here
	previousRight = rcRect.right;
	rcRect.left = previousRight + m_horizontalPointFivePercent;
	rcRect.right = rcRect.left + fivePercent;

	m_mainBtnTimer.MoveWindow( rcRect );
	m_mainBtnTimer.LoadBitmaps( IDB_BITMAP_TIMER_UP, IDB_BITMAP_TIMER_DOWN, IDB_BITMAP_TIMER_FOCUS );
	m_mainBtnTimer.ShowWindow( SW_SHOW );
	m_mainBtnTimer.UpdateWindow();
	m_mainBtnTimer.EnableWindow( TRUE );

	m_mainBtnTimerPause.MoveWindow(rcRect);
	m_mainBtnTimerPause.LoadBitmaps(IDB_BITMAP_TIMER_PAUSE_UP, IDB_BITMAP_TIMER_PAUSE_DOWN, IDB_BITMAP_TIMER_PAUSE_FOCUS);
	m_mainBtnTimerPause.ShowWindow(SW_HIDE);
	m_mainBtnTimerPause.UpdateWindow();
	m_mainBtnTimerPause.EnableWindow(FALSE);

	//Initialize main *Play Mode* button here
	previousRight = rcRect.right;
	rcRect.left = previousRight + m_horizontalPointFivePercent;
	rcRect.right = rcRect.left + fivePercent;

	m_mainBtnTestActive.MoveWindow(rcRect);
	m_mainBtnTestActive.LoadBitmaps(IDB_BITMAP_TEST_ACTIVE_UP, IDB_BITMAP_TEST_ACTIVE_DOWN, IDB_BITMAP_TEST_ACTIVE_FOCUS);
	m_mainBtnTestActive.ShowWindow(SW_HIDE);
	m_mainBtnTestActive.UpdateWindow();
	m_mainBtnTestActive.EnableWindow(FALSE);

	m_mainBtnTestDeactive.MoveWindow(rcRect);
	m_mainBtnTestDeactive.LoadBitmaps(IDB_BITMAP_TEST_DEACTIVE_UP, IDB_BITMAP_TEST_DEACTIVE_DOWN, IDB_BITMAP_TEST_DEACTIVE_FOCUS);
	m_mainBtnTestDeactive.ShowWindow(SW_SHOW);
	m_mainBtnTestDeactive.UpdateWindow();
	m_mainBtnTestDeactive.EnableWindow(TRUE);

	//Initialize main *Console* button here

	previousRight = rcRect.right;
	rcRect.left = previousRight + m_horizontalPointFivePercent;
	rcRect.right = rcRect.left + fivePercent;

	m_mainBtnConsole.MoveWindow(rcRect);
	m_mainBtnConsole.LoadBitmaps(IDB_BITMAP_CONSOLE_UP, IDB_BITMAP_CONSOLE_DOWN, IDB_BITMAP_CONSOLE_FOCUS);
	m_mainBtnConsole.ShowWindow(SW_SHOW);
	m_mainBtnConsole.UpdateWindow();
	m_mainBtnConsole.EnableWindow(TRUE);

	//Initialize main *Material* button here
	previousRight = rcRect.right;
	rcRect.left = previousRight + m_horizontalPointFivePercent;
	rcRect.right = rcRect.left + fivePercent;

	m_mainBtnMaterial.MoveWindow( rcRect );
	m_mainBtnMaterial.LoadBitmaps( IDB_BITMAP_MATERIAL_UP, IDB_BITMAP_MATERIAL_DOWN, IDB_BITMAP_MATERIAL_FOCUS, IDB_BITMAP_MATERIAL_DISABLE );
	m_mainBtnMaterial.ShowWindow(SW_HIDE);
	m_mainBtnMaterial.UpdateWindow();
	m_mainBtnMaterial.EnableWindow( CFalse );

	//Initialize main *PhysX* button here
	previousRight = rcRect.right;
	rcRect.left = previousRight + m_horizontalPointFivePercent;
	rcRect.right = rcRect.left + fivePercent;

	m_mainBtnPhysXEditor.MoveWindow( rcRect );
	m_mainBtnPhysXEditor.LoadBitmaps( IDB_BITMAP_PHYSXEDITOR_UP, IDB_BITMAP_PHYSXEDITOR_DOWN, IDB_BITMAP_PHYSXEDITOR_UP, IDB_BITMAP_PHYSXEDITOR_DISABLE );
	m_mainBtnPhysXEditor.ShowWindow( SW_HIDE );
	m_mainBtnPhysXEditor.UpdateWindow();
	m_mainBtnPhysXEditor.EnableWindow( CFalse );

	//Initialize main *Previous Animation* button here
	previousRight = rcRect.right;
	rcRect.left = previousRight + m_horizontalPointFivePercent;
	rcRect.right = rcRect.left + fivePercent;

	m_mainBtnPrevAnim.MoveWindow(rcRect);
	m_mainBtnPrevAnim.LoadBitmaps(IDB_BITMAP_ANIM_PREV_UP, IDB_BITMAP_ANIM_PREV_DOWN, IDB_BITMAP_ANIM_PREV_FOCUS, IDB_BITMAP_ANIM_PREV_DISABLE);
	m_mainBtnPrevAnim.ShowWindow(SW_HIDE);
	m_mainBtnPrevAnim.UpdateWindow();
	m_mainBtnPrevAnim.EnableWindow(FALSE);

	//Initialize main *Play Animation* button here
	previousRight = rcRect.right;
	rcRect.left = previousRight + m_horizontalPointFivePercent;
	rcRect.right = rcRect.left + fivePercent;

	m_mainBtnPlayAnim.MoveWindow(rcRect);
	m_mainBtnPlayAnim.LoadBitmaps(IDB_BITMAP_ANIM_PLAY_UP, IDB_BITMAP_ANIM_PLAY_DOWN, IDB_BITMAP_ANIM_PLAY_FOCUS, IDB_BITMAP_ANIM_PLAY_DISABLE);
	m_mainBtnPlayAnim.ShowWindow(SW_HIDE);
	m_mainBtnPlayAnim.UpdateWindow();
	m_mainBtnPlayAnim.EnableWindow(FALSE);

	//Initialize main *Pause Animation* button here

	m_mainBtnPauseAnim.MoveWindow(rcRect);
	m_mainBtnPauseAnim.LoadBitmaps(IDB_BITMAP_ANIM_PAUSE_UP, IDB_BITMAP_ANIM_PAUSE_DOWN, IDB_BITMAP_ANIM_PAUSE_FOCUS, IDB_BITMAP_ANIM_PAUSE_DISABLE);
	m_mainBtnPauseAnim.ShowWindow(SW_HIDE);
	m_mainBtnPauseAnim.UpdateWindow();
	m_mainBtnPauseAnim.EnableWindow(FALSE);

	//Initialize main *Next Animation* button here
	previousRight = rcRect.right;
	rcRect.left = previousRight + m_horizontalPointFivePercent;
	rcRect.right = rcRect.left + fivePercent;

	m_mainBtnNextAnim.MoveWindow(rcRect);
	m_mainBtnNextAnim.LoadBitmaps(IDB_BITMAP_ANIM_NEXT_UP, IDB_BITMAP_ANIM_NEXT_DOWN, IDB_BITMAP_ANIM_NEXT_FOCUS, IDB_BITMAP_ANIM_NEXT_DISABLE);
	m_mainBtnNextAnim.ShowWindow(SW_HIDE);
	m_mainBtnNextAnim.UpdateWindow();
	m_mainBtnNextAnim.EnableWindow(FALSE);

	//start from end
	//Initialize main *Web* button here ( Last button )
	rcRect.bottom = rcRect.top + (4 * (windowRect.bottom - windowRect.top) / 100);
	CInt fourPercent = ((4 * (windowRect.right - windowRect.left) / 100) * aspectRatio);
	CInt m_horizontalPointFourPercent = CInt(0.4 * (windowRect.right - windowRect.left) / 100);

	rcRect.right = CInt( 99 * ( windowRect.right - windowRect.left ) / 100);
	rcRect.left = rcRect.right - fourPercent;

	m_mainBtnWeb.MoveWindow( rcRect );
	m_mainBtnWeb.LoadBitmaps( IDB_BITMAP_WEB_UP, IDB_BITMAP_WEB_DOWN, IDB_BITMAP_WEB_FOCUS );
	m_mainBtnWeb.ShowWindow( SW_HIDE );
	m_mainBtnWeb.UpdateWindow();
	m_mainBtnWeb.EnableWindow( FALSE );

	//Initialize main *Facebook* button here
	previousLeft = rcRect.left;
	rcRect.right = previousLeft - m_horizontalPointFourPercent;
	rcRect.left = rcRect.right - fourPercent;

	m_mainBtnFacebook.MoveWindow( rcRect );
	m_mainBtnFacebook.LoadBitmaps( IDB_BITMAP_FACEBOOK_UP, IDB_BITMAP_FACEBOOK_DOWN, IDB_BITMAP_FACEBOOK_FOCUS );
	m_mainBtnFacebook.ShowWindow( SW_HIDE );
	m_mainBtnFacebook.UpdateWindow();
	m_mainBtnFacebook.EnableWindow( FALSE );

	//Initialize main *Twitter* button here
	previousLeft = rcRect.left;
	rcRect.right = previousLeft - m_horizontalPointFourPercent;
	rcRect.left = rcRect.right - fourPercent;

	m_mainBtnTwitter.MoveWindow( rcRect );
	m_mainBtnTwitter.LoadBitmaps( IDB_BITMAP_TWITTER_UP, IDB_BITMAP_TWITTER_DOWN, IDB_BITMAP_TWITTER_FOCUS );
	m_mainBtnTwitter.ShowWindow( SW_HIDE );
	m_mainBtnTwitter.UpdateWindow();
	m_mainBtnTwitter.EnableWindow( FALSE );

	//Initialize main *YouTube* button here
	previousLeft = rcRect.left;
	rcRect.right = previousLeft - m_horizontalPointFourPercent;
	rcRect.left = rcRect.right - fourPercent;

	m_mainBtnYoutube.MoveWindow( rcRect );
	m_mainBtnYoutube.LoadBitmaps( IDB_BITMAP_YOUTUBE_UP, IDB_BITMAP_YOUTUBE_DOWN, IDB_BITMAP_YOUTUBE_FOCUS );
	m_mainBtnYoutube.ShowWindow( SW_HIDE );
	m_mainBtnYoutube.UpdateWindow();
	m_mainBtnYoutube.EnableWindow( FALSE );

	//Engine Objects' icons
	CFloat ButtonSizeAndGap = 7.0f;
	CFloat ButtonSize = 5.0f;
	m_startLeftButtons = 7; //six.five percent

	//Initialize main *Sky* button here
	rcRect.left = 2 * m_horizontalPointFivePercent;
	rcRect.right = rcRect.left + fivePercent;
	rcRect.top = CInt( m_startLeftButtons * ( windowRect.bottom - windowRect.top ) / 100 );
	rcRect.bottom = rcRect.top + (ButtonSize * ( windowRect.bottom - windowRect.top ) / 100 );

	m_mainBtnSky.MoveWindow( rcRect );
	m_mainBtnSky.LoadBitmaps( IDB_BITMAP_SKY_UP, IDB_BITMAP_SKY_DOWN, IDB_BITMAP_SKY_FOCUS, IDB_BITMAP_SKY_DISABLE );
	m_mainBtnSky.ShowWindow( SW_SHOW );
	m_mainBtnSky.UpdateWindow();
	m_mainBtnSky.EnableWindow( TRUE );

	//Initialize main *Water* button here
	previousRight = rcRect.right;
	rcRect.top = CInt( ( m_startLeftButtons + ButtonSizeAndGap ) * ( windowRect.bottom - windowRect.top ) / 100 );
	rcRect.bottom = rcRect.top + (ButtonSize * ( windowRect.bottom - windowRect.top ) / 100 );

	m_mainBtnWater.MoveWindow( rcRect );
	m_mainBtnWater.LoadBitmaps(IDB_BITMAP_WATER_UP, IDB_BITMAP_WATER_DOWN, IDB_BITMAP_WATER_FOCUS, IDB_BITMAP_WATER_DISABLE);
	m_mainBtnWater.ShowWindow( SW_SHOW );
	m_mainBtnWater.UpdateWindow();
	m_mainBtnWater.EnableWindow( TRUE );

	//Initialize main *Light* button here
	previousRight = rcRect.right;
	rcRect.top = CInt( ( m_startLeftButtons + 2 * ButtonSizeAndGap ) * ( windowRect.bottom - windowRect.top ) / 100 );
	rcRect.bottom = rcRect.top + (ButtonSize * ( windowRect.bottom - windowRect.top ) / 100 );

	m_mainBtnLight.MoveWindow( rcRect );
	m_mainBtnLight.LoadBitmaps( IDB_BITMAP_LIGHT_UP, IDB_BITMAP_LIGHT_DOWN, IDB_BITMAP_LIGHT_FOCUS, IDB_BITMAP_LIGHT_DISABLE );
	m_mainBtnLight.ShowWindow( SW_SHOW );
	m_mainBtnLight.UpdateWindow();
	m_mainBtnLight.EnableWindow( TRUE );

	//Initialize main *Static Sound* button here
	rcRect.top = CInt( ( m_startLeftButtons + 3 * ButtonSizeAndGap ) * ( windowRect.bottom - windowRect.top ) / 100 );
	rcRect.bottom = rcRect.top + (ButtonSize * ( windowRect.bottom - windowRect.top ) / 100 );

	m_mainBtnStaticSound.MoveWindow( rcRect );
	m_mainBtnStaticSound.LoadBitmaps(IDB_BITMAP_STATICSOUND_UP, IDB_BITMAP_STATICSOUND_DOWN, IDB_BITMAP_STATICSOUND_FOCUS, IDB_BITMAP_STATICSOUND_DISABLE);
	m_mainBtnStaticSound.ShowWindow( SW_SHOW );
	m_mainBtnStaticSound.UpdateWindow();
	m_mainBtnStaticSound.EnableWindow( TRUE );

	//Initialize *Ambient Sound* button here
	rcRect.top = CInt( ( m_startLeftButtons + 4 * ButtonSizeAndGap ) * ( windowRect.bottom - windowRect.top ) / 100 );
	rcRect.bottom = rcRect.top + (ButtonSize * ( windowRect.bottom - windowRect.top ) / 100 );

	m_mainBtnAmbientSound.MoveWindow( rcRect );
	m_mainBtnAmbientSound.LoadBitmaps( IDB_BITMAP_AMBIENTSOUND_UP, IDB_BITMAP_AMBIENTSOUND_DOWN, IDB_BITMAP_AMBIENTSOUND_FOCUS, IDB_BITMAP_AMBIENTSOUND_DISABLE );
	m_mainBtnAmbientSound.ShowWindow( SW_SHOW );
	m_mainBtnAmbientSound.UpdateWindow();
	m_mainBtnAmbientSound.EnableWindow( TRUE );

	//Initialize main *Player* button here
	rcRect.top = CInt( ( m_startLeftButtons + 5 * ButtonSizeAndGap ) * ( windowRect.bottom - windowRect.top ) / 100 );
	rcRect.bottom = rcRect.top + (ButtonSize * ( windowRect.bottom - windowRect.top ) / 100 );

	m_mainBtnPlayer.MoveWindow( rcRect );
	m_mainBtnPlayer.LoadBitmaps( IDB_BITMAP_PLAYER_UP, IDB_BITMAP_PLAYER_DOWN, IDB_BITMAP_PLAYER_FOCUS );
	m_mainBtnPlayer.ShowWindow( SW_SHOW );
	m_mainBtnPlayer.UpdateWindow();
	m_mainBtnPlayer.EnableWindow( TRUE );

	//Initialize main *Player* button here
	rcRect.top = CInt((m_startLeftButtons + 6 * ButtonSizeAndGap) * (windowRect.bottom - windowRect.top) / 100);
	rcRect.bottom = rcRect.top + (ButtonSize * (windowRect.bottom - windowRect.top) / 100);

	m_mainBtnTrigger.MoveWindow(rcRect);
	m_mainBtnTrigger.LoadBitmaps(IDB_BITMAP_TRIGGER_UP, IDB_BITMAP_TRIGGER_DOWN, IDB_BITMAP_TRIGGER_FOCUS, IDB_BITMAP_TRIGGER_DISABLE);
	m_mainBtnTrigger.ShowWindow(SW_SHOW);
	m_mainBtnTrigger.UpdateWindow();
	m_mainBtnTrigger.EnableWindow(TRUE);

	m_horizontalSizeOfLeftCulomn = 2 * m_horizontalPointFivePercent + 1 * fivePercent; //2 gaps+1 Buttons
	m_horizontalSizeOfRightCulomn = 5 * m_horizontalPointFivePercent + 5.5 * fivePercent; //5 gaps+3 Buttons
	CFloat verticalOnePercent = ( windowRect.bottom - windowRect.top ) / 100.0f;

	//Vanda Engine Text (Version)
	rcRect.top = CInt( 90 * ( windowRect.bottom - windowRect.top ) / 100 );
	rcRect.bottom = CInt( 94 * ( windowRect.bottom - windowRect.top ) / 100 );
	rcRect.left =  CInt( 0.5 * ( windowRect.right - windowRect.left ) / 100 );
	rcRect.right = CInt( 1.7 * ( windowRect.right - windowRect.left ) / 100 );

	m_btnVandaVersion.MoveWindow( rcRect );
	m_btnVandaVersion.LoadBitmaps( IDB_BITMAP_VANDA_VERSION, IDB_BITMAP_VANDA_VERSION, IDB_BITMAP_VANDA_VERSION, IDB_BITMAP_VANDA_VERSION );
	m_btnVandaVersion.ShowWindow( SW_SHOW );
	m_btnVandaVersion.UpdateWindow();
	m_btnVandaVersion.EnableWindow( FALSE );

	//Vanda Engine Text
	rcRect.top = CInt( 85 * ( windowRect.bottom - windowRect.top ) / 100 );
	rcRect.bottom = CInt( 94 * ( windowRect.bottom - windowRect.top ) / 100 );
	rcRect.left =  CInt( 2.5 * ( windowRect.right - windowRect.left ) / 100 );
	rcRect.right = CInt( 3.5 * ( windowRect.right - windowRect.left ) / 100 );

	m_btnVandaText.MoveWindow( rcRect );
	m_btnVandaText.LoadBitmaps( IDB_BITMAP_VANDA_TEXT, IDB_BITMAP_VANDA_TEXT, IDB_BITMAP_VANDA_TEXT, IDB_BITMAP_VANDA_TEXT );
	m_btnVandaText.ShowWindow( SW_SHOW );
	m_btnVandaText.UpdateWindow();
	m_btnVandaText.EnableWindow( FALSE );

	//==============================================
	CFont fnt2;
	rcRect.top = long( 1 * ( windowRect.top - windowRect.bottom ) / 100);
	rcRect.bottom = long( 3 * ( windowRect.top - windowRect.bottom ) / 100);
	//ScreenToClient( &rcRect );
	CInt fontSizeY = rcRect.bottom - rcRect.top; 
	fnt2.CreateFont(-fontSizeY, 0, 0, 0, FW_NORMAL, FALSE, FALSE, FALSE, ANSI_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, PROOF_QUALITY, DEFAULT_PITCH, "Consolas");
	rcRect.top = long( 1 * ( windowRect.top - windowRect.bottom ) / 100);
	rcRect.bottom = long( 3.5 * ( windowRect.top - windowRect.bottom ) / 100);
	//ScreenToClient( &rcRect );

	//==============================================
	//static text: List of collada scenes
	rcRect.left = CInt( 100 * ( windowRect.right - windowRect.left ) / 100 ) - m_horizontalSizeOfRightCulomn + m_horizontalPointFivePercent;
	rcRect.right = CInt( 100 * ( windowRect.right - windowRect.left ) / 100 ) - m_horizontalPointFivePercent;

	rcRect.top = previousColumn1 + long(verticalOnePercent);
	rcRect.bottom = rcRect.top + long(2 * verticalOnePercent);

	//ScreenToClient( &rcRect );
	GetDlgItem( IDC_STATIC_SCENES )->MoveWindow( rcRect );
	GetDlgItem( IDC_STATIC_SCENES )->ShowWindow( SW_HIDE );
	GetDlgItem( IDC_STATIC_SCENES )->SetFont( &fnt2 );
	GetDlgItem( IDC_STATIC_SCENES )->UpdateWindow();

	//ScreenToClient( &rcRect );
	GetDlgItem(IDC_STATIC_PREFABS)->MoveWindow(rcRect);
	GetDlgItem(IDC_STATIC_PREFABS)->ShowWindow(SW_SHOW);
	GetDlgItem(IDC_STATIC_PREFABS)->SetFont(&fnt2);
	GetDlgItem(IDC_STATIC_PREFABS)->UpdateWindow();

	//list control: COLLADA scenes
	rcRect.left = CInt( 100 * ( windowRect.right - windowRect.left ) / 100 ) - m_horizontalSizeOfRightCulomn + 2 * m_horizontalPointFivePercent;
	rcRect.right = CInt( 100 * ( windowRect.right - windowRect.left ) / 100 ) - 2 * m_horizontalPointFivePercent;
	rcRect.top = previousColumn1 + long(5 * verticalOnePercent);
	rcRect.bottom = rcRect.top + long(19.5 * verticalOnePercent);

	//ScreenToClient( &rcRect );

	m_listBoxScenes.MoveWindow( rcRect );
	m_listBoxScenes.SetBkColor( RGB( 40, 40, 40 ));
	m_listBoxScenes.SetTextBkColor( RGB( 40, 40, 40 ) );
	m_listBoxScenes.SetTextColor(RGB(140, 140, 140));
	
	RECT tempRect;
	m_listBoxScenes.GetClientRect( &tempRect );

	m_listBoxScenes.InsertColumn(0, "Scenes", LVCFMT_LEFT |  LVS_SHOWSELALWAYS, ( tempRect.right - tempRect.left ) * 80 / 100 );
	m_listBoxScenes.SetFont( &fnt2 );
	m_listBoxScenes.ShowWindow( SW_SHOW );
	m_listBoxScenes.UpdateWindow();


	// Remove "Button" of COLLADA Scenes
	rcRect.left = ( windowRect.right - windowRect.left ) - long(( 2.6 * CFloat(( windowRect.right - windowRect.left ) / 100)));
	rcRect.right = rcRect.left + long( CFloat((2.6 * CFloat(( windowRect.right - windowRect.left ) / 100)) * aspectRatio));
	rcRect.top = previousColumn1 + long(26.5 * verticalOnePercent);
	rcRect.bottom = previousColumn1 + long(29.1 * verticalOnePercent);

	//ScreenToClient( &rcRect );
	m_btnRemoveScene.LoadBitmaps( IDB_BITMAP_DELETE_UP, IDB_BITMAP_DELETE_DOWN, IDB_BITMAP_DELETE_FOCUS, IDB_BITMAP_DELETE_DISABLE );
	m_btnRemoveScene.MoveWindow( rcRect );
	m_btnRemoveScene.EnableWindow( FALSE );
	m_btnRemoveScene.ShowWindow( SW_SHOW );
	m_btnRemoveScene.SetFont( &fnt2 );
	m_btnRemoveScene.UpdateWindow();

	previousLeft = rcRect.left;
	// Properties "Button" of scenes
	rcRect.right = previousLeft - long(m_horizontalPointFivePercent);
	rcRect.left = rcRect.right - long((2.6 * (windowRect.right - windowRect.left) / 100) * aspectRatio);
	rcRect.top = previousColumn1 + long(26.5 * verticalOnePercent);
	rcRect.bottom = previousColumn1 + long(29.1 * verticalOnePercent);

	//ScreenToClient( &rcRect );
	m_btnSceneProperties.LoadBitmaps( IDB_BITMAP_EDIT_UP, IDB_BITMAP_EDIT_DOWN, IDB_BITMAP_EDIT_FOCUS, IDB_BITMAP_EDIT_DISABLE );
	m_btnSceneProperties.MoveWindow( rcRect );
	m_btnSceneProperties.SetFont( &fnt2 );
	m_btnSceneProperties.EnableWindow( FALSE );
	m_btnSceneProperties.ShowWindow( SW_SHOW );
	m_btnSceneProperties.UpdateWindow();

	//==============================================

	//static text: list of objects
	rcRect.left = CInt( 100 * ( windowRect.right - windowRect.left ) / 100 ) - m_horizontalSizeOfRightCulomn + m_horizontalPointFivePercent;
	rcRect.right = CInt( 100 * ( windowRect.right - windowRect.left ) / 100 ) - m_horizontalPointFivePercent;
	rcRect.top = previousColumn2 + long(verticalOnePercent);
	rcRect.bottom = rcRect.top + long(2 * verticalOnePercent);

	//ScreenToClient( &rcRect );
	GetDlgItem(IDC_STATIC_OBJECTS)->MoveWindow(rcRect);
	GetDlgItem(IDC_STATIC_OBJECTS)->SetFont(&fnt2);
	GetDlgItem(IDC_STATIC_OBJECTS)->ShowWindow(SW_HIDE);
	GetDlgItem(IDC_STATIC_OBJECTS)->UpdateWindow();

	//ScreenToClient( &rcRect );
	GetDlgItem(IDC_STATIC_PREFAB_ELEMENTS)->MoveWindow(rcRect);
	GetDlgItem(IDC_STATIC_PREFAB_ELEMENTS)->SetFont(&fnt2);
	GetDlgItem(IDC_STATIC_PREFAB_ELEMENTS)->ShowWindow(SW_SHOW);
	GetDlgItem(IDC_STATIC_PREFAB_ELEMENTS)->UpdateWindow();

	//list control: COLLADA objects
	rcRect.left = CInt( 100 * ( windowRect.right - windowRect.left ) / 100 ) - m_horizontalSizeOfRightCulomn + 2 * m_horizontalPointFivePercent;
	rcRect.right = CInt( 100 * ( windowRect.right - windowRect.left ) / 100 ) - 2 * m_horizontalPointFivePercent;;
	rcRect.top = previousColumn2 + long(5 * verticalOnePercent);
	rcRect.bottom = rcRect.top + long(19.5 * verticalOnePercent);

	//ScreenToClient( &rcRect );

	m_listBoxObjects.MoveWindow( rcRect );
	m_listBoxObjects.SetBkColor( RGB( 40, 40, 40 ));
	m_listBoxObjects.SetTextBkColor( RGB( 40, 40, 40 ));
	m_listBoxObjects.SetFont( &fnt2 );
	m_listBoxObjects.SetTextColor(RGB(140, 140, 140));
	m_objectListImage.Create(16,16,ILC_COLOR24,10,5);
	CBitmap cBmp;
	CBitmap* cBmpMask = NULL;
	cBmp.LoadBitmap(IDB_BITMAP_OBJECTLIST_CAMERA);
	m_objectListImage.Add(&cBmp, cBmpMask);
	cBmp.DeleteObject();
	cBmp.LoadBitmap(IDB_BITMAP_OBJECTLIST_IMAGE);
	m_objectListImage.Add(&cBmp, cBmpMask);
	cBmp.DeleteObject();
	cBmp.LoadBitmap(IDB_BITMAP_OBJECTLIST_MATERIAL);
	m_objectListImage.Add(&cBmp, cBmpMask);
	cBmp.DeleteObject();
	cBmp.LoadBitmap(IDB_BITMAP_OBJECTLIST_MESH);
	m_objectListImage.Add(&cBmp, cBmpMask);
	cBmp.DeleteObject();
	cBmp.LoadBitmap(IDB_BITMAP_OBJECTLIST_ANIMATION);
	m_objectListImage.Add(&cBmp, cBmpMask);
	cBmp.DeleteObject();
	cBmp.LoadBitmap(IDB_BITMAP_OBJECTLIST_LIGHT);
	m_objectListImage.Add(&cBmp, cBmpMask);
	cBmp.DeleteObject();
	cBmp.LoadBitmap(IDB_BITMAP_OBJECTLIST_EFFECT);
	m_objectListImage.Add(&cBmp, cBmpMask);
	cBmp.DeleteObject();
	cBmp.LoadBitmap(IDB_BITMAP_OBJECTLIST_ANIMATIONCLIP);
	m_objectListImage.Add(&cBmp, cBmpMask);
	cBmp.DeleteObject();
	cBmp.LoadBitmap(IDB_BITMAP_OBJECTLIST_SKINCONTROLLER);
	m_objectListImage.Add(&cBmp, cBmpMask);
	cBmp.DeleteObject();
	cBmp.LoadBitmap(IDB_BITMAP_OBJECTLIST_SCENE);
	m_objectListImage.Add(&cBmp, cBmpMask);
	cBmp.DeleteObject();

	m_listBoxObjects.SetImageList( &m_objectListImage, LVSIL_SMALL );

	m_listBoxObjects.GetClientRect( &tempRect );

	m_listBoxObjects.InsertColumn(0, "Objects", LVCFMT_LEFT | LVS_REPORT | LVS_NOLABELWRAP, ( tempRect.right - tempRect.left ) * 80 / 100);
	m_listBoxObjects.ShowWindow( SW_SHOW );
	m_listBoxObjects.UpdateWindow();

	// Remove "Button" of COLLADA objects
	rcRect.left = (windowRect.right - windowRect.left) - long((2.6 * CFloat((windowRect.right - windowRect.left) / 100)));
	rcRect.right = rcRect.left + long(CFloat((2.6 * CFloat((windowRect.right - windowRect.left) / 100)) * aspectRatio));
	rcRect.top = previousColumn2 + long(26.5 * verticalOnePercent);
	rcRect.bottom = previousColumn2 + long(29.1 * verticalOnePercent);

	//ScreenToClient( &rcRect );
	m_btnRemoveObject.LoadBitmaps( IDB_BITMAP_DELETE_UP, IDB_BITMAP_DELETE_DOWN, IDB_BITMAP_DELETE_FOCUS, IDB_BITMAP_DELETE_DISABLE );
	m_btnRemoveObject.MoveWindow( rcRect );
	m_btnRemoveObject.SetFont( &fnt2 );
	m_btnRemoveObject.ShowWindow( SW_HIDE );
	m_btnRemoveObject.UpdateWindow();
	//==============================================

	//==============================================
	//static text: list of Engine objects
	rcRect.left = CInt( 100 * ( windowRect.right - windowRect.left ) / 100 ) - m_horizontalSizeOfRightCulomn + m_horizontalPointFivePercent;
	rcRect.right = CInt( 100 * ( windowRect.right - windowRect.left ) / 100 ) - m_horizontalPointFivePercent;
	rcRect.top = previousColumn3 + long(verticalOnePercent);
	rcRect.bottom = rcRect.top + long(2 * verticalOnePercent);

	//ScreenToClient( &rcRect );
	GetDlgItem( IDC_STATIC_ENGINE_OBJECTS )->MoveWindow( rcRect );
	GetDlgItem( IDC_STATIC_ENGINE_OBJECTS )->SetFont( &fnt2 );
	GetDlgItem( IDC_STATIC_ENGINE_OBJECTS )->ShowWindow( SW_SHOW );
	GetDlgItem( IDC_STATIC_ENGINE_OBJECTS )->UpdateWindow();

	//list control: Engine objetcs
	rcRect.left = CInt( 100 * ( windowRect.right - windowRect.left ) / 100 ) - m_horizontalSizeOfRightCulomn + 2 * m_horizontalPointFivePercent;
	rcRect.right = CInt( 100 * ( windowRect.right - windowRect.left ) / 100 ) - 2 * m_horizontalPointFivePercent;
	rcRect.top = previousColumn3 + long(5 * verticalOnePercent);
	rcRect.bottom = rcRect.top + long(19.5 * verticalOnePercent);
	//ScreenToClient( &rcRect );

	m_listBoxEngineObjects.MoveWindow( rcRect );
	m_listBoxEngineObjects.SetBkColor( RGB( 40, 40, 40 ));
	m_listBoxEngineObjects.SetTextBkColor( RGB( 40, 40, 40 ));
	m_listBoxEngineObjects.SetTextColor(RGB(140, 140, 140));

	m_engineObjectListImage.Create(16,16,ILC_COLOR24,6,6);
	cBmp.LoadBitmap(IDB_BITMAP_ENGINEOBJECTLIST_WATER);
	m_engineObjectListImage.Add(&cBmp, cBmpMask);
	cBmp.DeleteObject();
	cBmp.LoadBitmap(IDB_BITMAP_ENGINEOBJECTLIST_STATICSOUND);
	m_engineObjectListImage.Add(&cBmp, cBmpMask);
	cBmp.DeleteObject();
	cBmp.LoadBitmap(IDB_BITMAP_ENGINEOBJECTLIST_AMBIENTSOUND);
	m_engineObjectListImage.Add(&cBmp, cBmpMask);
	cBmp.DeleteObject();
	cBmp.LoadBitmap(IDB_BITMAP_ENGINEOBJECTLIST_SKY);
	m_engineObjectListImage.Add(&cBmp, cBmpMask);
	cBmp.DeleteObject();
	cBmp.LoadBitmap(IDB_BITMAP_ENGINEOBJECTLIST_LIGHT);
	m_engineObjectListImage.Add(&cBmp, cBmpMask);
	cBmp.DeleteObject();

	cBmp.LoadBitmap(IDB_BITMAP_PHYSXELEMENTLIST_TRIGGER);
	m_engineObjectListImage.Add(&cBmp, cBmpMask);
	cBmp.DeleteObject();


	m_listBoxEngineObjects.SetImageList( &m_engineObjectListImage, LVSIL_SMALL );

	//rcRect.right = CInt( 12.5 * ( windowRect.right - windowRect.left ) / 100);
	m_listBoxEngineObjects.GetClientRect( &tempRect );
	rcRect.left = tempRect.left;
	rcRect.right = tempRect.right;

	m_listBoxEngineObjects.InsertColumn(0, "Objects", LVCFMT_LEFT |  LVS_SHOWSELALWAYS, ( tempRect.right - tempRect.left ) * 80 / 100 );
	m_listBoxEngineObjects.ShowWindow( SW_SHOW );
	m_listBoxEngineObjects.SetFont( &fnt2 );
	m_listBoxEngineObjects.UpdateWindow();


	// Remove "Button" of engine objects
	rcRect.left = (windowRect.right - windowRect.left) - long((2.6 * CFloat((windowRect.right - windowRect.left) / 100)));
	rcRect.right = rcRect.left + long(CFloat((2.6 * CFloat((windowRect.right - windowRect.left) / 100)) * aspectRatio));
	rcRect.top = previousColumn3 + long(26.5 * verticalOnePercent);
	rcRect.bottom = previousColumn3 + long(29.1 * verticalOnePercent);

	//ScreenToClient( &rcRect );
	m_btnRemoveEngineObject.LoadBitmaps( IDB_BITMAP_DELETE_UP, IDB_BITMAP_DELETE_DOWN, IDB_BITMAP_DELETE_FOCUS, IDB_BITMAP_DELETE_DISABLE );
	m_btnRemoveEngineObject.MoveWindow( rcRect );
	m_btnRemoveEngineObject.SetFont( &fnt2 );
	m_btnRemoveEngineObject.EnableWindow( FALSE );
	m_btnRemoveEngineObject.ShowWindow( SW_SHOW );
	m_btnRemoveEngineObject.UpdateWindow();

	previousLeft = rcRect.left;
	// Properties "Button" of engine objects
	rcRect.right = previousLeft - long(m_horizontalPointFivePercent);
	rcRect.left = rcRect.right - long( (2.6 * ( windowRect.right - windowRect.left ) / 100) * aspectRatio);
	rcRect.top = previousColumn3 + long(26.5 * verticalOnePercent);
	rcRect.bottom = previousColumn3 + long(29.1 * verticalOnePercent);

	//ScreenToClient( &rcRect );
	m_btnEngineObjectProperties.LoadBitmaps( IDB_BITMAP_EDIT_UP, IDB_BITMAP_EDIT_DOWN, IDB_BITMAP_EDIT_FOCUS, IDB_BITMAP_EDIT_DISABLE );
	m_btnEngineObjectProperties.MoveWindow( rcRect );
	m_btnEngineObjectProperties.SetFont( &fnt2 );
	m_btnEngineObjectProperties.EnableWindow( FALSE );
	m_btnEngineObjectProperties.ShowWindow( SW_SHOW );
	m_btnEngineObjectProperties.UpdateWindow();
	
	//static text: list of PhysX Elements///////////////////
	rcRect.left = CInt( 100 * ( windowRect.right - windowRect.left ) / 100 ) - m_horizontalSizeOfRightCulomn + m_horizontalPointFivePercent;
	rcRect.right = CInt( 100 * ( windowRect.right - windowRect.left ) / 100 ) - m_horizontalPointFivePercent;
	rcRect.top = previousColumn3 + long(verticalOnePercent);
	rcRect.bottom = rcRect.top + long(2 * verticalOnePercent);

	//ScreenToClient( &rcRect );
	GetDlgItem( IDC_STATIC_PHYSX_ELEMENTS )->MoveWindow( rcRect );
	GetDlgItem( IDC_STATIC_PHYSX_ELEMENTS )->SetFont( &fnt2 );
	GetDlgItem( IDC_STATIC_PHYSX_ELEMENTS )->ShowWindow( SW_HIDE );
	GetDlgItem( IDC_STATIC_PHYSX_ELEMENTS )->UpdateWindow();

	//list control: PhysX Elements
	rcRect.left = CInt( 100 * ( windowRect.right - windowRect.left ) / 100 ) - m_horizontalSizeOfRightCulomn + 2 * m_horizontalPointFivePercent;
	rcRect.right = CInt( 100 * ( windowRect.right - windowRect.left ) / 100 ) - 2 * m_horizontalPointFivePercent;
	rcRect.top = previousColumn3 + long(5 * verticalOnePercent);
	rcRect.bottom = rcRect.top + long(19.5 * verticalOnePercent);
	//ScreenToClient( &rcRect );

	m_listBoxPhysXElements.MoveWindow( rcRect );
	m_listBoxPhysXElements.SetBkColor( RGB( 40, 40, 40 ));
	m_listBoxPhysXElements.SetTextBkColor( RGB( 40, 40, 40 ));
	m_listBoxPhysXElements.SetTextColor(RGB(140, 140, 140));
	m_listBoxPhysXElements.ShowWindow(SW_HIDE);

	m_listBoxPhysXElements.GetClientRect( &tempRect );
	rcRect.left = tempRect.left;
	rcRect.right = tempRect.right;

	m_physXElementListImage.Create(16,16,ILC_COLOR24,3,5);

	cBmp.LoadBitmap(IDB_BITMAP_PHYSXELEMENTLIST_DYNAMICRIGIDBODY);
	m_physXElementListImage.Add(&cBmp, cBmpMask);
	cBmp.DeleteObject();

	cBmp.LoadBitmap(IDB_BITMAP_PHYSXELEMENTLIST_STATICRIGIDBODY);
	m_physXElementListImage.Add(&cBmp, cBmpMask);
	cBmp.DeleteObject();

	cBmp.LoadBitmap(IDB_BITMAP_PHYSXELEMENTLIST_TRIGGER);
	m_physXElementListImage.Add(&cBmp, cBmpMask);
	cBmp.DeleteObject();

	m_listBoxPhysXElements.SetImageList( &m_physXElementListImage, LVSIL_SMALL );

	m_listBoxPhysXElements.InsertColumn(0, "Elements", LVCFMT_LEFT |  LVS_SHOWSELALWAYS, ( tempRect.right - tempRect.left ) * 80 / 100 );
	m_listBoxPhysXElements.ShowWindow( SW_HIDE );
	m_listBoxPhysXElements.SetFont( &fnt2 );
	m_listBoxPhysXElements.UpdateWindow();

	// Remove "Button" of PhysX elements
	rcRect.left = (windowRect.right - windowRect.left) - long((2.6 * CFloat((windowRect.right - windowRect.left) / 100)));
	rcRect.right = rcRect.left + long(CFloat((2.6 * CFloat((windowRect.right - windowRect.left) / 100)) * aspectRatio));
	rcRect.top = previousColumn3 + long(26.5 * verticalOnePercent);
	rcRect.bottom = previousColumn3 + long(29.1 * verticalOnePercent);

	//ScreenToClient( &rcRect );
	m_btnRemovePhysX.LoadBitmaps( IDB_BITMAP_DELETE_UP, IDB_BITMAP_DELETE_DOWN, IDB_BITMAP_DELETE_FOCUS, IDB_BITMAP_DELETE_DISABLE );
	m_btnRemovePhysX.MoveWindow( rcRect );
	m_btnRemovePhysX.SetFont( &fnt2 );
	m_btnRemovePhysX.EnableWindow( FALSE );
	m_btnRemovePhysX.ShowWindow( SW_HIDE );
	m_btnRemovePhysX.UpdateWindow();

	////////////////////////

	//Initialize rich edits///////////////
	//CRect rcRichRect;
	CFont richFnt;
	//ScreenToClient( &rcRect );
	rcRect.top = long( 1 * ( windowRect.top - windowRect.bottom ) / 100);
	rcRect.bottom = long( 3.5 * ( windowRect.top - windowRect.bottom ) / 100);
	//ScreenToClient( &rcRect );
	CInt fontSizeY2 = rcRect.bottom - rcRect.top; 
	richFnt.CreateFont(-fontSizeY2, 0, 0, 0, FW_NORMAL, FALSE, FALSE, FALSE, ANSI_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, PROOF_QUALITY, DEFAULT_PITCH, "Consolas");

	//Initialze the rich edit here
	rcRect.left = CInt(3.9 * (windowRect.right - windowRect.left) / 100);
	rcRect.top = CInt( 96.8 * ( windowRect.bottom - windowRect.top ) / 100 );
	rcRect.right =	CInt( 44.0 * ( windowRect.right - windowRect.left ) / 100);
	rcRect.bottom = CInt( 98.8 * ( windowRect.bottom - windowRect.top ) / 100 );
	//ScreenToClient( &rcRect );

	m_rich.MoveWindow( rcRect );
	m_rich.SetFont( &fnt2/*richFnt*/ );
	m_rich.SetBackgroundColor( FALSE, RGB(40,40,40));
	m_rich.ShowWindow( SW_SHOW );
	m_rich.UpdateWindow();

	//selected object
	rcRect.left = CInt( 45.0 * ( windowRect.right - windowRect.left ) / 100);
	rcRect.right = CInt( 65.0 * ( windowRect.right - windowRect.left ) / 100);
	rcRect.top = CInt( 96.8 * ( windowRect.bottom - windowRect.top ) / 100 );
	rcRect.bottom = CInt( 98.8 * ( windowRect.bottom - windowRect.top ) / 100 );

	m_staticSelectedObject.MoveWindow( rcRect );
	m_staticSelectedObject.SetFont( &fnt2/*richFnt*/ );
	m_staticSelectedObject.SetBackgroundColor( FALSE, RGB(40,40,40) );
	m_staticSelectedObject.ShowWindow( SW_SHOW );
	m_staticSelectedObject.UpdateWindow();

	//transformation area
	//X
	rcRect.left = CInt(66.0 * (windowRect.right - windowRect.left) / 100);
	rcRect.right = CInt(67.0 * (windowRect.right - windowRect.left) / 100);
	rcRect.top = CInt(96.8 * (windowRect.bottom - windowRect.top) / 100);
	rcRect.bottom = CInt(98.8 * (windowRect.bottom - windowRect.top) / 100);

	GetDlgItem(IDC_STATIC_X)->MoveWindow(rcRect);
	GetDlgItem(IDC_STATIC_X)->SetFont(&fnt2);
	GetDlgItem(IDC_STATIC_X)->ShowWindow(SW_SHOW);
	GetDlgItem(IDC_STATIC_X)->UpdateWindow();

	rcRect.left = CInt(67.7 * (windowRect.right - windowRect.left) / 100);
	rcRect.right = CInt(71.5 * (windowRect.right - windowRect.left) / 100);
	rcRect.top = CInt(96.7 * (windowRect.bottom - windowRect.top) / 100);
	rcRect.bottom = CInt(99.2 * (windowRect.bottom - windowRect.top) / 100);

	m_editX.MoveWindow(rcRect);
	m_editX.SetBackgroundColor(FALSE, RGB(40, 40, 40));
	m_editX.SetFont(&fnt2);
	m_editX.ShowWindow(SW_SHOW);
	m_editX.SetEventMask(ENM_CHANGE);
	m_editX.UpdateWindow();

	//Y
	rcRect.left = CInt(72.5 * (windowRect.right - windowRect.left) / 100);
	rcRect.right = CInt(73.5 * (windowRect.right - windowRect.left) / 100);
	rcRect.top = CInt(96.8 * (windowRect.bottom - windowRect.top) / 100);
	rcRect.bottom = CInt(98.8 * (windowRect.bottom - windowRect.top) / 100);

	GetDlgItem(IDC_STATIC_Y)->MoveWindow(rcRect);
	GetDlgItem(IDC_STATIC_Y)->SetFont(&fnt2);
	GetDlgItem(IDC_STATIC_Y)->ShowWindow(SW_SHOW);
	GetDlgItem(IDC_STATIC_Y)->UpdateWindow();

	rcRect.left = CInt(74.0 * (windowRect.right - windowRect.left) / 100);
	rcRect.right = CInt(77.8 * (windowRect.right - windowRect.left) / 100);
	rcRect.top = CInt(96.7 * (windowRect.bottom - windowRect.top) / 100);
	rcRect.bottom = CInt(99.2 * (windowRect.bottom - windowRect.top) / 100);

	m_editY.MoveWindow(rcRect);
	m_editY.SetBackgroundColor(FALSE, RGB(40, 40, 40));
	m_editY.SetFont(&fnt2);
	m_editY.SetEventMask(ENM_CHANGE);
	m_editY.ShowWindow(SW_SHOW);
	m_editY.UpdateWindow();

	//Z
	rcRect.left = CInt(78.8 * (windowRect.right - windowRect.left) / 100);
	rcRect.right = CInt(79.8 * (windowRect.right - windowRect.left) / 100);
	rcRect.top = CInt(96.8 * (windowRect.bottom - windowRect.top) / 100);
	rcRect.bottom = CInt(98.8 * (windowRect.bottom - windowRect.top) / 100);

	GetDlgItem(IDC_STATIC_Z)->MoveWindow(rcRect);
	GetDlgItem(IDC_STATIC_Z)->SetFont(&fnt2);
	GetDlgItem(IDC_STATIC_Z)->ShowWindow(SW_SHOW);
	GetDlgItem(IDC_STATIC_Z)->UpdateWindow();

	rcRect.left = CInt(80.3 * (windowRect.right - windowRect.left) / 100);
	rcRect.right = CInt(84.1 * (windowRect.right - windowRect.left) / 100);
	rcRect.top = CInt(96.7 * (windowRect.bottom - windowRect.top) / 100);
	rcRect.bottom = CInt(99.2 * (windowRect.bottom - windowRect.top) / 100);

	m_editZ.MoveWindow(rcRect);
	m_editZ.SetBackgroundColor(FALSE, RGB(40, 40, 40));
	m_editZ.SetFont(&fnt2);
	m_editZ.ShowWindow(SW_SHOW);
	m_editZ.SetEventMask(ENM_CHANGE);
	m_editZ.UpdateWindow();
	richFnt.Detach();
	//===========================================
	fnt2.Detach();

	GetMenu()->EnableMenuItem(ID_TOOLS_CULLFACES, MF_DISABLED | MF_GRAYED);
	GetMenu()->EnableMenuItem(ID_TOOLS_MATERIALEDITOR, MF_DISABLED | MF_GRAYED);
	GetMenu()->EnableMenuItem(ID_TOOLS_PHYSXEDITOR, MF_DISABLED | MF_GRAYED);
	GetMenu()->EnableMenuItem(ID_TOOLS_SCRIPTMANAGER, MF_DISABLED | MF_GRAYED);
	GetMenu()->EnableMenuItem(ID_TOOLS_IMPORTOPTIONS, MF_DISABLED | MF_GRAYED);
	GetMenu()->EnableMenuItem(ID_GEOMETRY_AMBIENTCOLOR, MF_DISABLED | MF_GRAYED);
	GetMenu()->EnableMenuItem(ID_TOOLS_GEOMETRYBASEDSELECTION, MF_DISABLED | MF_GRAYED);
	GetMenu()->EnableMenuItem(ID_IMPORT_COLLADA, MF_DISABLED | MF_GRAYED);


	//intitialize OpenGL window

	CRect rcClient;
	GetClientRect( &rcClient );

	rcClient.left = m_horizontalSizeOfLeftCulomn + m_horizontalPointFivePercent;
	rcClient.top = CInt( 6 * ( windowRect.bottom - windowRect.top ) / 100 );
	rcClient.right = CInt( 100 * ( windowRect.right - windowRect.left ) / 100 ) - m_horizontalSizeOfRightCulomn;
	rcClient.bottom = CInt(95.0 * ( windowRect.bottom - windowRect.top ) / 100 );

	//Create the OpenGL window here
	g_width = rcClient.right - rcClient.left;
	g_height = rcClient.bottom - rcClient.top;


	//Create the OpenGLw window here
	g_multipleView = CNew( CMultipleWindows );
	if (!g_multipleView->Create(NULL, NULL, WS_VISIBLE | WS_CHILD , rcClient, this, 0))
	{
		PostQuitMessage(0);
		return FALSE;
	}

	g_glUtil.InitGLEW();
	//if (!glewIsSupported("GL_VERSION_3_0"))
	//{
		//MessageBox( "Your implementation does not support OpenGL 3.3\nVanda may crash or you may not be able to use all the features of Vanda\nUpdating your graphics driver may fix this problem" );
	//}
	//ilInit();
	//iluInit();
	if(GLEW_ARB_color_buffer_float)
	{
		glClampColorARB( GL_CLAMP_VERTEX_COLOR_ARB, GL_FALSE );
		glClampColorARB( GL_CLAMP_FRAGMENT_COLOR_ARB, GL_FALSE );
	}

	if( !GLEW_EXT_framebuffer_object || !GLEW_ARB_texture_non_power_of_two || !g_options.m_enableFBO)
	{
		PrintInfo( "\nSwitching to old rendering style", COLOR_RED );
		g_useOldRenderingStyle = CTrue;
	}
	if( !g_useOldRenderingStyle )
	{
		if( !GLEW_EXT_framebuffer_multisample )
		{
			PrintInfo("\nGL_EXT_framebuffer_multisample is not supported\nSwitching to old rendering style");
			g_useOldRenderingStyle = CTrue; //See if we can we use multisampling with old rendering style?
		}
		else
			g_multipleView->m_multiSample = true; //use new rendering style with multisampling
	}
	if( g_useOldRenderingStyle && !WGLEW_ARB_multisample  )
			PrintInfo("\nYour implementation doesn't support multisampling");

	if( WGLEW_ARB_multisample && g_useOldRenderingStyle )
	{
		g_multipleView->DestroyWindow();
		g_multipleView->m_multiSample = true;
		if (!g_multipleView->Create(NULL, NULL, WS_BORDER | WS_VISIBLE | WS_CHILD , rcClient, this, 0))
		{
			PrintInfo( "\nWarning! Couldn't create an OpenGL window with multisampling", COLOR_YELLOW );
			g_multipleView->DestroyWindow();
			g_multipleView->m_multiSample = false;
			if (!g_multipleView->Create(NULL, NULL, WS_BORDER | WS_VISIBLE | WS_CHILD  , rcClient, this, 0))
			{
				PrintInfo( "\nWarning! Couldn't create an OpenGL window after multisampling faild", COLOR_YELLOW );

				PostQuitMessage(0);
				return FALSE;
			}
		}
	}

	//PrintInfo GL info here
	g_glUtil.GetGLInfo();
	if( WGLEW_EXT_swap_control )
			wglSwapIntervalEXT(0);

	g_render.Init();

	if( !g_multipleView->InitAll() )
	{
		MessageBox( "fatal error(s) occured. Please fix the error(s)", "Vanda Engine 1 Error", MB_OK );
	}

	if( g_multipleView->m_multiSample && GLEW_NV_multisample_filter_hint)
		glHint(GL_MULTISAMPLE_FILTER_HINT_NV,GL_NICEST);

	if( WGLEW_EXT_swap_control && g_options.m_disableVSync )
	{
		wglSwapIntervalEXT(0);
	}
	else
		wglSwapIntervalEXT(1);

	GetMenu()->EnableMenuItem( ID_MODIFY_BLOOM, MF_DISABLED | MF_GRAYED );
	g_render.m_useBloom = CFalse;

	if( GLEW_ARB_texture_non_power_of_two && g_render.UsingFBOs() && g_options.m_enableFBO && g_render.UsingShader() )
	{
		GetMenu()->EnableMenuItem( ID_MODIFY_BLOOM, MF_ENABLED );
		g_bloom = CNew( CBloom );
		g_bloom->CreateEmptyTexture( g_width, g_height, 3, GL_RGB );
		g_render.m_useBloom = CTrue;
	}
	if( g_useOldRenderingStyle )
	{
		GetMenu()->EnableMenuItem( ID_MODIFY_DOF, MF_GRAYED | MF_DISABLED );
		GetMenu()->EnableMenuItem( ID_MODIFY_BLOOM, MF_GRAYED | MF_DISABLED );
	}

	if( !g_useOldRenderingStyle )
		g_multipleView->InitFBOs( GL_RGBA, GL_RGBA );

	if( g_render.UsingShader() )
		g_render.m_useWaterReflection = CTrue;
	else
		g_render.m_useWaterReflection = CFalse;

	//configuration
	FILE *filePtr;
	CChar ConfigPath[MAX_NAME_SIZE];
	HRESULT doc_result = SHGetFolderPath(NULL, CSIDL_PERSONAL, NULL, SHGFP_TYPE_CURRENT, ConfigPath);
	if (doc_result != S_OK)
	{
		PrintInfo("\nCouldn't get the documents folder to write data", COLOR_RED);
	}
	else
	{
		Append(ConfigPath, "/Vanda/conf_editor.dat");
	}

	filePtr = fopen(ConfigPath, "rb");
	if (!filePtr)
	{
		CChar RTIPath[MAX_NAME_SIZE];
		SHGetFolderPath(NULL, CSIDL_PERSONAL, NULL, SHGFP_TYPE_CURRENT, RTIPath);
		Append(RTIPath, "/Vanda/");
		CreateWindowsDirectory(RTIPath);

		filePtr = fopen(ConfigPath, "wb");
		fwrite(&g_options, sizeof(COptions), 1, filePtr);
	}
	else
	{
		PrintInfo("Engine options:");
		fread(&g_options, sizeof(COptions), 1, filePtr);
		if (g_options.m_enableShader)
			PrintInfo("\nShader: ON");
		else
			PrintInfo("\nShader: OFF", COLOR_RED);

		if (g_options.m_numSamples)
			PrintInfo("\nMultisampling: ON");
		else
			PrintInfo("\nMultisampling: OFF", COLOR_RED);

		if (g_options.m_anisotropy)
			PrintInfo("\nAnisotropic Texture Filtering: ON");
		else
			PrintInfo("\nAnisotropic Texture Filtering: OFF", COLOR_RED);

		if (g_options.m_disableVSync)
			PrintInfo("\nVSync: OFF");
		else
			PrintInfo("\nVSync: ON");

		if (g_options.m_enableFBO)
			PrintInfo("\nFBO: ON");
		else
			PrintInfo("\nFBO: OFF", COLOR_RED);

		if (g_options.m_enableVBO)
			PrintInfo("\nVBO: ON");
		else
			PrintInfo("\nVBO: OFF", COLOR_RED);

		if (g_options.m_enableWaterReflection)
			PrintInfo("\nWater Reflection: ON");
		else
			PrintInfo("\nWater Reflection: OFF", COLOR_RED);

	}
	fclose(filePtr);
	////////////////

	//PackagePath path
	CChar PackageFilePath[MAX_NAME_SIZE];
	HRESULT doc_result_package = SHGetFolderPath(NULL, CSIDL_PERSONAL, NULL, SHGFP_TYPE_CURRENT, PackageFilePath);
	if (doc_result_package != S_OK)
	{
		PrintInfo("\nCouldn't get the documents folder to write data", COLOR_RED);
	}
	else
	{
		Append(PackageFilePath, "/Vanda/Packages/package_editor.dat");
	}
	filePtr = fopen(PackageFilePath, "rb");
	if (!filePtr)
	{
		CChar PackagePath[MAX_NAME_SIZE];
		SHGetFolderPath(NULL, CSIDL_PERSONAL, NULL, SHGFP_TYPE_CURRENT, PackagePath);
		Append(PackagePath, "/Vanda/Packages/");
		CreateWindowsDirectory(PackagePath);
		Append(PackagePath, "/PKG/");
		CreateWindowsDirectory(PackagePath);

		filePtr = fopen(PackageFilePath, "wb");
		CUInt packageSize = g_prefabPackagesAndNames.size();
		fwrite(&packageSize, sizeof(CUInt), 1, filePtr);
		fclose(filePtr);
	}
	else
	{
		//read prefab data
		CUInt packageSize;
		fread(&packageSize, sizeof(CUInt), 1, filePtr);
		fclose(filePtr);

		CChar PackagePath[MAX_NAME_SIZE];
		SHGetFolderPath(NULL, CSIDL_PERSONAL, NULL, SHGFP_TYPE_CURRENT, PackagePath);
		Append(PackagePath, "/Vanda/Packages/PKG/");

		CUInt index = 0;
		HANDLE hFind;
		WIN32_FIND_DATA data;
		CChar path[MAX_NAME_SIZE];
		sprintf(path, "%s%s", PackagePath, "*.*");
		hFind = FindFirstFile(path, &data);
		do
		{
			if (!Cmp(data.cFileName, ".") && !Cmp(data.cFileName, ".."))
			{
				CChar filePath[MAX_NAME_SIZE];
				sprintf(filePath, "%s%s", PackagePath, data.cFileName);

				CChar onlyFileName[MAX_NAME_SIZE];
				Cpy(onlyFileName, filePath);
				GetAfterPath(onlyFileName);
				GetWithoutDot(onlyFileName);

				filePtr = fopen(filePath, "rb");
				g_prefabPackagesAndNames.push_back(std::vector<std::string>());
				CUInt row_size;
				fread(&row_size, sizeof(CUInt), 1, filePtr);

				for (CUInt j = 0; j < row_size; j++)
				{
					CChar name[MAX_NAME_SIZE];
					fread(&name, sizeof(CChar), MAX_NAME_SIZE, filePtr);
					g_prefabPackagesAndNames[index].push_back(name);
				}
				fclose(filePtr);
				index++;
			}
		} while (FindNextFile(hFind, &data));

		FindClose(hFind);

		for (CUInt i = 0; i < g_prefabPackagesAndNames.size(); i++)
		{
			for (CUInt j = 0; j < g_prefabPackagesAndNames[i].size(); j++)
			{
				if (j == 0) continue;
				CChar packageNameAndPrefabName[MAX_NAME_SIZE];
				sprintf(packageNameAndPrefabName, "%s%s%s", g_prefabPackagesAndNames[i].front().c_str(), "_", g_prefabPackagesAndNames[i][j].c_str());
				g_allPrefabNames.push_back(packageNameAndPrefabName);
			}
		}

	}
	fclose(filePtr);
	//////////////////

	GetMenu()->CheckMenuItem( ID_TOOLS_CULLFACES, MF_UNCHECKED );
	GetMenu()->EnableMenuItem( ID_TOOLS_CULLFACES, MF_GRAYED );

	GetMenu()->CheckMenuItem( ID_TOOLS_GEOMETRYBASEDSELECTION, MF_CHECKED );

	GetMenu()->CheckMenuItem( ID_VIEW_LIGHT_POSITIONS, MF_CHECKED );
	GetMenu()->CheckMenuItem( ID_VIEW_SOUND_POSITIONS, MF_CHECKED );
	GetMenu()->CheckMenuItem( ID_ICONS_WATERICONS, MF_CHECKED );
	GetMenu()->CheckMenuItem( ID_ICONS_SKYICON, MF_CHECKED );
	GetMenu()->CheckMenuItem( ID_VIEW_PERSPECTIVE_GRIDS, MF_CHECKED );
	GetMenu()->CheckMenuItem( ID_VIEW_SHADOWDEPTHMAP, MF_UNCHECKED );
	GetMenu()->CheckMenuItem( ID_VIEW_STATISTICS, MF_UNCHECKED );
	GetMenu()->CheckMenuItem( ID_ICONS_CAMERAICONS, MF_CHECKED );

	GetMenu()->CheckMenuItem( ID_VIEW_BOUNDINGBOX, MF_UNCHECKED );
	GetMenu()->CheckMenuItem( ID_VIEW_OCTREE, MF_UNCHECKED );

	GetMenu()->CheckMenuItem( ID_VIEW_ALLCHANNELS, MF_CHECKED );

	m_askRemoveEngineObject = CTrue;

	//save functions///////////////////////////////////////
	//Get Document Directory
	HRESULT result = SHGetFolderPath(NULL, CSIDL_PERSONAL, NULL, SHGFP_TYPE_CURRENT, g_projectsPath);
	if (result != S_OK)
	{
        PrintInfo( "\nCouldn't get the documents folder to write data", COLOR_RED );
	}
	else
	{
		Append( g_projectsPath, "/Vanda/Projects/" );
	}

	Cpy( g_currentVSceneName, "\n" ); 
	FILE *ProjectsFilePtr;
	CChar DATPath[MAX_NAME_SIZE];
	sprintf( DATPath, "%s%s", g_projectsPath, "projects.dat" );
	ProjectsFilePtr = fopen( DATPath, "rb" );
	if(!ProjectsFilePtr )
	{
		CInt numProjects = 1;
		CInt numScenes = 0;
		CBool isActive = CTrue;

		PrintInfo( "\n'projects.dat' created successfully" );
		CreateWindowsDirectory( g_projectsPath );
		CChar defaultProject[MAX_NAME_SIZE];
		CChar PRJ[MAX_NAME_SIZE];
		Cpy( defaultProject, g_projectsPath );
		Append( defaultProject, "/default/" );
		CreateWindowsDirectory( defaultProject ); //default project directory
		Cpy(PRJ, g_projectsPath);
		Append(PRJ, "/PRJ/");
		CreateWindowsDirectory(PRJ); //default project directory

		ProjectsFilePtr = fopen(DATPath, "wb");

		CChar defaultProjectName[MAX_NAME_SIZE];
		//one default active project without any scenes
		Cpy(defaultProjectName, "default" );
		fwrite( &numProjects, sizeof( CInt ), 1, ProjectsFilePtr  );
		fclose(ProjectsFilePtr);
		CChar defaultPath[MAX_NAME_SIZE];
		sprintf(defaultPath, "%s%s%s", PRJ, defaultProjectName, ".prj");
		fopen(defaultPath, "wb");
		fwrite( defaultProjectName, sizeof( CChar ), MAX_NAME_SIZE, ProjectsFilePtr );
		fwrite( &numScenes, sizeof( CInt ), 1, ProjectsFilePtr  );
		fwrite( &isActive, sizeof( CBool ), 1, ProjectsFilePtr  );

		fclose( ProjectsFilePtr );

		CProjects* project = CNew(CProjects);
		Cpy(project->m_name, "default");
		project->m_isActive = CTrue;
		g_projects.push_back(project);
	}
	else
	{
		CInt numProjects;
		fread(&numProjects, sizeof(CInt), 1, ProjectsFilePtr);
		fclose(ProjectsFilePtr);

		HANDLE hFind;
		WIN32_FIND_DATA data;
		CChar path[MAX_NAME_SIZE];
		sprintf(path, "%s%s%s", g_projectsPath, "/PRJ/", "*.*");
		hFind = FindFirstFile(path, &data);
		do
		{
			if (!Cmp(data.cFileName, ".") && !Cmp(data.cFileName, ".."))
			{
				CChar filePath[MAX_NAME_SIZE];
				sprintf(filePath, "%s%s%s", g_projectsPath, "/PRJ/", data.cFileName);

				CChar onlyFileName[MAX_NAME_SIZE];
				Cpy(onlyFileName, filePath);
				GetAfterPath(onlyFileName);
				GetWithoutDot(onlyFileName);

				ProjectsFilePtr = fopen(filePath, "rb");

				CProjects* project = CNew(CProjects);

				CChar projectName[MAX_NAME_SIZE];
				fread(projectName, sizeof(CChar), MAX_NAME_SIZE, ProjectsFilePtr);
				Cpy(project->m_name, projectName);
				CInt numScenes;
				fread(&numScenes, sizeof(CInt), 1, ProjectsFilePtr);
				for (CInt j = 0; j < numScenes; j++)
				{
					CChar vsceneName[MAX_NAME_SIZE];
					fread(vsceneName, sizeof(CChar), MAX_NAME_SIZE, ProjectsFilePtr);
					project->m_sceneNames.push_back(vsceneName);
				}
				CBool isActive;
				fread(&isActive, sizeof(CBool), 1, ProjectsFilePtr);
				project->m_isActive = isActive;

				g_projects.push_back(project);

				fclose(ProjectsFilePtr);
			}
		} while (FindNextFile(hFind, &data));

		FindClose(hFind);
	}

	for( CUInt i = 0; i < g_projects.size(); i++ )
	{
		if( g_projects[i]->m_isActive )
		{
			for( CUInt j = 0; j < g_projects[i]->m_sceneNames.size(); j++ )
			{
				g_VSceneNamesOfCurrentProject.push_back( g_projects[i]->m_sceneNames[j].c_str() ); //VScene names of current project
			}
			Cpy( g_currentProjectPath, g_projectsPath );
			Append( g_currentProjectPath, g_projects[i]->m_name ); //current project path
			Append( g_currentProjectPath, "/" );

			CChar m_currentVSceneNameWithoutDot[MAX_NAME_SIZE];
			if (Cmp(g_currentVSceneName, "\n"))
				Cpy(m_currentVSceneNameWithoutDot, "Untitled");
			else
			{
				Cpy(m_currentVSceneNameWithoutDot, g_currentVSceneName);
				GetWithoutDot(m_currentVSceneNameWithoutDot);
			}

			CChar temp[256];
			sprintf(temp, "%s%s%s%s%s", "Vanda Engine 1.4 (", g_projects[i]->m_name, " - ", m_currentVSceneNameWithoutDot, ")");
			ex_pVandaEngine1Dlg->SetWindowTextA(temp);

			break;
		}
	}

	//save functions///////////////////////////////////
	if(!g_vandaDemo)
	{
		g_shadowProperties.m_enable = CTrue;
	}
	g_sceneBanner.SetBannerPath("Assets/Engine/Textures/Loading.dds");
    srand(time(NULL));
	//initialize lua/////////////////
	LuaOpenLibs(g_lua);
	lua_register(g_lua, "PlaySoundLoop", PlaySoundLoop);
	lua_register(g_lua, "PlaySoundOnce", PlaySoundOnce);
	lua_register(g_lua, "PauseSound", PauseSound);
	lua_register(g_lua, "StopSound", StopSound);
	lua_register(g_lua, "BlendCycle", BlendCycle);
	lua_register(g_lua, "ClearCycle", ClearCycle);
	lua_register(g_lua, "ExecuteAction", ExecuteAction);
	lua_register(g_lua, "ReverseExecuteAction", ReverseExecuteAction);
	lua_register(g_lua, "LoadVScene", LoadVScene);
	lua_register(g_lua, "ActivateCamera", ActivateCamera);

	////////////////////////////////

	//Remove desktop.ini read only flag. (used for Uninstall)
	//SetFileAttributes("C:/Users/Public/Desktop/desktop.ini", GetFileAttributes( "C:/Users/Public/Desktop/desktop.ini" ) & ~FILE_ATTRIBUTE_READONLY );
	PrintInfo( "\nVersion 1.4 initialized successfully" );
	//CAboutDlg dlgAbout;
	//dlgAbout.DoModal();
	ReleaseCapture();
	if( !g_openVINFile && g_options.m_showStartupDialog )
	{
		if( g_multipleView->m_enableTimer )
			g_multipleView->EnableTimer( CFalse );

		m_dlgWelcome.DoModal();

		if( g_multipleView->m_enableTimer )
			g_multipleView->EnableTimer( CTrue );
	}
	if( g_importCOLLADA )
	{
		OnMenuClickedImportCollada();
	}
	else if( g_openVINFile )
	{
		OnMenuClickedOpenVScene(CFalse);
	}
	SetDialogData3(CFalse);

	return TRUE;  // return TRUE  unless you set the focus to a control
}

CVoid CVandaEngine1Dlg::OnSysCommand(UINT nID, LPARAM lParam)
{

	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialog::OnSysCommand(nID, lParam);
	}
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

CVoid CVandaEngine1Dlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // device context for painting

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// Center icon in client rectangle
		CInt cxIcon = GetSystemMetrics(SM_CXICON);
		CInt cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		CInt x = (rect.Width() - cxIcon + 1) / 2;
		CInt y = (rect.Height() - cyIcon + 1) / 2;

		// Draw the icon
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialog::OnPaint();
	}
}

// The system calls this function to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CVandaEngine1Dlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

BOOL CVandaEngine1Dlg::OnCommand(WPARAM wParam, LPARAM lParam)
{
	// TODO: Add your specialized code here and/or call the base class
	if (wParam == ID_EDIT_TRANSLATE)
	{
		OnBnClickedBtnTranslate();
	}
	else if (wParam == ID_EDIT_ROTATE)
	{
		OnBnClickedBtnRotate();
	}
	else if (wParam == ID_EDIT_SCALE)
	{
		OnBnClickedBtnScale();
	}
	else if (wParam == ID_MODE_VSCENE)
	{
		if (g_editorMode != eMODE_VSCENE)
		{
			if (g_multipleView->m_enableTimer)
				g_multipleView->EnableTimer(CFalse);
			if (!OnMenuClickedNew(CTrue)) //ask to see if we should proceed?
				return FALSE;
			GetMenu()->CheckMenuItem(ID_MODE_VSCENE, MF_CHECKED);
			GetMenu()->CheckMenuItem(ID_MODE_PREFAB, MF_UNCHECKED);

			GetDlgItem(IDC_STATIC_SCENES)->ShowWindow(SW_HIDE);
			GetDlgItem(IDC_STATIC_SCENES)->UpdateWindow();
			GetDlgItem(IDC_STATIC_OBJECTS)->ShowWindow(SW_HIDE);
			GetDlgItem(IDC_STATIC_OBJECTS)->UpdateWindow();
			GetDlgItem(IDC_STATIC_PREFABS)->ShowWindow(SW_SHOW);
			GetDlgItem(IDC_STATIC_PREFABS)->UpdateWindow();
			GetDlgItem(IDC_STATIC_PREFAB_ELEMENTS)->ShowWindow(SW_SHOW);
			GetDlgItem(IDC_STATIC_PREFAB_ELEMENTS)->UpdateWindow();
			g_editorMode = eMODE_VSCENE;
			SortButtons();
			g_shareGeometriesBetweenScenes = CTrue;

			for (CUInt i = 0; i < g_projects.size(); i++)
			{
				if (g_projects[i]->m_isActive)
				{
					CChar m_currentVSceneNameWithoutDot[MAX_NAME_SIZE];
					if (Cmp(g_currentVSceneName, "\n"))
						Cpy(m_currentVSceneNameWithoutDot, "Untitled");
					else
					{
						Cpy(m_currentVSceneNameWithoutDot, g_currentVSceneName);
						GetWithoutDot(m_currentVSceneNameWithoutDot);
					}

					CChar temp[256];
					sprintf(temp, "%s%s%s%s%s", "Vanda Engine 1.4 (", g_projects[i]->m_name, " - ", m_currentVSceneNameWithoutDot, ")");
					ex_pVandaEngine1Dlg->SetWindowTextA(temp);
					break;
				}
			}

			if (g_currentCameraType == eCAMERA_DEFAULT_PHYSX)
				PrintInfo("\nPlay mode disabled");

			g_currentCameraType = eCAMERA_DEFAULT_FREE;
			m_mainBtnTestActive.ShowWindow(SW_HIDE);
			m_mainBtnTestActive.EnableWindow(FALSE);
			m_mainBtnTestActive.UpdateWindow();
			m_mainBtnTestDeactive.ShowWindow(SW_SHOW);
			m_mainBtnTestDeactive.EnableWindow(TRUE);
			m_mainBtnTestDeactive.UpdateWindow();
			SetDialogData3(CFalse);
			PrintInfo("\nVScene mode activated");
			if (g_multipleView->m_enableTimer)
				g_multipleView->EnableTimer(CTrue);
			g_multipleView->SetElapsedTimeFromBeginning();
			g_multipleView->RenderWindow();
		}
	}
	else if (wParam == ID_MODE_PREFAB)
	{
		if (g_editorMode != eMODE_PREFAB)
		{
			if (!OnMenuClickedNew(CTrue)) //ask to see if we should proceed?
				return FALSE;
			GetMenu()->CheckMenuItem(ID_MODE_VSCENE, MF_UNCHECKED);
			GetMenu()->CheckMenuItem(ID_MODE_PREFAB, MF_CHECKED);

			if (g_multipleView->m_enableTimer)
				g_multipleView->EnableTimer(CFalse);
			GetDlgItem(IDC_STATIC_PREFABS)->ShowWindow(SW_HIDE);
			GetDlgItem(IDC_STATIC_PREFABS)->UpdateWindow();
			GetDlgItem(IDC_STATIC_PREFAB_ELEMENTS)->ShowWindow(SW_HIDE);
			GetDlgItem(IDC_STATIC_PREFAB_ELEMENTS)->UpdateWindow();
			GetDlgItem(IDC_STATIC_SCENES)->ShowWindow(SW_SHOW);
			GetDlgItem(IDC_STATIC_SCENES)->UpdateWindow();
			GetDlgItem(IDC_STATIC_OBJECTS)->ShowWindow(SW_SHOW);
			GetDlgItem(IDC_STATIC_OBJECTS)->UpdateWindow();
			g_editorMode = eMODE_PREFAB;
			SortButtons();
			g_shareGeometriesBetweenScenes = CFalse; 

			CChar temp[256];
			sprintf(temp, "%s", "Vanda Engine 1.4 : Prefab Mode (Untitled)");
			ex_pVandaEngine1Dlg->SetWindowTextA(temp);

			if (g_currentCameraType == eCAMERA_DEFAULT_PHYSX)
				PrintInfo("\nPlay mode disabled");

			g_currentCameraType = eCAMERA_DEFAULT_FREE;
			m_mainBtnTestActive.ShowWindow(SW_HIDE);
			m_mainBtnTestActive.EnableWindow(FALSE);
			m_mainBtnTestActive.UpdateWindow();
			m_mainBtnTestDeactive.ShowWindow(SW_SHOW);
			m_mainBtnTestDeactive.EnableWindow(TRUE);
			m_mainBtnTestDeactive.UpdateWindow();

			g_render.SetActiveInstanceCamera(g_render.GetDefaultInstanceCamera());
			SetDialogData3(CFalse);
			PrintInfo("\nPrefab mode activated");

			if (g_multipleView->m_enableTimer)
				g_multipleView->EnableTimer(CTrue);
			g_multipleView->SetElapsedTimeFromBeginning();
			g_multipleView->RenderWindow();
		}
	}
	else if (wParam == ID_FILE_NEW)
	{
		if (g_multipleView->m_enableTimer)
			g_multipleView->EnableTimer(CFalse);
		OnMenuClickedNew(CTrue); //ask to see if we should proceed?
		if (g_multipleView->m_enableTimer)
			g_multipleView->EnableTimer(CTrue);
		g_multipleView->SetElapsedTimeFromBeginning();
		g_multipleView->RenderWindow();
	}
	else if (wParam == ID_FILE_OPEN)
	{
		if( g_multipleView->m_enableTimer )
			g_multipleView->EnableTimer( CFalse );
		OnMenuClickedOpenVScene();
		if( g_multipleView->m_enableTimer )
			g_multipleView->EnableTimer( CTrue );

		g_multipleView->SetElapsedTimeFromBeginning();
	}
	else if (wParam == ID_FILE_SAVE )
	{
		if( g_multipleView->m_enableTimer )
			g_multipleView->EnableTimer( CFalse );
		if (g_editorMode == eMODE_PREFAB)
			OnMenuClickedSavePrefabAs(CFalse);
		else
			OnMenuClickedSaveAs(CFalse);
		if( g_multipleView->m_enableTimer )
			g_multipleView->EnableTimer( CTrue );

		g_multipleView->SetElapsedTimeFromBeginning();
	}
	else if (wParam == ID_FILE_SAVEAS )
	{
		if( g_multipleView->m_enableTimer )
			g_multipleView->EnableTimer( CFalse );
		if (g_editorMode == eMODE_PREFAB)
			OnMenuClickedSavePrefabAs();
		else
			OnMenuClickedSaveAs();
		if (g_multipleView->m_enableTimer)
			g_multipleView->EnableTimer( CTrue );

		g_multipleView->SetElapsedTimeFromBeginning();
	}
	else if( wParam == ID_IMPORT_COLLADA )
	{
		if( g_multipleView->m_enableTimer )
			g_multipleView->EnableTimer( CFalse );
		OnMenuClickedImportCollada();
		if( g_multipleView->m_enableTimer )
			g_multipleView->EnableTimer( CTrue );

		g_multipleView->SetElapsedTimeFromBeginning();
		g_multipleView->RenderWindow();
	}
	//else if( wParam == ID_IMPORT_PHYSX ) removed in version 1.4 or later
	//{
	//	if( g_multipleView->m_enableTimer )
	//		g_multipleView->EnableTimer( CFalse );
	//	OnMenuClickedImportPhysX();
	//	if( g_multipleView->m_enableTimer )
	//		g_multipleView->EnableTimer( CTrue );

	//	g_multipleView->SetElapsedTimeFromBeginning();
	//	g_multipleView->RenderWindow();
	//}
	else if( wParam == ID_PROJECT_NEW )
	{
		if( g_multipleView->m_enableTimer )
			g_multipleView->EnableTimer( CFalse );

		CEditProjectProperties m_dlgNewProject;
		m_dlgNewProject.DoModal();

		if( g_multipleView->m_enableTimer )
			g_multipleView->EnableTimer( CTrue );

		g_multipleView->SetElapsedTimeFromBeginning();
		g_multipleView->RenderWindow();

	}
	else if( wParam == ID_PROJECT_EDITCURRENT )
	{
		if( g_multipleView->m_enableTimer )
			g_multipleView->EnableTimer( CFalse );

		CEditProjectProperties m_dlgNewProject;

		for( CUInt i = 0; i < g_projects.size(); i++ )
		{
			if( g_projects[i]->m_isActive )
			{
				m_dlgNewProject.SetName(g_projects[i]->m_name);
				break;
			}
		}
		m_dlgNewProject.SetEdit(CTrue);
		m_dlgNewProject.DoModal();

		if( g_multipleView->m_enableTimer )
			g_multipleView->EnableTimer( CTrue );

		g_multipleView->SetElapsedTimeFromBeginning();
		g_multipleView->RenderWindow();

	}
	else if (wParam == ID_PROJECT_EDITVSCENESOFCURRENTPROJECT)
	{
		if (g_multipleView->m_enableTimer)
			g_multipleView->EnableTimer(CFalse);

		CEditProjectVScenes* m_dlgProjectVScenes = CNew(CEditProjectVScenes);
		m_dlgProjectVScenes->SetState(CFalse);
		m_dlgProjectVScenes->DoModal();
		CDelete(m_dlgProjectVScenes);

		if (g_multipleView->m_enableTimer)
			g_multipleView->EnableTimer(CTrue);

		g_multipleView->SetElapsedTimeFromBeginning();
		g_multipleView->RenderWindow();

	}
	else if( wParam == ID_PROJECT_SETCURRENT )
	{
		if( g_multipleView->m_enableTimer )
			g_multipleView->EnableTimer( CFalse );

		CSetCurrentProject m_dlgSetCurrentProject;
		m_dlgSetCurrentProject.DoModal();

		if( g_multipleView->m_enableTimer )
			g_multipleView->EnableTimer( CTrue );

		g_multipleView->SetElapsedTimeFromBeginning();
		g_multipleView->RenderWindow();

	}
	else if( wParam == ID_PUBLISH_PROJECT )
	{
		OnBnClickedBtnPublishSolution();
	}
	else if( wParam == ID_FILE_EXIT )
	{
		//configuration
		CChar ConfigPath[MAX_NAME_SIZE];
		HRESULT result = SHGetFolderPath(NULL, CSIDL_PERSONAL, NULL, SHGFP_TYPE_CURRENT, ConfigPath);
		if (result != S_OK)
		{
			PrintInfo( "\nCouldn't get the documents folder to write data", COLOR_RED );
		}
		else
		{
			Append( ConfigPath, "/Vanda/conf_editor.dat" );
		}

		FILE *filePtr;
		filePtr = fopen( ConfigPath, "wb" );
		fwrite( &g_options, sizeof( COptions ), 1, filePtr  );

		fclose(filePtr);
		////////////////

		//PackagePath path
		CChar PackageFilePath[MAX_NAME_SIZE];
		HRESULT doc_result_package = SHGetFolderPath(NULL, CSIDL_PERSONAL, NULL, SHGFP_TYPE_CURRENT, PackageFilePath);
		if (doc_result_package != S_OK)
		{
			PrintInfo("\nCouldn't get the documents folder to write data", COLOR_RED);
		}
		else
		{
			Append(PackageFilePath, "/Vanda/Packages/package_editor.dat");
		}

		CChar PackagePath[MAX_NAME_SIZE];
		SHGetFolderPath(NULL, CSIDL_PERSONAL, NULL, SHGFP_TYPE_CURRENT, PackagePath);
		Append(PackagePath, "/Vanda/Packages/PKG/");

		filePtr = fopen(PackageFilePath, "wb");
		CUInt packageSize = g_prefabPackagesAndNames.size();
		fwrite(&packageSize, sizeof(CUInt), 1, filePtr);
		fclose(filePtr);

		for (CUInt i = 0; i < g_prefabPackagesAndNames.size(); i++)
		{
			CChar packagePath[MAX_NAME_SIZE];
			sprintf(packagePath, "%s%s%s", PackagePath, g_prefabPackagesAndNames[i][0].c_str(), ".pkg");
			filePtr = fopen(packagePath, "wb");
			CUInt row_size = g_prefabPackagesAndNames[i].size();
			fwrite(&row_size, sizeof(CUInt), 1, filePtr);
			for (CUInt j = 0; j < row_size; j++)
			{
				CChar name[MAX_NAME_SIZE];
				Cpy(name, g_prefabPackagesAndNames[i][j].c_str());
				fwrite(&name, sizeof(CChar), MAX_NAME_SIZE, filePtr);
			}
			fclose(filePtr);
		}
		///////////////////

		if( g_scene.size() > 0 || g_engineLights.size() > 0 || g_engineWaters.size() > 0 || g_menu.m_insertAndShowSky || g_menu.m_insertAmbientSound || g_engineStaticSounds.size() > 0 )
		{
			CInt iResponse;
			iResponse = MessageBox( "Save scene?", "Warning" , MB_YESNOCANCEL |MB_ICONSTOP);
			if( iResponse == IDYES )
			{
				if( g_multipleView->m_enableTimer )
					g_multipleView->EnableTimer( CFalse );
				if (g_editorMode == eMODE_PREFAB)
					OnMenuClickedSavePrefabAs();
				else
					OnMenuClickedSaveAs();
				if (g_multipleView->m_enableTimer)
					g_multipleView->EnableTimer( CTrue );

				OnMenuClickedNew(CFalse);
				PostQuitMessage( 0 );
			}
			else if (iResponse == IDNO)
			{
				OnMenuClickedNew(CFalse);
				PostQuitMessage(0);
			}
		}
		else
		{
			OnMenuClickedNew(CFalse);
			PostQuitMessage(0);
		}
	}
	else if( wParam == ID_INSERT_LIGHT )
	{
		if( g_multipleView->m_enableTimer )
			g_multipleView->EnableTimer( CFalse );
		OnMenuClickedInsertLight();
		if( g_multipleView->m_enableTimer )
			g_multipleView->EnableTimer( CTrue );

		g_multipleView->SetElapsedTimeFromBeginning();
		g_multipleView->RenderWindow();
	}
	else if( wParam == ID_INSERT_WATER )
	{
		if( g_multipleView->m_enableTimer )
			g_multipleView->EnableTimer( CFalse );
		OnMenuClickedInsertWater();
		if( g_multipleView->m_enableTimer )
			g_multipleView->EnableTimer( CTrue );

		g_multipleView->SetElapsedTimeFromBeginning();
		g_multipleView->RenderWindow();
	}
	else if( wParam == ID_INSERT_SOUND_AMBIENT )
	{
		if( g_multipleView->m_enableTimer )
			g_multipleView->EnableTimer( CFalse );
		OnMenuClickedInsertAmbientSound();
		if( g_multipleView->m_enableTimer )
			g_multipleView->EnableTimer( CTrue );

		g_multipleView->SetElapsedTimeFromBeginning();
	}
	else if( wParam == ID_INSERT_SOUND_STATIC )
	{
		if( g_multipleView->m_enableTimer )
			g_multipleView->EnableTimer( CFalse );
		OnMenuClickedInsertStaticSound();
		if( g_multipleView->m_enableTimer )
			g_multipleView->EnableTimer( CTrue );

		g_multipleView->SetElapsedTimeFromBeginning();
	}

	else if( wParam == ID_INSERT_SKYDOME )
	{
		if( g_multipleView->m_enableTimer )
			g_multipleView->EnableTimer( CFalse );
		OnMenuClickedInsertSkyDome();
		if( g_multipleView->m_enableTimer )
			g_multipleView->EnableTimer( CTrue );

		g_multipleView->SetElapsedTimeFromBeginning();
		g_multipleView->RenderWindow();
	}
	else if (wParam == ID_INSERT_PLAYER)
	{
		if (g_multipleView->m_enableTimer)
			g_multipleView->EnableTimer(CFalse);
		OnBnClickedBtnPlayer();
		if (g_multipleView->m_enableTimer)
			g_multipleView->EnableTimer(CTrue);

		g_multipleView->SetElapsedTimeFromBeginning();
		g_multipleView->RenderWindow();

	}
	else if (wParam == ID_INSERT_TRIGGER)
	{
		if (g_multipleView->m_enableTimer)
			g_multipleView->EnableTimer(CFalse);
		OnBnClickedBtnTrigger();
		if (g_multipleView->m_enableTimer)
			g_multipleView->EnableTimer(CTrue);

		g_multipleView->SetElapsedTimeFromBeginning();
		g_multipleView->RenderWindow();
	}
	else if (wParam == ID_PREFAB_EDITOR)
	{
		if (g_multipleView->m_enableTimer)
			g_multipleView->EnableTimer(CFalse);
		OnMenuClickedPrefab();
		if (g_multipleView->m_enableTimer)
			g_multipleView->EnableTimer(CTrue);

		g_multipleView->SetElapsedTimeFromBeginning();
		g_multipleView->RenderWindow();
	}

	else if( wParam == ID_LAYOUTS_4SPLIT )
	{
		g_menu.m_justPerspective = CFalse;
		PrintInfo( "\nSwitched to 4 viewports" );
		GetMenu()->CheckMenuItem( ID_LAYOUTS_PERSPECTIVE, MF_UNCHECKED );
		GetMenu()->CheckMenuItem( ID_LAYOUTS_4SPLIT, MF_CHECKED );

		g_multipleView->SetElapsedTimeFromBeginning();
		g_multipleView->RenderWindow();
	}
	else if( wParam == ID_LAYOUTS_PERSPECTIVE )
	{
		g_menu.m_justPerspective = CTrue;
		PrintInfo( "\nSwitched to perspective viewport" );
		GetMenu()->CheckMenuItem( ID_LAYOUTS_PERSPECTIVE, MF_CHECKED );
		GetMenu()->CheckMenuItem( ID_LAYOUTS_4SPLIT, MF_UNCHECKED );

		g_multipleView->SetElapsedTimeFromBeginning();
		g_multipleView->RenderWindow();
	}
	else if (wParam == ID_MODIFY_SHADOW )
	{
		if( g_multipleView->m_enableTimer )
			g_multipleView->EnableTimer( CFalse );

		m_dlgEditShadow = CNew( CEditShadow );
		m_dlgEditShadow->DoModal();
		CDelete( m_dlgEditShadow );

		if( g_multipleView->m_enableTimer )
			g_multipleView->EnableTimer( CTrue );

		g_multipleView->SetElapsedTimeFromBeginning();
		g_multipleView->RenderWindow();
	}
	else if (wParam == ID_MODIFY_DOF )
	{
		if( g_multipleView->m_enableTimer )
			g_multipleView->EnableTimer( CFalse );

		m_dlgEditDOF = CNew( CEditDOF );
		m_dlgEditDOF->DoModal();
		CDelete( m_dlgEditDOF );

		if( g_multipleView->m_enableTimer )
			g_multipleView->EnableTimer( CTrue );

		g_multipleView->SetElapsedTimeFromBeginning();
		g_multipleView->RenderWindow();
	}
	else if (wParam == ID_MODIFY_FOG )
	{
		if( g_multipleView->m_enableTimer )
			g_multipleView->EnableTimer( CFalse );

		m_dlgEditFog = CNew( CEditFog );
		m_dlgEditFog->DoModal();
		CDelete( m_dlgEditFog );

		if( g_multipleView->m_enableTimer )
			g_multipleView->EnableTimer( CTrue );

		g_multipleView->SetElapsedTimeFromBeginning();
		g_multipleView->RenderWindow();
	}
	else if (wParam == ID_TOOLS_PHYSXEDITOR )
	{
		OnBnClickedBtnPhysxEditor();
	}
	else if( wParam == ID_TOOLS_CAMERASPEED_INCREASE )
	{
		if( g_currentCameraType == eCAMERA_DEFAULT_FREE )
			g_camera->m_cameraSpeed += 1.0f;
		else if( g_currentCameraType == eCAMERA_DEFAULT_PHYSX )
		g_multipleView->m_nx->gCharacterSpeed += 1.0f;
	}
	else if( wParam == ID_TOOLS_CAMERASPEED_DECREASE )
	{
		if( g_currentCameraType == eCAMERA_DEFAULT_FREE )
		{
			g_camera->m_cameraSpeed -= 1.0f;
			if( g_camera->m_cameraSpeed < 0.0f )
			{
				g_camera->m_cameraSpeed += 1.0f;
				PrintInfo( "\nError: Couldn't decrease the camera speed", COLOR_RED );
			}
		}
		else if( g_currentCameraType == eCAMERA_DEFAULT_PHYSX )
		{
			g_multipleView->m_nx->gCharacterSpeed -= 1.0f;
			if( g_multipleView->m_nx->gCharacterSpeed < 0.0f )
			{
				g_multipleView->m_nx->gCharacterSpeed += 1.0f;
				PrintInfo( "\nError: Couldn't decrease the camera speed", COLOR_RED );
			}
		}
	}
	else if( wParam == ID_TOOLS_CAMERASPEED_DEFAULT )
	{
		if( g_currentCameraType == eCAMERA_DEFAULT_FREE )
		{
			g_camera->m_cameraSpeed = DEFAULT_CAMERA_SPEED;
			PrintInfo( "\nCamera speed set to default" );
		}
		else if( g_currentCameraType == eCAMERA_DEFAULT_PHYSX )
		{
			g_multipleView->m_nx->gCharacterSpeed = g_physXProperties.m_fCharacterSpeed;
			PrintInfo( "\nCamera speed set to default" );
		}

	}
	else if( wParam == ID_TOOLS_CAMERAZOOM_DEFAULT )
	{
		if( g_currentCameraType == eCAMERA_DEFAULT_FREE )
			g_render.GetDefaultInstanceCamera()->ZoomTransform2(0.0f);
		else if( g_currentCameraType == eCAMERA_DEFAULT_PHYSX )
			g_camera->m_cameraManager->SetAngle( DEFAULT_CAMERA_ANGLE );

		g_multipleView->SetElapsedTimeFromBeginning();
		g_multipleView->RenderWindow();
	}


	else if( wParam == ID_TOOLS_CULLFACES )
	{
		OnBnClickedCullFace();
		g_multipleView->SetElapsedTimeFromBeginning();
		g_multipleView->RenderWindow();
	}
	else if( wParam == ID_GEOMETRY_AMBIENTCOLOR )
	{
		if( g_multipleView->m_enableTimer )
			g_multipleView->EnableTimer( CFalse );

		OnMenuClickedGeneralAmbientColor();

		if( g_multipleView->m_enableTimer )
			g_multipleView->EnableTimer( CTrue );

		g_multipleView->SetElapsedTimeFromBeginning();
		g_multipleView->RenderWindow();
	}

	else if( wParam == ID_TOOLS_WATERATTACHMENT )
	{
		if( g_selectedName == -1 )
		{
			MessageBox( "Please select a prefab instance", "VandaEngine Error", MB_OK | MB_ICONINFORMATION);
			return FALSE;
		}
		else if( !g_menu.m_geometryBasedSelection )
		{
			MessageBox( "Water attachment does not work with material based selection.\nPlease switch to geometry based selection and try again!", "VandaEngine Error", MB_OK | MB_ICONERROR);
			return FALSE;
		}
		else if (g_selectedName != -1)
		{
			for (CUInt i = 0; i < g_instancePrefab.size(); i++)
			{
				if (g_instancePrefab[i]->GetNameIndex() == g_selectedName)
				{
					if (g_instancePrefab[i]->GetIsTrigger())
					{
						MessageBox("You cannot attach triggers to waters.", "Vanda Engine Error", MB_OK | MB_ICONINFORMATION);
						return FALSE;
					}
				}
			}
		}
		else
		{
			if( g_multipleView->m_enableTimer )
				g_multipleView->EnableTimer( CFalse );
			OnMenuClickedWaterAttachment();
			if( g_multipleView->m_enableTimer )
				g_multipleView->EnableTimer( CTrue );

			g_multipleView->SetElapsedTimeFromBeginning();
			g_multipleView->RenderWindow();
		}
	}
	else if( wParam == ID_TOOLS_SCRIPTMANAGER )
	{
		OnBnClickedBtnScriptManager();
	}
	else if( wParam == ID_TOOLS_SELECTCAMERA )
	{
		if( g_multipleView->m_enableTimer )
			g_multipleView->EnableTimer( CFalse );
		OnMenuClickedSelectCamera();
		if( g_multipleView->m_enableTimer )
			g_multipleView->EnableTimer( CTrue );
		g_multipleView->SetElapsedTimeFromBeginning();
		g_multipleView->RenderWindow();
	}
	else if( wParam == ID_TOOLS_OPTIONS )
	{
		if( g_multipleView->m_enableTimer )
			g_multipleView->EnableTimer( CFalse );
		OnMenuClickedGeneralOptions();
		if( g_multipleView->m_enableTimer )
			g_multipleView->EnableTimer( CTrue );
		g_multipleView->SetElapsedTimeFromBeginning();
		g_multipleView->RenderWindow();
	}
	else if( wParam == ID_TOOLS_IMPORTOPTIONS )
	{
		if( g_multipleView->m_enableTimer )
			g_multipleView->EnableTimer( CFalse );
		OnMenuClickedSceneOptions();
		if( g_multipleView->m_enableTimer )
			g_multipleView->EnableTimer( CTrue );
		g_multipleView->SetElapsedTimeFromBeginning();
		g_multipleView->RenderWindow();
	}
	else if (wParam == ID_TOOLS_CURRENTSCENEOPTIONS)
	{
		if (g_multipleView->m_enableTimer)
			g_multipleView->EnableTimer(CFalse);
		OnMenuClickedCurrentSceneOptions();
		if (g_multipleView->m_enableTimer)
			g_multipleView->EnableTimer(CTrue);
		g_multipleView->SetElapsedTimeFromBeginning();
		g_multipleView->RenderWindow();
	}

	else if( wParam == ID_TOOLS_GEOMETRYBASEDSELECTION )
	{
		g_menu.m_geometryBasedSelection = !g_menu.m_geometryBasedSelection;
		ex_pBtnMaterialEditor->EnableWindow( FALSE );
		GetMenu()->EnableMenuItem( ID_TOOLS_MATERIALEDITOR, MF_DISABLED | MF_GRAYED );

		ex_pBtnPhysXEditor->EnableWindow( FALSE );
		GetMenu()->EnableMenuItem( ID_TOOLS_PHYSXEDITOR, MF_DISABLED | MF_GRAYED );

		//ex_pBtnScriptEditor->EnableWindow( FALSE );
		//GetMenu()->EnableMenuItem( ID_TOOLS_SCRIPTMANAGER, MF_DISABLED | MF_GRAYED );

		ex_pStaticSelectedObject->SetWindowTextA( "Nothing Selected" );

		if( g_menu.m_geometryBasedSelection )
		{
			GetMenu()->CheckMenuItem( ID_TOOLS_GEOMETRYBASEDSELECTION, MF_CHECKED );
			PrintInfo( "\nGeometry Based Selection enabled" );
		}
		else
		{
			GetMenu()->CheckMenuItem( ID_TOOLS_GEOMETRYBASEDSELECTION, MF_UNCHECKED );
			PrintInfo( "\nMaterial Based Selection enabled" );
		}

		//material editor

		m_dlgEditMaterial->SetNormalBtnState( CFalse );
		m_dlgEditMaterial->SetDirtBtnState( CFalse );
		m_dlgEditMaterial->SetDiffuseBtnState( CFalse );
		m_dlgEditMaterial->SetGlossBtnState( CFalse );

		m_dlgEditMaterial->SetEditBoxNormalMapName( "\n" );
		m_dlgEditMaterial->SetEditBoxDirtMapName( "\n" );
		m_dlgEditMaterial->SetEditBoxDiffuseName( "\n" );
		m_dlgEditMaterial->SetEditBoxGlossMapName( "\n" );

		m_dlgEditMaterial->SetRemoveDirtBtnState( CFalse );
		m_dlgEditMaterial->SetRemoveNormalBtnState( CFalse );
		m_dlgEditMaterial->SetRemoveDiffuseBtnState( CFalse );
		m_dlgEditMaterial->SetRemoveGlossBtnState( CFalse );
		/////

		g_multipleView->SetElapsedTimeFromBeginning();
		g_multipleView->RenderWindow();
	}
	else if( wParam == ID_TOOLS_MATERIALEDITOR)
	{
		if( g_multipleView->m_enableTimer )
			g_multipleView->EnableTimer( CFalse );
		OnMenuClickedEditMaterial();
		if( g_multipleView->m_enableTimer )
			g_multipleView->EnableTimer( CTrue );

		g_multipleView->SetElapsedTimeFromBeginning();

	}
	else if( wParam == ID_VIEW_LIGHT_POSITIONS )
	{
		g_menu.m_showLightIcons = !g_menu.m_showLightIcons; 
		if( g_menu.m_showLightIcons )
		{
			GetMenu()->CheckMenuItem( ID_VIEW_LIGHT_POSITIONS, MF_CHECKED );
			PrintInfo( "\nLight icons enabled" );
		}
		else
		{
			GetMenu()->CheckMenuItem( ID_VIEW_LIGHT_POSITIONS, MF_UNCHECKED );
			PrintInfo( "\nLight icons disabled" );
		}
		g_multipleView->SetElapsedTimeFromBeginning();
		g_multipleView->RenderWindow();
	}
	else if( wParam == ID_VIEW_SOUND_POSITIONS )
	{
		g_menu.m_showSoundIcons = !g_menu.m_showSoundIcons; 
		if( g_menu.m_showSoundIcons )
		{
			GetMenu()->CheckMenuItem( ID_VIEW_SOUND_POSITIONS, MF_CHECKED );
			PrintInfo( "\nSound icons enabled" );
		}
		else
		{
			GetMenu()->CheckMenuItem( ID_VIEW_SOUND_POSITIONS, MF_UNCHECKED );
			PrintInfo( "\nSound icons disabled" );
		}
		g_multipleView->SetElapsedTimeFromBeginning();
		g_multipleView->RenderWindow();
	}
	else if( wParam == ID_ICONS_WATERICONS )
	{
		g_menu.m_showWaterIcons = !g_menu.m_showWaterIcons; 
		if( g_menu.m_showWaterIcons )
		{
			GetMenu()->CheckMenuItem( ID_ICONS_WATERICONS, MF_CHECKED );
			PrintInfo( "\nWater icons enabled" );
		}
		else
		{
			GetMenu()->CheckMenuItem( ID_ICONS_WATERICONS, MF_UNCHECKED );
			PrintInfo( "\nWater icons disabled" );
		}
		g_multipleView->SetElapsedTimeFromBeginning();
		g_multipleView->RenderWindow();
	}
	else if( wParam == ID_ICONS_SKYICON )
	{
		g_menu.m_showSkyIcon = !g_menu.m_showSkyIcon; 
		if( g_menu.m_showSkyIcon )
		{
			GetMenu()->CheckMenuItem( ID_ICONS_SKYICON, MF_CHECKED );
			PrintInfo( "\nSky icon enabled" );
		}
		else
		{
			GetMenu()->CheckMenuItem( ID_ICONS_SKYICON, MF_UNCHECKED );
			PrintInfo( "\nSky icon disabled" );
		}
		g_multipleView->SetElapsedTimeFromBeginning();
		g_multipleView->RenderWindow();
	}	
	else if( wParam == ID_ICONS_CAMERAICONS )
	{
		g_menu.m_showCameraIcons = !g_menu.m_showCameraIcons; 
		if( g_menu.m_showCameraIcons )
		{
			GetMenu()->CheckMenuItem( ID_ICONS_CAMERAICONS, MF_CHECKED );
			PrintInfo( "\nCamera icon enabled" );
		}
		else
		{
			GetMenu()->CheckMenuItem( ID_ICONS_CAMERAICONS, MF_UNCHECKED );
			PrintInfo( "\nCamera icon disabled" );
		}
		g_multipleView->SetElapsedTimeFromBeginning();
		g_multipleView->RenderWindow();
	}
	else if( wParam == ID_VIEW_PERSPECTIVE_GRIDS )
	{
		g_menu.m_showPerspectiveGrids = !g_menu.m_showPerspectiveGrids; 
		if( g_menu.m_showPerspectiveGrids )
		{
			GetMenu()->CheckMenuItem( ID_VIEW_PERSPECTIVE_GRIDS, MF_CHECKED );
			PrintInfo( "\nPerspective grids enabled" );
		}
		else
		{
			GetMenu()->CheckMenuItem( ID_VIEW_PERSPECTIVE_GRIDS, MF_UNCHECKED );
			PrintInfo( "\nPerspective grids disabled" );
		}
		g_multipleView->SetElapsedTimeFromBeginning();
		g_multipleView->RenderWindow();
	}
	else if( wParam == ID_VIEW_OCTREE )
	{
		g_menu.m_showOctree = !g_menu.m_showOctree; 
		if( g_menu.m_showOctree )
		{
			GetMenu()->CheckMenuItem( ID_VIEW_OCTREE, MF_CHECKED );
			PrintInfo( "\nOctree is visible" );
		}
		else
		{
			GetMenu()->CheckMenuItem( ID_VIEW_OCTREE, MF_UNCHECKED );
			PrintInfo( "\nOctree is invisible" );
		}
		g_multipleView->SetElapsedTimeFromBeginning();
		g_multipleView->RenderWindow();
	}
	else if( wParam == ID_VIEW_BOUNDINGBOX )
	{
		g_menu.m_showBoundingBox = !g_menu.m_showBoundingBox; 
		if( g_menu.m_showBoundingBox )
		{
			gPhysicsSDK->setParameter(NX_VISUALIZE_COLLISION_AABBS, 1);
			GetMenu()->CheckMenuItem( ID_VIEW_BOUNDINGBOX, MF_CHECKED );
			PrintInfo( "\nBounding boxes enabled" );
		}
		else
		{
			gPhysicsSDK->setParameter(NX_VISUALIZE_COLLISION_AABBS, 0);
			GetMenu()->CheckMenuItem( ID_VIEW_BOUNDINGBOX, MF_UNCHECKED );
			PrintInfo( "\nBounding boxes disabled" );
		}
		g_multipleView->SetElapsedTimeFromBeginning();
		g_multipleView->RenderWindow();
	}
	else if( wParam == ID_VIEW_SHADOWDEPTHMAP )
	{
		g_menu.m_showDynamicShadowDepthTexture = !g_menu.m_showDynamicShadowDepthTexture; 
		if( g_menu.m_showDynamicShadowDepthTexture )
		{
			GetMenu()->CheckMenuItem( ID_VIEW_SHADOWDEPTHMAP, MF_CHECKED );
			PrintInfo( "\nDepth map activated" );
		}
		else
		{
			GetMenu()->CheckMenuItem( ID_VIEW_SHADOWDEPTHMAP, MF_UNCHECKED );
			PrintInfo( "\nDepth map deactivated" );
		}
		g_multipleView->SetElapsedTimeFromBeginning();
		g_multipleView->RenderWindow();
	}
	else if( wParam == ID_VIEW_STATISTICS )
	{
		g_menu.m_showStatistics = !g_menu.m_showStatistics;
		if( g_menu.m_showStatistics )
			GetMenu()->CheckMenuItem( ID_VIEW_STATISTICS, MF_CHECKED );
		else
			GetMenu()->CheckMenuItem( ID_VIEW_STATISTICS, MF_UNCHECKED );
		g_multipleView->SetElapsedTimeFromBeginning();
		g_multipleView->RenderWindow();
	}
	else if( wParam == ID_VIEW_REPORT )
	{
		if( g_multipleView->m_enableTimer )
			g_multipleView->EnableTimer( CFalse );

		m_dlgConsole = CNew( CConsole );
		m_dlgConsole->DoModal();
		CDelete( m_dlgConsole );

		if( g_multipleView->m_enableTimer )
			g_multipleView->EnableTimer( CTrue );

		g_multipleView->SetElapsedTimeFromBeginning();
		g_multipleView->RenderWindow();
	}
	else if( wParam == ID_VIEW_ALLCHANNELS )
	{
		PrintInfo( "\nSwitched to all channels" );

		g_materialChannels = eCHANNELS_ALL;
		GetMenu()->CheckMenuItem( ID_VIEW_ALLCHANNELS, MF_CHECKED );
		GetMenu()->CheckMenuItem( ID_VIEW_DIFFUSECHANNEL, MF_UNCHECKED );
		GetMenu()->CheckMenuItem( ID_VIEW_NORMALMAPCHANNEL, MF_UNCHECKED );
		GetMenu()->CheckMenuItem( ID_VIEW_GLOSSMAPCHANNEL, MF_UNCHECKED );
		GetMenu()->CheckMenuItem( ID_VIEW_DIRTMAPCHANNEL, MF_UNCHECKED );
		GetMenu()->CheckMenuItem( ID_VIEW_ALPHAMAPCHANNEL, MF_UNCHECKED );

		GetMenu()->CheckMenuItem( ID_VIEW_VERTEXPOSITIONS, MF_UNCHECKED );
		GetMenu()->CheckMenuItem( ID_VIEW_NORMALVECTORS, MF_UNCHECKED );

		g_multipleView->SetElapsedTimeFromBeginning();
		g_multipleView->RenderWindow();
	}
	else if( wParam == ID_VIEW_DIFFUSECHANNEL )
	{
		PrintInfo( "\nSwitched to diffuse map channel" );

		g_materialChannels = eCHANNELS_DIFFUSE;
		GetMenu()->CheckMenuItem( ID_VIEW_ALLCHANNELS, MF_UNCHECKED );
		GetMenu()->CheckMenuItem( ID_VIEW_DIFFUSECHANNEL,  MF_CHECKED );
		GetMenu()->CheckMenuItem( ID_VIEW_NORMALMAPCHANNEL, MF_UNCHECKED );
		GetMenu()->CheckMenuItem( ID_VIEW_GLOSSMAPCHANNEL, MF_UNCHECKED );
		GetMenu()->CheckMenuItem( ID_VIEW_DIRTMAPCHANNEL, MF_UNCHECKED );
		GetMenu()->CheckMenuItem( ID_VIEW_ALPHAMAPCHANNEL, MF_UNCHECKED );

		GetMenu()->CheckMenuItem( ID_VIEW_VERTEXPOSITIONS, MF_UNCHECKED );
		GetMenu()->CheckMenuItem( ID_VIEW_NORMALVECTORS, MF_UNCHECKED );

		g_multipleView->SetElapsedTimeFromBeginning();
		g_multipleView->RenderWindow();

	}
	else if( wParam == ID_VIEW_NORMALMAPCHANNEL )
	{
		PrintInfo( "\nSwitched to normal map channel" );

		g_materialChannels = eCHANNELS_NORMALMAP;
		GetMenu()->CheckMenuItem( ID_VIEW_ALLCHANNELS, MF_UNCHECKED );
		GetMenu()->CheckMenuItem( ID_VIEW_DIFFUSECHANNEL, MF_UNCHECKED  );
		GetMenu()->CheckMenuItem( ID_VIEW_NORMALMAPCHANNEL, MF_CHECKED );
		GetMenu()->CheckMenuItem( ID_VIEW_GLOSSMAPCHANNEL, MF_UNCHECKED );
		GetMenu()->CheckMenuItem( ID_VIEW_DIRTMAPCHANNEL, MF_UNCHECKED );
		GetMenu()->CheckMenuItem( ID_VIEW_ALPHAMAPCHANNEL, MF_UNCHECKED );

		GetMenu()->CheckMenuItem( ID_VIEW_VERTEXPOSITIONS, MF_UNCHECKED );
		GetMenu()->CheckMenuItem( ID_VIEW_NORMALVECTORS, MF_UNCHECKED );

		g_multipleView->SetElapsedTimeFromBeginning();
		g_multipleView->RenderWindow();
	}
	else if( wParam == ID_VIEW_GLOSSMAPCHANNEL )
	{
		PrintInfo( "\nSwitched to gloss map channel" );

		g_materialChannels = eCHANNELS_GLOSSMAP;
		GetMenu()->CheckMenuItem( ID_VIEW_ALLCHANNELS, MF_UNCHECKED );
		GetMenu()->CheckMenuItem( ID_VIEW_DIFFUSECHANNEL, MF_UNCHECKED  );
		GetMenu()->CheckMenuItem( ID_VIEW_NORMALMAPCHANNEL, MF_UNCHECKED );
		GetMenu()->CheckMenuItem( ID_VIEW_GLOSSMAPCHANNEL, MF_CHECKED );
		GetMenu()->CheckMenuItem( ID_VIEW_DIRTMAPCHANNEL, MF_UNCHECKED );
		GetMenu()->CheckMenuItem( ID_VIEW_ALPHAMAPCHANNEL, MF_UNCHECKED );

		GetMenu()->CheckMenuItem( ID_VIEW_VERTEXPOSITIONS, MF_UNCHECKED );
		GetMenu()->CheckMenuItem( ID_VIEW_NORMALVECTORS, MF_UNCHECKED );

		g_multipleView->SetElapsedTimeFromBeginning();
		g_multipleView->RenderWindow();
	}
	else if( wParam == ID_VIEW_DIRTMAPCHANNEL )
	{
		PrintInfo( "\nSwitched to dirt map channel" );

		g_materialChannels = eCHANNELS_DIRTMAP;
		GetMenu()->CheckMenuItem( ID_VIEW_ALLCHANNELS, MF_UNCHECKED );
		GetMenu()->CheckMenuItem( ID_VIEW_DIFFUSECHANNEL, MF_UNCHECKED  );
		GetMenu()->CheckMenuItem( ID_VIEW_NORMALMAPCHANNEL, MF_UNCHECKED );
		GetMenu()->CheckMenuItem( ID_VIEW_GLOSSMAPCHANNEL, MF_UNCHECKED );
		GetMenu()->CheckMenuItem( ID_VIEW_DIRTMAPCHANNEL, MF_CHECKED );
		GetMenu()->CheckMenuItem( ID_VIEW_ALPHAMAPCHANNEL, MF_UNCHECKED );

		GetMenu()->CheckMenuItem( ID_VIEW_VERTEXPOSITIONS, MF_UNCHECKED );
		GetMenu()->CheckMenuItem( ID_VIEW_NORMALVECTORS, MF_UNCHECKED );

		g_multipleView->SetElapsedTimeFromBeginning();
		g_multipleView->RenderWindow();
	}
	else if( wParam == ID_VIEW_ALPHAMAPCHANNEL )
	{
		PrintInfo( "\nSwitched to alpha map channel" );

		g_materialChannels = eCHANNELS_ALPHAMAP;
		GetMenu()->CheckMenuItem( ID_VIEW_ALLCHANNELS, MF_UNCHECKED );
		GetMenu()->CheckMenuItem( ID_VIEW_DIFFUSECHANNEL, MF_UNCHECKED  );
		GetMenu()->CheckMenuItem( ID_VIEW_NORMALMAPCHANNEL, MF_UNCHECKED );
		GetMenu()->CheckMenuItem( ID_VIEW_GLOSSMAPCHANNEL, MF_UNCHECKED );
		GetMenu()->CheckMenuItem( ID_VIEW_DIRTMAPCHANNEL, MF_UNCHECKED );
		GetMenu()->CheckMenuItem( ID_VIEW_ALPHAMAPCHANNEL, MF_CHECKED );

		GetMenu()->CheckMenuItem( ID_VIEW_VERTEXPOSITIONS, MF_UNCHECKED );
		GetMenu()->CheckMenuItem( ID_VIEW_NORMALVECTORS, MF_UNCHECKED );

		g_multipleView->SetElapsedTimeFromBeginning();
		g_multipleView->RenderWindow();
	}
	else if( wParam == ID_VIEW_NORMALVECTORS  )
	{
		PrintInfo( "\nSwitched to normal vectors" );
		g_materialChannels = eCHANNELS_NORMAL;
		GetMenu()->CheckMenuItem( ID_VIEW_ALLCHANNELS, MF_UNCHECKED );
		GetMenu()->CheckMenuItem( ID_VIEW_DIFFUSECHANNEL, MF_UNCHECKED  );
		GetMenu()->CheckMenuItem( ID_VIEW_NORMALMAPCHANNEL, MF_UNCHECKED );
		GetMenu()->CheckMenuItem( ID_VIEW_GLOSSMAPCHANNEL, MF_UNCHECKED );
		GetMenu()->CheckMenuItem( ID_VIEW_DIRTMAPCHANNEL, MF_UNCHECKED );
		GetMenu()->CheckMenuItem( ID_VIEW_ALPHAMAPCHANNEL, MF_UNCHECKED );

		GetMenu()->CheckMenuItem( ID_VIEW_VERTEXPOSITIONS, MF_UNCHECKED );
		GetMenu()->CheckMenuItem( ID_VIEW_NORMALVECTORS, MF_CHECKED );

		g_multipleView->SetElapsedTimeFromBeginning();
		g_multipleView->RenderWindow();
	}
	else if( wParam == ID_VIEW_VERTEXPOSITIONS )
	{
		PrintInfo( "\nSwitched to vertex positions" );

		g_materialChannels = eCHANNELS_POSITION;

		GetMenu()->CheckMenuItem( ID_VIEW_ALLCHANNELS, MF_UNCHECKED );
		GetMenu()->CheckMenuItem( ID_VIEW_DIFFUSECHANNEL, MF_UNCHECKED  );
		GetMenu()->CheckMenuItem( ID_VIEW_NORMALMAPCHANNEL, MF_UNCHECKED );
		GetMenu()->CheckMenuItem( ID_VIEW_GLOSSMAPCHANNEL, MF_UNCHECKED );
		GetMenu()->CheckMenuItem( ID_VIEW_DIRTMAPCHANNEL, MF_UNCHECKED );
		GetMenu()->CheckMenuItem( ID_VIEW_ALPHAMAPCHANNEL, MF_UNCHECKED );

		GetMenu()->CheckMenuItem( ID_VIEW_VERTEXPOSITIONS, MF_CHECKED );
		GetMenu()->CheckMenuItem( ID_VIEW_NORMALVECTORS, MF_UNCHECKED );

		g_multipleView->SetElapsedTimeFromBeginning();
		g_multipleView->RenderWindow();
	}
	else if ( wParam == ID_POLGYGONMODE_FILL )
	{
		g_polygonMode = ePOLYGON_FILL;
		GetMenu()->CheckMenuItem( ID_POLGYGONMODE_FILL, MF_CHECKED );
		GetMenu()->CheckMenuItem( ID_POLGYGONMODE_LINE, MF_UNCHECKED );
		GetMenu()->CheckMenuItem( ID_POLGYGONMODE_POINT, MF_UNCHECKED );
		g_multipleView->SetElapsedTimeFromBeginning();
		g_multipleView->RenderWindow();
	}
	else if ( wParam == ID_POLGYGONMODE_LINE )
	{
		g_polygonMode = ePOLYGON_LINE;
		GetMenu()->CheckMenuItem( ID_POLGYGONMODE_FILL, MF_UNCHECKED );
		GetMenu()->CheckMenuItem( ID_POLGYGONMODE_LINE, MF_CHECKED );
		GetMenu()->CheckMenuItem( ID_POLGYGONMODE_POINT, MF_UNCHECKED );
		g_multipleView->SetElapsedTimeFromBeginning();
		g_multipleView->RenderWindow();
	}
	else if ( wParam == ID_POLGYGONMODE_POINT )
	{
		g_polygonMode = ePOLYGON_POINT;
		GetMenu()->CheckMenuItem( ID_POLGYGONMODE_FILL, MF_UNCHECKED );
		GetMenu()->CheckMenuItem( ID_POLGYGONMODE_LINE, MF_UNCHECKED );
		GetMenu()->CheckMenuItem( ID_POLGYGONMODE_POINT, MF_CHECKED );
		g_multipleView->SetElapsedTimeFromBeginning();
		g_multipleView->RenderWindow();
	}
	else if( wParam == ID_MODIFY_BLOOM )
	{
		if( g_multipleView->m_enableTimer )
			g_multipleView->EnableTimer( CFalse );

		m_dlgEditBloom = CNew( CEditBloom );
		m_dlgEditBloom->DoModal();
		CDelete( m_dlgEditBloom );

		if( g_multipleView->m_enableTimer )
			g_multipleView->EnableTimer( CTrue );

		g_multipleView->SetElapsedTimeFromBeginning();
		g_multipleView->RenderWindow();
	}
	else if( wParam == ID_MODIFY_LIGHT )
	{
		if( g_multipleView->m_enableTimer )
			g_multipleView->EnableTimer( CFalse );

		m_dlgEditLight = CNew( CEditLight );
		m_dlgEditLight->DoModal();
		CDelete( m_dlgEditLight );

		if( g_multipleView->m_enableTimer )
			g_multipleView->EnableTimer( CTrue );

		g_multipleView->SetElapsedTimeFromBeginning();
		g_multipleView->RenderWindow();
	}
	else if( wParam == ID_MODIFY_SCENEMANAGER )
	{
		if( g_multipleView->m_enableTimer )
			g_multipleView->EnableTimer( CFalse );

		m_dlgEditSceneManager = CNew( CEditSceneManager );
		m_dlgEditSceneManager->DoModal();
		CDelete( m_dlgEditSceneManager );

		if( g_multipleView->m_enableTimer )
			g_multipleView->EnableTimer( CTrue );

		g_multipleView->SetElapsedTimeFromBeginning();
		g_multipleView->RenderWindow();
	}
	else if( wParam == ID_MODIFY_GENERALPHYSXOPTIONS )
	{
		if( g_multipleView->m_enableTimer )
			g_multipleView->EnableTimer( CFalse );

		m_dlgEditGeneralPhysXProperties = CNew( CEditGeneralPhysXProperties );
		m_dlgEditGeneralPhysXProperties->DoModal();
		CDelete( m_dlgEditGeneralPhysXProperties );

		if( g_multipleView->m_enableTimer )
			g_multipleView->EnableTimer( CTrue );

		g_multipleView->SetElapsedTimeFromBeginning();
		g_multipleView->RenderWindow();
		
	}
	else if( wParam == ID_HELP_ONLINEHELP )
	{
		//ShellExecute(NULL, "open", "data\\help\\vanda.chm", NULL, NULL, SW_SHOWNORMAL);
		ShellExecute(NULL, "open", "http://vandaengine.org/vandaengine/Doc/Reference/", NULL, NULL, SW_SHOWNORMAL);

		g_multipleView->SetElapsedTimeFromBeginning();
	}
	else if( wParam == ID_HELP_ABOUT )
	{
		if( g_multipleView->m_enableTimer )
			g_multipleView->EnableTimer( CFalse );

		CAboutDlg dlgAbout;
		dlgAbout.DoModal();

		if( g_multipleView->m_enableTimer )
			g_multipleView->EnableTimer( CTrue );

		g_multipleView->SetElapsedTimeFromBeginning();
	}
	return CDialog::OnCommand(wParam, lParam);
}

CVoid CVandaEngine1Dlg::SortButtons()
{
	if (g_editorMode == eMODE_PREFAB)
	{
		//disable some buttons in prefab mode. For example it's not possible to create Sky or water in prefab mode.
		//In this mode, users can import 3D objects and assign materials and PhysX colliders to objects and save the
		//result as a prefab. Then they can create one (or multiple) instance of this prefab in VScene mode.
		Invalidate(TRUE);
		m_editX.ShowWindow(SW_HIDE);
		m_editY.ShowWindow(SW_HIDE);
		m_editZ.ShowWindow(SW_HIDE);

		GetDlgItem(IDC_STATIC_X)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_STATIC_Y)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_STATIC_Z)->ShowWindow(SW_HIDE);

		GetMenu()->EnableMenuItem(ID_INSERT_SKYDOME, MF_DISABLED | MF_GRAYED);
		m_mainBtnSky.ShowWindow(SW_HIDE);

		GetMenu()->EnableMenuItem(ID_INSERT_WATER, MF_DISABLED | MF_GRAYED);
		m_mainBtnWater.ShowWindow(SW_HIDE);

		GetMenu()->EnableMenuItem(ID_INSERT_SOUND_STATIC, MF_DISABLED | MF_GRAYED);
		m_mainBtnStaticSound.ShowWindow(SW_HIDE);

		GetMenu()->EnableMenuItem(ID_INSERT_SOUND_AMBIENT, MF_DISABLED | MF_GRAYED);
		m_mainBtnAmbientSound.ShowWindow(SW_HIDE);

		GetMenu()->EnableMenuItem(ID_INSERT_LIGHT, MF_DISABLED | MF_GRAYED);
		m_mainBtnLight.ShowWindow(SW_HIDE);

		GetMenu()->EnableMenuItem(ID_INSERT_TRIGGER, MF_DISABLED | MF_GRAYED);
		m_mainBtnTrigger.ShowWindow(SW_HIDE);

		GetDlgItem(IDC_STATIC_PHYSX_ELEMENTS)->ShowWindow(SW_SHOW);
		m_listBoxPhysXElements.ShowWindow(SW_SHOW);
		m_btnRemovePhysX.ShowWindow(SW_SHOW);

		m_mainBtnWaterAttach.ShowWindow(SW_HIDE);

		GetDlgItem(IDC_STATIC_ENGINE_OBJECTS)->ShowWindow(SW_HIDE);
		m_listBoxEngineObjects.ShowWindow(SW_HIDE);
		m_btnEngineObjectProperties.ShowWindow(SW_HIDE);
		m_btnRemoveEngineObject.ShowWindow(SW_HIDE);
		//Engine Objects' icons
		CRect windowRect;
		GetClientRect(&windowRect);

		CRect rcRect;

		CInt fivePercent;
		CInt previousRight;
		//Initialize main *New* button here
		CFloat aspectRatio = fabs(CFloat(windowRect.bottom - windowRect.top) / CFloat(windowRect.right - windowRect.left));
		fivePercent = ((5 * (windowRect.right - windowRect.left) / 100) * aspectRatio);
		m_horizontalPointFivePercent = CInt(0.5 * (windowRect.right - windowRect.left) / 100);

		CFloat ButtonSizeAndGap = 7.0;
		CFloat ButtonSize = 5.0f;
		m_startLeftButtons = 7; //six.five percent

		////Initialize main *Camera* button here
		//rcRect.left = 2 * m_horizontalPointFivePercent;
		//rcRect.right = rcRect.left + fivePercent;
		//rcRect.top = CInt(m_startLeftButtons * (windowRect.bottom - windowRect.top) / 100);
		//rcRect.bottom = rcRect.top + (ButtonSize * (windowRect.bottom - windowRect.top) / 100);

		//m_mainBtnPlayer.MoveWindow(rcRect);
		//m_mainBtnPlayer.UpdateWindow();


		GetMenu()->EnableMenuItem(ID_IMPORT_COLLADA, MF_ENABLED);
		m_mainBtnImportCollada.ShowWindow(SW_SHOW);

		GetMenu()->EnableMenuItem(ID_FILE_OPEN, MF_DISABLED | MF_GRAYED);
		m_mainBtnOpen.ShowWindow(SW_HIDE);

		GetMenu()->EnableMenuItem(ID_EDIT_TRANSLATE, MF_DISABLED | MF_GRAYED);
		m_mainBtnTranslate.ShowWindow(SW_HIDE);

		GetMenu()->EnableMenuItem(ID_EDIT_ROTATE, MF_DISABLED | MF_GRAYED);
		m_mainBtnRotate.ShowWindow(SW_HIDE);

		GetMenu()->EnableMenuItem(ID_EDIT_SCALE, MF_DISABLED | MF_GRAYED);
		m_mainBtnScale.ShowWindow(SW_HIDE);

		GetMenu()->EnableMenuItem(ID_INSERT_PLAYER, MF_DISABLED | MF_GRAYED);
		m_mainBtnPlayer.ShowWindow(SW_HIDE);

		GetMenu()->EnableMenuItem(ID_PUBLISH_PROJECT, MF_DISABLED | MF_GRAYED);
		GetMenu()->EnableMenuItem(ID_PROJECT_SETCURRENT, MF_DISABLED | MF_GRAYED);
		GetMenu()->EnableMenuItem(ID_PROJECT_EDITCURRENT, MF_DISABLED | MF_GRAYED);
		GetMenu()->EnableMenuItem(ID_PROJECT_EDITVSCENESOFCURRENTPROJECT, MF_DISABLED | MF_GRAYED);
		GetMenu()->EnableMenuItem(ID_PROJECT_NEW, MF_DISABLED | MF_GRAYED);
		m_mainBtnPublishSolution.ShowWindow(SW_HIDE);

		m_mainBtnMaterial.ShowWindow(SW_SHOW);
		m_mainBtnPhysXEditor.ShowWindow(SW_SHOW);

		GetMenu()->EnableMenuItem(ID_TOOLS_CULLFACES, MF_ENABLED);
		GetMenu()->EnableMenuItem(ID_TOOLS_MATERIALEDITOR, MF_ENABLED);
		GetMenu()->EnableMenuItem(ID_TOOLS_PHYSXEDITOR, MF_ENABLED);
		GetMenu()->EnableMenuItem(ID_TOOLS_SCRIPTMANAGER, MF_ENABLED);
		GetMenu()->EnableMenuItem(ID_TOOLS_IMPORTOPTIONS, MF_ENABLED);
		GetMenu()->EnableMenuItem(ID_TOOLS_CURRENTSCENEOPTIONS, MF_DISABLED | MF_GRAYED);
		GetMenu()->EnableMenuItem(ID_GEOMETRY_AMBIENTCOLOR, MF_ENABLED);
		GetMenu()->EnableMenuItem(ID_TOOLS_GEOMETRYBASEDSELECTION, MF_ENABLED);
		
		m_startLeftButtons = 6;

		rcRect.left = CInt(1 * (windowRect.right - windowRect.left) / 100);
		rcRect.right = rcRect.left + fivePercent;
		rcRect.top = CInt((m_startLeftButtons - 5.5)  * (windowRect.bottom - windowRect.top) / 100);
		rcRect.bottom = rcRect.top + (5 * (windowRect.bottom - windowRect.top) / 100);
		//ScreenToClient( &rcRect );
		m_mainBtnNew.MoveWindow(rcRect);
		m_mainBtnNew.UpdateWindow();

		previousRight = rcRect.right;
		rcRect.top = CInt((m_startLeftButtons - 5.5)  * (windowRect.bottom - windowRect.top) / 100);
		rcRect.bottom = rcRect.top + (5 * (windowRect.bottom - windowRect.top) / 100);
		rcRect.left = previousRight + m_horizontalPointFivePercent;
		rcRect.right = rcRect.left + fivePercent;
		m_mainBtnImportCollada.MoveWindow(rcRect);

		previousRight = rcRect.right;
		rcRect.left = previousRight + m_horizontalPointFivePercent;
		rcRect.right = rcRect.left + fivePercent;
		m_mainBtnSave.MoveWindow(rcRect);

		previousRight = rcRect.right;
		rcRect.left = previousRight + m_horizontalPointFivePercent;
		rcRect.right = rcRect.left + fivePercent;
		m_mainBtnSaveAs.MoveWindow(rcRect);

		previousRight = rcRect.right;
		rcRect.left = previousRight + m_horizontalPointFivePercent;
		rcRect.right = rcRect.left + fivePercent;
		m_mainBtnPrefab.MoveWindow(rcRect);

		previousRight = rcRect.right;
		rcRect.left = previousRight + m_horizontalPointFivePercent;
		rcRect.right = rcRect.left + fivePercent;
		m_mainBtnCameraAndRenderingManager.MoveWindow(rcRect);

		//Initialize main *Previous Animation* button here
		previousRight = rcRect.right;
		rcRect.left = previousRight + m_horizontalPointFivePercent;
		rcRect.right = rcRect.left + fivePercent;

		m_mainBtnPrevAnim.MoveWindow(rcRect);
		m_mainBtnPrevAnim.ShowWindow(SW_SHOW);
		m_mainBtnPrevAnim.UpdateWindow();
		m_mainBtnPrevAnim.EnableWindow(FALSE);

		//Initialize main *Play Animation* button here
		previousRight = rcRect.right;
		rcRect.left = previousRight + m_horizontalPointFivePercent;
		rcRect.right = rcRect.left + fivePercent;

		m_mainBtnPlayAnim.MoveWindow(rcRect);
		m_mainBtnPlayAnim.ShowWindow(SW_HIDE);
		m_mainBtnPlayAnim.UpdateWindow();
		m_mainBtnPlayAnim.EnableWindow(FALSE);

		//Initialize main *Pause Animation* button here

		m_mainBtnPauseAnim.MoveWindow(rcRect);
		m_mainBtnPauseAnim.ShowWindow(SW_SHOW);
		m_mainBtnPauseAnim.UpdateWindow();
		m_mainBtnPauseAnim.EnableWindow(FALSE);

		//Initialize main *Next Animation* button here
		previousRight = rcRect.right;
		rcRect.left = previousRight + m_horizontalPointFivePercent;
		rcRect.right = rcRect.left + fivePercent;

		m_mainBtnNextAnim.MoveWindow(rcRect);
		m_mainBtnNextAnim.ShowWindow(SW_SHOW);
		m_mainBtnNextAnim.UpdateWindow();
		m_mainBtnNextAnim.EnableWindow(FALSE);

		previousRight = rcRect.right;
		rcRect.left = previousRight + m_horizontalPointFivePercent;
		rcRect.right = rcRect.left + fivePercent;
		m_mainBtnTimer.MoveWindow(rcRect);
		m_mainBtnTimerPause.MoveWindow(rcRect);

		previousRight = rcRect.right;
		rcRect.left = previousRight + m_horizontalPointFivePercent;
		rcRect.right = rcRect.left + fivePercent;
		m_mainBtnMaterial.MoveWindow(rcRect);

		previousRight = rcRect.right;
		rcRect.left = previousRight + m_horizontalPointFivePercent;
		rcRect.right = rcRect.left + fivePercent;
		m_mainBtnPhysXEditor.MoveWindow(rcRect);

		previousRight = rcRect.right;
		rcRect.left = previousRight + m_horizontalPointFivePercent;
		rcRect.right = rcRect.left + fivePercent;
		m_mainBtnScriptManager.MoveWindow(rcRect);

		previousRight = rcRect.right;
		rcRect.left = previousRight + m_horizontalPointFivePercent;
		rcRect.right = rcRect.left + fivePercent;
		m_mainBtnTestActive.MoveWindow(rcRect);
		m_mainBtnTestDeactive.MoveWindow(rcRect);

		previousRight = rcRect.right;
		rcRect.left = previousRight + m_horizontalPointFivePercent;
		rcRect.right = rcRect.left + fivePercent;

		m_mainBtnConsole.MoveWindow(rcRect);
		m_mainBtnConsole.UpdateWindow();

	}
	else
	{
		Invalidate(TRUE);

		m_editX.ShowWindow(SW_SHOW);
		m_editY.ShowWindow(SW_SHOW);
		m_editZ.ShowWindow(SW_SHOW);

		m_editX.SetWindowTextA("\n");
		m_editY.SetWindowTextA("\n");
		m_editZ.SetWindowTextA("\n");

		GetDlgItem(IDC_STATIC_X)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_STATIC_Y)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_STATIC_Z)->ShowWindow(SW_SHOW);


		GetMenu()->EnableMenuItem(ID_INSERT_SKYDOME, MF_ENABLED);
		m_mainBtnSky.ShowWindow(SW_SHOW);

		GetMenu()->EnableMenuItem(ID_INSERT_WATER, MF_ENABLED);
		m_mainBtnWater.ShowWindow(SW_SHOW);

		GetMenu()->EnableMenuItem(ID_INSERT_SOUND_STATIC, MF_ENABLED);
		m_mainBtnStaticSound.ShowWindow(SW_SHOW);

		GetMenu()->EnableMenuItem(ID_INSERT_SOUND_AMBIENT, MF_ENABLED);
		m_mainBtnAmbientSound.ShowWindow(SW_SHOW);

		GetMenu()->EnableMenuItem(ID_INSERT_LIGHT, MF_ENABLED);
		m_mainBtnLight.ShowWindow(SW_SHOW);

		GetMenu()->EnableMenuItem(ID_INSERT_PLAYER, MF_ENABLED);
		m_mainBtnPlayer.ShowWindow(SW_SHOW);

		GetMenu()->EnableMenuItem(ID_INSERT_TRIGGER, MF_ENABLED);
		m_mainBtnTrigger.ShowWindow(SW_SHOW);

		m_mainBtnPrevAnim.ShowWindow(SW_HIDE);
		m_mainBtnPrevAnim.EnableWindow(FALSE);

		m_mainBtnPlayAnim.ShowWindow(SW_HIDE);
		m_mainBtnPlayAnim.EnableWindow(FALSE);

		m_mainBtnPauseAnim.ShowWindow(SW_HIDE);
		m_mainBtnPauseAnim.EnableWindow(FALSE);

		m_mainBtnNextAnim.ShowWindow(SW_HIDE);
		m_mainBtnNextAnim.EnableWindow(FALSE);

		GetDlgItem(IDC_STATIC_PHYSX_ELEMENTS)->ShowWindow(SW_HIDE);
		m_listBoxPhysXElements.ShowWindow(SW_HIDE);
		m_btnRemovePhysX.ShowWindow(SW_HIDE);

		GetDlgItem(IDC_STATIC_ENGINE_OBJECTS)->ShowWindow(SW_SHOW);
		m_listBoxEngineObjects.ShowWindow(SW_SHOW);
		m_btnEngineObjectProperties.ShowWindow(SW_SHOW);
		m_btnRemoveEngineObject.ShowWindow(SW_SHOW);

		//Engine Objects' icons
		CRect windowRect;
		GetClientRect(&windowRect);

		CRect rcRect;

		CInt fivePercent;
		CInt previousRight;
		//Initialize main *New* button here
		CFloat aspectRatio = fabs(CFloat(windowRect.bottom - windowRect.top) / CFloat(windowRect.right - windowRect.left));
		fivePercent = ((5 * (windowRect.right - windowRect.left) / 100) * aspectRatio);
		m_horizontalPointFivePercent = CInt(0.5 * (windowRect.right - windowRect.left) / 100);

		CFloat ButtonSizeAndGap = 7.0f;
		CFloat ButtonSize = 5.0f;
		m_startLeftButtons = 7; //six.five percent

		//Initialize main *Sky* button here
		rcRect.left = 2 * m_horizontalPointFivePercent;
		rcRect.right = rcRect.left + fivePercent;
		rcRect.top = CInt((CFloat)m_startLeftButtons * (windowRect.bottom - windowRect.top) / 100.0f);
		rcRect.bottom = rcRect.top + (ButtonSize * CFloat(windowRect.bottom - windowRect.top) / 100.0f);

		m_mainBtnSky.MoveWindow(rcRect);
		m_mainBtnSky.UpdateWindow();

		//Initialize main *Water* button here
		rcRect.top = CInt((m_startLeftButtons + ButtonSizeAndGap) * (windowRect.bottom - windowRect.top) / 100.0f);
		rcRect.bottom = rcRect.top + (ButtonSize * CFloat(windowRect.bottom - windowRect.top) / 100.0f);

		m_mainBtnWater.MoveWindow(rcRect);
		m_mainBtnWater.UpdateWindow();

		//Initialize main *Light* button here
		rcRect.top = CInt((m_startLeftButtons + 2 * ButtonSizeAndGap) * (windowRect.bottom - windowRect.top) / 100.0f);
		rcRect.bottom = rcRect.top + (ButtonSize * CFloat(windowRect.bottom - windowRect.top) / 100.0f);

		m_mainBtnLight.MoveWindow(rcRect);
		m_mainBtnLight.UpdateWindow();

		//Initialize main *Static Sound* button here
		rcRect.top = CInt((m_startLeftButtons + 3 * ButtonSizeAndGap) * (windowRect.bottom - windowRect.top) / 100.0f);
		rcRect.bottom = rcRect.top + (ButtonSize * CFloat(windowRect.bottom - windowRect.top) / 100.0f);

		m_mainBtnStaticSound.MoveWindow(rcRect);
		m_mainBtnStaticSound.UpdateWindow();

		//Initialize main *Ambient Sound* button here
		rcRect.top = CInt((m_startLeftButtons + 4 * ButtonSizeAndGap) * (windowRect.bottom - windowRect.top) / 100.0f);
		rcRect.bottom = rcRect.top + (ButtonSize * CFloat(windowRect.bottom - windowRect.top) / 100.0f);

		m_mainBtnAmbientSound.MoveWindow(rcRect);
		m_mainBtnAmbientSound.UpdateWindow();

		//Initialize main *Player* button here
		rcRect.top = CInt((m_startLeftButtons + 5 * ButtonSizeAndGap) * (windowRect.bottom - windowRect.top) / 100.0f);
		rcRect.bottom = rcRect.top + (ButtonSize * CFloat(windowRect.bottom - windowRect.top) / 100.0f);

		m_mainBtnPlayer.MoveWindow(rcRect);
		m_mainBtnPlayer.UpdateWindow();

		//Initialize main *Player* button here
		rcRect.top = CInt((m_startLeftButtons + 6 * ButtonSizeAndGap) * (windowRect.bottom - windowRect.top) / 100.0f);
		rcRect.bottom = rcRect.top + (ButtonSize * CFloat(windowRect.bottom - windowRect.top) / 100.0f);

		m_mainBtnTrigger.MoveWindow(rcRect);
		m_mainBtnTrigger.UpdateWindow();

		//Initialize main *Console* button here
		rcRect.top = CInt((m_startLeftButtons + 6 * ButtonSizeAndGap) * (windowRect.bottom - windowRect.top) / 100.0f);
		rcRect.bottom = rcRect.top + (ButtonSize * CFloat(windowRect.bottom - windowRect.top) / 100.0f);

		m_mainBtnConsole.MoveWindow(rcRect);
		m_mainBtnConsole.UpdateWindow();

		GetMenu()->EnableMenuItem(ID_IMPORT_COLLADA, MF_DISABLED | MF_GRAYED);
		m_mainBtnImportCollada.ShowWindow(SW_HIDE);

		GetMenu()->EnableMenuItem(ID_FILE_OPEN, MF_ENABLED);
		m_mainBtnOpen.ShowWindow(SW_SHOW);

		GetMenu()->EnableMenuItem(ID_FILE_SAVE, MF_ENABLED);
		m_mainBtnSave.ShowWindow(SW_SHOW);

		GetMenu()->EnableMenuItem(ID_FILE_SAVEAS, MF_ENABLED);
		m_mainBtnSaveAs.ShowWindow(SW_SHOW);

		GetMenu()->EnableMenuItem(ID_EDIT_TRANSLATE, MF_ENABLED);
		m_mainBtnTranslate.ShowWindow(SW_SHOW);

		GetMenu()->EnableMenuItem(ID_EDIT_ROTATE, MF_ENABLED);
		m_mainBtnRotate.ShowWindow(SW_SHOW);

		GetMenu()->EnableMenuItem(ID_EDIT_SCALE, MF_ENABLED);
		m_mainBtnScale.ShowWindow(SW_SHOW);

		GetMenu()->EnableMenuItem(ID_PUBLISH_PROJECT, MF_ENABLED);
		GetMenu()->EnableMenuItem(ID_PROJECT_SETCURRENT, MF_ENABLED);
		GetMenu()->EnableMenuItem(ID_PROJECT_EDITCURRENT, MF_ENABLED);
		GetMenu()->EnableMenuItem(ID_PROJECT_EDITVSCENESOFCURRENTPROJECT, MF_ENABLED);
		GetMenu()->EnableMenuItem(ID_PROJECT_NEW, MF_ENABLED);
		m_mainBtnPublishSolution.ShowWindow(SW_SHOW);

		m_mainBtnMaterial.ShowWindow(SW_HIDE);
		m_mainBtnPhysXEditor.ShowWindow(SW_HIDE);

		GetMenu()->EnableMenuItem(ID_TOOLS_CULLFACES, MF_DISABLED | MF_GRAYED);
		GetMenu()->EnableMenuItem(ID_TOOLS_MATERIALEDITOR, MF_DISABLED | MF_GRAYED);
		GetMenu()->EnableMenuItem(ID_TOOLS_PHYSXEDITOR, MF_DISABLED | MF_GRAYED);
		GetMenu()->EnableMenuItem(ID_TOOLS_SCRIPTMANAGER, MF_DISABLED | MF_GRAYED);
		GetMenu()->EnableMenuItem(ID_TOOLS_IMPORTOPTIONS, MF_DISABLED | MF_GRAYED);
		GetMenu()->EnableMenuItem(ID_TOOLS_CURRENTSCENEOPTIONS, MF_ENABLED);
		GetMenu()->EnableMenuItem(ID_GEOMETRY_AMBIENTCOLOR, MF_DISABLED | MF_GRAYED);
		GetMenu()->EnableMenuItem(ID_TOOLS_GEOMETRYBASEDSELECTION, MF_DISABLED | MF_GRAYED);

		m_startLeftButtons = 6;

		rcRect.left = CInt(1 * (windowRect.right - windowRect.left) / 100);
		rcRect.right = rcRect.left + fivePercent;
		rcRect.top = CInt((m_startLeftButtons - 5.5)  * (windowRect.bottom - windowRect.top) / 100);
		rcRect.bottom = rcRect.top + (5 * (windowRect.bottom - windowRect.top) / 100);
		//ScreenToClient( &rcRect );
		m_mainBtnNew.MoveWindow(rcRect);
		m_mainBtnNew.UpdateWindow();

		previousRight = rcRect.right;
		rcRect.top = CInt((m_startLeftButtons - 5.5)  * (windowRect.bottom - windowRect.top) / 100);
		rcRect.bottom = rcRect.top + (5 * (windowRect.bottom - windowRect.top) / 100);
		rcRect.left = previousRight + m_horizontalPointFivePercent;
		rcRect.right = rcRect.left + fivePercent;
		m_mainBtnOpen.MoveWindow(rcRect);

		previousRight = rcRect.right;
		rcRect.left = previousRight + m_horizontalPointFivePercent;
		rcRect.right = rcRect.left + fivePercent;
		m_mainBtnSave.MoveWindow(rcRect);

		previousRight = rcRect.right;
		rcRect.left = previousRight + m_horizontalPointFivePercent;
		rcRect.right = rcRect.left + fivePercent;
		m_mainBtnSaveAs.MoveWindow(rcRect);

		previousRight = rcRect.right;
		rcRect.left = previousRight + m_horizontalPointFivePercent;
		rcRect.right = rcRect.left + fivePercent;
		m_mainBtnTranslate.MoveWindow(rcRect);

		previousRight = rcRect.right;
		rcRect.left = previousRight + m_horizontalPointFivePercent;
		rcRect.right = rcRect.left + fivePercent;
		m_mainBtnRotate.MoveWindow(rcRect);

		previousRight = rcRect.right;
		rcRect.left = previousRight + m_horizontalPointFivePercent;
		rcRect.right = rcRect.left + fivePercent;
		m_mainBtnScale.MoveWindow(rcRect);

		previousRight = rcRect.right;
		rcRect.left = previousRight + m_horizontalPointFivePercent;
		rcRect.right = rcRect.left + fivePercent;
		m_mainBtnPrefab.MoveWindow(rcRect);

		previousRight = rcRect.right;
		rcRect.left = previousRight + m_horizontalPointFivePercent;
		rcRect.right = rcRect.left + fivePercent;
		m_mainBtnPublishSolution.MoveWindow(rcRect);

		previousRight = rcRect.right;
		rcRect.left = previousRight + m_horizontalPointFivePercent;
		rcRect.right = rcRect.left + fivePercent;
		m_mainBtnCameraAndRenderingManager.MoveWindow(rcRect);

		previousRight = rcRect.right;
		rcRect.left = previousRight + m_horizontalPointFivePercent;
		rcRect.right = rcRect.left + fivePercent;
		m_mainBtnWaterAttach.MoveWindow(rcRect);
		m_mainBtnWaterAttach.ShowWindow(SW_SHOW);

		previousRight = rcRect.right;
		rcRect.left = previousRight + m_horizontalPointFivePercent;
		rcRect.right = rcRect.left + fivePercent;
		m_mainBtnScriptManager.MoveWindow(rcRect);

		//previousRight = rcRect.right;
		//rcRect.left = previousRight + m_horizontalPointFivePercent;
		//rcRect.right = rcRect.left + fivePercent;
		//m_mainBtnPrevAnim.MoveWindow(rcRect);

		//previousRight = rcRect.right;
		//rcRect.left = previousRight + m_horizontalPointFivePercent;
		//rcRect.right = rcRect.left + fivePercent;
		//m_mainBtnPlayAnim.MoveWindow(rcRect);
		//m_mainBtnPauseAnim.MoveWindow(rcRect);

		//previousRight = rcRect.right;
		//rcRect.left = previousRight + m_horizontalPointFivePercent;
		//rcRect.right = rcRect.left + fivePercent;
		//m_mainBtnNextAnim.MoveWindow(rcRect);

		previousRight = rcRect.right;
		rcRect.left = previousRight + m_horizontalPointFivePercent;
		rcRect.right = rcRect.left + fivePercent;
		m_mainBtnTimerPause.MoveWindow(rcRect);
		m_mainBtnTimer.MoveWindow(rcRect);

		previousRight = rcRect.right;
		rcRect.left = previousRight + m_horizontalPointFivePercent;
		rcRect.right = rcRect.left + fivePercent;
		m_mainBtnTestActive.MoveWindow(rcRect);
		m_mainBtnTestDeactive.MoveWindow(rcRect);

		previousRight = rcRect.right;
		rcRect.left = previousRight + m_horizontalPointFivePercent;
		rcRect.right = rcRect.left + fivePercent;

		m_mainBtnConsole.MoveWindow(rcRect);
		m_mainBtnConsole.UpdateWindow();

	}
}

CBool CVandaEngine1Dlg::OnMenuClickedNew( CBool askQuestion )
{
	CInt iResponse;
	if( askQuestion )
	{ 
		if( g_scene.size() > 0 || g_engineLights.size() > 0 || g_engineWaters.size() > 0 || g_menu.m_insertAndShowSky || g_menu.m_insertAmbientSound || g_engineStaticSounds.size() > 0 ||  g_multipleView->m_nx->m_hasScene )
			iResponse = MessageBox( "Do you want to save your changes?", "Warning" , MB_YESNOCANCEL|MB_ICONSTOP);
	}

	if (iResponse == IDYES) //save changes
	{
		if( g_multipleView->m_enableTimer )
			g_multipleView->EnableTimer( CFalse );
		if (g_editorMode == eMODE_PREFAB)
			OnMenuClickedSavePrefabAs(CFalse);
		else
			OnMenuClickedSaveAs(CFalse);
		if (g_multipleView->m_enableTimer)
			g_multipleView->EnableTimer( CTrue );

	}
	else if (iResponse == IDCANCEL)
		return CFalse;

	g_clickedNew = CTrue;
	SetCapture();
	SetCursor( LoadCursorFromFile( "Assets/Engine/Icons/progress.ani") );

	if (g_physXProperties.m_bDebugMode)
		PrintInfo("\nPhysX debug deactivated");

	g_dofProperties.Reset();
	g_fogProperties.Reset();
	g_bloomProperties.Reset();
	g_lightProperties.Reset();
	g_shadowProperties.Reset();
	g_physXProperties.Reset();
	if(!g_vandaDemo)
		g_shadowProperties.m_enable = CTrue;
	g_pathProperties.Reset();
	g_sceneBanner.SetBannerPath("Assets/Engine/Textures/Loading.dds");
	g_sceneBanner.ClearVScenes();
	CDelete(g_externalPhysX);
	g_extraTexturesNamingConventions.Reset();
	g_sceneManagerObjectsPerSplit = 15;
	//g_options.Reset();
	g_useGlobalAmbientColor = CFalse;
	g_globalAmbientColor.r = g_globalAmbientColor.g = g_globalAmbientColor.b = 0.5f;
	g_globalAmbientColor.a = 1.0f;

	m_engineObjectListIndex = -1;
	m_sceneListIndex = -1;
	m_physXElementListIndex = -1;
	g_updateTextureViaEditor = CFalse;

	g_multipleView->distance_vector.clear();
	g_multipleView->sorted_prefabs.clear();

	//clear geometries
	for (CUInt i = 0; i < g_geometries.size(); i++)
		CDelete(g_geometries[i]);
	g_geometries.clear();

	for (CUInt i = 0; i < g_triggers.size(); i++)
	{
		CDelete(g_triggers[i]);
	}
	g_triggers.clear();

	//clear prefabs
	for (CUInt i = 0; i < g_prefab.size(); i++)
	{
		CDelete(g_prefab[i]);
	}
	g_prefab.clear();

	for (CUInt i = 0; i < g_instancePrefab.size(); i++)
	{
		CDelete(g_instancePrefab[i]);
	}
	g_instancePrefab.clear();

	g_octree->ResetState();
	g_render.SetScene( NULL );
	if( g_currentCameraType == eCAMERA_COLLADA )
	{
		g_currentCameraType = eCAMERA_DEFAULT_FREE;
		g_render.SetActiveInstanceCamera(g_render.GetDefaultInstanceCamera());

		g_multipleView->m_lockInput = CFalse;
	}
	g_render.GetDefaultInstanceCamera()->MoveTransform2(10, 5, 10);
	g_render.GetDefaultInstanceCamera()->SetPanAndTilt2(43, -25);

	Cpy(g_shadowProperties.m_directionalLightName, "\n" );
	if( g_multipleView->m_nx->m_hasScene || gPhysXscene )
	{
		if(!g_clickedOpen)
			ResetPhysX();
		g_physXProperties.m_bDebugMode = CFalse;
		GetMenu()->CheckMenuItem( ID_PHYSICS_DEBUGMODE, MF_UNCHECKED );

	}
	for( std::vector<CWater*>::iterator it = g_engineWaters.begin(); it != g_engineWaters.end(); it++ )
	{
		CDelete( *it );
	}
	if( g_engineWaters.size() > 0 )
		g_engineWaters.clear();

	for( std::vector<CStaticSound*>::iterator it = g_engineStaticSounds.begin(); it != g_engineStaticSounds.end(); it++ )
	{
		CDelete( *it );
	}
	if( g_engineStaticSounds.size() > 0 )
		g_engineStaticSounds.clear();

	if( g_engineObjectNames.size() > 0 )
		g_engineObjectNames.clear();
	//clear the scene
	for( std::vector<CScene*>::iterator it = g_scene.begin(); it != g_scene.end(); it++ )
		CDelete( *it );
	//Clear the vctor objects
	if( g_scene.size() > 0 ) 
		g_scene.clear();
		
	//clear all the default engines here
	for( std::vector<CInstanceLight*>::iterator it = g_engineLights.begin(); it != g_engineLights.end(); it++ )
	{
		CDelete( (*it)->m_abstractLight ) ;
		CDelete( *it );
	}
	//Clear the vctor objects
	if( g_engineLights.size() > 0 )
		g_engineLights.clear();


	for( std::vector<CImage*>::iterator it = g_images.begin(); it != g_images.end(); it++ )
	{
		CDelete( *it );
	}

	if( g_images.size() > 0 )
		g_images.clear();

	for( std::vector<CImage*>::iterator it = g_waterImages.begin(); it != g_waterImages.end(); it++ )
	{
		CDelete( *it );
	}
	if( g_waterImages.size() > 0 )
		g_waterImages.clear();

	for( std::vector<COpenALSoundBuffer*>::iterator it = g_soundBuffers.begin(); it != g_soundBuffers.end(); it++ )
	{
		CDelete( *it );
	}
	g_soundBuffers.clear();

	g_menu.m_insertAndShowSky = CFalse;
	CDelete( g_skyDome );
	GetMenu()->EnableMenuItem(ID_INSERT_SKYDOME, MF_ENABLED);
	m_mainBtnSky.EnableWindow(TRUE);

	//delete ambient sound
	if( g_menu.m_insertAmbientSound )
	{
		g_multipleView->m_soundSystem->StopALSound( *(g_multipleView->m_ambientSound->GetSoundSource()) );
		alSourcei(g_multipleView->m_ambientSound->GetSoundSource()->GetSource(), AL_BUFFER, AL_NONE); 
		CDelete( g_multipleView->m_ambientSound );			
	}
	g_menu.m_insertAmbientSound = CFalse;
	GetMenu()->EnableMenuItem(ID_INSERT_SOUND_AMBIENT, MF_ENABLED);
	m_mainBtnAmbientSound.EnableWindow(TRUE);

	m_savePathName = "\n";
	Cpy(m_strNewPackageName, "\n");
	Cpy(m_strNewPrefabName, "\n");
	Cpy(m_strNewPackageAndPrefabName, "\n");
	Cpy(m_strNewPassword, "\n");

	//Reset name indexes which are used for selection
	g_nameIndex = 1;
	CDelete( g_translateArrow );
	g_translateArrow = CNew( CScene );
	g_reportInfo = CFalse;
	g_shareGeometriesBetweenScenes = CFalse;

	if( !g_translateArrow->Load( "Assets/engine/Arrows/ArrowTranslate.dae", CFalse ) )
	{
		PrintInfo( "\nCouldn't load the arrows", COLOR_RED );
	}
	g_render.SetScene( g_translateArrow );
	g_translateArrow->Update();

	CDelete (g_scaleArrow);
	g_scaleArrow = CNew( CScene );
	if( !g_scaleArrow->Load( "Assets/engine/Arrows/ArrowScale.dae", CFalse ) )
	{
		PrintInfo( "\nCouldn't load the Scale arrow", COLOR_RED );
	}
	g_render.SetScene(g_scaleArrow);
	g_scaleArrow->Update();

	CDelete(g_rotateArrow);
	g_rotateArrow = CNew(CScene);
	if (!g_rotateArrow->Load("Assets/engine/Arrows/ArrowRotate.dae", CFalse))
	{
		PrintInfo("\nCouldn't load the Rotate arrow", COLOR_RED);
	}
	g_render.SetScene(g_rotateArrow);
	g_rotateArrow->Update();

	CDelete( g_negativeZArrow );
	g_negativeZArrow = CNew( CScene );
	if( !g_negativeZArrow->Load( "Assets/engine/Arrows/NegativeZArrow.dae", CFalse ) )
	{
		PrintInfo( "\nCouldn't load the negative Z arrow", COLOR_RED );
	}
	g_render.SetScene(g_negativeZArrow);
	g_negativeZArrow->Update();

	g_showArrow = CFalse;

	CDelete( g_centerArrowScene );

	g_centerArrowScene = CNew( CScene );
	if( !g_centerArrowScene->Load( "Assets/engine/Arrows/ArrowCenter.dae", CFalse ) )
	{
		PrintInfo( "\nCouldn't load the arrows", COLOR_RED );
	}
	g_reportInfo = CTrue;
	g_render.SetScene(g_centerArrowScene); 
	g_centerArrowScene->Update();
	if (g_editorMode == eMODE_PREFAB)
		g_shareGeometriesBetweenScenes = CFalse;
	else
		g_shareGeometriesBetweenScenes = CTrue;

	g_selectedName = g_tempLastEngineObjectSelectedName = g_lastEngineObjectSelectedName = g_multipleView->m_tempSelectedName = -1; 		//no object has been selected
	g_transformObject = CFalse;

	//Delete all items from list boxes

	for (int nItem = m_listBoxScenes.GetItemCount()-1; nItem >= 0 ;nItem-- )
	{
		m_listBoxScenes.DeleteItem(nItem);
	}
	m_btnRemoveScene.EnableWindow( FALSE );
	m_btnSceneProperties.EnableWindow( FALSE );
	for (int nItem2 = m_listBoxObjects.GetItemCount()-1; nItem2 >= 0 ;nItem2-- )
	{
		m_listBoxObjects.DeleteItem(nItem2);
	}
	for (int nItem3 = m_listBoxEngineObjects.GetItemCount()-1; nItem3 >= 0 ;nItem3-- )
	{
		m_listBoxEngineObjects.DeleteItem(nItem3);
	}
	m_btnRemoveEngineObject.EnableWindow( FALSE );
	m_btnEngineObjectProperties.EnableWindow( FALSE );

	for (int nItem4 = m_listBoxPhysXElements.GetItemCount()-1; nItem4 >= 0 ;nItem4-- )
	{
		m_listBoxPhysXElements.DeleteItem(nItem4);
	}
	m_btnRemovePhysX.EnableWindow( FALSE );
	//material editor

	m_dlgEditMaterial->SetNormalBtnState( CFalse );
	m_dlgEditMaterial->SetDirtBtnState( CFalse );
	m_dlgEditMaterial->SetDiffuseBtnState( CFalse );
	m_dlgEditMaterial->SetGlossBtnState( CFalse );

	m_dlgEditMaterial->SetEditBoxNormalMapName( "\n" );
	m_dlgEditMaterial->SetEditBoxDirtMapName( "\n" );
	m_dlgEditMaterial->SetEditBoxDiffuseName( "\n" );
	m_dlgEditMaterial->SetEditBoxGlossMapName( "\n" );

	m_dlgEditMaterial->SetRemoveDirtBtnState( CFalse );
	m_dlgEditMaterial->SetRemoveNormalBtnState( CFalse );
	m_dlgEditMaterial->SetRemoveDiffuseBtnState( CFalse );
	m_dlgEditMaterial->SetRemoveGlossBtnState( CFalse );
	/////

	ex_pBtnNextAnim->EnableWindow( FALSE );
	ex_pBtnPrevAnim->EnableWindow( FALSE );
	ex_pBtnPlayAnim->EnableWindow( FALSE );

	ex_pBtnMaterialEditor->EnableWindow( FALSE );
	GetMenu()->EnableMenuItem( ID_TOOLS_MATERIALEDITOR, MF_DISABLED | MF_GRAYED );

	ex_pBtnPhysXEditor->EnableWindow( FALSE );
	GetMenu()->EnableMenuItem( ID_TOOLS_PHYSXEDITOR, MF_DISABLED | MF_GRAYED );

	//ex_pBtnScriptEditor->EnableWindow( FALSE );
	//GetMenu()->EnableMenuItem( ID_TOOLS_SCRIPTMANAGER, MF_DISABLED | MF_GRAYED );

	Cpy( g_currentVSceneName, "\n" ); //save functions
	Cpy(g_currentPackageAndPrefabName, "\n");
	Cpy(g_currentPrefabName, "\n");
	Cpy(g_currentPackageName, "\n");
	Cpy(g_currentPackagePath, "\n");
	Cpy(g_currentPrefabAndSceneName, "\n");
	Cpy(g_currentInstancePrefabName, "\n");

	g_currentInstancePrefab = NULL;

	if (g_editorMode == eMODE_PREFAB)
	{
		CChar temp[256];
		sprintf(temp, "%s", "Vanda Engine 1.4 : Prefab Mode (Untitled)");
		ex_pVandaEngine1Dlg->SetWindowTextA(temp);
	}
	//clear the console
	ex_pRichEdit->SetWindowTextA( "" );
	g_selectedName = -1;
	SetDialogData3(CFalse);
	PrintInfo( "\nScene cleared successfully" );
	ReleaseCapture();
	g_clickedNew = CFalse;
	return CTrue;
}

CVoid CVandaEngine1Dlg::OnMenuClickedImportCollada()
{
    // Create an Open dialog; the default file name extension is ".bmp".
	CFileDialog dlgOpen(TRUE, _T("*.dae"), _T(""), OFN_HIDEREADONLY|OFN_OVERWRITEPROMPT | OFN_NOCHANGEDIR,
		_T("COLLADA File (*.dae)|*.dae||"), NULL, NULL);

	if( dlgOpen.DoModal() == IDOK )
	{
		//CString fileName( dlgOpen.GetFileName() );
		//for example this code converts 'test.dae' to 'data\test\test.dae'
		//CChar fileWithoutDot[MAX_NAME_SIZE];
		//Cpy( fileWithoutDot, (char*)fileName.GetString() );
		//GetWithoutDot( fileWithoutDot );
		//CChar filePath[MAX_NAME_SIZE];
		//sprintf( filePath, "%s%s", g_pathProperties.m_meshPath, (char*)fileName.GetString() );
		CString fileName( dlgOpen.GetPathName() );
		CString PurefileName( dlgOpen.GetFileName());

		//check if the scene already loaded
		CBool foundTarget = CFalse;
		Cpy(g_currentPrefabAndSceneName, "\n");

		//check for _LOD1, _LOD2, _LOD3 in prefab mode
		if (g_editorMode == eMODE_PREFAB)
		{
			if (!(CmpIn(PurefileName.GetString(), "_LOD1") || CmpIn(PurefileName.GetString(), "_LOD2") || CmpIn(PurefileName.GetString(), "_LOD3")))
			{
				PrintInfo("\nCouldn't find _LOD1, _LOD2, or LOD3 at the end of file name", COLOR_RED);
				return;
			}
		}

		for( CUInt i = 0; i < g_scene.size(); i++ )
		{
			if( Cmp( g_scene[i]->m_pureFileName, PurefileName.GetString()  ) )
			{
				if( MessageBox( "This scene already exists! Do you want to over write it?", "Vanda Engine 1 Warning", MB_ICONWARNING | MB_YESNO ) == IDYES)
				{
					for (int nItem = m_listBoxScenes.GetItemCount()-1; nItem >= 0 ;nItem-- )
					{
						CChar text[1204];
						m_listBoxScenes.GetItemText(nItem,0, text, 1024);
						if( Cmp( g_scene[i]->m_pureFileName, text  ) )
						{
							//select the item
							m_listBoxScenes.SetItemState(nItem, LVIS_SELECTED, LVIS_SELECTED | LVIS_FOCUSED );
							m_listBoxScenes.SetSelectionMark(nItem);
							m_listBoxScenes.UpdateWindow();
							m_askRemoveScene = CFalse;
							OnBnClickedBtnRemoveScene();
							m_askRemoveScene = CTrue;
							foundTarget = CTrue;
							break;
						}
					}
					if( foundTarget )
						break;
				}
				else
				{
					PrintInfo( "\nImporting the scene cancelled by the user" );
					return;
				}
			}
		} //check

		if (CmpIn(PurefileName.GetString(), "_LOD1"))
		{
			for (CUInt i = 0; i < g_scene.size(); i++)
			{
				if (CmpIn(g_scene[i]->m_pureFileName, "_LOD1"))
				{
					MessageBox("Up to one LOD 1 is supported.\nPlease remove current LOD 1 and try again.", "Vanda Engine Error", MB_OK | MB_ICONERROR);
					return;
				}
			}
		}
		else if (CmpIn(PurefileName.GetString(), "_LOD2"))
		{
			for (CUInt i = 0; i < g_scene.size(); i++)
			{
				if (CmpIn(g_scene[i]->m_pureFileName, "_LOD2"))
				{
					MessageBox("Up to one LOD 2 is supported.\nPlease remove current LOD 2 and try again.", "Vanda Engine Error", MB_OK | MB_ICONERROR);
					return;
				}
			}
		}
		else if (CmpIn(PurefileName.GetString(), "_LOD3"))
		{
			for (CUInt i = 0; i < g_scene.size(); i++)
			{
				if (CmpIn(g_scene[i]->m_pureFileName, "_LOD3"))
				{
					MessageBox("Up to one LOD 3 is supported.\nPlease remove current LOD 3 and try again.", "Vanda Engine Error", MB_OK | MB_ICONERROR);
					return;
				}
			}
		}

		SetCapture();
		SetCursor( LoadCursorFromFile( "Assets/Engine/Icons/progress.ani") );
		g_octree->ResetState();
		g_importColladaImages = CTrue;

		CScene * tempScene = new CScene();

		g_useOriginalPathOfDAETextures = CTrue;
		g_scene.push_back(tempScene);

		if( tempScene->Load( (char*)fileName.GetString() /*filePath*/, CTrue, CFalse ) )
		{
			tempScene->SetClipIndex( 0 );
			tempScene->BlendCycle(tempScene->GetCurrentClipIndex(), 1.0f, 0.0f);

			//save functions/////////////////////////////////
			for (CUInt index = 0; index < g_allPrefabNames.size(); index++)
			{
				tempScene->AddPrefabToList( g_allPrefabNames[index], CTrue ); //Write to zip file and copy the textures
			}
			//save functions/////////////////////////////////
			g_currentScene = tempScene; //mark the current scene. Save functions
			tempScene->SetupDiffuseTexture();
			PrintInfo( "\nReading Gloss Maps...", COLOR_GREEN );
			tempScene->SetupGlossTexture();
			PrintInfo( "\nReading Normal Maps...", COLOR_GREEN );
			tempScene->SetupNormalTexture();
			PrintInfo( "\nReading Dirt Maps...", COLOR_GREEN );
			tempScene->SetupDirtTexture();

			InsertItemToSceneList( tempScene->m_pureFileName );

			//remove the current contents of the object list
			for (int nItem = m_listBoxObjects.GetItemCount()-1; nItem >= 0 ;nItem-- )
			{
				m_listBoxObjects.DeleteItem(nItem);
			}

			for( CUInt j = 0; j < tempScene->m_images.size(); j++ )
			{
				InsertItemToObjectList( tempScene->m_images[j]->GetName(), eOBJECTLIST_IMAGE);
			}
			for( CUInt j = 0; j < tempScene->m_effects.size(); j++ )
			{
				InsertItemToObjectList( tempScene->m_effects[j]->GetName(), eOBJECTLIST_EFFECT);
			}
			for( CUInt j = 0; j < tempScene->m_materials.size(); j++ )
			{
				InsertItemToObjectList( tempScene->m_materials[j]->GetName(), eOBJECTLIST_MATERIAL);
			}
			//animations
			for( CUInt j = 0; j < tempScene->m_animations.size(); j++ )
			{
				InsertItemToObjectList( tempScene->m_animations[j]->GetName(), eOBJECTLIST_ANIMATION);
			}
			for( CUInt j = 0; j < tempScene->m_animationClips.size(); j++ )
			{
				InsertItemToObjectList( tempScene->m_animationClips[j]->GetName(), eOBJECTLIST_ANIMATIONCLIP);
			}
			//lights
			for( CUInt j = 0; j < tempScene->m_lights.size(); j++ )
			{
				InsertItemToObjectList( tempScene->m_lights[j]->GetName(), eOBJECTLIST_LIGHT);
			}
			//for( CUInt j = 0; j < tempScene->m_lightInstances.size(); j++ )
			//{
			//	InsertItemToObjectList( tempScene->m_lightInstances[j]->GetName());
			//}
			//camera
			for( CUInt j = 0; j < tempScene->m_cameras.size(); j++ )
			{
				InsertItemToObjectList( tempScene->m_cameras[j]->GetName(), eOBJECTLIST_CAMERA);
			}
			//for( CUInt j = 0; j < tempScene->m_cameraInstances.size(); j++ )
			//{
			//	InsertItemToObjectList( tempScene->m_cameraInstances[j]->GetName());
			//}
			//geometry
			for( CUInt j = 0; j < tempScene->m_geometries.size(); j++ )
			{
				InsertItemToObjectList( tempScene->m_geometries[j]->GetName(), eOBJECTLIST_MESH);
			}
			//for( CUInt j = 0; j < tempScene->m_instanceGeometries.size(); j++ )
			//{
			//	InsertItemToObjectList( tempScene->m_instanceGeometries[j]->GetName());
			//}
			//controller
			for( CUInt j = 0; j < tempScene->m_controllers.size(); j++ )
			{
				InsertItemToObjectList( tempScene->m_controllers[j]->GetName(), eOBJECTLIST_SKINCONTROLLER);
			}

			//Door
			if( CmpIn( tempScene->GetName(), "trigger" ) )
			{
				if(!g_multipleView->m_nx->m_hasScene)
				{
					tempScene->m_isTrigger = CTrue;
					tempScene->Update();
					tempScene->CreateTrigger( g_multipleView->m_nx );
				}
				else
				{
					PrintInfo( "\nCouldn't create triggers. Please remove current external PhysX scene", COLOR_RED );
				}
			}
			else if( CmpIn( tempScene->GetName(), "grass" ) )
			{
				tempScene->m_isGrass = CTrue;
				for( CUInt j = 0; j < tempScene->m_geometries.size(); j++ )
				{
					tempScene->m_geometries[j]->SetDiffuse( "grass_color" );
				}
			}

			PrintInfo("\nscene '");
			PrintInfo(tempScene->m_fileName, COLOR_RED_GREEN);
			PrintInfo("' imported successufully");
		}
		else
		{
			delete tempScene;
			tempScene = NULL;
			g_scene.erase(g_scene.end());
		}

		ReleaseCapture();
	}
}
//removed in version 1.4 or later
CVoid CVandaEngine1Dlg::OnMenuClickedImportPhysX()
{
    // Create an Open dialog; the default file name extension is ".bmp".
	CFileDialog dlgOpen(TRUE, _T("*.xml"), _T(""), OFN_HIDEREADONLY|OFN_OVERWRITEPROMPT | OFN_NOCHANGEDIR,
		_T("PhysX XML File (*.xml)|*.xml||"), NULL, NULL);

	if( dlgOpen.DoModal() == IDOK )
	{
		if( MessageBox( "Are you sure you want to replace the current PhysX scene with new PhysX scene?\nCurrent PhysX actors are lost", "Warning", MB_YESNO | MB_ICONWARNING) == IDYES )
		{
			SetCapture();
			SetCursor( LoadCursorFromFile( "Assets/Engine/Icons/progress.ani") );
			CString fileName( dlgOpen.GetPathName() );
			//for example this code converts 'test.nxb' to 'data\test\test.nxb'
			//CChar fileWithoutDot[MAX_NAME_SIZE];
			///Cpy( fileWithoutDot, (char*)fileName.GetString() );
			//GetWithoutDot( fileWithoutDot );
			CChar filePath[MAX_NAME_SIZE];
			sprintf( filePath, "%s", (char*)fileName.GetString() );
			
			CChar temp[MAX_NAME_SIZE];

			ResetPhysX();
			for( CUInt i = 0; i < g_scene.size(); i++ )
			{
				for( CUInt j = 0; j < g_scene[i]->m_instanceGeometries.size(); j++ )
				{
					g_scene[i]->m_instanceGeometries[j]->m_hasPhysX = CFalse;
					Cpy( g_scene[i]->m_instanceGeometries[j]->m_physXName, "\n" );
				}
			}
			if( g_multipleView->m_nx->LoadScene( filePath, NXU::FT_XML) )
			{
				g_physXProperties.m_bDebugMode = CTrue;
				GetMenu()->CheckMenuItem( ID_PHYSICS_DEBUGMODE, MF_CHECKED );
				PrintInfo( "\nPhysX debug activated" );

				g_multipleView->m_nx->SetSceneName( filePath );
				sprintf( temp, "\nPhysX scene '%s' imported successufully", filePath );
				PrintInfo( temp );
				m_btnRemovePhysX.EnableWindow( TRUE );
				m_physXElementListIndex = -1;
				for (int nItem = m_listBoxPhysXElements.GetItemCount()-1; nItem >= 0 ;nItem-- )
				{
					m_listBoxPhysXElements.DeleteItem(nItem);
				}
				CInt count = 0;
				for( std::vector<std::string>::iterator it = g_multipleView->m_nx->m_nxActorNames.begin(); it != g_multipleView->m_nx->m_nxActorNames.end();it++ )
				{
					CChar temp[MAX_NAME_SIZE];
					Cpy( temp, (*it).c_str() );
					InsertItemToPhysXList( temp, g_multipleView->m_nx->m_nxActorTypes[count] );
					count++;
				}
				CDelete(g_externalPhysX);
				g_externalPhysX = CNew(CExternalPhysX);
				g_externalPhysX->SetPhysXPath(filePath);
				//save functions/////////////////////////////////
				for( CUInt index = 0; index < g_VSceneNamesOfCurrentProject.size(); index++ )
				{
					g_externalPhysX->AddVSceneToList( g_VSceneNamesOfCurrentProject[index], CTrue);//Write to zip file and copy the texture
				}
				//save functions/////////////////////////////////

			}
			else
			{
				sprintf( temp, "\ncouldn't load the PhysX scene '%s'", filePath );
				PrintInfo( temp, COLOR_RED );
			}
			ReleaseCapture();
		}
	}
}

CVoid CVandaEngine1Dlg::OnMenuClickedGeneralOptions()
{
	m_dlgOptions = CNew( CEditOptions );
	INT_PTR result = m_dlgOptions->DoModal();
	CDelete( m_dlgOptions );
}

CVoid CVandaEngine1Dlg::OnMenuClickedSceneOptions()
{
	m_dlgSceneOptions = CNew( CEditSceneOptions );
	INT_PTR result = m_dlgSceneOptions->DoModal();
	CDelete( m_dlgSceneOptions );
}

CVoid CVandaEngine1Dlg::OnMenuClickedCurrentSceneOptions()
{
	m_dlgCurrentSceneOptions = CNew(CEditCurrentSceneOptions);
	INT_PTR result = m_dlgCurrentSceneOptions->DoModal();
	CDelete(m_dlgCurrentSceneOptions);
}

CVoid CVandaEngine1Dlg::OnMenuClickedSelectCamera()
{
	m_dlgSelectCamera = CNew( CSelectCamera );
	INT_PTR result = m_dlgSelectCamera->DoModal();
	CDelete( m_dlgSelectCamera );
}

CVoid CVandaEngine1Dlg::OnMenuClickedWaterAttachment()
{
	m_dlgWaterAttachment = CNew( CWaterAttachment );
	INT_PTR result = m_dlgWaterAttachment->DoModal();
	CDelete( m_dlgWaterAttachment );
}

CVoid CVandaEngine1Dlg::OnMenuClickedInsertLight()
{
	m_dlgAddLight = CNew( CAddLight );
	m_dlgAddLight->SetCreate( CTrue );
	INT_PTR result = m_dlgAddLight->DoModal();
	if ( result  == IDOK )
	{
		CInstanceLight* instance_light = new CInstanceLight();
		CLight* abstract_light = new CLight();

		instance_light->m_parent = NULL; //Not using COLLADA parent
		instance_light->m_abstractLight = abstract_light;
		abstract_light->SetName( m_dlgAddLight->GetName() );
		abstract_light->SetType( m_dlgAddLight->GetType() );
		switch( m_dlgAddLight->GetType() )
		{
		case eLIGHTTYPE_SPOT:
			instance_light->m_abstractLight->SetSpotCutoff( m_dlgAddLight->GetSpotCuttoff() );
			instance_light->m_abstractLight->SetSpotExponent( m_dlgAddLight->GetSpotExponent() );
			instance_light->m_abstractLight->SetSpotDirection( m_dlgAddLight->GetSpotDirection() );
			break;
		}

		abstract_light->SetAmbient ( m_dlgAddLight->GetAmbientColor() );
		abstract_light->SetDiffuse( m_dlgAddLight->GetDiffuseColor() );
		abstract_light->SetSpecular( m_dlgAddLight->GetSpecularColor() );
		abstract_light->SetPosition( m_dlgAddLight->GetLightPos() );
		abstract_light->SetShininess( m_dlgAddLight->GetShininess() );
		abstract_light->SetConstantAttenuation( m_dlgAddLight->GetConstantAttenuation() );
		abstract_light->SetLinearAttenuation( m_dlgAddLight->GetLinearAttenuation() );
		abstract_light->SetQuadraticAttenuation( m_dlgAddLight->GetQuadAttenuation() );

		instance_light->SetIndex();

		g_engineLights.push_back( instance_light );

		InserItemToEngineObjectList( abstract_light->GetName(), eENGINEOBJECTLIST_LIGHT );
		g_engineObjectNames.push_back( m_dlgAddLight->GetName() );

		CDelete( m_dlgAddLight );
		PrintInfo( "\nNew light added successfully" );
	}
	else if( result == IDCANCEL )
		CDelete( m_dlgAddLight );
}

CVoid CVandaEngine1Dlg::OnMenuClickedInsertStaticSound()
{
	m_dlgAddStaticSound = CNew( CAddStaticSound );
	m_dlgAddStaticSound->SetCreate( CTrue );
	INT_PTR result = m_dlgAddStaticSound->DoModal();
	if ( result  == IDOK )
	{
		COpenALSoundSource* m_staticSoundSource = CNew( COpenALSoundSource );
		CStaticSound* m_staticSound = CNew( CStaticSound );

		CChar temp[ MAX_NAME_SIZE];
		sprintf( temp, "%s", m_dlgAddStaticSound->m_strStaticSoundDataPath );
		COpenALSoundBuffer* m_staticSoundBuffer = GetSoundBuffer( GetAfterPath(temp) );
		if( m_staticSoundBuffer == NULL || (m_staticSoundBuffer && !m_staticSoundBuffer->m_loaded ) )
		{
			if( m_staticSoundBuffer == NULL )
			{
				m_staticSoundBuffer = CNew( COpenALSoundBuffer );
				g_soundBuffers.push_back( m_staticSoundBuffer );
			}
			else 
			{
				CChar tempBuffer[MAX_NAME_SIZE];
				sprintf( tempBuffer, "\nTrying to reload '%s%s", GetAfterPath(m_staticSoundBuffer->GetName() ), "'" );
				PrintInfo( tempBuffer, COLOR_YELLOW );
			}
			if( !m_staticSoundBuffer->LoadOggVorbisFromFile( temp ) )
			{
				CChar buffer[MAX_NAME_SIZE];
				sprintf( buffer, "\n%s%s%s", "Couldn't load the file '", temp, "'" );
				PrintInfo( buffer, COLOR_RED );
				m_staticSoundBuffer->m_loaded = CFalse;

			}
			else
			{
				CChar buffer[MAX_NAME_SIZE];
				sprintf( buffer, "\n%s%s%s", "ogg file '", temp, "' loaded successfully." );
				PrintInfo( buffer );
				m_staticSoundBuffer->m_loaded = CTrue;
			}
			m_staticSoundBuffer->SetName( temp );	
		}
		else
		{
				CChar temp[MAX_NAME_SIZE]; 
				sprintf( temp, "\n%s%s%s", "sound buffer '", GetAfterPath(m_staticSoundBuffer->GetName()), "' already exists." );
				PrintInfo( temp, COLOR_YELLOW );
		}

		m_staticSoundSource->BindSoundBuffer (*m_staticSoundBuffer);
		m_staticSoundSource->SetLooping( m_dlgAddStaticSound->GetLoopCondition() );
		m_staticSoundSource->SetPitch( m_dlgAddStaticSound->GetPitch() );
		m_staticSoundSource->SetReferenceDistance( m_dlgAddStaticSound->GetReferenceDistance() );
		m_staticSoundSource->SetMaxDistance( m_dlgAddStaticSound->GetMaxDistance() );
		m_staticSoundSource->SetRolloff( m_dlgAddStaticSound->GetRolloff() );
		m_staticSoundSource->SetSoundPosition( m_dlgAddStaticSound->GetSoundPos() );
		if( m_dlgAddStaticSound->GetPlayCondition() )
			g_multipleView->m_soundSystem->PlayALSound( *m_staticSoundSource );

		m_staticSound->SetName( m_dlgAddStaticSound->GetName() );
		m_staticSound->SetPath( temp );
		m_staticSound->SetPosition( m_dlgAddStaticSound->GetSoundPos() );
		m_staticSound->SetLoop( m_dlgAddStaticSound->GetLoopCondition() );
		m_staticSound->SetMaxDistance( m_dlgAddStaticSound->GetMaxDistance() );
		m_staticSound->SetPitch( m_dlgAddStaticSound->GetPitch() );
		m_staticSound->SetPlay( m_dlgAddStaticSound->GetPlayCondition() );
		m_staticSound->SetRefrenceDistance( m_dlgAddStaticSound->GetReferenceDistance() );
		m_staticSound->SetRolloff( m_dlgAddStaticSound->GetRolloff() );
		m_staticSound->SetSoundSource( m_staticSoundSource );
		m_staticSound->SetSoundBuffer( m_staticSoundBuffer );

		//save functions/////////////////////////////////
		for( CUInt index = 0; index < g_VSceneNamesOfCurrentProject.size(); index++ )
		{
			m_staticSound->AddVSceneToList( g_VSceneNamesOfCurrentProject[index], CTrue ); //Write to zip file and copy the textures
		}
		//save functions/////////////////////////////////

		g_engineStaticSounds.push_back( m_staticSound );
		InserItemToEngineObjectList( m_staticSound->GetName(), eENGINEOBJECTLIST_STATICSOUND );
		g_engineObjectNames.push_back( m_dlgAddStaticSound->GetName() );
		CDelete( m_dlgAddStaticSound );
	}
	else if( result == IDCANCEL )
		CDelete( m_dlgAddStaticSound );
}

CVoid CVandaEngine1Dlg::OnMenuClickedInsertSkyDome()
{
	m_dlgAddSkyDome = CNew( CAddSkyDome );
	m_dlgAddSkyDome->SetCreate( CTrue );
	INT_PTR result = m_dlgAddSkyDome->DoModal();
	if ( result == IDOK )
	{
		CChar temp[ MAX_NAME_SIZE];
		sprintf( temp, "%s", m_dlgAddSkyDome->m_strSkyDomePath );
		g_skyDome = CNew( CSkyDome );
		g_skyDome->SetName( m_dlgAddSkyDome->GetName() );
		g_skyDome->SetPath( temp );
		g_skyDome->SetRadius( m_dlgAddSkyDome->GetRadius() );
		g_skyDome->SetPosition( m_dlgAddSkyDome->GetPos() );
		g_skyDome->SetDampening( m_dlgAddSkyDome->GetDampening() );
		g_skyDome->SetSides( m_dlgAddSkyDome->GetSides() );
		g_skyDome->SetSlices( m_dlgAddSkyDome->GetSlices() );
		g_skyDome->SetExponential( m_dlgAddSkyDome->GetExponential() );

		g_skyDome->Initialize();
		//save functions/////////////////////////////////
		for( CUInt index = 0; index < g_VSceneNamesOfCurrentProject.size(); index++ )
		{
			g_skyDome->AddVSceneToList( g_VSceneNamesOfCurrentProject[index], CTrue ); //Write to zip file and copy the textures
		}
		//save functions/////////////////////////////////

		GetMenu()->EnableMenuItem( ID_INSERT_SKYDOME, MF_DISABLED | MF_GRAYED );
		m_mainBtnSky.EnableWindow( FALSE );

		g_menu.m_insertAndShowSky = CTrue;
		InserItemToEngineObjectList( g_skyDome->GetName(), eENGINEOBJECTLIST_SKY);
		g_engineObjectNames.push_back( m_dlgAddSkyDome->GetName() );
		CDelete( m_dlgAddSkyDome );
	}
	else if( result == IDCANCEL )
		CDelete( m_dlgAddSkyDome );

}

CVoid CVandaEngine1Dlg::OnMenuClickedInsertWater()
{
	m_dlgAddWater = CNew( CAddWater );
	m_dlgAddWater->SetCreate( CTrue );
	INT_PTR result = m_dlgAddWater->DoModal();
	if ( result  == IDOK )
	{
		CWater* water = new CWater;
		water->SetName( m_dlgAddWater->GetName() );
		water->SetDuDvMap( m_dlgAddWater->GetDuDvMap(), CTrue );
		water->SetNormalMap( m_dlgAddWater->GetNormalMap(), CTrue );
		water->SetHeight( m_dlgAddWater->GetHeight() );
		water->SetPos( m_dlgAddWater->GetPos() );
		water->SetLightPos( m_dlgAddWater->GetLightPos() );
		water->SetScale( m_dlgAddWater->GetScale() );
		water->SetSpeed( m_dlgAddWater->GetSpeed() );
		water->SetUV( m_dlgAddWater->GetUV() );
		water->CreateRenderTexture(g_waterTextureSize, 3, GL_RGB, WATER_REFLECTION_ID );
		water->CreateRenderTexture(g_waterTextureSize, 3, GL_RGB, WATER_REFRACTION_ID );
		water->CreateRenderTexture(g_waterTextureSize, 1, GL_DEPTH_COMPONENT, WATER_DEPTH_ID );
		water->SetSideVertexPositions();

		//save functions/////////////////////////////////
		for( CUInt index = 0; index < g_VSceneNamesOfCurrentProject.size(); index++ )
		{
			water->AddVSceneToList( g_VSceneNamesOfCurrentProject[index], CTrue ); //Write to zip file and copy the textures
		}
		//save functions/////////////////////////////////

		g_engineWaters.push_back( water );
		InserItemToEngineObjectList( water->GetName() , eENGINEOBJECTLIST_WATER);
		g_engineObjectNames.push_back( m_dlgAddWater->GetName() );
		CDelete( m_dlgAddWater );
		PrintInfo( "\nNew water surface added successfully" );
	}
	else if( result == IDCANCEL )
		CDelete( m_dlgAddWater );
}

CVoid CVandaEngine1Dlg::OnMenuClickedInsertAmbientSound()
{
	m_dlgAddAmbientSound = CNew( CAddAmbientSound );
	m_dlgAddAmbientSound->SetCreate( CTrue );
	INT_PTR result = m_dlgAddAmbientSound->DoModal();
	if ( result  == IDOK )
	{
		if( g_multipleView->m_ambientSound )
		{
			g_multipleView->m_soundSystem->StopALSound( *(g_multipleView->m_ambientSound->GetSoundSource()) );
			alSourcei(g_multipleView->m_ambientSound->GetSoundSource()->GetSource(), AL_BUFFER, AL_NONE);
			CDelete( g_multipleView->m_ambientSound )
		}

		COpenALSoundSource* m_ambientSoundSource = CNew( COpenALSoundSource );
		COpenALSoundBuffer* m_ambientSoundBuffer = CNew( COpenALSoundBuffer );

		CChar temp[ MAX_NAME_SIZE];
		sprintf( temp, "%s", m_dlgAddAmbientSound->m_strAmbientSoundBuffer );
		if( !m_ambientSoundBuffer->LoadOggVorbisFromFile( temp ) )
			return;
		m_ambientSoundSource->BindSoundBuffer( *(m_ambientSoundBuffer) );

		m_ambientSoundSource->SetLooping( true );
		m_ambientSoundSource->SetPitch( m_dlgAddAmbientSound->GetPitch() );
		m_ambientSoundSource->SetVolume( m_dlgAddAmbientSound->GetVolume() );

		g_multipleView->m_ambientSound = CNew( CAmbientSound );
		g_multipleView->m_ambientSound->SetSoundSource( m_ambientSoundSource );
		g_multipleView->m_ambientSound->SetSoundBuffer( m_ambientSoundBuffer );
		g_multipleView->m_ambientSound->SetName( m_dlgAddAmbientSound->GetName() );
		g_multipleView->m_ambientSound->SetPath( temp );
		g_multipleView->m_ambientSound->SetVolume( m_dlgAddAmbientSound->GetVolume() );
		g_multipleView->m_ambientSound->SetPitch( m_dlgAddAmbientSound->GetPitch() );

		g_multipleView->m_soundSystem->PlayALSound( *m_ambientSoundSource );
		//save functions/////////////////////////////////
		for( CUInt index = 0; index < g_VSceneNamesOfCurrentProject.size(); index++ )
		{
			g_multipleView->m_ambientSound->AddVSceneToList( g_VSceneNamesOfCurrentProject[index], CTrue ); //Write to zip file and copy the textures
		}
		//save functions/////////////////////////////////

		PrintInfo( "\nAmbient sound '" );
		sprintf( temp, "%s", m_dlgAddAmbientSound->m_strAmbientSoundBuffer );
		PrintInfo( temp, COLOR_RED_GREEN );
		PrintInfo( "' loaded successfully" );
		
		g_menu.m_playAmbientSound = CTrue;

		g_menu.m_insertAmbientSound = CTrue;
		InserItemToEngineObjectList( g_multipleView->m_ambientSound->GetName(), eENGINEOBJECTLIST_AMBIENTSOUND );
		GetMenu()->EnableMenuItem( ID_INSERT_SOUND_AMBIENT, MF_DISABLED | MF_GRAYED );
		m_mainBtnAmbientSound.EnableWindow( FALSE );
		g_engineObjectNames.push_back( m_dlgAddAmbientSound->GetName() );
		CDelete( m_dlgAddAmbientSound );
	}
	else if( result == IDCANCEL )
		CDelete( m_dlgAddAmbientSound );
}

CVoid CVandaEngine1Dlg::OnMenuClickedSavePrefabAs(CBool askQuestion)
{
	CBool foundLOD1 = CFalse;

	for (CUInt i = 0; i < g_scene.size(); i++)
	{
		if (CmpIn(g_scene[i]->GetName(), "_LOD1"))
		{
			foundLOD1 = CTrue;
			break;
		}
	}
	if (!foundLOD1)
	{
		MessageBox("Please import LOD1 and try again.", "Vanda Engine Error", MB_OK | MB_ICONERROR);
		return;
	}
	CInt result = IDOK;

	if (askQuestion || Cmp(g_currentPackageAndPrefabName, "\n"))
	{
		m_dlgSavePrefabs = CNew(CSavePrefabDlg);
		result = m_dlgSavePrefabs->DoModal();
	}

	if (result == IDOK)
	{
		SetCapture();
		SetCursor(LoadCursorFromFile("Assets/Engine/Icons/progress.ani"));

		if (askQuestion || Cmp(g_currentPackageAndPrefabName, "\n")) //nothing has been saved
		{
			Cpy(m_strNewPackageAndPrefabName, m_dlgSavePrefabs->m_strNewPackageAndPrefabName.c_str());

			if (g_vandaDemo)
				Cpy(m_strNewPassword, "\n");
			else
			{
				Cpy(m_strNewPassword, (CChar*)m_dlgSavePrefabs->m_strPassword.c_str());
			}

			Cpy(m_strNewPackageName, (CChar*)m_dlgSavePrefabs->m_strNewPackageName.c_str());
			Cpy(m_strNewPrefabName, (CChar*)m_dlgSavePrefabs->m_strNewPrefabName.c_str());
		}
		//Save functions////////////////////////////////////
		CChar pureFileName[MAX_NAME_SIZE];
		Cpy(pureFileName, m_strNewPackageAndPrefabName);


		CInt saveAlgorithm = -1;
		if (Cmp(g_currentPackageAndPrefabName, "\n") || g_openNewPrefabFromList)
		{
			saveAlgorithm = 1;
		}
		else
		{
			if (Cmp(g_currentPackageAndPrefabName, m_strNewPackageAndPrefabName))
			{
				saveAlgorithm = 2;
			}
			else //scene does not exists
			{
				saveAlgorithm = 3;
			}
		}
		g_openNewPrefabFromList = CFalse;
		//Create All Directories
		CChar docPath[MAX_NAME_SIZE];
		HRESULT result = SHGetFolderPath(NULL, CSIDL_PERSONAL, NULL, SHGFP_TYPE_CURRENT, docPath);
		if (result != S_OK)
		{
			PrintInfo("\nCouldn't get the documents folder to write data", COLOR_RED);
			return;
		}

		CChar newPackage[MAX_NAME_SIZE];
		sprintf(newPackage, "%s%s%s%s", docPath, "/Vanda/Packages/", m_strNewPackageName, "/");

		CChar newPrefabName[MAX_NAME_SIZE];
		sprintf(newPrefabName, "%s%s", newPackage, m_strNewPrefabName);

		CChar newExternalScenePath[MAX_NAME_SIZE];
		sprintf(newExternalScenePath, "%s%s", newPrefabName, "/External Scenes/");

		CChar newExternalSceneTexturesPath[MAX_NAME_SIZE];
		sprintf(newExternalSceneTexturesPath, "%s%s", newPrefabName, "/Textures/");

		CChar newScriptPath[MAX_NAME_SIZE];
		sprintf(newScriptPath, "%s%s", newPrefabName, "/Scripts/");

		//Directories of current scene
		CChar currentPackagePath[MAX_NAME_SIZE];
		sprintf(currentPackagePath, "%s%s%s%s", docPath, "/Vanda/Packages/", g_currentPackageName, "/");

		CChar currentPrefabPath[MAX_NAME_SIZE];
		sprintf(currentPrefabPath, "%s%s", currentPackagePath, g_currentPrefabName);

		CChar currentExternalScenePath[MAX_NAME_SIZE];
		sprintf(currentExternalScenePath, "%s%s", currentPrefabPath, "/External Scenes/");

		CChar currentExternalSceneTexturesPath[MAX_NAME_SIZE];
		sprintf(currentExternalSceneTexturesPath, "%s%s", currentPrefabPath, "/Textures/");

		CChar currentScriptPath[MAX_NAME_SIZE];
		sprintf(currentScriptPath, "%s%s", currentPrefabPath, "/Scripts/");

		if (saveAlgorithm == 1)
			Cpy(g_currentPassword, m_strNewPassword);

		if (saveAlgorithm == 1 || saveAlgorithm == 3)
		{
			//create a new directory based on the save file name
			CreateWindowsDirectory(newPackage);
			CreateWindowsDirectory(newPrefabName);
			CreateWindowsDirectory(newExternalScenePath);
			CreateWindowsDirectory(newExternalSceneTexturesPath);
			CreateWindowsDirectory(newScriptPath);
		}

		if (saveAlgorithm == 1 || saveAlgorithm == 2)
		{
			//copy the assets from original place to the saved directory
			//scenes
			for (CUInt i = 0; i < g_scene.size(); i++)
			{
				if (g_scene[i]->IsInPrefabList(pureFileName, CTrue, CTrue))
				{
					if (g_scene[i]->m_compress)
					{
						//Compress dae files////////////////////////////////////////////////
						//zip pathr
						CChar zipPathTemp[MAX_NAME_SIZE];
						Cpy(zipPathTemp, g_scene[i]->GetFileName());
						CChar* zipPathPointer = GetAfterPath(zipPathTemp);
						GetWithoutDot(zipPathPointer);
						CChar zipPath[MAX_NAME_SIZE];
						sprintf(zipPath, "%s%s%s", newExternalScenePath, zipPathPointer, ".zip");
						//file name inside zip file
						CChar fileNameInZip[MAX_NAME_SIZE];
						Cpy(fileNameInZip, GetAfterPath(g_scene[i]->GetFileName()));

						CChar temp[MAX_NAME_SIZE];
						sprintf(temp, "\n%s %s %s", "Compressing '", fileNameInZip, "' ...");
						PrintInfo(temp);
						if (Cmp(g_currentPassword, "\n"))
						{
							if (!g_vandaDemo)
							{
								//generate a password
								CSavePrefabDlg *m_dlgSavePrefabs = CNew(CSavePrefabDlg);
								m_dlgSavePrefabs->GenerateRandomPassword();
								Cpy(g_currentPassword, m_dlgSavePrefabs->m_strPassword.c_str());
								CDelete(m_dlgSavePrefabs);
								g_scene[i]->WriteZipFile(zipPath, fileNameInZip, g_scene[i]->GetFileName(), g_currentPassword);
							}
							else
							{
								g_scene[i]->WriteZipFile(zipPath, fileNameInZip, g_scene[i]->GetFileName(), NULL);
							}
						}
						else
						{
							g_scene[i]->WriteZipFile(zipPath, fileNameInZip, g_scene[i]->GetFileName(), g_currentPassword);
						}

						g_scene[i]->m_compress = CFalse;
					}
					//////////////////////////////////////////////////////////////////

					for (CUInt j = 0; j < g_scene[i]->m_instanceGeometries.size(); j++)
					{
						if (g_scene[i]->m_instanceGeometries[j]->m_hasEnterScript && g_scene[i]->m_instanceGeometries[j]->m_updateEnterScript)
						{
							CopyOneFileToDstDirectory(g_scene[i]->m_instanceGeometries[j]->m_enterScript, newScriptPath);
							g_scene[i]->m_instanceGeometries[j]->m_updateEnterScript = CFalse;

							CChar* TempAfterPath = GetAfterPath(g_scene[i]->m_instanceGeometries[j]->m_enterScript);
							CChar NewPathAndName[MAX_NAME_SIZE];
							Cpy(NewPathAndName, newScriptPath);
							Append(NewPathAndName, TempAfterPath);
							Cpy(g_scene[i]->m_instanceGeometries[j]->m_enterScript, NewPathAndName);
						}
						if (g_scene[i]->m_instanceGeometries[j]->m_hasExitScript && g_scene[i]->m_instanceGeometries[j]->m_updateExitScript)
						{
							CopyOneFileToDstDirectory(g_scene[i]->m_instanceGeometries[j]->m_exitScript, newScriptPath);
							g_scene[i]->m_instanceGeometries[j]->m_updateExitScript = CFalse;

							CChar* TempAfterPath = GetAfterPath(g_scene[i]->m_instanceGeometries[j]->m_exitScript);
							CChar NewPathAndName[MAX_NAME_SIZE];
							Cpy(NewPathAndName, newScriptPath);
							Append(NewPathAndName, TempAfterPath);
							Cpy(g_scene[i]->m_instanceGeometries[j]->m_exitScript, NewPathAndName);

						}
					}

					for (CUInt j = 0; j < g_scene[i]->m_geometries.size(); j++)
					{
						CUInt groupSize = CUInt(g_scene[i]->m_geometries[j]->m_groups.size());
						for (CUInt k = 0; k < groupSize; k++)
						{

							if (g_scene[i]->m_geometries[j]->m_groups[k]->m_hasGlossMap && g_scene[i]->m_geometries[j]->m_groups[k]->m_updateGlossMap)
							{
								CopyOneFileToDstDirectory(g_scene[i]->m_geometries[j]->m_groups[k]->m_strOriginalGlossMapPath, newExternalSceneTexturesPath);
								g_scene[i]->m_geometries[j]->m_groups[k]->m_updateGlossMap = CFalse;
							}

							if (g_scene[i]->m_geometries[j]->m_groups[k]->m_hasDirtMap && g_scene[i]->m_geometries[j]->m_groups[k]->m_updateDirtMap)
							{
								CopyOneFileToDstDirectory(g_scene[i]->m_geometries[j]->m_groups[k]->m_strOriginalDirtMapPath, newExternalSceneTexturesPath);
								g_scene[i]->m_geometries[j]->m_groups[k]->m_updateDirtMap = CFalse;
							}

							if (g_scene[i]->m_geometries[j]->m_groups[k]->m_hasNormalMap && g_scene[i]->m_geometries[j]->m_groups[k]->m_updateNormalMap)
							{
								CopyOneFileToDstDirectory(g_scene[i]->m_geometries[j]->m_groups[k]->m_strOriginalNormalMapPath, newExternalSceneTexturesPath);
								g_scene[i]->m_geometries[j]->m_groups[k]->m_updateNormalMap = CFalse;
							}

							if (g_scene[i]->m_geometries[j]->m_groups[k]->m_hasDiffuse && g_scene[i]->m_geometries[j]->m_groups[k]->m_updateDiffuse)
							{
								CopyOneFileToDstDirectory(g_scene[i]->m_geometries[j]->m_groups[k]->m_strOriginalDiffusePath, newExternalSceneTexturesPath);
								g_scene[i]->m_geometries[j]->m_groups[k]->m_updateDiffuse = CFalse;
							}
						} //groups
					} //geometries
				}
			} //scenes
		}
		else if (saveAlgorithm == 3)
		{
			//first of all, copy the assets from current place to the saved directory
			CopyAllFilesFromSrcToDstDirectory(currentExternalScenePath, newExternalScenePath);
			CopyAllFilesFromSrcToDstDirectory(currentExternalSceneTexturesPath, newExternalSceneTexturesPath);
			CopyAllFilesFromSrcToDstDirectory(currentScriptPath, newScriptPath);

			//then copy changed assets
			//scenes
			for (CUInt i = 0; i < g_scene.size(); i++)
			{
				if (g_scene[i]->IsInPrefabList(g_currentPackageAndPrefabName, CTrue, CFalse))
				{
					if (g_scene[i]->m_compress)
					{
						//Compress dae files////////////////////////////////////////////////
						//zip path
						CChar zipPathTemp[MAX_NAME_SIZE];
						Cpy(zipPathTemp, g_scene[i]->GetFileName());
						CChar* zipPathPointer = GetAfterPath(zipPathTemp);
						GetWithoutDot(zipPathPointer);
						CChar zipPath[MAX_NAME_SIZE];
						sprintf(zipPath, "%s%s%s", newExternalScenePath, zipPathPointer, ".zip");
						//file name inside zip file
						CChar fileNameInZip[MAX_NAME_SIZE];
						Cpy(fileNameInZip, GetAfterPath(g_scene[i]->GetFileName()));

						CChar temp[MAX_NAME_SIZE];
						sprintf(temp, "\n%s %s %s", "Compressing '", fileNameInZip, "' ...");
						PrintInfo(temp);
						if (Cmp(g_currentPassword, "\n"))
						{
							if (!g_vandaDemo)
							{
								//generate a password
								CSavePrefabDlg *m_dlgSavePrefabs = CNew(CSavePrefabDlg);
								m_dlgSavePrefabs->GenerateRandomPassword();
								Cpy(g_currentPassword, m_dlgSavePrefabs->m_strPassword.c_str());
								CDelete(m_dlgSavePrefabs);
								g_scene[i]->WriteZipFile(zipPath, fileNameInZip, g_scene[i]->GetFileName(), g_currentPassword);
							}
							else
							{
								g_scene[i]->WriteZipFile(zipPath, fileNameInZip, g_scene[i]->GetFileName(), NULL);
							}
						}
						else
						{
							g_scene[i]->WriteZipFile(zipPath, fileNameInZip, g_scene[i]->GetFileName(), g_currentPassword);
						}
						g_scene[i]->m_compress = CFalse;
					}
					//////////////////////////////////////////////////////////////////

					for (CUInt j = 0; j < g_scene[i]->m_instanceGeometries.size(); j++)
					{
						if (g_scene[i]->m_instanceGeometries[j]->m_hasEnterScript && g_scene[i]->m_instanceGeometries[j]->m_updateEnterScript)
						{
							CopyOneFileToDstDirectory(g_scene[i]->m_instanceGeometries[j]->m_enterScript, newScriptPath);
							g_scene[i]->m_instanceGeometries[j]->m_updateEnterScript = CFalse;

							CChar* TempAfterPath = GetAfterPath(g_scene[i]->m_instanceGeometries[j]->m_enterScript);
							CChar NewPathAndName[MAX_NAME_SIZE];
							Cpy(NewPathAndName, newScriptPath);
							Append(NewPathAndName, TempAfterPath);
							Cpy(g_scene[i]->m_instanceGeometries[j]->m_enterScript, NewPathAndName);

						}
						if (g_scene[i]->m_instanceGeometries[j]->m_hasExitScript && g_scene[i]->m_instanceGeometries[j]->m_updateExitScript)
						{
							CopyOneFileToDstDirectory(g_scene[i]->m_instanceGeometries[j]->m_exitScript, newScriptPath);
							g_scene[i]->m_instanceGeometries[j]->m_updateExitScript = CFalse;

							CChar* TempAfterPath = GetAfterPath(g_scene[i]->m_instanceGeometries[j]->m_exitScript);
							CChar NewPathAndName[MAX_NAME_SIZE];
							Cpy(NewPathAndName, newScriptPath);
							Append(NewPathAndName, TempAfterPath);
							Cpy(g_scene[i]->m_instanceGeometries[j]->m_exitScript, NewPathAndName);

						}
					}

					for (CUInt j = 0; j < g_scene[i]->m_geometries.size(); j++)
					{
						CUInt groupSize = CUInt(g_scene[i]->m_geometries[j]->m_groups.size());
						for (CUInt k = 0; k < groupSize; k++)
						{
							if (g_scene[i]->m_geometries[j]->m_groups[k]->m_hasGlossMap && g_scene[i]->m_geometries[j]->m_groups[k]->m_updateGlossMap)
							{
								CopyOneFileToDstDirectory(g_scene[i]->m_geometries[j]->m_groups[k]->m_strOriginalGlossMapPath, newExternalSceneTexturesPath);
								g_scene[i]->m_geometries[j]->m_groups[k]->m_updateGlossMap = CFalse;
							}

							if (g_scene[i]->m_geometries[j]->m_groups[k]->m_hasDirtMap && g_scene[i]->m_geometries[j]->m_groups[k]->m_updateDirtMap)
							{
								CopyOneFileToDstDirectory(g_scene[i]->m_geometries[j]->m_groups[k]->m_strOriginalDirtMapPath, newExternalSceneTexturesPath);
								g_scene[i]->m_geometries[j]->m_groups[k]->m_updateDirtMap = CFalse;
							}

							if (g_scene[i]->m_geometries[j]->m_groups[k]->m_hasNormalMap && g_scene[i]->m_geometries[j]->m_groups[k]->m_updateNormalMap)
							{
								CopyOneFileToDstDirectory(g_scene[i]->m_geometries[j]->m_groups[k]->m_strOriginalNormalMapPath, newExternalSceneTexturesPath);
								g_scene[i]->m_geometries[j]->m_groups[k]->m_updateNormalMap = CFalse;
							}

							if (g_scene[i]->m_geometries[j]->m_groups[k]->m_hasDiffuse && g_scene[i]->m_geometries[j]->m_groups[k]->m_updateDiffuse)
							{
								CopyOneFileToDstDirectory(g_scene[i]->m_geometries[j]->m_groups[k]->m_strOriginalDiffusePath, newExternalSceneTexturesPath);
								g_scene[i]->m_geometries[j]->m_groups[k]->m_updateDiffuse = CFalse;
							}
						} //groups
					} //geometries
				}
				g_scene[i]->IsInPrefabList(pureFileName, CTrue, CTrue);
			} //scenes
		}
		HANDLE hFind;
		WIN32_FIND_DATA data;

		//Delete removed scenes : Zip files
		CChar path[MAX_NAME_SIZE];
		sprintf(path, "%s%s", newExternalScenePath, "*.*");
		hFind = FindFirstFile(path, &data);
		do
		{
			CChar filePath[MAX_NAME_SIZE];
			sprintf(filePath, "%s%s", newExternalScenePath, data.cFileName);
			CBool foundTarget = CFalse;
			for (CUInt i = 0; i < g_scene.size(); i++)
			{
				CChar sceneWithoutDot[MAX_NAME_SIZE];
				Cpy(sceneWithoutDot, g_scene[i]->GetName());
				GetWithoutDot(sceneWithoutDot);
				Append(sceneWithoutDot, ".zip");
				if (Cmp(sceneWithoutDot, data.cFileName))
				{
					foundTarget = CTrue;
					break;
				}
			}

			//Remove Files
			if (!foundTarget)
			{
				if (!DeleteFile(filePath))
				{
					//CChar temp[MAX_NAME_SIZE];
					//sprintf( temp, "\n%s%s", "Error: Couldn't remove the file ", data.cFileName );
					//PrintInfo( temp, COLOR_RED );
				}
			}
		} while (FindNextFile(hFind, &data));
		FindClose(hFind);

		//Delete removed scenes : scripts
		CChar tempScriptPath[MAX_NAME_SIZE];
		sprintf(tempScriptPath, "%s%s", newScriptPath, "*.*");
		hFind = FindFirstFile(tempScriptPath, &data);
		do
		{
			CChar scriptTempPath[MAX_NAME_SIZE];
			sprintf(scriptTempPath, "%s%s", newScriptPath, data.cFileName);

			CBool foundTarget = CFalse;
			for (CUInt i = 0; i <g_scene.size(); i++)
			{
				for (CUInt j = 0; j <g_scene[i]->m_instanceGeometries.size(); j++)
				{
					if (g_scene[i]->m_instanceGeometries[j]->m_hasEnterScript && Cmp(GetAfterPath(g_scene[i]->m_instanceGeometries[j]->m_enterScript), data.cFileName))
					{
						foundTarget = CTrue;
						break;
					}
					if (g_scene[i]->m_instanceGeometries[j]->m_hasExitScript && Cmp(GetAfterPath(g_scene[i]->m_instanceGeometries[j]->m_exitScript), data.cFileName))
					{
						foundTarget = CTrue;
						break;
					}
				}
			}

			//Remove Files
			if (!foundTarget)
			{
				if (!DeleteFile(scriptTempPath))
				{
					//CChar temp[MAX_NAME_SIZE];
					//sprintf( temp, "\n%s%s", "Error: Couldn't remove the file ", data.cFileName );
					//PrintInfo( temp, COLOR_RED );
				}
			}
		} while (FindNextFile(hFind, &data));
		FindClose(hFind);

		//Delete removed scenes : Unshared image files
		CChar path2[MAX_NAME_SIZE];
		sprintf(path2, "%s%s", newExternalSceneTexturesPath, "*.*");
		hFind = FindFirstFile(path2, &data);
		do
		{
			CChar filePath2[MAX_NAME_SIZE];
			sprintf(filePath2, "%s%s", newExternalSceneTexturesPath, data.cFileName);
			CBool foundTarget = CFalse;
			for (CUInt i = 0; i < g_images.size(); i++)
			{
				if (Cmp(GetAfterPath(g_images[i]->m_fileName), data.cFileName))
				{
					foundTarget = CTrue;
					break;
				}
			}

			//Remove Files
			if (!foundTarget)
			{
				if (!DeleteFile(filePath2))
				{
					//CChar temp[MAX_NAME_SIZE];
					//sprintf( temp, "\n%s%s", "Error: Couldn't remove the file ", data.cFileName );
					//PrintInfo( temp, COLOR_RED );
				}
			}
		} while (FindNextFile(hFind, &data));
		FindClose(hFind);

		//Save functions////////////////////////////////////
		CChar fileName[MAX_NAME_SIZE];
		sprintf(fileName, "%s%s%s%s", newPrefabName, "/", m_strNewPackageAndPrefabName, ".vpf");
		FILE *filePtr;
		filePtr = fopen(fileName, "wb");
		if (!filePtr)
		{
			//fclose( filePtr );
			CChar temp[MAX_NAME_SIZE];
			sprintf(temp, "%s%s%s", "Couldn't open the file '", fileName, "' to save data");
			MessageBox(temp, "Vanda Engine Error", MB_OK);
			return;
		}
		//Save version
		CChar engineName[MAX_NAME_SIZE] = "VandaEngine";
		fwrite(&engineName, sizeof(CChar), MAX_NAME_SIZE, filePtr);
		fwrite(&g_edition, sizeof(CChar), MAX_NAME_SIZE, filePtr);
		fwrite(&g_maxVersion, 1, sizeof(CInt), filePtr);
		fwrite(&g_minVersion, 1, sizeof(CInt), filePtr);
		fwrite(&g_bugFixesVersion, 1, sizeof(CInt), filePtr);
		fwrite(&g_currentPassword, sizeof(CChar), MAX_NAME_SIZE, filePtr);
		//save engine options

		fwrite(&g_shadowProperties, sizeof(CShadowProperties), 1, filePtr);
		fwrite(&g_physXProperties, sizeof(CPhysXProperties), 1, filePtr);
		fwrite(&g_dofProperties, sizeof(CDOFProperties), 1, filePtr);
		fwrite(&g_fogProperties, sizeof(CFogProperties), 1, filePtr);
		fwrite(&g_bloomProperties, sizeof(CBloomProperties), 1, filePtr);
		fwrite(&g_lightProperties, sizeof(CLightProperties), 1, filePtr);
		fwrite(&g_pathProperties, sizeof(CPathProperties), 1, filePtr);
		fwrite(&g_vandaDemo, sizeof(CBool), 1, filePtr);
		fwrite(g_sceneBanner.GetBannerPath(), sizeof(CChar), MAX_NAME_SIZE, filePtr);
		fwrite(&g_extraTexturesNamingConventions, sizeof(CExtraTexturesNamingConventions), 1, filePtr);
		fwrite(&g_useGlobalAmbientColor, sizeof(CBool), 1, filePtr);
		fwrite(&g_globalAmbientColor, sizeof(CColor4f), 1, filePtr);
		fwrite(&g_sceneManagerObjectsPerSplit, sizeof(CInt), 1, filePtr);
		//save physx data

		g_characterPos = g_multipleView->m_nx->gCharacterPos;
		g_cameraPitchYawTilt.x = g_camera->m_perspectiveCameraPitch;
		g_cameraPitchYawTilt.y = g_camera->m_perspectiveCameraYaw;
		g_cameraPitchYawTilt.z = g_camera->m_perspectiveCameraTilt;

		g_cameraInstancePos.x = g_render.GetDefaultInstanceCamera()->m_x;
		g_cameraInstancePos.y = g_render.GetDefaultInstanceCamera()->m_y;
		g_cameraInstancePos.z = g_render.GetDefaultInstanceCamera()->m_z;
		g_cameraInstancePanTilt.x = g_render.GetDefaultInstanceCamera()->m_pan;
		g_cameraInstancePanTilt.y = g_render.GetDefaultInstanceCamera()->m_tilt;


		fwrite(&g_multipleView->m_nx->m_hasScene, sizeof(CBool), 1, filePtr);
		fwrite(g_multipleView->m_nx->m_sceneName, sizeof(CChar), MAX_NAME_SIZE, filePtr);
		fwrite(&g_characterPos, sizeof(NxExtendedVec3), 1, filePtr);
		fwrite(&g_cameraPitchYawTilt, sizeof(CVec3f), 1, filePtr);
		fwrite(&g_cameraInstancePos, sizeof(CVec3f), 1, filePtr);
		fwrite(&g_cameraInstancePanTilt, sizeof(CVec2f), 1, filePtr);

		CInt tempSceneSize = (CInt)g_scene.size();
		CInt tempGeoSize;
		fwrite(&tempSceneSize, 1, sizeof(CInt), filePtr);
		for (CUInt i = 0; i < g_scene.size(); i++)
		{
			//CChar path[MAX_NAME_SIZE];
			//CChar file[MAX_NAME_SIZE];
			//Cpy( file, g_scene[i]->m_pureFileName );
			//GetWithoutDot(file);
			//sprintf( path, "Assets/meshes/%s/%s", file, g_scene[i]->m_pureFileName );

			fwrite(g_scene[i]->GetFileName(), 1, sizeof(CChar) * MAX_NAME_SIZE, filePtr);
			CInt clipIndex = g_scene[i]->GetClipIndexForStartup();
			fwrite(&clipIndex, 1, sizeof(CInt), filePtr);
			fwrite(&g_scene[i]->m_playAnimation, 1, sizeof(CBool), filePtr);
			fwrite(&g_scene[i]->m_loopAnimationAtStartup, 1, sizeof(CBool), filePtr);
			fwrite(&g_scene[i]->m_isVisible, 1, sizeof(CBool), filePtr);

			tempGeoSize = (CInt)g_scene[i]->m_geometries.size();
			fwrite(&tempGeoSize, 1, sizeof(CInt), filePtr);

			/////////////////////////////////////
			for (CUInt j = 0; j < g_scene[i]->m_geometries.size(); j++)
			{

				fwrite(&g_scene[i]->m_geometries[j]->m_hasDirtMap, sizeof(CBool), 1, filePtr);
				fwrite(g_scene[i]->m_geometries[j]->m_strDirtMap, sizeof(CChar), MAX_NAME_SIZE, filePtr);

				fwrite(&g_scene[i]->m_geometries[j]->m_hasNormalMap, sizeof(CBool), 1, filePtr);
				fwrite(g_scene[i]->m_geometries[j]->m_strNormalMap, sizeof(CChar), MAX_NAME_SIZE, filePtr);

				fwrite(&g_scene[i]->m_geometries[j]->m_parallaxMapBias, sizeof(CFloat), 1, filePtr);
				fwrite(&g_scene[i]->m_geometries[j]->m_parallaxMapScale, sizeof(CFloat), 1, filePtr);

				fwrite(&g_scene[i]->m_geometries[j]->m_hasGlossMap, sizeof(CBool), 1, filePtr);
				fwrite(g_scene[i]->m_geometries[j]->m_strGlossMap, sizeof(CChar), MAX_NAME_SIZE, filePtr);

				fwrite(&g_scene[i]->m_geometries[j]->m_hasHeightMap, sizeof(CBool), 1, filePtr);
				fwrite(g_scene[i]->m_geometries[j]->m_strHeightMap, sizeof(CChar), MAX_NAME_SIZE, filePtr);

				fwrite(&g_scene[i]->m_geometries[j]->m_hasDuDvMap, sizeof(CBool), 1, filePtr);
				fwrite(g_scene[i]->m_geometries[j]->m_strDuDvMap, sizeof(CChar), MAX_NAME_SIZE, filePtr);

				fwrite(&g_scene[i]->m_geometries[j]->m_hasDiffuse, sizeof(CBool), 1, filePtr);
				fwrite(g_scene[i]->m_geometries[j]->m_strDiffuse, sizeof(CChar), MAX_NAME_SIZE, filePtr);

				//save cull face( enabled or disabled data) for the current geo
				fwrite(&g_scene[i]->m_geometries[j]->m_cullFaces, sizeof(CBool), 1, filePtr);

				//write PhysX data
				CUInt physx_point_size = (CUInt)g_scene[i]->m_geometries[j]->m_physx_points.size();
				fwrite(&physx_point_size, sizeof(CUInt), 1, filePtr);
				for (CUInt m = 0; m < g_scene[i]->m_geometries[j]->m_physx_points.size(); m++)
				{
					fwrite(g_scene[i]->m_geometries[j]->m_physx_points[m], sizeof(CVec3f), 1, filePtr);
				}

				CUInt physx_triangles_size = (CUInt)g_scene[i]->m_geometries[j]->m_physx_triangles.size();
				fwrite(&physx_triangles_size, sizeof(CUInt), 1, filePtr);
				for (CUInt m = 0; m < g_scene[i]->m_geometries[j]->m_physx_triangles.size(); m++)
				{
					fwrite(&g_scene[i]->m_geometries[j]->m_physx_triangles[m]->m_count, sizeof(CUInt), 1, filePtr);
					for (CUInt n = 0; n <g_scene[i]->m_geometries[j]->m_physx_triangles[m]->m_count * 3; n++)
					{
						fwrite(&g_scene[i]->m_geometries[j]->m_physx_triangles[m]->m_indexes[n], sizeof(CUInt), 1, filePtr);
					}
				}
				CInt cm_size = g_scene[i]->m_geometries[j]->m_collapseMap.num;
				fwrite(&cm_size, sizeof(CInt), 1, filePtr);
				for (CInt m = 0; m < g_scene[i]->m_geometries[j]->m_collapseMap.num; m++)
				{
					fwrite(&g_scene[i]->m_geometries[j]->m_collapseMap[m], sizeof(CInt), 1, filePtr);
				}
				///////////////////

				CUInt groupSize = CUInt(g_scene[i]->m_geometries[j]->m_groups.size());
				fwrite(&groupSize, sizeof(CUInt), 1, filePtr);
				//store group info

				for (CUInt k = 0; k < groupSize; k++)
				{

					fwrite(&g_scene[i]->m_geometries[j]->m_groups[k]->m_hasGlossMap, sizeof(CBool), 1, filePtr);
					fwrite(g_scene[i]->m_geometries[j]->m_groups[k]->m_strGlossMap, sizeof(CChar), MAX_NAME_SIZE, filePtr);

					fwrite(&g_scene[i]->m_geometries[j]->m_groups[k]->m_hasDirtMap, sizeof(CBool), 1, filePtr);
					fwrite(g_scene[i]->m_geometries[j]->m_groups[k]->m_strDirtMap, sizeof(CChar), MAX_NAME_SIZE, filePtr);

					fwrite(&g_scene[i]->m_geometries[j]->m_groups[k]->m_hasNormalMap, sizeof(CBool), 1, filePtr);
					fwrite(g_scene[i]->m_geometries[j]->m_groups[k]->m_strNormalMap, sizeof(CChar), MAX_NAME_SIZE, filePtr);

					fwrite(&g_scene[i]->m_geometries[j]->m_groups[k]->m_parallaxMapBias, sizeof(CFloat), 1, filePtr);
					fwrite(&g_scene[i]->m_geometries[j]->m_groups[k]->m_parallaxMapScale, sizeof(CFloat), 1, filePtr);

					fwrite(&g_scene[i]->m_geometries[j]->m_groups[k]->m_hasDiffuse, sizeof(CBool), 1, filePtr);
					fwrite(g_scene[i]->m_geometries[j]->m_groups[k]->m_strDiffuse, sizeof(CChar), MAX_NAME_SIZE, filePtr);
				}
			}// for all of the geos

			//write the instance geometries
			CUInt size = (CUInt)g_scene[i]->m_instanceGeometries.size();
			fwrite(&size, 1, sizeof(CUInt), filePtr);
			for (CUInt j = 0; j < g_scene[i]->m_instanceGeometries.size(); j++)
			{
				fwrite(g_scene[i]->m_instanceGeometries[j]->m_abstractGeometry->GetName(), 1, sizeof(CChar) * MAX_NAME_SIZE, filePtr);
				fwrite(g_scene[i]->m_instanceGeometries[j]->m_firstLocalToWorldMatrix, 1, sizeof(CMatrix), filePtr);
				fwrite(&g_scene[i]->m_instanceGeometries[j]->m_hasPhysX, 1, sizeof(CBool), filePtr);
				fwrite(g_scene[i]->m_instanceGeometries[j]->m_physXName, 1, sizeof(CChar) * MAX_NAME_SIZE, filePtr);
				fwrite(&g_scene[i]->m_instanceGeometries[j]->m_lodAlgorithm, 1, sizeof(CPhysXAlgorithm), filePtr);
				fwrite(&g_scene[i]->m_instanceGeometries[j]->m_physXDensity, 1, sizeof(CFloat), filePtr);
				fwrite(&g_scene[i]->m_instanceGeometries[j]->m_physXPercentage, 1, sizeof(CInt), filePtr);
				fwrite(&g_scene[i]->m_instanceGeometries[j]->m_isTrigger, 1, sizeof(CBool), filePtr);
				fwrite(&g_scene[i]->m_instanceGeometries[j]->m_isInvisible, 1, sizeof(CBool), filePtr);
				fwrite(&g_scene[i]->m_instanceGeometries[j]->m_enterScript, 1, sizeof(CChar) * MAX_NAME_SIZE, filePtr);
				fwrite(&g_scene[i]->m_instanceGeometries[j]->m_hasEnterScript, 1, sizeof(CBool), filePtr);
				fwrite(&g_scene[i]->m_instanceGeometries[j]->m_exitScript, 1, sizeof(CChar) * MAX_NAME_SIZE, filePtr);
				fwrite(&g_scene[i]->m_instanceGeometries[j]->m_hasExitScript, 1, sizeof(CBool), filePtr);

			}
		} //for all of the scenes

		fclose(filePtr);

//		//save functions////////////////////////////////////////

		CBool foundTarget = CFalse;
		CChar pureFileName2[MAX_NAME_SIZE];
		Cpy(pureFileName2, m_strNewPackageAndPrefabName);
		for (CUInt vs = 0; vs < g_VSceneNamesOfCurrentProject.size(); vs++)
		{
			if (Cmp(pureFileName2, g_VSceneNamesOfCurrentProject[vs].c_str()))
			{
				foundTarget = CTrue;
				break;
			}
		}

		if (!foundTarget)
		{
			g_allPrefabNames.push_back(GetAfterPath(pureFileName));
		}

		//save functions///////////////////////////////////

		Cpy(g_currentPackageAndPrefabName, m_strNewPackageAndPrefabName);
		Cpy(g_currentPrefabName, m_strNewPrefabName);
		Cpy(g_currentPackageName, m_strNewPackageName);

		CChar temp[256];
		sprintf(temp, "%s%s%s", "Vanda Engine 1.4 : Prefab Mode (", g_currentPackageAndPrefabName, ")");
		ex_pVandaEngine1Dlg->SetWindowTextA(temp);

		if (m_dlgSavePrefabs)
			CDelete(m_dlgSavePrefabs);
		PrintInfo("\nPrefab Saved Successfully");
		ReleaseCapture();
	}
	else if (result == IDCANCEL)
		return;
}

CVoid CVandaEngine1Dlg::OnMenuClickedSaveAs(CBool askQuestion)
{
	CInt result;
	CEditVSceneName* dlgSave;
	dlgSave = CNew(CEditVSceneName);
	dlgSave->SetInitialData(CTrue);
	CString m_strpathName;

	if (!askQuestion && m_savePathName.Compare("\n") != 0)
	{
		result = IDOK;
		m_strpathName = m_savePathName;
	}
	else if (Cmp(g_currentVSceneName, "\n")) //nothing has been saved
	{
		result = dlgSave->DoModal();
	}
	else
	{
		result = dlgSave->DoModal();
	}

	if (result == IDOK)
	{
		SetCapture();
		SetCursor( LoadCursorFromFile( "Assets/Engine/Icons/progress.ani") );

		if( Cmp( g_currentVSceneName, "\n" ) ) //nothing has been saved
		{
			m_strpathName = dlgSave->GetName();
			m_savePathName = m_strpathName;
			if( g_vandaDemo )
				Cpy( g_currentPassword, "\n" );
			else
			{
				static string charset = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ1234567890=*&^%$#@!~-+)(_";
				string result;
				result.resize(100);

				for (int i = 0; i < 100; i++)
					result[i] = charset[rand() % charset.length()];
				Cpy(g_currentPassword, result.c_str());
			}
		}
		else if( askQuestion || m_savePathName.Compare( "\n" ) == 0 )
		{
			m_strpathName = (CString)dlgSave->GetName();
			m_savePathName = m_strpathName;
		}
		//Save functions////////////////////////////////////
		
		CBool overwriteExistingScene = CFalse;
		CChar* pureFileName =  (CChar*)GetAfterPath(m_savePathName.GetBuffer( m_savePathName.GetLength()));
		for( CUInt vs = 0; vs < g_VSceneNamesOfCurrentProject.size(); vs++ )
		{
			if (Cmp(pureFileName, g_VSceneNamesOfCurrentProject[vs].c_str() ) )
			{
				overwriteExistingScene = CTrue;
				break;
			}
		}

		CInt saveAlgorithm = -1;
		if( Cmp( g_currentVSceneName, "\n" ) )
		{
			if( overwriteExistingScene ) //scene already exists
			{
				saveAlgorithm = 0;
			}
			else
			{
				saveAlgorithm = 1;
			}
		} 
		else
		{
			if( Cmp( g_currentVSceneName, pureFileName ) ) //2-b
			{
				saveAlgorithm = 2;
			}
			else if( overwriteExistingScene ) //scene already exists 2-a
			{
				saveAlgorithm = 3;
			}
			else //scene does not exists
			{
				saveAlgorithm = 4;
			}
		}
		m_savePathName.ReleaseBuffer();

		//Create All Directories
		CChar currentSceneNameWithoutDot[MAX_NAME_SIZE];
		Cpy( currentSceneNameWithoutDot, pureFileName );
		GetWithoutDot( currentSceneNameWithoutDot );

		CChar g_currentVSceneNameWithoutDot[MAX_NAME_SIZE];
		Cpy( g_currentVSceneNameWithoutDot, g_currentVSceneName );
		GetWithoutDot( g_currentVSceneNameWithoutDot );

		CChar VScenePath[MAX_NAME_SIZE];
		sprintf( VScenePath, "%s%s", g_currentProjectPath, currentSceneNameWithoutDot );

		CChar waterTexturesPath[MAX_NAME_SIZE];
		sprintf( waterTexturesPath, "%s%s%s", g_currentProjectPath, currentSceneNameWithoutDot, "/Waters/" );

		CChar soundPath[MAX_NAME_SIZE];
		sprintf( soundPath, "%s%s%s", g_currentProjectPath, currentSceneNameWithoutDot, "/Sounds/" );

		CChar ambientSoundPath[MAX_NAME_SIZE];
		sprintf( ambientSoundPath, "%s%s%s", g_currentProjectPath, currentSceneNameWithoutDot, "/Sounds/Ambient/" );

		CChar staticSoundPath[MAX_NAME_SIZE];
		sprintf( staticSoundPath, "%s%s%s", g_currentProjectPath, currentSceneNameWithoutDot, "/Sounds/Static/" );

		CChar skyPath[MAX_NAME_SIZE];
		sprintf( skyPath, "%s%s%s", g_currentProjectPath, currentSceneNameWithoutDot, "/Sky/" );

		CChar bannerPath[MAX_NAME_SIZE];
		sprintf( bannerPath, "%s%s%s", g_currentProjectPath, currentSceneNameWithoutDot, "/Banner/" );

		CChar scriptPath[MAX_NAME_SIZE];
		sprintf(scriptPath, "%s%s%s", g_currentProjectPath, currentSceneNameWithoutDot, "/Script/");

		//Directories of g_currentVSceneName
		CChar currentWaterTexturesPath[MAX_NAME_SIZE];
		sprintf( currentWaterTexturesPath, "%s%s%s", g_currentProjectPath, g_currentVSceneNameWithoutDot, "/Waters/" );

		CChar currentSoundPath[MAX_NAME_SIZE];
		sprintf( currentSoundPath, "%s%s%s", g_currentProjectPath, g_currentVSceneNameWithoutDot, "/Sounds/" );

		CChar currentAmbientSoundPath[MAX_NAME_SIZE];
		sprintf( currentAmbientSoundPath, "%s%s%s", g_currentProjectPath, g_currentVSceneNameWithoutDot, "/Sounds/Ambient/" );

		CChar currentStaticSoundPath[MAX_NAME_SIZE];
		sprintf( currentStaticSoundPath, "%s%s%s", g_currentProjectPath, g_currentVSceneNameWithoutDot, "/Sounds/Static/" );

		CChar currentSkyPath[MAX_NAME_SIZE];
		sprintf( currentSkyPath, "%s%s%s", g_currentProjectPath, g_currentVSceneNameWithoutDot, "/Sky/" );

		CChar currentBannerPath[MAX_NAME_SIZE];
		sprintf( currentBannerPath, "%s%s%s", g_currentProjectPath, g_currentVSceneNameWithoutDot, "/Banner/" );

		CChar currentScriptPath[MAX_NAME_SIZE];
		sprintf(currentScriptPath, "%s%s%s", g_currentProjectPath, g_currentVSceneNameWithoutDot, "/Script/");

		if( saveAlgorithm == 0 || saveAlgorithm == 3)
		{
			//Remove the contents of existing directory
			RemoveAllFilesInDirectory( waterTexturesPath );
			RemoveAllFilesInDirectory( ambientSoundPath );
			RemoveAllFilesInDirectory( staticSoundPath );
			RemoveAllFilesInDirectory( skyPath );
			RemoveAllFilesInDirectory(bannerPath);
			RemoveAllFilesInDirectory( scriptPath );

		}
		else if( saveAlgorithm == 1 || saveAlgorithm == 4)
		{
			//create a new directory based on the save file name
			CreateWindowsDirectory( VScenePath );
			CreateWindowsDirectory( waterTexturesPath );
			CreateWindowsDirectory( soundPath );
			CreateWindowsDirectory( ambientSoundPath );
			CreateWindowsDirectory( staticSoundPath );
			CreateWindowsDirectory( skyPath );
			CreateWindowsDirectory(bannerPath);
			CreateWindowsDirectory(scriptPath);

		}
		if( saveAlgorithm == 0 || saveAlgorithm == 1 || saveAlgorithm == 2 )
		{
			//copy the assets from original place to the existing directory
			//banner
			if( g_sceneBanner.IsInVSceneList(pureFileName, CTrue, CTrue ) )
			{
				CopyOneFileToDstDirectory( g_sceneBanner.GetBannerPath(), bannerPath );
			}
			CChar* tempAfterPath = GetAfterPath(g_sceneBanner.GetBannerPath());
			CChar newPathAndName[MAX_NAME_SIZE];
			Cpy(newPathAndName, bannerPath );
			Append(newPathAndName, tempAfterPath );
			g_sceneBanner.SetBannerPath( newPathAndName );

			//scripts
			for (CUInt j = 0; j < g_instancePrefab.size(); j++)
			{
				if (CmpIn(g_instancePrefab[j]->GetName(), "Vanda_Basics_Box_Trigger") || CmpIn(g_instancePrefab[j]->GetName(), "Vanda_Basics_Sphere_Trigger"))
				{
					CScene* scene = g_instancePrefab[j]->GetScene(0);
					for (CUInt k = 0; k < scene->m_instanceGeometries.size(); k++)
					{
						if (scene->m_instanceGeometries[k]->m_hasEnterScript && scene->m_instanceGeometries[k]->m_updateEnterScript)
						{
							CopyOneFileToDstDirectory(scene->m_instanceGeometries[k]->m_enterScript, scriptPath);
							scene->m_instanceGeometries[k]->m_updateEnterScript = CFalse;

							CChar* TempAfterPath = GetAfterPath(scene->m_instanceGeometries[k]->m_enterScript);
							CChar NewPathAndName[MAX_NAME_SIZE];
							Cpy(NewPathAndName, scriptPath);
							Append(NewPathAndName, TempAfterPath);
							Cpy(scene->m_instanceGeometries[k]->m_enterScript, NewPathAndName);
						}
						if (scene->m_instanceGeometries[k]->m_hasExitScript && scene->m_instanceGeometries[k]->m_updateExitScript)
						{
							CopyOneFileToDstDirectory(scene->m_instanceGeometries[k]->m_exitScript, scriptPath);
							scene->m_instanceGeometries[k]->m_updateExitScript = CFalse;

							CChar* TempAfterPath = GetAfterPath(scene->m_instanceGeometries[k]->m_exitScript);
							CChar NewPathAndName[MAX_NAME_SIZE];
							Cpy(NewPathAndName, scriptPath);
							Append(NewPathAndName, TempAfterPath);
							Cpy(scene->m_instanceGeometries[k]->m_exitScript, NewPathAndName);
						}

						if (scene->m_instanceGeometries[k]->m_hasEnterScript)
						{
							g_instancePrefab[j]->SetEnterScript(scene->m_instanceGeometries[k]->m_enterScript);
						}
						else
						{
							g_instancePrefab[j]->SetEnterScript("\n");
						}

						if (scene->m_instanceGeometries[k]->m_hasExitScript)
						{
							g_instancePrefab[j]->SetExitScript(scene->m_instanceGeometries[k]->m_exitScript);
						}
						else
						{
							g_instancePrefab[j]->SetExitScript("\n");
						}

					}
				}
			}

			//sky dome
			if( g_skyDome )
			{
				if( g_skyDome->IsInVSceneList(pureFileName, CTrue, CTrue ) )
				{
					CopyOneFileToDstDirectory( g_skyDome->GetPath(), skyPath );
				}
				CChar* tempAfterPath = GetAfterPath(g_skyDome->GetPath());
				CChar newPathAndName[MAX_NAME_SIZE];
				Cpy(newPathAndName, skyPath );
				Append(newPathAndName, tempAfterPath );
				g_skyDome->SetPath( newPathAndName );
			}
			//ambient sound
			if( g_multipleView->m_ambientSound )
			{
				if( g_multipleView->m_ambientSound->IsInVSceneList(pureFileName, CTrue, CTrue ) )
				{
					CopyOneFileToDstDirectory( g_multipleView->m_ambientSound->GetPath(), ambientSoundPath );
				}
				CChar* tempAfterPath = GetAfterPath(g_multipleView->m_ambientSound->GetPath());
				CChar newPathAndName[MAX_NAME_SIZE];
				Cpy(newPathAndName, ambientSoundPath );
				Append(newPathAndName, tempAfterPath );
				g_multipleView->m_ambientSound->SetPath( newPathAndName );
			}
			//3d sounds
			//scenes
			for( CUInt i = 0 ; i < g_engineStaticSounds.size(); i++ )
			{
				if( g_engineStaticSounds[i]->IsInVSceneList(pureFileName,CTrue, CTrue ) )
				{
					CopyOneFileToDstDirectory( g_engineStaticSounds[i]->GetPath(), staticSoundPath );
				}
				CChar* tempAfterPath = GetAfterPath(g_engineStaticSounds[i]->GetPath());
				CChar newPathAndName[MAX_NAME_SIZE];
				Cpy(newPathAndName, staticSoundPath );
				Append(newPathAndName, tempAfterPath );
				g_engineStaticSounds[i]->SetPath( newPathAndName );
			}
			//waters
			for( CUInt i = 0 ; i < g_engineWaters.size(); i++ )
			{
				if( g_engineWaters[i]->IsInVSceneList(pureFileName,CTrue, CTrue ) )
				{
					CopyOneFileToDstDirectory( g_engineWaters[i]->GetDuDvMapName(), waterTexturesPath );
					CopyOneFileToDstDirectory( g_engineWaters[i]->GetNormalMapName(), waterTexturesPath );
				}
				CChar* dudvTempAfterPath = GetAfterPath(g_engineWaters[i]->GetDuDvMapName());
				CChar dudvNewPathAndName[MAX_NAME_SIZE];
				Cpy(dudvNewPathAndName, waterTexturesPath );
				Append(dudvNewPathAndName, dudvTempAfterPath );
				g_engineWaters[i]->SetDuDvMapName( dudvNewPathAndName );

				CChar* normalTempAfterPath = GetAfterPath(g_engineWaters[i]->GetNormalMapName());
				CChar normalNewPathAndName[MAX_NAME_SIZE];
				Cpy(normalNewPathAndName, waterTexturesPath );
				Append(normalNewPathAndName, normalTempAfterPath );
				g_engineWaters[i]->SetNormalMapName( normalNewPathAndName );
			}

		}
		else if( saveAlgorithm == 3 || saveAlgorithm == 4 )
		{
			//copy the assets from current place to the saved directory
			CopyAllFilesFromSrcToDstDirectory(currentWaterTexturesPath, waterTexturesPath);
			CopyAllFilesFromSrcToDstDirectory(currentAmbientSoundPath, ambientSoundPath);
			CopyAllFilesFromSrcToDstDirectory(currentStaticSoundPath, staticSoundPath);
			CopyAllFilesFromSrcToDstDirectory(currentSkyPath, skyPath);
			CopyAllFilesFromSrcToDstDirectory(currentBannerPath, bannerPath);
			CopyAllFilesFromSrcToDstDirectory(currentScriptPath, scriptPath);

			//Banner
			if( g_sceneBanner.IsInVSceneList(g_currentVSceneName, CTrue, CFalse ) )
			{
				CopyOneFileToDstDirectory( g_sceneBanner.GetBannerPath(), bannerPath );
			}
			CChar* tempAfterPath = GetAfterPath(g_sceneBanner.GetBannerPath());
			CChar newPathAndName[MAX_NAME_SIZE];
			Cpy(newPathAndName, bannerPath );
			Append(newPathAndName, tempAfterPath );
			g_sceneBanner.SetBannerPath( newPathAndName );
			g_sceneBanner.IsInVSceneList(pureFileName,CTrue, CTrue );

			//scripts
			for (CUInt j = 0; j < g_instancePrefab.size(); j++)
			{
				if (CmpIn(g_instancePrefab[j]->GetName(), "Vanda_Basics_Box_Trigger") || CmpIn(g_instancePrefab[j]->GetName(), "Vanda_Basics_Sphere_Trigger"))
				{
					CScene* scene = g_instancePrefab[j]->GetScene(0);
					for (CUInt k = 0; k < scene->m_instanceGeometries.size(); k++)
					{
						if (scene->m_instanceGeometries[k]->m_hasEnterScript && scene->m_instanceGeometries[k]->m_updateEnterScript)
						{
							CopyOneFileToDstDirectory(scene->m_instanceGeometries[k]->m_enterScript, scriptPath);
							scene->m_instanceGeometries[k]->m_updateEnterScript = CFalse;

							CChar* TempAfterPath = GetAfterPath(scene->m_instanceGeometries[k]->m_enterScript);
							CChar NewPathAndName[MAX_NAME_SIZE];
							Cpy(NewPathAndName, scriptPath);
							Append(NewPathAndName, TempAfterPath);
							Cpy(scene->m_instanceGeometries[k]->m_enterScript, NewPathAndName);
						}
						if (scene->m_instanceGeometries[k]->m_hasExitScript && scene->m_instanceGeometries[k]->m_updateExitScript)
						{
							CopyOneFileToDstDirectory(scene->m_instanceGeometries[k]->m_exitScript, scriptPath);
							scene->m_instanceGeometries[k]->m_updateExitScript = CFalse;

							CChar* TempAfterPath = GetAfterPath(scene->m_instanceGeometries[k]->m_exitScript);
							CChar NewPathAndName[MAX_NAME_SIZE];
							Cpy(NewPathAndName, scriptPath);
							Append(NewPathAndName, TempAfterPath);
							Cpy(scene->m_instanceGeometries[k]->m_exitScript, NewPathAndName);
						}

						if (scene->m_instanceGeometries[k]->m_hasEnterScript)
						{
							g_instancePrefab[j]->SetEnterScript(scene->m_instanceGeometries[k]->m_enterScript);
						}
						else
						{
							g_instancePrefab[j]->SetEnterScript("\n");
						}

						if (scene->m_instanceGeometries[k]->m_hasExitScript)
						{
							g_instancePrefab[j]->SetExitScript(scene->m_instanceGeometries[k]->m_exitScript);
						}
						else
						{
							g_instancePrefab[j]->SetExitScript("\n");
						}


					}
				}
			}

			//sky
			if( g_skyDome )
			{
				if( g_skyDome->IsInVSceneList(g_currentVSceneName, CTrue, CFalse ) )
				{
					CopyOneFileToDstDirectory( g_skyDome->GetPath(), skyPath );
				}
				CChar* tempAfterPath = GetAfterPath(g_skyDome->GetPath());
				CChar newPathAndName[MAX_NAME_SIZE];
				Cpy(newPathAndName, skyPath );
				Append(newPathAndName, tempAfterPath );
				g_skyDome->SetPath( newPathAndName );

				g_skyDome->IsInVSceneList(pureFileName,CTrue, CTrue );
			}
			//ambient sound
			if( g_multipleView->m_ambientSound )
			{
				if( g_multipleView->m_ambientSound->IsInVSceneList(g_currentVSceneName, CTrue, CFalse ) )
				{
					CopyOneFileToDstDirectory( g_multipleView->m_ambientSound->GetPath(), ambientSoundPath );
				}
				CChar* tempAfterPath = GetAfterPath(g_multipleView->m_ambientSound->GetPath());
				CChar newPathAndName[MAX_NAME_SIZE];
				Cpy(newPathAndName, ambientSoundPath );
				Append(newPathAndName, tempAfterPath );
				g_multipleView->m_ambientSound->SetPath( newPathAndName );

				g_multipleView->m_ambientSound->IsInVSceneList(pureFileName,CTrue, CTrue );
			}
			//3d sounds
			for( CUInt i = 0 ; i < g_engineStaticSounds.size(); i++ )
			{
				if( g_engineStaticSounds[i]->IsInVSceneList(g_currentVSceneName,CTrue, CFalse ) )
				{
					CopyOneFileToDstDirectory( g_engineStaticSounds[i]->GetPath(), staticSoundPath );
				}
				CChar* tempAfterPath = GetAfterPath(g_engineStaticSounds[i]->GetPath());
				CChar newPathAndName[MAX_NAME_SIZE];
				Cpy(newPathAndName, staticSoundPath );
				Append(newPathAndName, tempAfterPath );
				g_engineStaticSounds[i]->SetPath( newPathAndName );

				g_engineStaticSounds[i]->IsInVSceneList(pureFileName,CTrue, CTrue );

			}

			//waters
			for( CUInt i = 0 ; i < g_engineWaters.size(); i++ )
			{
				if( g_engineWaters[i]->IsInVSceneList(g_currentVSceneName,CTrue, CFalse ) )
				{
					CopyOneFileToDstDirectory( g_engineWaters[i]->GetDuDvMapName(), waterTexturesPath );
					CopyOneFileToDstDirectory( g_engineWaters[i]->GetNormalMapName(), waterTexturesPath );
				}
				CChar* dudvTempAfterPath = GetAfterPath(g_engineWaters[i]->GetDuDvMapName());
				CChar dudvNewPathAndName[MAX_NAME_SIZE];
				Cpy(dudvNewPathAndName, waterTexturesPath );
				Append(dudvNewPathAndName, dudvTempAfterPath );
				g_engineWaters[i]->SetDuDvMapName( dudvNewPathAndName );

				CChar* normalTempAfterPath = GetAfterPath(g_engineWaters[i]->GetNormalMapName());
				CChar normalNewPathAndName[MAX_NAME_SIZE];
				Cpy(normalNewPathAndName, waterTexturesPath );
				Append(normalNewPathAndName, normalTempAfterPath );
				g_engineWaters[i]->SetNormalMapName( normalNewPathAndName );

				g_engineWaters[i]->IsInVSceneList(pureFileName,CTrue, CTrue );
			}
		}
		HANDLE hFind;
		WIN32_FIND_DATA data;

		//Delete removed banners
		CChar bannerTempPath[MAX_NAME_SIZE];
		sprintf( bannerTempPath, "%s%s", bannerPath, "*.*" );
		hFind = FindFirstFile( bannerTempPath, &data );
		do
		{
			CChar bannerTempPath[MAX_NAME_SIZE];
			sprintf( bannerTempPath, "%s%s", bannerPath, data.cFileName );
			CBool foundTarget = CFalse;
			if( Cmp( GetAfterPath( g_sceneBanner.GetBannerPath()), data.cFileName ) )
			{
				foundTarget = CTrue;
			}

			//Remove Files
			if( !foundTarget )
			{
				if( !DeleteFile( bannerTempPath) )
				{
					//CChar temp[MAX_NAME_SIZE];
					//sprintf( temp, "\n%s%s", "Error: Couldn't remove the file ", data.cFileName );
					//PrintInfo( temp, COLOR_RED );
				}
			}
		}while (FindNextFile( hFind, &data));
		FindClose(hFind);

		//Delete removed scripts
		CChar tempScriptPath[MAX_NAME_SIZE];
		sprintf(tempScriptPath, "%s%s", scriptPath, "*.*");
		hFind = FindFirstFile(tempScriptPath, &data);
		do
		{
			CChar scriptTempPath[MAX_NAME_SIZE];
			sprintf(scriptTempPath, "%s%s", scriptPath, data.cFileName);

			CBool foundTarget = CFalse;
			for (CUInt j = 0; j < g_instancePrefab.size(); j++)
			{
				if (CmpIn(g_instancePrefab[j]->GetName(), "Vanda_Basics_Box_Trigger") || CmpIn(g_instancePrefab[j]->GetName(), "Vanda_Basics_Sphere_Trigger"))
				{
					CScene* scene = g_instancePrefab[j]->GetScene(0);
					for (CUInt k = 0; k < scene->m_instanceGeometries.size(); k++)
					{
						if (scene->m_instanceGeometries[k]->m_hasEnterScript && Cmp(GetAfterPath(scene->m_instanceGeometries[k]->m_enterScript), data.cFileName))
						{
							foundTarget = CTrue;
							break;
						}
						if (scene->m_instanceGeometries[k]->m_hasExitScript && Cmp(GetAfterPath(scene->m_instanceGeometries[k]->m_exitScript), data.cFileName))
						{
							foundTarget = CTrue;
							break;
						}
					}
				}
			}

			//Remove Files
			if (!foundTarget)
			{
				if (!DeleteFile(scriptTempPath))
				{
					//CChar temp[MAX_NAME_SIZE];
					//sprintf( temp, "\n%s%s", "Error: Couldn't remove the file ", data.cFileName );
					//PrintInfo( temp, COLOR_RED );
				}
			}
		} while (FindNextFile(hFind, &data));
		FindClose(hFind);

		//Delete removed sky textures
		if( g_skyDome )
		{
			CChar skyTempPath[MAX_NAME_SIZE];
			sprintf( skyTempPath, "%s%s", skyPath, "*.*" );
			hFind = FindFirstFile( skyTempPath, &data );
			do
			{
				CChar skyTempPath[MAX_NAME_SIZE];
				sprintf( skyTempPath, "%s%s", skyPath, data.cFileName );
				CBool foundTarget = CFalse;
				if( Cmp( GetAfterPath( g_skyDome->GetPath()), data.cFileName ) )
				{
					foundTarget = CTrue;
				}

				//Remove Files
				if( !foundTarget )
				{
					if( !DeleteFile( skyTempPath) )
					{
						//CChar temp[MAX_NAME_SIZE];
						//sprintf( temp, "\n%s%s", "Error: Couldn't remove the file ", data.cFileName );
						//PrintInfo( temp, COLOR_RED );
					}
				}
			}while (FindNextFile( hFind, &data));
			FindClose(hFind);
		}
		else
		{
			RemoveAllFilesInDirectory( skyPath );
		}


		//delete removed ambient sounds
		if( g_multipleView->m_ambientSound )
		{
			CChar ambientSoundTempPath[MAX_NAME_SIZE];
			sprintf( ambientSoundTempPath, "%s%s", ambientSoundPath, "*.*" );
			hFind = FindFirstFile( ambientSoundTempPath, &data );
			do
			{
				CChar soundTempPath[MAX_NAME_SIZE];
				sprintf( soundTempPath, "%s%s", ambientSoundPath, data.cFileName );

				CBool foundTarget = CFalse;
				if( Cmp( GetAfterPath( g_multipleView->m_ambientSound->GetPath()), data.cFileName ) )
				{
					foundTarget = CTrue;
				}

				//Remove Files
				if( !foundTarget )
				{
					if( !DeleteFile( soundTempPath) )
					{
						//CChar temp[MAX_NAME_SIZE];
						//sprintf( temp, "\n%s%s", "Error: Couldn't remove the file ", data.cFileName );
						//PrintInfo( temp, COLOR_RED );
					}
				}
			}while (FindNextFile( hFind, &data));
			FindClose(hFind);
		}
		else
		{
			RemoveAllFilesInDirectory( ambientSoundPath );
		}


		//Delete removed static sounds
		CChar tempStaticSoundPath[MAX_NAME_SIZE];
		sprintf( tempStaticSoundPath, "%s%s", staticSoundPath, "*.*" );
		hFind = FindFirstFile( tempStaticSoundPath, &data );
		do
		{
			CChar soundTempPath[MAX_NAME_SIZE];
			sprintf( soundTempPath, "%s%s", staticSoundPath, data.cFileName );

			CBool foundTarget = CFalse;
			for( CUInt i = 0 ; i < g_engineStaticSounds.size(); i++ )
			{
				if( Cmp( GetAfterPath( g_engineStaticSounds[i]->GetPath()), data.cFileName ) )
				{
					foundTarget = CTrue;
					break;
				}
			}

			//Remove Files
			if( !foundTarget )
			{
				if( !DeleteFile( soundTempPath) )
				{
					//CChar temp[MAX_NAME_SIZE];
					//sprintf( temp, "\n%s%s", "Error: Couldn't remove the file ", data.cFileName );
					//PrintInfo( temp, COLOR_RED );
				}
			}
		}while (FindNextFile( hFind, &data));
		FindClose(hFind);

		//Delete removed water textures
		CChar tempWaterPath[MAX_NAME_SIZE];
		sprintf( tempWaterPath, "%s%s", waterTexturesPath, "*.*" );
		hFind = FindFirstFile( tempWaterPath, &data );
		do
		{
			CChar tempWaterPath[MAX_NAME_SIZE];
			sprintf( tempWaterPath, "%s%s", waterTexturesPath, data.cFileName );

			CBool foundTarget = CFalse;
			for( CUInt i = 0 ; i < g_engineWaters.size(); i++ )
			{
				if( Cmp( GetAfterPath( g_engineWaters[i]->GetDuDvMapName()), data.cFileName ) )
				{
					foundTarget = CTrue;
					break;
				}
				if( Cmp( GetAfterPath( g_engineWaters[i]->GetNormalMapName()), data.cFileName ) )
				{
					foundTarget = CTrue;
					break;
				}

			}

			//Remove Files
			if( !foundTarget )
			{
				if( !DeleteFile( tempWaterPath) )
				{
					//CChar temp[MAX_NAME_SIZE];
					//sprintf( temp, "\n%s%s", "Error: Couldn't remove the file ", data.cFileName );
					//PrintInfo( temp, COLOR_RED );
				}
			}
		}while (FindNextFile( hFind, &data));
		FindClose(hFind);

		//Mark Current VScene 
		Cpy( g_currentVSceneName, pureFileName );


		//Save functions////////////////////////////////////
		CChar* sourcePath = (CChar*)(m_strpathName.GetBuffer(m_strpathName.GetLength()));
		CChar dstPath[MAX_NAME_SIZE];
		sprintf(dstPath, "%s%s%s%s", g_currentProjectPath, currentSceneNameWithoutDot, "/", GetAfterPath(g_currentVSceneName));

		FILE *filePtr;
		filePtr = fopen(dstPath, "wb");
		if(!filePtr )
		{
			CChar temp[MAX_NAME_SIZE];
			sprintf( temp, "%s%s%s", "Couldn't open the file '", m_strpathName, "' to save data" );
			MessageBox( temp , "Vanda Engine Error", MB_OK );
			return;
		}

		//Save version
		CChar engineName[MAX_NAME_SIZE] = "VandaEngine";
		fwrite( &engineName, sizeof( CChar), MAX_NAME_SIZE, filePtr );
		fwrite(  &g_edition, sizeof( CChar), MAX_NAME_SIZE, filePtr );
		fwrite( &g_maxVersion, 1, sizeof( CInt ), filePtr );
		fwrite( &g_minVersion, 1, sizeof( CInt ), filePtr );
		fwrite( &g_bugFixesVersion, 1, sizeof( CInt ), filePtr );
		fwrite(  &g_currentPassword, sizeof( CChar), MAX_NAME_SIZE, filePtr );
		//save engine options

		fwrite( &g_shadowProperties, sizeof( CShadowProperties ), 1, filePtr  );
		fwrite( &g_physXProperties, sizeof( CPhysXProperties ), 1, filePtr );
		fwrite( &g_dofProperties, sizeof( CDOFProperties ), 1, filePtr  );
		fwrite( &g_fogProperties, sizeof( CFogProperties ), 1, filePtr  );
		fwrite( &g_bloomProperties, sizeof( CBloomProperties ), 1, filePtr  );
		fwrite( &g_lightProperties, sizeof( CLightProperties ), 1, filePtr  );
		fwrite( &g_pathProperties, sizeof( CPathProperties ), 1, filePtr  );
		fwrite( &g_vandaDemo, sizeof(CBool), 1, filePtr);
		fwrite(g_sceneBanner.GetBannerPath(), sizeof(CChar), MAX_NAME_SIZE, filePtr );
		fwrite( &g_extraTexturesNamingConventions, sizeof( CExtraTexturesNamingConventions ), 1, filePtr  );
		fwrite( &g_useGlobalAmbientColor, sizeof( CBool ), 1, filePtr  );
		fwrite( &g_globalAmbientColor, sizeof( CColor4f ), 1, filePtr  );
		fwrite( &g_sceneManagerObjectsPerSplit, sizeof( CInt), 1, filePtr );

		//save physx data
		CBool foundCharacterTarget = CFalse;
		CInstancePrefab* instance_prefab = NULL;
		for (CUInt i = 0; i < g_instancePrefab.size(); i++)
		{
			if (Cmp(g_instancePrefab[i]->GetPrefab()->GetPackageName(), "Vanda_Basics") && Cmp(g_instancePrefab[i]->GetPrefab()->GetPrefabName(), "Character"))
			{
				instance_prefab = g_instancePrefab[i];
				foundCharacterTarget = CTrue;
				break;
			}
		}
		if (foundCharacterTarget)
		{
			g_characterPos.x = instance_prefab->GetTranslate().x;
			g_characterPos.y = instance_prefab->GetTranslate().y + (g_physXProperties.m_fCapsuleHeight / 2.f);
			g_characterPos.z = instance_prefab->GetTranslate().z;

			g_cameraPitchYawTilt.x = 0.0f; // no pitch
			g_cameraPitchYawTilt.y = NxMath::degToRad(instance_prefab->GetRotate().y);
			g_cameraPitchYawTilt.z = 0.0f; //tilt
		}
		else
		{ 
			NxExtendedVec3 pos(0.0f, 1.6f, 0.0f);
			g_characterPos = pos;
			g_cameraPitchYawTilt.x = 0.0f;
			g_cameraPitchYawTilt.y = 0.0f;
			g_cameraPitchYawTilt.z = 0.0f;
		}

		g_cameraInstancePos.x = g_render.GetDefaultInstanceCamera()->m_x;
		g_cameraInstancePos.y = g_render.GetDefaultInstanceCamera()->m_y;
		g_cameraInstancePos.z = g_render.GetDefaultInstanceCamera()->m_z;
		g_cameraInstancePanTilt.x = g_render.GetDefaultInstanceCamera()->m_pan;
		g_cameraInstancePanTilt.y = g_render.GetDefaultInstanceCamera()->m_tilt;

		fwrite( &g_multipleView->m_nx->m_hasScene, sizeof( CBool ), 1, filePtr  );
		fwrite( g_multipleView->m_nx->m_sceneName, sizeof( CChar ), MAX_NAME_SIZE, filePtr  );
		fwrite( &g_characterPos, sizeof( NxExtendedVec3 ), 1, filePtr );
		fwrite( &g_cameraPitchYawTilt, sizeof( CVec3f ), 1, filePtr );
		fwrite( &g_cameraInstancePos, sizeof( CVec3f ), 1, filePtr );
		fwrite( &g_cameraInstancePanTilt, sizeof( CVec2f ), 1, filePtr );
		
		//save prefabs here
		CUInt pr_size = 0;
		for (CUInt i = 0; i < g_prefab.size(); i++)
		{
			if (Cmp(g_prefab[i]->GetPackageName(), "Vanda_Basics") && (Cmp(g_prefab[i]->GetPrefabName(), "Box_Trigger") || Cmp(g_prefab[i]->GetPrefabName(), "Sphere_Trigger")))
				continue;
			pr_size++;
		}

		CUInt prefabSize = pr_size;
		fwrite(&prefabSize, sizeof(CUInt), 1, filePtr);
		for (CUInt i = 0; i < g_prefab.size(); i++)
		{
			if (Cmp(g_prefab[i]->GetPackageName(), "Vanda_Basics") && (Cmp(g_prefab[i]->GetPrefabName(), "Box_Trigger") || Cmp(g_prefab[i]->GetPrefabName(), "Sphere_Trigger")))
				continue;
			
			//write prefab data
			fwrite(g_prefab[i]->GetName(), sizeof(CChar), MAX_NAME_SIZE, filePtr);
			fwrite(g_prefab[i]->GetPackageName(), sizeof(CChar), MAX_NAME_SIZE, filePtr);
			fwrite(g_prefab[i]->GetPrefabName(), sizeof(CChar), MAX_NAME_SIZE, filePtr);

			CUInt prefabInstanceSize = g_prefab[i]->GetNumInstances();
			fwrite(&prefabInstanceSize, sizeof(CUInt), 1, filePtr);

			for (CUInt j = 0; j < g_prefab[i]->GetNumInstances(); j++)
			{
				//write instance data
				fwrite(g_prefab[i]->GetInstance(j), sizeof(CInstancePrefab), 1, filePtr);
			}
		}

		fwrite( &g_menu.m_insertAndShowSky, sizeof( CBool ), 1, filePtr );
		if( g_menu.m_insertAndShowSky )
		{
			fwrite( g_skyDome->m_name, sizeof( CChar ), MAX_NAME_SIZE, filePtr );
			fwrite( g_skyDome->m_path, sizeof( CChar ), MAX_NAME_SIZE, filePtr );
			fwrite( &g_skyDome->m_numSlices, sizeof( CInt ), 1, filePtr );
			fwrite( &g_skyDome->m_numSides, sizeof( CInt ), 1, filePtr );
			fwrite( &g_skyDome->m_radius, sizeof( CFloat ), 1, filePtr );
			fwrite( g_skyDome->m_position, sizeof( CFloat ), 3, filePtr );
			fwrite( &g_skyDome->m_dampening, sizeof( CFloat ), 1, filePtr );
			fwrite( &g_skyDome->m_exponential, sizeof( CBool ), 1, filePtr );
		}
		//save all the waters 
		CInt tempWaterCount = (CInt)g_engineWaters.size();
		fwrite( &tempWaterCount, sizeof( CInt ), 1, filePtr );
		for( CUInt i = 0 ; i < g_engineWaters.size(); i++ )
		{
			fwrite( &g_engineWaters[i]->m_strWaterName, sizeof( CChar ), MAX_NAME_SIZE, filePtr  );
			fwrite( &g_engineWaters[i]->m_strNormalMap, sizeof( CChar ), MAX_NAME_SIZE, filePtr  );
			fwrite( &g_engineWaters[i]->m_strDuDvMap, sizeof( CChar ), MAX_NAME_SIZE, filePtr  );
			fwrite( g_engineWaters[i]->m_fWaterCPos, sizeof( CFloat ), 3, filePtr );
			fwrite( g_engineWaters[i]->m_fWaterLPos, sizeof( CFloat ), 3, filePtr );
			fwrite( &g_engineWaters[i]->m_fWaterHeight, sizeof( CFloat ), 1, filePtr );
			fwrite( &g_engineWaters[i]->m_fWaterSpeed, sizeof( CFloat ), 1, filePtr );
			fwrite( &g_engineWaters[i]->m_fWaterScale, sizeof( CFloat ), 1, filePtr );
			fwrite( &g_engineWaters[i]->m_fWaterUV, sizeof( CFloat ), 1, filePtr );
			CInt tempInstancePrefabCount = (CInt)g_engineWaters[i]->m_instancePrefab.size();
			fwrite( &tempInstancePrefabCount, sizeof( CInt ), 1, filePtr );
			for( CUInt j = 0; j < g_engineWaters[i]->m_instancePrefab.size(); j++ )
				fwrite( g_engineWaters[i]->m_instancePrefab[j]->GetName(), sizeof( CChar ), MAX_NAME_SIZE, filePtr );
		}

		//save engine lights
		CInt tempLightCount = (CInt)g_engineLights.size();
		fwrite( &tempLightCount, sizeof( CInt), 1, filePtr );

		for( CUInt i = 0; i < g_engineLights.size(); i++ )
		{
			fwrite( g_engineLights[i]->m_abstractLight->GetName(), sizeof( CChar ), MAX_NAME_SIZE, filePtr  );
			fwrite( g_engineLights[i]->m_abstractLight->GetPosition(), sizeof( CFloat ), 4, filePtr  );
			fwrite( &g_engineLights[i]->m_abstractLight->m_constantAttenuation, sizeof( CFloat ), 1, filePtr  );
			fwrite( &g_engineLights[i]->m_abstractLight->m_linearAttenuation, sizeof( CFloat ), 1, filePtr  );
			fwrite( &g_engineLights[i]->m_abstractLight->m_quadraticAttenuation, sizeof( CFloat ), 1, filePtr  );
			fwrite( &g_engineLights[i]->m_abstractLight->m_shininess, sizeof( CFloat ), 1, filePtr  );
			fwrite( g_engineLights[i]->m_abstractLight->GetAmbient(), sizeof( CFloat ), 4, filePtr  );
			fwrite( g_engineLights[i]->m_abstractLight->GetDiffuse(), sizeof( CFloat ), 4, filePtr  );
			fwrite( g_engineLights[i]->m_abstractLight->GetSpecular(), sizeof( CFloat ), 4, filePtr  );
			fwrite( &g_engineLights[i]->m_abstractLight->m_lightType, sizeof( CLightType ), 1, filePtr  );
			if( g_engineLights[i]->m_abstractLight->m_lightType == eLIGHTTYPE_SPOT)
			{
				fwrite( &g_engineLights[i]->m_abstractLight->m_spotCutoff, sizeof( CFloat ), 1, filePtr  );
				fwrite( g_engineLights[i]->m_abstractLight->GetSpotDirection(), sizeof( CFloat ), 4, filePtr  );
				fwrite( &g_engineLights[i]->m_abstractLight->m_spotExponent, sizeof( CFloat ), 1, filePtr  );
			}
		}
		//save static sounds data
		CInt tempStaticSoundCount = (CInt)g_engineStaticSounds.size();
		fwrite( &tempStaticSoundCount, sizeof( CInt ), 1, filePtr );

		for( CUInt i = 0 ; i < g_engineStaticSounds.size(); i++ )
		{
			fwrite( &g_engineStaticSounds[i]->m_name, sizeof( CChar ), MAX_NAME_SIZE, filePtr  );
			fwrite( &g_engineStaticSounds[i]->m_path, sizeof( CChar ), MAX_NAME_SIZE, filePtr  );
			fwrite( &g_engineStaticSounds[i]->m_loop, sizeof( CBool ), 1, filePtr  );
			fwrite( &g_engineStaticSounds[i]->m_fStaticSoundMaxDistance, sizeof( CFloat ), 1, filePtr  );
			fwrite( &g_engineStaticSounds[i]->m_fStaticSoundPitch, sizeof( CFloat ), 1, filePtr  );
			fwrite( &g_engineStaticSounds[i]->m_play, sizeof( CBool ), 1, filePtr  );
			fwrite( &g_engineStaticSounds[i]->m_fStaticSoundPos, sizeof( CFloat ), 3, filePtr  );
			fwrite( &g_engineStaticSounds[i]->m_fStaticSoundReferenceDistance, sizeof( CFloat ), 1, filePtr  );
			fwrite( &g_engineStaticSounds[i]->m_fStaticSoundRolloff, sizeof( CFloat ), 1, filePtr  );
			fwrite( &g_engineStaticSounds[i]->m_fStaticSoundVolume, sizeof( CFloat ), 1, filePtr  );
		}
		//Save ambient sound
		fwrite( &g_menu.m_insertAmbientSound, sizeof( CBool ), 1, filePtr  );
		if( g_menu.m_insertAmbientSound )
		{
			fwrite( g_multipleView->m_ambientSound->GetName(), sizeof( CChar ), MAX_NAME_SIZE, filePtr  );
			fwrite( g_multipleView->m_ambientSound->GetPath(), sizeof( CChar ), MAX_NAME_SIZE, filePtr  );
			fwrite( &g_multipleView->m_ambientSound->m_volume, sizeof( CFloat ), 1, filePtr  );
			fwrite( &g_multipleView->m_ambientSound->m_pitch, sizeof( CFloat ), 1, filePtr  );
		}
		//Save triggers
		CInt triggerSize = (CInt)g_triggers.size();
		fwrite(&triggerSize, sizeof(CInt), 1, filePtr);
		for (CUInt i = 0; i < g_triggers.size(); i++)
		{
			CTriggerType type = g_triggers[i]->GetTriggerType();
			CVec3f translation = g_triggers[i]->GetInstancePrefab()->GetTranslate();
			CVec4f rotation = g_triggers[i]->GetInstancePrefab()->GetRotate();
			CVec3f scaling = g_triggers[i]->GetInstancePrefab()->GetScale();
			CChar m_enterScript[MAX_NAME_SIZE];
			CChar m_exitScript[MAX_NAME_SIZE];
			Cpy(m_enterScript, g_triggers[i]->GetInstancePrefab()->GetScene(0)->m_instanceGeometries[0]->m_enterScript);
			Cpy(m_exitScript, g_triggers[i]->GetInstancePrefab()->GetScene(0)->m_instanceGeometries[0]->m_exitScript);

			fwrite(g_triggers[i]->GetName(), sizeof(CChar), MAX_NAME_SIZE, filePtr);
			fwrite(&type, sizeof(CTriggerType), 1, filePtr);

			//save instance data
			CChar instance_name[MAX_NAME_SIZE];
			Cpy(instance_name, g_triggers[i]->GetInstancePrefab()->GetName());
			fwrite(instance_name, sizeof(CChar), MAX_NAME_SIZE, filePtr);
			fwrite(&translation, sizeof(CVec3f), 1, filePtr);
			fwrite(&rotation, sizeof(CVec4f), 1, filePtr);
			fwrite(&scaling, sizeof(CVec3f), 1, filePtr);
			fwrite(m_enterScript, sizeof(CChar), MAX_NAME_SIZE, filePtr);
			fwrite(m_exitScript, sizeof(CChar), MAX_NAME_SIZE, filePtr);
		}

		fclose( filePtr );

		//save functions////////////////////////////////////////

		CBool foundTarget = CFalse;
		CChar* pureFileName2 =  (CChar*)GetAfterPath(m_strpathName.GetBuffer( m_strpathName.GetLength()));
		for( CUInt vs = 0; vs < g_VSceneNamesOfCurrentProject.size(); vs++ )
		{
			if (Cmp(pureFileName2, g_VSceneNamesOfCurrentProject[vs].c_str() ) )
			{
				foundTarget = CTrue;
				break;
			}
		}

		if( !foundTarget )
		{
			g_VSceneNamesOfCurrentProject.push_back( GetAfterPath(pureFileName) );

			for( CUInt p = 0; p < g_projects.size(); p++ )
			{
				if( g_projects[p]->m_isActive )
				{
					g_projects[p]->m_sceneNames.push_back(GetAfterPath(pureFileName));
					break;
				}
			}
		}
		m_strpathName.ReleaseBuffer();

		FILE *ProjectsFilePtr;
		CChar DATPath[MAX_NAME_SIZE];
		sprintf( DATPath, "%s%s", g_projectsPath, "projects.dat" );

		DeleteFile( DATPath );
ProjectsFilePtr = fopen(DATPath, "wb");
if (!ProjectsFilePtr)
{
	MessageBox("Couldn't open 'assets/Projects/projects.dat' to save data!", "Vanda Engine Error", MB_OK | MB_ICONERROR);
	//return;
}

//save projects

CInt numProjects = (CInt)g_projects.size();
fwrite(&numProjects, sizeof(CInt), 1, ProjectsFilePtr);
fclose(ProjectsFilePtr);

for (CInt i = 0; i < numProjects; i++)
{
	CChar filePath[MAX_URI_SIZE];
	sprintf(filePath, "%s%s%s%s", g_projectsPath, "PRJ/", g_projects[i]->m_name, ".prj");
	ProjectsFilePtr = fopen(filePath, "wb");

	fwrite(g_projects[i]->m_name, sizeof(CChar), MAX_NAME_SIZE, ProjectsFilePtr);
	CInt numScenes = (CInt)g_projects[i]->m_sceneNames.size();
	fwrite(&numScenes, sizeof(CInt), 1, ProjectsFilePtr);
	for (CInt j = 0; j < numScenes; j++)
	{
		CChar vsceneName[MAX_NAME_SIZE];
		Cpy(vsceneName, g_projects[i]->m_sceneNames[j].c_str());
		fwrite(vsceneName, sizeof(CChar), MAX_NAME_SIZE, ProjectsFilePtr);
	}
	fwrite(&g_projects[i]->m_isActive, sizeof(CBool), 1, ProjectsFilePtr);

	fclose(ProjectsFilePtr);

}

//save functions///////////////////////////////////
for (CUInt i = 0; i < g_projects.size(); i++)
{
	if (g_projects[i]->m_isActive)
	{
		CChar m_currentVSceneNameWithoutDot[MAX_NAME_SIZE];
		if (Cmp(g_currentVSceneName, "\n"))
			Cpy(m_currentVSceneNameWithoutDot, "Untitled");
		else
		{
			Cpy(m_currentVSceneNameWithoutDot, g_currentVSceneName);
			GetWithoutDot(m_currentVSceneNameWithoutDot);
		}

		CChar temp[256];
		sprintf(temp, "%s%s%s%s%s", "Vanda Engine 1.4 (", g_projects[i]->m_name, " - ", m_currentVSceneNameWithoutDot, ")");
		ex_pVandaEngine1Dlg->SetWindowTextA(temp);

		break;
	}
}

CChar packagePath[MAX_NAME_SIZE];
sprintf(packagePath, "%s%s%s", g_currentProjectPath, currentSceneNameWithoutDot, "/packages.pkg");
FILE *PackageFilePtr;
PackageFilePtr = fopen(packagePath, "wb");
if (!PackageFilePtr)
{
	CChar temp[MAX_NAME_SIZE];
	sprintf(temp, "%s%s%s", "Couldn't open the file '", packagePath, "' to save data");
	MessageBox(temp, "Vanda Engine Error", MB_OK);
	ReleaseCapture();
	return;
}
CUInt Size = g_prefab.size();
fwrite(&Size, sizeof(CUInt), 1, PackageFilePtr);
for (CUInt i = 0; i < g_prefab.size(); i++)
{
	//write prefab data
	fwrite(g_prefab[i]->GetName(), sizeof(CChar), MAX_NAME_SIZE, PackageFilePtr);
	fwrite(g_prefab[i]->GetPackageName(), sizeof(CChar), MAX_NAME_SIZE, PackageFilePtr);
	fwrite(g_prefab[i]->GetPrefabName(), sizeof(CChar), MAX_NAME_SIZE, PackageFilePtr);
}
fclose(PackageFilePtr);


if (dlgSave)
CDelete(dlgSave);
PrintInfo("\nScene Saved Successfully");
ReleaseCapture();
	}
	else if (result == IDCANCEL)
		return;
}

CBool CVandaEngine1Dlg::OnMenuClickedInsertPrefab(CPrefab* prefab, CChar* packageStr, CChar* prefabStr)
{
	CChar packagePath[MAX_NAME_SIZE];
	CChar prefabPath[MAX_NAME_SIZE];
	CChar prefabAndPackageName[MAX_NAME_SIZE];
	CChar packageName[MAX_NAME_SIZE];
	CChar prefabName[MAX_NAME_SIZE];

	if (!prefab)
	{
		if (packageStr)
		{
			m_dlgPrefabs->m_selectedPackageName = packageStr;
			m_dlgPrefabs->m_selectedPrefabName = prefabStr;
		}
		else
		{
			int nSelected = -1;
			POSITION p = m_dlgPrefabs->m_listPrefabPackages.GetFirstSelectedItemPosition();
			while (p)
			{
				nSelected = m_dlgPrefabs->m_listPrefabPackages.GetNextSelectedItem(p);
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
				m_dlgPrefabs->m_listPrefabPackages.GetItem(&lvi);
				m_dlgPrefabs->m_selectedPackageName = szBuffer;
			}
			else
			{
				MessageBox("Please select a package!", "Vanda Engine Error", MB_OK | MB_ICONINFORMATION);
				return CFalse;
			}

			nSelected = -1;
			p = m_dlgPrefabs->m_listPrefabs.GetFirstSelectedItemPosition();
			while (p)
			{
				nSelected = m_dlgPrefabs->m_listPrefabs.GetNextSelectedItem(p);
			}
			if (nSelected >= 0)
			{
				TCHAR szBuffer[1024];
				DWORD cchBuf(1024);
				LVITEM lvi;
				lvi.iItem = nSelected;
				lvi.iSubItem = 0;
				lvi.mask = LVIF_TEXT | LVIF_IMAGE;
				lvi.pszText = szBuffer;
				lvi.cchTextMax = cchBuf;
				m_dlgPrefabs->m_listPrefabs.GetItem(&lvi);

				m_dlgPrefabs->m_selectedPrefabName = szBuffer;

			}
			else
			{
				MessageBox("Please select an item!", "Vanda Engine Error", MB_OK | MB_ICONINFORMATION);
				return CFalse;
			}
			ex_pDlgPrefabs->OnBnClickedOk();
		}
	}


	HRESULT doc_result = SHGetFolderPath(NULL, CSIDL_PERSONAL, NULL, SHGFP_TYPE_CURRENT, packagePath);
	if (doc_result != S_OK)
	{
		PrintInfo("\nCouldn't get the documents folder", COLOR_RED);
		return CFalse;
	}
	else
	{
		if (!prefab)
		{
			Cpy(packageName, (CChar*)m_dlgPrefabs->m_selectedPackageName.c_str());
			Cpy(prefabName, (CChar*)m_dlgPrefabs->m_selectedPrefabName.c_str());
			Cpy(prefabAndPackageName, (CChar*)m_dlgPrefabs->m_selectedPackageName.c_str());
			Append(prefabAndPackageName, "_");
			Append(prefabAndPackageName, (CChar*)m_dlgPrefabs->m_selectedPrefabName.c_str());

			Append(packagePath, "/Vanda/Packages/");
			Append(packagePath, (CChar*)m_dlgPrefabs->m_selectedPackageName.c_str());
			Append(packagePath, "/");

			Cpy(prefabPath, packagePath);
			Append(prefabPath, (CChar*)m_dlgPrefabs->m_selectedPrefabName.c_str());
			Append(prefabPath, "/");
			Append(prefabPath, prefabAndPackageName);
			Append(prefabPath, ".vpf");
		}
		else
		{
			Cpy(prefabAndPackageName, prefab->GetName());

			Append(packagePath, "/Vanda/Packages/");
			Append(packagePath, prefab->GetPackageName());
			Append(packagePath, "/");

			Cpy(prefabPath, packagePath);
			Append(prefabPath, prefab->GetPrefabName());
			Append(prefabPath, "/");
			Append(prefabPath, prefabAndPackageName);
			Append(prefabPath, ".vpf");
		}
	}

	CPrefab* m_newPrefab = NULL;
	CInstancePrefab* m_newInstancePrefab = NULL;
	CBool foundPrefab = CFalse;
	if (!prefab)
	{
		//Creata a new prefab
		m_newInstancePrefab = CNew(CInstancePrefab);
		m_newInstancePrefab->SetNameIndex(); //for selection only
		m_newInstancePrefab->GenQueryIndex();
		g_selectedName = m_newInstancePrefab->GetNameIndex(); //select this element
		g_instancePrefab.push_back(m_newInstancePrefab);
		g_currentInstancePrefab = m_newInstancePrefab;

		m_newPrefab = GetPrefab(prefabAndPackageName);
		if (!m_newPrefab)
		{
			m_newPrefab = CNew(CPrefab);
			g_prefab.push_back(m_newPrefab);
		}
		else
		{
			foundPrefab = CTrue;
		}
	}
	else
	{
		m_newPrefab = prefab;
	}


	FILE *filePtr = fopen(prefabPath, "rb");
	if (filePtr)
	{
		CChar reportTemp[MAX_NAME_SIZE];
		sprintf(reportTemp, "\nVPF file '%s'...", prefabAndPackageName);
		PrintInfo(reportTemp, COLOR_RED_GREEN);

		SetCapture();
		SetCursor(LoadCursorFromFile("Assets/Engine/Icons/progress.ani"));

		if (!prefab)
		{
			Cpy(g_currentPrefabName, (CChar*)m_dlgPrefabs->m_selectedPrefabName.c_str()); //For save functions
			Cpy(g_currentPackageName, (CChar*)m_dlgPrefabs->m_selectedPackageName.c_str()); //For save functions
			Cpy(g_currentPackageAndPrefabName, prefabAndPackageName); //For save functions
			Cpy(g_currentPackagePath, packagePath);

			Cpy(ex_pVandaEngine1Dlg->m_strNewPrefabName, (CChar*)m_dlgPrefabs->m_selectedPrefabName.c_str());
			Cpy(ex_pVandaEngine1Dlg->m_strNewPackageName, (CChar*)m_dlgPrefabs->m_selectedPackageName.c_str());
			Cpy(ex_pVandaEngine1Dlg->m_strNewPackageAndPrefabName, prefabAndPackageName);
		}
		else
		{
			Cpy(g_currentPrefabName, prefab->GetPrefabName()); //For save functions
			Cpy(g_currentPackageName, prefab->GetPackageName()); //For save functions
			Cpy(g_currentPackageAndPrefabName, prefabAndPackageName); //For save functions
			Cpy(g_currentPackagePath, packagePath);

			Cpy(ex_pVandaEngine1Dlg->m_strNewPrefabName, prefab->GetPrefabName());
			Cpy(ex_pVandaEngine1Dlg->m_strNewPackageName, prefab->GetPackageName());
			Cpy(ex_pVandaEngine1Dlg->m_strNewPackageAndPrefabName, prefabAndPackageName);

		}
		if (!prefab )
		{
			m_newPrefab->SetName(prefabAndPackageName);
			m_newPrefab->SetPackageName(packageName);
			m_newPrefab->SetPrefabName(prefabName);
			m_newPrefab->SetInstance(m_newInstancePrefab);
			m_newPrefab->SetCurrentInstance(m_newInstancePrefab);
			m_newInstancePrefab->SetPrefab(m_newPrefab);
			CChar instanceName[MAX_NAME_SIZE];
			sprintf(instanceName, "%i%s%s", m_newPrefab->GetInstanceIndex(), "_", m_newPrefab->GetName());

			m_newInstancePrefab->SetName(instanceName);
			Cpy(g_currentInstancePrefabName, instanceName);
			m_newInstancePrefab->SetVisible(CTrue);
			SetDialogData3(CTrue, m_newInstancePrefab);

		}

		CChar engineName[MAX_NAME_SIZE];
		fread(&engineName, sizeof(CChar), MAX_NAME_SIZE, filePtr);
		if (!CmpIn(engineName, "VandaEngine"))
		{
			fclose(filePtr);
			MessageBox("Invalid Vin file!", "Vanda Engine Error", MB_OK | MB_ICONERROR);
			return CFalse;
		}

		fread(&g_edition, sizeof(CChar), MAX_NAME_SIZE, filePtr);
		fread(&g_maxVersion, 1, sizeof(CInt), filePtr);
		fread(&g_minVersion, 1, sizeof(CInt), filePtr);
		fread(&g_bugFixesVersion, 1, sizeof(CInt), filePtr);

		fread(&g_currentPassword, sizeof(CChar), MAX_NAME_SIZE, filePtr);

		//read engine options
		CShadowProperties shadowProperties;
		fread(&shadowProperties, sizeof(CShadowProperties), 1, filePtr);

		CPhysXProperties physXProperties;
		CDOFProperties dofProperties;
		CFogProperties fogProperties;
		CBloomProperties bloomProperties;
		CLightProperties lightProperties;
		CPathProperties pathProperties;
		fread(&physXProperties, sizeof(CPhysXProperties), 1, filePtr);
		fread(&dofProperties, sizeof(CDOFProperties), 1, filePtr);
		fread(&fogProperties, sizeof(CFogProperties), 1, filePtr);
		fread(&bloomProperties, sizeof(CBloomProperties), 1, filePtr);
		fread(&lightProperties, sizeof(CLightProperties), 1, filePtr);
		fread(&pathProperties, sizeof(CPathProperties), 1, filePtr);
		fread(&g_vandaDemo, sizeof(CBool), 1, filePtr);

		CChar banner[MAX_NAME_SIZE];
		fread(&banner, sizeof(CChar), MAX_NAME_SIZE, filePtr);
		
		CExtraTexturesNamingConventions namingConventions;
		CBool bGlobalAmbientColor;
		CColor4f globalAmbientColor;
		CInt sceneManagersObjectsPerSplit;
		fread(&namingConventions, sizeof(CExtraTexturesNamingConventions), 1, filePtr);
		fread(&bGlobalAmbientColor, sizeof(CBool), 1, filePtr);
		fread(&globalAmbientColor, sizeof(CColor4f), 1, filePtr);
		fread(&sceneManagersObjectsPerSplit, sizeof(CInt), 1, filePtr);

		//read physX 
		CBool insertPhysXScene = CFalse;
		CChar strPhysXSceneName[MAX_NAME_SIZE];
		NxExtendedVec3 characterPos;
		CVec3f characterPitchYawTilt;
		CVec3f cameraInstancePos;
		CVec2f cameraInstancePanTilt;
		fread(&insertPhysXScene, sizeof(CBool), 1, filePtr);
		fread(strPhysXSceneName, sizeof(CChar), MAX_NAME_SIZE, filePtr);
		fread(&characterPos, sizeof(NxExtendedVec3), 1, filePtr);
		fread(&characterPitchYawTilt, sizeof(CVec3f), 1, filePtr);

		fread(&cameraInstancePos, sizeof(CVec3f), 1, filePtr);
		fread(&cameraInstancePanTilt, sizeof(CVec2f), 1, filePtr);

		CChar tempSceneName[MAX_NAME_SIZE];

		CInt tempSceneSize, tempGeoSize;
		fread(&tempSceneSize, sizeof(CInt), 1, filePtr);

		for (CInt i = 0; i < tempSceneSize; i++)
		{
			CInt clipIndex;
			CBool playAnimation, loopAnimation, isVisible;

			fread(tempSceneName, sizeof(CChar), MAX_NAME_SIZE, filePtr);
			fread(&clipIndex, 1, sizeof(CInt), filePtr);
			fread(&playAnimation, 1, sizeof(CBool), filePtr);
			fread(&loopAnimation, 1, sizeof(CBool), filePtr);
			fread(&isVisible, 1, sizeof(CBool), filePtr);

			fread(&tempGeoSize, sizeof(CInt), 1, filePtr);


			CBool sceneLoaded = CFalse;
			CChar * nameOnly = GetAfterPath(tempSceneName);

			//save functions. it should be copies in WIN32 Project as well
			CChar name[MAX_NAME_SIZE];

			sprintf(name, "%s%s%s%s", packagePath, g_currentPrefabName, "/External Scenes/", nameOnly);
			g_useOriginalPathOfDAETextures = CFalse;

			CChar prefabAndSceneName[MAX_NAME_SIZE];
			sprintf(prefabAndSceneName, "%s%s%s", g_currentPackageAndPrefabName, "_", nameOnly);
			Cpy(g_currentPrefabAndSceneName, prefabAndSceneName);

			CScene * tempScene = new CScene();
			g_scene.push_back(tempScene);

			if (tempScene->Load(name, CTrue, CTrue))
			{
				tempScene->SetClipIndexForStartup(clipIndex);
				tempScene->m_playAnimation = playAnimation;
				tempScene->m_isVisible = isVisible;
				if (tempScene->m_playAnimation)
				{
					tempScene->m_animationStatus = eANIM_PLAY;
					tempScene->SetClipIndex(clipIndex, loopAnimation);
				}
				else
				{
					tempScene->m_animationStatus = eANIM_PAUSE;
				}
				tempScene->m_loopAnimationAtStartup = loopAnimation;
				//save functions/////////////////////////////////
				g_currentScene = tempScene; //mark the current scene. Save functions

				for (CUInt index = 0; index < g_allPrefabNames.size(); index++)
				{
					if (Cmp(g_currentPackageAndPrefabName, g_allPrefabNames[index].c_str())) //current scene name found
						tempScene->AddPrefabToList(g_allPrefabNames[index], CFalse); //Do not write to zip file
					else
						tempScene->AddPrefabToList(g_allPrefabNames[index], CTrue); //Write to zip file
				}
				//save functions/////////////////////////////////
				g_render.SetScene(tempScene);
				tempScene->Update();
				if (CmpIn(tempScene->GetName(), "_LOD1"))
				{
					m_newPrefab->GetCurrentInstance()->SetScene(0, tempScene);
					m_newPrefab->SetHasLod(0);
					tempScene->SetDocURI(m_newPrefab->GetCurrentInstance()->GetName());
				}
				else if (CmpIn(tempScene->GetName(), "_LOD2"))
				{
					m_newPrefab->GetCurrentInstance()->SetScene(1, tempScene);
					m_newPrefab->SetHasLod(1);
					tempScene->SetDocURI(m_newPrefab->GetCurrentInstance()->GetName());
				}
				else if (CmpIn(tempScene->GetName(), "_LOD3"))
				{
					m_newPrefab->GetCurrentInstance()->SetScene(2, tempScene);
					m_newPrefab->SetHasLod(2);
					tempScene->SetDocURI(m_newPrefab->GetCurrentInstance()->GetName());
				}
				PrintInfo("\nScene ' ");
				PrintInfo(tempScene->m_fileName, COLOR_RED_GREEN);
				PrintInfo(" ' loaded successfully");

				sceneLoaded = CTrue;

			}
			else
			{
				CChar tempErrorName[MAX_NAME_SIZE];
				sprintf(tempErrorName, "\nCouldn't load the scene '%s'", name);
				PrintInfo(tempErrorName, COLOR_RED);
				delete tempScene;
				tempScene = NULL;
				g_scene.erase(g_scene.end());
				fclose(filePtr);
				ReleaseCapture();
				sceneLoaded = CFalse;
			}
			if (!sceneLoaded)
				return CFalse;
			if (CmpIn(tempScene->GetName(), "trigger")) //triggers
			{
				if (!g_multipleView->m_nx->m_hasScene)
				{
					tempScene->m_isTrigger = CTrue;
					tempScene->Update();
					tempScene->CreateTrigger(g_multipleView->m_nx);
				}
				else
				{
					PrintInfo("\nCouldn't create the triggers. In order to create triggers from COLLADA files, you should remove current external PhysX scene.", COLOR_RED);
				}

			}
			else if (CmpIn(tempScene->GetName(), "grass"))
			{
				tempScene->m_isGrass = CTrue;
				for (CUInt j = 0; j < (CUInt)tempGeoSize; j++)
				{
					if (tempScene->m_geometries.size() >= j + 1)
						tempScene->m_geometries[j]->SetDiffuse("grass_color");
				}
			}

			for (CUInt j = 0; j < (CUInt)tempGeoSize; j++)
			{
				CChar m_strNormalMap[MAX_NAME_SIZE];
				CChar m_strDirtMap[MAX_NAME_SIZE];
				CChar m_strGlossMap[MAX_NAME_SIZE];
				CChar m_strHeightMap[MAX_NAME_SIZE];
				CChar m_strDuDvMap[MAX_NAME_SIZE];
				CChar m_strDiffuse[MAX_NAME_SIZE];
				CBool m_hasNormalMap, m_hasGlossMap, m_hasHeightMap, m_hasDuDvMap, m_hasDirtMap, m_hasDiffuse;
				CBool m_cullFaces;
				CUInt m_groupSize;
				CFloat m_bias, m_scale;

				fread(&m_hasDirtMap, sizeof(CBool), 1, filePtr);
				fread(m_strDirtMap, sizeof(CChar), MAX_NAME_SIZE, filePtr);

				fread(&m_hasNormalMap, sizeof(CBool), 1, filePtr);
				fread(m_strNormalMap, sizeof(CChar), MAX_NAME_SIZE, filePtr);
				fread(&m_bias, sizeof(CFloat), 1, filePtr);
				fread(&m_scale, sizeof(CFloat), 1, filePtr);

				fread(&m_hasGlossMap, sizeof(CBool), 1, filePtr);
				fread(m_strGlossMap, sizeof(CChar), MAX_NAME_SIZE, filePtr);

				fread(&m_hasHeightMap, sizeof(CBool), 1, filePtr);
				fread(m_strHeightMap, sizeof(CChar), MAX_NAME_SIZE, filePtr);

				fread(&m_hasDuDvMap, sizeof(CBool), 1, filePtr);
				fread(m_strDuDvMap, sizeof(CChar), MAX_NAME_SIZE, filePtr);

				fread(&m_hasDiffuse, sizeof(CBool), 1, filePtr);
				fread(m_strDiffuse, sizeof(CChar), MAX_NAME_SIZE, filePtr);

				//load cull face( enabled or disabled data) for the current geo
				fread(&m_cullFaces, sizeof(CBool), 1, filePtr);

				//read PhysX data
				CUInt physx_point_size;
				fread(&physx_point_size, sizeof(CUInt), 1, filePtr);
				for (CUInt m = 0; m < physx_point_size; m++)
				{
					CVec3f* point = CNew(CVec3f);
					fread(point, sizeof(CVec3f), 1, filePtr);
					if (tempScene->m_geometries.size() >= j + 1)
						tempScene->m_geometries[j]->m_physx_points.push_back(point);
				}
				CUInt physx_triangles_size;
				fread(&physx_triangles_size, sizeof(CUInt), 1, filePtr);

				for (CUInt m = 0; m < physx_triangles_size; m++)
				{
					CTriangles* tri = CNew(CTriangles);
					CUInt count;
					fread(&count, sizeof(CUInt), 1, filePtr);
					tri->m_count = count;
					tri->m_indexes = CNewData(CUInt, tri->m_count * 3);

					for (CUInt n = 0; n < tri->m_count * 3; n++)
					{
						CUInt index;
						fread(&index, sizeof(CUInt), 1, filePtr);
						tri->m_indexes[n] = index;
					}
					if (tempScene->m_geometries.size() >= j + 1)
						tempScene->m_geometries[j]->m_physx_triangles.push_back(tri);
				}

				CInt cm_size = 0;
				fread(&cm_size, sizeof(CInt), 1, filePtr);
				if (tempScene->m_geometries.size() >= j + 1)
					tempScene->m_geometries[j]->m_collapseMap.SetSize(cm_size);
				for (CInt m = 0; m < cm_size; m++)
				{
					CInt cm_value;
					fread(&cm_value, sizeof(CInt), 1, filePtr);
					if (tempScene->m_geometries.size() >= j + 1)
						tempScene->m_geometries[j]->m_collapseMap[m] = cm_value;
				}

				///////////////////

				fread(&m_groupSize, sizeof(CUInt), 1, filePtr);
				//store group info

				if (sceneLoaded && !tempScene->m_isGrass )
				{
					if (m_hasDirtMap && tempScene->m_geometries.size() >= j + 1)
					{
						tempScene->m_geometries[j]->m_hasDirtMap = CTrue;
						Cpy(tempScene->m_geometries[j]->m_strDirtMap, m_strDirtMap);
					}
					if (m_hasNormalMap && tempScene->m_geometries.size() >= j + 1)
					{
						tempScene->m_geometries[j]->m_hasNormalMap = CTrue;
						Cpy(tempScene->m_geometries[j]->m_strNormalMap, m_strNormalMap);
					}
					if (tempScene->m_geometries.size() >= j + 1)
					{
						tempScene->m_geometries[j]->m_parallaxMapBias = m_bias;
						tempScene->m_geometries[j]->m_parallaxMapScale = m_scale;
					}
					if (m_hasGlossMap && tempScene->m_geometries.size() >= j + 1)
					{
						tempScene->m_geometries[j]->m_hasGlossMap = CTrue;
						Cpy(tempScene->m_geometries[j]->m_strGlossMap, m_strGlossMap);
					}
					if (m_hasDiffuse && tempScene->m_geometries.size() >= j + 1)
					{
						tempScene->m_geometries[j]->m_hasDiffuse = CTrue;
						Cpy(tempScene->m_geometries[j]->m_strDiffuse, m_strDiffuse);
					}
					//if( m_hasHeightMap )
					//	tempScene->m_geometries[j]->SetHeightMap( m_strHeightMap );
					//if( m_hasDuDvMap )
					//	tempScene->m_geometries[j]->SetDuDvMap(); //under construction!
					if (m_cullFaces && tempScene->m_geometries.size() >= j + 1)
						tempScene->m_geometries[j]->SetCullFace(CTrue);
					else if (tempScene->m_geometries.size() >= j + 1)
						tempScene->m_geometries[j]->SetCullFace(CFalse);

					for (CUInt k = 0; k < m_groupSize; k++)
					{

						CChar m_strGroupGlossMap[MAX_NAME_SIZE];
						CBool m_hasGroupGlossMap;
						fread(&m_hasGroupGlossMap, sizeof(CBool), 1, filePtr);
						fread(m_strGroupGlossMap, sizeof(CChar), MAX_NAME_SIZE, filePtr);
						if (m_hasGroupGlossMap && tempScene->m_geometries.size() >= j + 1)
							tempScene->m_geometries[j]->m_groups[k]->SetGlossMap(m_strGroupGlossMap);

						CChar m_strGroupDirtMap[MAX_NAME_SIZE];
						CBool m_hasGroupDirtMap;
						fread(&m_hasGroupDirtMap, sizeof(CBool), 1, filePtr);
						fread(m_strGroupDirtMap, sizeof(CChar), MAX_NAME_SIZE, filePtr);
						if (m_hasGroupDirtMap && tempScene->m_geometries.size() >= j + 1)
							tempScene->m_geometries[j]->m_groups[k]->SetDirtMap(m_strGroupDirtMap);

						CChar m_strGroupNormalMap[MAX_NAME_SIZE];
						CFloat m_bias, m_scale;
						CBool m_hasGroupNormalMap;
						fread(&m_hasGroupNormalMap, sizeof(CBool), 1, filePtr);
						fread(m_strGroupNormalMap, sizeof(CChar), MAX_NAME_SIZE, filePtr);
						fread(&m_bias, sizeof(CFloat), 1, filePtr);
						fread(&m_scale, sizeof(CFloat), 1, filePtr);
						if (m_hasGroupNormalMap && tempScene->m_geometries.size() >= j + 1)
							tempScene->m_geometries[j]->m_groups[k]->SetNormalMap(m_strGroupNormalMap, m_bias, m_scale);

						CChar m_strGroupDiffuseMap[MAX_NAME_SIZE];
						CBool m_hasGroupDiffuse;
						fread(&m_hasGroupDiffuse, sizeof(CBool), 1, filePtr);
						fread(m_strGroupDiffuseMap, sizeof(CChar), MAX_NAME_SIZE, filePtr);
						if (m_hasGroupDiffuse && tempScene->m_geometries.size() >= j + 1)
							tempScene->m_geometries[j]->m_groups[k]->SetDiffuse(m_strGroupDiffuseMap);
					}
				}
			} //for all of the geos

			CUInt instanceGeoSize;
			fread(&instanceGeoSize, 1, sizeof(CUInt), filePtr);
			for (CUInt j = 0; j < instanceGeoSize; j++)
			{
				CChar geoName[MAX_NAME_SIZE];
				CMatrix instanceLocalToWorldMatrix;
				CBool hasPhysX;
				CChar PhysXName[MAX_NAME_SIZE];
				CPhysXAlgorithm physXAlgorithm;
				CFloat physXDensity;
				CInt physXPercentage;
				CBool isTrigger;
				CBool isInvisible;
				CBool hasScriptEnter;
				CBool hasScriptExit;
				CChar scriptEnter[MAX_NAME_SIZE];
				CChar scriptExit[MAX_NAME_SIZE];
				fread(geoName, 1, sizeof(CChar) * MAX_NAME_SIZE, filePtr);
				fread(instanceLocalToWorldMatrix, 1, sizeof(CMatrix), filePtr);
				fread(&hasPhysX, 1, sizeof(CBool), filePtr);
				fread(PhysXName, 1, sizeof(CChar) * MAX_NAME_SIZE, filePtr);
				fread(&physXAlgorithm, 1, sizeof(CPhysXAlgorithm), filePtr);
				fread(&physXDensity, 1, sizeof(CFloat), filePtr);
				fread(&physXPercentage, 1, sizeof(CInt), filePtr);
				fread(&isTrigger, 1, sizeof(CBool), filePtr);
				fread(&isInvisible, 1, sizeof(CBool), filePtr);
				fread(scriptEnter, 1, sizeof(CChar) * MAX_NAME_SIZE, filePtr);
				fread(&hasScriptEnter, 1, sizeof(CBool), filePtr);
				fread(scriptExit, 1, sizeof(CChar) * MAX_NAME_SIZE, filePtr);
				fread(&hasScriptExit, 1, sizeof(CBool), filePtr);

				if (hasPhysX)
				{
					for (CUInt k = 0; k < tempScene->m_instanceGeometries.size(); k++)
					{
						if (Cmp(geoName, tempScene->m_instanceGeometries[k]->m_abstractGeometry->GetName()))
						{
							CBool equal = CTrue;
							for (CUInt l = 0; l < 16; l++)
							{
								if (instanceLocalToWorldMatrix[l] != tempScene->m_instanceGeometries[k]->m_firstLocalToWorldMatrix[l])
								{
									equal = CFalse;
									break;
								}
							}
							if (equal)
							{
								//if (tempScene->GeneratePhysX(physXAlgorithm, physXDensity, physXPercentage, isTrigger, isInvisible, tempScene->m_instanceGeometries[k], CTrue))
								//{
									tempScene->m_instanceGeometries[k]->m_lodAlgorithm = physXAlgorithm;
									tempScene->m_instanceGeometries[k]->m_hasPhysX = CTrue;
									tempScene->m_instanceGeometries[k]->m_physXDensity = physXDensity;
									tempScene->m_instanceGeometries[k]->m_physXPercentage = physXPercentage;
									tempScene->m_instanceGeometries[k]->m_isTrigger = isTrigger;
									tempScene->m_instanceGeometries[k]->m_isInvisible = isInvisible;

									CChar scriptEnterPath[MAX_NAME_SIZE];
									CChar scriptExitPath[MAX_NAME_SIZE];

									CChar* tempEnterPath = GetAfterPath(scriptEnter);
									CChar* tempExitPath = GetAfterPath(scriptExit);
									//Copy this to Win32 Project as well
									if (hasScriptEnter)
										sprintf(scriptEnterPath, "%s%s%s%s", packagePath, g_currentPrefabName, "/Scripts/", tempEnterPath);
									else
										Cpy(scriptEnterPath, "\n");

									if (hasScriptExit)
										sprintf(scriptExitPath, "%s%s%s%s", packagePath, g_currentPrefabName, "/Scripts/", tempExitPath);
									else
										Cpy(scriptExitPath, "\n");

									Cpy(tempScene->m_instanceGeometries[k]->m_enterScript, scriptEnterPath);
									Cpy(tempScene->m_instanceGeometries[k]->m_exitScript, scriptExitPath);
									tempScene->m_instanceGeometries[k]->m_hasEnterScript = hasScriptEnter;
									tempScene->m_instanceGeometries[k]->m_hasExitScript = hasScriptExit;

									//if (physXDensity > 0 || tempScene->m_instanceGeometries[k]->m_abstractGeometry->m_hasAnimation)
									//	InsertItemToPhysXList(tempScene->m_instanceGeometries[k]->m_physXName, ePHYSXELEMENTLIST_DYNAMIC_RIGIDBODY);
									//else if (isTrigger)
									//	InsertItemToPhysXList(tempScene->m_instanceGeometries[k]->m_physXName, ePHYSXELEMENTLIST_TRIGGER);
									//else
									//	InsertItemToPhysXList(tempScene->m_instanceGeometries[k]->m_physXName, ePHYSXELEMENTLIST_STATIC_RIGIDBODY);
								//}
							}
						}
					} //for
				} //if has PhysX
			}
		} // for all of the scenes
		if (g_currentCameraType == eCAMERA_DEFAULT_FREE)
		{
			g_multipleView->m_nx->gControllers->reportSceneChanged();
			gPhysXscene->simulate(1.0f / 60.0f/*elapsedTime*/);
			gPhysXscene->flushStream();
			gPhysXscene->fetchResults(NX_ALL_FINISHED, true);
		}

		fclose(filePtr);
		ReleaseCapture();

		if (!prefab)
		{
			m_newInstancePrefab->UpdateBoundingBox(CTrue);
			m_newInstancePrefab->CalculateDistance();
			CChar tempInstanceName[MAX_NAME_SIZE];
			sprintf(tempInstanceName, "%s%s%s", "\nPrefab Instance ' ", m_newInstancePrefab->GetName(), " ' created successfully");
			PrintInfo(tempInstanceName, COLOR_GREEN);

			//insert items
			//insert new instance and select it
			if (!packageStr)
			{
				InsertItemToSceneList(m_newInstancePrefab->GetName());
				m_listBoxScenes.SetItemState(m_listBoxScenes.GetItemCount() - 1, LVIS_SELECTED, LVIS_SELECTED);
				m_listBoxScenes.SetSelectionMark(m_listBoxScenes.GetItemCount() - 1);
				//delete elements of previous selected prefab from the list
				for (int nItem = m_listBoxObjects.GetItemCount() - 1; nItem >= 0; nItem--)
				{
					m_listBoxObjects.DeleteItem(nItem);
				}
				//show the elements of newly selected prefab
				for (CUInt j = 0; j < 3; j++)
				{
					if (m_newInstancePrefab->GetPrefab()->GetHasLod(j))
						InsertItemToObjectList(m_newInstancePrefab->GetScene(j)->GetName(), eOBJECTLIST_SCENE);
				}
			}
		}
		//if (!prefab)
		//	g_octree->ResetState(); //recalculate octree
		CChar temp[MAX_NAME_SIZE];
		sprintf(temp, "\ntotal errors: %i, total warnings: %i", totalErrors, totalWarnings);
		PrintInfo(temp, COLOR_RED_GREEN);
		totalErrors = totalWarnings = 0;
	}
	return CTrue;
}

CBool CVandaEngine1Dlg::OnMenuClickedOpenPrefab()
{
	int nSelected = -1;
	POSITION p = m_dlgPrefabs->m_listPrefabPackages.GetFirstSelectedItemPosition();
	while (p)
	{
		nSelected = m_dlgPrefabs->m_listPrefabPackages.GetNextSelectedItem(p);
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
		m_dlgPrefabs->m_listPrefabPackages.GetItem(&lvi);
		m_dlgPrefabs->m_selectedPackageName = szBuffer;
	}
	else
	{
		MessageBox("Please select a package!", "Vanda Engine Error", MB_OK | MB_ICONINFORMATION);
		return CFalse;
	}

	nSelected = -1;
	p = m_dlgPrefabs->m_listPrefabs.GetFirstSelectedItemPosition();
	while (p)
	{
		nSelected = m_dlgPrefabs->m_listPrefabs.GetNextSelectedItem(p);
	}
	if (nSelected >= 0)
	{
		TCHAR szBuffer[1024];
		DWORD cchBuf(1024);
		LVITEM lvi;
		lvi.iItem = nSelected;
		lvi.iSubItem = 0;
		lvi.mask = LVIF_TEXT | LVIF_IMAGE;
		lvi.pszText = szBuffer;
		lvi.cchTextMax = cchBuf;
		m_dlgPrefabs->m_listPrefabs.GetItem(&lvi);

		m_dlgPrefabs->m_selectedPrefabName = szBuffer;

	}
	else
	{
		MessageBox("Please select an item!", "Vanda Engine Error", MB_OK | MB_ICONINFORMATION);
		return CFalse;
	}
	ex_pDlgPrefabs->OnBnClickedOk();
	CInt iResponse = IDNO;
	if (g_scene.size() > 0)
		iResponse = MessageBox("Do you want to save your changes?", "Warning", MB_YESNOCANCEL | MB_ICONSTOP);
	else
		iResponse = IDNO;

	CBool openScene = CFalse;
	if (iResponse == IDYES)
	{
		if (g_multipleView->m_enableTimer)
			g_multipleView->EnableTimer(CFalse);
		OnMenuClickedSaveAs();
		if (g_multipleView->m_enableTimer)
			g_multipleView->EnableTimer(CTrue);
	}
	CChar packagePath[MAX_NAME_SIZE];
	CChar prefabPath[MAX_NAME_SIZE];
	CChar prefabAndPackageName[MAX_NAME_SIZE];

	HRESULT doc_result = SHGetFolderPath(NULL, CSIDL_PERSONAL, NULL, SHGFP_TYPE_CURRENT, packagePath);
	if (doc_result != S_OK)
	{
		PrintInfo("\nCouldn't get the documents folder", COLOR_RED);
		return CFalse;
	}
	else
	{
		Cpy(prefabAndPackageName, (CChar*)m_dlgPrefabs->m_selectedPackageName.c_str());
		Append(prefabAndPackageName, "_");
		Append(prefabAndPackageName, (CChar*)m_dlgPrefabs->m_selectedPrefabName.c_str());

		Append(packagePath, "/Vanda/Packages/");
		Append(packagePath, (CChar*)m_dlgPrefabs->m_selectedPackageName.c_str());
		Append(packagePath, "/");

		Cpy(prefabPath, packagePath);
		Append(prefabPath, (CChar*)m_dlgPrefabs->m_selectedPrefabName.c_str());
		Append(prefabPath, "/");
		Append(prefabPath, prefabAndPackageName);
		Append(prefabPath, ".vpf");
	}

	//close the previous scene
	if (g_multipleView->m_enableTimer)
		g_multipleView->EnableTimer(CFalse);
	OnMenuClickedNew(CFalse);
	if (g_multipleView->m_enableTimer)
		g_multipleView->EnableTimer(CTrue);

	g_multipleView->SetElapsedTimeFromBeginning();
	g_multipleView->RenderWindow();

	Cpy(g_currentPrefabName, (CChar*)m_dlgPrefabs->m_selectedPrefabName.c_str()); //For save functions
	Cpy(g_currentPackageName, (CChar*)m_dlgPrefabs->m_selectedPackageName.c_str()); //For save functions
	Cpy(g_currentPackageAndPrefabName, prefabAndPackageName); //For save functions
	Cpy(g_currentPackagePath, packagePath);

	Cpy(ex_pVandaEngine1Dlg->m_strNewPrefabName, (CChar*)m_dlgPrefabs->m_selectedPrefabName.c_str());
	Cpy(ex_pVandaEngine1Dlg->m_strNewPackageName, (CChar*)m_dlgPrefabs->m_selectedPackageName.c_str());
	Cpy(ex_pVandaEngine1Dlg->m_strNewPackageAndPrefabName, prefabAndPackageName);

	FILE *filePtr = fopen(prefabPath, "rb");
	if (filePtr)
	{
		CChar reportTemp[MAX_NAME_SIZE];
		sprintf(reportTemp, "\nVPF file '%s'...", prefabAndPackageName);
		PrintInfo(reportTemp, COLOR_RED_GREEN);

		SetCapture();
		SetCursor(LoadCursorFromFile("Assets/Engine/Icons/progress.ani"));

		CChar engineName[MAX_NAME_SIZE];
		fread(&engineName, sizeof(CChar), MAX_NAME_SIZE, filePtr);
		if (!CmpIn(engineName, "VandaEngine"))
		{
			fclose(filePtr);
			MessageBox("Invalid VPF file!", "Vanda Engine Error", MB_OK | MB_ICONERROR);
			return CFalse;
		}
		if (g_currentCameraType == eCAMERA_DEFAULT_PHYSX)
			PrintInfo("\nPlay mode disabled");

		g_currentCameraType = eCAMERA_DEFAULT_FREE;
		m_mainBtnTestActive.ShowWindow(SW_HIDE);
		m_mainBtnTestActive.EnableWindow(FALSE);
		m_mainBtnTestActive.UpdateWindow();
		m_mainBtnTestDeactive.ShowWindow(SW_SHOW);
		m_mainBtnTestDeactive.EnableWindow(TRUE);
		m_mainBtnTestDeactive.UpdateWindow();

		g_render.SetActiveInstanceCamera(g_render.GetDefaultInstanceCamera());

		fread(&g_edition, sizeof(CChar), MAX_NAME_SIZE, filePtr);
		fread(&g_maxVersion, 1, sizeof(CInt), filePtr);
		fread(&g_minVersion, 1, sizeof(CInt), filePtr);
		fread(&g_bugFixesVersion, 1, sizeof(CInt), filePtr);
		fread(&g_currentPassword, sizeof(CChar), MAX_NAME_SIZE, filePtr);

		//read engine options
		fread(&g_shadowProperties, sizeof(CShadowProperties), 1, filePtr);

		switch (g_shadowProperties.m_shadowResolution)
		{
		case eSHADOW_1024:
			g_dynamicShadowMap->depth_size = 1024;
			break;
		case eSHADOW_2048:
			g_dynamicShadowMap->depth_size = 2048;
			break;
		case eSHADOW_4096:
			g_dynamicShadowMap->depth_size = 4096;
			break;
		default:
			break;
		}
		g_dynamicShadowMap->RegenerateDepthTex(g_dynamicShadowMap->depth_size);

		switch (g_shadowProperties.m_shadowSplits)
		{
		case eSHADOW_1_SPLIT:
			g_dynamicShadowMap->cur_num_splits = 1;
			break;
		case eSHADOW_2_SPLITS:
			g_dynamicShadowMap->cur_num_splits = 2;
			break;
		case eSHADOW_3_SPLITS:
			g_dynamicShadowMap->cur_num_splits = 3;
			break;
		case eSHADOW_4_SPLITS:
			g_dynamicShadowMap->cur_num_splits = 4;
			break;
		default:
			break;
		}

		g_dynamicShadowMap->split_weight = g_shadowProperties.m_shadowSplitWeight;
		fread(&g_physXProperties, sizeof(CPhysXProperties), 1, filePtr);
		ResetPhysX(); //reset the physX based on the g_physXProperties information
		fread(&g_dofProperties, sizeof(CDOFProperties), 1, filePtr);
		fread(&g_fogProperties, sizeof(CFogProperties), 1, filePtr);
		fread(&g_bloomProperties, sizeof(CBloomProperties), 1, filePtr);
		fread(&g_lightProperties, sizeof(CLightProperties), 1, filePtr);
		fread(&g_pathProperties, sizeof(CPathProperties), 1, filePtr);
		fread(&g_vandaDemo, sizeof(CBool), 1, filePtr);

		CChar banner[MAX_NAME_SIZE];
		fread(&banner, sizeof(CChar), MAX_NAME_SIZE, filePtr);

		fread(&g_extraTexturesNamingConventions, sizeof(CExtraTexturesNamingConventions), 1, filePtr);
		fread(&g_useGlobalAmbientColor, sizeof(CBool), 1, filePtr);
		fread(&g_globalAmbientColor, sizeof(CColor4f), 1, filePtr);
		fread(&g_sceneManagerObjectsPerSplit, sizeof(CInt), 1, filePtr);

		CFog fog;
		fog.SetColor(g_fogProperties.m_fogColor);
		fog.SetDensity(g_fogProperties.m_fogDensity);

		//read physX 
		CBool insertPhysXScene = CFalse;
		CChar strPhysXSceneName[MAX_NAME_SIZE];
		NxExtendedVec3 characterPos;
		CVec3f characterPitchYawTilt;
		CVec3f cameraInstancePos;
		CVec2f cameraInstancePanTilt;
		fread(&insertPhysXScene, sizeof(CBool), 1, filePtr);
		fread(strPhysXSceneName, sizeof(CChar), MAX_NAME_SIZE, filePtr);
		fread(&characterPos, sizeof(NxExtendedVec3), 1, filePtr);
		fread(&characterPitchYawTilt, sizeof(CVec3f), 1, filePtr);
		g_characterPos = characterPos;

		g_multipleView->m_nx->gCharacterPos.x = g_characterPos.x;
		g_multipleView->m_nx->gCharacterPos.y = g_characterPos.y;
		g_multipleView->m_nx->gCharacterPos.z = g_characterPos.z;

		g_cameraPitchYawTilt.x = g_camera->m_perspectiveCameraPitch = characterPitchYawTilt.x;
		g_cameraPitchYawTilt.y = g_camera->m_perspectiveCameraYaw = characterPitchYawTilt.y;
		g_cameraPitchYawTilt.z = g_camera->m_perspectiveCameraTilt = characterPitchYawTilt.z;
		PrintInfo("\nDefault PhysX camera info imported successfully");

		fread(&cameraInstancePos, sizeof(CVec3f), 1, filePtr);
		fread(&cameraInstancePanTilt, sizeof(CVec2f), 1, filePtr);
		g_cameraInstancePos = cameraInstancePos;
		g_cameraInstancePanTilt = cameraInstancePanTilt;
		g_render.GetDefaultInstanceCamera()->MoveTransform2(cameraInstancePos.x, cameraInstancePos.y, cameraInstancePos.z);
		g_render.GetDefaultInstanceCamera()->SetPanAndTilt2(cameraInstancePanTilt.x, cameraInstancePanTilt.y);
		PrintInfo("\nDefault Free camera info imported successfully");

		if (insertPhysXScene)
		{
			//Copy this part to Win32 Project. Save functions
			CChar temp[MAX_NAME_SIZE];
			CChar* PhysXName = GetAfterPath(strPhysXSceneName);
			CChar PhysXPath[MAX_NAME_SIZE];
			sprintf(PhysXPath, "%s%s%s%s", packagePath, g_currentPrefabName, "/External Physics/", PhysXName);
			if (g_multipleView->m_nx->LoadScene(PhysXPath, NXU::FT_XML))
			{
				g_physXProperties.m_bDebugMode = CTrue;
				m_btnRemovePhysX.EnableWindow(TRUE);
				GetMenu()->CheckMenuItem(ID_PHYSICS_DEBUGMODE, MF_CHECKED);
				PrintInfo("\nPhysX debug activated");

				g_multipleView->m_nx->SetSceneName(PhysXPath);
				sprintf(temp, "\nPhysX scene '%s' imported successufully", PhysXPath);
				PrintInfo(temp);

				m_physXElementListIndex = -1;
				for (int nItem = m_listBoxPhysXElements.GetItemCount() - 1; nItem >= 0; nItem--)
				{
					m_listBoxPhysXElements.DeleteItem(nItem);
				}
				CInt count = 0;
				for (std::vector<std::string>::iterator it = g_multipleView->m_nx->m_nxActorNames.begin(); it != g_multipleView->m_nx->m_nxActorNames.end(); it++)
				{
					CChar temp[MAX_NAME_SIZE];
					Cpy(temp, (*it).c_str());
					InsertItemToPhysXList(temp, g_multipleView->m_nx->m_nxActorTypes[count]);
					count++;
				}
				//save functions/////////////////////////////////
				CDelete(g_externalPhysX);
				g_externalPhysX = CNew(CExternalPhysX);
				for (CUInt index = 0; index < g_allPrefabNames.size(); index++)
				{
					if (Cmp(g_currentPackageAndPrefabName, g_allPrefabNames[index].c_str())) //current scene name found
						g_externalPhysX->AddVSceneToList(g_allPrefabNames[index], CFalse); //Do not write to zip file
					else
						g_externalPhysX->AddVSceneToList(g_allPrefabNames[index], CTrue); //Write to zip file
				}
				g_externalPhysX->SetPhysXPath(PhysXPath);
				//save functions/////////////////////////////////

			}
			else
			{
				sprintf(temp, "\nCouldn't load the PhysX scene '%s'", PhysXPath);
				PrintInfo(temp, COLOR_RED);
				g_multipleView->m_nx->SetSceneName(PhysXPath);
			}
		}
		g_multipleView->m_nx->ResetCharacterPos(characterPos);

		CChar tempSceneName[MAX_NAME_SIZE];

		CInt tempSceneSize, tempGeoSize;
		fread(&tempSceneSize, sizeof(CInt), 1, filePtr);

		for (CInt i = 0; i < tempSceneSize; i++)
		{
			CInt clipIndex;
			CBool playAnimation, loopAnimation, isVisible;

			fread(tempSceneName, sizeof(CChar), MAX_NAME_SIZE, filePtr);
			fread(&clipIndex, 1, sizeof(CInt), filePtr);
			fread(&playAnimation, 1, sizeof(CBool), filePtr);
			fread(&loopAnimation, 1, sizeof(CBool), filePtr);
			fread(&isVisible, 1, sizeof(CBool), filePtr);

			fread(&tempGeoSize, sizeof(CInt), 1, filePtr);


			CBool sceneLoaded = CFalse;
			CScene * tempScene = new CScene();
			CChar * nameOnly = GetAfterPath(tempSceneName);

			CChar prefabAndSceneName[MAX_NAME_SIZE];
			sprintf(prefabAndSceneName, "%s%s%s", g_currentPackageAndPrefabName, "_", nameOnly);
			Cpy(g_currentPrefabAndSceneName, prefabAndSceneName);

			//save functions. it should be copies in WIN32 Project as well
			CChar name[MAX_NAME_SIZE];

			sprintf(name, "%s%s%s%s", packagePath, g_currentPrefabName, "/External Scenes/", nameOnly);
			g_useOriginalPathOfDAETextures = CFalse;
			g_scene.push_back(tempScene);

			if (tempScene->Load(name, CTrue, CTrue))
			{
				tempScene->SetClipIndexForStartup(clipIndex);
				tempScene->m_playAnimation = playAnimation;
				tempScene->m_isVisible = isVisible;
				if (tempScene->m_playAnimation)
				{
					tempScene->m_animationStatus = eANIM_PLAY;
					tempScene->SetClipIndex(clipIndex, loopAnimation);
				}
				else
				{
					tempScene->m_animationStatus = eANIM_PAUSE;
				}
				tempScene->m_loopAnimationAtStartup = loopAnimation;
				//save functions/////////////////////////////////
				g_currentScene = tempScene; //mark the current scene. Save functions

				for (CUInt index = 0; index < g_allPrefabNames.size(); index++)
				{
					if (Cmp(g_currentPackageAndPrefabName, g_allPrefabNames[index].c_str())) //current scene name found
						tempScene->AddPrefabToList(g_allPrefabNames[index], CFalse); //Do not write to zip file
					else
						tempScene->AddPrefabToList(g_allPrefabNames[index], CTrue); //Write to zip file
				}
				//save functions/////////////////////////////////
				g_render.SetScene(tempScene);
				tempScene->Update();


				InsertItemToSceneList(tempScene->m_pureFileName);
				PrintInfo("\nScene ' ");
				PrintInfo(tempScene->m_fileName, COLOR_RED_GREEN);
				PrintInfo(" ' loaded successfully");

				sceneLoaded = CTrue;

				if (i == tempSceneSize - 1) //last scene is selected, so show its objects
				{
					//remove the current contents of the object list
					for (int nItem = m_listBoxObjects.GetItemCount() - 1; nItem >= 0; nItem--)
					{
						m_listBoxObjects.DeleteItem(nItem);
					}
				}
			}
			else
			{
				CChar tempErrorName[MAX_NAME_SIZE];
				sprintf(tempErrorName, "\nCouldn't load the scene '%s'", name);
				PrintInfo(tempErrorName, COLOR_RED);
				delete tempScene;
				tempScene = NULL;
				g_scene.erase(g_scene.end());
				fclose(filePtr);
				ReleaseCapture();
				sceneLoaded = CFalse;
			}
			if (!sceneLoaded)
				return CFalse;
			if (CmpIn(tempScene->GetName(), "trigger")) //triggers
			{
				if (!g_multipleView->m_nx->m_hasScene)
				{
					tempScene->m_isTrigger = CTrue;
					tempScene->Update();
					tempScene->CreateTrigger(g_multipleView->m_nx);
				}
				else
				{
					PrintInfo("\nCouldn't create the triggers. In order to create triggers from COLLADA files, you should remove current external PhysX scene.", COLOR_RED);
				}

			}
			else if (CmpIn(tempScene->GetName(), "grass"))
			{
				tempScene->m_isGrass = CTrue;
				for (CUInt j = 0; j < (CUInt)tempGeoSize; j++)
				{
					if (tempScene->m_geometries.size() >= j + 1)
						tempScene->m_geometries[j]->SetDiffuse("grass_color");
				}
			}

			if (i == tempSceneSize - 1) //last scene is selected, so show its objects
			{
				for (CUInt j = 0; j < tempScene->m_images.size(); j++)
				{
					InsertItemToObjectList(tempScene->m_images[j]->GetName(), eOBJECTLIST_IMAGE);
				}
				for (CUInt j = 0; j < tempScene->m_effects.size(); j++)
				{
					InsertItemToObjectList(tempScene->m_effects[j]->GetName(), eOBJECTLIST_EFFECT);
				}
				for (CUInt j = 0; j < tempScene->m_materials.size(); j++)
				{
					InsertItemToObjectList(tempScene->m_materials[j]->GetName(), eOBJECTLIST_MATERIAL);
				}
				//animations
				for (CUInt j = 0; j < tempScene->m_animations.size(); j++)
				{
					InsertItemToObjectList(tempScene->m_animations[j]->GetName(), eOBJECTLIST_ANIMATION);
				}
				for (CUInt j = 0; j < tempScene->m_animationClips.size(); j++)
				{
					InsertItemToObjectList(tempScene->m_animationClips[j]->GetName(), eOBJECTLIST_ANIMATIONCLIP);
				}
				//lights
				for (CUInt j = 0; j < tempScene->m_lights.size(); j++)
				{
					InsertItemToObjectList(tempScene->m_lights[j]->GetName(), eOBJECTLIST_LIGHT);
				}
				//for( CUInt j = 0; j < tempScene->m_lightInstances.size(); j++ )
				//{
				//	InsertItemToObjectList( tempScene->m_lightInstances[j]->GetName());
				//}
				//camera
				for (CUInt j = 0; j < tempScene->m_cameras.size(); j++)
				{
					InsertItemToObjectList(tempScene->m_cameras[j]->GetName(), eOBJECTLIST_CAMERA);
				}
				//for( CUInt j = 0; j < tempScene->m_cameraInstances.size(); j++ )
				//{
				//	InsertItemToObjectList( tempScene->m_cameraInstances[j]->GetName());
				//}
				//geometry
				for (CUInt j = 0; j < tempScene->m_geometries.size(); j++)
				{
					if (tempScene->m_geometries.size() >= j + 1)
						InsertItemToObjectList(tempScene->m_geometries[j]->GetName(), eOBJECTLIST_MESH);
				}
				//for( CUInt j = 0; j < tempScene->m_instanceGeometries.size(); j++ )
				//{
				//	InsertItemToObjectList( tempScene->m_instanceGeometries[j]->GetName());
				//}
				//controller
				for (CUInt j = 0; j < tempScene->m_controllers.size(); j++)
				{
					InsertItemToObjectList(tempScene->m_controllers[j]->GetName(), eOBJECTLIST_SKINCONTROLLER);
				}
			}

			for (CUInt j = 0; j < (CUInt)tempGeoSize; j++)
			{
				CChar m_strNormalMap[MAX_NAME_SIZE];
				CChar m_strDirtMap[MAX_NAME_SIZE];
				CChar m_strGlossMap[MAX_NAME_SIZE];
				CChar m_strHeightMap[MAX_NAME_SIZE];
				CChar m_strDuDvMap[MAX_NAME_SIZE];
				CChar m_strDiffuse[MAX_NAME_SIZE];
				CBool m_hasNormalMap, m_hasGlossMap, m_hasHeightMap, m_hasDuDvMap, m_hasDirtMap, m_hasDiffuse;
				CBool m_cullFaces;
				CUInt m_groupSize;
				CFloat m_bias, m_scale;

				fread(&m_hasDirtMap, sizeof(CBool), 1, filePtr);
				fread(m_strDirtMap, sizeof(CChar), MAX_NAME_SIZE, filePtr);

				fread(&m_hasNormalMap, sizeof(CBool), 1, filePtr);
				fread(m_strNormalMap, sizeof(CChar), MAX_NAME_SIZE, filePtr);
				fread(&m_bias, sizeof(CFloat), 1, filePtr);
				fread(&m_scale, sizeof(CFloat), 1, filePtr);

				fread(&m_hasGlossMap, sizeof(CBool), 1, filePtr);
				fread(m_strGlossMap, sizeof(CChar), MAX_NAME_SIZE, filePtr);

				fread(&m_hasHeightMap, sizeof(CBool), 1, filePtr);
				fread(m_strHeightMap, sizeof(CChar), MAX_NAME_SIZE, filePtr);

				fread(&m_hasDuDvMap, sizeof(CBool), 1, filePtr);
				fread(m_strDuDvMap, sizeof(CChar), MAX_NAME_SIZE, filePtr);

				fread(&m_hasDiffuse, sizeof(CBool), 1, filePtr);
				fread(m_strDiffuse, sizeof(CChar), MAX_NAME_SIZE, filePtr);

				//load cull face( enabled or disabled data) for the current geo
				fread(&m_cullFaces, sizeof(CBool), 1, filePtr);

				//read PhysX data
				CUInt physx_point_size;
				fread(&physx_point_size, sizeof(CUInt), 1, filePtr);
				for (CUInt m = 0; m < physx_point_size; m++)
				{
					CVec3f* point = CNew(CVec3f);
					fread(point, sizeof(CVec3f), 1, filePtr);
					if (tempScene->m_geometries.size() >= j + 1)
						tempScene->m_geometries[j]->m_physx_points.push_back(point);
				}
				CUInt physx_triangles_size;
				fread(&physx_triangles_size, sizeof(CUInt), 1, filePtr);

				for (CUInt m = 0; m < physx_triangles_size; m++)
				{
					CTriangles* tri = CNew(CTriangles);
					CUInt count;
					fread(&count, sizeof(CUInt), 1, filePtr);
					tri->m_count = count;
					tri->m_indexes = CNewData(CUInt, tri->m_count * 3);

					for (CUInt n = 0; n < tri->m_count * 3; n++)
					{
						CUInt index;
						fread(&index, sizeof(CUInt), 1, filePtr);
						tri->m_indexes[n] = index;
					}

					if (tempScene->m_geometries.size() >= j + 1)
						tempScene->m_geometries[j]->m_physx_triangles.push_back(tri);
				}

				CInt cm_size = 0;
				fread(&cm_size, sizeof(CInt), 1, filePtr);
				if (tempScene->m_geometries.size() >= j + 1)
					tempScene->m_geometries[j]->m_collapseMap.SetSize(cm_size);
				for (CInt m = 0; m < cm_size; m++)
				{
					CInt cm_value;
					fread(&cm_value, sizeof(CInt), 1, filePtr);
					if (tempScene->m_geometries.size() >= j + 1)
						tempScene->m_geometries[j]->m_collapseMap[m] = cm_value;
				}

				///////////////////

				fread(&m_groupSize, sizeof(CUInt), 1, filePtr);
				//store group info

				if (sceneLoaded && !tempScene->m_isGrass)
				{
					if (m_hasDirtMap && tempScene->m_geometries.size() >= j + 1)
					{
						tempScene->m_geometries[j]->m_hasDirtMap = CTrue;
						Cpy(tempScene->m_geometries[j]->m_strDirtMap, m_strDirtMap);
					}
					if (m_hasNormalMap && tempScene->m_geometries.size() >= j + 1)
					{
						tempScene->m_geometries[j]->m_hasNormalMap = CTrue;
						Cpy(tempScene->m_geometries[j]->m_strNormalMap, m_strNormalMap);
					}
					if (tempScene->m_geometries.size() >= j + 1)
					{
						tempScene->m_geometries[j]->m_parallaxMapBias = m_bias;
						tempScene->m_geometries[j]->m_parallaxMapScale = m_scale;
					}
					if (m_hasGlossMap && tempScene->m_geometries.size() >= j + 1)
					{
						tempScene->m_geometries[j]->m_hasGlossMap = CTrue;
						Cpy(tempScene->m_geometries[j]->m_strGlossMap, m_strGlossMap);
					}
					if (m_hasDiffuse && tempScene->m_geometries.size() >= j + 1)
					{
						tempScene->m_geometries[j]->m_hasDiffuse = CTrue;
						Cpy(tempScene->m_geometries[j]->m_strDiffuse, m_strDiffuse);
					}
					//if( m_hasHeightMap )
					//	tempScene->m_geometries[j]->SetHeightMap( m_strHeightMap );
					//if( m_hasDuDvMap )
					//	tempScene->m_geometries[j]->SetDuDvMap(); //under construction!
					if (m_cullFaces && tempScene->m_geometries.size() >= j + 1)
						tempScene->m_geometries[j]->SetCullFace(CTrue);
					else if (tempScene->m_geometries.size() >= j + 1)
						tempScene->m_geometries[j]->SetCullFace(CFalse);

					for (CUInt k = 0; k < m_groupSize; k++)
					{

						CChar m_strGroupGlossMap[MAX_NAME_SIZE];
						CBool m_hasGroupGlossMap;
						fread(&m_hasGroupGlossMap, sizeof(CBool), 1, filePtr);
						fread(m_strGroupGlossMap, sizeof(CChar), MAX_NAME_SIZE, filePtr);
						if (m_hasGroupGlossMap && tempScene->m_geometries.size() >= j + 1)
							tempScene->m_geometries[j]->m_groups[k]->SetGlossMap(m_strGroupGlossMap);

						CChar m_strGroupDirtMap[MAX_NAME_SIZE];
						CBool m_hasGroupDirtMap;
						fread(&m_hasGroupDirtMap, sizeof(CBool), 1, filePtr);
						fread(m_strGroupDirtMap, sizeof(CChar), MAX_NAME_SIZE, filePtr);
						if (m_hasGroupDirtMap && tempScene->m_geometries.size() >= j + 1)
							tempScene->m_geometries[j]->m_groups[k]->SetDirtMap(m_strGroupDirtMap);

						CChar m_strGroupNormalMap[MAX_NAME_SIZE];
						CFloat m_bias, m_scale;
						CBool m_hasGroupNormalMap;
						fread(&m_hasGroupNormalMap, sizeof(CBool), 1, filePtr);
						fread(m_strGroupNormalMap, sizeof(CChar), MAX_NAME_SIZE, filePtr);
						fread(&m_bias, sizeof(CFloat), 1, filePtr);
						fread(&m_scale, sizeof(CFloat), 1, filePtr);
						if (m_hasGroupNormalMap && tempScene->m_geometries.size() >= j + 1)
							tempScene->m_geometries[j]->m_groups[k]->SetNormalMap(m_strGroupNormalMap, m_bias, m_scale);

						CChar m_strGroupDiffuseMap[MAX_NAME_SIZE];
						CBool m_hasGroupDiffuse;
						fread(&m_hasGroupDiffuse, sizeof(CBool), 1, filePtr);
						fread(m_strGroupDiffuseMap, sizeof(CChar), MAX_NAME_SIZE, filePtr);
						if (m_hasGroupDiffuse && tempScene->m_geometries.size() >= j + 1)
							tempScene->m_geometries[j]->m_groups[k]->SetDiffuse(m_strGroupDiffuseMap);
					}
				}
			} //for all of the geos

			CUInt instanceGeoSize;
			fread(&instanceGeoSize, 1, sizeof(CUInt), filePtr);
			for (CUInt j = 0; j < instanceGeoSize; j++)
			{
				CChar geoName[MAX_NAME_SIZE];
				CMatrix instanceLocalToWorldMatrix;
				CBool hasPhysX;
				CChar PhysXName[MAX_NAME_SIZE];
				CPhysXAlgorithm physXAlgorithm;
				CFloat physXDensity;
				CInt physXPercentage;
				CBool isTrigger;
				CBool isInvisible;
				CBool hasScriptEnter;
				CBool hasScriptExit;
				CChar scriptEnter[MAX_NAME_SIZE];
				CChar scriptExit[MAX_NAME_SIZE];
				fread(geoName, 1, sizeof(CChar) * MAX_NAME_SIZE, filePtr);
				fread(instanceLocalToWorldMatrix, 1, sizeof(CMatrix), filePtr);
				fread(&hasPhysX, 1, sizeof(CBool), filePtr);
				fread(PhysXName, 1, sizeof(CChar) * MAX_NAME_SIZE, filePtr);
				fread(&physXAlgorithm, 1, sizeof(CPhysXAlgorithm), filePtr);
				fread(&physXDensity, 1, sizeof(CFloat), filePtr);
				fread(&physXPercentage, 1, sizeof(CInt), filePtr);
				fread(&isTrigger, 1, sizeof(CBool), filePtr);
				fread(&isInvisible, 1, sizeof(CBool), filePtr);
				fread(scriptEnter, 1, sizeof(CChar) * MAX_NAME_SIZE, filePtr);
				fread(&hasScriptEnter, 1, sizeof(CBool), filePtr);
				fread(scriptExit, 1, sizeof(CChar) * MAX_NAME_SIZE, filePtr);
				fread(&hasScriptExit, 1, sizeof(CBool), filePtr);

				if (hasPhysX)
				{
					for (CUInt k = 0; k < tempScene->m_instanceGeometries.size(); k++)
					{
						if (Cmp(geoName, tempScene->m_instanceGeometries[k]->m_abstractGeometry->GetName()))
						{
							CBool equal = CTrue;
							for (CUInt l = 0; l < 16; l++)
							{
								if (instanceLocalToWorldMatrix[l] != tempScene->m_instanceGeometries[k]->m_localToWorldMatrix[l])
								{
									equal = CFalse;
									break;
								}
							}
							if (equal)
							{
								if (tempScene->GeneratePhysX(physXAlgorithm, physXDensity, physXPercentage, isTrigger, isInvisible, tempScene->m_instanceGeometries[k], CTrue))
								{
									CChar scriptEnterPath[MAX_NAME_SIZE];
									CChar scriptExitPath[MAX_NAME_SIZE];

									CChar* tempEnterPath = GetAfterPath(scriptEnter);
									CChar* tempExitPath = GetAfterPath(scriptExit);
									//Copy this to Win32 Project as well
									if (hasScriptEnter)
										sprintf(scriptEnterPath, "%s%s%s%s", packagePath, g_currentPrefabName, "/Scripts/", tempEnterPath);
									else
										Cpy(scriptEnterPath, "\n");

									if (hasScriptExit)
										sprintf(scriptExitPath, "%s%s%s%s", packagePath, g_currentPrefabName, "/Scripts/", tempExitPath);
									else
										Cpy(scriptExitPath, "\n");

									Cpy(tempScene->m_instanceGeometries[k]->m_enterScript, scriptEnterPath);
									Cpy(tempScene->m_instanceGeometries[k]->m_exitScript, scriptExitPath);
									tempScene->m_instanceGeometries[k]->m_hasEnterScript = hasScriptEnter;
									tempScene->m_instanceGeometries[k]->m_hasExitScript = hasScriptExit;

									if (physXDensity > 0 || tempScene->m_instanceGeometries[k]->m_abstractGeometry->m_hasAnimation)
										InsertItemToPhysXList(tempScene->m_instanceGeometries[k]->m_physXName, ePHYSXELEMENTLIST_DYNAMIC_RIGIDBODY);
									else if (isTrigger)
										InsertItemToPhysXList(tempScene->m_instanceGeometries[k]->m_physXName, ePHYSXELEMENTLIST_TRIGGER);
									else
										InsertItemToPhysXList(tempScene->m_instanceGeometries[k]->m_physXName, ePHYSXELEMENTLIST_STATIC_RIGIDBODY);
								}
							}
						}
					} //for
				} //if has PhysX
			}
		} // for all of the scenes
		if (g_currentCameraType == eCAMERA_DEFAULT_FREE)
		{
			g_multipleView->m_nx->gControllers->reportSceneChanged();
			gPhysXscene->simulate(1.0f / 60.0f/*elapsedTime*/);
			gPhysXscene->flushStream();
			gPhysXscene->fetchResults(NX_ALL_FINISHED, true);
		}
		g_updateOctree = CTrue;
		CChar temp[256];
		sprintf(temp, "%s%s%s", "Vanda Engine 1.4 : Prefab Mode (", prefabAndPackageName, ")");
		ex_pVandaEngine1Dlg->SetWindowTextA(temp);

		fclose(filePtr);
		ReleaseCapture();

		CChar temp2[MAX_NAME_SIZE];
		sprintf(temp2, "\ntotal errors: %i, total warnings: %i", totalErrors, totalWarnings);
		PrintInfo(temp2, COLOR_RED_GREEN);
		totalErrors = totalWarnings = 0;
	}
	else
	{
		g_openNewPrefabFromList = CTrue;
	}
	return CTrue;

}

CBool CVandaEngine1Dlg::OnMenuClickedOpenVScene(CBool askQuestion)
{
	CInt iResponse = IDNO;
	if( askQuestion && (g_scene.size() > 0 || g_engineLights.size() > 0 || g_engineWaters.size() > 0 || g_menu.m_insertAndShowSky || g_menu.m_insertAmbientSound || g_multipleView->m_nx->m_hasScene ) )
		iResponse= MessageBox( "Do you want to save your changes?", "Warning" , MB_YESNOCANCEL|MB_ICONSTOP);
	else
		iResponse = IDNO;
	CBool openScene = CFalse;
	if( iResponse == IDYES )
	{
		openScene = CTrue;
		if( g_multipleView->m_enableTimer )
			g_multipleView->EnableTimer( CFalse );
		OnMenuClickedSaveAs();
		if( g_multipleView->m_enableTimer )
			g_multipleView->EnableTimer( CTrue );
	}

	if ( iResponse == IDNO || openScene )
	{
		if (g_multipleView->m_enableTimer)
			g_multipleView->EnableTimer(CFalse);
		OnMenuClickedNew(CFalse); //do not ask question
		if (g_multipleView->m_enableTimer)
			g_multipleView->EnableTimer(CTrue);
		g_multipleView->RenderWindow();

		g_clickedOpen = CTrue;
		CString m_strpathName;
		INT_PTR result;
		if( askQuestion )
		{
			// Create an Open dialog; the default file name extension is ".vin".
			CEditProjectVScenes* m_dlgProjectVScenes = CNew(CEditProjectVScenes);
			m_dlgProjectVScenes->SetState(CTrue);
			result = m_dlgProjectVScenes->DoModal();
			m_strpathName = (CString)m_dlgProjectVScenes->GetName();
			CDelete(m_dlgProjectVScenes);
		}
		else
		{
			result = IDOK;
			m_strpathName = g_fileNameInCommandLine;
		}


		if (result == IDOK) //open a new scene
		{
			CChar reportTemp[MAX_NAME_SIZE];
			CChar pathName[MAX_NAME_SIZE];
			Cpy( pathName, m_strpathName.GetString() );
			sprintf( reportTemp, "\nVIN file '%s'...", GetAfterPath( pathName ) );
			PrintInfo( reportTemp, COLOR_RED_GREEN );

			CChar currentVSceneName[MAX_NAME_SIZE];
			Cpy(currentVSceneName, (CChar*)GetAfterPath(m_strpathName.GetBuffer(m_strpathName.GetLength()))); //For save functions
			m_strpathName.ReleaseBuffer();  //for save functions

			CChar currentVSceneNameWithoutDot[MAX_NAME_SIZE];
			Cpy(currentVSceneNameWithoutDot, currentVSceneName);
			GetWithoutDot(currentVSceneNameWithoutDot);

			CChar filePath[MAX_NAME_SIZE];
			sprintf(filePath, "%s%s%s%s", g_currentProjectPath, currentVSceneNameWithoutDot, "/", currentVSceneName);
			FILE *filePtr;
			filePtr = fopen(filePath, "rb");
			if(!filePtr )
			{
				MessageBox( "Couldn't open the file to load data\nMake sure that your selected scene belongs to current project", "Vanda Engine Error", MB_OK | MB_ICONERROR);
				return CFalse;
			}
			CChar engineName[MAX_NAME_SIZE];
			fread( &engineName, sizeof( CChar), MAX_NAME_SIZE, filePtr );
			if( !CmpIn( engineName, "VandaEngine" ) )
			{
				fclose( filePtr );
				MessageBox( "Invalid Vin file!", "Vanda Engine Error", MB_OK | MB_ICONERROR );
				return CFalse;
			}
			if (g_currentCameraType == eCAMERA_DEFAULT_PHYSX)
				PrintInfo("\nPlay mode disabled");

			g_currentCameraType = eCAMERA_DEFAULT_FREE;
			m_mainBtnTestActive.ShowWindow(SW_HIDE);
			m_mainBtnTestActive.EnableWindow(FALSE);
			m_mainBtnTestActive.UpdateWindow();
			m_mainBtnTestDeactive.ShowWindow(SW_SHOW);
			m_mainBtnTestDeactive.EnableWindow(TRUE);
			m_mainBtnTestDeactive.UpdateWindow();
			
			g_render.SetActiveInstanceCamera(g_render.GetDefaultInstanceCamera());

			g_importColladaImages = CFalse;

			SetCapture();
			SetCursor(LoadCursorFromFile("Assets/Engine/Icons/progress.ani"));
			Cpy(g_currentVSceneName, currentVSceneNameWithoutDot);
			fread(  &g_edition, sizeof( CChar), MAX_NAME_SIZE, filePtr );
			fread( &g_maxVersion, 1, sizeof( CInt ), filePtr );
			fread( &g_minVersion, 1, sizeof( CInt ), filePtr );
			fread( &g_bugFixesVersion, 1, sizeof( CInt ), filePtr );
			fread(  &g_currentPassword, sizeof( CChar), MAX_NAME_SIZE, filePtr );

			//read engine options
			fread( &g_shadowProperties, sizeof( CShadowProperties ), 1, filePtr  );
			switch( g_shadowProperties.m_shadowResolution )
			{
			case eSHADOW_1024:
				g_dynamicShadowMap->depth_size = 1024;
					break;
			case eSHADOW_2048:
				g_dynamicShadowMap->depth_size = 2048;
				break;
			case eSHADOW_4096:
				g_dynamicShadowMap->depth_size = 4096;
				break;
			default:
				break;
			}
			g_dynamicShadowMap->RegenerateDepthTex( g_dynamicShadowMap->depth_size );

			switch( g_shadowProperties.m_shadowSplits )
			{
			case eSHADOW_1_SPLIT:
				g_dynamicShadowMap->cur_num_splits = 1;
				break;
			case eSHADOW_2_SPLITS:
				g_dynamicShadowMap->cur_num_splits = 2;
				break;
			case eSHADOW_3_SPLITS:
				g_dynamicShadowMap->cur_num_splits = 3;
				break;
			case eSHADOW_4_SPLITS:
				g_dynamicShadowMap->cur_num_splits = 4;
				break;
			default:
				break;
			}

			g_dynamicShadowMap->split_weight = g_shadowProperties.m_shadowSplitWeight;
			fread( &g_physXProperties, sizeof( CPhysXProperties ), 1, filePtr );
			ResetPhysX(); //reset the physX based on the g_physXProperties information
			fread( &g_dofProperties, sizeof( CDOFProperties ), 1, filePtr  );
			fread( &g_fogProperties, sizeof( CFogProperties ), 1, filePtr  );
			fread( &g_bloomProperties, sizeof( CBloomProperties ), 1, filePtr  );
			fread( &g_lightProperties, sizeof( CLightProperties ), 1, filePtr  );
			fread( &g_pathProperties, sizeof( CPathProperties ), 1, filePtr  );
			fread( &g_vandaDemo, sizeof(CBool), 1, filePtr);

			CChar banner[MAX_NAME_SIZE];
			fread(&banner, sizeof(CChar), MAX_NAME_SIZE, filePtr );
			g_sceneBanner.SetBannerPath(banner);
			//save functions/////////////////////////////////
			g_sceneBanner.ClearVScenes();
			for( CUInt index = 0; index < g_VSceneNamesOfCurrentProject.size(); index++ )
			{
				if( Cmp( g_currentVSceneName, g_VSceneNamesOfCurrentProject[index].c_str() ) ) //current scene name found
					g_sceneBanner.AddVSceneToList( g_VSceneNamesOfCurrentProject[index], CFalse ); //Do not write to zip file
				else
					g_sceneBanner.AddVSceneToList( g_VSceneNamesOfCurrentProject[index], CTrue ); //Write to zip file
			}
			//save functions/////////////////////////////////

			fread( &g_extraTexturesNamingConventions, sizeof( CExtraTexturesNamingConventions ), 1, filePtr  );
			fread( &g_useGlobalAmbientColor, sizeof( CBool ), 1, filePtr  );
			fread( &g_globalAmbientColor, sizeof( CColor4f ), 1, filePtr  );
			fread( &g_sceneManagerObjectsPerSplit, sizeof( CInt), 1, filePtr );

			CFog fog;
			fog.SetColor( g_fogProperties.m_fogColor );
			fog.SetDensity( g_fogProperties.m_fogDensity );

			//read physX 
			CBool insertPhysXScene = CFalse;
			CChar strPhysXSceneName[MAX_NAME_SIZE];
			NxExtendedVec3 characterPos;
			CVec3f characterPitchYawTilt;
			CVec3f cameraInstancePos;
			CVec2f cameraInstancePanTilt;
			fread( &insertPhysXScene, sizeof( CBool ), 1, filePtr  );
			fread( strPhysXSceneName, sizeof( CChar ), MAX_NAME_SIZE, filePtr  );
			fread( &characterPos, sizeof( NxExtendedVec3 ), 1, filePtr );
			fread( &characterPitchYawTilt, sizeof( CVec3f ), 1, filePtr );
			g_characterPos = characterPos;

			g_multipleView->m_nx->gCharacterPos.x = g_characterPos.x;
			g_multipleView->m_nx->gCharacterPos.y = g_characterPos.y;
			g_multipleView->m_nx->gCharacterPos.z = g_characterPos.z;

			g_cameraPitchYawTilt.x = g_camera->m_perspectiveCameraPitch = characterPitchYawTilt.x;
			g_cameraPitchYawTilt.y = g_camera->m_perspectiveCameraYaw = characterPitchYawTilt.y;
			g_cameraPitchYawTilt.z = g_camera->m_perspectiveCameraTilt = characterPitchYawTilt.z;
			PrintInfo( "\nDefault PhysX camera info imported successfully" );

			fread( &cameraInstancePos, sizeof( CVec3f ), 1, filePtr );
			fread( &cameraInstancePanTilt, sizeof( CVec2f ), 1, filePtr );
			g_cameraInstancePos = cameraInstancePos;
			g_cameraInstancePanTilt = cameraInstancePanTilt;
			g_render.GetDefaultInstanceCamera()->MoveTransform2(cameraInstancePos.x, cameraInstancePos.y, cameraInstancePos.z);
			g_render.GetDefaultInstanceCamera()->SetPanAndTilt2(cameraInstancePanTilt.x, cameraInstancePanTilt.y);
			PrintInfo( "\nDefault Free camera info imported successfully" );

			if( insertPhysXScene )
			{
				//Copy this part to Win32 Project. Save functions
				CChar temp[MAX_NAME_SIZE];
				CChar* PhysXName = GetAfterPath( strPhysXSceneName );
				CChar PhysXPath[MAX_NAME_SIZE];
				CChar g_currentVSceneNameWithoutDot[MAX_NAME_SIZE];
				Cpy( g_currentVSceneNameWithoutDot, g_currentVSceneName );
				GetWithoutDot( g_currentVSceneNameWithoutDot );
				sprintf( PhysXPath, "%s%s%s%s", g_currentProjectPath, g_currentVSceneNameWithoutDot, "/External Physics/", PhysXName );
				if( g_multipleView->m_nx->LoadScene( PhysXPath, NXU::FT_XML ) )
				{
					g_physXProperties.m_bDebugMode = CTrue;
					m_btnRemovePhysX.EnableWindow( TRUE );
					GetMenu()->CheckMenuItem( ID_PHYSICS_DEBUGMODE, MF_CHECKED );
					PrintInfo( "\nPhysX debug activated" );

					g_multipleView->m_nx->SetSceneName( PhysXPath );
					sprintf( temp, "\nPhysX scene '%s' imported successufully", PhysXPath );
					PrintInfo( temp );

					m_physXElementListIndex = -1;
					for (int nItem = m_listBoxPhysXElements.GetItemCount()-1; nItem >= 0 ;nItem-- )
					{
						m_listBoxPhysXElements.DeleteItem(nItem);
					}
					CInt count = 0;
					for( std::vector<std::string>::iterator it = g_multipleView->m_nx->m_nxActorNames.begin(); it != g_multipleView->m_nx->m_nxActorNames.end();it++ )
					{
						CChar temp[MAX_NAME_SIZE];
						Cpy( temp, (*it).c_str() );
						InsertItemToPhysXList( temp, g_multipleView->m_nx->m_nxActorTypes[count] );
						count++;
					}
					//save functions/////////////////////////////////
					CDelete(g_externalPhysX);
					g_externalPhysX = CNew(CExternalPhysX);
					for( CUInt index = 0; index < g_VSceneNamesOfCurrentProject.size(); index++ )
					{
						if( Cmp( g_currentVSceneName, g_VSceneNamesOfCurrentProject[index].c_str() ) ) //current scene name found
							g_externalPhysX->AddVSceneToList( g_VSceneNamesOfCurrentProject[index], CFalse ); //Do not write to zip file
						else
							g_externalPhysX->AddVSceneToList( g_VSceneNamesOfCurrentProject[index], CTrue ); //Write to zip file
					}
					g_externalPhysX->SetPhysXPath(PhysXPath);
					//save functions/////////////////////////////////

				}
				else
				{
					sprintf( temp, "\nCouldn't load the PhysX scene '%s'", PhysXPath );
					PrintInfo( temp, COLOR_RED );
					g_multipleView->m_nx->SetSceneName( PhysXPath );
				}
			}
			g_multipleView->m_nx->ResetCharacterPos( characterPos );

			//read prefabs here
			CUInt prefabSize;
			fread(&prefabSize, sizeof(CUInt), 1, filePtr);
			for (CUInt i = 0; i < prefabSize; i++)
			{
				//read prefab data
				CChar name[MAX_NAME_SIZE];
				CChar package_name[MAX_NAME_SIZE];
				CChar prefab_name[MAX_NAME_SIZE];
				fread(name, sizeof(CChar), MAX_NAME_SIZE, filePtr);
				fread(package_name, sizeof(CChar), MAX_NAME_SIZE, filePtr);
				fread(prefab_name, sizeof(CChar), MAX_NAME_SIZE, filePtr);

				CPrefab* new_prefab = CNew(CPrefab);
				new_prefab->SetName(name);
				new_prefab->SetPackageName(package_name);
				new_prefab->SetPrefabName(prefab_name);


				g_prefab.push_back(new_prefab);

				CUInt prefabInstanceSize;
				fread(&prefabInstanceSize, sizeof(CUInt), 1, filePtr);

				for (CUInt j = 0; j < prefabInstanceSize; j++)
				{
					//read instance data
					CInstancePrefab* new_instance_prefab = CNew(CInstancePrefab);
					g_currentInstancePrefab = new_instance_prefab;
					fread(new_instance_prefab, sizeof(CInstancePrefab), 1, filePtr);
					new_prefab->SetInstance(new_instance_prefab);
					new_prefab->SetCurrentInstance(new_instance_prefab);
					new_instance_prefab->SetPrefab(new_prefab);
					new_instance_prefab->SetNameIndex(); //for selection only
					new_instance_prefab->GenQueryIndex();
					g_instancePrefab.push_back(new_instance_prefab);
					Cpy(g_currentInstancePrefabName, new_instance_prefab->GetName());
					g_editorMode = eMODE_PREFAB; //to load textures from prefab locations
					OnMenuClickedInsertPrefab(new_prefab);
					g_editorMode = eMODE_VSCENE; //to load textures from prefab locations
					if (CmpIn(new_instance_prefab->GetName(), "Vanda_Basics_Box_Trigger") || CmpIn(new_instance_prefab->GetName(), "Vanda_Basics_Sphere_Trigger"))
					{
						for (CUInt k = 0; k < new_instance_prefab->GetScene(0)->m_instanceGeometries.size(); k++)
						{
							Cpy(new_instance_prefab->GetScene(0)->m_instanceGeometries[k]->m_enterScript, new_instance_prefab->GetEnterScript());
							Cpy(new_instance_prefab->GetScene(0)->m_instanceGeometries[k]->m_exitScript, new_instance_prefab->GetExitScript());
							if (!Cmp(new_instance_prefab->GetScene(0)->m_instanceGeometries[k]->m_enterScript, "\n" ))
								new_instance_prefab->GetScene(0)->m_instanceGeometries[k]->m_hasEnterScript = CTrue;
							else
								new_instance_prefab->GetScene(0)->m_instanceGeometries[k]->m_hasEnterScript = CFalse;

							if (!Cmp(new_instance_prefab->GetScene(0)->m_instanceGeometries[k]->m_exitScript, "\n"))
								new_instance_prefab->GetScene(0)->m_instanceGeometries[k]->m_hasExitScript = CTrue;
							else
								new_instance_prefab->GetScene(0)->m_instanceGeometries[k]->m_hasExitScript = CFalse;

						}
					}
					new_instance_prefab->UpdateBoundingBox();
					new_instance_prefab->CalculateDistance();

					CChar tempInstanceName[MAX_NAME_SIZE];
					sprintf(tempInstanceName, "%s%s%s", "\nPrefab Instance ' ", new_instance_prefab->GetName(), " ' created successfully");
					PrintInfo(tempInstanceName, COLOR_GREEN);

					InsertItemToSceneList(new_instance_prefab->GetName());
				}
			}

			//g_octree->ResetState();

			//insert items
			//insert new instance and select it
			m_listBoxScenes.SetItemState(m_listBoxScenes.GetItemCount() - 1, LVIS_SELECTED, LVIS_SELECTED);
			m_listBoxScenes.SetSelectionMark(m_listBoxScenes.GetItemCount() - 1);
			//delete elements of previous selected prefab from the list
			for (int nItem = m_listBoxObjects.GetItemCount() - 1; nItem >= 0; nItem--)
			{
				m_listBoxObjects.DeleteItem(nItem);
			}
			//show the elements of newly selected prefab instance
			for (CUInt j = 0; j < 3; j++)
			{
				if (g_instancePrefab.size())
				{
					if (g_instancePrefab[g_instancePrefab.size() - 1]->GetPrefab()->GetHasLod(j))
						InsertItemToObjectList(g_instancePrefab[g_instancePrefab.size() - 1]->GetScene(j)->GetName(), eOBJECTLIST_SCENE);
				}
			}


			CChar g_currentVSceneNameWithoutDot[MAX_NAME_SIZE];
			Cpy(g_currentVSceneNameWithoutDot, g_currentVSceneName);
			GetWithoutDot(g_currentVSceneNameWithoutDot);

			CBool showSky;
			fread( &showSky, sizeof( CBool ), 1, filePtr );

			if( showSky )
			{
				CChar name[MAX_NAME_SIZE];
				CChar path[MAX_NAME_SIZE];
				CInt slices, sides;
				CFloat dampening, radius, position[3];
				CBool exponential;
				fread( name, sizeof( CChar ), MAX_NAME_SIZE, filePtr );
				fread( path, sizeof( CChar ), MAX_NAME_SIZE, filePtr );
				fread( &slices, sizeof( CInt ), 1, filePtr );
				fread( &sides, sizeof( CInt ), 1, filePtr );
				fread( &radius, sizeof( CFloat ), 1, filePtr );
				fread( position, sizeof( CFloat ), 3, filePtr );
				fread( &dampening, sizeof( CFloat ), 1, filePtr );
				fread( &exponential, sizeof( CBool ), 1, filePtr );
				
				CChar skyPath[MAX_NAME_SIZE];
				CChar* tempPath = GetAfterPath( path );
				//Copy this to Win32 Project as well
				sprintf( skyPath, "%s%s%s%s", g_currentProjectPath, g_currentVSceneNameWithoutDot, "/Sky/", tempPath );
				g_skyDome = CNew( CSkyDome );
				g_skyDome->SetName( name );
				g_skyDome->SetPath( skyPath );
				g_skyDome->SetSlices( slices );
				g_skyDome->SetSides( sides );
				g_skyDome->SetRadius( radius );
				g_skyDome->SetPosition( position );
				g_skyDome->SetDampening( dampening );
				g_skyDome->SetExponential( exponential );
				g_skyDome->Initialize();
				g_menu.m_insertAndShowSky = CTrue;
				GetMenu()->EnableMenuItem( ID_INSERT_SKYDOME, MF_DISABLED | MF_GRAYED );
				m_mainBtnSky.EnableWindow( FALSE );

				InserItemToEngineObjectList( g_skyDome->GetName() , eENGINEOBJECTLIST_SKY);

				//save functions/////////////////////////////////
				for( CUInt index = 0; index < g_VSceneNamesOfCurrentProject.size(); index++ )
				{
					if( Cmp( g_currentVSceneName, g_VSceneNamesOfCurrentProject[index].c_str() ) ) //current scene name found
						g_skyDome->AddVSceneToList( g_VSceneNamesOfCurrentProject[index], CFalse ); //Do not write to zip file
					else
						g_skyDome->AddVSceneToList( g_VSceneNamesOfCurrentProject[index], CTrue ); //Write to zip file
				}
				//save functions/////////////////////////////////

			}
			else
				g_menu.m_insertAndShowSky = CFalse;

			CInt tempWaterCount, tempInstancePrefabWaterCount;
			CChar strNormalMap[ MAX_NAME_SIZE];
			CChar strDuDvMap[ MAX_NAME_SIZE ];
			CChar strWaterName[MAX_NAME_SIZE];
			CFloat waterPos[3];
			CFloat waterLightPos[3];
			CFloat waterHeight, waterSpeed, waterScale, waterUV;

			fread( &tempWaterCount, sizeof( CInt ), 1, filePtr );
			for( CInt i = 0 ; i < tempWaterCount; i++ )
			{
				CWater* water = new CWater;

				fread( strWaterName, sizeof( CChar ), MAX_NAME_SIZE, filePtr  );
				fread( strNormalMap, sizeof( CChar ), MAX_NAME_SIZE, filePtr  );
				fread( strDuDvMap, sizeof( CChar ), MAX_NAME_SIZE, filePtr  );

				fread( waterPos, sizeof( CFloat ), 3, filePtr );
				fread( waterLightPos, sizeof( CFloat ), 3, filePtr );
				fread( &waterHeight, sizeof( CFloat ), 1, filePtr );
				fread( &waterSpeed, sizeof( CFloat ), 1, filePtr );
				fread( &waterScale, sizeof( CFloat ), 1, filePtr );
				fread( &waterUV, sizeof( CFloat ), 1, filePtr );

				fread(&tempInstancePrefabWaterCount, sizeof(CInt), 1, filePtr);
				for (CInt j = 0; j < tempInstancePrefabWaterCount; j++)
				{
					CChar instanceName[MAX_NAME_SIZE];
					fread(instanceName, sizeof(CChar), MAX_NAME_SIZE, filePtr);

					for (CUInt k = 0; k < g_instancePrefab.size(); k++)
					{
						if (Cmp(g_instancePrefab[k]->GetName(), instanceName))
							water->m_instancePrefab.push_back(g_instancePrefab[k]);
					}
				}

				CChar g_currentVSceneNameWithoutDot[MAX_NAME_SIZE];
				Cpy( g_currentVSceneNameWithoutDot, g_currentVSceneName );
				GetWithoutDot( g_currentVSceneNameWithoutDot );

				CChar dudvPath[MAX_NAME_SIZE];
				CChar* DuDvAfterPath = GetAfterPath( strDuDvMap );
				//Copy this to Win32 Project as well
				sprintf( dudvPath, "%s%s%s%s", g_currentProjectPath, g_currentVSceneNameWithoutDot, "/Waters/", DuDvAfterPath );

				CChar normalPath[MAX_NAME_SIZE];
				CChar* normalAfterPath = GetAfterPath( strNormalMap );
				//Copy this to Win32 Project as well
				sprintf( normalPath, "%s%s%s%s", g_currentProjectPath, g_currentVSceneNameWithoutDot, "/Waters/", normalAfterPath );

				water->SetName( strWaterName );
				water->SetDuDvMap( dudvPath );
				water->SetNormalMap( normalPath );
				water->SetHeight( waterHeight );
				water->SetSpeed( waterSpeed );
				water->SetScale( waterScale );
				water->SetUV( waterUV );
				water->SetPos( waterPos );
				water->SetLightPos( waterLightPos );
				water->CreateRenderTexture(g_waterTextureSize, 3, GL_RGB, WATER_REFLECTION_ID );
				water->CreateRenderTexture(g_waterTextureSize, 3, GL_RGB, WATER_REFRACTION_ID );
				water->CreateRenderTexture(g_waterTextureSize, 1, GL_DEPTH_COMPONENT, WATER_DEPTH_ID );
				water->SetSideVertexPositions();

				//save functions/////////////////////////////////
				for( CUInt index = 0; index < g_VSceneNamesOfCurrentProject.size(); index++ )
				{
					if( Cmp( g_currentVSceneName, g_VSceneNamesOfCurrentProject[index].c_str() ) ) //current scene name found
						water->AddVSceneToList( g_VSceneNamesOfCurrentProject[index], CFalse ); //Do not write to zip file
					else
						water->AddVSceneToList( g_VSceneNamesOfCurrentProject[index], CTrue ); //Write to zip file
				}
				//save functions/////////////////////////////////

				g_engineWaters.push_back( water );
				InserItemToEngineObjectList( water->GetName() , eENGINEOBJECTLIST_WATER);

			}
			//Engine Lights
			CInt tempLightCount;
			fread( &tempLightCount, sizeof( CInt ), 1, filePtr );
			CFloat m_fLightPos[4]; 	CFloat m_fConstantAttenuation; CFloat m_fLinearAttenuation; CFloat m_fQuadAttenuation; CFloat m_fSpotCuttoff;
			CFloat m_fSpotDirection[4]; CFloat m_fSpotExponent; CFloat m_fShininess; CFloat m_fAmbientColor[4]; CFloat m_fDiffuseColor[4];
			CFloat m_fSpecularColor[4]; CLightType m_lightType;
			CChar lightName[ MAX_NAME_SIZE ];
			for( CInt i = 0 ; i < tempLightCount; i++ )
			{
				fread( lightName, sizeof( CChar ), MAX_NAME_SIZE, filePtr  );
				fread( m_fLightPos, sizeof( CFloat ), 4, filePtr  );
				fread( &m_fConstantAttenuation, sizeof( CFloat ), 1, filePtr  );
				fread( &m_fLinearAttenuation, sizeof( CFloat ), 1, filePtr  );
				fread( &m_fQuadAttenuation, sizeof( CFloat ), 1, filePtr  );
				fread( &m_fShininess, sizeof( CFloat ), 1, filePtr  );
				fread( m_fAmbientColor, sizeof( CFloat ), 4, filePtr  );
				fread( m_fDiffuseColor, sizeof( CFloat ), 4, filePtr  );
				fread( m_fSpecularColor, sizeof( CFloat ), 4, filePtr  );
				fread( &m_lightType, sizeof( CLightType ), 1, filePtr  );
				if( m_lightType == eLIGHTTYPE_SPOT)
				{
					fread( &m_fSpotCuttoff, sizeof( CFloat ), 1, filePtr  );
					fread( m_fSpotDirection, sizeof( CFloat ), 4, filePtr  );
					fread( &m_fSpotExponent, sizeof( CFloat ), 1, filePtr  );
				}
				CInstanceLight* instance_light = new CInstanceLight();
				CLight* abstract_light = new CLight();

				instance_light->m_parent = NULL; //Not using COLLADA parent
				instance_light->m_abstractLight = abstract_light;
				abstract_light->SetName( lightName );
				abstract_light->SetType( m_lightType );
				switch( m_lightType )
				{
				case eLIGHTTYPE_SPOT:
					instance_light->m_abstractLight->SetSpotCutoff( m_fSpotCuttoff );
					instance_light->m_abstractLight->SetSpotExponent( m_fSpotExponent );
					instance_light->m_abstractLight->SetSpotDirection( m_fSpotDirection );
					break;
				}

				abstract_light->SetAmbient ( m_fAmbientColor );
				abstract_light->SetDiffuse( m_fDiffuseColor );
				abstract_light->SetSpecular( m_fSpecularColor );
				abstract_light->SetPosition( m_fLightPos );
				abstract_light->SetShininess( m_fShininess );
				abstract_light->SetConstantAttenuation( m_fConstantAttenuation );
				abstract_light->SetLinearAttenuation( m_fLinearAttenuation );
				abstract_light->SetQuadraticAttenuation( m_fQuadAttenuation );

				instance_light->SetIndex();

				g_engineLights.push_back( instance_light );

				InserItemToEngineObjectList( lightName, eENGINEOBJECTLIST_LIGHT );
				g_engineObjectNames.push_back( lightName );

			}

			//static sounds
			CInt tempStaticSoundCount;
			fread( &tempStaticSoundCount, sizeof( CInt ), 1, filePtr );
			CFloat staticSoundMaxDistance, staticSoundPitch, staticSoundReferenceDistance, staticSoundRolloff, staticSoundVolume;
			CFloat staticSoundPos[3];
			CBool play, loop;

			CChar name[ MAX_NAME_SIZE ], path[ MAX_NAME_SIZE ];
			CBool notLoaded = CFalse;

			for( CInt i = 0 ; i < tempStaticSoundCount; i++ )
			{
				fread( name, sizeof( CChar ), MAX_NAME_SIZE, filePtr  );
				fread( path, sizeof( CChar ), MAX_NAME_SIZE, filePtr  );
				fread( &loop, sizeof( CBool ), 1, filePtr  );
				fread( &staticSoundMaxDistance, sizeof( CFloat ), 1, filePtr  );
				fread( &staticSoundPitch, sizeof( CFloat ), 1, filePtr  );
				fread( &play, sizeof( CBool ), 1, filePtr  );
				fread( staticSoundPos, sizeof( CFloat ), 3, filePtr  );
				fread( &staticSoundReferenceDistance, sizeof( CFloat ), 1, filePtr  );
				fread( &staticSoundRolloff, sizeof( CFloat ), 1, filePtr  );
				fread( &staticSoundVolume, sizeof( CFloat ), 1, filePtr  );

				CChar StaticSoundPath[MAX_NAME_SIZE];
				CChar* StaticSoundName = GetAfterPath( path );
				//Copy this to Win32 Project as well
				sprintf( StaticSoundPath, "%s%s%s%s", g_currentProjectPath, g_currentVSceneNameWithoutDot, "/Sounds/Static/", StaticSoundName );

				COpenALSoundBuffer* m_staticSoundBuffer = GetSoundBuffer( GetAfterPath(StaticSoundPath) );
				
				if( m_staticSoundBuffer == NULL || (m_staticSoundBuffer && !m_staticSoundBuffer->m_loaded ) )
				{
					if( m_staticSoundBuffer == NULL )
					{
						m_staticSoundBuffer = CNew( COpenALSoundBuffer );
						g_soundBuffers.push_back( m_staticSoundBuffer );
					}
					else 
					{
						CChar tempBuffer[MAX_NAME_SIZE];
						sprintf( tempBuffer, "\nTrying to reload '%s%s", GetAfterPath(m_staticSoundBuffer->GetName() ), "'" );
						PrintInfo( tempBuffer, COLOR_YELLOW );
					}

					if( !m_staticSoundBuffer->LoadOggVorbisFromFile( StaticSoundPath ) )
					{
						CChar buffer[MAX_NAME_SIZE];
						sprintf( buffer, "\n%s%s%s", "Couldn't load the file '", StaticSoundPath, "'" );
						PrintInfo( buffer, COLOR_RED );
						m_staticSoundBuffer->m_loaded = CFalse;

					}
					else
					{
						CChar buffer[MAX_NAME_SIZE];
						sprintf( buffer, "\n%s%s%s", "ogg file '", StaticSoundPath, "' loaded successfully." );
						PrintInfo( buffer );
						m_staticSoundBuffer->m_loaded = CTrue;
					}

					m_staticSoundBuffer->SetName( StaticSoundPath );	
				}
				else
				{
						CChar temp[MAX_NAME_SIZE]; 
						sprintf( temp, "\n%s%s%s", "sound buffer '", GetAfterPath(m_staticSoundBuffer->GetName()), "' already exists." );
						PrintInfo( temp, COLOR_YELLOW );
				}
				COpenALSoundSource* m_staticSoundSource = CNew( COpenALSoundSource );
				CStaticSound* m_staticSound = CNew( CStaticSound );

				m_staticSoundSource->BindSoundBuffer (*m_staticSoundBuffer);
				m_staticSoundSource->SetLooping( loop );
				m_staticSoundSource->SetPitch( staticSoundPitch );

				//m_staticSoundSource->SetVolume( staticSoundVolume );
				m_staticSoundSource->SetReferenceDistance( staticSoundReferenceDistance );
				m_staticSoundSource->SetMaxDistance( staticSoundMaxDistance );
				m_staticSoundSource->SetRolloff( staticSoundRolloff );
				m_staticSoundSource->SetSoundPosition( staticSoundPos );

				m_staticSound->SetName( name );
				m_staticSound->SetPath( StaticSoundPath );
				m_staticSound->SetPosition( staticSoundPos );
				m_staticSound->SetLoop( loop );
				m_staticSound->SetMaxDistance( staticSoundMaxDistance );
				m_staticSound->SetPitch( staticSoundPitch );
				m_staticSound->SetPlay( play );
				m_staticSound->SetRefrenceDistance( staticSoundReferenceDistance );
				m_staticSound->SetRolloff( staticSoundRolloff );
				m_staticSound->SetSoundSource( m_staticSoundSource );
				m_staticSound->SetSoundBuffer( m_staticSoundBuffer );

				//save functions/////////////////////////////////
				for( CUInt index = 0; index < g_VSceneNamesOfCurrentProject.size(); index++ )
				{
					if( Cmp( g_currentVSceneName, g_VSceneNamesOfCurrentProject[index].c_str() ) ) //current scene name found
						m_staticSound->AddVSceneToList( g_VSceneNamesOfCurrentProject[index], CFalse ); //Do not write to zip file
					else
						m_staticSound->AddVSceneToList( g_VSceneNamesOfCurrentProject[index], CTrue ); //Write to zip file
				}
				//save functions/////////////////////////////////

				g_engineStaticSounds.push_back( m_staticSound );
				if( play && !notLoaded)
					g_multipleView->m_soundSystem->PlayALSound( *m_staticSoundSource );

				if( !notLoaded )
				{
					InserItemToEngineObjectList( m_staticSound->GetName() , eENGINEOBJECTLIST_STATICSOUND);
					PrintInfo( "\nStatic sound '", COLOR_GREEN );
					PrintInfo( StaticSoundPath, COLOR_RED_GREEN );
					PrintInfo( "' initialized successfully", COLOR_GREEN );
				}
				else
				{
					PrintInfo( "\nCouldn't load the static sound '", COLOR_RED );
					PrintInfo( StaticSoundPath, COLOR_RED_GREEN );
					PrintInfo( "'", COLOR_RED );
				}


			}

			//Ambient Sound
			CBool insertAmbientSound = CFalse;
			CChar strAmbientSoundName[MAX_NAME_SIZE];
			CChar strAmbientSoundPath[MAX_NAME_SIZE];
			CFloat volume, pitch;
			fread( &insertAmbientSound, sizeof( CBool ), 1, filePtr  );
			if( insertAmbientSound )
			{
				fread( strAmbientSoundName, sizeof( CChar ), MAX_NAME_SIZE, filePtr  );
				fread( strAmbientSoundPath, sizeof( CChar ), MAX_NAME_SIZE, filePtr  );
				fread( &volume, sizeof( CFloat ), 1, filePtr  );
				fread( &pitch, sizeof( CFloat ), 1, filePtr  );
			}

			CChar AmbientSoundPath[MAX_NAME_SIZE];
			CChar* AmbientSoundName = GetAfterPath( strAmbientSoundPath );
			//Copy this to Win32 Project as well
			sprintf( AmbientSoundPath, "%s%s%s%s", g_currentProjectPath, g_currentVSceneNameWithoutDot, "/Sounds/Ambient/", AmbientSoundName );
			//load the ambient sound if it exists
			if( insertAmbientSound )
			{
				CDelete( g_multipleView->m_ambientSound );
				COpenALSoundSource* m_ambientSoundSource = CNew( COpenALSoundSource );
				COpenALSoundBuffer* m_ambientSoundBuffer = CNew( COpenALSoundBuffer );

				//Initialize ambient sound here
				// Velocity of the source sound.
				if( !m_ambientSoundBuffer->LoadOggVorbisFromFile( AmbientSoundPath ) )
				{
					g_menu.m_insertAmbientSound = CTrue;
					g_multipleView->m_ambientSound = CNew( CAmbientSound );
					m_ambientSoundSource->BindSoundBuffer ( *m_ambientSoundBuffer );
					g_multipleView->m_ambientSound->SetName( strAmbientSoundName );
					g_multipleView->m_ambientSound->SetPath( strAmbientSoundPath );
					g_multipleView->m_ambientSound->SetSoundSource( m_ambientSoundSource );
					g_multipleView->m_ambientSound->SetSoundBuffer( m_ambientSoundBuffer );
					g_multipleView->m_ambientSound->SetVolume( volume );
					g_multipleView->m_ambientSound->SetPitch( pitch );
	
					PrintInfo("\nCouldn't load ambient sound '", COLOR_RED);
					PrintInfo( AmbientSoundPath, COLOR_RED_GREEN );
					PrintInfo( "'", COLOR_RED );
				}
				else
				{
					m_ambientSoundSource->BindSoundBuffer ( *m_ambientSoundBuffer );

					m_ambientSoundSource->SetLooping( true );
					m_ambientSoundSource->SetPitch( pitch );
					m_ambientSoundSource->SetVolume( volume );
					g_multipleView->m_soundSystem->PlayALSound( *m_ambientSoundSource );

					g_multipleView->m_ambientSound = CNew( CAmbientSound );
					g_multipleView->m_ambientSound->SetSoundSource( m_ambientSoundSource );
					g_multipleView->m_ambientSound->SetSoundBuffer( m_ambientSoundBuffer );
					g_multipleView->m_ambientSound->SetName( strAmbientSoundName );
					g_multipleView->m_ambientSound->SetPath( AmbientSoundPath );
					g_multipleView->m_ambientSound->SetVolume( volume );
					g_multipleView->m_ambientSound->SetPitch( pitch );
					//save functions/////////////////////////////////
					for( CUInt index = 0; index < g_VSceneNamesOfCurrentProject.size(); index++ )
					{
						if( Cmp( g_currentVSceneName, g_VSceneNamesOfCurrentProject[index].c_str() ) ) //current scene name found
							g_multipleView->m_ambientSound->AddVSceneToList( g_VSceneNamesOfCurrentProject[index], CFalse ); //Do not write to zip file
						else
							g_multipleView->m_ambientSound->AddVSceneToList( g_VSceneNamesOfCurrentProject[index], CTrue ); //Write to zip file
					}
					//save functions/////////////////////////////////

					PrintInfo("\nAmbient sound '");
					PrintInfo( AmbientSoundPath, COLOR_RED_GREEN );
					PrintInfo( "' loaded successfully" );
					
					g_menu.m_playAmbientSound = CTrue;
					g_menu.m_insertAmbientSound = CTrue;
					GetMenu()->EnableMenuItem( ID_INSERT_SOUND_AMBIENT, MF_DISABLED | MF_GRAYED );
					m_mainBtnAmbientSound.EnableWindow( FALSE );

					InserItemToEngineObjectList( g_multipleView->m_ambientSound->GetName(), eENGINEOBJECTLIST_AMBIENTSOUND );
				}

			}

			CPrefab* box = CNew(CPrefab);
			CChar pr_name[MAX_NAME_SIZE];
			CChar package_name[MAX_NAME_SIZE];
			CChar prefab_name[MAX_NAME_SIZE];
			Cpy(package_name, "Vanda_Basics");
			Cpy(prefab_name, "Box_Trigger");
			sprintf(pr_name, "%s%s%s", package_name, "_", prefab_name);

			box->SetName(pr_name);
			box->SetPackageName(package_name);
			box->SetPrefabName(prefab_name);

			g_prefab.push_back(box);

			CPrefab* sphere = CNew(CPrefab);
			CChar pr_name2[MAX_NAME_SIZE];
			CChar package_name2[MAX_NAME_SIZE];
			CChar prefab_name2[MAX_NAME_SIZE];
			Cpy(package_name2, "Vanda_Basics");
			Cpy(prefab_name2, "Sphere_Trigger");
			sprintf(pr_name2, "%s%s%s", package_name2, "_", prefab_name2);

			sphere->SetName(pr_name2);
			sphere->SetPackageName(package_name2);
			sphere->SetPrefabName(prefab_name2);

			g_prefab.push_back(sphere);

			CInt triggerSize;
			fread(&triggerSize, sizeof(CInt), 1, filePtr);
			for (CInt i = 0; i < triggerSize; i++)
			{
				CChar trigger_name[MAX_NAME_SIZE];
				CChar instance_name[MAX_NAME_SIZE];
				CTriggerType type;
				CVec3f translation;
				CVec4f rotation;
				CVec3f scaling;
				CChar m_enterScript[MAX_NAME_SIZE];
				CChar m_exitScript[MAX_NAME_SIZE];
				fread(trigger_name, sizeof(CChar), MAX_NAME_SIZE, filePtr);
				fread(&type, sizeof(CTriggerType), 1, filePtr);

				fread(instance_name, sizeof(CChar), MAX_NAME_SIZE, filePtr);
				fread(&translation, sizeof(CVec3f), 1, filePtr);
				fread(&rotation, sizeof(CVec4f), 1, filePtr);
				fread(&scaling, sizeof(CVec3f), 1, filePtr);
				fread(m_enterScript, sizeof(CChar), MAX_NAME_SIZE, filePtr);
				fread(m_exitScript, sizeof(CChar), MAX_NAME_SIZE, filePtr);

				//read prefab data

				CTrigger* new_trigger = CNew(CTrigger);
				CPrefab* new_prefab;
				if (type == eTRIGGER_BOX)
				{
					new_trigger->SetTriggerType(eTRIGGER_BOX);
					new_prefab = box;
				}
				else
				{
					new_trigger->SetTriggerType(eTRIGGER_SPHERE);
					new_prefab = sphere;
				}

				new_trigger->SetName(trigger_name);

				CInstancePrefab* new_instance_prefab = CNew(CInstancePrefab);
				g_currentInstancePrefab = new_instance_prefab;
				new_prefab->SetInstance(new_instance_prefab);
				new_prefab->SetCurrentInstance(new_instance_prefab);
				new_instance_prefab->SetPrefab(new_prefab);
				new_instance_prefab->SetName(instance_name);
				new_instance_prefab->SetNameIndex(); //for selection only
				new_instance_prefab->GenQueryIndex();
				g_instancePrefab.push_back(new_instance_prefab);
				Cpy(g_currentInstancePrefabName, new_instance_prefab->GetName());
				g_importPrefab = CTrue;
				ex_pVandaEngine1Dlg->OnMenuClickedInsertPrefab(new_prefab);
				g_importPrefab = CFalse;

				new_trigger->SetInstancePrefab(g_instancePrefab[g_instancePrefab.size() - 1]); //last element
				new_trigger->GetInstancePrefab()->SetIsTrigger(CTrue);

				new_trigger->GetInstancePrefab()->SetTranslate(translation);
				new_trigger->GetInstancePrefab()->SetRotate(rotation);
				new_trigger->GetInstancePrefab()->SetScale(scaling);
				new_trigger->GetInstancePrefab()->UpdateBoundingBox();
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

				g_triggers.push_back(new_trigger);
				ex_pVandaEngine1Dlg->InserItemToEngineObjectList(new_trigger->GetName(), eENGINEOBJECTLIST_TRIGGER);
				g_engineObjectNames.push_back(new_trigger->GetName());


			}

			fclose( filePtr );
			ReleaseCapture();
			m_savePathName = m_strpathName;
			Cpy(g_currentVSceneName, currentVSceneName);
			for (CUInt i = 0; i < g_projects.size(); i++)
			{
				if (g_projects[i]->m_isActive)
				{
					CChar m_currentVSceneNameWithoutDot[MAX_NAME_SIZE];
					if (Cmp(g_currentVSceneName, "\n"))
						Cpy(m_currentVSceneNameWithoutDot, "Untitled");
					else
					{
						Cpy(m_currentVSceneNameWithoutDot, g_currentVSceneName);
						GetWithoutDot(m_currentVSceneNameWithoutDot);
					}

					CChar temp[256];
					sprintf(temp, "%s%s%s%s%s", "Vanda Engine 1.4 (", g_projects[i]->m_name, " - ", m_currentVSceneNameWithoutDot, ")");
					ex_pVandaEngine1Dlg->SetWindowTextA(temp);

					break;
				}
			}

			CChar temp[MAX_NAME_SIZE];
			sprintf( temp, "\ntotal errors: %i, total warnings: %i", totalErrors, totalWarnings );
			PrintInfo( temp, COLOR_RED_GREEN );
			totalErrors = totalWarnings = 0;

		}

		if (g_editorMode == eMODE_VSCENE)
		{
			g_multipleView->RenderQueries(CTrue);
		}
		g_transformObject = CFalse;
		g_selectedName = -1;
		g_clickedOpen = CFalse;
	}
	else if( iResponse == IDCANCEL )
		return CFalse;
	return CTrue;

}

CVoid CVandaEngine1Dlg::OnMenuClickedPrefab()
{
	m_dlgPrefabs = CNew(CPrefabDlg);
	ex_pDlgPrefabs = m_dlgPrefabs;
	INT_PTR result = m_dlgPrefabs->DoModal();
	CDelete(m_dlgPrefabs);
}

CVoid CVandaEngine1Dlg::OnMenuClickedEditPhysX()
{
	if( g_selectedName != -1 )
	{
		CBool foundTarget = CFalse;
		for( CUInt i = 0 ; i < g_scene.size(); i++ )
		{
			for( CUInt j = 0; j < g_scene[i]->m_instanceGeometries.size(); j++ )
			{
				CGeometry* m_geometry = g_scene[i]->m_instanceGeometries[j]->m_abstractGeometry;
				if( g_menu.m_geometryBasedSelection )
				{
					if(  g_scene[i]->m_instanceGeometries[j]->m_nameIndex == g_selectedName )
					{
						foundTarget = CTrue;
						g_render.SetSelectedScene( g_scene[i] ); 
						SetDialogData( CTrue, g_scene[i]->m_instanceGeometries[j], m_geometry );
					}
				}
			}
		}
	}

	INT_PTR result = m_dlgEditPhysX->DoModal();
}

CVoid CVandaEngine1Dlg::OnMenuClickedEditMaterial()
{
	m_dlgEditMaterial->m_firstBias = m_dlgEditMaterial->m_firstScale = CTrue;
	if( g_selectedName != -1 )
	{
		CBool foundTarget = CFalse;
		for( CUInt i = 0 ; i < g_scene.size(); i++ )
		{
			for( CUInt j = 0; j < g_scene[i]->m_instanceGeometries.size(); j++ )
			{
				CGeometry* m_geometry = g_scene[i]->m_instanceGeometries[j]->m_abstractGeometry;
				if( g_menu.m_geometryBasedSelection )
				{
					if(  g_scene[i]->m_instanceGeometries[j]->m_nameIndex == g_selectedName )
					{
						foundTarget = CTrue;
						g_render.SetSelectedScene( g_scene[i] ); 
						SetDialogData( CTrue, g_scene[i]->m_instanceGeometries[j], m_geometry );
					}
				}
				else //material based selection
				{
					for( CUInt k = 0; k < m_geometry->m_groups.size(); k++ )
					{
						CPolyGroup* m_group = m_geometry->m_groups[k];
						if( m_group->m_nameIndex == g_selectedName )
						{
							foundTarget = CTrue;
							g_render.SetSelectedScene( g_scene[i] ); 
							SetDialogData2( CTrue, g_scene[i]->m_instanceGeometries[j]->m_abstractGeometry->m_groups[k] );
						}
					}
				}
			}

		}
	}
	INT_PTR result = m_dlgEditMaterial->DoModal();
}

BOOL CVandaEngine1Dlg::OnEraseBkgnd(CDC* pDC)
{
	// TODO: Add your message handler code here and/or call default
    CRect rect;
    GetClientRect(&rect);
    CBrush myBrush( RGB(40,40,40) );    // dialog background color
    CBrush *pOld = pDC->SelectObject(&myBrush);
    BOOL bRes = pDC->PatBlt(0, 0, rect.Width(), rect.Height(), PATCOPY);


	CRect windowRect;
	GetClientRect( &windowRect );
	CFloat aspectRatio = fabs( CFloat( windowRect.bottom - windowRect.top ) / CFloat( windowRect.right - windowRect.left ) );
	//Standard size of left buttons = 6
	CInt fivePercent = ( (5 * ( windowRect.right - windowRect.left ) / 100) * aspectRatio);
	//horizontal gap between buttons
	m_horizontalPointFivePercent = CInt(0.5 * (windowRect.right - windowRect.left) / 100);
	m_horizontalSizeOfLeftCulomn = 2 * m_horizontalPointFivePercent + 1 * fivePercent; //2 gaps+1 Buttons
	m_horizontalSizeOfRightCulomn = 5 * m_horizontalPointFivePercent + 5.5 * fivePercent; //5 gaps+3 Buttons

	//Vertical gap between areas
	CFloat verticalOnePercent = ( windowRect.bottom - windowRect.top ) / 100.0f;

	CRect rcRect;
	rcRect.left = CInt( 100 * ( windowRect.right - windowRect.left ) / 100 ) - m_horizontalSizeOfRightCulomn + m_horizontalPointFivePercent;
	rcRect.right = CInt( 99.5 * ( windowRect.right - windowRect.left ) / 100 );
	//ScreenToClient( rcRect );
	CPen * myPen = new CPen(PS_SOLID, 1, RGB(15, 15, 15));
	pDC->SelectObject(myPen);  
	//COLLADA scenes area

	//m_startLeftButtons = 6;
	previousColumn1 = /*m_startLeftButtons - 1*/5 * (windowRect.bottom - windowRect.top) / 100;

	rcRect.top = previousColumn1 + long(4 * verticalOnePercent);
	rcRect.bottom = rcRect.top + long(26.1 * verticalOnePercent);

	pDC->RoundRect(rcRect.left, rcRect.top, rcRect.right, rcRect.bottom, 5, 5);
	/////
	//COLLADA objects area
	previousColumn2 = rcRect.bottom;

	rcRect.top = previousColumn2 + long(4 * verticalOnePercent);
	rcRect.bottom = rcRect.top + long(26.1 * verticalOnePercent);
	pDC->RoundRect(rcRect.left, rcRect.top, rcRect.right, rcRect.bottom, 5, 5);

	/////
	//Engine Objects area
	previousColumn3 = rcRect.bottom;

	rcRect.top = previousColumn3 + long(4 * verticalOnePercent);
	rcRect.bottom = rcRect.top + long(26.1 * verticalOnePercent);
	pDC->RoundRect(rcRect.left, rcRect.top, rcRect.right, rcRect.bottom, 5, 5);
	/////
	rcRect.left = CInt(3.6 * (windowRect.right - windowRect.left) / 100);
	rcRect.top = CInt(96.0 * (windowRect.bottom - windowRect.top) / 100);
	rcRect.right = CInt(44.3 * (windowRect.right - windowRect.left) / 100);
	rcRect.bottom = CInt(99.5 * (windowRect.bottom - windowRect.top) / 100);

	pDC->RoundRect(rcRect.left, rcRect.top, rcRect.right, rcRect.bottom, 5, 5);

	rcRect.left = CInt(44.7 * (windowRect.right - windowRect.left) / 100);
	rcRect.right = CInt(65.3 * (windowRect.right - windowRect.left) / 100);

	pDC->RoundRect(rcRect.left, rcRect.top, rcRect.right, rcRect.bottom, 5, 5);
	/////

	if (g_editorMode == eMODE_VSCENE)
	{
		rcRect.left = CInt(67.4 * (windowRect.right - windowRect.left) / 100);
		rcRect.right = CInt(71.8 * (windowRect.right - windowRect.left) / 100);
		pDC->RoundRect(rcRect.left, rcRect.top, rcRect.right, rcRect.bottom, 5, 5);

		rcRect.left = CInt(73.7 * (windowRect.right - windowRect.left) / 100);
		rcRect.right = CInt(78.1 * (windowRect.right - windowRect.left) / 100);
		pDC->RoundRect(rcRect.left, rcRect.top, rcRect.right, rcRect.bottom, 5, 5);

		rcRect.left = CInt(80.0 * (windowRect.right - windowRect.left) / 100);
		rcRect.right = CInt(84.4 * (windowRect.right - windowRect.left) / 100);
		pDC->RoundRect(rcRect.left, rcRect.top, rcRect.right, rcRect.bottom, 5, 5);
	}

	//PhysX Elements area
	previousColumn4 = rcRect.bottom;

	//rcRect.top = previousColumn4 + long(4 * verticalOnePercent);
	//rcRect.bottom = rcRect.top + long(26.5 * verticalOnePercent);
	//pDC->RoundRect( rcRect.left, rcRect.top, rcRect.right, rcRect.bottom, 10, 10 );
	/////

	delete myPen; 

    pDC->SelectObject(pOld);    // restore old brush

    return bRes;                       // CDialog::OnEraseBkgnd(pDC);

	return CDialog::OnEraseBkgnd(pDC);
}

CVoid CVandaEngine1Dlg::OnBnClickedCullFace()
{
	//reverse cull face for the current selected items
	for( CUInt i = 0 ; i < g_scene.size(); i++ )
	{
		for( CUInt j = 0; j < g_scene[i]->m_instanceGeometries.size(); j++ )
		{
			if(  g_scene[i]->m_instanceGeometries[j]->m_nameIndex == g_selectedName )
			{
				if( g_scene[i]->m_instanceGeometries[j]->m_abstractGeometry->m_cullFaces )
					CheckCullFace( eCULLFACETYPEFORMENU_UNCHECKED );
				else
					CheckCullFace( eCULLFACETYPEFORMENU_CHECKED );

				g_scene[i]->m_instanceGeometries[j]->m_abstractGeometry->m_cullFaces = !g_scene[i]->m_instanceGeometries[j]->m_abstractGeometry->m_cullFaces;

			}
		}
	}
}


void CVandaEngine1Dlg::OnBnClickedBtnEngineObjectProperties()
{
	int nSelected = -1; 
	POSITION p = m_listBoxEngineObjects.GetFirstSelectedItemPosition();
	while(p)
	{
		nSelected = m_listBoxEngineObjects.GetNextSelectedItem(p);
	}
	if( nSelected >= 0 )
	{
		TCHAR szBuffer[1024];
		DWORD cchBuf(1024);
		LVITEM lvi;
		lvi.iItem = nSelected;
		lvi.iSubItem = 0;
		lvi.mask = LVIF_TEXT;
		lvi.pszText = szBuffer;
		lvi.cchTextMax = cchBuf;
		m_listBoxEngineObjects.GetItem(&lvi);

		if( g_skyDome && Cmp( g_skyDome->GetName(), szBuffer )  )
		{
			if( g_multipleView->m_enableTimer )
				g_multipleView->EnableTimer( CFalse );
		
			ChangeSkyDomeProperties();

			if( g_multipleView->m_enableTimer )
				g_multipleView->EnableTimer( CTrue );

			g_multipleView->SetElapsedTimeFromBeginning();
			g_multipleView->RenderWindow();
			return;
		}
		for( CUInt i = 0; i < g_engineLights.size(); i++ )
		{
			if( Cmp( g_engineLights[i]->m_abstractLight->GetName(), szBuffer ) )
			{
				if( g_multipleView->m_enableTimer )
					g_multipleView->EnableTimer( CFalse );

				ChangeLightProperties( g_engineLights[i]);

				if( g_multipleView->m_enableTimer )
					g_multipleView->EnableTimer( CTrue );

				g_multipleView->SetElapsedTimeFromBeginning();
				g_multipleView->RenderWindow();
				return;
			}
		}
		for( CUInt i = 0; i < g_engineWaters.size(); i++ )
		{
			if( Cmp( g_engineWaters[i]->GetName(), szBuffer ) )
			{
				if( g_multipleView->m_enableTimer )
					g_multipleView->EnableTimer( CFalse );

				ChangeWaterProperties(g_engineWaters[i]);

				if( g_multipleView->m_enableTimer )
					g_multipleView->EnableTimer( CTrue );

				g_multipleView->SetElapsedTimeFromBeginning();
				g_multipleView->RenderWindow();
				return;
			}
		}

		for( CUInt i = 0; i < g_engineStaticSounds.size(); i++ )
		{
			if( Cmp( g_engineStaticSounds[i]->GetName(), szBuffer ) )
			{
				if( g_multipleView->m_enableTimer )
					g_multipleView->EnableTimer( CFalse );

				ChangeStaticSoundProperties(g_engineStaticSounds[i]);

				if( g_multipleView->m_enableTimer )
					g_multipleView->EnableTimer( CTrue );

				g_multipleView->SetElapsedTimeFromBeginning();
				return;
			}
		}
		for (CUInt i = 0; i < g_triggers.size(); i++)
		{
			if (Cmp(g_triggers[i]->GetName(), szBuffer))
			{
				if (g_multipleView->m_enableTimer)
					g_multipleView->EnableTimer(CFalse);

				m_dlgAddTrigger = CNew(CAddTrigger);
				m_dlgAddTrigger->Init(g_triggers[i]);
				m_dlgAddTrigger->DoModal();
				CDelete(m_dlgAddTrigger);
				m_listBoxEngineObjects.DeleteItem(nSelected);
				InserItemToEngineObjectList(g_triggers[i]->GetName(), eENGINEOBJECTLIST_TRIGGER);
				for (std::vector<std::string>::iterator it = g_engineObjectNames.begin(); it != g_engineObjectNames.end(); it++)
					if (Cmp("", (*it).c_str()))
						(*it) = g_triggers[i]->GetName();

				if (g_multipleView->m_enableTimer)
					g_multipleView->EnableTimer(CTrue);

				g_multipleView->SetElapsedTimeFromBeginning();
				return;

			}
		}
		if( g_menu.m_insertAmbientSound && Cmp( g_multipleView->m_ambientSound->GetName(), szBuffer )  )
		{
			if( g_multipleView->m_enableTimer )
				g_multipleView->EnableTimer( CFalse );

			ChangeAmbientSoundProperties();

			if( g_multipleView->m_enableTimer )
				g_multipleView->EnableTimer( CTrue );

			g_multipleView->SetElapsedTimeFromBeginning();
			return;
		}
	}
}
CVoid CVandaEngine1Dlg::SortEngineObjectList(CInt selectedIndex)
{
	m_engineObjectListIndex--;
}
void CVandaEngine1Dlg::OnBnClickedBtnRemoveEngineObject()
{
	int nSelected = -1; 
	POSITION p = m_listBoxEngineObjects.GetFirstSelectedItemPosition();
	while(p)
	{
		nSelected = m_listBoxEngineObjects.GetNextSelectedItem(p);
	}
	if( nSelected >= 0 )
	{
		TCHAR szBuffer[1024];
		DWORD cchBuf(1024);
		LVITEM lvi;
		lvi.iItem = nSelected;
		lvi.iSubItem = 0;
		lvi.mask = LVIF_TEXT;
		lvi.pszText = szBuffer;
		lvi.cchTextMax = cchBuf;
		m_listBoxEngineObjects.GetItem(&lvi);

		CInt result = IDYES;
		if( m_askRemoveEngineObject )
		{
			CChar tempString[MAX_NAME_SIZE];
			sprintf( tempString, "Delete object '%s' ?", szBuffer );
			result = MessageBox( tempString, "Warning", MB_YESNO | MB_ICONERROR );
		}
		if( result == IDYES )
		{
			for( CUInt i = 0; i < g_engineObjectNames.size(); i++ )
			{
				if( Cmp( g_engineObjectNames[i].c_str(), szBuffer ) )
					g_engineObjectNames[i] = "";
			}

			if( g_skyDome && Cmp( g_skyDome->GetName(), szBuffer )  )
			{
				if( g_skyDome->GetIndex() == g_selectedName || g_skyDome->GetIndex()  == g_lastEngineObjectSelectedName )
				{
					g_showArrow = CFalse;
				}
				CDelete( g_skyDome );
				g_menu.m_insertAndShowSky = CFalse;
				GetMenu()->EnableMenuItem( ID_INSERT_SKYDOME, MF_ENABLED );
				m_mainBtnSky.EnableWindow( TRUE );

				m_listBoxEngineObjects.DeleteItem(nSelected);
				g_multipleView->SetElapsedTimeFromBeginning();
				g_multipleView->RenderWindow();
				if( m_listBoxEngineObjects.GetItemCount() == 0 )
				{
					m_btnRemoveEngineObject.EnableWindow( FALSE );
					m_btnEngineObjectProperties.EnableWindow( FALSE );
				}
				SortEngineObjectList(nSelected);
				return;
			}
			for( CUInt i = 0; i < g_engineLights.size(); i++ )
			{
				if( Cmp( g_engineLights[i]->m_abstractLight->GetName(), szBuffer ) )
				{
					if( g_engineLights[i]->GetIndex() == g_selectedName || g_engineLights[i]->GetIndex()  == g_lastEngineObjectSelectedName )
					{
						g_showArrow = CFalse;
					}

					if( g_engineLights[i]->m_abstractLight->GetType() == eLIGHTTYPE_DIRECTIONAL )
						if( Cmp( g_shadowProperties.m_directionalLightName, g_engineLights[i]->m_abstractLight->GetName() ) )
							Cpy( g_shadowProperties.m_directionalLightName, "\n" );

					CDelete( g_engineLights[i] );
					g_engineLights.erase( g_engineLights.begin() + i );

					m_listBoxEngineObjects.DeleteItem(nSelected);
					g_multipleView->SetElapsedTimeFromBeginning();
					g_multipleView->RenderWindow();
					if( m_listBoxEngineObjects.GetItemCount() == 0 )
					{
						m_btnRemoveEngineObject.EnableWindow( FALSE );
						m_btnEngineObjectProperties.EnableWindow( FALSE );
					}
					SortEngineObjectList(nSelected);
					return;

				}
			}
			for( CUInt i = 0; i < g_engineWaters.size(); i++ )
			{
				if( Cmp( g_engineWaters[i]->GetName(), szBuffer ) )
				{
					if( g_engineWaters[i]->GetIndex() == g_selectedName || g_engineWaters[i]->GetIndex()  == g_lastEngineObjectSelectedName )
					{
						g_showArrow = CFalse;
					}

					//delete the scene
					CDelete(  g_engineWaters[i] );
					//delete the vector that holds the scene
					g_engineWaters.erase( g_engineWaters.begin( ) + i );

					m_listBoxEngineObjects.DeleteItem(nSelected);
					g_multipleView->SetElapsedTimeFromBeginning();
					g_multipleView->RenderWindow();
					if( m_listBoxEngineObjects.GetItemCount() == 0 )
					{
						m_btnRemoveEngineObject.EnableWindow( FALSE );
						m_btnEngineObjectProperties.EnableWindow( FALSE );
					}
					SortEngineObjectList(nSelected);
					return;
				}
			}

			for( CUInt i = 0; i < g_engineStaticSounds.size(); i++ )
			{
				if( Cmp( g_engineStaticSounds[i]->GetName(), szBuffer ) )
				{
					if( g_engineStaticSounds[i]->GetIndex() == g_selectedName || g_engineStaticSounds[i]->GetIndex()  == g_lastEngineObjectSelectedName )
					{
						g_showArrow = CFalse;
					}

					//delete the scene
					CDelete(  g_engineStaticSounds[i] );
					//delete the vector that holds the scene
					g_engineStaticSounds.erase( g_engineStaticSounds.begin( ) + i );

					m_listBoxEngineObjects.DeleteItem(nSelected);
					g_multipleView->SetElapsedTimeFromBeginning();
					g_multipleView->RenderWindow();
					if( m_listBoxEngineObjects.GetItemCount() == 0 )
					{
						m_btnRemoveEngineObject.EnableWindow( FALSE );
						m_btnEngineObjectProperties.EnableWindow( FALSE );
					}
					SortEngineObjectList(nSelected);
					return;
				}
			}

			for (CUInt i = 0; i < g_triggers.size(); i++)
			{
				if (Cmp(g_triggers[i]->GetName(), szBuffer))
				{
					if (g_triggers[i]->GetInstancePrefab()->GetNameIndex() == g_selectedName || g_triggers[i]->GetInstancePrefab()->GetNameIndex() == g_lastEngineObjectSelectedName)
					{
						g_showArrow = CFalse;
					}

					CDelete(g_triggers[i]);
					g_triggers.erase(g_triggers.begin() + i);

					m_listBoxEngineObjects.DeleteItem(nSelected);
					g_multipleView->SetElapsedTimeFromBeginning();
					g_multipleView->RenderQueries(CTrue);
					g_multipleView->RenderWindow();
					if (m_listBoxEngineObjects.GetItemCount() == 0)
					{
						m_btnRemoveEngineObject.EnableWindow(FALSE);
						m_btnEngineObjectProperties.EnableWindow(FALSE);
					}
					SortEngineObjectList(nSelected);
					return;
				}
			}

			if( g_menu.m_insertAmbientSound && Cmp( g_multipleView->m_ambientSound->GetName(), szBuffer )  )
			{
				g_multipleView->m_soundSystem->StopALSound( *(g_multipleView->m_ambientSound->GetSoundSource()) );
				alSourcei(g_multipleView->m_ambientSound->GetSoundSource()->GetSource(), AL_BUFFER, AL_NONE); 
				CDelete( g_multipleView->m_ambientSound );
				g_menu.m_insertAmbientSound = CFalse;
				m_listBoxEngineObjects.DeleteItem(nSelected);
				GetMenu()->EnableMenuItem( ID_INSERT_SOUND_AMBIENT, MF_ENABLED );
				m_mainBtnAmbientSound.EnableWindow( TRUE );
				g_multipleView->SetElapsedTimeFromBeginning();
				if( m_listBoxEngineObjects.GetItemCount() == 0 )
				{
					m_btnRemoveEngineObject.EnableWindow( FALSE );
					m_btnEngineObjectProperties.EnableWindow( FALSE );
				}
				SortEngineObjectList(nSelected);
				return;
			}
		} //end of if( result == IDYES )
	} // end of if( nSelected >= 0 )
}

CVoid CVandaEngine1Dlg::SortPhysXList()
{
	m_physXElementListIndex--;
}

CVoid CVandaEngine1Dlg::SortSceneList(CInt selectedIndex)
{
	m_sceneListIndex--;
}

CVoid CVandaEngine1Dlg::OnBnClickedBtnRemoveScene()
{
	int nSelected = -1; 
	POSITION p = m_listBoxScenes.GetFirstSelectedItemPosition();
	while(p)
	{
		nSelected = m_listBoxScenes.GetNextSelectedItem(p);
	}
	if( nSelected >= 0 )
	{
		TCHAR szBuffer[1024];
		DWORD cchBuf(1024);
		LVITEM lvi;
		lvi.iItem = nSelected;
		lvi.iSubItem = 0;
		lvi.mask = LVIF_TEXT;
		lvi.pszText = szBuffer;
		lvi.cchTextMax = cchBuf;
		m_listBoxScenes.GetItem(&lvi);
		
		CChar tempString[MAX_NAME_SIZE];
		CInt result;
		if( m_askRemoveScene )
		{
			if (g_editorMode == eMODE_VSCENE)
				sprintf(tempString, "Delete prefab instance '%s' ?", szBuffer);
			else
				sprintf( tempString, "Delete Scene '%s' ?", szBuffer );
		    result = MessageBox( tempString, "Warning", MB_YESNO | MB_ICONERROR );
		}
		else
		{
			result = IDYES;
		}
		CPrefab* dstPrefab = NULL;
		if( result == IDYES )
		{
			SetCapture();
			SetCursor( LoadCursorFromFile( "Assets/Engine/Icons/progress.ani") );

			if (g_editorMode == eMODE_VSCENE)
			{
				//if there are multiple instances of one prefab, do not remove shared images

				for (CUInt i = 0; i < g_instancePrefab.size(); i++)
				{
					CBool foundTarget = CFalse;
					if (Cmp(g_instancePrefab[i]->GetName(), szBuffer))
					{
						if (g_selectedName == g_instancePrefab[i]->GetNameIndex())
						{
							g_selectedName = -1;
							g_lastEngineObjectSelectedName = -1;
							g_showArrow = CFalse;
							SetDialogData3(CFalse);
						}

						dstPrefab = g_instancePrefab[i]->GetPrefab();
						//remove instance from prefab
						for (CUInt j = 0; j < dstPrefab->GetNumInstances(); j++)
						{
							CBool foundTarget = CFalse;
							if (Cmp(dstPrefab->GetInstance(j)->GetName(), g_instancePrefab[i]->GetName()))
							{
								dstPrefab->RemoveInstance(j);
								foundTarget = CTrue;
							}
							if (foundTarget)
								break;
						}
						for (CUInt k = 0; k < 3; k++)
						{
							if (g_instancePrefab[i]->GetScene(k))
							{
								CScene* scene = g_instancePrefab[i]->GetScene(k);
								RemoveSelectedScene(scene->GetName(), scene->GetDocURI());
							}
						}

						//remove it from water as well
						for (CUInt k = 0; k < g_engineWaters.size(); k++)
						{
							for (CUInt l = 0; l < g_engineWaters[k]->m_instancePrefab.size(); l++)
							{
								if (Cmp(g_engineWaters[k]->m_instancePrefab[l]->GetName(), g_instancePrefab[i]->GetName()))
									g_engineWaters[k]->m_instancePrefab.erase(g_engineWaters[k]->m_instancePrefab.begin() + l);
							}
						}

						CDelete(g_instancePrefab[i]);
						g_instancePrefab.erase(g_instancePrefab.begin() + i);

						foundTarget = CTrue;
					}
					if (foundTarget)
						break;
				}

			}

			g_octree->ResetState();
			Cpy(g_shadowProperties.m_directionalLightName, "\n" );

			if (g_editorMode == eMODE_PREFAB)
			{
				RemoveSelectedScene(szBuffer);
			}
			else //vscene
			{
				if (dstPrefab->GetNumInstances() == 0)
				{
					//now remove the prefab
					for (CUInt k = 0; k < g_prefab.size(); k++)
					{
						if (Cmp(dstPrefab->GetName(), g_prefab[k]->GetName()))
						{
							CChar prefabName[MAX_NAME_SIZE];
							sprintf(prefabName, "%s%s%s", "\nPrefab ' ", dstPrefab->GetName(), " ' removed from memory");
							PrintInfo(prefabName, COLOR_YELLOW);

							CDelete(g_prefab[k]);
							g_prefab.erase(g_prefab.begin() + k);
							break;
						}
					}
				}
				g_multipleView->RenderQueries(CTrue);
			}

			m_listBoxScenes.DeleteItem(nSelected);

			//remove the current contents of the object list
			for (int nItem = m_listBoxObjects.GetItemCount() - 1; nItem >= 0; nItem--)
			{
				m_listBoxObjects.DeleteItem(nItem);
			}

			if( m_listBoxScenes.GetItemCount() == 0)
			{
				m_btnRemoveScene.EnableWindow( FALSE );
				m_btnSceneProperties.EnableWindow( FALSE );
			}

			SortSceneList( nSelected );

			g_multipleView->SetElapsedTimeFromBeginning();
			g_multipleView->RenderWindow();

			ReleaseCapture();
		} //if

		if( g_currentCameraType == eCAMERA_DEFAULT_FREE || g_currentCameraType == eCAMERA_COLLADA )
		{
			if( gPhysXscene )
			{
			  // Run collision and dynamics for delta time since the last frame
				gPhysXscene->simulate(1.0f/60.0f/*elapsedTime*/);
				gPhysXscene->flushStream();
				gPhysXscene->fetchResults(NX_ALL_FINISHED, true);
			}
		}

	} //if
}

CVoid CVandaEngine1Dlg::OnBnClickedBtnRemoveObject()
{
	int nSelected = -1; 
	POSITION p = m_listBoxObjects.GetFirstSelectedItemPosition();
	while(p)
	{
		nSelected = m_listBoxObjects.GetNextSelectedItem(p);
	}
	if( nSelected >= 0 )
	{
		TCHAR szBuffer[1024];
		DWORD cchBuf(1024);
		LVITEM lvi;
		lvi.iItem = nSelected;
		lvi.iSubItem = 0;
		lvi.mask = LVIF_TEXT;
		lvi.pszText = szBuffer;
		lvi.cchTextMax = cchBuf;
		m_listBoxObjects.GetItem(&lvi);

		CChar tempString[MAX_NAME_SIZE];
		sprintf( tempString, "Delete Object '%s' ?", szBuffer );
		CInt result = MessageBox( tempString, "Warning", MB_YESNO | MB_ICONERROR );
	} //if
}

HBRUSH CVandaEngine1Dlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	HBRUSH hbr = (HBRUSH)m_brush;// = CDialog::OnCtlColor(pDC, pWnd, nCtlColor);
	switch (nCtlColor)
	{
		case CTLCOLOR_STATIC:
			switch (pWnd->GetDlgCtrlID())
			{
				case IDC_STATIC_SCENES:
				case IDC_STATIC_OBJECTS:
				case IDC_STATIC_PREFABS:
				case IDC_STATIC_PREFAB_ELEMENTS:
				case IDC_STATIC_ENGINE_OBJECTS:
				case IDC_STATIC_PHYSX_ELEMENTS:
				case IDC_STATIC_X:
				case IDC_STATIC_Y:
				case IDC_STATIC_Z:
					pDC->SetTextColor(RGB(140, 140, 140));
					pDC->SetBkColor(RGB(40, 40, 40));
					hbr = (HBRUSH)GetStockObject(NULL_BRUSH);
					break;
			}
			break;
		default:
			hbr = CDialog::OnCtlColor(pDC, pWnd, nCtlColor);
			break;
	}
	return hbr;
}

//To get rid of the Esc key
void CVandaEngine1Dlg::OnCancel()
{
	//CDialog::OnCancel();
}

CVoid CVandaEngine1Dlg::InsertItemToSceneList( char * sceneName )
{
	m_sceneListIndex++;
	int index = m_sceneListIndex;
	LVITEM lvItem;
	lvItem.mask = LVIF_TEXT;
	lvItem.iItem = index;
	lvItem.iSubItem = 0;
	lvItem.pszText = sceneName;
	m_listBoxScenes.InsertItem(&lvItem);

	m_listBoxScenes.SetExtendedStyle( LVS_EX_INFOTIP | LVS_EX_ONECLICKACTIVATE | LVS_EX_CHECKBOXES);
	m_listBoxScenes.SetItemState(index, LVIS_SELECTED, LVIS_SELECTED | LVIS_FOCUSED );
	m_listBoxScenes.SetSelectionMark(index);
	m_listBoxScenes.EnsureVisible(index, FALSE);
	m_listBoxScenes.UpdateWindow();
	if( m_listBoxScenes.GetItemCount() )
	{
		m_btnRemoveScene.EnableWindow( TRUE );
		if (g_editorMode == eMODE_PREFAB)
		{
			for (CUInt i = 0; i < g_scene.size(); i++)
			{
				if (Cmp(g_scene[i]->m_pureFileName, sceneName))
				{
					if (g_scene[i]->m_hasAnimation)
						m_btnSceneProperties.EnableWindow(TRUE);
					//if we are loading the scenes from a vin file, set the check box based on the visible property of the scene
					if (g_scene[i]->m_isVisible)
						m_listBoxScenes.SetCheck(index);
					else
						m_listBoxScenes.SetCheck(index, FALSE);

				}
			}
		}
		else //vscene mode
		{
			for (CUInt i = 0; i < g_instancePrefab.size(); i++)
			{
				if (Cmp(g_instancePrefab[i]->GetName(), sceneName))
				{
					//if we are loading the scenes from a vin file, set the check box based on the visible property of the scene
					if (g_instancePrefab[i]->GetVisible())
						m_listBoxScenes.SetCheck(index);
					else
						m_listBoxScenes.SetCheck(index, FALSE);

				}
			}

		}

	}
	m_objectListIndex = -1;
}

CVoid CVandaEngine1Dlg::InsertItemToObjectList( char * objectName, int imageIndex)
{
	m_objectListIndex++;
	int objectIndex = m_objectListIndex;
	LVITEM lvItem;
	lvItem.mask = LVIF_TEXT | LVIF_IMAGE;
	lvItem.iItem = objectIndex;
	lvItem.iSubItem = 0;
	lvItem.iImage = imageIndex;
	lvItem.pszText = objectName;
	m_listBoxObjects.InsertItem(&lvItem);
	m_listBoxObjects.SetExtendedStyle( LVS_EX_INFOTIP | LVS_EX_ONECLICKACTIVATE | LVS_EX_LABELTIP );
	m_listBoxObjects.SetItemState(objectIndex, LVIS_SELECTED, LVIS_ACTIVATING);
	m_listBoxObjects.SetSelectionMark(objectIndex);
	m_listBoxObjects.EnsureVisible(objectIndex, TRUE);
	m_listBoxObjects.UpdateWindow();
}

CVoid CVandaEngine1Dlg::InserItemToEngineObjectList( char * objectName, int imageIndex )
{
	m_engineObjectListIndex++;
	int engineObjectIndex = m_engineObjectListIndex;
	LVITEM lvItem;
	lvItem.mask = LVIF_TEXT | LVIF_IMAGE;
	lvItem.iItem = engineObjectIndex;
	lvItem.iSubItem = 0;
	lvItem.iImage = imageIndex;
	lvItem.pszText = objectName;
	m_listBoxEngineObjects.InsertItem(&lvItem);
	m_listBoxEngineObjects.SetExtendedStyle( LVS_EX_INFOTIP | LVS_EX_ONECLICKACTIVATE | LVS_EX_LABELTIP );
	m_listBoxEngineObjects.SetItemState(engineObjectIndex, LVIS_SELECTED,LVIS_ACTIVATING);
	m_listBoxEngineObjects.SetSelectionMark(engineObjectIndex);
	m_listBoxEngineObjects.EnsureVisible(engineObjectIndex, FALSE);
	m_listBoxEngineObjects.UpdateWindow();

	//if( m_listBoxEngineObjects.GetItemCount() )
	//{
	//	m_btnRemoveEngineObject.EnableWindow( TRUE );
	//	m_btnEngineObjectProperties.EnableWindow( TRUE );
	//}
}

CVoid CVandaEngine1Dlg::InsertItemToPhysXList( char * objectName, int imageIndex )
{
	m_physXElementListIndex++;
	int physXElementIndex = m_physXElementListIndex;
	LVITEM lvItem;
	lvItem.mask = LVIF_TEXT | LVIF_IMAGE;
	lvItem.iItem = physXElementIndex;
	lvItem.iSubItem = 0;
	lvItem.iImage = imageIndex;
	lvItem.pszText = objectName;
	m_listBoxPhysXElements.InsertItem(&lvItem);
	m_listBoxPhysXElements.SetExtendedStyle( LVS_EX_INFOTIP | LVS_EX_ONECLICKACTIVATE | LVS_EX_LABELTIP );
	m_listBoxPhysXElements.SetItemState(physXElementIndex, LVIS_SELECTED,LVIS_ACTIVATING);
	m_listBoxPhysXElements.SetSelectionMark(physXElementIndex);
	m_listBoxPhysXElements.EnsureVisible(physXElementIndex, FALSE);
	m_listBoxPhysXElements.UpdateWindow();
}

COpenALSoundBuffer *CVandaEngine1Dlg::GetSoundBuffer( const CChar * name )
{
	if (name == NULL) return NULL;
	for(CUInt i=0; i<g_soundBuffers.size(); i++)
	{
		if ( ICmp(GetAfterPath(g_soundBuffers[i]->GetName()), name ) )
			return g_soundBuffers[i];
	}
	return NULL;
}



CVoid CVandaEngine1Dlg::ChangeLightProperties(CInstanceLight* light )
{
	m_dlgAddLight = CNew( CAddLight );
	m_dlgAddLight->SetName( light->m_abstractLight->GetName() );
	m_dlgAddLight->SetPos( light->m_abstractLight->GetPosition() );
	m_dlgAddLight->SetAmbient( light->m_abstractLight->GetAmbient() );
	m_dlgAddLight->SetDiffuse( light->m_abstractLight->GetDiffuse() );
	m_dlgAddLight->SetSpecular( light->m_abstractLight->GetSpecular() );
	m_dlgAddLight->SetType( light->m_abstractLight->GetType() );
	m_dlgAddLight->SetShininess( light->m_abstractLight->GetShininess() );
	m_dlgAddLight->SetConstantAttenuation( light->m_abstractLight->GetConstantAttenuation() );
	m_dlgAddLight->SetLinearAttenuation( light->m_abstractLight->GetLinearAttenuation() );
	m_dlgAddLight->SetQuadAttenuation( light->m_abstractLight->GetQuadraticAttenuation() );
	if( m_dlgAddLight->GetType() == eLIGHTTYPE_SPOT )
	{
		m_dlgAddLight->SetSpotDirection( light->m_abstractLight->GetSpotDirection() );
		m_dlgAddLight->SetSpotExponent( light->m_abstractLight->GetSpotExponent() );
		m_dlgAddLight->SetSpotCuttoff( light->m_abstractLight->GetSpotCutoff() );
	}
	m_dlgAddLight->SetEditMode( CTrue );

	INT_PTR result = m_dlgAddLight->DoModal();
	if ( result  == IDOK )
	{
		if( light->GetIndex() == g_selectedName || light->GetIndex()  == g_lastEngineObjectSelectedName )
		{
			g_showArrow = CFalse;
		}
		m_askRemoveEngineObject = CFalse;
		m_askRemoveScene = CTrue;
		OnBnClickedBtnRemoveEngineObject();
		m_askRemoveEngineObject = CTrue;

		CInstanceLight* instance_light = new CInstanceLight();
		CLight* abstract_light = new CLight();

		instance_light->m_parent = NULL; //Not using COLLADA parent
		instance_light->m_abstractLight = abstract_light;
		abstract_light->SetName( m_dlgAddLight->GetName() );
		abstract_light->SetType( m_dlgAddLight->GetType() );
		switch( m_dlgAddLight->GetType() )
		{
		case eLIGHTTYPE_SPOT:
			instance_light->m_abstractLight->SetSpotCutoff( m_dlgAddLight->GetSpotCuttoff() );
			instance_light->m_abstractLight->SetSpotExponent( m_dlgAddLight->GetSpotExponent() );
			instance_light->m_abstractLight->SetSpotDirection( m_dlgAddLight->GetSpotDirection() );
			break;
		}

		abstract_light->SetAmbient ( m_dlgAddLight->GetAmbientColor() );
		abstract_light->SetDiffuse( m_dlgAddLight->GetDiffuseColor() );
		abstract_light->SetSpecular( m_dlgAddLight->GetSpecularColor() );
		abstract_light->SetPosition( m_dlgAddLight->GetLightPos() );
		abstract_light->SetShininess( m_dlgAddLight->GetShininess() );
		abstract_light->SetConstantAttenuation( m_dlgAddLight->GetConstantAttenuation() );
		abstract_light->SetLinearAttenuation( m_dlgAddLight->GetLinearAttenuation() );
		abstract_light->SetQuadraticAttenuation( m_dlgAddLight->GetQuadAttenuation() );

		instance_light->SetIndex();

		g_engineLights.push_back( instance_light );
		InserItemToEngineObjectList( instance_light->m_abstractLight->GetName() , eENGINEOBJECTLIST_LIGHT);
		if( !Cmp( m_dlgAddLight->GetName(), m_dlgAddLight->GetTempName() ) )
			for( std::vector<std::string>::iterator it = g_engineObjectNames.begin(); it != g_engineObjectNames.end();it++ )
				if( Cmp( m_dlgAddLight->GetTempName(), (*it).c_str() ) )
					(*it) = m_dlgAddLight->GetName();
		CDelete( m_dlgAddLight );
	}
	else if( result == IDCANCEL )
		CDelete( m_dlgAddLight );


}

CVoid CVandaEngine1Dlg::ChangeWaterProperties(CWater* water)
{
	m_dlgAddWater = CNew( CAddWater );
	m_dlgAddWater->SetName( water->GetName() );

	CChar* DuDvName =  water->GetDuDvMapName(); //GetAfterPath( water->GetDuDvMapName() );
	CChar* NormalName = water->GetNormalMapName(); //GetAfterPath( water->GetNormalMapName() );
	//CChar DuDvName[MAX_NAME_SIZE];
	//CChar NormalName[MAX_NAME_SIZE];
	//sprintf( DuDvName, "%s%s", g_pathProperties.m_waterPath, DuDv );
	//sprintf( NormalName, "%s%s", g_pathProperties.m_waterPath, Normal );

	m_dlgAddWater->SetDuDvMapName( DuDvName );
	m_dlgAddWater->SetNormalMapName( NormalName );
	m_dlgAddWater->SetHeight( water->GetHeight() );
	m_dlgAddWater->SetScale( water->GetScale() );
	m_dlgAddWater->SetUV( water->GetUV() );
	m_dlgAddWater->SetLightPos( water->GetLightPos() );
	m_dlgAddWater->SetPos( water->GetPos() );
	m_dlgAddWater->SetSpeed( water->GetSpeed() );
	m_dlgAddWater->SetEditMode( CTrue );

	INT_PTR result = m_dlgAddWater->DoModal();
	if ( result  == IDOK )
	{
		if( water->GetIndex() == g_selectedName || water->GetIndex()  == g_lastEngineObjectSelectedName )
		{
			g_showArrow = CFalse;
		}
		m_askRemoveEngineObject = CFalse;
		OnBnClickedBtnRemoveEngineObject();
		m_askRemoveEngineObject = CTrue;

		CWater* water = new CWater;
		water->SetName( m_dlgAddWater->GetName() );
		water->SetDuDvMap( m_dlgAddWater->GetDuDvMap(), CTrue );
		water->SetNormalMap( m_dlgAddWater->GetNormalMap(), CTrue );
		water->SetPos( m_dlgAddWater->GetPos() );
		water->SetLightPos( m_dlgAddWater->GetLightPos() );
		water->SetHeight( m_dlgAddWater->GetHeight() );
		water->SetScale( m_dlgAddWater->GetScale() );
		water->SetUV( m_dlgAddWater->GetUV() );
		water->SetSpeed( m_dlgAddWater->GetSpeed() );
		water->CreateRenderTexture(g_waterTextureSize, 3, GL_RGB, WATER_REFLECTION_ID );
		water->CreateRenderTexture(g_waterTextureSize, 3, GL_RGB, WATER_REFRACTION_ID );
		water->CreateRenderTexture(g_waterTextureSize, 1, GL_DEPTH_COMPONENT, WATER_DEPTH_ID );
		water->SetSideVertexPositions();
		//save functions/////////////////////////////////
		for( CUInt index = 0; index < g_VSceneNamesOfCurrentProject.size(); index++ )
		{
			water->AddVSceneToList( g_VSceneNamesOfCurrentProject[index], CTrue ); //Write to zip file and copy the textures
		}
		//save functions/////////////////////////////////

		g_engineWaters.push_back( water );
		InserItemToEngineObjectList( water->GetName() , eENGINEOBJECTLIST_WATER);
		if( !Cmp( m_dlgAddWater->GetName(), m_dlgAddWater->GetTempName() ) )
			for( std::vector<std::string>::iterator it = g_engineObjectNames.begin(); it != g_engineObjectNames.end();it++ )
				if( Cmp( m_dlgAddWater->GetTempName(), (*it).c_str() ) )
					(*it) = m_dlgAddWater->GetName();
		CDelete( m_dlgAddWater );
	}
	else if( result == IDCANCEL )
		CDelete( m_dlgAddWater );
}

CVoid CVandaEngine1Dlg::ChangeStaticSoundProperties(CStaticSound* sound)
{
	m_dlgAddStaticSound = CNew( CAddStaticSound );
	m_dlgAddStaticSound->SetSoundPos( sound->GetPosition() );
	m_dlgAddStaticSound->SetName( sound->GetName() );

	CChar StaticSoundPath[MAX_NAME_SIZE];
	CChar* StaticSoundName = GetAfterPath( sound->GetPath() );
	sprintf( StaticSoundPath, "%s", sound->GetPath() );

	m_dlgAddStaticSound->SetPath( StaticSoundPath );
	m_dlgAddStaticSound->SetPitch( sound->GetPitch() );
	m_dlgAddStaticSound->SetVolume( sound->GetVolume() );
	m_dlgAddStaticSound->SetRolloff( sound->GetRolloff() );
	m_dlgAddStaticSound->SetMaxDistance( sound->GetMaxDistance() );
	m_dlgAddStaticSound->SetReferenceDistance( sound->GetRefrenceDistance() );
	m_dlgAddStaticSound->SetPlay( sound->GetPlay() );
	m_dlgAddStaticSound->SetLoop( sound->GetLoop() );
	m_dlgAddStaticSound->SetEditMode( CTrue );
	//m_dlgAddStaticSound->SetIndex( sound->GetIndex() );

	INT_PTR result = m_dlgAddStaticSound->DoModal();
	if ( result  == IDOK )
	{
		if( sound->GetIndex() == g_selectedName || sound->GetIndex()  == g_lastEngineObjectSelectedName )
		{
			g_showArrow = CFalse;
		}
		m_askRemoveEngineObject = CFalse;
		OnBnClickedBtnRemoveEngineObject();
		m_askRemoveEngineObject = CTrue;

		COpenALSoundSource* m_staticSoundSource = CNew( COpenALSoundSource );
		CStaticSound* m_staticSound = CNew( CStaticSound );

		CChar temp[ MAX_NAME_SIZE];
		//if( m_dlgAddStaticSound->m_strStaticSoundPureDataPath.IsEmpty() )
			Cpy( temp, m_dlgAddStaticSound->GetPath() );
		//else
		//	sprintf( temp, "%s%s.ogg", g_pathProperties.m_soundPath, m_dlgAddStaticSound->m_strStaticSoundPureDataPath );

		COpenALSoundBuffer* m_staticSoundBuffer = GetSoundBuffer( GetAfterPath(temp) );
		if( m_staticSoundBuffer == NULL || (m_staticSoundBuffer && !m_staticSoundBuffer->m_loaded ) )
		{
			if( m_staticSoundBuffer == NULL )
			{
				m_staticSoundBuffer = CNew( COpenALSoundBuffer );
				g_soundBuffers.push_back( m_staticSoundBuffer );
			}
			else 
			{
				CChar tempBuffer[MAX_NAME_SIZE];
				sprintf( tempBuffer, "\nTrying to reload '%s%s", GetAfterPath(m_staticSoundBuffer->GetName() ), "'" );
				PrintInfo( tempBuffer, COLOR_YELLOW );
			}
			if( !m_staticSoundBuffer->LoadOggVorbisFromFile( temp ) )
			{
				CChar buffer[MAX_NAME_SIZE];
				sprintf( buffer, "\n%s%s%s", "Couldn't load the file '", temp, "'" );
				PrintInfo( buffer, COLOR_RED );
				m_staticSoundBuffer->m_loaded = CFalse;

			}
			else
			{
				CChar buffer[MAX_NAME_SIZE];
				sprintf( buffer, "\n%s%s%s", "ogg file '", temp, "' loaded successfully." );
				PrintInfo( buffer );
				m_staticSoundBuffer->m_loaded = CTrue;
			}
			m_staticSoundBuffer->SetName( temp );	
		}
		else
		{
				CChar temp[MAX_NAME_SIZE]; 
				sprintf( temp, "\n%s%s%s", "sound buffer '", GetAfterPath(m_staticSoundBuffer->GetName()), "' already exists." );
				PrintInfo( temp, COLOR_YELLOW );
		}

		m_staticSoundSource->BindSoundBuffer (*m_staticSoundBuffer);
		m_staticSoundSource->SetLooping( m_dlgAddStaticSound->GetLoopCondition() );
		m_staticSoundSource->SetPitch( m_dlgAddStaticSound->GetPitch() );
		m_staticSoundSource->SetReferenceDistance( m_dlgAddStaticSound->GetReferenceDistance() );
		m_staticSoundSource->SetMaxDistance( m_dlgAddStaticSound->GetMaxDistance() );
		m_staticSoundSource->SetRolloff( m_dlgAddStaticSound->GetRolloff() );
		m_staticSoundSource->SetSoundPosition( m_dlgAddStaticSound->GetSoundPos() );

		m_staticSound->SetName( m_dlgAddStaticSound->GetName() );
		m_staticSound->SetPath( temp );
		m_staticSound->SetPosition( m_dlgAddStaticSound->GetSoundPos() );
		m_staticSound->SetLoop( m_dlgAddStaticSound->GetLoopCondition() );
		m_staticSound->SetMaxDistance( m_dlgAddStaticSound->GetMaxDistance() );
		m_staticSound->SetPitch( m_dlgAddStaticSound->GetPitch() );
		m_staticSound->SetPlay( m_dlgAddStaticSound->GetPlayCondition() );
		m_staticSound->SetRefrenceDistance( m_dlgAddStaticSound->GetReferenceDistance() );
		m_staticSound->SetRolloff( m_dlgAddStaticSound->GetRolloff() );
		//m_staticSound->SetIndex( m_dlgAddStaticSound->GetIndex() );

		m_staticSound->SetSoundSource( m_staticSoundSource );
		m_staticSound->SetSoundBuffer( m_staticSoundBuffer );

		if( m_dlgAddStaticSound->GetPlayCondition() )
		{
			g_multipleView->m_soundSystem->PlayALSound( *m_staticSoundSource );
		}
		//save functions/////////////////////////////////
		for( CUInt index = 0; index < g_VSceneNamesOfCurrentProject.size(); index++ )
		{
			m_staticSound->AddVSceneToList( g_VSceneNamesOfCurrentProject[index], CTrue ); //Write to zip file and copy the textures
		}
		//save functions/////////////////////////////////

		g_engineStaticSounds.push_back( m_staticSound );
		InserItemToEngineObjectList( m_staticSound->GetName() , eENGINEOBJECTLIST_STATICSOUND);

		if( !Cmp( m_dlgAddStaticSound->GetName(), m_dlgAddStaticSound->GetTempName() ) )
			for( std::vector<std::string>::iterator it = g_engineObjectNames.begin(); it != g_engineObjectNames.end();it++ )
				if( Cmp( m_dlgAddStaticSound->GetTempName(), (*it).c_str() ) )
					(*it) = m_dlgAddStaticSound->GetName();

		CDelete( m_dlgAddStaticSound );
	}
	else if( result == IDCANCEL )
		CDelete( m_dlgAddStaticSound );
}

CVoid CVandaEngine1Dlg::ChangeAmbientSoundProperties()
{
	m_dlgAddAmbientSound = CNew( CAddAmbientSound );
	m_dlgAddAmbientSound->SetName( g_multipleView->m_ambientSound->GetName() );

	CChar AmbientSoundPath[MAX_NAME_SIZE];
	CChar* AmbientSoundName = GetAfterPath( g_multipleView->m_ambientSound->GetPath() );
	sprintf( AmbientSoundPath, "%s", g_multipleView->m_ambientSound->GetPath() );

	m_dlgAddAmbientSound->SetPath( AmbientSoundPath );
	m_dlgAddAmbientSound->SetVolume( g_multipleView->m_ambientSound->GetVolume() );
	m_dlgAddAmbientSound->SetPitch( g_multipleView->m_ambientSound->GetPitch() );
	m_dlgAddAmbientSound->SetEditMode( CTrue );

	INT_PTR result = m_dlgAddAmbientSound->DoModal();
	if ( result  == IDOK )
	{
		m_askRemoveEngineObject = CFalse;
		OnBnClickedBtnRemoveEngineObject();
		m_askRemoveEngineObject = CTrue;

		COpenALSoundSource* m_ambientSoundSource = CNew( COpenALSoundSource );
		COpenALSoundBuffer* m_ambientSoundBuffer = CNew( COpenALSoundBuffer );

		CChar temp[ MAX_NAME_SIZE];
		//if( m_dlgAddAmbientSound->m_strPureAmbientSoundBuffer.IsEmpty() )
			Cpy( temp, m_dlgAddAmbientSound->GetPath() );
		//else
		//	sprintf( temp, "%s%s.ogg", g_pathProperties.m_soundPath, m_dlgAddAmbientSound->m_strPureAmbientSoundBuffer );

		if( !m_ambientSoundBuffer->LoadOggVorbisFromFile( temp ) )
			return;
		m_ambientSoundSource->BindSoundBuffer( *(m_ambientSoundBuffer) );

		m_ambientSoundSource->SetLooping( true );
		m_ambientSoundSource->SetPitch( m_dlgAddAmbientSound->GetPitch() );
		m_ambientSoundSource->SetVolume( m_dlgAddAmbientSound->GetVolume() );

		g_multipleView->m_ambientSound = CNew( CAmbientSound );
		g_multipleView->m_ambientSound->SetSoundSource( m_ambientSoundSource );
		g_multipleView->m_ambientSound->SetSoundBuffer( m_ambientSoundBuffer );
		g_multipleView->m_ambientSound->SetName( m_dlgAddAmbientSound->GetName() );
		g_multipleView->m_ambientSound->SetPath( temp );
		g_multipleView->m_ambientSound->SetVolume( m_dlgAddAmbientSound->GetVolume() );
		g_multipleView->m_ambientSound->SetPitch( m_dlgAddAmbientSound->GetPitch() );


		g_multipleView->m_soundSystem->PlayALSound( *m_ambientSoundSource );
		//save functions/////////////////////////////////
		for( CUInt index = 0; index < g_VSceneNamesOfCurrentProject.size(); index++ )
		{
			g_multipleView->m_ambientSound->AddVSceneToList( g_VSceneNamesOfCurrentProject[index], CTrue ); //Write to zip file and copy the textures
		}
		//save functions/////////////////////////////////

		PrintInfo( "\nAmbient sound '" );
		sprintf( temp, "%s", m_dlgAddAmbientSound->m_strAmbientSoundBuffer );
		PrintInfo( temp, COLOR_RED_GREEN );
		PrintInfo( "' loaded successfully" );
		
		g_menu.m_playAmbientSound = CTrue;
		GetMenu()->EnableMenuItem( ID_INSERT_SOUND_AMBIENT, MF_DISABLED | MF_GRAYED );
		m_mainBtnAmbientSound.EnableWindow( FALSE );

		g_menu.m_insertAmbientSound = CTrue;
		InserItemToEngineObjectList( g_multipleView->m_ambientSound->GetName() , eENGINEOBJECTLIST_AMBIENTSOUND);

		if( !Cmp( m_dlgAddAmbientSound->GetName(), m_dlgAddAmbientSound->GetTempName() ) )
			for( std::vector<std::string>::iterator it = g_engineObjectNames.begin(); it != g_engineObjectNames.end();it++ )
				if( Cmp( m_dlgAddAmbientSound->GetTempName(), (*it).c_str() ) )
					(*it) = m_dlgAddAmbientSound->GetName();

		CDelete( m_dlgAddAmbientSound );
	}
	else if( result == IDCANCEL )
		CDelete( m_dlgAddAmbientSound );
}

CVoid CVandaEngine1Dlg::ChangeSkyDomeProperties()
{
	m_dlgAddSkyDome = CNew( CAddSkyDome );

	m_dlgAddSkyDome->SetPos( g_skyDome->GetPosition() );
	m_dlgAddSkyDome->SetName( g_skyDome->GetName() );
	m_dlgAddSkyDome->SetPath( g_skyDome->GetPath() );
	m_dlgAddSkyDome->SetRadius( g_skyDome->GetRadius() );
	m_dlgAddSkyDome->SetDampening( g_skyDome->GetDampening() );
	m_dlgAddSkyDome->SetSides( g_skyDome->GetSides() );
	m_dlgAddSkyDome->SetSlices( g_skyDome->GetSlices() );
	m_dlgAddSkyDome->SetExponential( g_skyDome->GetExponential() );
	m_dlgAddSkyDome->SetEditMode( CTrue );

	INT_PTR result = m_dlgAddSkyDome->DoModal();
	if ( result == IDOK )
	{
		if( g_skyDome->GetIndex() == g_selectedName || g_skyDome->GetIndex()  == g_lastEngineObjectSelectedName )
		{
			g_showArrow = CFalse;
		}
		m_askRemoveEngineObject = CFalse;
		OnBnClickedBtnRemoveEngineObject();
		m_askRemoveEngineObject = CTrue;
				
		CDelete( g_skyDome );
		g_skyDome = CNew( CSkyDome );
		CChar temp[ MAX_NAME_SIZE];
		//if( m_dlgAddSkyDome->m_strSkyDomePurePath.IsEmpty() )
			Cpy( temp, m_dlgAddSkyDome->GetPath() );
		//else
		//	sprintf( temp, "%s%s.dds", g_pathProperties.m_skyPath, m_dlgAddSkyDome->GetPurePath() );
		g_skyDome = CNew( CSkyDome );
		g_skyDome->SetName( m_dlgAddSkyDome->GetName() );
		g_skyDome->SetPath( temp );
		g_skyDome->SetRadius( m_dlgAddSkyDome->GetRadius() );
		g_skyDome->SetPosition( m_dlgAddSkyDome->GetPos() );
		g_skyDome->SetDampening( m_dlgAddSkyDome->GetDampening() );
		g_skyDome->SetSides( m_dlgAddSkyDome->GetSides() );
		g_skyDome->SetSlices( m_dlgAddSkyDome->GetSlices() );
		g_skyDome->SetExponential( m_dlgAddSkyDome->GetExponential() );

		g_skyDome->Initialize();
		g_menu.m_insertAndShowSky = CTrue;
		InserItemToEngineObjectList( g_skyDome->GetName(), eENGINEOBJECTLIST_SKY );

		if( !Cmp( m_dlgAddSkyDome->GetName(), m_dlgAddSkyDome->GetTempName() ) )
			for( std::vector<std::string>::iterator it = g_engineObjectNames.begin(); it != g_engineObjectNames.end();it++ )
				if( Cmp( m_dlgAddSkyDome->GetTempName(), (*it).c_str() ) )
					(*it) = m_dlgAddSkyDome->GetName();

		GetMenu()->EnableMenuItem( ID_INSERT_SKYDOME, MF_DISABLED | MF_GRAYED );
		CDelete( m_dlgAddSkyDome );
		m_mainBtnSky.EnableWindow( FALSE );

		//save functions/////////////////////////////////
		for( CUInt index = 0; index < g_VSceneNamesOfCurrentProject.size(); index++ )
		{
			g_skyDome->AddVSceneToList( g_VSceneNamesOfCurrentProject[index], CTrue ); //Write to zip file and copy the textures
		}
		//save functions/////////////////////////////////


	}
	else if( result == IDCANCEL )
		CDelete( m_dlgAddSkyDome );

}

void CVandaEngine1Dlg::OnClose()
{
	//configuration
	CChar ConfigPath[MAX_NAME_SIZE];
	HRESULT result = SHGetFolderPath(NULL, CSIDL_PERSONAL, NULL, SHGFP_TYPE_CURRENT, ConfigPath);
	if (result != S_OK)
	{
        PrintInfo( "\nCouldn't get the documents folder to write data", COLOR_RED );
	}
	else
	{
		Append( ConfigPath, "/Vanda/conf_editor.dat" );
	}

	FILE *filePtr;
	filePtr = fopen( ConfigPath, "wb" );
	fwrite( &g_options, sizeof( COptions ), 1, filePtr  );
	fclose(filePtr);

	////////////////
	//PackagePath path
	CChar PackageFilePath[MAX_NAME_SIZE];
	HRESULT doc_result_package = SHGetFolderPath(NULL, CSIDL_PERSONAL, NULL, SHGFP_TYPE_CURRENT, PackageFilePath);
	if (doc_result_package != S_OK)
	{
		PrintInfo("\nCouldn't get the documents folder to write data", COLOR_RED);
	}
	else
	{
		Append(PackageFilePath, "/Vanda/Packages/package_editor.dat");
	}

	CChar PackagePath[MAX_NAME_SIZE];
	SHGetFolderPath(NULL, CSIDL_PERSONAL, NULL, SHGFP_TYPE_CURRENT, PackagePath);
	Append(PackagePath, "/Vanda/Packages/PKG/");

	filePtr = fopen(PackageFilePath, "wb");
	CUInt packageSize = g_prefabPackagesAndNames.size();
	fwrite(&packageSize, sizeof(CUInt), 1, filePtr);
	fclose(filePtr);

	for (CUInt i = 0; i < g_prefabPackagesAndNames.size(); i++)
	{
		CChar packagePath[MAX_NAME_SIZE];
		sprintf(packagePath, "%s%s%s", PackagePath, g_prefabPackagesAndNames[i][0].c_str(), ".pkg");
		filePtr = fopen(packagePath, "wb");
		CUInt row_size = g_prefabPackagesAndNames[i].size();
		fwrite(&row_size, sizeof(CUInt), 1, filePtr);
		for (CUInt j = 0; j < row_size; j++)
		{
			CChar name[MAX_NAME_SIZE];
			Cpy(name, g_prefabPackagesAndNames[i][j].c_str());
			fwrite(&name, sizeof(CChar), MAX_NAME_SIZE, filePtr);
		}
		fclose(filePtr);
	}
	///////////////////

	if( g_scene.size() > 0 || g_engineLights.size() > 0 || g_engineWaters.size() > 0 || g_menu.m_insertAndShowSky || g_menu.m_insertAmbientSound || g_engineStaticSounds.size() > 0 )
	{
		CInt iResponse;
		iResponse = MessageBox( "Save scene?", "Warning" , MB_YESNOCANCEL |MB_ICONSTOP);
		if( iResponse == IDYES )
		{
			if( g_multipleView->m_enableTimer )
				g_multipleView->EnableTimer( CFalse );
			if (g_editorMode == eMODE_PREFAB)
				OnMenuClickedSavePrefabAs();
			else
				OnMenuClickedSaveAs();

			OnMenuClickedNew(CFalse);
			if (g_multipleView->m_enableTimer)
				g_multipleView->EnableTimer(CTrue);

			PostQuitMessage( 0 );
		}
		else if (iResponse == IDNO)
		{
			OnMenuClickedNew(CFalse);
			PostQuitMessage(0);
		}
	}
	else
	{
		OnMenuClickedNew(CFalse);
		PostQuitMessage(0);
	}
	CDialog::OnClose();
}

//void CVandaEngine1Dlg::OnNMReturnListScenes(NMHDR *pNMHDR, LRESULT *pResult)
//{
//	// TODO: Add your control notification handler code here
//	*pResult = 0;
//}

BOOL CVandaEngine1Dlg::PreTranslateMessage(MSG* pMsg)
{
    if (pMsg->message == WM_KEYDOWN)
    {
        if ((pMsg->wParam == VK_RETURN) || (pMsg->wParam == VK_ESCAPE))
            pMsg->wParam = VK_TAB;
    }
	if (NULL != m_pToolTip)
            m_pToolTip->RelayEvent(pMsg);
    return CDialog::PreTranslateMessage(pMsg);
}
void CVandaEngine1Dlg::OnBnClickedBtnNew()
{
	if( g_multipleView->m_enableTimer )
		g_multipleView->EnableTimer( CFalse );
	OnMenuClickedNew( CTrue ); //ask to see if we should proceed?
	if( g_multipleView->m_enableTimer )
		g_multipleView->EnableTimer( CTrue );

	g_multipleView->SetElapsedTimeFromBeginning();
	g_multipleView->RenderWindow();
}

void CVandaEngine1Dlg::OnBnClickedBtnSave()
{
	if( g_multipleView->m_enableTimer )
		g_multipleView->EnableTimer( CFalse );

	if (g_editorMode == eMODE_PREFAB)
		OnMenuClickedSavePrefabAs(CFalse);
	else
		OnMenuClickedSaveAs(CFalse);

	if( g_multipleView->m_enableTimer )
		g_multipleView->EnableTimer( CTrue );

	g_multipleView->SetElapsedTimeFromBeginning();
}


void CVandaEngine1Dlg::OnBnClickedBtnSaveas()
{
	if( g_multipleView->m_enableTimer )
		g_multipleView->EnableTimer( CFalse );
	if (g_editorMode == eMODE_PREFAB)
		OnMenuClickedSavePrefabAs();
	else
		OnMenuClickedSaveAs();
	if (g_multipleView->m_enableTimer)
		g_multipleView->EnableTimer( CTrue );

	g_multipleView->SetElapsedTimeFromBeginning();
}

void CVandaEngine1Dlg::OnBnClickedBtnOpen()
{
	if( g_multipleView->m_enableTimer )
		g_multipleView->EnableTimer( CFalse );
	if (g_editorMode == eMODE_PREFAB)
		OnMenuClickedOpenPrefab();
	else
		OnMenuClickedOpenVScene();
	if( g_multipleView->m_enableTimer )
		g_multipleView->EnableTimer( CTrue );

	g_multipleView->SetElapsedTimeFromBeginning();
}

void CVandaEngine1Dlg::OnBnClickedBtnCollada()
{
	if( g_multipleView->m_enableTimer )
		g_multipleView->EnableTimer( CFalse );
	OnMenuClickedImportCollada();
	if( g_multipleView->m_enableTimer )
		g_multipleView->EnableTimer( CTrue );

	g_multipleView->SetElapsedTimeFromBeginning();
	g_multipleView->RenderWindow();
}
//removed in version 1.4 or later
//void CVandaEngine1Dlg::OnBnClickedBtnPhysx()
//{
//	if( g_multipleView->m_enableTimer )
//		g_multipleView->EnableTimer( CFalse );
//	OnMenuClickedImportPhysX();
//	if( g_multipleView->m_enableTimer )
//		g_multipleView->EnableTimer( CTrue );
//
//	g_multipleView->SetElapsedTimeFromBeginning();
//	g_multipleView->RenderWindow();
//}

void CVandaEngine1Dlg::OnBnClickedBtnAnimPrev()
{
	if( g_multipleView->m_enableTimer )
		g_multipleView->EnableTimer( CFalse );

	g_render.GetSelectedScene()->SetPrevAnimation();

	if( g_multipleView->m_enableTimer )
		g_multipleView->EnableTimer( CTrue );

	g_multipleView->SetElapsedTimeFromBeginning();
	g_multipleView->RenderWindow();
}

void CVandaEngine1Dlg::OnBnClickedBtnAnimNext()
{
	if( g_multipleView->m_enableTimer )
		g_multipleView->EnableTimer( CFalse );

	g_render.GetSelectedScene()->SetNextAnimation();
	if( g_multipleView->m_enableTimer )
		g_multipleView->EnableTimer( CTrue );

	g_multipleView->SetElapsedTimeFromBeginning();
	g_multipleView->RenderWindow();
}

void CVandaEngine1Dlg::OnBnClickedBtnAnimPlay()
{
	if( g_multipleView->m_enableTimer )
		g_multipleView->EnableTimer( CFalse );

	g_render.GetSelectedScene()->m_animationStatus = eANIM_PLAY; 
	if( !g_render.GetSelectedScene()->UpdateAnimationLists() && g_render.GetSelectedScene()->GetNumClips() == 1)
	{
		g_render.GetSelectedScene()->SetClipIndex(0);
	}
	PrintInfo( "\nAnimation '" + (CString) g_render.GetSelectedScene()->m_animationClips[g_render.GetSelectedScene()->GetCurrentClipIndex()]->GetName() + "'activated" );
	g_render.GetSelectedScene()->BlendCycle(g_render.GetSelectedScene()->GetCurrentClipIndex(), 1.0f, 0.0f);
	ex_pBtnPlayAnim->EnableWindow( FALSE );
	ex_pBtnPlayAnim->ShowWindow( SW_HIDE );
	ex_pBtnPauseAnim->EnableWindow( TRUE );
	ex_pBtnPauseAnim->ShowWindow( SW_SHOW );

	if( g_multipleView->m_enableTimer )
		g_multipleView->EnableTimer( CTrue );

}

BOOL CVandaEngine1Dlg::OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message)
{
	//::SetCursor( LoadCursor( AfxGetInstanceHandle(), MAKEINTRESOURCE( IDC_DEFAULT_ARROW ) ) );
 //   return TRUE;
	return CDialog::OnSetCursor(pWnd, nHitTest, message);
}

void CVandaEngine1Dlg::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: Add your message handler code here and/or call default
	CDialog::OnTimer(nIDEvent);
}

void CVandaEngine1Dlg::OnBnClickedBtnTimer()
{
	g_multipleView->m_enableTimer = CFalse;
	g_multipleView->EnableTimer( g_multipleView->m_enableTimer );
	ex_pBtnTimerPause->EnableWindow(TRUE);
	ex_pBtnTimerPause->ShowWindow(SW_SHOW);
	ex_pBtnTimerPlay->EnableWindow(FALSE);
	ex_pBtnTimerPlay->ShowWindow(SW_HIDE);

	PrintInfo( "\nTimer Disabled" );

	g_multipleView->SetElapsedTimeFromBeginning();

}

void CVandaEngine1Dlg::OnBnClickedBtnAnimPause()
{
	g_render.GetSelectedScene()->m_animationStatus = eANIM_PAUSE;
	PrintInfo("\nAnimation '" + (CString)g_render.GetSelectedScene()->m_animationClips[g_render.GetSelectedScene()->GetCurrentClipIndex()]->GetName() + "'paused");

	ex_pBtnPlayAnim->EnableWindow( TRUE );
	ex_pBtnPlayAnim->ShowWindow( SW_SHOW );
	ex_pBtnPauseAnim->EnableWindow( FALSE );
	ex_pBtnPauseAnim->ShowWindow( SW_HIDE );
}

void CVandaEngine1Dlg::OnBnClickedBtnWeb()
{
	ShellExecute(NULL, "open", "http://www.vandaengine.org", NULL, NULL, SW_SHOWNORMAL);
	g_multipleView->SetElapsedTimeFromBeginning();
}

void CVandaEngine1Dlg::OnBnClickedBtnFacebook()
{
	ShellExecute(NULL, "open", "https://www.facebook.com/vandaengine", NULL, NULL, SW_SHOWNORMAL);
	g_multipleView->SetElapsedTimeFromBeginning();
}


void CVandaEngine1Dlg::OnBnClickedBtnMaterial()
{
	if( g_multipleView->m_enableTimer )
		g_multipleView->EnableTimer( CFalse );
	OnMenuClickedEditMaterial();
	if( g_multipleView->m_enableTimer )
		g_multipleView->EnableTimer( CTrue );
	g_multipleView->SetElapsedTimeFromBeginning();
	g_multipleView->RenderWindow();

}

void CVandaEngine1Dlg::OnBnClickedBtnLight()
{
	if( g_multipleView->m_enableTimer )
		g_multipleView->EnableTimer( CFalse );
	OnMenuClickedInsertLight();
	if( g_multipleView->m_enableTimer )
		g_multipleView->EnableTimer( CTrue );

	g_multipleView->SetElapsedTimeFromBeginning();
	g_multipleView->RenderWindow();
}

void CVandaEngine1Dlg::OnBnClickedBtnWater()
{
	if( g_multipleView->m_enableTimer )
		g_multipleView->EnableTimer( CFalse );
	OnMenuClickedInsertWater();
	if( g_multipleView->m_enableTimer )
		g_multipleView->EnableTimer( CTrue );

	g_multipleView->SetElapsedTimeFromBeginning();
	g_multipleView->RenderWindow();
}

void CVandaEngine1Dlg::OnBnClickedBtnAmbientsound()
{
	if( g_multipleView->m_enableTimer )
		g_multipleView->EnableTimer( CFalse );
	OnMenuClickedInsertAmbientSound();
	if( g_multipleView->m_enableTimer )
		g_multipleView->EnableTimer( CTrue );

	g_multipleView->SetElapsedTimeFromBeginning();
}

void CVandaEngine1Dlg::OnBnClickedBtnStaticsound()
{
	if( g_multipleView->m_enableTimer )
		g_multipleView->EnableTimer( CFalse );
	OnMenuClickedInsertStaticSound();
	if( g_multipleView->m_enableTimer )
		g_multipleView->EnableTimer( CTrue );

	g_multipleView->SetElapsedTimeFromBeginning();
}

void CVandaEngine1Dlg::OnBnClickedBtnSky()
{
	if( g_multipleView->m_enableTimer )
		g_multipleView->EnableTimer( CFalse );
	OnMenuClickedInsertSkyDome();
	if( g_multipleView->m_enableTimer )
		g_multipleView->EnableTimer( CTrue );

	g_multipleView->SetElapsedTimeFromBeginning();
	g_multipleView->RenderWindow();
}

void CVandaEngine1Dlg::OnBnClickedBtnPlayer()
{
	CBool foundTarget = CFalse;
	for (CUInt i = 0; i < g_instancePrefab.size(); i++)
	{
		if (Cmp(g_instancePrefab[i]->GetPrefab()->GetPackageName(), "Vanda_Basics") && Cmp(g_instancePrefab[i]->GetPrefab()->GetPrefabName(), "Character"))
		{
			foundTarget = CTrue;
			break;
		}
	}
	if (foundTarget)
	{
		MessageBox("You have already inserted one player in your scene", "Vanda Engine Error", MB_OK | MB_ICONINFORMATION);
		return;
	}

	CChar packageName[MAX_NAME_SIZE];
	CChar prefabName[MAX_NAME_SIZE];

	Cpy(packageName, "Vanda_Basics");

	Cpy(prefabName, "Character");


	g_importPrefab = CTrue;
	if (g_multipleView->m_enableTimer)
		g_multipleView->EnableTimer(CFalse);
	CBool PhysXCam = CFalse;

	g_arrowPosition.x = g_arrowPosition.y = g_arrowPosition.z = 0.0f;
	g_arrowRotate.x = g_arrowRotate.y = g_arrowRotate.z = 0.0f;
	g_arrowScale.x = g_arrowScale.y = g_arrowScale.z = 1.0f;

	ex_pVandaEngine1Dlg->m_dlgPrefabs = CNew(CPrefabDlg);
	ex_pDlgPrefabs = ex_pVandaEngine1Dlg->m_dlgPrefabs;
	ex_pVandaEngine1Dlg->OnMenuClickedInsertPrefab(NULL, packageName, prefabName);
	CDelete(ex_pVandaEngine1Dlg->m_dlgPrefabs);
	if (g_multipleView->m_enableTimer)
		g_multipleView->EnableTimer(CTrue);
	g_importPrefab = CFalse;
	if (g_editorMode == eMODE_VSCENE)
		g_multipleView->RenderQueries(CTrue);

	g_multipleView->SetElapsedTimeFromBeginning();
}

void CVandaEngine1Dlg::OnBnClickedBtnConsole()
{
	if( g_multipleView->m_enableTimer )
		g_multipleView->EnableTimer( CFalse );

	m_dlgConsole = CNew( CConsole );
	m_dlgConsole->DoModal();
	CDelete( m_dlgConsole );

	if( g_multipleView->m_enableTimer )
		g_multipleView->EnableTimer( CTrue );

	g_multipleView->SetElapsedTimeFromBeginning();
	g_multipleView->RenderWindow();

}

void CVandaEngine1Dlg::OnLvnItemchangedListScenes(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMLISTVIEW pNMListView = reinterpret_cast<LPNMLISTVIEW>(pNMHDR);

	if (pNMListView->uOldState == 0 && pNMListView->uNewState == 0)
		return;	// No change
 
	BOOL bPrevState = (BOOL)(((pNMListView->uOldState &
	LVIS_STATEIMAGEMASK)>>12)-1); // Old check box state
	if (bPrevState < 0)	// On startup there's no previous state
	bPrevState = 0; // so assign as false (unchecked)
 
	// New check box state
	BOOL bChecked=(BOOL)(((pNMListView->uNewState & LVIS_STATEIMAGEMASK)>>12)-1);
	if (bChecked < 0) // On non-checkbox notifications assume false
	bChecked = 0;
 
	if (bPrevState != bChecked) // change in check box
	{
		for(CInt nItem =0 ; nItem <  m_listBoxScenes.GetItemCount(); nItem++)
		{
			BOOL bChecked = m_listBoxScenes.GetCheck(nItem);
			CString strText = m_listBoxScenes.GetItemText(nItem, 0);
			char charPtr[MAX_NAME_SIZE];
			sprintf(charPtr, "%s", strText);

			if (g_editorMode == eMODE_PREFAB)
			{
				if (bChecked == 0)
				{
					for (CUInt i = 0; i < g_scene.size(); i++)
					{
						if (Cmp(g_scene[i]->m_pureFileName, charPtr))
						{
							g_scene[i]->m_isVisible = CFalse;
						}
					}
				}
				else
				{
					for (CUInt i = 0; i < g_scene.size(); i++)
					{
						if (Cmp(g_scene[i]->m_pureFileName, charPtr))
						{
							g_scene[i]->m_isVisible = CTrue;
						}
					}
				}
			}
			else
			{
				if (bChecked == 0)
				{
					for (CUInt i = 0; i < g_instancePrefab.size(); i++)
					{
						if (Cmp(g_instancePrefab[i]->GetName(), charPtr))
						{
							g_instancePrefab[i]->SetVisible( CFalse );
						}
					}
				}
				else
				{
					for (CUInt i = 0; i < g_instancePrefab.size(); i++)
					{
						if (Cmp(g_instancePrefab[i]->GetName(), charPtr))
						{
							g_instancePrefab[i]->SetVisible(CTrue);
						}
					}
				}
			}
		}
		g_multipleView->SetElapsedTimeFromBeginning();
		g_multipleView->RenderWindow();

		return;
	}

	//remove the current contents of the object list
	for (int nItem = m_listBoxObjects.GetItemCount()-1; nItem >= 0 ;nItem-- )
	{
		m_listBoxObjects.DeleteItem(nItem);
	}

	int nSelected = -1; 
	POSITION p = m_listBoxScenes.GetFirstSelectedItemPosition();
	while(p)
	{
		nSelected = m_listBoxScenes.GetNextSelectedItem(p);
	}
	if( nSelected >= 0 )
	{
		m_btnRemoveScene.EnableWindow( TRUE );
		TCHAR szBuffer[1024];
		DWORD cchBuf(1024);
		LVITEM lvi;
		lvi.iItem = nSelected;
		lvi.iSubItem = 0;
		lvi.mask = LVIF_TEXT;
		lvi.pszText = szBuffer;
		lvi.cchTextMax = cchBuf;
		m_listBoxScenes.GetItem(&lvi);
		
		m_objectListIndex = -1;

		if (g_editorMode == eMODE_PREFAB)
		{
			for (CUInt i = 0; i < g_scene.size(); i++)
			{
				if (Cmp(g_scene[i]->m_pureFileName, szBuffer))
				{
					if (g_scene[i]->m_hasAnimation)
						m_btnSceneProperties.EnableWindow(TRUE);

					for (CUInt j = 0; j < g_scene[i]->m_images.size(); j++)
					{
						InsertItemToObjectList(g_scene[i]->m_images[j]->GetName(), eOBJECTLIST_IMAGE);
					}
					for (CUInt j = 0; j < g_scene[i]->m_effects.size(); j++)
					{
						InsertItemToObjectList(g_scene[i]->m_effects[j]->GetName(), eOBJECTLIST_EFFECT);
					}
					for (CUInt j = 0; j < g_scene[i]->m_materials.size(); j++)
					{
						InsertItemToObjectList(g_scene[i]->m_materials[j]->GetName(), eOBJECTLIST_MATERIAL);
					}
					//animations
					for (CUInt j = 0; j < g_scene[i]->m_animations.size(); j++)
					{
						InsertItemToObjectList(g_scene[i]->m_animations[j]->GetName(), eOBJECTLIST_ANIMATION);
					}
					for (CUInt j = 0; j < g_scene[i]->m_animationClips.size(); j++)
					{
						InsertItemToObjectList(g_scene[i]->m_animationClips[j]->GetName(), eOBJECTLIST_ANIMATIONCLIP);
					}
					//lights
					for (CUInt j = 0; j < g_scene[i]->m_lights.size(); j++)
					{
						InsertItemToObjectList(g_scene[i]->m_lights[j]->GetName(), eOBJECTLIST_LIGHT);
					}
					//for( CUInt j = 0; j < g_scene[i]->m_lightInstances.size(); j++ )
					//{
					//	InsertItemToObjectList( g_scene[i]->m_lightInstances[j]->GetName());
					//}
					//camera
					for (CUInt j = 0; j < g_scene[i]->m_cameras.size(); j++)
					{
						InsertItemToObjectList(g_scene[i]->m_cameras[j]->GetName(), eOBJECTLIST_CAMERA);
					}
					//for( CUInt j = 0; j < g_scene[i]->m_cameraInstances.size(); j++ )
					//{
					//	InsertItemToObjectList( g_scene[i]->m_cameraInstances[j]->GetName());
					//}
					//geometry
					for (CUInt j = 0; j < g_scene[i]->m_geometries.size(); j++)
					{
						InsertItemToObjectList(g_scene[i]->m_geometries[j]->GetName(), eOBJECTLIST_MESH);
					}
					//for( CUInt j = 0; j < g_scene[i]->m_instanceGeometries.size(); j++ )
					//{
					//	InsertItemToObjectList( g_scene[i]->m_instanceGeometries[j]->GetName());
					//}
					//controller
					for (CUInt j = 0; j < g_scene[i]->m_controllers.size(); j++)
					{
						InsertItemToObjectList(g_scene[i]->m_controllers[j]->GetName(), eOBJECTLIST_SKINCONTROLLER);
					}
				}
			}
		}
		else //prefab mode
		{
			CInstancePrefab* m_newInstancePrefab;
			for (CUInt i = 0; i < g_instancePrefab.size(); i++)
			{
				if (Cmp(g_instancePrefab[i]->GetName(), szBuffer))
				{
					m_newInstancePrefab = g_instancePrefab[i];
				}
			}
			//show the elements of newly selected prefab
			for (CUInt j = 0; j < 3; j++)
			{
				if (m_newInstancePrefab->GetPrefab()->GetHasLod(j))
					InsertItemToObjectList(m_newInstancePrefab->GetScene(j)->GetName(), eOBJECTLIST_SCENE);
			}

		}
	}
	else
	{
		m_btnRemoveScene.EnableWindow( FALSE );
		m_btnSceneProperties.EnableWindow( FALSE );
	}
}

void CVandaEngine1Dlg::OnLvnItemchangedListEngineObjects(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMLISTVIEW pNMLV = reinterpret_cast<LPNMLISTVIEW>(pNMHDR);
	// TODO: Add your control notification handler code here
	POSITION p = m_listBoxEngineObjects.GetFirstSelectedItemPosition();
	CInt nSelected = -1;
	while(p)
	{
		nSelected = m_listBoxEngineObjects.GetNextSelectedItem(p);
	}
	if( nSelected >= 0 )
	{
		m_btnRemoveEngineObject.EnableWindow( TRUE );
		m_btnEngineObjectProperties.EnableWindow( TRUE );
	}
	else
	{
		m_btnRemoveEngineObject.EnableWindow( FALSE );
		m_btnEngineObjectProperties.EnableWindow( FALSE );
	}
}

void CVandaEngine1Dlg::ResetPhysX(CBool releaseActors)
{
	if( releaseActors )
	{
		g_multipleView->m_nx->m_nxActorNames.clear();
		g_multipleView->m_nx->m_nxActorTypes.clear();

		CUInt actor_counts = gPhysXscene->getNbActors();
		for( CUInt i = 0; i < actor_counts; i++ )
		{
			if( !gPhysXscene->getActors()[i]->getName() ) continue;
			gPhysXscene->releaseActor( *gPhysXscene->getActors()[i] );
		}
	}
	//always release ground plane
	if (!releaseActors)
		g_multipleView->m_nx->ReleaseGroundPlane();

	g_multipleView->m_nx->gControllers->reportSceneChanged();

	//release character controller
	for( CUInt i = 0; i < gPhysXscene->getNbActors(); i++ )
	{
		if( !gPhysXscene->getActors()[i]->getName() )
		{
			gPhysXscene->releaseActor( *gPhysXscene->getActors()[i] );
			break;
		}
	}

	g_multipleView->m_nx->ReleaseCharacterControllers();

	g_multipleView->m_nx->gDefaultGravity.x = g_physXProperties.m_fGravityX;
	g_multipleView->m_nx->gDefaultGravity.y = g_physXProperties.m_fGravityY;
	g_multipleView->m_nx->gDefaultGravity.z = g_physXProperties.m_fGravityZ;
	gPhysicsSDK->setParameter( NX_SKIN_WIDTH, g_physXProperties.m_fDefaultSkinWidth );

	NxMaterial* defaultMaterial = gPhysXscene->getMaterialFromIndex(0);
	defaultMaterial->setRestitution(g_physXProperties.m_fDefaultRestitution);
	defaultMaterial->setStaticFriction(g_physXProperties.m_fDefaultStaticFriction);
	defaultMaterial->setDynamicFriction(g_physXProperties.m_fDefaultDynamicFriction);

	if (g_physXProperties.m_bGroundPlane)
		g_multipleView->m_nx->CreateGroundPlane(g_physXProperties.m_fGroundHeight);
	g_multipleView->m_nx->InitCharacterControllers(g_multipleView->m_nx->gCharacterPos.x, g_multipleView->m_nx->gCharacterPos.y, g_multipleView->m_nx->gCharacterPos.z, g_physXProperties.m_fCapsuleRadius, g_physXProperties.m_fCapsuleHeight, g_physXProperties.m_fCharacterSkinWidth, g_physXProperties.m_fCharacterSlopeLimit, g_physXProperties.m_fCharacterStepOffset);

	if( g_physXProperties.m_bApplyGravity )
	{
		g_multipleView->m_nx->gDefaultGravity = NxVec3( g_physXProperties.m_fGravityX, g_physXProperties.m_fGravityY, g_physXProperties.m_fGravityZ );
	}
	else
	{
		g_multipleView->m_nx->gDefaultGravity = NxVec3(0.0f);
	}
	g_multipleView->m_nx->gDesiredDistance = g_physXProperties.m_fCameraCharacterDistance;
	g_multipleView->m_nx->debugMode = g_physXProperties.m_bDebugMode;
	g_multipleView->m_nx->gCharacterSpeed = g_physXProperties.m_fCharacterSpeed;

	if( g_currentCameraType == eCAMERA_DEFAULT_FREE || g_currentCameraType == eCAMERA_COLLADA )
	{
		if( gPhysXscene )
		{
		  // Run collision and dynamics for delta time since the last frame
			gPhysXscene->simulate(1.0f/60.0f/*elapsedTime*/);
			gPhysXscene->flushStream();
			gPhysXscene->fetchResults(NX_ALL_FINISHED, true);
		}
	}
}

void CVandaEngine1Dlg::OnBnClickedBtnRemovePhysx()
{

	if( g_multipleView->m_enableTimer )
		g_multipleView->EnableTimer( CFalse );

	if( !g_multipleView->m_nx->m_hasScene )
	{
		int nSelected = -1; 
		POSITION p = m_listBoxPhysXElements.GetFirstSelectedItemPosition();
		while(p)
		{
			nSelected = m_listBoxPhysXElements.GetNextSelectedItem(p);
		}
		if( nSelected >= 0 )
		{
			TCHAR szBuffer[1024];
			DWORD cchBuf(1024);
			LVITEM lvi;
			lvi.iItem = nSelected;
			lvi.iSubItem = 0;
			lvi.mask = LVIF_TEXT;
			lvi.pszText = szBuffer;
			lvi.cchTextMax = cchBuf;
			m_listBoxPhysXElements.GetItem(&lvi);

			CInt result = IDYES;
			CChar tempString[MAX_NAME_SIZE];
			sprintf( tempString, "Delete PhysX actor '%s' ?", szBuffer );
			result = MessageBox( tempString, "Warning", MB_YESNO | MB_ICONERROR );
			if( result == IDYES )
			{
				for( CUInt i = 0 ; i < g_scene.size(); i++ )
				{
					CBool foundTarget = CFalse;
					for( CUInt j = 0 ; j < g_scene[i]->m_instanceGeometries.size(); j++ )
					{
						if( g_scene[i]->m_instanceGeometries[j]->m_hasPhysX && Cmp( g_scene[i]->m_instanceGeometries[j]->m_physXName, szBuffer ) )
						{
							foundTarget = CTrue;
							if( gPhysXscene )
							{
								for( CUInt k = 0; k < gPhysXscene->getNbActors(); k++ )
								{
									CChar actorName[MAX_NAME_SIZE];

									if( !gPhysXscene->getActors()[k]->getName() ) continue;
									Cpy( actorName, gPhysXscene->getActors()[k]->getName() );

									if( Cmp( actorName, g_scene[i]->m_instanceGeometries[j]->m_physXName ) )
									{
										gPhysXscene->releaseActor( *gPhysXscene->getActors()[k] );
										g_scene[i]->m_instanceGeometries[j]->m_hasPhysX = CFalse;
										Cpy( g_scene[i]->m_instanceGeometries[j]->m_physXName, "\n" );
										PrintInfo( "\nActor removed successfully" );
										m_listBoxPhysXElements.DeleteItem(nSelected);
										SortPhysXList();
										if( g_scene[i]->m_instanceGeometries[j]->m_physXDensity > 0 )
											g_updateOctree = CTrue;

										break;
									}
								}

							}
							break;
						}
					}
					if( foundTarget )
						break;
				}
			}
		}
	}
	else
	{

		if( MessageBox( "Remove Current PhysX Scene?", "Vanda Engine 1 Warning", MB_YESNO | MB_ICONWARNING ) == IDYES )
		{
			CDelete(g_externalPhysX);
			ResetPhysX();
			m_physXElementListIndex = -1;
			for (int nItem = m_listBoxPhysXElements.GetItemCount()-1; nItem >= 0 ;nItem-- )
			{
				m_listBoxPhysXElements.DeleteItem(nItem);
			}
			m_btnRemovePhysX.EnableWindow( FALSE );
		}
	}
	g_multipleView->m_nx->gControllers->reportSceneChanged();
	if( g_currentCameraType == eCAMERA_DEFAULT_FREE && gPhysXscene )
	{
	  // Run collision and dynamics for delta time since the last frame
		gPhysXscene->simulate(1.0f/60.0f/*elapsedTime*/);
		gPhysXscene->flushStream();
		gPhysXscene->fetchResults(NX_ALL_FINISHED, true);
	}

	if( g_multipleView->m_enableTimer )
		g_multipleView->EnableTimer( CTrue );

	g_multipleView->SetElapsedTimeFromBeginning();
	g_multipleView->RenderWindow();

}

CVoid CVandaEngine1Dlg::OnMenuClickedGeneralAmbientColor()
{
	m_dlgGeneralAmbientColor = CNew( CGeneralAmbientColor );
	m_dlgGeneralAmbientColor->DoModal();
	CDelete(m_dlgGeneralAmbientColor );
}
void CVandaEngine1Dlg::OnBnClickedBtnTwitter()
{
	ShellExecute(NULL, "open", "https://www.twitter.com/vanda_engine", NULL, NULL, SW_SHOWNORMAL);
	g_multipleView->SetElapsedTimeFromBeginning();
}

void CVandaEngine1Dlg::OnBnClickedBtnYoutube()
{
	ShellExecute(NULL, "open", "https://www.youtube.com/channel/UCLTmJ3HgZuIoTSxyhD2UnkA", NULL, NULL, SW_SHOWNORMAL);
	g_multipleView->SetElapsedTimeFromBeginning();
}

void CVandaEngine1Dlg::OnLvnItemchangedListPhysxElements(NMHDR *pNMHDR, LRESULT *pResult)
{
	if( !g_multipleView->m_nx->m_hasScene )
	{
		LPNMLISTVIEW pNMLV = reinterpret_cast<LPNMLISTVIEW>(pNMHDR);
		POSITION p = m_listBoxPhysXElements.GetFirstSelectedItemPosition();
		CInt nSelected = -1;
		while(p)
		{
			nSelected = m_listBoxPhysXElements.GetNextSelectedItem(p);
		}
		if( nSelected >= 0 )
			m_btnRemovePhysX.EnableWindow( TRUE );
		else
			m_btnRemovePhysX.EnableWindow( FALSE );
	}
}

void CVandaEngine1Dlg::OnBnClickedBtnPublishSolution()
{
	if( g_multipleView->m_enableTimer )
		g_multipleView->EnableTimer( CFalse );

	m_dlgPublishProject = CNew(CPublishProject);
	CInt result = m_dlgPublishProject->DoModal();
	if( result == IDOK )
	{
		if( !Cmp(g_currentVSceneName, "\n") && m_dlgPublishProject->m_saveCurrentScene )
		{
			for( CUInt index = 0; index < g_VSceneNamesOfCurrentProjectToBePublished.size(); index++ )
			{
				CChar temp[MAX_NAME_SIZE];
				Cpy( temp, g_VSceneNamesOfCurrentProjectToBePublished[index].c_str() );
				if( Cmp( g_currentVSceneName, temp ) )
				{
					//found the target
					OnMenuClickedSaveAs( CFalse );
					break;
				}
			}
		}

		SetCapture();
		SetCursor( LoadCursorFromFile( "Assets/Engine/Icons/progress.ani") );

		CChar rootPath[MAX_NAME_SIZE];
		sprintf( rootPath, "%s%s%s%s", m_dlgPublishProject->m_strDestination, "/", m_dlgPublishProject->m_strName, "/" );
		CreateWindowsDirectory( rootPath );
		CopyAllFilesFromSrcToDstDirectory( "Assets/Engine/DLLs/", rootPath );
		CopyAllFilesFromSrcToDstDirectory( "Assets/Engine/Publish/", rootPath );
		CChar oldFileName[MAX_NAME_SIZE];
		sprintf( oldFileName, "%s%s", rootPath, "publish.exe" );
		CChar newFileName[MAX_NAME_SIZE];
		sprintf( newFileName, "%s%s%s", rootPath, m_dlgPublishProject->m_strName, ".exe" );

		rename(oldFileName, newFileName);

		CChar driversPath[MAX_NAME_SIZE];
		sprintf( driversPath, "%s%s%s%s", m_dlgPublishProject->m_strDestination, "/", m_dlgPublishProject->m_strName, "/Drivers/" );
		CreateWindowsDirectory( driversPath );
		CopyAllFilesFromSrcToDstDirectory( "Assets/Engine/Drivers/", driversPath );

		CChar assetPath[MAX_NAME_SIZE];
		sprintf( assetPath, "%s%s%s%s", m_dlgPublishProject->m_strDestination, "/", m_dlgPublishProject->m_strName, "/Assets/" );
		CreateWindowsDirectory( assetPath );

		CChar logoPath[MAX_NAME_SIZE];
		sprintf( logoPath, "%s%s%s%s", m_dlgPublishProject->m_strDestination, "/", m_dlgPublishProject->m_strName, "/Assets/Logo/" );
		CreateWindowsDirectory( logoPath );
		CChar tempFileName[MAX_NAME_SIZE];
		Cpy( tempFileName, m_dlgPublishProject->m_strSplash.GetBuffer(m_dlgPublishProject->m_strSplash.GetLength()) );
		m_dlgPublishProject->m_strSplash.ReleaseBuffer();
		CopyOneFileToDstDirectory( tempFileName, logoPath );
		CChar* afterPath = GetAfterPath( tempFileName );
		CChar fileToRename[MAX_NAME_SIZE];
		sprintf( fileToRename, "%s%s", logoPath, afterPath );
		CChar renamedFileName[MAX_NAME_SIZE];
		sprintf( renamedFileName, "%s%s", logoPath, "Logo.bmp" );
		rename( fileToRename, renamedFileName );

		CChar vScenesPath[MAX_NAME_SIZE];
		sprintf( vScenesPath, "%s%s%s%s", m_dlgPublishProject->m_strDestination, "/", m_dlgPublishProject->m_strName, "/Assets/VScenes/" );
		CreateWindowsDirectory( vScenesPath );

		CChar configPath[MAX_NAME_SIZE];
		sprintf( configPath, "%s%s%s%s", m_dlgPublishProject->m_strDestination, "/", m_dlgPublishProject->m_strName, "/Assets/Config/" );
		CreateWindowsDirectory( configPath );
		CopyOneFileToDstDirectory( "Assets/Config/conf_win32.dat", configPath);

		CChar savePath[MAX_NAME_SIZE];
		sprintf( savePath, "%s%s%s%s", m_dlgPublishProject->m_strDestination, "/", m_dlgPublishProject->m_strName, "/Assets/Save/" );
		CreateWindowsDirectory( savePath );

		CChar savePathFile[MAX_NAME_SIZE];
		sprintf( savePathFile, "%s%s", savePath, "publish.txt" );
		FILE* ProjectsFilePtr =  fopen( savePathFile, "wt" );

		if( !ProjectsFilePtr )
		{
			MessageBox( "Couldn't open 'assets/save/publish.txt' to save data!", "Vanda Engine Error", MB_OK | MB_ICONERROR);
			return;
		}
		
		CChar nameWithoutDot[MAX_NAME_SIZE];
		Cpy( nameWithoutDot, g_firstSceneNameToBeLoaded.c_str() );
		GetWithoutDot( nameWithoutDot );

		CChar save[MAX_NAME_SIZE];
		sprintf( save, "%s%s%s", "file = Assets/VScenes/", nameWithoutDot, "\n" );
		std::string finalSave(save);
		fwrite( save, finalSave.length(), 1, ProjectsFilePtr );
		fclose(ProjectsFilePtr);


		CChar enginePath[MAX_NAME_SIZE];
		sprintf( enginePath, "%s%s%s%s", m_dlgPublishProject->m_strDestination, "/", m_dlgPublishProject->m_strName, "/Assets/Engine/" );
		CreateWindowsDirectory( enginePath );

		CChar shaderRootPath[MAX_NAME_SIZE];
		sprintf( shaderRootPath, "%s%s%s%s", m_dlgPublishProject->m_strDestination, "/", m_dlgPublishProject->m_strName, "/Assets/Engine/Shaders/" );
		CreateWindowsDirectory( shaderRootPath );
		CopyAllFilesFromSrcToDstDirectory( "Assets/Engine/Shaders/", shaderRootPath );

		CChar dofShaderPath[MAX_NAME_SIZE];
		sprintf( dofShaderPath, "%s%s%s%s", m_dlgPublishProject->m_strDestination, "/", m_dlgPublishProject->m_strName, "/Assets/Engine/Shaders/DOF/" );
		CreateWindowsDirectory( dofShaderPath );
		CopyAllFilesFromSrcToDstDirectory( "Assets/Engine/Shaders/DOF/", dofShaderPath );

		CChar shadowShaderPath[MAX_NAME_SIZE];
		sprintf( shadowShaderPath, "%s%s%s%s", m_dlgPublishProject->m_strDestination, "/", m_dlgPublishProject->m_strName, "/Assets/Engine/Shaders/Shadow/" );
		CreateWindowsDirectory( shadowShaderPath );
		CopyAllFilesFromSrcToDstDirectory( "Assets/Engine/Shaders/Shadow/", shadowShaderPath );

		CChar shadowShaderNormalPath[MAX_NAME_SIZE];
		sprintf(shadowShaderNormalPath, "%s%s%s%s", m_dlgPublishProject->m_strDestination, "/", m_dlgPublishProject->m_strName, "/Assets/Engine/Shaders/Shadow/shadow_normal/");
		CreateWindowsDirectory(shadowShaderNormalPath);
		CopyAllFilesFromSrcToDstDirectory("Assets/Engine/Shaders/Shadow/shadow_normal/", shadowShaderNormalPath);

		CChar spotShadowShaderPath[MAX_NAME_SIZE];
		sprintf( spotShadowShaderPath, "%s%s%s%s", m_dlgPublishProject->m_strDestination, "/", m_dlgPublishProject->m_strName, "/Assets/Engine/Shaders/Shadow/Spot/" );
		CreateWindowsDirectory( spotShadowShaderPath );
		CopyAllFilesFromSrcToDstDirectory( "Assets/Engine/Shaders/Shadow/Spot/", spotShadowShaderPath );

		CChar spotNormalShadowShaderPath[MAX_NAME_SIZE];
		sprintf(spotNormalShadowShaderPath, "%s%s%s%s", m_dlgPublishProject->m_strDestination, "/", m_dlgPublishProject->m_strName, "/Assets/Engine/Shaders/Shadow/Spot/spot_normal/");
		CreateWindowsDirectory(spotNormalShadowShaderPath);
		CopyAllFilesFromSrcToDstDirectory("Assets/Engine/Shaders/Shadow/Spot/spot_normal/", spotNormalShadowShaderPath);

		std::vector<std::string> m_packagePrefabNames;
		std::vector<std::string> m_packageNames;
		std::vector<std::string> m_prefabNames;


		//create all of scene the directories
		for( CUInt i = 0; i < g_VSceneNamesOfCurrentProjectToBePublished.size(); i++ )
		{
			CChar currentSceneNameWithoutDot[MAX_NAME_SIZE];
			Cpy( currentSceneNameWithoutDot, g_VSceneNamesOfCurrentProjectToBePublished[i].c_str() );
			GetWithoutDot( currentSceneNameWithoutDot );

			//original paths
			CChar originalVScenePath[MAX_NAME_SIZE];
			sprintf( originalVScenePath, "%s%s%s", g_currentProjectPath, currentSceneNameWithoutDot, "/" );

			CChar originalWaterTexturesPath[MAX_NAME_SIZE];
			sprintf( originalWaterTexturesPath, "%s%s%s", g_currentProjectPath, currentSceneNameWithoutDot, "/Waters/" );

			CChar originalSoundPath[MAX_NAME_SIZE];
			sprintf( originalSoundPath, "%s%s%s", g_currentProjectPath, currentSceneNameWithoutDot, "/Sounds/" );

			CChar originalAmbientSoundPath[MAX_NAME_SIZE];
			sprintf( originalAmbientSoundPath, "%s%s%s", g_currentProjectPath, currentSceneNameWithoutDot, "/Sounds/Ambient/" );

			CChar originalStaticSoundPath[MAX_NAME_SIZE];
			sprintf( originalStaticSoundPath, "%s%s%s", g_currentProjectPath, currentSceneNameWithoutDot, "/Sounds/Static/" );

			CChar originalSkyPath[MAX_NAME_SIZE];
			sprintf( originalSkyPath, "%s%s%s", g_currentProjectPath, currentSceneNameWithoutDot, "/Sky/" );

			CChar originalBannerPath[MAX_NAME_SIZE];
			sprintf(originalBannerPath, "%s%s%s", g_currentProjectPath, currentSceneNameWithoutDot, "/Banner/");

			CChar originalScriptPath[MAX_NAME_SIZE];
			sprintf(originalScriptPath, "%s%s%s", g_currentProjectPath, currentSceneNameWithoutDot, "/Script/");

			////////

			CChar VScenePath[MAX_NAME_SIZE];
			sprintf( VScenePath, "%s%s%s%s", rootPath, "/assets/VScenes/", currentSceneNameWithoutDot, "/" );
			CreateWindowsDirectory( VScenePath );
			CopyAllFilesFromSrcToDstDirectory(originalVScenePath, VScenePath);

			CChar waterTexturesPath[MAX_NAME_SIZE];
			sprintf( waterTexturesPath, "%s%s%s%s", rootPath, "/assets/VScenes/", currentSceneNameWithoutDot, "/Waters/" );
			CreateWindowsDirectory( waterTexturesPath );
			CopyAllFilesFromSrcToDstDirectory(originalWaterTexturesPath, waterTexturesPath);

			CChar soundPath[MAX_NAME_SIZE];
			sprintf( soundPath, "%s%s%s%s", rootPath, "/assets/VScenes/", currentSceneNameWithoutDot, "/Sounds/" );
			CreateWindowsDirectory( soundPath );

			CChar ambientSoundPath[MAX_NAME_SIZE];
			sprintf( ambientSoundPath, "%s%s%s%s", rootPath, "/assets/VScenes/", currentSceneNameWithoutDot, "/Sounds/Ambient/" );
			CreateWindowsDirectory( ambientSoundPath );
			CopyAllFilesFromSrcToDstDirectory(originalAmbientSoundPath, ambientSoundPath);

			CChar staticSoundPath[MAX_NAME_SIZE];
			sprintf( staticSoundPath, "%s%s%s%s", rootPath, "/assets/VScenes/", currentSceneNameWithoutDot, "/Sounds/Static/" );
			CreateWindowsDirectory( staticSoundPath );
			CopyAllFilesFromSrcToDstDirectory(originalStaticSoundPath, staticSoundPath);

			CChar skyPath[MAX_NAME_SIZE];
			sprintf( skyPath, "%s%s%s%s", rootPath, "/assets/VScenes/", currentSceneNameWithoutDot, "/Sky/" );
			CreateWindowsDirectory( skyPath );
			CopyAllFilesFromSrcToDstDirectory(originalSkyPath, skyPath);

			CChar bannerPath[MAX_NAME_SIZE];
			sprintf(bannerPath, "%s%s%s%s", rootPath, "/assets/VScenes/", currentSceneNameWithoutDot, "/Banner/");
			CreateWindowsDirectory(bannerPath);
			CopyAllFilesFromSrcToDstDirectory(originalBannerPath, bannerPath);

			CChar scriptPath[MAX_NAME_SIZE];
			sprintf(scriptPath, "%s%s%s%s", rootPath, "/assets/VScenes/", currentSceneNameWithoutDot, "/Script/");
			CreateWindowsDirectory(scriptPath);
			CopyAllFilesFromSrcToDstDirectory(originalScriptPath, scriptPath);

			//copy package names
			CChar packagePath[MAX_NAME_SIZE];
			sprintf(packagePath, "%s%s", originalVScenePath, "/packages.pkg");
			FILE *PackageFilePtr;
			PackageFilePtr = fopen(packagePath, "rb");
			if (!PackageFilePtr)
			{
				CChar temp[MAX_NAME_SIZE];
				sprintf(temp, "%s%s%s", "Couldn't open the file '", packagePath, "' to save data");
				MessageBox(temp, "Vanda Engine Error", MB_OK);
				ReleaseCapture();
				PrintInfo("Publish failed.", COLOR_RED);
				return;
			}
			CUInt size = -1;
			fread(&size, sizeof(CUInt), 1, PackageFilePtr);
			for (CUInt i = 0; i < size; i++)
			{
				CChar name[MAX_NAME_SIZE];
				CChar package_name[MAX_NAME_SIZE];
				CChar prefab_name[MAX_NAME_SIZE];
				//write prefab data
				fread(name, sizeof(CChar), MAX_NAME_SIZE, PackageFilePtr);
				fread(package_name, sizeof(CChar), MAX_NAME_SIZE, PackageFilePtr);
				fread(prefab_name, sizeof(CChar), MAX_NAME_SIZE, PackageFilePtr);

				CBool foundTarget = CFalse;
				for (CUInt j = 0; j < m_packagePrefabNames.size(); j++)
				{
					if (Cmp(name, m_packagePrefabNames[j].c_str()))
					{
						foundTarget = CTrue;
						break;
					}
				}
				if (!foundTarget)
				{
					m_packagePrefabNames.push_back(name);
					m_packageNames.push_back(package_name);
					m_prefabNames.push_back(prefab_name);
				}
			}
			fclose(PackageFilePtr);
		}

		CChar srcRootPackagePath[MAX_NAME_SIZE];
		SHGetFolderPath(NULL, CSIDL_PERSONAL, NULL, SHGFP_TYPE_CURRENT, srcRootPackagePath);
		Append(srcRootPackagePath, "/Vanda/Packages/");

		CChar dstRootPackagePath[MAX_NAME_SIZE];
		sprintf(dstRootPackagePath, "%s%s", assetPath, "packages/");
		CreateWindowsDirectory(dstRootPackagePath);

		std::vector<std::string> dstUniquePackagePath;
		for (CUInt i = 0; i < m_packagePrefabNames.size(); i++)
		{
			CChar dstPackagePath[MAX_NAME_SIZE];
			sprintf(dstPackagePath, "%s%s%s", dstRootPackagePath, m_packageNames[i].c_str(), "/");

			CBool found = CFalse;
			for (CUInt j = 0; j < dstUniquePackagePath.size(); j++)
			{
				if (Cmp(dstUniquePackagePath[j].c_str(), dstPackagePath))
				{
					found = CTrue;
					break;
				}
			}
			if (!found)
			{
				CreateWindowsDirectory(dstPackagePath);
				dstUniquePackagePath.push_back(dstPackagePath);
			}

			CChar dstPrefabPath[MAX_NAME_SIZE];
			sprintf(dstPrefabPath, "%s%s%s", dstPackagePath, m_prefabNames[i].c_str(), "/");
			CreateWindowsDirectory(dstPrefabPath);

			CChar srcPrefabPath[MAX_NAME_SIZE];
			sprintf(srcPrefabPath, "%s%s%s%s%s", srcRootPackagePath, m_packageNames[i].c_str(), "/", m_prefabNames[i].c_str(), "/");
			CopyAllFilesAndFoldersToDstDirectory(srcPrefabPath, dstPrefabPath);

		}

		CChar temp[MAX_NAME_SIZE];
		sprintf( temp, "\n%s%s%s%s%s", "Project published to '", m_dlgPublishProject->m_strDestination,"/", m_dlgPublishProject->m_strName, "' successfully");
		PrintInfo(temp);

		CChar openShell[MAX_NAME_SIZE];
		sprintf( openShell, "%s%s%s", m_dlgPublishProject->m_strDestination, "/", m_dlgPublishProject->m_strName );
		ShellExecute( NULL, "open", openShell, NULL, NULL, SW_SHOWNORMAL );
		CDelete(m_dlgPublishProject);

		m_packagePrefabNames.clear();
		m_packageNames.clear();
		m_prefabNames.clear();
		dstUniquePackagePath.clear();
		ReleaseCapture();
	}
	g_VSceneNamesOfCurrentProjectToBePublished.clear();
	if( g_multipleView->m_enableTimer )
		g_multipleView->EnableTimer( CTrue );

	g_multipleView->SetElapsedTimeFromBeginning();
}

void CVandaEngine1Dlg::OnBnClickedBtnScriptManager()
{
	CBool foundTarget = CFalse;
	if (g_selectedName != -1)
	{
		if (g_editorMode == eMODE_VSCENE)
		{
			for (CUInt i = 0; i < g_instancePrefab.size(); i++)
			{
				if (g_instancePrefab[i]->GetNameIndex() == g_selectedName)
				{
					if (CmpIn(g_instancePrefab[i]->GetName(), "Vanda_Basics_Box_Trigger") || CmpIn(g_instancePrefab[i]->GetName(), "Vanda_Basics_Sphere_Trigger"))
					{
						ex_pAddScript->SetInstanceGeo(g_instancePrefab[i]->GetScene(0)->m_instanceGeometries[0]);

						foundTarget = CTrue;
						break;
					}
				}
			}
		}
		else //prefab mode
		{
			for (CUInt i = 0; i < g_scene.size(); i++)
			{
				for (CUInt j = 0; j < g_scene[i]->m_instanceGeometries.size(); j++)
				{
					if (g_scene[i]->m_instanceGeometries[j]->m_nameIndex == g_selectedName)
					{
						if (g_scene[i]->m_instanceGeometries[j]->m_isTrigger)
						{
							ex_pAddScript->SetInstanceGeo(g_scene[i]->m_instanceGeometries[j]);
							foundTarget = CTrue;
							break;
						}
					}
				}
			}
		}
	}
	if (!foundTarget)
	{
		MessageBox("Please select a trigger", "Vanda Engine Error", MB_OK | MB_ICONINFORMATION);
		return;
	}
	if( g_multipleView->m_enableTimer )
		g_multipleView->EnableTimer( CFalse );

	if( g_vandaDemo )
	{
		MessageBox( "This button opens the script manager that lets you load another scene or activate an animation while colliding the appropriate triggers.\nThis feature is not available in RTI demo Edition Version 1.x", "Vanda Engine Error", MB_OK | MB_ICONINFORMATION );
	}
	else
	{
		m_dlgAddScript->DoModal();
	}
	if( g_multipleView->m_enableTimer )
		g_multipleView->EnableTimer( CTrue );

	g_multipleView->SetElapsedTimeFromBeginning();

}

void CVandaEngine1Dlg::OnBnClickedBtnCameraRenderingManager()
{
	if( g_multipleView->m_enableTimer )
		g_multipleView->EnableTimer( CFalse );
	OnMenuClickedSelectCamera();
	if( g_multipleView->m_enableTimer )
		g_multipleView->EnableTimer( CTrue );
	g_multipleView->SetElapsedTimeFromBeginning();
	g_multipleView->RenderWindow();
}

void CVandaEngine1Dlg::OnBnClickedBtnSceneProperties()
{
	int nSelected = -1; 
	POSITION p = m_listBoxScenes.GetFirstSelectedItemPosition();
	while(p)
	{
		nSelected = m_listBoxScenes.GetNextSelectedItem(p);
	}
	if( nSelected >= 0 )
	{
		TCHAR szBuffer[1024];
		DWORD cchBuf(1024);
		LVITEM lvi;
		lvi.iItem = nSelected;
		lvi.iSubItem = 0;
		lvi.mask = LVIF_TEXT;
		lvi.pszText = szBuffer;
		lvi.cchTextMax = cchBuf;
		m_listBoxScenes.GetItem(&lvi);
		CScene* scene = NULL;
		for( CUInt i = 0; i < g_scene.size(); i++ )
		{
			if( Cmp( szBuffer, g_scene[i]->GetName() ) )
			{
				scene = g_scene[i];
			}
		}
		if( g_multipleView->m_enableTimer )
			g_multipleView->EnableTimer( CFalse );

		CSceneProperties* m_dlgSceneProperties = CNew (CSceneProperties);
		m_dlgSceneProperties->SetScene( scene );
		m_dlgSceneProperties->DoModal();
		CDelete( m_dlgSceneProperties );

		if( g_multipleView->m_enableTimer )
			g_multipleView->EnableTimer( CTrue );
		g_multipleView->SetElapsedTimeFromBeginning();
		g_multipleView->RenderWindow();
	}
}


void CVandaEngine1Dlg::OnBnClickedBtnPhysxEditor()
{
	if( g_multipleView->m_enableTimer )
		g_multipleView->EnableTimer( CFalse );
	OnMenuClickedEditPhysX();
	if( g_multipleView->m_enableTimer )
		g_multipleView->EnableTimer( CTrue );
	g_multipleView->SetElapsedTimeFromBeginning();
	g_multipleView->RenderWindow();
}

void CVandaEngine1Dlg::OnNcLButtonDblClk(UINT nHitTest, CPoint point)
{
	// TODO: Add your message handler code here and/or call default

	//CDialog::OnNcLButtonDblClk(nHitTest, point);
}



void CVandaEngine1Dlg::OnBnClickedBtnPrefabs()
{
	if (g_currentCameraType == eCAMERA_DEFAULT_PHYSX)
	{
		MessageBox("You can not insert prefabs in Play Mode.Please exit from play mode and try again.", "Vanda Engine Error", MB_OK | MB_ICONINFORMATION);
		return;
	}

	if (g_multipleView->m_enableTimer)
		g_multipleView->EnableTimer(CFalse);
	OnMenuClickedPrefab();
	if (g_multipleView->m_enableTimer)
		g_multipleView->EnableTimer(CTrue);
	g_multipleView->SetElapsedTimeFromBeginning();
	g_multipleView->RenderWindow();
}

void CVandaEngine1Dlg::RemoveSelectedScene(CChar* szBuffer, CChar* sceneId)
{
	for (CUInt i = 0; i < g_scene.size(); i++)
	{
		CBool condition = CFalse;
		if (sceneId)
			condition = Cmp(g_scene[i]->m_pureFileName, szBuffer) && Cmp(g_scene[i]->GetDocURI(), sceneId);
		else
			condition = Cmp(g_scene[i]->m_pureFileName, szBuffer);
		if (condition)
		{
			//remove unshared images
			for (CUInt j = 0; j < g_scene[i]->m_instanceGeometries.size(); j++)
			{
				CGeometry* m_geo = g_scene[i]->m_instanceGeometries[j]->m_abstractGeometry;
				for (CUInt k = 0; k < m_geo->m_groups.size(); k++)
				{
					for (CUInt l = 0; l < 4; l++)
					{
						CImage* currentImage = NULL;
						if (l == 0 && m_geo->m_groups[k]->m_hasDiffuse && m_geo->m_groups[k]->m_diffuseImg)
						{
							currentImage = m_geo->m_groups[k]->m_diffuseImg;
						}
						else if (l == 1 && m_geo->m_groups[k]->m_hasNormalMap && m_geo->m_groups[k]->m_normalMapImg)
						{
							currentImage = m_geo->m_groups[k]->m_normalMapImg;
						}
						else if (l == 2 && m_geo->m_groups[k]->m_hasGlossMap && m_geo->m_groups[k]->m_glossMapImg)
						{
							currentImage = m_geo->m_groups[k]->m_glossMapImg;
						}
						else if (l == 3 && m_geo->m_groups[k]->m_hasDirtMap && m_geo->m_groups[k]->m_dirtMapImg)
						{
							currentImage = m_geo->m_groups[k]->m_dirtMapImg;
						}
						if (currentImage)
						{
							//see if other scenes use this texture
							CBool foundTarget = CFalse;
							for (CUInt m = 0; m < g_scene.size(); m++)
							{
								CBool condition = CFalse;
								if (g_editorMode == eMODE_PREFAB)
									condition = Cmp(g_scene[m]->GetName(), g_scene[i]->GetName());
								else
									condition = Cmp(g_scene[m]->GetName(), g_scene[i]->GetName()) && Cmp(g_scene[m]->GetDocURI(), g_scene[i]->GetDocURI());

								if (!condition)
								{
									for (CUInt n = 0; n < g_scene[m]->m_instanceGeometries.size(); n++)
									{
										CGeometry* m_geo = g_scene[m]->m_instanceGeometries[n]->m_abstractGeometry;
										for (CUInt o = 0; o < m_geo->m_groups.size(); o++)
										{
											if (m_geo->m_groups[o]->m_hasDiffuse && Cmp(GetAfterPath(currentImage->m_fileName), GetAfterPath(m_geo->m_groups[o]->m_diffuseImg->m_fileName)))
											{
												foundTarget = CTrue;
												break;
											}
											else if (m_geo->m_groups[o]->m_hasNormalMap && Cmp(GetAfterPath(currentImage->m_fileName), GetAfterPath(m_geo->m_groups[o]->m_normalMapImg->m_fileName)))
											{
												foundTarget = CTrue;
												break;
											}
											else if (m_geo->m_groups[o]->m_hasGlossMap && Cmp(GetAfterPath(currentImage->m_fileName), GetAfterPath(m_geo->m_groups[o]->m_glossMapImg->m_fileName)))
											{
												foundTarget = CTrue;
												break;
											}
											else if (m_geo->m_groups[o]->m_hasDirtMap && Cmp(GetAfterPath(currentImage->m_fileName), GetAfterPath(m_geo->m_groups[o]->m_dirtMapImg->m_fileName)))
											{
												foundTarget = CTrue;
												break;
											}
										}
										if (foundTarget)
											break;
									}
								}
								if (foundTarget)
									break;
							}
							if (!foundTarget)
							{
								for (CUInt p = 0; p < g_images.size(); p++)
								{
									if (Cmp(GetAfterPath(g_images[p]->m_fileName), GetAfterPath(currentImage->m_fileName)))
									{
										CChar* nameAfterPath = GetAfterPath(currentImage->m_fileName);
										CChar temp[MAX_NAME_SIZE];
										sprintf(temp, "\n%s%s%s", "Image '", nameAfterPath, "' removed from memory");
										PrintInfo(temp, COLOR_YELLOW);
										CDelete(currentImage);
										g_images.erase(g_images.begin() + p);
										break;
									}
								}
							} //if (!foundTarget )
						} //if( currentImage )
					} //for( CUInt l = 0; l < 6; l++ )
				} //for groups
			} //for other scenes
			//save functions///////////////////////////////////////////////////
		} //if
	} //for
	//remove unshared geometry
	if (g_editorMode == eMODE_VSCENE)
	{
		CBool foundTarget = CFalse;
		for (CUInt i = 0; i < g_scene.size(); i++)
		{
			CBool condition = CFalse;
			if (sceneId)
				condition = Cmp(g_scene[i]->m_pureFileName, szBuffer) && Cmp(g_scene[i]->GetDocURI(), sceneId);
			else
				condition = Cmp(g_scene[i]->m_pureFileName, szBuffer);
			if (condition)
			{
				//delete unshared geometries
				for (CUInt m = 0; m < g_scene[i]->m_instanceGeometries.size(); m++)
				{
					CGeometry* m_geo1 = g_scene[i]->m_instanceGeometries[m]->m_abstractGeometry;

					for (CUInt j = 0; j < g_scene.size(); j++)
					{
						CBool condition = CFalse;
						if (sceneId)
							condition = Cmp(g_scene[j]->m_pureFileName, szBuffer) && Cmp(g_scene[j]->GetDocURI(), sceneId);
						else
							condition = Cmp(g_scene[j]->m_pureFileName, szBuffer);
						if (!condition)
						{
							//delete unshared geometries
							for (CUInt k = 0; k < g_scene[j]->m_instanceGeometries.size(); k++)
							{
								CGeometry* m_geo2 = g_scene[j]->m_instanceGeometries[k]->m_abstractGeometry;
								if (ICmp(m_geo1->GetName(), m_geo2->GetName()) &&
									ICmp(m_geo1->GetDocURI(), m_geo2->GetDocURI()))
								{
									foundTarget = CTrue;
									break;
								}
							}
						}
						if (foundTarget)
							break;
					}
					if (foundTarget)
						break;
				}
			}
			if (foundTarget)
				break;
		}
		if (!foundTarget)
		{
			for (CUInt i = 0; i < g_scene.size(); i++)
			{
				CBool condition = CFalse;
				if (sceneId)
					condition = Cmp(g_scene[i]->m_pureFileName, szBuffer) && Cmp(g_scene[i]->GetDocURI(), sceneId);
				else
					condition = Cmp(g_scene[i]->m_pureFileName, szBuffer);
				if (condition)
				{
					//delete unshared geometries
					for (CUInt j = 0; j < g_scene[i]->m_instanceGeometries.size(); j++)
					{
						CGeometry* m_geo1 = g_scene[i]->m_instanceGeometries[j]->m_abstractGeometry;

						for (CUInt l = 0; l < g_geometries.size(); l++)
						{
							if (ICmp(g_geometries[l]->GetName(), m_geo1->GetName()) &&
								ICmp(g_geometries[l]->GetDocURI(), m_geo1->GetDocURI()))
							{
								CChar temp[MAX_NAME_SIZE];
								sprintf(temp, "%s%s%s%s", "\nGeometry '", g_geometries[l]->GetName(), g_geometries[l]->GetDocURI(), "' removed from memory.");
								PrintInfo(temp, COLOR_YELLOW);
								CDelete(g_geometries[l]);
								g_geometries.erase(g_geometries.begin() + l);
								break;
							}
						}
					}
					break;
				}
			}
		}
	}

	//remove selected scene
	for (CUInt i = 0; i < g_scene.size(); i++)
	{
		CBool condition = CFalse;
		if (sceneId)
			condition = Cmp(g_scene[i]->m_pureFileName, szBuffer) && Cmp(g_scene[i]->GetDocURI(), sceneId);
		else
			condition = Cmp(g_scene[i]->m_pureFileName, szBuffer);
		if (condition)
		{
			//delete the scene
			CChar sceneName[MAX_NAME_SIZE];
			sprintf(sceneName, "%s%s%s", "\nScene ' ", g_scene[i]->GetName(), " ' removed from memory");
			PrintInfo(sceneName, COLOR_YELLOW);

			CDelete(g_scene[i]);
			//delete the vector that holds the scene
			g_scene.erase(g_scene.begin() + i);
		}
	}
}

void CVandaEngine1Dlg::OnBnClickedBtnTimerPause()
{
	g_multipleView->m_enableTimer = CTrue;
	g_multipleView->EnableTimer(g_multipleView->m_enableTimer);

	ex_pBtnTimerPlay->EnableWindow(TRUE);
	ex_pBtnTimerPlay->ShowWindow(SW_SHOW);
	ex_pBtnTimerPause->EnableWindow(FALSE);
	ex_pBtnTimerPause->ShowWindow(SW_HIDE);

	PrintInfo("\nTimer Enabled");

	g_multipleView->SetElapsedTimeFromBeginning();

}


void CVandaEngine1Dlg::OnBnClickedBtnPlayActive()
{
	if (g_currentCameraType == eCAMERA_DEFAULT_PHYSX)
		ex_pVandaEngine1Dlg->ResetPhysX(CTrue);

	g_currentCameraType = eCAMERA_DEFAULT_FREE;
	m_mainBtnTestActive.ShowWindow(SW_HIDE);
	m_mainBtnTestActive.EnableWindow(FALSE);
	m_mainBtnTestActive.UpdateWindow();
	m_mainBtnTestDeactive.ShowWindow(SW_SHOW);
	m_mainBtnTestDeactive.EnableWindow(TRUE);
	m_mainBtnTestDeactive.UpdateWindow();

	g_render.SetActiveInstanceCamera(g_render.GetDefaultInstanceCamera());
	if (g_editorMode == eMODE_VSCENE)
	{
		for (CUInt i = 0; i < g_instancePrefab.size(); i++)
		{
			g_instancePrefab[i]->UpdateBoundingBox(CTrue);
		}
	}
	else
	{
		for (CUInt i = 0; i < g_scene.size(); i++)
		{
			g_render.SetScene(g_scene[i]);
			g_render.GetScene()->Update();
		}
	}

	if (g_editorMode == eMODE_PREFAB)
	{
		for (CUInt i = 0; i < g_scene.size(); i++)
		{
			for (CUInt j = 0; j < g_scene[i]->m_instanceGeometries.size(); j++)
			{
				if (g_scene[i]->m_instanceGeometries[j]->m_hasPhysX)
				{
					g_scene[i]->GeneratePhysX(g_scene[i]->m_instanceGeometries[j]->m_lodAlgorithm, g_scene[i]->m_instanceGeometries[j]->m_physXDensity, g_scene[i]->m_instanceGeometries[j]->m_physXPercentage, g_scene[i]->m_instanceGeometries[j]->m_isTrigger, g_scene[i]->m_instanceGeometries[j]->m_isInvisible, g_scene[i]->m_instanceGeometries[j]);
				}
			}
		}
	}

	g_multipleView->m_nx->gControllers->reportSceneChanged();
	gPhysXscene->simulate(1.0f / 60.0f/*elapsedTime*/);
	gPhysXscene->flushStream();
	gPhysXscene->fetchResults(NX_ALL_FINISHED, true);
	PrintInfo("\nPlay mode disabled");

}


void CVandaEngine1Dlg::OnBnClickedBtnPlayDeactive()
{
	g_render.SetActiveInstanceCamera(NULL);
	g_currentCameraType = eCAMERA_DEFAULT_PHYSX;
	m_mainBtnTestActive.ShowWindow(SW_SHOW);
	m_mainBtnTestActive.EnableWindow(TRUE);
	m_mainBtnTestActive.UpdateWindow();
	m_mainBtnTestDeactive.ShowWindow(SW_HIDE);
	m_mainBtnTestDeactive.EnableWindow(FALSE);
	m_mainBtnTestDeactive.UpdateWindow();

	for (CUInt i = 0; i < g_instancePrefab.size(); i++)
	{
		if (Cmp(g_instancePrefab[i]->GetPrefab()->GetPackageName(), "Vanda_Basics") && Cmp(g_instancePrefab[i]->GetPrefab()->GetPrefabName(), "Character"))
		{
			g_multipleView->m_nx->gCharacterPos.x = g_characterPos.x = g_instancePrefab[i]->GetTranslate().x;
			g_multipleView->m_nx->gCharacterPos.y = g_characterPos.y = g_instancePrefab[i]->GetTranslate().y + (g_physXProperties.m_fCapsuleHeight / 2.f);
			g_multipleView->m_nx->gCharacterPos.z = g_characterPos.z = g_instancePrefab[i]->GetTranslate().z;

			g_camera->m_perspectiveCameraPitch = 0.0f; // no pitch
			g_camera->m_perspectiveCameraYaw = NxMath::degToRad(g_instancePrefab[i]->GetRotate().y);
			g_camera->m_perspectiveCameraTilt = 0.0f; //tilt
			g_multipleView->m_nx->SetCharacterPos(g_characterPos);
		}
	}


	if (g_editorMode == eMODE_VSCENE)
	{
		for (CUInt i = 0; i < g_instancePrefab.size(); i++)
		{
			g_instancePrefab[i]->UpdateBoundingBox();
		}

		//generate physX colliders
		for (CUInt i = 0; i < g_scene.size(); i++)
		{
			for (CUInt j = 0; j < g_scene[i]->m_instanceGeometries.size(); j++)
			{
				if (g_scene[i]->m_instanceGeometries[j]->m_hasPhysX)
				{
					g_scene[i]->GeneratePhysX(g_scene[i]->m_instanceGeometries[j]->m_lodAlgorithm, g_scene[i]->m_instanceGeometries[j]->m_physXDensity, g_scene[i]->m_instanceGeometries[j]->m_physXPercentage, g_scene[i]->m_instanceGeometries[j]->m_isTrigger, g_scene[i]->m_instanceGeometries[j]->m_isInvisible, g_scene[i]->m_instanceGeometries[j]);
				}
			}
		}
	}

	PrintInfo("\nPlay mode enabled");

}


void CVandaEngine1Dlg::OnBnClickedBtnTranslate()
{
	g_currentTransformType = eCTranslate;
	if (g_selectedName != -1)
	{
		CFloat val;
		CChar temp1[MAX_NAME_SIZE];
		val = roundf(g_arrowPosition.x * 100) / 100;
		sprintf(temp1, "%.02f", val);
		ex_pVandaEngine1Dlg->m_editX.SetWindowTextA(temp1);

		CChar temp2[MAX_NAME_SIZE];
		val = roundf(g_arrowPosition.y * 100) / 100;
		sprintf(temp2, "%.02f", val);
		ex_pVandaEngine1Dlg->m_editY.SetWindowTextA(temp2);

		CChar temp3[MAX_NAME_SIZE];
		val = roundf(g_arrowPosition.z * 100) / 100;
		sprintf(temp3, "%.02f", val);
		ex_pVandaEngine1Dlg->m_editZ.SetWindowTextA(temp3);
	}
	PrintInfo("\nTranslation Activated");
}


void CVandaEngine1Dlg::OnBnClickedBtnRotate()
{
	g_currentTransformType = eCRotate;
	if (g_selectedName != -1)
	{
		for (CUInt i = 0; i < g_instancePrefab.size(); i++)
		{
			if (g_instancePrefab[i]->GetNameIndex() == g_selectedName)
			{
				CFloat val;
				CChar temp1[MAX_NAME_SIZE];
				val = roundf(g_instancePrefab[i]->GetRotate().x * 100) / 100;
				sprintf(temp1, "%.02f", val);
				ex_pVandaEngine1Dlg->m_editX.SetWindowTextA(temp1);

				CChar temp2[MAX_NAME_SIZE];
				val = roundf(g_instancePrefab[i]->GetRotate().y * 100) / 100;
				sprintf(temp2, "%.02f", val);
				ex_pVandaEngine1Dlg->m_editY.SetWindowTextA(temp2);

				CChar temp3[MAX_NAME_SIZE];
				val = roundf(g_instancePrefab[i]->GetRotate().z * 100) / 100;
				sprintf(temp3, "%.02f", val);
				ex_pVandaEngine1Dlg->m_editZ.SetWindowTextA(temp3);
				break;
			}
		}
	}

	PrintInfo("\nRotation Activated");
}


void CVandaEngine1Dlg::OnBnClickedBtnScale()
{
	g_currentTransformType = eCScale;

	if (g_selectedName != -1)
	{
		for (CUInt i = 0; i < g_instancePrefab.size(); i++)
		{
			if (g_instancePrefab[i]->GetNameIndex() == g_selectedName)
			{
				CFloat val;
				CChar temp1[MAX_NAME_SIZE];
				val = roundf(g_instancePrefab[i]->GetScale().x * 100) / 100;
				sprintf(temp1, "%.02f", val);
				ex_pVandaEngine1Dlg->m_editX.SetWindowTextA(temp1);

				CChar temp2[MAX_NAME_SIZE];
				val = roundf(g_instancePrefab[i]->GetScale().y * 100) / 100;
				sprintf(temp2, "%.02f", val);
				ex_pVandaEngine1Dlg->m_editY.SetWindowTextA(temp2);

				CChar temp3[MAX_NAME_SIZE];
				val = roundf(g_instancePrefab[i]->GetScale().z * 100) / 100;
				sprintf(temp3, "%.02f", val);
				ex_pVandaEngine1Dlg->m_editZ.SetWindowTextA(temp3);
				break;
			}
		}
	}


	PrintInfo("\nScale Activated");
}


void CVandaEngine1Dlg::OnEnChangeEditX()
{
	CString m_str;
	m_editX.GetWindowTextA(m_str);
	CFloat val = atof(m_str);
	//selected object
	COLORREF color = COLOR_WHITE;
	CHARFORMAT cf;
	cf.dwMask = CFM_COLOR/* | CFM_SIZE*/;
	cf.dwEffects = NULL;
	cf.crTextColor = color;
	CInt nSel = m_editX.GetWindowTextLength();
	m_editX.SetSel(0, nSel);
	m_editX.SetSelectionCharFormat(cf);
	m_editX.ReplaceSel(m_str);

	if (!g_transformObject && !g_multipleView->m_lMouseDown)
	{
		if (g_currentTransformType == eCTranslate)
		{
			g_transformObject = CTrue;
			for (CUInt i = 0; i < g_instancePrefab.size(); i++)
			{
				if (g_instancePrefab[i]->GetNameIndex() == g_selectedName)
				{
					g_arrowPosition.x = val;
					g_instancePrefab[i]->UpdateArrow(CTrue);
					break;
				}
			}
			g_transformObject = CFalse;
		}
		else if (g_currentTransformType == eCRotate)
		{
			g_transformObject = CTrue;
			for (CUInt i = 0; i < g_instancePrefab.size(); i++)
			{
				if (g_instancePrefab[i]->GetNameIndex() == g_selectedName)
				{
					g_arrowRotate.z = val;
					g_transformDirection = YTRANS;
					g_instancePrefab[i]->UpdateArrow(CTrue);
					break;
				}
			}
			g_transformObject = CFalse;
		}
		else if (g_currentTransformType == eCScale)
		{
			g_transformObject = CTrue;
			for (CUInt i = 0; i < g_instancePrefab.size(); i++)
			{
				if (g_instancePrefab[i]->GetNameIndex() == g_selectedName)
				{
					g_arrowScale.x = val;
					g_transformDirection = XTRANS;
					g_instancePrefab[i]->UpdateArrow(CTrue);
					break;
				}
			}
			g_transformObject = CFalse;
		}
	}
}


void CVandaEngine1Dlg::OnEnChangeEditY()
{
	CString m_str;
	m_editY.GetWindowTextA(m_str);
	CFloat val = atof(m_str);
	//selected object
	COLORREF color = COLOR_WHITE;
	CHARFORMAT cf;
	cf.dwMask = CFM_COLOR/* | CFM_SIZE*/;
	cf.dwEffects = NULL;
	cf.crTextColor = color;

	CInt nSel = m_editY.GetWindowTextLength();
	m_editY.SetSel(0, nSel);

	m_editY.SetSelectionCharFormat(cf);
	m_editY.ReplaceSel(m_str);

	if (!g_transformObject && !g_multipleView->m_lMouseDown)
	{
		if (g_currentTransformType == eCTranslate)
		{
			g_transformObject = CTrue;
			for (CUInt i = 0; i < g_instancePrefab.size(); i++)
			{
				if (g_instancePrefab[i]->GetNameIndex() == g_selectedName)
				{
					g_arrowPosition.y = val;
					g_instancePrefab[i]->UpdateArrow(CTrue);
					break;
				}
			}
			g_transformObject = CFalse;
		}
		else if (g_currentTransformType == eCRotate)
		{
			g_transformObject = CTrue;
			for (CUInt i = 0; i < g_instancePrefab.size(); i++)
			{
				if (g_instancePrefab[i]->GetNameIndex() == g_selectedName)
				{
					g_arrowRotate.y = val;
					g_transformDirection = ZTRANS;
					g_instancePrefab[i]->UpdateArrow(CTrue);
					break;
				}
			}
			g_transformObject = CFalse;
		}
		else if (g_currentTransformType == eCScale)
		{
			g_transformObject = CTrue;
			for (CUInt i = 0; i < g_instancePrefab.size(); i++)
			{
				if (g_instancePrefab[i]->GetNameIndex() == g_selectedName)
				{
					g_arrowScale.y = val;
					g_transformDirection = YTRANS;
					g_instancePrefab[i]->UpdateArrow(CTrue);
					break;
				}
			}
			g_transformObject = CFalse;
		}
	}

}


void CVandaEngine1Dlg::OnEnChangeEditZ()
{
	CString m_str;
	m_editZ.GetWindowTextA(m_str);
	CFloat val = atof(m_str);
	//selected object
	COLORREF color = COLOR_WHITE;
	CHARFORMAT cf;
	cf.dwMask = CFM_COLOR/* | CFM_SIZE*/;
	cf.dwEffects = NULL;
	cf.crTextColor = color;

	CInt nSel = m_editZ.GetWindowTextLength();
	m_editZ.SetSel(0, nSel);

	m_editZ.SetSelectionCharFormat(cf);
	m_editZ.ReplaceSel(m_str);

	if (!g_transformObject && !g_multipleView->m_lMouseDown)
	{
		if (g_currentTransformType == eCTranslate)
		{
			g_transformObject = CTrue;
			for (CUInt i = 0; i < g_instancePrefab.size(); i++)
			{
				if (g_instancePrefab[i]->GetNameIndex() == g_selectedName)
				{
					g_arrowPosition.z = val;
					g_instancePrefab[i]->UpdateArrow(CTrue);
					break;
				}
			}
			g_transformObject = CFalse;
		}
		else if (g_currentTransformType == eCRotate)
		{
			g_transformObject = CTrue;
			for (CUInt i = 0; i < g_instancePrefab.size(); i++)
			{
				if (g_instancePrefab[i]->GetNameIndex() == g_selectedName)
				{
					g_arrowRotate.x = val;
					g_transformDirection = XTRANS;
					g_instancePrefab[i]->UpdateArrow(CTrue);
					break;
				}
			}
			g_transformObject = CFalse;
		}
		else if (g_currentTransformType == eCScale)
		{
			g_transformObject = CTrue;
			for (CUInt i = 0; i < g_instancePrefab.size(); i++)
			{
				if (g_instancePrefab[i]->GetNameIndex() == g_selectedName)
				{
					g_arrowScale.z = val;
					g_transformDirection = ZTRANS;
					g_instancePrefab[i]->UpdateArrow(CTrue);
					break;
				}
			}
			g_transformObject = CFalse;
		}
	}

}



void CVandaEngine1Dlg::OnBnClickedBtnWaterAttach()
{
	if (g_selectedName == -1)
	{
		MessageBox("Please select a prefab instance", "VandaEngine Error", MB_OK | MB_ICONINFORMATION);
		return;
	}
	else if (!g_menu.m_geometryBasedSelection)
	{
		MessageBox("Water attachment does not work with material based selection.\nPlease switch to geometry based selection and try again!", "VandaEngine Error", MB_OK | MB_ICONERROR);
		return;
	}
	else if (g_selectedName != -1)
	{
		for (CUInt i = 0; i < g_instancePrefab.size(); i++)
		{
			if (g_instancePrefab[i]->GetNameIndex() == g_selectedName)
			{
				if (g_instancePrefab[i]->GetIsTrigger())
				{
					MessageBox("You cannot attach triggers to waters.", "Vanda Engine Error", MB_OK | MB_ICONINFORMATION);
					return;
				}
			}
		}
	}
	else
	{
		if (g_multipleView->m_enableTimer)
			g_multipleView->EnableTimer(CFalse);
		OnMenuClickedWaterAttachment();
		if (g_multipleView->m_enableTimer)
			g_multipleView->EnableTimer(CTrue);

		g_multipleView->SetElapsedTimeFromBeginning();
		g_multipleView->RenderWindow();
	}
}


void CVandaEngine1Dlg::OnBnClickedBtnTrigger()
{
	if (g_multipleView->m_enableTimer)
		g_multipleView->EnableTimer(CFalse);
	m_dlgAddTrigger = CNew(CAddTrigger);
	m_dlgAddTrigger->DoModal();
	CDelete(m_dlgAddTrigger);

	if (g_multipleView->m_enableTimer)
		g_multipleView->EnableTimer(CTrue);

	g_multipleView->SetElapsedTimeFromBeginning();

}
