/* See.cpp
 * 
 * Kubo Ryosuke
 */

#include "See.h"
#include "core/move/MoveGenerator.h"
#include "core/move/MoveTable.h"
#include "core/util/Data.h"
#include <algorithm>

namespace sunfish {

	template <bool shallow, Direction dir, bool isFirst>
	void See::generateAttacker(const Board& board, const Position& to, const Bitboard& occ, Attacker* dependOn, bool shortOnly) {

		// 斜め
		if (dir == Direction::LeftUp || dir == Direction::LeftDown ||
				dir == Direction::RightUp || dir == Direction::RightDown) {
			// 距離1
			if (isFirst) {
				auto from = (dir == Direction::LeftUp ? to.leftUp() :
										 dir == Direction::LeftDown ? to.leftDown() :
										 dir == Direction::RightUp ? to.rightUp() :
										 to.rightDown());
				auto piece = board.getBoardPiece(from);
				if (!piece.isEmpty()) {
  				if (dir == Direction::LeftUp ? MovableTable[piece].rightDown :
  						 dir == Direction::LeftDown ? MovableTable[piece].rightUp :
  						 dir == Direction::RightUp ? MovableTable[piece].leftDown :
  						 MovableTable[piece].leftUp) {
						if (piece.isBlack()) {
							_b[_bnum++] = { material::pieceExchange(piece), dependOn, false };
							if (!shallow && !shortOnly) { generateAttackerR<false, dir>(board, from, occ, &_b[_bnum-1]); }
						} else {
							assert(piece.isWhite());
							_w[_wnum++] = { material::pieceExchange(piece), dependOn, false };
							if (!shallow && !shortOnly) { generateAttackerR<false, dir>(board, from, occ, &_w[_wnum-1]); }
						}
  				}
					return;
				}
			}

			// 長い距離
			if (!shortOnly) {
  			auto bb = (dir == Direction::LeftUp ? MoveTables::LeftUp.get(to, occ) :
  								 dir == Direction::LeftDown ? MoveTables::LeftDown.get(to, occ) :
  								 dir == Direction::RightUp ? MoveTables::RightUp.get(to, occ) :
  								 MoveTables::RightDown.get(to, occ));
  			if (bb) {
    			auto result = bb & board.getBBishop();
    			if (result) {
    				auto from = result.pickFirst();
  					_b[_bnum++] = { material::BishopEx, dependOn, false };
  					if (!shallow) { generateAttackerR<false, dir>(board, from, occ, &_b[_bnum-1]); }
  					return;
    			}
    			result = bb & board.getWBishop();
    			if (result) {
    				auto from = result.pickFirst();
  					_w[_wnum++] = { material::BishopEx, dependOn, false };
  					if (!shallow) { generateAttackerR<false, dir>(board, from, occ, &_w[_wnum-1]); }
  					return;
    			}
    			result = bb & board.getBHorse();
    			if (result) {
    				auto from = result.pickFirst();
  					_b[_bnum++] = { material::HorseEx, dependOn, false };
  					if (!shallow) { generateAttackerR<false, dir>(board, from, occ, &_b[_bnum-1]); }
  					return;
    			}
    			result = bb & board.getWHorse();
    			if (result) {
    				auto from = result.pickFirst();
  					_w[_wnum++] = { material::HorseEx, dependOn, false };
  					if (!shallow) { generateAttackerR<false, dir>(board, from, occ, &_w[_wnum-1]); }
  					return;
    			}
  			}
			}
		}

		// 縦と横
		else if (dir == Direction::Up || dir == Direction::Down ||
						 dir == Direction::Left || dir == Direction::Right) {
			// 距離1
			if (isFirst) {
				auto from = (dir == Direction::Up ? to.up() :
										 dir == Direction::Down ? to.down() :
										 dir == Direction::Left ? to.left() :
										 to.right());
				auto piece = board.getBoardPiece(from);
				if (!piece.isEmpty()) {
  				if (dir == Direction::Up ? MovableTable[piece].down :
  						 dir == Direction::Down ? MovableTable[piece].up :
  						 dir == Direction::Left ? MovableTable[piece].right :
  						 MovableTable[piece].left) {
						if (piece.isBlack()) {
							_b[_bnum++] = { material::pieceExchange(piece), dependOn, false };
							if (!shallow && !shortOnly) { generateAttackerR<false, dir>(board, from, occ, &_b[_bnum-1]); }
						} else {
							assert(piece.isWhite());
							_w[_wnum++] = { material::pieceExchange(piece), dependOn, false };
							if (!shallow && !shortOnly) { generateAttackerR<false, dir>(board, from, occ, &_w[_wnum-1]); }
						}
  				}
					return;
				}
			}

			// 長い距離
			if (!shortOnly) {
  			auto bb = (dir == Direction::Up ? MoveTables::BLance.get(to, occ) :
  								 dir == Direction::Down ? MoveTables::WLance.get(to, occ) :
  								 dir == Direction::Left ? MoveTables::Left.get(to, occ) :
  								 MoveTables::Right.get(to, occ));
  			auto result = bb & board.getBRook();
  			if (result) {
  				auto from = result.pickFirst();
  				_b[_bnum++] = { material::RookEx, dependOn, false };
  				if (!shallow) { generateAttackerR<false, dir>(board, from, occ, &_b[_bnum-1]); }
  				return;
  			}
  			result = bb & board.getWRook();
  			if (result) {
  				auto from = result.pickFirst();
  				_w[_wnum++] = { material::RookEx, dependOn, false };
  				if (!shallow) { generateAttackerR<false, dir>(board, from, occ, &_w[_wnum-1]); }
  				return;
  			}
  			result = bb & board.getBDragon();
  			if (result) {
  				auto from = result.pickFirst();
  				_b[_bnum++] = { material::DragonEx, dependOn, false };
  				if (!shallow) { generateAttackerR<false, dir>(board, from, occ, &_b[_bnum-1]); }
  				return;
  			}
  			result = bb & board.getWDragon();
  			if (result) {
  				auto from = result.pickFirst();
  				_w[_wnum++] = { material::DragonEx, dependOn, false };
  				if (!shallow) { generateAttackerR<false, dir>(board, from, occ, &_w[_wnum-1]); }
  				return;
  			}
  			if (dir == Direction::Down) {
  				result = bb & board.getBLance();
  				if (result) {
  					auto from = result.pickFirst();
  					_b[_bnum++] = { material::LanceEx, dependOn, false };
  					if (!shallow) { generateAttackerR<false, dir>(board, from, occ, &_b[_bnum-1]); }
    				return;
  				}
  			} else if (dir == Direction::Up) {
  				result = bb & board.getWLance();
  				if (result) {
  					auto from = result.pickFirst();
  					_w[_wnum++] = { material::LanceEx, dependOn, false };
  					if (!shallow) { generateAttackerR<false, dir>(board, from, occ, &_w[_wnum-1]); }
    				return;
  				}
  			}
			}
		}

	}

