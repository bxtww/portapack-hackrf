/*
 * Copyright (C) 2014 Jared Boone, ShareBrained Technology, Inc.
 *
 * This file is part of PortaPack.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2, or (at your option)
 * any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; see the file COPYING.  If not, write to
 * the Free Software Foundation, Inc., 51 Franklin Street,
 * Boston, MA 02110-1301, USA.
 */

#ifndef __UTILITY_H__
#define __UTILITY_H__

#include <type_traits>
#include <cstdint>
#include <cstddef>
#include <algorithm>
#include <complex>
#include <memory>

#define LOCATE_IN_RAM __attribute__((section(".ramtext")))

constexpr size_t operator "" _KiB(unsigned long long v) {
	return v * 1024;
}

constexpr size_t operator "" _MiB(unsigned long long v) {
	return v * 1024 * 1024;
}

template<typename E>
constexpr typename std::underlying_type<E>::type toUType(E enumerator) noexcept {
	/* Thanks, Scott Meyers! */
	return static_cast<typename std::underlying_type<E>::type>(enumerator);
}

inline uint32_t flp2(uint32_t v) {
	v |= v >>  1;
	v |= v >>  2;
	v |= v >>  4;
	v |= v >>  8;
	v |= v >> 16;
	return v - (v >> 1);
}

uint32_t gcd(const uint32_t u, const uint32_t v);

template<class T>
inline constexpr T pow(const T base, unsigned const exponent) {
	return (exponent == 0) ? 1 : (base * pow(base, exponent - 1));
}

constexpr bool power_of_two(const size_t n) {
	return (n & (n - 1)) == 0;
}

constexpr size_t log_2(const size_t n, const size_t p = 0) {
	return (n <= 1) ? p : log_2(n / 2, p + 1);
}

float fast_log2(const float val);
float fast_pow2(const float val);

float mag2_to_dbv_norm(const float mag2);

inline float magnitude_squared(const std::complex<float> c) {
	const auto r = c.real();
	const auto r2 = r * r;
	const auto i = c.imag();
	const auto i2 = i * i;
	return r2 + i2;
}

template<class T>
struct range_t {
	const T minimum;
	const T maximum;

	const T& clip(const T& value) const {
		return std::max(std::min(value, maximum), minimum);
	}

	void reset_if_outside(T& value, const T& reset_value) const {
		if( (value < minimum ) ||
			(value > maximum ) ) {
			value = reset_value;
		}
	}

	bool below_range(const T& value) const {
		return value < minimum;
	}

	bool contains(const T& value) const {
		// TODO: Subtle gotcha here! Range test doesn't include maximum!
		return (value >= minimum) && (value < maximum);
	}

	bool out_of_range(const T& value) const {
		// TODO: Subtle gotcha here! Range test in contains() doesn't include maximum!
		return !contains(value);
	}
};

#endif/*__UTILITY_H__*/
