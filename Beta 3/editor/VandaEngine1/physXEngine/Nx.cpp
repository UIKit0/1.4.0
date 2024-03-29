//Copyright (C) 2015 Zehne Ziba Co., Ltd.
//This file is licensed and distributed under MIT license

#include "stdafx.h"
#include "Nx.h"
#include "../VandaEngine1dlg.h"
#include "../graphicsEngine/scene.h"
NxPhysicsSDK*     gPhysicsSDK;
NxScene*          gPhysXscene;
std::map< CUInt, NxMaterial* > g_physXMaterial;
CNovodex::CNovodex()
{
	m_hasScene = CFalse; //imported scene from 3ds Max or Maya. by default I don't import a scene
}

CNovodex::~CNovodex()
{
	g_physXMaterial.clear();
	m_nxActorNames.clear();
	m_nxActorTypes.clear();
}

CBool CNovodex::initNx(CFloat XCharacterPos, CFloat YCharacterPos, CFloat ZCharacterPos, CFloat gravity, CFloat crtlRadius, CFloat crtlHeight, CFloat crtSlopeLimit, CFloat crtSkinWidth, CFloat crtStepOffset, CBool createScene)
{
	if( createScene )
	{
		gPhysXscene = NULL;
		gPhysicsSDK = NULL;
	}

	
	//Default gravity
	gDefaultGravity.x = g_physXProperties.m_fGravityX;
	gDefaultGravity.y = g_physXProperties.m_fGravityY;
	gDefaultGravity.z = g_physXProperties.m_fGravityZ;
	wireFrame = false;
	zero = NxVec3(0,0,0);

	gJump = false;
	gControllers = NULL;

	gTimestepMultiplier = 1;
	bPushCharacter = false;
	gCharacterVec = NxVec3(1,0,0);
	gCharacterSpeed = g_physXProperties.m_fCharacterSpeed;
	bFixedStep = true; //editor uses fixed steps in order to prevent jumpings while changing drastic valuse of elapsed time.
	gDesiredDistance = 5.0f;
	G = -98.1f;

	gTriggerReport.hitName = NULL;
	gDefaultGravity.y = gravity;

	if( createScene )
	{
		gAllocator = NULL;
		gAllocator = new UserAllocator;
		NxPhysicsSDKDesc desc;
		gPhysicsSDK = NxCreatePhysicsSDK( NX_PHYSICS_SDK_VERSION, gAllocator, NULL, desc );
		if( !gPhysicsSDK )
		{
			MessageBox( NULL, "Couldn't initialize physX", "Vanda Engine Error", MB_OK | MB_ICONINFORMATION );
			return false;
		}
		gPhysicsSDK->setParameter( NX_SKIN_WIDTH, g_physXProperties.m_fDefaultSkinWidth );

		NxSceneDesc sceneDesc;
		sceneDesc.gravity = gDefaultGravity;
		sceneDesc.simType = NX_SIMULATION_HW;

		gPhysXscene = gPhysicsSDK->createScene( sceneDesc );
		if( !gPhysXscene )
		{
			sceneDesc.simType = NX_SIMULATION_SW;
			gPhysXscene = gPhysicsSDK->createScene(sceneDesc);
		}
	}
	gPhysXscene->setGroupCollisionFlag( GROUP_COLLIDABLE_NON_PUSHABLE, GROUP_COLLIDABLE_PUSHABLE, CTrue );
	gPhysXscene->setGroupCollisionFlag( GROUP_COLLIDABLE_PUSHABLE, GROUP_COLLIDABLE_PUSHABLE, CTrue );
	gPhysXscene->setUserTriggerReport(&gTriggerReport);

	debugMode = true;

	if( debugMode )
	{
        // Set the debug visualization parameters
        gPhysicsSDK->setParameter(NX_VISUALIZATION_SCALE, 1.0);
        gPhysicsSDK->setParameter(NX_VISUALIZE_COLLISION_SHAPES, 1);
        gPhysicsSDK->setParameter(NX_VISUALIZE_ACTOR_AXES, 1);
		gPhysicsSDK->setParameter(NX_VISUALIZE_CONTACT_POINT, 1);
		gPhysicsSDK->setParameter(NX_VISUALIZE_CONTACT_NORMAL, 1);
		gPhysicsSDK->setParameter(NX_VISUALIZE_CONTACT_FORCE, 1);
		gPhysicsSDK->setParameter(NX_VISUALIZE_COLLISION_STATIC, 1);
		gPhysicsSDK->setParameter(NX_VISUALIZE_COLLISION_DYNAMIC, 1);
	}

	// Create the default material
	if( createScene )
	{
		NxMaterial* defaultMaterial = gPhysXscene->getMaterialFromIndex(0); 
		defaultMaterial->setRestitution(g_physXProperties.m_fDefaultRestitution);
		defaultMaterial->setStaticFriction(g_physXProperties.m_fDefaultStaticFriction);
		defaultMaterial->setDynamicFriction(g_physXProperties.m_fDefaultDynamicFriction);
	}


	InitCharacterControllers(XCharacterPos, YCharacterPos, ZCharacterPos, crtlRadius, crtlHeight, crtSkinWidth, crtSlopeLimit, crtStepOffset);
	CFloat TimeStep = 1.0f / 60.0f;
	if (bFixedStep)	
		gPhysXscene->setTiming(TimeStep, 1, NX_TIMESTEP_FIXED);
	else
		gPhysXscene->setTiming(TimeStep, 1, NX_TIMESTEP_VARIABLE);


	gCharacterPos.x = XCharacterPos;
	gCharacterPos.y = YCharacterPos;
	gCharacterPos.z = ZCharacterPos;

	gDefaultGravity = NxVec3(0.0f); //By default, disable gravity that affects character controller
	CreateGroundPlane(g_physXProperties.m_fGroundHeight);
	
	return true;
}

CVoid CNovodex::runPhysics( NxVec3 forceDirection, CFloat forceSpeed, CInt moveDirection, CFloat elapsedTime )
{
	if( gPhysXscene )
	{
	  // Run collision and dynamics for delta time since the last frame
		gPhysXscene->simulate(1.0f/60.0f/*elapsedTime*/);
		gPhysXscene->flushStream();
		gPhysXscene->fetchResults(NX_ALL_FINISHED, true);

		// Update the box character's position according to by testing its
		// axis-aligned bounding box against a copy of the current scene.
		UpdateCharacter(forceDirection, forceSpeed, elapsedTime, moveDirection);

		// Update the box character's position according to results
		// gathered from the updated scene
		PostUpdateCharacter();
	}
}

