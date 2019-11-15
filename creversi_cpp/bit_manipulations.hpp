#pragma once
#include <string>
#include <array>

#include "board.hpp"

// bitwise manipulation functions utility
namespace bit_manipulations {

	void init();

	bool equal(board, board);

	// flipVertical
	//   0  1  2  3  4  5  6  7
	//   8  9 10 11 12 13 14 15
	//  16 17 18 19 20 21 22 23
	//  24 25 26 27 28 29 30 31
	//  32 33 34 35 36 37 38 39
	//  40 41 42 43 44 45 46 47
	//  48 49 50 51 52 53 54 55
	//  56 57 58 59 60 61 62 63
	//  |  |  |  |  |  |  |  |
	//  v  v  v  v  v  v  v  v
	//  56 57 58 59 60 61 62 63
	//  48 49 50 51 52 53 54 55
	//  40 41 42 43 44 45 46 47
	//  32 33 34 35 36 37 38 39
	//  24 25 26 27 28 29 30 31
	//  16 17 18 19 20 21 22 23
	//   8  9 10 11 12 13 14 15
	//   0  1  2  3  4  5  6  7
	double_board flipVertical(double_board);
	board flipVertical(board);
	uint64_t flipVertical(uint64_t);

	// mirrorHorizontal
	//   0  1  2  3  4  5  6  7
	//   8  9 10 11 12 13 14 15
	//  16 17 18 19 20 21 22 23
	//  24 25 26 27 28 29 30 31
	//  32 33 34 35 36 37 38 39
	//  40 41 42 43 44 45 46 47
	//  48 49 50 51 52 53 54 55
	//  56 57 58 59 60 61 62 63
	//  |  |  |  |  |  |  |  |
	//  v  v  v  v  v  v  v  v
	//   7  6  5  4  3  2  1  0
	//  15 14 13 12 11 10  9  8
	//  23 22 21 20 19 18 17 16
	//  31 30 29 28 27 26 25 24
	//  39 38 37 36 35 34 33 32
	//  47 46 45 44 43 42 41 40
	//  55 54 53 52 51 50 49 48
	//  63 62 61 60 59 58 57 56
	double_board mirrorHorizontal(double_board);
	board mirrorHorizontal(board);
	uint64_t mirrorHorizontal(uint64_t);

	// flipDiagA1H8
	//   0  1  2  3  4  5  6  7
	//   8  9 10 11 12 13 14 15
	//  16 17 18 19 20 21 22 23
	//  24 25 26 27 28 29 30 31
	//  32 33 34 35 36 37 38 39
	//  40 41 42 43 44 45 46 47
	//  48 49 50 51 52 53 54 55
	//  56 57 58 59 60 61 62 63
	//  |  |  |  |  |  |  |  |
	//  v  v  v  v  v  v  v  v
	//   0  8 16 24 32 40 48 56
	//   1  9 17 25 33 41 49 57
	//   2 10 18 26 34 42 50 58
	//   3 11 19 27 35 43 51 59
	//   4 12 20 28 36 44 52 60
	//   5 13 21 29 37 45 53 61
	//   6 14 22 30 38 46 54 62
	//   7 15 23 31 39 47 55 63
	double_board flipDiagA1H8(double_board);
	board flipDiagA1H8(board);
	uint64_t flipDiagA1H8(uint64_t);

	// flipDiagA8H1
	//   0  1  2  3  4  5  6  7
	//   8  9 10 11 12 13 14 15
	//  16 17 18 19 20 21 22 23
	//  24 25 26 27 28 29 30 31
	//  32 33 34 35 36 37 38 39
	//  40 41 42 43 44 45 46 47
	//  48 49 50 51 52 53 54 55
	//  56 57 58 59 60 61 62 63
	//  |  |  |  |  |  |  |  |
	//  v  v  v  v  v  v  v  v
	//   0  8 16 24 32 40 48 56
	//   1  9 17 25 33 41 49 57
	//   2 10 18 26 34 42 50 58
	//   3 11 19 27 35 43 51 59
	//   4 12 20 28 36 44 52 60
	//   5 13 21 29 37 45 53 61
	//   6 14 22 30 38 46 54 62
	//   7 15 23 31 39 47 55 63
	double_board flipDiagA8H1(double_board);
	board flipDiagA8H1(board);
	uint64_t flipDiagA8H1(uint64_t);

	// rotate180
	//   0  1  2  3  4  5  6  7
	//   8  9 10 11 12 13 14 15
	//  16 17 18 19 20 21 22 23
	//  24 25 26 27 28 29 30 31
	//  32 33 34 35 36 37 38 39
	//  40 41 42 43 44 45 46 47
	//  48 49 50 51 52 53 54 55
	//  56 57 58 59 60 61 62 63
	//  |  |  |  |  |  |  |  |
	//  v  v  v  v  v  v  v  v
	//  63 62 61 60 59 58 57 56
	//  55 54 53 52 51 50 49 48
	//  47 46 45 44 43 42 41 40
	//  39 38 37 36 35 34 33 32
	//  31 30 29 28 27 26 25 24
	//  23 22 21 20 19 18 17 16
	//  15 14 13 12 11 10  9  8
	//   7  6  5  4  3  2  1  0
	double_board rotate180(double_board);
	board rotate180(board);
	uint64_t rotate180(uint64_t);

