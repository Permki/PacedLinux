/*
-------------------------------------------------------------------------------
lookup3.c, by Bob Jenkins, May 2006, Public Domain.

These are functions for producing 32-bit hashes for hash table lookup.
hashword(), hashlittle(), hashlittle2(), hashbig(), mix(), and final()
are externally useful functions.  Routines to test the hash are included
if SELF_TEST is defined.  You can use this free for any purpose.  It's in
the public domain.  It has no warranty.

You probably want to use hashlittle().  hashlittle() and hashbig()
hash byte arrays.  hashlittle() is is faster than hashbig() on
little-endian machines.  Intel and AMD are little-endian machines.
On second thought, you probably want hashlittle2(), which is identical to
hashlittle() except it returns two 32-bit hashes for the price of one.
You could implement hashbig2() if you wanted but I haven't bothered here.

If you want to find a hash of, say, exactly 7 integers, do
  a = i1;  b = i2;  c = i3;
  mix(a,b,c);
  a += i4; b += i5; c += i6;
  mix(a,b,c);
  a += i7;
  final(a,b,c);
then use c as the hash value.  If you have a variable length array of
4-byte integers to hash, use hashword().  If you have a byte array (like
a character string), use hashlittle().  If you have several byte arrays, or
a mix of things, see the comments above hashlittle().

Why is this so big?  I read 12 bytes at a time into 3 4-byte integers,
then mix those integers.  This is fast (you can do a lot more thorough
mixing with 12*3 instructions on 3 integers than you can with 3 instructions
on 1 byte), but shoehorning those bytes into integers efficiently is messy.
-------------------------------------------------------------------------------
*/
#ifndef _JENKINS_HASH_C_H_
#define _JENKINS_HASH_C_H_

#include <nfp.h>

/** Hash return value
 */
typedef struct hash_result_t
{
    unsigned int value_0;
    unsigned int value_1;
} hash_result_t;


/** Calculates a hash over a given key in C using the Jenkins hash algorithm.
 * See lookup3.c. Function takes two seeds and return two 32-bit values.
 *
 * @param in_key_index  the key, an array of unsigned int values
 * @param in_key_length the key length in 32 bit long words
 * @return              64 bit hash result of type hash_result_t
 */
 __declspec(gp_reg) hash_result_t hash_jenkins (
    __declspec(local_mem) unsigned int      *in_key_index,
    __declspec(gp_reg)    unsigned int      in_key_length
);



#endif  // _JENKINS_HASH_C_H_