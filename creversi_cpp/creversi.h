#pragma once

#include "board.hpp"
#include "state.hpp"
#include "hand.hpp"
#include "utils.hpp"
#include "value.hpp"
#include <cstring>

enum Piece {
	Empty = 0,
	Black,
	White
};

int __move_rotate90(const int move) {
	if (move == PASS) return move;
	const int i = move / 8;
	const int j = move % 8;
	return j * 8 + (7 - i);
}
int __move_rotate180(const int move) {
	if (move == PASS) return move;
	return 63 - move;
}
int __move_rotate270(const int move) {
	if (move == PASS) return move;
	const int i = move / 8;
	const int j = move % 8;
	return (7 - j) * 8 + i;
}

class __Board
{
public:
	__Board(const __Board &) = default;
	__Board() : bd(board::initial_board()), is_black_turn(true) {}
	__Board(const std::string &line, const bool is_black_turn) : bd(is_black_turn ? utils::line_to_bd<true>(line) : utils::line_to_bd<false>(line)), is_black_turn(is_black_turn) {}
	~__Board() {}

	operator const board&() const {
		return bd;
	}

	void set(const std::string &line, const bool is_black_turn) {
		bd = is_black_turn ? utils::line_to_bd<true>(line) : utils::line_to_bd<false>(line);
	}

	void set_bitboard(const char* bitboard, const bool is_black_turn) {
		const uint64_t* player = (const uint64_t*)bitboard;
		const uint64_t* opponent = (const uint64_t*)(bitboard + 8);
		bd = board(*player, *opponent);
		this->is_black_turn = is_black_turn;
	}

	void to_bitboard(char* bitboard) const {
		const uint64_t& player = bd.player();
		const uint64_t& opponent = bd.opponent();
		std::memcpy(bitboard, (const char*)&player, 8);
		std::memcpy(bitboard + 8, (const char*)&opponent, 8);
	}

	void reset() {
		bd = board::initial_board();
		is_black_turn = true;
	}

	std::string dump() const {
		return is_black_turn ? utils::to_s<true>(bd) : utils::to_s<false>(bd);
	}

	void move(const int move) {
		if (move == PASS)
			bd = board::reverse_board(bd);
		else
			bd = state::move(bd, move);
		is_black_turn = !is_black_turn;
	}

	int move_from_str(const std::string& str) {
		int move = to_hand(str);
		bd = state::move(bd, move);
		is_black_turn = !is_black_turn;
		return move;
	}

	void move_pass() {
		bd = board::reverse_board(bd);
		is_black_turn = !is_black_turn;
	}

	bool is_legal(const int move) {
		const uint64_t mobility_pos = state::mobility_pos(bd);
		if (move == PASS && mobility_pos == 0)
			return true;
		return half_board(mobility_pos).get(move);
	}

	bool is_game_over() const {
		return state::is_gameover(bd);
	}

	int piece_num() const {
		return (int)_popcnt64(bd.player());
	}

	int opponent_piece_num() const {
		return (int)_popcnt64(bd.opponent());
	}

	int piece_sum() const {
		return bit_manipulations::stone_sum(bd);
	}

	int puttable_num() const {
		return state::mobility_count(bd);
	}

	int opponent_puttable_num() const {
		return state::mobility_count(board::reverse_board(bd));
	}

	int diff_num() const {
		return value::diff_num(bd);
	}

	int puttable_diff() const {
		return value::puttable_diff(bd);
	}

	bool turn() const {
		return is_black_turn;
	}

	std::string to_s_ffo() const {
		return is_black_turn ? utils::to_s_ffo<true>(bd) : utils::to_s_ffo<false>(bd);
	}

	int piece(const int sq) const {
		if (bd.player().get(sq)) {
			return is_black_turn ? Black : White;
		}
		else if (bd.opponent().get(sq)) {
			return is_black_turn ? White : Black;
		}
		else {
			return Empty;
		}
	}

	void piece_planes(char* mem) const {
		float (*data)[64] = (float(*)[64])mem;
		std::fill_n((float*)data, 64 * 2, 0.0f);
		for (uint64_t bits = bd.player(); bits; bits = _blsr_u64(bits)) {
			int sq = bit_manipulations::bit_to_pos(_blsi_u64(bits));
			data[0][sq] = 1.0f;
		}
		for (uint64_t bits = bd.opponent(); bits; bits = _blsr_u64(bits)) {
			int sq = bit_manipulations::bit_to_pos(_blsi_u64(bits));
			data[1][sq] = 1.0f;
		}
	}

	void piece_planes_rotate90(char* mem) const {
		float(*data)[64] = (float(*)[64])mem;
		std::fill_n((float*)data, 64 * 2, 0.0f);
		for (uint64_t bits = bd.player(); bits; bits = _blsr_u64(bits)) {
			int sq = bit_manipulations::bit_to_pos(_blsi_u64(bits));
			data[0][__move_rotate90(sq)] = 1.0f;
		}
		for (uint64_t bits = bd.opponent(); bits; bits = _blsr_u64(bits)) {
			int sq = bit_manipulations::bit_to_pos(_blsi_u64(bits));
			data[1][__move_rotate90(sq)] = 1.0f;
		}
	}

	void piece_planes_rotate180(char* mem) const {
		float(*data)[64] = (float(*)[64])mem;
		std::fill_n((float*)data, 64 * 2, 0.0f);
		for (uint64_t bits = bd.player(); bits; bits = _blsr_u64(bits)) {
			int sq = bit_manipulations::bit_to_pos(_blsi_u64(bits));
			data[0][__move_rotate180(sq)] = 1.0f;
		}
		for (uint64_t bits = bd.opponent(); bits; bits = _blsr_u64(bits)) {
			int sq = bit_manipulations::bit_to_pos(_blsi_u64(bits));
			data[1][__move_rotate180(sq)] = 1.0f;
		}
	}

	void piece_planes_rotate270(char* mem) const {
		float(*data)[64] = (float(*)[64])mem;
		std::fill_n((float*)data, 64 * 2, 0.0f);
		for (uint64_t bits = bd.player(); bits; bits = _blsr_u64(bits)) {
			int sq = bit_manipulations::bit_to_pos(_blsi_u64(bits));
			data[0][__move_rotate270(sq)] = 1.0f;
		}
		for (uint64_t bits = bd.opponent(); bits; bits = _blsr_u64(bits)) {
			int sq = bit_manipulations::bit_to_pos(_blsi_u64(bits));
			data[1][__move_rotate270(sq)] = 1.0f;
		}
	}

private:
	board bd;
	bool is_black_turn;
};

class __LegalMoveList
{
public:
	__LegalMoveList() {}
	__LegalMoveList(const __Board& board) : bits(state::mobility_pos(board)), pass(false) {
		if (!bits)
			pass = true;
	}

	bool end() const {
		if (!pass)
			return !bits;
		else
			return false;
	}
	int next() {
		if (!pass) {
			int sq = bit_manipulations::bit_to_pos(_blsi_u64(bits));
			bits = _blsr_u64(bits);
			return sq;
		}
		else {
			pass = false;
			return PASS;
		}
	}
	int size() const {
		if (!pass)
			return (int)_popcnt64(bits);
		else
			return 1;
	}

private:
	uint64_t bits;
	bool pass;
};

std::string __move_to_str(const int move) { return to_s(move); }
std::string __move_to_STR(const int move) { return to_S(move); }
int __move_from_str(const std::string& str) { return to_hand(str); }
