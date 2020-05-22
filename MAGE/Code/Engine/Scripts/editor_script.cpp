//-----------------------------------------------------------------------------
// Engine Includes
//-----------------------------------------------------------------------------
#pragma region

#include "editor_script.hpp"
#include "scene\scene.hpp"
#include "ImGui\imgui.h"

#pragma endregion

//-----------------------------------------------------------------------------
// Engine Definitions
//-----------------------------------------------------------------------------
namespace mage::script {

	//-------------------------------------------------------------------------
	// Editor Utilities
	//-------------------------------------------------------------------------
	namespace {

		void DrawWidget(rendering::CameraLens& lens,
						const F32x2& clipping_planes) {

			//-----------------------------------------------------------------
			// Aperture Radius
			//-----------------------------------------------------------------
			auto aperture_radius = lens.GetApertureRadius();
			ImGui::SliderFloat("Aperture Radius", &aperture_radius, 0.0f, 0.10f);
			lens.SetApertureRadius(aperture_radius);

			//-----------------------------------------------------------------
			// Focal Length
			//-----------------------------------------------------------------
			auto focal_length = lens.GetFocalLength();
			ImGui::SliderFloat("Focal Length", &focal_length, 0.01f, 0.10f);
			lens.SetFocalLength(focal_length);

			//-----------------------------------------------------------------
			// Maximum Radius of the Circle-of-Confusion
			//-----------------------------------------------------------------
			auto focus_distance = lens.GetFocusDistance();
			ImGui::SliderFloat("Focus Distance", &focus_distance,
							   clipping_planes[0],
							   clipping_planes[1]);
			lens.SetFocusDistance(focus_distance);
		}

		void DrawWidget(rendering::VoxelizationSettings& settings) {
			using rendering::VoxelizationSettings;

			//-----------------------------------------------------------------
			// Voxelization
			//-----------------------------------------------------------------
			if (ImGui::TreeNode("Voxelization")) {
				auto origin = VoxelizationSettings::GetVoxelGridCenter();
				ImGui::InputFloat3("Origin", origin.data());
				VoxelizationSettings::SetVoxelGridCenter(origin);

				auto exponent = static_cast< S32 >(
					VoxelizationSettings::GetMaxVoxelTextureMipLevel());
				ImGui::SliderInt("Voxel Grid Resolution", &exponent, 0, 8);
				VoxelizationSettings::SetVoxelGridResolution(
					static_cast< U32 >(exponent));

				auto voxel_size = VoxelizationSettings::GetVoxelSize();
				ImGui::DragFloat("Voxel Size", &voxel_size,
								 0.01f, 0.01f, 10.0f, "%.2f");
				VoxelizationSettings::SetVoxelSize(voxel_size);

				ImGui::TreePop();
			}

			//-----------------------------------------------------------------
			// Voxel Cone Tracing
			//-----------------------------------------------------------------
			if (ImGui::TreeNode("Voxel Cone Tracing")) {
				auto vct = settings.UsesVCT();
				ImGui::Checkbox("VCT", &vct);
				settings.SetVCT(vct);

				auto cone_step = settings.GetConeStep();
				ImGui::DragFloat("Cone Step", &cone_step,
								 0.01f, 0.01f, 1.0f, "%.2f");
				settings.SetConeStep(cone_step);

				auto max_cone_distance = settings.GetMaxConeDistance();
				ImGui::DragFloat("Max Cone Distance", &max_cone_distance,
								 1.0f / 1024.0f, 0.0f, 1.0f, "%.4f");
				settings.SetMaxConeDistance(max_cone_distance);

				ImGui::TreePop();
			}
		}

		void DrawWidget(rendering::Fog& fog) {

			//-----------------------------------------------------------------
			// Base Color
			//-----------------------------------------------------------------
			SRGB color(fog.GetBaseColor());
			ImGui::ColorEdit3("Base Color", color.data());
			fog.GetBaseColor() = RGB(color);

			//-----------------------------------------------------------------
			// Density
			//-----------------------------------------------------------------
			auto density = fog.GetDensity();
			ImGui::DragFloat("Density", &density, 0.00001f, 0.0f, 1.0f, "%.5f");
			fog.SetDensity(density);
		}

		void DrawWidget(rendering::Sky& sky) {

			//-----------------------------------------------------------------
			// Texture
			//-----------------------------------------------------------------
			if (sky.GetTexture()) {
				const UTF16toUTF8 guid(sky.GetTexture()->GetGuid());
				ImGui::LabelText("Sky Texture", guid.c_str());
			}
			else {
				ImGui::Text("mage_black_texture");
				ImGui::Text("1 x 1 texel");
			}

			//-----------------------------------------------------------------
			// Density
			//-----------------------------------------------------------------
			auto scale_z = sky.GetScaleZ();
			ImGui::DragFloat("Stretching", &scale_z, 0.01f, 1.0f, 10.0f, "%.2f");
			sky.SetScaleZ(scale_z);
		}

