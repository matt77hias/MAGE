#pragma once

//-----------------------------------------------------------------------------
// Engine Includes
//-----------------------------------------------------------------------------
#pragma region

#include "scene\component.hpp"
#include "renderer\pass\sprite_batch.hpp"
#include "resource\texture\texture.hpp"
#include "spectrum\spectrum.hpp"

#pragma endregion

//-----------------------------------------------------------------------------
// Engine Declarations and Definitions
//-----------------------------------------------------------------------------
namespace mage::rendering {

	#pragma warning( push )
	#pragma warning( disable : 4324 ) // Added padding.

	/**
	 A class of sprite images.
	 */
	class alignas(16) SpriteImage : public Component {

	public:

		//---------------------------------------------------------------------
		// Constructors and Destructors
		//---------------------------------------------------------------------

		/**
		 Constructs a sprite image.
		 */
		SpriteImage() noexcept;

		/**
		 Constructs a sprite image from the given sprite image.

		 @param[in]		sprite
						A reference to the sprite image to copy.
		 */
		SpriteImage(const SpriteImage& sprite) noexcept;

		/**
		 Constructs a sprite image by moving the given sprite image.

		 @param[in]		sprite
						A reference to the sprite image to move.
		 */
		SpriteImage(SpriteImage&& sprite) noexcept;

		/**
		 Destruct this sprite image.
		 */
		virtual ~SpriteImage();

		//---------------------------------------------------------------------
		// Assignment Operators
		//---------------------------------------------------------------------

		/**
		 Copies the given sprite image to this sprite image.

		 @param[in]		sprite
						A reference to the sprite image to copy.
		 @return		A reference to the copy of the given sprite image (i.e.
						this sprite image).
		 */
		SpriteImage& operator=(const SpriteImage& sprite) noexcept;

		/**
		 Moves the given sprite image to this sprite image.

		 @param[in]		sprite
						A reference to the sprite image to move.
		 @return		A reference to the moved sprite image (i.e. this sprite
						image).
		 */
		SpriteImage& operator=(SpriteImage&& sprite) noexcept;

		//---------------------------------------------------------------------
		// Member Methods
		//---------------------------------------------------------------------

		/**
		 Draws this sprite image.

		 @param[in,out]	sprite_batch
						A reference to the sprite batch used for rendering this
						sprite image.
		 */
		void Draw(SpriteBatch& sprite_batch) const;

		//---------------------------------------------------------------------
		// Member Methods
		//---------------------------------------------------------------------

		/**
		 Returns the sprite transform of this sprite image.

		 @return		A reference to the sprite transform of this sprite
						image.
		 */
		[[nodiscard]]
		SpriteTransform2D& GetSpriteTransform() noexcept {
			return m_sprite_transform;
		}

		/**
		 Returns the sprite transform of this sprite image.

		 @return		A reference to the sprite transform of this sprite
						image.
		 */
		[[nodiscard]]
		const SpriteTransform2D& GetSpriteTransform() const noexcept {
			return m_sprite_transform;
		}

		//---------------------------------------------------------------------
		// Member Methods: Sprite Transform
		//---------------------------------------------------------------------

		/**
		 Returns the sprite effects of this sprite image.

		 @return		The sprite effects of this sprite image.
		 */
		[[nodiscard]]
		SpriteEffect GetSpriteEffects() const noexcept {
			return m_sprite_effects;
		}

		/**
		 Sets the sprite effects of this sprite image to the given sprite
		 effects.

		 @param[in]		sprite_effects
						The sprite effects.
		 */
		void SetSpriteEffects(SpriteEffect sprite_effects) noexcept {
			m_sprite_effects = sprite_effects;
		}

		//---------------------------------------------------------------------
		// Member Methods: Image
		//---------------------------------------------------------------------

		/**
		 Returns the (linear) base color of this sprite image.

		 @return		A reference to the (linear) base color of this sprite
						image.
		 */
		[[nodiscard]]
		RGBA& GetBaseColor() noexcept {
			return m_base_color;
		}

		/**
		 Returns the (linear) base color of this sprite image.

		 @return		A reference to the (linear) base color of this sprite
						image.
		 */
		[[nodiscard]]
		const RGBA& GetBaseColor() const noexcept {
			return m_base_color;
		}

		/**
		 Returns the base color texture region of this sprite image.

		 @return		The base color texture region of this sprite image.
		 */
		[[nodiscard]]
		const RECT GetBaseColorTextureRegion() const noexcept {
			return m_base_color_texture_region;
		}

		/**
		 Checks whether the base color texture region of this sprite image
		 corresponds to the maximum texture region.

		 @return		@c true if the base color texture region of this sprite
						image corresponds to the maximum texture region.
						@c false otherwise.
		 */
		[[nodiscard]]
		bool HasMaximumBaseColorTextureRegion() const noexcept {
			return 0 == m_base_color_texture_region.left
				&& 0 == m_base_color_texture_region.top
				&& 0 == m_base_color_texture_region.right
				&& 0 == m_base_color_texture_region.bottom;
		}

		/**
		 Sets the base color texture region of this sprite image to the maximum
		 texture region.
		 */
		void SetMaximumBaseColorTextureRegion() noexcept {
			m_base_color_texture_region = {};
		}

		/**
		 Sets the base color texture region of this sprite image to the given
		 texture region.

		 @param[in]		texture_region
						A reference to the texture region.
		 */
		void SetBaseColorTextureRegion(const RECT& texture_region) noexcept {
			m_base_color_texture_region = texture_region;
		}

		/**
		 Returns the base color texture of this sprite image.

		 @return		A pointer to the base color texture of this sprite
						image.
		 */
		[[nodiscard]]
		TexturePtr GetBaseColorTexture() const noexcept {
			return m_base_color_texture;
		}

		/**
		 Returns the shader resource view of the base color texture of this
		 sprite image.

		 @return		@c nullptr, if this sprite image has no base color
						texture.
		 @return		A pointer to the shader resource view of the base
						color texture of this sprite image.
		 */
		[[nodiscard]]
		ID3D11ShaderResourceView* GetBaseColorSRV() const noexcept {
			return m_base_color_texture ? m_base_color_texture->Get() : nullptr;
		}

		/**
		 Sets the base color texture of this sprite image to the given base
		 color texture.

		 @param[in]		base_color_texture
						A pointer to the base color texture.
		 */
		void SetBaseColorTexture(TexturePtr base_color_texture) noexcept {
			m_base_color_texture = std::move(base_color_texture);
		}

	private:

		//---------------------------------------------------------------------
		// Member Variables: Transform
		//---------------------------------------------------------------------

		/**
		 The sprite transform of this sprite image.
		 */
		SpriteTransform2D m_sprite_transform;

		//---------------------------------------------------------------------
		// Member Variables: Sprite Effects
		//---------------------------------------------------------------------

		/**
		 The sprite effects of this sprite image.
		 */
		SpriteEffect m_sprite_effects;

		//---------------------------------------------------------------------
		// Member Variables: Image
		//---------------------------------------------------------------------

		/**
		 The (linear) base color of this sprite image.
		 */
		RGBA m_base_color;

		/**
		 A pointer to the base color texture region of this sprite image.

		 If every member variable of the rectangle is zero, the full texture
		 region is considered.
		 */
		RECT m_base_color_texture_region;

		/**
		 A pointer to the base color texture of this sprite image.
		 */
		TexturePtr m_base_color_texture;
	};

	#pragma warning( pop )
}