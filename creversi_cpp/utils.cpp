#include "utils.hpp"

#include <iostream>

#include "bit_manipulations.hpp"
#include "state.hpp"

namespace utils {

	void init_all() {
		bit_manipulations::init();
	}

	std::string to_s(uint64_t bits) {
		half_board bb(bits);
		std::string res;
		for (int i = 0; ; ++i) {
			for (int j = 0; j < 8; ++j) {
				res += bb.get(i * 8 + j) ? 'x' : '.';
			}
			if (i == 7)
				break;
			res += '\n';
		}
		return res;
	}

	template<bool is_black>
	std::string to_s(const board &bd) {
		std::string res;
		res += " |abcdefgh\n";
		res += "-+--------";
		for (int i = 0; i < 8; ++i) {
			res += '\n';
			res += (char)('1' + i);
			res += '|';
			for (int j = 0; j < 8; ++j) {
				if (bd.player().get(i * 8 + j)) {
					res += is_black ? 'x' : 'o';
				}
				else if (bd.opponent().get(i * 8 + j)) {
					res += is_black ? 'o' : 'x';
				}
				else {
					res += '.';
				}
			}
		}
		return res;
	}
	template std::string to_s<true>(const board &bd);
	template std::string to_s<false>(const board &bd);

	template<bool is_black>
	std::string to_s_ffo(const board &bd) {
		std::string res;
		for (int i = 0; i < 8; ++i) {
			for (int j = 0; j < 8; ++j) {
				if (bd.player().get(i * 8 + j)) {
					res += is_black ? 'X' : 'O';
				}
				else if (bd.opponent().get(i * 8 + j)) {
					res += is_black ? 'O' : 'X';
				}
				else {
					res += '-';
				}
			}
		}
		return res;
	}
	template std::string to_s_ffo<true>(const board &bd);
	template std::string to_s_ffo<false>(const board &bd);

	std::pair<board, bool> input() {
		std::string hs;
		std::cin >> hs;
		char hand = hs[0];
		half_board black = 0;
		half_board white = 0;
		for (int i = 0; i < 8; ++i) {
			std::string line;
			std::cin >> line;
			for (int j = 0; j < 8; ++j) {
				switch (line[j]) {
				case 'x':
					black.set(i * 8 + j);
					white.reset(i * 8 + j);
					break;
				case 'o':
					black.reset(i * 8 + j);
					white.set(i * 8 + j);
					break;
				default:
					black.reset(i * 8 + j);
					white.reset(i * 8 + j);
					break;
				}
			}
		}
		board bd(black, white);
		if (hand == 'x') {
			return std::make_pair(bd, true);
		}
		else {
			return std::make_pair(board::reverse_board(bd), false);
		}
	}

	template<bool is_black>
	board line_to_bd(const std::string &line) {
		half_board black = 0;
		half_board white = 0;
		for (int i = 0; i < 64; ++i) {
			switch (line[i]) {
			case is_black ? 'X' : 'O':
				black.set(i);
				white.reset(i);
				break;
			case is_black ? 'O' : 'X':
				black.reset(i);
				white.set(i);
				break;
			default:
				black.reset(i);
				white.reset(i);
				break;
			}
		}
		return board(black, white);
	}
	template board line_to_bd<true>(const std::string &line);
	template board line_to_bd<false>(const std::string &line);

} // namespace utils