CVoid CNovodex::releaseNx()
{

	gTriggerReport.hitName = NULL;
	ReleaseCharacterControllers();

	if( gPhysXscene)
	{
		gPhysicsSDK->releaseScene( *gPhysXscene );
		gPhysXscene = NULL;
	}
	if( gPhysicsSDK )
	{
		gPhysicsSDK->release();
		gPhysicsSDK = NULL;
	}
	if( gAllocator )
	{
        CDelete(gAllocator);
		gAllocator = NULL;
	}
}

CVoid CNovodex::UpdateCharacter( NxVec3 forceDirection, CFloat forceSpeed, CFloat deltaTime, CInt moveDirection )
{
	deltaTime *= (CFloat)gTimestepMultiplier;

	NxVec3 disp = gDefaultGravity;
	CFloat vdisp = 0.0f;
	gCharacterPos = GetFilteredCharacterPos();
    
	NxVec3 horizontalDisp;
	CFloat verticalDisp = 0.0f;
	horizontalDisp.x = horizontalDisp.y = horizontalDisp.z = 0;

	CBool pushCharacter = false;
	switch( moveDirection )
	{
	case IDLE:
		break;
	case MOVE_FRONT:
	pushCharacter = true;
	horizontalDisp = -gDir;
	break;
	case MOVE_BACK:
	pushCharacter = true;
	horizontalDisp = gDir;
	break;
	case MOVE_LEFT:
	pushCharacter = true;
	horizontalDisp = gRightDir;
	break;
	case MOVE_RIGHT:
	pushCharacter = true;
	horizontalDisp = -gRightDir;
	break;
	case MOVE_FRONT_RIGHT:
	pushCharacter = true;
	horizontalDisp = -gRightDir - gDir ;
	break;
	case MOVE_FRONT_LEFT:
	pushCharacter = true;
	horizontalDisp = gRightDir - gDir ;
	break;
	case MOVE_BACK_RIGHT:
	pushCharacter = true;
	horizontalDisp = gDir - gRightDir ;
	break;
	case MOVE_BACK_LEFT:
	pushCharacter = true;
	horizontalDisp = gDir + gRightDir ;
	break;
	case MOVE_UP:
	pushCharacter = true;
	verticalDisp = 1;
	break;
	case MOVE_DOWN:
	pushCharacter = true;
	verticalDisp = -1;
	break;

	default:
		break;
	}
	if( pushCharacter || bPushCharacter ) //bPushCharater is specifed inside directionalForceFunction
	{
		//horizontalDisp.y = 0.0f;
		horizontalDisp.normalize();
		forceDirection.normalize();

		disp += horizontalDisp * gCharacterSpeed;
		disp += forceDirection * forceSpeed;

		vdisp += verticalDisp * gCharacterSpeed * deltaTime;
	}

	NxU32 collisionFlags = MoveCharacter(disp, deltaTime, COLLIDABLE_MASK, GetHeight(deltaTime) + vdisp );
	if (collisionFlags & NXCC_COLLISION_DOWN)  StopJump();
}


NxF32 CNovodex::cameraHit()
{
	if( gPhysXscene )
	{
		NxShape* characterShape = GetCharacterActor()->getShapes()[0];

		NxExtendedVec3 cpos = GetFilteredCharacterPos();	// First character = player

		NxRay worldRay;
		worldRay.orig.x	= (NxReal)cpos.x;
		worldRay.orig.y	= (NxReal)cpos.y;
		worldRay.orig.z	= (NxReal)cpos.z;
		worldRay.dir	=  gDir;
		NxRaycastHit hit;
		characterShape->setFlag(NX_SF_DISABLE_RAYCASTING, true);
		NxShape* shape = gPhysXscene->raycastClosestShape(worldRay, NX_ALL_SHAPES, hit);
		characterShape->setFlag(NX_SF_DISABLE_RAYCASTING, false);

		gWorldRay = worldRay;
		if( shape )
		{
			if ( shape->getGroup() == GROUP_TRIGGER || shape->getGroup() == GROUP_COLLIDABLE_NON_PUSHABLE_NO_CAMERA_HIT )
				return gDesiredDistance;
			else
			{
				return hit.distance;
			}
		}
		else 
			return NX_MAX_F32;
	}
	return 0.0f;
	//return shape ? hit.distance : NX_MAX_F32;
}

CVoid CNovodex::GetCameraAndCharacterPositions( const CFloat pitch, const CFloat yaw, NxVec3& cameraPos, NxVec3& characterPos, NxVec3& cameraDirection )
{
    // Setup the view matrix
	NxMat33 RotX;	RotX.rotX(pitch);
	NxMat33 RotY;	RotY.rotY(yaw+NxPiF32);
	NxMat33 ViewMat = RotY*RotX;
	gDir = -ViewMat.getColumn(2); 
	gRightDir = ViewMat.getColumn(0);
    
	cameraDirection = gDir;
    dist = cameraHit();
	if(dist>gDesiredDistance)	dist = gDesiredDistance;

	NxExtendedVec3 cpos = GetFilteredCharacterPos();

	NxVec3 cameraGoal((NxReal)cpos.x, (NxReal)cpos.y, (NxReal)cpos.z);

	characterPos = cameraGoal;

	cameraGoal += gDir*dist;

	cameraPos = cameraGoal;
}

NxReal CNovodex::updateTime()
{
    NxReal deltaTime;
    static __int64 gTime,gLastTime;
    __int64 freq;
    QueryPerformanceCounter((LARGE_INTEGER *)&gTime);  // Get current count
    QueryPerformanceFrequency((LARGE_INTEGER *)&freq); // Get processor freq
    deltaTime = (CFloat)(gTime - gLastTime)/(CFloat)freq;
    gLastTime = gTime;

	return deltaTime;
}

NxF32 CNovodex::GetHeight(NxF32 elapsedTime)
{
	if (!gJump)  return 0.0f;
	jumpTime += elapsedTime;
	NxF32 h = G*jumpTime*jumpTime + gV0*jumpTime;
	return (h - gDefaultGravity.y)*elapsedTime;
}

CVoid CNovodex::StartJump(NxF32 v0)
{
	if (gJump)  return;
	jumpTime = 0.0f;
	gV0	= v0;
	gJump = true;
}

CVoid CNovodex::StopJump()
{
	if (!gJump)  return;
	gJump = false;
}

CVoid CNovodex::SetActorCollisionGroup(NxActor* actor, NxCollisionGroup group)
{
    NxShape*const* shapes = actor->getShapes();
    NxU32 nShapes = actor->getNbShapes();
    while (nShapes--)
    {
        shapes[nShapes]->setGroup(group);
	}
}

