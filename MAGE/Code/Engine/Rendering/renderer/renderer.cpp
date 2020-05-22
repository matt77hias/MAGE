//-----------------------------------------------------------------------------
// Engine Includes
//-----------------------------------------------------------------------------
#pragma region

#include "renderer\renderer.hpp"
#include "renderer\output_manager.hpp"
#include "renderer\pass\aa_pass.hpp"
#include "renderer\pass\back_buffer_pass.hpp"
#include "renderer\pass\bounding_volume_pass.hpp"
#include "renderer\pass\deferred_pass.hpp"
#include "renderer\pass\depth_pass.hpp"
#include "renderer\pass\forward_pass.hpp"
#include "renderer\pass\lbuffer_pass.hpp"
#include "renderer\pass\postprocess_pass.hpp"
#include "renderer\pass\sky_pass.hpp"
#include "renderer\pass\sprite_pass.hpp"
#include "renderer\pass\voxelization_pass.hpp"
#include "renderer\pass\voxel_grid_pass.hpp"
#include "renderer\buffer\scene_buffer.hpp"
#include "ImGui\imgui_impl_dx11.h"

// Include HLSL bindings.
#include "hlsl.hpp"

#pragma endregion

//-----------------------------------------------------------------------------
// Engine Declarations and Definitions
//-----------------------------------------------------------------------------
namespace mage::rendering {

	//-------------------------------------------------------------------------
	// Renderer::Impl
	//-------------------------------------------------------------------------
	#pragma region

	/**
	 A class of renderers.
	 */
	class Renderer::Impl {

	public:

		//---------------------------------------------------------------------
		// Constructors and Destructors
		//---------------------------------------------------------------------

		/**
		 Constructs a renderer.

		 @param[in,out]	device
						A reference to the device.
		 @param[in,out]	device_context
						A reference to the device context.
		 @param[in,out]	display_configuration
						A reference to the display configuration.
		 @param[in,out]	swap_chain
						A reference to the swap chain.
		 @param[in,out]	resource_manager
						A pointer to the resource manager.
		 */
		explicit Impl(ID3D11Device& device,
					  ID3D11DeviceContext& device_context,
					  DisplayConfiguration& display_configuration,
					  SwapChain& swap_chain,
					  ResourceManager& resource_manager);

		/**
		 Constructs a renderer from the given renderer.

		 @param[in]		renderer
						A reference to the renderer to copy.
		 */
		Impl(const Impl& renderer) = delete;

		/**
		 Constructs a renderer by moving the given renderer.

		 @param[in]		renderer
						A reference to the renderer to move.
		 */
		Impl(Impl&& renderer) noexcept;

		/**
		 Destructs this renderer.
		 */
		~Impl();

		//---------------------------------------------------------------------
		// Assignment Operators
		//---------------------------------------------------------------------

		/**
		 Copies the given renderer to this renderer.

		 @param[in]		renderer
						A reference to the renderer to copy.
		 @return		A reference to the copy of the given renderer (i.e.
						this renderer).
		 */
		Impl& operator=(const Impl& renderer) = delete;

		/**
		 Moves the given renderer to this renderer.

		 @param[in]		renderer
						A reference to the renderer to move.
		 @return		A reference to the moved renderer (i.e. this renderer).
		 */
		Impl& operator=(Impl&& renderer) noexcept;

		//---------------------------------------------------------------------
		// Member Methods
		//---------------------------------------------------------------------

		/**
		 Binds the persistent state of this renderer.

		 @throws		Exception
						Failed to bind the persistent state of this renderer.
		 */
		void BindPersistentState();

		/**
		 Renders the given world.

		 @param[in]		world
						A reference to the world.
		 @param[in]		time
						A reference to the game time.
		 @throws		Exception
						Failed to render the world.
		 */
		void Render(const World& world, const GameTime& time);

	private:

		//---------------------------------------------------------------------
		// Member Methods
		//---------------------------------------------------------------------

		void InitializePasses();

		void UpdateBuffers(const World& world, const GameTime& time);

		void UpdateWorldBuffer(const GameTime& time);

		void Render(const World& world, const Camera& camera);

		void XM_CALLCONV RenderForward(const World& world,
									   const Camera& camera,
									   FXMMATRIX world_to_projection);

