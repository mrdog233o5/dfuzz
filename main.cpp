#include <algorithm>
#include <iostream>
#include <curl/curl.h>
#include <string>
#include <thread>
#include <vector>

using namespace std;
const char letters[] = "1234567890qwertyuiopasdfghjklzxcvbnm\0";
const char lettersNoN[] = "1234567890qwertyuiopasdfghjklzxcvbnm";
char n = '\0';
char blank;
CURL* curl = curl_easy_init();

// Write callback function to capture the headers
size_t WriteHeaderCallback(void* contents, size_t size, size_t nmemb, string* response) {
    size_t totalSize = size * nmemb;
    response->append(static_cast<char*>(contents), totalSize);
    return totalSize;
}

int getReturnCode(const char* url) {
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
    vector<string> usedDir;

    while (1) {
        string currentWord;
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
        
        for (int j = 0; j < out.size(); j++) {
            word = word + letters[out[j]];
            if (out[j] != BASE-1) {
                steak*=0;
            }
        }
        bool dirUsed = false;
        for (int i = 0; i < word.size(); i++) {
            if (word[i] != n && word[i] != blank) {
                currentWord.push_back(word[i]);
            }
        }

        for (int i = 0; i < usedDir.size(); i++) {
            if (usedDir[i] == currentWord) {
                dirUsed = true;
            }
        }
        if (currentWord.size() > 0 && dirUsed == false) {
            string combined = url + currentWord;
            const char* result = combined.c_str();
            int code = getReturnCode(result);
            if (code != 0 && code != 404) {
                cout << result << "  -  " << code << endl;
                usedDir.push_back(currentWord);
            }
        }
        times++;
        if (steak) {
            break;
        }
    }
    return times;
}

int main (int argc, char *argv[]) {
    // argument syntax = dfuzz <target url> <max lenth of the words to try>
    const char * URL = argv[1];
    const int LENGTH = stoi(argv[2]);
    const string issues[] = {"wifi is off","url doesnt exist","typo in url"};
    int code = getReturnCode(URL);
    string headingLogo = "██████╗ ███████╗██╗   ██╗███████╗███████╗\n██╔══██╗██╔════╝██║   ██║╚══███╔╝╚══███╔╝\n██║  ██║█████╗  ██║   ██║  ███╔╝   ███╔╝ \n██║  ██║██╔══╝  ██║   ██║ ███╔╝   ███╔╝  \n██████╔╝██║     ╚██████╔╝███████╗███████╗\n╚═════╝ ╚═╝      ╚═════╝ ╚══════╝╚══════╝";
    cout << headingLogo << endl << "fuzzing tool created by mrdog233o5" << endl << endl;

    if (code == 0 || code == 404) {
        cout << "failed to access url : " << URL << endl;
        cout << "possible issues : " << endl;
        for (int i = 0; i < sizeof(issues)/sizeof(issues[0]); i++) {
            cout << i+1 << " - " << issues[i] << endl;
        }
    } else {
        cout << "***All avaiable directories : " << endl << endl;
        fuzz(LENGTH,sizeof(letters)/sizeof(char)-1,URL);
        cout << endl << "done" << endl;
    }

    return 0;
}