CVoid CNovodex::PostUpdateCharacter()
{
	NxReal maxTimestep;
	NxTimeStepMethod method;
	NxU32 maxIter;
	NxU32 numSubSteps;

	gPhysXscene->getTiming(maxTimestep, maxIter, method, &numSubSteps);
	if (numSubSteps)  UpdateControllers();
}


NxActor* CNovodex::CreateCapsule(const NxVec3& pos, const NxReal height, const NxReal radius, const NxReal density, NxMat33 rotation, const CChar* name, CBool isTrigger, CBool isKinematic )
{
	// Add a single-shape actor to the scene
	NxActorDesc actorDesc;
	NxBodyDesc bodyDesc;
	bodyDesc.setToDefault();
	if( isKinematic )
		bodyDesc.flags = NX_BF_KINEMATIC;
	// The actor has one shape, a capsule
	NxCapsuleShapeDesc capsuleDesc;
	if( isTrigger && density == 0 && !isKinematic)
		capsuleDesc.shapeFlags |= NX_TRIGGER_ENABLE;

	capsuleDesc.height = height;
	capsuleDesc.radius = radius;
	if( !isKinematic )
	{
		capsuleDesc.localPose.t = pos/*NxVec3(0.0f, radius + 0.5f * height, 0.0f )*/;
		capsuleDesc.localPose.M = rotation;
	}
	actorDesc.shapes.pushBack(&capsuleDesc);
	if( isKinematic )
	{
		actorDesc.body = &bodyDesc;
		actorDesc.density = 10.0f; //none-zero density
	}
	else	if (density)
	{
		actorDesc.body = &bodyDesc;
		actorDesc.density = density;
	}
	else
	{
		actorDesc.body = NULL;
	}
	if( isKinematic )
	{
		actorDesc.globalPose.t = pos;
		actorDesc.globalPose.M = rotation;
	}
	actorDesc.name = name;
	NxActor* actor = gPhysXscene->createActor(actorDesc);	
	if( actor && actor->isDynamic() )
	{
		if( !isKinematic )
			SetActorCollisionGroup(actor, GROUP_COLLIDABLE_PUSHABLE );
		else
			SetActorCollisionGroup(actor, GROUP_COLLIDABLE_NON_PUSHABLE );
	}	
	else if( isTrigger )
	{
		SetActorCollisionGroup(actor, GROUP_TRIGGER );
	}

	return actor;
}

NxActor* CNovodex::CreateSphere(const NxVec3& pos, const NxReal radius, const NxReal density, const CChar* name, CBool isTrigger, CBool isKinematic)
{
	// Add a single-shape actor to the scene
	NxActorDesc actorDesc;
	NxBodyDesc bodyDesc;
	if( isKinematic )
		bodyDesc.flags = NX_BF_KINEMATIC;
	// The actor has one shape, a sphere
	NxSphereShapeDesc sphereDesc;
	if( isTrigger && density == 0 && !isKinematic)
		sphereDesc.shapeFlags |= NX_TRIGGER_ENABLE;

	sphereDesc.radius = radius;
	if( !isKinematic )
		sphereDesc.localPose.t = pos;
	actorDesc.shapes.pushBack(&sphereDesc);

	if( isKinematic )
	{
		actorDesc.body = &bodyDesc;
		actorDesc.density = 10.0f; //none-zero density
	}
	else if (density )
	{
		actorDesc.body = &bodyDesc;
		actorDesc.density = density;
	}
	else
	{
		actorDesc.body = NULL;
	}
	if( isKinematic )
		actorDesc.globalPose.t = pos;
	//actorDesc.globalPose.t = pos;
	actorDesc.name = name;
	NxActor* actor = gPhysXscene->createActor(actorDesc);
	if( actor && actor->isDynamic() )
	{
		if( !isKinematic )
			SetActorCollisionGroup(actor, GROUP_COLLIDABLE_PUSHABLE );
		else
			SetActorCollisionGroup(actor, GROUP_COLLIDABLE_NON_PUSHABLE );
	}
	else if( isTrigger )
	{
		SetActorCollisionGroup(actor, GROUP_TRIGGER );
	}

	return actor;	
}

NxActor* CNovodex::CreateWallAdv(const NxVec3& pos, const NxReal length, const NxReal height, const NxReal Diameter, const NxReal xDegree, const NxReal yDegree, const NxReal zDegree, CBool noCameraHit ){

    NxActor* actor;

	NxVec3 rot0( 0, 0, 0 );
	NxVec3 rot1( 0, 0, 0 );
	NxVec3 rot2( 0, 0, 0 );
	NxMat33 rot( rot0, rot1, rot2 );
	actor = CreateBox(pos, NxVec3( Diameter,height,length ), 10, rot, NULL, CFalse, CFalse );
	if(! noCameraHit )
        SetActorCollisionGroup(actor, GROUP_COLLIDABLE_NON_PUSHABLE);
	else
        SetActorCollisionGroup(actor, GROUP_COLLIDABLE_NON_PUSHABLE_NO_CAMERA_HIT);

	NxMat33 tempMatrix;
	NxMat33 globalOrientation;
	globalOrientation = actor->getGlobalOrientation();
	NxMat33 Mx = globalOrientation;
	Mx.rotX( NxMath::degToRad(xDegree) );
	tempMatrix = Mx;
	NxMat33 My = globalOrientation;
	My.rotY( NxMath::degToRad(yDegree) );
	tempMatrix = My * tempMatrix;
	NxMat33 Mz = globalOrientation;
	Mz.rotZ( NxMath::degToRad(zDegree) );
	tempMatrix = Mz * tempMatrix;
	actor->setGlobalOrientation( tempMatrix );

	return actor;
}

   
NxActor* CNovodex::CreateBox(const NxVec3& pos, const NxVec3& boxDim, const NxReal density, NxMat33 rotation, const CChar* name, CBool isTrigger, CBool isKinematic )
{
	// Add a single-shape actor to the scene
	NxActorDesc actorDesc;
	NxBodyDesc bodyDesc;
	if( isKinematic )
		bodyDesc.flags = NX_BF_KINEMATIC;
	// The actor has one shape, a box
	NxBoxShapeDesc boxDesc;
	boxDesc.dimensions.set(boxDim.x,boxDim.y,boxDim.z);
	if( isTrigger && density == 0 && !isKinematic)
		boxDesc.shapeFlags |= NX_TRIGGER_ENABLE;

	if( !isKinematic )
	{
		boxDesc.localPose.t = pos;
		boxDesc.localPose.M = rotation;
	}
	actorDesc.shapes.pushBack(&boxDesc);

	if( isKinematic )
	{
		actorDesc.body = &bodyDesc;
		actorDesc.density = 10.0f; //none-zero density
	}
	else if (density )
	{
		actorDesc.body = &bodyDesc;
		actorDesc.density = density;
	}
	else
	{
		actorDesc.body = NULL;
	}
	if( isKinematic )
	{
		actorDesc.globalPose.t = pos;
		actorDesc.globalPose.M = rotation;
	}
	
	actorDesc.name = name;
	NxActor* actor = gPhysXscene->createActor(actorDesc);
	if( actor && actor->isDynamic() )
	{
		if( !isKinematic )
			SetActorCollisionGroup(actor, GROUP_COLLIDABLE_PUSHABLE );
		else
			SetActorCollisionGroup(actor, GROUP_COLLIDABLE_NON_PUSHABLE );
	}
	else if( isTrigger )
	{
		SetActorCollisionGroup(actor, GROUP_TRIGGER );
	}
	return actor;
}

