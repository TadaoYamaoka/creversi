from libcpp.string cimport string
from libcpp.vector cimport vector
from libcpp cimport bool

import numpy as np
cimport numpy as np

import locale

dtypeBitboard = np.dtype((np.uint8, 16))
dtypeTurn = np.dtype(np.bool)
dtypeMove = np.dtype(np.int8)
dtypeReward = np.dtype(np.int8)
dtypeDone = np.dtype(np.bool)

TrainingData = np.dtype([
	('bitboard', dtypeBitboard),
	('turn', dtypeTurn),
	('move', dtypeMove),
	('reward', dtypeReward),
	('done', dtypeDone),
	])


SQUARES = [
	A1, B1, C1, D1, E1, F1, G1, H1,
	A2, B2, C2, D2, E2, F2, G2, H2,
	A3, B3, C3, D3, E3, F3, G3, H3,
	A4, B4, C4, D4, E4, F4, G4, H4,
	A5, B5, C5, D5, E5, F5, G5, H5,
	A6, B6, C6, D6, E6, F6, G6, H6,
	A7, B7, C7, D7, E7, F7, G7, H7,
	A8, B8, C8, D8, E8, F8, G8, H8,
] = range(64)

[BLACK_TURN, WHITE_TURN] = [True, False]
PIECES = [EMPTY, BLACK, WHITE] = range(3)

PASS = 64
RESULT_FLAGS = [NONE, RESIGNED, TIME_OUT, MUTUAL_SCORE] = range(4)

SVG_PIECE_DEFS = [
	'<g id="black"><circle cx="10" cy="10" r="8.5" fill="black"/></g>',
	'<g id="white"><circle cx="10" cy="10" r="8.5" fill="white"/></g>',
]
SVG_PIECE_DEF_IDS = [None,
	"black", "white",
]
SVG_BOARD = '<rect fill="green" height="161" width="161" x="10" y="10" />'
SVG_SQUARES = '<g stroke="black"><rect width="161" height="161" stroke-width="1.5" fill="none" x="10" y="10" /><line stroke-width="1.0" x1="10.5" x2="170.5" y1="30.5" y2="30.5" /><line stroke-width="1.0" x1="10.5" x2="170.5" y1="50.5" y2="50.5" /><line stroke-width="1.0" x1="10.5" x2="170.5" y1="70.5" y2="70.5" /><line stroke-width="1.0" x1="10.5" x2="170.5" y1="90.5" y2="90.5" /><line stroke-width="1.0" x1="10.5" x2="170.5" y1="110.5" y2="110.5" /><line stroke-width="1.0" x1="10.5" x2="170.5" y1="130.5" y2="130.5" /><line stroke-width="1.0" x1="10.5" x2="170.5" y1="150.5" y2="150.5" /><line stroke-width="1.0" x1="30.5" x2="30.5" y1="10.5" y2="170.5" /><line stroke-width="1.0" x1="50.5" x2="50.5" y1="10.5" y2="170.5" /><line stroke-width="1.0" x1="70.5" x2="70.5" y1="10.5" y2="170.5" /><line stroke-width="1.0" x1="90.5" x2="90.5" y1="10.5" y2="170.5" /><line stroke-width="1.0" x1="110.5" x2="110.5" y1="10.5" y2="170.5" /><line stroke-width="1.0" x1="130.5" x2="130.5" y1="10.5" y2="170.5" /><line stroke-width="1.0" x1="150.5" x2="150.5" y1="10.5" y2="170.5" /></g>'
SVG_COORDINATES = '<g><text font-family="serif" font-size="9.5" text-anchor="middle" x="20.5" y="8">a</text><text font-family="serif" font-size="9.5" text-anchor="middle" x="40.5" y="8">b</text><text font-family="serif" font-size="9.5" text-anchor="middle" x="60.5" y="8">c</text><text font-family="serif" font-size="9.5" text-anchor="middle" x="80.5" y="8">d</text><text font-family="serif" font-size="9.5" text-anchor="middle" x="100.5" y="8">e</text><text font-family="serif" font-size="9.5" text-anchor="middle" x="120.5" y="8">f</text><text font-family="serif" font-size="9.5" text-anchor="middle" x="140.5" y="8">g</text><text font-family="serif" font-size="9.5" text-anchor="middle" x="160.5" y="8">h</text><text font-family="serif" font-size="9.5" x="2.5" y="23.5">1</text><text font-family="serif" font-size="9.5" x="2.5" y="43.5">2</text><text font-family="serif" font-size="9.5" x="2.5" y="63.5">3</text><text font-family="serif" font-size="9.5" x="2.5" y="83.5">4</text><text font-family="serif" font-size="9.5" x="2.5" y="103.5">5</text><text font-family="serif" font-size="9.5" x="2.5" y="123.5">6</text><text font-family="serif" font-size="9.5" x="2.5" y="143.5">7</text><text font-family="serif" font-size="9.5" x="2.5" y="163.5">8</text></g>'

