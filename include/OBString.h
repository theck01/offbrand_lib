/**
 * @file OBString.h
 * @brief OBString Public Interface
 * @author theck
 */

#ifndef OBSTRING_H
#define OBSTRING_H

#include "offbrand.h"
#include "OBVector.h"

/* Class type declaration */
typedef struct OBString_struct OBString;


/* PUBLIC METHODS */

/**
 * @brief Creates a new OBString with the string of characters
 *
 * @param str A NUL terminated C string
 * @param length The length of the str, a protection mechanism against buffer
 * overflows
 *
 * @return An instance of OBString encapsulating the given C string
 *
 * @details The provided C string is not stored directly in an instance of
 * OBString and so can be modified and free'd without affecting the created
 * OBString.
 */
OBString * createString(const char *str, size_t length);

/**
 * @brief Copies a sequence of characters from an OBString to create a new
 * instance of OBString
 * 
 * @param s A pointer to an instance of OBString
 * @param start An integer denoting the position of the start of the
 * sequence to copy within the string, character at position start included
 * @param end An integer denoting the position of the end of the
 * sequence to copy within the string, character at position end included
 *
 * @return An instance of OBString containing the seqence of characters selected
 * from the provided OBString
 */
OBString * copyStringSequence(OBString *s, uint32_t start, uint32_t end);

/**
 * @brief Gets the length of an OBString
 *
 * @param s A pointer to an instance of OBString
 *
 * @return Integer length of the OBString instance
 */
uint64_t lengthOfString(const OBString *s);

/**
 * @brief returns the character at the specified index
 *
 * @param s A pointer to an instance of OBString
 * @param i An integer index value within the string
 *
 */
char charAtStringIndex(const OBString *s, uint64_t i);

/**
 * @brief concatenates the two OBString instances into a new instance of
 * OBString
 *
 * @param s1 A pointer to an instance of OBString
 * @param s2 A pointer to an instance of OBString
 *
 * @return A pointer to an new instance of OBString that contains string 
 * s1+s2
 */
OBString * contatenateStrings(const OBString *s1, const OBString *s2);

/**
 * @brief Compares two instances of OBString
 *
 * @param a A non-NULL obj pointer to type OBString
 * @param b A non-NULL obj pointer to type OBString
 *
 * @retval OB_LESS_THAN obj a is less than b
 * @retval OB_GREATER_THAN obj a is equivalent to b
 * @retval OB_EQUAL_TO obj a is greater than b
 */
int8_t compareStrings(const obj *a, const obj *b);

/**
 * @brief Gets the C string representation of an OBString instance
 * 
 * @param s A pointer to an instance of OBString
 *
 * @return A C string containing the same sequence of characters as the OBString
 * instance
 *
 * @warning The returned array is a newly allocated block that must be free'd
 * by the caller, the function does not give access to OBString internal values
 */
char * getCString(const OBString *s);

/**
 * @brief Tokenizes an OBString over a character sequence
 *
 * @param s A pointer to an instance of OBString to be tokenized
 * @param delim An OBString delimeter on which to tokenize s 
 *
 * @return An OBVector containing the resulting OBStrings derived from
 * splitting the original on the delimeter string
 *
 * @details A multicharacter OBString will not cause the the original string to
 * be split on each character, but rather the original string will be split
 * on complete instances of delim
 */
OBVector * splitString(const OBString *s, const OBString *delim);

/**
 * @brief Searches for an substring in an OBString
 *
 * @param s A pointer to an instance of OBString to be searched
 * @param to_find A pointer to an instance of OBString containing the substring
 * to find
 *
 * @retval 0 The substring was not found in the search string
 * @retval non-zero The substring was found in the search string
 */
uint8_t findSubString(const OBString *s, const OBString *to_find);

/**
 * @brief Finds the pattern matching an expression in an OBString
 *
 * @param s A pointer to and instance of OBString to be searched
 * @param regex A pointer to an instance of OBString containing the expression
 * to attempt to match
 * 
 * @return An OBString containing the first matching sequence in the OBString
 *
 * @details The POSIX Extended Regular Expression syntax is used to interpret
 * the provided expression
 */
OBString * matchStringRegex(const OBString *s, const OBString *regex);

#endif

