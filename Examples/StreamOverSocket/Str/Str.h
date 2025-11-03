/**
 * @file Str.h
 * @author Ali Mirghasemi (ali.mirghasemi1376@gmail.com)
 * @brief this library can use to handle with ASCII strings in c
 * @version 0.2
 * @date 2021-08-08
 *
 * @copyright Copyright (c) 2021
 *
 */
#ifndef _STR_UTILS_H_
#define _STR_UTILS_H_

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

#define STR_VER_MAJOR       0
#define STR_VER_MINOR       5
#define STR_VER_FIX         0

#include <stdint.h>

/********************************************************************************************/
/*                                     Configuration                                        */
/********************************************************************************************/
#define STR_ENABLE_PARSE                                    1

#define STR_ENABLE_CONVERT_STR                              1

#define MEM_MAX_LENGTH                                      1024

#define STR_MAX_LENGTH                                      1024

#define STR_ENABLE_LONG_NUMBER                              1

#define STR_ENABLE_DOUBLE                                   1

#define STR_USE_CONST_VARIABLES                             0

#define STR_USE_STRING_LIBRARY                              1

typedef int Mem_CmpResult;
typedef short Str_CmpResult;

typedef int32_t  Str_Num;
typedef uint32_t Str_UNum;

#if STR_ENABLE_LONG_NUMBER
    typedef int64_t  Str_Long;
    typedef uint64_t Str_ULong;
#endif

typedef int16_t Str_LenType;
typedef int16_t Mem_LenType;
/********************************************************************************************/

#if STR_USE_STRING_LIBRARY
#include <string.h>

#define Mem_copy                            memcpy
#define Mem_compare                         memcmp
#define Mem_move                            memmove
#define Mem_indexOf                         memchr
#define Mem_set                             memset

#define Str_copy                            strcpy
#define Str_copyFix                         strncpy
#define Str_compare                         strcmp
#define Str_compareFix                      strncmp
#define Str_len                             strlen
#define Str_indexOf                         strchr
#define Str_lastIndexOf                     strrchr
#define Str_indexOfStr                      strstr
#define Str_append                          strcat

#else

void*       Mem_copy(void* dest, const void* src, Mem_LenType len);
char        Mem_compare(const void* arr1, const void* arr2, Mem_LenType len);
void*       Mem_move(void* dest, const void* src, Mem_LenType len);
void*       Mem_indexOf(const void* arr, unsigned char value, Mem_LenType len);
void*       Mem_set(void* arr, unsigned char value, Mem_LenType len);



char*       Str_copy(char* dest, const char* str);
char*       Str_copyFix(char* dest, const char* str, Str_LenType len);
char        Str_compare(const char* str1, const char* str2);
char        Str_compareFix(const char* str1, const char* str2, Str_LenType len);
Str_LenType Str_len(const char* str);
char*       Str_indexOf(char* str, char c);
char*       Str_lastIndexOf(char* str, char c);
char*       Str_indexOfStr(char* str, char* sub);
char*       Str_append(char* str, char* sub);

#endif // STR_USE_STRING_LIBRARY

#define STR_NORMAL_LEN      0
/**
 * @brief result type for str functions
 */
typedef enum {
    Str_Ok      = 0,
    Str_Error   = 1,
} Str_Result;
/**
 * @brief hold result of multi str find
 */
typedef struct {
    const char*     IndexOf;
    Str_LenType     Position;
} Str_MultiResult;
/**
 * @brief custom radix for parse and convert functions
 * other values is valid too
 */
typedef enum {
    Str_Binary      = 2,
    Str_Nibble      = 4,
    Str_Octal       = 8,
    Str_Decimal     = 10,
    Str_Hex         = 16,
}  Str_Radix;
/**
 * @brief return type for ignore role function
 */
typedef enum {
    Str_NotIgnore   = 0,
    Str_Ignore      = 1,
} Str_Ignore_Result;
/**
 * @brief custom role for ignore characters
 */
typedef Str_Ignore_Result (*Str_IgnoreRoleFn) (char character);
/**
 * @brief string holder
 */
typedef struct {
    char*           Text;
    Str_LenType     Len;
} Str;
/**
 * @brief const string holder
 */
typedef struct {
    const char*     Text;
    Str_LenType     Len;
} StrConst;


void*       Mem_copyReverse(void* dest, const void* src, Mem_LenType len);
void*       Mem_reverse(void* arr, Mem_LenType len);

char*       Str_copyUntil(char* dest, const char* src, char endChar);
char*       Str_copyLine(char* dest, const char* src);

char*       Str_reverseIndexOf(const char* str, char c, const char* startOfStr);
char*       Str_reverseIndexOfFix(const char* str, char c, int length);

