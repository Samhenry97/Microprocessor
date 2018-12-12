#ifndef CACHE_HPP
#define CACHE_HPP

#include "types.hpp"

#include <cmath>
using std::log2;

#define S 64
#define E 1
#define B 128

class Cache {
private:
	struct Line {
		int tag;
		word block[B >> 2];
		bool dirty;
		word memStart;
		word lru;

		Line();
		word get(word offset);
		void set(word offset, word val);
		void evict(word address, word tag);
	};

	word s, b, e, bMask, sMask, tMask;
	Line *data;

public:
	Cache();
	~Cache() { delete [] data; }

	word memLoad(word address);

	void memStore(word address, word value);
};

#endif // CACHE_HPP