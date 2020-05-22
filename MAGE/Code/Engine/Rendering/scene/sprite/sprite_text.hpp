#pragma once

//-----------------------------------------------------------------------------
// Engine Includes
//-----------------------------------------------------------------------------
#pragma region

#include "scene\component.hpp"
#include "resource\font\sprite_font.hpp"

#pragma endregion

//-----------------------------------------------------------------------------
// Engine Declarations and Definitions
//-----------------------------------------------------------------------------
namespace mage::rendering {

	#pragma warning( push )
	#pragma warning( disable : 4324 ) // Added padding.

	/**
	 A class of sprite texts.
	 */
	class alignas(16) SpriteText : public Component {

	public:

		//---------------------------------------------------------------------
		// Effect
		//---------------------------------------------------------------------

		/**
		 An enumeration of the different text effects.
		 This contains:

		 @c None,
		 @c DropShadow and
		 @c Outline.
		 */
		enum class TextEffect : U8 {
			None = 0u,
			DropShadow,
			Outline
		};

		//---------------------------------------------------------------------
		// Constructors and Destructors
		//---------------------------------------------------------------------

		/**
		 Constructs a sprite text.
		 */
		SpriteText();

		/**
		 Constructs a sprite text from the given sprite text.

		 @param[in]		sprite
						A reference to the sprite text to copy.
		 */
		SpriteText(const SpriteText& sprite);

		/**
		 Constructs a sprite text by moving the given sprite text.

		 @param[in]		sprite
						A reference to the sprite text to move.
		 */
		SpriteText(SpriteText&& sprite) noexcept;

		/**
		 Destruct this sprite text.
		 */
		virtual ~SpriteText();

		//---------------------------------------------------------------------
		// Assignment Operators
		//---------------------------------------------------------------------

		/**
		 Copies the given sprite text to this sprite text.

		 @param[in]		sprite
						A reference to the sprite text to copy.
		 @return		A reference to the copy of the given sprite text (i.e.
						this sprite text).
		 */
		SpriteText& operator=(const SpriteText& sprite);

		/**
		 Moves the given sprite text to this sprite text.

		 @param[in]		sprite
						A reference to the sprite text to move.
		 @return		A reference to the moved sprite text (i.e. this sprite
						text).
		 */
		SpriteText& operator=(SpriteText&& sprite) noexcept;

		//---------------------------------------------------------------------
		// Member Methods
		//---------------------------------------------------------------------

		/**
		 Draws this sprite text.

		 @param[in,out]	sprite_batch
						A reference to the sprite batch used for rendering this
						sprite text.
		 */
		void Draw(SpriteBatch& sprite_batch) const;

		//---------------------------------------------------------------------
		// Member Methods
		//---------------------------------------------------------------------

		/**
		 Returns the sprite transform of this sprite text.

		 @return		A reference to the sprite transform of this sprite
						text.
		 */
		[[nodiscard]]
		SpriteTransform2D& GetSpriteTransform() noexcept {
			return m_sprite_transform;
		}

		/**
		 Returns the sprite transform of this sprite text.

		 @return		A reference to the sprite transform of this sprite
						text.
		 */
		[[nodiscard]]
		const SpriteTransform2D& GetSpriteTransform() const noexcept {
			return m_sprite_transform;
		}

		//---------------------------------------------------------------------
		// Member Methods: Sprite Transform
		//---------------------------------------------------------------------

		/**
		 Returns the sprite effects of this sprite text.

		 @return		The sprite effects of this sprite text.
		 */
		[[nodiscard]]
		SpriteEffect GetSpriteEffects() const noexcept {
			return m_sprite_effects;
		}

		/**
		 Sets the sprite effects of this sprite text to the given sprite
		 effects.

		 @param[in]		sprite_effects
						The sprite effects.
		 */
		void SetSpriteEffects(SpriteEffect sprite_effects) noexcept {
			m_sprite_effects = sprite_effects;
		}

		//---------------------------------------------------------------------
		// Member Methods: Text
		//---------------------------------------------------------------------

		/**
		 Clears the text of this sprite text.
		 */
		void ClearText() noexcept {
			m_strings.clear();
		}

		/**
		 Returns the text of this sprite text.

		 @return		A reference to a vector containing the color strings
						of this sprite text.
		 */
		[[nodiscard]]
		const std::vector< ColorString >& GetText() const noexcept {
			return m_strings;
		}

		/**
		 Sets the text of this sprite text to the given text.

		 @param[in]		text
						A reference to the text.
		 */
		void SetText(const ColorString& text) {
			ClearText();
			AppendText(text);
		}

