#pragma once

//-----------------------------------------------------------------------------
// Engine Includes
//-----------------------------------------------------------------------------
#pragma region

#include "scene\component.hpp"
#include "spectrum\spectrum.hpp"
#include "geometry\bounding_volume.hpp"

#pragma endregion

//-----------------------------------------------------------------------------
// Engine Declarations and Definitions
//-----------------------------------------------------------------------------
namespace mage::rendering {

	#pragma warning( push )
	#pragma warning( disable : 4324 ) // Added padding.

	/**
	 A class of omni lights.
	 */
	class alignas(16) OmniLight : public Component {

	public:

		//---------------------------------------------------------------------
		// Constructors and Destructors
		//---------------------------------------------------------------------

		/**
		 Constructs an omni light.
		 */
		OmniLight() noexcept;

		/**
		 Constructs an omni light from the given omni light.

		 @param[in]		light
						A reference to the omni light to copy.
		 */
		OmniLight(const OmniLight& light) noexcept;

		/**
		 Constructs an omni light by moving the given omni light.

		 @param[in]		light
						A reference to the omni light to move.
		 */
		OmniLight(OmniLight&& light) noexcept;

		/**
		 Destructs this omni light.
		 */
		virtual ~OmniLight();

		//---------------------------------------------------------------------
		// Assignment Operators
		//---------------------------------------------------------------------

		/**
		 Copies the given omni light to this omni light.

		 @param[in]		light
						A reference to the omni light to copy.
		 @return		A reference to the copy of the given omni light (i.e.
						this omni light).
		 */
		OmniLight& operator=(const OmniLight& light) noexcept;

		/**
		 Moves the given omni light to this omni light.

		 @param[in]		light
						A reference to the omni light to move.
		 @return		A reference to the moved omni light (i.e. this omni
						light).
		 */
		OmniLight& operator=(OmniLight&& light) noexcept;

		//---------------------------------------------------------------------
		// Member Methods: Lighting
		//---------------------------------------------------------------------

		/**
		 Returns the (linear) RGB base color of this spotlight.

		 @return		A reference to the (linear) base color of this
						spotlight.
		 */
		[[nodiscard]]
		RGB& GetBaseColor() noexcept {
			return m_base_color;
		}

		/**
		 Returns the (linear) base color of this spotlight.

		 @return		A reference to the (linear) base color of this
						spotlight.
		 */
		[[nodiscard]]
		const RGB& GetBaseColor() const noexcept {
			return m_base_color;
		}

		/**
		 Returns the power of this omni light.

		 @return		The power in watts of this omni light.
		 */
		[[nodiscard]]
		F32 GetPower() const noexcept {
			return GetIntensity() * 0.25f * XM_1DIVPI;
		}

		/**
		 Sets the power of this omni light to the given radiance.

		 @param[in]		power
						The power in watts.
		 */
		void SetPower(F32 power) noexcept {
			SetIntensity(power * 4.0f * XM_PI);
		}

		/**
		 Returns the power spectrum of this omni light.

		 @return		The power spectrum of this omni light.
		 */
		[[nodiscard]]
		const RGB GetPowerSpectrum() const noexcept {
			const auto P = GetPower() * XMLoad(m_base_color);
			return RGB(XMStore< F32x3 >(P));
		}

		/**
		 Returns the radiant intensity of this omni light.

		 @return		The radiant intensity in watts per steradians of this
						omni light.
		 */
		[[nodiscard]]
		F32 GetIntensity() const noexcept {
			return m_intensity;
		}

		/**
		 Sets the radiant intensity of this omni light to the given radial
		 intensity.

		 @param[in]		intensity
						The radiant intensity in watts per steradians.
		 */
		void SetIntensity(F32 intensity) noexcept {
			m_intensity = std::abs(intensity);
		}

		/**
		 Returns the radiant intensity spectrum of this omni light.

		 @return		The radiant intensity spectrum of this omni light.
		 */
		[[nodiscard]]
		const RGB GetIntensitySpectrum() const noexcept {
			const auto I = GetIntensity() * XMLoad(m_base_color);
			return RGB(XMStore< F32x3 >(I));
		}

		//---------------------------------------------------------------------
		// Member Methods: Range and Attenuation
		//---------------------------------------------------------------------

		/**
		 Returns the AABB of this omni light.

		 @return		A reference to the AABB of this omni light.
		 */
		[[nodiscard]]
		const AABB& GetAABB() const noexcept {
			return m_aabb;
		}

		/**
		 Returns the BoundingSphere of this omni light.

		 @return		A reference to the BoundingSphere of this omni light.
		 */
		[[nodiscard]]
		const BoundingSphere& GetBoundingSphere() const noexcept {
			return m_sphere;
		}

