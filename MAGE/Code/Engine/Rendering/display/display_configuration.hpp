#pragma once

//-----------------------------------------------------------------------------
// Engine Includes
//-----------------------------------------------------------------------------
#pragma region

#include "direct3d11.hpp"
#include "renderer\configuration.hpp"
#include "type\types.hpp"

#pragma endregion

//-----------------------------------------------------------------------------
// Engine Declarations and Definitions
//-----------------------------------------------------------------------------
namespace mage::rendering {

	/**
	 A class of display configurations.
	 */
	class DisplayConfiguration {

	public:

		//---------------------------------------------------------------------
		// Constructors and Destructors
		//---------------------------------------------------------------------

		/**
		 Constructs a display configuration.

		 @pre			@a display_mode must be compatible with @a adapter
						and @a output.
		 @param[in]		adapter
						A pointer to the adapter.
		 @param[in]		output
						A pointer to the output.
		 @param[in]		display_mode
						A reference to the display mode.
		 */
		explicit DisplayConfiguration(ComPtr< DXGIAdapter > adapter,
									  ComPtr< DXGIOutput >  output,
			                          const DXGI_MODE_DESC& display_mode)
			: m_adapter(std::move(adapter)),
			m_output(std::move(output)),
			m_display_mode(display_mode),
			m_aa(AntiAliasing::None),
			m_windowed(true),
			m_vsync(false) {}

		/**
		 Constructs a display configuration from the given display
		 configuration.

		 @param[in]		configuration
						A reference to a display configuration to copy.
		 */
		DisplayConfiguration(
			const DisplayConfiguration& configuration) = default;

		/**
		 Constructs a display configuration by moving the given display
		 configuration.

		 @param[in]		configuration
						A reference to a display configuration to move.
		 */
		DisplayConfiguration(
			DisplayConfiguration&& configuration) noexcept = default;

		/**
		 Destructs this display configuration.
		 */
		~DisplayConfiguration() = default;

		//---------------------------------------------------------------------
		// Assignment Operators
		//---------------------------------------------------------------------

		/**
		 Copies the given display configuration to this display configuration.

		 @param[in]		configuration
						A reference to a display configuration to copy.
		 @return		A reference to the copy of the given display
						configuration (i.e. this display configuration).
		 */
		DisplayConfiguration& operator=(
			const DisplayConfiguration& configuration) = default;

		/**
		 Moves the given display configuration to this display configuration.

		 @param[in]		configuration
						A reference to a display configuration to move.
		 @return		A reference to the moved display configuration (i.e.
						this display configuration).
		 */
		DisplayConfiguration& operator=(
			DisplayConfiguration&& configuration) noexcept = default;

		//---------------------------------------------------------------------
		// Member Methods: Adapter
		//---------------------------------------------------------------------

		/**
		 Returns the adapter of this display configuration.

		 @return		A pointer to the adapter of this display configuration.
		 */
		[[nodiscard]]
		DXGIAdapter* GetAdapter() const noexcept {
			return m_adapter.Get();
		}

		//---------------------------------------------------------------------
		// Member Methods: Output
		//---------------------------------------------------------------------

		/**
		 Returns the output of this display configuration.

		 @return		A pointer to the output of this display configuration.
		 */
		[[nodiscard]]
		DXGIOutput* GetOutput() const noexcept {
			return m_output.Get();
		}

		//---------------------------------------------------------------------
		// Member Methods: Display Resolution
		//---------------------------------------------------------------------

		/**
		 Returns the display width in pixels of this display configuration.

		 @return		The display width in pixels of this display
						configuration.
		 */
		[[nodiscard]]
		U32 GetDisplayWidth() const noexcept {
			return m_display_mode.Width;
		}

		/**
		 Returns the display height in pixels of this display configuration.

		 @return		The display height in pixels of this display
						configuration.
		 */
		[[nodiscard]]
		U32 GetDisplayHeight() const noexcept {
			return m_display_mode.Height;
		}

		/**
		 Returns the display resolution in pixels of this display
		 configuration.

		 @return		The display resolution in pixels of this display
						configuration.
		 */
		[[nodiscard]]
		const U32x2 GetDisplayResolution() const noexcept {
			return { GetDisplayWidth(), GetDisplayHeight() };
		}

