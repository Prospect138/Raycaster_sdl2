#include <stdint.h>
#include <random>

void invert(uint8_t* ptr, int size) {
	std::mt19937 range(std::random_device{}());
	std::uniform_int_distribution<> dist(0, 255);

	char mask[size];
	for (int i = 0; i < size; i++){
		mask[i] = dist(range);
		ptr[i] ^= mask[i];
	}
}

int main() {
	int size = 4;
	uint8_t ptr1[size] = {32, 12, 167, 99};
	invert(&ptr1[0], size);
	return 0;
}