	template <bool black>
	void See::generateKnightAttacker(const Board& board, const Position& from) {
		auto& num = black ? _bnum : _wnum;
		auto list = black ? _b : _w;

		auto piece = board.getBoardPiece(from);
		if ((black && piece == Piece::BKnight) || (!black && piece == Piece::WKnight)) {
			list[num++] = { material::pieceExchange(piece), nullptr, false };
			return;
		}
	}

	template <bool shallow>
	void See::generateAttackers(const Board& board, const Position& to, const Bitboard& occ, const Position& exceptPos, Direction exceptDir, HSideType sideTypeH, VSideType sideTypeV) {

		_bnum = 0;
		_wnum = 0;

#define GEN(dirname, except, shortOnly) \
		if (!(except)) { \
  		if (exceptDir != Direction::dirname) { \
    		generateAttacker<shallow, Direction::dirname, true>(board, to, occ, nullptr, (shortOnly)); \
  		} else { \
    		generateAttackerR<shallow, Direction::dirname>(board, exceptPos, occ, nullptr); \
  		} \
		}

		GEN(Up, sideTypeH == HSideType::Top, sideTypeH == HSideType::Top2);
		GEN(Down, sideTypeH == HSideType::Bottom, sideTypeH == HSideType::Bottom2);
		GEN(Left, sideTypeV == VSideType::Left, sideTypeV == VSideType::Left2);
		GEN(Right, sideTypeV == VSideType::Right, sideTypeV == VSideType::Right2);
		GEN(LeftUp,
				sideTypeH == HSideType::Top || sideTypeV == VSideType::Left,
				sideTypeH == HSideType::Top2 || sideTypeV == VSideType::Left2);
		GEN(RightUp,
				sideTypeH == HSideType::Top || sideTypeV == VSideType::Right,
				sideTypeH == HSideType::Top2 || sideTypeV == VSideType::Right2);
		GEN(LeftDown,
				sideTypeH == HSideType::Bottom || sideTypeV == VSideType::Left,
				sideTypeH == HSideType::Bottom2 || sideTypeV == VSideType::Left2);
		GEN(RightDown,
				sideTypeH == HSideType::Bottom || sideTypeV == VSideType::Right,
				sideTypeH == HSideType::Bottom2 || sideTypeV == VSideType::Right2);
#undef GEN

		// 桂馬
		if (sideTypeH != HSideType::Bottom && sideTypeH != HSideType::Bottom2) {
			if (exceptDir != Direction::LeftDownKnight && sideTypeV != VSideType::Left) {
				auto from = to.left().down(2);
				generateKnightAttacker<true>(board, from);
			}
			if (exceptDir != Direction::RightDownKnight && sideTypeV != VSideType::Right) {
				auto from = to.right().down(2);
				generateKnightAttacker<true>(board, from);
			}
		}
		if (sideTypeH != HSideType::Top && sideTypeH != HSideType::Top2) {
			if (exceptDir != Direction::LeftUpKnight && sideTypeV != VSideType::Left) {
  			auto from = to.left().up(2);
  			generateKnightAttacker<false>(board, from);
  		}
  		if (exceptDir != Direction::RightUpKnight && sideTypeV != VSideType::Right) {
  			auto from = to.right().up(2);
  			generateKnightAttacker<false>(board, from);
  		}
		}

		assert(_bnum < (int)(sizeof(_b) / sizeof(_b[0])));
		assert(_wnum < (int)(sizeof(_w) / sizeof(_w[0])));

#define SET_REF(i) do { _bref[i].attacker = &_b[i]; _wref[i].attacker = &_w[i]; } while(false)
		SET_REF(0); SET_REF(1); SET_REF(2); SET_REF(3);
		SET_REF(4); SET_REF(5); SET_REF(6); SET_REF(7);
		SET_REF(8); SET_REF(9); SET_REF(10); SET_REF(11);
		SET_REF(12); SET_REF(13); SET_REF(14); SET_REF(15);

		std::sort(_bref, _bref + _bnum);
		std::sort(_wref, _wref + _wnum);

	}

