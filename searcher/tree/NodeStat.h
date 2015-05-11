/* NodeStat.h
 *
 * Kubo Ryosuke
 */

#ifndef __SUNFISH_NODESTAT__
#define __SUNFISH_NODESTAT__

#include <cstdint>

namespace sunfish {

class NodeStat {
public:

  enum {
    NullMove   = 0x01,
    Recapture  = 0x02,
    Mate       = 0x04,
    HashCut    = 0x08,
    Recursion  = 0x10,
    MateThreat = 0x20,

    Default = NullMove | Recapture | Mate | HashCut | Recursion,
  };

private:

  uint32_t _stat;

  void set(uint32_t flag) {
    _stat |= flag;
  }

  void unset(uint32_t flag) {
    _stat &= ~flag;
  }

  bool is(uint32_t flag) const {
    return _stat & flag;
  }

public:
  NodeStat() : _stat(Default) {
  }

  NodeStat(uint32_t stat) : _stat(stat) {
  }

  NodeStat(const NodeStat& src) : _stat(src._stat) {
  }

#define __METHOD__(s) \
  NodeStat& set ## s() { \
    set(s); \
    return *this; \
  } \
  NodeStat& unset ## s() { \
    unset(s); \
    return *this; \
  } \
  bool is ## s() const { \
    return is(s); \
  }

  __METHOD__(NullMove);
  __METHOD__(Recapture);
  __METHOD__(Mate);
  __METHOD__(HashCut);
  __METHOD__(Recursion);
  __METHOD__(MateThreat);

  operator uint32_t() const {
    return _stat;
  }
};

} // namespace sunfish

#endif // __SUNFISH_NODESTAT__
