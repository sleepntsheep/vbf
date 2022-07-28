#include <stdio.h>
#include <string.h>
#include <stdnoreturn.h>
#include <stdlib.h>
#include "bf.h"
#include "str.h"
#include "backend/tui.h"
#include "xmalloc.h"

char *argv0;

struct string*
readstdin()
{
    struct string *str = str_init();
    char buf[1024] = { 0 };
    while (fgets(buf, sizeof buf, stdin))
        str_push(str, buf);
    return str;
}

struct string*
readfile(const char *file)
{
    FILE *f = fopen(file, "r");
    fseek(f, 0, SEEK_END);
    long fsize = ftell(f);
    fseek(f, 0, SEEK_SET);  /* same as rewind(f); */
    char *s = xmalloc(fsize + 1);
    fread(s, fsize, 1, f);
    fclose(f);
    return str_from_charp(s);
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

enum Mode {
    NO_VISUAL,
    TUI,
    SDL2,
};

int
main(int argc,
     char **argv)
{
    argv0 = *argv;
    struct string *program = NULL;
    enum Mode mode = NO_VISUAL;

    for (int i = 1; i < argc; i++)
        if (!strcmp(argv[i], "-h"))
            print_usage();
        else if (!strcmp(argv[i], "-v"))
            print_usage();
        else if (!strcmp(argv[i], "-t"))
            mode = TUI;
        else if (!strcmp(argv[i], "-s"))
            mode = SDL2;
        else
            program = readfile(argv[i]);

    if (program == NULL) 
        program = readstdin();

    struct bf *bf = bf_init(program);

    switch (mode)
    {
    case NO_VISUAL:
        bf_interpretall(bf);
        break;
    case TUI:
        tui_run(tui_init(bf));
        break;
    case SDL2:
        break;
    }

    return EXIT_SUCCESS;
}

