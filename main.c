#include <stdio.h>
#include <string.h>
#include <stdnoreturn.h>
#include <stdlib.h>
#include "bf.h"
#include "tui.h"

char *argv0;

char
*readstdin()
{
    size_t cap = 1024;
    char buf[1024] = { 0 };
    char *s = malloc(cap);
    size_t len = 0;
    while (fgets(buf, sizeof buf, stdin)) {
        size_t blen = strlen(buf);
        strcpy(s+len, buf);
        s = realloc(s, cap += blen);
        len += blen;
    }
    s[len] = 0;
    return s;
}

char
*readfile(const char *file)
{
    FILE *f = fopen(file, "r");
    fseek(f, 0, SEEK_END);
    long fsize = ftell(f);
    fseek(f, 0, SEEK_SET);  /* same as rewind(f); */
    char *s = malloc(fsize + 1);
    fread(s, fsize, 1, f);
    fclose(f);
    return s;
}

#define USAGE "Usage: %s FILE [options] \
                \
                Options:\
                \t-h          display this help and exit\
                \t-v          output version and exit\
                "

#define VERSION "0.1.0"

noreturn void
print_usage()
{
    printf(USAGE, argv0);
    exit(EXIT_SUCCESS);
}

noreturn void
print_version()
{
    printf(VERSION);
    exit(EXIT_SUCCESS);
}

int
main(int argc,
     char **argv)
{
    argv0 = *argv;
    char *program = NULL;

    for (int i = 1; i < argc; i++) {
        if (!strcmp(argv[i], "-h"))
            print_usage();
        else if (!strcmp(argv[i], "-v"))
            print_usage();
        else
            program = readfile(argv[i]);
    }

    if (program == NULL) 
        program = readstdin();

    bf *bf = bf_init(program);

    tui_run(tui_init(bf));

    free(program);

    return EXIT_SUCCESS;
}