		void DrawWidget(rendering::CameraSettings& settings) {
			using rendering::RenderMode;
			using rendering::BRDF;
			using rendering::ToneMapping;
			using rendering::RenderLayer;

			//-----------------------------------------------------------------
			// Render Mode
			//-----------------------------------------------------------------
			static constexpr const_zstring render_mode_names[] = {
				"None",
				"Forward",
				"Deferred",
				"Solid",
				"Voxel Grid",
				"Base Color",
				"Base Color Coefficient",
				"Base Color Texture",
				"Material",
				"Material Coefficient",
				"Material Texture",
				"Roughness",
				"Roughness Coefficient",
				"Roughness Texture",
				"Metalness",
				"Metalness Coefficient",
				"Metalness Texture",
				"Shading Normal",
				"TSNM Shading Normal",
				"Depth",
				"Distance",
				"UV"
			};
			static constexpr RenderMode render_modes[] = {
				RenderMode::None,
				RenderMode::Forward,
				RenderMode::Deferred,
				RenderMode::Solid,
				RenderMode::VoxelGrid,
				RenderMode::FalseColor_BaseColor,
				RenderMode::FalseColor_BaseColorCoefficient,
				RenderMode::FalseColor_BaseColorTexture,
				RenderMode::FalseColor_Material,
				RenderMode::FalseColor_MaterialCoefficient,
				RenderMode::FalseColor_MaterialTexture,
				RenderMode::FalseColor_Roughness,
				RenderMode::FalseColor_RoughnessCoefficient,
				RenderMode::FalseColor_RoughnessTexture,
				RenderMode::FalseColor_Metalness,
				RenderMode::FalseColor_MetalnessCoefficient,
				RenderMode::FalseColor_MetalnessTexture,
				RenderMode::FalseColor_ShadingNormal,
				RenderMode::FalseColor_TSNMShadingNormal,
				RenderMode::FalseColor_Depth,
				RenderMode::FalseColor_Distance,
				RenderMode::FalseColor_UV
			};
			static_assert(std::size(render_mode_names) == std::size(render_modes));

			auto render_mode_index = static_cast< int >(settings.GetRenderMode());
			ImGui::Combo("Render Mode", &render_mode_index, render_mode_names,
						 static_cast< int >(std::size(render_mode_names)));
			settings.SetRenderMode(render_modes[render_mode_index]);

			//-----------------------------------------------------------------
			// BRDF
			//-----------------------------------------------------------------
			static constexpr const_zstring brdf_names[] = {
				"Lambertian",
				"Blinn-Phong",
				"Cook-Torrance",
				"Frostbite"
			};
			static constexpr BRDF brdfs[] = {
				BRDF::Lambertian,
				BRDF::BlinnPhong,
				BRDF::CookTorrance,
				BRDF::Frostbite
			};
			static_assert(std::size(brdf_names) == std::size(brdfs));

			auto brdf_index = static_cast< int >(settings.GetBRDF());
			ImGui::Combo("BRDF", &brdf_index, brdf_names,
						 static_cast< int >(std::size(brdf_names)));
			settings.SetBRDF(brdfs[brdf_index]);

			//-----------------------------------------------------------------
			// Tone Mapping
			//-----------------------------------------------------------------
			static constexpr const_zstring tone_mapping_names[] = {
				"None",
				"ACES Filmic",
				"Max3",
				"Reinhard",
				"Uncharted"
			};
			static constexpr ToneMapping tone_mappings[] = {
				ToneMapping::None,
				ToneMapping::ACESFilmic,
				ToneMapping::Max3,
				ToneMapping::Reinhard,
				ToneMapping::Uncharted
			};
			static_assert(std::size(tone_mapping_names) == std::size(tone_mappings));

			auto tone_mapping_index = static_cast< int >(settings.GetToneMapping());
			ImGui::Combo("Tone Mapping", &tone_mapping_index, tone_mapping_names,
						 static_cast< int >(std::size(tone_mappings)));
			settings.SetToneMapping(tone_mappings[tone_mapping_index]);

			//-----------------------------------------------------------------
			// Voxelization Settings
			//-----------------------------------------------------------------
			auto render_mode = settings.GetRenderMode();
			if (RenderMode::Forward   == render_mode ||
				RenderMode::Deferred  == render_mode ||
				RenderMode::VoxelGrid == render_mode) {

				DrawWidget(settings.GetVoxelizationSettings());
			}

			//-----------------------------------------------------------------
			// Gamma Exponent
			//-----------------------------------------------------------------
			auto gamma = settings.GetGamma();
			ImGui::DragFloat("Gamma", &gamma, 0.01f, 0.01f, 10.0f, "%.2f");
			settings.SetGamma(gamma);

			//-----------------------------------------------------------------
			// Render Layers
			//-----------------------------------------------------------------
			if (ImGui::Button("Render Layers")) {
				ImGui::OpenPopup("Render Layers");
			}
			if (ImGui::BeginPopup("Render Layers")) {

				if (auto wireframe = settings.ContainsRenderLayer(RenderLayer::Wireframe);
					ImGui::Checkbox("Wireframe", &wireframe)) {

					settings.ToggleRenderLayer(RenderLayer::Wireframe);
				}

				if (auto aabb = settings.ContainsRenderLayer(RenderLayer::AABB);
					ImGui::Checkbox("AABB", &aabb)) {

					settings.ToggleRenderLayer(RenderLayer::AABB);
				}

				ImGui::EndPopup();
			}
		}

