/* TT.h
 * 
 * Kubo Ryosuke
 */

#ifndef __SUNFISH_TT__
#define __SUNFISH_TT__

#include "TTE.h"
#include "../table/HashTable.h"

namespace sunfish {

	class TT : public HashTable<TTEs> {
	private:

		unsigned _age;

	public:

		TT() : HashTable<TTEs>() {}
		TT(const TT&) = delete;
		TT(TT&&) = delete;

		void evolve() {
			_age = (_age + 1) % TTE::AgeMax;
		}

		bool entry(uint64_t hash,
				Value alpha, Value beta, Value value,
				int depth, int ply,
				const NodeStat& stat, const Move& move) {
			TTE e;
			TTEs& entities = _getEntity(hash);
			entities.get(hash, e);
			if (e.update(hash, alpha, beta, value,
					depth, ply, stat, move, _age)) {
				entities.set(e);
				return true;
			}
			return false;
		}

		bool get(uint64_t hash, TTE& e) {
			return _getEntity(hash).get(hash, e) && e.is(hash);
		}

	};

}

#endif // __SUNFISH_TT__
