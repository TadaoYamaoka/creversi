creversi: 高速なPythonのリバーシライブラリ
==========================================

概要
----

creversiは、盤面管理、合法手生成、および機械学習向けのサポートを備えた高速なPythonのリバーシライブラリです。
以下は、盤を作成して、開始局面で合法手を生成して表示し、1手打つ処理の例です。

.. code:: python

    >>> import creversi

    >>> board = creversi.Board()

    >>> for move in board.legal_moves:
    ...     print(creversi.move_to_str(move))

::

    d3
    c4
    f5
    e6

.. code:: python

    >>> board.move_from_str('d3')

機能
----

* Python 3.5以上とCython 0.29以上をサポート

* IPython/Jupyter Notebookと統合

  .. code:: python

      >>> board

  .. image:: https://raw.githubusercontent.com/wiki/TadaoYamaoka/creversi/images/board.svg?sanitize=true

  直前の手をハイライトして表示する場合

  .. code:: python

      >>> move = creversi.move_from_str('c3')
      >>> board.to_svg(move)

  .. image:: https://raw.githubusercontent.com/wiki/TadaoYamaoka/creversi/images/board2.svg?sanitize=true

* テキスト形式で盤面を表示

  .. code:: python

      >>> board = creversi.Board('------------------OOO------OXX----OOXX----OX--------------------', creversi.BLACK_TURN)
      >>> print(board)

  ::

         |abcdefgh
        -+--------
        1|........
        2|........
        3|..ooo...
        4|...oxx..
        5|..ooxx..
        6|..ox....
        7|........
        8|........

* 打ち手の表現

  打ち手は0から64の数値で扱う。座標a1が0、b1が1、…、h8が63になり、パスが64になる。ヘルパー関数で文字列形式に変換できる。

  .. code:: python

      >>> move = list(board.legal_moves)[0]
      >>> move
      9
      >>> creversi.move_to_str(move)
      'b2'

  文字列形式から数値の打ち手に変換できる。パスを表す文字列は、'pass'となる。

  .. code:: python

      >>> creversi.move_from_str('b2')
      9

* 打つ

  数値の打ち手を使う場合

  .. code:: python

      >>> move = creversi.move_from_str('d3')
      >>> board.move(move)

  文字列を直接の打ち手に使う場合

  .. code:: python

      >>> board.move_from_str('d3')

* 合法手生成

  合法手生成は、Pythonのイテレータで取得する。

  .. code:: python

      >>> for move in board.legal_moves
      ...     print(creversi.move_to_str(move))

* 合法手チェック

  手が合法かどうかチェックする。

  .. code:: python

      >>> board.is_legal(move)
      False

* 手番の表現

  手番はbool型で黒番かどうかを表す。黒番、白番はそれぞれ定数BLACK_TURN、WHITE_TURNで定義されている。

  .. code:: python

      >>> board.turn
      True
      >>> board.turn == creversi.WHITE_TURN
      False

* 終局判定

  .. code:: python

      >>> board.is_game_over()
      False

* 局面の文字列形式

  局面を文字列として取得、設定することができる。

  .. code:: python

      >>> line = board.to_line()
      >>> line
      '------------------OOO------OXX----OOXX----OX--------------------'

      >>> board.set_line('------------------OOO------OXX----OOXX----OX--------------------', creversi.BLACK_TURN)

  コンストラクタでも初期化可能

  .. code:: python

      >>> board = creversi.Board('------------------OOO------OXX----OOXX----OX--------------------', creversi.BLACK_TURN)

* 石の数の取得

  石の合計

  .. code:: python

      >>> board.piece_sum()

  手番側の石の数

  .. code:: python

      >>> board.piece_num()

  相手番側の石の数

  .. code:: python

      >>> board.opponent_piece_num()

  手番側から見た石の差

  .. code:: python

      >>> board.diff_num()

  置ける石の数

  .. code:: python

      >>> board.puttable_num()

  相手の置ける石の数

  .. code:: python

      >>> board.opponent_puttable_num()

* 局面のビットボード形式

  局面をビットボードとして取得、設定することができる。

  .. code:: python

      >>> import numpy as np

      >>> bitboard = np.empty(1, creversi.dtypeBitboard)
      >>> board.to_bitboard(bitboard)

      >>> board.set_bitboard(bitboard, creversi.BLACK_TURN)

* 局面の2次元ベクトル表現

  石のある位置を1、それ以外を0とした2次元ベクトルを、手番側の石と相手番の石の2チャンネルで表現したものをNCHW形式のndarrayで取得できる。
  畳み込み層への入力とすることができる。

  .. code:: python

      >>> import numpy as np

      >>> planes = np.empty(1, np.empty((1, 2, 8, 8), dtype=np.float32))
      >>> board.piece_planes(planes[0])

  2次元ベクトルを90°、180°、270°回転したものも取得できる。

  .. code:: python

      >>> board.piece_planes_rotate90(planes[0])
      >>> board.piece_planes_rotate180(planes[0])
      >>> board.piece_planes_rotate270(planes[0])

* 機械学習向け訓練データ形式

  機械学習向け訓練データ形式をサポートする。

  .. code:: python

      >>> import numpy as np

      >>> data = np.empty(1, creversi.TrainingData)
      >>> board.to_bitboard(data['bitboard'])
      >>> data['turn'] = board.turn
      >>> data['move'] = list(board.legal_moves)[0]
      >>> data['reward'] = 1
      >>> data['done'] = False

* Gym環境

  OpenAI Gymのインターフェースをサポートする。

  .. code:: python

      >>> import gym
      >>> import creversi.gym_reversi

      >>> env = gym.make('Reversi-v0').unwrapped

      >>> env.reset()
      >>> env.board
      >>> next_board, reward, done, _ = env.step(move)

  環境の並列実行バージョンを提供する。

  .. code:: python

      >>> import gym
      >>> from creversi.gym_reversi.envs import ReversiVecEnv

      >>> BATCH_SIZE = 8
      >>> vecenv = ReversiVecEnv(BATCH_SIZE)

      >>> board0 = vecenv.envs[0].board
      >>> rewards, dones = vecenv.step(moves)

インストール
------------

以下のコマンドでインストールします。インストールにはCythonと対応したC++コンパイラが必要です。

::

    pip install git+https://github.com/TadaoYamaoka/creversi

謝辞
----

高速化のために多くの部分で
`issen <https://github.com/primenumber/issen>`_
のソースを流用しています。

ライセンス
----------

creversiはGPL3の元にライセンスされています。詳細はLICENSEを確認してください。
