#pragma once

//-----------------------------------------------------------------------------
// Engine Includes
//-----------------------------------------------------------------------------
#pragma region

#include "sampling\primes.hpp"
#include "logging\logging.hpp"

#pragma endregion

//-----------------------------------------------------------------------------
// Engine Declarations and Definitions
//-----------------------------------------------------------------------------
namespace mage {

	[[nodiscard]]
	inline F32 RadicalInverse(std::size_t index, F32 base) noexcept {
		auto result = 0.0f;
		auto denom  = 1.0f;

		while (index) {
			denom  *= base;
			result += fmod(index, base) / denom;
			index  /= base;
		}

		return result;
	}

	[[nodiscard]]
	inline F32 VanderCorput(std::size_t index) noexcept {
		return RadicalInverse(index, 2.0f);
	}

	inline void Halton(std::size_t index, gsl::span< F32 > sample) noexcept {
		using std::size;
		const auto nb_dims = static_cast< std::size_t >(size(sample));
		Assert(nb_dims <= std::size(g_primes));

		std::size_t i = 0u;
		for (auto& dim : sample) {
			dim = RadicalInverse(index, static_cast< F32 >(g_primes[i++]));
		}
	}

	[[nodiscard]]
	inline const F32x2 Halton2D(std::size_t index) noexcept {
		const auto x = RadicalInverse(index, 2.0f);
		const auto y = RadicalInverse(index, 3.0f);

		return { x, y };
	}

	[[nodiscard]]
	inline const F32x3 Halton3D(std::size_t index) noexcept {
		const auto x = RadicalInverse(index, 2.0f);
		const auto y = RadicalInverse(index, 3.0f);
		const auto z = RadicalInverse(index, 5.0f);

		return { x, y, z };
	}

	[[nodiscard]]
	inline const F32x4 Halton4D(std::size_t index) noexcept {
		const auto x = RadicalInverse(index, 2.0f);
		const auto y = RadicalInverse(index, 3.0f);
		const auto z = RadicalInverse(index, 5.0f);
		const auto w = RadicalInverse(index, 7.0f);

		return { x, y, z, w };
	}

	inline void Hammersley(std::size_t index, gsl::span< F32 > sample,
						   std::size_t nb_samples) noexcept {
		using std::begin;
		using std::end;

		Assert(index < nb_samples);

		using std::size;
		const auto nb_dims = static_cast< std::size_t >(size(sample));
		Assert(0 < nb_dims && nb_dims - 1 <= size(g_primes));

		*begin(sample) = index / static_cast< F32 >(nb_samples);

		std::size_t i = 0u;
		for (auto it = begin(sample) + 1; it != end(sample); ++it) {
			*it = RadicalInverse(index, static_cast< F32 >(g_primes[i++]));
		}
	}

	[[nodiscard]]
	inline const F32x2 Hammersley2D(std::size_t index, std::size_t nb_samples) noexcept {
		Assert(index < nb_samples);

		const auto x = index / static_cast< F32 >(nb_samples);
		const auto y = RadicalInverse(index, 2.0f);

		return { x, y };
	}

	[[nodiscard]]
	inline const F32x3 Hammersley3D(std::size_t index, std::size_t nb_samples) noexcept {
		Assert(index < nb_samples);

		const auto x = index / static_cast< F32 >(nb_samples);
		const auto y = RadicalInverse(index, 2.0f);
		const auto z = RadicalInverse(index, 3.0f);

		return { x, y, z };
	}

	[[nodiscard]]
	inline const F32x4 Hammersley4D(std::size_t index, std::size_t nb_samples) noexcept {
		Assert(index < nb_samples);

		const auto x = index / static_cast< F32 >(nb_samples);
		const auto y = RadicalInverse(index, 2.0f);
		const auto z = RadicalInverse(index, 3.0f);
		const auto w = RadicalInverse(index, 5.0f);

		return { x, y, z, w };
	}

	[[nodiscard]]
	inline const F32x2 Roth(std::size_t index, size_t nb_samples) noexcept {
		return Hammersley2D(index, nb_samples);
	}
}