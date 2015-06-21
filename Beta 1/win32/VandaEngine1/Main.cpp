//Copyright (C) 2015 Zehne Ziba Co., Ltd.
//This file is licensed and distributed under MIT license

#include "stdafx.h"
#include "main.h"
#include "GraphicsEngine/Animation.h"

//Vanda C Functions to be used in Lua scripts
CGeometry* GetGeometryFromScenes(const CChar * name, const CChar * DocURI)
{
	for (CUInt i = 0; i<g_geometries.size(); i++)
	{
		if (ICmp(g_geometries[i]->GetName(), name) &&
			ICmp(g_geometries[i]->GetDocURI(), DocURI))
			return g_geometries[i];
	}
	return NULL;
}

CInt PlaySound(lua_State *L)
{
	int argc = lua_gettop(L);

	CBool setLoop = CFalse;
	for ( int n=1; n<=argc; ++n )
	{
		if( Cmp( "VANDA_LOOP",  lua_tostring(L, n) ) )
		{
			setLoop = CTrue;
			break;
		}
	}

	for ( int n=1; n<=argc; ++n )
	{
		for( CUInt i = 0 ; i < g_engineStaticSounds.size(); i++ )
		{
			if( Cmp( g_engineStaticSounds[i]->GetName(),  lua_tostring(L, n) ) )
			{
				g_engineStaticSounds[i]->m_source->SetLooping( setLoop );
				g_engineStaticSounds[i]->SetLoop( setLoop );
				g_engineStaticSounds[i]->SetPlay(CTrue);
				g_soundSystem->PlayALSound( *(g_engineStaticSounds[i]->m_source) );
				break;
			}
		}
	}
	return 0; // number of return values
}

CInt PauseSound(lua_State *L)
{
	int argc = lua_gettop(L);

	for ( int n=1; n<=argc; ++n )
	{
		for( CUInt i = 0 ; i < g_engineStaticSounds.size(); i++ )
		{
			if( Cmp( g_engineStaticSounds[i]->GetName(),  lua_tostring(L, n) ) )
			{
				g_engineStaticSounds[i]->SetPlay(CFalse);
				g_soundSystem->PauseALSound( *(g_engineStaticSounds[i]->m_source) );
				break;
			}
		}
	}
	return 0; // number of return values
}

CInt StopSound(lua_State *L)
{
	int argc = lua_gettop(L);

	for ( int n=1; n<=argc; ++n )
	{
		for( CUInt i = 0 ; i < g_engineStaticSounds.size(); i++ )
		{
			if( Cmp( g_engineStaticSounds[i]->GetName(),  lua_tostring(L, n) ) )
			{
				g_engineStaticSounds[i]->SetPlay(CFalse);
				g_soundSystem->StopALSound( *(g_engineStaticSounds[i]->m_source) );
				break;
			}
		}
	}
	return 0; // number of return values
}

CInt BlendCycle(lua_State *L)
{
	int argc = lua_gettop(L);
	if (argc < 5)
	{
		//PrintInfo("\nPlease specify 5 arguments for BlendCycle()", COLOR_RED);
		return 0;
	}
	CScene* scene = NULL;

	//find the scene
	CChar luaToString[MAX_NAME_SIZE];
	Cpy(luaToString, lua_tostring(L, 1));
	StringToUpper(luaToString);

	CChar luaToString2[MAX_NAME_SIZE];
	Cpy(luaToString2, lua_tostring(L, 2));
	StringToUpper(luaToString2);

	for (CUInt i = 0; i < g_scene.size(); i++)
	{
		CChar prefabName[MAX_NAME_SIZE];
		Cpy(prefabName, g_scene[i]->GetDocURI());
		StringToUpper(prefabName);

		CChar sceneName[MAX_NAME_SIZE];
		Cpy(sceneName, g_scene[i]->GetName());
		StringToUpper(sceneName);

		if (Cmp(prefabName, luaToString) && Cmp(sceneName, luaToString2))
		{
			g_scene[i]->m_animationStatus = eANIM_PLAY;
			scene = g_scene[i];
			break;
		}
	}

	if (!scene)
	{
		//CChar temp[MAX_NAME_SIZE];
		//sprintf(temp, "\n%s%s%s", "Couldn't find the scene name '", luaToString, "'");
		//PrintInfo(temp, COLOR_RED);
		return 0;
	}

	CChar luaToString3[MAX_NAME_SIZE];
	Cpy(luaToString3, lua_tostring(L, 3));
	StringToUpper(luaToString3);
	CBool foundTarget = CFalse;
	CInt index;
	for (CInt i = 0; i < scene->GetNumClips(); i++)
	{
		CChar animationName[MAX_NAME_SIZE];
		Cpy(animationName, scene->m_animationClips[i]->GetName());
		StringToUpper(animationName);
		if (Cmp(luaToString3, animationName))
		{
			index = i;
			foundTarget = CTrue;
			break;
		}
	}
	if (!foundTarget)
	{
		//CChar temp[MAX_NAME_SIZE];
		//sprintf(temp, "\n%s%s%s", "Couldn't find the animation clip '", luaToString3, "'");
		//PrintInfo(temp, COLOR_RED);
		return 0;
	}
	scene->BlendCycle(index, (CFloat)lua_tonumber(L, 4), (CFloat)lua_tonumber(L, 5));
	return 0;
}

CInt ClearCycle(lua_State *L)
{
	int argc = lua_gettop(L);
	if (argc < 4)
	{
		//PrintInfo("\nPlease specify 4 arguments for ClearCycle()", COLOR_RED);
		return 0;
	}
	CScene* scene = NULL;

	//find the scene
	CChar luaToString[MAX_NAME_SIZE];
	Cpy(luaToString, lua_tostring(L, 1));
	StringToUpper(luaToString);

	CChar luaToString2[MAX_NAME_SIZE];
	Cpy(luaToString2, lua_tostring(L, 2));
	StringToUpper(luaToString2);

	for (CUInt i = 0; i < g_scene.size(); i++)
	{
		CChar prefabName[MAX_NAME_SIZE];
		Cpy(prefabName, g_scene[i]->GetDocURI());
		StringToUpper(prefabName);

		CChar sceneName[MAX_NAME_SIZE];
		Cpy(sceneName, g_scene[i]->GetName());
		StringToUpper(sceneName);

		if (Cmp(prefabName, luaToString) && Cmp(sceneName, luaToString2))
		{
			g_scene[i]->m_animationStatus = eANIM_PLAY;
			scene = g_scene[i];
			break;
		}
	}

	if (!scene)
	{
		//CChar temp[MAX_NAME_SIZE];
		//sprintf(temp, "\n%s%s%s", "Couldn't find the scene name '", luaToString, "'");
		//PrintInfo(temp, COLOR_RED);
		return 0;
	}

	CChar luaToString3[MAX_NAME_SIZE];
	Cpy(luaToString3, lua_tostring(L, 3));
	StringToUpper(luaToString3);
	CBool foundTarget = CFalse;
	CInt index;
	for (CInt i = 0; i < scene->GetNumClips(); i++)
	{
		CChar animationName[MAX_NAME_SIZE];
		Cpy(animationName, scene->m_animationClips[i]->GetName());
		StringToUpper(animationName);
		if (Cmp(luaToString3, animationName))
		{
			index = i;
			g_scene[i]->m_animationStatus = eANIM_PLAY;
			foundTarget = CTrue;
			break;
		}
	}
	if (!foundTarget)
	{
		//CChar temp[MAX_NAME_SIZE];
		//sprintf(temp, "\n%s%s%s", "Couldn't find the animation clip '", luaToString3, "'");
		//PrintInfo(temp, COLOR_RED);
		return 0;
	}
	scene->ClearCycle(index, (CFloat)lua_tonumber(L, 4));
	return 0;
}

CInt ExecuteAction(lua_State *L)
{
	int argc = lua_gettop(L);
	if (argc < 4)
	{
		//PrintInfo("\nPlease specify at least 5 arguments for ExecuteAction()", COLOR_RED);
		return 0;
	}
	CScene* scene = NULL;

	//find the scene
	CChar luaToString[MAX_NAME_SIZE];
	Cpy(luaToString, lua_tostring(L, 1));
	StringToUpper(luaToString);

	CChar luaToString2[MAX_NAME_SIZE];
	Cpy(luaToString2, lua_tostring(L, 2));
	StringToUpper(luaToString2);

	for (CUInt i = 0; i < g_scene.size(); i++)
	{
		CChar prefabName[MAX_NAME_SIZE];
		Cpy(prefabName, g_scene[i]->GetDocURI());
		StringToUpper(prefabName);

		CChar sceneName[MAX_NAME_SIZE];
		Cpy(sceneName, g_scene[i]->GetName());
		StringToUpper(sceneName);

		if (Cmp(prefabName, luaToString) && Cmp(sceneName, luaToString2))
		{
			g_scene[i]->m_animationStatus = eANIM_PLAY;
			scene = g_scene[i];
			break;
		}
	}

	if (!scene)
	{
		//CChar temp[MAX_NAME_SIZE];
		//sprintf(temp, "\n%s%s%s", "Couldn't find the scene name '", luaToString, "'");
		//PrintInfo(temp, COLOR_RED);
		return 0;
	}

	CChar luaToString3[MAX_NAME_SIZE];
	Cpy(luaToString3, lua_tostring(L, 3));
	StringToUpper(luaToString3);
	CBool foundTarget = CFalse;
	CInt index;
	for (CInt i = 0; i < scene->GetNumClips(); i++)
	{
		CChar animationName[MAX_NAME_SIZE];
		Cpy(animationName, scene->m_animationClips[i]->GetName());
		StringToUpper(animationName);
		if (Cmp(luaToString3, animationName))
		{
			index = i;
			foundTarget = CTrue;
			break;
		}
	}
	if (!foundTarget)
	{
		//CChar temp[MAX_NAME_SIZE];
		//sprintf(temp, "\n%s%s%s", "Couldn't find the animation clip '", luaToString3, "'");
		//PrintInfo(temp, COLOR_RED);
		return 0;
	}
	CFloat weight = 1.0f;
	CBool lock = CFalse;
	if (argc > 5)
		weight = lua_tonumber(L, 6);
	if (argc > 6)
		lock = (CBool)lua_toboolean(L, 7);
	scene->ExecuteAction(index, (CFloat)lua_tonumber(L, 4), (CFloat)lua_tonumber(L, 5), weight, lock);
	return 0;
}

CInt ReverseExecuteAction(lua_State *L)
{
	int argc = lua_gettop(L);
	if (argc < 3)
	{
		//PrintInfo("\nPlease specify 3 arguments for ReverseExecuteAction()", COLOR_RED);
		return 0;
	}
	CScene* scene = NULL;

	//find the scene
	CChar luaToString[MAX_NAME_SIZE];
	Cpy(luaToString, lua_tostring(L, 1));
	StringToUpper(luaToString);

	CChar luaToString2[MAX_NAME_SIZE];
	Cpy(luaToString2, lua_tostring(L, 2));
	StringToUpper(luaToString2);

	for (CUInt i = 0; i < g_scene.size(); i++)
	{
		CChar prefabName[MAX_NAME_SIZE];
		Cpy(prefabName, g_scene[i]->GetDocURI());
		StringToUpper(prefabName);

		CChar sceneName[MAX_NAME_SIZE];
		Cpy(sceneName, g_scene[i]->GetName());
		StringToUpper(sceneName);

		if (Cmp(prefabName, luaToString) && Cmp(sceneName, luaToString2))
		{
			g_scene[i]->m_animationStatus = eANIM_PLAY;
			scene = g_scene[i];
			break;
		}
	}

	if (!scene)
	{
		//CChar temp[MAX_NAME_SIZE];
		//sprintf(temp, "\n%s%s%s", "Couldn't find the scene name '", luaToString, "'");
		//PrintInfo(temp, COLOR_RED);
		return 0;
	}

	CChar luaToString3[MAX_NAME_SIZE];
	Cpy(luaToString3, lua_tostring(L, 3));
	StringToUpper(luaToString3);
	CBool foundTarget = CFalse;
	CInt index;
	for (CInt i = 0; i < scene->GetNumClips(); i++)
	{
		CChar animationName[MAX_NAME_SIZE];
		Cpy(animationName, scene->m_animationClips[i]->GetName());
		StringToUpper(animationName);
		if (Cmp(luaToString3, animationName))
		{
			index = i;
			foundTarget = CTrue;
			break;
		}
	}
	if (!foundTarget)
	{
		//CChar temp[MAX_NAME_SIZE];
		//sprintf(temp, "\n%s%s%s", "Couldn't find the animation clip '", luaToString3, "'");
		//PrintInfo(temp, COLOR_RED);
		return 0;
	}
	scene->ReverseExecuteAction(index);
	return 0;
}

CInt LoadVScene(lua_State *L)
{
	//if( g_testScript )
	//	return 0;
	g_clickedNew = g_clickedOpen = CTrue;

	int argc = lua_gettop(L);
	if( argc < 1 )
	{
		//PrintInfo( "\nPlease specify 1 argument for LoadVScene()", COLOR_RED );
		return 0;
	}
	Cpy( g_currentVSceneNameViaScript, lua_tostring(L, 1) );
	g_loadSceneViaScript = CTrue;
	return 0;
}

CInt ActivateCamera( lua_State* L )
{
	int argc = lua_gettop(L);

	//find the scene
	CChar luaToString[MAX_NAME_SIZE];
	Cpy( luaToString, lua_tostring(L, 1) );
	StringToUpper( luaToString );

	if( Cmp( "DEFAULT_PHYSX", luaToString ) )
	{
		g_render.SetActiveInstanceCamera( NULL );
		g_currentCameraType = eCAMERA_DEFAULT_PHYSX;
	}
	else if( Cmp( "DEFAULT_FREE", luaToString ) )
	{
		g_render.SetActiveInstanceCamera( g_render.GetDefaultInstanceCamera() );
		g_currentCameraType = eCAMERA_DEFAULT_FREE;
	}
	else
	{
		for( CUInt i = 0; i < g_cameraInstances.size(); i++ )
		{
			CChar camera[MAX_NAME_SIZE];
			Cpy( camera, g_cameraInstances[i]->m_abstractCamera->GetName() );
			StringToUpper( camera );

			if( Cmp( camera, luaToString ) )
			{
				g_render.SetActiveInstanceCamera(  g_cameraInstances[i] );
				g_currentCameraType = eCAMERA_COLLADA;
				break;
			}
		}
	}
	return 0;
}

CMain::CMain()
{
	m_showHelpInfo = CFalse;
	m_timerCounter = 0;
	m_totalElapsedTime = 0;
	m_lockInput = CFalse;
	m_loadScene = CFalse;
	m_soundSystem = NULL;
	m_renderQuery = CFalse;
}

CMain::~CMain()
{
	//CDelete( m_particleSystem );
	m_dof.DeleteFBOs();
	g_octree->ResetState();
	distance_vector.clear();
	sorted_prefabs.clear();

}

CBool CMain::Init()
{
	//initialize lua/////////////////
	LuaOpenLibs(g_lua);
	lua_register(g_lua, "PlaySound", PlaySound);
	lua_register(g_lua, "PauseSound", PauseSound);
	lua_register(g_lua, "StopSound", StopSound);
	lua_register(g_lua, "BlendCycle", BlendCycle);
	lua_register(g_lua, "ClearCycle", ClearCycle);
	lua_register(g_lua, "ExecuteAction", ExecuteAction);
	lua_register(g_lua, "ReverseExecuteAction", ReverseExecuteAction);
	lua_register(g_lua, "LoadVScene", LoadVScene);
	lua_register(g_lua, "ActivateCamera", ActivateCamera);

	////////////////////////////////

	glShadeModel(GL_SMOOTH);										// Enable Smooth Shading
	glClearColor(0.6f, 0.6f, 0.6f, 1.0f);							// Black Background
	glEnable(GL_TEXTURE_2D);							
	glClearDepth(1.0f);									
	glEnable(GL_DEPTH_TEST);							
	glDepthFunc(GL_LEQUAL);								
	glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);	
	glEnable( GL_CULL_FACE );
	glCullFace( GL_BACK ); 
	glLightModeli( GL_LIGHT_MODEL_COLOR_CONTROL, GL_SEPARATE_SPECULAR_COLOR );
	GLfloat globalAmbientLight[] = { 0.0f, 0.0f, 0.0f, 1.0f };
	glLightModelfv( GL_LIGHT_MODEL_AMBIENT, globalAmbientLight );
	glEnable( GL_NORMALIZE );

	CFog fog;
	fog.SetColor( g_fogProperties.m_fogColor );
	fog.SetDensity( g_fogProperties.m_fogDensity );
	//SetTimer( 1, 0 , 0 );
	glDrawBuffer( GL_BACK );
	// I use this variable to load this data once. We may destroy and create the window multiple times,
	// But we don't need to load and unload some data for multiple times
	//initialize audio
	// Position of the listener.
	m_listenerPos[0] = m_listenerPos[1] = m_listenerPos[2] = 0.0f;
	// Velocity of the listener.
	m_listenerVel[0] = m_listenerVel[1] = m_listenerVel[2] =  0.0f;
	// Orientation of the listener. (first 3 elements are "at", second 3 are "up")
	m_listenerOri[0] = 0.0f;
	m_listenerOri[1] = 0.0f;
	m_listenerOri[2] = 1.0f;
	m_listenerOri[3] = 0.0f;
	m_listenerOri[4] = 1.0f;
	m_listenerOri[5] = 0.0f;

	g_soundSystem = m_soundSystem = CNew(COpenALSystem);
	m_soundSystem->Init();

	m_ambientSound = NULL;
	alDistanceModel( AL_INVERSE_DISTANCE );

	g_input.AcquireAll();
	if( !g_input.Initialize( g_window.m_windowGL.hWnd, g_instance, true, IS_USEKEYBOARD | IS_USEMOUSE ) )
	{
		MessageBox( NULL, _T("CMain::Init():Couldn't initialize Input Engine"), _T("VandaEngine Error"), MB_OK );
		return false;
	}

	g_timer = CNew( CTimer );
	if( !g_timer->Init() )
	{
		//Report an error message
		MessageBox( NULL, _T("Couldn't initialize the timer"), _T("VandaEngine Error"), MB_OK | MB_ICONINFORMATION );
		return CFalse; 
	}
	g_nx = CNew( CNovodex );
	if (!g_nx->initNx(10.0, 5.0, 10.0, g_physXProperties.m_fGravityY, g_physXProperties.m_fCapsuleRadius, g_physXProperties.m_fCapsuleHeight, g_physXProperties.m_fCharacterSlopeLimit, g_physXProperties.m_fCharacterSkinWidth, g_physXProperties.m_fCharacterStepOffset))
	{
		MessageBox( NULL, _T("Couldn't initialize physX"),_T("VandaEngine Error"), MB_OK | MB_ICONINFORMATION );
		return false;
	}
	g_font = CNew( CFont );
	if( !g_font->Init( _T("Arial"), 13 ) )
	{
		MessageBox( NULL, _T("Couldn't initialize the font"),_T("VandaEngine Error"), MB_OK | MB_ICONINFORMATION );
		return false;
	}

	g_render.Init();
	g_camera = new CUpdateCamera();

	//ilInit();
	//iluInit();
	if(GLEW_ARB_color_buffer_float)
	{
		glClampColorARB( GL_CLAMP_VERTEX_COLOR_ARB, GL_FALSE );
		glClampColorARB( GL_CLAMP_FRAGMENT_COLOR_ARB, GL_FALSE );
	}

	g_render.m_useBloom = CFalse;
	//bloom effect requires NPOT textures, FBO and shaders
	if( GLEW_ARB_texture_non_power_of_two && g_render.UsingFBOs() && g_options.m_enableFBO && g_render.UsingShader() && g_options.m_enableShader )
	{
		g_bloom = CNew( CBloom );
		g_bloom->CreateEmptyTexture( g_width, g_height, GL_RGB, GL_RGB );
		g_render.m_useBloom = CTrue;
	}

	m_dynamicShadowMap = CNew( CShadowMap );
	g_dynamicShadowMap = m_dynamicShadowMap;
	if( m_dynamicShadowMap->Init() )
		g_render.m_useDynamicShadowMap = CTrue;
	else
	{
		g_render.m_useDynamicShadowMap = CFalse;
	}

	if( !g_useOldRenderingStyle )
		InitFBOs( GL_RGBA, GL_RGBA );

	if( g_render.UsingShader() && g_options.m_enableShader )
	{
		g_render.m_useWaterReflection = CTrue;
	}
	else
	{
		g_render.m_useWaterReflection = CFalse;
	}
	g_octree = CNew( COctree );
	return CTrue;
}