	Value See::search(bool black, Value value, Value alpha, Value beta) {

#define SEARCH(i, c) if (i < _ ## c ## num) { \
			auto att = _ ## c ## ref[i].attacker; \
			if (!att->used && (att->dependOn == nullptr || att->dependOn->used)) { \
				if (value - att->value >= beta) { return beta; } \
				att->used = true; \
				auto result = Value::max(0, value - search(!black, att->value, -beta+value, -alpha+value)); \
				att->used = false; \
				return result; \
			} \
	}
		if (black) {
			SEARCH(0, b); SEARCH(1, b); SEARCH(2, b); SEARCH(3, b);
			SEARCH(4, b); SEARCH(5, b); SEARCH(6, b); SEARCH(7, b);
			SEARCH(8, b); SEARCH(9, b); SEARCH(10, b); SEARCH(11, b);
			SEARCH(12, b); SEARCH(13, b); SEARCH(14, b); SEARCH(15, b);
		} else {
			SEARCH(0, w); SEARCH(1, w); SEARCH(2, w); SEARCH(3, w);
			SEARCH(4, w); SEARCH(5, w); SEARCH(6, w); SEARCH(7, w);
			SEARCH(8, w); SEARCH(9, w); SEARCH(10, w); SEARCH(11, w);
			SEARCH(12, w); SEARCH(13, w); SEARCH(14, w); SEARCH(15, w);
		}

		return 0;

	}

	template <bool shallow>
	Value See::search(const Board& board, const Move& move, Value alpha, Value beta) {

		assert(beta <= Value::PieceInf);
		assert(alpha >= -Value::PieceInf);

		// 取った駒の価値
		Value captured = material::pieceExchange(board.getBoardPiece(move.to()));

		// 動かした駒の価値
		Piece piece = move.piece();
		if (move.promote()) {
			piece = piece.unpromote();
		}
		Value attacker = material::pieceExchange(piece);

		if (captured <= alpha) {
			return alpha;
		}

		if (captured - attacker >= beta) {
			return beta;
		}

		// 移動可能な駒を列挙する。
		generateAttackers<shallow>(board, move);

		return captured - search(!board.isBlack(), attacker, -beta+captured, -alpha+captured);

	}
	template Value See::search<true>(const Board&, const Move&, Value, Value);
	template Value See::search<false>(const Board&, const Move&, Value, Value);

}
