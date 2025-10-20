#ifndef CUTILS_H
#define CUTILS_H

#include <stdbool.h>
#include <stdio.h>
#include <string.h>

#define CU_DA_INITIAL_CAPACITY 1

#ifndef CU_ASSERT
#include <assert.h>
#define CU_ASSERT assert
#endif // CU_ASSERT

#if !defined(CU_REALLOC) || !defined(CU_FREE)
#include <stdlib.h>
#endif // !defined(CU_REALLOC) || !defined(CU_FREE)

#ifndef CU_REALLOC
#define CU_REALLOC realloc
#endif // CU_REALLOC

#ifndef CU_FREE
#define CU_FREE free
#endif // CU_FREE

#define cu_da_reserve(da, cap)                                            \
    do {                                                                  \
        if ((cap) > (da)->capacity) {                                     \
            while (cap > (da)->capacity) {                                \
                if ((da)->capacity == 0)                                  \
                    (da)->capacity = CU_DA_INITIAL_CAPACITY;              \
                else (da)->capacity *= 2;                                 \
            }                                                             \
            (da)->items = CU_REALLOC((da)->items,                            \
                                  (da)->capacity * sizeof(*(da)->items)); \
            CU_ASSERT((da)->items && "CU_REALLOC failed");                      \
        }                                                                 \
    } while (0)

#define cu_da_append(da, item)                \
    do {                                      \
        cu_da_reserve((da), (da)->count + 1); \
        (da)->items[(da)->count++] = (item);  \
    } while (0)

#define cu_da_append_many(da, new_items, new_items_count)              \
    do {                                                               \
        cu_da_reserve((da), (da)->count + (new_items_count));          \
        memcpy((da)->items + (da)->count,                              \
               (new_items), (new_items_count) * sizeof(*(da)->items)); \
        (da)->count += (new_items_count);                              \
    } while (0)

#define cu_da_free(da)      \
    do {                    \
        CU_FREE((da)->items);  \
        (da)->items = NULL; \
        (da)->count = 0;    \
        (da)->capacity = 0; \
    } while (0)

typedef struct {
    size_t count;
    size_t capacity;
    char *items;
} CU_String_Builder;

#define cu_sb_append(sb, ch) cu_da_append((sb), (ch))

#define cu_sb_append_cstr(sb, s)                   \
    do {                                           \
        const char *str = (s);                     \
        cu_da_append_many((sb), str, strlen(str)); \
    } while (0)

#define cu_sb_append_null(sb) cu_sb_append((sb), '\0')

#define cu_sb_free(sb) cu_da_free((sb))

bool cu_read_entire_file(char *filepath, CU_String_Builder *sb);

#ifdef CUTILS_IMPLEMENTATION

bool cu_read_entire_file(char *filepath, CU_String_Builder *sb)
{
    FILE *f = fopen(filepath, "rb");
    if (!f) goto error;
    if (fseek(f, 0, SEEK_END) < 0) goto error;
    long n = ftell(f);
    if (n < 0) goto error;
    if (fseek(f, 0, SEEK_SET) < 0) goto error;
    size_t new_count = sb->count + n;
    cu_da_reserve(sb, new_count);
    size_t read = fread(sb->items + sb->count, sizeof(char), n, f);
    if (read != n || ferror(f)) goto error;
    sb->count = new_count;
    fclose(f);
    return true;
error:
    if (f) fclose(f);
    return false;
}

#endif // CUTILS_IMPLEMENTATION

#endif // CUTILS_H
