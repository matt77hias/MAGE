#pragma once

//-----------------------------------------------------------------------------
// Engine Includes
//-----------------------------------------------------------------------------
#pragma region

#include "transform\transform.hpp"

#pragma endregion

//-----------------------------------------------------------------------------
// Engine Declarations and Definitions
//-----------------------------------------------------------------------------
namespace mage {

	//-------------------------------------------------------------------------
	// Transform
	//-------------------------------------------------------------------------
	#pragma region

	// Forward declaration.
	class Node;

	/**
	 A class of transforms.
	 */
	class alignas(16) Transform {

	public:

		//---------------------------------------------------------------------
		// Constructors and Destructors
		//---------------------------------------------------------------------

		/**
		 Constructs a transform.
		 */
		Transform() noexcept
			: m_transform(),
			m_object_to_world(),
			m_world_to_object(),
			m_dirty_object_to_world(true),
			m_dirty_world_to_object(true),
			m_owner() {}

		/**
		 Constructs a transform from the given transform.

		 @param[in]		transform
						A reference to the transform to copy.
		 */
		Transform(const Transform& transform) noexcept
			: m_transform(transform.m_transform),
			m_object_to_world(),
			m_world_to_object(),
			m_dirty_object_to_world(true),
			m_dirty_world_to_object(true),
			m_owner() {}

		/**
		 Constructs a transform by moving the given transform.

		 @param[in]		transform
						A reference to the transform to move.
		 */
		Transform(Transform&& transform) noexcept = default;

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
		Transform& operator=(const Transform& transform) noexcept {
			m_transform = transform.m_transform;
			SetDirty();
			return *this;
		}

		/**
		 Moves the given transform to this transform.

		 @param[in]		transform
						A reference to the transform to move.
		 @return		A reference to the copy of the given transform (i.e.
						this transform).
		 */
		Transform& operator=(Transform&& transform) noexcept {
			m_transform = std::move(transform.m_transform);
			m_owner     = std::move(transform.m_owner);
			SetDirty();
			return *this;
		}

		//---------------------------------------------------------------------
		// Member Methods: Translation
		//---------------------------------------------------------------------
		#pragma region

		/**
		 Sets the x-value of the translation component of this transform to the
		 given value.

		 @param[in]		x
						The x-value of the translation component.
		 */
		void SetTranslationX(F32 x) noexcept {
			m_transform.SetTranslationX(x);
			SetDirty();
		}

		/**
		 Sets the y-value of the translation component of this transform to the
		 given value.

		 @param[in]		y
						The y-value of the translation component.
		 */
		void SetTranslationY(F32 y) noexcept {
			m_transform.SetTranslationY(y);
			SetDirty();
		}

		/**
		 Sets the z-value of the translation component of this transform to the
		 given value.

		 @param[in]		z
						The z-value of the translation component.
		 */
		void SetTranslationZ(F32 z) noexcept {
			m_transform.SetTranslationZ(z);
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
			m_transform.SetTranslation(x, y, z);
			SetDirty();
		}

		/**
		 Sets the translation component of this transform to the given
		 translation component.

		 @param[in]		translation
						The translation component.
		 */
		void SetTranslation(F32x3 translation) noexcept {
			m_transform.SetTranslation(std::move(translation));
			SetDirty();
		}

		/**
		 Sets the translation component of this transform to the given
		 translation component.

		 @param[in]		translation
						The translation component.
		 */
		void XM_CALLCONV SetTranslation(FXMVECTOR translation) noexcept {
			m_transform.SetTranslation(translation);
			SetDirty();
		}

		/**
		 Adds the given x-value to the translation component of this transform.

		 @param[in]		x
						The x-value of the translation component to add.
		 */
		void AddTranslationX(F32 x) noexcept {
			m_transform.AddTranslationX(x);
			SetDirty();
		}

		/**
		 Adds the given y-value to the translation component of this transform.

		 @param[in]		y
						The y-value of the translation component to add.
		 */
		void AddTranslationY(F32 y) noexcept {
			m_transform.AddTranslationY(y);
			SetDirty();
		}

		/**
		 Adds the given z-value to the translation component of this transform.

		 @param[in]		z
						The z-value of the translation component to add.
		 */
		void AddTranslationZ(F32 z) noexcept {
			m_transform.AddTranslationZ(z);
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
			m_transform.AddTranslation(x, y, z);
			SetDirty();
		}

