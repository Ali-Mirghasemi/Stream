#include "Str.h"

#if STR_USE_STRING_LIBRARY
    #include <string.h>
#endif

/**
 * variables
 **/

#if STR_USE_CONST_VARIABLES
    const Str_Radix __Str_Decimal       = Str_Decimal;
    const char __Str_0                  = '0';
    const char __Str_9                  = '9';
    const char __Str_Zero               = 0;
    const char __Str_One                = 1;
    const char __Str_Null               = '\0';
    const Str_LenType __Str_MaxLength   = STR_MAX_LENGTH;
#else
    #define __Str_Decimal               Str_Decimal
    #define __Str_MaxLength             STR_MAX_LENGTH
    #define __Str_0                     '0'
    #define __Str_9                     '9'
    #define __Str_Zero                  0
    #define __Str_One                   1
    #define __Str_Null                  '\0'
#endif // STR_USE_CONST_VARIABLES

#ifndef NULL
    #define NULL                         ((const void*) 0)
#endif


/**
 * @brief this Function can use for copy array into another array with reverse order
 *
 * @param src address of source array that we want copy from
 * @param dest address of destination array that we want copy into
 * @param len number of bytes
 * @return void* address of destination array
 */
void*       Mem_copyReverse(void* dest, const void* src, Mem_LenType len) {
    unsigned char* pD = (unsigned char*) dest;
    unsigned char* pS = (unsigned char*) src + (len - 1);
	while (len-- > 0){
		*pD++ = *pS--;
	}
	return dest;
}
/**
 * @brief reverse order of bytes in array
 *
 * @param arr address of array that we want reverse order
 * @param len number of byte
 * @return void* address of array
 */
void*       Mem_reverse(void* arr, Mem_LenType len) {
    unsigned char* pStart = (unsigned char*) arr;
	unsigned char* pEnd = &((unsigned char*)arr)[len - 1];
	unsigned char temp;
	len >>= 1;
	while (len-- > 0){
		temp = *pEnd;
		*pEnd-- = *pStart;
		*pStart++ = temp;
	}
	return arr;
}
/**
 * @brief this function return address of last character
 *
 * @param str
 * @return
 */
char* Str_indexOfEnd(const char* str) {
    Str_LenType len = Str_len(str);
    if (len) {
        return (char*) &str[len - 1];
    }
    else {
        return NULL;
    }
}
/**
 * @brief this function return end of string (Null character)
 *
 * @param str address of string must include 'null' in end of it
 * @return char* address of null character
 */
char* Str_indexOfNull(const char* str) {
    Str_LenType len = Str_len(str);
    if (len) {
        return (char*) &str[len];
    }
    else {
        return NULL;
    }
}
/**
 * @brief compare two string and result is opposite of Str_compare
 *
 * @param str1 address of string 1
 * @param str2 address of string 2
 * @return char result of compare,
 *      str1 > str2 -> result < 0
 *      str1 == str2 -> result == 0
 *      str1 < str2 -> result > 0
 */
char        Str_compareInverse(const char* str1, const char* str2) {
    return Str_compare(str2, str1);
}
/**
 * @brief compare word in str, with priority of word and continue until word reach null
 *
 * @param str address of base string
 * @param word address of word that we want compare with str
 * @return char result of compare,
 *      str > word -> result > 0
 *      str == word -> result == 0
 *      str < word -> result < 0
 */
char        Str_compareWord(const char* str, const char* word) {
    char res;
	while (*word != __Str_Null){
		res = *str++ - *word++;
		if (res != 0){
			break;
		}
	}
	return res;
}
/**
 * @brief find first digit in string and return address of digit
 *
 * @param str address of string
 * @return char* address of digit in str, return null if nothing found
 */
char*       Str_findDigit(const char* str) {
    while ((*str < __Str_0 || *str > __Str_9) &&
            *str != __Str_Null){
		str++;
	}
	return *str == __Str_Null ? NULL : (char*) str;
}
/**
 * @brief find first digit in string and return address of digit, keep search until reach null or reach end charachter
 *
 * @param str address of string
 * @param endChar ending character
 * @return char* address of digit in str, return null if nothing found
 */
char*       Str_findDigitUntil(const char* str, char endChar) {
    while ((*str < __Str_0 || *str > __Str_9) &&
            *str != __Str_Null &&
            *str != endChar){
		str++;
	}
	return *str == __Str_Null || *str == endChar ? NULL : (char*) str;
}
/**
 * @brief find first digit in string and return address of digit, keep search until reach null or pass the length limit
 *
 * @param str address of string
 * @param len length of string
 * @return char* address of digit in str, return null if nothing found
 */
char*       Str_findDigitFix(const char* str, Str_LenType len) {
    while ((*str < __Str_0 || *str > __Str_9) &&
            *str != __Str_Null &&
            len-- > 0){
		str++;
	}
	return *str == __Str_Null || len == 0 ? NULL : (char*) str;
}
/**
 * @brief find a number and return last digit of it
 *
 * @param str address of string
 * @return char* address of digit in str, return null if nothing found
 */
char*       Str_findLastDigit(const char* str) {
    str = Str_findDigit(str);
    if (str != NULL) {
        while (*str >= __Str_0 && *str <= __Str_9){
            str++;
        }
        str--;
    }
	return (char*) str;
}
/**
 * @brief find a number and return last digit of it with ending charachter condition
 *
 * @param str address of string
 * @param endChar ending charachter
 * @return char* address of digit in str, return null if nothing found
 */
char*       Str_findLastDigitUntil(const char* str, char endChar) {
    str = Str_findDigitUntil(str, endChar);
    if (str != NULL) {
        while (*str >= __Str_0 && *str <= __Str_9){
            str++;
        }
        str--;
    }
	return (char*) str;
}
/**
 * @brief find a number and return last digit of it with length condition
 *
 * @param str address of string
 * @param len length of string
 * @return char* address of digit in str, return null if nothing found
 */
char*       Str_findLastDigitFix(const char* str, Str_LenType len) {
    str = Str_findDigitFix(str, len);
    if (str != NULL) {
        while (*str >= __Str_0 && *str <= __Str_9){
            str++;
        }
        str--;
    }
	return (char*) str;
}
/**
 * @brief find digit from end of string in backward
 *
 * @param str address of string, must include null
 * @return char* address of digit in str, return null if nothing found
 */
char*       Str_findReverseDigit(const char* str) {
    return Str_findReverseDigitFix(str, Str_len(str));
}
/**
 * @brief find digit from end of string in backward with fixed string length
 *
 * @param str address of string
 * @param len lenght of string
 * @return char* address of digit in str, return null if nothing found
 */
char*       Str_findReverseDigitFix(const char* str, Str_LenType len) {
    str = &str[len - 1];
    while ((*str < __Str_0 || *str > __Str_9) &&
            len-- > 0){
		str--;
	}
	return *str == __Str_Null || len == 0 ? NULL : (char*) str;
}
/**
 * @brief find multiple indexes of 'c' in string
 *
 * @param str address of strings
 * @param c charachter that we want search for indexes
 * @param indexes a pointer array that will hold our indexes
 * @return Str_LenType number of indexes that found
 */
Str_LenType Str_indexesOf(const char* str, char c, char const** indexes) {
    Str_LenType count = 0;
	while (*str != __Str_Null){
		if (c == *str){
			*indexes++ = str;
			count++;
		}
		str++;
	}
	return count;
}
/**
 * @brief find multiple indexes of 'c' in string until reach null or pass the limit
 *
 * @param str address of strings
 * @param c charachter that we want search for indexes
 * @param indexes a pointer array that will hold our indexes
 * @param len lenght of string
 * @return Str_LenType number of indexes that found
 */
Str_LenType Str_indexesOfFix(const char* str, char c, char const** indexes, Str_LenType len) {
    Str_LenType count = 0;
	while (*str != __Str_Null && len-- > 0){
		if (c == *str){
			*indexes++ = str;
			count++;
		}
		str++;
	}
	return count;
}
/**
 * @brief find multiple indexes of 'c' in string until reach null or ending charachter
 *
 * @param str address of strings
 * @param c charachter that we want search for indexes
 * @param indexes a pointer array that will hold our indexes
 * @param endChar ending charachter
 * @return Str_LenType number of indexes that found
 */
