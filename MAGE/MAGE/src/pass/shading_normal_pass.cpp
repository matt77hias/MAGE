//-----------------------------------------------------------------------------
// Engine Includes
//-----------------------------------------------------------------------------
#pragma region

#include "pass\shading_normal_pass.hpp"
#include "rendering\rendering_state_cache.hpp"
#include "resource\resource_factory.hpp"
#include "math\view_frustum.hpp"
#include "logging\error.hpp"

// Include HLSL bindings.
#include "..\..\shaders\hlsl.hpp"

#pragma endregion

//-----------------------------------------------------------------------------
// Engine Definitions
//-----------------------------------------------------------------------------
namespace mage {

	ShadingNormalPass::ShadingNormalPass()
		: m_device_context(GetImmediateDeviceContext()), 
		m_render_mode(RenderMode::None), 
		m_vs(CreateShadingNormalVS()), 
		m_ps{ CreateShadingNormalPS(), CreateShadingNormalTSNMPS() }, 
		m_bound_ps(PSIndex::Count),
		m_model_buffer(), m_scene_buffer() {}

	ShadingNormalPass::ShadingNormalPass(ShadingNormalPass &&render_pass) = default;

	ShadingNormalPass::~ShadingNormalPass() = default;

	void ShadingNormalPass::BindPS(PSIndex index) noexcept {
		if (m_bound_ps != index) {
			m_ps[static_cast< size_t >(index)]->BindShader(m_device_context);
			m_bound_ps = index;
		}
	}

	void ShadingNormalPass::BindPS(const Material *material) noexcept {
		if (RenderMode::TSNMShadingNormal == m_render_mode
			&& material->GetNormalSRV()) {

			// Bind the pixel shader.
			BindPS(PSIndex::TSNMShadingNormal);

			// Bind the normal SRV.
			PS::BindSRV(m_device_context,
				SLOT_SRV_NORMAL, material->GetNormalSRV());
		}
		else {

			// Bind the pixel shader.
			BindPS(PSIndex::ShadingNormal);
		}
	}

	void XM_CALLCONV ShadingNormalPass::BindModelData(
		FXMMATRIX object_to_view, 
		FXMMATRIX world_to_object) noexcept {

		ModelTransformBuffer buffer;
		buffer.m_object_to_view    = XMMatrixTranspose(object_to_view);
		buffer.m_normal_to_view    = world_to_object;
		buffer.m_texture_transform = XMMatrixIdentity();

		// Update the model buffer.
		m_model_buffer.UpdateData(m_device_context, buffer);
		// Bind the model buffer.
		VS::BindConstantBuffer(m_device_context, 
			SLOT_CBUFFER_PER_DRAW, m_model_buffer.Get());
	}

	void XM_CALLCONV ShadingNormalPass::BindSceneData(
		FXMMATRIX view_to_projection) noexcept {

		// Update the scene buffer.
		m_scene_buffer.UpdateData(m_device_context, XMMatrixTranspose(view_to_projection));
		// Bind the scene buffer.
		VS::BindConstantBuffer(m_device_context, 
			SLOT_CBUFFER_PER_FRAME, m_scene_buffer.Get());
	}

	void ShadingNormalPass::Render(const PassBuffer *scene, const CameraNode *node) {
		Assert(scene);
		Assert(node);

		// Reset the render mode.
		m_render_mode = node->GetSettings()->GetRenderMode();
		// Reset the bound pixel shader index.
		m_bound_ps = PSIndex::Count;
		
		// Bind the vertex shader.
		m_vs->BindShader(m_device_context);
		// Bind the rasterization state.
		RenderingStateCache::Get()->BindCullCounterClockwiseRasterizerState(m_device_context);
		// Bind the depth-stencil state.
		RenderingStateCache::Get()->BindDepthDefaultDepthStencilState(m_device_context);
		// Bind the blend state.
		RenderingStateCache::Get()->BindOpaqueBlendState(m_device_context);
		// Bind the sampler.
		if (RenderMode::ShadingNormal != m_render_mode) {
			PS::BindSampler(m_device_context, SLOT_SAMPLER_DEFAULT,
				RenderingStateCache::Get()->GetLinearWrapSamplerState());
		}
			
		// Obtain node components.
		const TransformNode * const transform = node->GetTransform();
		const Camera        * const camera    = node->GetCamera();
		const XMMATRIX world_to_view          = transform->GetWorldToViewMatrix();
		const XMMATRIX view_to_projection     = camera->GetViewToProjectionMatrix();
		const XMMATRIX world_to_projection    = world_to_view * view_to_projection;

		// Bind the scene data.
		BindSceneData(view_to_projection);
		
		ProcessModels(scene->m_opaque_brdf_models,      world_to_projection, world_to_view);
		ProcessModels(scene->m_transparent_brdf_models, world_to_projection, world_to_view);
	}

	void XM_CALLCONV ShadingNormalPass::ProcessModels(
		const vector< const ModelNode * > &models,
		FXMMATRIX world_to_projection, 
		FXMMATRIX world_to_view) noexcept {

		for (const auto node : models) {

			// Obtain node components (1/2).
			const TransformNode * const transform = node->GetTransform();
			const Model         * const model     = node->GetModel();
			const XMMATRIX object_to_world        = transform->GetObjectToWorldMatrix();
			const XMMATRIX object_to_projection   = object_to_world * world_to_projection;

			// Apply view frustum culling.
			if (ViewFrustum::Cull(object_to_projection, model->GetAABB())) {
				continue;
			}

			// Obtain node components (2/2).
			const XMMATRIX object_to_view         = object_to_world * world_to_view;
			const XMMATRIX world_to_object        = transform->GetWorldToObjectMatrix();

			// Bind the model data.
			BindModelData(object_to_view, world_to_object);
			// Bind the pixel shader.
			BindPS(model->GetMaterial());
			// Bind the model mesh.
			model->BindMesh(m_device_context);
			// Draw the model.
			model->Draw(m_device_context);
		}
	}
}