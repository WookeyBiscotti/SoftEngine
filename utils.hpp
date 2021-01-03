#pragma once

#include <cinttypes>

inline float fastInvSqrt(float number) {
	const float x2 = number * 0.5f;
	const float threehalfs = 1.5f;

	union {
		float f;
		uint32_t i;
	} conv = {number}; // member 'f' set to value of 'number'.
	conv.i = 0x5f3759df - (conv.i >> 1);
	conv.f *= threehalfs - x2 * conv.f * conv.f;
	return conv.f;
}