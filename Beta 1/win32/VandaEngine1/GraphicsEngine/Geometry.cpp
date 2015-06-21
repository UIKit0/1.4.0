/*
 * Copyright 2006 Sony Computer Entertainment Inc.
 *
 * Licensed under the SCEA Shared Source License, Version 1.0 (the "License"); you may not use this 
 * file except in compliance with the License. You may obtain a copy of the License at:
 * http://research.scea.com/scea_shared_source_license.html
 *
 * Unless required by applicable law or agreed to in writing, software distributed under the License 
 * is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or 
 * implied. See the License for the specific language governing permissions and limitations under the 
 * License. 
 */

#include "stdafx.h"
#include "window.h"
#include "geometry.h"
//#include "scene.h"
#include "matrix.h"
#include "render.h"
#include "node.h"
#include "../sceneManagerEngine/lod.h"
#include "../common/utility.h"
#include "../Main.h"


CFloat ticker = 0.0f;

CInputType CPolyGroup::SetType( CChar * s )
{
	if ( CmpIn( s, "VERTEX" ) )
		return ePOINT_SOURCE;
	if ( CmpIn( s, "NORMAL" ) )
		return eNORMAL_SOURCE;
	if ( CmpIn( s, "TEXCOORD" )|| CmpIn( s, "UV" )) // until the schema is locked down got to suppor both
		return eTEXCOORD0_SOURCE;
	if ( CmpIn( s, "TEXTANGENT" ) ) // I'm not sure if TANGENT is also used? Shuld be checked
		return eTANGENT_SOURCE;
	if( CmpIn( s, "TEXBINORMAL" ) )
		return eBINORMAL_SOURCE;

	return eUNKNOWN;
}

/*cfxMaterial* */ CVoid CPolyGroup::SetupMaterialForDraw(CNode *parentNode, CInstance * instance, CBool isSelected, CBool hasDiffuse )
{
	// This method contains code that is common to both Draw and DrawSkinned.  Was going to combine
	// Draw and DrawSkinned into one function, but decided to retain the two seperate functions so the
	// use of parameters would be properly checked.
	//cfxMaterial* currentMaterial = 0;

	// resolve instance material
	for ( CUInt  j=0; j < instance->m_materialInstances.size(); j++ )
	{
		CInstanceMaterial* thisInstanceMaterial = instance->m_materialInstances[j];
		if (thisInstanceMaterial)
		{
			std::string symbol = thisInstanceMaterial->m_symbol;
			std::string target = thisInstanceMaterial->m_target;
			if (symbol == std::string(m_materialName)) {
				if (thisInstanceMaterial->m_targetMaterial)	// common material
				{
					g_render.SetMaterial(thisInstanceMaterial->m_targetMaterial, isSelected, hasDiffuse ); 
					return /*NULL*/;
				}//else if (thisInstanceMaterial->m_targetcfxMaterial) // fx material
				//{
				//	//TODO: we might have to reset FXMaterial back to 0 after we draw this.
				//	currentMaterial = thisInstanceMaterial->m_targetcfxMaterial;
				//}
			}
		}
	}

//	if(currentMaterial)
//	{
//		// Pushes the setparam values into cgFX for this material
//		currentMaterial->setParameterValues();
//
//		// Get the effect pointer
//		cfxEffect* currentEffect = currentMaterial->getEffect();
//
//		// See if there are any common semantic Parameters we need to set
//		CGparameter worldviewprojectionParam = currentEffect->getParameterBySemantic("WORLDVIEWPROJECTION");
//		CGparameter viewinverseParam = currentEffect->getParameterBySemantic("VIEWINVERSE");
//		CGparameter worldParam = currentEffect->getParameterBySemantic("WORLD");
//		CGparameter worldinversetransposeParam = currentEffect->getParameterBySemantic("WORLDINVERSETRANSPOSE");
//		CGparameter lightPositionParam = currentEffect->getParameterBySemantic("LIGHTPOSITION");
//		CGparameter timeParam = currentEffect->getParameterBySemantic("TIME");
//
//		// WORLDVIEWPROJECTION is the world+view+projection matrix of this object which we get from GL
//		if (worldviewprojectionParam)
//		{
//			cgGLSetStateMatrixParameter(worldviewprojectionParam, CG_GL_MODELVIEW_PROJECTION_MATRIX, CG_GL_MATRIX_IDENTITY);
//		}
//
//		// VIEWINVERSE is the inverse of the view matrix which is the same as the camera's LocalToWorldMatrix
//		if(viewinverseParam)
//		{
//			CInstanceCamera *inst = g_render.GetActiveInstanceCamera();
//			cgGLSetMatrixParameterfc( viewinverseParam, (const CFloat *)(inst->m_parent->GetLocalToWorldMatrix())); 
//		}
//
//		// WORLD is the localtoworld matrix for this object which we get from the scene graph
//		if (worldParam)
//		{
//			cgGLSetMatrixParameterfc( worldParam, (const CFloat *)(parentNode->GetLocalToWorldMatrix()));
//		}
//
//		// WORLDINVERSETRANSPOSE is inversetransposelocaltoworld matrix for this object from the scene graph
//		if (worldinversetransposeParam)
//		{
//			cgGLSetMatrixParameterfc( worldinversetransposeParam, (const CFloat *)(parentNode->GetInverseTransposeLocalToWorldMatrix()));
//		}
//
//		// LIGHTPOSITION gets the position of the primary (nearist) light
//		if (lightPositionParam)
//		{
//			assert(g_render.GetScene()->GetLightInstanceCount());  // There should always be one
//#if 0
//			//EXPERIMENTAL this gets the closest light instance, need some way to get the parent node of this polygroup
//			//EXPERIMENTAL may want to check to make sure NearistInstanceLight isn't null
//			CInstanceLight *instanceLight = ??? parentnode->NearistInstanceLight;
//#else
//			CInstanceLight *instanceLight =g_render.GetScene()->GetLightInstances(0);
//#endif
//			// Get the light position from the light instance's parent node's LocalToWorldMatrix
//			// REFACTOR IN PROGRESS we should have an accessor for getting the position
//			CFloat *lightMatrix = (CFloat *)instanceLight->m_parent->GetLocalToWorldMatrix();
//
//			CGtype lighttype = cgGetParameterType(lightPositionParam);
//			if (lighttype == CG_FLOAT3)
//			{
//				cgSetParameter3f(lightPositionParam, lightMatrix[12], lightMatrix[13], lightMatrix[14]);
//			}
//			else if (lighttype == CG_FLOAT4)
//			{
//				cgSetParameter4f(lightPositionParam, lightMatrix[12], lightMatrix[13], lightMatrix[14], 0);
//			}
//		}
//
//		// TIME gets a tickcount for driving animated shaders
//		if (timeParam)
//		{
//			cgSetParameter1f(timeParam, ticker);
//		}
//
//		// Setup the state for the cfxMaterial
//		currentMaterial->setEffectPassState(0,0);
//		return currentMaterial;
//	}
	return /*NULL*/;
}

