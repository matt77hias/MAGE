#pragma once

//-----------------------------------------------------------------------------
// Engine Includes
//-----------------------------------------------------------------------------
#pragma region

#include "math\coordinate_system.hpp"
#include "math\math_utils.hpp"

#pragma endregion

//-----------------------------------------------------------------------------
// Engine Declarations and Definitions
//-----------------------------------------------------------------------------
namespace mage {

	/**
	 A class of transforms.
	 */
	class alignas(16) Transform final {

	public:

		//---------------------------------------------------------------------
		// Constructors and Destructors
		//---------------------------------------------------------------------

		/**
		 Constructs a transform from the given translation, rotation and scale 
		 component.

		 @param[in]		translation
						The translation component.
		 @param[in]		rotation
						The rotation component.
		 @param[in]		scale
						The scale component.
		 */
		explicit Transform(
			XMFLOAT3 translation = { 0.0f, 0.0f, 0.0f }, 
			XMFLOAT3 rotation    = { 0.0f, 0.0f, 0.0f }, 
			XMFLOAT3 scale       = { 1.0f, 1.0f, 1.0f })
			: m_translation(std::move(translation)), 
			m_rotation(std::move(rotation)), 
			m_scale(std::move(scale)) {
			
			SetDirty();
		}
		
		/**
		 Constructs a transform from the given translation, rotation and scale 
		 component.

		 @param[in]		translation
						The translation component.
		 @param[in]		rotation
						The rotation component.
		 @param[in]		scale
						The scale component.
		 */
		explicit Transform(
			FXMVECTOR translation, 
			FXMVECTOR rotation, 
			FXMVECTOR scale)
			: m_translation(), 
			m_rotation(), 
			m_scale() {
			
			SetTranslation(translation);
			SetRotation(rotation);
			SetScale(scale);
		}

		/**
		 Constructs a transform from the given transform.

		 @param[in]		transform
						A reference to the transform to copy.
		 */
		Transform(const Transform &transform) = default;

		/**
		 Constructs a transform by moving the given transform.

		 @param[in]		transform
						A reference to the transform to move.
		 */
		Transform(Transform &&transform) = default;

		/**
		 Destructs this transform.
		 */
		~Transform() = default;
		
		//---------------------------------------------------------------------
		// Assignment Operators
		//---------------------------------------------------------------------

		/**
		 Copies the given transform to this transform.

		 @param[in]		transform
						A reference to the transform to copy.
		 @return		A reference to the copy of the given transform (i.e. 
						this transform).
		 */
		Transform &operator=(const Transform &transform) = default;

		/**
		 Moves the given transform to this transform.

		 @param[in]		transform
						A reference to the transform to move.
		 @return		A reference to the moved transform (i.e. this 
						transform).
		 */
		Transform &operator=(Transform &&transform) = default;

		//---------------------------------------------------------------------
		// Member Methods: Translation
		//---------------------------------------------------------------------

		/**
		 Sets the x-value of the translation component of this transform to the 
		 given value.

		 @param[in]		x
						The x-value of the translation component.
		 */
		void SetTranslationX(F32 x) noexcept {
			m_translation.x = x;
			SetDirty();
		}
		
		/**
		 Sets the y-value of the translation component of this transform to the 
		 given value.

		 @param[in]		y
						The y-value of the translation component.
		 */
		void SetTranslationY(F32 y) noexcept {
			m_translation.y = y;
			SetDirty();
		}
		
		/**
		 Sets the z-value of the translation component of this transform to the 
		 given value.

		 @param[in]		z
						The z-value of the translation component.
		 */
		void SetTranslationZ(F32 z) noexcept {
			m_translation.z = z;
			SetDirty();
		}
		
		/**
		 Sets the translation component of this transform to the given 
		 translation component.

		 @param[in]		x
						The x-value of the translation component.
		 @param[in]		y
						The y-value of the translation component.
		 @param[in]		z
						The z-value of the translation component.
		 */
		void SetTranslation(F32 x, F32 y, F32 z) noexcept {
			m_translation.x = x;
			m_translation.y = y;
			m_translation.z = z;
			SetDirty();
		}
		
		/**
		 Sets the translation component of this transform to the given 
		 translation component.

		 @param[in]		translation
						The translation component.
		 */
		void SetTranslation(XMFLOAT3 translation) noexcept {
			m_translation = std::move(translation);
			SetDirty();
		}
		
