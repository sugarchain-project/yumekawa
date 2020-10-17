# block6513497.raw

## Get a blockhash (6513497)
```bash
getblock "855f0c66238bc0246c8ca25cf958283fd49b9fb4b217ddeb518e5ea9f5071b9e" 0

000000202aefa253216496936af48300ceb69f23f836e431202de2b7d3f198ae3c7936901b5d18979918d5036320989495993a5860923d9bc11f72f89958e3d6bc42bb81a893515f60b2001e24924a3301010000000001010000000000000000000000000000000000000000000000000000000000000000ffffffff200359636304a893515f0817fff8da000000000d2f6e6f64655374726174756d2f00000000030000000000000000266a24aa21a9ede2f61c3f71d1defd3fa999dfa36953755c690689799962b48bebd836974e8cf9343333f3000000001600140bc5a2983b87f338f4066b95ae4b59ea65528b54cccccc0c00000000160014ea11a82df3eff65676d54ae8750ca728fe5911260120000000000000000000000000000000000000000000000000000000000000000000000000
```

## Make a binary file
```bash
sugarchain-cli getblock "855f0c66238bc0246c8ca25cf958283fd49b9fb4b217ddeb518e5ea9f5071b9e" 0 > ./src/bench/data/block6513497.hex
xxd -r -p ./src/bench/data/block6513497.hex ./src/bench/data/block6513497.raw
```

## nBits `src/bench/rpc_blockchain.cpp`
- BTC `block413567`
```py
bitcoin-cli getblockheader "0000000000000000025aff8be8a55df8f89c77296db6198f272d6577325d4069" | grep bits
"18058436"

>>> print int("0x18058436", 0)
403014710

>>> "%08x" % 403014710
'18058436'
```
- SUGAR `block6513497`
```py
sugarchain-cli getblockheader "855f0c66238bc0246c8ca25cf958283fd49b9fb4b217ddeb518e5ea9f5071b9e" | grep bits
"1e00b260"

>>> print int("0x1e00b260", 0)
503362144
```

## Benchmarking
```bash
src/bench/bench_sugarchain
```
Referrence:
- https://bitcoin.stackexchange.com/questions/37605/raw-block-data-in-hexadecimals
- https://blockchain.info/block/0000000000000000025aff8be8a55df8f89c77296db6198f272d6577325d4069?format=hex
- https://stackoverflow.com/questions/6292645/convert-binary-data-to-hexadecimal-in-a-shell-script/8874643
- https://stackoverflow.com/questions/7826526/transform-hexadecimal-information-to-binary-using-a-linux-command
- https://stackoverflow.com/questions/209513/convert-hex-string-to-int-in-python
- https://github.com/bitcoin/bitcoin/blob/master/doc/benchmarking.md
