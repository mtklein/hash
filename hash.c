#include "hash.h"
#include <assert.h>
#include <stdlib.h>

struct hash {
    int len, cap;
    struct {
        int   meta, hash;
        void* val;
    } table[];
};

static void insert(struct hash* h, int hash, void* val) {
    assert(h && h->len < h->cap);
    const int mask = h->cap-1;

    int ix = hash & mask;
    for (int round = 0; round <= mask; round++) {
        if (h->table[ix].meta == 0) {
            h->table[ix].meta = 1;
            h->table[ix].hash = hash;
            h->table[ix].val  = val;
            h->len++;
            return;
        }
        ix = (ix+1) & mask;
    }

    assert(0 && "unreachable");
}

struct hash* hash_insert(struct hash* h, int hash, void* val) {
    const int len = h ? h->len : 0,
              cap = h ? h->cap : 0;
    if (len >= cap*3/4) {
        const int new_cap = cap ? 2*cap : 1;

        struct hash* grown = calloc(1, sizeof *h + (size_t)new_cap * sizeof *h->table);
        grown->cap = new_cap;

        for (int ix = 0; ix < cap; ix++) {
            if (h->table[ix].meta) {
                insert(grown, h->table[ix].hash, h->table[ix].val);
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
    const int mask = h ? h->cap-1 : -1;

    int ix = hash & mask;
    for (int round = 0; round <= mask; round++) {
        if (h->table[ix].meta == 0) {
            return 0;
        }
        if (h->table[ix].hash == hash && match(h->table[ix].val, ctx)) {
            return 1;
        }
        ix = (ix+1) & mask;
    }
    return 0;
}
