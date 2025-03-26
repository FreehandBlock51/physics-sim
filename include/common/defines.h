#pragma once

/**
 * Common macros and typedefs used across the project
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdnoreturn.h>
#include <assert.h>

/**
 * [internal] used to allow the preprocessor to evaluate macros before
 * stringifying them
 */
#define _strify(s) #s
/**
 * Given a macro, evaluates it, then returns the string representation
 * of the result
 */
#define strify(s) _strify(s)

/**
 * Used in functions or code paths that aren't finished; says as such
 * and aborts.
 */
#define TODO() ___abort_with_msg(__FILE__ "|" strify(__LINE__) ": Not yet implemented!\n")

/**
 * @brief Prints the given message and aborts
 * @param msg The message to print
 * @return nothing -- this pretends to return int so that TODO can use it as a placeholder value
 */
static inline noreturn int ___abort_with_msg(const char * restrict msg) {
    fprintf(stderr, "%s", msg);
    abort();
}

/**
 * Anything smaller than this is considered zero; any two numbers less
 * than this amount apart are considered equal
 */
#define PHYSICS_EPSILON 1.0e-6

/**
 * Indicates that a function should not be used outside of the file
 * it was defined in.  In C, marking a function static tells the
 * compiler to do just that
 */
#define PRIVATE_FUNC static

/**
 * A contitional printf -- only call if if logging is enabled (the default)
 */
#ifndef NO_LOGGING
#define l_printf(...) printf(__VA_ARGS__)
#else
#define l_printf(...)
#endif

/**
 * Checks the status of some action.  If it is a fail state, perform
 * cleanup.
 *
 * ex: FAIL_UNLESS(int, try_action(foo), \
 *      printf("Failure! :(\n"); \
 *      exit \
 * )
 * will run try_action, and, if it returns a nonzero value, print a
 * failure message to stdout and exit the program.
 *
 * NOTE: cleanup can also end with `return` if the result should be
 * returned on failure
 */
#define FAIL_UNLESS(result_type, status_code_provider, cleanup)  \
{                                                       \
    result_type __result;                               \
    if (!(__result = (status_code_provider))) {            \
         cleanup                                        \
            (__result);                                 \
    }                                                   \
}

/**
 * The real number type used in the physics engine
 */
typedef float phy_real_t;

/**
 * [internal] used to allow the preprocessor to evaluate macros before
 * joining them together
 */
#define __JOIN2(a, b) a ## b
/**
 * Joins two pieces of text together, after evaluating any macros
 */
#define JOIN2(a, b) __JOIN2(a, b)

// use a seperate internal for join3 to reduce amount of macro calls
/**
 * [internal] used to allow the preprocessor to evaluate macros before
 * joining them together
 */
#define __JOIN3(a, b, c) a ## b ## c
/**
 * Joins three pieces of text together, after evaluating any macros
 */
#define JOIN3(a, b, c) __JOIN3(a, b, c)

/**
 * Asserts a value (so it crashes when debugging),
 * then returns early if that value isn't met
 */
#define safe_assert(expr, result_on_failure) \
    assert(expr); if (!(expr)) { return result_on_failure; } (void)0
