#include "../lib/cache.hpp"
#include "../lib/cpu.hpp"

Cache::Line::Line() {
	tag = -1;
	dirty = false;
	lru = 0;
	memStart = 0;
}

word Cache::Line::get(word offset) {
	return block[offset >> 2];
}

void Cache::Line::set(word offset, word val) {
	block[offset >> 2] = val;
	dirty = true;
}

void Cache::Line::evict(word address, word newTag) {
	CPU &cpu = CPU::instance();

	if(dirty) {
		for(word i = 0; i < (B >> 2); i++) {
			cpu.shell->mem_store(memStart + (i << 2), block[i]);
		}
	}

	memStart = address - (address % B);
	dirty = false;
	lru = 0;
	tag = newTag;
	for(word i = 0; i < (B >> 2); i++) {
		block[i] = cpu.shell->mem_load(memStart + (i << 2));
	}
}

Cache::Cache() : data(new Line[S * E]) {
	s = log2(S);
	b = log2(B);
	e = log2(E);

	for(word i = 0; i < b; i++) { bMask |= (1 << i); }

	for(word i = 0; i < s; i++) { sMask |= (1 << i); }
	sMask <<= b;

	for(word i = 0; i < 32 - s - b; i++) { tMask |= (1 << i); }
	tMask <<= b + s;

	for(word i = 0; i < S * E; i++) {
		data[i] = Line();
	}
}

word Cache::memLoad(word address) {
	word blockOffset = address & bMask;
	word set = ((address & sMask) >> b) << e;
	word setEnd = set + E;
	int tag = (address & tMask) >> (b + s);

	for(word i = set; i < setEnd; i++) {		// Check for any cache hits
		if(data[i].tag == tag) {
			for(word j = set; j < setEnd; j++) {	// Increment LRU for all lines
				data[j].lru++;
			}
			data[i].lru = 0;					// Set the hit LRU to 0
			return data[i].get(blockOffset);	// Return that value!
		}
	}

	CPU::instance().stats.load_misses++;

	for(word i = set; i < setEnd; i++) { data[i].lru++; }

	word index = set;
	for(word i = set; i < setEnd; i++) {
		if(data[i].lru > data[index].lru) {
			index = i;
		}
	}

	data[index].evict(address, tag);
	return data[index].get(blockOffset);
}

void Cache::memStore(word address, word value) {
	word blockOffset = address & bMask;
	word set = ((address & sMask) >> b) << e;
	word setEnd = set + E;
	int tag = (address & tMask) >> (b + s);

	for(word i = set; i < setEnd; i++) {		// Check for any cache hits
		if(data[i].tag == tag) {
			for(word j = set; j < setEnd; j++) {	// Increment LRU for all lines
				data[j].lru++;
			}
			data[i].lru = 0;					// Set the hit LRU to 0
			data[i].set(blockOffset, value);	// Return that value!
			return;
		}
	}

	CPU::instance().stats.store_misses++;

	for(word i = set; i < setEnd; i++) { data[i].lru++; }

	word index = set;
	for(word i = set; i < setEnd; i++) {
		if(data[i].lru > data[index].lru) {
			index = i;
		}
	}

	data[index].evict(address, tag);
	data[index].set(blockOffset, value);
}