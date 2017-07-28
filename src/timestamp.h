#pragma once

#include <cstdint>

inline uint16_t msdos_time(uint16_t hour, uint16_t min, uint16_t sec) {
	return hour << 11 | min << 5 | (sec / 2);
}

inline uint16_t msdos_date(uint16_t year, uint16_t month, uint16_t day) {
	return year << 11 | month << 5 | day;
}
