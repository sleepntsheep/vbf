cmake_minimum_required(VERSION 3.21)

project(vbf VERSION 0.1.0 LANGUAGES C)

option(HAVE_NCURSES "ncurses.h" ON)
option(HAVE_SDL2 "SDL2/SDL.h, SDL2/SDL_ttf.h" ON)


set (source
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

add_executable(vbf 
	${source}
)

set_property(TARGET vbf PROPERTY C_STANDARD 11)

if (MINGW32)
    target_link_libraries(vbf PUBLIC mingw32)
endif()

if (HAVE_SDL2)
    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -DHAVE_SDL2")
    target_link_libraries(vbf PUBLIC SDL2main SDL2 SDL2_ttf)
endif()

if(HAVE_NCURSES)
    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -DHAVE_NCURSES")
    target_link_libraries(vbf PUBLIC ncurses)
endif()



