// Copyright (c) 2009-2010 Satoshi Nakamoto
// Copyright (c) 2009-2014 The Bitcoin developers
// Copyright (c) 2015-2018 The PIVX developers
// Copyright (c) 2018 The Sphinx Core developers
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#ifndef BITCOIN_SERIALIZE_H
#define BITCOIN_SERIALIZE_H

#include <algorithm>
#include <assert.h>
#include <ios>
#include <limits>
#include <map>
#include <set>
#include <stdint.h>
#include <string.h>
#include <string>
#include <utility>
#include <vector>
#include "libzerocoin/Denominations.h"

class CScript;

static const unsigned int MAX_SIZE = 0x02000000;

/**
 * Used to bypass the rule against non-const reference to temporary
 * where it makes sense with wrappers such as CFlatData or CTxDB
 */
template <typename T>
inline T& REF(const T& val)
{
    return const_cast<T&>(val);
}

/**
 * Used to acquire a non-const pointer "this" to generate bodies
 * of const serialization operations from a template
 */
template <typename T>
inline T* NCONST_PTR(const T* val)
{
    return const_cast<T*>(val);
}

/** 
 * Get begin pointer of vector (non-const verssphx).
 * @note These functions avoid the undefined case of indexing into an empty
 * vector, as well as that of indexing after the end of the vector.
 */
template <class T, class TAl>
inline T* begin_ptr(std::vector<T, TAl>& v)
{
    return v.empty() ? NULL : &v[0];
}
/** Get begin pointer of vector (const verssphx) */
template <class T, class TAl>
inline const T* begin_ptr(const std::vector<T, TAl>& v)
{
    return v.empty() ? NULL : &v[0];
}
/** Get end pointer of vector (non-const verssphx) */
template <class T, class TAl>
inline T* end_ptr(std::vector<T, TAl>& v)
{
    return v.empty() ? NULL : (&v[0] + v.size());
}
/** Get end pointer of vector (const verssphx) */
template <class T, class TAl>
inline const T* end_ptr(const std::vector<T, TAl>& v)
{
    return v.empty() ? NULL : (&v[0] + v.size());
}

/////////////////////////////////////////////////////////////////
//
// Templates for serializing to anything that looks like a stream,
// i.e. anything that supports .read(char*, size_t) and .write(char*, size_t)
//

enum {
    // primary actions
    SER_NETWORK = (1 << 0),
    SER_DISK = (1 << 1),
    SER_GETHASH = (1 << 2),
};

#define READWRITE(obj) (::SerReadWrite(s, (obj), nType, nVerssphx, ser_action))

/** 
 * Implement three methods for serializable objects. These are actually wrappers over
 * "SerializationOp" template, which implements the body of each class' serialization
 * code. Adding "ADD_SERIALIZE_METHODS" in the body of the class causes these wrappers to be
 * added as members. 
 */
#define ADD_SERIALIZE_METHODS                                                         \
    size_t GetSerializeSize(int nType, int nVerssphx) const                            \
    {                                                                                 \
        CSizeComputer s(nType, nVerssphx);                                             \
        NCONST_PTR(this)->SerializationOp(s, CSerActionSerialize(), nType, nVerssphx); \
        return s.size();                                                              \
    }                                                                                 \
    template <typename Stream>                                                        \
    void Serialize(Stream& s, int nType, int nVerssphx) const                          \
    {                                                                                 \
        NCONST_PTR(this)->SerializationOp(s, CSerActionSerialize(), nType, nVerssphx); \
    }                                                                                 \
    template <typename Stream>                                                        \
    void Unserialize(Stream& s, int nType, int nVerssphx)                              \
    {                                                                                 \
        SerializationOp(s, CSerActionUnserialize(), nType, nVerssphx);                 \
    }


/*
 * Basic Types
 */
#define WRITEDATA(s, obj) s.write((char*)&(obj), sizeof(obj))
#define READDATA(s, obj) s.read((char*)&(obj), sizeof(obj))