		/**
		 Returns the super-sampled display width in pixels of this display
		 configuration.

		 @return		The super-sampled display width in pixels of this
						display configuration.
		 */
		[[nodiscard]]
		U32 GetSSDisplayWidth() const noexcept {
			return GetResolutionMultiplier(m_aa) * GetDisplayWidth();
		}

		/**
		 Returns the super-sampled display height in pixels of this display
		 configuration.

		 @return		The super-sampled display height in pixels of this
						display configuration.
		 */
		[[nodiscard]]
		U32 GetSSDisplayHeight() const noexcept {
			return GetResolutionMultiplier(m_aa) * GetDisplayHeight();
		}

		/**
		 Returns the super-sampled display resolution in pixels of this display
		 configuration.

		 @return		The super-sampled display resolution in pixels of this
						display configuration.
		 */
		[[nodiscard]]
		const U32x2 GetSSDisplayResolution() const noexcept {
			return { GetSSDisplayWidth(), GetSSDisplayHeight() };
		}

		//---------------------------------------------------------------------
		// Member Methods: Display Refresh Rate
		//---------------------------------------------------------------------

		/**
		 Returns the rounded display refresh rate of this display
		 configuration.

		 @return		The rounded display refresh rate of this display
						configuration.
		 */
		[[nodiscard]]
		U32 GetDisplayRoundedRefreshRate() const noexcept {
			const auto n = static_cast< F32 >(m_display_mode.RefreshRate.Numerator);
			const auto d = static_cast< F32 >(m_display_mode.RefreshRate.Denominator);
			return static_cast< U32 >(round(n / d));
		}

		/**
		 Returns the display refresh rate of this display configuration.

		 @return		The display refresh rate of this display configuration.
		 */
		[[nodiscard]]
		const DXGI_RATIONAL GetDisplayRefreshRate() const noexcept{
			return m_display_mode.RefreshRate;
		}

		//---------------------------------------------------------------------
		// Member Methods: Display Format
		//---------------------------------------------------------------------

		/**
		 Returns the display format of this display configuration.

		 @return		The display format of this display configuration.
		 */
		[[nodiscard]]
		DXGI_FORMAT GetDisplayFormat() const noexcept {
			return m_display_mode.Format;
		}

		//---------------------------------------------------------------------
		// Member Methods: Display Mode
		//---------------------------------------------------------------------

		/**
		 Returns the display mode of this display configuration.

		 @return		The display mode of this display configuration.
		 */
		[[nodiscard]]
		const DXGI_MODE_DESC& GetDisplayMode() const noexcept {
			return m_display_mode;
		}

		/**
		 Sets the display mode of this display configuration to the given
		 display mode.

		 @pre			@a display_mode must be compatible with the adapter and
						output of this display configuration.
		 @param[in]		display_mode
						A reference to the display mode.
		 */
		void SetDisplayMode(const DXGI_MODE_DESC& display_mode) noexcept {
			m_display_mode = display_mode;
		}

		//---------------------------------------------------------------------
		// Member Methods: Anti-Aliasing
		//---------------------------------------------------------------------

		/**
		 Checks whether this display configuration uses AA.

		 @return		@c true if this display configuration uses AA.
						@c false otherwise.
		 */
		[[nodiscard]]
		bool UsesAA() const noexcept {
			return AntiAliasing::None != m_aa;
		}

		/**
		 Checks whether this display configuration uses MSAA.

		 @return		@c true if this display configuration uses MSAA.
						@c false otherwise.
		 */
		[[nodiscard]]
		bool UsesMSAA() const noexcept {
			switch (m_aa) {

			case AntiAliasing::MSAA_2x:
			case AntiAliasing::MSAA_4x:
			case AntiAliasing::MSAA_8x:
				return true;
			default:
				return false;

			}
		}

		 /**
		 Checks whether this display configuration uses SSAA.

		 @return		@c true if this display configuration uses SSAA.
						@c false otherwise.
		 */
		[[nodiscard]]
		bool UsesSSAA() const noexcept {
			switch (m_aa) {

			case AntiAliasing::SSAA_2x:
			case AntiAliasing::SSAA_3x:
			case AntiAliasing::SSAA_4x:
				return true;
			default:
				return false;

			}
		}

