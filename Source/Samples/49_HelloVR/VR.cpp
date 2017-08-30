
//#include <Urho3D/Engine/Application.h>
//#include <Urho3D/Engine/Engine.h>
#include <Urho3D/Core/Context.h>
#include <Urho3D/Core/CoreEvents.h>
#include <Urho3D/Core/Timer.h>
#include <Urho3D/Graphics/AnimationController.h>
#include <Urho3D/Graphics/Camera.h>
#include <Urho3D/Graphics/Graphics.h>
#include <Urho3D/Graphics/Material.h>
#include <Urho3D/Graphics/Model.h>
#include <Urho3D/Graphics/Octree.h>
#include <Urho3D/Graphics/StaticModelGroup.h>
#include <Urho3D/Input/Input.h>
#include <Urho3D/IO/Log.h>
#include <Urho3D/IO/MemoryBuffer.h>
#include <Urho3D/Physics/CollisionShape.h>
#include <Urho3D/Physics/PhysicsEvents.h>
#include <Urho3D/Physics/PhysicsWorld.h>
#include <Urho3D/Physics/RigidBody.h>
#include <Urho3D/Resource/ResourceCache.h>
#include <Urho3D/Scene/Scene.h>
#include <Urho3D/Scene/SceneEvents.h>
#include "../Engine/Console.h"
#include "../Engine/DebugHud.h"
#include "VR.h"

VR::VR(Context* context) :
	Object(context)
{
	SubscribeToEvent(E_UPDATE, URHO3D_HANDLER(VR, HandleUpdate));
	SubscribeToEvent(E_POSTRENDERUPDATE, URHO3D_HANDLER(VR, HandlePostRender));
	m_pHMD = NULL;
}

void VR::RegisterObject(Context* context)
{
}

void VR::InitializeVR()
{
	vr::EVRInitError eError = vr::VRInitError_None;
	m_pHMD = vr::VR_Init(&eError, vr::VRApplication_Scene);
	if (eError != vr::VRInitError_None)
	{
		m_pHMD = NULL;
		URHO3D_LOGERROR(vr::VR_GetVRInitErrorAsEnglishDescription(eError));
		return;
	}
	if (!vr::VRCompositor())
	{
		m_pHMD = NULL;
		URHO3D_LOGERROR("Compositor initialization failed. See log file for details");
		return;
	}
	if (m_pHMD)
	{
		Node* leftCameraNode_ = headNode_->CreateChild("LeftCamera");
		leftCamera_ = leftCameraNode_->CreateComponent<Camera>();
		leftCamera_->SetFarClip(900.0f);
		Node* rightCameraNode_ = headNode_->CreateChild("RightCamera");
		rightCamera_ = rightCameraNode_->CreateComponent<Camera>();
		rightCamera_->SetFarClip(900.0f);

		uint32_t m_nRenderWidth;
		uint32_t m_nRenderHeight;
		m_pHMD->GetRecommendedRenderTargetSize(&m_nRenderWidth, &m_nRenderHeight);
		//the second rendertexture to have SetSize() called will have shadows...
		leftRenderTexture = new Texture2D(context_);
		leftRenderTexture->SetSize(m_nRenderWidth, m_nRenderHeight, Graphics::GetRGBFormat(), TEXTURE_RENDERTARGET);
		leftRenderTexture->SetFilterMode(FILTER_BILINEAR);
		RenderSurface* leftSurface = leftRenderTexture->GetRenderSurface();
		leftSurface->SetUpdateMode(SURFACE_UPDATEALWAYS);
		SharedPtr<Viewport> leftrttViewport(new Viewport(context_, leftCamera_->GetScene(), leftCamera_));
		leftSurface->SetViewport(0, leftrttViewport);
		rightRenderTexture = new Texture2D(context_);
		rightRenderTexture->SetSize(m_nRenderWidth, m_nRenderHeight, Graphics::GetRGBFormat(), TEXTURE_RENDERTARGET);
		rightRenderTexture->SetFilterMode(FILTER_BILINEAR);
		RenderSurface* rightSurface = rightRenderTexture->GetRenderSurface();
		rightSurface->SetUpdateMode(SURFACE_UPDATEALWAYS);
		SharedPtr<Viewport> rightrttViewport(new Viewport(context_, rightCamera_->GetScene(), rightCamera_));
		rightSurface->SetViewport(0, rightrttViewport);
	}
	if (m_pHMD)
	{
		Matrix4 leftEyePos = ConvertHmdMatrix34_tToMatrix4(m_pHMD->GetEyeToHeadTransform(vr::Eye_Left));
		Matrix4 rightEyePos = ConvertHmdMatrix34_tToMatrix4(m_pHMD->GetEyeToHeadTransform(vr::Eye_Right));
		leftCamera_->GetNode()->SetTransform(leftEyePos);
		rightCamera_->GetNode()->SetTransform(rightEyePos);
		//hackiness because we can't get the correct projection and eyetohead transform
		leftCamera_->GetNode()->SetRotation(Quaternion(0, -4, 0));
		rightCamera_->GetNode()->SetRotation(Quaternion(0, 4, 0));
		leftCamera_->SetFov(112);
		rightCamera_->SetFov(112);
		//end hackiness
	}
}

