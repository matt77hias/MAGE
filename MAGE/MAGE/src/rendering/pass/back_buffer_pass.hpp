#pragma once

//-----------------------------------------------------------------------------
// Engine Includes
//-----------------------------------------------------------------------------
#pragma region

#include "shader\shader.hpp"

#pragma endregion

//-----------------------------------------------------------------------------
// Engine Declarations end Definitions
//-----------------------------------------------------------------------------
namespace mage {

	/**
	 A class of back buffer passes for tone mapping and gamma correcting HDR 
	 images.
	 */
	class BackBufferPass final {

	public:

		//---------------------------------------------------------------------
		// Constructors and Destructors
		//---------------------------------------------------------------------

		/**
		 Constructs an back buffer pass.

		 @pre			The renderer associated with the current engine must be 
						loaded.
		 @pre			The resource manager associated with the current engine 
						must be loaded.
		 */
		BackBufferPass();

		/**
		 Constructs an back buffer pass from the given back buffer pass.

		 @param[in]		pass
						A reference to the back buffer pass to copy.
		 */
		BackBufferPass(const BackBufferPass &pass) = delete;

		/**
		 Constructs an back buffer pass by moving the given back buffer pass.

		 @param[in]		pass
						A reference to the Image pass to move.
		 */
		BackBufferPass(BackBufferPass &&pass) noexcept;

		/**
		 Destructs this back buffer pass.
		 */
		~BackBufferPass();

		//---------------------------------------------------------------------
		// Assignment Operators
		//---------------------------------------------------------------------

		/**
		 Copies the given back buffer pass to this back buffer pass.

		 @param[in]		pass
						A reference to the back buffer pass to copy.
		 @return		A reference to the copy of the given back buffer pass 
						(i.e. this back buffer pass).
		 */
		BackBufferPass &operator=(const BackBufferPass &pass) = delete;

		/**
		 Moves the given back buffer pass to this back buffer pass.

		 @param[in]		pass
						A reference to the back buffer pass to move.
		 @return		A reference to the moved back buffer pass (i.e. this 
						back buffer pass).
		 */
		BackBufferPass &operator=(BackBufferPass &&pass) = delete;

		//---------------------------------------------------------------------
		// Member Methods
		//---------------------------------------------------------------------

		/**
		 Renders.
		 */
		void Render() const noexcept;
		
	private:

		//---------------------------------------------------------------------
		// Member Methods
		//---------------------------------------------------------------------

		/**
		 Binds the fixed state of this back buffer pass.
		 */
		void BindFixedState() const noexcept;

		//---------------------------------------------------------------------
		// Member Variables
		//---------------------------------------------------------------------

		/**
		 A pointer to the device context of this back buffer pass.
		 */
		ID3D11DeviceContext * const m_device_context;

		/**
		 A pointer to the vertex shader of this back buffer pass.
		 */
		const VertexShaderPtr m_vs;

		/**
		 A pointer to the pixel shader of this back buffer pass.
		 */
		const PixelShaderPtr m_ps;
	};
}