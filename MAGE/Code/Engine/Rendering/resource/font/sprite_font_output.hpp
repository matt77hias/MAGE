#pragma once

//-----------------------------------------------------------------------------
// Engine Includes
//-----------------------------------------------------------------------------
#pragma region

#include "direct3d11.hpp"
#include "resource\font\glyph.hpp"

#pragma endregion

//-----------------------------------------------------------------------------
// System Includes
//-----------------------------------------------------------------------------
#pragma region

#include <vector>

#pragma endregion

//-----------------------------------------------------------------------------
// Engine Declarations and Definitions
//-----------------------------------------------------------------------------
namespace mage::rendering {

	/**
	 A struct of sprite font outputs for storing the data of a sprite font.
	 */
	struct SpriteFontOutput {

	public:

		//---------------------------------------------------------------------
		// Constructors and Destructors
		//---------------------------------------------------------------------

		/**
		 Constructs a sprite font output.
		 */
		SpriteFontOutput() = default;

		/**
		 Constructs a sprite font output from the given sprite font output.

		 @param[in]		output
						A reference to the sprite font output to copy.
		 */
		SpriteFontOutput(const SpriteFontOutput& output) = delete;

		/**
		 Constructs a sprite font output by moving the given sprite font output.

		 @param[in]		output
						A reference to the sprite font output to move.
		 */
		SpriteFontOutput(SpriteFontOutput&& output) noexcept = default;

		/**
		 Destructs this sprite font output.
		 */
		~SpriteFontOutput() = default;

		//---------------------------------------------------------------------
		// Assignment Operators
		//---------------------------------------------------------------------

		/**
		 Copies the given sprite font output to this sprite font output.

		 @param[in]		output
						A reference to the sprite font output to copy.
		 @return		A reference to the copy of the given sprite font output
						(i.e. this sprite font output).
		 */
		SpriteFontOutput& operator=(const SpriteFontOutput& output) = delete;

		/**
		 Moves the given sprite font output to this sprite font output.

		 @param[in]		output
						A reference to the sprite font output to move.
		 @return		A reference to the moved sprite font output (i.e. this
						sprite font output).
		 */
		SpriteFontOutput& operator=(SpriteFontOutput&& output) noexcept = default;

		//---------------------------------------------------------------------
		// Member Variables
		//---------------------------------------------------------------------

		/**
		 A pointer to the texture of the sprite font of this sprite font
		 output.
		 */
		ComPtr< ID3D11ShaderResourceView > m_texture_srv;

		/**
		 A vector containing the glyphs of the sprite font of this sprite font
		 output.
		 */
		std::vector< Glyph > m_glyphs;

		/**
		 The default character of the sprite font of this sprite font output.
		 */
		wchar_t m_default_character;

		/**
		 The (extra) line spacing of the sprite font of this sprite font
		 output.
		 */
		F32 m_line_spacing;
	};
}