		/**
		 Returns the anti-aliasing mode of this display configuration.

		 @return		The anti-aliasing mode of this display configuration.
		 */
		[[nodiscard]]
		AntiAliasing GetAA() const noexcept {
			return m_aa;
		}

		/**
		 Sets the anti-aliasing mode of this display configuration to the given
		 anti-aliasing mode.

		 @param[in]		aa
						The anti-aliasing mode.
		 */
		void SetAA(AntiAliasing aa) noexcept {
			m_aa = aa;
		}

		//---------------------------------------------------------------------
		// Member Methods: Windowed/Fullscreen Mode
		//---------------------------------------------------------------------

		/**
		 Checks whether the application should run in windowed mode for this
		 display configuration.

		 @return		@c true if the application should run in windowed mode
						for this display configuration. @c false otherwise.
		 */
		[[nodiscard]]
		bool IsWindowed() const noexcept {
			return m_windowed;
		}

		/**
		 Sets the windowed/fullscreen mode of this display configuration to the
		 given windowed/fullscreen mode.

		 @param[in]		windowed
						@c true if windowed mode. @c false otherwise.
		 */
		void SetWindowed(bool windowed = true) noexcept {
			m_windowed = windowed;
		}

		/**
		 Checks whether the application should run in full screen mode for this
		 display configuration.

		 @return		@c true if the application should run in full screen
						mode for this display configuration. @c false
						otherwise.
		 */
		[[nodiscard]]
		bool IsFullScreen() const noexcept {
			return !m_windowed;
		}

		/**
		 Sets the windowed/fullscreen mode of this display configuration to the
		 given windowed/fullscreen mode.

		 @param[in]		fullscreen
						@c true if fullscreen mode. @c false otherwise.
		 */
		void SetFullScreen(bool fullscreen = true) noexcept {
			SetWindowed(!fullscreen);
		}

		//---------------------------------------------------------------------
		// Member Methods: VSync
		//---------------------------------------------------------------------

		/**
		 Checks whether V-sync should be enabled for this display
		 configuration.

		 @return		@c true if v-sync should be enabled for this display
						configuration. @c false otherwise.
		 */
		[[nodiscard]]
		bool IsVSynced() const noexcept {
			return m_vsync;
		}

		/**
		 Sets the V-sync mode of this display configuration to the given V-sync
		 mode.

		 @param[in]		vsync
						@c true if V-sync mode. @c false otherwise.
		 */
		void SetVSync(bool vsync = true) noexcept {
			m_vsync = vsync;
		}

	private:

		//---------------------------------------------------------------------
		// Member Variables: Adapter
		//---------------------------------------------------------------------

		/**
		 A pointer to the adapter (e.g. video card) of this display
		 configuration.
		 */
		ComPtr< DXGIAdapter > m_adapter;

		//---------------------------------------------------------------------
		// Member Variables: Output
		//---------------------------------------------------------------------

		/**
		A pointer to the output (e.g. screen monitor) of this display
		configuration.
		*/
		ComPtr< DXGIOutput > m_output;

		//---------------------------------------------------------------------
		// Member Variables: Display Mode
		//---------------------------------------------------------------------

		/**
		 The display mode of this display configuration.
		 */
		DXGI_MODE_DESC m_display_mode;

		//---------------------------------------------------------------------
		// Member Variables: Anti-Aliasing
		//---------------------------------------------------------------------

		/**
		 The Anti-Aliasing mode of this display configuration.
		 */
		AntiAliasing m_aa;

		//---------------------------------------------------------------------
		// Member Variables: Windowed/Fullscreen Mode
		//---------------------------------------------------------------------

		/**
		 Flag indicating whether the application should run in windowed mode
		 for this display configuration.
		 */
		bool m_windowed;

		//---------------------------------------------------------------------
		// Member Variables: VSync
		//---------------------------------------------------------------------

		/**
		 Flag indicating whether V-sync should be enabled for this display
		 configuration.
		 */
		bool m_vsync;
	};
}