/**
 * The entry point for the simulation
 */

/* Our entry point depends on the build flags */
extern int text_main(void);
extern int graphic_main(void);

#ifdef USE_TEXT
#define ENTRY_POINT text_main
#else
#define ENTRY_POINT graphic_main
#endif

#include <malloc.h>
#include <stdio.h>

int main(void) {
    return ENTRY_POINT();
}