Str_LenType Str_indexesOfUntil(const char* str, char c, char const** indexes, char endChar) {
    Str_LenType count = 0;
	while (*str != __Str_Null && *str != endChar){
		if (c == *str){
			*indexes++ = str;
			count++;
		}
		str++;
	}
	return count;
}
/**
 * @brief This Fntion finds the address of a character in a given number and returns the last one
 *
 * @param str address of string
 * @param c charachter that we want to search
 * @param num The number of times the Fuction looks for a character, must greater than 0
 * @return char* address of charachter, return null if not found
 */
char* Str_indexOfAt(const char* str, char c, Str_LenType num) {
    if (str != NULL) {
        if (num > 0) {
            while ((str = Str_indexOf(str, c)) != NULL && --num > 0) {
                str = str + 1;
            }
        }
        return (char*) str;
    }
    else {
        return NULL;
    }
}
/**
 * @brief This Fntion finds the address of a character in a given number and returns the last one until reach null or ending charachter
 *
 * @param str address of string
 * @param c charachter that we want to search
 * @param num The number of times the Fntion looks for a character
 * @param end ending charachter
 * @return char* address of charachter, return null if not found
 */
char* Str_indexOfAtUntil(const char* str, char c, Str_LenType num, char end) {
    return Str_indexOfAtFix(str, c, num, Str_posOf(str, end));
}
/**
 * @brief This Fntion finds the address of a character in a given number and returns the last one until reach null or pass the limit
 *
 * @param str address of string
 * @param c charachter that we want to search
 * @param num The number of times the Fntion looks for a character
 * @param len length of string
 * @return char* address of charachter, return null if not found
 */
char* Str_indexOfAtFix(const char* str, char c, Str_LenType num, Str_LenType len) {
    char* temp = NULL;
    while (num-- > 0) {
        temp = Mem_indexOf(str, c, len);
        if (!temp) {
            break;
        }
        len -= temp - str;
        str = temp;
    }
    return temp;
}

char* Str_reverseIndexOf(const char* str, char c, const char* startOfStr) {
    while (startOfStr <= str) {
        if (*str == c) {
            return (char*) str;
        }
        str--;
    }
    return NULL;
}

char* Str_reverseIndexOfFix(const char* str, char c, int length) {
    while (length-- > 0) {
        if (*str == c) {
            return (char*) str;
        }
        str--;
    }
    return NULL;
}

/**
 * @brief revesre order of charachter in string
 *
 * @param str address of string
 * @return char* address of str
 */
char*       Str_reverse(char* str) {
    return Mem_reverse(str, Str_len(str));
}
/**
 * @brief copy source string in reverse order into destination string
 *
 * @param src address of source string
 * @param dest address of destination string
 * @return char* address of dest
 */
char*       Str_copyReverse(char* dest, const char* src) {
    Mem_LenType len = Str_len(src);
    Mem_copyReverse(dest, src, len);
    dest[len] = __Str_Null;
    return dest;
}
/**
 * @brief copy a string into dest from given start position
 *
 * @param str address of base string
 * @param dest address of destination string
 * @param start Copy start location
 * @return char* address of dest
 */
char*       Str_substr(char* dest, const char* str, Str_LenType start) {
    return Str_copy(dest, &str[start]);
}
/**
 * @brief copy a string into dest from given start position
 *
 * @param str address of base string
 * @param dest address of destination string
 * @param start Copy start location
 * @param len length of characters to be copy
 * @return char* address of dest
 */
char*       Str_substrFix(char* dest, const char* str, Str_LenType start, Str_LenType len) {
    Str_copyFix(dest, &str[start], len);
    dest[len] = __Str_Null;
    return dest;
}
/**
 * @brief copy a string into dest from given start position until reach ending charachter
 *
 * @param str address of base string
 * @param dest address of destination string
 * @param start Copy start location
 * @param endChar ending charachter
 * @return char* address of dest
 */
char*       Str_substrUntil(char* dest, const char* str, Str_LenType start, char endChar) {
    Str_LenType len = Str_posOf(str, endChar);
    Str_copyFix(dest, &str[start], len);
    dest[len] = __Str_Null;
    return dest;
}
/**
 * @brief replace first word that find in string with a another word
 *
 * @param str address of string
 * @param word addres of word
 * @param replacement address of replacement
 * @return char* return address of str if word found otherwise return null
 */
char*       Str_replace(char* str, const char* word, const char* replacement) {
    char* pWord = Str_indexOfStr(str, word);
    if (pWord != NULL) {
        Str_LenType wordLen = Str_len(word);
        Str_LenType repLen = Str_len(replacement);
        if (wordLen != repLen) {
            char* pEndOfWord = pWord + wordLen;
            int len = (Str_LenType) ((char*) Mem_indexOf(pEndOfWord, __Str_Null, __Str_MaxLength) - pEndOfWord) + 1;
            Mem_move(pEndOfWord + (repLen - wordLen), pEndOfWord, len);
        }
        Str_copyFix(pWord, replacement, repLen);
        return pWord;
    }
    return NULL;
}
/**
 * @brief replace all words that find in string with a replacement word
 *
 * @param str address of string
 * @param word addres of word
 * @param replacement address of replacement
 * @return char* return address of str if word found otherwise return null
 */
int       Str_replaceAll(char* str, const char* word, const char* replacement) {
    int count = 0;
    int replacementLen = Str_len(replacement);
    while ((str = Str_replace(str, word, replacement)) != NULL) {
        str = str + replacementLen;
        count++;
    }
    return count;
}
/**
 * @brief copy sourc string into destination string until reach ending char
 *
 * @param src address of source string
 * @param dest address of destination string
 * @param endChar ending character
 * @return char* return null if source not contain ending character otherwise return destination
 */
char*       Str_copyUntil(char* dest, const char* src, char endChar) {
    char* pStr = Str_indexOf(src, endChar);
	if (pStr != NULL){
        Str_LenType len = (Str_LenType) (pStr - src);
        Str_copyFix(dest, src, len);
        dest[len] = __Str_Null;
        return dest;
	}
	return pStr;
}
/**
 * @brief read a line form source and copy it into destination
 *
 * @param src address of source string
 * @param dest address of destination string
 * @return char* return null if source not contain ending character otherwise return destination
 */
char*       Str_copyLine(char* dest, const char* src) {
    return Str_copyUntil(dest, src, '\n');
}
/**
 * @brief sort array of strings with selection sort algorithm
 *
 * @param strs array of strings
 * @param len length of array
 * @return char** return address of strs
 */
const char** Str_sort(const char** strs, Str_LenType len) {
    return Str_selectionSortBlock(strs, (Mem_LenType) len, (Str_CompareFn) Str_compare);
}
/**
 * @brief sort array of strings with quick sort algorithm
 *
 * @param strs array of strings
 * @param len length of array
 * @return const char** return address of strs
 */
const char** Str_quickSort(const char** strs, Str_LenType len) {
    return Str_quickSortBlock(strs, 0, (Mem_LenType) len - 1, (Str_CompareFn) Str_compare);
}
/**
 * @brief sort array of strings in reverse order, use selection sort algorithm
 *
 * @param strs array of strings
 * @param len length of array
 * @return char** return address of strs
 */
const char** Str_sortReverse(const char** strs, Str_LenType len) {
    return Str_selectionSortBlock(strs, (Mem_LenType) len, (Str_CompareFn) Str_compareInverse);
}
/**
 * @brief sort array of strings in reverse order, use quick sort algorithm
 *
 * @param strs array of strings
 * @param len length of array
 * @return char** return address of strs
 */
