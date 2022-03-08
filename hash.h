#pragma once

// Create with hash_insert(NULL, ...); free with free().
struct hash* hash_insert(      struct hash*, int hash, void* val);
_Bool        hash_lookup(const struct hash*, int hash, _Bool(*)(void* val, void* ctx), void* ctx);
