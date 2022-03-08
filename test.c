#include "hash.h"
#undef NDEBUG
#include <assert.h>
#include <stdint.h>
#include <stdlib.h>

static void test(struct hash* (*fn)(struct hash*)) {
    free(fn(NULL));
}

static _Bool is_ctx(void* val, void* ctx) {
    return val == ctx;
}

static struct hash* basics(struct hash* h) {
    assert(!hash_lookup(h, 5, is_ctx, (void*)5));
    assert(!hash_lookup(h, 7, is_ctx, (void*)7));

    h = hash_insert(h, 5, (void*)5);

    assert( hash_lookup(h, 5, is_ctx, (void*)5));
    assert(!hash_lookup(h, 5, is_ctx, (void*)7));
    assert(!hash_lookup(h, 7, is_ctx, (void*)7));

    h = hash_insert(h, 7, (void*)7);

    assert( hash_lookup(h, 5, is_ctx, (void*)5));
    assert( hash_lookup(h, 7, is_ctx, (void*)7));
    assert(!hash_lookup(h, 5, is_ctx, (void*)7));
    assert(!hash_lookup(h, 7, is_ctx, (void*)5));

    return h;
}

static struct hash* thorough(struct hash* h) {
    const int K = 47;
    for (int i = 0; i < K; i++) {
        for (int j = 0; j < K; j++) {
            assert(hash_lookup(h, j, is_ctx, (void*)(uintptr_t)j) == (j<i));
        }
        h = hash_insert(h, i, (void*)(uintptr_t)i);
    }
    return h;
}

int main(void) {
    test(basics);
    test(thorough);
    return 0;
}
