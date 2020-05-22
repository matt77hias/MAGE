#pragma once

//-----------------------------------------------------------------------------
// Engine Definitions
//-----------------------------------------------------------------------------
namespace mage::rendering {

	template< typename VertexT >
	[[nodiscard]]
	HRESULT CreateStaticVertexBuffer(ID3D11Device& device,
									 NotNull< ID3D11Buffer** > buffer,
									 gsl::span< const VertexT > vertices) noexcept {

		// Create the buffer descriptor.
		D3D11_BUFFER_DESC buffer_desc = {};
		buffer_desc.BindFlags      = D3D11_BIND_VERTEX_BUFFER;
		buffer_desc.ByteWidth      = static_cast< U32 >(vertices.size_bytes());
		// GPU:    read + no write
		// CPU: no read + no write
		buffer_desc.Usage          = D3D11_USAGE_IMMUTABLE;
		buffer_desc.CPUAccessFlags = 0u;

		// Specify the subresource data.
		D3D11_SUBRESOURCE_DATA init_data = {};
		init_data.pSysMem = vertices.data();

		// Create the vertex buffer.
		return device.CreateBuffer(&buffer_desc, &init_data, buffer);
	}

	template< typename VertexT >
	[[nodiscard]]
	HRESULT CreateDynamicVertexBuffer(ID3D11Device& device,
									  NotNull< ID3D11Buffer** > buffer,
									  gsl::span< const VertexT > vertices) noexcept {

		// Create the buffer descriptor.
		D3D11_BUFFER_DESC buffer_desc = {};
		buffer_desc.BindFlags      = D3D11_BIND_VERTEX_BUFFER;
		buffer_desc.ByteWidth      = static_cast< U32 >(vertices.size_bytes());
		// GPU:    read + no write
		// CPU: no read +    write
		buffer_desc.Usage          = D3D11_USAGE_DYNAMIC;
		buffer_desc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;

		// Specify the subresource data.
		D3D11_SUBRESOURCE_DATA init_data = {};
		init_data.pSysMem = vertices.data();

		// Create the vertex buffer.
		return device.CreateBuffer(&buffer_desc, &init_data, buffer);
	}

	template< typename VertexT >
	[[nodiscard]]
	HRESULT CreateDynamicVertexBuffer(ID3D11Device& device,
									  NotNull< ID3D11Buffer** > buffer,
									  std::size_t nb_vertices) noexcept {

		// Create the buffer descriptor.
		D3D11_BUFFER_DESC buffer_desc = {};
		buffer_desc.BindFlags      = D3D11_BIND_VERTEX_BUFFER;
		buffer_desc.ByteWidth      = static_cast< U32 >(sizeof(VertexT) * nb_vertices);
		// GPU:    read + no write
		// CPU: no read +    write
		buffer_desc.Usage          = D3D11_USAGE_DYNAMIC;
		buffer_desc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;

		// Create the vertex buffer.
		return device.CreateBuffer(&buffer_desc, nullptr, buffer);
	}

	template< typename IndexT >
	[[nodiscard]]
	HRESULT CreateStaticIndexBuffer(ID3D11Device& device,
									NotNull< ID3D11Buffer** > buffer,
									gsl::span< const IndexT > indices) noexcept {

		// Create the buffer descriptor.
		D3D11_BUFFER_DESC buffer_desc = {};
		buffer_desc.BindFlags      = D3D11_BIND_INDEX_BUFFER;
		buffer_desc.ByteWidth      = static_cast< U32 >(indices.size_bytes());
		// GPU:    read + no write
		// CPU: no read + no write
		buffer_desc.Usage          = D3D11_USAGE_IMMUTABLE;
		buffer_desc.CPUAccessFlags = 0u;

		// Specify the subresource data.
		D3D11_SUBRESOURCE_DATA init_data = {};
		init_data.pSysMem = indices.data();

		// Create the index buffer.
		return device.CreateBuffer(&buffer_desc, &init_data, buffer);
	}

	template< typename T >
	[[nodiscard]]
	HRESULT CreateStaticConstantBuffer(ID3D11Device& device,
									   NotNull< ID3D11Buffer** > buffer,
									   gsl::span< const T > data) noexcept {

		// Create the buffer descriptor.
		D3D11_BUFFER_DESC buffer_desc = {};
		buffer_desc.BindFlags      = D3D11_BIND_CONSTANT_BUFFER;
		buffer_desc.ByteWidth      = static_cast< U32 >(data.size_bytes());
		// GPU:    read + no write
		// CPU: no read + no write
		buffer_desc.Usage          = D3D11_USAGE_IMMUTABLE;
		buffer_desc.CPUAccessFlags = 0u;

		// Specify the subresource data.
		D3D11_SUBRESOURCE_DATA init_data = {};
		init_data.pSysMem = data.data();

		// Create the constant buffer.
		return device.CreateBuffer(&buffer_desc, &init_data, buffer);
	}

