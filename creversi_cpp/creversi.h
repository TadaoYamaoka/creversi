#pragma once

#include "board.hpp"
#include "state.hpp"
#include "hand.hpp"
#include "utils.hpp"
#include "value.hpp"

enum Piece {
	Empty = 0,
	Black,
	White
};

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

	void reset() {
		bd = board::initial_board();
	}

	std::string dump() const {
		return is_black_turn ? utils::to_s<true>(bd) : utils::to_s<false>(bd);
	}

	void move(const int move) {
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

	bool is_game_over() const {
		return state::is_gameover(bd);
	}

	int stone_sum() const {
		return bit_manipulations::stone_sum(bd);
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
		for (uint64_t bits = bd.player(); bits; bits = _blsr_u64(bits)) {
			int sq = bit_manipulations::bit_to_pos(_blsi_u64(bits));
			data[0][sq] = 1.0f;
		}
		for (uint64_t bits = bd.player(); bits; bits = _blsr_u64(bits)) {
			int sq = bit_manipulations::bit_to_pos(_blsi_u64(bits));
			data[1][sq] = 1.0f;
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
	__LegalMoveList(const __Board& board) : bits(state::mobility_pos(board)) {}

	bool end() const { return !bits; }
	int next() {
		int sq = bit_manipulations::bit_to_pos(_blsi_u64(bits));
		bits = _blsr_u64(bits);
		return sq;
	}
	int size() const { return (int)_popcnt64(bits); }

private:
	uint64_t bits;
};

std::string __move_to_str(const int move) { return to_s(move); }
int __move_from_str(const std::string str) { return to_hand(str); }
