#include <iostream>
#include <pair_BLS12381.h>
#include <bls_BLS12381.h>
#include <randapi.h>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <ctime>
#include <vector>
#include <sys/time.h>
#include "gmpxx.h"
#include <sstream>
#include <iomanip>
#include <chrono>
#include <random>

using namespace B384_58;
using namespace BLS12381;
using namespace core;
using namespace std;
using namespace chrono;

/**
 * Initializes the random seed for generating random numbers
 * @param rng Seed to be initialized
 */
void initRNG(csprng *rng);

/**
 * Generates a random 256-bit number in the Fq field, where q is the order of the BLS12381 curve.
 * Note: Run initRNG before calling this function to initialize the seed.
 * @param big The variable to store the generated random number
 * @param rng Seed used for random generation (initialized by initRNG)
 */
void randBig(BIG big, csprng &rng);

/**
 * Generates a random point on the G1 group of the BLS12381 curve
 * @param ecp Output G1 point
 */
ECP randECP(csprng &rng);

/**
 * Generates a random point on the G2 group of the BLS12381 curve
 * @param ecp Output G2 point
 */
ECP2 randECP2(csprng &rng);

/**
 * Converts data of type char* to a string
 * @param ch Data to be converted of type char*
 * @return The converted string
 */
string charsToString(char *ch);

/**
 * Converts a BIG integer to an mpz_class integer
 * @param big BIG integer to be converted
 * @return Converted mpz_class integer
 */
mpz_class BIG_to_mpz(BIG big);

/**
 * Converts an mpz_class integer to a BIG integer
 * @param t mpz_class integer to be converted
 * @param big Output BIG integer
 */
void mpz_to_BIG(const mpz_class& t, BIG& big);

/**
 * Converts a string to a BIG integer
 * @param hex_string The string to be converted
 * @param big Output BIG integer
 */
void str_to_BIG(string hex_string, BIG &big);

/**
 * Elliptic curve multiplication with constant using mpz_class
 * @param P1 Elliptic curve point
 * @param t The multiplier
 */
void ECP_mul(ECP& P1, const mpz_class& t);

/**
 * Elliptic curve multiplication with constant using BIG
 * @param P2 Elliptic curve point
 * @param t The multiplier
 */
void ECP2_mul(ECP2& P2, const mpz_class& t);

/**
 * Initializes the random number generator in GMP
 * @param state Random number generator to be initialized
 */
void initState(gmp_randstate_t& state);

/**
 * Generates a random mpz_class integer
 * @param bits Number of bits for the generated integer
 * @param state Random number generator
 * @return Randomly generated mpz_class integer
 */
mpz_class rand_mpz(gmp_randstate_t state);

/**
 * Computes base^exp % mod, with the result stored in res
 * @param base Base number
 * @param exp Exponent
 * @param mod Modulus
 * @return Result of modular exponentiation
 */
mpz_class pow_mpz(const mpz_class& base, const mpz_class& exp, const mpz_class& mod);

/**
 * Computes the modular multiplicative inverse of a under modulo m, result stored in res
 * @param a Integer to find the inverse of
 * @param m Modulus
 * @return Modular multiplicative inverse, or 0 if it doesn't exist
 */
mpz_class invert_mpz(const mpz_class& a, const mpz_class& m);

/**
 * Outputs a mpz_t integer in hexadecimal format, including a newline
 * @param mpz Integer to view
 */
void show_mpz(mpz_t mpz);

/**
 * Hashes an octet to a 256-bit integer, used by hashToZp256
 * @param num Hash result
 * @param ct Octet to hash
 * @param q Order of the elliptic curve to mod the hash result
 */
void hashZp256(BIG res, octet *ct, BIG q);

/**
 * Hashes a BIG integer to a 256-bit integer
 * @param res Hash result
 * @param beHashed Integer to hash
 * @param q Order of the elliptic curve to mod the hash result
 */
void hashToZp256(BIG res, BIG beHashed, BIG q);

/**
 * Hashes an mpz_class integer to a 256-bit integer
 * @param res Hash result
 * @param beHashed Integer to hash
 * @param q Order of the elliptic curve to mod the hash result
 */
mpz_class hashToZp256(mpz_class beHashed, mpz_class q);

/**
 * Hashes a 256-bit integer to a point on the G1 group
 * @param big Integer to hash
 * @param q Order of the elliptic curve to mod the hash result
 */
ECP hashToPoint(BIG big, BIG q);

/**
 * Hashes a 256-bit integer to a point on the G2 group
 * @param big Integer to hash
 * @param q Order of the elliptic curve to mod the hash result
 */
ECP hashToPoint(mpz_class big, mpz_class q);

/**
 * Bilinear pairing
 * @param alpha1 Element on G1
 * @param alpha2 Element on G2
 * @return Result of bilinear pairing, an element on GT
 */
FP12 e(ECP P1, ECP2 P2);

/**
 * Computes the modular multiplicative inverse of an integer a under modulo m, result stored in res
 * @param res Stores the multiplicative inverse
 * @param a Integer to find the inverse of
 * @param m Modulus
 */
void BIG_inv(BIG &res, const BIG a, const BIG m);

/**
 * Outputs a BIG integer in hexadecimal format, including a newline
 * @param big Integer to view
 */
void showBIG(BIG big);

/**
 * Outputs an FP12 element of the BLS12381 elliptic curve GT in hexadecimal format, including a newline
 * @param fp12 Element on GT to view
 */
void showFP12(FP12 fp12);

/**
 * Outputs a separator line for debugging purposes
 * @param text Text content of the separator line
 */
void printLine(const string& text);