class SvgWrapper(str):
	def _repr_svg_(self):
		return self

cdef extern from "utils.hpp" namespace "utils":
	void init_all()

init_all()

cdef extern from "creversi.h":
	cdef cppclass __Board:
		__Board() except +
		__Board(const string &line, const bool is_black_turn) except +
		__Board(const __Board& board, const bool is_black_turn) except +
		void set(const string &line, const bool is_black_turn)
		void set_bitboard(const char* bitboard, const bool is_black_turn)
		void to_bitboard(char* bitboard)
		void reset()
		string dump()
		void move(const int move)
		int move_from_str(const string& str)
		void move_pass()
		bool is_legal(const int move)
		bool is_game_over()
		int piece_num()
		int opponent_piece_num()
		int piece_sum()
		int puttable_num()
		int opponent_puttable_num()
		int diff_num()
		int puttable_diff()
		bool turn()
		string to_s_ffo()
		int piece(const int sq)
		void piece_planes(char* mem)
		void piece_planes_rotate90(char* mem)
		void piece_planes_rotate180(char* mem)
		void piece_planes_rotate270(char* mem)

cdef class Board:
	cdef __Board __board

	def __cinit__(self, str line=None, bool is_black_turn=True):
		cdef string line_b
		if line:
			line_b = line.encode('ascii')
			self.__board = __Board(line_b, is_black_turn)
		else:
			self.__board = __Board()

	def __copy__(self):
		return Board(board=self)

	def copy(self):
		return Board(board=self)

	def set_line(self, str line, bool is_black_turn):
		cdef string line_b = line.encode('ascii')
		self.__board.set(line_b, is_black_turn)

	def set_bitboard(self, np.ndarray bitboard, bool is_black_turn):
		self.__board.set_bitboard(bitboard.data, is_black_turn)

	def to_bitboard(self, np.ndarray bitboard):
		self.__board.to_bitboard(bitboard.data)

	def reset(self):
		self.__board.reset()

	def __repr__(self):
		return self.__board.dump().decode('ascii')

	def move(self, int move):
		self.__board.move(move)

	def move_from_str(self, str str):
		cdef string str_b = str.encode('ascii')
		return self.__board.move_from_str(str_b)

	def move_pass(self):
		self.__board.move_pass()

	def is_legal(self, int move):
		return self.__board.is_legal(move)

	@property
	def legal_moves(self):
		return LegalMoveList(self)

	def is_game_over(self):
		return self.__board.is_game_over()

	def piece_num(self):
		return self.__board.piece_num()

	def opponent_piece_num(self):
		return self.__board.opponent_piece_num()

	def piece_sum(self):
		return self.__board.piece_sum()

	def puttable_num(self):
		return self.__board.puttable_num()

	def opponent_puttable_num(self):
		return self.__board.opponent_puttable_num()

	def diff_num(self):
		return self.__board.diff_num()

	def puttable_diff(self):
		return self.__board.puttable_diff()

	@property
	def turn(self):
		return self.__board.turn()

	def to_line(self):
		return self.__board.to_s_ffo().decode('ascii')

	def piece(self, int sq):
		return self.__board.piece(sq)

	def piece_planes(self, np.ndarray features):
		return self.__board.piece_planes(features.data)

	def piece_planes_rotate90(self, np.ndarray features):
		return self.__board.piece_planes_rotate90(features.data)

	def piece_planes_rotate180(self, np.ndarray features):
		return self.__board.piece_planes_rotate180(features.data)

	def piece_planes_rotate270(self, np.ndarray features):
		return self.__board.piece_planes_rotate270(features.data)

	def to_svg(self, lastmove=None, float scale=1.0):
		import xml.etree.ElementTree as ET

		svg = ET.Element("svg", {
			"xmlns": "http://www.w3.org/2000/svg",
			"version": "1.1",
			"xmlns:xlink": "http://www.w3.org/1999/xlink",
			"width": str(215 * scale),
			"height": str(215 * scale),
			"viewBox": "0 0 172 172",
		})

		defs = ET.SubElement(svg, "defs")
		for piece_def in SVG_PIECE_DEFS:
			defs.append(ET.fromstring(piece_def))

		svg.append(ET.fromstring(SVG_BOARD))

		cdef int i, j
		if lastmove is not None and lastmove != PASS:
			i, j = divmod(lastmove, 8)
			ET.SubElement(svg, "rect", {
				"x": str(10.5 + j * 20),
				"y": str(10.5 + i * 20),
				"width": str(20),
				"height": str(20),
				"fill": "#8bbf83"
			})

		svg.append(ET.fromstring(SVG_SQUARES))
		svg.append(ET.fromstring(SVG_COORDINATES))

		cdef int sq
		cdef double x, y
		for sq in SQUARES:
			pc = self.__board.piece(sq)
			if pc != NONE:
				i, j = divmod(sq, 8)
				x = 10.5 + j * 20
				y = 10.5 + i * 20

				ET.SubElement(svg, "use", {
					"xlink:href": "#{}".format(SVG_PIECE_DEF_IDS[pc]),
					"x": str(x),
					"y": str(y),
				})

		return SvgWrapper(ET.tostring(svg).decode("utf-8"))
	
	def _repr_svg_(self):
		return self.to_svg()