		/**
		 Sets the translation component of this transform to the given 
		 translation component.

		 @param[in]		translation
						The translation component.
		 */
		void XM_CALLCONV SetTranslation(FXMVECTOR translation) noexcept {
			XMStoreFloat3(&m_translation, translation);
			SetDirty();
		}

		/**
		 Adds the given x-value to the translation component of this transform.

		 @param[in]		x
						The x-value of the translation component to add.
		 */
		void AddTranslationX(F32 x) noexcept {
			m_translation.x += x;
			SetDirty();
		}
		
		/**
		 Adds the given y-value to the translation component of this transform.

		 @param[in]		y
						The y-value of the translation component to add.
		 */
		void AddTranslationY(F32 y) noexcept {
			m_translation.y += y;
			SetDirty();
		}
		
		/**
		 Adds the given z-value to the translation component of this transform.

		 @param[in]		z
						The z-value of the translation component to add.
		 */
		void AddTranslationZ(F32 z) noexcept {
			m_translation.z += z;
			SetDirty();
		}
		
		/**
		 Adds the given translation component to the translation component of 
		 this transform.

		 @param[in]		x
						The x-value of the translation component to add.
		 @param[in]		y
						The y-value of the translation component to add.
		 @param[in]		z
						The z-value of the translation component to add.
		 */
		void AddTranslation(F32 x, F32 y, F32 z) noexcept {
			m_translation.x += x;
			m_translation.y += y;
			m_translation.z += z;
			SetDirty();
		}
		
		/**
		 Adds the given translation component to the translation component of 
		 this transform.

		 @param[in]		translation
						A reference to the translation component to add.
		 */
		void AddTranslation(const XMFLOAT3 &translation) noexcept {
			AddTranslation(translation.x, translation.y, translation.z);
		}
		
		/**
		 Adds the given translation component to the translation component of 
		 this transform.

		 @param[in]		translation
						The translation component to add.
		 */
		void XM_CALLCONV AddTranslation(FXMVECTOR translation) noexcept {
			AddTranslation(XMVectorGetX(translation), 
				           XMVectorGetY(translation), 
				           XMVectorGetZ(translation));
		}

		/**
		 Returns the x-value of the translation component of this transform.

		 @return		The x-value of the translation component of this 
						transform.
		 */
		F32 GetTranslationX() const noexcept {
			return m_translation.x;
		}
		
		/**
		 Returns the y-value of the translation component of this transform.

		 @return		The y-value of the translation component of this 
						transform.
		 */
		F32 GetTranslationY() const noexcept {
			return m_translation.y;
		}
		
		/**
		 Returns the z-value of the translation component of this transform.

		 @return		The z-value of the translation component of this 
						transform.
		 */
		F32 GetTranslationZ() const noexcept {
			return m_translation.z;
		}
		
		/**
		 Returns the translation component of this transform.

		 @return		The translation component of this transform.
		 */
		const XMFLOAT3 GetTranslation() const noexcept {
			return m_translation;
		}
		
		/**
		 Returns the object-to-parent translation matrix of this transform.

		 @return		The object-to-parent translation matrix of this 
						transform.
		 */
		const XMMATRIX GetObjectToParentTranslationMatrix() const noexcept {
			return XMMatrixTranslationFromVector(XMLoadFloat3(&m_translation));
		}

		/**
		 Returns the parent-to-object translation matrix of this transform.

		 @return		The parent-to-object translation matrix of this 
						transform.
		 */
		const XMMATRIX GetParentToObjectTranslationMatrix() const noexcept {
			return XMMatrixTranslationFromVector(-XMLoadFloat3(&m_translation));
		}

		//---------------------------------------------------------------------
		// Member Methods: Rotation
		//---------------------------------------------------------------------

		/**
		 Sets the x-value of the rotation component of this transform to the 
		 given value.

		 @param[in]		x
						The x-value of the rotation component.
		 */
		void SetRotationX(F32 x) noexcept {
			m_rotation.x = x;
			SetDirty();
		}
		
		/**
		 Sets the y-value of the rotation component of this transform to the 
		 given value.

		 @param[in]		y
						The y-value of the rotation component.
		 */
		void SetRotationY(F32 y) noexcept {
			m_rotation.y = y;
			SetDirty();
		}
		