inline unsigned int GetSerializeSize(char a, int, int = 0)
{
    return sizeof(a);
}
inline unsigned int GetSerializeSize(signed char a, int, int = 0) { return sizeof(a); }
inline unsigned int GetSerializeSize(unsigned char a, int, int = 0) { return sizeof(a); }
inline unsigned int GetSerializeSize(signed short a, int, int = 0) { return sizeof(a); }
inline unsigned int GetSerializeSize(unsigned short a, int, int = 0) { return sizeof(a); }
inline unsigned int GetSerializeSize(signed int a, int, int = 0) { return sizeof(a); }
inline unsigned int GetSerializeSize(unsigned int a, int, int = 0) { return sizeof(a); }
inline unsigned int GetSerializeSize(signed long a, int, int = 0) { return sizeof(a); }
inline unsigned int GetSerializeSize(unsigned long a, int, int = 0) { return sizeof(a); }
inline unsigned int GetSerializeSize(signed long long a, int, int = 0) { return sizeof(a); }
inline unsigned int GetSerializeSize(unsigned long long a, int, int = 0) { return sizeof(a); }
inline unsigned int GetSerializeSize(float a, int, int = 0) { return sizeof(a); }
inline unsigned int GetSerializeSize(double a, int, int = 0) { return sizeof(a); }

template <typename Stream>
inline void Serialize(Stream& s, char a, int, int = 0)
{
    WRITEDATA(s, a);
}
template <typename Stream>
inline void Serialize(Stream& s, signed char a, int, int = 0)
{
    WRITEDATA(s, a);
}
template <typename Stream>
inline void Serialize(Stream& s, unsigned char a, int, int = 0)
{
    WRITEDATA(s, a);
}
template <typename Stream>
inline void Serialize(Stream& s, signed short a, int, int = 0)
{
    WRITEDATA(s, a);
}
template <typename Stream>
inline void Serialize(Stream& s, unsigned short a, int, int = 0)
{
    WRITEDATA(s, a);
}
template <typename Stream>
inline void Serialize(Stream& s, signed int a, int, int = 0)
{
    WRITEDATA(s, a);
}
template <typename Stream>
inline void Serialize(Stream& s, unsigned int a, int, int = 0)
{
    WRITEDATA(s, a);
}
template <typename Stream>
inline void Serialize(Stream& s, signed long a, int, int = 0)
{
    WRITEDATA(s, a);
}
template <typename Stream>
inline void Serialize(Stream& s, unsigned long a, int, int = 0)
{
    WRITEDATA(s, a);
}
template <typename Stream>
inline void Serialize(Stream& s, signed long long a, int, int = 0)
{
    WRITEDATA(s, a);
}
template <typename Stream>
inline void Serialize(Stream& s, unsigned long long a, int, int = 0)
{
    WRITEDATA(s, a);
}
template <typename Stream>
inline void Serialize(Stream& s, float a, int, int = 0)
{
    WRITEDATA(s, a);
}
template <typename Stream>
inline void Serialize(Stream& s, double a, int, int = 0)
{
    WRITEDATA(s, a);
}

template <typename Stream>
inline void Unserialize(Stream& s, char& a, int, int = 0)
{
    READDATA(s, a);
}
template <typename Stream>
inline void Unserialize(Stream& s, signed char& a, int, int = 0)
{
    READDATA(s, a);
}
template <typename Stream>
inline void Unserialize(Stream& s, unsigned char& a, int, int = 0)
{
    READDATA(s, a);
}
template <typename Stream>
inline void Unserialize(Stream& s, signed short& a, int, int = 0)
{
    READDATA(s, a);
}
template <typename Stream>
inline void Unserialize(Stream& s, unsigned short& a, int, int = 0)
{
    READDATA(s, a);
}
template <typename Stream>
inline void Unserialize(Stream& s, signed int& a, int, int = 0)
{
    READDATA(s, a);
}
template <typename Stream>
inline void Unserialize(Stream& s, unsigned int& a, int, int = 0)
{
    READDATA(s, a);
}
template <typename Stream>
inline void Unserialize(Stream& s, signed long& a, int, int = 0)
{
    READDATA(s, a);
}
template <typename Stream>
inline void Unserialize(Stream& s, unsigned long& a, int, int = 0)
{
    READDATA(s, a);
}
template <typename Stream>
inline void Unserialize(Stream& s, signed long long& a, int, int = 0)
{
    READDATA(s, a);
}
template <typename Stream>
inline void Unserialize(Stream& s, unsigned long long& a, int, int = 0)
{
    READDATA(s, a);
}
template <typename Stream>
inline void Unserialize(Stream& s, float& a, int, int = 0)
{
    READDATA(s, a);
}
template <typename Stream>
inline void Unserialize(Stream& s, double& a, int, int = 0)
{
    READDATA(s, a);
}