NxActor* CNovodex::CreateTriggerBox(const NxVec3& pos, const NxVec3& boxDim, NxMat33 rotation, const CChar* name, CBool isKinematic )
{
	// Add a single-shape actor to the scene
	NxActorDesc actorDesc;

	NxBoxShapeDesc boxDesc;
	boxDesc.dimensions = boxDim;
	boxDesc.shapeFlags |= NX_TRIGGER_ENABLE;

	actorDesc.shapes.pushBack(&boxDesc);
	actorDesc.globalPose.t = pos;
	actorDesc.globalPose.M = rotation;
	actorDesc.name = name;
	NxActor* actor = gPhysXscene->createActor(actorDesc); 
	SetActorCollisionGroup( actor , GROUP_TRIGGER );
	return actor;
}

NxActor* CNovodex::CreateTriggerWall(const NxVec3& pos, const NxVec3& boxDim, const NxReal rDegree, const char* triggerName)
{
	NxActor* actor;
	NxActorDesc actorDesc;
 	NxMat33 rTemp;

	NxBoxShapeDesc boxDesc;
	boxDesc.dimensions = boxDim;
	boxDesc.shapeFlags |= NX_TRIGGER_ENABLE;

	actorDesc.shapes.pushBack(&boxDesc);
	actorDesc.globalPose.t = pos + NxVec3(0, boxDim.y, 0);
	 
	actor = gPhysXscene->createActor(actorDesc);	

	actor->userData = (CVoid*)-1;
	actor->setName(triggerName);

	rTemp = actor->getGlobalOrientation();
	rTemp.rotY(NxMath::degToRad(rDegree));
	actor->setGlobalOrientation(rTemp);
	SetActorCollisionGroup( actor, GROUP_TRIGGER );

	return actor;
}
NxActor* CNovodex::CreateConvexMesh( CInt vertexCount, CFloat* meshVertices, const NxVec3& pos,  NxMat33 rotation, const NxReal density, const CChar* name, CBool isTrigger, CBool isKinematic )
{
	NxActorDesc actorDesc;
	NxBodyDesc bodyDesc;
	if( isKinematic )
		bodyDesc.flags = NX_BF_KINEMATIC;

	// Create descriptor for triangle mesh
	NxConvexMeshDesc convexDesc;

	//NxMaterialDesc materialDesc;
	//materialDesc.restitution = 0.0f;
	//NxMaterial *newMaterial = gPhysXscene->createMaterial(materialDesc);

	convexDesc.numVertices			= vertexCount;
	convexDesc.pointStrideBytes		=  3 * sizeof(CFloat);
	convexDesc.points				= meshVertices;
	convexDesc.flags				= NX_CF_COMPUTE_CONVEX;

	NxConvexMeshDesc convexDesc2;

	convexDesc2.numVertices			= vertexCount;
	convexDesc2.pointStrideBytes		=  3 * sizeof(CFloat);
	convexDesc2.points				= meshVertices;
	convexDesc2.flags				= NX_CF_COMPUTE_CONVEX | NX_CF_INFLATE_CONVEX;


	// The actor has one shape, a convex mesh
	NxConvexShapeDesc convexShapeDesc;
	//convexShapeDesc.materialIndex = newMaterial->getMaterialIndex();
	NxConvexShapeDesc convexShapeDesc2;
	//convexShapeDesc2.materialIndex = newMaterial->getMaterialIndex();

	if( isTrigger && density == 0 && !isKinematic)
		convexShapeDesc.shapeFlags |= NX_TRIGGER_ENABLE;
	if( isTrigger && density == 0 && !isKinematic)
		convexShapeDesc2.shapeFlags |= NX_TRIGGER_ENABLE;

	//	convexShapeDesc.meshData = gPhysicsSDK->createConvexMesh(convexDesc);
	NxInitCooking();
	//UserStream upstream = UserStream("c:\\tmp.bin", false);
	//UserStream downstream = UserStream("c:\\tmp.bin", true);
	MemoryWriteBuffer buf;

	bool status = NxCookConvexMesh(convexDesc, buf/*upstream*/);
	if( status )
	{
		convexShapeDesc.meshData = gPhysicsSDK->createConvexMesh(MemoryReadBuffer(buf.data)/*downstream*/);
		NxCloseCooking();

		if( !isKinematic )
		{
			convexShapeDesc.localPose.t = pos;
			convexShapeDesc.localPose.M = rotation;
		}
		if( !isKinematic && density > 0.0f )
			convexShapeDesc.mass = density;
		actorDesc.shapes.pushBack(&convexShapeDesc);
	}
	else
	{
		convexShapeDesc.meshData = NULL;
		NxCloseCooking();

		NxInitCooking();

		status = NxCookConvexMesh(convexDesc2, buf/*upstream*/);
		if( status )
		{
			convexShapeDesc2.meshData = gPhysicsSDK->createConvexMesh(MemoryReadBuffer(buf.data)/*downstream*/);

			if( !isKinematic )
			{
				convexShapeDesc2.localPose.t = pos;
				convexShapeDesc2.localPose.M = rotation;
			}
			if( !isKinematic && density > 0.0f )
				convexShapeDesc2.mass = density;
			actorDesc.shapes.pushBack(&convexShapeDesc2);
			NxCloseCooking();
		}
		else
		{
			NxCloseCooking();
			convexShapeDesc2.meshData = NULL;
			return NULL;
		}
	}

	if( isKinematic )
	{
		actorDesc.body = &bodyDesc;
		actorDesc.density = 10.0f;
	}
	else if (density)
	{
		actorDesc.body = &bodyDesc;
		actorDesc.density = density;
	}
	else
	{
		actorDesc.body = NULL;
	}
	if( isKinematic )
	{
		actorDesc.globalPose.t = pos;
		actorDesc.globalPose.M = rotation;
	}
	actorDesc.name = name;
	
	NxActor *actor = gPhysXscene->createActor(actorDesc);
	actor->setMass(density);

	if( actor && actor->isDynamic() )
	{
		if( !isKinematic )
			SetActorCollisionGroup(actor, GROUP_COLLIDABLE_PUSHABLE );
		else
			SetActorCollisionGroup(actor, GROUP_COLLIDABLE_NON_PUSHABLE );
	}
	else if( isTrigger )
	{
		SetActorCollisionGroup(actor, GROUP_TRIGGER );
	}

	return actor;
}

