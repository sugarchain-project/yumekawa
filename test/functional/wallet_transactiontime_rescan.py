#!/usr/bin/env python3
# Copyright (c) 2018-2019 The Bitcoin Core developers
# Distributed under the MIT software license, see the accompanying
# file COPYING or http://www.opensource.org/licenses/mit-license.php.
"""Test transaction time during old block rescanning
"""

from test_framework.test_framework import BitcoinTestFramework
from test_framework.util import (
    assert_equal,
    assert_raises_rpc_error
)


class TransactionTimeRescanTest(BitcoinTestFramework):
    def set_test_params(self):
        self.setup_clean_chain = False
        self.num_nodes = 3

    def skip_test_if_missing_module(self):
        self.skip_if_no_wallet()

    def run_test(self):
        self.log.info('Prepare nodes and wallet')

        minernode = self.nodes[0] # node used to mine BTC and create transactions
        usernode = self.nodes[1] # user node with correct time
        restorenode = self.nodes[2] # node used to restore user wallet and check time determination in ComputeSmartTime (wallet.cpp)

        # time constant
        mocktime = 1525107225
        ten_days = 10 * 24 * 60 * 60

        # synchronize nodes and time
        self.sync_all()
        minernode.setmocktime(mocktime)
        usernode.setmocktime(mocktime)
        restorenode.setmocktime(mocktime)

        # prepare miner wallet
        minernode.createwallet(wallet_name='default')
        miner_wallet = minernode.get_wallet_rpc('default')
        m1 = miner_wallet.getnewaddress()

        # prepare the user wallet with 3 public key
        wo1 = 'bcrt1qqvs4h9g5kemwu8zdffv3c9tm3dm2qp9uzlxwzx'
        wo2 = 'bcrt1qnw7peeq89hwrg674zjf4phzzluld798p667q7q'
        wo3 = 'bcrt1qdr36v724uddf50jhnju3amzap2gmmt9tf4aac5'

        usernode.createwallet(wallet_name='wo', disable_private_keys=True)
        wo_wallet = usernode.get_wallet_rpc('wo')

        wo_wallet.importpubkey(pubkey='02f8802962c2ca756d8961c719f75bdfff2b6dbb775788ed54a68bcb182c0ea1ef')
        wo_wallet.importpubkey(pubkey='0330f4a96800a38fa0a49cde4b5f802e95a0c1044e5954389a8d00cecde01b842b')
        wo_wallet.importpubkey(pubkey='02f5ccbee49546e0b5bf2e76422ed959e983dafaa94c8abafc9c8f8b99ef23d16a')

        self.log.info('Start transactions')
        
        # check blockcount
        assert_equal(minernode.getblockcount(), 200)

        # generate some btc to create transactions and check blockcount
        minernode.generatetoaddress(300, m1)
        assert_equal(minernode.getblockcount(), 500)

        # synchronize nodes and time
        self.sync_all()
        minernode.setmocktime(mocktime + ten_days)
        usernode.setmocktime(mocktime + ten_days)
        restorenode.setmocktime(mocktime + ten_days)
        # send 10 btc to user first watch-only address
        self.log.info('Send 10 btc to user')
        txid1 = miner_wallet.sendtoaddress(wo1, 10)
        
        # generate blocks and check blockcount
        minernode.generatetoaddress(100, m1)
        assert_equal(minernode.getblockcount(), 600)

        # synchronize nodes and time
        self.sync_all()
        minernode.setmocktime(mocktime + ten_days + ten_days)
        usernode.setmocktime(mocktime + ten_days + ten_days)
        restorenode.setmocktime(mocktime + ten_days + ten_days)
        # send 5 btc to our second watch-only address
        self.log.info('Send 5 btc to user')
        txid2 = miner_wallet.sendtoaddress(wo2, 5)
        
        # generate blocks and check blockcount
        minernode.generatetoaddress(100, m1)
        assert_equal(minernode.getblockcount(), 700)

        # synchronize nodes and time
        self.sync_all()
        minernode.setmocktime(mocktime + ten_days + ten_days + ten_days)
        usernode.setmocktime(mocktime + ten_days + ten_days + ten_days)
        restorenode.setmocktime(mocktime + ten_days + ten_days + ten_days)
        # send 1 btc to our third watch-only address
        self.log.info('Send 1 btc to user')
        txid3 = miner_wallet.sendtoaddress(wo3, 1)
        
        # generate more blocks and check blockcount
        minernode.generatetoaddress(100, m1)
        assert_equal(minernode.getblockcount(), 800)

        # check user final balance and transaction count
        self.log.info('Check user final balance')
        assert_equal(wo_wallet.getbalance(), 16)
        assert_equal(len(wo_wallet.listtransactions()), 3)

        # check transactions time
        self.log.info('Check transactions time')
        for tx in wo_wallet.listtransactions():
            if tx['address'] == 'bcrt1qqvs4h9g5kemwu8zdffv3c9tm3dm2qp9uzlxwzx':
                assert_equal(tx['blocktime'], 1525971225)
                assert_equal(tx['time'], 1525971225)
            elif tx['address'] == 'bcrt1qnw7peeq89hwrg674zjf4phzzluld798p667q7q':
                assert_equal(tx['blocktime'], 1526835225)
                assert_equal(tx['time'], 1526835225)
            elif tx['address'] == 'bcrt1qdr36v724uddf50jhnju3amzap2gmmt9tf4aac5':
                assert_equal(tx['blocktime'], 1527699225)
                assert_equal(tx['time'], 1527699225)

        # restore user wallet without rescan
        self.log.info('Restore user wallet on another node without rescan')
        restorenode.createwallet(wallet_name='wo', disable_private_keys=True, )
        restorewo_wallet = restorenode.get_wallet_rpc('wo')

        restorewo_wallet.importpubkey(pubkey='02f8802962c2ca756d8961c719f75bdfff2b6dbb775788ed54a68bcb182c0ea1ef', rescan=False)
        restorewo_wallet.importpubkey(pubkey='0330f4a96800a38fa0a49cde4b5f802e95a0c1044e5954389a8d00cecde01b842b', rescan=False)
        restorewo_wallet.importpubkey(pubkey='02f5ccbee49546e0b5bf2e76422ed959e983dafaa94c8abafc9c8f8b99ef23d16a', rescan=False)

        # check user has 0 balance and no transaction
        assert_equal(restorewo_wallet.getbalance(), 0)
        assert_equal(len(restorewo_wallet.listtransactions()), 0)

        # proceed to rescan, first with an incomplete one, then with a full rescan
        self.log.info('Rescan last history part')
        restorewo_wallet.rescanblockchain(650)
        self.log.info('Rescan all history')
        restorewo_wallet.rescanblockchain()

        # check user final balance and transaction count
        self.log.info('Check user final balance after restoration')
        assert_equal(restorewo_wallet.getbalance(), 16)
        assert_equal(len(restorewo_wallet.listtransactions()), 3)

        # check transactions time
        self.log.info('Check transactions time after restoration')
        for tx in restorewo_wallet.listtransactions():
            if tx['address'] == 'bcrt1qqvs4h9g5kemwu8zdffv3c9tm3dm2qp9uzlxwzx':
                assert_equal(tx['blocktime'], 1525971225)
                assert_equal(tx['time'], 1525971225)
            elif tx['address'] == 'bcrt1qnw7peeq89hwrg674zjf4phzzluld798p667q7q':
                assert_equal(tx['blocktime'], 1526835225)
                assert_equal(tx['time'], 1526835225)
            elif tx['address'] == 'bcrt1qdr36v724uddf50jhnju3amzap2gmmt9tf4aac5':
                assert_equal(tx['blocktime'], 1527699225)
                assert_equal(tx['time'], 1527699225)

if __name__ == '__main__':
    TransactionTimeRescanTest().main()