		void XM_CALLCONV RenderDeferred(const World& world,
										const Camera& camera,
										FXMMATRIX world_to_projection);

		void XM_CALLCONV RenderSolid(const World& world,
									 const Camera& camera,
									 FXMMATRIX world_to_projection);

		void XM_CALLCONV RenderFalseColor(const World& world,
										  const Camera& camera,
										  FXMMATRIX world_to_projection,
										  FalseColor false_color);

		void XM_CALLCONV RenderVoxelGrid(const World& world,
										 const Camera& camera,
										 FXMMATRIX world_to_projection);

		void RenderAA(const Camera& camera);

		void RenderPostProcessing(const Camera& camera);

		//---------------------------------------------------------------------
		// Member Variables
		//---------------------------------------------------------------------

		/**
		 A reference to the display configuration of this renderer.
		 */
		std::reference_wrapper< DisplayConfiguration > m_display_configuration;

		/**
		 A reference to the device of this renderer.
		 */
		std::reference_wrapper< ID3D11Device > m_device;

		/**
		 A reference to the device context of this renderer.
		 */
		std::reference_wrapper< ID3D11DeviceContext > m_device_context;

		/**
		 A reference to the resource manager of this renderer.
		 */
		std::reference_wrapper< ResourceManager > m_resource_manager;

		/**
		 A pointer to the output manager of this rendering manager.
		 */
		UniquePtr< OutputManager > m_output_manager;

		/**
		 A pointer to the state manager of this rendering manager.
		 */
		UniquePtr< StateManager > m_state_manager;

		//---------------------------------------------------------------------
		// Member Variables: Buffers
		//---------------------------------------------------------------------

		/**
		 A pointer to the world buffer of this renderer.
		 */
		ConstantBuffer< WorldBuffer > m_world_buffer;

		//---------------------------------------------------------------------
		// Member Variables: Render Passes
		//---------------------------------------------------------------------

		/**
		 A pointer to the AA pass of this renderer.
		 */
		UniquePtr< AAPass > m_aa_pass;

		/**
		 A pointer to the back buffer pass of this renderer.
		 */
		UniquePtr< BackBufferPass > m_back_buffer_pass;

		/**
		 A pointer to the bounding volume pass of this renderer.
		 */
		UniquePtr< BoundingVolumePass > m_bounding_volume_pass;

		/**
		 A pointer to the deferred pass of this renderer.
		 */
		UniquePtr< DeferredPass > m_deferred_pass;

		/**
		 A pointer to the depth pass of this renderer.
		 */
		UniquePtr< DepthPass > m_depth_pass;

		/**
		 A pointer to the forward pass of this renderer.
		 */
		UniquePtr< ForwardPass > m_forward_pass;

		/**
		 A pointer to the LBuffer pass of this renderer.
		 */
		UniquePtr< LBufferPass >  m_lbuffer_pass;

		/**
		 A pointer to the post-process pass of this renderer.
		 */
		UniquePtr< PostProcessPass > m_postprocess_pass;

		/**
		 A pointer to the sky pass of this renderer.
		 */
		UniquePtr< SkyPass > m_sky_pass;

		/**
		 A pointer to the sprite pass of this renderer.
		 */
		UniquePtr< SpritePass > m_sprite_pass;

		/**
		 A pointer to the voxel grid pass of this renderer.
		 */
		UniquePtr< VoxelGridPass > m_voxel_grid_pass;

		/**
		 A pointer to the voxelization pass of this renderer.
		 */
		UniquePtr< VoxelizationPass > m_voxelization_pass;
	};

	Renderer::Impl::Impl(ID3D11Device& device,
						 ID3D11DeviceContext& device_context,
						 DisplayConfiguration& display_configuration,
						 SwapChain& swap_chain,
						 ResourceManager& resource_manager)
		: m_display_configuration(display_configuration),
		m_device(device),
		m_device_context(device_context),
		m_resource_manager(resource_manager),
		m_output_manager(MakeUnique< OutputManager >(device,
													 display_configuration,
													 swap_chain)),
		m_state_manager(MakeUnique< StateManager >(device)),
		m_world_buffer(device),
		m_aa_pass(),
		m_back_buffer_pass(),
		m_bounding_volume_pass(),
		m_deferred_pass(),
		m_depth_pass(),
		m_forward_pass(),
		m_lbuffer_pass(),
		m_postprocess_pass(),
		m_sky_pass(),
		m_sprite_pass(),
		m_voxel_grid_pass(),
		m_voxelization_pass() {

		InitializePasses();
	}