const char** Str_quickSortReverse(const char** strs, Str_LenType len) {
    return Str_quickSortBlock(strs, 0, (Mem_LenType) len - 1, (Str_CompareFn) Str_compareInverse);
}
/**
 * @brief Split string into 2D character array
 *
 * @param src address of string
 * @param separator separator character
 * @param strs address of 2D array
 * @param rowLen length of each row
 * @param len length of rows
 * @return Str_LenType return number of strs found
 */
Str_LenType Str_split(const char* src, char separator, char* strs, Str_LenType rowLen, Str_LenType len) {
    Str_LenType count = 0;
    Str_LenType tmpLen;
    char* end;

    // for null character
    rowLen--;

    if (*src != __Str_Null) {
        while ((end = Str_indexOf(src, separator)) != NULL && len-- > 0) {
            tmpLen = (Str_UNum)(end - src);
            // check it's enough space for string
            if (rowLen < tmpLen) {
                tmpLen = rowLen;
            }
            Str_copyFix(strs, src, tmpLen);
            strs[tmpLen] = __Str_Null;
            strs += rowLen + 1;
            src = end + 1;
            count++;
        }

        if (len > 0) {
            end = Str_indexOfNull(src);
            tmpLen = (Str_UNum)(end - src);
            if (rowLen < tmpLen) {
                tmpLen = rowLen;
            }
            Str_copyFix(strs, src, tmpLen);
            strs[tmpLen] = __Str_Null;
            count++;
        }
    }

    return count;
}
/**
 * @brief Split string into character pointer array
 *
 * @param src address of string
 * @param separator separator character
 * @param strs address of array
 * @param len length of array
 * @param setNull if it's 1 change base string and replace separator characters with null
 * @return Str_LenType return number of strs found
 */
Str_LenType Str_splitPtr(char* src, char separator, char** strs, Str_LenType len, uint8_t setNull) {
    Str_UNum count = 0;
    char* end;

    if (*src != __Str_Null) {
        while ((end = Str_indexOf(src, separator)) != NULL && len-- > 0) {
            *strs = src;
            if (setNull) {
                *end = __Str_Null;
            }
            strs++;
            src = end + 1;
            count++;
        }

        if (len > 0) {
            *strs = src;
            count++;
        }
    }

    return count;
}
/**
 * @brief Split string in serialize mode, put in in loop until return NULL
 * it's similar to strtok
 * remember it will replace separator characters with null
 *
 * @param src address of base string
 * @param separator separator character
 * @param ptr temporay character pointer
 * @return char* return next token or NULL if there is any
 */
char* Str_splitToken(char* src, char separator, char** ptr, uint8_t setNull) {
    char* end;

    if (src == NULL || *src == __Str_Null) {
        *ptr = NULL;
        return NULL;
    }

    if (*ptr < src) {
        *ptr = src;
    }

    src = *ptr;
    if (**ptr != __Str_Null) {
        end = Str_indexOf(*ptr, separator);
        if (end) {
            *ptr = end + 1;
            if (setNull) {
                *end = __Str_Null;
            }
        }
        else {
            *ptr = Str_indexOfNull(*ptr);
        }
    }

    return src;
}
/**
 * @brief find position of a character in string
 *
 * @param str address of string
 * @param c searching character
 * @return Str_LenType position of character
 */
Str_LenType Str_posOf(const char* str, char c) {
    return (Str_LenType) (Str_indexOf(str, c) - str);
}
/**
 * @brief find position of a character in string with reverse search
 *
 * @param str address of string
 * @param c searching character
 * @return Str_LenType position of charachter
 */
Str_LenType Str_lastPosOf(const char* str, char c) {
    return (Str_LenType) (Str_lastIndexOf(str, c) - str);
}
/**
 * @brief find position of start of word in string
 *
 * @param str address of string
 * @param word searching string
 * @return Str_LenType
 */
Str_LenType Str_posOfStr(const char* str, const char* word) {
    return (Str_LenType) (Str_indexOfStr(str, word) - str);
}
/**
 * @brief find string in array of strings with linear search algorithm
 *
 * @param strs array of strings
 * @param len length of strs
 * @param str searching string
 * @return Str_LenType position of str in strs
 */
Str_LenType Str_multiCompare(const char** strs, Str_LenType len, const char* str) {
    return Str_linearSearch(strs, len, str, (Str_CompareFn) Str_compare);
}
/**
 * @brief find string in array of strings with binary search algorithm
 *
 * @param strs array of strings
 * @param len length of strs
 * @param str searching string
 * @return Str_LenType position of str in strs
 */
Str_LenType Str_multiCompareSorted(const char** strs, Str_LenType len, const char* str) {
    return Str_binarySearch(strs, len, str, (Str_CompareFn) Str_compare);
}
/**
 * @brief find first string that found in array of strings
 *
 * @param src addres of source string
 * @param strs array of strings that we want find in source string
 * @param len length of strs
 * @param result index and position of finded string
 * @return const char* address of finded string in source string
 */
const char* Str_findStrs(const char* src, const char** strs, Str_LenType len, Str_MultiResult* result) {
    Str_LenType index;

    while (*src != __Str_Null) {
        for (index = 0; index < len; index++) {
            if (Str_compareWord(src, strs[index]) == 0) {
                result->IndexOf = src;
                result->Position = index;
                return src;
            }
        }
        src++;
    }
    return NULL;
}
/**
 * @brief find first string that found in array of strings with binary search algorithm
 *
 * @param src addres of source string
 * @param strs array of strings that we want find in source string
 * @param len length of strs
 * @param result index and position of finded string
 * @return const char* address of finded string in source string
 */
const char* Str_findStrsSorted(const char* src, const char** strs, Str_LenType len, Str_MultiResult* result) {
    Str_LenType index;
    Str_LenType pos;

    while (*src != __Str_Null) {
        for (index = 0; index < len; index++) {
            if ((pos = Str_binarySearch(strs, len, src, (Str_CompareFn) Str_compareWord)) != -1) {
                result->IndexOf = src;
                result->Position = pos;
                return src;
            }
        }
        src++;
    }
    return NULL;
}
/**
 * @brief find first string that found in array of strings
 *
 * @param src addres of source string
 * @param strs array of strings that we want find in source string
 * @param len length of strs
 * @param result index and position of finded string
 * @param srcLen len of source string
 * @return const char* address of finded string in source string
 */
const char* Str_findStrsFix(const char* src, const char** strs, Str_LenType len, Str_MultiResult* result, Str_LenType srcLen) {
    Str_LenType index;

    while (*src != __Str_Null && srcLen-- > 0) {
        for (index = 0; index < len; index++) {
            if (Str_compareWord(src, strs[index]) == 0) {
                result->IndexOf = src;
                result->Position = index;
                return src;
            }
        }
        src++;
    }
    return NULL;
}
/**
 * @brief find first string that found in array of strings with binary search algorithm
 *
 * @param src addres of source string
 * @param strs array of strings that we want find in source string
 * @param len length of strs
 * @param result index and position of finded string
 * @param srcLen len of source string
 * @return const char* address of finded string in source string
 */
const char* Str_findStrsSortedFix(const char* src, const char** strs, Str_LenType len, Str_MultiResult* result, Str_LenType srcLen) {
    Str_LenType index;
    Str_LenType pos;

    while (*src != __Str_Null && srcLen-- > 0) {
        for (index = 0; index < len; index++) {
            if ((pos = Str_binarySearch(strs, len, src, (Str_CompareFn) Str_compareWord)) >= 0) {
                result->IndexOf = src;
                result->Position = pos;
                return src;
            }
        }
        src++;
    }
    return NULL;
}
/**
 * @brief parse string text into real values
 * ex: "\"Test\"" -> "Test"
 * ex: "\"Line1 \\nLine 2\"" -> "Line 1 \nLine 2"
 *
 * @param string
 * @param str
 * @return Str_LenType
 */
