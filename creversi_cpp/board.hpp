#pragma once
#include <array>
#ifdef _MSC_VER
#include <intrin.h>
#define _popcnt64 __popcnt64
#else
#include <x86intrin.h>
#endif
#include <immintrin.h>

#ifdef _MSC_VER
inline __m256i operator ^(__m256i s1, __m256i s2) { return _mm256_xor_si256(s1, s2); }
inline __m128i operator ^(__m128i s1, __m128i s2) { return _mm_xor_si128(s1, s2); }
inline __m128i operator |(__m128i s1, __m128i s2) { return _mm_or_si128(s1, s2); }
inline __m128i operator &(__m128i s1, __m128i s2) { return _mm_and_si128(s1, s2); }
inline __m128i operator ~(__m128i s1) { return _mm_xor_si128(s1, _mm_set1_epi64x(-1)); }
inline __m128i& operator |=(__m128i& s1, __m128i s2) { return s1 = _mm_or_si128(s1, s2); }
#endif

#ifdef __GNUC__
inline
unsigned char _bittest64(const long long *a, long long b)
{
	return (*a & (1LL << b)) != 0 ? 1 : 0;
}
inline
unsigned char _bittestandset64(long long *a, long long b)
{
	const long long mask = 1LL << b;
	unsigned char val = (*a & mask) != 0 ? 1 : 0;
	*a |= mask;
	return val;
}
inline
unsigned char _bittestandreset64(long long *a, long long b)
{
	const long long mask = 1LL << b;
	unsigned char val = (*a & mask) != 0 ? 1 : 0;
	*a &= ~mask;
	return val;
}
#define _mm_setr_epi64x(v0, v1) _mm_set_epi64x((v1), (v0))
#define _mm256_set_m128i(v0, v1)  _mm256_insertf128_si256(_mm256_castsi128_si256(v1), (v0), 1)
#define _mm256_setr_m128i(v0, v1) _mm256_set_m128i((v1), (v0))

#include <byteswap.h>
#define _byteswap_uint64(x) bswap_64(x)

#define _rotr64 __rorq
#endif

struct half_board {
	uint64_t data;
	half_board() = default;
	constexpr half_board(const half_board &) = default;
	constexpr half_board(half_board &&) = default;
	constexpr half_board(const uint64_t data) : data(data) {}
	half_board &operator=(const half_board &) = default;
	half_board &operator=(half_board &&) = default;
	operator uint64_t&() { return data; }
	operator const uint64_t&() const { return data; }
	bool get(const size_t index) const { return _bittest64((const long long*)&data, index); }
	bool set(const size_t index) { return _bittestandset64((long long*)&data, index); }
	bool reset(const size_t index) { return _bittestandreset64((long long*)&data, index); }
	bool setval(const size_t index, const bool bit) {
		return bit ? set(index) : reset(index);
	}
};

struct board {
	__m128i data;
	board() = default;
	board(const board &) = default;
	board(const uint64_t player, const uint64_t opponent)
		: data(_mm_setr_epi64x(player, opponent)) {}
	board(__m128i data) : data(data) {}
	operator __m128i() { return data; }
	operator __m128i() const { return data; }
	board &operator=(const board &) = default;
	board &operator=(board &&) = default;
	const half_board player() const { return _mm_cvtsi128_si64(data); }
	const half_board opponent() const {
		return _mm_extract_epi64(data, 1);
	}
	static board initial_board() {
		return board(UINT64_C(0x0000000810000000), UINT64_C(0x0000001008000000));
	}
	static board empty_board() {
		return board(_mm_setzero_si128());
	}
	static board reverse_board(const board &bd) {
		return board(_mm_alignr_epi8(bd.data, bd.data, 8));
	}
};

struct double_board {
	__m256i data;
	double_board() = default;
	double_board(const double_board &) = default;
	double_board(const board &bd1, const board &bd2)
		: data(_mm256_setr_m128i(bd1, bd2)) {}
	explicit double_board(const board &bd)
		: data(_mm256_broadcastsi128_si256(bd)) {}
	double_board(const uint64_t player1, const uint64_t opponent1,
		const uint64_t player2, const uint64_t opponent2)
		: data(_mm256_setr_epi64x(player1, opponent1, player2, opponent2)) {}
	double_board(const __m256i data) : data(data) {}
	operator __m256i() { return data; }
	operator __m256i() const { return data; }
	double_board &operator=(const double_board &) = default;
	double_board &operator=(double_board &&) = default;
	const board board1() const { return _mm256_castsi256_si128(data); }
	const board board2() const { return _mm256_extracti128_si256(data, 1); }
	uint64_t operator[](const size_t index) {
		switch (index) {
		case 0: return _mm256_extract_epi64(data, 0);
		case 1: return _mm256_extract_epi64(data, 1);
		case 2: return _mm256_extract_epi64(data, 2);
		case 3: return _mm256_extract_epi64(data, 3);
		default: return 0;
		}
	}
};

namespace std {

	template<>
	struct hash<board> {
	public:
		size_t operator()(const board &bd) const {
			return _byteswap_uint64(bd.player()) + bd.opponent() * 17;
		}
	};

} // namespace std