NxActor* CNovodex::CreateTriangleMesh( CInt vertexCount, CInt faceCount, CFloat* meshVertices, CInt* meshFaces, CBool isTrigger, const CChar* name )
{
	// Create descriptor for triangle mesh
	NxTriangleMeshDesc triangleMeshDesc;

	triangleMeshDesc.numVertices			= vertexCount;
	triangleMeshDesc.pointStrideBytes		= 3 * sizeof(CFloat);
	triangleMeshDesc.points				= meshVertices;
	triangleMeshDesc.numTriangles			= faceCount;
	triangleMeshDesc.flags					= 0;
	triangleMeshDesc.triangles				= meshFaces;
	triangleMeshDesc.triangleStrideBytes	= 3 * sizeof(CInt);
	
	//Alternative:	see NxMeshFlags
	//triangleMeshDesc->flags				= NX_MF_16_BIT_INDICES
	//triangleMeshDesc->triangles			= indices16;
	//triangleMeshDesc->triangleStrideBytes	= 3 * sizeof(NxU16);

	// The actor has one shape, a triangle mesh
	NxInitCooking();
	MemoryWriteBuffer buf;

	CBool status = NxCookTriangleMesh(triangleMeshDesc, buf);
	NxTriangleMesh* pMesh;
	if (status)
	{
		pMesh = gPhysicsSDK->createTriangleMesh(MemoryReadBuffer(buf.data));
	}
	else
	{
		assert(false);
		pMesh = NULL;
	}
	NxCloseCooking();
	// Create TriangleMesh above code segment.

	NxTriangleMeshShapeDesc tmsd;
	if( isTrigger )
		tmsd.shapeFlags |= NX_TRIGGER_ENABLE;

	tmsd.meshData		= pMesh;
	tmsd.userData		= &triangleMeshDesc;
	tmsd.localPose.t	= NxVec3(0.f, 0.0f, 0.f);
	tmsd.meshPagingMode = NX_MESH_PAGING_AUTO;
	
	NxActorDesc actorDesc;
	NxBodyDesc  bodyDesc;
	
	assert(tmsd.isValid());
	actorDesc.shapes.pushBack(&tmsd);
	//Dynamic triangle mesh don't be supported anymore. So body = NULL
	actorDesc.body			= NULL;		
	actorDesc.globalPose.t	= NxVec3(0.0f, 0.0f, 0.0f);
	actorDesc.name = name;

	if (pMesh)
	{
		// Save mesh in userData for drawing
		pMesh->saveToDesc(triangleMeshDesc);
		//
		assert(actorDesc.isValid());
		NxActor *actor = gPhysXscene->createActor(actorDesc);
		assert(actor);
		if( isTrigger )
		{
			SetActorCollisionGroup(actor, GROUP_TRIGGER );
		}

		return actor;
	}

	return NULL;
}

NxActor* CNovodex::CreateGroundPlane(CFloat groundHeight)
{
    // Create a plane with default descriptor
    NxPlaneShapeDesc planeDesc;
    NxActorDesc actorDesc;
	actorDesc.shapes.pushBack(&planeDesc);
	m_planeGroundActor = gPhysXscene->createActor(actorDesc);
	m_planeGroundActor->setName("physx_ground_plane");
	return m_planeGroundActor;
}


CVoid CNovodex::ReleaseGroundPlane()
{
	gPhysXscene->releaseActor(*m_boxGroundActor);
	gPhysXscene->releaseActor(*m_planeGroundActor);
}

CVoid CNovodex::rotateX( NxActor* actor,NxReal angle  )
{
	NxMat33 M = actor->getGlobalOrientation();
	M.rotX( NxMath::degToRad(angle) );
	actor->setGlobalOrientation( M );
}

CVoid CNovodex::rotateY( NxActor* actor,NxReal angle  )
{
	NxMat33 M = actor->getGlobalOrientation();
	M.rotY( NxMath::degToRad(angle) );
	actor->setGlobalOrientation( M );
}

CVoid CNovodex::rotateZ( NxActor* actor,NxReal angle  )
{
	NxMat33 M = actor->getGlobalOrientation();
	M.rotZ( angle );
	actor->setGlobalOrientation( M );
}

CVoid CNovodex::rotate( NxActor* actor, NxReal xRot, NxReal yRot, NxReal zRot )
{
	NxMat33 tempMatrix;
	NxMat33 globalOrientation;
	globalOrientation = actor->getGlobalOrientation();
	NxMat33 Mx = globalOrientation;
	Mx.rotX( NxMath::degToRad(xRot) );
	tempMatrix = Mx;
	NxMat33 My = globalOrientation;
	My.rotY( NxMath::degToRad(yRot) );
	tempMatrix = My * tempMatrix;
	NxMat33 Mz = globalOrientation;
	Mz.rotZ( NxMath::degToRad(zRot) );
	tempMatrix = Mz * tempMatrix;
	actor->setGlobalOrientation( tempMatrix );
}



CVoid CNovodex::debugRenderer()
{
	if( gPhysXscene )
	{
		//glUseProgram(0);
		gDebugRenderer.renderData(*gPhysXscene->getDebugRenderable());
	}
}

CVoid CNovodex::processInputs()
{
}

NxVec3 CNovodex::ApplyForceToActor(NxActor* actor, const NxVec3& forceDir, const NxReal forceStrength, CBool forceMode)
{
	NxVec3 forceVec = forceStrength*forceDir;

	if (forceMode)
		actor->addForce(forceVec);
	else 
		actor->addTorque(forceVec);

	return forceVec;
}

