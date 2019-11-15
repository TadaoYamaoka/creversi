#include "bit_manipulations.hpp"

namespace bit_manipulations {

	constexpr int base3_bits = 12;
	constexpr int base3_size = 1 << base3_bits;
	alignas(32) uint16_t base3[base3_size];

#ifdef __INTEL_COMPILER
	__m128i operator^(__m128i lhs, __m128i rhs) {
		return _mm_xor_si128(lhs, rhs);
	}

	__m256i operator^(__m256i lhs, __m256i rhs) {
		return _mm256_xor_si256(lhs, rhs);
	}
#endif

	uint64_t delta_swap(uint64_t bits, uint64_t mask, int delta) {
		uint64_t tmp = mask & (bits ^ (bits << delta));
		return bits ^ tmp ^ (tmp >> delta);
	}

	__m128i delta_swap(__m128i bits, __m128i mask, int delta) {
		__m128i tmp = _mm_and_si128(mask, (bits ^ _mm_slli_epi64(bits, delta)));
		return bits ^ tmp ^ _mm_srli_epi64(tmp, delta);
	}

	__m256i delta_swap(__m256i bits, __m256i mask, int delta) {
		__m256i tmp = _mm256_and_si256(mask, (bits ^ _mm256_slli_epi64(bits, delta)));
		return bits ^ tmp ^ _mm256_srli_epi64(tmp, delta);
	}

	alignas(32) __m128i flip_vertical_shuffle_table;
	alignas(32) __m256i flip_vertical_shuffle_table_256;
	alignas(32) __m128i rotr8_shuffle_table[8];
	alignas(32) __m256i rotr8_shuffle_table_256[8];

	void init() {
		flip_vertical_shuffle_table = _mm_set_epi8(
			8, 9, 10, 11, 12, 13, 14, 15,
			0, 1, 2, 3, 4, 5, 6, 7);
		flip_vertical_shuffle_table_256 = _mm256_set_epi8(
			24, 25, 26, 27, 28, 29, 30, 31,
			16, 17, 18, 19, 20, 21, 22, 23,
			8, 9, 10, 11, 12, 13, 14, 15,
			0, 1, 2, 3, 4, 5, 6, 7);
		for (int i = 0; i < 8; ++i) {
			rotr8_shuffle_table[i] = _mm_set_epi8(
				(7 + i) % 8 + 8,
				(6 + i) % 8 + 8,
				(5 + i) % 8 + 8,
				(4 + i) % 8 + 8,
				(3 + i) % 8 + 8,
				(2 + i) % 8 + 8,
				(1 + i) % 8 + 8,
				(0 + i) % 8 + 8,
				(7 + i) % 8,
				(6 + i) % 8,
				(5 + i) % 8,
				(4 + i) % 8,
				(3 + i) % 8,
				(2 + i) % 8,
				(1 + i) % 8,
				(0 + i) % 8);
			rotr8_shuffle_table_256[i] = _mm256_set_epi8(
				(7 + i) % 8 + 24,
				(6 + i) % 8 + 24,
				(5 + i) % 8 + 24,
				(4 + i) % 8 + 24,
				(3 + i) % 8 + 24,
				(2 + i) % 8 + 24,
				(1 + i) % 8 + 24,
				(0 + i) % 8 + 24,
				(7 + i) % 8 + 16,
				(6 + i) % 8 + 16,
				(5 + i) % 8 + 16,
				(4 + i) % 8 + 16,
				(3 + i) % 8 + 16,
				(2 + i) % 8 + 16,
				(1 + i) % 8 + 16,
				(0 + i) % 8 + 16,
				(7 + i) % 8 + 8,
				(6 + i) % 8 + 8,
				(5 + i) % 8 + 8,
				(4 + i) % 8 + 8,
				(3 + i) % 8 + 8,
				(2 + i) % 8 + 8,
				(1 + i) % 8 + 8,
				(0 + i) % 8 + 8,
				(7 + i) % 8,
				(6 + i) % 8,
				(5 + i) % 8,
				(4 + i) % 8,
				(3 + i) % 8,
				(2 + i) % 8,
				(1 + i) % 8,
				(0 + i) % 8);
		}
		for (int i = 0; i < base3_size; ++i) {
			int res = 0;
			int pow3 = 1;
			for (int j = 0; j < base3_bits; ++j) {
				res += ((i >> j) & 1) * pow3;
				pow3 *= 3;
			}
			base3[i] = res;
		}
	}