		void DrawWidget(rendering::Viewport& viewport,
						const F32x2& display_resolution) {

			static bool normalization = false;

			if (normalization) {
				//-------------------------------------------------------------
				// Top Left
				//-------------------------------------------------------------
				auto top_left
					= AbsoluteToNormalized(F32x2(viewport.GetTopLeft()),
										   display_resolution);
				ImGui::InputFloat2("Top Left", top_left.data());
				viewport.SetTopLeft(S32x2(
					NormalizedToAbsolute(top_left, display_resolution)));

				//-------------------------------------------------------------
				// Width and Height
				//-------------------------------------------------------------
				auto resolution
					= AbsoluteToNormalized(F32x2(viewport.GetSize()),
										   display_resolution);
				ImGui::InputFloat2("Resolution", resolution.data());
				viewport.SetSize(U32x2(
					NormalizedToAbsolute(resolution, display_resolution)));
			}
			else {
				//-------------------------------------------------------------
				// Top Left
				//-------------------------------------------------------------
				auto top_left = S32x2(viewport.GetTopLeft());
				ImGui::InputInt2("Top Left", top_left.data());
				viewport.SetTopLeft(top_left);

				//-------------------------------------------------------------
				// Width and Height
				//-------------------------------------------------------------
				auto resolution = S32x2(viewport.GetSize());
				ImGui::InputInt2("Resolution", resolution.data());
				viewport.SetSize(U32x2(resolution));
			}

			ImGui::Checkbox("Normalization", &normalization);
		}

		void DrawWidget(rendering::Camera& camera,
						const U32x2& display_resolution) {

			//-----------------------------------------------------------------
			// Clipping Planes
			//-----------------------------------------------------------------
			auto clipping_planes = camera.GetClippingPlanes();
			ImGui::InputFloat2("Clipping Planes", clipping_planes.data());
			camera.SetClippingPlanes(clipping_planes);

			//-----------------------------------------------------------------
			// Lens
			//-----------------------------------------------------------------
			if (ImGui::TreeNode("Lens")) {
				DrawWidget(camera.GetLens(), clipping_planes);
				ImGui::TreePop();
			}

			//-----------------------------------------------------------------
			// Viewport
			//-----------------------------------------------------------------
			if (ImGui::TreeNode("Viewport")) {
				DrawWidget(camera.GetViewport(),
						   static_cast< F32x2 >(display_resolution));
				ImGui::TreePop();
			}

			//-----------------------------------------------------------------
			// Settings
			//-----------------------------------------------------------------
			if (ImGui::TreeNode("Settings")) {
				DrawWidget(camera.GetSettings());
				ImGui::TreePop();
			}

			//-----------------------------------------------------------------
			// Fog
			//-----------------------------------------------------------------
			if (ImGui::TreeNode("Fog")) {
				DrawWidget(camera.GetSettings().GetFog());
				ImGui::TreePop();
			}

			//-----------------------------------------------------------------
			// Sky
			//-----------------------------------------------------------------
			if (ImGui::TreeNode("Sky")) {
				DrawWidget(camera.GetSettings().GetSky());
				ImGui::TreePop();
			}
		}

		void DrawWidget(rendering::OrthographicCamera& camera,
						const U32x2& display_resolution) {

			//-----------------------------------------------------------------
			// Size
			//-----------------------------------------------------------------
			auto size = camera.GetSize();
			ImGui::InputFloat2("Size", size.data());
			camera.SetSize(size);

			//-----------------------------------------------------------------
			// Camera
			//-----------------------------------------------------------------
			DrawWidget(static_cast< rendering::Camera& >(camera),
					   display_resolution);
		}

		void DrawWidget(rendering::PerspectiveCamera& camera,
						const U32x2& display_resolution) {

			//-----------------------------------------------------------------
			// Aspect Ratio
			//-----------------------------------------------------------------
			auto aspect_ratio = camera.GetAspectRatio();
			ImGui::InputFloat("Aspect Ratio", &aspect_ratio);
			camera.SetAspectRatio(aspect_ratio);

			//-----------------------------------------------------------------
			// Vertical Field-of-View
			//-----------------------------------------------------------------
			auto fov_y = camera.GetFOVY();
			ImGui::InputFloat("Vertical FOV", &fov_y);
			camera.SetFOVY(fov_y);

			//-----------------------------------------------------------------
			// Camera
			//-----------------------------------------------------------------
			DrawWidget(static_cast< rendering::Camera& >(camera),
					   display_resolution);
		}

		void DrawWidget(rendering::AmbientLight& light) {

			//-----------------------------------------------------------------
			// Base Color
			//-----------------------------------------------------------------
			SRGB color(light.GetBaseColor());
			ImGui::ColorEdit3("Base Color", color.data());
			light.GetBaseColor() = RGB(color);

			//-----------------------------------------------------------------
			// Radiance
			//-----------------------------------------------------------------
			auto radiance = light.GetRadiance();
			ImGui::InputFloat("Radiance", &radiance);
			light.SetRadiance(radiance);
		}

		void DrawWidget(rendering::DirectionalLight& light) {

			//-----------------------------------------------------------------
			// Base Color
			//-----------------------------------------------------------------
			SRGB color(light.GetBaseColor());
			ImGui::ColorEdit3("Base Color", color.data());
			light.GetBaseColor() = RGB(color);

			//-----------------------------------------------------------------
			// Irradiance
			//-----------------------------------------------------------------
			auto irradiance = light.GetIrradiance();
			ImGui::InputFloat("Irradiance", &irradiance);
			light.SetIrradiance(irradiance);

			//-----------------------------------------------------------------
			// Size
			//-----------------------------------------------------------------
			auto size = light.GetSize();
			ImGui::InputFloat2("Size", size.data());
			light.SetSize(size);

			//-----------------------------------------------------------------
			// Range
			//-----------------------------------------------------------------
			auto range = light.GetRange();
			ImGui::InputFloat("Range", &range);
			light.SetRange(range);

			//-----------------------------------------------------------------
			// Shadows
			//-----------------------------------------------------------------
			auto shadows = light.UseShadows();
			ImGui::Checkbox("Shadows", &shadows);
			light.SetShadows(shadows);

			//-----------------------------------------------------------------
			// Clipping Planes and Size
			//-----------------------------------------------------------------
			if (shadows) {
				auto clipping_planes = light.GetClippingPlanes();
				ImGui::InputFloat2("Clipping Planes", clipping_planes.data());
				light.SetClippingPlanes(clipping_planes);
			}
		}