	Renderer::Impl::Impl(Impl&& world_renderer) noexcept = default;

	Renderer::Impl::~Impl() = default;

	Renderer::Impl& Renderer::Impl
		::operator=(Impl&& world_renderer) noexcept = default;

	void Renderer::Impl::InitializePasses() {
		m_aa_pass = MakeUnique< AAPass >(m_device_context,
										 *m_state_manager.get(),
										 m_resource_manager);

		m_back_buffer_pass = MakeUnique< BackBufferPass >(m_device_context,
														  *m_state_manager.get(),
														  m_resource_manager);

		m_bounding_volume_pass = MakeUnique< BoundingVolumePass >(m_device,
																  m_device_context,
																  *m_state_manager.get(),
																  m_resource_manager);
		m_deferred_pass = MakeUnique< DeferredPass >(m_device_context,
													 *m_state_manager.get(),
													 m_resource_manager);

		m_depth_pass = MakeUnique< DepthPass >(m_device,
											   m_device_context,
											   *m_state_manager.get(),
											   m_resource_manager);

		m_forward_pass = MakeUnique< ForwardPass >(m_device,
												   m_device_context,
												   *m_state_manager.get(),
												   m_resource_manager);

		m_lbuffer_pass = MakeUnique< LBufferPass >(m_device,
												   m_device_context,
												   *m_state_manager.get(),
												   m_resource_manager);

		m_postprocess_pass = MakeUnique< PostProcessPass >(m_device_context,
														   *m_state_manager.get(),
														   m_resource_manager);

		m_sky_pass = MakeUnique< SkyPass >(m_device_context,
										   *m_state_manager.get(),
										   m_resource_manager);

		m_sprite_pass = MakeUnique< SpritePass >(m_device,
												 m_device_context,
												 *m_state_manager.get(),
												 m_resource_manager);

		m_voxel_grid_pass = MakeUnique< VoxelGridPass >(m_device_context,
														*m_state_manager.get(),
														m_resource_manager);

		m_voxelization_pass = MakeUnique< VoxelizationPass >(m_device,
															 m_device_context,
															 *m_state_manager.get(),
															 m_resource_manager);
	}

	void Renderer::Impl::BindPersistentState() {
		m_state_manager->BindPersistentState(m_device_context);
	}

	void Renderer::Impl::Render(const World& world, const GameTime& time) {
		// Update the buffers.
		UpdateBuffers(world, time);

		// Bind the world buffer.
		m_world_buffer.Bind< Pipeline >(m_device_context, SLOT_CBUFFER_WORLD);

		m_output_manager->BindBegin(m_device_context);

		// Render the world for each camera.
		world.ForEach< Camera >([this, &world](const Camera& camera) {
			if (State::Active != camera.GetState()) {
				return;
			}

			// Render the world.
			Render(world, camera);
		});

		m_output_manager->BindGUI(m_device_context);

		// Bind the maximum viewport.
		const Viewport viewport(
			m_display_configuration.get().GetDisplayResolution());
		viewport.Bind(m_device_context);

		//---------------------------------------------------------------------
		// Sprite Pass
		//---------------------------------------------------------------------
		m_sprite_pass->Render(world);

		// GUI
		ImGui::Render();
		ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());

		m_output_manager->BindEnd(m_device_context);