CVoid CPolyGroup::Draw(CNode *parentNode, CInstance * instance, CBool isSelected, CBool hasDiffuse )
{
	/*cfxMaterial* currentMaterial = */SetupMaterialForDraw(parentNode, instance, isSelected, hasDiffuse );

	Render();
	
	// Restore the old material state
	//if(currentMaterial)
	//{
	//	currentMaterial->resetEffectPassState(0,0);
	//}
}

CImage* CPolyGroup::GetImage( const CChar * name )
{
	if ( name == NULL )
		return NULL;
	for( CUInt i = 0; i < g_images.size(); i++ )
	{
		if ( ICmp( GetAfterPath(g_images[i]->m_fileName), name ) )
			return g_images[i];
	}
	return NULL;
}

CBool CPolyGroup::SetGlossMap( const CChar * path )
{ 
	CChar m_strTemp[MAX_NAME_SIZE];
	Cpy( m_strTemp, path );
	//CTexture::DeleteTexture( m_glossImg );
	
	CChar m_strTemp2[MAX_NAME_SIZE];
	sprintf( m_strTemp2, "%s%s", m_strTemp, ".dds" );
	CImage* tempImg = GetImage( m_strTemp2 );

	if( tempImg == NULL )
	{
		tempImg = CNew( CImage );
		CChar m_strTemp3[MAX_NAME_SIZE];
		CChar g_currentVSceneNameWithoutDot[MAX_NAME_SIZE];
		Cpy( g_currentVSceneNameWithoutDot, g_currentVSceneName );
		GetWithoutDot( g_currentVSceneNameWithoutDot );

		sprintf( m_strTemp3, "%s%s%s%s", g_currentPackagePath, g_currentPrefabName, "/Textures/", m_strTemp2 );
		if( !CTexture::LoadDDSTexture( tempImg, m_strTemp3, NULL ) )
		{
			CDelete( tempImg );
			return CFalse;
		}
	
		m_glossMapImg = tempImg;
		Cpy( m_strGlossMap, m_strTemp2 );
		GetWithoutDot( m_strGlossMap );
		m_glossMapImg->SetFileName( m_strTemp2 );
		m_hasGlossMap = CTrue;
		m_images.push_back( m_glossMapImg );
		g_images.push_back( m_glossMapImg );
	}
	else
	{
		m_glossMapImg = tempImg;
		m_hasGlossMap = CTrue;
		Cpy( m_strGlossMap, m_strTemp2 );
		GetWithoutDot( m_strGlossMap );

	}
	return CTrue;
}

CBool CPolyGroup::SetDirtMap( const CChar * path )
{ 
	CChar m_strTemp[MAX_NAME_SIZE];
	Cpy( m_strTemp, path );

	//CTexture::DeleteTexture( m_dirtImg );
	
	CChar m_strTemp2[MAX_NAME_SIZE];
	sprintf( m_strTemp2, "%s%s", m_strTemp, ".dds" );
	CImage* tempImg = GetImage( m_strTemp2 );

	if( tempImg == NULL )
	{
		tempImg = CNew( CImage );
		CChar m_strTemp3[MAX_NAME_SIZE];
		CChar g_currentVSceneNameWithoutDot[MAX_NAME_SIZE];
		Cpy( g_currentVSceneNameWithoutDot, g_currentVSceneName );
		GetWithoutDot( g_currentVSceneNameWithoutDot );

		sprintf(m_strTemp3, "%s%s%s%s", g_currentPackagePath, g_currentPrefabName, "/Textures/", m_strTemp2);
		if (!CTexture::LoadDDSTexture(tempImg, m_strTemp3, NULL))
		{
			CDelete( tempImg );
			return CFalse;
		}
	
		m_dirtMapImg = tempImg;
		Cpy( m_strDirtMap, m_strTemp2 );
		GetWithoutDot( m_strDirtMap );
		m_dirtMapImg->SetFileName( m_strTemp2 );
		m_hasDirtMap = CTrue;
		m_images.push_back( m_dirtMapImg );
		g_images.push_back( m_dirtMapImg );
	}
	else
	{
		m_dirtMapImg = tempImg;
		m_hasDirtMap = CTrue;
		Cpy( m_strDirtMap, m_strTemp2 );
		GetWithoutDot( m_strDirtMap );

	}
	return CTrue;
}

CBool CPolyGroup::SetNormalMap( const CChar * path, CFloat bias, CFloat scale )
{ 
	m_parallaxMapBias = bias;
	m_parallaxMapScale = scale;

	CChar m_strTemp[MAX_NAME_SIZE];
	Cpy( m_strTemp, path );
	//CTexture::DeleteTexture( m_normalImg );
	
	CChar m_strTemp2[MAX_NAME_SIZE];
	sprintf( m_strTemp2, "%s%s", m_strTemp, ".dds" );
	CImage* tempImg = GetImage( m_strTemp2 );

	if( tempImg == NULL )
	{
		tempImg = CNew( CImage );
		CChar m_strTemp3[MAX_NAME_SIZE];
		CChar g_currentVSceneNameWithoutDot[MAX_NAME_SIZE];
		Cpy( g_currentVSceneNameWithoutDot, g_currentVSceneName );
		GetWithoutDot( g_currentVSceneNameWithoutDot );

		sprintf(m_strTemp3, "%s%s%s%s", g_currentPackagePath, g_currentPrefabName, "/Textures/", m_strTemp2);
		if (!CTexture::LoadDDSTexture(tempImg, m_strTemp3, NULL))
		{
			CDelete( tempImg );
			return CFalse;
		}
	
		m_normalMapImg = tempImg;
		Cpy( m_strNormalMap, m_strTemp2 );
		GetWithoutDot( m_strNormalMap );
		m_normalMapImg->SetFileName( m_strTemp2 );
		m_hasNormalMap = CTrue;
		m_images.push_back( m_normalMapImg );
		g_images.push_back( m_normalMapImg );
	}
	else
	{
		m_normalMapImg = tempImg;
		m_hasNormalMap = CTrue;
		Cpy( m_strNormalMap, m_strTemp2 );
		GetWithoutDot( m_strNormalMap );

	}
	return CTrue;
}

