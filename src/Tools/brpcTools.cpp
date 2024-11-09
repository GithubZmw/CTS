#include "brpcTools.h"

gmp_randstate_t state;

std::string mpz_to_str(const mpz_class &value) {
    return value.get_str(16);
}

mpz_class str_to_mpz(const string &str) {
    return mpz_class(str, 16);
}

std::string ECP_to_str(ECP ecp) {
    BIG x, y;// 每个坐标的长度是96个字节
    ECP_get(x, y, &ecp);
    return mpz_to_str(BIG_to_mpz(x)) + "," + mpz_to_str(BIG_to_mpz(y));
}

ECP str_to_ECP(const std::string &str) {
    ECP ecp;
    int pos = str.find(",");
    BIG x, y;
    str_to_BIG(str.substr(0, pos), x);
    str_to_BIG(str.substr(pos + 1), y);
    ECP_set(&ecp, x, y);
    return ecp;
}

std::string ECP2_to_str(ECP2 ecp2, bool compressed) {
    char buffer[2 * 48 * 2];
    octet S;
    S.val = buffer;
    S.max = sizeof(buffer);
    S.len = 0;
    ECP2_toOctet(&S, const_cast<ECP2 *>(&ecp2), compressed);
    std::string hex_string;
    for (int i = 0; i < S.len; i++) {
        char hex[3];
        sprintf(hex, "%02X", (unsigned char) S.val[i]);
        hex_string.append(hex);
    }
    return hex_string;
}

ECP2 str_to_ECP2(const std::string &hex_string) {
    ECP2 ecp2;
    size_t len = hex_string.length() / 2;
    char buffer[len];
    for (size_t i = 0; i < len; i++) {
        sscanf(hex_string.substr(i * 2, 2).c_str(), "%2hhX", &buffer[i]);
    }
    octet S;
    S.val = buffer;
    S.max = len;
    S.len = len;
    if (ECP2_fromOctet(&ecp2, &S) != 1) {
        std::cerr << "Invalid ECP2 point representation." << std::endl;
    }
    return ecp2;
}


std::string FP12_to_str(FP12 fp12) {
    char buffer[24 * 48];
    octet S;
    S.val = buffer;
    S.max = sizeof(buffer);
    S.len = 0;
    FP12_toOctet(&S, const_cast<FP12 *>(&fp12));
    std::string hex_string;
    for (int i = 0; i < S.len; i++) {
        char hex[3];
        sprintf(hex, "%02X", (unsigned char) S.val[i]);
        hex_string.append(hex);
    }
    return hex_string;
}

FP12 str_to_FP12(const std::string &hex_string) {
    FP12 fp12;
    size_t len = hex_string.length() / 2;
    char buffer[len];
    for (size_t i = 0; i < len; i++) {
        sscanf(hex_string.substr(i * 2, 2).c_str(), "%2hhX", &buffer[i]);
    }
    octet S;
    S.val = buffer;
    S.max = len;
    S.len = len;
    FP12_fromOctet(&fp12, &S);
    return fp12;
}

std::string mpzArr_to_str(const std::vector<mpz_class> &mpzs) {
    string str;
    for (int i = 0; i < mpzs.size(); i++) {
        str += mpzs[i].get_str() + ",";
    }
    return str;
}

std::vector<mpz_class> str_to_mpzArr(const std::string &str) {
    vector<mpz_class> mpzs;
    stringstream ss(str);
    string item;
    while (getline(ss, item, ',')) {
        mpzs.push_back(mpz_class(item));
    }
    return mpzs;
}

std::string ECPArr_to_str(const std::vector<ECP> &ecps) {
    std::ostringstream oss;
    for (size_t i = 0; i < ecps.size(); ++i) {
        if (i != 0) oss << ";";
        oss << ECP_to_str(ecps[i]);
    }
    return oss.str();
}

std::vector<ECP> str_to_ECPArr(const std::string &str) {
    std::vector<ECP> ecps;
    size_t start = 0;
    size_t end = str.find(';');
    while (end != std::string::npos) {
        ecps.emplace_back(str_to_ECP(str.substr(start, end - start))); // 直接构造 ECP
        start = end + 1;
        end = str.find(';', start);
    }
    if (start < str.size()) {
        ecps.emplace_back(str_to_ECP(str.substr(start)));
    }
    return ecps;
}


std::string ECP2Arr_to_str(const std::vector<ECP2> &ecp2s, bool compressed) {
    std::ostringstream oss;
    for (size_t i = 0; i < ecp2s.size(); ++i) {
        if (i != 0) oss << ";";
        oss << ECP2_to_str(ecp2s[i], compressed);
    }
    return oss.str();
}

vector<ECP2> str_to_ECP2Arr(const std::string &str) {
    std::vector<ECP2> ecp2s;
    size_t start = 0;
    size_t end = str.find(';');
    while (end != std::string::npos) {
        ecp2s.emplace_back(str_to_ECP2(str.substr(start, end - start))); // 直接构造 ECP
        start = end + 1;
        end = str.find(';', start);
    }
    if (start < str.size()) {
        ecp2s.emplace_back(str_to_ECP2(str.substr(start)));
    }
    return ecp2s;
}
