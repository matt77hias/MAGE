#pragma once

//-----------------------------------------------------------------------------
// Engine Definitions
//-----------------------------------------------------------------------------
namespace mage::loader {

	template< typename T >
	inline void VARReader::ReadVARVariable() {
		auto name  = Read< std::string >();
		auto value = Read< T >();

		m_variable_buffer.insert_or_assign(std::move(name), std::move(value));
	}

	template< typename T, std::size_t N, size_t A >
	inline void VARReader::ReadVARVariable() {
		auto name  = Read< std::string >();
		auto value = Read< T, N, A >();

		m_variable_buffer.insert_or_assign(std::move(name), std::move(value));
	}
}