		/**
		 Adds the given translation component to the translation component of
		 this transform.

		 @param[in]		translation
						A reference to the translation component to add.
		 */
		void AddTranslation(const F32x3& translation) noexcept {
			m_transform.AddTranslation(translation);
			SetDirty();
		}

		/**
		 Adds the given translation component to the translation component of
		 this transform.

		 @param[in]		translation
						The translation component to add.
		 */
		void XM_CALLCONV AddTranslation(FXMVECTOR translation) noexcept {
			m_transform.AddTranslation(translation);
			SetDirty();
		}

		/**
		 Returns the x-value of the translation component of this transform.

		 @return		The x-value of the translation component of this
						transform.
		 */
		[[nodiscard]]
		F32 GetTranslationX() const noexcept {
			return m_transform.GetTranslationX();
		}

		/**
		 Returns the y-value of the translation component of this transform.

		 @return		The y-value of the translation component of this
						transform.
		 */
		[[nodiscard]]
		F32 GetTranslationY() const noexcept {
			return m_transform.GetTranslationY();
		}

		/**
		 Returns the z-value of the translation component of this transform.

		 @return		The z-value of the translation component of this
						transform.
		 */
		[[nodiscard]]
		F32 GetTranslationZ() const noexcept {
			return m_transform.GetTranslationZ();
		}

		/**
		 Returns the translation component of this transform.

		 @return		The translation component of this transform.
		 */
		[[nodiscard]]
		const F32x3 GetTranslationView() const noexcept {
			return m_transform.GetTranslationView();
		}

		/**
		 Returns the translation component of this transform.

		 @return		The translation component of this transform.
		 */
		[[nodiscard]]
		const XMVECTOR XM_CALLCONV GetTranslation() const noexcept {
			return m_transform.GetTranslation();
		}

		/**
		 Returns the object-to-parent translation matrix of this transform.

		 @return		The object-to-parent translation matrix of this
						transform.
		 */
		[[nodiscard]]
		const XMMATRIX XM_CALLCONV
			GetObjectToParentTranslationMatrix() const noexcept {

			return m_transform.GetObjectToParentTranslationMatrix();
		}

		/**
		 Returns the parent-to-object translation matrix of this transform.

		 @return		The parent-to-object translation matrix of this
						transform.
		 */
		[[nodiscard]]
		const XMMATRIX XM_CALLCONV
			GetParentToObjectTranslationMatrix() const noexcept {

			return m_transform.GetParentToObjectTranslationMatrix();
		}

		#pragma endregion

		//---------------------------------------------------------------------
		// Member Methods: Rotation
		//---------------------------------------------------------------------
		#pragma region

		/**
		 Sets the x-value of the rotation component of this transform to the
		 given value.

		 @param[in]		x
						The x-value of the rotation component.
		 */
		void SetRotationX(F32 x) noexcept {
			m_transform.SetRotationX(x);
			SetDirty();
		}

		/**
		 Sets the y-value of the rotation component of this transform to the
		 given value.

		 @param[in]		y
						The y-value of the rotation component.
		 */
		void SetRotationY(F32 y) noexcept {
			m_transform.SetRotationY(y);
			SetDirty();
		}

		/**
		 Sets the z-value of the rotation component of this transform to the
		 given value.

		 @param[in]		z
						The z-value of the rotation component.
		 */
		void SetRotationZ(F32 z) noexcept {
			m_transform.SetRotationZ(z);
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
			m_transform.SetRotation(x, y, z);
			SetDirty();
		}

		/**
		 Sets the rotation component of this transform to the given rotation
		 component.

		 @param[in]		rotation
						A reference to the rotation component.
		 */
		void SetRotation(const F32x3& rotation) noexcept {
			m_transform.SetRotation(rotation);
			SetDirty();
		}

		/**
		 Sets the rotation component of this transform to the given rotation
		 component.

		 @param[in]		rotation
						The rotation component.
		 */
		void XM_CALLCONV SetRotation(FXMVECTOR rotation) noexcept {
			m_transform.SetRotation(rotation);
			SetDirty();
		}

		/**
		 Sets the rotation component of this transform to a rotation of the
		 given angle around the given direction.

		 @param[in]		direction
						The unit-length direction.
		 @param[in]		angle
						The angle.
		 */
		void XM_CALLCONV SetRotationAroundDirection(FXMVECTOR direction,
			                                        F32 angle) noexcept {

			m_transform.SetRotationAroundDirection(direction, angle);
			SetDirty();
		}

		/**
		 Adds the given x-value to the rotation component of this transform.

		 @param[in]		x
						The x-value of the rotation component to add.
		 */
		void AddRotationX(F32 x) noexcept {
			m_transform.AddRotationX(x);
			SetDirty();
		}

