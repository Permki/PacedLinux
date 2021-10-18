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
#ifndef _JENKINS_HASH_C_
#define _JENKINS_HASH_C_

#include <nfp.h>
#include <stdint.h>
#include <nfp/me.h>

#include "jenkins_hash_c.h"



/*
-------------------------------------------------------------------------------
mix -- mix 3 32-bit values reversibly.

This is reversible, so any information in (a,b,c) before mix() is
still in (a,b,c) after mix().

If four pairs of (a,b,c) inputs are run through mix(), or through
mix() in reverse, there are at least 32 bits of the output that
are sometimes the same for one pair and different for another pair.
This was tested for:
* pairs that differed by one bit, by two bits, in any combination
  of top bits of (a,b,c), or in any combination of bottom bits of
  (a,b,c).
* "differ" is defined as +, -, ^, or ~^.  For + and -, I transformed
  the output delta to a Gray code (a^(a>>1)) so a string of 1's (as
  is commonly produced by subtraction) look like a single 1-bit
  difference.
* the base values were pseudorandom, all zero but one bit set, or
  all zero plus a counter that starts at zero.

Some k values for my "a-=c; a^=rot(c,k); c+=b;" arrangement that
satisfy this are
    4  6  8 16 19  4
    9 15  3 18 27 15
   14  9  3  7 17  3
Well, "9 15 3 18 27 15" didn't quite get 32 bits diffing
for "differ" defined as + with a one-bit base and a two-bit delta.  I
used http://burtleburtle.net/bob/hash/avalanche.html to choose
the operations, constants, and arrangements of the variables.

This does not achieve avalanche.  There are input bits of (a,b,c)
that fail to affect some output bits of (a,b,c), especially of a.  The
most thoroughly mixed value is c, but it doesn't really even achieve
avalanche in c.

This allows some parallelism.  Read-after-writes are good at doubling
the number of bits affected, so the goal of mixing pulls in the opposite
direction as the goal of parallelism.  I did what I could.  Rotates
seem to cost as much as shifts on every machine I could lay my hands
on, and rotates are much kinder to the top and bottom bits, so I used
rotates.
-------------------------------------------------------------------------------
*/
#define mix(a,b,c) \
{ \
  a -= c;  a ^= rotl32(c, 4);  c += b; \
  b -= a;  b ^= rotl32(a, 6);  a += c; \
  c -= b;  c ^= rotl32(b, 8);  b += a; \
  a -= c;  a ^= rotl32(c,16);  c += b; \
  b -= a;  b ^= rotl32(a,19);  a += c; \
  c -= b;  c ^= rotl32(b, 4);  b += a; \
}

/*
-------------------------------------------------------------------------------
final -- final mixing of 3 32-bit values (a,b,c) into c

Pairs of (a,b,c) values differing in only a few bits will usually
produce values of c that look totally different.  This was tested for
* pairs that differed by one bit, by two bits, in any combination
  of top bits of (a,b,c), or in any combination of bottom bits of
  (a,b,c).
* "differ" is defined as +, -, ^, or ~^.  For + and -, I transformed
  the output delta to a Gray code (a^(a>>1)) so a string of 1's (as
  is commonly produced by subtraction) look like a single 1-bit
  difference.
* the base values were pseudorandom, all zero but one bit set, or
  all zero plus a counter that starts at zero.

These constants passed:
 14 11 25 16 4 14 24
 12 14 25 16 4 14 24
and these came close:
  4  8 15 26 3 22 24
 10  8 15 26 3 22 24
 11  8 15 26 3 22 24
-------------------------------------------------------------------------------
*/
#define final(a,b,c) \
{ \
  c ^= b; c -= rotl32(b,14); \
  a ^= c; a -= rotl32(c,11); \
  b ^= a; b -= rotl32(a,25); \
  c ^= b; c -= rotl32(b,16); \
  a ^= c; a -= rotl32(c,4);  \
  b ^= a; b -= rotl32(a,14); \
  c ^= b; c -= rotl32(b,24); \
}

#define HASH_JENKINS_SEED_DEFAULT_0 0x12345678
#define HASH_JENKINS_SEED_DEFAULT_1 0x9abcdef0



 __declspec(gp_reg) hash_result_t hash_jenkins(__declspec(local_mem) unsigned int *in_key_index, __declspec(gp_reg) unsigned int in_key_length)
{
    __declspec(gp_reg) unsigned int     a,b,c;
    __declspec(gp_reg) hash_result_t    out_hash;

    /* Set up the internal state */
    /*
     * Use the same values as in the microCode implementation
     */
    a = b = c = 0xdeadbeaf + in_key_length + HASH_JENKINS_SEED_DEFAULT_0;
    c += HASH_JENKINS_SEED_DEFAULT_1;


    /*------------------------------------------------- handle most of the key */
    while (in_key_length > 3)
    {
        a += in_key_index[0];
        b += in_key_index[1];
        c += in_key_index[2];
        mix(a,b,c);
        in_key_length -= 3;
        in_key_index += 3;
    }

    /*------------------------------------------- handle the last 3 32-bit words */
    switch(in_key_length)                     /* all the case statements fall through */
    {
        case 3 : c+=in_key_index[2];
        case 2 : b+=in_key_index[1];
        case 1 : a+=in_key_index[0];
        final(a,b,c);
        case 0:     /* case 0: nothing left to add */
        break;
    }

    /*------------------------------------------------------ report the result */
    out_hash.value_0 = b;
    out_hash.value_1 = c;

    return out_hash;
}





#endif  // _JENKINS_HASH_C_