/**
 * @file obstring.h
 * @brief obstring Public Interface
 * @author theck
 */

#ifndef OBSTRING_H
#define OBSTRING_H

#include "offbrand.h"
#include "obvector.h"
#include <regex.h>

/** Class type declaration */
typedef struct obstring_struct obstring;


/* PUBLIC METHODS */

/**
 * @brief Creates a new obstring with the string of characters
 *
 * @param str A NUL terminated C string
 *
 * @return An instance of obstring encapsulating the given C string
 *
 * @details The provided C string is not stored directly in an instance of
 * obstring and so can be modified and free'd without affecting the created
 * obstring.
 *
 * @warning Constructor assumes that the input string NUL terminated with
 * finite length, any C strings passed to the constructor should be known to
 * meet these constraints
 */
obstring * obstring_new(const char *str);

/**
 * @brief Copies a sequence of characters from an obstring to create a new
 * instance of obstring
 *
 * @param s A pointer to an instance of obstring
 * @param start An integer denoting the position of the start of the
 * sequence to copy within the string, character at position start included.
 * Negative integers are accepted, with -1 indicating the final non-NUL
 * character in the string
 * @param length An integer denoting the length of the string to be copied
 *
 * @return An instance of obstring containing the seqence of characters
 * selected from the provided obstring
 *
 * @warning If start and length combine to specify a range beyond the string
 * bounds then any extension beyond the bounds will return only existing
 * characters in the range, an empty string if none exist
 */
obstring * obstring_copy_substring(const obstring *s, int64_t start,
                                   uint32_t length);

/**
 * @brief Gets the length of an obstring
 *
 * @param s A pointer to an instance of obstring
 *
 * @return Integer length of the obstring instance
 */
uint32_t obstring_length(const obstring *s);

/**
 * @brief returns the character at the specified index
 *
 * @param s A pointer to an instance of obstring
 * @param i An integer denoting a characters position in the string.
 * Negative integers are accepted, with -1 indicating the final non-NUL
 * character in the string
 *
 * @return The character at the index within the string, '\0' if the character
 * is out of bounds
 */
char obstring_char_at_index(const obstring *s, int64_t i);

/**
 * @brief concatenates the two obstring instances into a new instance of
 * obstring
 *
 * @param s1 A pointer to an instance of obstring
 * @param s2 A pointer to an instance of obstring
 *
 * @return A pointer to an new instance of obstring that contains string
 * s1+s2
 */
obstring * obstring_concat(const obstring *s1, const obstring *s2);

/**
 * @brief Gets the C string representation of an obstring instance
 *
 * @param s A pointer to an instance of obstring
 *
 * @return A C string containing the same sequence of characters as the obstring
 * instance
 *
 * @warning The returned pointer is directed at an internal C string that should
 * not be altered, a new obstring should be created if an alteration is desired
 */
const char * obstring_cstring(const obstring *s);

/**
 * @brief Tokenizes an obstring over a character sequence
 *
 * @param s A pointer to an instance of obstring to be tokenized
 * @param delim A NUL terminated C string indicating the character or sequence
 * of characters on which to break a string
 *
 * @return An obvector containing the resulting obstrings derived from
 * splitting the original on the delimeter string
 *
 * @details A multicharacter obstring will not cause the the original string to
 * be split on each character, but rather the original string will be split
 * on complete instances of delim.
 *
 * @warning Function does not attempt to verify that delim is in fact NUL
 * terminated
 */
obvector * obstring_split(const obstring *s, const char *delim);

/**
 * @brief Searches for an substring in an obstring
 *
 * @param s A pointer to an instance of obstring to be searched
 * @param to_find A NUL terminated C string containing the sequence of
 * characters to find
 *
 * @retval 0 The substring was not found in the search string
 * @retval non-zero The substring was found in the search string
 *
 * @warning Function does not attempt to verify that delim is in fact NUL
 * terminated
 */
uint8_t obstring_find_substring(const obstring *s, const char *to_find);

/**
 * @brief Finds the pattern matching an expression in an obstring
 *
 * @param s A pointer to and instance of obstring to be searched
 * @param regex A pointer to an instance of obstring containing the expression
 * to attempt to match
 *
 * @return An obstring containing the first matching sequence in the obstring
 *
 * @details The POSIX Extended Regular Expression syntax is used to interpret
 * the provided expression
 *
 * @warning Function does not attempt to verify that delim is in fact NUL
 * terminated
 */
obstring * obstring_match_regex(const obstring *s, const char *regex);

#endif