		/**
		 Adds the given x-value to the rotation component of this transform.

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
		void AddRotationX(F32 x, F32 min_angle, F32 max_angle) noexcept {
			m_transform.AddRotationX(x, min_angle, max_angle);
			SetDirty();
		}

		/**
		 Adds the given y-value to the rotation component of this transform.

		 @param[in]		y
						The y-value of the rotation component to add.
		 */
		void AddRotationY(F32 y) noexcept {
			m_transform.AddRotationY(y);
			SetDirty();
		}

		/**
		 Adds the given y-value to the rotation component of this transform.

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
		void AddRotationY(F32 y, F32 min_angle, F32 max_angle) noexcept {
			m_transform.AddRotationY(y, min_angle, max_angle);
			SetDirty();
		}

		/**
		 Adds the given z-value to the rotation component of this transform.

		 @param[in]		z
						The z-value of the rotation component to add.
		 */
		void AddRotationZ(F32 z) noexcept {
			m_transform.AddRotationZ(z);
			SetDirty();
		}

		/**
		 Adds the given z-value to the rotation component of this transform.

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
		void AddRotationZ(F32 z, F32 min_angle, F32 max_angle) noexcept {
			m_transform.AddRotationZ(z, min_angle, max_angle);
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
			m_transform.AddRotation(x, y, z);
			SetDirty();
		}

		/**
		 Adds the given rotation component to the rotation component of this
		 transform.

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
		void AddRotation(F32 x, F32 y, F32 z, F32 min_angle, F32 max_angle) noexcept {
			m_transform.AddRotation(x, y, z, min_angle, max_angle);
			SetDirty();
		}

		/**
		 Adds the given rotation component to the rotation component of this
		 transform.

		 @param[in]		rotation
						A reference to the rotation component to add.
		 */
		void AddRotation(const F32x3& rotation) noexcept {
			m_transform.AddRotation(rotation);
			SetDirty();
		}

		/**
		 Adds the given rotation component to the rotation component of this
		 transform.

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
		void AddRotation(const F32x3& rotation, F32 min_angle, F32 max_angle) noexcept {
			m_transform.AddRotation(rotation, min_angle, max_angle);
			SetDirty();
		}

		/**
		 Adds the given rotation component to the rotation component of this
		 transform.

		 @param[in]		rotation
						The rotation component to add.
		 */
		void XM_CALLCONV AddRotation(FXMVECTOR rotation) noexcept {
			m_transform.AddRotation(rotation);
			SetDirty();
		}

		/**
		 Adds the given rotation component to the rotation component of this
		 transform.

		 @pre			@a min_angles lie in [-pi, pi].
		 @pre			@a max_angles lie in [-pi, pi].
		 @pre			@a min_angles is not greater than @a max_angles.
		 @param[in]		rotation
						The rotation component to add.
		 @param[in]		min_angles
						The minimum angles (in radians).
		 @param[in]		max_angles
						The maximum angles (in radians).
		 */
		void XM_CALLCONV AddRotation(FXMVECTOR rotation,
									 FXMVECTOR min_angles,
									 FXMVECTOR max_angles) noexcept {

			m_transform.AddRotation(rotation, min_angles, max_angles);
			SetDirty();
		}

		/**
		 Returns the x-value of the rotation component of this transform.

		 @return		The x-value of the rotation component of this transform.
		 */
		[[nodiscard]]
		F32 GetRotationX() const noexcept {
			return m_transform.GetRotationX();
		}

		/**
		 Returns the y-value of the rotation component of this transform.

		 @return		The y-value of the rotation component of this transform.
		 */
		[[nodiscard]]
		F32 GetRotationY() const noexcept {
			return m_transform.GetRotationY();
		}

		/**
		 Returns the z-value of the rotation component of this transform.

		 @return		The z-value of the rotation component of this transform.
		 */
		[[nodiscard]]
		F32 GetRotationZ() const noexcept {
			return m_transform.GetRotationZ();
		}

		/**
		 Returns the rotation component of this transform.

		 @return		The rotation component of this transform.
		 */
		[[nodiscard]]
		const F32x3 GetRotationView() const noexcept {
			return m_transform.GetRotationView();
		}

		/**
		 Returns the rotation component of this transform.

		 @return		The rotation component of this transform.
		 */
		[[nodiscard]]
		const XMVECTOR XM_CALLCONV GetRotation() const noexcept {
			return m_transform.GetRotation();
		}

