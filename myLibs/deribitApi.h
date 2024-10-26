#include <map>
#include <ctime>
#include <iostream>
#include <string>
#include <nlohmann/json.hpp>
#include "utils.h"

using namespace std;
using json = nlohmann::json;

const long long DEFAULT_TIMEOUT_MS = 10000;               // 10 seconds
const long long TOKEN_REFRESH_OFFSET_S = 60;              // 60 seconds
const string ENV_FIlE = ".env";

class tradeManager {
    private:
        string authToken = "";
        long long expiresOn = 0;
    
    public:
        // generates a new authentication token
        bool authenticate(){
            map<string, string> env = readEnv(ENV_FIlE);
            if (!env.count("DERIBIT_CLIENT_ID") || !env.count("DERIBIT_CLIENT_SECRET")){
                cerr << "please create a '" << ENV_FIlE << "' file and add the variables 'DERIBIT_CLIENT_ID' and 'DERIBIT_CLIENT_SECRET' to it";
                return false;
            }
            const string CLIENT_ID = env["DERIBIT_CLIENT_ID"];
            const string CLIENT_SECRET = env["DERIBIT_CLIENT_SECRET"];

            string req = "POST";
            string url = "https://test.deribit.com/api/v2/public/auth";
            string payload = R"({
                "method": "public/auth",
                "params": {
                    "grant_type": "client_credentials",
                    "client_id": ")" + CLIENT_ID + R"(",
                    "client_secret": ")" + CLIENT_SECRET + R"("
                }
            })";

            long long startTime = (long long)(time(0));
            string response = sendRequest(req, DEFAULT_TIMEOUT_MS, url , payload);
            auto parsed = json::parse(response);
            if (parsed.contains("result") && parsed["result"].contains("access_token") && parsed["result"].contains("expires_in")){
                authToken = "Bearer " + parsed["result"]["access_token"].get<string>();
                expiresOn = startTime + (long long)(parsed["result"]["expires_in"]);
                cout << "Authorization: " << authToken << endl;
                cout << "Token expires in - " << (long long)(parsed["result"]["expires_in"]) << " seconds" << endl;
                return true;
            }
            else {
                cerr << "Authorization Failed. Response: " << response << endl;
                return false;
            }
        }

        // refreshes the token if about to expire
        bool verifyToken(){
            if (((long long)(time(0)) + TOKEN_REFRESH_OFFSET_S) <= expiresOn){
                return true;
            }
            cout << "Authorization Expired: Trying to Refresh" << endl;
            return authenticate();
        }

        // --- TRADING FUNCTIONS BEGIN HERE ---
        // A. placing an order - supports both buy and sell
        string placeOrder(int buy, string symbol, double amount, string type = "market"){
            string req = "POST";
            string method = (buy) ? "private/buy" : "private/sell";
            string url = "https://test.deribit.com/api/v2/" + method;
            string payload = R"({
                "method": ")" + method + R"(",
                "params": {
                    "instrument_name": ")" + symbol + R"(",
                    "amount": )" + to_string(amount) + R"(, 
                    "type": ")" + type + R"("
                }
            })";

            if (verifyToken()){
                return sendRequest(req, DEFAULT_TIMEOUT_MS, url , payload, authToken);
            }
            return "Authorization Failed";
        }

        // B. cancelling an order
        string cancelOrder(string order_id){
            string req = "POST";
            string url = "https://test.deribit.com/api/v2/private/cancel";
            string payload = R"({
                "method": "private/cancel",
                "params": {
                    "order_id": ")" + order_id + R"("
                }
            })";

            if (verifyToken()){
                return sendRequest(req, DEFAULT_TIMEOUT_MS, url , payload, authToken);
            }
            return "Authorization Failed";
        }

        // C. modifying an order
        string modifyOrder(string order_id, double amount){
            string req = "POST";
            string url = "https://test.deribit.com/api/v2/private/edit";
            string payload = R"({
                "method": "private/cancel",
                "params": {
                    "order_id": ")" + order_id + R"(",
                    "amount": )" + to_string(amount) + R"(
                }
            })";

            if (verifyToken()){
                return sendRequest(req, DEFAULT_TIMEOUT_MS, url , payload, authToken);
            }
            return "Authorization Failed";
        }

        // D. get order book 
        string getOrderBook(string symbol, long long depth = 0){
            string req = "POST";
            string url = "https://test.deribit.com/api/v2/public/get_order_book";
            string payload = R"({
                "method": "public/get_order_book",
                "params": {
                    "instrument_name": ")" + symbol + R"(")" + 
                    ((depth > 0) ? R"(,"depth": )" + to_string(depth) : "") + R"(
                }
            })";

            return sendRequest(req, DEFAULT_TIMEOUT_MS, url , payload);
        }

        // E. get all the poitions
        string getPositions(){
            string req = "POST";
            string url = "https://test.deribit.com/api/v2/private/get_positions";
            string payload = R"({
                "method": "private/get_positions",
                "params": {}
            })";

            if (verifyToken()){
                return sendRequest(req, DEFAULT_TIMEOUT_MS, url , payload, authToken);
            }
            return "Authorization Failed";
        }
};