Str_LenType Str_parseString(const char* string, char* str) {
    unsigned int unicodeChar;
    char* pSrc = str;
    // check start double quote
    if (*string++ != '"') {
        return -1;
    }
    // parse string
    while (*string != '\0' && *string != '"') {
        if (*string == '\\') {
            string++;
            switch (*string) {
                case '"': // quotaion mark
                    *str = '"';
                    break;
                case '\\': // reverse solidus
                    *str = '\\';
                    break;
                case '/': // solidus
                    *str = '/';
                    break;
                case 'b': // backspace
                    *str = '\b';
                    break;
                case 'f': // formfeed
                    *str = '\f';
                    break;
                case 'n': // line feed
                    *str = '\n';
                    break;
                case 'r': // carriage return
                    *str = '\r';
                    break;
                case 't': // tab
                    *str = '\t';
                    break;
                case 'u': // 4x HEX Digit
                    if (Str_convertUNumFix(++string, &unicodeChar, Str_Hex, 4) == Str_Ok) {
                        char temp = (char) (unicodeChar >> 8);
                        string += 3;
                        if (temp != __Str_Null) {
                            *str++ = temp;
                        }
                        *str = (char) unicodeChar;
                    }
                    else {
                        return -1;
                    }
                    break;
                default: // not support
                    return -1;
            }
        }
        else {
            *str = *string;
        }
        str++;
        string++;
    }
    // check end double quote
    if (*string != '"') {
        return -1;
    }
    // return len of str
    *str = __Str_Null;
    return (Str_LenType)(str - pSrc);
}
/**
 * @brief parse string text into real values
 * ex: "\"Test\"" -> "Test"
 * ex: "\"Line1 \\nLine 2\"" -> "Line 1 \nLine 2"
 *
 * @param str
 * @return Str_LenType
 */
Str_LenType Str_fromString(char* str) {
    return Str_parseString(str, str);
}
/**
 * @brief convert str into valid string format for serialize
 * ex: "Text\\n" -> "\"Text\\n\""
 *
 * @param str
 * @param string
 * @return char* return last index of string
 */
char* Str_convertString(const char* str, char* string) {
    // set first double quote
    *string++ = '"';
    // convert str
    while (*str != __Str_Null) {
        if (*str < ' ' || *str == '\\' || *str == '/' || *str == '"') {
            *string++ = '\\';
            switch (*str) {
                case '"':
                    *string = '"';
                    break;
                case '\n':
                    *string = 'n';
                    break;
                case '\r':
                    *string = 'r';
                    break;
                case '\t':
                    *string = 't';
                    break;
                case '\\':
                    *string = '\\';
                    break;
                case '/':
                    *string = '/';
                    break;
                case '\b':
                    *string = 'b';
                    break;
                case '\f':
                    *string = 'f';
                    break;
                default:
                    // ignore character
                    string--;
                    break;
            }
        }
        else {
            *string = *str;
        }
        string++;
        str++;
    }
    // set second double qoute
    *string++ = '"';
    *string = __Str_Null;
    return string;
}
/**
 * @brief convert str into valid string format for serialize
 * ex: "Text\n" -> "\"Text\\n\""
 *
 * @param str
 * @param string
 * @param len number of characters for convert
 * @return char* return last index of string
 */
char* Str_convertStringFix(const char* str, char* string, Str_LenType len) {
    // set first double quote
    *string++ = '"';
    // convert str
    while (*str != __Str_Null && len-- > 0) {
        if (*str < ' ' || *str == '\\' || *str == '/' || *str == '"') {
            *string++ = '\\';
            switch (*str) {
                case '"':
                    *string = '"';
                    break;
                case '\n':
                    *string = 'n';
                    break;
                case '\r':
                    *string = 'r';
                    break;
                case '\t':
                    *string = 't';
                    break;
                case '\\':
                    *string = '\\';
                    break;
                case '/':
                    *string = '/';
                    break;
                case '\b':
                    *string = 'b';
                    break;
                case '\f':
                    *string = 'f';
                    break;
                default:
                    // ignore character
                    string--;
                    break;
            }
        }
        else {
            *string = *str;
        }
        string++;
        str++;
    }
    // set second double qoute
    *string++ = '"';
    *string = __Str_Null;
    return string;
}
/**
 * @brief convert a number into string with specific base index and length
 *
 * @param num number that we want convert
 * @param base base index
 * @param minLen minimum length of result string
 * @param str address destination string
 * @return Str_LenType return length of str
 */
Str_LenType Str_parseNum(Str_Num num, Str_Radix base, Str_LenType minLen, char* str) {
    if (num < 0){
		*str++ = '-';
		num *= -1;
		return Str_parseUNum((Str_UNum) num, base, minLen, str) + 1;
	}
	else {
		return Str_parseUNum((Str_UNum) num, base, minLen, str);
	}
}
/**
 * @brief convert a unsigned number into string with specific base index and length
 *
 * @param num number that we want convert
 * @param base base index
 * @param minLen minimum length of result string
 * @param str address destination string
 * @return Str_LenType return length of str
 */
Str_LenType Str_parseUNum(Str_UNum num, Str_Radix base, Str_LenType minLen, char* str) {
    Str_LenType count = 0;
	char* pStr = str;
	char temp;
	do {
		temp = num % (Str_LenType) base;
		*pStr++ = temp < __Str_Decimal ? (temp + __Str_0) : (temp + 0x37);
		count++;
		num /= base;
	} while (num != 0 || count < minLen);
	Mem_reverse(str, (Mem_LenType) count);
	*pStr = __Str_Null;
	return count;
}
/**
 * @brief convert a string into number
 *
 * @param str address of source string
 * @param num address of output number
 * @param base base index
 * @return Str_Result result of convert
 */
Str_Result Str_convertNum(const char* str, Str_Num* num, Str_Radix base) {
    return Str_convertNumFix(str, num, base, __Str_MaxLength);
}
/**
 * @brief convert a string into unsigned number
 *
 * @param str address of source string
 * @param num address of output number
 * @param base base index
 * @return Str_Result result of convert
 */
Str_Result Str_convertUNum(const char* str, Str_UNum* num, Str_Radix base) {
    return Str_convertUNumFix(str, num, base, __Str_MaxLength);
}
/**
 * @brief convert a string into number with fixed length
 *
 * @param str address of source string
 * @param num address of output number
 * @param base base index
 * @return Str_Result result of convert
 */
Str_Result Str_convertNumFix(const char* str, Str_Num* num, Str_Radix base, Str_LenType len) {
    if (*str == '-'){
		Str_Result res;
		res = Str_convertUNumFix(++str, (Str_UNum*) num, base, --len);
		*num *= -1;
		return res;
	}
	else {
		return Str_convertUNumFix(str, (Str_UNum*) num, base, len);
	}
}
/**
 * @brief convert a string into unsigned number with fixed length
 *
 * @param str address of source string
 * @param num address of output number
 * @param base base index
 * @return Str_Result result of convert
 */
Str_Result Str_convertUNumFix(const char* str, Str_UNum* num, Str_Radix base, Str_LenType len) {
    Str_UNum temp;
	*num = 0;
	while (*str != __Str_Null && len-- > 0){
		if (*str >= __Str_0 && *str <= __Str_9){
			temp = *str - __Str_0;
		}
		else if (*str >= 'A' && *str <= 'Z'){
			temp = *str - 0x37;
		}
		else if (*str >= 'a' && *str <= 'z'){
			temp = *str - 0x57;
		}
		else {
			return Str_Error;
		}
		if (temp >= base){
			return Str_Error;
		}
		*num = *num * base + temp;
		str++;
	}
	return Str_Ok;
}
#if STR_ENABLE_LONG_NUMBER
/**
 * @brief convert a long number into string with specific base index and length
 *
 * @param num number that we want convert
 * @param base base index
 * @param minLen minimum length of result string
 * @param str address destination string
 * @return Str_LenType return length of str
 */
Str_LenType Str_parseLong(Str_Long num, Str_Radix base, Str_LenType minLen, char* str) {
    if (num < 0){
		*str++ = '-';
		num *= -1;
		return Str_parseULong((Str_ULong) num, base, minLen, str) + 1;
	}
	else {
		return Str_parseULong((Str_ULong) num, base, minLen, str);
	}
}
/**
 * @brief convert a unsigned long number into string with specific base index and length
 *
 * @param num number that we want convert
 * @param base base index
 * @param minLen minimum length of result string
 * @param str address destination string
 * @return Str_LenType return length of str
 */
