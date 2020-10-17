// Copyright (c) 2016-2020 The Bitcoin Core developers
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#include <bench/bench.h>
#include <bench/data.h>

#include <rpc/blockchain.h>
#include <streams.h>
#include <validation.h>

#include <univalue.h>

static void BlockToJsonVerbose(benchmark::Bench& bench)
{
    CDataStream stream(benchmark::data::block6513497, SER_NETWORK, PROTOCOL_VERSION);
    char a = '\0';
    stream.write(&a, 1); // Prevent compaction

    CBlock block;
    stream >> block;

    CBlockIndex blockindex;
    const uint256 blockHash = block.GetHash();
    blockindex.phashBlock = &blockHash;
    blockindex.nBits = 503362144; // TODO.ZENY.YESPOWER // block6513497 // >>> print int("0x1e00b260", 0)

    bench.run([&] {
        (void)blockToJSON(block, &blockindex, &blockindex, /*verbose*/ true);
    });
}

BENCHMARK(BlockToJsonVerbose);
