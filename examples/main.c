#include <errno.h>
#include <stdio.h>
#include <string.h>

#define CUTILS_IMPLEMENTATION
#include "../cutils.h"

typedef struct {
    size_t count;
    size_t capacity;
    int *items;
} Numbers;

int main(int argc, char **argv)
{
    char *program = argv[0];
    if (argc != 2) {
        fprintf(stderr, "ERROR: invalid usage!\n");
        fprintf(stderr, "Usage: %s <filepath>\n", program);
        exit(1);
    }
    char *filepath = argv[1];

    CU_String_Builder contents = {0};
    if (!cu_read_entire_file(filepath, &contents)) {
        fprintf(stderr, "ERROR: could not read file: %s\n", filepath);
        exit(1);
    }
    cu_sb_append_null(&contents);
    printf("%s", contents.items);
    cu_sb_free(&contents);

    Numbers arr = {0};
    for (int i = 0; i < 10; ++i) {
        cu_da_append(&arr, i+1);
        printf("%d ", arr.items[i]);
    }
    puts("");
    cu_da_free(&arr);

    CU_String_Builder sb = {0};
    cu_sb_append_cstr(&sb, "Hello, World!");
    cu_sb_append_null(&sb);
    printf("%s\n", sb.items);
    cu_sb_free(&sb);

    return 0;
}