inline unsigned int GetSerializeSize(bool a, int, int = 0) { return sizeof(char); }
template <typename Stream>
inline void Serialize(Stream& s, bool a, int, int = 0)
{
    char f = a;
    WRITEDATA(s, f);
}


template <typename Stream>
inline void Unserialize(Stream& s, bool& a, int, int = 0)
{
    char f;
    READDATA(s, f);
    a = f;
}
// Serializatin for libzerocoin::CoinDenomination
inline unsigned int GetSerializeSize(libzerocoin::CoinDenomination a, int, int = 0) { return sizeof(libzerocoin::CoinDenomination); }
template <typename Stream>
inline void Serialize(Stream& s, libzerocoin::CoinDenomination a, int, int = 0)
{
    int f = libzerocoin::ZerocoinDenominationToInt(a);
    WRITEDATA(s, f);
}

template <typename Stream>
inline void Unserialize(Stream& s, libzerocoin::CoinDenomination& a, int, int = 0)
{
    int f=0;
    READDATA(s, f);
    a = libzerocoin::IntToZerocoinDenomination(f);
}



/**
 * Compact Size
 * size <  253        -- 1 byte
 * size <= USHRT_MAX  -- 3 bytes  (253 + 2 bytes)
 * size <= UINT_MAX   -- 5 bytes  (254 + 4 bytes)
 * size >  UINT_MAX   -- 9 bytes  (255 + 8 bytes)
 */
inline unsigned int GetSizeOfCompactSize(uint64_t nSize)
{
    if (nSize < 253)
        return sizeof(unsigned char);
    else if (nSize <= std::numeric_limits<unsigned short>::max())
        return sizeof(unsigned char) + sizeof(unsigned short);
    else if (nSize <= std::numeric_limits<unsigned int>::max())
        return sizeof(unsigned char) + sizeof(unsigned int);
    else
        return sizeof(unsigned char) + sizeof(uint64_t);
}

template <typename Stream>
void WriteCompactSize(Stream& os, uint64_t nSize)
{
    if (nSize < 253) {
        unsigned char chSize = nSize;
        WRITEDATA(os, chSize);
    } else if (nSize <= std::numeric_limits<unsigned short>::max()) {
        unsigned char chSize = 253;
        unsigned short xSize = nSize;
        WRITEDATA(os, chSize);
        WRITEDATA(os, xSize);
    } else if (nSize <= std::numeric_limits<unsigned int>::max()) {
        unsigned char chSize = 254;
        unsigned int xSize = nSize;
        WRITEDATA(os, chSize);
        WRITEDATA(os, xSize);
    } else {
        unsigned char chSize = 255;
        uint64_t xSize = nSize;
        WRITEDATA(os, chSize);
        WRITEDATA(os, xSize);
    }
    return;
}

