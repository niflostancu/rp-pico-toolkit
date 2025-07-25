#ifndef _LIB_RP_UTIL_PREPROCESSOR_H
#define _LIB_RP_UTIL_PREPROCESSOR_H
/**
 * C preprocessor helpers.
 */

/* token stringification */
#define STRINGIFY(s) _STRINGIFY_I(s)
#define _STRINGIFY_I(s) #s

/* concatenation macros */
#define PRIMITIVE_CAT(a, ...) a ## __VA_ARGS__
#define CAT(a, ...) PRIMITIVE_CAT(a, __VA_ARGS__)
#define CAT3(a, b, c) CAT(a, CAT(b, c))
#define CAT5(a, b, c, d, e) CAT(a, CAT(b, CAT(c, CAT(d, e))))

#endif

