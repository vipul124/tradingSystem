# tradingSystem 

## Requirements 
- **C++ 17** or later
- [curl library](https://github.com/curl/curl) - for making web requests
- [nlohmann/json](https://github.com/nlohmann/json) - json parsing
- [websocketpp](https://github.com/zaphoyd/websocketpp) - in order to run the websocket server
- [openssl/openssl](https://github.com/openssl/openssl)

## Setup 
1. **Clone the repo** -
```bash
git clone https://github.com/vipul124/tradingSystem.git
cd tradingSystem
```

2. **Install C++ dependencies** -
```bash
vcpkg install curl
vcpkg install nlohmann-json
vcpkg install websocketpp
vcpkg install openssl
```

3. **Creating env file and build folder** -
```bash
mkdir build
cd build

# creating .env file in build folder
touch .env
echo "DERIBIT_CLIENT_ID={client_id}" >> .env
echo "DERIBIT_CLIENT_SECRET={client_secret}" >> .env
```

4. **Build the Application** -
```bash
cmake .. -DCMAKE_TOOLCHAIN_FILE={/path/to/vcpkg/installation}/scripts/buildsystems/vcpkg.cmake
cmake --build .
```

## Running the Application
  - To run the cpp file containing sample commands of order execution system that we have created using deribit-api -> `./main`
  - To start the websocket server -> `./webServer`

  ### Testing the websocket server with Postman
   - endpoint - `ws://localhost:8080`
   - message example -
        ```json
        {
            "method": "subscribe",
            "symbol": "BTC-PERPETUAL",
            "depth": 1,
            "timeout": 3
        }
        ```
       