Str_LenType Str_parseULong(Str_ULong num, Str_Radix base, Str_LenType minLen, char* str) {
    Str_LenType count = 0;
	char* pStr = str;
	char temp;
	do {
		temp = num % (Str_LenType) base;
		*pStr++ = temp < __Str_Decimal ? (temp + __Str_0) : (temp + 0x37);
		count++;
		num /= base;
	} while (num != 0 || count < minLen);
	Mem_reverse(str, (Mem_LenType) count);
	*pStr = __Str_Null;
	return count;
}
/**
 * @brief convert a string into long number
 *
 * @param str address of source string
 * @param num address of output number
 * @param base base index
 * @return Str_Result result of convert
 */
Str_Result Str_convertLong(const char* str, Str_Long* num, Str_Radix base) {
    return Str_convertLongFix(str, num, base, __Str_MaxLength);
}
/**
 * @brief convert a string into unsigned long number
 *
 * @param str address of source string
 * @param num address of output number
 * @param base base index
 * @return Str_Result result of convert
 */
Str_Result Str_convertULong(const char* str, Str_ULong* num, Str_Radix base) {
    return Str_convertULongFix(str, num, base, __Str_MaxLength);
}
/**
 * @brief convert a string into long number with fixed length
 *
 * @param str address of source string
 * @param num address of output number
 * @param base base index
 * @param len
 * @return Str_Result result of convert
 */
Str_Result Str_convertLongFix(const char* str, Str_Long* num, Str_Radix base, Str_LenType len) {
    if (*str == '-'){
		Str_Result res;
		res = Str_convertULongFix(++str, (Str_ULong*) num, base, --len);
		*num *= -1;
		return res;
	}
	else {
		return Str_convertULongFix(str, (Str_ULong*) num, base, len);
	}
}
/**
 * @brief convert a string into unsigned long number with fixed length
 *
 * @param str address of source string
 * @param num address of output number
 * @param base base index
 * @return Str_Result result of convert
 */
Str_Result Str_convertULongFix(const char* str, Str_ULong* num, Str_Radix base, Str_LenType len) {
    Str_UNum temp;
	*num = 0;
	while (*str != __Str_Null && len-- > 0){
		if (*str >= __Str_0 && *str <= __Str_9){
			temp = *str - __Str_0;
		}
		else if (*str >= 'A' && *str <= 'Z'){
			temp = *str - 0x37;
		}
		else if (*str >= 'a' && *str <= 'z'){
			temp = *str - 0x57;
		}
		else {
			return Str_Error;
		}
		if (temp >= base){
			return Str_Error;
		}
		*num = *num * base + temp;
		str++;
	}
	return Str_Ok;
}
#endif // STR_ENABLE_LONG_NUMBER
/**
 * @brief convert float number into string
 *
 * @param num float number
 * @param str address of output string
 * @return Str_LenType
 */
Str_LenType Str_parseFloat(float num, char* str) {
	int numInt = (int) num;
	Str_LenType len;
	len = Str_parseNum(numInt, __Str_Decimal, STR_NORMAL_LEN, str);
    str = str + len;
    num = num - numInt;
    if (num != 0) {
        *str++ = '.';
        len++;
        if (num < 0){
            num *= -1;
        }
        numInt = (int) num;
        while (num != 0) {
            num *= __Str_Decimal;
            numInt = (int) num;
            *str++ = numInt + __Str_0;
            len++;
            num -= numInt;
        }
    }
    *str = __Str_Null;
	return len;
}
/**
 * @brief convert float number into string
 *
 * @param num float number
 * @param str address of output string
 * @param decimalLen resolution of floating part
 * @return Str_LenType
 */
Str_LenType Str_parseFloatFix(float num, char* str, Str_LenType decimalLen) {
    int pow = decimalLen;
	int numInt = (int) num;
	Str_LenType len;
	len = Str_parseNum(numInt, __Str_Decimal, STR_NORMAL_LEN, str);
    if (decimalLen != 0) {
        str = str + len;
        *str++ = '.';
        num = num - numInt;
        if (num < 0){
            num *= -1;
        }
        while (pow-- != 0){
            num *= __Str_Decimal;
        }
	    len += Str_parseNum((int) num, __Str_Decimal, STR_NORMAL_LEN, str) + 1;
    }
	return len;
}
/**
 * @brief convert a string into float number
 *
 * @param str address of source string
 * @param num address of output number
 * @return Str_Result result of convert
 */
Str_Result Str_convertFloat(const char* str, float* num) {
    return Str_convertFloatFix(str, num, Str_len(str));
}
/**
 * @brief convert a string into float number with fixed length
 *
 * @param str address of source string
 * @param num address of output number
 * @param len number of charactres for convert
 * @return Str_Result result of convert
 */
Str_Result Str_convertFloatFix(const char* str, float* num, Str_LenType len) {
    const char* pDot = Str_indexOf(str, '.');
	float temp;
	int numInt;
	Str_LenType strLen;
	if (pDot == NULL){
		pDot = Mem_indexOf(str, __Str_Null, __Str_MaxLength);
	}
	strLen = (Str_LenType)(pDot - str);
	if (strLen > len){
		strLen = len;
	}
	if (Str_convertNumFix(str, &numInt, __Str_Decimal, strLen) == Str_Ok){
		*num = numInt;
		if (*pDot != __Str_Null){
			len -= strLen + 1;
			if (Str_convertNumFix(++pDot, &numInt, Str_Decimal, len) != Str_Ok){
				return Str_Error;
			}
			temp = numInt;
            while (len-- > 0) {
                temp /= __Str_Decimal;
            }
            *num = *num < 0 ? *num - temp : *num + temp;
		}
		return Str_Ok;
	}
	return Str_Error;
}
/**
 * @brief ignore all characters below than space character (' ' = 0x20)
 *
 * @param str
 * @return char* return address of first character
 */
char* Str_ignoreWhitespace(const char* str) {
    while (*str <= ' ' && *str != __Str_Null) {
        str++;
    }
    return (char*) str;
}
/**
 * @brief ignore all characters above and equal than space character (' ' = 0x20)
 *
 * @param str
 * @return char* return address of first whitespace
 */
char* Str_ignoreCharacters(const char* str) {
    while (*str >= ' ' && *str != __Str_Null) {
        str++;
    }
    return (char*) str;
}
/**
 * @brief ignore all a-z,A-Z,0-9 characters
 *
 * @param str
 * @return char*
 */
char* Str_ignoreAlphaNumeric(const char* str) {
    while (*str != __Str_Null &&
            ((*str >= '0' && *str <= '9') ||
             (*str >= 'A' && *str <= 'Z') ||
             (*str >= 'a' && *str <= 'z'))) {
        str++;
    }
    return (char*) str;
}
/**
 * @brief ignore all alphabet characters, A-Z,a-z
 *
 * @param str
 * @return char*
 */
char*       Str_ignoreAlphabet(const char* str) {
    while (*str != __Str_Null &&
            ((*str >= 'A' && *str <= 'Z') ||
             (*str >= 'a' && *str <= 'z'))) {
        str++;
    }
    return (char*) str;
}
/**
 * @brief ignore all numeric characters, 0-9
 *
 * @param str
 * @return char*
 */
char* Str_ignoreNumeric(const char* str) {
    while (*str != __Str_Null && (*str >= '0' && *str <= '9')) {
        str++;
    }
    return (char*) str;
}
/**
 * @brief ignore all special characters
 *
 * @param str
 * @return char*
 */
char* Str_ignoreSpecialCharacters(const char* str) {
    while (*str != __Str_Null &&
            ((*str >= '!' && *str <= '/') ||
             (*str >= ':' && *str <= '@') ||
             (*str >= '[' && *str <= '`') ||
             (*str >= '{' && *str <= '~'))) {
        str++;
    }
    return (char*) str;
}
/**
 * @brief ignore all following characters: 0-9,A-Z,a-z, '-', '_'
 *
 * @param str
 * @return char*
 */