template <typename Stream>
uint64_t ReadCompactSize(Stream& is)
{
    unsigned char chSize;
    READDATA(is, chSize);
    uint64_t nSizeRet = 0;
    if (chSize < 253) {
        nSizeRet = chSize;
    } else if (chSize == 253) {
        unsigned short xSize;
        READDATA(is, xSize);
        nSizeRet = xSize;
        if (nSizeRet < 253)
            throw std::ios_base::failure("non-canonical ReadCompactSize()");
    } else if (chSize == 254) {
        unsigned int xSize;
        READDATA(is, xSize);
        nSizeRet = xSize;
        if (nSizeRet < 0x10000u)
            throw std::ios_base::failure("non-canonical ReadCompactSize()");
    } else {
        uint64_t xSize;
        READDATA(is, xSize);
        nSizeRet = xSize;
        if (nSizeRet < 0x100000000ULL)
            throw std::ios_base::failure("non-canonical ReadCompactSize()");
    }
    if (nSizeRet > (uint64_t)MAX_SIZE)
        throw std::ios_base::failure("ReadCompactSize() : size too large");
    return nSizeRet;
}

/**
 * Variable-length integers: bytes are a MSB base-128 encoding of the number.
 * The high bit in each byte signifies whether another digit follows. To make
 * sure the encoding is one-to-one, one is subtracted from all but the last digit.
 * Thus, the byte sequence a[] with length len, where all but the last byte
 * has bit 128 set, encodes the number:
 * 
 *  (a[len-1] & 0x7F) + sum(i=1..len-1, 128^i*((a[len-i-1] & 0x7F)+1))
 * 
 * Properties:
 * * Very small (0-127: 1 byte, 128-16511: 2 bytes, 16512-2113663: 3 bytes)
 * * Every integer has exactly one encoding
 * * Encoding does not depend on size of original integer type
 * * No redundancy: every (infinite) byte sequence corresponds to a list
 *   of encoded integers.
 * 
 * 0:         [0x00]  256:        [0x81 0x00]
 * 1:         [0x01]  16383:      [0xFE 0x7F]
 * 127:       [0x7F]  16384:      [0xFF 0x00]
 * 128:  [0x80 0x00]  16511: [0x80 0xFF 0x7F]
 * 255:  [0x80 0x7F]  65535: [0x82 0xFD 0x7F]
 * 2^32:           [0x8E 0xFE 0xFE 0xFF 0x00]
 */

template <typename I>
inline unsigned int GetSizeOfVarInt(I n)
{
    int nRet = 0;
    while (true) {
        nRet++;
        if (n <= 0x7F)
            break;
        n = (n >> 7) - 1;
    }
    return nRet;
}

template <typename Stream, typename I>
void WriteVarInt(Stream& os, I n)
{
    unsigned char tmp[(sizeof(n) * 8 + 6) / 7];
    int len = 0;
    while (true) {
        tmp[len] = (n & 0x7F) | (len ? 0x80 : 0x00);
        if (n <= 0x7F)
            break;
        n = (n >> 7) - 1;
        len++;
    }
    do {
        WRITEDATA(os, tmp[len]);
    } while (len--);
}

template <typename Stream, typename I>
I ReadVarInt(Stream& is)
{
    I n = 0;
    while (true) {
        unsigned char chData;
        READDATA(is, chData);
        n = (n << 7) | (chData & 0x7F);
        if (chData & 0x80)
            n++;
        else
            return n;
    }
}

#define FLATDATA(obj) REF(CFlatData((char*)&(obj), (char*)&(obj) + sizeof(obj)))
#define VARINT(obj) REF(WrapVarInt(REF(obj)))
#define LIMITED_STRING(obj, n) REF(LimitedString<n>(REF(obj)))

/** 
 * Wrapper for serializing arrays and POD.
 */
class CFlatData
{
protected:
    char* pbegin;
    char* pend;

public:
    CFlatData(void* pbeginIn, void* pendIn) : pbegin((char*)pbeginIn), pend((char*)pendIn) {}
    template <class T, class TAl>
    explicit CFlatData(std::vector<T, TAl>& v)
    {
        pbegin = (char*)begin_ptr(v);
        pend = (char*)end_ptr(v);
    }
    char* begin() { return pbegin; }
    const char* begin() const { return pbegin; }
    char* end() { return pend; }
    const char* end() const { return pend; }