CVoid CMain::Release()
{
	glDeleteTextures(eGBUFFER_NUM_TEXTURES, &m_textureTarget[0] );								
	glDeleteTextures(1, &m_textureTargetSwapLights);								
	glDeleteTextures(1, &m_textureTargetSumLighting);								
	glDeleteTextures(1, &m_textureFogDof);								

	glDeleteFramebuffersEXT( 1, &m_mFboID );
	glDeleteFramebuffersEXT( 1, &m_mFboID2 );
	glDeleteFramebuffersEXT( 1, &m_fboID );
	glDeleteFramebuffersEXT( 1, &m_fboID2 );
	glDeleteFramebuffersEXT( 1, &m_fboIDSum );
	glDeleteFramebuffersEXT( 1, &m_fboIDFogDof );
	glDeleteFramebuffersEXT( 1, &m_mFboIDFogDof );

	for( std::vector<CScene*>::iterator it = g_scene.begin(); it != g_scene.end(); it++ )
	{
		CDelete( *it );
	}

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

	//Clear the vctor objects
	g_scene.clear();
	g_render.Destroy();

	CDelete( m_dynamicShadowMap );
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
	g_engineStaticSounds.clear();

	//delete the static sound buffers
	for( std::vector<COpenALSoundBuffer*>::iterator it = g_soundBuffers.begin(); it != g_soundBuffers.end(); it++ )
	{
		CDelete( *it );
	}
	g_soundBuffers.clear();

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

	//Release Audio
	CDelete( m_ambientSound );
	CDelete(m_soundSystem );

	g_input.Shutdown();
	CDelete( g_timer );
	if( g_nx )
	{
		g_nx->releaseNx();
		CDelete( g_nx )
	}
	if( g_font )
	{
		g_font->Release();
		CDelete( g_font );
	}
	CDelete( g_camera );
}

CVoid CMain::ResetTimer()
{
	elapsedTime = g_timer->GetElapsedSeconds( CTrue ); //start from the beginning
	g_timer->GetFps( 1, CTrue );
	g_elapsedTime = elapsedTime;
	m_timerCounter  = 0;
	m_totalElapsedTime = 0;
}

CBool CMain::Render()
{
	g_numVerts = 0; //debug info
	UpdateAnimations();

	if( !g_useOldRenderingStyle && g_window.m_windowGL.multiSampling && g_options.m_enableFBO)
		g_render.BindFBO(m_mFboID);
	else if( !g_useOldRenderingStyle && !g_window.m_windowGL.multiSampling && g_options.m_enableFBO)
		g_render.BindFBO( m_fboID );

	if( !g_useOldRenderingStyle )
	{
		GLenum DrawBuffers[] = { GL_COLOR_ATTACHMENT0_EXT, GL_COLOR_ATTACHMENT1_EXT, GL_COLOR_ATTACHMENT2_EXT, GL_COLOR_ATTACHMENT3_EXT,
		GL_COLOR_ATTACHMENT4_EXT, GL_COLOR_ATTACHMENT5_EXT};
		glDrawBuffers(eGBUFFER_NUM_TEXTURES, DrawBuffers);
	}
	else
		glDrawBuffer( GL_BACK );
	m_cameraTypeOfPreviousFrame = m_cameraType;

	elapsedTime = g_timer->GetElapsedSeconds();
	g_elapsedTime = elapsedTime;
	m_totalElapsedTime += elapsedTime;

	if( g_currentCameraType == eCAMERA_DEFAULT_FREE )
	{
		CInstanceCamera *instanceCamera = g_render.GetDefaultInstanceCamera();
		if (instanceCamera)
		{
			m_cameraType = eCAMERA_DEFAULT_FREE;
			SetInstanceCamera(instanceCamera, g_width / 2., g_height / 2.);
			m_lockInput = CFalse;
		}
		else
		{
			g_currentCameraType = eCAMERA_DEFAULT_PHYSX;
		}
	}
	CInstanceCamera *instanceCamera = g_render.GetActiveInstanceCamera();
	if( g_currentCameraType == eCAMERA_COLLADA )
	{
		if( instanceCamera )		
		{
			m_cameraType = eCAMERA_COLLADA;

			m_lockInput = CTrue;
			SetInstanceCamera(instanceCamera, g_width / 2., g_height / 2.);
		}
		else
		{
			g_currentCameraType = eCAMERA_DEFAULT_PHYSX;
		}
	}
	if( g_currentCameraType == eCAMERA_DEFAULT_PHYSX )
	{
		m_cameraType = eCAMERA_DEFAULT_PHYSX;

		m_lockInput = CFalse;
		g_camera->m_cameraManager->SetPerspective( g_camera->m_cameraManager->GetAngle(), g_width , g_height , 0.01, 1000. );
		if( g_shadowProperties.m_enable && g_render.UsingShadowShader() && g_render.m_useDynamicShadowMap && g_options.m_enableShader )
		{
			//shadow
			glGetFloatv(GL_PROJECTION_MATRIX, cam_proj);
		}

		//previous position of character controller
		g_camera->m_perspectiveCharacterPosOfPreviousFrame = g_camera->m_perspectiveCharacterPos;
		g_nx->GetCameraAndCharacterPositions( g_camera->m_perspectiveCameraPitch,
			g_camera->m_perspectiveCameraYaw,
			g_camera->m_perspectiveCameraPos,
			g_camera->m_perspectiveCharacterPos,
			g_camera->m_perspectiveCameraDir );

		g_camera->m_cameraManager->UpdateCameraPosDir( g_camera->m_perspectiveCameraPos,
			g_camera->m_perspectiveCharacterPos,
			g_camera->m_perspectiveCameraDir,
			g_camera->m_perspectiveCameraTilt );
	}
	//if( g_shadowProperties.m_enable && g_render.UsingShadowShader() && g_render.m_useDynamicShadowMap && g_options.m_enableShader )
	//{
		//shadow
		// store the inverse of the resulting modelview matrix for the shadow computation
		glGetFloatv(GL_MODELVIEW_MATRIX, cam_modelview);
		g_camera->m_cameraManager->GetInverseMatrix( cam_inverse_modelview );
		//////////////
	//}
	//shadow
	const float bias[16] = {	0.5f, 0.0f, 0.0f, 0.0f, 
								0.0f, 0.5f, 0.0f, 0.0f,
								0.0f, 0.0f, 0.5f, 0.0f,
								0.5f, 0.5f, 0.5f, 1.0f	};
	if( !g_useOldRenderingStyle && g_shadowProperties.m_enable && g_render.UsingShadowShader() && g_render.m_useDynamicShadowMap && g_options.m_enableShader )
	{
		float light_dir[4] = {g_defaultDirectionalLight.x,  g_defaultDirectionalLight.y, g_defaultDirectionalLight.z, 1.0f};
		if(!instanceCamera )
		{
			cam_pos[0] = g_camera->m_perspectiveCameraPos.x;
			cam_pos[1] = g_camera->m_perspectiveCameraPos.y + 1.0;
			cam_pos[2] = g_camera->m_perspectiveCameraPos.z;

			cam_dir[0] = g_camera->m_perspectiveCharacterPos.x - cam_pos[0];
			cam_dir[1] = g_camera->m_perspectiveCharacterPos.y + g_camera->m_perspectiveCameraTilt - cam_pos[1];
			cam_dir[2] = g_camera->m_perspectiveCharacterPos.z - cam_pos[2];
		}
		if( !Cmp( g_shadowProperties.m_directionalLightName, "\n" ) && g_shadowProperties.m_enable && g_render.UsingShadowShader())
			m_dynamicShadowMap->MakeShadowMap( cam_pos, cam_dir, light_dir ); 
	}
	g_camera->m_cameraManager->UpdateFrustum();

	m_calculateDistance = CFalse;
	if ( m_cameraType != m_cameraTypeOfPreviousFrame )
	{
		m_calculateDistance = CTrue;
	}
	//calculate distance between geometries and main camera
	else if( !(g_camera->m_perspectiveCharacterPosOfPreviousFrame.x - g_camera->m_perspectiveCharacterPos.x == 0.0f &&
		g_camera->m_perspectiveCharacterPosOfPreviousFrame.y - g_camera->m_perspectiveCharacterPos.y == 0.0f &&
		g_camera->m_perspectiveCharacterPosOfPreviousFrame.z - g_camera->m_perspectiveCharacterPos.z == 0.0f ) )
	{
		m_calculateDistance = CTrue;
	}
	else if( instanceCamera )
	{
		if( !(g_camera->m_perspectiveCameraPosOfPreviousFrame.x - g_camera->m_perspectiveCameraPos.x == 0.0f &&
			g_camera->m_perspectiveCameraPosOfPreviousFrame.y - g_camera->m_perspectiveCameraPos.y == 0.0f &&
			g_camera->m_perspectiveCameraPosOfPreviousFrame.z - g_camera->m_perspectiveCameraPos.z == 0.0f ) )
		{
			m_calculateDistance = CTrue;
		}
	}
	UpdatePrefabInstanceBB();

	if (m_calculateDistance)
	{
		for( CUInt i = 0; i < g_scene.size(); i++ )
		{
			g_scene[i]->CalculateDistances();
		}
		for (CUInt i = 0; i < g_instancePrefab.size(); i++)
		{
			g_instancePrefab[i]->CalculateDistance();
		}
		for (CUInt i = 0; i < g_engineLights.size(); i++)
		{
			g_engineLights[i]->CalculateDistance();
		}

	}

	//audio
	m_ambientSourcePos[0] = m_listenerPos[0] = g_camera->m_perspectiveCameraPos.x;
	m_ambientSourcePos[1] = m_listenerPos[1] = g_camera->m_perspectiveCameraPos.y;
	m_ambientSourcePos[2] = m_listenerPos[2] = g_camera->m_perspectiveCameraPos.z;

	m_listenerOri[0] = -g_camera->m_perspectiveCameraDir.x;
	m_listenerOri[1] = g_camera->m_perspectiveCameraDir.y;
	m_listenerOri[2] = -g_camera->m_perspectiveCameraDir.z;

	m_soundSystem->SetListenerPosition( m_listenerPos );
	//m_soundSystem->SetListenerVelocity( m_listenerVel );
	m_soundSystem->SetListenerOrientation( m_listenerOri );
	//set ambient sound
	if(	g_databaseVariables.m_insertAmbientSound )
		m_ambientSound->GetSoundSource()->SetSoundPosition( m_ambientSourcePos );

	g_render.m_useShader = CTrue;

	glPolygonMode( GL_FRONT_AND_BACK, GL_FILL );

	glPushAttrib(GL_LIGHTING_BIT);
	//create all the waters textures here
	if ( g_options.m_enableShader && g_render.UsingShader() && g_render.m_useShader )
	{
		for( CUInt i = 0 ; i < g_engineWaters.size(); i++ )
		{
			CVec3f waterPoints[4];
			waterPoints[0].x = g_engineWaters[i]->m_sidePoint[0].x;waterPoints[0].y = g_engineWaters[i]->m_sidePoint[0].y;waterPoints[0].z = g_engineWaters[i]->m_sidePoint[0].z;
			waterPoints[1].x = g_engineWaters[i]->m_sidePoint[1].x;waterPoints[1].y = g_engineWaters[i]->m_sidePoint[1].y;waterPoints[1].z = g_engineWaters[i]->m_sidePoint[1].z;
			waterPoints[2].x = g_engineWaters[i]->m_sidePoint[2].x;waterPoints[2].y = g_engineWaters[i]->m_sidePoint[2].y;waterPoints[2].z = g_engineWaters[i]->m_sidePoint[2].z;
			waterPoints[3].x = g_engineWaters[i]->m_sidePoint[3].x;waterPoints[3].y = g_engineWaters[i]->m_sidePoint[3].y;waterPoints[3].z = g_engineWaters[i]->m_sidePoint[3].z;

			if( g_camera->m_cameraManager->IsBoxInFrustum( waterPoints, 4 ) )
			{
				g_engineWaters[i]->CreateReflectionTexture(g_waterTextureSize );
				//g_engineWaters[i]->CreateRefractionDepthTexture(g_waterTextureSize );
			}
		}
	}


	if( !g_useOldRenderingStyle && g_window.m_windowGL.multiSampling && g_options.m_enableFBO)
		g_render.BindFBO(m_mFboID);
	else if( !g_useOldRenderingStyle && !g_window.m_windowGL.multiSampling && g_options.m_enableFBO)
		g_render.BindFBO( m_fboID );

	if( g_useOldRenderingStyle || !g_options.m_enableFBO)
		glClearColor( 0.4f, 0.4f, 0.4f, 1.0f );
	else
		glClearColor( 0.0f, 0.0f, 0.0f, 1.0f );

	glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );

	//Draw grids of the lower left window (perspective)
	glViewport( 0, 0 , g_width , g_height );// resets the viewport to new dimensions.
	if( g_window.m_windowGL.multiSampling && g_options.m_numSamples )
		glEnable( GL_MULTISAMPLE );
	else
		glDisable( GL_MULTISAMPLE );

	if( !m_loadScene )
		if(!ProcessInputs() )
			return CFalse;

	//render waters here
	CVec3f cameraPos( g_camera->m_perspectiveCameraPos.x, g_camera->m_perspectiveCameraPos.y, g_camera->m_perspectiveCameraPos.z );

	if ( g_options.m_enableShader && g_render.UsingShader() && g_render.m_useShader )
	{
 		for( CUInt i = 0 ; i < g_engineWaters.size(); i++ )
		{
			CVec3f waterPoints[4];
			waterPoints[0].x = g_engineWaters[i]->m_sidePoint[0].x;waterPoints[0].y = g_engineWaters[i]->m_sidePoint[0].y;waterPoints[0].z = g_engineWaters[i]->m_sidePoint[0].z;
			waterPoints[1].x = g_engineWaters[i]->m_sidePoint[1].x;waterPoints[1].y = g_engineWaters[i]->m_sidePoint[1].y;waterPoints[1].z = g_engineWaters[i]->m_sidePoint[1].z;
			waterPoints[2].x = g_engineWaters[i]->m_sidePoint[2].x;waterPoints[2].y = g_engineWaters[i]->m_sidePoint[2].y;waterPoints[2].z = g_engineWaters[i]->m_sidePoint[2].z;
			waterPoints[3].x = g_engineWaters[i]->m_sidePoint[3].x;waterPoints[3].y = g_engineWaters[i]->m_sidePoint[3].y;waterPoints[3].z = g_engineWaters[i]->m_sidePoint[3].z;

			if( g_camera->m_cameraManager->IsBoxInFrustum( waterPoints, 4 ) )
			{
				glUseProgram( g_render.m_waterProgram );
				g_engineWaters[i]->RenderWater(cameraPos, elapsedTime );
				glUseProgram( 0 );
			}
		}
	}

	if( g_shadowProperties.m_enable && g_render.UsingShadowShader() && g_render.m_useDynamicShadowMap && g_options.m_enableShader )
	{
		glActiveTexture(GL_TEXTURE6 );
		glBindTexture(GL_TEXTURE_2D_ARRAY_EXT, m_dynamicShadowMap->depth_tex_ar);
		if( g_shadowProperties.m_shadowType > 3 || g_shadowProperties.m_shadowType == 0 /* 0 is debug state*/)
			glTexParameteri( GL_TEXTURE_2D_ARRAY_EXT, GL_TEXTURE_COMPARE_MODE, GL_COMPARE_R_TO_TEXTURE);
		else 
			glTexParameteri( GL_TEXTURE_2D_ARRAY_EXT, GL_TEXTURE_COMPARE_MODE, GL_NONE);
		//glTexParameteri( GL_TEXTURE_2D_ARRAY_EXT, GL_TEXTURE_COMPARE_MODE, GL_NONE);

		for(int i=m_dynamicShadowMap->cur_num_splits; i<MAX_SPLITS; i++)
			far_bound[i] = 0;

		// for every active split
		for(int i=0; i<m_dynamicShadowMap->cur_num_splits; i++)
		{
			float light_m[16];

			// f[i].fard is originally in eye space - tell's us how far we can see.
			// Here we compute it in camera homogeneous coordinates. Basically, we calculate
			// cam_proj * (0, 0, f[i].fard, 1)^t and then normalize to [0; 1]
			far_bound[i] = 0.5f*(-m_dynamicShadowMap->f[i].fard*cam_proj[10]+cam_proj[14])/m_dynamicShadowMap->f[i].fard + 0.5f;

			// compute a matrix that transforms from camera eye space to light clip space
			// and pass it to the shader through the OpenGL texture matrices, since we
			// don't use them now
			glActiveTexture(GL_TEXTURE0 + (GLenum)i);
			glMatrixMode(GL_TEXTURE);
			glLoadMatrixf(bias);
			glMultMatrixf(m_dynamicShadowMap->shad_cpm[i]);
			// multiply the light's (bias*crop*proj*modelview) by the inverse camera modelview
			// so that we can transform a pixel as seen from the camera
			glMultMatrixf(cam_inverse_modelview);
			
			// compute a normal matrix for the same thing (to transform the normals)
			// Basically, N = ((L)^-1)^-t
			glGetFloatv(GL_TEXTURE_MATRIX, light_m);
			matrix4<float> nm;
			nm.set_value(light_m);
			nm = inverse(nm);
			nm = transpose(nm);

			glActiveTexture(GL_TEXTURE0 + (GLenum)(i+4));
			glMatrixMode(GL_TEXTURE);
			glLoadMatrixf(nm._array);
		}
		glMatrixMode( GL_MODELVIEW );
	}

	g_numLights = 0;
	if (!g_render.UsingShader())
		glEnable( GL_LIGHTING );

	ManageLODs();
	EngineLightPass();
	//COLLADALightPass();
	DefaultLightPass();
	FixedFunctionLightPass();

	if (!g_useOldRenderingStyle &&  g_window.m_windowGL.multiSampling && g_options.m_numSamples && g_options.m_enableFBO)
		g_render.BindForWriting( m_mFboID );
	else if( !g_useOldRenderingStyle && g_options.m_enableFBO)
		g_render.BindForWriting( m_fboID );

	g_currentInstanceLight = NULL;
	g_totalLights = g_numLights;

	CBool useFog;
	if( g_shadowProperties.m_shadowType == eSHADOW_SINGLE_HL && g_shadowProperties.m_enable && g_render.UsingShadowShader())
		useFog = CFalse;
	else 
		useFog = CTrue;

	if( ( g_fogProperties.m_enable || g_dofProperties.m_enable ) && useFog )
	{
		if( !g_useOldRenderingStyle && g_window.m_windowGL.multiSampling && g_options.m_numSamples && g_options.m_enableFBO)
			g_render.BindForWriting(m_mFboIDFogDof);
		else if(!g_useOldRenderingStyle && g_options.m_enableFBO )
			g_render.BindForWriting( m_fboIDFogDof );
		glDrawBuffer( GL_COLOR_ATTACHMENT0_EXT );
		glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
		g_fogBlurPass = CTrue;
		g_octree->Render();
		Render3DAnimatedModels( CTrue);
		Render3DModelsControlledByPhysX();
		//render water
		if ( g_options.m_enableShader && g_render.UsingShader() && g_render.m_useShader )
		{
 			for( CUInt i = 0 ; i < g_engineWaters.size(); i++ )
			{
				CVec3f waterPoints[4];
				waterPoints[0].x = g_engineWaters[i]->m_sidePoint[0].x;waterPoints[0].y = g_engineWaters[i]->m_sidePoint[0].y;waterPoints[0].z = g_engineWaters[i]->m_sidePoint[0].z;
				waterPoints[1].x = g_engineWaters[i]->m_sidePoint[1].x;waterPoints[1].y = g_engineWaters[i]->m_sidePoint[1].y;waterPoints[1].z = g_engineWaters[i]->m_sidePoint[1].z;
				waterPoints[2].x = g_engineWaters[i]->m_sidePoint[2].x;waterPoints[2].y = g_engineWaters[i]->m_sidePoint[2].y;waterPoints[2].z = g_engineWaters[i]->m_sidePoint[2].z;
				waterPoints[3].x = g_engineWaters[i]->m_sidePoint[3].x;waterPoints[3].y = g_engineWaters[i]->m_sidePoint[3].y;waterPoints[3].z = g_engineWaters[i]->m_sidePoint[3].z;

				if( g_camera->m_cameraManager->IsBoxInFrustum( waterPoints, 4 ) )
				{
					glUseProgram( g_render.m_waterFogBlurProgram );
					glUniform1f(glGetUniformLocation( g_render.m_waterFogBlurProgram , "focalDistance"), m_dof.m_focalDistance);
					glUniform1f(glGetUniformLocation( g_render.m_waterFogBlurProgram , "focalRange"), m_dof.m_focalRange);
					if( g_fogProperties.m_enable && useFog )
						glUniform1i(glGetUniformLocation( g_render.m_waterFogBlurProgram , "enableFog"), CTrue );
					else
						glUniform1i(glGetUniformLocation( g_render.m_waterFogBlurProgram , "enableFog"), CFalse );
					g_engineWaters[i]->RenderWater(cameraPos, elapsedTime );
					glUseProgram( 0 );
				}
			}
		}
		g_fogBlurPass = CFalse;
		BlendFogWithScene();
	}

	if( g_updateOctree && g_scene.size() > 0 )
	{
		//Force Update
		//for( CUInt i = 0 ; i < g_scene.size(); i++ )
		//{
		//	g_render.SetScene( g_scene[i] );
		//	g_render.GetScene()->m_update = CTrue;
		//}

		Render3DModels( CTrue, NULL );
		g_octree->Init();
		g_octree->GetWorldDimensions();
		g_octree->SetName( "octree_root" );
		g_octree->SetLevel(0);
		g_octree->AttachGeometriesToNode();
		g_updateOctree = CFalse;
		g_main->RenderQueries(CTrue);
	}


	if( g_shadowProperties.m_enable && g_render.UsingShadowShader() && g_render.m_useDynamicShadowMap && g_options.m_enableShader )
	{
		//reset matrice
		for( CUInt i = 0; i < 8; i++ )
		{
			glActiveTexture(GL_TEXTURE0 + i );
			glMatrixMode( GL_TEXTURE );
			glLoadIdentity();
		}
		glActiveTexture(GL_TEXTURE0);
	}

	glDisable( GL_LIGHT0 );

	for( CInt i = 0; i <g_numLights ; i++ )
	{
		glDisable( GL_LIGHT0 + i );
	}
	glDisable( GL_LIGHTING ); //just for per vertex lighting
	glMatrixMode( GL_MODELVIEW );

	glPopAttrib();

	
	for( CUInt i = 0 ; i < g_scene.size(); i++ )
	{
		g_render.SetScene( g_scene[i] );
		if( !g_render.GetScene()->m_isTrigger )
		{
			for( CUInt j = 0; j < g_scene[i]->m_instanceGeometries.size(); j++ )
			{
				g_scene[i]->m_instanceGeometries[j]->m_renderCount = 0;
			}
		}
		if( g_render.GetScene()->m_hasAnimation )
		{
			g_render.GetScene()->m_updateAnimation = CTrue;
		}
	}

	if( g_databaseVariables.m_insertAndShowSky )
	{
		g_skyDome->RenderDome();
	}

	if( g_physXProperties.m_bDebugMode )
		g_nx->debugMode = CTrue;
	else
		g_nx->debugMode = CFalse;

	if( g_nx->debugMode )
		g_nx->debugRenderer();

	if( g_databaseVariables.m_showPerspectiveGrids )
		DrawGrid();
	if( g_databaseVariables.m_showSoundPositions )
	{
		CFloat pos[3];
		for( CUInt i = 0; i < g_engineStaticSounds.size(); i++ )
		{
			 g_engineStaticSounds[i]->GetPosition(pos);
			 g_render.DrawCube( 150.0f, pos );
		}
	}
	glFlush();
	if( !g_useOldRenderingStyle && g_window.m_windowGL.multiSampling && g_options.m_enableFBO)
	{
		g_render.BindForReading(m_mFboID);
		glReadBuffer( GL_COLOR_ATTACHMENT0_EXT );
		g_render.BindForWriting( m_fboID);
		glDrawBuffer( GL_COLOR_ATTACHMENT0_EXT );
		glBlitFramebufferEXT(0, 0, g_width, g_height, 0, 0, g_width, g_height, GL_COLOR_BUFFER_BIT, GL_NEAREST);
	}
	//m_particleSystem->Render();

	if( !g_useOldRenderingStyle && g_options.m_enableFBO)
	{
		if( g_dofProperties.m_enable )
		{
			glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, 0);

			glMatrixMode(GL_PROJECTION);
			glPushMatrix();
			glLoadIdentity();
			glMatrixMode(GL_MODELVIEW);
			glPushMatrix();
			glLoadIdentity();

			/* Second pass: downsampling */
			glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, m_dof.m_fbo[0]);
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
			glDrawBuffer(GL_COLOR_ATTACHMENT0_EXT);
			glActiveTextureARB(GL_TEXTURE0_ARB);
			glBindTexture(GL_TEXTURE_2D, m_textureTarget[0]);
			glUseProgram(g_render.m_dofProgram[0]);
			glUniform1i( glGetUniformLocation(g_render.m_dofProgram[0], "Tex0"), 0 );
			glViewport(0, 0, g_width/2, g_height/2);
			m_dof.DrawQuad();

			/* Third pass: Gaussian filtering along the X axis */
			glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, m_dof.m_fbo[1]);
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
			glActiveTextureARB(GL_TEXTURE0_ARB);
			glBindTexture(GL_TEXTURE_2D, m_dof.m_texid[0]);
			glUseProgram(g_render.m_dofProgram[1]);
			glUniform1i( glGetUniformLocation(g_render.m_dofProgram[1], "Tex0"), 0 );
			glUniform1i( glGetUniformLocation(g_render.m_dofProgram[1], "Width"), g_width );
			m_dof.DrawQuad();

			/* Fourth pass: Gaussian filtering along the Y axis */
			glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, m_dof.m_fbo[2]);
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
			glUseProgram(g_render.m_dofProgram[2]);
			glUniform1i( glGetUniformLocation(g_render.m_dofProgram[2], "Tex0"), 0 );
			glUniform1i( glGetUniformLocation(g_render.m_dofProgram[2], "Height"), g_height );
			glActiveTextureARB(GL_TEXTURE0_ARB);
			glBindTexture(GL_TEXTURE_2D, m_dof.m_texid[1]);
			m_dof.DrawQuad();

			/* Fifth pass: final compositing */
			glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, m_dof.m_fbo[3]);
			////glDrawBuffer(GL_BACK);
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
			glActiveTexture(GL_TEXTURE0);
			glBindTexture(GL_TEXTURE_2D, m_textureTarget[0]);
			glActiveTexture(GL_TEXTURE1);
			glBindTexture(GL_TEXTURE_2D, m_dof.m_texid[2]);
			glUseProgram(g_render.m_dofProgram[3]);
			glUniform1i( glGetUniformLocation(g_render.m_dofProgram[3], "Tex0"), 0 );
			glUniform1i( glGetUniformLocation(g_render.m_dofProgram[3], "Tex1"), 1 );

			glViewport(0, 0, g_width, g_height);

			glBegin(GL_QUADS);
				glMultiTexCoord2f(GL_TEXTURE0, 0.0f, 0.0f);
				glMultiTexCoord2f(GL_TEXTURE1, 0.0f, 0.0f);
				glVertex2d(-1, -1);
				glMultiTexCoord2f(GL_TEXTURE0, 1.0f, 0.0f);
				glMultiTexCoord2f(GL_TEXTURE1, 1.0f, 0.0f);
				glVertex2d(1, -1);
				glMultiTexCoord2f(GL_TEXTURE0, 1.0f, 1.0f);
				glMultiTexCoord2f(GL_TEXTURE1, 1.0f, 1.0f);
				glVertex2d(1, 1);
				glMultiTexCoord2f(GL_TEXTURE0, 0.0f, 1.0f);
				glMultiTexCoord2f(GL_TEXTURE1, 0.0f, 1.0f);
				glVertex2d(-1, 1);
			glEnd();

			glMatrixMode(GL_PROJECTION);
			glPopMatrix();
			glMatrixMode(GL_MODELVIEW);
			glPopMatrix();
		}
		glActiveTexture(GL_TEXTURE0);
		glUseProgram(0);
		glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, 0);
		glViewport(0, 0, g_width, g_height);
		glDrawBuffer( GL_BACK );
		glClear( GL_COLOR_BUFFER_BIT ); 
		glPushAttrib( GL_ENABLE_BIT );
		glMatrixMode(GL_PROJECTION); glPushMatrix(); glLoadIdentity();
		glOrtho(0, 1, 0, 1, 0.01, 100);
		glMatrixMode(GL_MODELVIEW); glPushMatrix();	glLoadIdentity();
		glDisable( GL_LIGHTING );
		glDisable(GL_DEPTH_TEST); 
		if( g_dofProperties.m_enable )
			glBindTexture( GL_TEXTURE_2D, m_dof.m_texid[3] );
		else
			glBindTexture( GL_TEXTURE_2D, m_textureTarget[0] );
		glEnable( GL_TEXTURE_2D);
		glBegin(GL_QUADS); 
		glTexCoord2d(0,	0);	glVertex3d(0, 0, -1);
		glTexCoord2d(1, 0);	glVertex3d(1, 0, -1);
		glTexCoord2d(1, 1);	glVertex3d(1, 1, -1);
		glTexCoord2d(0, 1);	glVertex3d(0, 1, -1);
		glEnd();

		if( g_render.m_useBloom && g_bloomProperties.m_enable)
		{
			//if( g_dofProperties.m_enable )
			//	g_bloom->CreateRuntimeTexture( g_width, g_height, m_dof.m_texid[3]  );
			//else
			glPushAttrib( GL_CURRENT_BIT );
			g_bloom->CreateRuntimeTexture( g_width, g_height, m_textureTarget[0]  );

			glEnable(GL_BLEND); glBlendFunc(GL_SRC_ALPHA, GL_ONE);
			glColor4f( g_bloomProperties.m_bloomColor[0], g_bloomProperties.m_bloomColor[1], g_bloomProperties.m_bloomColor[2], g_bloomProperties.m_bloomIntensity );
			glBindTexture( GL_TEXTURE_2D, g_bloom->m_bloomTexture );
			glBegin(GL_QUADS); 
			glTexCoord2d(0,	0);	glVertex3d(0, 0, -1); 
			glTexCoord2d(1, 0);	glVertex3d(1, 0, -1);
			glTexCoord2d(1, 1);	glVertex3d(1, 1, -1);
			glTexCoord2d(0, 1);	glVertex3d(0, 1, -1);
			glEnd();
			glDisable( GL_BLEND );
			glPopAttrib();
		}
 		//m_dynamicShadowMap->ShowDepthTex();
		DrawGUI();
		glPopAttrib();
		glMatrixMode(GL_PROJECTION); glPopMatrix();
		glMatrixMode(GL_MODELVIEW); glPopMatrix();
	}
	else
	{
		DrawGUI();
	}
	RenderQueries();

	return CTrue;
}

