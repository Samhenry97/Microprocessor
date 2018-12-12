#ifndef UTIL_HPP
#define UTIL_HPP

#include "types.hpp"

struct Util {
	// Rotates <immediate> by <rotateAmt>
	// <immediate> The value to rotate
	// <rotateAmt> The amount to rotate
	// <return> The rotated value
	static word rotateRight(word immediate, word rotateAmt) {
		return (immediate >> rotateAmt) | (immediate << (32 - rotateAmt));
	}
};

#endif // UTIL_HPP