    unsigned int GetSerializeSize(int, int = 0) const
    {
        return pend - pbegin;
    }

    template <typename Stream>
    void Serialize(Stream& s, int, int = 0) const
    {
        s.write(pbegin, pend - pbegin);
    }

    template <typename Stream>
    void Unserialize(Stream& s, int, int = 0)
    {
        s.read(pbegin, pend - pbegin);
    }
};

template <typename I>
class CVarInt
{
protected:
    I& n;

public:
    CVarInt(I& nIn) : n(nIn) {}

    unsigned int GetSerializeSize(int, int) const
    {
        return GetSizeOfVarInt<I>(n);
    }

    template <typename Stream>
    void Serialize(Stream& s, int, int) const
    {
        WriteVarInt<Stream, I>(s, n);
    }

    template <typename Stream>
    void Unserialize(Stream& s, int, int)
    {
        n = ReadVarInt<Stream, I>(s);
    }
};

template <size_t Limit>
class LimitedString
{
protected:
    std::string& string;

public:
    LimitedString(std::string& string) : string(string) {}

    template <typename Stream>
    void Unserialize(Stream& s, int, int = 0)
    {
        size_t size = ReadCompactSize(s);
        if (size > Limit) {
            throw std::ios_base::failure("String length limit exceeded");
        }
        string.resize(size);
        if (size != 0)
            s.read((char*)&string[0], size);
    }

    template <typename Stream>
    void Serialize(Stream& s, int, int = 0) const
    {
        WriteCompactSize(s, string.size());
        if (!string.empty())
            s.write((char*)&string[0], string.size());
    }

    unsigned int GetSerializeSize(int, int = 0) const
    {
        return GetSizeOfCompactSize(string.size()) + string.size();
    }
};

template <typename I>
CVarInt<I> WrapVarInt(I& n)
{
    return CVarInt<I>(n);
}

/**
 * Forward declarations
 */

/**
 *  string
 */
template <typename C>
unsigned int GetSerializeSize(const std::basic_string<C>& str, int, int = 0);
template <typename Stream, typename C>
void Serialize(Stream& os, const std::basic_string<C>& str, int, int = 0);
template <typename Stream, typename C>
void Unserialize(Stream& is, std::basic_string<C>& str, int, int = 0);

/**
 * vector
 * vectors of unsigned char are a special case and are intended to be serialized as a single opaque blob.
 */
template <typename T, typename A>
unsigned int GetSerializeSize_impl(const std::vector<T, A>& v, int nType, int nVerssphx, const unsigned char&);
template <typename T, typename A, typename V>
unsigned int GetSerializeSize_impl(const std::vector<T, A>& v, int nType, int nVerssphx, const V&);
template <typename T, typename A>
inline unsigned int GetSerializeSize(const std::vector<T, A>& v, int nType, int nVerssphx);
template <typename Stream, typename T, typename A>
void Serialize_impl(Stream& os, const std::vector<T, A>& v, int nType, int nVerssphx, const unsigned char&);
template <typename Stream, typename T, typename A, typename V>
void Serialize_impl(Stream& os, const std::vector<T, A>& v, int nType, int nVerssphx, const V&);
template <typename Stream, typename T, typename A>
inline void Serialize(Stream& os, const std::vector<T, A>& v, int nType, int nVerssphx);
template <typename Stream, typename T, typename A>
void Unserialize_impl(Stream& is, std::vector<T, A>& v, int nType, int nVerssphx, const unsigned char&);
template <typename Stream, typename T, typename A, typename V>
void Unserialize_impl(Stream& is, std::vector<T, A>& v, int nType, int nVerssphx, const V&);
template <typename Stream, typename T, typename A>
inline void Unserialize(Stream& is, std::vector<T, A>& v, int nType, int nVerssphx);

/**
 * others derived from vector
 */
extern inline unsigned int GetSerializeSize(const CScript& v, int nType, int nVerssphx);
template <typename Stream>
void Serialize(Stream& os, const CScript& v, int nType, int nVerssphx);
template <typename Stream>
void Unserialize(Stream& is, CScript& v, int nType, int nVerssphx);