cdef extern from "creversi.h":
	cdef cppclass __LegalMoveList:
		__LegalMoveList() except +
		__LegalMoveList(const __Board& board) except +
		bool end()
		int next()
		int size()

	string __move_to_str(const int move)
	string __move_to_STR(const int move)
	int __move_from_str(const string& str) except +
	int __move_rotate90(const int move)
	int __move_rotate180(const int move)
	int __move_rotate270(const int move)

cdef class LegalMoveList:
	cdef __LegalMoveList __ml

	def __cinit__(self, Board board):
		self.__ml = __LegalMoveList(board.__board)

	def __iter__(self):
		return self

	def __next__(self):
		if self.__ml.end():
			raise StopIteration()
		return self.__ml.next()

	def __len__(self):
		return self.__ml.size()

def move_to_str(int move):
	return __move_to_str(move).decode('ascii')

def move_to_STR(int move):
	return __move_to_STR(move).decode('ascii')

def move_from_str(str str):
	cdef string str_b = str.encode('ascii')
	return __move_from_str(str_b)

def move_rotate90(int move):
	return __move_rotate90(move)

def move_rotate180(int move):
	return __move_rotate180(move)

def move_rotate270(int move):
	return __move_rotate270(move)

cdef extern from "ggf_parser.h":
	cdef cppclass __GgfParser:
		__GgfParser() except +
		vector[vector[string]] names;
		vector[vector[float]] ratings;
		vector[int] results;
		vector[int] result_flags;
		vector[vector[int]] moves;
		vector[vector[float]] evaluations;
		void parse_file(const string& path) except +
		void parse_str(const string& str)
		int game_num()

cdef class GgfParser:
	cdef __GgfParser __parser

	def __cinit__(self):
		self.__parser = __GgfParser()

	def parse_file(self, str path):
		cdef string path_b = path.encode(locale.getpreferredencoding())
		self.__parser.parse_file(path_b)

	def parse_str(self, str str):
		cdef string str_b = str.encode('ascii')
		self.__parser.parse_str(str_b)

	@property
	def names(self):
		return [[name.decode('ascii') for name in names] for names in self.__parser.names]

	@property
	def ratings(self):
		return self.__parser.ratings

	@property
	def results(self):
		return self.__parser.results

	@property
	def result_flags(self):
		return self.__parser.result_flags

	@property
	def moves(self):
		return self.__parser.moves

	@property
	def evaluations(self):
		return self.__parser.evaluations

	def game_num(self):
		return self.__parser.game_num()