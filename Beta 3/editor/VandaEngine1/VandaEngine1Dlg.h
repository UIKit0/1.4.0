//Copyright (C) 2015 Zehne Ziba Co., Ltd.
//This file is licensed and distributed under MIT license

// VandaEngine1Dlg.h : header file
//
#pragma once
#pragma warning(disable:4311)
#pragma warning(disable:4312)
#include "resource.h"
#include "afxcmn.h"
#include "graphicsEngine/PerspectiveWindow.h"
#include "graphicsEngine/OpenGLUtility.h"
#include "graphicsEngine/render.h"
#include "common/vector.h"
#include "PrefabDlg.h"
#include "SavePrefabDlg.h"
#include "AddTrigger.h"
#include "AddLight.h"
#include "AddWater.h"
#include "AddScript.h"
#include "AddSkyDome.h"
#include "AddAmbientSound.h"
#include "AddStaticSound.h"
#include "GeneralAmbientColor.h"
#include "EditMaterial.h"
#include "EditPhysX.h"
#include "EditDOF.h"
#include "EditFog.h"
#include "EditBloom.h"
#include "EditOptions.h"
#include "EditLight.h"
#include "EditSceneManager.h"
#include "EditGeneralPhysXProperties.h"
#include "EditProjectProperties.h"
#include "SetCurrentProject.h"
#include "EditProjectVScenes.h"
#include "EditSceneOptions.h"
#include "EditCurrentSceneOptions.h"
#include "EditVSceneName.h"
#include "WelcomeDialog.h"
#include "ShadowDialog.h"
#include "Console.h"
#include "waterAttachment.h"
#include "selectCamera.h"
#include "graphicsEngine/water.h"
#include "graphicsEngine/sky.h"
#include <vector>
#include "afxwin.h"
#include "AudioEngine/openAL.h"
#include "AudioEngine/OpenALSystem.h"
#include "AudioEngine/OpenALSoundBuffer.h"
#include "AudioEngine/OpenALSoundSource.h"
#include "AudioEngine/StaticSound.h"
#include "scenemanagerEngine/octree.h"
#include "PhysX.h"
#include "physXEngine/Trigger.h"
#include "SceneBanner.h"
#include "PublishProject.h"
#include "CustomButton.h"
#include "CustomBitmapButton.h"
#include "common/luaforcpp.h"
#include "Common/Prefab.h"
#include "sceneProperties.h"
#include "afxext.h"
class CScene;

struct COptions
{
	CInt m_numSamples;
	CInt m_anisotropy;
	CBool m_showStartupDialog;
	CBool m_disableVSync;
	CBool m_enableWaterReflection;
	CBool m_enableShader;
	CBool m_enableFBO;
	CBool m_enableVBO;

	COptions()
	{
		m_numSamples = 4;
		m_anisotropy = 4;
		m_showStartupDialog = CTrue;
		m_disableVSync = CTrue;
		m_enableWaterReflection = CTrue;
		m_enableShader = CTrue;
		m_enableFBO = CTrue;
		m_enableVBO = CTrue;
	}

	CVoid Reset()
	{
		m_numSamples = 4;
		m_anisotropy = 4;
		m_showStartupDialog = CTrue;
		m_disableVSync = CTrue;
		m_enableWaterReflection = CTrue;
		m_enableShader = CTrue;
		m_enableFBO = CTrue;
		m_enableVBO = CTrue;
	}

};

struct CProjects
{
	CChar m_name[MAX_NAME_SIZE];
	std::vector<std::string> m_sceneNames;
	CBool m_isActive;
};

struct CExtraTexturesNamingConventions
{
	CChar m_diffuseNameExcept[MAX_NAME_SIZE];
	CChar m_glossMapName[MAX_NAME_SIZE];
	CChar m_normalMapName[MAX_NAME_SIZE];
	CChar m_dirtMapName[MAX_NAME_SIZE];

	CExtraTexturesNamingConventions()
	{
		Cpy( m_diffuseNameExcept, "" );
		Cpy( m_glossMapName, "_g" );
		Cpy( m_normalMapName, "_n" );
		Cpy( m_dirtMapName, "_d" );
	}

	CVoid Reset()
	{
		Cpy( m_diffuseNameExcept, "" );
		Cpy( m_glossMapName, "_g" );
		Cpy( m_normalMapName, "_n" );
		Cpy( m_dirtMapName, "_d" );
	}
};