		/**
		 Sets the z-value of the rotation component of this transform to the 
		 given value.

		 @param[in]		z
						The z-value of the rotation component.
		 */
		void SetRotationZ(F32 z) noexcept {
			m_rotation.z = z;
			SetDirty();
		}
		
		/**
		 Sets the rotation component of this transform to the given rotation 
		 component.

		 @param[in]		x
						The x-value of the rotation component.
		 @param[in]		y
						 The y-value of the rotation component.
		 @param[in]		z
						The z-value of the rotation component.
		 */
		void SetRotation(F32 x, F32 y, F32 z) noexcept {
			m_rotation.x = x;
			m_rotation.y = y;
			m_rotation.z = z;
			SetDirty();
		}
		
		/**
		 Sets the rotation component of this transform to the given rotation 
		 component.

		 @param[in]		rotation
						The rotation component.
		 */
		void SetRotation(XMFLOAT3 rotation) noexcept {
			m_rotation = std::move(rotation);
			SetDirty();
		}

		/**
		 Sets the rotation component of this transform to the given rotation 
		 component.

		 @param[in]		rotation
						The rotation component.
		 */
		void XM_CALLCONV SetRotation(FXMVECTOR rotation) noexcept {
			XMStoreFloat3(&m_rotation, rotation);
			SetDirty();
		}
		
		/**
		 Sets the rotation component to a rotation of the given angle around 
		 the given normal.

		 @param[in]		normal
						The normal.
		 @param[in]		angle
						The angle.
		 */
		void XM_CALLCONV SetRotationAroundDirection(
			FXMVECTOR normal, F32 angle) noexcept {

			const XMMATRIX rotation_m = XMMatrixRotationNormal(normal, angle);
			
			XMFLOAT4X4 rotation;
			XMStoreFloat4x4(&rotation, rotation_m);

			// cosf function instead of sinf in case the angles are not in [-1,1]
			m_rotation.y = -asinf(rotation._32);
			const F32 cp = cosf(m_rotation.y);
			const F32 cr = rotation._22 / cp;
			m_rotation.z = acosf(cr);
			const F32 cy = rotation._33 / cp;
			m_rotation.x = acosf(cy);

			SetDirty();
		}
		
		/**
		 Adds the given x-value to the rotation component of this transform.

		 @param[in]		x
						The x-value of the rotation component to add.
		 */
		void AddRotationX(F32 x) noexcept {
			m_rotation.x += x;
			SetDirty();
		}
		
		/**
		 Adds the given y-value to the rotation component of this transform.

		 @param[in]		y
						The y-value of the rotation component to add.
		 */
		void AddRotationY(F32 y) noexcept {
			m_rotation.y += y;
			SetDirty();
		}
		
		/**
		 Adds the given z-value to the rotation component of this transform.

		 @param[in]		z
						The z-value of the rotation component to add.
		 */
		void AddRotationZ(F32 z) noexcept {
			m_rotation.z += z;
			SetDirty();
		}
		
		/**
		 Adds the given rotation component to the rotation component of this 
		 transform.

		 @param[in]		x
						The x-value of the rotation component to add.
		 @param[in]		y
						The y-value of the rotation component to add.
		 @param[in]		z
						The z-value of the rotation component to add.
		 */
		void AddRotation(F32 x, F32 y, F32 z) noexcept {
			m_rotation.x += x;
			m_rotation.y += y;
			m_rotation.z += z;
			SetDirty();
		}
		
		/**
		 Adds the given rotation component to the rotation component of this 
		 transform.

		 @param[in]		rotation
						A reference to the rotation component to add.
		 */
		void AddRotation(const XMFLOAT3 &rotation) noexcept {
			AddRotation(rotation.x, rotation.y, rotation.z);
		}
		
		/**
		 Adds the given rotation component to the rotation component of this 
		 transform.

		 @param[in]		rotation
						The rotation component to add.
		 */
		void XM_CALLCONV AddRotation(FXMVECTOR rotation) noexcept {
			AddRotation(XMVectorGetX(rotation), 
				        XMVectorGetY(rotation), 
				        XMVectorGetZ(rotation));
		}