void VR::HandleUpdate(StringHash eventType, VariantMap& eventData)
{
	if (m_pHMD) {
		UpdateHMDMatrixPose(); //get tracking data from OpenVR
							   // Process SteamVR controller state
		for (vr::TrackedDeviceIndex_t unDevice = 0; unDevice < vr::k_unMaxTrackedDeviceCount; unDevice++)
		{
			vr::VRControllerState_t state;
			if (m_pHMD->GetControllerState(unDevice, &state, sizeof(state)))
			{
				//m_rbShowTrackedDevice[unDevice] = state.ulButtonPressed == 0;toggle visibility of controller
			}
		}
		headNode_->SetTransform(GetHeadTransform());
		leftHandNode_->SetTransform(GetHandTransform(false));
		rightHandNode_->SetTransform(GetHandTransform(true));
		Matrix4 leftEyePos = ConvertHmdMatrix34_tToMatrix4(m_pHMD->GetEyeToHeadTransform(vr::Eye_Left));
		Matrix4 rightEyePos = ConvertHmdMatrix34_tToMatrix4(m_pHMD->GetEyeToHeadTransform(vr::Eye_Right));
		Matrix4 leftProjection = ConvertHmdMatrix44_tToMatrix4(m_pHMD->GetProjectionMatrix(vr::Eye_Left, 0.1f, 30.f));
		Matrix4 rightProjection = ConvertHmdMatrix44_tToMatrix4(m_pHMD->GetProjectionMatrix(vr::Eye_Right, 0.1f, 30.f));
		//leftCamera_->SetProjection(ConvertHmdMatrix44_tToMatrix4(m_pHMD->GetProjectionMatrix(vr::Eye_Left, 0.1f, 900)) * leftEyePos * GetHeadTransform());
		//rightCamera_->SetProjection(ConvertHmdMatrix44_tToMatrix4(m_pHMD->GetProjectionMatrix(vr::Eye_Right, 0.1f, 900)) * rightEyePos * GetHeadTransform());
	}
}

//get tracking data from OpenVR
void VR::UpdateHMDMatrixPose()
{
	if (!m_pHMD)
		return;
	vr::VRCompositor()->WaitGetPoses(m_rTrackedDevicePose, vr::k_unMaxTrackedDeviceCount, NULL, 0);
}

void VR::HandlePostRender(StringHash eventType, VariantMap& eventData)
{
	if (!m_pHMD)
		return;
	vr::EVRCompositorError eError = vr::VRCompositorError_None;
	vr::Texture_t leftEyeTexture = { (void*)((uintptr_t)(leftRenderTexture->GetObjectPointer())), vr::TextureType_DirectX, vr::ColorSpace_Auto };
	eError = vr::VRCompositor()->Submit(vr::Eye_Left, &leftEyeTexture);
	if (eError != vr::VRCompositorError_None)
	{
		URHO3D_LOGERROR("LeftEyeError");
		URHO3D_LOGERROR(String((unsigned)eError));
	}
	vr::Texture_t rightEyeTexture = { (void*)((uintptr_t)(rightRenderTexture->GetObjectPointer())), vr::TextureType_DirectX, vr::ColorSpace_Auto };
	eError = vr::VRCompositor()->Submit(vr::Eye_Right, &rightEyeTexture);
	if (eError != vr::VRCompositorError_None)
	{
		URHO3D_LOGERROR("RightEyeError");
		URHO3D_LOGERROR(String((unsigned)eError));
	}
}

