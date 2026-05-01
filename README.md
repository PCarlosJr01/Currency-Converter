# Currency Converter (C++)

A C++ command-line currency converter that integrates with a live exchange rate API.  
This project demonstrates real-world software engineering concepts including API consumption, JSON parsing, caching strategies, and robust error handling.



## Features

- ive exchange rate fetching via REST API (libcurl)
- JSON parsing using nlohmann/json
- Smart caching system (1-hour validity)
- Offline fallback using cached data
- Input validation and error handling
- Accurate currency conversion with formatted output
- Built using CMake with MinGW toolchain



## How It Works

1. Checks if cached exchange rate data is valid (within 1 hour)
2. If expired or missing:
   - Fetches latest rates from API
   - Stores response locally as JSON
3. Parses cached data to compute conversion rates
4. Falls back to cached data if API request fails



## Tech Stack

- **Language:** C++
- **Build System:** CMake
- **Compiler:** MinGW (MSYS2)
- **Libraries:**
  - libcurl (HTTP requests)
  - nlohmann/json (JSON parsing)



## How to Build

```bash
cd Currency-Converter
mkdir build
cd build
cmake .. -G "MinGW Makefiles" -DOPENSSL_ROOT_DIR=C:/msys64/ucrt64 -DZLIB_ROOT=C:/msys64/ucrt64
cmake --build .

./converter.exe