		/**
		 Adds the given x-value to the rotation component of this transform and 
		 clamps the resulting rotation component of this transform between the 
		 given values.

		 @pre			@a min_angle lies in [-pi, pi].
		 @pre			@a max_angle lies in [-pi, pi].
		 @pre			@a min_angle is not greater than @a max_angle.
		 @param[in]		x
						The x-value of the rotation component to add.
		 @param[in]		min_angle
						The minimum angle (in radians).
		 @param[in]		max_angle
						The maximum angle (in radians).
		 */
		void AddAndClampRotationX(
			F32 x, F32 min_angle, F32 max_angle) noexcept {
			
			m_rotation.x = ClampAngleRadians(m_rotation.x + x, min_angle, max_angle);
			SetDirty();
		}

		/**
		 Adds the given y-value to the rotation component of this transform and 
		 clamps the resulting rotation component of this transform between the 
		 given values.

		 @pre			@a min_angle lies in [-pi, pi].
		 @pre			@a max_angle lies in [-pi, pi].
		 @pre			@a min_angle is not greater than @a max_angle.
		 @param[in]		y
						The y-value of the rotation component to add.
		 @param[in]		min_angle
						The minimum angle (in radians).
		 @param[in]		max_angle
						The maximum angle (in radians).
		 */
		void AddAndClampRotationY(
			F32 y, F32 min_angle, F32 max_angle) noexcept {
			
			m_rotation.y = ClampAngleRadians(m_rotation.y + y, min_angle, max_angle);
			SetDirty();
		}

		/**
		 Adds the given z-value to the rotation component of this transform and 
		 clamps the resulting rotation component of this transform between the 
		 given values.

		 @pre			@a min_angle lies in [-pi, pi].
		 @pre			@a max_angle lies in [-pi, pi].
		 @pre			@a min_angle is not greater than @a max_angle.
		 @param[in]		z
						The z-value of the rotation component to add.
		 @param[in]		min_angle
						The minimum angle (in radians).
		 @param[in]		max_angle
						The maximum angle (in radians).
		 */
		void AddAndClampRotationZ(
			F32 z, F32 min_angle, F32 max_angle) noexcept {
			
			m_rotation.z = ClampAngleRadians(m_rotation.z + z, min_angle, max_angle);
			SetDirty();
		}

		/**
		 Adds the given rotation component to the rotation component of this 
		 transform and clamps the resulting rotation component of this 
		 transform between the given values.

		 @pre			@a min_angle lies in [-pi, pi].
		 @pre			@a max_angle lies in [-pi, pi].
		 @pre			@a min_angle is not greater than @a max_angle.
		 @param[in]		x
						The x-value of the rotation component to add.
		 @param[in]		y
						The y-value of the rotation component to add.
		 @param[in]		z
						The z-value of the rotation component to add.
		 @param[in]		min_angle
						The minimum angle (in radians).
		 @param[in]		max_angle
						The maximum angle (in radians).
		 */
		void AddAndClampRotation(
			F32 x, F32 y, F32 z, F32 min_angle, F32 max_angle) noexcept {

			m_rotation.x = ClampAngleRadians(m_rotation.x + x, min_angle, max_angle);
			m_rotation.y = ClampAngleRadians(m_rotation.y + y, min_angle, max_angle);
			m_rotation.z = ClampAngleRadians(m_rotation.z + z, min_angle, max_angle);
			SetDirty();
		}

		/**
		 Adds the given rotation component to the rotation component of this 
		 transform and clamps the resulting rotation component of this 
		 transform between the given values.

		 @pre			@a min_angle lies in [-pi, pi].
		 @pre			@a max_angle lies in [-pi, pi].
		 @pre			@a min_angle is not greater than @a max_angle.
		 @param[in]		rotation
						A reference to the rotation component to add.
		 @param[in]		min_angle
						The minimum angle (in radians).
		 @param[in]		max_angle
						The maximum angle (in radians).
		 */
		void AddAndClampRotation(
			const XMFLOAT3 &rotation, F32 min_angle, F32 max_angle) noexcept {

			AddAndClampRotation(rotation.x, rotation.y, rotation.z, min_angle, max_angle);
		}

		/**
		 Adds the given rotation component to the rotation component of this 
		 transform and clamps the resulting rotation component of this 
		 transform between the given values.

		 @pre			@a min_angle lies in [-pi, pi].
		 @pre			@a max_angle lies in [-pi, pi].
		 @pre			@a min_angle is not greater than @a max_angle.
		 @param[in]		rotation
						The rotation component to add.
		 @param[in]		min_angle
						The minimum angle (in radians).
		 @param[in]		max_angle
						The maximum angle (in radians).
		 */
		void XM_CALLCONV AddAndClampRotation(
			FXMVECTOR rotation, F32 min_angle, F32 max_angle) noexcept {

			AddAndClampRotation(XMVectorGetX(rotation), 
				                XMVectorGetY(rotation), 
				                XMVectorGetZ(rotation), 
				                min_angle, max_angle);
		}