char*       Str_indexOfEnd(const char* str);
char*       Str_indexOfNull(const char* str);

char        Str_compareInverse(const char* str1, const char* str2);
char        Str_compareWord(const char* str, const char* word);

char*       Str_findDigit(const char* str);
char*       Str_findDigitUntil(const char* str, char endChar);
char*       Str_findDigitFix(const char* str, Str_LenType len);

char*       Str_findLastDigit(const char* str);
char*       Str_findLastDigitUntil(const char* str, char endChar);
char*       Str_findLastDigitFix(const char* str, Str_LenType len);

char*       Str_ignoreWhitespace(const char* str);
char*       Str_ignoreCharacters(const char* str);
char*       Str_ignoreAlphaNumeric(const char* str);
char*       Str_ignoreAlphabet(const char* str);
char*       Str_ignoreNumeric(const char* str);
char*       Str_ignoreSpecialCharacters(const char* str);
char*       Str_ignoreNameCharacters(const char* str);
char*       Str_ignoreCommandCharacters(const char* str);
char*       Str_ignore(const char* str, Str_IgnoreRoleFn cmp);

char*       Str_ignoreWhitespaceReverse(const char* str);
char*       Str_ignoreCharactersReverse(const char* str);

void        Str_upperCase(char* str);
void        Str_upperCaseFix(char* str, Str_LenType len);
void        Str_lowerCase(char* str);
void        Str_lowerCaseFix(char* str, Str_LenType len);

char*       Str_trimLeft(char* str);
char*       Str_trimRight(char* str);
char*       Str_trim(char* str);

Str_LenType Str_removeBackspace(char* str);
Str_LenType Str_removeBackspaceFix(char* str, Str_LenType len);

char*       Str_findReverseDigit(const char* str);
char*       Str_findReverseDigitFix(const char* str, Str_LenType len);

Str_LenType Str_indexesOf(const char* str, char c, char const** indexes);
Str_LenType Str_indexesOfFix(const char* str, char c, char const** indexes, Str_LenType len);
Str_LenType Str_indexesOfUntil(const char* str, char c, char const** indexes, char endChar);

char*       Str_indexOfAt(const char* str, char c, Str_LenType num);
char*       Str_indexOfAtUntil(const char* str, char c, Str_LenType num, char end);
char*       Str_indexOfAtFix(const char* str, char c, Str_LenType num, Str_LenType len);

char*       Str_reverse(char* str);

char*       Str_copyReverse(char* dest, const char* src);

char*       Str_substr(char* dest, const char* str, Str_LenType start);
char*       Str_substrFix(char* dest, const char* str, Str_LenType start, Str_LenType len);
char*       Str_substrUntil(char* dest, const char* str, Str_LenType start, char endChar);

char*       Str_replace(char* str, const char* word, const char* replacement);
int         Str_replaceAll(char* str, const char* word, const char* replacement);

const char** Str_sort(const char** strs, Str_LenType len);
const char** Str_quickSort(const char** strs, Str_LenType len);
const char** Str_sortReverse(const char** strs, Str_LenType len);
const char** Str_quickSortReverse(const char** strs, Str_LenType len);

Str_LenType Str_split(const char* src, char separator, char* strs, Str_LenType rowLen, Str_LenType len);
Str_LenType Str_splitPtr(char* src, char separator, char** strs, Str_LenType len, uint8_t setNull);
char*       Str_splitToken(char* src, char separator, char** ptr, uint8_t setNull);

Str_LenType Str_posOf(const char* str, char c);
Str_LenType Str_lastPosOf(const char* str, char c);

Str_LenType Str_posOfStr(const char* str, const char* word);

Str_LenType Str_multiCompare(const char** strs, Str_LenType len, const char* str);
Str_LenType Str_multiCompareSorted(const char** strs, Str_LenType len, const char* str);

const char* Str_findStrs(const char* src, const char** strs, Str_LenType len, Str_MultiResult* result);
const char* Str_findStrsSorted(const char* src, const char** strs, Str_LenType len, Str_MultiResult* result);

const char* Str_findStrsFix(const char* src, const char** strs, Str_LenType len, Str_MultiResult* result, Str_LenType srcLen);
const char* Str_findStrsSortedFix(const char* src, const char** strs, Str_LenType len, Str_MultiResult* result, Str_LenType srcLen);

Str_LenType Str_parseNum(Str_Num num, Str_Radix base, Str_LenType minLen, char* str);
Str_LenType Str_parseUNum(Str_UNum num, Str_Radix base, Str_LenType minLen, char* str);