		/**
		 Returns the object-to-parent rotation quaternion of this transform.

		 @return		The object-to-parent rotation quaternion of this
						transform.
		 */
		[[nodiscard]]
		const XMVECTOR XM_CALLCONV
			GetObjectToParentRotationQuaternion() const noexcept {

			return m_transform.GetObjectToParentRotationQuaternion();
		}

		/**
		 Returns the parent-to-object rotation quaternion of this transform.

		 @return		The parent-to-object rotation quaternion of this
						transform.
		 */
		[[nodiscard]]
		const XMVECTOR XM_CALLCONV
			GetParentToObjectRotationQuaternion() const noexcept {

			return m_transform.GetParentToObjectRotationQuaternion();
		}

		/**
		 Returns the object-to-parent rotation matrix of this transform.

		 @return		The object-to-parent rotation matrix of this transform.
		 */
		[[nodiscard]]
		const XMMATRIX XM_CALLCONV
			GetObjectToParentRotationMatrix() const noexcept {

			return m_transform.GetObjectToParentRotationMatrix();
		}

		/**
		 Returns the parent-to-object rotation matrix of this transform.

		 @return		The parent-to-object rotation matrix of this transform.
		 */
		[[nodiscard]]
		const XMMATRIX XM_CALLCONV
			GetParentToObjectRotationMatrix() const noexcept {

			return m_transform.GetParentToObjectRotationMatrix();
		}

		#pragma endregion

		//---------------------------------------------------------------------
		// Member Methods: Scale
		//---------------------------------------------------------------------
		#pragma region

		/**
		 Sets the x-value of the scale component of this transform to the given
		 value.

		 @param[in]		x
						The x-value of the scale component.
		 */
		void SetScaleX(F32 x) noexcept {
			m_transform.SetScaleX(x);
			SetDirty();
		}

		/**
		 Sets the y-value of the scale component of this transform to the given
		 value.

		 @param[in]		y
						The y-value of the scale component.
		 */
		void SetScaleY(F32 y) noexcept {
			m_transform.SetScaleY(y);
			SetDirty();
		}

		/**
		 Sets the z-value of the scale component of this transform to the given
		 value.

		 @param[in]		z
						The z-value of the scale component.
		 */
		void SetScaleZ(F32 z) noexcept {
			m_transform.SetScaleZ(z);
			SetDirty();
		}

		/**
		 Sets the scale component of this transform to the given scale
		 component.

		 @param[in]		s
						The scale component.
		 */
		void SetScale(F32 s) noexcept {
			m_transform.SetScale(s);
			SetDirty();
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
			m_transform.SetScale(x, y, z);
			SetDirty();
		}

		/**
		 Sets the scale component of this transform to the given scale
		 component.

		 @param[in]		scale
						The scale component.
		 */
		void SetScale(F32x3 scale) noexcept {
			m_transform.SetScale(std::move(scale));
			SetDirty();
		}

		/**
		 Sets the scale component of this transform to the given scale
		 component.

		 @param[in]		scale
						The scale component.
		 */
		void XM_CALLCONV SetScale(FXMVECTOR scale) noexcept {
			m_transform.SetScale(scale);
			SetDirty();
		}

		/**
		 Adds the given x-value to the scale component of this transform.

		 @param[in]		x
						The x-value of the scale component to add.
		 */
		void AddScaleX(F32 x) noexcept {
			m_transform.AddScaleX(x);
			SetDirty();
		}

		/**
		 Adds the given y-value to the scale component of this transform.

		 @param[in]		y
						The y-value of the scale component to add.
		 */
		void AddScaleY(F32 y) noexcept {
			m_transform.AddScaleY(y);
			SetDirty();
		}

		/**
		 Adds the given z-value to the scale component of this transform.

		 @param[in]		z
						The z-value of the scale component to add.
		 */
		void AddScaleZ(F32 z) noexcept {
			m_transform.AddScaleZ(z);
			SetDirty();
		}

		/**
		 Adds the given scale component to the scale component of this transform.

		 @param[in]		s
						The scale component to add.
		 */
		void AddScale(F32 s) noexcept {
			m_transform.AddScale(s);
			SetDirty();
		}

		/**
		 Adds the given scale component to the scale component of this transform.

		 @param[in]		x
						The x-value of the scale component to add.
		 @param[in]		y
						The y-value of the scale component to add.
		 @param[in]		z
						The z-value of the scale component to add.
		 */
		void AddScale(F32 x, F32 y, F32 z) noexcept {
			m_transform.AddScale(x, y, z);
			SetDirty();
		}