		/**
		 Returns the x-value of the rotation component of this transform.

		 @return		The x-value of the rotation component of this 
						transform.
		 */
		F32 GetRotationX() const noexcept {
			return m_rotation.x;
		}
		
		/**
		 Returns the y-value of the rotation component of this transform.

		 @return		The y-value of the rotation component of this 
						transform.
		 */
		F32 GetRotationY() const noexcept {
			return m_rotation.y;
		}
		
		/**
		 Returns the z-value of the rotation component of this transform.

		 @return		The z-value of the rotation component of this 
						transform.
		 */
		F32 GetRotationZ() const noexcept {
			return m_rotation.z;
		}
		
		/**
		 Returns the rotation component of this transform.

		 @return		The rotation component of this transform.
		 */
		const XMFLOAT3 GetRotation() const noexcept {
			return m_rotation;
		}
		
		/**
		 Returns the object-to-parent rotation matrix of this transform.

		 @return		The object-to-parent rotation matrix of this transform.
		 */
		const XMMATRIX GetObjectToParentRotationMatrix() const noexcept {
			return XMMatrixRotationZ(GetRotationZ()) 
				 * XMMatrixRotationX(GetRotationX()) 
				 * XMMatrixRotationY(GetRotationY());
		}

		/**
		 Returns the parent-to-object rotation matrix of this transform.

		 @return		The parent-to-object rotation matrix of this transform.
		 */
		const XMMATRIX GetParentToObjectRotationMatrix() const noexcept {
			return XMMatrixRotationY(-GetRotationY()) 
				 * XMMatrixRotationX(-GetRotationX()) 
				 * XMMatrixRotationZ(-GetRotationZ());
		}

		//---------------------------------------------------------------------
		// Member Methods: Scale
		//---------------------------------------------------------------------

		/**
		 Sets the x-value of the scale component of this transform to the given 
		 value.

		 @param[in]		x
						The x-value of the scale component.
		 */
		void SetScaleX(F32 x) noexcept {
			m_scale.x = x;
			SetDirty();
		}
		
		/**
		 Sets the y-value of the scale component of this transform to the given 
		 value.

		 @param[in]		y
						The y-value of the scale component.
		 */
		void SetScaleY(F32 y) noexcept {
			m_scale.y = y;
			SetDirty();
		}
		
		/**
		 Sets the z-value of the scale component of this transform to the given 
		 value.

		 @param[in]		z
						The z-value of the scale component.
		 */
		void SetScaleZ(F32 z) noexcept {
			m_scale.z = z;
			SetDirty();
		}

		/**
		 Sets the scale component of this transform to the given scale 
		 component.

		 @param[in]		s
						The scale component.
		 */
		void SetScale(F32 s) noexcept {
			SetScale(s, s, s);
		}
		
		/**
		 Sets the scale component of this transform to the given scale 
		 component.

		 @param[in]		x
						The x-value of the scale component.
		 @param[in]		y
						The y-value of the scale component.
		 @param[in]		z
						The z-value of the scale component.
		 */
		void SetScale(F32 x, F32 y, F32 z) noexcept {
			m_scale.x = x;
			m_scale.y = y;
			m_scale.z = z;
			SetDirty();
		}
		
		/**
		 Sets the scale component of this transform to the given scale 
		 component.

		 @param[in]		scale
						The scale component.
		 */
		void SetScale(XMFLOAT3 scale) noexcept {
			m_scale = std::move(scale);
			SetDirty();
		}

		/**
		 Sets the scale component of this transform to the given scale 
		 component.

		 @param[in]		scale
						The scale component.
		 */
		void XM_CALLCONV SetScale(FXMVECTOR scale) noexcept {
			XMStoreFloat3(&m_scale, scale);
			SetDirty();
		}

		/**
		 Adds the given x-value to the scale component of this transform.

		 @param[in]		x
						The x-value of the scale component to add.
		 */
		void AddScaleX(F32 x) noexcept {
			m_scale.x += x;
			SetDirty();
		}
		
