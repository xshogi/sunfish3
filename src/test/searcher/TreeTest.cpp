/* TreeTest.cpp
 *
 * Kubo Ryosuke
 */

#if !defined(NDEBUG)

#include "test/Test.h"
#include "core/record/CsaReader.h"
#include "searcher/tree/Tree.h"
#include "searcher/eval/Evaluator.h"

using namespace sunfish;

TEST(TreeTest, testSortByValue) {

  Tree tree;
  int32_t values[5];

  tree.initGenPhase();

  auto& moves = tree.getMoves();
  moves.add(Move(Piece::Rook, S28, S58)); values[0] = 7;
  moves.add(Move(Piece::Silver, S39, S48)); values[1] = -48;
  moves.add(Move(Piece::Pawn, S77, S76)); values[2] = 50;
  moves.add(Move(Piece::Rook, S28, S68)); values[3] = -21;
  moves.add(Move(Piece::Pawn, S27, S26)); values[4] = 39;
  tree.setSortValues(values);

  tree.sortAfterCurrent();

  ASSERT_EQ(Move(Piece::Pawn, S77, S76), tree.getMoves()[0]);
  ASSERT_EQ(Move(Piece::Pawn, S27, S26), tree.getMoves()[1]);
  ASSERT_EQ(Move(Piece::Rook, S28, S58), tree.getMoves()[2]);
  ASSERT_EQ(Move(Piece::Rook, S28, S68), tree.getMoves()[3]);
  ASSERT_EQ(Move(Piece::Silver, S39, S48), tree.getMoves()[4]);

  ASSERT_EQ(50, tree.getSortValue(tree.getBegin()+0));
  ASSERT_EQ(39, tree.getSortValue(tree.getBegin()+1));
  ASSERT_EQ(7, tree.getSortValue(tree.getBegin()+2));
  ASSERT_EQ(-21, tree.getSortValue(tree.getBegin()+3));
  ASSERT_EQ(-48, tree.getSortValue(tree.getBegin()+4));

}

TEST(TreeTest, testAddMove) {

  Tree tree;
  Move move1(Piece::Pawn, S77, S76, false);
  Move move2(Piece::Rook, S28, S58, false);

  auto ite = tree.addMove(move1);
  ASSERT_EQ(*ite, move1);

  ite = tree.addMove(move2);
  ASSERT_EQ(*ite, move2);

}

TEST(TreeTest, testRepetition) {

  Tree tree;
  Evaluator eval(Evaluator::InitType::Zero);

  {
    std::string src = "\
P1-KY+HI *  *  *  *  * -KE-OU\n\
P2 * +TO *  *  *  * +UM-GI-KY\n\
P3-FU *  * -FU *  *  * -FU * \n\
P4 *  *  *  *  *  * -FU * -FU\n\
P5 *  *  * -KE-FU-FU *  *  * \n\
P6 * +FU-FU+KA *  * -KE+OU+FU\n\
P7+FU *  *  * +FU-KI * +FU * \n\
P8 *  *  * +HI *  * -KI-GI * \n\
P9+KY+KE *  *  * +KI *  * +KY\n\
P+00FU00GI00GI00KI\n\
P-00FU00FU00FU\n\
-\n\
";
    std::istringstream iss(src);
    Board board;
    CsaReader::readBoard(iss, board);

    std::vector<Move> moves;
    moves.push_back(Move(Piece::King, S26, S17, false));
    moves.push_back(Move(Piece::Silver, S37, S28, false));
    moves.push_back(Move(Piece::King, S17, S26, false));

    tree.init(0, board, eval, moves);
    ASSERT_EQ((int)RepStatus::None, (int)tree.getCheckRepStatus());
    tree.initGenPhase();
    tree.addMove(Move(Piece::Silver, S28, S37, false));
    tree.selectNextMove();
    tree.makeMove(eval);
    ASSERT_EQ((int)RepStatus::Win, (int)tree.getCheckRepStatus());
    tree.unmakeMove();
    ASSERT_EQ((int)RepStatus::None, (int)tree.getCheckRepStatus());
    tree.release(moves);
  }

  {
    std::string src = "\
P1-KY+HI *  *  *  *  * -KE-OU\n\
P2 * +TO *  *  *  * +UM-GI-KY\n\
P3-FU *  * -FU *  *  * -FU * \n\
P4 *  *  *  *  *  * -FU * -FU\n\
P5 *  *  * -KE-FU-FU *  *  * \n\
P6 * +FU-FU+KA *  * -KE+OU+FU\n\
P7+FU *  *  * +FU-KI * +FU * \n\
P8 *  *  * +HI *  * -KI-GI * \n\
P9+KY+KE *  *  * +KI *  * +KY\n\
P+00FU00GI00GI00KI\n\
P-00FU00FU00FU\n\
-\n\
";
    std::istringstream iss(src);
    Board board;
    CsaReader::readBoard(iss, board);

    std::vector<Move> moves;
    moves.push_back(Move(Piece::Silver, S37, S28, false));
    moves.push_back(Move(Piece::King, S17, S26, false));

    tree.init(0, board, eval, moves);
    ASSERT_EQ((int)RepStatus::None, (int)tree.getCheckRepStatus());
    tree.initGenPhase();
    tree.addMove(Move(Piece::Silver, S28, S37, false));
    tree.selectNextMove();
    tree.makeMove(eval);
    ASSERT_EQ((int)RepStatus::None, (int)tree.getCheckRepStatus());
    {
      tree.initGenPhase();
      tree.addMove(Move(Piece::King, S26, S17, false));
      tree.selectNextMove();
      tree.makeMove(eval);
      ASSERT_EQ((int)RepStatus::Lose, (int)tree.getCheckRepStatus());
      tree.unmakeMove();
    }
    tree.unmakeMove();
    ASSERT_EQ((int)RepStatus::None, (int)tree.getCheckRepStatus());
    tree.release(moves);
  }

}

#endif // !defined(NDEBUG)