CVoid CMain::DrawGrid(CVoid)
{
	glPushAttrib( GL_ENABLE_BIT | GL_CURRENT_BIT );
	glDisable( GL_TEXTURE_2D );
	glDisable( GL_LIGHTING );
	//Draw the grid here
	glLineWidth( 1.0f );
	glColor3f( 0.7f, 0.7f, 0.7f );
	glBegin( GL_LINES );
	for( CInt index = -100; index <= 100; index += 1 )
	{
		glVertex3f( -100.0f, 0.0f, (CFloat)index );
		glVertex3f(  100.0f, 0.0f, (CFloat)index );

		glVertex3f( (CFloat)index, 0.0f, -100.0f );
		glVertex3f( (CFloat)index, 0.0f, 100.0f );
	}
	glEnd();
	glLineWidth( 3.0f );
	glBegin( GL_LINES );
	//Draw the axes here
	glColor3f( 1.0f, 0.0f, 0.0f );
	glVertex3f( 0.0f, 0.0f, 0.0f );
	glVertex3f( 1.0f, 0.0f, 0.0f );
	
	glColor3f( 0.0f, 1.0f, 0.0f );
	glVertex3f( 0.0f, 0.0f, 0.0f );
	glVertex3f( 0.0f, 1.0f, 0.0f );

	glColor3f( 0.0f, 0.0f, 1.0f );
	glVertex3f( 0.0f, 0.0f, 0.0f );
	glVertex3f( 0.0f, 0.0f, 1.0f );

	glEnd();

	glPopAttrib();
}


CBool CMain::ProcessInputs()
{
	g_input.Update();

	static CUInt cameraIndex = 0;
	static CBool pKeyDown = CFalse;
	static CBool oKeyDown = CFalse;

	if( g_input.KeyDown( DIK_P ) && !pKeyDown && g_cameraInstances.size() > 0 )
	{
		pKeyDown = CTrue;
		cameraIndex++;
		if( cameraIndex > g_cameraInstances.size() - 1)
			cameraIndex = 0;
		g_render.SetActiveInstanceCamera( g_cameraInstances[cameraIndex] );
		g_currentCameraType = eCAMERA_COLLADA;
	}
	if( g_input.KeyUp( DIK_P  ) )
		pKeyDown = CFalse;

	if( g_input.KeyDown( DIK_O ) && !oKeyDown && g_cameraInstances.size() > 0 )
	{
		oKeyDown = CTrue;
		if( cameraIndex > 0 )
			cameraIndex--;
		else
			cameraIndex = g_cameraInstances.size() - 1;

		g_render.SetActiveInstanceCamera( g_cameraInstances[cameraIndex] );
		g_currentCameraType = eCAMERA_COLLADA;

	}
	if( g_input.KeyUp( DIK_O  ) )
		oKeyDown = CFalse;

	if( g_input.KeyDown( DIK_I ) )
	{
		g_render.SetActiveInstanceCamera(NULL);
		g_currentCameraType = eCAMERA_DEFAULT_PHYSX;
	}
	if( g_input.KeyDown( DIK_U ) )
	{
		g_render.SetActiveInstanceCamera(g_render.GetDefaultInstanceCamera());
		g_currentCameraType = eCAMERA_DEFAULT_FREE;
	}

	static CBool f1KeyDown = CFalse;
	if( g_input.KeyDown( DIK_F1 ) && !f1KeyDown )
	{
		f1KeyDown = CTrue;
		m_showHelpInfo = !m_showHelpInfo;
	}
	if( g_input.KeyUp( DIK_F1 ) )
		f1KeyDown = CFalse;

	if( g_input.KeyDown( DIK_ESCAPE ) )
		return CFalse;


	if( g_currentCameraType == eCAMERA_DEFAULT_PHYSX )
	{
		CBool forceApplied = false;

		if( (g_input.KeyDown( DIK_W ) || g_input.KeyDown( DIK_UP )) && (g_input.KeyDown( DIK_D ) || g_input.KeyDown( DIK_RIGHT )) )
		{
			ApplyForce( MOVE_FRONT_RIGHT, elapsedTime );
			forceApplied = true;
		}
		else	if( (g_input.KeyDown( DIK_W ) || g_input.KeyDown( DIK_UP )) && (g_input.KeyDown( DIK_A ) || g_input.KeyDown( DIK_LEFT )) )
		{
			ApplyForce( MOVE_FRONT_LEFT, elapsedTime );
			forceApplied = true;
		}
		else if( (g_input.KeyDown( DIK_S ) || g_input.KeyDown( DIK_DOWN )) && (g_input.KeyDown( DIK_D ) || g_input.KeyDown( DIK_RIGHT )) )
		{
			ApplyForce( MOVE_BACK_RIGHT, elapsedTime );
			forceApplied = true;
		}
		else if( (g_input.KeyDown( DIK_S ) || g_input.KeyDown( DIK_DOWN )) && (g_input.KeyDown( DIK_A ) || g_input.KeyDown( DIK_LEFT )) )
		{
			ApplyForce( MOVE_BACK_LEFT, elapsedTime );
			forceApplied = true;
		}

		else if( g_input.KeyDown( DIK_W ) || g_input.KeyDown( DIK_UP ) )
		{
			ApplyForce( MOVE_FRONT, elapsedTime );
			forceApplied = true;
		}
		else if( g_input.KeyDown( DIK_S ) || g_input.KeyDown( DIK_DOWN ) )
		{
			ApplyForce( MOVE_BACK, elapsedTime );
			forceApplied = true;
		}
		else if( g_input.KeyDown( DIK_A ) || g_input.KeyDown( DIK_LEFT ) )
		{
			ApplyForce( MOVE_LEFT, elapsedTime );
			forceApplied = true;
		}
		else if( g_input.KeyDown( DIK_D ) || g_input.KeyDown( DIK_RIGHT ) )
		{
			ApplyForce( MOVE_RIGHT, elapsedTime );
			forceApplied = true;
		}
		else if( g_input.KeyDown( DIK_Q ) )
		{
			ApplyForce( MOVE_UP, elapsedTime );
			forceApplied = true;
		}
		else if( g_input.KeyDown( DIK_E ) )
		{
			ApplyForce( MOVE_DOWN, elapsedTime );
			forceApplied = true;
		}

		if( !forceApplied )
			ApplyForce( IDLE, elapsedTime );

		if( g_physXProperties.m_bApplyGravity && g_physXProperties.m_bJumping)
			if( g_input.KeyDown( DIK_SPACE ) )
				if( !g_nx->gJump )
					g_nx->StartJump(g_physXProperties.m_fJumpPower);

		//Manage gravity
		//static CBool gKeyDown = CFalse;
		//CFloat gravity = 0.0f;
		//static CBool pKeyDown = CFalse;

		//if( g_input.KeyDown( DIK_P ) && !pKeyDown )
		//{
		//	pKeyDown = CTrue;
		//	g_physXProperties.m_bDebugMode = ! g_physXProperties.m_bDebugMode;
		//}
		//if( g_input.KeyUp( DIK_P ) )
		//	pKeyDown = CFalse;

		//static CBool bKeyDown = CFalse;

		//if( g_input.KeyDown( DIK_B ) && !bKeyDown )
		//{
		//	bKeyDown = CTrue;
		//	g_databaseVariables.m_showBoundingBox = ! g_databaseVariables.m_showBoundingBox;
		//}
		//if( g_input.KeyUp( DIK_B  ) )
		//	bKeyDown = CFalse;

		//if( g_input.KeyDown( DIK_G ) && !gKeyDown )
		//{
		//	gKeyDown = CTrue;
		//	g_physXProperties.m_bApplyGravity = !g_physXProperties.m_bApplyGravity;
		//	if( g_physXProperties.m_bApplyGravity )
		//		g_nx->gDefaultGravity = NxVec3( g_physXProperties.m_fGravityX, g_physXProperties.m_fGravityY, g_physXProperties.m_fGravityZ );
		//	else
		//		g_nx->gDefaultGravity = NxVec3(0.0f);

		//}
		//if( g_input.KeyUp( DIK_G ) )
		//	gKeyDown = CFalse;

		CInt dx, dy;
		g_input.GetMouseMovement( dx, dy );

		if( dx > 0 ) 
		{
			g_camera->m_perspectiveCameraYaw -= (CFloat)dx * 0.004f;
		}
		else if( dx < 0 )
		{
			g_camera->m_perspectiveCameraYaw -= (CFloat)dx * 0.004f;
		}
		if( g_input.ButtonDown(1) )
		{
			if( dy > 0 ) 
				g_camera->m_cameraManager->SetZoomOut( elapsedTime * 50.0f );
			else if( dy < 0 )
				g_camera->m_cameraManager->SetZoomIn( elapsedTime * 50.0f );
		}
		else
		{
			if( dy > 0 ) 
			{
				g_camera->m_perspectiveCameraTilt -= (CFloat)dy * 0.004f;
			}
			else if( dy < 0 )
			{
				g_camera->m_perspectiveCameraTilt -= (CFloat)dy * 0.004f;
			}

		}
	}
	else if( g_currentCameraType == eCAMERA_DEFAULT_FREE )
	{
		CInt dx, dy;
		g_input.GetMouseMovement( dx, dy );

		if( g_input.ButtonDown(1) )
		{
			g_render.GetDefaultInstanceCamera()->ZoomTransform( dy * 0.04 );
		}
		else
		{
			g_render.GetDefaultInstanceCamera()->SetPanAndTilt( -(CFloat)dx * 0.2f, -(CFloat)dy * 0.2f );
		}

		if (g_input.KeyDown( DIK_S ))
		{
			// UI code to move the camera closer
			g_render.GetDefaultInstanceCamera()->MoveTransform(elapsedTime * g_camera->m_cameraSpeed, 0.0f, 0.0f);
		}

		if (g_input.KeyDown( DIK_W ))
		{
			// UI code to move the camera farther away
			g_render.GetDefaultInstanceCamera()->MoveTransform(-elapsedTime* g_camera->m_cameraSpeed, 0.0f, 0.0f);
		}

		if (g_input.KeyDown( DIK_Q ))
		{
			// UI code to move the camera farther up
			g_render.GetDefaultInstanceCamera()->MoveTransform(0.0f, 0.0f, elapsedTime* g_camera->m_cameraSpeed);
		}

		if (g_input.KeyDown( DIK_E ))
		{
			// UI code to move the camera farther down
			g_render.GetDefaultInstanceCamera()->MoveTransform(0.0f, 0.0f, -elapsedTime* g_camera->m_cameraSpeed);
		}

		if (g_input.KeyDown( DIK_D ))
		{
			// UI code to move the camera farther right
			g_render.GetDefaultInstanceCamera()->MoveTransform(0.0f, -elapsedTime* g_camera->m_cameraSpeed, 0.0f);
		}

		if (g_input.KeyDown( DIK_A ))
		{
			// UI code to move the camera farther left
			g_render.GetDefaultInstanceCamera()->MoveTransform(0.0f, elapsedTime* g_camera->m_cameraSpeed, 0.0f);
		}
	}
	return CTrue;
}