		/**
		 Adds the given y-value to the scale component of this transform.

		 @param[in]		y
						The y-value of the scale component to add.
		 */
		void AddScaleY(F32 y) noexcept {
			m_scale.y += y;
			SetDirty();
		}
		
		/**
		 Adds the given z-value to the scale component of this transform.

		 @param[in]		z
						The z-value of the scale component to add.
		 */
		void AddScaleZ(F32 z) noexcept {
			m_scale.z += z;
			SetDirty();
		}

		/**
		 Adds the given scale component to the scale component of this 
		 transform.

		 @param[in]		s
						The scale component to add.
		 */
		void AddScale(F32 s) noexcept {
			AddScale(s, s, s);
		}
		
		/**
		 Adds the given scale component to the scale component of this 
		 transform.

		 @param[in]		x
						The x-value of the scale component to add.
		 @param[in]		y
						The y-value of the scale component to add.
		 @param[in]		z
						The z-value of the scale component to add.
		 */
		void AddScale(F32 x, F32 y, F32 z) noexcept {
			m_scale.x += x;
			m_scale.y += y;
			m_scale.z += z;
			SetDirty();
		}
		
		/**
		 Adds the given scale component to the scale component of this 
		 transform.

		 @param[in]		scale
						A reference to the scale component to add.
		 */
		void AddScale(const XMFLOAT3 &scale) noexcept {
			AddScale(scale.x, scale.y, scale.z);
		}

		/**
		 Adds the given scale component to the scale component of this 
		 transform.

		 @param[in]		scale
						The scale component to add.
		 */
		void XM_CALLCONV AddScale(FXMVECTOR scale) noexcept {
			AddScale(XMVectorGetX(scale), XMVectorGetY(scale), XMVectorGetZ(scale));
		}
		
		/**
		 Returns the x-value of the scale component of this transform.

		 @return		The x-value of the scale component of this transform.
		 */
		F32 GetScaleX() const noexcept {
			return m_scale.x;
		}
		
		/**
		 Returns the y-value of the scale component of this transform.

		 @return		The y-value of the scale component of this transform.
		 */
		F32 GetScaleY() const noexcept {
			return m_scale.y;
		}
		
		/**
		 Returns the z-value of the scale component of this transform.

		 @return		The z-value of the scale component of this transform.
		 */
		F32 GetScaleZ() const noexcept {
			return m_scale.z;
		}
		
		/**
		 Returns the scale component of this transform.

		 @return		The scale component of this transform.
		 */
		const XMFLOAT3 GetScale() const noexcept {
			return m_scale;
		}
		
		/**
		 Returns the object-to-parent scale matrix of this transform.

		 @return		The scale object-to-parent matrix of this transform.
		 */
		const XMMATRIX GetObjectToParentScaleMatrix() const noexcept {
			return XMMatrixScalingFromVector(XMLoadFloat3(&m_scale));
		}

		/**
		 Returns the parent-to-object scale matrix of this transform.

		 @return		The parent-to-object scale matrix of this transform.
		 */
		const XMMATRIX GetParentToObjectScaleMatrix() const noexcept {
			return XMMatrixScalingFromVector(XMVectorSet(
				1.0f / m_scale.x, 1.0f / m_scale.y, 1.0f / m_scale.z, 0.0f));
		}

		//---------------------------------------------------------------------
		// Member Methods: Object Space
		//---------------------------------------------------------------------

		/**
		 Returns the position of the local origin of this transform expressed 
		 in object space coordinates.

		 @return		The position of the local origin of this transform 
						expressed in object space coordinates.
		 */
		const XMVECTOR GetObjectOrigin() const noexcept {
			return XMVectorSet(0.0f, 0.0f, 0.0f, 1.0f);
		}

		/**
		 Returns the direction of the local x-axis of this transform expressed 
		 in object space coordinates.

		 @return		The direction of the local x-axis of this transform 
						expressed in object space coordinates.
		 */
		const XMVECTOR GetObjectAxisX() const noexcept {
			return XMVectorSet(1.0f, 0.0f, 0.0f, 0.0f);
		}
		
		/**
		 Returns the direction of the local y-axis of this transform expressed 
		 in object space coordinates.

		 @return		The direction of the local y-axis of this transform 
						expressed in object space coordinates.
		 */
		const XMVECTOR GetObjectAxisY() const noexcept {
			return XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f);
		}
		