Str_LenType Str_parseString(const char* string, char* str);
Str_LenType Str_fromString(char* str);

Str_Result Str_convertNum(const char* str, Str_Num* num, Str_Radix base);
Str_Result Str_convertUNum(const char* str, Str_UNum* num, Str_Radix base);
Str_Result Str_convertNumFix(const char* str, Str_Num* num, Str_Radix base, Str_LenType len);
Str_Result Str_convertUNumFix(const char* str, Str_UNum* num, Str_Radix base, Str_LenType len);

char* Str_convertString(const char* str, char* string);
char* Str_convertStringFix(const char* str, char* string, Str_LenType len);

#if STR_ENABLE_LONG_NUMBER

Str_LenType Str_parseLong(Str_Long num, Str_Radix base, Str_LenType minLen, char* str);
Str_LenType Str_parseULong(Str_ULong num, Str_Radix base, Str_LenType minLen, char* str);

Str_Result Str_convertLong(const char* str, Str_Long* num, Str_Radix base);
Str_Result Str_convertULong(const char* str, Str_ULong* num, Str_Radix base);
Str_Result Str_convertLongFix(const char* str, Str_Long* num, Str_Radix base, Str_LenType len);
Str_Result Str_convertULongFix(const char* str, Str_ULong* num, Str_Radix base, Str_LenType len);

#endif // STR_ENABLE_LONG_NUMBER

Str_LenType Str_parseFloat(float num, char* str);
Str_LenType Str_parseFloatFix(float num, char* str, Str_LenType decimalLen);

Str_Result Str_convertFloat(const char* str, float* num);
Str_Result Str_convertFloatFix(const char* str, float* num, Str_LenType len);

#if STR_ENABLE_DOUBLE

Str_LenType Str_parseDouble(double num, char* str);
Str_LenType Str_parseDoubleFix(double num, char* str, Str_LenType decimalLen);

Str_Result Str_convertDouble(const char* str, double* num);
Str_Result Str_convertDoubleFix(const char* str, double* num, Str_LenType len);

#endif // STR_ENABLE_DOUBLE

Str_Result Str_getNum(const char* str, int* num, const char** numPos);
Str_Result Str_getUNum(const char* str, unsigned int* num, const char** numPos);
Str_Result Str_getFloat(const char* str, float* num, const char** numPos);

Str_Result Str_getToken(const char* str, char separator, Str_LenType index, char* token);
Str_Result Str_getTokenFix(const char* str, char separator, Str_LenType index, char* token, Str_LenType len);

/**
 * Sorting Functions
 **/

#define Mem_castItem(TYPE, ITEM)            ((TYPE*) ITEM)

typedef Mem_CmpResult (*Mem_CompareFn) (const void* itemA, const void* itemB, Mem_LenType itemLen);
typedef void (*Mem_SwapFn) (void* itemA, void* itemB, Mem_LenType itemLen);

void*       Mem_sort(void* items, Mem_LenType len, Mem_LenType itemLen, Mem_CompareFn cmp, Mem_SwapFn swap);
void*       Mem_quickSort(void* items, Mem_LenType len, Mem_LenType itemLen, Mem_CompareFn cmp, Mem_SwapFn swap);

Mem_LenType Mem_partition(void* items, Mem_LenType low, Mem_LenType high, Mem_LenType itemLen, Mem_CompareFn cmp, Mem_SwapFn swap);
void*       Mem_quickSortBlock(void* items, Mem_LenType low, Mem_LenType high, Mem_LenType itemLen, Mem_CompareFn cmp, Mem_SwapFn swap);

Mem_LenType Mem_linearSearch(const void* items, Mem_LenType len, Mem_LenType itemLen, const void* item, Mem_CompareFn cmp);
Mem_LenType Mem_binarySearch(const void* items, Mem_LenType len, Mem_LenType itemLen, const void* item, Mem_CompareFn cmp);

typedef Str_CmpResult (*Str_CompareFn) (const char* itemA, const char* ItemB);

void        Str_swap(const char** itemA, const char** itemB);
Str_LenType Str_partition(const char** items, Str_LenType low, Str_LenType high, Str_CompareFn cmp);
const char** Str_quickSortBlock(const char** items, Mem_LenType low, Mem_LenType high, Str_CompareFn cmp);

const char** Str_selectionSortBlock(const char** items, Mem_LenType len, Str_CompareFn cmp);

Str_LenType Str_linearSearch(const char** strs, Str_LenType len, const char* str, Str_CompareFn cmp);
Str_LenType Str_binarySearch(const char** strs, Str_LenType len, const char* str, Str_CompareFn cmp);

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif // _STR_UTILS_H_