		void DrawWidget(rendering::OmniLight& light) {

			//-----------------------------------------------------------------
			// Base Color
			//-----------------------------------------------------------------
			SRGB color(light.GetBaseColor());
			ImGui::ColorEdit3("Base Color", color.data());
			light.GetBaseColor() = RGB(color);

			//-----------------------------------------------------------------
			// Intensity
			//-----------------------------------------------------------------
			auto intensity = light.GetIntensity();
			ImGui::InputFloat("Intensity", &intensity);
			light.SetIntensity(intensity);

			//-----------------------------------------------------------------
			// Power
			//-----------------------------------------------------------------
			auto power = light.GetPower();
			ImGui::InputFloat("Power", &power);
			light.SetPower(power);

			//-----------------------------------------------------------------
			// Range
			//-----------------------------------------------------------------
			auto range = light.GetRange();
			ImGui::InputFloat("Range", &range);
			light.SetRange(range);

			//-----------------------------------------------------------------
			// Shadows
			//-----------------------------------------------------------------
			auto shadows = light.UseShadows();
			ImGui::Checkbox("Shadows", &shadows);
			light.SetShadows(shadows);

			//-----------------------------------------------------------------
			// Clipping Planes
			//-----------------------------------------------------------------
			if (shadows) {
				auto clipping_planes = light.GetClippingPlanes();
				ImGui::InputFloat2("Clipping Planes", clipping_planes.data());
				light.SetClippingPlanes(clipping_planes);
			}
		}

		void DrawWidget(rendering::SpotLight& light) {

			//-----------------------------------------------------------------
			// Base Color
			//-----------------------------------------------------------------
			SRGB color(light.GetBaseColor());
			ImGui::ColorEdit3("Base Color", color.data());
			light.GetBaseColor() = RGB(color);

			//-----------------------------------------------------------------
			// Intensity
			//-----------------------------------------------------------------
			auto intensity = light.GetIntensity();
			ImGui::InputFloat("Intensity", &intensity);
			light.SetIntensity(intensity);

			//-----------------------------------------------------------------
			// Power
			//-----------------------------------------------------------------
			auto power = light.GetPower();
			ImGui::InputFloat("Power", &power);
			light.SetPower(power);

			//-----------------------------------------------------------------
			// Range
			//-----------------------------------------------------------------
			auto range = light.GetRange();
			ImGui::InputFloat("Range", &range);
			light.SetRange(range);

			//-----------------------------------------------------------------
			// Cosine Penumbra and Umbra
			//-----------------------------------------------------------------
			auto cos_penumbra = light.GetStartAngularCutoff();
			ImGui::InputFloat("Cosine penumbra", &cos_penumbra);
			auto cos_umbra = light.GetEndAngularCutoff();
			ImGui::InputFloat("Cosine umbra", &cos_umbra);
			light.SetAngularCutoff(cos_penumbra, cos_umbra);

			//-----------------------------------------------------------------
			// Shadows
			//-----------------------------------------------------------------
			auto shadows = light.UseShadows();
			ImGui::Checkbox("Shadows", &shadows);
			light.SetShadows(shadows);

			//-----------------------------------------------------------------
			// Clipping Planes
			//-----------------------------------------------------------------
			if (shadows) {
				auto clipping_planes = light.GetClippingPlanes();
				ImGui::InputFloat2("Clipping Planes", clipping_planes.data());
				light.SetClippingPlanes(clipping_planes);
			}
		}

		void DrawWidget(TextureTransform2D& transform) {
			ImGui::Text("Texture Transform:");

			//-----------------------------------------------------------------
			// Translation
			//-----------------------------------------------------------------
			auto translation = transform.GetTranslationView();
			ImGui::InputFloat2("Translation", translation.data());
			transform.SetTranslation(translation);

			//-----------------------------------------------------------------
			// Rotation Origin
			//-----------------------------------------------------------------
			auto rotation_origin = transform.GetRotationOriginView();
			ImGui::InputFloat2("Rotation Origin", rotation_origin.data());
			transform.SetRotationOrigin(rotation_origin);

			//-----------------------------------------------------------------
			// Rotation
			//-----------------------------------------------------------------
			auto rotation = transform.GetRotation();
			ImGui::DragFloat("Rotation", &rotation, 0.0001f, -XM_PI, XM_PI);
			transform.SetRotation(rotation);

			//-----------------------------------------------------------------
			// Scale
			//-----------------------------------------------------------------
			auto scale = transform.GetScaleView();
			ImGui::InputFloat2("Scale", scale.data());
			transform.SetScale(scale);
		}