void VR::FixedUpdate(float timeStep)
{
}

Matrix4 VR::GetHeadTransform()
{
	if (m_rTrackedDevicePose[vr::k_unTrackedDeviceIndex_Hmd].bPoseIsValid)
	{
		return ConvertHmdMatrix34_tToMatrix4(m_rTrackedDevicePose[vr::k_unTrackedDeviceIndex_Hmd].mDeviceToAbsoluteTracking);
	}
	return Matrix4();
}

Matrix4 VR::GetHandTransform(bool isRightHand)
{
	uint32_t handIndex = (uint32_t)(m_pHMD->GetTrackedDeviceIndexForControllerRole(isRightHand ? vr::TrackedControllerRole_RightHand : vr::TrackedControllerRole_LeftHand));
	if (handIndex < vr::k_unMaxTrackedDeviceCount && m_rTrackedDevicePose[handIndex].bPoseIsValid)
	{
		return ConvertHmdMatrix34_tToMatrix4(m_rTrackedDevicePose[handIndex].mDeviceToAbsoluteTracking);
	}
	return Matrix4();
}

bool VR::GetControllerState(bool isRightHand, vr::VRControllerState_t *pControllerState, uint32_t unControllerStateSize)
{
	TrackedDeviceIndex_t handIndex = m_pHMD->GetTrackedDeviceIndexForControllerRole(isRightHand ? vr::TrackedControllerRole_RightHand : vr::TrackedControllerRole_LeftHand);
	vr::VRControllerState_t state = vr::VRControllerState_t();
	if (m_pHMD->GetControllerState(handIndex, &state, sizeof(state)))
	{
		*pControllerState = state;
		return true;
	}
	return false;
}

// not working
/*void VR::TriggerHapticPulse(bool isRightHand, unsigned short duration)
{
	unsigned int axisId = (int)vr::k_EButton_SteamVR_Touchpad - (int)vr::k_EButton_Axis0;
	//URHO3D_LOGINFO("Haptic Pulse sent strength " + String(axisId));
	m_pHMD->TriggerHapticPulse(isRightHand ? vr::TrackedControllerRole_RightHand : vr::TrackedControllerRole_LeftHand, axisId, (char)duration);
}*/

Matrix4 VR::ConvertHmdMatrix34_tToMatrix4(const vr::HmdMatrix34_t &matPose) //Converts a SteamVR matrix to urho3d matrix class
{
	return Matrix4(
		matPose.m[0][0], matPose.m[0][1], -matPose.m[0][2], matPose.m[0][3],
		matPose.m[1][0], matPose.m[1][1], -matPose.m[1][2], matPose.m[1][3],
		-matPose.m[2][0], -matPose.m[2][1], matPose.m[2][2], -matPose.m[2][3],
		//1, 0, 0, 1
		matPose.m[3][0], matPose.m[3][1], matPose.m[3][2], matPose.m[3][3]
	);
}

Matrix4 VR::ConvertHmdMatrix44_tToMatrix4(const vr::HmdMatrix44_t &matPose) //Converts a SteamVR matrix to urho3d matrix class
{
	return Matrix4(
		matPose.m[0][0], matPose.m[0][1], matPose.m[0][2], matPose.m[0][3],
		matPose.m[1][0], matPose.m[1][1], matPose.m[1][2], matPose.m[1][3],
		matPose.m[2][0], matPose.m[2][1], matPose.m[2][2], -matPose.m[2][3],
		//0, 0, 0, 0
		matPose.m[3][0], matPose.m[3][1], matPose.m[3][2], matPose.m[3][3]
	);
}    

void VR::Stop()
{
	if (m_pHMD)
	{
		vr::VR_Shutdown();
		m_pHMD = NULL;
	}
}