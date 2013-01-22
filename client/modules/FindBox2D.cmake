find_path(BOX2D_INCLUDE_DIR Box2D
    HINTS
        ENV BOX2DDIR
    PATH_SUFFIXES include/Box2D include
)
 
find_library(BOX2D_LIBRARY
    NAMES Box2D
    HINTS
        ENV BOX2DDIR
    PATH_SUFFIXES lib Box2D
)
 
FIND_PACKAGE_HANDLE_STANDARD_ARGS(BOX2D DEFAULT_MSG BOX2D_LIBRARY BOX2D_INCLUDE_DIR)
