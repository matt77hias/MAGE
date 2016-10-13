#pragma once

#include <malloc.h>

void *AllocAligned(size_t size) {
	return _aligned_malloc(size, L1_CACHE_LINE_SIZE);
}


template <typename T> 
T *AllocAligned(uint32_t count) {
	return (T *)AllocAligned(count * sizeof(T));
}


void FreeAligned(void *ptr) {
	if (!ptr) {
		return;
	}

	_aligned_free(ptr);
}