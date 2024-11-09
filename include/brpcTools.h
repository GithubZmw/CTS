#include "Tools.h"

/**
 * Converts an mpz_class to a std::string
 * @param value The mpz_class to be converted
 * @return The converted string
 */
std::string mpz_to_str(const mpz_class& value);

/**
 * Converts a string to mpz_class
 * @param str The string to be converted
 * @return The converted mpz_class
 */
mpz_class str_to_mpz(const std::string& str);

/**
 * Converts an ECP point to a std::string
 * @param ecp The ECP point to be converted
 * @return The converted string
 */
std::string ECP_to_str(ECP ecp);

/**
 * Converts a string to an ECP point
 * @param str The string to be converted
 * @return The converted ECP point
 */
ECP str_to_ECP(const std::string& str);

/**
 * Converts an ECP2 point to a std::string
 * @param ecp2 The ECP2 point to be converted
 * @param compressed Whether to use compressed format
 * @return The converted string
 */
std::string ECP2_to_str(ECP2 ecp2, bool compressed = true);

/**
 * Converts a string to an ECP2 point
 * @param hex_string The string to be converted
 * @return The converted ECP2 point
 */
ECP2 str_to_ECP2(const std::string& hex_string);

/**
 * Converts an FP12 to a std::string
 * @param fp12 The FP12 to be converted
 * @return The converted string
 */
std::string FP12_to_str(FP12 fp12);

/**
 * Converts a string to an FP12
 * @param hex_string The string to be converted
 * @return The converted FP12
 */
FP12 str_to_FP12(const std::string& hex_string);

/**
 * Converts an array of mpz_class to a std::string
 * @param mpzs The array of mpz_class values to be converted
 * @return The converted string
 */
std::string mpzArr_to_str(const std::vector<mpz_class>& mpzs);

/**
 * Converts a string to an array of mpz_class
 * @param str The string to be converted
 * @return The converted array of mpz_class
 */
std::vector<mpz_class> str_to_mpzArr(const std::string& str);

/**
 * Converts an array of ECP points to a std::string
 * @param ecps The array of ECP points to be converted
 * @return The converted string
 */
std::string ECPArr_to_str(const std::vector<ECP>& ecps);

/**
 * Converts a string to an array of ECP points
 * @param str The string to be converted
 * @return The converted array of ECP points
 */
std::vector<ECP> str_to_ECPArr(const std::string& str);

/**
 * Converts an array of ECP2 points to a std::string
 * @param ecp2s The array of ECP2 points to be converted
 * @param compressed Whether to use compressed format
 * @return The converted string
 */
std::string ECP2Arr_to_str(const std::vector<ECP2>& ecp2s, bool compressed = true);

/**
 * Converts a string to an array of ECP2 points
 * @param str The string to be converted
 * @return The converted array of ECP2 points
 */
std::vector<ECP2> str_to_ECP2Arr(const std::string& str);