NxVec3 CNovodex::ApplyForceToActorAtShape(NxActor* actor, NxShape* shape, const NxVec3& forceDir, const NxReal forceStrength, CBool forceMode, CBool shapeSelectMode)
{
	NxVec3 forceVec = forceStrength*forceDir;

	if (forceMode) 
	{
		if (shapeSelectMode) 
		{
			actor->addForceAtLocalPos(forceVec, shape->getLocalPosition());
		} 
		else 
		{
			actor->addForce(forceVec);
		}
	} 
	else 
	{ 
		actor->addTorque(forceVec);
	}

	return forceVec;
}

NxVec3 ApplyVelocityToActor(NxActor* actor, const NxVec3& velDir, const NxReal velStrength, CBool velMode)
{
	NxVec3 velVec = velStrength*velDir;

	if (velMode) 
	{
		actor->moveGlobalPosition(actor->getGlobalPosition() + 0.0001f * velStrength * velDir);
	} 
	else 
	{
		NxMat33 orient = actor->getGlobalOrientation();
        NxMat33 m;

		m.id();

		if (velDir == NxVec3(1,0,0))
           m.rotX(0.01f);
		else if (velDir == NxVec3(-1,0,0))
           m.rotX(-0.01f);
		else if (velDir == NxVec3(0,1,0))
           m.rotY(0.01f);
		else if (velDir == NxVec3(0,-1,0))
           m.rotY(-0.01f);
		else if (velDir == NxVec3(0,0,1))
           m.rotZ(0.01f);
		else if (velDir == NxVec3(0,0,-1))
           m.rotZ(-0.01f);

		orient = m * orient;

		actor->moveGlobalOrientation(orient);
	}
	return velVec;
}

CVoid CNovodex::InitCharacterControllers(CFloat XPos, CFloat YPos, CFloat ZPos, CFloat radius, CFloat height, CFloat skinWidth, CFloat slopeLimit, CFloat stepOffset)
{
	gCM = NULL;
	gControllers = NULL;
	gCM = NxCreateControllerManager( gAllocator );
	NxCapsuleControllerDesc desc;
	desc.position.x		= XPos;
	desc.position.y		= YPos;
	desc.position.z		= ZPos;
	desc.radius			= radius;
	desc.height			= height;
	desc.upDirection	= NX_Y;
	desc.slopeLimit = cosf(NxMath::degToRad(slopeLimit));
	desc.skinWidth = skinWidth;
	desc.stepOffset = stepOffset;
	desc.climbingMode = CLIMB_CONSTRAINED;
	desc.interactionFlag	= NXIF_INTERACTION_INCLUDE;

	desc.callback		= &gControllerHitReport;
	
	gControllers = gCM->createController(gPhysXscene, desc);
}

CVoid CNovodex::ReleaseCharacterControllers()
{
	if( gCM )
		gCM->releaseController( *gControllers );
	NxReleaseControllerManager(gCM);

}

NxU32 CNovodex::MoveCharacter( const NxVec3& dispVector, NxF32 elapsedTime, NxU32 collisionGroups, NxF32 heightDelta)
{
	gControllerHitReport.SetgNbPts(0);
    NxF32 sharpness = 1.0f;
    NxU32 collisionFlags;
    NxVec3 d = dispVector * elapsedTime;

    if (heightDelta != 0.0f) 
	{
		d.y+=heightDelta;
	}
    gControllers->move(d, collisionGroups, 0.001f, collisionFlags, sharpness);

    return collisionFlags;
}

const NxExtendedVec3 CNovodex::GetFilteredCharacterPos()
{
	return gControllers->getFilteredPosition();
}

CVoid  CNovodex::SetCharacterPos( NxExtendedVec3 pos)
{
	gControllers->setPosition( pos );
}

const NxExtendedVec3 CNovodex::GetRawCharacterPos()
{
	return gControllers->getPosition();
}

NxActor* CNovodex::GetCharacterActor()
{
	return gControllers->getActor();
}

CBool CNovodex::ResetCharacterPos()
{
	NxExtendedVec3 sp;
	sp.x = 0;
	sp.y = 0;
	sp.z = 0;
    return gControllers->setPosition(sp);
}

CBool CNovodex::ResetCharacterPos( NxExtendedVec3 pos )
{
    return gControllers->setPosition(pos);
}

CVoid CNovodex::UpdateControllers()
{
    gCM->updateControllers();
}

CVoid CNovodex::SetupGLMatrix(const NxVec3& pos, const NxMat33& orient)
{
	CFloat glmat[16];	//4x4 column major matrix for OpenGL.
	orient.getColumnMajorStride4(&(glmat[0]));
	pos.get(&(glmat[12]));

	//clear the elements we don't need:
	glmat[3] = glmat[7] = glmat[11] = 0.0f;
	glmat[15] = 1.0f;

	glMultMatrixf(&(glmat[0]));
}
char* CNovodex::CheckHit()
{
	return gTriggerReport.hitName;
}
NxActor* CNovodex::HitActor()
{
	return gTriggerReport.hitActor;
}

CVoid TriggerReport::onTrigger(NxShape& triggerShape, NxShape& otherShape, NxTriggerFlag status)
{
		hitName = NULL;
		hitActor = NULL;

		NxActor* otherActor = &otherShape.getActor();
		char* OtherName = (char *)otherActor->getName();

		if (status & NX_TRIGGER_ON_ENTER & !OtherName)
		{
			// A body just entered the trigger area
			//gNbTouchedBodies++;
			NxActor* triggerActor = &triggerShape.getActor();
			hitActor = triggerActor;
			hitName = (char *)triggerActor->getName();
			if( !triggerActor || !hitActor || !hitName )
				return;

			CBool foundTarget = CFalse;
			CInstanceGeometry* instance_geo = NULL;
			for( CUInt i = 0; i < g_scene.size(); i++ )
			{
				for( CUInt j = 0; j < g_scene[i]->m_instanceGeometries.size(); j++ )
				{
					if( g_scene[i]->m_instanceGeometries[j]->m_hasEnterScript && Cmp( hitName, g_scene[i]->m_instanceGeometries[j]->m_physXName ) )
					{
						instance_geo = g_scene[i]->m_instanceGeometries[j];
						foundTarget = CTrue;
						break;
					}
				}
				if( foundTarget )
					break;
			}
			if( foundTarget )
			{
				LuaLoadAndExecute( g_lua, instance_geo->m_enterScript );
				return;
			}
			
		}
		if (status & NX_TRIGGER_ON_LEAVE)
		{
			// A body just left the trigger area
			//gNbTouchedBodies--;
			NxActor* triggerActor = &triggerShape.getActor();
			hitActor = triggerActor;
			hitName = (char *)triggerActor->getName();
			if( !triggerActor || !hitActor || !hitName )
				return;

			CBool foundTarget = CFalse;
			CInstanceGeometry* instance_geo = NULL;

			for( CUInt i = 0; i < g_scene.size(); i++ )
			{
				for( CUInt j = 0; j < g_scene[i]->m_instanceGeometries.size(); j++ )
				{
					if( g_scene[i]->m_instanceGeometries[j]->m_hasExitScript && Cmp( hitName, g_scene[i]->m_instanceGeometries[j]->m_physXName ) )
					{
						instance_geo = g_scene[i]->m_instanceGeometries[j];
						foundTarget = CTrue;
						break;
					}
				}
				if( foundTarget )
					break;
			}
			if( foundTarget )
			{
				LuaLoadAndExecute( g_lua, instance_geo->m_exitScript );
				return;
			}
		}
//		NX_ASSERT(gNbTouchedBodies>=0);

		// Mark actors in the trigger area to apply forcefield forces to them
		//NxActor* triggerActor = &triggerShape.getActor();
		//if (triggerActor->userData == (CVoid*)-1)
		//{
  //          if (status & NX_TRIGGER_ON_STAY)
  //          {
  //              NxActor* otherActor = &otherShape.getActor();
  //              otherActor->userData = (CVoid*)3;
  //          }
		//}
}