CBool CPolyGroup::SetDiffuse( const CChar* path )
{ 
	CChar m_strTemp[MAX_NAME_SIZE];
	Cpy( m_strTemp, path );
	//CTexture::DeleteTexture( m_diffuseImg );
	
	CChar m_strTemp2[MAX_NAME_SIZE];
	sprintf( m_strTemp2, "%s%s", m_strTemp, ".dds" );
	CImage* tempImg = GetImage( m_strTemp2 );

	if( tempImg == NULL )
	{
		tempImg = CNew( CImage );
		CChar m_strTemp3[MAX_NAME_SIZE];
		CChar g_currentVSceneNameWithoutDot[MAX_NAME_SIZE];
		Cpy( g_currentVSceneNameWithoutDot, g_currentVSceneName );
		GetWithoutDot( g_currentVSceneNameWithoutDot );

		sprintf(m_strTemp3, "%s%s%s%s", g_currentPackagePath, g_currentPrefabName, "/Textures/", m_strTemp2);
		if (!CTexture::LoadDDSTexture(tempImg, m_strTemp3, NULL))
		{
			CDelete( tempImg );
			return CFalse;
		}
	
		m_diffuseImg = tempImg;
		Cpy( m_strDiffuse, m_strTemp2 );
		GetWithoutDot( m_strDiffuse );
		m_diffuseImg->SetFileName( m_strTemp2 );
		m_hasDiffuse = CTrue;
		m_images.push_back( m_diffuseImg );
		g_images.push_back( m_diffuseImg );
	}
	else
	{
		m_diffuseImg = tempImg;
		m_hasDiffuse = CTrue;
		Cpy( m_strDiffuse, m_strTemp2 );
		GetWithoutDot( m_strDiffuse );

	}
	return CTrue;
}

CVoid CGeometry::CalculateLODInfo( CPhysXAlgorithm algorithm )
{
	List<int> permutation;
	m_lod = CNew( CLOD);
	m_lod->SetAlgorithm( algorithm );
	m_lod->ProgressiveMesh(this, permutation);
	m_lod->PermuteVertices(this, permutation);
	CDelete( m_lod );
}

CImage* CGeometry::GetImage( const CChar * name )
{
	if ( name == NULL )
		return NULL;

	for( CUInt i = 0; i < g_images.size(); i++ )
	{
		if ( ICmp( GetAfterPath(g_images[i]->m_fileName), name ) )
			return g_images[i];
	}
	return NULL;
}

CVoid CGeometry::SetDiffuse( const CChar * path )
{ 
	Cpy( m_strDiffuse, path );
	//CTexture::DeleteTexture( m_normalMapImg );
	
	CChar m_strTemp[MAX_NAME_SIZE];
	sprintf( m_strTemp, "%s%s", m_strDiffuse, ".dds" );
	m_diffuseImg = GetImage( m_strTemp );
	if( m_diffuseImg == NULL )
	{
		m_diffuseImg = CNew( CImage );
		//save functions. it should be copies in WIN32 Project as well
		CChar g_currentVSceneNameWithoutDot[MAX_NAME_SIZE];
		Cpy( g_currentVSceneNameWithoutDot, g_currentVSceneName );
		GetWithoutDot( g_currentVSceneNameWithoutDot );
		sprintf( m_strDiffuse, "%s%s%s%s", "assets/vscenes/", g_currentVSceneNameWithoutDot, "/Textures/", m_strTemp );
		if(!CTexture::LoadDDSTexture( m_diffuseImg, m_strDiffuse, NULL ) )
			MessageBox( NULL, _T(" CGeometry::SetDiffuse>Couldn't load the diffuse texture"), _T("VandaEngine Error"), MB_OK );

		Cpy( m_strDiffuse, m_strTemp );
		GetWithoutDot( m_strDiffuse );
		m_diffuseImg->SetFileName( m_strTemp );
		m_images.push_back( m_diffuseImg );
		g_images.push_back( m_diffuseImg );
	}
	m_hasDiffuse = CTrue;
};

CVoid CGeometry::SetNormalMap( const CChar * path )
{ 
	Cpy( m_strNormalMap, path );
	//CTexture::DeleteTexture( m_normalMapImg );
	
	CChar m_strTemp[MAX_NAME_SIZE];
	sprintf( m_strTemp, "%s%s", m_strNormalMap, ".dds" );
	m_normalMapImg = GetImage( m_strTemp );
	if( m_normalMapImg == NULL )
	{
		m_normalMapImg = CNew( CImage );
		//save functions. it should be copies in WIN32 Project as well
		CChar g_currentVSceneNameWithoutDot[MAX_NAME_SIZE];
		Cpy( g_currentVSceneNameWithoutDot, g_currentVSceneName );
		GetWithoutDot( g_currentVSceneNameWithoutDot );
		sprintf( m_strNormalMap, "%s%s%s%s", "assets/vscenes/", g_currentVSceneNameWithoutDot, "/Textures/", m_strTemp );
		if(!CTexture::LoadDDSTexture( m_normalMapImg, m_strNormalMap, NULL ) )
			MessageBox( NULL, _T(" CGeometry::SetNormalMap>Couldn't load the Normal map"), _T("VandaEngine Error"), MB_OK );

		Cpy( m_strNormalMap, m_strTemp );
		GetWithoutDot( m_strNormalMap );
		m_normalMapImg->SetFileName( m_strTemp );
		m_images.push_back( m_normalMapImg );
		g_images.push_back( m_normalMapImg );
	}
	m_hasNormalMap = CTrue;
	//m_hasHeightMap = CTrue; //Currently the alpha channel of normal map consists of height map
};