		//---------------------------------------------------------------------
		// Back Buffer
		//---------------------------------------------------------------------
		m_back_buffer_pass->Render();
	}

	void Renderer::Impl::UpdateBuffers(const World& world,
									   const GameTime& time) {
		// Update the world buffer.
		UpdateWorldBuffer(time);

		// Update the buffer of each camera.
		world.ForEach< Camera >([this](const Camera& camera) {
			if (State::Active == camera.GetState()) {
				camera.UpdateBuffer(m_device_context,
									m_display_configuration.get().GetAA());
			}
		});

		// Update the buffer of each model.
		world.ForEach< Model >([this](const Model& model) {
			if (State::Active == model.GetState()) {
				model.UpdateBuffer(m_device_context);
			}
		});
	}

	void Renderer::Impl::UpdateWorldBuffer(const GameTime& time) {
		WorldBuffer buffer;

		// Display
		{
			buffer.m_display_resolution
				= m_display_configuration.get().GetDisplayResolution();
			buffer.m_display_inv_resolution = XMStore< F32x2 >(
				XMVectorReciprocal(XMLoad(buffer.m_display_resolution)));
		}

		// SS Display
		{
			buffer.m_ss_display_resolution
				= m_display_configuration.get().GetSSDisplayResolution();
			buffer.m_ss_display_inv_resolution = XMStore< F32x2 >(
				XMVectorReciprocal(XMLoad(buffer.m_ss_display_resolution)));
		}

		// Voxelization
		{
			buffer.m_voxel_grid_center
				= VoxelizationSettings::GetVoxelGridCenter();
			buffer.m_voxel_texture_max_mip_level
				= VoxelizationSettings::GetMaxVoxelTextureMipLevel();
			buffer.m_voxel_grid_resolution
				= VoxelizationSettings::GetVoxelGridResolution();
			buffer.m_voxel_grid_inv_resolution
				= 1.0f / buffer.m_voxel_grid_resolution;
			buffer.m_voxel_size
				= VoxelizationSettings::GetVoxelSize();
			buffer.m_voxel_inv_size
				= 1.0f / buffer.m_voxel_size;
		}

		// Time
		{
			buffer.m_time = static_cast< F32 >(time.GetWallClockTotalDeltaTime().count());
		}

		// Gamma
		{
			buffer.m_inv_gamma = 1.0f / CameraSettings::GetGamma();
		}

		// Update the world buffer.
		m_world_buffer.UpdateData(m_device_context, buffer);
	}

	void Renderer::Impl::Render(const World& world, const Camera& camera) {
		// Bind the camera to the pipeline.
		camera.BindBuffer< Pipeline >(m_device_context,
									  SLOT_CBUFFER_PRIMARY_CAMERA);

		// Obtain the world-to-projection transformation matrix of the
		// camera for view frustum culling.
		const auto& transform            = camera.GetOwner()->GetTransform();
		const auto  world_to_camera      = transform.GetWorldToObjectMatrix();
		const auto  camera_to_projection = camera.GetCameraToProjectionMatrix();
		const auto  world_to_projection  = world_to_camera * camera_to_projection;

		const auto  render_mode          = camera.GetSettings().GetRenderMode();

		m_output_manager->BindBeginViewport(m_device_context);

		//---------------------------------------------------------------------
		// RenderMode
		//---------------------------------------------------------------------
		switch (render_mode) {

		case RenderMode::Forward: {
			RenderForward(world, camera, world_to_projection);
			break;
		}

		case RenderMode::Deferred: {
			RenderDeferred(world, camera, world_to_projection);
			break;
		}

		case RenderMode::Solid: {
			RenderSolid(world, camera, world_to_projection);
			break;
		}

		case RenderMode::VoxelGrid: {
			RenderVoxelGrid(world, camera, world_to_projection);
			break;
		}

		case RenderMode::FalseColor_BaseColor: {
			RenderFalseColor(world, camera, world_to_projection,
							 FalseColor::BaseColor);
			break;
		}
		case RenderMode::FalseColor_BaseColorCoefficient: {
			RenderFalseColor(world, camera, world_to_projection,
							 FalseColor::BaseColorCoefficient);
			break;
		}
		case RenderMode::FalseColor_BaseColorTexture: {
			RenderFalseColor(world, camera, world_to_projection,
							 FalseColor::BaseColorTexture);
			break;
		}
		case RenderMode::FalseColor_Material: {
			RenderFalseColor(world, camera, world_to_projection,
							 FalseColor::Material);
			break;
		}
		case RenderMode::FalseColor_MaterialCoefficient: {
			RenderFalseColor(world, camera, world_to_projection,
							 FalseColor::MaterialCoefficient);
			break;
		}
		case RenderMode::FalseColor_MaterialTexture: {
			RenderFalseColor(world, camera, world_to_projection,
							 FalseColor::MaterialTexture);
			break;
		}
		case RenderMode::FalseColor_Roughness: {
			RenderFalseColor(world, camera, world_to_projection,
							 FalseColor::Roughness);
			break;
		}
		case RenderMode::FalseColor_RoughnessCoefficient: {
			RenderFalseColor(world, camera, world_to_projection,
							 FalseColor::RoughnessCoefficient);
			break;
		}
		case RenderMode::FalseColor_RoughnessTexture: {
			RenderFalseColor(world, camera, world_to_projection,
							 FalseColor::RoughnessTexture);
			break;
		}
		case RenderMode::FalseColor_Metalness: {
			RenderFalseColor(world, camera, world_to_projection,
							 FalseColor::Metalness);
			break;
		}
		case RenderMode::FalseColor_MetalnessCoefficient: {
			RenderFalseColor(world, camera, world_to_projection,
							 FalseColor::MetalnessCoefficient);
			break;
		}
		case RenderMode::FalseColor_MetalnessTexture: {
			RenderFalseColor(world, camera, world_to_projection,
							 FalseColor::MetalnessTexture);
			break;
		}
		case RenderMode::FalseColor_ShadingNormal: {
			RenderFalseColor(world, camera, world_to_projection,
							 FalseColor::ShadingNormal);
			break;
		}
		case RenderMode::FalseColor_TSNMShadingNormal: {
			RenderFalseColor(world, camera, world_to_projection,
							 FalseColor::TSNMShadingNormal);
			break;
		}
		case RenderMode::FalseColor_Depth: {
			RenderFalseColor(world, camera, world_to_projection,
							 FalseColor::Depth);
			break;
		}
		case RenderMode::FalseColor_Distance: {
			RenderFalseColor(world, camera, world_to_projection,
							 FalseColor::Distance);
			break;
		}
		case RenderMode::FalseColor_UV: {
			RenderFalseColor(world, camera, world_to_projection,
							 FalseColor::UV);
			break;
		}

		default: {
			const Viewport viewport(camera.GetViewport(),
									m_display_configuration.get().GetAA());
			viewport.Bind(m_device_context);
			m_output_manager->BindBeginForward(m_device_context);

			break;
		}

		}

		//---------------------------------------------------------------------
		// RenderLayer
		//---------------------------------------------------------------------
		const auto& settings = camera.GetSettings();
		if (settings.ContainsRenderLayer(RenderLayer::Wireframe)) {
			m_forward_pass->RenderWireframe(world, world_to_projection);
		}
		if (settings.ContainsRenderLayer(RenderLayer::AABB)) {
			m_bounding_volume_pass->Render(world, world_to_projection);
		}

		m_output_manager->BindEndForward(m_device_context);

		//---------------------------------------------------------------------
		// Anti-aliasing
		//---------------------------------------------------------------------
		RenderAA(camera);

		//---------------------------------------------------------------------
		// Post-processing
		//---------------------------------------------------------------------
		RenderPostProcessing(camera);
	}

	void XM_CALLCONV Renderer::Impl::RenderForward(const World& world,
												   const Camera& camera,
												   FXMMATRIX world_to_projection) {

		const auto vct = camera.GetSettings().GetVoxelizationSettings().UsesVCT();

		//---------------------------------------------------------------------
		// LBuffer + Voxelization
		//---------------------------------------------------------------------
		if (vct) {
			const auto world_to_voxel
				= VoxelizationSettings::GetWorldToVoxelMatrix();

			// TODO: world_to_projection + world_to_voxel for culling
			m_lbuffer_pass->Render(world, world_to_projection);

			const auto voxel_grid_resolution
				= VoxelizationSettings::GetVoxelGridResolution();

			m_voxelization_pass->Render(world, world_to_voxel,
										voxel_grid_resolution);
		}
		else {
			m_lbuffer_pass->Render(world, world_to_projection);
		}

		const Viewport viewport(camera.GetViewport(),
								m_display_configuration.get().GetAA());
		viewport.Bind(m_device_context);
		m_output_manager->BindBeginForward(m_device_context);

		//---------------------------------------------------------------------
		// Depth
		//---------------------------------------------------------------------
		if (vct) {
			const auto& transform            = camera.GetOwner()->GetTransform();
			const auto  world_to_camera      = transform.GetWorldToObjectMatrix();
			const auto  camera_to_projection = camera.GetCameraToProjectionMatrix();
			m_depth_pass->Render(world, world_to_camera, camera_to_projection);
		}

		//---------------------------------------------------------------------
		// Forward: opaque fragments
		//---------------------------------------------------------------------
		m_forward_pass->Render(world, world_to_projection,
							   camera.GetSettings().GetBRDF(), vct);

		//---------------------------------------------------------------------
		// Sky
		//---------------------------------------------------------------------
		m_sky_pass->Render(camera.GetSettings().GetSky().GetSRV());

		//---------------------------------------------------------------------
		// Forward: transparent fragments
		//---------------------------------------------------------------------
		m_forward_pass->RenderTransparent(world, world_to_projection,
										  camera.GetSettings().GetBRDF(), vct);
	}

	void XM_CALLCONV Renderer::Impl::RenderDeferred(const World& world,
													const Camera& camera,
													FXMMATRIX world_to_projection) {

		const auto vct = camera.GetSettings().GetVoxelizationSettings().UsesVCT();

		//---------------------------------------------------------------------
		// LBuffer + Voxelization
		//---------------------------------------------------------------------
		if (vct) {
			const auto world_to_voxel
				= VoxelizationSettings::GetWorldToVoxelMatrix();

			// TODO: world_to_projection + world_to_voxel for culling
			m_lbuffer_pass->Render(world, world_to_projection);

			const auto voxel_grid_resolution
				= VoxelizationSettings::GetVoxelGridResolution();

			m_voxelization_pass->Render(world, world_to_voxel,
										voxel_grid_resolution);
		}
		else {
			m_lbuffer_pass->Render(world, world_to_projection);
		}

		const Viewport viewport(camera.GetViewport(),
								m_display_configuration.get().GetAA());
		viewport.Bind(m_device_context);
		m_output_manager->BindBeginGBuffer(m_device_context);

		//---------------------------------------------------------------------
		// GBuffer: opaque fragments
		//---------------------------------------------------------------------
		m_forward_pass->RenderGBuffer(world, world_to_projection);

		m_output_manager->BindEndGBuffer(m_device_context);
		m_output_manager->BindBeginDeferred(m_device_context);

		//---------------------------------------------------------------------
		// Deferred: opaque fragments
		//---------------------------------------------------------------------
		if (m_display_configuration.get().UsesMSAA()) {
			m_deferred_pass->Render(camera.GetSettings().GetBRDF(), vct);
		}
		else {
			m_deferred_pass->Dispatch(viewport.GetSize(),
									  camera.GetSettings().GetBRDF(), vct);
		}

		m_output_manager->BindEndDeferred(m_device_context);
		m_output_manager->BindBeginForward(m_device_context);

		//---------------------------------------------------------------------
		// Forward: emissive fragments.
		//---------------------------------------------------------------------
		m_forward_pass->RenderEmissive(world, world_to_projection);

		//---------------------------------------------------------------------
		// Perform a sky pass.
		//---------------------------------------------------------------------
		m_sky_pass->Render(camera.GetSettings().GetSky().GetSRV());

		//---------------------------------------------------------------------
		// Forward: transparent fragments
		//---------------------------------------------------------------------
		m_forward_pass->RenderTransparent(world, world_to_projection,
										  camera.GetSettings().GetBRDF(), vct);
	}

	void XM_CALLCONV Renderer::Impl::RenderSolid(const World& world,
												 const Camera& camera,
												 FXMMATRIX world_to_projection) {

		//---------------------------------------------------------------------
		// LBuffer
		//---------------------------------------------------------------------
		m_lbuffer_pass->Render(world, world_to_projection);

		const Viewport viewport(camera.GetViewport(),
								m_display_configuration.get().GetAA());
		viewport.Bind(m_device_context);
		m_output_manager->BindBeginForward(m_device_context);

		//---------------------------------------------------------------------
		// Forward
		//---------------------------------------------------------------------
		m_forward_pass->RenderSolid(world, world_to_projection);
	}

	void XM_CALLCONV Renderer::Impl::RenderFalseColor(const World& world,
													  const Camera& camera,
													  FXMMATRIX world_to_projection,
													  FalseColor false_color) {

		const Viewport viewport(camera.GetViewport(),
								m_display_configuration.get().GetAA());
		viewport.Bind(m_device_context);
		m_output_manager->BindBeginForward(m_device_context);

		//---------------------------------------------------------------------
		// Forward
		//---------------------------------------------------------------------
		m_forward_pass->RenderFalseColor(world, world_to_projection, false_color);
	}

	void XM_CALLCONV Renderer::Impl::RenderVoxelGrid(const World& world,
													 const Camera& camera,
													 FXMMATRIX world_to_projection) {

		//---------------------------------------------------------------------
		// LBuffer
		//---------------------------------------------------------------------
		m_lbuffer_pass->Render(world, world_to_projection);

		//---------------------------------------------------------------------
		// Voxelization
		//---------------------------------------------------------------------
		const auto world_to_voxel
			= VoxelizationSettings::GetWorldToVoxelMatrix();
		const auto voxel_grid_resolution
			= VoxelizationSettings::GetVoxelGridResolution();
		m_voxelization_pass->Render(world, world_to_voxel,
									voxel_grid_resolution);


		const Viewport viewport(camera.GetViewport(),
								m_display_configuration.get().GetAA());
		viewport.Bind(m_device_context);
		m_output_manager->BindBeginForward(m_device_context);

		//---------------------------------------------------------------------
		// Voxel Grid
		//---------------------------------------------------------------------
		m_voxel_grid_pass->Render(voxel_grid_resolution);
	}

	void Renderer::Impl::RenderPostProcessing(const Camera& camera) {
		const auto& viewport = camera.GetViewport();
		viewport.Bind(m_device_context);

		m_output_manager->BindBeginPostProcessing(m_device_context);

		//---------------------------------------------------------------------
		// Depth-of-field
		//---------------------------------------------------------------------
		if (camera.GetLens().HasFiniteAperture()) {
			m_output_manager->BindPingPong(m_device_context);

			m_postprocess_pass->DispatchDOF(viewport.GetSize());
		}

		m_output_manager->BindEndPostProcessing(m_device_context);
		m_output_manager->BindEndViewport(m_device_context);

		//---------------------------------------------------------------------
		// Low Dynamic Range
		//---------------------------------------------------------------------
		m_postprocess_pass->DispatchLDR(viewport.GetSize(),
										camera.GetSettings().GetToneMapping());
	}

	void Renderer::Impl::RenderAA(const Camera& camera) {
		const auto desc = m_display_configuration.get().GetAA();

		switch (desc) {

		case AntiAliasing::FXAA: {
			m_output_manager->BindBeginResolve(m_device_context);

			//-----------------------------------------------------------------
			// AA pre-processing
			//-----------------------------------------------------------------
			m_aa_pass->DispatchPreprocess(camera.GetViewport().GetSize(),
										  AntiAliasing::FXAA);

			m_output_manager->BindEndResolve(m_device_context);
			m_output_manager->BindPingPong(m_device_context);

			//-----------------------------------------------------------------
			// FXAA
			//-----------------------------------------------------------------
			m_aa_pass->Dispatch(camera.GetViewport().GetSize(),
								AntiAliasing::FXAA);

			break;
		}

		case AntiAliasing::MSAA_2x:
		case AntiAliasing::MSAA_4x:
		case AntiAliasing::MSAA_8x:
		case AntiAliasing::SSAA_2x:
		case AntiAliasing::SSAA_3x:
		case AntiAliasing::SSAA_4x: {
			m_output_manager->BindBeginResolve(m_device_context);

			//-----------------------------------------------------------------
			// MSAA/SSAA
			//-----------------------------------------------------------------
			m_aa_pass->Dispatch(camera.GetViewport().GetSize(), desc);

			m_output_manager->BindEndResolve(m_device_context);
			break;
		}

		}
	}

	#pragma endregion

	//-------------------------------------------------------------------------
	// Renderer
	//-------------------------------------------------------------------------
	#pragma region

	Renderer::Renderer(ID3D11Device& device,
					   ID3D11DeviceContext& device_context,
					   DisplayConfiguration& display_configuration,
					   SwapChain& swap_chain,
					   ResourceManager& resource_manager)
		: m_impl(MakeUnique< Impl >(device,
									device_context,
									display_configuration,
									swap_chain,
									resource_manager)) {}

	Renderer::Renderer(Renderer&& renderer) noexcept = default;

	Renderer::~Renderer() = default;

	Renderer& Renderer::operator=(Renderer&& renderer) noexcept = default;

	void Renderer::BindPersistentState() {
		m_impl->BindPersistentState();
	}

	void Renderer::Render(const World& world, const GameTime& time) {
		m_impl->Render(world, time);
	}

	#pragma endregion
}