		/**
		 Adds the given scale component to the scale component of this transform.

		 @param[in]		scale
						A reference to the scale component to add.
		 */
		void AddScale(const F32x3& scale) noexcept {
			m_transform.AddScale(scale);
			SetDirty();
		}

		/**
		 Adds the given scale component to the scale component of this transform.

		 @param[in]		scale
						The scale component to add.
		 */
		void XM_CALLCONV AddScale(FXMVECTOR scale) noexcept {
			m_transform.AddScale(scale);
			SetDirty();
		}

		/**
		 Returns the x-value of the scale component of this transform.

		 @return		The x-value of the scale component of this transform.
		 */
		[[nodiscard]]
		F32 GetScaleX() const noexcept {
			return m_transform.GetScaleX();
		}

		/**
		 Returns the y-value of the scale component of this transform.

		 @return		The y-value of the scale component of this transform.
		 */
		[[nodiscard]]
		F32 GetScaleY() const noexcept {
			return m_transform.GetScaleY();
		}

		/**
		 Returns the z-value of the scale component of this transform.

		 @return		The z-value of the scale component of this transform.
		 */
		[[nodiscard]]
		F32 GetScaleZ() const noexcept {
			return m_transform.GetScaleZ();
		}

		/**
		 Returns the scale component of this transform.

		 @return		The scale component of this transform.
		 */
		[[nodiscard]]
		const F32x3 GetScaleView() const noexcept {
			return m_transform.GetScaleView();
		}

		/**
		 Returns the scale component of this transform.

		 @return		The scale component of this transform.
		 */
		[[nodiscard]]
		const XMVECTOR XM_CALLCONV GetScale() const noexcept {
			return m_transform.GetScale();
		}

		/**
		 Returns the object-to-parent scale matrix of this transform.

		 @return		The scale object-to-parent matrix of this transform.
		 */
		[[nodiscard]]
		const XMMATRIX XM_CALLCONV
			GetObjectToParentScaleMatrix() const noexcept {

			return m_transform.GetObjectToParentScaleMatrix();
		}

		/**
		 Returns the parent-to-object scale matrix of this transform.

		 @return		The parent-to-object scale matrix of this transform.
		 */
		[[nodiscard]]
		const XMMATRIX XM_CALLCONV
			GetParentToObjectScaleMatrix() const noexcept {

			return m_transform.GetParentToObjectScaleMatrix();
		}

		#pragma endregion

		//---------------------------------------------------------------------
		// Member Methods: Object Space
		//---------------------------------------------------------------------
		#pragma region

		/**
		 Returns the position of the local origin of this transform expressed
		 in object space coordinates.

		 @return		The position of the local origin of this transform
						expressed in object space coordinates.
		 */
		[[nodiscard]]
		const XMVECTOR XM_CALLCONV GetObjectOrigin() const noexcept {
			return m_transform.GetObjectOrigin();
		}

		/**
		 Returns the direction of the local x-axis of this transform expressed
		 in object space coordinates.

		 @return		The direction of the local x-axis of this transform
						expressed in object space coordinates.
		 */
		[[nodiscard]]
		const XMVECTOR XM_CALLCONV GetObjectAxisX() const noexcept {
			return m_transform.GetObjectAxisX();
		}

		/**
		 Returns the direction of the local y-axis of this transform expressed
		 in object space coordinates.

		 @return		The direction of the local y-axis of this transform
						expressed in object space coordinates.
		 */
		[[nodiscard]]
		const XMVECTOR XM_CALLCONV GetObjectAxisY() const noexcept {
			return m_transform.GetObjectAxisY();
		}

		/**
		 Returns the direction of the local z-axis of this transform expressed
		 in object space coordinates.

		 @return		The direction of the local z-axis of this transform
						expressed in object space coordinates.
		 */
		[[nodiscard]]
		const XMVECTOR XM_CALLCONV GetObjectAxisZ() const noexcept {
			return m_transform.GetObjectAxisZ();
		}

		#pragma endregion

		//---------------------------------------------------------------------
		// Member Methods: Parent Space
		//---------------------------------------------------------------------
		#pragma region

		/**
		 Returns the position of the local origin of this transform expressed
		 in parent space coordinates.

		 @return		The position of the local origin of this transform
						expressed in parent space coordinates.
		 */
		[[nodiscard]]
		const XMVECTOR XM_CALLCONV GetParentOrigin() const noexcept {
			return m_transform.GetParentOrigin();
		}