		void DrawWidget(rendering::Material& material) {
			ImGui::Text("Material:");

			//-----------------------------------------------------------------
			// Transparency
			//-----------------------------------------------------------------
			auto transparency = material.IsTransparant();
			ImGui::Checkbox("Transparency", &transparency);
			material.SetTransparent(transparency);

			//-----------------------------------------------------------------
			// Radiance
			//-----------------------------------------------------------------
			auto radiance = material.GetRadiance();
			ImGui::InputFloat("Radiance", &radiance);
			material.SetRadiance(radiance);

			//-----------------------------------------------------------------
			// Base Color
			//-----------------------------------------------------------------
			SRGBA color(material.GetBaseColor());
			ImGui::ColorEdit4("Base Color", color.data());
			material.GetBaseColor() = RGBA(color);

			//-----------------------------------------------------------------
			// Base Color Texture
			//-----------------------------------------------------------------
			const auto base_color_tex = material.GetBaseColorTexture();
			if (base_color_tex) {
				const UTF16toUTF8 guid(base_color_tex->GetGuid());
				ImGui::Text(guid.c_str());
				const auto resolution
					= rendering::GetTexture2DSize(*material.GetBaseColorSRV());
				ImGui::Text("%u x %u texels", resolution[0], resolution[1]);
			}
			else {
				ImGui::Text("mage_black_texture");
				ImGui::Text("1 x 1 texel");
			}

			//-----------------------------------------------------------------
			// Roughness
			//-----------------------------------------------------------------
			auto roughness = material.GetRoughness();
			ImGui::SliderFloat("Roughness", &roughness, 0.0f, 1.0f);
			material.SetRoughness(roughness);

			//-----------------------------------------------------------------
			// Metalness
			//-----------------------------------------------------------------
			auto metalness = material.GetMetalness();
			ImGui::SliderFloat("Metalness", &metalness, 0.0f, 1.0f);
			material.SetMetalness(metalness);

			//-----------------------------------------------------------------
			// Material Texture
			//-----------------------------------------------------------------
			const auto material_tex = material.GetMaterialTexture();
			if (material_tex) {
				const UTF16toUTF8 guid(material_tex->GetGuid());
				ImGui::Text(guid.c_str());
				const auto resolution
					= rendering::GetTexture2DSize(*material.GetMaterialSRV());
				ImGui::Text("%u x %u texels", resolution[0], resolution[1]);
			}
			else {
				ImGui::Text("mage_black_texture");
				ImGui::Text("1 x 1 texel");
			}

			//-----------------------------------------------------------------
			// Normal Texture
			//-----------------------------------------------------------------
			const auto normal_tex = material.GetNormalTexture();
			if (normal_tex) {
				const UTF16toUTF8 guid(normal_tex->GetGuid());
				ImGui::Text(guid.c_str());
				const auto resolution
					= rendering::GetTexture2DSize(*material.GetNormalSRV());
				ImGui::Text("%u x %u texels", resolution[0], resolution[1]);
			}
		}

		void DrawWidget(rendering::Model& model) {
			//-----------------------------------------------------------------
			// Texture transform
			//-----------------------------------------------------------------
			DrawWidget(model.GetTextureTransform());

			ImGui::Separator();

			//-----------------------------------------------------------------
			// Material
			//-----------------------------------------------------------------
			DrawWidget(model.GetMaterial());

			//-----------------------------------------------------------------
			// Occlusion
			//-----------------------------------------------------------------
			auto light_occlusion = model.OccludesLight();
			ImGui::Checkbox("Occludes Light", &light_occlusion);
			model.SetLightOcclusion(light_occlusion);
		}

		void DrawWidget(SpriteTransform2D& transform,
						const F32x2& display_resolution,
						const F32x2& texture_resolution) {

			ImGui::Text("Sprite Transform:");

			static bool normalization = false;

			//-----------------------------------------------------------------
			// Translation
			//-----------------------------------------------------------------
			if (normalization) {
				auto translation = AbsoluteToNormalized(transform.GetTranslationView(),
														display_resolution);
				ImGui::InputFloat2("Translation", translation.data());
				transform.SetTranslation(
					NormalizedToAbsolute(translation, display_resolution));
			}
			else {
				auto translation = transform.GetTranslationView();
				ImGui::InputFloat2("Translation", translation.data());
				transform.SetTranslation(translation);
			}

			//-----------------------------------------------------------------
			// Depth
			//-----------------------------------------------------------------
			auto depth = transform.GetDepth();
			ImGui::InputFloat("Depth", &depth);
			transform.SetDepth(depth);

			//-----------------------------------------------------------------
			// Rotation Origin
			//-----------------------------------------------------------------
			if (normalization) {
				auto rotation_origin = AbsoluteToNormalized(
					transform.GetRotationOriginView(), texture_resolution);
				ImGui::InputFloat2("Rotation Origin", rotation_origin.data());
				transform.SetRotationOrigin(
					NormalizedToAbsolute(rotation_origin, texture_resolution));
			}
			else {
				auto rotation_origin = transform.GetRotationOriginView();
				ImGui::InputFloat2("Rotation Origin", rotation_origin.data());
				transform.SetRotationOrigin(rotation_origin);
			}

			//-----------------------------------------------------------------
			// Rotation
			//-----------------------------------------------------------------
			auto rotation = transform.GetRotation();
			ImGui::DragFloat("Rotation", &rotation, 0.0001f, -XM_PI, XM_PI);
			transform.SetRotation(rotation);

			//-----------------------------------------------------------------
			// Scale
			//-----------------------------------------------------------------
			auto scale = transform.GetScaleView();
			ImGui::InputFloat2("Scale", scale.data());
			transform.SetScale(scale);

			ImGui::Checkbox("Normalization", &normalization);
		}