CVoid CMain::ApplyForce( CInt direction, CFloat elapsedTime )
{
	g_nx->runPhysics( NxVec3( 0.0f, 0.0f, 0.0f ), 0.0f, direction, elapsedTime );
}

CBool CMain::Reset()
{
	g_clickedNew = CTrue;
	g_dofProperties.Reset();
	g_fogProperties.Reset();
	g_bloomProperties.Reset();
	g_lightProperties.Reset();
	g_shadowProperties.Reset();
	g_physXProperties.Reset();
	g_shadowProperties.m_enable = CTrue;
	g_pathProperties.Reset();
	g_extraTexturesNamingConventions.Reset();
	g_sceneManagerObjectsPerSplit = 15;
	//g_options.Reset();
	g_useGlobalAmbientColor = CFalse;
	g_globalAmbientColor.r = g_globalAmbientColor.g = g_globalAmbientColor.b = 0.5f;
	g_globalAmbientColor.a = 1.0f;

	//clear geometries
	for (CUInt i = 0; i < g_geometries.size(); i++)
		CDelete(g_geometries[i]);
	g_geometries.clear();
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

	if( g_currentCameraType != eCAMERA_DEFAULT_PHYSX )
	{
		g_currentCameraType = eCAMERA_DEFAULT_PHYSX;
		g_render.SetActiveInstanceCamera(NULL);
		g_main->m_lockInput = CFalse;
	}
	g_main->distance_vector.clear();
	g_main->sorted_prefabs.clear();

	Cpy(g_shadowProperties.m_directionalLightName, "\n" );
	if( g_nx->m_hasScene || gPhysXscene)
	{
		if(!g_clickedOpen)
			ResetPhysX();
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

	CDelete( g_skyDome );

	//delete ambient sound
	if( g_main->m_ambientSound )
	{
		g_main->m_soundSystem->StopALSound( *(g_main->m_ambientSound->GetSoundSource()) );
		alSourcei(g_main->m_ambientSound->GetSoundSource()->GetSource(), AL_BUFFER, AL_NONE); 
		CDelete( g_main->m_ambientSound );			
	}
	//Reset name indexes which are used for selection
	g_nameIndex = 1;

	g_selectedName = -1; 		//no object has been selected

	Cpy( g_currentVSceneName, "\n" ); //save functions
	Cpy(g_currentVSceneName, "\n"); //save functions
	Cpy(g_currentPrefabName, "\n");
	Cpy(g_currentPackagePath, "\n");
	Cpy(g_currentPrefabAndSceneName, "\n");
	Cpy(g_currentInstancePrefabName, "\n");

	g_currentInstancePrefab = NULL;

	g_clickedNew = CFalse;

	return CTrue;
}

CBool CMain::InsertPrefab(CPrefab* prefab)
{
	CChar packagePath[MAX_NAME_SIZE];
	CChar prefabPath[MAX_NAME_SIZE];
	CChar prefabAndPackageName[MAX_NAME_SIZE];

	Cpy(prefabAndPackageName, prefab->GetName());

	Cpy(packagePath, "Assets/Packages/");
	Append(packagePath, prefab->GetPackageName());
	Append(packagePath, "/");

	Cpy(prefabPath, packagePath);
	Append(prefabPath, prefab->GetPrefabName());
	Append(prefabPath, "/");
	Append(prefabPath, prefabAndPackageName);
	Append(prefabPath, ".vpf");

	Cpy(g_currentPackagePath, packagePath);
	Cpy(g_currentPrefabName, prefab->GetPrefabName());

	CInstancePrefab* m_newInstancePrefab = NULL;
	CPrefab* m_newPrefab = prefab;

	FILE *filePtr = fopen(prefabPath, "rb");
	if (filePtr)
	{

		CChar engineName[MAX_NAME_SIZE];
		fread(&engineName, sizeof(CChar), MAX_NAME_SIZE, filePtr);
		if (!CmpIn(engineName, "VandaEngine"))
		{
			fclose(filePtr);
			MessageBoxA(NULL, "Invalid VPF file!", "Vanda Engine Error", MB_OK | MB_ICONERROR);
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
		CBool demo;
		fread(&demo, sizeof(CBool), 1, filePtr);

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
			CScene * tempScene = new CScene();
			g_scene.push_back(tempScene);

			CChar * nameOnly = GetAfterPath(tempSceneName);

			//save functions. it should be copies in WIN32 Project as well
			CChar name[MAX_NAME_SIZE];

			sprintf(name, "%s%s%s%s", packagePath, prefab->GetPrefabName(), "/External Scenes/", nameOnly);

			CChar prefabAndSceneName[MAX_NAME_SIZE];
			sprintf(prefabAndSceneName, "%s%s%s", prefabAndPackageName, "_", nameOnly);
			Cpy(g_currentPrefabAndSceneName, prefabAndSceneName);

			if (tempScene->Load(name))
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
				//save functions/////////////////////////////////
				g_currentScene = tempScene; //mark the current scene. Save functions

				//save functions/////////////////////////////////
				g_render.SetScene(tempScene);
				tempScene->Update(0.0f, CTrue);

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

				sceneLoaded = CTrue;

			}
			else
			{
				delete tempScene;
				tempScene = NULL;
				g_scene.erase(g_scene.end());
				fclose(filePtr);
				MessageBox(NULL, _T("Couldn't load the scene"), _T("VandaEngine Error"), MB_OK);
				sceneLoaded = CFalse;
			}
			if (!sceneLoaded)
				return CFalse;
			if (CmpIn(tempScene->GetName(), "trigger")) //triggers
			{
				if (!g_nx->m_hasScene)
				{
					tempScene->m_isTrigger = CTrue;
					tempScene->Update();
					tempScene->CreateTrigger(g_nx);
				}
				else
				{
					MessageBoxA(NULL, "\nCouldn't create the triggers. In order to create triggers from COLLADA files, you should remove current external PhysX scene.", "Error", MB_OK);
				}

			}
			else if (CmpIn(tempScene->GetName(), "grass") )
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
					if( tempScene->m_geometries.size() >= j + 1 )
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
				if(tempScene->m_geometries.size() >= j + 1)
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
					if ( tempScene->m_geometries.size() >= j + 1)
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
					else if ( tempScene->m_geometries.size() >= j + 1)
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
										sprintf(scriptEnterPath, "%s%s%s%s", packagePath, prefab->GetPrefabName(), "/Scripts/", tempEnterPath);
									else
										Cpy(scriptEnterPath, "\n");

									if (hasScriptExit)
										sprintf(scriptExitPath, "%s%s%s%s", packagePath, prefab->GetPrefabName(), "/Scripts/", tempExitPath);
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
			g_nx->gControllers->reportSceneChanged();
			gPhysXscene->simulate(1.0f / 60.0f/*elapsedTime*/);
			gPhysXscene->flushStream();
			gPhysXscene->fetchResults(NX_ALL_FINISHED, true);
		}

		fclose(filePtr);
	}
	return CTrue;
}

CBool CMain::Load( CChar* pathName )
{
	g_clickedOpen = CTrue;

	Cpy( g_currentVSceneName, GetAfterPath(pathName));

	FILE *filePtr;
	filePtr = fopen( pathName, "rb" );
	if(!filePtr )
	{
		CChar temp[MAX_NAME_SIZE];
		sprintf( temp, "%s%s%s", "Couldn't open the file: '", pathName, "' to load data" );
		//fclose( filePtr );
		MessageBoxA( NULL, temp, "Error", MB_OK );
		return CFalse;
	}
	CChar engineName[MAX_NAME_SIZE];
	fread( &engineName, sizeof( CChar), MAX_NAME_SIZE, filePtr );
	if( !CmpIn( engineName, "VandaEngine" ) )
	{
		fclose( filePtr );
		MessageBoxA( NULL, "Invalid Vin file!", "Vanda Engine Error", MB_OK | MB_ICONERROR );
		return CFalse;
	}
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
	CBool demo;
	fread( &demo, sizeof(CBool), 1, filePtr);
	if(demo)
	{
		MessageBoxA( NULL, "Thank you for your interest in Vanda Engine RTI.\nUnfortunatelly this feature is not supported in demo version. Please order Vanda Engine to run the final output.", "Warning", MB_OK | MB_ICONERROR);
		return CFalse;
	}

	CChar bannerName[MAX_NAME_SIZE];
	fread(&bannerName, sizeof(CChar), MAX_NAME_SIZE, filePtr );
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

	g_nx->gCharacterPos.x = characterPos.x;
	g_nx->gCharacterPos.y = characterPos.y;
	g_nx->gCharacterPos.z = characterPos.z;

	g_camera->m_perspectiveCameraPitch = characterPitchYawTilt.x;
	g_camera->m_perspectiveCameraYaw = characterPitchYawTilt.y;
	g_camera->m_perspectiveCameraTilt = characterPitchYawTilt.z;

	fread( &cameraInstancePos, sizeof( CVec3f ), 1, filePtr );
	fread( &cameraInstancePanTilt, sizeof( CVec2f ), 1, filePtr );
	g_render.GetDefaultInstanceCamera()->MoveTransform2(cameraInstancePos.x, cameraInstancePos.y, cameraInstancePos.z);
	g_render.GetDefaultInstanceCamera()->SetPanAndTilt2(cameraInstancePanTilt.x, cameraInstancePanTilt.y);

	///////////////////////////////////

	if( insertPhysXScene )
	{
		//Copy this part to Win32 Project. Save functions
		//CChar temp[MAX_NAME_SIZE];
		CChar* PhysXName = GetAfterPath( strPhysXSceneName );
		CChar PhysXPath[MAX_NAME_SIZE];
		CChar g_currentVSceneNameWithoutDot[MAX_NAME_SIZE];
		Cpy( g_currentVSceneNameWithoutDot, g_currentVSceneName );
		GetWithoutDot( g_currentVSceneNameWithoutDot );
		sprintf( PhysXPath, "%s%s%s%s", "Assets/VScenes/", g_currentVSceneNameWithoutDot, "/External Physics/", PhysXName );
	
		if( g_nx->LoadScene( PhysXPath, NXU::FT_XML ) )
		{
			g_nx->SetSceneName( PhysXPath );
			//sprintf( temp, "PhysX scene '%s' imported successufully\n", strPhysXSceneName );
			//PrintInfo2( temp );
		}
		else
		{
			//sprintf( temp, "couldn't load the PhysX scene '%s'\n", strPhysXSceneName );
			//PrintInfo2( temp );
			MessageBox( NULL, _T("couldn't load the PhysX scene"), _T( "VandaEngine Error" ), MB_OK );

		}
	}

	g_nx->ResetCharacterPos( characterPos );

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
			fread(new_instance_prefab, sizeof(CInstancePrefab), 1, filePtr);
			new_prefab->SetInstance(new_instance_prefab);
			new_prefab->SetCurrentInstance(new_instance_prefab);
			new_instance_prefab->SetPrefab(new_prefab);
			g_instancePrefab.push_back(new_instance_prefab);
			Cpy(g_currentInstancePrefabName, new_instance_prefab->GetName());
			InsertPrefab(new_prefab);
			new_instance_prefab->UpdateBoundingBox();

			CChar tempInstanceName[MAX_NAME_SIZE];
			sprintf(tempInstanceName, "%s%s%s", "\nPrefab Instance ' ", new_instance_prefab->GetName(), " ' created successfully");
		}
	}
	g_updateOctree = CTrue;

	CChar g_currentVSceneNameWithoutDot[MAX_NAME_SIZE];
	Cpy( g_currentVSceneNameWithoutDot, g_currentVSceneName );
	GetWithoutDot( g_currentVSceneNameWithoutDot );

	//generate physX colliders
	for (CUInt i = 0; i < g_scene.size(); i++)
	{
		g_render.SetScene(g_scene[i]);
		for (CUInt j = 0; j < g_scene[i]->m_instanceGeometries.size(); j++)
		{
			if (g_scene[i]->m_instanceGeometries[j]->m_hasPhysX)
			{
				g_scene[i]->GeneratePhysX(g_scene[i]->m_instanceGeometries[j]->m_lodAlgorithm, g_scene[i]->m_instanceGeometries[j]->m_physXDensity, g_scene[i]->m_instanceGeometries[j]->m_physXPercentage, g_scene[i]->m_instanceGeometries[j]->m_isTrigger, g_scene[i]->m_instanceGeometries[j]->m_isInvisible, g_scene[i]->m_instanceGeometries[j]);
			}
		}
		g_render.GetScene()->m_update = CFalse;
	}

	//if( g_currentCameraType == eCAMERA_DEFAULT_FREE )
	//{
		g_nx->gControllers->reportSceneChanged();
		gPhysXscene->simulate(1.0f/60.0f/*elapsedTime*/);
		gPhysXscene->flushStream();
		gPhysXscene->fetchResults(NX_ALL_FINISHED, true);
	//}
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

		g_skyDome = CNew( CSkyDome );
		CChar skyPath[MAX_NAME_SIZE];
		CChar* tempPath = GetAfterPath( path );
		//Copy this to Win32 Project as well
		sprintf( skyPath, "%s%s%s%s", "assets/vscenes/", g_currentVSceneNameWithoutDot, "/Sky/", tempPath );

		g_skyDome->SetName( name );
		g_skyDome->SetPath( skyPath );
		g_skyDome->SetSlices( slices );
		g_skyDome->SetSides( sides );
		g_skyDome->SetRadius( radius );
		g_skyDome->SetPosition( position );
		g_skyDome->SetDampening( dampening );
		g_skyDome->SetExponential( exponential );
		g_skyDome->Initialize();
		g_databaseVariables.m_insertAndShowSky = CTrue;
	}
	else
		g_databaseVariables.m_insertAndShowSky = CFalse;

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

		fread( &tempInstancePrefabWaterCount, sizeof( CInt ), 1, filePtr );
		for (CInt j = 0; j < tempInstancePrefabWaterCount; j++)
		{
			CChar instanceName[MAX_NAME_SIZE];
			fread(instanceName, sizeof(CChar), MAX_NAME_SIZE, filePtr);

			for( CUInt k = 0 ; k < g_instancePrefab.size(); k++ )
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
		sprintf( dudvPath, "%s%s%s%s", "assets/vscenes/", g_currentVSceneNameWithoutDot, "/Waters/", DuDvAfterPath );

		CChar normalPath[MAX_NAME_SIZE];
		CChar* normalAfterPath = GetAfterPath( strNormalMap );

		//Copy this to Win32 Project as well
		sprintf( normalPath, "%s%s%s%s", "assets/vscenes/", g_currentVSceneNameWithoutDot, "/Waters/", normalAfterPath );

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
		g_engineWaters.push_back( water );

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

	}
	//static sounds
	CInt tempStaticSoundCount;
	fread( &tempStaticSoundCount, sizeof( CInt ), 1, filePtr );
	CFloat staticSoundMaxDistance, staticSoundPitch, staticSoundReferenceDistance, staticSoundRolloff, staticSoundVolume;
	CFloat staticSoundPos[3];
	CBool play, loop;

	CChar name[ MAX_NAME_SIZE ], path[ MAX_NAME_SIZE ];
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
		sprintf( StaticSoundPath, "%s%s%s%s", "assets/vscenes/", g_currentVSceneNameWithoutDot, "/Sounds/Static/", StaticSoundName );

		COpenALSoundBuffer* m_staticSoundBuffer = GetSoundBuffer( GetAfterPath(StaticSoundPath ));
		
		if( m_staticSoundBuffer == NULL ) 
		{
			m_staticSoundBuffer = CNew( COpenALSoundBuffer );

			if( !m_staticSoundBuffer->LoadOggVorbisFromFile( StaticSoundPath ) )
				MessageBoxA( NULL, "Couldn't load the static sound", "Report", MB_OK );

			// all of the buffers are loaded from one location and may be shared.
			//however, I continue to create the sound if it fails to load

			m_staticSoundBuffer->SetName( StaticSoundPath );	
			g_soundBuffers.push_back( m_staticSoundBuffer );
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
		m_staticSound->SetIndex();
		g_engineStaticSounds.push_back( m_staticSound );
		if( play )
			m_soundSystem->PlayALSound( *m_staticSoundSource );

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
	sprintf( AmbientSoundPath, "%s%s%s%s", "assets/vscenes/", g_currentVSceneNameWithoutDot, "/Sounds/Ambient/", AmbientSoundName );

	//load the ambient sound if it exists
	if( insertAmbientSound )
	{
		CDelete( m_ambientSound );
		COpenALSoundSource* m_ambientSoundSource = CNew( COpenALSoundSource );
		COpenALSoundBuffer* m_ambientSoundBuffer = CNew( COpenALSoundBuffer );

		//Initialize ambient sound here
		// Velocity of the source sound.
		m_ambientSoundBuffer->LoadOggVorbisFromFile( AmbientSoundPath );
		m_ambientSoundSource->BindSoundBuffer ( *m_ambientSoundBuffer );

		m_ambientSoundSource->SetLooping( true );
		m_ambientSoundSource->SetPitch( pitch );
		m_ambientSoundSource->SetVolume( volume );
		m_soundSystem->PlayALSound( *m_ambientSoundSource );

		m_ambientSound = CNew( CAmbientSound );
		m_ambientSound->SetSoundSource( m_ambientSoundSource );
		m_ambientSound->SetSoundBuffer( m_ambientSoundBuffer );
		m_ambientSound->SetName( strAmbientSoundName );
		m_ambientSound->SetPath( AmbientSoundPath );
		m_ambientSound->SetVolume( volume );
		m_ambientSound->SetPitch( pitch );

		//CChar temp[MAX_NAME_SIZE];
		//sprintf( temp, "ambient sound '%s' loaded successfully\n", strAmbientSoundPath );
		//PrintInfo2( temp );
		
		g_databaseVariables.m_playAmbientSound = CTrue;
		g_databaseVariables.m_insertAmbientSound = CTrue;

	}
	fclose( filePtr );
	//m_particleSystem = CNew( CParticleSystem( 1000, "p1" ) );
	m_dof.InitFBOs( g_width, g_height );
	ResetTimer();
	g_clickedOpen = CFalse;
	return CTrue;

}

COpenALSoundBuffer *CMain::GetSoundBuffer( const CChar * name )
{
	if (name == NULL) return NULL;
	for(CUInt i=0; i<g_soundBuffers.size(); i++)
	{
		if ( ICmp(GetAfterPath(g_soundBuffers[i]->GetName()), name ) )
			return g_soundBuffers[i];
	}
	return NULL;
}