NxControllerAction ControllerHitReport::  onShapeHit(const NxControllerShapeHit& hit)
	{

		//RenderTerraintriangle(hit.faceID);

        if (1 && hit.shape)
        {
            NxCollisionGroup group = hit.shape->getGroup();
            if (group == GROUP_COLLIDABLE_PUSHABLE )
            {
                NxActor& actor = hit.shape->getActor();

                if (actor.isDynamic())
                {
                    if ((gPts[gNbPts].x!=hit.worldPos.x) || (gPts[gNbPts].y!=hit.worldPos.y) || (gPts[gNbPts].z!=hit.worldPos.z))
                    {
                        gPts[gNbPts++].x = (NxReal)hit.worldPos.x;
                        gPts[gNbPts++].y = (NxReal)hit.worldPos.y;
                        gPts[gNbPts++].z = (NxReal)hit.worldPos.z;
                        if (gNbPts==MAX_NB_PTS)	gNbPts=0;
                    }

				    // We only allow horizontal pushes. Vertical pushes when we stand on dynamic objects creates
				    // useless stress on the solver. It would be possible to enable/disable vertical pushes on
				    // particular objects, if the gameplay requires it.
				    if (hit.dir.y==0.0f)
				    {
						NxF32 coeff = actor.getMass() * hit.length * g_physXProperties.m_fCharacterPower;
						actor.addForce( hit.dir*coeff, NX_IMPULSE );						
					}
				}
			}
		}

		return NX_ACTION_NONE;
    }

NxControllerAction  ControllerHitReport::onControllerHit(const NxControllersHit& hit)
{
    return NX_ACTION_NONE;
}
CVoid ControllerHitReport::SetgNbPts(NxU32 val)
{
	gNbPts = val;
}



class MyUserNotify: public NXU_userNotify, public NXU_errorReport
{
public:
	virtual void NXU_errorMessage(bool isError, const char *str)
	{
		if (isError)
		{
			//printf("NxuStream ERROR: %s\r\n", str);
		}
		else
		{
			//printf("NxuStream WARNING: %s\r\n", str);
		}
	}

	virtual void NXU_notifyScene(NxU32 sno,	NxScene	*scene,	const	char *userProperties)
	{
        // Set the debug visualization parameters
        gPhysicsSDK->setParameter(NX_VISUALIZATION_SCALE, 1.0);
        gPhysicsSDK->setParameter(NX_VISUALIZE_COLLISION_SHAPES, 1);
        gPhysicsSDK->setParameter(NX_VISUALIZE_ACTOR_AXES, 1);
		gPhysicsSDK->setParameter(NX_VISUALIZE_CONTACT_POINT, 1);
		gPhysicsSDK->setParameter(NX_VISUALIZE_CONTACT_NORMAL, 1);
		gPhysicsSDK->setParameter(NX_VISUALIZE_CONTACT_FORCE, 1);
	};

	virtual void NXU_notifyJoint(NxJoint	*joint,	const	char *userProperties){};

	virtual void NXU_notifyActor(NxActor	*actor,	const	char *userProperties)
	{
		//NxShape*const* shapes = actor->getShapes();
		//NxU32 nShapes = actor->getNbShapes();
		//while (nShapes--)
		//{
		//	if( actor->isDynamic() )
		//	{
		//		shapes[nShapes]->setGroup(GROUP_COLLIDABLE_PUSHABLE); 
		//		shapes[nShapes]->setSkinWidth( 0.1 );
		//	}
		//	else
		//	{
		//		// I set the group to GROUP_COLLIDABLE_NON_PUSHABLE, so the character pushes the dynamic objects and can't push the static objects.
		//		shapes[nShapes]->setGroup(GROUP_COLLIDABLE_NON_PUSHABLE); 
		//	}
		//}
	};

