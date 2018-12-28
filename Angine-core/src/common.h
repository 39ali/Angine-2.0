#pragma once
//#define STATIC_GLEW
//#include <glew\glew.h>
#include <string>
#include <vector>


/// TODO : remove what's above

#include <stdint.h>

using int8 = int8_t;
using int16 = int16_t;
using int32 = int32_t;
using int64 = int64_t;
using uint8 = uint8_t;
using uint16 = uint16_t;
using uint32 = uint32_t;
using uint64 = uint64_t;
using uintptr = uintptr_t;



#define A_LOG(type ,message, ...) \
fprintf(stderr, "[%s] ", type); \
fprintf(stderr,message, ##__VA_ARGS__); \
fprintf(stderr,"\n");