	bool equal(board lhs, board rhs) {
		__m128i tmp = _mm_xor_si128(lhs.data, rhs.data);
		return _mm_test_all_zeros(tmp, tmp);
	}

	double_board flipVertical(double_board dbd) {
		return double_board(_mm256_shuffle_epi8(dbd, flip_vertical_shuffle_table_256));
	}

	board flipVertical(board bd) {
		return board(_mm_shuffle_epi8(bd, flip_vertical_shuffle_table));
	}

	uint64_t flipVertical(uint64_t bits) {
		return _byteswap_uint64(bits);
	}

	double_board mirrorHorizontal(double_board dbd) {
		__m256i mask1 = _mm256_set1_epi8(0x55);
		__m256i mask2 = _mm256_set1_epi8(0x33);
		__m256i mask3 = _mm256_set1_epi8(0x0f);
		dbd = _mm256_or_si256(_mm256_and_si256(_mm256_srli_epi64(dbd, 1), mask1),
			_mm256_slli_epi64(_mm256_and_si256(dbd, mask1), 1));
		dbd = _mm256_or_si256(_mm256_and_si256(_mm256_srli_epi64(dbd, 2), mask2),
			_mm256_slli_epi64(_mm256_and_si256(dbd, mask2), 2));
		dbd = _mm256_or_si256(_mm256_and_si256(_mm256_srli_epi64(dbd, 4), mask3),
			_mm256_slli_epi64(_mm256_and_si256(dbd, mask3), 4));
		return dbd;
	}

	board mirrorHorizontal(board bd) {
		__m128i mask1 = _mm_set1_epi8(0x55);
		__m128i mask2 = _mm_set1_epi8(0x33);
		__m128i mask3 = _mm_set1_epi8(0x0f);
		bd = _mm_or_si128(_mm_and_si128(_mm_srli_epi64(bd, 1), mask1),
			_mm_slli_epi64(_mm_and_si128(bd, mask1), 1));
		bd = _mm_or_si128(_mm_and_si128(_mm_srli_epi64(bd, 2), mask2),
			_mm_slli_epi64(_mm_and_si128(bd, mask2), 2));
		bd = _mm_or_si128(_mm_and_si128(_mm_srli_epi64(bd, 4), mask3),
			_mm_slli_epi64(_mm_and_si128(bd, mask3), 4));
		return bd;
	}

	uint64_t mirrorHorizontal(uint64_t bits) {
		uint64_t mask1 = UINT64_C(0x5555555555555555);
		uint64_t mask2 = UINT64_C(0x3333333333333333);
		uint64_t mask3 = UINT64_C(0x0f0f0f0f0f0f0f0f);
		bits = ((bits >> 1) & mask1) | ((bits & mask1) << 1);
		bits = ((bits >> 2) & mask2) | ((bits & mask2) << 2);
		bits = ((bits >> 4) & mask3) | ((bits & mask3) << 4);
		return bits;
	}

	double_board flipDiagA1H8(double_board dbd) {
		__m256i mask1 = _mm256_set1_epi16(0x5500);
		__m256i mask2 = _mm256_set1_epi32(0x33330000);
		__m256i mask3 = _mm256_set1_epi64x(UINT64_C(0x0f0f0f0f00000000));
		__m256i data = delta_swap(dbd, mask3, 28);
		data = delta_swap(data, mask2, 14);
		return double_board(delta_swap(data, mask1, 7));
	}

	board flipDiagA1H8(board bd) {
		__m128i mask1 = _mm_set1_epi16(0x5500);
		__m128i mask2 = _mm_set1_epi32(0x33330000);
		__m128i mask3 = _mm_set1_epi64x(UINT64_C(0x0f0f0f0f00000000));
		__m128i data = delta_swap(bd, mask3, 28);
		data = delta_swap(data, mask2, 14);
		return board(delta_swap(data, mask1, 7));
	}

	uint64_t flipDiagA1H8(uint64_t bits) {
		uint64_t mask1 = UINT64_C(0x5500550055005500);
		uint64_t mask2 = UINT64_C(0x3333000033330000);
		uint64_t mask3 = UINT64_C(0x0f0f0f0f00000000);
		bits = delta_swap(bits, mask3, 28);
		bits = delta_swap(bits, mask2, 14);
		return delta_swap(bits, mask1, 7);
	}