char* Str_ignoreNameCharacters(const char* str) {
    while (*str != __Str_Null &&
            ((*str >= '0' && *str <= '9') ||
             (*str >= 'A' && *str <= 'Z') ||
             (*str >= 'a' && *str <= 'z') ||
              *str == '-' || *str == '_')) {
        str++;
    }
    return (char*) str;
}
/**
 * @brief ingore following characters: ':'-'@','#'-'&','!'
 *
 * @param str
 * @return char*
 */
char* Str_ignoreCommandCharacters(const char* str) {
    while (*str != __Str_Null &&
            ((*str >= ':' && *str <= '@') ||
             (*str >= '#' && *str <= '&') ||
              *str == '!')) {
        str++;
    }
    return (char*) str;
}
/**
 * @brief ignore characters in given str with custom role
 *
 * @param str
 * @param cmp
 * @return char*
 */
char* Str_ignore(const char* str, Str_IgnoreRoleFn cmp) {
    while (*str != __Str_Null && (char) cmp(*str)) {
        str++;
    }
    return (char*) str;
}
/**
 * @brief ignore whitespaces from last of string
 *
 * @param str
 * @return char*
 */
char* Str_ignoreWhitespaceReverse(const char* str) {
    str = Str_indexOfEnd(str);
    while (*str <= ' ' && *str != __Str_Null) {
        str--;
    }
    return (char*) str;
}
/**
 * @brief ignore characters from last of string
 *
 * @param str
 * @return char*
 */
char* Str_ignoreCharactersReverse(const char* str) {
    str = Str_indexOfEnd(str);
    while (*str > ' ' && *str != __Str_Null) {
        str--;
    }
    return (char*) str;
}
/**
 * @brief change all characters into upper case
 *
 * @param str
 * @return char*
 */
void Str_upperCase(char* str) {
    while (*str != __Str_Null) {
        if (*str >= 'a' && *str <= 'z') {
            *str = *str - 32;
        }
        str++;
    }
}
/**
 * @brief change all characters into upper case, with fixed Length
 *
 * @param str
 * @param len
 * @return char*
 */
void Str_upperCaseFix(char* str, Str_LenType len) {
    while (*str != __Str_Null && len-- > 0) {
        if (*str >= 'a' && *str <= 'z') {
            *str = *str - 32;
        }
        str++;
    }
}
/**
 * @brief change all characters into lower case
 *
 * @param str
 */
void Str_lowerCase(char* str) {
    while (*str != __Str_Null) {
        if (*str >= 'A' && *str <= 'Z') {
            *str = *str + 32;
        }
        str++;
    }
}
/**
 * @brief change all characters into lower case with fix Length
 *
 * @param str
 */
void Str_lowerCaseFix(char* str, Str_LenType len) {
    while (*str != __Str_Null && len-- > 0) {
        if (*str >= 'A' && *str <= 'Z') {
            *str = *str + 32;
        }
        str++;
    }
}
/**
 * @brief remove all whitespace in left of string
 *
 * @param str
 * @return char*
 */
char* Str_trimLeft(char* str) {
    char* pEnd = Str_ignoreWhitespace(str);
    if (pEnd != str) {
        Mem_move(str, pEnd, Str_len(str) - (Mem_LenType)(pEnd - str) + 1);
    }
    return str;
}
/**
 * @brief remove all whitespaces in right of string
 *
 * @param str
 * @return char*
 */
char* Str_trimRight(char* str) {
    char* pEnd = Str_ignoreWhitespaceReverse(str) + 1;
    *pEnd = __Str_Null;
    return str;
}
/**
 * @brief trim left and right of string
 *
 * @param str
 * @return char*
 */
char* Str_trim(char* str) {
    Str_trimRight(str);
    Str_trimLeft(str);
    return str;
}
/**
 * @brief remove all backspace ('\b') characters and left character
 * ex: "ABCD\bER" -> "ABCER"
 *
 * @param str
 * @return Str_LenType return new length
 */
Str_LenType Str_removeBackspace(char* str) {
    Str_LenType len = Str_len(str);
    if (len > 0) {
        char* pStr = str + len - 1;
        while (str < pStr) {
            if (*pStr-- == '\b') {
                Mem_move(pStr, pStr + 2, len - (Mem_LenType)(pStr - str));
                len -= 2;
            }
        }
        if (*pStr == '\b') {
            Mem_move(pStr, pStr + 1, len - (Mem_LenType)(pStr - str));
            len -= 1;
        }
    }
    return len;
}
/**
 * @brief remove all backspace ('\b') characters and left character with fix len
 * ex: "ABCD\bER" -> "ABCER"
 *
 * @param str
 * @param len
 * @return Str_LenType return new length
 */
Str_LenType Str_removeBackspaceFix(char* str, Str_LenType len) {
    if (len > 0) {
        char* pStr = str + len - 1;
        while (str < pStr) {
            if (*pStr-- == '\b') {
                Mem_move(pStr, pStr + 2, len - (Mem_LenType)(pStr - str));
                len -= 2;
            }
        }
        if (*pStr == '\b') {
            Mem_move(pStr, pStr + 1, len - (Mem_LenType)(pStr - str));
            len -= 1;
        }
    }
    return len;
}
#if STR_ENABLE_DOUBLE
/**
 * @brief convert double number into string
 *
 * @param num float number
 * @param str address of output string
 * @return Str_LenType
 */
Str_LenType Str_parseDouble(double num, char* str) {
	int numInt = (int) num;
	Str_LenType len;
	len = Str_parseNum(numInt, __Str_Decimal, STR_NORMAL_LEN, str);
    str = str + len;
    num = num - numInt;
    if (num != 0) {
        *str++ = '.';
        len++;
        if (num < 0){
            num *= -1;
        }
        numInt = (int) num;
        while (num != 0) {
            num *= __Str_Decimal;
            numInt = (int) num;
            *str++ = numInt + __Str_0;
            len++;
            num -= numInt;
        }
    }
    *str = __Str_Null;
	return len;
}
/**
 * @brief convert double number into string
 *
 * @param num float number
 * @param str address of output string
 * @param decimalLen resolution of floating part
 * @return Str_LenType
 */
Str_LenType Str_parseDoubleFix(double num, char* str, Str_LenType decimalLen) {
    int pow = decimalLen;
	int numInt = (int) num;
	Str_LenType len;
	len = Str_parseNum(numInt, __Str_Decimal, STR_NORMAL_LEN, str);
    if (decimalLen != 0) {
        str = str + len;
        *str++ = '.';
        num = num - numInt;
        if (num < 0){
            num *= -1;
        }
        while (pow-- != 0){
            num *= __Str_Decimal;
        }
	    len += Str_parseNum((int) num, __Str_Decimal, STR_NORMAL_LEN, str) + 1;
    }
	return len;
}

/**
 * @brief convert a string into double number
 *
 * @param str address of source string
 * @param num address of output number
 * @return Str_Result result of convert
 */
Str_Result Str_convertDouble(const char* str, double* num) {
    return Str_convertDoubleFix(str, num, Str_len(str));
}
/**
 * @brief convert a string into double number with fixed length
 *
 * @param str address of source string
 * @param num address of output number
 * @return Str_Result result of convert
 */
Str_Result Str_convertDoubleFix(const char* str, double* num, Str_LenType len) {
    const char* pDot = Str_indexOf(str, '.');
	double temp;
	int numInt;
	Str_LenType strLen;
	if (pDot == NULL){
		pDot = Mem_indexOf(str, __Str_Null, __Str_MaxLength);
	}
	strLen = (Str_LenType)(pDot - str);
	if (strLen > len){
		strLen = len;
	}
	if (Str_convertNumFix(str, &numInt, __Str_Decimal, strLen) == Str_Ok){
		*num = numInt;
		if (*pDot != __Str_Null){
			len -= strLen + 1;
			if (Str_convertNumFix(++pDot, &numInt, __Str_Decimal, len) != Str_Ok){
				return Str_Error;
			}
			temp = numInt;
            while (len-- > 0) {
                temp /= __Str_Decimal;
            }
            *num = *num < 0 ? *num - temp : *num + temp;
		}
		return Str_Ok;
	}
	return Str_Error;
}

