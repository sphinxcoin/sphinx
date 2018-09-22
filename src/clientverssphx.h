// Copyright (c) 2009-2018 The Bitcoin Core developers
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#ifndef BITCOIN_CLIENTVERSSPHX_H
#define BITCOIN_CLIENTVERSSPHX_H

#if defined(HAVE_CONFIG_H)
#include "config/sphx-config.h"
#endif //HAVE_CONFIG_H

// Check that required client information is defined
#if !defined(CLIENT_VERSSPHX_MAJOR) || !defined(CLIENT_VERSSPHX_MINOR) || !defined(CLIENT_VERSSPHX_REVISSPHX) || !defined(CLIENT_VERSSPHX_BUILD) || !defined(CLIENT_VERSSPHX_IS_RELEASE) || !defined(COPYRIGHT_YEAR)
#error Client verssphx information missing: verssphx is not defined by sphx-config.h or in any other way
#endif

/**
 * Converts the parameter X to a string after macro replacement on X has been performed.
 * Don't merge these into one macro!
 */
#define STRINGIZE(X) DO_STRINGIZE(X)
#define DO_STRINGIZE(X) #X

//! Copyright string used in Windows .rc files
#define COPYRIGHT_STR "2009-" STRINGIZE(COPYRIGHT_YEAR) " The Bitcoin Core Developers, 2014-" STRINGIZE(COPYRIGHT_YEAR) " The Dash Core Developers, 2015-" STRINGIZE(COPYRIGHT_YEAR) " The PIVX Core Developers" STRINGIZE(COPYRIGHT_YEAR) " The Sphinx Core Developers"

/**
 * sphxd-res.rc includes this file, but it cannot cope with real c++ code.
 * WINDRES_PREPROC is defined to indicate that its pre-processor is running.
 * Anything other than a define should be guarded below.
 */

#if !defined(WINDRES_PREPROC)

#include <string>
#include <vector>

static const int CLIENT_VERSSPHX =
    1000000 * CLIENT_VERSSPHX_MAJOR  ///
    + 10000 * CLIENT_VERSSPHX_MINOR  ///
    + 100 * CLIENT_VERSSPHX_REVISSPHX ///
    + 1 * CLIENT_VERSSPHX_BUILD;

extern const std::string CLIENT_NAME;
extern const std::string CLIENT_BUILD;
extern const std::string CLIENT_DATE;


std::string FormatFullVerssphx();
std::string FormatSubVerssphx(const std::string& name, int nClientVerssphx, const std::vector<std::string>& comments);

#endif // WINDRES_PREPROC

#endif // BITCOIN_CLIENTVERSSPHX_H
