#include "combinatorics.h"


uint64_t factorial(uint64_t val) {
    
    uint64_t result{1};
    
    for(;val > 0;val--)
        result *= val;
    return result;
}


uint64_t permutation(uint64_t val, uint64_t val2) {

    if(val < val2)
        return 0;
    
    uint64_t n{factorial(val)};
    uint64_t n_k{factorial(val - val2)};
    return n / n_k;
}


uint64_t combination(uint64_t val, uint64_t val2) {
    
    if(val < val2)
        return 0;
    
    uint64_t n{factorial(val)};
    uint64_t k{factorial(val2)};
    uint64_t n_k{factorial(val - val2)};

    return n / (k*n_k);
}