struct CPathProperties
{
	CChar m_meshPath[MAX_NAME_SIZE];
	CChar m_meshDiffuseTexturePath[MAX_NAME_SIZE];
	CChar m_meshOtherTexturesPath[MAX_NAME_SIZE];
	CChar m_waterPath[MAX_NAME_SIZE];
	CChar m_skyPath[MAX_NAME_SIZE];
	CChar m_soundPath[MAX_NAME_SIZE];
	CChar m_physXPath[MAX_NAME_SIZE];

	CPathProperties()
	{
		Cpy( m_meshOtherTexturesPath, "Assets/Textures/Extra/" );
		Cpy( m_meshDiffuseTexturePath, "Assets/Textures/Diffuse/" );
		Cpy( m_skyPath, "Assets/Engine/Textures/Sky/" );
		Cpy( m_waterPath, "Assets/Engine/Textures/Water/" );
		Cpy( m_meshPath, "Assets/Meshes/" );
		Cpy( m_soundPath, "Assets/Engine/Sounds/" );
		Cpy( m_physXPath, "Assets/Engine/PhysX/" );
	}

	CVoid Reset()
	{
		Cpy( m_meshOtherTexturesPath, "Assets/Textures/Extra/" );
		Cpy( m_meshDiffuseTexturePath, "Assets/Textures/Diffuse/" );
		Cpy( m_skyPath, "Assets/Engine/Textures/Sky/" );
		Cpy( m_waterPath, "Assets/Engine/Textures/Water/" );
		Cpy( m_meshPath, "Assets/Meshes/" );
		Cpy( m_soundPath, "Assets/Engine/Sounds/" );
		Cpy( m_physXPath, "Assets/Engine/PhysX/" );
	}

};

struct CLightProperties
{
	CFloat m_ambientColor[4];
	CFloat m_specularColor[4];
	CFloat m_shininess;
	CFloat m_tolerance;

	CLightProperties()
	{
		for(CInt i = 0; i < 3; i++ )
		{
			m_ambientColor[i] = 1.0f;
			m_specularColor[i] = 0.6f;
		}
		m_ambientColor[3] = m_specularColor[3] = 1.0;
		m_tolerance = 0.2f;
		m_shininess = 50.0f;
	}

	CVoid Reset()
	{
		for(CInt i = 0; i < 3; i++ )
		{
			m_ambientColor[i] = 1.0f;
			m_specularColor[i] = 0.6f;
		}
		m_ambientColor[3] = m_specularColor[3] = 1.0;
		m_tolerance = 0.2f;
		m_shininess = 50.0f;
	}

};

struct CShadowProperties
{
	CFloat m_shadowSplitWeight;
	CFloat m_shadowNearClipPlane;
	CFloat m_shadowFarClipPlane;
	CShadowType m_shadowType;
	CShadowSplits m_shadowSplits;
	CShadowResolution m_shadowResolution;
	CFloat m_intensity;
	CBool m_enable;
	CChar m_directionalLightName[MAX_NAME_SIZE];

	CShadowProperties()
	{
		m_shadowSplitWeight = 0.6f;
		m_shadowNearClipPlane = 0.1f;
		m_shadowFarClipPlane = 60.f;
		m_shadowType = eSHADOW_PCF_GAUSSIAN;
		m_shadowSplits = eSHADOW_3_SPLITS; //3 splits
		m_shadowResolution = eSHADOW_2048;
		m_intensity = 0.5f;
		m_enable = CFalse;
		Cpy( m_directionalLightName, "\n" );
	}

	CVoid Reset()
	{
		m_shadowSplitWeight = 0.3f;
		m_shadowNearClipPlane = 0.1f;
		m_shadowFarClipPlane = 30.f;
		m_shadowType = eSHADOW_PCF_GAUSSIAN;
		m_shadowSplits = eSHADOW_3_SPLITS; //3 splits
		m_shadowResolution = eSHADOW_1024;
		m_intensity = 0.5f;
		m_enable = CFalse;
		Cpy( m_directionalLightName, "\n" );
	}

};

struct CPhysXProperties
{
	CFloat m_fDefaultRestitution;
	CFloat m_fDefaultStaticFriction;
	CFloat m_fDefaultSkinWidth;
	CFloat m_fDefaultDynamicFriction;
	CFloat m_fGroundHeight;
	CFloat m_fGravityX;
	CFloat m_fGravityY;
	CFloat m_fGravityZ;
	CFloat m_fCameraCharacterDistance;
	CFloat m_fCapsuleRadius;
	CFloat m_fCapsuleHeight;
	CFloat m_fCharacterPower;
	CFloat m_fCharacterSpeed;
	CFloat m_fCharacterSlopeLimit;
	CFloat m_fCharacterStepOffset;
	CFloat m_fCharacterSkinWidth;
	CFloat m_fJumpPower;
	CBool m_bDebugMode;
	CBool m_bJumping;
	CBool m_bApplyGravity;
	CBool m_bGroundPlane;