		/**
		 Returns the direction of the local x-axis of this transform expressed
		 in parent space coordinates.

		 @return		The direction of the local x-axis of this transform
						expressed in parent space coordinates.
		 */
		[[nodiscard]]
		const XMVECTOR XM_CALLCONV GetParentAxisX() const noexcept {
			return m_transform.GetParentAxisX();
		}

		/**
		 Returns the direction of the local y-axis of this transform expressed
		 in parent space coordinates.

		 @return		The direction of the local y-axis of this transform
						expressed in parent space coordinates.
		 */
		[[nodiscard]]
		const XMVECTOR XM_CALLCONV GetParentAxisY() const noexcept {
			return m_transform.GetParentAxisY();
		}

		/**
		 Returns the direction of the local z-axis of this transform expressed
		 in parent space coordinates.

		 @return		The direction of the local z-axis of this transform
						expressed in parent space coordinates.
		 */
		[[nodiscard]]
		const XMVECTOR XM_CALLCONV GetParentAxisZ() const noexcept {
			return m_transform.GetParentAxisZ();
		}

		#pragma endregion

		//---------------------------------------------------------------------
		// Member Methods: World Space
		//---------------------------------------------------------------------
		#pragma region

		/**
		 Returns the position of the local origin of this transform expressed
		 in world space coordinates.

		 @return		The position of the local origin of this transform
						expressed in world space coordinates.
		 */
		[[nodiscard]]
		const XMVECTOR XM_CALLCONV GetWorldOrigin() const noexcept {
			return GetObjectToWorldMatrix().r[3];
		}

		/**
		 Returns the direction of the local x-axis of this transform expressed
		 in world space coordinates.

		 @return		The direction of the local x-axis of this transform
						expressed in world space coordinates.
		 */
		[[nodiscard]]
		const XMVECTOR XM_CALLCONV GetWorldAxisX() const noexcept {
			return XMVector3Normalize(GetObjectToWorldMatrix().r[0]);
		}

		/**
		 Returns the direction of the local y-axis of this transform expressed
		 in world space coordinates.

		 @return		The direction of the local y-axis of this transform
						expressed in world space coordinates.
		 */
		[[nodiscard]]
		const XMVECTOR XM_CALLCONV GetWorldAxisY() const noexcept {
			return XMVector3Normalize(GetObjectToWorldMatrix().r[1]);
		}

		/**
		 Returns the direction of the local z-axis of this transform expressed
		 in world space coordinates.

		 @return		The direction of the local z-axis of this transform
						expressed in world space coordinates.
		 */
		[[nodiscard]]
		const XMVECTOR XM_CALLCONV GetWorldAxisZ() const noexcept {
			return XMVector3Normalize(GetObjectToWorldMatrix().r[2]);
		}

		#pragma endregion

		//---------------------------------------------------------------------
		// Member Methods: Transformation
		//---------------------------------------------------------------------
		#pragma region

		/**
		 Sets the local transform of this transform to the given local
		 transform.

		 @param[in]		transform
						A reference to the local transform.
		 */
		void SetLocalTransform(const SETTransform3D& transform) noexcept {
			m_transform = transform;
			SetDirty();
		}

		/**
		 Returns the object-to-parent matrix of this transform.

		 @return		The object-to-parent matrix of this transform.
		 */
		[[nodiscard]]
		const XMMATRIX XM_CALLCONV GetObjectToParentMatrix() const noexcept {
			return m_transform.GetObjectToParentMatrix();
		}

		/**
		 Returns the parent-to-object matrix of this transform.

		 @return		The parent-to-object matrix of this transform.
		 */
		[[nodiscard]]
		const XMMATRIX XM_CALLCONV GetParentToObjectMatrix() const noexcept {
			return m_transform.GetParentToObjectMatrix();
		}

		/**
		 Returns the object-to-world matrix of this transform.

		 @return		The object-to-world matrix of this transform.
		 */
		[[nodiscard]]
		const XMMATRIX XM_CALLCONV GetObjectToWorldMatrix() const noexcept {
			UpdateObjectToWorldMatrix();
			return m_object_to_world;
		}

		/**
		 Returns the world-to-object matrix of this transform.

		 @return		The world-to-object matrix of this transform.
		 */
		[[nodiscard]]
		const XMMATRIX XM_CALLCONV GetWorldToObjectMatrix() const noexcept {
			UpdateWorldToObjectMatrix();
			return m_world_to_object;
		}