		void DrawWidget(rendering::SpriteImage& sprite,
						const U32x2& display_resolution) {

			using rendering::SpriteEffect;

			const auto base_color_tex = sprite.GetBaseColorTexture();
			const auto texture_resolution = !base_color_tex ? U32x2(1u, 1u)
				: rendering::GetTexture2DSize(*sprite.GetBaseColorSRV());

			//-----------------------------------------------------------------
			// Sprite transform
			//-----------------------------------------------------------------
			DrawWidget(sprite.GetSpriteTransform(),
					   static_cast< F32x2 >(display_resolution),
					   static_cast< F32x2 >(texture_resolution));

			ImGui::Separator();

			//-----------------------------------------------------------------
			// Base Color
			//-----------------------------------------------------------------
			SRGBA color(sprite.GetBaseColor());
			ImGui::ColorEdit4("Base Color", color.data());
			sprite.GetBaseColor() = RGBA(color);

			//-----------------------------------------------------------------
			// Base Color Texture
			//-----------------------------------------------------------------
			if (base_color_tex) {
				const UTF16toUTF8 guid(base_color_tex->GetGuid());
				ImGui::Text(guid.c_str());
				ImGui::Text("%u x %u texels",
							texture_resolution[0], texture_resolution[1]);
			}
			else {
				ImGui::Text("mage_black_texture");
				ImGui::Text("1 x 1 texel");
			}

			//-----------------------------------------------------------------
			// Sprite Effects
			//-----------------------------------------------------------------
			static constexpr const_zstring sprite_effect_names[] = {
				"None",
				"Mirror X",
				"Mirror Y",
				"Mirror XY"
			};
			static constexpr SpriteEffect sprite_effects[] = {
				SpriteEffect::None,
				SpriteEffect::MirrorX,
				SpriteEffect::MirrorY,
				SpriteEffect::MirrorXY
			};
			static_assert(std::size(sprite_effect_names) == std::size(sprite_effects));

			auto sprite_effect_index = static_cast< int >(sprite.GetSpriteEffects());
			ImGui::Combo("Sprite Effects", &sprite_effect_index, sprite_effect_names,
						 static_cast< int >(std::size(sprite_effect_names)));
			sprite.SetSpriteEffects(sprite_effects[sprite_effect_index]);
		}

		void DrawWidget(rendering::SpriteText& sprite,
						const U32x2& display_resolution) {

			using rendering::SpriteEffect;
			using rendering::SpriteText;

			const auto font = sprite.GetFont();
			const auto texture_resolution = !font ? U32x2(1u, 1u)
				: rendering::GetTexture2DSize(*sprite.GetFontSRV());

			//-----------------------------------------------------------------
			// Sprite transform
			//-----------------------------------------------------------------
			DrawWidget(sprite.GetSpriteTransform(),
					   static_cast< F32x2 >(display_resolution),
					   static_cast< F32x2 >(texture_resolution));

			ImGui::Separator();

			//-----------------------------------------------------------------
			// Sprite font
			//-----------------------------------------------------------------
			if (font) {
				const UTF16toUTF8 guid(font->GetGuid());
				ImGui::Text(guid.c_str());
				ImGui::Text("%u x %u texels",
							texture_resolution[0], texture_resolution[1]);
			}
			else {
				ImGui::Text("no font");
				ImGui::Text("1 x 1 texel");
			}

			//-----------------------------------------------------------------
			// Sprite effects
			//-----------------------------------------------------------------
			static constexpr const_zstring sprite_effect_names[] = {
				{ "None"    },
				{ "Flip X"  },
				{ "Flip Y"  },
				{ "Flip XY" }
			};
			static constexpr SpriteEffect sprite_effects[] = {
				{ SpriteEffect::None     },
				{ SpriteEffect::MirrorX  },
				{ SpriteEffect::MirrorY  },
				{ SpriteEffect::MirrorXY }
			};
			static_assert(std::size(sprite_effect_names) == std::size(sprite_effects));

			auto sprite_effect_index = static_cast< int >(sprite.GetSpriteEffects());
			ImGui::Combo("Sprite Effects", &sprite_effect_index, sprite_effect_names,
						 static_cast< int >(std::size(sprite_effect_names)));
			sprite.SetSpriteEffects(sprite_effects[sprite_effect_index]);

			//-----------------------------------------------------------------
			// Text effect color
			//-----------------------------------------------------------------
			SRGBA color(sprite.GetTextEffectColor());
			ImGui::ColorEdit4("Text Effect Color", color.data());
			sprite.GetTextEffectColor() = RGBA(color);

			//-----------------------------------------------------------------
			// Text effects
			//-----------------------------------------------------------------
			static constexpr const_zstring text_effect_names[] = {
				"None",
				"Drop Shadow",
				"Outline"
			};
			static constexpr SpriteText::TextEffect text_effects[] = {
				SpriteText::TextEffect::None,
				SpriteText::TextEffect::DropShadow,
				SpriteText::TextEffect::Outline
			};
			static_assert(std::size(text_effect_names) == std::size(text_effects));

			auto text_effect_index = static_cast< int >(sprite.GetTextEffect());
			ImGui::Combo("Text Effect", &text_effect_index, text_effect_names,
						 static_cast< int >(std::size(text_effect_names)));
			sprite.SetTextEffect(text_effects[text_effect_index]);
		}