	CPhysXProperties()
	{
		m_fDefaultRestitution = 0.3f;
		m_fDefaultStaticFriction = 0.5f;
		m_fDefaultDynamicFriction = 0.5f;
		m_fDefaultSkinWidth = 0.01f;
		m_fGroundHeight = 0.0f;
		m_fGravityX = 0.0f;
		m_fGravityY = -9.8f;
		m_fGravityZ = 0.0f;
		m_fCameraCharacterDistance = 3.0f;
		m_fCapsuleRadius = 0.4f;
		m_fCapsuleHeight = 1.8f;
		m_fCharacterPower = 3.0f;
		m_fCharacterSpeed = 2.0f;
		m_fJumpPower = 35.0f;
		m_fCharacterSlopeLimit = 45.0f;
		m_fCharacterStepOffset = 0.1f;
		m_fCharacterSkinWidth = 0.1f;

		m_bDebugMode = m_bJumping = m_bApplyGravity = CFalse;
		m_bGroundPlane = CTrue;
	}

	CVoid Reset()
	{
		m_fDefaultRestitution = 0.3f;
		m_fDefaultStaticFriction = 0.5f;
		m_fDefaultDynamicFriction = 0.5f;
		m_fDefaultSkinWidth = 0.01f;
		m_fGroundHeight = 0.0f;
		m_fGravityX = 0.0f;
		m_fGravityY = -9.8f;
		m_fGravityZ = 0.0f;
		m_fCameraCharacterDistance = 3.0f;
		m_fCapsuleRadius = 0.4f;
		m_fCapsuleHeight = 1.8f;
		m_fCharacterPower = 3.0f;
		m_fCharacterSpeed = 2.0f;
		m_fJumpPower = 35.0f;
		m_fCharacterSlopeLimit = 45.0f;
		m_fCharacterStepOffset = 0.1f;
		m_fCharacterSkinWidth = 0.1f;

		m_bDebugMode = m_bJumping = m_bApplyGravity = CFalse;
		m_bGroundPlane = CTrue;
	}
};

struct CDOFProperties
{
	CBool m_enable;
	CFloat m_dofFocalDistance;
	CFloat m_dofFocalRange;
	CBool m_debug;

	CDOFProperties()
	{
		m_dofFocalDistance = 5.0f;
		m_dofFocalRange = 3.0f;
		m_enable = CFalse;
		m_debug = CFalse;
	}

	CVoid Reset()
	{
		m_dofFocalDistance = 5.0f;
		m_dofFocalRange = 3.0f;
		m_enable = CFalse;
		m_debug = CFalse;
	}

};

struct CFogProperties
{
	CBool m_enable;
	CFloat m_fogDensity;
	CFloat m_fogColor[4];

	CFogProperties()
	{
		m_enable = CFalse;
		m_fogDensity = 0.1f;
		for( CInt i = 0; i < 4; i++ )
		{
			if( i < 3 )
				m_fogColor[i] = 0.5f;
			else
				m_fogColor[i] = 1.0f;
		}
	}

	CVoid Reset()
	{
		m_enable = CFalse;
		m_fogDensity = 0.1f;
		for( CInt i = 0; i < 4; i++ )
		{
			if( i < 3 )
				m_fogColor[i] = 0.5f;
			else
				m_fogColor[i] = 1.0f;
		}
	}

};

struct CBloomProperties
{
	CBool m_enable;
	CFloat m_bloomIntensity;
	CFloat m_bloomColor[3];

	CBloomProperties()
	{
		m_enable = CTrue;
		m_bloomIntensity = 1.0f;
		for( CInt i = 0; i < 3; i++ )
		{
			m_bloomColor[i] = 1.0f;
		}
	}

	CVoid Reset()
	{
		m_enable = CTrue;
		m_bloomIntensity = 1.0f;
		for( CInt i = 0; i < 3; i++ )
		{
			m_bloomColor[i] = 1.0f;
		}
	}

};

