#include <iostream>
#include <curl/curl.h>
#include <string>

using namespace std;

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
            std::cerr << "Failed to send request: " << curl_easy_strerror(res) << std::endl;
            return 0;
        }
        // Cleanup
        curl_easy_cleanup(curl);
    }
    return 0;
}

int main (int argc, char *argv[]) {
    int length = stoi(argv[2]);
    cout << getReturnCode(argv[1]) << endl;
    for (int i = 0; i < length; i++) {
        for (int j = 33; j < 126+1; j++) {
            cout << char(j);
        }
        cout << endl;
    }
    return 0;
}