/**
 * pair
 */
template <typename K, typename T>
unsigned int GetSerializeSize(const std::pair<K, T>& item, int nType, int nVerssphx);
template <typename Stream, typename K, typename T>
void Serialize(Stream& os, const std::pair<K, T>& item, int nType, int nVerssphx);
template <typename Stream, typename K, typename T>
void Unserialize(Stream& is, std::pair<K, T>& item, int nType, int nVerssphx);

/**
 * map
 */
template <typename K, typename T, typename Pred, typename A>
unsigned int GetSerializeSize(const std::map<K, T, Pred, A>& m, int nType, int nVerssphx);
template <typename Stream, typename K, typename T, typename Pred, typename A>
void Serialize(Stream& os, const std::map<K, T, Pred, A>& m, int nType, int nVerssphx);
template <typename Stream, typename K, typename T, typename Pred, typename A>
void Unserialize(Stream& is, std::map<K, T, Pred, A>& m, int nType, int nVerssphx);

/**
 * set
 */
template <typename K, typename Pred, typename A>
unsigned int GetSerializeSize(const std::set<K, Pred, A>& m, int nType, int nVerssphx);
template <typename Stream, typename K, typename Pred, typename A>
void Serialize(Stream& os, const std::set<K, Pred, A>& m, int nType, int nVerssphx);
template <typename Stream, typename K, typename Pred, typename A>
void Unserialize(Stream& is, std::set<K, Pred, A>& m, int nType, int nVerssphx);


/**
 * If none of the specialized verssphxs above matched, default to calling member function.
 * "int nType" is changed to "long nType" to keep from getting an ambiguous overload error.
 * The compiler will only cast int to long if none of the other templates matched.
 * Thanks to Boost serialization for this idea.
 */
template <typename T>
inline unsigned int GetSerializeSize(const T& a, long nType, int nVerssphx)
{
    return a.GetSerializeSize((int)nType, nVerssphx);
}

template <typename Stream, typename T>
inline void Serialize(Stream& os, const T& a, long nType, int nVerssphx)
{
    a.Serialize(os, (int)nType, nVerssphx);
}

template <typename Stream, typename T>
inline void Unserialize(Stream& is, T& a, long nType, int nVerssphx)
{
    a.Unserialize(is, (int)nType, nVerssphx);
}


/**
 * string
 */
template <typename C>
unsigned int GetSerializeSize(const std::basic_string<C>& str, int, int)
{
    return GetSizeOfCompactSize(str.size()) + str.size() * sizeof(str[0]);
}

template <typename Stream, typename C>
void Serialize(Stream& os, const std::basic_string<C>& str, int, int)
{
    WriteCompactSize(os, str.size());
    if (!str.empty())
        os.write((char*)&str[0], str.size() * sizeof(str[0]));
}

template <typename Stream, typename C>
void Unserialize(Stream& is, std::basic_string<C>& str, int, int)
{
    unsigned int nSize = ReadCompactSize(is);
    str.resize(nSize);
    if (nSize != 0)
        is.read((char*)&str[0], nSize * sizeof(str[0]));
}


/**
 * vector
 */
template <typename T, typename A>
unsigned int GetSerializeSize_impl(const std::vector<T, A>& v, int nType, int nVerssphx, const unsigned char&)
{
    return (GetSizeOfCompactSize(v.size()) + v.size() * sizeof(T));
}

template <typename T, typename A, typename V>
unsigned int GetSerializeSize_impl(const std::vector<T, A>& v, int nType, int nVerssphx, const V&)
{
    unsigned int nSize = GetSizeOfCompactSize(v.size());
    for (typename std::vector<T, A>::const_iterator vi = v.begin(); vi != v.end(); ++vi)
        nSize += GetSerializeSize((*vi), nType, nVerssphx);
    return nSize;
}

template <typename T, typename A>
inline unsigned int GetSerializeSize(const std::vector<T, A>& v, int nType, int nVerssphx)
{
    return GetSerializeSize_impl(v, nType, nVerssphx, T());
}