struct CMenuVariables
{
	CBool m_justPerspective;
	CBool m_useShader; //deprecate; Moved to COptions::m_enableShader
	CBool m_multisampling; //deprecate; Moved to COptions::m_numSamples
	CBool m_fog; //deprecate; Moved to CFogProperties::m_enable
	CBool m_dof; //deprecate; Moved to CDOFProperties::m_enable
	CBool m_showPerspectiveGrids;
	CBool m_showLightIcons;
	CBool m_showSoundIcons;
	CBool m_showWaterIcons;
	CBool m_showSkyIcon;
	CBool m_showBoundingBox;
	CBool m_showOctree;
	CBool m_showDynamicShadowDepthTexture;
	CBool m_enableDynamicShadow; //deprecate; Moved to CShadowProperties::m_enable
	CBool m_insertAndShowSky;
	CBool m_insertAmbientSound;
	CBool m_playAmbientSound;
	CBool m_geometryBasedSelection;
	CBool m_showStatistics;
	CBool m_showCameraIcons;
	CMenuVariables()
	{
		m_justPerspective = CFalse;  
		m_useShader = CTrue;
		m_multisampling = CTrue;
		m_fog = CFalse;
		m_dof = CFalse;

		m_showPerspectiveGrids = CTrue;
		m_showLightIcons = CTrue;
		m_showSoundIcons = CTrue;
		m_showWaterIcons = CTrue;
		m_showSkyIcon = CTrue;
		m_showCameraIcons = CTrue;
		m_showBoundingBox = CFalse;
		m_showOctree = CFalse;
		m_showDynamicShadowDepthTexture = CFalse;
		m_enableDynamicShadow = CTrue;
		m_insertAndShowSky = CFalse;
		m_insertAmbientSound = CFalse;
		m_playAmbientSound = CFalse;
		m_geometryBasedSelection = CTrue;

		m_showStatistics = CFalse;
	}
};
// CVandaEngine1Dlg dialog
class CVandaEngine1Dlg : public CDialog
{
// Construction
public:
	CVandaEngine1Dlg(CWnd* pParent = NULL);	// standard constructor
	~CVandaEngine1Dlg();
// Dialog Data
	enum { IDD = IDD_VANDAENGINE_DIALOG };

	protected:
	virtual CVoid DoDataExchange(CDataExchange* pDX);	// DDX/DDV support


// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	virtual BOOL OnInitDialog();
	afx_msg CVoid OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg CVoid OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()

	CBrush m_brush; // used to specify the colors of controls

	CToolTipCtrl* m_pToolTip;
public:
//	afx_msg CVoid OnClose();
public:
	CRichEditCtrl m_rich;
	CBool OnMenuClickedNew( CBool askQuestion );
	CBool OnMenuClickedOpenVScene( CBool askQuestion = CTrue);
	CBool OnMenuClickedOpenPrefab();
	CBool OnMenuClickedInsertPrefab(CPrefab* prefab = NULL, CChar* packageName = NULL, CChar* prefabName = NULL); //if prefab is not null, we are loading from vin file
	CVoid OnMenuClickedSaveAs(CBool askQuestion = CTrue);
	CVoid OnMenuClickedSavePrefabAs(CBool askQuestion = CTrue);

protected:
	virtual BOOL OnCommand(WPARAM wParam, LPARAM lParam);
	CVoid OnMenuClickedImportCollada();
	CVoid OnMenuClickedImportPhysX(); //removed in version 1.4 or later
	CVoid OnMenuClickedInsertLight();
	CVoid OnMenuClickedInsertStaticSound();
	CVoid OnMenuClickedInsertSkyDome();
	CVoid OnMenuClickedWaterAttachment();
	CVoid OnMenuClickedInsertWater();
	CVoid OnMenuClickedInsertAmbientSound();
	CVoid OnMenuClickedSelectCamera();
	CVoid OnMenuClickedGeneralOptions();
	CVoid OnMenuClickedSceneOptions();
	CVoid OnMenuClickedCurrentSceneOptions();
	CVoid OnMenuClickedEditMaterial();
	CVoid OnMenuClickedEditPhysX();
	CVoid OnMenuClickedPrefab();
	CVoid OnMenuClickedGeneralAmbientColor();
	CVoid SortButtons();


	CString m_fileName;
	CString m_savePathName; //It's used for save button
	CChar m_strNewPackageName[MAX_NAME_SIZE];
	CChar m_strNewPrefabName[MAX_NAME_SIZE];
	CChar m_strNewPackageAndPrefabName[MAX_NAME_SIZE];
	CChar m_strNewPassword[MAX_NAME_SIZE];
	CBool m_askRemoveEngineObject;
	CBool m_askRemoveScene;
public:
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
public:
	CVoid OnBnClickedCullFace();

public:
	afx_msg CVoid OnBnClickedBtnRemoveScene();
	CVoid SortSceneList(CInt selectedIndex);
	CVoid SortPhysXList();
public:
	afx_msg CVoid OnBnClickedBtnRemoveObject();
public:
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
public:

	CVoid InsertItemToSceneList( CChar * sceneName );
	CVoid InsertItemToObjectList( char * objectName, int imageIndex = 0 );
	CVoid InserItemToEngineObjectList( char * objectName, int imageIndex  = 0 );
	CVoid InsertItemToPhysXList( CChar * physXObjectName, int imageIndex = 0 );

	COpenALSoundBuffer* GetSoundBuffer( const CChar * name );
	CInt m_engineObjectListIndex;
	CInt m_sceneListIndex;
	CInt m_objectListIndex;
	CInt m_physXElementListIndex;
	CImageList m_objectListImage;
	CImageList m_engineObjectListImage;
	CImageList m_physXElementListImage;

	CVoid ChangeLightProperties(CInstanceLight* light );
	CVoid ChangeWaterProperties( CWater* water);
	CVoid ChangeStaticSoundProperties(CStaticSound* staticSound);
	CVoid ChangeAmbientSoundProperties();
	CVoid ChangeSkyDomeProperties();
public:
	CWaterAttachment* m_dlgWaterAttachment;
	CAddTrigger* m_dlgAddTrigger;
	CAddLight* m_dlgAddLight;
	CAddWater* m_dlgAddWater;
	CPublishProject* m_dlgPublishProject;
	CAddStaticSound* m_dlgAddStaticSound;
	CAddSkyDome* m_dlgAddSkyDome;
	CAddAmbientSound* m_dlgAddAmbientSound;
	CGeneralAmbientColor* m_dlgGeneralAmbientColor;
	CSelectCamera* m_dlgSelectCamera;
	CEditMaterial* m_dlgEditMaterial;
	CEditPhysX* m_dlgEditPhysX;
	CAddScript* m_dlgAddScript;
	CEditBloom* m_dlgEditBloom;
	CEditLight* m_dlgEditLight;
	CEditSceneManager* m_dlgEditSceneManager;
	CEditGeneralPhysXProperties* m_dlgEditGeneralPhysXProperties;
	CEditShadow* m_dlgEditShadow;
	CEditOptions* m_dlgOptions;
	CEditSceneOptions* m_dlgSceneOptions;
	CEditCurrentSceneOptions* m_dlgCurrentSceneOptions;
	CWelcomeDialog m_dlgWelcome;
	CEditDOF* m_dlgEditDOF;
	CEditFog* m_dlgEditFog;
	CConsole* m_dlgConsole;
	CPrefabDlg* m_dlgPrefabs;
	CSavePrefabDlg *m_dlgSavePrefabs;
protected:
	virtual void OnCancel();
public:
	CListCtrl m_listBoxScenes;
public:
	CListCtrl m_listBoxObjects;
public:
	CCustomBitmapButton m_btnRemoveScene;
public:
	CCustomBitmapButton m_btnRemoveObject;
public:
	CListCtrl m_listBoxEngineObjects;
public:
	CCustomBitmapButton m_btnRemoveEngineObject;
public:
	CCustomBitmapButton m_btnEngineObjectProperties;
public:
	afx_msg void OnBnClickedBtnRemoveEngineObject();
	CVoid SortEngineObjectList(CInt selectedIndex);
public:
	afx_msg void OnBnClickedBtnEngineObjectProperties();
public:
	afx_msg void OnClose();
public:
	afx_msg BOOL CVandaEngine1Dlg::PreTranslateMessage(MSG* pMsg);
public:
	afx_msg void OnBnClickedBtnNew();
public:
	afx_msg void OnBnClickedBtnSave();
public:
	CCustomBitmapButton m_mainBtnSave;
public:
	CCustomBitmapButton m_mainBtnImportCollada;
public:
	CCustomBitmapButton m_mainBtnNew;
public:
	afx_msg void OnBnClickedBtnCollada();
//public:
	//CCustomBitmapButton m_mainBtnImportPhysX; removed in version 1.4 or later
//public:
	//afx_msg void OnBnClickedBtnPhysx(); 
public:
	CCustomBitmapButton m_mainBtnPrevAnim;
public:
	afx_msg void OnBnClickedBtnAnimPrev();
public:
	CCustomBitmapButton m_mainBtnOpen;
public:
	afx_msg void OnBnClickedBtnOpen();
public:
	CCustomBitmapButton m_mainBtnSaveAs;
public:
	afx_msg void OnBnClickedBtnSaveas();
public:
	CCustomBitmapButton m_mainBtnPlayAnim;
public:
	CCustomBitmapButton m_mainBtnNextAnim;
public:
	afx_msg void OnBnClickedBtnAnimNext();
public:
	afx_msg void OnBnClickedBtnAnimPlay();
public:
	afx_msg BOOL OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message);
