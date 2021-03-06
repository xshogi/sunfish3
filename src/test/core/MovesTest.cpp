/* MovesTest.cpp
 *
 * Kubo Ryosuke
 */

#if !defined(NDEBUG)

#include "test/Test.h"
#include "core/move/Moves.h"

using namespace sunfish;

TEST(MovesTest, test) {
  {
    Moves moves;
    moves.add(Move(Piece::BPawn, S57, S56, false));
    moves.add(Move(Piece::BRook, S28, S24, false));
    moves.add(Move(Piece::BBishop, S77, S22, false));
    moves.add(Move(Piece::BLance, S19, S18, false));

    ASSERT_EQ(4, moves.size());
    ASSERT_EQ(Piece::BPawn, moves[0].piece());
    ASSERT_EQ(Piece::BRook, moves[1].piece());
    ASSERT_EQ(Piece::BBishop, moves[2].piece());
    ASSERT_EQ(Piece::BLance, moves[3].piece());
  }

  {
    Moves moves;
    moves.add(Move(Piece::BPawn, S57, S56, false));
    moves.add(Move(Piece::BRook, S28, S24, false));
    moves.add(Move(Piece::BBishop, S77, S22, false));
    moves.add(Move(Piece::BLance, S19, S18, false));

    moves.remove(1);

    ASSERT_EQ(3, moves.size());
    ASSERT_EQ(Piece::BPawn, moves[0].piece());
    ASSERT_EQ(Piece::BLance, moves[1].piece());
    ASSERT_EQ(Piece::BBishop, moves[2].piece());
  }

  {
    Moves moves;
    moves.add(Move(Piece::BPawn, S57, S56, false));
    moves.add(Move(Piece::BRook, S28, S24, false));
    moves.add(Move(Piece::BBishop, S77, S22, false));
    moves.add(Move(Piece::BLance, S19, S18, false));

    moves.remove(moves.end()-1);

    ASSERT_EQ(3, moves.size());
    ASSERT_EQ(Piece::BPawn, moves[0].piece());
    ASSERT_EQ(Piece::BRook, moves[1].piece());
    ASSERT_EQ(Piece::BBishop, moves[2].piece());
  }

  {
    Moves moves;
    moves.add(Move(Piece::BPawn, S57, S56, false));
    moves.add(Move(Piece::BRook, S28, S24, false));
    moves.add(Move(Piece::BBishop, S77, S22, false));
    moves.add(Move(Piece::BLance, S19, S18, false));

    moves.removeAfter(2);

    ASSERT_EQ(2, moves.size());
    ASSERT_EQ(Piece::BPawn, moves[0].piece());
    ASSERT_EQ(Piece::BRook, moves[1].piece());
  }

  {
    Moves moves;
    moves.add(Move(Piece::BPawn, S57, S56, false));
    moves.add(Move(Piece::BRook, S28, S24, false));
    moves.add(Move(Piece::BBishop, S77, S22, false));
    moves.add(Move(Piece::BLance, S19, S18, false));

    moves.removeAfter(moves.begin()+1);

    ASSERT_EQ(1, moves.size());
    ASSERT_EQ(Piece::BPawn, moves[0].piece());
  }

  {
    Moves moves;
    moves.add(Move(Piece::BPawn, S57, S56, false));
    moves.add(Move(Piece::BRook, S28, S24, false));
    moves.add(Move(Piece::BBishop, S77, S22, false));
    moves.add(Move(Piece::BLance, S19, S18, false));

    moves.removeStable(1);

    ASSERT_EQ(3, moves.size());
    ASSERT_EQ(Piece::BPawn, moves[0].piece());
    ASSERT_EQ(Piece::BBishop, moves[1].piece());
    ASSERT_EQ(Piece::BLance, moves[2].piece());
  }

  {
    Moves moves;
    moves.add(Move(Piece::BPawn, S57, S56, false));
    moves.add(Move(Piece::BRook, S28, S24, false));
    moves.add(Move(Piece::BBishop, S77, S22, false));
    moves.add(Move(Piece::BLance, S19, S18, false));

    moves.removeStable(moves.begin()+1);

    ASSERT_EQ(3, moves.size());
    ASSERT_EQ(Piece::BPawn, moves[0].piece());
    ASSERT_EQ(Piece::BBishop, moves[1].piece());
    ASSERT_EQ(Piece::BLance, moves[2].piece());
  }
}

#endif // !defined(NDEBUG)
