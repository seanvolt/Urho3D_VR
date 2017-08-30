
#pragma once

#include <Urho3D/Graphics/Texture2D.h>
#include <Urho3D/Input/Controls.h>
#include <../ThirdParty/OpenVR/headers/openvr.h>
#include <../ThirdParty/OpenVR/headers/openvr_capi.h>

using namespace Urho3D;
namespace Urho3D {

	class Drawable;
	class Node;
	class Scene;
	class CollisionShape;

/// Player component, responsible for physical movement according to controls, as well as animation.
class VR : public Object
{
	URHO3D_OBJECT(VR, Object);

public:
	/// Construct.
	VR(Context* context);

	/// Register object factory and attributes.
	static void RegisterObject(Context* context);

	// Starts the VR subsystem
	virtual void InitializeVR();
	// Handle update per visual frame
	virtual void HandleUpdate(StringHash eventType, VariantMap& eventData);
	// Get tracking data from OpenVR
	virtual void UpdateHMDMatrixPose();
	// Post Render update -- we send the images to the headset after rendering
	virtual void HandlePostRender(StringHash eventType, VariantMap& eventData);
	/// Handle physics world update. Called by LogicComponent base class.
	virtual void FixedUpdate(float timeStep);
	// Converts a SteamVR matrix to urho3d matrix class
	virtual Matrix4 ConvertHmdMatrix34_tToMatrix4(const vr::HmdMatrix34_t &matPose);
	// Converts a SteamVR matrix to urho3d matrix class
	virtual Matrix4 ConvertHmdMatrix44_tToMatrix4(const vr::HmdMatrix44_t &matPose);
	// Cleans up the VR subsystem
	virtual void Stop();

	/// Returns head transform
	Matrix4 GetHeadTransform();
	Matrix4 GetHandTransform(bool isRightHand);
	virtual bool GetControllerState(bool isRightHand, vr::VRControllerState_t *pControllerState, uint32_t unControllerStateSize);
	//virtual void TriggerHapticPulse(bool isRightHand, unsigned short duration);

	//pointer to the OpenVR API
	vr::IVRSystem* m_pHMD;
	SharedPtr<Node> headNode_;
	SharedPtr<Camera> leftCamera_;
	SharedPtr<Camera> rightCamera_;
	SharedPtr<Node> rightHandNode_;
	SharedPtr<Node> leftHandNode_;
	vr::TrackedDevicePose_t m_rTrackedDevicePose[vr::k_unMaxTrackedDeviceCount];
	Matrix4 m_rmat4DevicePose[vr::k_unMaxTrackedDeviceCount];
	Matrix4 m_mat4HMDPose;
	SharedPtr<Texture2D> leftRenderTexture;
	SharedPtr<Texture2D> rightRenderTexture;
};

}