public:
	afx_msg void OnTimer(UINT_PTR nIDEvent);
public:
	CCustomBitmapButton m_mainBtnTimer;
public:
	CCustomBitmapButton m_mainBtnMaterial;
public:
	afx_msg void OnBnClickedBtnTimer();
public:
	CCustomBitmapButton m_mainBtnPauseAnim;
public:
	afx_msg void OnBnClickedBtnAnimPause();
public:
	CCustomBitmapButton m_mainBtnWeb;
public:
	afx_msg void OnBnClickedBtnWeb();
public:
	CCustomBitmapButton m_mainBtnFacebook;
public:
	afx_msg void OnBnClickedBtnFacebook();
	CRichEditCtrl m_staticSelectedObject;
	afx_msg void OnBnClickedBtnMaterial();
	CCustomBitmapButton m_mainBtnSky;
	CCustomBitmapButton m_mainBtnWater;
	CCustomBitmapButton m_mainBtnStaticSound;
	CCustomBitmapButton m_mainBtnAmbientSound;
	CCustomBitmapButton m_mainBtnPlayer;
	CCustomBitmapButton m_mainBtnLight;
	afx_msg void OnBnClickedBtnLight();
	afx_msg void OnBnClickedBtnWater();
	afx_msg void OnBnClickedBtnAmbientsound();
	afx_msg void OnBnClickedBtnStaticsound();
	afx_msg void OnBnClickedBtnSky();
	afx_msg void OnBnClickedBtnPlayer();

	CInt m_horizontalSizeOfLeftCulomn;
	CInt m_horizontalSizeOfRightCulomn;
	CInt m_horizontalPointFivePercent;
	CInt m_startLeftButtons;
	long previousColumn1, previousColumn2, previousColumn3, previousColumn4;
	CCustomBitmapButton m_mainBtnConsole;
	afx_msg void OnBnClickedBtnConsole();
//	afx_msg void OnHdnItemclickListScenes(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnLvnItemchangedListEngineObjects(NMHDR *pNMHDR, LRESULT *pResult);

	void ResetPhysX(CBool releaseActors = CTrue);
	CListCtrl m_listBoxPhysXElements;
	CCustomBitmapButton m_btnRemovePhysX;
	afx_msg void OnBnClickedBtnRemovePhysx();
	CCustomBitmapButton m_btnVandaText;
	CCustomBitmapButton m_mainBtnTwitter;
	afx_msg void OnBnClickedBtnTwitter();
	CCustomBitmapButton m_mainBtnYoutube;
	afx_msg void OnBnClickedBtnYoutube();
	afx_msg void OnLvnItemchangedListPhysxElements(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnBnClickedBtnPublishSolution();
	afx_msg void OnBnClickedBtnScriptManager();
	CCustomBitmapButton m_mainBtnPublishSolution;
	CCustomBitmapButton m_mainBtnScriptManager;
	CCustomBitmapButton m_mainBtnCameraAndRenderingManager;
	afx_msg void OnBnClickedBtnCameraRenderingManager();
	CCustomBitmapButton m_btnVandaVersion;
	CCustomBitmapButton m_btnSceneProperties;
	afx_msg void OnBnClickedBtnSceneProperties();
	CCustomBitmapButton m_mainBtnPhysXEditor;
	afx_msg void OnBnClickedBtnPhysxEditor();
	afx_msg void OnNcLButtonDblClk(UINT nHitTest, CPoint point);
	afx_msg void OnLvnItemchangedListScenes(NMHDR *pNMHDR, LRESULT *pResult);
	CCustomBitmapButton m_mainBtnPrefab;
	afx_msg void OnBnClickedBtnPrefabs();
	void RemoveSelectedScene(CChar* szBuffer, CChar* sceneId = NULL);
	CCustomBitmapButton m_mainBtnTranslate;
	CCustomBitmapButton m_mainBtnRotate;
	CCustomBitmapButton m_mainBtnScale;
	CCustomBitmapButton m_mainBtnTimerPause;
	afx_msg void OnBnClickedBtnTimerPause();
	CCustomBitmapButton m_mainBtnTestActive;
	CCustomBitmapButton m_mainBtnTestDeactive;
	afx_msg void OnBnClickedBtnPlayActive();
	afx_msg void OnBnClickedBtnPlayDeactive();
	afx_msg void OnBnClickedBtnTranslate();
	afx_msg void OnBnClickedBtnRotate();
	afx_msg void OnBnClickedBtnScale();
	CRichEditCtrl m_editX;
	CRichEditCtrl m_editY;
	CRichEditCtrl m_editZ;
	afx_msg void OnEnChangeEditX();
	afx_msg void OnEnChangeEditY();
	afx_msg void OnEnChangeEditZ();
	CCustomBitmapButton m_mainBtnWaterAttach;
	afx_msg void OnBnClickedBtnWaterAttach();
	CCustomBitmapButton m_mainBtnTrigger;
	afx_msg void OnBnClickedBtnTrigger();
};