CVoid CGeometry::SetDirtMap( const CChar * path ) { 
	Cpy( m_strDirtMap, path );

	//CTexture::DeleteTexture( m_dirtMapImg );
	CChar m_strTemp[MAX_NAME_SIZE];
	sprintf( m_strTemp, "%s%s", m_strDirtMap, ".dds" );
	m_dirtMapImg = GetImage( m_strTemp );
	if( m_dirtMapImg == NULL )
	{
		m_dirtMapImg = CNew( CImage );
		//save functions. it should be copies in WIN32 Project as well
		CChar g_currentVSceneNameWithoutDot[MAX_NAME_SIZE];
		Cpy( g_currentVSceneNameWithoutDot, g_currentVSceneName );
		GetWithoutDot( g_currentVSceneNameWithoutDot );
		sprintf( m_strDirtMap, "%s%s%s%s", "assets/vscenes/", g_currentVSceneNameWithoutDot, "/Textures/", m_strTemp );
		if(!CTexture::LoadDDSTexture( m_dirtMapImg, m_strDirtMap, NULL ) )
			MessageBox( NULL, _T(" CGeometry::SetDirtMap>Couldn't load the dirt map"), _T("VandaEngine Error"), MB_OK );

		Cpy( m_strDirtMap, m_strTemp );
		GetWithoutDot( m_strDirtMap );
		m_dirtMapImg->SetFileName( m_strTemp );
		m_images.push_back( m_dirtMapImg );
		g_images.push_back( m_dirtMapImg );
	}

	m_hasDirtMap = CTrue;
};

CVoid CGeometry::SetGlossMap( const CChar * path ) {
	Cpy( m_strGlossMap, path );
	//CTexture::DeleteTexture( m_glossMapImg );
	CChar m_strTemp[MAX_NAME_SIZE];
	Cpy( m_strTemp, m_strGlossMap );
	m_glossMapImg = GetImage( m_strTemp );
	Cpy( m_strTemp, m_strGlossMap );
	if( m_glossMapImg == NULL )
	{
		m_glossMapImg = CNew( CImage );
		//save functions. it should be copies in WIN32 Project as well
		CChar g_currentVSceneNameWithoutDot[MAX_NAME_SIZE];
		Cpy( g_currentVSceneNameWithoutDot, g_currentVSceneName );
		GetWithoutDot( g_currentVSceneNameWithoutDot );
		sprintf( m_strGlossMap, "%s%s%s%s", "assets/vscenes/", g_currentVSceneNameWithoutDot, "/Textures/", m_strTemp );
		if(!CTexture::LoadDDSTexture( m_glossMapImg, m_strGlossMap, NULL ) )
			MessageBox( NULL, _T(" CGeometry::SetGlossMap>Couldn't load the Gloss map"), _T("VandaEngine Error"), MB_OK );

		Cpy( m_strGlossMap, m_strTemp );
		GetWithoutDot( m_strGlossMap );
		m_glossMapImg->SetFileName( m_strTemp );
		m_images.push_back( m_glossMapImg );
		g_images.push_back( m_glossMapImg );
	}
	m_hasGlossMap = CTrue;
}

CVoid CGeometry::SetHeightMap( const CChar * path ) {
	Cpy( m_strHeightMap, path);
	//CTexture::DeleteTexture( m_heightMapImg );
	CChar m_strTemp[MAX_NAME_SIZE];
	Cpy( m_strTemp, m_strHeightMap );
	m_heightMapImg = GetImage( m_strTemp );
	Cpy( m_strTemp, m_strHeightMap );
	if( m_heightMapImg == NULL )
	{
		m_heightMapImg = CNew( CImage );
		//save functions. it should be copies in WIN32 Project as well
		CChar g_currentVSceneNameWithoutDot[MAX_NAME_SIZE];
		Cpy( g_currentVSceneNameWithoutDot, g_currentVSceneName );
		GetWithoutDot( g_currentVSceneNameWithoutDot );
		sprintf( m_strHeightMap, "%s%s%s%s", "assets/vscenes/", g_currentVSceneNameWithoutDot, "/Textures/", m_strTemp );
		if(!CTexture::LoadDDSTexture( m_heightMapImg, m_strHeightMap, NULL ) )
			MessageBox( NULL, _T(" CGeometry::SetHeightMap>Couldn't load the height map"), _T("VandaEngine Error"), MB_OK );

		Cpy( m_strHeightMap, m_strTemp );
		Cpy( m_strHeightMap, m_strTemp );
		GetWithoutDot( m_strHeightMap );
		m_heightMapImg->SetFileName( m_strTemp );
		m_hasHeightMap = CTrue;
		m_images.push_back( m_heightMapImg );
		g_images.push_back( m_heightMapImg );
	}
};

