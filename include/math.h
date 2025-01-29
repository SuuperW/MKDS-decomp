#pragma once

#ifndef SD_MATH_H
#define SD_MATH_H

#ifdef __cplusplus
template<typename N>
static inline N max(N a, N b) {
	if (a > b) return a; return b;
}
template<typename N>
static inline N min(N a, N b) {
	if (a < b) return a; return b;
}
#endif

#endif