		void DrawWidget(Transform& transform) {
			//-----------------------------------------------------------------
			// Translation
			//-----------------------------------------------------------------
			if (auto translation = transform.GetTranslationView();
				ImGui::InputFloat3("Translation", translation.data())) {

				transform.SetTranslation(translation);
			}

			//-----------------------------------------------------------------
			// Rotation
			//-----------------------------------------------------------------
			if (auto rotation = transform.GetRotationView();
				ImGui::InputFloat3("Rotation", rotation.data())) {

				transform.SetRotation(rotation);
			}

			//-----------------------------------------------------------------
			// Scale
			//-----------------------------------------------------------------
			if (auto scale = transform.GetScaleView();
				ImGui::InputFloat3("Scale", scale.data())) {

				transform.SetScale(scale);
			}
		}

		void DrawWidget(Node& node,
						const U32x2& display_resolution) {

			using rendering::OrthographicCamera;
			using rendering::PerspectiveCamera;
			using rendering::AmbientLight;
			using rendering::DirectionalLight;
			using rendering::OmniLight;
			using rendering::SpotLight;
			using rendering::Model;
			using rendering::SpriteImage;
			using rendering::SpriteText;

			//-----------------------------------------------------------------
			// Name
			//-----------------------------------------------------------------
			char buffer[128];
			WriteTo(buffer, "{}", node.GetName());
			if (ImGui::InputText("", buffer, std::size(buffer))) {
				node.SetName(std::string(buffer));
			}

			ImGui::SameLine();

			//-----------------------------------------------------------------
			// State
			//-----------------------------------------------------------------
			if (auto active = (State::Active == node.GetState());
				ImGui::Checkbox("Active", &active)) {
				node.SetState(active ? State::Active : State::Passive);
			}

			ImGui::Separator();

			//-----------------------------------------------------------------
			// Transform
			//-----------------------------------------------------------------
			if (ImGui::TreeNode("Transform")) {
				DrawWidget(node.GetTransform());
				ImGui::TreePop();
			}

			//-----------------------------------------------------------------
			// OrthographicCameras
			//-----------------------------------------------------------------
			node.ForEach< OrthographicCamera >([&display_resolution](OrthographicCamera& camera) {
				char guid[20];
				WriteTo(guid, "{}", camera.GetGuid());
				if (ImGui::TreeNode(guid, "Orthographic Camera")) {

					if (auto active = (State::Active == camera.GetState());
					ImGui::Checkbox("Active", &active)) {
						camera.SetState(active ? State::Active : State::Passive);
					}

					DrawWidget(camera, display_resolution);

					ImGui::TreePop();
				}
			});

			//-----------------------------------------------------------------
			// PerspectiveCameras
			//-----------------------------------------------------------------
			node.ForEach< PerspectiveCamera >([&display_resolution](PerspectiveCamera& camera) {
				char guid[20];
				WriteTo(guid, "{}", camera.GetGuid());
				if (ImGui::TreeNode(guid, "Perspective Camera")) {

					if (auto active = (State::Active == camera.GetState());
					ImGui::Checkbox("Active", &active)) {
						camera.SetState(active ? State::Active : State::Passive);
					}

					DrawWidget(camera, display_resolution);

					ImGui::TreePop();
				}
			});

			//-----------------------------------------------------------------
			// AmbientLights
			//-----------------------------------------------------------------
			node.ForEach< AmbientLight >([](AmbientLight& light) {
				char guid[20];
				WriteTo(guid, "{}", light.GetGuid());
				if (ImGui::TreeNode(guid, "Ambient Light")) {

					if (auto active = (State::Active == light.GetState());
					ImGui::Checkbox("Active", &active)) {
						light.SetState(active ? State::Active : State::Passive);
					}

					DrawWidget(light);

					ImGui::TreePop();
				}
			});

			//-----------------------------------------------------------------
			// DirectionalLights
			//-----------------------------------------------------------------
			node.ForEach< DirectionalLight >([](DirectionalLight& light) {
				char guid[20];
				WriteTo(guid, "{}", light.GetGuid());
				if (ImGui::TreeNode(guid, "Directional Light")) {

					if (auto active = (State::Active == light.GetState());
					ImGui::Checkbox("Active", &active)) {
						light.SetState(active ? State::Active : State::Passive);
					}

					DrawWidget(light);

					ImGui::TreePop();
				}
			});

			//-----------------------------------------------------------------
			// OmniLights
			//-----------------------------------------------------------------
			node.ForEach< OmniLight >([](OmniLight& light) {
				char guid[20];
				WriteTo(guid, "{}", light.GetGuid());
				if (ImGui::TreeNode(guid, "Omni Light")) {

					if (auto active = (State::Active == light.GetState());
					ImGui::Checkbox("Active", &active)) {
						light.SetState(active ? State::Active : State::Passive);
					}

					DrawWidget(light);

					ImGui::TreePop();
				}
			});

			//-----------------------------------------------------------------
			// SpotLights
			//-----------------------------------------------------------------
			node.ForEach< SpotLight >([](SpotLight& light) {
				char guid[20];
				WriteTo(guid, "{}", light.GetGuid());
				if (ImGui::TreeNode(guid, "Spotlight")) {

					if (auto active = (State::Active == light.GetState());
					ImGui::Checkbox("Active", &active)) {
						light.SetState(active ? State::Active : State::Passive);
					}

					DrawWidget(light);

					ImGui::TreePop();
				}
			});

			//-----------------------------------------------------------------
			// Models
			//-----------------------------------------------------------------
			node.ForEach< Model >([](Model& model) {
				char guid[20];
				WriteTo(guid, "{}", model.GetGuid());
				if (ImGui::TreeNode(guid, "Model")) {

					if (auto active = (State::Active == model.GetState());
					ImGui::Checkbox("Active", &active)) {
						model.SetState(active ? State::Active : State::Passive);
					}

					DrawWidget(model);

					ImGui::TreePop();
				}
			});

			//-----------------------------------------------------------------
			// SpriteImages
			//-----------------------------------------------------------------
			node.ForEach< SpriteImage >([&display_resolution](SpriteImage& sprite) {
				char guid[20];
				WriteTo(guid, "{}", sprite.GetGuid());
				if (ImGui::TreeNode(guid, "Sprite Image")) {

					if (auto active = (State::Active == sprite.GetState());
					ImGui::Checkbox("Active", &active)) {
						sprite.SetState(active ? State::Active : State::Passive);
					}

					DrawWidget(sprite, display_resolution);

					ImGui::TreePop();
				}
			});

			//-----------------------------------------------------------------
			// SpriteTexts
			//-----------------------------------------------------------------
			node.ForEach< SpriteText >([&display_resolution](SpriteText& sprite) {
				char guid[20];
				WriteTo(guid, "{}", sprite.GetGuid());
				if (ImGui::TreeNode(guid, "Sprite Text")) {

					if (auto active = (State::Active == sprite.GetState());
					ImGui::Checkbox("Active", &active)) {
						sprite.SetState(active ? State::Active : State::Passive);
					}

					DrawWidget(sprite, display_resolution);

					ImGui::TreePop();
				}
			});
		}