CVoid CGeometry::SetSkins(CInt nbrJoints )
{
	(CVoid) nbrJoints;
	for(CUInt v = 0; v < m_vertexcount; v++)
	{//	CInt v = indexes[p];
		CInt nbrInf = m_weights[v].m_numInfluences; 
		if( nbrInf )
		{
			CVec3f BSMVert; 
			CVec3f BSMNorm; 
			CVec3f transVert; 
			CVec3f weightVert;
			CVec3f transNorm; 
			CVec3f weightNorm; 
			CInt joint = 0; 
			CFloat weight = 0; 
			CFloat weightTotal = 0; 
			CFloat weightNormalized = 0; 	
			//moved the bind shape multiplication to CScene::ReadGeometry. 
			BSMVert = m_bindPoints[v];
			BSMNorm = m_bindNormals[v];

			for( CInt j = 0; j < nbrInf ; j++)
			{
				joint =  m_weights[v].m_joints[j];				
				weight = m_weights[v].m_weights[j];

				assert( weight >= 0 && weight < 1.01f ); 
				assert( joint > -1 && joint < nbrJoints ); 

				CMatrix3x4TransformSkin( m_skinMatrixStack[joint], BSMVert, transVert );
				CMatrix3x4RotateSkin( m_skinMatrixStack[joint], BSMNorm, transNorm ); 

				transVert.x *= weight;
				transVert.y *= weight; 
				transVert.z *= weight; 

				transNorm.x *= weight; 
				transNorm.y *= weight; 
				transNorm.z *= weight; 

				weightVert.x += transVert.x;  
				weightVert.y += transVert.y;  
				weightVert.z += transVert.z;

				weightNorm.x += transNorm.x;
				weightNorm.y += transNorm.y; 
				weightNorm.z += transNorm.z; 

				weightTotal += m_weights[v].m_weights[j];
			}

			if (weightTotal != 1.0f)
			{
				weightNormalized = 1.0f/weightTotal;
				weightVert.x *= weightNormalized;
				weightVert.y *= weightNormalized; 
				weightVert.z *= weightNormalized; 
				weightNorm.x *= weightNormalized; 
				weightNorm.y *= weightNormalized; 
				weightNorm.z *= weightNormalized; 
			}

			m_points[v] = weightVert;
			if ( m_normals )
				m_normals[v] = weightNorm; 

			if( m_upAxis == eCZUp )
			{
				CVec3f tempv;
				CMatrix TransformZUpRotationForSkins;
				CMatrixLoadIdentity( TransformZUpRotationForSkins );
				CVec4f rot( 1.0, 0.0, 0.0, 90.0 );
				CMatrix4x4RotateAngleAxis( TransformZUpRotationForSkins, rot );

				tempv = m_points[v];
				CMatrixTransform(TransformZUpRotationForSkins, tempv, m_points[v] );

				if ( m_normals )
				{
					tempv = m_normals[v];
					CMatrixTransform(TransformZUpRotationForSkins, tempv, m_normals[v] );
				}
			}
		}
		else
		{
			if (m_skinData->m_bindShapeMats)
			{
				CMatrix3x4TransformSkin( m_skinData->m_bindShapeMats, m_bindPoints[v], m_points[v] );
				CMatrix3x4TransformSkin( m_skinData->m_bindShapeMats, m_bindNormals[v], m_normals[v] );
			}

			//PrintInfo("\nCGeometry::SetSkins:: No Influences", COLOR_RED ); 
		}
	}
}

CChar * CGeometry::GetSkinName()
{
	if ( m_skinData )
		return m_skinData->GetName();
	return 	NULL;
}

	 // Draw function iterates all the polygroups in the geometry and draws them

CVoid CGeometry::Draw(CNode *parentNode, CInstance * instance)
{
	SetRender();
  	if ( m_skinData )
	{
		DrawSkinned(parentNode, instance ); 	
	}
	else
	{
		for (CUInt i = 0; i < m_groups.size(); i++ )
		{
			if (g_main->m_renderQuery && m_groups[i]->m_diffuseImg && m_groups[i]->m_diffuseImg->GetFormat() == 4)
				glDepthMask(GL_FALSE);

			SetGroupRender( m_groups[i] );
			if( g_selectedName == m_nameIndex )
				m_groups[i]->Draw(parentNode, instance, CTrue, m_groups[i]->m_hasDiffuse ); 
			else
				m_groups[i]->Draw(parentNode, instance, CFalse, m_groups[i]->m_hasDiffuse); 
			//TotalNumTris++;

			if (g_main->m_renderQuery && m_groups[i]->m_diffuseImg && m_groups[i]->m_diffuseImg->GetFormat() == 4)
				glDepthMask(GL_TRUE);

			ResetGroupStates( m_groups[i] );
		}			
	}
	ResetStates();
}

CVoid CGeometry::DrawSkinned(CNode *parentNode, CInstance * instance )
{	
	if ( m_skinData )
	{
		// copy the skinned matrices from the joint nodes 
		m_skinData->UpdateCombinedMats(); 
		
		// use the gemetry normals 
		if ( !m_skinData->GetBindNormals() && !m_bindNormals )
		{
			m_bindNormals = CNewData( CVec3f, m_vertexcount );  	
			memcpy(m_bindNormals, m_normals, m_vertexcount * sizeof( CVec3f ));  
			m_bindPoints = CNewData( CVec3f, m_vertexcount );  	
			memcpy(m_bindPoints, m_points, m_vertexcount * sizeof( CVec3f ));  
		}

		m_skinMatrixStack = m_skinData->m_skinningMats; 
		m_skinMatrixStack3x4 = m_skinData->m_skinningMats3x4; 
		SetSkins(m_skinData->GetJointCount());

		if (g_render.UsingVBOs() && g_options.m_enableVBO )
		{
			//g_render.CopyVBOData(GL_ARRAY_BUFFER, m_VBOIDs[eGeoPoints],m_points, m_vertexcount*3*sizeof(CFloat));
			//g_render.CopyVBOData(GL_ARRAY_BUFFER, m_VBOIDs[eGeoNormals],m_normals, m_vertexcount*3*sizeof(CFloat));
			//glBufferSubData is faster than glBufferData, since it doesn't need freeing and reallocating the memory
			g_render.CopyVBOSubData(GL_ARRAY_BUFFER, m_VBOIDs[eGeoPoints],m_points, 0, m_vertexcount*3*sizeof(CFloat));
			g_render.CopyVBOSubData(GL_ARRAY_BUFFER, m_VBOIDs[eGeoNormals],m_normals, 0, m_vertexcount*3*sizeof(CFloat));
		}

		for (CUInt i = 0; i < m_groups.size(); i++ )
		{
			if (g_main->m_renderQuery && m_groups[i]->m_diffuseImg && m_groups[i]->m_diffuseImg->GetFormat() == 4)
				glDepthMask(GL_FALSE);

			SetGroupRender( m_groups[i] );

			if( g_selectedName == m_nameIndex )
				m_groups[i]->Draw(parentNode, instance, CTrue, m_groups[i]->m_hasDiffuse );
			else 
				m_groups[i]->Draw(parentNode, instance, CFalse, m_groups[i]->m_hasDiffuse );

			if (g_main->m_renderQuery && m_groups[i]->m_diffuseImg && m_groups[i]->m_diffuseImg->GetFormat() == 4)
				glDepthMask(GL_TRUE);

			ResetGroupStates( m_groups[i] );
		}		
	}
}

