# Horner method

* A number `n` in base `b` can be represented as:

    `n = n_kn_{k-1}...n_2n_1n_0`
    
    `n = b^0*n_0 + b^1*n_1 + b^2*n_2 + ... + b^{k-1}*n_{k-1} + b^k*n_k`
    
    `n = n_0 + b*(n_1 + b*(n_2 + ... + b*(n_{k-1} + b*n_k) )`
    
    `n = (n_0n_1...n_{r-1}) + b^r((n_rn_{r+1}...n_{2r-1}) + ... + b^r(n_{k-r+1}...n_k) )`
    
* Number of base `b` digits in number `n` is `ceil`($log_2n / log_2b$).
* Optimized method uses last formula to do the base conversion.

## Build Instructions
* g++-7 -O3 bases.cpp -I. -shared -o libbases.so -fPIC -Wall
* g++-7 -O3 bases_unittest.cpp -I/home/gaurav/Code/C++/googletest/googletest/include/ -I. -lgtest -L/home/gaurav/Code/C++/googletest/googlemock/gtest/ -L/home/gaurav/Code/C++/bases/ -lbases -lpthread -o unittest
* g++-7 -O3 bases_benchmark.cpp -I/home/gaurav/Code/C++/googlebenchmark/include/ -I. -lbenchmark -L/home/gaurav/Code/C++/googlebenchmark/build/src/ -L/home/gaurav/Code/C++/bases/ -lbases -lpthread -o benchmark

## Benchmark Results

    ---------------------------------------------------------------------------
    Benchmark                                    Time           CPU Iterations
    ---------------------------------------------------------------------------
    BM_baseline                                 93 ns         93 ns    7580731
    BM_toBase36_size16                         251 ns        251 ns    2790510
    BM_toBase36Horner_size16                  1253 ns       1253 ns     562359
    BM_toBase36Horner_size1000             4263691 ns    4262057 ns        176
    BM_toBase36HornerOptimized_size1000     104725 ns     104688 ns       6685


**Notes:**

1. toBase36Horner is much slower than toBase36 for the same input size.
   Former is performing more arithmetic operations than later since it tries
   to evaluate Base-36 digits at the time of processing Base-16 digits (which
   gives it the advantage that it can handle input strings of size greater
   than 16 since it doesn't overflow), while the latter evaluates all
   Base-36 digits at the end once.

2. toBase36HornerOptimized processes input and output digits in chunks to
   to minimize arithmetic operations and thus becomes much faster. 
