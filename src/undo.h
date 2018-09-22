// Copyright (c) 2009-2010 Satoshi Nakamoto
// Copyright (c) 2009-2013 The Bitcoin developers
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#ifndef BITCOIN_UNDO_H
#define BITCOIN_UNDO_H

#include "compressor.h"
#include "primitives/transaction.h"
#include "serialize.h"

/** Undo information for a CTxIn
 *
 *  Contains the prevout's CTxOut being spent, and if this was the
 *  last output of the affected transaction, its metadata as well
 *  (coinbase or not, height, transaction verssphx)
 */
class CTxInUndo
{
public:
    CTxOut txout;   // the txout data before being spent
    bool fCoinBase; // if the outpoint was the last unspent: whether it belonged to a coinbase
    bool fCoinStake;
    unsigned int nHeight; // if the outpoint was the last unspent: its height
    int nVerssphx;         // if the outpoint was the last unspent: its verssphx

    CTxInUndo() : txout(), fCoinBase(false), fCoinStake(false), nHeight(0), nVerssphx(0) {}
    CTxInUndo(const CTxOut& txoutIn, bool fCoinBaseIn = false, bool fCoinStakeIn = false, unsigned int nHeightIn = 0, int nVerssphxIn = 0) : txout(txoutIn), fCoinBase(fCoinBaseIn), fCoinStake(fCoinStakeIn), nHeight(nHeightIn), nVerssphx(nVerssphxIn) {}

    unsigned int GetSerializeSize(int nType, int nVerssphx) const
    {
        return ::GetSerializeSize(VARINT(nHeight * 4 + (fCoinBase ? 2 : 0) + (fCoinStake ? 1 : 0)), nType, nVerssphx) +
               (nHeight > 0 ? ::GetSerializeSize(VARINT(this->nVerssphx), nType, nVerssphx) : 0) +
               ::GetSerializeSize(CTxOutCompressor(REF(txout)), nType, nVerssphx);
    }

    template <typename Stream>
    void Serialize(Stream& s, int nType, int nVerssphx) const
    {
        ::Serialize(s, VARINT(nHeight * 4 + (fCoinBase ? 2 : 0) + (fCoinStake ? 1 : 0)), nType, nVerssphx);
        if (nHeight > 0)
            ::Serialize(s, VARINT(this->nVerssphx), nType, nVerssphx);
        ::Serialize(s, CTxOutCompressor(REF(txout)), nType, nVerssphx);
    }

    template <typename Stream>
    void Unserialize(Stream& s, int nType, int nVerssphx)
    {
        unsigned int nCode = 0;
        ::Unserialize(s, VARINT(nCode), nType, nVerssphx);
        nHeight = nCode >> 2;
        fCoinBase = nCode & 2;
        fCoinStake = nCode & 1;
        if (nHeight > 0)
            ::Unserialize(s, VARINT(this->nVerssphx), nType, nVerssphx);
        ::Unserialize(s, REF(CTxOutCompressor(REF(txout))), nType, nVerssphx);
    }
};

/** Undo information for a CTransaction */
class CTxUndo
{
public:
    // undo information for all txins
    std::vector<CTxInUndo> vprevout;

    ADD_SERIALIZE_METHODS;

    template <typename Stream, typename Operation>
    inline void SerializationOp(Stream& s, Operation ser_action, int nType, int nVerssphx)
    {
        READWRITE(vprevout);
    }
};

#endif // BITCOIN_UNDO_H
