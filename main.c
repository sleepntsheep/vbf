#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "bf.h"
#include "str.h"
#include "log.h"
#include "xmalloc.h"
#ifdef HAVE_NCURSES
#include "backend/tui.h"
#endif
#ifdef HAVE_SDL2
#include "backend/gui.h"
#endif

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
    if (f == NULL)
    {
        warn("can't open file (%s)", file);
        return NULL;
    }
    fseek(f, 0, SEEK_END);
    long fsize = ftell(f);
    fseek(f, 0, SEEK_SET);  /* same as rewind(f); */
    char *s = xmalloc(fsize + 1);
    fread(s, fsize, 1, f);
    fclose(f);
    return str_from_charp(s);
}

#define USAGE "Usage: %s [FILE] [options]" \
"\n" \
"Options:\n" \
"\t-h          display this help and exit\n" \
"\t-v          output version and exit\n" \
"\t-t          visualize with ncurses tui backend\n" \
"\t-n 	       interpret without visualizing\n"

#define VERSION "0.1.1"

_Noreturn void
print_usage()
{
    printf(USAGE, argv0);
    exit(EXIT_SUCCESS);
}

_Noreturn void
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
main
(int argc,
     char **argv)
{
    argv0 = *argv;
    struct string *program = NULL;
#if HAVE_SDL2
    enum Mode mode = SDL2;
#else
    enum Mode mode = NO_VISUAL;
#endif

    for (int i = 1; i < argc; i++)
        if (!strcmp(argv[i], "-h"))
            print_usage();
        else if (!strcmp(argv[i], "-v"))
            print_usage();
        else if (!strcmp(argv[i], "-t"))
            mode = TUI;
        else if (!strcmp(argv[i], "-n"))
            mode = NO_VISUAL;
        else
            program = readfile(argv[i]);

    if (program == NULL)
    {
        info("no file specified, reading from stdin");
        program = readstdin();
    }

    struct bf *bf = bf_init(program);

    switch (mode)
    {
    case NO_VISUAL:
        bf_interpretall(bf);
        break;
#ifdef HAVE_NCURSES
    case TUI:
        tui_run(tui_init(bf));
        break;
#endif
#ifdef HAVE_SDL2
    case SDL2:
        {
            struct gui *gui = gui_init(bf);
            gui_run(gui);
            free(gui);
            break;
        }
#endif
    default:
        info("mode (%d) not handled, maybe vbf wasn't compiled with the right flags", mode);
        break;
    }

    return EXIT_SUCCESS;
}