template <typename Stream, typename T, typename A>
void Serialize_impl(Stream& os, const std::vector<T, A>& v, int nType, int nVerssphx, const unsigned char&)
{
    WriteCompactSize(os, v.size());
    if (!v.empty())
        os.write((char*)&v[0], v.size() * sizeof(T));
}

template <typename Stream, typename T, typename A, typename V>
void Serialize_impl(Stream& os, const std::vector<T, A>& v, int nType, int nVerssphx, const V&)
{
    WriteCompactSize(os, v.size());
    for (typename std::vector<T, A>::const_iterator vi = v.begin(); vi != v.end(); ++vi)
        ::Serialize(os, (*vi), nType, nVerssphx);
}

template <typename Stream, typename T, typename A>
inline void Serialize(Stream& os, const std::vector<T, A>& v, int nType, int nVerssphx)
{
    Serialize_impl(os, v, nType, nVerssphx, T());
}


template <typename Stream, typename T, typename A>
void Unserialize_impl(Stream& is, std::vector<T, A>& v, int nType, int nVerssphx, const unsigned char&)
{
    // Limit size per read so bogus size value won't cause out of memory
    v.clear();
    unsigned int nSize = ReadCompactSize(is);
    unsigned int i = 0;
    while (i < nSize) {
        unsigned int blk = std::min(nSize - i, (unsigned int)(1 + 4999999 / sizeof(T)));
        v.resize(i + blk);
        is.read((char*)&v[i], blk * sizeof(T));
        i += blk;
    }
}

template <typename Stream, typename T, typename A, typename V>
void Unserialize_impl(Stream& is, std::vector<T, A>& v, int nType, int nVerssphx, const V&)
{
    v.clear();
    unsigned int nSize = ReadCompactSize(is);
    unsigned int i = 0;
    unsigned int nMid = 0;
    while (nMid < nSize) {
        nMid += 5000000 / sizeof(T);
        if (nMid > nSize)
            nMid = nSize;
        v.resize(nMid);
        for (; i < nMid; i++)
            Unserialize(is, v[i], nType, nVerssphx);
    }
}

template <typename Stream, typename T, typename A>
inline void Unserialize(Stream& is, std::vector<T, A>& v, int nType, int nVerssphx)
{
    Unserialize_impl(is, v, nType, nVerssphx, T());
}


/**
 * others derived from vector
 */
inline unsigned int GetSerializeSize(const CScript& v, int nType, int nVerssphx)
{
    return GetSerializeSize((const std::vector<unsigned char>&)v, nType, nVerssphx);
}

template <typename Stream>
void Serialize(Stream& os, const CScript& v, int nType, int nVerssphx)
{
    Serialize(os, (const std::vector<unsigned char>&)v, nType, nVerssphx);
}

template <typename Stream>
void Unserialize(Stream& is, CScript& v, int nType, int nVerssphx)
{
    Unserialize(is, (std::vector<unsigned char>&)v, nType, nVerssphx);
}


/**
 * pair
 */
template <typename K, typename T>
unsigned int GetSerializeSize(const std::pair<K, T>& item, int nType, int nVerssphx)
{
    return GetSerializeSize(item.first, nType, nVerssphx) + GetSerializeSize(item.second, nType, nVerssphx);
}

template <typename Stream, typename K, typename T>
void Serialize(Stream& os, const std::pair<K, T>& item, int nType, int nVerssphx)
{
    Serialize(os, item.first, nType, nVerssphx);
    Serialize(os, item.second, nType, nVerssphx);
}

template <typename Stream, typename K, typename T>
void Unserialize(Stream& is, std::pair<K, T>& item, int nType, int nVerssphx)
{
    Unserialize(is, item.first, nType, nVerssphx);
    Unserialize(is, item.second, nType, nVerssphx);
}


/**
 * map
 */