		void DrawGraph(Node& node,
					   ProxyPtr< Node >& selected) {

			char guid[20];
			WriteTo(guid, "{}", node.GetGuid());
			char name[128];
			WriteTo(name, "{}", node.GetName());

			if (node.ContainsChilds()) {
				static constexpr ImGuiTreeNodeFlags node_flags
					= ImGuiTreeNodeFlags_OpenOnArrow
					| ImGuiTreeNodeFlags_OpenOnDoubleClick;

				const ImGuiTreeNodeFlags flags = (node.Get() == selected)
					? node_flags | ImGuiTreeNodeFlags_Selected : node_flags;

				const auto node_open = ImGui::TreeNodeEx(guid, flags, name);

				if (ImGui::IsItemClicked()) {
					selected = node.Get();
				}

				if (node_open) {
					node.ForEachChild([&selected](Node& child) {
						DrawGraph(child, selected);
					});

					ImGui::TreePop();
				}
			}
			else {
				static constexpr ImGuiTreeNodeFlags node_flags
					= ImGuiTreeNodeFlags_OpenOnArrow
					| ImGuiTreeNodeFlags_OpenOnDoubleClick
					| ImGuiTreeNodeFlags_Leaf
					| ImGuiTreeNodeFlags_NoTreePushOnOpen;

				const ImGuiTreeNodeFlags flags = (node.Get() == selected)
					? node_flags | ImGuiTreeNodeFlags_Selected : node_flags;

				ImGui::TreeNodeEx(guid, flags, name);
				if (ImGui::IsItemClicked()) {
					selected = node.Get();
				}
			}
		}

		void DrawGraph(Scene& scene,
					   ProxyPtr< Node >& selected) {

			ImGui::Begin("Scene Graph");
			// Increase spacing to differentiate leaves from expanded contents.
			ImGui::PushStyleVar(ImGuiStyleVar_IndentSpacing,
								ImGui::GetFontSize() * 3.0f);

			scene.ForEach< Node >([&selected](Node& node) {
				if (!node.HasParent()) {
					DrawGraph(node, selected);
				}
			});

			ImGui::PopStyleVar();
			ImGui::End();
		}

		void DrawInspector(ProxyPtr< Node >& selected,
						   const U32x2& display_resolution) {

			ImGui::Begin("Inspector");

			if (selected && State::Terminated != selected->GetState()) {
				DrawWidget(*selected, display_resolution);
			}

			ImGui::End();
		}
	}

	//-------------------------------------------------------------------------
	// EditorScript
	//-------------------------------------------------------------------------
	#pragma region

	EditorScript::EditorScript() noexcept
		: m_visible(false), m_selected() {}

	EditorScript::EditorScript(const EditorScript& script) noexcept = default;

	EditorScript::EditorScript(EditorScript&& script) noexcept = default;

	EditorScript::~EditorScript() = default;

	EditorScript& EditorScript
		::operator=(const EditorScript& script) noexcept = default;

	EditorScript& EditorScript
		::operator=(EditorScript&& script) noexcept = default;

	void EditorScript::Load([[maybe_unused]] Engine& engine) {
		m_selected = nullptr;
	}

	void EditorScript::Update([[maybe_unused]] Engine& engine) {
		const auto& input_manager = engine.GetInputManager();
		const auto& keyboard      = input_manager.GetKeyboard();

		if (keyboard.IsActivated(DIK_F4)) {
			m_visible = !m_visible;
		}
		if (m_visible) {
			return;
		}

		const auto scene = engine.GetScene();
		DrawGraph(*scene, m_selected);

		const auto config = engine.GetRenderingManager().GetDisplayConfiguration();
		const auto display_resolution = config.GetDisplayResolution();
		DrawInspector(m_selected, display_resolution);
	}

	#pragma endregion
}