	virtual void NXU_notifyCloth(NxCloth *cloth, const char *userProperties){};
	virtual void NXU_notifyFluid(NxFluid *fluid, const char *userProperties){};
 	virtual void NXU_notifyTriangleMesh(NxTriangleMesh *t,const char *userProperties){};
 	virtual void NXU_notifyConvexMesh(NxConvexMesh *c,const char *userProperties){};
 	virtual void NXU_notifyClothMesh(NxClothMesh *t,const char *userProperties){};
 	virtual void NXU_notifyCCDSkeleton(NxCCDSkeleton *t,const char *userProperties){};
	virtual void NXU_notifyHeightField(NxHeightField *t,const char *userProperties){};
	virtual NxScene *NXU_preNotifyScene(unsigned int sno, NxSceneDesc &scene, const char *userProperties){ return 0; };
	virtual bool NXU_preNotifyJoint(NxJointDesc &joint, const char *userProperties) { return true; };
	virtual bool NXU_preNotifyActor(NxActorDesc &actor, const char *userProperties)
	{

		for (NxU32 i=0; i<actor.shapes.size(); i++)
		{
			NxShapeDesc *s = actor.shapes[i];
			//s->shapeFlags|=NX_SF_VISUALIZATION; // make sure the shape visualization flags are on so we can see things!
			for(std::map<CUInt, NxMaterial*>::iterator it = g_physXMaterial.begin(); it != g_physXMaterial.end(); it++ )
			{
				if( (*it).first == s->materialIndex )
				{
					NxMaterial* material = (*it).second;
					s->materialIndex = material->getMaterialIndex();

					char temp[MAX_NAME_SIZE];
					PrintInfo( "\nAssigning PhysX material '" );
					sprintf( temp, "%i", material->getMaterialIndex() );
					PrintInfo( temp, COLOR_RED_GREEN );
					PrintInfo( "' to the actor shape '" );
					char temp2[MAX_NAME_SIZE];
					sprintf( temp2, "%s", s->name );
					PrintInfo( temp2, COLOR_RED_GREEN );
					PrintInfo( "'");
					break;
				}
			}
		}
		
		NxActor* a = gPhysXscene->createActor( actor );
		g_multipleView->m_nx->m_nxActorNames.push_back( a->getName() );

		CChar temp[512];
		PrintInfo( "\nActor '" );
		sprintf( temp, " %s " , a->getName() );
		PrintInfo( temp, COLOR_RED_GREEN );
		PrintInfo( "' loaded successfuly" );

		NxShape*const* shapes = a->getShapes();
		NxU32 nShapes = a->getNbShapes();
		while (nShapes--)
		{
			if( a->isDynamic() )
			{
				shapes[nShapes]->setGroup(GROUP_COLLIDABLE_PUSHABLE); 
				g_multipleView->m_nx->m_nxActorTypes.push_back( ePHYSXELEMENTLIST_DYNAMIC_RIGIDBODY );
			}
			else
			{
				// I set the group to GROUP_COLLIDABLE_NON_PUSHABLE, so the character pushes the dynamic objects and can't push the static objects.
				shapes[nShapes]->setGroup(GROUP_COLLIDABLE_NON_PUSHABLE); 
				g_multipleView->m_nx->m_nxActorTypes.push_back( ePHYSXELEMENTLIST_STATIC_RIGIDBODY );
			}
		}
		return true;
	};
	virtual bool NXU_preNotifyMaterial(NxMaterialDesc &desc,const char *userProperties, CUInt index)
	{
		NxMaterial* material = gPhysXscene->createMaterial( desc );
		g_physXMaterial[index] = material;

		char temp[MAX_NAME_SIZE];
		PrintInfo( "\nCreating PhysX material '" );
		sprintf( temp,  "%i", material->getMaterialIndex() );
		PrintInfo( temp, COLOR_RED_GREEN );
		PrintInfo("'");
		PrintInfo( "\nMaterial Properties:" );
		char temp2[MAX_NAME_SIZE];
		sprintf( temp2, "\nBounce: %f, Static friction: %f, Dynamic friction: %f", material->getRestitution(),  material->getStaticFriction(), material->getDynamicFriction() );
		PrintInfo( temp2, COLOR_RED_GREEN );
		char temp3[MAX_NAME_SIZE];
		sprintf( temp3, "\nStatic friction V: %f, Dynamic friction V: %f, Anisotropy direction: Vec3( %f, %f, %f)", material->getStaticFrictionV(), material->getDynamicFrictionV(), material->getDirOfAnisotropy().x,  material->getDirOfAnisotropy().y, material->getDirOfAnisotropy().z);
		PrintInfo( temp3, COLOR_RED_GREEN );

		return true;
	};
	virtual bool NXU_preNotifyTriangleMesh(NxTriangleMeshDesc &t,const char *userProperties) { return true;	};
	virtual bool NXU_preNotifyConvexMesh(NxConvexMeshDesc &t,const char *userProperties)
	{
		return true;
	}
	virtual bool NXU_preNotifyClothMesh(NxClothMeshDesc &t,const char *userProperties) { return true; };
	virtual bool NXU_preNotifyCCDSkeleton(NxSimpleTriangleMesh &t,const char *userProperties) {	return true; };
	virtual bool NXU_preNotifyHeightField(NxHeightFieldDesc &t,const char *userProperties) { return true; }
	virtual bool NXU_preNotifySceneInstance(const char *name,const char *sceneName,const char *userProperties,NxMat34 &rootNode,NxU32 depth) { return true; };
	virtual void NXU_notifySceneFailed(unsigned	int	sno, NxSceneDesc &scene, const char	*userProperties) {};
	virtual void NXU_notifyJointFailed(NxJointDesc &joint, const char	*userProperties) {};
	virtual void NXU_notifyActorFailed(NxActorDesc &actor, const char	*userProperties) {};
	virtual void NXU_notifyTriangleMeshFailed(NxTriangleMeshDesc &t,const char *userProperties) {};
	virtual void NXU_notifyConvexMeshFailed(NxConvexMeshDesc &t,const char *userProperties)	{};
	virtual void NXU_notifyClothMeshFailed(NxClothMeshDesc &t,const char *userProperties) {};
	virtual void NXU_notifyCCDSkeletonFailed(NxSimpleTriangleMesh &t,const char *userProperties) {};
	virtual void NXU_notifyHeightFieldFailed(NxHeightFieldDesc &t,const char *userProperties) {};
};

MyUserNotify gUserNotify;

NX_BOOL CNovodex::LoadScene(const CChar *pFilename,NXU::NXU_FileType type)
{
	NX_BOOL success = false;

	if (gPhysicsSDK)
	{
		NXU::releasePersistentMemory();

		NXU::NxuPhysicsCollection *c = NXU::loadCollection(pFilename, type );
		if ( c )
		{
	//		//if (gPhysXscene)
	//		//{
	//		//	releaseNx( CFalse );
	//		//}

			if (gPhysicsSDK)
			{
				NxSceneDesc sceneDesc;
				sceneDesc.gravity = g_multipleView->m_nx->gDefaultGravity;
				sceneDesc.simType = NX_SIMULATION_HW;

				NxScene* defaultScene = gPhysicsSDK->createScene( sceneDesc );
				if( !defaultScene )
				{
					sceneDesc.simType = NX_SIMULATION_SW;
					defaultScene = gPhysicsSDK->createScene(sceneDesc);
				}

				success = NXU::instantiateCollection( c, *gPhysicsSDK, defaultScene, 0, &gUserNotify );
				for( CUInt i = 1; i < gPhysicsSDK->getNbScenes(); i++ )
				{
					NxScene* tempScene = gPhysicsSDK->getScene(i);
					for( CUInt j = 0; j < tempScene->getNbActors(); j++ )
					{
						tempScene->releaseActor( *tempScene->getActors()[j] );
					}
					gPhysicsSDK->releaseScene( *tempScene );

				}
				defaultScene = NULL;

			}
			NXU::releaseCollection(c);

		}
	}

	return success;
}

