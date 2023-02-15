#ifndef BLOOM_FILTER_HPP
#define BLOOM_FILTER_HPP

#include <cstdint>  // uint64_t

#include <bitset>
#include "math.h"

#include "murmurhash.hpp"



template <typename Key>
struct BloomHash {
    //default seed in some implementations is 104729 (the value can be quite big!);
    std::size_t operator()(Key key, unsigned int seed) const {
        std::size_t len = sizeof(key);
        return murmur3_32(reinterpret_cast<uint8_t*>(&key), len, seed);
    }
};

template <typename Key, unsigned int m, typename Hash = BloomHash<Key>>
class BloomFilter {
public:
    explicit BloomFilter(unsigned int num_hashes);

    BloomFilter(std::initializer_list<Key> init, unsigned int num_hashes);

    template <typename It>
    BloomFilter(It first, It last, unsigned int num_hashes);

    bool insert(const Key& key);

    bool contains(const Key& key) const;

    uint64_t approx_size() const;

    template <typename It>
    double false_positive_rate(It positive_begin, It positive_end,
                               It negative_begin, It negative_end) const;

    double space_ratio(uint64_t num_elements);

private:
    std::bitset<m> data_;
    unsigned int k_;
};

template<typename Key, unsigned int m, typename Hash>
BloomFilter<Key, m, Hash>::BloomFilter(unsigned int num_hashes) :k_{num_hashes}{}

template<typename Key, unsigned int m, typename Hash>
BloomFilter<Key, m, Hash>::BloomFilter(std::initializer_list<Key> init, unsigned int num_hashes) :k_{num_hashes}{
    for (Key el:init){
        Hash hash_function;
        //set seed to 21, 42...
        for(uint32_t seed = 21; seed < (k_+1)*21; seed+=21){
                //apply hash function
                uint32_t result = hash_function(el, seed);
                //change the bits in data_
                data_.set(result%m,1);
        }
    }
}

template<typename Key, unsigned int m, typename Hash>
template<typename It>
BloomFilter<Key, m, Hash>::BloomFilter(It first, It last, unsigned int num_hashes) :k_{num_hashes}{
    for (auto i = first; i!=last; i++){
        //create hash function object
        Hash hash_function;
        for(uint32_t seed = 21; seed < (k_+1)*21; seed+=21){
            //apply hash function
            uint32_t result = hash_function(*i, seed);
            //change the bits in data_
            data_.set(result%m,1);
        }

    }
}

template<typename Key, unsigned int m, typename Hash>
bool BloomFilter<Key, m, Hash>::insert(const Key &key) {

    //if the key is already coded in the bitset
    if (contains(key)) return false;

    //create hash function object
    Hash hash_function;
    for(unsigned int seed = 21; seed < (k_+1)*21; seed+=21){

        //apply hash function
        uint32_t result = hash_function(key, seed);
        //change the bits in data_
        data_.set((result%m), 1);
    }
    return true;
}

template<typename Key, unsigned int m, typename Hash>
bool BloomFilter<Key, m, Hash>::contains(const Key &key) const {
    Hash hash_function;
    for(unsigned int seed = 21; seed < (k_+1)*21; seed+=21){ //smaller seed takes less time.
        //apply hash function
        uint32_t result = hash_function(key, seed);
        //check bits in data_
        if(data_[result%m]!=1) return false;
    }
    return true;
}

template<typename Key, unsigned int m, typename Hash>
uint64_t BloomFilter<Key, m, Hash>::approx_size() const {
    //using the formula from the task:
    return static_cast<uint64_t>(
            - static_cast<double>(m)/static_cast<double>(k_)*
            std::log(1.0 - (static_cast<double>(data_.count())/static_cast<double>(m)))
    );

}

template<typename Key, unsigned int m, typename Hash>
template<typename It>
double BloomFilter<Key, m, Hash>::false_positive_rate(It positive_begin, It positive_end, It negative_begin,
                                                      It negative_end) const {
    //find the number of false positives
    unsigned int FP = 0;
    for(auto i = positive_begin; i != positive_end; i++){
        if(!contains(*i)) FP++;
    }

    //find number of true negatives
    unsigned int TN = 0;
    for(auto i = negative_begin; i != negative_end; i++){
        if(!contains(*i)) TN++;
    }
    //find the TPR
    return static_cast<double>(FP)/(TN+FP);
}

template<typename Key, unsigned int m, typename Hash>
double BloomFilter<Key, m, Hash>::space_ratio(uint64_t num_elements) {
    double we_use_space = static_cast<double>(sizeof(data_));
    double would_use_space = (sizeof(Key))*(num_elements);
    return we_use_space/would_use_space;
}


#endif  // BLOOM_FILTER_HPP