		/**
		 Sets the text of this sprite text to the given text.

		 @param[in]		text
						A reference to the text.
		 */
		void SetText(ColorString&& text) {
			ClearText();
			AppendText(std::move(text));
		}

		/**
		 Appends the given text to the end of the text of this sprite text.

		 @param[in]		text
						A reference to the text.
		 */
		void AppendText(const ColorString& text) {
			m_strings.push_back(text);
		}

		/**
		 Appends the given text to the end of the text of this sprite text.

		 @param[in]		text
						A reference to the text.
		 */
		void AppendText(ColorString&& text) {
			m_strings.push_back(std::move(text));
		}

		/**
		 Traverses all color strings of this sprite text.

		 @tparam		ActionT
						An action to perform on all color strings of this
						sprite text. The action must accept @c ColorString&
						values.
		 @param[in]		action
						The action.
		 */
		template< typename ActionT >
		void ForEachColorString(ActionT&& action);

		/**
		 Traverses all color strings of this sprite text.

		 @tparam		ActionT
						An action to perform on all color strings of this
						sprite text. The action must accept @c const
						@c ColorString& values.
		 @param[in]		action
						The action.
		 */
		template< typename ActionT >
		void ForEachColorString(ActionT&& action) const;

		//---------------------------------------------------------------------
		// Member Methods: Text Effect
		//---------------------------------------------------------------------

		/**
		 Returns the (linear) text effect color of this sprite text.

		 @return		A reference to the (linear) text effect color of this
						sprite text.
		 */
		[[nodiscard]]
		RGBA& GetTextEffectColor() noexcept {
			return m_text_effect_color;
		}

		/**
		 Returns the (linear) text effect color of this sprite text.

		 @return		A reference to the (linear) text effect color of this
						sprite text.
		 */
		[[nodiscard]]
		const RGBA& GetTextEffectColor() const noexcept {
			return m_text_effect_color;
		}

		/**
		 Returns the text effect of this sprite text.

		 @return		The text effect of this sprite text.
		 */
		[[nodiscard]]
		TextEffect GetTextEffect() const noexcept {
			return m_text_effect;
		}

		/**
		 Sets the text effect of this sprite text to the given text effect.

		 @param[in]		text_effect
						The text effect.
		 */
		void SetTextEffect(TextEffect text_effect) noexcept {
			m_text_effect = text_effect;
		}

		//---------------------------------------------------------------------
		// Member Methods: Text Font
		//---------------------------------------------------------------------

		/**
		 Returns the font of this sprite text.

		 @return		A pointer to the font of this sprite text.
		 */
		[[nodiscard]]
		SpriteFontPtr GetFont() const noexcept {
			return m_font;
		}

		/**
		 Returns the shader resource view of the font of this sprite text.

		 @return		A pointer to the shader resource view of the font of
						this sprite text.
		 */
		[[nodiscard]]
		ID3D11ShaderResourceView* GetFontSRV() const noexcept {
			return m_font->Get();
		}

		/**
		 Sets the font of this sprite text to the given font.

		 @param[in]		font
						A pointer to the font of this sprite text.
		 */
		void SetFont(SpriteFontPtr font) noexcept {
			m_font = std::move(font);
		}

	private:

		//---------------------------------------------------------------------
		// Member Variables: Transform
		//---------------------------------------------------------------------

		/**
		 The sprite transform of this sprite text.
		 */
		SpriteTransform2D m_sprite_transform;

		//---------------------------------------------------------------------
		// Member Variables: Sprite Effects
		//---------------------------------------------------------------------

		/**
		 The sprite effects of this sprite text.
		 */
		SpriteEffect m_sprite_effects;

		//---------------------------------------------------------------------
		// Member Variables: Text
		//---------------------------------------------------------------------

		/**
		 A vector with the color strings of this sprite text.
		 */
		std::vector< ColorString > m_strings;

		//---------------------------------------------------------------------
		// Member Variables: Text Effect
		//---------------------------------------------------------------------

		/**
		 The (linear) text effect color of this sprite text.
		 */
		RGBA m_text_effect_color;

		/**
		 The text effect of this sprite text.
		 */
		TextEffect m_text_effect;

		//---------------------------------------------------------------------
		// Member Variables: Text Font
		//---------------------------------------------------------------------

		/**
		 A pointer to the sprite font of this sprite text.
		 */
		SpriteFontPtr m_font;
	};

	#pragma warning( pop )
}

//-----------------------------------------------------------------------------
// Engine Includes
//-----------------------------------------------------------------------------
#pragma region

#include "scene\sprite\sprite_text.tpp"

#pragma endregion