		/**
		 Returns the direction of the local z-axis of this transform expressed 
		 in object space coordinates.

		 @return		The direction of the local z-axis of this transform 
						expressed in object space coordinates.
		 */
		const XMVECTOR GetObjectAxisZ() const noexcept {
			return XMVectorSet(0.0f, 0.0f, 1.0f, 0.0f);
		}
		
		/**
		 Returns the local Cartesian axes system of this transform in object 
		 space coordinates.

		 @return		The local Cartesian axes system of this transform 
						expressed in object space coordinates.
		 */
		const CartesianAxesSystem GetObjectAxes() const noexcept {
			return CartesianAxesSystem(GetObjectAxisX(), GetObjectAxisY(), GetObjectAxisZ());
		}
		
		/**
		 Returns the local Cartesian coordinate system of this transform in 
		 object space coordinates.

		 @return		The local Cartesian coordinate system of this transform 
						expressed in object space coordinates.
		 */
		const CartesianCoordinateSystem GetObjectCoordinateSystem() const noexcept {
			return CartesianCoordinateSystem(GetObjectOrigin(), GetObjectAxes());
		}
		
		//---------------------------------------------------------------------
		// Member Methods: Parent Space
		//---------------------------------------------------------------------

		/**
		 Returns the position of the local origin of this transform expressed 
		 in parent space coordinates.

		 @return		The position of the local origin of this transform 
						expressed in parent space coordinates.
		 */
		const XMVECTOR GetParentOrigin() const noexcept {
			return XMLoadFloat3(&m_translation);
		}

		/**
		 Returns the direction of the local x-axis of this transform expressed 
		 in parent space coordinates.

		 @return		The direction of the local x-axis of this transform 
						expressed in parent space coordinates.
		 */
		const XMVECTOR GetParentAxisX() const noexcept {
			return TransformObjectToParentDirection(GetObjectAxisX());
		}
		
		/**
		 Returns the direction of the local y-axis of this transform expressed 
		 in parent space coordinates.

		 @return		The direction of the local y-axis of this transform 
						expressed in parent space coordinates.
		 */
		const XMVECTOR GetParentAxisY() const noexcept {
			return TransformObjectToParentDirection(GetObjectAxisY());
		}
		
		/**
		 Returns the direction of the local z-axis of this transform expressed 
		 in parent space coordinates.

		 @return		The direction of the local z-axis of this transform 
						expressed in parent space coordinates.
		 */
		const XMVECTOR GetParentAxisZ() const noexcept {
			return TransformObjectToParentDirection(GetObjectAxisZ());
		}
		
		/**
		 Returns the local Cartesian axes system of this transform expressed in 
		 parent space coordinates.

		 @return		The local Cartesian axes system of this transform 
						expressed in parent space coordinates.
		 */
		const CartesianAxesSystem GetParentAxes() const noexcept {
			return CartesianAxesSystem(GetParentAxisX(), 
				                       GetParentAxisY(), 
				                       GetParentAxisZ());
		}
		
		/**
		 Returns the local Cartesian coordinate system of this transform in 
		 parent space coordinates.

		 @return		The local Cartesian coordinate system of this transform 
						expressed in parent space coordinates.
		 */
		const CartesianCoordinateSystem GetParentCoordinateSystem() const noexcept {
			return CartesianCoordinateSystem(GetParentOrigin(), GetParentAxes());
		}

		//---------------------------------------------------------------------
		// Member Methods: Transformation
		//---------------------------------------------------------------------

		/**
		 Returns the object-to-parent matrix of this transform.

		 @return		The object-to-parent matrix of this transform.
		 */
		const XMMATRIX GetObjectToParentMatrix() const noexcept {
			UpdateObjectToParentMatrix();
			return m_object_to_parent;
		}

		/**
		 Returns the parent-to-object matrix of this transform.

		 @return		The parent-to-object matrix of this transform.
		 */
		const XMMATRIX GetParentToObjectMatrix() const noexcept {
			UpdateParentToObjectMatrix();
			return m_parent_to_object;
		}

		/**
		 Transforms the given vector expressed in object space coordinates to 
		 parent space coordinates.

		 @param[in]		vector
						The vector expressed in object space coordinates.
		 @return		The transformed vector expressed in parent space 
						coordinates.
		 */
		const XMVECTOR XM_CALLCONV TransformObjectToParent(
			FXMVECTOR vector) const noexcept {
			
			return XMVector4Transform(vector, GetObjectToParentMatrix());
		}