	template< typename T >
	[[nodiscard]]
	HRESULT CreateDynamicConstantBuffer(ID3D11Device& device,
										NotNull< ID3D11Buffer** > buffer,
										gsl::span< const T > data) noexcept {

		// Create the buffer descriptor.
		D3D11_BUFFER_DESC buffer_desc = {};
		buffer_desc.BindFlags      = D3D11_BIND_CONSTANT_BUFFER;
		buffer_desc.ByteWidth      = static_cast< U32 >(data.size_bytes());
		// GPU:    read + no write
		// CPU: no read +    write
		buffer_desc.Usage          = D3D11_USAGE_DYNAMIC;
		buffer_desc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;

		// Specify the subresource data.
		D3D11_SUBRESOURCE_DATA init_data = {};
		init_data.pSysMem = data.data();

		// Create the constant buffer.
		return device.CreateBuffer(&buffer_desc, &init_data, buffer);
	}

	template< typename T >
	[[nodiscard]]
	HRESULT CreateDynamicConstantBuffer(ID3D11Device& device,
										NotNull< ID3D11Buffer** > buffer,
										std::size_t nb_data_elements) noexcept {

		// Create the buffer descriptor.
		D3D11_BUFFER_DESC buffer_desc = {};
		buffer_desc.BindFlags      = D3D11_BIND_CONSTANT_BUFFER;
		buffer_desc.ByteWidth      = static_cast< U32 >(sizeof(T) * nb_data_elements);
		// GPU:    read + no write
		// CPU: no read +    write
		buffer_desc.Usage          = D3D11_USAGE_DYNAMIC;
		buffer_desc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;

		// Create the constant buffer.
		return device.CreateBuffer(&buffer_desc, nullptr, buffer);
	}

	template< typename T >
	[[nodiscard]]
	HRESULT CreateStaticStructuredBuffer(ID3D11Device& device,
										 NotNull< ID3D11Buffer** > buffer,
										 gsl::span< const T > data) noexcept {

		// Create the buffer descriptor.
		D3D11_BUFFER_DESC buffer_desc = {};
		buffer_desc.BindFlags           = D3D11_BIND_SHADER_RESOURCE;
		buffer_desc.MiscFlags           = D3D11_RESOURCE_MISC_BUFFER_STRUCTURED;
		buffer_desc.ByteWidth           = static_cast< U32 >(data.size_bytes());
		buffer_desc.StructureByteStride = static_cast< U32 >(sizeof(T));
		// GPU:    read + no write
		// CPU: no read + no write
		buffer_desc.Usage               = D3D11_USAGE_IMMUTABLE;
		buffer_desc.CPUAccessFlags      = 0u;

		// Specify the subresource data.
		D3D11_SUBRESOURCE_DATA init_data = {};
		init_data.pSysMem = data.data();

		// Create the structured buffer.
		return device.CreateBuffer(&buffer_desc, &init_data, buffer);
	}

	template< typename T >
	[[nodiscard]]
	HRESULT CreateDynamicStructuredBuffer(ID3D11Device& device,
										  NotNull< ID3D11Buffer** > buffer,
										  gsl::span< const T > data) noexcept {

		// Create the buffer descriptor.
		D3D11_BUFFER_DESC buffer_desc = {};
		buffer_desc.BindFlags           = D3D11_BIND_SHADER_RESOURCE;
		buffer_desc.MiscFlags           = D3D11_RESOURCE_MISC_BUFFER_STRUCTURED;
		buffer_desc.ByteWidth           = static_cast< U32 >(data.size_bytes());
		buffer_desc.StructureByteStride = static_cast< U32 >(sizeof(T));
		// GPU:    read + no write
		// CPU: no read +    write
		buffer_desc.Usage               = D3D11_USAGE_DYNAMIC;
		buffer_desc.CPUAccessFlags      = D3D11_CPU_ACCESS_WRITE;

		// Specify the subresource data.
		D3D11_SUBRESOURCE_DATA init_data = {};
		init_data.pSysMem = data.data();

		// Create the structured buffer.
		return device.CreateBuffer(&buffer_desc, &init_data, buffer);
	}

	template< typename T >
	[[nodiscard]]
	HRESULT CreateDynamicStructuredBuffer(ID3D11Device& device,
										  NotNull< ID3D11Buffer** > buffer,
										  std::size_t nb_data_elements) noexcept {

		// Create the buffer descriptor.
		D3D11_BUFFER_DESC buffer_desc = {};
		buffer_desc.BindFlags           = D3D11_BIND_SHADER_RESOURCE;
		buffer_desc.MiscFlags           = D3D11_RESOURCE_MISC_BUFFER_STRUCTURED;
		buffer_desc.ByteWidth           = static_cast< U32 >(sizeof(T) * nb_data_elements);
		buffer_desc.StructureByteStride = static_cast< U32 >(sizeof(T));
		// GPU:    read + no write
		// CPU: no read +    write
		buffer_desc.Usage               = D3D11_USAGE_DYNAMIC;
		buffer_desc.CPUAccessFlags      = D3D11_CPU_ACCESS_WRITE;

		// Create the structured buffer.
		return device.CreateBuffer(&buffer_desc, nullptr, buffer);
	}
}