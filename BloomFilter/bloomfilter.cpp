#include <iostream>
#include <vector>
#include <bitset>
using namespace std;

static const int kNumBits                   = 1 << 10;
static const int kNumHashFuncs              = 3;
static const unsigned kSalts[kNumHashFuncs] = {137, 69, 545};

class BloomFilter
{
    bitset<kNumBits> _bs;
    vector<unsigned> genHashes(const string &key);
public:
    BloomFilter();
    void add(const string &key);
    bool exist(const string &key);
};

BloomFilter::BloomFilter()
{
    _bs.reset();
}

vector<unsigned> BloomFilter::genHashes(const string &key)
{
    vector<unsigned> hashes(kNumHashFuncs, 0);
    for (size_t i = 0; i < key.size(); ++i) {
        for (int j = 0; j < kNumHashFuncs; ++j) {
            hashes[j] = hashes[j] * kSalts[j] + static_cast<unsigned>(key[i]);
        }
    }
    for (int i = 0; i < kNumHashFuncs; ++i) {
        hashes[i] %= kNumBits;
    }
    return hashes;
}

void BloomFilter::add(const string &key)
{
    vector<unsigned> hashes(genHashes(key));
    for (vector<unsigned>::const_iterator it = hashes.begin(); 
            it != hashes.end(); ++it) {
        _bs.set(*it, 1);
    }
}

bool BloomFilter::exist(const string &key)
{
    bool ret = false;

    vector<unsigned> hashes(genHashes(key));
    for (vector<unsigned>::const_iterator it = hashes.begin(); 
            it != hashes.end(); ++it) {
        if (_bs[*it])
            return true;
    }
    return ret;
}

int main()
{
    BloomFilter bf;

    bf.add("hoge");
    bf.add("fuga");

    cout << (bf.exist("hoge") ? "true" : "false") << endl;
    cout << (bf.exist("fuga") ? "true" : "false") << endl;
    cout << (bf.exist("kuso") ? "true" : "false") << endl;

    return 0;
}