//Edition.MaxVersion.MinVersion.BugFixes;
//For example: RTI.1.0.0
extern CInt g_maxVersion;
extern CInt g_minVersion;
extern CInt g_bugFixesVersion;
extern CChar g_edition[MAX_NAME_SIZE];
extern CBool g_useOriginalPathOfDAETextures;
extern CBool g_updateTextureViaEditor;
extern CChar g_currentVSceneName[MAX_NAME_SIZE]; //save functions
extern std::vector<std::string> g_VSceneNamesOfCurrentProject; //save functions
extern std::vector<std::string> g_VSceneNamesOfCurrentProjectToBePublished; //save functions
extern std::vector<std::string> g_allPrefabNames;
extern std::string g_firstSceneNameToBeLoaded; //save functions
extern CChar g_projectsPath[MAX_NAME_SIZE];
extern CChar g_currentProjectPath[MAX_NAME_SIZE];
extern CChar g_currentPackageAndPrefabName[MAX_NAME_SIZE]; //save functions
extern CChar g_currentPackageName[MAX_NAME_SIZE]; //save functions
extern CChar g_currentPrefabName[MAX_NAME_SIZE]; //save functions
extern CChar g_currentPackagePath[MAX_NAME_SIZE]; //save functions
extern CChar g_currentPrefabAndSceneName[MAX_NAME_SIZE]; //save functions
extern CChar g_currentInstancePrefabName[MAX_NAME_SIZE];
extern CTFormType g_currentTransformType;
extern CBool g_shareGeometriesBetweenScenes;
extern CBool g_openNewPrefabFromList;
extern CScene* g_currentScene; //current scene that we are working on it.
extern CInstancePrefab* g_currentInstancePrefab;
extern CChar g_currentPassword[MAX_NAME_SIZE];
extern CBool g_vandaDemo;
extern CBool g_importCOLLADA;
extern CBool g_openVINFile;
extern CChar g_fileNameInCommandLine[MAX_NAME_SIZE];

extern 	std::vector<COpenALSoundBuffer*>g_soundBuffers;
extern CRichEditCtrl *ex_pRichEdit; // rich edit pointer
extern CRichEditCtrl *ex_pRichEditScript;
extern CVandaEngine1Dlg* ex_pVandaEngine1Dlg;
extern CPrefabDlg* ex_pDlgPrefabs;
extern CRichEditCtrl *ex_pStaticSelectedObject;

extern CMenu *ex_pMenu; //pointer to menu ( used for cull face check mark )

extern CEditMaterial *ex_pEditMaterial; //extern
extern CEditPhysX *ex_pEditPhysX;
extern CAddScript* ex_pAddScript;
extern CButton* ex_pBtnPrevAnim;
extern CButton* ex_pBtnPlayAnim;
extern CButton* ex_pBtnPauseAnim;
extern CButton* ex_pBtnNextAnim;
extern CButton* ex_pBtnTimerPlay;
extern CButton* ex_pBtnTimerPause;
extern CButton* ex_pBtnScriptEditor;
extern CButton* ex_pBtnTestModeActive;
extern CButton* ex_pBtnTestModeDeactive;
extern CButton* ex_pBtnPhysXEditor;
extern std::vector<CScene*> g_scene;
extern std::vector<CPrefab*> g_prefab;
extern std::vector<CGeometry *> g_geometries;
extern std::vector<CInstancePrefab*>g_instancePrefab;
extern CBool g_importPrefab;
extern CScene* g_translateArrow;
extern CScene* g_scaleArrow;
extern CScene* g_rotateArrow;
extern CScene* g_negativeZArrow;
extern CScene* g_centerArrowScene;
extern CVec3f g_arrowPosition;
extern CVec3f g_arrowRotate;
extern CVec3f g_arrowScale;