	double_board flipDiagA8H1(double_board dbd) {
		__m256i mask1 = _mm256_set1_epi16((short)0xaa00);
		__m256i mask2 = _mm256_set1_epi32((int)0xcccc0000);
		__m256i mask3 = _mm256_set1_epi64x(UINT64_C(0xf0f0f0f000000000));
		__m256i data = delta_swap(dbd, mask3, 36);
		data = delta_swap(data, mask2, 18);
		return double_board(delta_swap(data, mask1, 9));
	}

	board flipDiagA8H1(board bd) {
		__m128i mask1 = _mm_set1_epi16((short)0xaa00);
		__m128i mask2 = _mm_set1_epi32((int)0xcccc0000);
		__m128i mask3 = _mm_set1_epi64x(
			UINT64_C(0xf0f0f0f000000000));
		__m128i data = delta_swap(bd, mask3, 36);
		data = delta_swap(data, mask2, 18);
		return board(delta_swap(data, mask1, 9));
	}

	uint64_t flipDiagA8H1(uint64_t bits) {
		uint64_t mask1 = UINT64_C(0xaa00aa00aa00aa00);
		uint64_t mask2 = UINT64_C(0xcccc0000cccc0000);
		uint64_t mask3 = UINT64_C(0xf0f0f0f000000000);
		bits = delta_swap(bits, mask3, 36);
		bits = delta_swap(bits, mask2, 18);
		return delta_swap(bits, mask1, 9);
	}

	double_board rotate180(double_board dbd) {
		return mirrorHorizontal(flipVertical(dbd));
	}

	board rotate180(board bd) {
		return mirrorHorizontal(flipVertical(bd));
	}

	uint64_t rotate180(uint64_t bits) {
		return mirrorHorizontal(flipVertical(bits));
	};

	double_board rotate90clockwise(double_board dbd) {
		return flipVertical(flipDiagA8H1(dbd));
	}

	board rotate90clockwise(board bd) {
		return flipVertical(flipDiagA8H1(bd));
	}

	uint64_t rotate90clockwise(uint64_t bits) {
		return flipVertical(flipDiagA8H1(bits));
	}

	double_board rotate90antiClockwise(double_board dbd) {
		return flipVertical(flipDiagA1H8(dbd));
	}

	board rotate90antiClockwise(board bd) {
		return flipVertical(flipDiagA1H8(bd));
	}

	uint64_t rotate90antiClockwise(uint64_t bits) {
		return flipVertical(flipDiagA1H8(bits));
	}

	__m128i rotr(__m128i bits, int index) {
		board bd(bits);
		return board(_rotr64(bd.player(), index),
			_rotr64(bd.opponent(), index));
	}

	__m256i rotr8_epi64(__m256i bits, int index) {
		return _mm256_shuffle_epi8(bits, rotr8_shuffle_table_256[index]);
	}

	__m128i rotr8_epi64(__m128i bits, int index) {
		return _mm_shuffle_epi8(bits, rotr8_shuffle_table[index]);
	}

	uint64_t rotr(uint64_t bits, int index) {
		return _rotr64(bits, index);
	}

	double_board pseudoRotate45clockwise(double_board dbd) {
		__m256i mask1 = _mm256_set1_epi8(0x55);
		__m256i mask2 = _mm256_set1_epi8(0x33);
		__m256i mask3 = _mm256_set1_epi8(0x0f);
		__m256i data = __m256i(dbd) ^ _mm256_and_si256(mask1, (__m256i(dbd) ^ rotr8_epi64(dbd, 1)));
		data = data ^ _mm256_and_si256(mask2, (data ^ rotr8_epi64(data, 2)));
		return data ^ _mm256_and_si256(mask3, (data ^ rotr8_epi64(data, 4)));
	}

	board pseudoRotate45clockwise(board bd) {
		__m128i mask1 = _mm_set1_epi8(0x55);
		__m128i mask2 = _mm_set1_epi8(0x33);
		__m128i mask3 = _mm_set1_epi8(0x0f);
		__m128i data = __m128i(bd) ^ _mm_and_si128(mask1, (__m128i(bd) ^ rotr8_epi64(bd, 1)));
		data = data ^ _mm_and_si128(mask2, (data ^ rotr8_epi64(data, 2)));
		return data ^ _mm_and_si128(mask3, (data ^ rotr8_epi64(data, 4)));
	}