CBool CMain::InitFBOs( CInt channels, CInt type )
{
	//glGetIntegerv( GL_MAX_DRAW_BUFFERS, &g_numBuffers );

	CUInt numSamples;
	GLenum status;
	if( g_window.m_windowGL.multiSampling ) //initialize 2 separate FBOs
	{
		switch( g_options.m_numSamples  )
		{
		case 2:
			numSamples = 2;
			break;
		case 4:
			numSamples = 4;
			break;
		case 8:
			numSamples = 8;
			break;
		case 16:
			numSamples = 16;
			break;
		default:
			numSamples = 0;
		}
		GLint samples;
		glGetIntegerv(GL_MAX_SAMPLES_EXT, &samples);
		if( numSamples > (CUInt)samples )
			numSamples = samples;
		m_mFboID = g_render.GenerateFBO();
		g_render.BindFBO( m_mFboID );
		//g_render.BindForWriting( m_mFboID );
		//g_render.Attach2DTextureToFBOColor( m_bloomTexture );
		m_rbDepthID = g_render.GenerateRenderBuffer();
		g_render.BindRenderBuffer( m_rbDepthID );
		g_render.RenderbufferDepthStorageMultisample( numSamples, g_width, g_height );
		g_render.AttachDepthToFBO( m_rbDepthID );

		for( CInt i = 0; i < eGBUFFER_NUM_TEXTURES; i++ )
		{
			m_rbColorID[i] = g_render.GenerateRenderBuffer();
			g_render.BindRenderBuffer( m_rbColorID[i] );
			g_render.RenderbufferColorStorageMultisample( numSamples, type, g_width, g_height );
			g_render.AttachRenderBufferToFBOColor( m_rbColorID[i], i );
		}
		GLenum DrawBuffers[] = { GL_COLOR_ATTACHMENT0_EXT, GL_COLOR_ATTACHMENT1_EXT, GL_COLOR_ATTACHMENT2_EXT, GL_COLOR_ATTACHMENT3_EXT,
		GL_COLOR_ATTACHMENT4_EXT, GL_COLOR_ATTACHMENT5_EXT};
		glDrawBuffers(eGBUFFER_NUM_TEXTURES, DrawBuffers);

		status = glCheckFramebufferStatusEXT(GL_FRAMEBUFFER_EXT);
		switch(status)
		{
			case GL_FRAMEBUFFER_COMPLETE_EXT:
				break;
			default:
				MessageBoxA( NULL, "CMain::InitFBo(): An error occured while creating the FBO", "Vanda Engine 1 Error",  MB_OK | MB_ICONERROR);
				break;
		}

		g_render.BindFBO(0);
		g_render.BindRenderBuffer(0);

		//second multisample FBO for multi light passes
		m_mFboID2 = g_render.GenerateFBO();
		g_render.BindFBO( m_mFboID2 );
		m_rbDepthMID2 = g_render.GenerateRenderBuffer();
		g_render.BindRenderBuffer( m_rbDepthMID2 );
		g_render.RenderbufferDepthStorageMultisample( numSamples, g_width, g_height );
		g_render.AttachDepthToFBO( m_rbDepthMID2 );

		m_rbColorID2 = g_render.GenerateRenderBuffer();
		g_render.BindRenderBuffer( m_rbColorID2 );
		g_render.RenderbufferColorStorageMultisample( numSamples, type, g_width, g_height );
		g_render.AttachRenderBufferToFBOColor( m_rbColorID2, 0 );

		glDrawBuffer(GL_COLOR_ATTACHMENT0_EXT);

		status = glCheckFramebufferStatusEXT(GL_FRAMEBUFFER_EXT);
		switch(status)
		{
			case GL_FRAMEBUFFER_COMPLETE_EXT:
				break;
			default:
				MessageBoxA( NULL, "CMain::InitFBo(): An error occured while creating the FBO", "Vanda Engine 1 Error",  MB_OK | MB_ICONERROR);
				break;
		}

		g_render.BindFBO(0);
		g_render.BindRenderBuffer(0);

	}

	for( CInt i = 0; i < eGBUFFER_NUM_TEXTURES; i++ )
	{
		glGenTextures(1, &m_textureTarget[i] );								
		glBindTexture(GL_TEXTURE_2D, m_textureTarget[i] );					
		glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		if( i == eGBUFFER_TEXTURE_TYPE_POSITION || i == eGBUFFER_TEXTURE_TYPE_NORMAL ) //vertex or normal
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA32F, g_width, g_height, 0, GL_RGBA, GL_FLOAT, NULL);	
		else
    		glTexImage2D(GL_TEXTURE_2D, 0, channels, g_width, g_height, 0, type, GL_UNSIGNED_BYTE, NULL );
	}

	m_fboID = g_render.GenerateFBO();
	g_render.BindFBO( m_fboID );
	//g_render.BindForWriting( m_fboID );


	for( CInt i = 0; i < eGBUFFER_NUM_TEXTURES; i++ )
	{
		g_render.Attach2DTextureToFBOColor( m_textureTarget[i], i );
	}

	m_rbDepthID2 = g_render.GenerateRenderBuffer();
	g_render.BindRenderBuffer( m_rbDepthID2 );
	g_render.RenderbufferStorage( g_width, g_height );
	g_render.AttachDepthToFBO( m_rbDepthID2 );

	GLenum DrawBuffers[] = { GL_COLOR_ATTACHMENT0_EXT, GL_COLOR_ATTACHMENT1_EXT, GL_COLOR_ATTACHMENT2_EXT, GL_COLOR_ATTACHMENT3_EXT,
	GL_COLOR_ATTACHMENT4_EXT, GL_COLOR_ATTACHMENT5_EXT};
	glDrawBuffers(eGBUFFER_NUM_TEXTURES, DrawBuffers);

	status = glCheckFramebufferStatusEXT(GL_FRAMEBUFFER_EXT);
	switch(status)
	{
		case GL_FRAMEBUFFER_COMPLETE_EXT:
			break;
		default:
			MessageBoxA( NULL, "CMain::InitFBo(): An error occured while creating the FBO", "Vanda Engine 1 Error",  MB_OK | MB_ICONERROR);
			break;
	}

	//second FBO for multi light passes
	glGenTextures(1, &m_textureTargetSwapLights );								
	glBindTexture(GL_TEXTURE_2D, m_textureTargetSwapLights );					
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	//glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA32F, g_width, g_height, 0, GL_RGBA, GL_FLOAT, NULL);	
  	glTexImage2D(GL_TEXTURE_2D, 0, channels, g_width, g_height, 0, type, GL_UNSIGNED_BYTE, NULL );

	m_fboID2 = g_render.GenerateFBO();
	g_render.BindFBO( m_fboID2 );

	g_render.Attach2DTextureToFBOColor( m_textureTargetSwapLights, 0 );

	m_rbDepthIDSwapLights = g_render.GenerateRenderBuffer();
	g_render.BindRenderBuffer( m_rbDepthIDSwapLights );
	g_render.RenderbufferStorage( g_width, g_height );
	g_render.AttachDepthToFBO( m_rbDepthIDSwapLights );

	glDrawBuffer(GL_COLOR_ATTACHMENT0_EXT);

	status = glCheckFramebufferStatusEXT(GL_FRAMEBUFFER_EXT);
	switch(status)
	{
		case GL_FRAMEBUFFER_COMPLETE_EXT:
			break;
		default:
			MessageBoxA( NULL, "CMain::InitFBo(): An error occured while creating the FBO", "Vanda Engine 1 Error",  MB_OK | MB_ICONERROR);
			break;
	}

	//Final FBO for multi light passes
	glGenTextures(1, &m_textureTargetSumLighting );								
	glBindTexture(GL_TEXTURE_2D, m_textureTargetSumLighting );					
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	//glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA32F, g_width, g_height, 0, GL_RGBA, GL_FLOAT, NULL);	
   	glTexImage2D(GL_TEXTURE_2D, 0, channels, g_width, g_height, 0, type, GL_UNSIGNED_BYTE, NULL );

	m_fboIDSum = g_render.GenerateFBO();
	g_render.BindFBO( m_fboIDSum );
	g_render.Attach2DTextureToFBOColor( m_textureTargetSumLighting, 0 );

	status = glCheckFramebufferStatusEXT(GL_FRAMEBUFFER_EXT);
	switch(status)
	{
		case GL_FRAMEBUFFER_COMPLETE_EXT:
			break;
		default:
			MessageBoxA( NULL, "CMain::InitFBo(): An error occured while creating the FBO", "Vanda Engine 1 Error",  MB_OK | MB_ICONERROR);
			break;
	}

	g_render.BindFBO( 0 );

	glBindTexture(GL_TEXTURE_2D, 0);



	// FBO for fog and depth of field
	glGenTextures(1, &m_textureFogDof );								
	glBindTexture(GL_TEXTURE_2D, m_textureFogDof );					
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	//glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA32F, g_width, g_height, 0, GL_RGBA, GL_FLOAT, NULL);	
   	glTexImage2D(GL_TEXTURE_2D, 0, channels, g_width, g_height, 0, type, GL_UNSIGNED_BYTE, NULL );

	m_fboIDFogDof = g_render.GenerateFBO();
	g_render.BindFBO( m_fboIDFogDof );
	g_render.Attach2DTextureToFBOColor( m_textureFogDof, 0 );
	glDrawBuffer(GL_COLOR_ATTACHMENT0_EXT);

	m_rbDepthIDFogDof = g_render.GenerateRenderBuffer();
	g_render.BindRenderBuffer( m_rbDepthIDFogDof );
	g_render.RenderbufferStorage( g_width, g_height );
	g_render.AttachDepthToFBO( m_rbDepthIDFogDof );
	glDrawBuffer(GL_COLOR_ATTACHMENT0_EXT);

	status = glCheckFramebufferStatusEXT(GL_FRAMEBUFFER_EXT);
	switch(status)
	{
		case GL_FRAMEBUFFER_COMPLETE_EXT:
			break;
		default:
			MessageBoxA( NULL, "CMain::InitFBo(): An error occured while creating the FBO", "Vanda Engine 1 Error",  MB_OK | MB_ICONERROR);
			break;
	}

	//Multisample FBO for depth and DOF
	m_mFboIDFogDof = g_render.GenerateFBO();
	g_render.BindFBO( m_mFboIDFogDof );

	m_rbColorIDFogDof = g_render.GenerateRenderBuffer();
	g_render.BindRenderBuffer( m_rbColorIDFogDof );
	g_render.RenderbufferColorStorageMultisample( numSamples, type, g_width, g_height );
	g_render.AttachRenderBufferToFBOColor( m_rbColorIDFogDof, 0 );

	m_rbMDepthIDFogDof = g_render.GenerateRenderBuffer();
	g_render.BindRenderBuffer( m_rbMDepthIDFogDof );
	g_render.RenderbufferDepthStorageMultisample( numSamples, g_width, g_height );
	g_render.AttachDepthToFBO( m_rbMDepthIDFogDof );

	glDrawBuffers(eGBUFFER_NUM_TEXTURES, DrawBuffers);

	status = glCheckFramebufferStatusEXT(GL_FRAMEBUFFER_EXT);
	switch(status)
	{
		case GL_FRAMEBUFFER_COMPLETE_EXT:
			break;
		default:
			MessageBoxA( NULL, "CMain::InitFBo(): An error occured while creating the FBO", "Vanda Engine 1 Error",  MB_OK | MB_ICONERROR);
			break;
	}

	//g_render.BindFBO(0);
	g_render.BindFBO( 0 );
	g_render.BindRenderBuffer(0);

	glBindTexture(GL_TEXTURE_2D, 0);

	return CTrue;

}


CVoid CMain::SetInstanceCamera( CInstanceCamera * inst, CFloat sWidth, CFloat sHeight )
{
	//previous position of the perspective camera (used while instanced camera is enabled)
	g_camera->m_perspectiveCameraPosOfPreviousFrame = g_camera->m_perspectiveCameraPos;

	// Get the camera from the instance and set the projection matrix from it
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(inst->m_abstractCamera->GetYFov(),
					( sWidth / sHeight ),
					/*inst->m_abstractCamera->GetZNear()*/0.1,
					/*inst->m_abstractCamera->GetZFar()*/10000);

	if( g_shadowProperties.m_enable && g_render.UsingShadowShader() && g_render.m_useDynamicShadowMap && g_options.m_enableShader )
	{
		//shadow
		glGetFloatv(GL_PROJECTION_MATRIX, cam_proj);
	}

	// Now get the node from the instance, and set the modelview matrix to the inverse of 
	// the node's matrix.
	glMatrixMode(GL_MODELVIEW);
	CMatrix mat;
	const CMatrix *ltow;
	ltow = inst->m_parent->GetLocalToWorldMatrix();
	CMatrixCopy(*ltow, mat); 
	//CMatrix3x4Invert( mat, mat);
	//LoadMatrix( mat );
	CVec3f at(0,0,-1),pos( 0,0,0), tAt, tPos;
	CMatrixTransform( mat, at, tAt );
	CMatrixTransform( mat, pos, tPos );
	g_camera->m_perspectiveCameraPos.x = tPos.x;
	g_camera->m_perspectiveCameraPos.y = tPos.y;
	g_camera->m_perspectiveCameraPos.z = tPos.z;

	//it's used for OpenAL
	g_camera->m_perspectiveCameraDir.x = -(tAt.x - tPos.x);
	g_camera->m_perspectiveCameraDir.y = tAt.y - tPos.y;
	g_camera->m_perspectiveCameraDir.z = -(tAt.z - tPos.z);

	//shadow 
	cam_pos[0] = g_camera->m_perspectiveCameraPos.x;
	cam_pos[1] = g_camera->m_perspectiveCameraPos.y;
	cam_pos[2] = g_camera->m_perspectiveCameraPos.z;

	cam_dir[0] = -g_camera->m_perspectiveCameraDir.x;
	cam_dir[1] = g_camera->m_perspectiveCameraDir.y;
	cam_dir[2] = -g_camera->m_perspectiveCameraDir.z;

	g_render.ModelViewMatrix();
	g_render.IdentityMatrix();
	gluLookAt( tPos.x , tPos.y, tPos.z, tAt.x, tAt.y, tAt.z, 0.0, 1.0, 0.0 );
	//if( g_shadowProperties.m_enable && g_render.UsingShadowShader()  && g_render.m_useDynamicShadowMap && g_options.m_enableShader )
	//{
		// store the inverse of the resulting modelview matrix for the shadow computation
		glGetFloatv(GL_MODELVIEW_MATRIX, cam_modelview);
		g_camera->m_cameraManager->GetInverseMatrix( cam_inverse_modelview );
	//}
}

CVoid CMain::UpdateAnimations()
{
	for (CUInt i = 0; i < g_instancePrefab.size(); i++)
	{
		if (!g_instancePrefab[i]->GetVisible()) continue;
		g_currentInstancePrefab = g_instancePrefab[i];

		CScene* scene = NULL;

		CPrefab* prefab = g_instancePrefab[i]->GetPrefab();
		for (CUInt j = 0; j < 3; j++)
		{
			if (prefab && prefab->GetHasLod(j) && g_instancePrefab[i]->GetSceneVisible(j))
			{
				scene = g_instancePrefab[i]->GetScene(j);
				break;
			}
		}
		if (!scene) continue;
		if (!scene->m_isVisible) continue;
		g_render.SetScene(scene);
		if (!g_render.GetScene()->m_isTrigger && !g_render.GetScene()->m_isGrass)
		{
			if (g_render.GetScene()->m_hasAnimation && g_render.GetScene()->m_updateAnimation)
			{
				if (g_render.GetScene()->m_animationStatus == eANIM_PLAY && g_render.GetScene()->UpdateAnimationLists())
				{
					g_render.GetScene()->Update(elapsedTime);
					g_render.GetScene()->m_updateAnimation = CFalse;
					g_render.GetScene()->m_update = CFalse;
				} //if
				else if (g_render.GetScene()->m_update)
				{
					g_render.GetScene()->Update();
					g_render.GetScene()->m_update = CFalse;
				} //else

			} //if
			else if (g_render.GetScene()->m_update)
			{
				g_render.GetScene()->Update();
				g_render.GetScene()->m_update = CFalse;
			} //else
		} //if
	} //for
}
CVoid CMain::Render3DModelsControlledByPhysX(CBool sceneManager)
{
	//3D Model data
	for (CUInt i = 0; i < g_instancePrefab.size(); i++)
	{
		if (!g_instancePrefab[i]->GetVisible()) continue;
		g_currentInstancePrefab = g_instancePrefab[i];
		//if (m_bQuery)
		//{
		//	if (g_instancePrefab[i]->GetResult() == 0)
		//	{
		//		if (g_instancePrefab[i]->GetDistanceFromCamera() - g_instancePrefab[i]->GetRadius() >(g_instancePrefab[i]->GetRadius() + 20.0f))
		//			continue;
		//	}
		//}

		CScene* scene = NULL;

		CPrefab* prefab = g_instancePrefab[i]->GetPrefab();
		for (CUInt j = 0; j < 3; j++)
		{
			if (prefab && prefab->GetHasLod(j) && g_instancePrefab[i]->GetSceneVisible(j))
			{
				scene = g_instancePrefab[i]->GetScene(j);
				break;
			}
		}
		if (!scene) continue;
		g_render.SetScene(scene);

		if (!g_render.GetScene()->m_isTrigger && !g_render.GetScene()->m_isGrass)
		{
			if (g_render.GetScene()->m_update)
			{
				g_render.GetScene()->Update();
				g_render.GetScene()->m_update = CFalse;
			}
			g_render.GetScene()->RenderModelsControlledByPhysX(sceneManager);

			if (g_options.m_enableShader && g_render.UsingShader() && g_render.m_useShader)
			{
				glUseProgram(0);
			}

		}
	}

}

CVoid CMain::Render3DModelsControlledByPhysXForWater(CWater* water, CBool sceneManager)
{
	//3D Model data
	for (CUInt i = 0; i < water->m_instancePrefab.size(); i++)
	{
		if (!water->m_instancePrefab[i]->GetVisible()) continue;
		g_currentInstancePrefab = water->m_instancePrefab[i];

		CScene* scene = NULL;

		CPrefab* prefab = water->m_instancePrefab[i]->GetPrefab();
		for (CUInt j = 0; j < 3; j++)
		{
			if (prefab && prefab->GetHasLod(j) && water->m_instancePrefab[i]->GetSceneVisible(j))
			{
				scene = water->m_instancePrefab[i]->GetScene(j);
				break;
			}
		}
		if (!scene) continue;
		g_render.SetScene(scene);

		if (!g_render.GetScene()->m_isTrigger && !g_render.GetScene()->m_isGrass)
		{
			if (g_render.GetScene()->m_update)
			{
				g_render.GetScene()->Update();
				g_render.GetScene()->m_update = CFalse;
			}
			g_render.GetScene()->RenderModelsControlledByPhysX(sceneManager);

			if (g_options.m_enableShader && g_render.UsingShader() && g_render.m_useShader)
			{
				glUseProgram(0);
			}

		}
	}

}


CVoid CMain::Render3DAnimatedModels(CBool sceneManager)
{
	//3D Model data

	for (CUInt i = 0; i < g_instancePrefab.size(); i++)
	{
		if (!g_instancePrefab[i]->GetVisible()) continue;
		g_currentInstancePrefab = g_instancePrefab[i];
		//if (m_bQuery)
		//{
		//	if (g_instancePrefab[i]->GetResult() == 0)
		//	{
		//		if (g_instancePrefab[i]->GetDistanceFromCamera() - g_instancePrefab[i]->GetRadius() >(g_instancePrefab[i]->GetRadius() + 20.0f))
		//			continue;
		//	}
		//}

		CScene* scene = NULL;

		CPrefab* prefab = g_instancePrefab[i]->GetPrefab();
		for (CUInt j = 0; j < 3; j++)
		{
			if (prefab && prefab->GetHasLod(j) && g_instancePrefab[i]->GetSceneVisible(j))
			{
				scene = g_instancePrefab[i]->GetScene(j);
				break;
			}
		}
		if (!scene) continue;

		g_render.SetScene(scene);
		if (!g_render.GetScene()->m_isTrigger && !g_render.GetScene()->m_isGrass)
		{
			if (g_render.GetScene()->m_hasAnimation && g_render.GetScene()->m_updateAnimation)
			{
				if (g_render.GetScene()->m_animationStatus == eANIM_PLAY  && g_render.GetScene()->UpdateAnimationLists())
				{
					g_render.GetScene()->Update(elapsedTime);
					g_render.GetScene()->m_updateAnimation = CFalse;
					g_render.GetScene()->m_update = CFalse;
				}
				else if (g_render.GetScene()->m_update)
				{
					g_render.GetScene()->Update();
					g_render.GetScene()->m_update = CFalse;
				}

			}
			else if (g_render.GetScene()->m_update)
			{
				g_render.GetScene()->Update();
				g_render.GetScene()->m_update = CFalse;
			}
			if (g_currentInstancePrefab)
			{
				g_render.ModelViewMatrix();
				g_render.PushMatrix();
				g_render.MultMatrix(*(g_instancePrefab[i]->GetInstanceMatrix()));
			}
			g_render.GetScene()->RenderAnimatedModels(sceneManager, CTrue); //render controller
			if (g_currentInstancePrefab)
			{
				g_render.PopMatrix();
			}
			g_render.GetScene()->RenderAnimatedModels(sceneManager, CFalse); //render animated models
			if (g_options.m_enableShader && g_render.UsingShader() && g_render.m_useShader)
			{
				glUseProgram(0);
			}

		}
	}
}