extern CBool g_showArrow;
extern COpenGLUtility g_glUtil;
extern CImage* g_soundImg;
extern CImage* g_pointLightImg;
extern CImage* g_directionalLightImg;
extern CImage* g_spotLightImg;
extern CImage* g_pointLightCOLLADAImg;
extern CImage* g_directionalLightCOLLADAImg;
extern CImage* g_spotLightCOLLADAImg;
extern CImage* g_skyImg;
extern CImage* g_waterImg;
extern CImage* g_cameraImg;
extern CColor4f g_globalAmbientColor;
extern CBool g_useGlobalAmbientColor;
extern CVec4f g_defaultDirectionalLight;
extern std::vector<CInstanceLight*> g_engineLights; //these aren't collada lights, they are specified via the editor
extern std::vector<CTrigger*> g_triggers;
extern std::vector<CWater*> g_engineWaters; //We may use multiple water surfaces in our engine
extern std::vector<CInstanceCamera*> g_cameraInstances;
extern std::vector<CStaticSound*> g_engineStaticSounds;
extern COpenALSystem* g_soundSystem;
extern std::vector<std::string> g_engineObjectNames;
extern CUpdateCamera *g_camera;
extern GLuint g_shaderType;
extern CSkyDome *g_skyDome;   //Currently each map has just one sky, But I may decide to add extra layers later
extern NxExtendedVec3 g_characterPos; //default camera attached to character controller
extern CVec3f g_cameraPitchYawTilt; //default camera attached to character controller
extern CVec3f g_cameraInstancePos; //default free camera
extern CVec2f g_cameraInstancePanTilt; //default free camera
extern 	CBloom*	g_bloom;
extern 	CExternalPhysX*  g_externalPhysX;
extern CSceneBanner g_sceneBanner;
extern std::vector<CImage*>g_images; // This variable holds the information of all the images of Engine
extern std::vector<CImage*>g_waterImages; // This variable holds the information of water images of Engine
extern CShadowMap* g_dynamicShadowMap;
extern COctree* g_octree;
extern CMultipleWindows *g_multipleView;
extern CInt numErrors, numWarnings, totalErrors, totalWarnings, ReportLineCounter; //number of warning, errors and lines used by richedit 22

extern CBool g_updateOctree;
extern CBool g_useOldRenderingStyle;
extern CFloat g_elapsedTime;
extern CInt g_width;
extern CInt g_height;
extern CMenuVariables g_menu;
extern CShadowProperties g_shadowProperties;
extern CPhysXProperties g_physXProperties;
extern CDOFProperties g_dofProperties;
extern CLightProperties g_lightProperties;
extern CFogProperties g_fogProperties;
extern CPathProperties g_pathProperties;
extern CExtraTexturesNamingConventions g_extraTexturesNamingConventions;
extern CBloomProperties g_bloomProperties;
extern COptions g_options;
extern std::vector<CProjects*> g_projects;
extern CPolygonMode g_polygonMode;
extern CBool g_firstPass;
extern CMaterialChannels g_materialChannels;
extern CInstanceLight* g_currentInstanceLight;
extern CInt g_sceneManagerObjectsPerSplit;
extern CLuaState g_lua;
extern CBool g_testScript;
extern CBool g_renderShadow;

extern CEditorMode g_editorMode;

extern CBool g_clickedOpen;
extern CBool g_clickedNew;

extern CCameraType g_currentCameraType;
extern std::vector<std::vector<std::string>> g_prefabPackagesAndNames;

DWORD __stdcall MEditStreamOutCallback(DWORD dwCookie, LPBYTE pbBuff, LONG cb, LONG *pcb);
DWORD __stdcall MEditStreamInCallback(DWORD dwCookie, LPBYTE pbBuff, LONG cb, LONG *pcb);


CVoid CheckCullFace( CCullFaceTypeForMenu type ); // cull face check mark
CVoid SetDialogData( CBool selected, CInstanceGeometry* instanceGeo = NULL, CGeometry* geo = NULL, CBool engineObject = CFalse, CBool showDialog = CTrue );
CVoid SetDialogData2( CBool selected, CPolyGroup* group = NULL, CBool engineObject = CFalse, CBool showDialog = CTrue );
CVoid SetDialogData3(CBool selected, CInstancePrefab* instancePrefab = NULL);