CVoid	CGeometry::GetJointsForWeights( CJoint *& Joints, CInt & NumJoints )
{	
	if ( m_skinData )
	{
		Joints = m_skinData->GetJoints();
		NumJoints = m_skinData->GetJointCount(); 
	}
};

CVoid CGeometry::ResetGrassStates()
{
    glClientActiveTexture(GL_TEXTURE0);
    glDisableClientState(GL_TEXTURE_COORD_ARRAY);

	//alpha map
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, 0);
	glDisable(GL_TEXTURE_2D);

	//diffuse
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, 0);
	glDisable(GL_TEXTURE_2D);

	glDisableClientState(GL_NORMAL_ARRAY);
	glDisableClientState( GL_VERTEX_ARRAY ); 

	g_render.BindVBO(GL_ARRAY_BUFFER, 0 );
	glEnable( GL_CULL_FACE );
}

CVoid CGeometry::SetGrassRender()
{
	glDisable( GL_CULL_FACE );

	glActiveTexture(GL_TEXTURE0);
	glEnable( GL_TEXTURE_2D );
	glBindTexture(GL_TEXTURE_2D, m_diffuseImg->GetId() );

	static CFloat time;
	time += g_elapsedTime;
	if( time >= 360.0f )
		time -= 360.0f;
	if( g_render.UsingShader() && g_render.m_useShader && g_options.m_enableShader)
	{
		glUniform1f(glGetUniformLocation( g_render.m_grassProgram , "time"), NxMath::degToRad(time));

		glUniform1i(glGetUniformLocation( g_render.m_grassProgram , "colorMap"), 0);

		if( g_fogProperties.m_enable )
			glUniform1i(glGetUniformLocation( g_render.m_grassProgram , "enableFog"), CTrue );
		else
			glUniform1i(glGetUniformLocation( g_render.m_grassProgram , "enableFog"), CFalse );
	} 

	if (g_render.UsingVBOs() && g_options.m_enableVBO )
	{
		glEnableClientState(GL_VERTEX_ARRAY); 
		g_render.BindVBO(GL_ARRAY_BUFFER, m_VBOIDs[eGeoPoints]);
		glVertexPointer( 3, GL_FLOAT, 0, NULL);				

		if (m_texCoords[0]) {
			glClientActiveTexture(GL_TEXTURE0);
			glEnableClientState(GL_TEXTURE_COORD_ARRAY);
			g_render.BindVBO(GL_ARRAY_BUFFER, m_VBOIDs[eGeoTexCoord0]);
			glTexCoordPointer( 2, GL_FLOAT, 0, NULL);
		}

		return;
	}

	g_render.BindVBO(GL_ARRAY_BUFFER, 0 ); //disable VBOs
	glEnableClientState(GL_VERTEX_ARRAY); 
	glVertexPointer ( 3, GL_FLOAT, 0, m_points );				
	if (m_texCoords[0]) {
		glClientActiveTexture(GL_TEXTURE0);
		glEnableClientState(GL_TEXTURE_COORD_ARRAY);
		glTexCoordPointer( 2, GL_FLOAT, 0, m_texCoords[0] );	
	}
}

CVoid CGeometry::SetGroupRender( CPolyGroup* group )
{
	if (!g_renderShadow && !g_main->m_renderQuery && group->m_hasDiffuse)
	{
		glActiveTexture(GL_TEXTURE0);
		glEnable( GL_TEXTURE_2D );
		glBindTexture(GL_TEXTURE_2D, group->m_diffuseImg->GetId() );
		if( (!g_render.m_useShader || !g_render.UsingShader() || !g_options.m_enableShader) && group->m_diffuseImg->GetFormat() == 4)
		{
			glEnable( GL_ALPHA_TEST );
			glAlphaFunc( GL_GREATER, 0 );
		}
	}
	if (g_renderShadow && (!g_render.m_useShader || !g_render.UsingShader() || !g_options.m_enableShader) && group->m_diffuseImg->GetFormat() == 4)
	{
		glActiveTexture(GL_TEXTURE0);
		glEnable(GL_TEXTURE_2D);
		glBindTexture(GL_TEXTURE_2D, group->m_diffuseImg->GetId());
		if ((!g_render.m_useShader || !g_render.UsingShader() || !g_options.m_enableShader) && group->m_diffuseImg->GetFormat() == 4)
		{
			glEnable(GL_ALPHA_TEST);
			glAlphaFunc(GL_GREATER, 0);
		}
	}

	if( g_render.UsingShader() && g_render.m_useShader && g_options.m_enableShader)
	{
		if( group->m_hasGlossMap )
		{
			glActiveTexture( GL_TEXTURE3 );
			glEnable( GL_TEXTURE_2D );
			glBindTexture( GL_TEXTURE_2D, group->m_glossMapImg->GetId() );
		}

		if( group->m_hasDirtMap )
		{
			glActiveTexture( GL_TEXTURE5 );
			glEnable( GL_TEXTURE_2D );
			glBindTexture( GL_TEXTURE_2D, group->m_dirtMapImg->GetId() );
		}

		if( group->m_hasNormalMap )
		{
			glActiveTexture( GL_TEXTURE1 );
			glEnable( GL_TEXTURE_2D );
			glBindTexture( GL_TEXTURE_2D, group->m_normalMapImg->GetId() );
		}

		if( group->m_hasDiffuse )
		{
			glUniform1i(glGetUniformLocation( g_shaderType , "colorMap"), 0);
			glUniform1i(glGetUniformLocation( g_shaderType , "enableColorMap"), CTrue );
		}
		else
		{
			glUniform1i(glGetUniformLocation( g_shaderType , "enableColorMap"), CFalse );
		}

		if( group->m_hasNormalMap )
		{
			glUniform1i(glGetUniformLocation( g_shaderType , "normalMap"), 1);
			glUniform1i(glGetUniformLocation( g_shaderType , "enableNormalMap"), CTrue );

			if( group->m_normalMapImg->GetFormat() == 4 )
			{
				glUniform1i(glGetUniformLocation( g_shaderType , "enableParallaxMap"), CTrue );
				glUniform1f(glGetUniformLocation( g_shaderType , "parallaxMapBias"), group->m_parallaxMapBias );
				glUniform1f(glGetUniformLocation( g_shaderType , "parallaxMapScale"), group->m_parallaxMapScale );
			}
			else 
				glUniform1i(glGetUniformLocation( g_shaderType , "enableParallaxMap"), CFalse );
		}
		else
			glUniform1i(glGetUniformLocation( g_shaderType , "enableNormalMap"), CFalse );

		if( group->m_hasDirtMap && m_texCoords[1] )
		{
			glUniform1i(glGetUniformLocation( g_shaderType , "dirtMap"), 5);
			glUniform1i(glGetUniformLocation( g_shaderType , "enableDirtMap"), CTrue );
		}
		else
			glUniform1i(glGetUniformLocation( g_shaderType , "enableDirtMap"), CFalse );

		if( group->m_hasDiffuse && group->m_diffuseImg->GetFormat() == 4 )
		{
				glUniform1i(glGetUniformLocation( g_shaderType , "alphaMap"), 0);
				glUniform1i(glGetUniformLocation( g_shaderType , "enableAlphaMap"), CTrue );
		}
		else
		{
			glUniform1i(glGetUniformLocation( g_shaderType , "enableAlphaMap"), CFalse );
		}

		if( group->m_hasGlossMap )
		{
			glUniform1i(glGetUniformLocation( g_shaderType , "glossMap"), 3);
			glUniform1i(glGetUniformLocation( g_shaderType , "enableGlossMap"), CTrue );
		}
		else
			glUniform1i(glGetUniformLocation( g_shaderType , "enableGlossMap"), CFalse );


	}
}