		/**
		 Transforms the given point expressed in object space coordinates to 
		 parent space coordinates.

		 @param[in]		point
						The point expressed in object space coordinates.
		 @return		The transformed point expressed in parent space 
						coordinates.
		 */
		const XMVECTOR XM_CALLCONV TransformObjectToParentPoint(
			FXMVECTOR point) const noexcept {
			
			return XMVector3TransformCoord(point, GetObjectToParentMatrix());
		}

		/**
		 Transforms the given direction expressed in object space coordinates to 
		 parent space coordinates.

		 @param[in]		direction
						The direction expressed in object space coordinates.
		 @return		The transformed direction expressed in parent space 
						coordinates.
		 */
		const XMVECTOR XM_CALLCONV TransformObjectToParentDirection(
			FXMVECTOR direction) const noexcept {
			
			return XMVector3TransformNormal(direction, GetObjectToParentMatrix());
		}

		/**
		 Transforms the given vector expressed in parent space coordinates to 
		 object space coordinates.

		 @param[in]		vector
						The vector expressed in parent space coordinates.
		 @return		The transformed vector expressed in object space 
						coordinates.
		 */
		const XMVECTOR XM_CALLCONV TransformParentToObject(
			FXMVECTOR vector) const noexcept {
			
			return XMVector4Transform(vector, GetParentToObjectMatrix());
		}

		/**
		 Transforms the given point expressed in parent space coordinates to 
		 object space coordinates.

		 @param[in]		point
						The point expressed in parent space coordinates.
		 @return		The transformed point expressed in object space 
						coordinates.
		 */
		const XMVECTOR XM_CALLCONV TransformParentToObjectPoint(
			FXMVECTOR point) const noexcept {
			
			return XMVector3TransformCoord(point, GetParentToObjectMatrix());
		}

		/**
		 Transforms the given direction expressed in parent space coordinates to 
		 object space coordinates.

		 @param[in]		direction
						The direction expressed in parent space coordinates.
		 @return		The transformed direction expressed in object space 
						coordinates.
		 */
		const XMVECTOR XM_CALLCONV TransformParentToObjectDirection(
			FXMVECTOR direction) const noexcept {
			
			return XMVector3TransformNormal(direction, GetParentToObjectMatrix());
		}

	private:

		//---------------------------------------------------------------------
		// Member Methods
		//---------------------------------------------------------------------

		/**
		 Sets this transform to dirty.
		 */
		void SetDirty() const noexcept {
			m_dirty_object_to_parent = true;
			m_dirty_parent_to_object = true;
		}

		/**
		 Updates the object-to-parent matrix of this transform if dirty.
		 */
		void UpdateObjectToParentMatrix() const noexcept {
			if (m_dirty_object_to_parent) {
				m_object_to_parent = GetObjectToParentScaleMatrix() 
					               * GetObjectToParentRotationMatrix() 
					               * GetObjectToParentTranslationMatrix();
				m_dirty_object_to_parent = false;
			}
		}
		
		/**
		 Updates the parent-to-object matrix of this transform if dirty.
		 */
		void UpdateParentToObjectMatrix() const noexcept {
			if (m_dirty_parent_to_object) {
				m_parent_to_object = GetParentToObjectTranslationMatrix() 
					               * GetParentToObjectRotationMatrix() 
					               * GetParentToObjectScaleMatrix();
				m_dirty_parent_to_object = false;
			}
		}

		//---------------------------------------------------------------------
		// Member Variables
		//---------------------------------------------------------------------

		/**
		 The translation component of this transform.
		 */
		XMFLOAT3 m_translation;

		/**
		 The rotation component (in radians) of this transform.
		 */
		XMFLOAT3 m_rotation;

		/**
		 The scale component of this transform.
		 */
		XMFLOAT3 m_scale;

		/**
		 The cached object-to-parent matrix of this transform.
		 */
		mutable XMMATRIX m_object_to_parent;

		/**
		 The cached parent-to-object matrix of this transform.
		 */
		mutable XMMATRIX m_parent_to_object;

		/**
		 A flag indicating whether the object-to-parent matrix of this transform 
		 is dirty.
		 */
		mutable bool m_dirty_object_to_parent;

		/**
		 A flag indicating whether the parent-to-object matrix of this transform 
		 is dirty.
		 */
		mutable bool m_dirty_parent_to_object;
	};
}