CVoid CMain::Render3DAnimatedModelsForWater(CWater* water, CBool sceneManager)
{
	//3D Model data
	for (CUInt i = 0; i < water->m_instancePrefab.size(); i++)
	{
		if (!water->m_instancePrefab[i]->GetVisible()) continue;
		g_currentInstancePrefab = water->m_instancePrefab[i];

		CScene* scene = NULL;

		CPrefab* prefab = water->m_instancePrefab[i]->GetPrefab();
		for (CUInt j = 0; j < 3; j++)
		{
			if (prefab && prefab->GetHasLod(j) && water->m_instancePrefab[i]->GetSceneVisible(j))
			{
				scene = water->m_instancePrefab[i]->GetScene(j);
				break;
			}
		}
		if (!scene) continue;

		g_render.SetScene(scene);
		if (!g_render.GetScene()->m_isTrigger && !g_render.GetScene()->m_isGrass)
		{
			if (g_render.GetScene()->m_hasAnimation && g_render.GetScene()->m_updateAnimation)
			{
				if (g_render.GetScene()->m_animationStatus == eANIM_PLAY  && g_render.GetScene()->UpdateAnimationLists())
				{
					g_render.GetScene()->Update(elapsedTime);
					g_render.GetScene()->m_updateAnimation = CFalse;
					g_render.GetScene()->m_update = CFalse;
				}
				else if (g_render.GetScene()->m_update)
				{
					g_render.GetScene()->Update();
					g_render.GetScene()->m_update = CFalse;
				}

			}
			else if (g_render.GetScene()->m_update)
			{
				g_render.GetScene()->Update();
				g_render.GetScene()->m_update = CFalse;
			}
			if (g_currentInstancePrefab)
			{
				g_render.ModelViewMatrix();
				g_render.PushMatrix();
				g_render.MultMatrix(*(water->m_instancePrefab[i]->GetInstanceMatrix()));
			}
			g_render.GetScene()->RenderAnimatedModels(sceneManager, CTrue); //render controller
			if (g_currentInstancePrefab)
			{
				g_render.PopMatrix();
			}
			g_render.GetScene()->RenderAnimatedModels(sceneManager, CFalse); //render animated models
			if (g_options.m_enableShader && g_render.UsingShader() && g_render.m_useShader)
			{
				glUseProgram(0);
			}

		}
	}
}

CVoid CMain::RenderQueries(CBool init)
{
	if (m_bQuery || init)
	{
		m_renderQuery = CTrue;
		glDepthFunc(GL_LEQUAL);
		glColorMask(GL_FALSE, GL_FALSE, GL_FALSE, GL_FALSE);
		//glDepthMask(GL_FALSE);
		if (!g_useOldRenderingStyle && g_window.m_windowGL.multiSampling && g_options.m_enableFBO)
			g_render.BindFBO(m_mFboID);
		else if (!g_useOldRenderingStyle && !g_window.m_windowGL.multiSampling && g_options.m_enableFBO)
			g_render.BindFBO(m_fboID);
		glClear(GL_DEPTH_BUFFER_BIT);

		g_render.m_useShader = CFalse;
		glPushAttrib(GL_ENABLE_BIT | GL_LIGHTING_BIT);
		glDisable(GL_LIGHTING);
		//glDisable(GL_DEPTH_TEST);
		for (CUInt i = 0; i < 8; i++)
		{
			glActiveTexture(GL_TEXTURE0 + (GLenum)i);
			glBindTexture(GL_TEXTURE_2D, 0);
			glDisable(GL_TEXTURE_2D);
		}

		g_render.ModelViewMatrix();
		//for (CUInt i = 0; i < g_instancePrefab.size(); i++)
		//{
		//	if (!g_instancePrefab[i]->GetVisible()) continue;
		//	g_currentInstancePrefab = g_instancePrefab[i];

		//	glBeginQuery(GL_SAMPLES_PASSED, g_instancePrefab[i]->GetQueryIndex());
		//	COpenGLUtility glUtil;
		//	CVector min(g_instancePrefab[i]->GetMinAABB().x, g_instancePrefab[i]->GetMinAABB().y, g_instancePrefab[i]->GetMinAABB().z);
		//	CVector max(g_instancePrefab[i]->GetMaxAABB().x, g_instancePrefab[i]->GetMaxAABB().y, g_instancePrefab[i]->GetMaxAABB().z);
		//	Point p(g_instancePrefab[i]->GetCenter().x, g_instancePrefab[i]->GetCenter().y, g_instancePrefab[i]->GetCenter().z);
		//	if (g_camera->m_cameraManager->IsSphereInFrustum(p, g_instancePrefab[i]->GetRadius()))
		//		glUtil.DrawCCWBox(min, max, CFalse, CFalse);
		//	glEndQuery(GL_SAMPLES_PASSED);
		//}
		if (m_calculateDistance || init)
		{
			distance_vector.clear();
			sorted_prefabs.clear();

			CFloat distance;
			for (CUInt i = 0; i < g_instancePrefab.size(); i++)
			{
				distance = g_instancePrefab[i]->GetDistanceFromCamera();
				distance_vector.push_back(distance);
			}
			std::sort(distance_vector.begin(), distance_vector.end());
			for (CUInt i = 0; i < distance_vector.size(); i++)
			{
				for (CUInt j = 0; j < g_instancePrefab.size(); j++)
				{
					if (distance_vector[i] == g_instancePrefab[j]->GetDistanceFromCamera())
					{
						sorted_prefabs.push_back(g_instancePrefab[j]);
						break;
					}
				}
			}
		}
		for (CUInt i = 0; i < sorted_prefabs.size(); i++)
		{
			if (!sorted_prefabs[i]->GetVisible()) continue;
			g_currentInstancePrefab = sorted_prefabs[i];

			glBeginQuery(GL_SAMPLES_PASSED, sorted_prefabs[i]->GetQueryIndex());

			CScene* scene = NULL;

			CPrefab* prefab = sorted_prefabs[i]->GetPrefab();
			if (prefab && prefab->GetHasLod(0))
				scene = sorted_prefabs[i]->GetScene(0);
			if (!scene) continue;
			g_render.SetScene(scene);
			if (g_render.GetScene() && !g_render.GetScene()->m_isTrigger && !g_render.GetScene()->m_isGrass)
			{
				if (g_render.GetScene()->m_update)
				{
					g_render.GetScene()->Update();
					g_render.GetScene()->m_update = CFalse;
				}
			}
			g_render.GetScene()->Render(CTrue);
			if (g_currentInstancePrefab)
			{
				g_render.PushMatrix();
				g_render.MultMatrix(*(sorted_prefabs[i]->GetInstanceMatrix()));
			}
			g_render.GetScene()->RenderAnimatedModels(CTrue, CTrue); //render controller
			if (g_currentInstancePrefab)
			{
				g_render.PopMatrix();
			}
			g_render.GetScene()->RenderAnimatedModels(CTrue, CFalse); //render animated models
			g_render.GetScene()->RenderModelsControlledByPhysX(CTrue);
			glEndQuery(GL_SAMPLES_PASSED);
		}
		g_render.m_useShader = CTrue;
		glDisable(GL_CULL_FACE);
		glDepthMask(GL_FALSE);

		if ( g_engineLights.size() > 0)
		{
			//set all the lights here
			for (CUInt j = 0; j < g_engineLights.size(); j++)
			{
				if (Cmp(g_shadowProperties.m_directionalLightName, g_engineLights[j]->m_abstractLight->GetName()))
					continue;

				CInstanceLight *instanceLight = g_engineLights[j];

				CVec3f  Position;
				if (instanceLight->m_parent)
				{
					float *matrix = (float *)instanceLight->m_parent->GetLocalToWorldMatrix();
					Position.x = matrix[12]; Position.y = matrix[13]; Position.z = matrix[14];
				}
				else
				{
					Position.x = instanceLight->m_abstractLight->GetPosition()[0];
					Position.y = instanceLight->m_abstractLight->GetPosition()[1];
					Position.z = instanceLight->m_abstractLight->GetPosition()[2];
				}
				glBeginQuery(GL_SAMPLES_PASSED, instanceLight->GetQueryIndex());
				CFloat color[4] = { 1.0f, 1.0f, 1.0f, 1.0f };
				CFloat pos[3]; pos[0] = Position.x; pos[1] = Position.y; pos[2] = Position.z;
				g_render.DrawSolidSphere(instanceLight->GetRadius(), 10, 10, pos, color);
				glEndQuery(GL_SAMPLES_PASSED);
			}
		}
		glDepthMask(CTrue);

		glPopAttrib();
		glColorMask(GL_TRUE, GL_TRUE, GL_TRUE, GL_TRUE);
		//glDepthMask(GL_TRUE);
		m_renderQuery = CFalse;
		g_render.BindFBO(0);

	}
}

CVoid CMain::Render3DModels(CBool sceneManager, CChar* parentTreeNameOfGeometries)
{
	//3D Model data

	for (CUInt i = 0; i < g_instancePrefab.size(); i++)
	{
		if (!g_instancePrefab[i]->GetVisible()) continue;
		g_currentInstancePrefab = g_instancePrefab[i];

		//if (m_bQuery)
		//{
		//	if (g_instancePrefab[i]->GetResult() == 0)
		//	{
		//		if (g_instancePrefab[i]->GetDistanceFromCamera() - g_instancePrefab[i]->GetRadius() > (g_instancePrefab[i]->GetRadius() + 20.0f))
		//			continue;
		//	}
		//}

		CScene* scene = NULL;

		CPrefab* prefab = g_instancePrefab[i]->GetPrefab();
		for (CUInt j = 0; j < 3; j++)
		{
			if (prefab && prefab->GetHasLod(j) && g_instancePrefab[i]->GetSceneVisible(j))
			{
				scene = g_instancePrefab[i]->GetScene(j);
				break;
			}
		}
		if (!scene) continue;
		g_render.SetScene(scene);

		if (!g_render.GetScene()->m_isTrigger && !g_render.GetScene()->m_isGrass)
		{
			if (g_render.GetScene()->m_update)
			{
				g_render.GetScene()->Update();
				g_render.GetScene()->m_update = CFalse;
			}
			g_render.GetScene()->Render(sceneManager, parentTreeNameOfGeometries);

			if (g_options.m_enableShader && g_render.UsingShader() && g_render.m_useShader)
			{
				glUseProgram(0);
			}
			if (g_databaseVariables.m_showBoundingBox)
			{
				g_render.GetScene()->RenderAABBWithLines();
			}
		}
	}

	////render grass
	//for (CUInt i = 0; i < g_instancePrefab.size(); i++)
	//{
	//	if (!g_instancePrefab[i]->GetVisible()) continue;
	//	g_currentInstancePrefab = g_instancePrefab[i];
	//	GLint resultAvailable = GL_FALSE, result;
	//	glGetQueryObjectiv(g_instancePrefab[i]->GetQueryIndex(), GL_QUERY_RESULT_AVAILABLE, &resultAvailable);
	//	if (resultAvailable == GL_TRUE)
	//	{
			//glGetQueryObjectiv(g_instancePrefab[i]->GetQueryIndex(), GL_QUERY_RESULT, &result);
			//if (result == 0)
			//{
			//	if (g_instancePrefab[i]->GetDistanceFromCamera() - g_instancePrefab[i]->GetRadius() > (g_instancePrefab[i]->GetRadius() + 20.0f))
			//		continue;
			//}
	//	}

	//	CScene* scene = NULL;

	//	CPrefab* prefab = g_instancePrefab[i]->GetPrefab();
	//	for (CUInt j = 0; j < 3; j++)
	//	{
	//		if (prefab && prefab->GetHasLod(j) && g_instancePrefab[i]->GetSceneVisible(j))
	//		{
	//			scene = g_instancePrefab[i]->GetScene(j);
	//			break;
	//		}
	//	}
	//	if (!scene) continue;
	//	if (!scene->m_isVisible) continue;
	//	g_render.SetScene(scene);

	//	if (g_render.GetScene()->m_isGrass)
	//	{
	//		if (g_render.GetScene()->m_update)
	//		{
	//			g_render.GetScene()->Update();
	//			g_render.GetScene()->m_update = CFalse;
	//		}
	//		if (g_options.m_enableShader && g_render.UsingShader() && g_render.m_useShader)
	//			glUseProgram(g_render.m_grassProgram);
	//		g_render.GetScene()->RenderGrass(sceneManager, parentTreeNameOfGeometries);
	//		if (g_options.m_enableShader && g_render.UsingShader() && g_render.m_useShader)
	//			glUseProgram(0);
	//		//if( g_databaseVariables.m_showBoundingBox )
	//		//	g_render.GetScene()->RenderAABBWithLines();
	//	}
	//}

}
CVoid CMain::Render3DModelsForWater(CWater* water, CBool sceneManager, CChar* parentTreeNameOfGeometries)
{
	//3D Model data
	for (CUInt i = 0; i < water->m_instancePrefab.size(); i++)
	{
		if (!water->m_instancePrefab[i]->GetVisible()) continue;

		CScene* scene = NULL;

		CPrefab* prefab = water->m_instancePrefab[i]->GetPrefab();
		for (CUInt j = 0; j < 3; j++)
		{
			if (prefab && prefab->GetHasLod(j) && water->m_instancePrefab[i]->GetSceneVisible(j))
			{
				scene = water->m_instancePrefab[i]->GetScene(j);
				break;
			}
		}
		if (!scene) continue;
		g_render.SetScene(scene);

		if (!g_render.GetScene()->m_isTrigger && !g_render.GetScene()->m_isGrass)
		{
			if (g_render.GetScene()->m_update)
			{
				g_render.GetScene()->Update();
				g_render.GetScene()->m_update = CFalse;
			}
			g_render.GetScene()->Render(sceneManager, parentTreeNameOfGeometries);

			if (g_options.m_enableShader && g_render.UsingShader() && g_render.m_useShader)
			{
				glUseProgram(0);
			}
			if (g_databaseVariables.m_showBoundingBox)
			{
				g_render.GetScene()->RenderAABBWithLines();
			}
		}
	}

	//render grass
	for (CUInt i = 0; i < water->m_instancePrefab.size(); i++)
	{
		if (!water->m_instancePrefab[i]->GetVisible()) continue;
		g_currentInstancePrefab = water->m_instancePrefab[i];

		CScene* scene = NULL;

		CPrefab* prefab = water->m_instancePrefab[i]->GetPrefab();
		for (CUInt j = 0; j < 3; j++)
		{
			if (prefab && prefab->GetHasLod(j) && water->m_instancePrefab[i]->GetSceneVisible(j))
			{
				scene = water->m_instancePrefab[i]->GetScene(j);
				break;
			}
		}
		if (!scene) continue;
		if (!scene->m_isVisible) continue;
		g_render.SetScene(scene);

		if (g_render.GetScene()->m_isGrass)
		{
			if (g_render.GetScene()->m_update)
			{
				g_render.GetScene()->Update();
				g_render.GetScene()->m_update = CFalse;
			}
			if (g_options.m_enableShader && g_render.UsingShader() && g_render.m_useShader)
				glUseProgram(g_render.m_grassProgram);
			g_render.GetScene()->RenderGrass(sceneManager, parentTreeNameOfGeometries);
			if (g_options.m_enableShader && g_render.UsingShader() && g_render.m_useShader)
				glUseProgram(0);
			//if( g_databaseVariables.m_showBoundingBox )
			//	g_render.GetScene()->RenderAABBWithLines();
		}
	}

}

CVoid CMain::DrawGUI()
{
	m_timerCounter++;

	g_font->StartRendering();
	//g_font->Print( "Vanda Engine 1", 10.0f, 980.0f, 0.0f, 1.0f, 1.0f, 1.0f );
	//g_font->Print( "Copyright (C) 2015 Zehne Ziba Co., Ltd.", 10.0f, 950.0f, 0.0f, 1.0f, 1.0f, 1.0f );
	//g_font->Print( "http://www.vandaengine.org", 10.0f, 920.0f, 0.0f, 1.0f, 1.0f, 1.0f );
	//g_font->Print( "Press F4 for help...", 10.0f, 890.0f, 0.0f, 1.0f, 1.0f, 1.0f );

	if( m_showHelpInfo )
	{
		//g_font->Print( "F1 key: Enable / Disable help.\n", 10.0f, 830.0f, 0.0f, 1.0f, 1.0f, 0.5f );
		//g_font->Print( "Right Arrow key or D key: Move Right.\n", 10.0f, 800.0f, 0.0f, 1.0f, 1.0f, 0.5f );
		//g_font->Print( "Up Arrow key or W key: Move Front.\n", 10.0f, 770.0f, 0.0f, 1.0f, 1.0f, 0.5f );
		//g_font->Print( "Down Arrow key or S key: Move Back.\n", 10.0f, 740.0f, 0.0f, 1.0f, 1.0f, 0.5f );
		//g_font->Print( "Left Arrow key or A key: Move Left.\n", 10.0f, 710.0f, 0.0f, 1.0f, 1.0f, 0.5f );
		//g_font->Print( "E key: Move Down.\n", 10.0f, 680.0f, 0.0f, 1.0f, 1.0f, 0.5f );
		//g_font->Print( "Q key: Move Up (If gravity is disabled).\n", 10.0f, 650.0f, 0.0f, 1.0f, 1.0f, 0.5f );
		//g_font->Print( "G key: Enable / Disable gravity.\n", 10.0f, 620.0f, 0.0f, 1.0f, 1.0f, 0.5f );
		//g_font->Print( "Spacebar key: Jump (If gravity is enabled).\n", 10.0f, 590.0f, 0.0f, 1.0f, 1.0f, 0.5f );
		//g_font->Print( "Mouse movement: Camera rotation.\n", 10.0f, 560.0f, 0.0f, 1.0f, 1.0f, 0.5f );
		//g_font->Print( "Right mouse button: Zoom in / Zoom out.\n", 10.0f, 530.0f, 0.0f, 1.0f, 1.0f, 0.5f );
		//g_font->Print( "P key: Enable / Disable physics debug mode.\n", 10.0f, 500.0f, 0.0f, 1.0f, 1.0f, 0.5f );

		//if( g_databaseVariables.m_showStatistics )
		//{
			static int m_numVerts = 0;
			static int m_fps = 0;

			CChar temp[MAX_NAME_SIZE];
			CInt fps = (CInt)g_timer->GetFps(m_timerCounter);

			if( m_totalElapsedTime >= 0.5 ) //Every 0.5 seconds update the FPS
			{
				ResetTimer();
				m_numVerts = g_numVerts;
				m_fps = fps;
			}

			sprintf(temp, "FPS : %i", m_fps);
			g_font->Print("---Statistics---", 10.0f, 990.0, 0.0f, 0.85f, 0.67f, 0.0f);
			g_font->Print(temp, 10.0f, 960.0, 0.0f, 0.85f, 0.67f, 0.0f);
			sprintf(temp, "Rendered Vertexes : %i", m_numVerts);
			g_font->Print(temp, 10.0f, 930.0, 0.0f, 0.85f, 0.67f, 0.0f);
			sprintf(temp, "Rendered Triangles : %i", m_numVerts / 3);
			g_font->Print(temp, 10.0f, 900, 0.0f, 0.85f, 0.67f, 0.0f);
			g_font->Print("Object -------------------- Samples", 10.0f, 870, 0.0f, 0.85f, 0.67f, 0.0f);
			for (CUInt i = 0; i < sorted_prefabs.size(); i++)
			{
				if (i <= 20)
				{
					char temp[MAX_NAME_SIZE];
					sprintf(temp, "%s : %i", sorted_prefabs[i]->GetName(), sorted_prefabs[i]->GetResult());
					g_font->Print(temp, 10.0f, 870.0f - (i + 1) * 30, 0.0f, 0.85f, 0.67f, 0.0f);
				}
				else
				{
					break;
				}
			}
			//for (CUInt i = 0; i < g_engineLights.size(); i++)
			//{
			//	char temp[200];
			//	sprintf(temp, "%s : %i", g_engineLights[i]->m_abstractLight->GetName(), g_engineLights[i]->GetVisible());
			//	g_font->Print(temp, 10.0f, 910.0f - (g_instancePrefab.size() + i + 1) * 30, 0.0f, 0.65f, 0.5f, 0.5f);
			//}

		//}
	}
	//if( g_databaseVariables.m_showStatistics )
	//{
	//}

	g_font->EndRendering();

}

