from .creversi import GgfParser as Parser
import creversi

class Exporter:
    def __init__(self, path=None):
        if path:
            self.open(path)
        else:
            self.kifu = None

    def open(self, path):
        self.kifu = open(path, 'w')

    def close(self):
        self.kifu.close()

    def newgame(self, names=['', '']):
        self.kifu.write('(;GM[Othello]PB[{}]PW[{}]'.format(names[0], names[1]))
        self.moves = []
        self.is_black_turn = True

    def move(self, move, evaluation=None):
        self.moves.append(('B[' if self.is_black_turn else 'W[') + creversi.move_to_STR(move) + ('' if evaluation is None else ('/' + str(evaluation))) + ']')
        self.is_black_turn = not self.is_black_turn

    def endgame(self, result):
        self.kifu.write('TY[8]RE[' + str(result) + ']BO[8 ---------------------------O*------*O--------------------------- *]')
        for move in self.moves:
            self.kifu.write(move)
        self.kifu.write(';)\n')