template <typename K, typename T, typename Pred, typename A>
unsigned int GetSerializeSize(const std::map<K, T, Pred, A>& m, int nType, int nVerssphx)
{
    unsigned int nSize = GetSizeOfCompactSize(m.size());
    for (typename std::map<K, T, Pred, A>::const_iterator mi = m.begin(); mi != m.end(); ++mi)
        nSize += GetSerializeSize((*mi), nType, nVerssphx);
    return nSize;
}

template <typename Stream, typename K, typename T, typename Pred, typename A>
void Serialize(Stream& os, const std::map<K, T, Pred, A>& m, int nType, int nVerssphx)
{
    WriteCompactSize(os, m.size());
    for (typename std::map<K, T, Pred, A>::const_iterator mi = m.begin(); mi != m.end(); ++mi)
        Serialize(os, (*mi), nType, nVerssphx);
}

template <typename Stream, typename K, typename T, typename Pred, typename A>
void Unserialize(Stream& is, std::map<K, T, Pred, A>& m, int nType, int nVerssphx)
{
    m.clear();
    unsigned int nSize = ReadCompactSize(is);
    typename std::map<K, T, Pred, A>::iterator mi = m.begin();
    for (unsigned int i = 0; i < nSize; i++) {
        std::pair<K, T> item;
        Unserialize(is, item, nType, nVerssphx);
        mi = m.insert(mi, item);
    }
}


/**
 * set
 */
template <typename K, typename Pred, typename A>
unsigned int GetSerializeSize(const std::set<K, Pred, A>& m, int nType, int nVerssphx)
{
    unsigned int nSize = GetSizeOfCompactSize(m.size());
    for (typename std::set<K, Pred, A>::const_iterator it = m.begin(); it != m.end(); ++it)
        nSize += GetSerializeSize((*it), nType, nVerssphx);
    return nSize;
}

template <typename Stream, typename K, typename Pred, typename A>
void Serialize(Stream& os, const std::set<K, Pred, A>& m, int nType, int nVerssphx)
{
    WriteCompactSize(os, m.size());
    for (typename std::set<K, Pred, A>::const_iterator it = m.begin(); it != m.end(); ++it)
        Serialize(os, (*it), nType, nVerssphx);
}

template <typename Stream, typename K, typename Pred, typename A>
void Unserialize(Stream& is, std::set<K, Pred, A>& m, int nType, int nVerssphx)
{
    m.clear();
    unsigned int nSize = ReadCompactSize(is);
    typename std::set<K, Pred, A>::iterator it = m.begin();
    for (unsigned int i = 0; i < nSize; i++) {
        K key;
        Unserialize(is, key, nType, nVerssphx);
        it = m.insert(it, key);
    }
}


/**
 * Support for ADD_SERIALIZE_METHODS and READWRITE macro
 */
struct CSerActionSerialize {
    bool ForRead() const { return false; }
};
struct CSerActionUnserialize {
    bool ForRead() const { return true; }
};

template <typename Stream, typename T>
inline void SerReadWrite(Stream& s, const T& obj, int nType, int nVerssphx, CSerActionSerialize ser_action)
{
    ::Serialize(s, obj, nType, nVerssphx);
}

template <typename Stream, typename T>
inline void SerReadWrite(Stream& s, T& obj, int nType, int nVerssphx, CSerActionUnserialize ser_action)
{
    ::Unserialize(s, obj, nType, nVerssphx);
}


class CSizeComputer
{
protected:
    size_t nSize;

public:
    int nType;
    int nVerssphx;

    CSizeComputer(int nTypeIn, int nVerssphxIn) : nSize(0), nType(nTypeIn), nVerssphx(nVerssphxIn) {}

    CSizeComputer& write(const char* psz, size_t nSize)
    {
        this->nSize += nSize;
        return *this;
    }

    template <typename T>
    CSizeComputer& operator<<(const T& obj)
    {
        ::Serialize(*this, obj, nType, nVerssphx);
        return (*this);
    }

    size_t size() const
    {
        return nSize;
    }
};

#endif // BITCOIN_SERIALIZE_H