CVoid CGeometry::ResetGroupStates( CPolyGroup* group )
{
	if( g_render.UsingShader() && g_render.m_useShader && g_options.m_enableShader)
	{

		if( group->m_hasDirtMap )
		{
			glActiveTexture(GL_TEXTURE5);
			glBindTexture(GL_TEXTURE_2D, 0);
			glDisable(GL_TEXTURE_2D);
		}

		if( group->m_hasNormalMap )
		{
			glActiveTexture(GL_TEXTURE1);
			glBindTexture(GL_TEXTURE_2D, 0);
			glDisable(GL_TEXTURE_2D);
		}

		if( group->m_hasGlossMap )
		{
			glActiveTexture(GL_TEXTURE3);
			glBindTexture(GL_TEXTURE_2D, 0);
			glDisable(GL_TEXTURE_2D);
		}
	}

	if( group->m_hasDiffuse )
	{
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, 0);
		glDisable(GL_TEXTURE_2D);
		if( (!g_render.m_useShader || !g_render.UsingShader() || !g_options.m_enableShader) && group->m_diffuseImg->GetFormat() == 4)
		{
			glDisable( GL_ALPHA_TEST );
		}
	}


}

CVoid CGeometry::ResetStates()
{
    glClientActiveTexture(GL_TEXTURE0);
    glDisableClientState(GL_TEXTURE_COORD_ARRAY);

	if( g_render.UsingShader() && g_render.m_useShader && g_options.m_enableShader )
	{
		glClientActiveTexture(GL_TEXTURE1);
		glDisableClientState(GL_TEXTURE_COORD_ARRAY);

		glClientActiveTexture(GL_TEXTURE2);
		glDisableClientState(GL_TEXTURE_COORD_ARRAY);

		glClientActiveTexture(GL_TEXTURE3);
		glDisableClientState(GL_TEXTURE_COORD_ARRAY);

	}
	glDisableClientState(GL_NORMAL_ARRAY);
	glDisableClientState( GL_VERTEX_ARRAY );

	g_render.BindVBO(GL_ARRAY_BUFFER, 0 );

}