	uint64_t pseudoRotate45clockwise(uint64_t bits) {
		uint64_t mask1 = UINT64_C(0x5555555555555555);
		uint64_t mask2 = UINT64_C(0x3333333333333333);
		uint64_t mask3 = UINT64_C(0x0f0f0f0f0f0f0f0f);
		bits = bits ^ (mask1 & (bits ^ rotr(bits, 8)));
		bits = bits ^ (mask2 & (bits ^ rotr(bits, 16)));
		return bits ^ (mask3 & (bits ^ rotr(bits, 32)));
	}

	double_board pseudoRotate45antiClockwise(double_board dbd) {
		__m256i mask1 = _mm256_set1_epi8((char)0xaa);
		__m256i mask2 = _mm256_set1_epi8((char)0xcc);
		__m256i mask3 = _mm256_set1_epi8((char)0xf0);
		__m256i data = __m256i(dbd) ^ _mm256_and_si256(mask1, (__m256i(dbd) ^ rotr8_epi64(dbd, 1)));
		data = data ^ _mm256_and_si256(mask2, (data ^ rotr8_epi64(data, 2)));
		return data ^ _mm256_and_si256(mask3, (data ^ rotr8_epi64(data, 4)));
	}

	board pseudoRotate45antiClockwise(board bd) {
		__m128i mask1 = _mm_set1_epi8((char)0xaa);
		__m128i mask2 = _mm_set1_epi8((char)0xcc);
		__m128i mask3 = _mm_set1_epi8((char)0xf0);
		__m128i data = __m128i(bd) ^ _mm_and_si128(mask1, (__m128i(bd) ^ rotr8_epi64(bd, 1)));
		data = data ^ _mm_and_si128(mask2, (data ^ rotr8_epi64(data, 2)));
		return data ^ _mm_and_si128(mask3, (data ^ rotr8_epi64(data, 4)));
	}

	uint64_t pseudoRotate45antiClockwise(uint64_t bits) {
		uint64_t mask1 = UINT64_C(0xaaaaaaaaaaaaaaaa);
		uint64_t mask2 = UINT64_C(0xcccccccccccccccc);
		uint64_t mask3 = UINT64_C(0xf0f0f0f0f0f0f0f0);
		bits = bits ^ (mask1 & (bits ^ rotr(bits, 8)));
		bits = bits ^ (mask2 & (bits ^ rotr(bits, 16)));
		return bits ^ (mask3 & (bits ^ rotr(bits, 32)));
	}

	uint64_t tails(uint64_t bits) {
		++bits;
		return ((int64_t)bits & -(int64_t)bits) - 1;
	}

	board tails(board bd) {
		return board(tails(bd.player()), tails(bd.opponent()));
	}

	board definites_horizontal_top(board bd) {
		return board(_mm_or_si128(tails(bd),
			mirrorHorizontal(tails(mirrorHorizontal(bd)))));
	}
	board definites_horizontal(board bd) {
		return board(_mm_or_si128(
			definites_horizontal_top(bd),
			flipVertical(definites_horizontal_top(flipVertical(bd)))));
	}

	board definites(board bd) {
		return board(_mm_or_si128(
			definites_horizontal(bd),
			flipDiagA1H8(definites_horizontal(flipDiagA1H8(bd)))));
	}

	uint64_t stones(board bd) {
		return _mm_cvtsi128_si64(_mm_or_si128(_mm_alignr_epi8(bd, bd, 8), bd));
	}

	__m128i stones(double_board dbd) {
		__m256i res = _mm256_or_si256(_mm256_alignr_epi8(dbd, dbd, 8), dbd);
		return _mm256_castsi256_si128(_mm256_permute4x64_epi64(res, 0x08));
	}

	int stone_sum(board bd) {
		return (int)_popcnt64(stones(bd));
	}

	int bit_to_pos(uint64_t bit) {
		return (int)_tzcnt_u64(bit);
	}

	uint16_t toBase3(uint16_t black, uint16_t white) {
		return base3[black] + 2 * base3[white];
	}

	uint16_t toBase3_8(uint8_t black, uint8_t white) {
		return base3[black] + 2 * base3[white];
	}

