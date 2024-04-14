#include <stdint.h>
void invert(uint32_t* ptr, uint32_t mask) {
	*ptr ^= mask;
}