		/**
		 Returns the range of this omni light expressed in light space.

		 @return		The range of this omni light expressed in light space.
		 */
		[[nodiscard]]
		F32 GetRange() const noexcept {
			return m_clipping_planes[1u];
		}

		/**
		 Returns the range of this omni light expressed in world space.

		 @return		The range of this omni light expressed in world space.
		 @note			Non-uniform scaling is not supported for omni lights.
		 */
		[[nodiscard]]
		F32 GetWorldRange() const noexcept;

		/**
		 Sets the range of this omni light to the given value expressed in
		 light space.

		 @param[in]		range
						The range expressed in light space.
		 */
		void SetRange(F32 range) noexcept {
			m_clipping_planes[1u] = range;

			// Update the bounding volumes.
			UpdateBoundingVolumes();
		}

		//---------------------------------------------------------------------
		// Member Methods: Shadowing
		//---------------------------------------------------------------------

		/**
		 Checks whether shadows should be used for this omni light.

		 @return		@c true if shadows should be used for this omni light.
						@c false otherwise.
		 */
		[[nodiscard]]
		bool UseShadows() const noexcept {
			return m_shadows;
		}

		/**
		 Enables shadows for this omni light.
		 */
		void EnableShadows() noexcept {
			SetShadows(true);
		}

		/**
		 Dissables shadows for this omni light.
		 */
		void DissableShadows() noexcept {
			SetShadows(false);
		}

		/**
		 Toggles shadows for this omni light.
		 */
		void ToggleShadows() noexcept {
			SetShadows(!m_shadows);
		}

		/**
		 Sets shadows for this omni light to the given value.

		 @param[in]		shadows
						@c true if shadows should be used for this omni light.
						@c false otherwise.
		 */
		void SetShadows(bool shadows) noexcept {
			m_shadows = shadows;
		}

		/**
		 Returns the clipping planes of this omni light expressed in light
		 space.

		 @return		The clipping planes of this omni light expressed in
						light space.
		 */
		[[nodiscard]]
		const F32x2 GetClippingPlanes() const noexcept {
			return m_clipping_planes;
		}

		/**
		 Sets the clipping planes of this omni light expressed in light space
		 to the given clipping planes.

		 @param[in]		clipping_planes
						A reference to the clipping planes.
		 */
		void SetClippingPlanes(const F32x2& clipping_planes) noexcept {
			m_clipping_planes = clipping_planes;

			// Update the bounding volumes.
			UpdateBoundingVolumes();
		}

		/**
		 Returns the light-to-projection matrix of the (forward) light camera
		 of this omni light.

		 @return		The light-to-projection matrix of the (forward) light
						camera of this omni light.
		 */
		[[nodiscard]]
		const XMMATRIX XM_CALLCONV
			GetLightToProjectionMatrix() const noexcept {

			#ifdef DISABLE_INVERTED_Z_BUFFER
			const auto [near_plane, far_plane] = m_clipping_planes;
			#else  // DISABLE_INVERTED_Z_BUFFER
			const auto [far_plane, near_plane] = m_clipping_planes;
			#endif // DISABLE_INVERTED_Z_BUFFER

			const auto m22 = far_plane / (far_plane - near_plane);
			const auto m32 = -near_plane * m22;

			return {
				1.0f, 0.0f, 0.0f, 0.0f,
				0.0f, 1.0f, 0.0f, 0.0f,
				0.0f, 0.0f,  m22, 1.0f,
				0.0f, 0.0f,  m32, 0.0f
			};
		}

	private:

		//---------------------------------------------------------------------
		// Member Methods
		//---------------------------------------------------------------------

		/**
		 Updates the bounding volumes of this omni light.
		 */
		void UpdateBoundingVolumes() noexcept;

		//---------------------------------------------------------------------
		// Member Variables
		//---------------------------------------------------------------------

		/**
		 A flag indicating whether shadows should be calculated or not for
		 this omni light.
		 */
		bool m_shadows;

		/**
		 The clipping planes of this omni light expressed in light space.
		 */
		F32x2 m_clipping_planes;

		/**
		 The AABB of this omni light.
		 */
		AABB m_aabb;

		/**
		 The BoundingSphere of this omni light.
		 */
		BoundingSphere m_sphere;

		/**
		 The (linear) base color of this omni light.
		 */
		RGB m_base_color;

		/**
		 The radiant intensity in watts per steradians of this omni light.
		 */
		F32 m_intensity;
	};

	#pragma warning( pop )
}
