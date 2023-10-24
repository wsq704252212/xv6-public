#include "types.h"
#include "prng.h"

// Initialize the Xorshift32 generator with a seed
void xorshift32_init(struct xorshift32 *prng, uint seed) {
    prng->state = seed;
}

// Generate the next random number
uint xorshift32_next(struct xorshift32 *prng) {
    prng->state ^= (prng->state << 13);
    prng->state ^= (prng->state >> 17);
    prng->state ^= (prng->state << 5);
    return prng->state;
}