	// rotate90clockwise
	//   0  1  2  3  4  5  6  7
	//   8  9 10 11 12 13 14 15
	//  16 17 18 19 20 21 22 23
	//  24 25 26 27 28 29 30 31
	//  32 33 34 35 36 37 38 39
	//  40 41 42 43 44 45 46 47
	//  48 49 50 51 52 53 54 55
	//  56 57 58 59 60 61 62 63
	//  |  |  |  |  |  |  |  |
	//  v  v  v  v  v  v  v  v
	//  56 48 40 32 24 16  8  0
	//  57 49 41 33 25 17  9  1
	//  58 50 42 34 26 18 10  2
	//  59 51 43 35 27 19 11  3
	//  60 52 44 36 28 20 12  4
	//  61 53 45 37 29 21 13  5
	//  62 54 46 38 30 22 14  6
	//  63 55 47 39 31 23 15  7
	double_board rotate90clockwise(double_board);
	board rotate90clockwise(board);
	uint64_t rotate90clockwise(uint64_t);

	// rotate90antiClockwise
	//   0  1  2  3  4  5  6  7
	//   8  9 10 11 12 13 14 15
	//  16 17 18 19 20 21 22 23
	//  24 25 26 27 28 29 30 31
	//  32 33 34 35 36 37 38 39
	//  40 41 42 43 44 45 46 47
	//  48 49 50 51 52 53 54 55
	//  56 57 58 59 60 61 62 63
	//  |  |  |  |  |  |  |  |
	//  v  v  v  v  v  v  v  v
	//   7 15 23 31 39 47 55 63
	//   6 14 22 30 38 46 54 62
	//   5 13 21 29 37 45 53 61
	//   4 12 20 28 36 44 52 60
	//   3 11 19 27 35 43 51 59
	//   2 10 18 26 34 42 50 58
	//   1  9 17 25 33 41 49 57
	//   0  8 16 24 32 40 48 56
	double_board rotate90antiClockwise(double_board);
	board rotate90antiClockwise(board);
	uint64_t rotate90antiClockwise(uint64_t);

	// pseudoRotate45clockwise
	//   0  1  2  3  4  5  6  7
	//   8  9 10 11 12 13 14 15
	//  16 17 18 19 20 21 22 23
	//  24 25 26 27 28 29 30 31
	//  32 33 34 35 36 37 38 39
	//  40 41 42 43 44 45 46 47
	//  48 49 50 51 52 53 54 55
	//  56 57 58 59 60 61 62 63
	//  |  |  |  |  |  |  |  |
	//  v  v  v  v  v  v  v  v
	//  56 49 42 35 28 21 14  7
	//   0 57 50 43 36 29 22 15
	//   8  1 58 51 44 37 30 23
	//  16  9  2 59 52 45 38 31
	//  24 17 10  3 60 53 46 39
	//  32 25 18 11  4 61 54 47
	//  40 33 26 19 12  5 62 55
	//  48 41 34 27 20 13  6 63
	double_board pseudoRotate45clockwise(double_board);
	board pseudoRotate45clockwise(board);
	uint64_t pseudoRotate45clockwise(uint64_t);

	// pseudoRotate45antiClockwise
	//   0  1  2  3  4  5  6  7
	//   8  9 10 11 12 13 14 15
	//  16 17 18 19 20 21 22 23
	//  24 25 26 27 28 29 30 31
	//  32 33 34 35 36 37 38 39
	//  40 41 42 43 44 45 46 47
	//  48 49 50 51 52 53 54 55
	//  56 57 58 59 60 61 62 63
	//  |  |  |  |  |  |  |  |
	//  v  v  v  v  v  v  v  v
	//   0  9 18 27 36 45 54 63
	//   8 17 26 35 44 53 62  7
	//  16 25 34 43 52 61  6 15
	//  24 33 42 51 60  5 14 23
	//  32 41 50 59  4 13 22 31
	//  40 49 58  3 12 21 30 39
	//  48 57  2 11 20 29 38 47
	//  56  1 10 19 28 37 46 55
	double_board pseudoRotate45antiClockwise(double_board);
	board pseudoRotate45antiClockwise(board);
	uint64_t pseudoRotate45antiClockwise(uint64_t);

	board definites(board);

	uint64_t stones(board);
	__m128i stones(double_board);
	int stone_sum(board);

	int bit_to_pos(uint64_t);
	__m256i rotr8_epi64(__m256i, int);
	__m128i rotr8_epi64(__m128i, int);
	uint64_t rotr(uint64_t, int);

	uint16_t toBase3(uint16_t, uint16_t);
	uint16_t toBase3_8(uint8_t, uint8_t);
	board toBase3_4x16(board);
	board toBase3_8x8(board);
	std::string toBase81(board);
	board toBoard(const std::string &);

} // namespace bit_manipulations

inline bool operator==(const board &lhs, const board &rhs) {
	return bit_manipulations::equal(lhs, rhs);
}

inline bool operator<(const board &lhs, const board &rhs) {
	return (lhs.player() == rhs.player()) ?
		(lhs.opponent() < rhs.opponent()) : (lhs.player() < rhs.player());
}
