/* Solver.cpp
 *
 * Kubo Ryosuke
 */

#include "Solver.h"
#include "logger/Logger.h"
#include "core/record/CsaReader.h"

namespace sunfish {

void Solver::setProblems(const std::vector<std::string>& problems) {
  _problems.clear();
  _problems.reserve(problems.size());
  _problems = problems;
}

void Solver::solve() {
  for (const auto& problem : _problems) {
    solve(problem);
  }

  double correctRatio = (double)_summary.correct / _summary.total;
  double incorrectRatio = (double)_summary.incorrect / _summary.total;
  double ignoreRatio = (double)_summary.ignore / _summary.total;

  Loggers::message << "Complete!!";
  Loggers::message << "";
  Loggers::message << "Summary:";
  Loggers::message << "  total     : " << _summary.total;
  Loggers::message << "  correct   : " << _summary.correct << " (" << correctRatio * 100.0 << "%)";
  Loggers::message << "  incorrect : " << _summary.incorrect << " (" << incorrectRatio * 100.0 << "%)";
  Loggers::message << "  ignore    : " << _summary.ignore << " (" << ignoreRatio * 100.0 << "%)";
}

void Solver::solve(const std::string& problem) {

  Loggers::message << "[" << problem << "]";

  // 棋譜を読み込む
  if (!CsaReader::read(problem, _record)) {
    _errors.push_back({ problem, ErrorCode::FILE_READ_ERROR });
    _summary.ignore++;
    Loggers::message << "read error: [" << problem << "]";
    return;
  }

  // 不正な手数
  if (_record.getTotalCount() == 0) {
    _errors.push_back({ problem, ErrorCode::INVALID_RECORD_LENGTH });
    _summary.ignore++;
    Loggers::message << "invalid record: [" << problem << "]";
    return;
  }

  // 序盤30手を飛ばす
  int size = (int)_record.getTotalCount();
  int startPos = std::max(0, std::min(30, size-30));

  while ((int)_record.getCount() > startPos && _record.unmakeMove())
    ;

  while (true) {
    Move correct = _record.getNextMove();
    if (correct.isEmpty()) {
      break;
    }
    solve(_record.getBoard(), correct);
    if (!_record.makeMove()) {
      break;
    }
  }

}

void Solver::solve(const Board& board, const Move& correct) {

  Loggers::message << board.toStringCsa();

  Move answer;
  bool ok = _searcher.idsearch(board, answer);

  bool result;
  if (ok && answer == correct) {
    _summary.correct++;
    result = true;
  } else {
    _summary.incorrect++;
    result = false;
  }
  _summary.total++;

  bool black = board.isBlack();
  Loggers::message << "correct: " << correct.toStringCsa(black);
  Loggers::message << "answer : " << (ok ? answer.toStringCsa(black) : "Resign");
  Loggers::message << "result : " << (result ? "Good" : "Bad");
  Loggers::message << "";

}

} // namespace sunfish
