# hybrid-data-structures
A C++ library for performance/cache optimized hybrid data structures. 

![GitHub](https://img.shields.io/github/license/luo-anthony/hybrid-data-structures)



## Overview 
This library will contain performance/cache-optimized hybrid data structures such as a Hybrid HashSet or HashMap that uses a dense array for lookups for small capacities, hash-based data structures for medium-large capacities, and tries / various trees for extremely large capacities (to avoid hash collision overhead). This library is inspired by [LLVM's high-performance hybrid data structures](https://www.youtube.com/watch?v=vElZc6zSIXM&list=PLiDK46vnsTT-JerJ8k5-W5L-yKvSu0M41&index=3).