#endif // STR_ENABLE_DOUBLE
/**
 * @brief get first number that found in string
 *
 * @param str address of source string
 * @param num number that found
 * @param numPos index of number that found
 * @return Str_Result result of searching for number
 */
Str_Result Str_getNum(const char* str, int* num, const char** numPos) {
    const char* baseStr = str;
	str = Str_findDigit(str);
	if (str != NULL){
		if (baseStr < str && *(str - 1) == '-'){
			str--;
		}
		*numPos = str;
		if (Str_convertNumFix(str, num, __Str_Decimal, (Str_LenType) (Str_findLastDigit(str) - str + 1)) == Str_Ok){
			return Str_Ok;
		}
	}
	return Str_Error;
}
/**
 * @brief get first unsigned number that found in string
 *
 * @param str address of source string
 * @param num number that found
 * @param numPos index of number that found
 * @return Str_Result result of searching for number
 */
Str_Result Str_getUNum(const char* str, unsigned int* num, const char** numPos) {
	str = Str_findDigit(str);
	if (str != 0){
		*numPos = str;
		if (Str_convertUNumFix(str, num, __Str_Decimal, (Str_LenType) (Str_findLastDigit(str) - str + 1)) == Str_Ok){
			return Str_Ok;
		}
	}
	return Str_Error;
}
/**
 * @brief get first float number that found in string
 *
 * @param str address of source string
 * @param num number that found
 * @param numPos index of number that found
 * @return Str_Result result of searching for number
 */
Str_Result Str_getFloat(const char* str, float* num, const char** numPos) {
	const char* pDot = str;
	str = Str_findDigit(str);
	if (str != NULL){
		if (pDot < str && *(str - 1) == '-'){
			str--;
		}
		pDot = Str_findLastDigit(str) + 1;
		if (*(pDot) == '.'){
			pDot = Str_findLastDigit(pDot);
            pDot = pDot == NULL ? Str_indexOfEnd(str) : pDot + 1;
		}
		*numPos = str;
		if (Str_convertFloatFix(str, num, pDot - str) == Str_Ok){
			return Str_Ok;
		}
	}
	return Str_Error;
}
/**
 * @brief get token from an string
 * ex: getToken("123,ABCD,48", ',', 1, token);
 *     assert(token, "ABCD");
 *
 * @param str
 * @param separator
 * @param index
 * @param token
 * @return Str_Result
 */
Str_Result Str_getToken(const char* str, char separator, Str_LenType index, char* token) {
    return Str_getTokenFix(str, separator, index, token, STR_NORMAL_LEN);
}
/**
 * @brief get token from an string
 * ex: getTokenFix("123,ABCD,48", ',', 1, token, sizeof(token) - 1);
 *     assert(token, "ABCD");
 *
 * @param str
 * @param separator
 * @param index
 * @param token
 * @param len
 * @return Str_Result
 */
Str_Result Str_getTokenFix(const char* str, char separator, Str_LenType index, char* token, Str_LenType len) {
    if ((str = Str_indexOfAt(str, separator, index)) != NULL) {
        char* end;
        Mem_LenType tokenLen;
        if (*str == separator) {
            str++;
        }
        if ((end = Str_indexOf(str, separator)) == NULL) {
            end = Str_indexOfNull(str);
        }
        tokenLen = (Mem_LenType)(end - str);
        if (STR_NORMAL_LEN != len && tokenLen >= len) {
            tokenLen = len;
        }
        Mem_copy(token, str, tokenLen);
        token[tokenLen] = __Str_Null;
        return Str_Ok;
    }

    return Str_Error;
}
/**
 * @brief swap address of two string
 *
 * @param itemA string 1
 * @param itemB string 2
 */
void Str_swap(const char** itemA, const char** itemB) {
    const char* temp = *itemA;
    *itemA = *itemB;
    *itemB = temp;
}

Str_LenType Str_partition(const char** items, Str_LenType low, Str_LenType high, Str_CompareFn cmp) {
    const char* pivot = items[high];    // pivot
    Str_LenType i = (low - 1);  // Index of smaller element
    Str_LenType j;
    for (j = low; j <= high - 1; j++)
    {
        // If current element is smaller than the pivot
        if (cmp(items[j], pivot) < 0)
        {
            i++;    // increment index of smaller element
            Str_swap(&items[i], &items[j]);
        }
    }
    Str_swap(&items[i + 1], &items[high]);
    return (i + 1);
}
/**
 * @brief run quick sort algorithm for items with given compare Fntion
 *
 * @param items array of strings
 * @param low position of start
 * @param high position of end
 * @param cmp address of compare Fntion
 * @return char** return items
 */
const char** Str_quickSortBlock(const char** items, Mem_LenType low, Mem_LenType high, Str_CompareFn cmp) {
    if (low < high) {
        /* pi is partitioning index, arr[p] is now
           at right place */
        Str_LenType pi = Str_partition(items, low, high, cmp);

        // Separately sort elements before
        // partition and after partition
        Str_quickSortBlock(items, low, pi - 1, cmp);
        Str_quickSortBlock(items, pi + 1, high, cmp);
    }
    return items;
}
/**
 * @brief run selection sort algorithm for item with given compare Fntion
 *
 * @param items array of strings
 * @param len length of array
 * @param cmp address of compare Fntion
 * @return char**
 */
const char** Str_selectionSortBlock(const char** items, Mem_LenType len, Str_CompareFn cmp) {
    Mem_LenType i;
    Mem_LenType j;
    Mem_LenType min_idx;

    // One by one move boundary of unsorted sub-array
    for (i = 0; i < len-1; i++)
    {
        // Find the minimum element in unsorted array
        min_idx = i;
        for (j = i+1; j < len; j++) {
          if (cmp(items[j], items[min_idx]) < 0) {
            min_idx = j;
          }
        }
        // Swap the found minimum element with the first element
        Str_swap(&items[min_idx], &items[i]);
    }
    return items;
}
/**
 * @brief find string in array of strings
 *
 * @param strs array of strings
 * @param len len of array
 * @param str
 * @param cmp address of comperator function
 * @return Str_LenType index of string in array
 */
Str_LenType Str_linearSearch(const char** strs, Str_LenType len, const char* str, Str_CompareFn cmp) {
    Str_LenType tempLen = len;
    while (len--) {
        if (cmp(str, *strs++) == 0) {
            return tempLen - len - 1;
        }
    }
    return -1;
}
/**
 * @brief search a string in array of strings with binary search algorithm
 *
 * @param strs
 * @param len
 * @param str
 * @param cmp
 * @return Str_LenType
 */
Str_LenType Str_binarySearch(const char** strs, Str_LenType len, const char* str, Str_CompareFn cmp) {
    Str_LenType left = 0;
    Str_LenType mid;
    char result;

    len--;

    while (left <= len) {
        mid = (len + left) >> 1;
        result = cmp(str, strs[mid]);
        // Check if str is present at mid
        if (result == 0) {
            return mid;
        }
        // If str greater, ignore left half
        else if (result > 0) {
            left = mid + 1;
        }
        // If x is smaller, ignore right half
        else {
            len = mid - 1;
        }
    }
    // if we reach here, then element was
    // not present
    return -1;
}
/**
 * @brief run selection sort algorithm for item with given compare Fntion, and swap function
 *
 * @param items array of items such as struct
 * @param len length of array
 * @param itemLen size of single item
 * @param cmp address of compare Fntion
 * @param swap address of swap function
 * @return char**
 */
