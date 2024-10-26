#include <iostream>
#include <map>
#include <fstream>
#include <string>
#include <sstream>
#include <curl/curl.h>
using namespace std;

// callback helper function to write response
size_t WriteCallback(void* contents, size_t size, size_t nmemb, string* userp) {
    size_t totalSize = size * nmemb;
    userp->append((char*)contents, totalSize);
    return totalSize;
}

// webRequests
string sendRequest(const string req, const long timeout, const string &url, const string &payload, const string &auth = ""){
    CURL* curl;
    CURLcode status;
    string response;

    // initialize
    curl_global_init(CURL_GLOBAL_DEFAULT);
    curl = curl_easy_init();

    if (curl){
        // headers
        struct curl_slist* headers = nullptr;
        headers = curl_slist_append(headers, "Content-Type: application/json");
        if (auth != ""){
            string authHeader = "Authorization: " + auth;
            headers = curl_slist_append(headers, authHeader.c_str());
        }

        // set curl parameters
        curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
        curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);
        if (req == "POST") {
            // by default adding post fields switches the curl mode to POST
            curl_easy_setopt(curl, CURLOPT_POSTFIELDS, payload.c_str());
        }
        if (req == "GET"){
            curl_easy_setopt(curl, CURLOPT_HTTPGET, 1);
        }
        curl_easy_setopt(curl, CURLOPT_TIMEOUT_MS, timeout);
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallback);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, &response);

        // performing the post request
        status = curl_easy_perform(curl);
        if (status != CURLE_OK){
            cerr << req << " Error - " << curl_easy_strerror(status) << endl;
        }

        curl_slist_free_all(headers);
        curl_easy_cleanup(curl);
    }

    curl_global_cleanup();
    return response;
}

// readEnv 
map<string, string> readEnv(const string &fileName){
    map<string, string> env;
    ifstream file(fileName);
    string line;

    if (!file){
        cerr << "please create the file - " << fileName << endl;
    }
    while (getline(file, line)){
        istringstream var(line);
        string key, value;
        if (getline(var, key, '=') && getline(var, value)){
            env[key] = value;
        }
    }

    return env;
}