		/**
		 Transforms the given vector expressed in object space coordinates
		 to parent space coordinates.

		 @param[in]		vector
						The vector expressed in object space coordinates.
		 @return		The transformed vector expressed in parent space
						coordinates.
		 */
		[[nodiscard]]
		const XMVECTOR XM_CALLCONV
			TransformObjectToParent(FXMVECTOR vector) const noexcept {

			return m_transform.TransformObjectToParent(vector);
		}

		/**
		 Transforms the given point expressed in object space coordinates
		 to parent space coordinates.

		 @param[in]		point
						The point expressed in object space coordinates.
		 @return		The transformed point expressed in parent space
						coordinates.
		 */
		[[nodiscard]]
		const XMVECTOR XM_CALLCONV
			TransformObjectToParentPoint(FXMVECTOR point) const noexcept {

			return m_transform.TransformObjectToParentPoint(point);
		}

		/**
		 Transforms the given direction expressed in object space coordinates
		 to parent space coordinates.

		 @param[in]		direction
						The direction expressed in object space coordinates.
		 @return		The transformed direction expressed in parent space
						coordinates.
		 */
		[[nodiscard]]
		const XMVECTOR XM_CALLCONV
			TransformObjectToParentDirection(FXMVECTOR direction) const noexcept {

			return m_transform.TransformObjectToParentDirection(direction);
		}

		/**
		 Transforms the given vector expressed in parent space coordinates
		 to object space coordinates.

		 @param[in]		vector
						The vector expressed in parent space coordinates.
		 @return		The transformed vector expressed in object space
						coordinates.
		 */
		[[nodiscard]]
		const XMVECTOR XM_CALLCONV
			TransformParentToObject(FXMVECTOR vector) const noexcept {

			return m_transform.TransformParentToObject(vector);
		}

		/**
		 Transforms the given point expressed in parent space coordinates
		 to object space coordinates.

		 @param[in]		point
						The point expressed in parent space coordinates.
		 @return		The transformed point expressed in object space
						coordinates.
		 */
		[[nodiscard]]
		const XMVECTOR XM_CALLCONV
			TransformParentToObjectPoint(FXMVECTOR point) const noexcept {

			return m_transform.TransformParentToObjectPoint(point);
		}

		/**
		 Transforms the given direction expressed in parent space coordinates
		 to object space coordinates.

		 @param[in]		direction
						The direction expressed in parent space coordinates.
		 @return		The transformed direction expressed in object space
						coordinates.
		 */
		[[nodiscard]]
		const XMVECTOR XM_CALLCONV
			TransformParentToObjectDirection(FXMVECTOR direction) const noexcept {

			return m_transform.TransformParentToObjectDirection(direction);
		}

		/**
		 Transforms the given vector expressed in object space coordinates
		 to world space coordinates.

		 @param[in]		vector
						The vector expressed in object space coordinates.
		 @return		The transformed vector expressed in world space
						coordinates.
		 */
		[[nodiscard]]
		const XMVECTOR XM_CALLCONV
			TransformObjectToWorld(FXMVECTOR vector) const noexcept {

			return XMVector4Transform(vector, GetObjectToWorldMatrix());
		}

		/**
		 Transforms the given point expressed in object space coordinates
		 to world space coordinates.

		 @param[in]		point
						The point expressed in object space coordinates.
		 @return		The transformed point expressed in world space
						coordinates.
		 */
		[[nodiscard]]
		const XMVECTOR XM_CALLCONV
			TransformObjectToWorldPoint(FXMVECTOR point) const noexcept {

			return XMVector3TransformCoord(point, GetObjectToWorldMatrix());
		}

		/**
		 Transforms the given direction expressed in object space coordinates
		 to world space coordinates.

		 @param[in]		direction
						The direction expressed in object space coordinates.
		 @return		The transformed direction expressed in world space
						coordinates.
		 */
		[[nodiscard]]
		const XMVECTOR XM_CALLCONV
			TransformObjectToWorldDirection(FXMVECTOR direction) const noexcept {

			return XMVector3TransformNormal(direction, GetObjectToWorldMatrix());
		}

		/**
		 Transforms the given vector expressed in world space coordinates
		 to object space coordinates.

		 @param[in]		vector
						The vector expressed in world space coordinates.
		 @return		The transformed vector expressed in object space
						coordinates.
		 */
		[[nodiscard]]
		const XMVECTOR XM_CALLCONV
			TransformWorldToObject(FXMVECTOR vector) const noexcept {

			return XMVector4Transform(vector, GetWorldToObjectMatrix());
		}