void CMain::ResetPhysX(CBool releaseActors)
{
	if( releaseActors )
	{
		CUInt actor_counts = gPhysXscene->getNbActors();
		for( CUInt i = 0; i < actor_counts; i++ )
		{
			if( !gPhysXscene->getActors()[i]->getName() ) continue;
			gPhysXscene->releaseActor( *gPhysXscene->getActors()[i] );
		}
	}
	//always release ground plane
	if (!releaseActors)
		g_nx->ReleaseGroundPlane();

	g_nx->gControllers->reportSceneChanged();

	//release character controller
	for( CUInt i = 0; i < gPhysXscene->getNbActors(); i++ )
	{
		if( !gPhysXscene->getActors()[i]->getName() )
		{
			gPhysXscene->releaseActor( *gPhysXscene->getActors()[i] );
			break;
		}
	}

	g_nx->ReleaseCharacterControllers();

	g_nx->gDefaultGravity.x = g_physXProperties.m_fGravityX;
	g_nx->gDefaultGravity.y = g_physXProperties.m_fGravityY;
	g_nx->gDefaultGravity.z = g_physXProperties.m_fGravityZ;
	gPhysicsSDK->setParameter( NX_SKIN_WIDTH, g_physXProperties.m_fDefaultSkinWidth );

	NxMaterial* defaultMaterial = gPhysXscene->getMaterialFromIndex(0);
	defaultMaterial->setRestitution(g_physXProperties.m_fDefaultRestitution);
	defaultMaterial->setStaticFriction(g_physXProperties.m_fDefaultStaticFriction);
	defaultMaterial->setDynamicFriction(g_physXProperties.m_fDefaultDynamicFriction);
	if (g_physXProperties.m_bGroundPlane)
		g_nx->CreateGroundPlane(g_physXProperties.m_fGroundHeight);
	g_nx->InitCharacterControllers(g_nx->gCharacterPos.x, g_nx->gCharacterPos.y, g_nx->gCharacterPos.z, g_physXProperties.m_fCapsuleRadius, g_physXProperties.m_fCapsuleHeight, g_physXProperties.m_fCharacterSkinWidth, g_physXProperties.m_fCharacterSlopeLimit, g_physXProperties.m_fCharacterStepOffset);
	
	if( g_physXProperties.m_bApplyGravity )
	{
		g_nx->gDefaultGravity = NxVec3( g_physXProperties.m_fGravityX, g_physXProperties.m_fGravityY, g_physXProperties.m_fGravityZ );
	}
	else
	{
		g_nx->gDefaultGravity = NxVec3(0.0f);
	}
	g_nx->gDesiredDistance = g_physXProperties.m_fCameraCharacterDistance;
	g_nx->debugMode = g_physXProperties.m_bDebugMode;
	g_nx->gCharacterSpeed = g_physXProperties.m_fCharacterSpeed;

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

CVoid CMain::ManageLODs()
{
	CInt m_numSamples;
	if (g_window.m_windowGL.multiSampling)
		m_numSamples = g_width * g_height *  g_options.m_numSamples;
	else
		m_numSamples = g_width * g_height;

	//CInt index = 0;
	//m_bQuery = CFalse;
	//for (CUInt i = 0; i < g_instancePrefab.size(); i++)
	//{
	//	GLint resultAvailable = GL_FALSE;
	//	glGetQueryObjectiv(g_instancePrefab[i]->GetQueryIndex(), GL_QUERY_RESULT_AVAILABLE, &resultAvailable);

	//	if (resultAvailable)
	//		index++;
	//}
	//CInt numLights = 0;
	//for (CUInt i = 0; i < g_engineLights.size(); i++)
	//{
	//	if (Cmp(g_shadowProperties.m_directionalLightName, g_engineLights[i]->m_abstractLight->GetName()))
	//		continue;
	//	numLights++;
	//	GLint resultAvailable = GL_FALSE;
	//	glGetQueryObjectiv(g_engineLights[i]->GetQueryIndex(), GL_QUERY_RESULT_AVAILABLE, &resultAvailable);

	//	if (resultAvailabrenle)
	//		index++;
	//}

	//if (index == g_instancePrefab.size() + numLights)
	//{
		//all results are available
		m_bQuery = CTrue;
	//}

	if (m_bQuery)
	{
		for (CUInt i = 0; i < g_instancePrefab.size(); i++)
		{
			GLint result;
			glGetQueryObjectiv(g_instancePrefab[i]->GetQueryIndex(), GL_QUERY_RESULT, &result);
			g_instancePrefab[i]->SetResult(result);

			if (result == 0)
			{
				g_instancePrefab[i]->SetSceneVisible(0, CFalse);
				g_instancePrefab[i]->SetSceneVisible(1, CFalse);
				g_instancePrefab[i]->SetSceneVisible(2, CFalse);
				continue;
			}

			CFloat percentage;
			percentage = ((CFloat)result / (CFloat)m_numSamples) * 100.0f;
			if (percentage >= 12.0f || (g_instancePrefab[i]->GetDistanceFromCamera() - g_instancePrefab[i]->GetRadius() <= (g_instancePrefab[i]->GetRadius() + 20.0f)))
			{
				if (g_instancePrefab[i]->GetElapsedTime() >= 0.01f)
				{
					if (g_instancePrefab[i]->GetSceneVisible(1) && g_instancePrefab[i]->GetScene(1) && g_instancePrefab[i]->GetScene(1)->m_hasAnimation)
					{
						for (CUInt j = 0; j < g_instancePrefab[i]->GetScene(1)->m_animationClips.size(); j++)
						{
							if (g_instancePrefab[i]->GetScene(0))
							{
								g_instancePrefab[i]->GetScene(0)->m_animationClips[j]->SetCurrentTime(g_instancePrefab[i]->GetScene(1)->m_animationClips[j]->GetCurrentTime());
								g_instancePrefab[i]->GetScene(0)->m_animationClips[j]->SetCurrentDelayInTime(g_instancePrefab[i]->GetScene(1)->m_animationClips[j]->GetCurrentDelayInTime());
								g_instancePrefab[i]->GetScene(0)->m_animationClips[j]->SetCurrentDelayOutTime(g_instancePrefab[i]->GetScene(1)->m_animationClips[j]->GetCurrentDelayOutTime());
								g_instancePrefab[i]->GetScene(0)->m_animationClips[j]->SetCurrentWeight(g_instancePrefab[i]->GetScene(1)->m_animationClips[j]->GetCurrentWeight());
							}
						}
					}
					else if (g_instancePrefab[i]->GetSceneVisible(2) && g_instancePrefab[i]->GetScene(2) && g_instancePrefab[i]->GetScene(2)->m_hasAnimation)
					{
						for (CUInt j = 0; j < g_instancePrefab[i]->GetScene(2)->m_animationClips.size(); j++)
						{
							if (g_instancePrefab[i]->GetScene(0))
							{
								g_instancePrefab[i]->GetScene(0)->m_animationClips[j]->SetCurrentTime(g_instancePrefab[i]->GetScene(2)->m_animationClips[j]->GetCurrentTime());
								g_instancePrefab[i]->GetScene(0)->m_animationClips[j]->SetCurrentDelayInTime(g_instancePrefab[i]->GetScene(2)->m_animationClips[j]->GetCurrentDelayInTime());
								g_instancePrefab[i]->GetScene(0)->m_animationClips[j]->SetCurrentDelayOutTime(g_instancePrefab[i]->GetScene(2)->m_animationClips[j]->GetCurrentDelayOutTime());
								g_instancePrefab[i]->GetScene(0)->m_animationClips[j]->SetCurrentWeight(g_instancePrefab[i]->GetScene(2)->m_animationClips[j]->GetCurrentWeight());
							}
						}
					}

					g_instancePrefab[i]->SetSceneVisible(0, CTrue);
					g_instancePrefab[i]->SetSceneVisible(1, CFalse);
					g_instancePrefab[i]->SetSceneVisible(2, CFalse);
					g_instancePrefab[i]->ResetElapsedTime();
				}
			}
			else if (percentage >= 1.5f && percentage < 10.0f)
			{
				if (g_instancePrefab[i]->GetElapsedTime() >= 0.01f)
				{
					if (g_instancePrefab[i]->GetSceneVisible(0) && g_instancePrefab[i]->GetScene(0) && g_instancePrefab[i]->GetScene(0)->m_hasAnimation)
					{
						for (CUInt j = 0; j < g_instancePrefab[i]->GetScene(0)->m_animationClips.size(); j++)
						{
							if (g_instancePrefab[i]->GetScene(1))
							{
								g_instancePrefab[i]->GetScene(1)->m_animationClips[j]->SetCurrentTime(g_instancePrefab[i]->GetScene(0)->m_animationClips[j]->GetCurrentTime());
								g_instancePrefab[i]->GetScene(1)->m_animationClips[j]->SetCurrentDelayInTime(g_instancePrefab[i]->GetScene(0)->m_animationClips[j]->GetCurrentDelayInTime());
								g_instancePrefab[i]->GetScene(1)->m_animationClips[j]->SetCurrentDelayOutTime(g_instancePrefab[i]->GetScene(0)->m_animationClips[j]->GetCurrentDelayOutTime());
								g_instancePrefab[i]->GetScene(1)->m_animationClips[j]->SetCurrentWeight(g_instancePrefab[i]->GetScene(0)->m_animationClips[j]->GetCurrentWeight());
							}
						}
					}
					else if (g_instancePrefab[i]->GetSceneVisible(2) && g_instancePrefab[i]->GetScene(2) && g_instancePrefab[i]->GetScene(2)->m_hasAnimation)
					{
						for (CUInt j = 0; j < g_instancePrefab[i]->GetScene(2)->m_animationClips.size(); j++)
						{
							if (g_instancePrefab[i]->GetScene(1))
							{
								g_instancePrefab[i]->GetScene(1)->m_animationClips[j]->SetCurrentTime(g_instancePrefab[i]->GetScene(2)->m_animationClips[j]->GetCurrentTime());
								g_instancePrefab[i]->GetScene(1)->m_animationClips[j]->SetCurrentDelayInTime(g_instancePrefab[i]->GetScene(2)->m_animationClips[j]->GetCurrentDelayInTime());
								g_instancePrefab[i]->GetScene(1)->m_animationClips[j]->SetCurrentDelayOutTime(g_instancePrefab[i]->GetScene(2)->m_animationClips[j]->GetCurrentDelayOutTime());
								g_instancePrefab[i]->GetScene(1)->m_animationClips[j]->SetCurrentWeight(g_instancePrefab[i]->GetScene(2)->m_animationClips[j]->GetCurrentWeight());
							}
						}
					}

					if (g_instancePrefab[i]->GetPrefab()->GetHasLod(1))
					{
						g_instancePrefab[i]->SetSceneVisible(0, CFalse);
						g_instancePrefab[i]->SetSceneVisible(1, CTrue);
						g_instancePrefab[i]->SetSceneVisible(2, CFalse);
						g_instancePrefab[i]->ResetElapsedTime();
					}
				}
			}
			else if (percentage > 0.003f && percentage < 1.4f)
			{
				if (g_instancePrefab[i]->GetElapsedTime() >= 0.01f)
				{
					if (!g_instancePrefab[i]->GetSceneVisible(0) && !g_instancePrefab[i]->GetSceneVisible(1) && !g_instancePrefab[i]->GetSceneVisible(2))
					{
						if (g_instancePrefab[i]->GetPrefab()->GetHasLod(2))
						{
							g_instancePrefab[i]->SetSceneVisible(0, CFalse);
							g_instancePrefab[i]->SetSceneVisible(1, CFalse);
							g_instancePrefab[i]->SetSceneVisible(2, CTrue);
							g_instancePrefab[i]->ResetElapsedTime();
						}
						else if (g_instancePrefab[i]->GetPrefab()->GetHasLod(1))
						{
							g_instancePrefab[i]->SetSceneVisible(0, CFalse);
							g_instancePrefab[i]->SetSceneVisible(1, CTrue);
							g_instancePrefab[i]->SetSceneVisible(2, CFalse);
							g_instancePrefab[i]->ResetElapsedTime();
						}
						else if (g_instancePrefab[i]->GetPrefab()->GetHasLod(0))
						{
							g_instancePrefab[i]->SetSceneVisible(0, CTrue);
							g_instancePrefab[i]->SetSceneVisible(1, CFalse);
							g_instancePrefab[i]->SetSceneVisible(2, CFalse);
							g_instancePrefab[i]->ResetElapsedTime();
						}
					}
					if (g_instancePrefab[i]->GetPrefab()->GetHasLod(2))
					{
						if (g_instancePrefab[i]->GetSceneVisible(1))
						{
							if (g_instancePrefab[i]->GetScene(1) && g_instancePrefab[i]->GetScene(1)->m_hasAnimation)
							{
								for (CUInt j = 0; j < g_instancePrefab[i]->GetScene(1)->m_animationClips.size(); j++)
								{
									if (g_instancePrefab[i]->GetScene(2))
									{
										g_instancePrefab[i]->GetScene(2)->m_animationClips[j]->SetCurrentTime(g_instancePrefab[i]->GetScene(1)->m_animationClips[j]->GetCurrentTime());
										g_instancePrefab[i]->GetScene(2)->m_animationClips[j]->SetCurrentDelayInTime(g_instancePrefab[i]->GetScene(1)->m_animationClips[j]->GetCurrentDelayInTime());
										g_instancePrefab[i]->GetScene(2)->m_animationClips[j]->SetCurrentDelayOutTime(g_instancePrefab[i]->GetScene(1)->m_animationClips[j]->GetCurrentDelayOutTime());
										g_instancePrefab[i]->GetScene(2)->m_animationClips[j]->SetCurrentWeight(g_instancePrefab[i]->GetScene(1)->m_animationClips[j]->GetCurrentWeight());
									}
								}
							}

							g_instancePrefab[i]->SetSceneVisible(0, CFalse);
							g_instancePrefab[i]->SetSceneVisible(1, CFalse);
							g_instancePrefab[i]->SetSceneVisible(2, CTrue);
							g_instancePrefab[i]->ResetElapsedTime();
						}
					}
					else if (g_instancePrefab[i]->GetSceneVisible(0))
					{
						if (g_instancePrefab[i]->GetPrefab()->GetHasLod(1))
						{
							if (g_instancePrefab[i]->GetScene(0) && g_instancePrefab[i]->GetScene(0)->m_hasAnimation)
							{
								for (CUInt j = 0; j < g_instancePrefab[i]->GetScene(0)->m_animationClips.size(); j++)
								{
									if (g_instancePrefab[i]->GetScene(1))
									{
										g_instancePrefab[i]->GetScene(1)->m_animationClips[j]->SetCurrentTime(g_instancePrefab[i]->GetScene(0)->m_animationClips[j]->GetCurrentTime());
										g_instancePrefab[i]->GetScene(1)->m_animationClips[j]->SetCurrentDelayInTime(g_instancePrefab[i]->GetScene(0)->m_animationClips[j]->GetCurrentDelayInTime());
										g_instancePrefab[i]->GetScene(1)->m_animationClips[j]->SetCurrentDelayOutTime(g_instancePrefab[i]->GetScene(0)->m_animationClips[j]->GetCurrentDelayOutTime());
										g_instancePrefab[i]->GetScene(1)->m_animationClips[j]->SetCurrentWeight(g_instancePrefab[i]->GetScene(0)->m_animationClips[j]->GetCurrentWeight());
									}
								}
							}

							g_instancePrefab[i]->SetSceneVisible(0, CFalse);
							g_instancePrefab[i]->SetSceneVisible(1, CTrue);
							g_instancePrefab[i]->SetSceneVisible(2, CFalse);
							g_instancePrefab[i]->ResetElapsedTime();
						}
					}

				}
			}

			g_instancePrefab[i]->SetElapsedTime(g_elapsedTime);
		}

		for (CUInt i = 0; i < g_engineLights.size(); i++)
		{
			if (Cmp(g_shadowProperties.m_directionalLightName, g_engineLights[i]->m_abstractLight->GetName()))
				continue;

			GLint result;
			glGetQueryObjectiv(g_engineLights[i]->GetQueryIndex(), GL_QUERY_RESULT, &result);

			if (result == 0 && g_engineLights[i]->GetDistanceFromCamera() > g_engineLights[i]->GetRadius())
				g_engineLights[i]->SetVisible(CFalse);
			else
				g_engineLights[i]->SetVisible(CTrue);
		}

	}
}

CVoid CMain::EngineLightPass()
{
	if( !g_useOldRenderingStyle && g_options.m_enableFBO && g_options.m_enableShader && g_render.UsingShader() && g_render.m_useShader && g_engineLights.size() > 0 )
	{
		//set all the lights here
		for( CUInt j = 0; j < g_engineLights.size(); j++ )
		{
			CInstanceLight *instanceLight = g_engineLights[j];
			g_currentInstanceLight = instanceLight;

			CVec3f  Position;
			if( instanceLight->m_parent )
			{
				float *matrix = (float *)instanceLight->m_parent->GetLocalToWorldMatrix();
				Position.x = matrix[12]; Position.y = matrix[13]; Position.z =  matrix[14]; 
			}
			else
			{
				Position.x = instanceLight->m_abstractLight->GetPosition()[0];
				Position.y = instanceLight->m_abstractLight->GetPosition()[1];
				Position.z = instanceLight->m_abstractLight->GetPosition()[2];
			}
			CBool visible = CFalse;
			if (Cmp(g_shadowProperties.m_directionalLightName, g_engineLights[j]->m_abstractLight->GetName()))
				visible = CTrue;
			else
				visible = instanceLight->GetVisible();

			if( g_camera->m_cameraManager->IsSphereInFrustum( Position.x, Position.y, Position.z, (CFloat)instanceLight->GetRadius() ) && visible)
			{
				if( Cmp(g_shadowProperties.m_directionalLightName, g_engineLights[j]->m_abstractLight->GetName() ) )
				{
					g_firstPass = CTrue;
				//	GLenum DrawBuffers[] = { GL_COLOR_ATTACHMENT0_EXT, GL_COLOR_ATTACHMENT1_EXT, GL_COLOR_ATTACHMENT2_EXT, GL_COLOR_ATTACHMENT3_EXT,
				//	GL_COLOR_ATTACHMENT4_EXT, GL_COLOR_ATTACHMENT5_EXT};
				//	glDrawBuffers(eGBUFFER_NUM_TEXTURES, DrawBuffers);
				}
				else
				{
					g_firstPass = CFalse;
				}
				glDrawBuffer( GL_COLOR_ATTACHMENT0_EXT );

				if( !g_useOldRenderingStyle && g_window.m_windowGL.multiSampling && g_options.m_numSamples && g_options.m_enableFBO)
				{
					if( g_numLights % 2 == 0 )
						g_render.BindForWriting( m_mFboID );
					else
						g_render.BindForWriting( m_mFboID2 );
				}
				else if( !g_useOldRenderingStyle && g_options.m_enableFBO)
				{
					if( g_numLights % 2 == 0 )
						g_render.BindForWriting( m_fboID );
					else
						g_render.BindForWriting( m_fboID2 );
				}
				if( g_numLights != 0 )
				{
					glClearColor( 0.0f, 0.0f, 0.0f, 1.0f );
					glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
				}
				glViewport( 0, 0, g_width, g_height );

				if( g_numLights < 8 ) // 8 lights are supported
				{
					CBool defaultLight;
					if( Cmp(g_shadowProperties.m_directionalLightName, g_engineLights[j]->m_abstractLight->GetName() ) || ( Cmp(g_shadowProperties.m_directionalLightName, "\n" ) && g_engineLights[j]->m_abstractLight->GetType() == eLIGHTTYPE_DIRECTIONAL ))
						defaultLight = CTrue;
					else
						defaultLight = CFalse;
					if( g_render.SetInstanceLight(g_engineLights[j],0, defaultLight) )
					{
						if( defaultLight )
							Cpy( g_shadowProperties.m_directionalLightName, g_engineLights[j]->m_abstractLight->GetName() );
						g_octree->Render();
						Render3DAnimatedModels( CTrue);
						Render3DModelsControlledByPhysX();
						BlendLights(g_numLights);
						++g_numLights;
						g_octree->ResetOctreeGeoCount();
					}
				}
			}
		}
		if( g_numLights > 1 && (g_numLights % 2 == 0) )
		{
			if( !g_useOldRenderingStyle && g_window.m_windowGL.multiSampling && g_options.m_numSamples && g_options.m_enableFBO)
				g_render.BindForReading( m_mFboID2 );
			else if( !g_useOldRenderingStyle && g_options.m_enableFBO)
				g_render.BindForReading( m_fboID2 );

			glReadBuffer(GL_COLOR_ATTACHMENT0_EXT);

			if( !g_useOldRenderingStyle && g_window.m_windowGL.multiSampling && g_options.m_numSamples && g_options.m_enableFBO)
				g_render.BindForWriting( m_mFboID );
			else if( !g_useOldRenderingStyle && g_options.m_enableFBO)
				g_render.BindForWriting( m_fboID );

			glDrawBuffer(GL_COLOR_ATTACHMENT0_EXT);

			if ( g_render.UsingFBOs() && g_options.m_enableFBO )
			{
				glBlitFramebufferEXT(0, 0, g_width, g_height, 0, 0, g_width, g_height, GL_COLOR_BUFFER_BIT, GL_NEAREST);
				glBlitFramebufferEXT(0, 0, g_width, g_height, 0, 0, g_width, g_height, GL_DEPTH_BUFFER_BIT, GL_NEAREST );
			}	
		}
	} //if !g_useOldRenderingStyle
	else if( g_engineLights.size() > 0 && ( g_useOldRenderingStyle || !g_options.m_enableFBO ) && g_render.UsingShader() && g_render.m_useShader )//old rendering style
	{
		glDrawBuffer( GL_BACK );
		//set all the lights here
		for( CUInt j = 0; j < g_engineLights.size(); j++ )
		{
			if( j == 0 )
			if( g_numLights < 1 ) // 1 light is supported
			{
				if( g_render.SetInstanceLight(g_engineLights[j],0, CTrue) )
				{
					g_octree->Render();
					Render3DAnimatedModels( CTrue);
					Render3DModelsControlledByPhysX();
					g_octree->ResetOctreeGeoCount();
					++g_numLights;
					break;
				}
			}
		}
	}
	else
	{
		for( CUInt j = 0; j < g_engineLights.size(); j++ )
		{
			if( g_numLights < 8 ) // 8 lights is supported
			{
				if( g_render.SetInstanceLight(g_engineLights[j],g_numLights, CTrue) )
				{
					++g_numLights;
				}
			}
		}
	}
}

CVoid CMain::COLLADALightPass()
{
	if( !g_useOldRenderingStyle && g_options.m_enableFBO && g_options.m_enableShader && g_render.UsingShader() && g_render.m_useShader )
	{
		//COLLADA Lights. Not recommended
		for( CUInt i = 0 ; i < g_scene.size(); i++ )
		{ 
			g_render.SetScene( g_scene[i] );
			
			//set all the lights here
			for( CUInt j = 0; j < g_render.GetScene()->GetLightInstanceCount(); j++ )
			{
				CInstanceLight *instanceLight = g_render.GetScene()->GetLightInstances(j);
				if( instanceLight->m_abstractLight->GetType() == eLIGHTTYPE_AMBIENT )
					continue;

				g_currentInstanceLight = instanceLight;

				CVec3f  Position;
				if( instanceLight->m_parent )
				{
					float *matrix = (float *)instanceLight->m_parent->GetLocalToWorldMatrix();
					Position.x = matrix[12]; Position.y = matrix[13]; Position.z =  matrix[14]; 
				}
				else
				{
					Position.x = instanceLight->m_abstractLight->GetPosition()[0];
					Position.y = instanceLight->m_abstractLight->GetPosition()[1];
					Position.z = instanceLight->m_abstractLight->GetPosition()[2];
				}

				if( g_camera->m_cameraManager->IsSphereInFrustum( Position.x, Position.y, Position.z, (CFloat)instanceLight->GetRadius() ) )
				{
					if( Cmp(g_shadowProperties.m_directionalLightName, instanceLight->m_abstractLight->GetName() ) )
					{
						g_firstPass = CTrue;
					//	GLenum DrawBuffers[] = { GL_COLOR_ATTACHMENT0_EXT, GL_COLOR_ATTACHMENT1_EXT, GL_COLOR_ATTACHMENT2_EXT, GL_COLOR_ATTACHMENT3_EXT,
					//	GL_COLOR_ATTACHMENT4_EXT, GL_COLOR_ATTACHMENT5_EXT};
					//	glDrawBuffers(eGBUFFER_NUM_TEXTURES, DrawBuffers);
					}
					else
					{
						g_firstPass = CFalse;
					}
					glDrawBuffer( GL_COLOR_ATTACHMENT0_EXT );

					if( !g_useOldRenderingStyle && g_window.m_windowGL.multiSampling && g_options.m_numSamples && g_options.m_enableFBO)
					{
						if( g_numLights % 2 == 0 )
							g_render.BindForWriting( m_mFboID );
						else
							g_render.BindForWriting( m_mFboID2 );
					}
					else if( !g_useOldRenderingStyle && g_options.m_enableFBO)
					{
						if( g_numLights % 2 == 0 )
							g_render.BindForWriting( m_fboID );
						else
							g_render.BindForWriting( m_fboID2 );
					}
					if( g_numLights != 0 ) //clear m_fboID2 or m_mFboID2
					{
						glClearColor( 0.0f, 0.0f, 0.0f, 1.0f );
						glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
					}
					glViewport( 0, 0, g_width, g_height );

					if( g_numLights < 8 ) // 8 lights are supported
					{
						if( instanceLight )
						{
							CBool defaultLight;
							if( Cmp(g_shadowProperties.m_directionalLightName, instanceLight->m_abstractLight->GetName() ) || ( Cmp(g_shadowProperties.m_directionalLightName, "\n" ) && instanceLight->m_abstractLight->GetType() == eLIGHTTYPE_DIRECTIONAL ))
								defaultLight = CTrue;
							else
								defaultLight = CFalse;
							if( g_render.SetInstanceLight(instanceLight,0, defaultLight) )
							{
								if( defaultLight )
									Cpy( g_shadowProperties.m_directionalLightName, instanceLight->m_abstractLight->GetName() );
								g_octree->Render();
								Render3DAnimatedModels( CTrue);
								Render3DModelsControlledByPhysX();
								BlendLights(g_numLights);
								++g_numLights;
								g_octree->ResetOctreeGeoCount();
								g_render.SetScene( g_scene[i] );
							}
						}
					}
				}
			}
		}
		if( g_numLights > 1 && (g_numLights % 2 == 0) )
		{
			if( !g_useOldRenderingStyle && g_window.m_windowGL.multiSampling && g_options.m_numSamples && g_options.m_enableFBO)
				g_render.BindForReading( m_mFboID2 );
			else if( !g_useOldRenderingStyle && g_options.m_enableFBO)
				g_render.BindForReading( m_fboID2 );

			glReadBuffer(GL_COLOR_ATTACHMENT0_EXT);

			if( !g_useOldRenderingStyle && g_window.m_windowGL.multiSampling && g_options.m_numSamples && g_options.m_enableFBO)
				g_render.BindForWriting( m_mFboID );
			else if( !g_useOldRenderingStyle && g_options.m_enableFBO)
				g_render.BindForWriting( m_fboID );

			glDrawBuffer(GL_COLOR_ATTACHMENT0_EXT);

			if ( g_render.UsingFBOs() && g_options.m_enableFBO )
			{
				glBlitFramebufferEXT(0, 0, g_width, g_height, 0, 0, g_width, g_height, GL_COLOR_BUFFER_BIT, GL_NEAREST);
				glBlitFramebufferEXT(0, 0, g_width, g_height, 0, 0, g_width, g_height, GL_DEPTH_BUFFER_BIT, GL_NEAREST );
			}
		}
	} //if !g_useOldRenderingStyle
	else if( ( g_useOldRenderingStyle || !g_options.m_enableFBO ) && g_render.UsingShader() && g_render.m_useShader )//old rendering style
	{
		for( CUInt i = 0 ; i < g_scene.size(); i++ )
		{
			g_render.SetScene( g_scene[i] );
			
			//set all the lights here
			for( CUInt j = 0; j < g_render.GetScene()->GetLightInstanceCount(); j++ )
			{
				if( j == 0 )
				{
					if( g_numLights < 1 ) // 1 lights is supported
					{
						CInstanceLight *instanceLight = g_render.GetScene()->GetLightInstances(j);
						if( instanceLight )
						{
							if( g_render.SetInstanceLight(instanceLight,0, CTrue) )
							{
								g_octree->Render();
								Render3DAnimatedModels( CTrue);
								Render3DModelsControlledByPhysX();
								g_octree->ResetOctreeGeoCount();
								++g_numLights;
							}
						}
					}
				}
			}
		}
	}
	else
	{
		for( CUInt i = 0 ; i < g_scene.size(); i++ )
		{
			g_render.SetScene( g_scene[i] );
			
			//set all the lights here
			for( CUInt j = 0; j < g_render.GetScene()->GetLightInstanceCount(); j++ )
			{
				if( g_numLights < 8 ) // 8 lights is supported
				{
					CInstanceLight *instanceLight = g_render.GetScene()->GetLightInstances(j);
					if( instanceLight )
					{
						if( g_render.SetInstanceLight(instanceLight,g_numLights, CTrue) )
						{
							++g_numLights;
						}
					}
				}
			}
		}
	}
}

CVoid CMain::FixedFunctionLightPass()
{
	if( !g_render.UsingShader() || !g_render.m_useShader || !g_options.m_enableShader)
	{
		g_octree->Render();
		Render3DAnimatedModels( CTrue);
		Render3DModelsControlledByPhysX();

	}
}

CVoid CMain::DefaultLightPass()
{
	if( g_numLights == 0 )
	{
		if (!g_render.UsingShader())
			glEnable(GL_LIGHT0);	 //just for per vertex lighting 	

		////This is the properties of the camera light
		GLfloat light_pos0[4] = {g_camera->m_perspectiveCameraPos.x,g_camera->m_perspectiveCameraPos.y, g_camera->m_perspectiveCameraPos.z, 1.0f };
		////g_defaultDirectionalLight.x = light_pos0[0]; g_defaultDirectionalLight.y = light_pos0[1]; g_defaultDirectionalLight.z = light_pos0[2]; g_defaultDirectionalLight.w = 0.0f;

		//GLfloat light_ambient0[4] = { 0.5f, 0.5f, 0.5f, 0.0f };
		//GLfloat light_diffuse0[4] = { 0.4f, 0.4f, 0.4f, 1.0f };
		//GLfloat light_specular0[4] = { 0.6f, 0.6f, 0.6f, 1.0f };
		//GLfloat light_shininess0 = 5.0f;

		GLfloat light_ambient0[4] = { 0.0f, 0.0f, 0.0f, 0.0f };
		GLfloat light_diffuse0[4] = { 0.0f, 0.0f, 0.0f, 1.0f };
		GLfloat light_specular0[4] = { 0.0f, 0.0f, 0.0f, 1.0f };
		GLfloat light_shininess0 = 5.0f;

		glLightfv( GL_LIGHT0, GL_POSITION, light_pos0  );
		glLightfv( GL_LIGHT0, GL_AMBIENT , light_ambient0 );
		glLightfv( GL_LIGHT0, GL_DIFFUSE , light_diffuse0 );
		glLightfv( GL_LIGHT0, GL_SPECULAR, light_specular0 );
		glLightf ( GL_LIGHT0, GL_SHININESS, light_shininess0 );
		glLightf ( GL_LIGHT0, GL_SPOT_CUTOFF,(GLfloat)180.0f );
		glLightf ( GL_LIGHT0, GL_CONSTANT_ATTENUATION , (GLfloat)1.0f );
		glLightf ( GL_LIGHT0, GL_LINEAR_ATTENUATION,	(GLfloat)0.0f );
		glLightf ( GL_LIGHT0, GL_QUADRATIC_ATTENUATION, (GLfloat)0.0f );
		
		g_octree->Render();
		Render3DAnimatedModels( CTrue);
		Render3DModelsControlledByPhysX();

		if( g_options.m_enableShader  && g_render.UsingShader()  && g_render.m_useShader)
			g_octree->ResetOctreeGeoCount();

		g_numLights++;
	}
}

CVoid CMain::BlendFogWithScene()
{
	if( !g_useOldRenderingStyle && g_window.m_windowGL.multiSampling && g_options.m_numSamples && g_options.m_enableFBO)
	{
		g_render.BindForReading( m_mFboID );
		glReadBuffer(GL_COLOR_ATTACHMENT0_EXT);
		g_render.BindForWriting( m_fboID );
		glDrawBuffer(GL_COLOR_ATTACHMENT0_EXT);
		glBlitFramebufferEXT(0, 0, g_width, g_height, 0, 0, g_width, g_height, GL_COLOR_BUFFER_BIT, GL_NEAREST);

		g_render.BindForReading( m_mFboIDFogDof );
		glReadBuffer(GL_COLOR_ATTACHMENT0_EXT);
		g_render.BindForWriting( m_fboIDFogDof );
		glDrawBuffer(GL_COLOR_ATTACHMENT0_EXT);
		glBlitFramebufferEXT(0, 0, g_width, g_height, 0, 0, g_width, g_height, GL_COLOR_BUFFER_BIT, GL_NEAREST);
	}
	glDrawBuffer(GL_COLOR_ATTACHMENT0_EXT);
	glActiveTextureARB(GL_TEXTURE0_ARB);
	glBindTexture(GL_TEXTURE_2D, m_textureTarget[0]);
	glActiveTextureARB(GL_TEXTURE1_ARB);
	glBindTexture(GL_TEXTURE_2D, m_textureFogDof);

	g_render.BindForWriting( m_fboIDSum );

	glClear(GL_COLOR_BUFFER_BIT);

	glUseProgram(g_render.m_blendTexturesProgram);
	glUniform1i( glGetUniformLocation(g_render.m_blendTexturesProgram, "tex_unit_0"), 0 );
	glUniform1i( glGetUniformLocation(g_render.m_blendTexturesProgram, "tex_unit_1"), 1 );
	glUniform1i( glGetUniformLocation(g_render.m_blendTexturesProgram, "blendLights"), CFalse );


	glMatrixMode(GL_PROJECTION); glPushMatrix(); glLoadIdentity();
	gluOrtho2D(0, g_width, 0, g_height);
	glMatrixMode(GL_MODELVIEW); glPushMatrix();	glLoadIdentity();
	glBegin(GL_QUADS);
	glTexCoord2i(0,	0);	glVertex2i(0, 0); 
	glTexCoord2i(1, 0);	glVertex2i(g_width, 0);
	glTexCoord2i(1, 1);	glVertex2i(g_width, g_height);
	glTexCoord2i(0, 1);	glVertex2i(0, g_height);
	glEnd();
	glFlush();

	glMatrixMode(GL_PROJECTION); glPopMatrix();
	glMatrixMode(GL_MODELVIEW); glPopMatrix();
	//glActiveTextureARB(GL_TEXTURE0_ARB);
	//glBindTexture(GL_TEXTURE_2D, 0);
	//glActiveTextureARB(GL_TEXTURE1_ARB);
	//glBindTexture(GL_TEXTURE_2D, 0);
	glUseProgram(0);
	g_render.BindFBO(0);

	//copy  sum to the default FBO
	g_render.BindForReading( m_fboIDSum );
	glReadBuffer(GL_COLOR_ATTACHMENT0_EXT);
	if( !g_useOldRenderingStyle && g_window.m_windowGL.multiSampling && g_options.m_numSamples && g_options.m_enableFBO)
	{
		g_render.BindForWriting( m_mFboID );
	}
	else if( !g_useOldRenderingStyle && g_options.m_enableFBO)
	{
		g_render.BindForWriting( m_fboID );
	}
	glDrawBuffer(GL_COLOR_ATTACHMENT0_EXT);

	if ( g_render.UsingFBOs() && g_options.m_enableFBO )
		glBlitFramebufferEXT(0, 0, g_width, g_height, 0, 0, g_width, g_height, GL_COLOR_BUFFER_BIT, GL_LINEAR);

}

CVoid CMain::BlendLights(CUInt lightIndex)
{
	if( lightIndex == 0 ) return;
	if( !g_useOldRenderingStyle && g_window.m_windowGL.multiSampling && g_options.m_numSamples && g_options.m_enableFBO)
	{
		g_render.BindForReading( m_mFboID );
		glReadBuffer(GL_COLOR_ATTACHMENT0_EXT);
		g_render.BindForWriting( m_fboID );
		glDrawBuffer(GL_COLOR_ATTACHMENT0_EXT);
		glBlitFramebufferEXT(0, 0, g_width, g_height, 0, 0, g_width, g_height, GL_COLOR_BUFFER_BIT, GL_NEAREST);

		g_render.BindForReading( m_mFboID2 );
		glReadBuffer(GL_COLOR_ATTACHMENT0_EXT);
		g_render.BindForWriting( m_fboID2 );
		glDrawBuffer(GL_COLOR_ATTACHMENT0_EXT);
		glBlitFramebufferEXT(0, 0, g_width, g_height, 0, 0, g_width, g_height, GL_COLOR_BUFFER_BIT, GL_NEAREST);

	}
	glDrawBuffer(GL_COLOR_ATTACHMENT0_EXT);
	glActiveTextureARB(GL_TEXTURE0_ARB);
	glBindTexture(GL_TEXTURE_2D, m_textureTarget[0]);
	glActiveTextureARB(GL_TEXTURE1_ARB);
	glBindTexture(GL_TEXTURE_2D, m_textureTargetSwapLights);

	g_render.BindForWriting( m_fboIDSum );

	glClear(GL_COLOR_BUFFER_BIT);

	glUseProgram(g_render.m_blendTexturesProgram);
	glUniform1i( glGetUniformLocation(g_render.m_blendTexturesProgram, "tex_unit_0"), 0 );
	glUniform1i( glGetUniformLocation(g_render.m_blendTexturesProgram, "tex_unit_1"), 1 );
	glUniform1i( glGetUniformLocation(g_render.m_blendTexturesProgram, "blendLights"), CTrue );


	glMatrixMode(GL_PROJECTION); glPushMatrix(); glLoadIdentity();
	gluOrtho2D(0, g_width, 0, g_height);
	glMatrixMode(GL_MODELVIEW); glPushMatrix();	glLoadIdentity();
	glBegin(GL_QUADS);
	glTexCoord2i(0,	0);	glVertex2i(0, 0); 
	glTexCoord2i(1, 0);	glVertex2i(g_width, 0);
	glTexCoord2i(1, 1);	glVertex2i(g_width, g_height);
	glTexCoord2i(0, 1);	glVertex2i(0, g_height);
	glEnd();
	glFlush();

	glMatrixMode(GL_PROJECTION); glPopMatrix();
	glMatrixMode(GL_MODELVIEW); glPopMatrix();
	//glActiveTextureARB(GL_TEXTURE0_ARB);
	//glBindTexture(GL_TEXTURE_2D, 0);
	//glActiveTextureARB(GL_TEXTURE1_ARB);
	//glBindTexture(GL_TEXTURE_2D, 0);
	glUseProgram(0);
	g_render.BindFBO(0);

	//copy light sum to the default FBO
	g_render.BindForReading( m_fboIDSum );
	glReadBuffer(GL_COLOR_ATTACHMENT0_EXT);
	if( !g_useOldRenderingStyle && g_window.m_windowGL.multiSampling && g_options.m_numSamples && g_options.m_enableFBO)
	{
		if( lightIndex % 2  == 0 )
		{
			g_render.BindForWriting( m_mFboID );
		}
		else
		{
			g_render.BindForWriting( m_mFboID2 );
		}
	}
	else if( !g_useOldRenderingStyle && g_options.m_enableFBO)
	{
		if( lightIndex % 2  == 0 )
			g_render.BindForWriting( m_fboID );
		else
			g_render.BindForWriting( m_fboID2 );
	}
	glDrawBuffer(GL_COLOR_ATTACHMENT0_EXT);

	if ( g_render.UsingFBOs() && g_options.m_enableFBO )
		glBlitFramebufferEXT(0, 0, g_width, g_height, 0, 0, g_width, g_height, GL_COLOR_BUFFER_BIT, GL_LINEAR);

}

CVoid CMain::ResetPhysXCounts()
{
	for( CUInt i = 0 ; i < g_scene.size(); i++ )
	{
		for( CUInt j = 0; j < g_scene[i]->m_instanceGeometries.size(); j++ )
			g_scene[i]->m_instanceGeometries[j]->m_physXCount = 0;
	}
}

CImage *CWater::GetWaterImage( const CChar * name )
{
	if (name == NULL) return NULL;
	for(CUInt i=0; i<g_waterImages.size(); i++)
	{
		if ( ICmp(GetAfterPath(g_waterImages[i]->GetName()), name ) )
			return g_waterImages[i];
	}
	return NULL;
}

CVoid CMain::UpdatePrefabInstanceBB()
{
	for (CUInt i = 0; i < g_instancePrefab.size(); i++)
	{
		if (!g_instancePrefab[i]->GetVisible()) continue;
		g_currentInstancePrefab = g_instancePrefab[i];
		CBool foundTarget = CFalse;
		CScene* scene = NULL;

		CPrefab* prefab = g_instancePrefab[i]->GetPrefab();
		for (CUInt j = 0; j < 3; j++)
		{
			if (prefab && prefab->GetHasLod(j))
			{
				scene = g_instancePrefab[i]->GetScene(j);

				if (!scene) continue;
				if (scene->GetUpdateBB())
				{
					g_instancePrefab[i]->UpdateBoundingBox(CFalse);
					scene->SetUpdateBB(CFalse);
					foundTarget = CTrue;
					break;
				}
			}
		}
		if (foundTarget)
			continue;
	}
}