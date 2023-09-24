#include <algorithm>
#include <iostream>
#include <curl/curl.h>
#include <string>
#include <vector>

using namespace std;

const char letters[] = "1234567890QWERTYUIOPASDFGHJKLZXCVBNMqwertyuiopasdfghjklzxcvbnm";

// Write callback function to capture the headers
size_t WriteHeaderCallback(void* contents, size_t size, size_t nmemb, string* response) {
    size_t totalSize = size * nmemb;
    response->append(static_cast<char*>(contents), totalSize);
    return totalSize;
}

int getReturnCode(const char* url) {
    CURL* curl = curl_easy_init();
    if (curl) {
        // Set the target URL
        curl_easy_setopt(curl, CURLOPT_URL, url);

        // Exclude the body and capture only the headers
        curl_easy_setopt(curl, CURLOPT_NOBODY, 1L);

        // Set the write callback function to capture the headers
        string response;
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteHeaderCallback);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, &response);

        // Perform the request
        CURLcode res = curl_easy_perform(curl);
        if (res == CURLE_OK) {
            // Get the return code from the response
            long httpCode = 0;
            curl_easy_getinfo(curl, CURLINFO_RESPONSE_CODE, &httpCode);
            return httpCode;
        } else {
            return 0;
        }
        // Cleanup
        curl_easy_cleanup(curl);
    }
    return 0;
}

int fuzz(int DIGITS, int BASE, const char* url) {
    int size = DIGITS;
    int times = 0;
    
    while (1) {
        vector<int> out(size);
        string word = "";
        int steak = 1;
        for (int i = 0; i < times; i ++) {
            out[0] = out[0]+1;
            for (int j = 0; j < out.size(); j++) {
                if (out[j] >= BASE) {
                    try {
                        out[j+1] = out[j+1]+1;
                        out[j] = 0;
                    } catch (...) {
                        void(0);
                    }
                } else {
                    break;
                }
            }
        }
        times++;

        for (int j = 0; j < out.size(); j++) {
            word = word + letters[out[j]];
            if (out[j] != BASE-1) {
                steak*=0;
            }
        }
        
        string combined = url + word;
        const char* result = combined.c_str();
        cout << result << "  -  " << getReturnCode(result) << endl;

        if (steak) {
            break;
        }
    }
    return times;
}

int main (int argc, char *argv[]) {
    int length = stoi(argv[2]);
    fuzz(stoi(argv[2]),sizeof(letters)/sizeof(char)-1,argv[1]);

    return 0;
}

