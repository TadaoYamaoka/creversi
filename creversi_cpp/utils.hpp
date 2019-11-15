#pragma once

#include <string>
#include <utility>
#include <tuple>

#include "board.hpp"

namespace utils {

	void init_all();
	std::string to_s(uint64_t);
	template<bool is_black>
	std::string to_s(const board &);
	template<bool is_black>
	std::string to_s_ffo(const board &);
	std::pair<board, bool> input();
	template<bool is_black>
	board line_to_bd(const std::string &);

} // namespace utils
