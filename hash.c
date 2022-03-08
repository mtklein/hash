#include "hash.h"
#include <assert.h>
#include <stdlib.h>

struct hash {
    int len, mask;
    struct {
        int   meta, hash;
        void* val;
    } table[];
};

static void insert(struct hash* h, int hash, void* val) {
    assert(h && h->len <= h->mask);
    const int mask = h->mask;

    for (int i = hash & mask, round = 0; round <= mask; round++) {
        if (h->table[i].meta == 0) {
            h->table[i].meta = 1;
            h->table[i].hash = hash;
            h->table[i].val  = val;
            h->len++;
            return;
        }
        i = (i+1) & mask;
    }

    assert(0 && "unreachable");
}

struct hash* hash_insert(struct hash* h, int hash, void* val) {
    const int len = h ? h->len    : 0,
              cap = h ? h->mask+1 : 0;
    if (len >= cap*3/4) {
        const int new_cap = cap ? 2*cap : 1;

        struct hash* grown = calloc(1, sizeof *h + (size_t)new_cap * sizeof *h->table);
        grown->mask = new_cap-1;

        for (int i = 0; i < cap; i++) {
            if (h->table[i].meta) {
                insert(grown, h->table[i].hash, h->table[i].val);
            }
        }
        assert(grown->len == len);

        free(h);
        h = grown;
    }
    insert(h, hash, val);
    return h;
}

_Bool hash_lookup(const struct hash* h, int hash, _Bool(*match)(void* val, void* ctx), void* ctx) {
    const int mask = h ? h->mask : -1;

    for (int i = hash & mask, round = 0; round <= mask; round++) {
        if (h->table[i].meta == 0) {
            return 0;
        }
        if (h->table[i].hash == hash && match(h->table[i].val, ctx)) {
            return 1;
        }
        i = (i+1) & mask;
    }
    return 0;
}