	board toBase3_4x16(board bd) {
		__m128i hi = _mm_and_si128(_mm_srli_epi16(bd, 4), _mm_set1_epi8(0x0F));
		__m128i lo = _mm_and_si128(bd, _mm_set1_epi8(0x0F));
		__m128i b = _mm_unpacklo_epi8(hi, lo);
		__m128i w = _mm_unpackhi_epi8(hi, lo);
		__m128i table1 = _mm_setr_epi8(0, 1, 3, 4, 9, 10, 12, 13, 27, 28, 30, 31, 36, 37, 39, 40); // base2 -> base3 table
		__m128i table2 = _mm_setr_epi8(0, 2, 6, 8, 18, 20, 24, 26, 54, 56, 60, 62, 72, 74, 78, 80); // base2 -> base3 table * 2
		__m128i b_base3 = _mm_shuffle_epi8(table1, b);
		__m128i w_base3 = _mm_shuffle_epi8(table2, w);
		return _mm_add_epi8(b_base3, w_base3);
	}

	board toBase3_8x8(board bd) {
		__m128i coeff = _mm_set1_epi16(0x5101);
		return _mm_maddubs_epi16(toBase3_4x16(bd), coeff);
	}

	std::string toBase81(board bd) {
		__m128i hi = _mm_and_si128(_mm_srli_epi16(bd, 4), _mm_set1_epi8(0x0F));
		__m128i lo = _mm_and_si128(bd, _mm_set1_epi8(0x0F));
		__m128i b = _mm_unpacklo_epi8(lo, hi);
		__m128i w = _mm_unpackhi_epi8(lo, hi);
		__m128i table1 = _mm_setr_epi8(0, 1, 3, 4, 9, 10, 12, 13, 32, 33, 35, 36, 41, 42, 44, 45); // 32 * a[3] + 9 * a[2] + 3 * a[1] + a[0]
		__m128i table2 = _mm_setr_epi8(33, 35, 39, 41, 51, 53, 57, 59, 97, 99, 103, 105, 115, 117, 121, 123); // table1 * 2 + 33
		__m128i b_base3 = _mm_shuffle_epi8(table1, b);
		__m128i w_base3 = _mm_shuffle_epi8(table2, w);
		__m128i res = _mm_add_epi8(b_base3, w_base3);
		std::string res_str(16, 'A');
		_mm_storeu_si128((__m128i*)res_str.data(), res);
		return res_str;
	}

	// divide 16 integers under 27 by 9
	__m128i div9_epu8(__m128i x) {
		__m128i hi = _mm_and_si128(_mm_maddubs_epi16(x, _mm_set1_epi16((short)0x1d00)), _mm_set1_epi16((short)0xFF00));
		__m128i lo = _mm_srli_epi16(_mm_maddubs_epi16(x, _mm_set1_epi16((short)0x001d)), 8);
		return _mm_or_si128(hi, lo);
	}

	board toBoard(const std::string &str) {
		__m128i data = _mm_loadu_si128((__m128i*)str.data());
		data = _mm_sub_epi8(data, _mm_set1_epi8(33)); // substruct offset
		__m128i b = _mm_and_si128(_mm_srli_epi16(data, 2), _mm_set1_epi8(0x08)); // a3 black
		__m128i w = _mm_and_si128(_mm_srli_epi16(data, 3), _mm_set1_epi8(0x08)); // a3 white
		data = _mm_and_si128(data, _mm_set1_epi8(0x1F));
		__m128i a2 = div9_epu8(data);
		b |= _mm_and_si128(_mm_slli_epi16(a2, 2), _mm_set1_epi8(0x04)); // a2 black
		w |= _mm_and_si128(_mm_slli_epi16(a2, 1), _mm_set1_epi8(0x04)); // a2 white
		data = _mm_sub_epi8(data, _mm_mullo_epi16(a2, _mm_set1_epi16(9)));
		__m128i table1 = _mm_setr_epi8(0x0, 0x1, 0x0, 0x2, 0x3, 0x2, 0x0, 0x1, 0x0, 0, 0, 0, 0, 0, 0, 0);
		__m128i table2 = _mm_setr_epi8(0x0, 0x0, 0x1, 0x0, 0x0, 0x1, 0x2, 0x2, 0x3, 0, 0, 0, 0, 0, 0, 0);
		b |= _mm_shuffle_epi8(table1, data);
		w |= _mm_shuffle_epi8(table2, data);
		b = _mm_maddubs_epi16(b, _mm_set1_epi16(0x1001));
		w = _mm_maddubs_epi16(w, _mm_set1_epi16(0x1001));
		return _mm_packus_epi16(b, w);
	}

} // namespace bit_manipulations
