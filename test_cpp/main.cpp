#include <iostream>

#include "board.hpp"
#include "state.hpp"
#include "hand.hpp"
#include "utils.hpp"
#include "bit_manipulations.hpp"

#include "creversi.h"
#include "ggf_parser.h"

void test() {
	board bd = board::initial_board();

	std::cout << utils::to_s<true>(bd) << std::endl;

	bool is_pass = true;
	//for (uint64_t bits = ~bit_manipulations::stones(bd); bits; bits = _blsr_u64(bits)) {
	for (uint64_t bits = state::mobility_pos(bd); bits; bits = _blsr_u64(bits)) {
		int pos = bit_manipulations::bit_to_pos(_blsi_u64(bits));
		board next = state::move(bd, pos);
		if (next.player() != bd.opponent()) {
			std::cout << pos << "\t" << to_s(pos) << std::endl;
			is_pass = false;
		}
	}

	hand h = to_hand("c4");
	bd = state::move(bd, h);

	std::cout << utils::to_s<false>(bd) << std::endl;
}

void test_ggf_parser() {
	auto board = __Board();
	auto parser = __GgfParser();
	parser.parse_file("R:\\othello_ggf\\Othello.01e4.ggf");
	for (int i = 0; i < parser.game_num(); ++i) {
		board.reset();
		std::cout << parser.names[i][0] << "," << parser.names[i][1];
		for (auto move : parser.moves[i]) {
			//std::cout << " " << move;
			if (move != PASS)
				if (!board.is_legal(move))
					throw "illegal move";
			board.move(move);
		}
		std::cout << " " << parser.results[i] << " " << parser.result_flags[i] << std::endl;
	}
}

int main() {
	utils::init_all();

	//test();
	test_ggf_parser();
}