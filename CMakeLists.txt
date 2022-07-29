cmake_minimum_required(VERSION 3.14)

project(vbf VERSION 0.1.0 LANGUAGES C)
add_executable(vbf 
    main.c
    bf.c
    log.c
    stack.c
    str.c
    utils.c
    xmalloc.c
    backend/tui.c
    backend/gui.c
    )
target_link_libraries(vbf SDL2 SDL2_ttf ncurses)