		/**
		 Transforms the given point expressed in world space coordinates
		 to object space coordinates.

		 @param[in]		point
						The point expressed in world space coordinates.
		 @return		The transformed point expressed in object space
						coordinates.
		 */
		[[nodiscard]]
		const XMVECTOR XM_CALLCONV
			TransformWorldToObjectPoint(FXMVECTOR point) const noexcept {

			return XMVector3TransformCoord(point, GetWorldToObjectMatrix());
		}

		/**
		 Transforms the direction vector expressed in world space coordinates
		 to object space coordinates.

		 @param[in]		direction
						The direction expressed in world space coordinates.
		 @return		The transformed direction expressed in object space
						coordinates.
		 */
		[[nodiscard]]
		const XMVECTOR XM_CALLCONV
			TransformWorldToObjectDirection(FXMVECTOR direction) const noexcept {

			return XMVector3TransformNormal(direction, GetWorldToObjectMatrix());
		}

		#pragma endregion

		//---------------------------------------------------------------------
		// Member Methods: Identification
		//---------------------------------------------------------------------
		#pragma region

		/**
		 Checks whether this transform has an owner.

		 @return		@c true if this transform has an owner. @c false
						otherwise.
		 */
		[[nodiscard]]
		bool HasOwner() const noexcept {
			return bool(m_owner);
		}

		/**
		 Returns the owner of this transform.

		 @return		A pointer to the owner of this transform.
		 */
		[[nodiscard]]
		ProxyPtr< Node > GetOwner() noexcept {
			return m_owner;
		}

		/**
		 Returns the owner of this transform.

		 @return		A pointer to the owner of this transform.
		 */
		[[nodiscard]]
		ProxyPtr< const Node > GetOwner() const noexcept {
			return m_owner;
		}

		#pragma endregion

		//---------------------------------------------------------------------
		// Member Methods: Update
		//---------------------------------------------------------------------
		#pragma region

		/**
		 Sets this transform to dirty.
		 */
		void SetDirty() const noexcept;

		#pragma endregion

	private:

		//---------------------------------------------------------------------
		// Friends
		//---------------------------------------------------------------------

		friend class TransformClient;

		//---------------------------------------------------------------------
		// Member Methods: Update
		//---------------------------------------------------------------------
		#pragma region

		/**
		 Sets the owner of this transform to the given owner.

		 @param[in]		owner
						A pointer to the owner.
		 */
		void SetOwner(ProxyPtr< Node > owner) noexcept {
			m_owner = std::move(owner);
			SetDirty();
		}

		/**
		 Updates the object-to-world matrix of this transform if dirty.

		 @pre			This transform must have an owner.
		 */
		void UpdateObjectToWorldMatrix() const noexcept;

		/**
		 Updates the world-to-object matrix of this transform if dirty.

		 @pre			This transform must have an owner.
		 */
		void UpdateWorldToObjectMatrix() const noexcept;

		#pragma endregion

		//---------------------------------------------------------------------
		// Member Variables
		//---------------------------------------------------------------------

		/**
		 The local transform of this transform.
		 */
		SETTransform3D m_transform;

		/**
		 The cached object-to-world matrix of this transform.
		 */
		mutable XMMATRIX m_object_to_world;

		/**
		 The cached world-to-object matrix of this transform.
		 */
		mutable XMMATRIX m_world_to_object;

		/**
		 A flag indicating whether the object-to-world matrix of this transform
		 is dirty.
		 */
		mutable bool m_dirty_object_to_world;

		/**
		 A flag indicating whether the world-to-object matrix of this transform
		 is dirty.
		 */
		mutable bool m_dirty_world_to_object;

		/**
		 A pointer to the node owning this transform.
		 */
		ProxyPtr< Node > m_owner;
	};

	#pragma endregion

	//-------------------------------------------------------------------------
	// TransformClient
	//-------------------------------------------------------------------------
	#pragma region

	/**
	 A class of transform clients.
	 */
	class TransformClient {

	private:

		//---------------------------------------------------------------------
		// Friends
		//---------------------------------------------------------------------

		friend class Node;

		//---------------------------------------------------------------------
		// Static Member Methods
		//---------------------------------------------------------------------

		/**
		 Sets the owner of the given transform to the given owner.

		 @param[in]		transform
						A reference to the transform.
		 @param[in]		owner
						A pointer to the owner.
		 */
		static void SetOwner(Transform& transform,
							 ProxyPtr< Node > owner) noexcept {

			transform.SetOwner(std::move(owner));
		}
	};

	#pragma endregion
}