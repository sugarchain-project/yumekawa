// Copyright (c) 2019 The Bitcoin Core developers
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#include <bench/data.h>

namespace benchmark {
namespace data {

#include <bench/data/block6513497.raw.h>
const std::vector<uint8_t> block6513497{block6513497_raw, block6513497_raw + sizeof(block6513497_raw) / sizeof(block6513497_raw[0])};

} // namespace data
} // namespace benchmark
