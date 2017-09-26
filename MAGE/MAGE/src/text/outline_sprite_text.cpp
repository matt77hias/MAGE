//-----------------------------------------------------------------------------
// Engine Includes
//-----------------------------------------------------------------------------
#pragma region

#include "text\outline_sprite_text.hpp"

#pragma endregion

//-----------------------------------------------------------------------------
// Engine Definitions
//-----------------------------------------------------------------------------
namespace mage {

	OutlineSpriteText::OutlineSpriteText(SharedPtr< SpriteFont > font,
		const Color &border_color, SpriteEffect effects)
		: SpriteText(font, effects),
		m_border_color(border_color) {}

	OutlineSpriteText::OutlineSpriteText(SharedPtr< SpriteFont > font,
		FXMVECTOR border_color, SpriteEffect effects)
		: SpriteText(font, effects),
		m_border_color() {
		SetBorderColor(border_color);
	}

	OutlineSpriteText::OutlineSpriteText(
		const OutlineSpriteText &sprite_text) = default;

	OutlineSpriteText::OutlineSpriteText(
		OutlineSpriteText &&sprite_text) = default;

	OutlineSpriteText::~OutlineSpriteText() = default;

	UniquePtr< Sprite > OutlineSpriteText::CloneImplementation() const {
		return MakeUnique< OutlineSpriteText >(*this);
	}

	void OutlineSpriteText::Draw(SpriteBatch &sprite_batch) const {
		const wchar_t * const text  = c_str();
		SpriteTransform transform(*GetTransform());
		const XMVECTOR border_color = GetBorderColorVector();
		const SpriteEffect effects  = GetSpriteEffects();
		
		// +1, +1
		transform.AddTranslation(XMFLOAT2(1.0f, 1.0f));
		GetRawFont()->DrawString(sprite_batch, 
			text, transform, border_color, effects);
		// -1, +1
		transform.AddTranslationX(-2.0f);
		GetRawFont()->DrawString(sprite_batch, 
			text, transform, border_color, effects);
		// -1, -1
		transform.AddTranslationY(-2.0f);
		GetRawFont()->DrawString(sprite_batch, 
			text, transform, border_color, effects);
		// +1, -1
		transform.AddTranslationX(2.0f);
		GetRawFont()->DrawString(sprite_batch, 
			text, transform, border_color, effects);

		GetRawFont()->DrawString(sprite_batch, 
			GetTextWithColors(), *GetTransform(), effects);
	}
}