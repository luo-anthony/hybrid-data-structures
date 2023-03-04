# hybrid-data-structures
A C++ library for various hybrid data structures. 

[![License](https://img.shields.io/badge/license-MIT-green)](./LICENSE)
![Issues](https://img.shields.io/github/issues/luo-anthony/hybrid-data-structures)
[![Actions Status](https://github.com/luo-anthony/hybrid-data-structures/workflows/Ubuntu/badge.svg)](https://github.com/luo-anthony/hybrid-data-structures/actions)
[![Actions Status](https://github.com/luo-anthony/hybrid-data-structures/workflows/Style/badge.svg)](https://github.com/luo-anthony/hybrid-data-structures/actions)
[![Actions Status](https://github.com/luo-anthony/hybrid-data-structures/workflows/Install/badge.svg)](https://github.com/luo-anthony/hybrid-data-structures/actions)
[![codecov](https://codecov.io/gh/luo-anthony/hybrid-data-structures/branch/main/graph/badge.svg?token=CSKV986JHM)](https://codecov.io/gh/luo-anthony/hybrid-data-structures)

## Overview 

This library will contain various hybrid data structures such as the folllowing:
 - **Consistent Hash Ring with Bounded Loads**, based on [this paper from Google Research](https://ai.googleblog.com/2017/04/consistent-hashing-with-bounded-loads.html)
 - **Hybrid HashSet / HashMap (maybe)**, loosely inspired by [LLVM's high-performance hybrid data structures](https://www.youtube.com/watch?v=vElZc6zSIXM&list=PLiDK46vnsTT-JerJ8k5-W5L-yKvSu0M41&index=3)
 - **Various Cache Structures (maybe)**, such as LRU/LFU cache


#### Building Library 
```
cmake -S all -B build -DENABLE_TEST_COVERAGE=1 -DUSE_STATIC_ANALYZER=clang-tidy
cmake --build build -j4

# run tests
cmake --build build/test --target test

# format code
cmake --build build --target fix-format

# run standalone
./build/standalone/HybridStructures --help

# build docs
cmake --build build --target GenerateDocs

```

NOTE: `.gcno` and `.gcda` coverage files are located in `build/test/CMakeFiles/HybridStructuresTests.dir/source/`

#### References
Using C++ project template from: https://github.com/TheLartians/ModernCppStarter
