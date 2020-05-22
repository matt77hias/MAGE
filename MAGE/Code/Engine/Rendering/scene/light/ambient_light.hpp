#pragma once

//-----------------------------------------------------------------------------
// Engine Includes
//-----------------------------------------------------------------------------
#pragma region

#include "scene\component.hpp"
#include "spectrum\spectrum.hpp"

#pragma endregion

//-----------------------------------------------------------------------------
// Engine Declarations and Definitions
//-----------------------------------------------------------------------------
namespace mage::rendering {

	#pragma warning( push )
	#pragma warning( disable : 4324 ) // Added padding.

	/**
	 A class of ambient lights.
	 */
	class alignas(16) AmbientLight : public Component {

	public:

		//---------------------------------------------------------------------
		// Constructors and Destructors
		//---------------------------------------------------------------------

		/**
		 Constructs an ambient light.
		 */
		AmbientLight() noexcept;

		/**
		 Constructs an ambient light from the given ambient light.

		 @param[in]		light
						A reference to the ambient light to copy.
		 */
		AmbientLight(const AmbientLight& light) noexcept;

		/**
		 Constructs an ambient light by moving the given ambient light.

		 @param[in]		light
						A reference to the ambient light to move.
		 */
		AmbientLight(AmbientLight&& light) noexcept;

		/**
		 Destructs this ambient light.
		 */
		virtual ~AmbientLight();

		//---------------------------------------------------------------------
		// Assignment Operators
		//---------------------------------------------------------------------

		/**
		 Copies the given ambient light to this ambient light.

		 @param[in]		light
						A reference to the ambient light to copy.
		 @return		A reference to the copy of the given ambient light
						(i.e. this ambient light).
		 */
		AmbientLight& operator=(const AmbientLight& light) noexcept;

		/**
		 Moves the given ambient light to this ambient light.

		 @param[in]		light
						A reference to the ambient light to move.
		 @return		A reference to the moved ambient light (i.e. this
						ambient light).
		 */
		AmbientLight& operator=(AmbientLight&& light) noexcept;

		//---------------------------------------------------------------------
		// Member Methods: Lighting
		//---------------------------------------------------------------------

		/**
		 Returns the (linear) base color of this ambient light.

		 @return		A reference to the sRGB base color of this ambient
						light.
		 */
		[[nodiscard]]
		RGB& GetBaseColor() noexcept {
			return m_base_color;
		}

		/**
		 Returns the (linear) base color of this ambient light.

		 @return		A reference to the sRGB base color of this ambient
						light.
		 */
		[[nodiscard]]
		const RGB& GetBaseColor() const noexcept {
			return m_base_color;
		}

		/**
		 Returns the radiance of this ambient light.

		 @return		The radiance in watts per square meter per steradians
						of this ambient light.
		 */
		[[nodiscard]]
		F32 GetRadiance() const noexcept {
			return m_radiance;
		}

		/**
		 Sets the radiance of this ambient light to the given radiance.

		 @param[in]		radiance
						The radiance in watts per square meter per steradians.
		 */
		void SetRadiance(F32 radiance) noexcept {
			m_radiance = std::abs(radiance);
		}

		/**
		 Returns the radiance spectrum of this ambient light.

		 @return		The radiance spectrum of this ambient light.
		 */
		[[nodiscard]]
		const RGB GetRadianceSpectrum() const noexcept {
			const auto L = m_radiance * XMLoad(m_base_color);
			return RGB(XMStore< F32x3 >(L));
		}

	private:

		//---------------------------------------------------------------------
		// Member Variables: Lighting
		//---------------------------------------------------------------------

		/**
		 The (linear) base color of this ambient light.
		 */
		RGB m_base_color;

		/**
		 The radiance in watts per square meter per steradians of this ambient
		 light.
		 */
		F32 m_radiance;
	};

	#pragma warning( pop )
}