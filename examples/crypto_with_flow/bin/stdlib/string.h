#ifndef STRING_H
#define STRING_H

#include <stddef.h>
//@ #include <crypto.gh>

char *strcpy(char *d, char *s);
    //@ requires [?f]string(s, ?cs) &*& chars(d, length(cs) + 1, _);
    //@ ensures [f]string(s, cs) &*& chars(d, length(cs) + 1, append(cs, {0})) &*& result == d;

void memcpy(void *array, void *array0, size_t count);
    /*@ requires chars(array, count, ?cs) &*&
                 [?f]optional_crypto_chars(?cc, array0, count, ?cs0); @*/
    /*@ ensures  optional_crypto_chars(cc, array, count, cs0) &*&
                 [f]optional_crypto_chars(cc, array0, count, cs0); @*/

void memmove(void *dest, void *src, size_t count);
    /*@
    requires
        chars(src, count, ?cs) &*&
        dest <= src ?
            chars(dest, src - dest, _)
        :
            chars(src + count, dest - src, _);
    @*/
    /*@
    ensures
        chars(dest, count, cs) &*&
        dest <= src ?
            chars(dest + count, src - dest, _)
        :
            chars(src, dest - src, _);
    @*/

int strlen(char *string);
    //@ requires [?f]string(string, ?cs);
    //@ ensures [f]string(string, cs) &*& result == length(cs);

//@ predicate memcpm_hmac(cryptogram cg) = true;


int memcmp(char *array, char *array0, size_t count);
    /*@ requires [?f1]chars(array, ?n, ?cs) &*&
                 [?f2]optional_crypto_chars(?cc, array0, ?n0, ?cs0) &*&
                 n <= length(cs) &*& n0 <= length(cs0) &*&
                 //only allowed for checking sha512 hmac
                 cc ?
                   memcpm_hmac(?hmac_cg) &*&
                   n0 == n &*& (n0 == 48 || n0 == 64) &*&
                   cs0 == chars_for_cg(hmac_cg) &*&
                   hmac_cg == cg_hmac(?p, ?c, ?pay)
                 :
                   true; @*/
    /*@ ensures  [f1]chars(array, n, cs) &*&
                 [f2]optional_crypto_chars(cc, array0, n0, cs0) &*&
                 true == ((result == 0) == (take(count, cs) == take(count, cs0))); @*/

int strcmp(char *s1, char *s2);
    //@ requires [?f1]string(s1, ?cs1) &*& [?f2]string(s2, ?cs2);
    //@ ensures [f1]string(s1, cs1) &*& [f2]string(s2, cs2) &*& true == ((result == 0) == (cs1 == cs2));

char *memchr(char *array, char c, size_t count);
    //@ requires [?f]chars(array, count, ?cs);
    //@ ensures [f]chars(array, count, cs) &*& result == 0 ? mem(c, cs) == false : mem(c, cs) == true &*& result == array + index_of(c, cs);

char* strchr(char *str, char c);
    //@ requires [?f]string(str, ?cs);
    /*@ ensures
            [f]string(str, cs) &*&
            c == 0 ? 
                result == str + length(cs)
            : 
                result == 0 ?
                    mem(c, cs) == false
                :
                    mem(c, cs) == true &*& result == str + index_of(c, cs);
    @*/

void* memset(void *array, char value, size_t size);
    //@ requires chars(array, size, ?cs);
    //@ ensures chars(array, size, ?cs1) &*& all_eq(cs1, value) == true &*& result == array;

char *strdup(char *string);
    //@ requires [?f]string(string, ?cs);
    //@ ensures [f]string(string, cs) &*& result == 0 ? true : string(result, cs) &*& malloc_block_chars(result, length(cs) + 1);

#endif