void* Mem_sort(void* items, Mem_LenType len, Mem_LenType itemLen, Mem_CompareFn cmp, Mem_SwapFn swap) {
    Mem_LenType i;
    Mem_LenType j;
    Mem_LenType min_idx;
    unsigned char* pItems = (unsigned char*) items;

    // One by one move boundary of unsorted sub-array
    len *= itemLen;
    for (i = 0; i < len - itemLen; i += itemLen)
    {
        // Find the minimum element in unsorted array
        min_idx = i;
        for (j = i + itemLen; j < len; j += itemLen) {
          if (cmp(&pItems[j], &pItems[min_idx], itemLen) < 0) {
            min_idx = j;
          }
        }
        // Swap the found minimum element with the first element
        swap(&pItems[min_idx], &pItems[i], itemLen);
    }
    return items;
}
/**
 * @brief sort array of items with quick sort algorithm
 *
 * @param items array of items such as struct
 * @param len length of array
 * @param itemLen sizeof single item
 * @param cmp address of compare function
 * @param swap address of swap function
 * @return const char** return address of strs
 */
void* Mem_quickSort(void* items, Mem_LenType len, Mem_LenType itemLen, Mem_CompareFn cmp, Mem_SwapFn swap) {
    return Mem_quickSortBlock(items, 0, (Mem_LenType) (len - 1) * itemLen, itemLen, cmp, swap);
}
Mem_LenType Mem_partition(void* items, Mem_LenType low, Mem_LenType high, Mem_LenType itemLen, Mem_CompareFn cmp, Mem_SwapFn swap) {
    unsigned char* pItems = (unsigned char*) items;
    const unsigned char* pivot = &pItems[high];    // pivot
    Mem_LenType i = (low - itemLen);  // Index of smaller element
    Mem_LenType j;
    for (j = low; j <= high - itemLen; j += itemLen)
    {
        // If current element is smaller than the pivot
        if (cmp(&pItems[j], pivot, itemLen) < 0)
        {
            i += itemLen;    // increment index of smaller element
            swap(&pItems[i], &pItems[j], itemLen);
        }
    }
    swap(&pItems[i + itemLen], &pItems[high], itemLen);
    return (i + itemLen);
}
void* Mem_quickSortBlock(void* items, Mem_LenType low, Mem_LenType high, Mem_LenType itemLen, Mem_CompareFn cmp, Mem_SwapFn swap) {
    if (low < high) {
        /* pi is partitioning index, arr[p] is now
           at right place */
        Mem_LenType pi = Mem_partition(items, low, high, itemLen, cmp, swap);

        // Separately sort elements before
        // partition and after partition
        Mem_quickSortBlock(items, low, pi - itemLen, itemLen, cmp, swap);
        Mem_quickSortBlock(items, pi + itemLen, high, itemLen, cmp, swap);
    }
    return items;
}
/**
 * @brief find item in array
 *
 * @param items
 * @param len
 * @param itemLen
 * @param item
 * @param cmp
 * @return Mem_LenType
 */
Mem_LenType Mem_linearSearch(const void* items, Mem_LenType len, Mem_LenType itemLen, const void* item, Mem_CompareFn cmp) {
    unsigned char* pItems = (unsigned char*) items;
    Mem_LenType pIndex;
    len *= itemLen;


    for (pIndex = 0; pIndex < len; pIndex += itemLen, pItems += itemLen) {
        if (cmp(item, pItems, itemLen) == 0) {
            return pIndex / itemLen;
        }

    }
    return -1;
}
/**
 * @brief find item in array with binary search algorithm
 *
 * @param items
 * @param len
 * @param itemLen
 * @param item
 * @param cmp
 * @return Mem_LenType
 */
Mem_LenType Mem_binarySearch(const void* items, Mem_LenType len, Mem_LenType itemLen, const void* item, Mem_CompareFn cmp) {
    unsigned char* pItems = (unsigned char*) items;
    Mem_LenType left = 0;
    Mem_LenType mid;
    char result;

    len--;

    while (left <= len) {
        mid = (len + left) >> 1;
        result = cmp(item, &pItems[mid * itemLen], itemLen);
        // Check if str is present at mid
        if (result == 0) {
            return mid;
        }
        // If str greater, ignore left half
        else if (result > 0) {
            left = mid + 1;
        }
        // If x is smaller, ignore right half
        else {
            len = mid - 1;
        }
    }
    // if we reach here, then element was
    // not present
    return -1;
}

#if !STR_USE_STRING_LIBRARY

void* Mem_copy(void* dest, const void* src, Mem_LenType len) {
    unsigned char* pDest = (unsigned char*) dest;
    const unsigned char* pSrc = (const unsigned char*) src;
    while (len-- > 0) {
        *pDest++ = *pSrc++;
    }
    return dest;
}
char Mem_compare(const void* arr1, const void* arr2, Mem_LenType len) {
    const unsigned char* pArr1 = (const unsigned char*) arr1;
    const unsigned char* pArr2 = (const unsigned char*) arr2;
    char result;
    while (len-- > 0) {
        if((result = *pArr1++ - *pArr2++)) {
            break;
        }
    }
    return result;
}
void* Mem_move(void* dest, const void* src, Mem_LenType len) {
    unsigned char* pDest = (unsigned char*) dest;
    const unsigned char* pSrc = (const unsigned char*) src;
    if (len <= 0 || src == dest) {
        return NULL;
    }
    if (pDest < pSrc) {
        while (len-- > 0) {
            *pDest++ = *pSrc++;
        }
    }
    else {
        const unsigned char* lastSrc = (const unsigned char*) (pSrc + (len - 1));
        unsigned char* lastDest = (unsigned char*) (pDest + (len - 1));
        while (len-- > 0) {
            *lastDest-- = *lastSrc--;
        }
    }
    return dest;
}
void* Mem_indexOf(const void* arr, unsigned char value, Mem_LenType len) {
    const unsigned char* pArr = (const unsigned char*) arr;

    while (len-- > 0) {
        if (*pArr == value) {
            return (void*) pArr;;
        }
        pArr++;
    }
    return NULL;
}
void* Mem_set(void* arr, unsigned char value, Mem_LenType len) {
    unsigned char* pArr = (unsigned char*) arr;

    while (len-- > 0) {
        *pArr++ = value;
    }
    return arr;
}



char* Str_copy(char* dest, const char* src) {
    char* pDest = dest;
    while (*src != __Str_Null) {
        *pDest++ = *src++;
    }
    *pDest = __Str_Null;
    return dest;
}
char* Str_copyFix(char* dest, const char* src, Str_LenType len) {
    return Mem_copy(dest, src, len);
}
char Str_compare(const char* str1, const char* str2) {
    char result;
    while (*str1 != __Str_Null) {
        if ((result = *str1++ - *str2++)) {
            break;
        }
    }
    return result;
}
char Str_compareFix(const char* str1, const char* str2, Str_LenType len) {
    return Mem_compare(str1, str2, len);
}
Str_LenType Str_len(const char* str) {
    char* pStr = str;
    while (*pStr != __Str_Null) {
        pStr++;
    }
    return (Str_LenType) (pStr - str);
}
char* Str_indexOf(char* str, char c) {
    while (*str != __Str_Null) {
        if (*str == c) {
            return str;
        }
        str++;
    }
    return NULL;
}
char* Str_lastIndexOf(char* str, char c) {
    char* pStr = (char*) Mem_indexOf(str, '\'0', __Str_MaxLength) - 1;
    while (pStr >= str) {
        if (*pStr == c) {
            return pStr;
        }
        pStr--;
    }
    return NULL;
}
char* Str_indexOfStr(char* str, char* sub) {
    while (*str != __Str_Null && str != NULL) {
        str = Str_indexOf(str, *sub);
        if (Str_compareWord(str, sub) == 0) {
            return str;
        }
    }
    return NULL;
}
char* Str_append(char* str, char* sub) {
    char* pStr = (char*) Mem_indexOf(str, '\'0', __Str_MaxLength);
    Str_copy(pStr, sub);
    return str;
}

#endif // STR_USE_STRING_LIBRARY