CVoid CGeometry::SetRender()
{
	if( m_cullFaces )
		glEnable( GL_CULL_FACE );
	else
		glDisable( GL_CULL_FACE );

	if( g_render.UsingShader() && g_render.m_useShader && g_options.m_enableShader)
	{
		if( g_fogProperties.m_enable )
			glUniform1i(glGetUniformLocation( g_shaderType , "enableFog"), CTrue );
		else
			glUniform1i(glGetUniformLocation( g_shaderType , "enableFog"), CFalse );
	} 

	if (g_render.UsingVBOs() && g_options.m_enableVBO )
	{
		glEnableClientState(GL_VERTEX_ARRAY); 
		g_render.BindVBO(GL_ARRAY_BUFFER, m_VBOIDs[eGeoPoints]);
		glVertexPointer( 3, GL_FLOAT, 0, NULL);				

		if (m_normals) {
			glEnableClientState(GL_NORMAL_ARRAY);
			g_render.BindVBO(GL_ARRAY_BUFFER, m_VBOIDs[eGeoNormals]);
			glNormalPointer( GL_FLOAT, 0, NULL);
		}
		if (m_texCoords[0]) {
			glClientActiveTexture(GL_TEXTURE0);
			glEnableClientState(GL_TEXTURE_COORD_ARRAY);
			g_render.BindVBO(GL_ARRAY_BUFFER, m_VBOIDs[eGeoTexCoord0]);
			glTexCoordPointer( 2, GL_FLOAT, 0, NULL);
		}
		if( g_render.UsingShader() && g_render.m_useShader && g_options.m_enableShader)
		{
			if (m_texCoords[1] && m_hasDirtMap ) {
				glClientActiveTexture(GL_TEXTURE3);
				glEnableClientState(GL_TEXTURE_COORD_ARRAY);
				g_render.BindVBO(GL_ARRAY_BUFFER, m_VBOIDs[eGeoTexCoord1]);
				glTexCoordPointer( 2, GL_FLOAT, 0, NULL );
			}
		}

		if( g_render.UsingShader()  && m_hasNormalMap && g_render.m_useShader && g_options.m_enableShader)
		{
			glClientActiveTexture(GL_TEXTURE1);
			glEnableClientState(GL_TEXTURE_COORD_ARRAY);
			g_render.BindVBO(GL_ARRAY_BUFFER, m_VBOIDs[eGeoTangents]);
			glTexCoordPointer(3, GL_FLOAT,0, NULL );
			
			glClientActiveTexture(GL_TEXTURE2);
			glEnableClientState(GL_TEXTURE_COORD_ARRAY);
			g_render.BindVBO(GL_ARRAY_BUFFER, m_VBOIDs[eGeoBiNormals]);
			glTexCoordPointer(3, GL_FLOAT,0, NULL );

		}

		return;
	}
	g_render.BindVBO(GL_ARRAY_BUFFER, 0 ); //disable VBOs
	glEnableClientState(GL_VERTEX_ARRAY); 
	glVertexPointer ( 3, GL_FLOAT, 0, m_points );				
	if (m_normals) {
		glEnableClientState(GL_NORMAL_ARRAY);
		glNormalPointer( GL_FLOAT, 0, m_normals );
	}
	if (m_texCoords[0]) {
		glClientActiveTexture(GL_TEXTURE0);
		glEnableClientState(GL_TEXTURE_COORD_ARRAY);
		glTexCoordPointer( 2, GL_FLOAT, 0, m_texCoords[0] );	
	}
	if( g_render.UsingShader() && m_hasNormalMap && g_render.m_useShader && g_options.m_enableShader)
	{
		glClientActiveTexture(GL_TEXTURE1);
		glEnableClientState(GL_TEXTURE_COORD_ARRAY);
		glTexCoordPointer(3, GL_FLOAT,0, m_tangents );

		glClientActiveTexture(GL_TEXTURE2);
		glEnableClientState(GL_TEXTURE_COORD_ARRAY);
		glTexCoordPointer(3, GL_FLOAT,0, m_bNormals );
	}
	if( g_render.UsingShader() && g_render.m_useShader && g_options.m_enableShader)
	{
		if (m_texCoords[1] && m_hasDirtMap ) {
			glClientActiveTexture(GL_TEXTURE3);
			glEnableClientState(GL_TEXTURE_COORD_ARRAY);
			glTexCoordPointer( 2, GL_FLOAT, 0, m_texCoords[1]);
		}
	}
}

CTriangles::CTriangles()
{
	m_VBOID = 0;
}
CTriangles::~CTriangles()
{
	if (m_VBOID!=0)
	{
		g_render.FreeVBO(m_VBOID);
		m_VBOID=0;
	}
	CDeleteData(m_indexes);
}
CVoid CTriangles::Render()
{
	if ( g_render.UsingVBOs() && g_options.m_enableVBO )
	{
		g_render.BindVBO(GL_ELEMENT_ARRAY_BUFFER, m_VBOID);
		glDrawElements( GL_TRIANGLES, m_count * 3, GL_UNSIGNED_INT, 0);
		g_render.BindVBO(GL_ELEMENT_ARRAY_BUFFER, 0 );
	}
	else
	{
		g_render.BindVBO(GL_ELEMENT_ARRAY_BUFFER, 0 ); // Disable VBOs
		glDrawElements( GL_TRIANGLES, m_count * 3, GL_UNSIGNED_INT, m_indexes);
	}
}

CVoid CTriangles::SetVBOs()
{
	m_VBOID = g_render.GenerateVBO();
	g_render.CopyVBOData(GL_ELEMENT_ARRAY_BUFFER, m_VBOID, m_indexes, m_count * 3 * sizeof(CFloat));
}

CGeometry::~CGeometry()
{
	while(!m_physx_triangles.empty())
	{
		CDelete(m_physx_triangles[0]);
		m_physx_triangles.erase(m_physx_triangles.begin());
	}
	while(!m_physx_points.empty())
	{
		CDelete(m_physx_points[0]);
		m_physx_points.erase(m_physx_points.begin());
	}

	while(!m_groups.empty())
	{
		CDelete(m_groups[0]);
		m_groups.erase(m_groups.begin());
	}
	
	CDeleteData(m_points);
	CDeleteData(m_normals);

	for( CInt i = 0; i < MAX_TEX_COORDS; i++ )
	{
		if( m_texCoords[i] )
			CDeleteData(m_texCoords[i]);
	}

	CDeleteData(m_tangents);
	CDeleteData(m_bNormals);
	CDeleteData(m_skinIndex);
	CDeleteData(m_weights);

	//Delete external image data 
	//CDelete(m_normalMapImg);

	//Clear all the images
	//for( std::vector<CImage*>::iterator it = m_images.begin(); it != m_images.end(); it++ )
	//{
	//	CDelete( *it );
	//}
	//Clear the vector objects
	m_images.clear();

	//CDelete(m_dirtMapImg);
	//CDelete(m_glossMapImg);
	//CDelete(m_heightMapImg);
	//CDelete(m_dudvMapImg);


	if( g_render.UsingVBOs() )
	{
		if (m_VBOIDs[eGeoPoints]!=0)
		{
			g_render.FreeVBO(m_VBOIDs[eGeoPoints]);
			m_VBOIDs[eGeoPoints]=0;
		}
		if( m_VBOIDs[eGeoNormals] != 0 )
		{
			g_render.FreeVBO(m_VBOIDs[eGeoNormals]);
			m_VBOIDs[eGeoNormals]=0;
		}

		for( int i = 0; i < MAX_TEX_COORDS; i++ )
		{
			if( m_VBOIDs[eGeoTexCoord0 + i ] != 0 )
			{
				g_render.FreeVBO(m_VBOIDs[eGeoTexCoord0 + i ]);
				m_VBOIDs[eGeoTexCoord0 + i ]= 0;
			}
		}

		if( m_VBOIDs[eGeoTangents] != 0 )
		{
			g_render.FreeVBO(m_VBOIDs[eGeoTangents]);
			m_VBOIDs[eGeoTangents]=0;
		}
		if( m_VBOIDs[eGeoBiNormals] != 0 )
		{
			g_render.FreeVBO(m_VBOIDs[eGeoBiNormals]);
			m_VBOIDs[eGeoBiNormals]=0;
		}
	}
}