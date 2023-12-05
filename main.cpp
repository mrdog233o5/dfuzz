#include <cstring>
#include <iostream>
#include <curl/curl.h>
#include <string>
#include <vector>
#include <fstream>

using namespace std;
const char letters[] = "1234567890qwertyuiopasdfghjklzxcvbnm\0";
const char lettersNoN[] = "1234567890qwertyuiopasdfghjklzxcvbnm";
char n = '\0';
char blank;
CURL* curl = curl_easy_init();

vector<string> fuzzWordlist(const char* url, const char* wordlist);
size_t WriteHeaderCallback(void* contents, size_t size, size_t nmemb, string* response);
int getReturnCode(const char* url);
vector<string> fuzzAll(const char* url, int DIGITS, int BASE);
bool checkURL(const char * URL);

int main (int argc, char *argv[]) {
    string URLstr = argv[2];
    if (URLstr.back() != '/') {
        URLstr += '/';
    }
    const char* URL = URLstr.c_str();
    // check argv
    if (argc < 2) {
        cout << "missing argument, try to run \n man dfuzz" << endl;
        return 1;
    }

    // argument syntax = dfuzz <target url> <max lenth of the words to try>
     if (argc < 2) {
        cout << "missing mode" << endl;
        return 1;
    }
    if (argc < 3) {
        cout << "missing url" << endl;
        return 1;
    }
    string headingLogo = "██████╗ ███████╗██╗   ██╗███████╗███████╗\n██╔══██╗██╔════╝██║   ██║╚══███╔╝╚══███╔╝\n██║  ██║█████╗  ██║   ██║  ███╔╝   ███╔╝ \n██║  ██║██╔══╝  ██║   ██║ ███╔╝   ███╔╝  \n██████╔╝██║     ╚██████╔╝███████╗███████╗\n╚═════╝ ╚═╝      ╚═════╝ ╚══════╝╚══════╝";
    cout << headingLogo << endl << "fuzzing tool created by mrdog233o5" << endl << endl;

    if (checkURL(URL)) {
        cout << "check pass" << endl;
        if (strcmp(argv[1], "all") == 0) {
            cout << "selected mode: all" << endl;
            cout << "***All avaiable directories : " << endl << endl;
            fuzzAll(URL, stoi(argv[3]),sizeof(letters)/sizeof(char)-1);
            cout << endl << "done" << endl;
        } else if (strcmp(argv[1], "word") == 0) {
            cout << "selected mode: wordlist" << endl;
            cout << "***All avaiable directories : " << endl << endl;
            fuzzWordlist(URL, argv[3]);
            cout << endl << "done" << endl;
        } else {
            cout << "mode not found!" << endl;
            return 1;
        }
    }

    return 0;
}

bool checkURL(const char * URL) {
    const string issues[] = {"wifi is off","url doesnt exist","typo in url"};
    int code = getReturnCode(URL);
    if (code == 0 || code == 404) {
        cout << "failed to access url : " << URL << endl;
        cout << "possible issues : " << endl;
        for (int i = 0; i < sizeof(issues)/sizeof(issues[0]); i++) {
            cout << i+1 << " - " << issues[i] << endl;
        }
        return false;
    }
    return true;
}

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

vector<string> fuzzAll(const char* url, int DIGITS, int BASE) { 
    int size = DIGITS;
    int times = 0;
    vector<string> res;

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

        for (int i = 0; i < res.size(); i++) {
            if (res[i] == currentWord) {
                dirUsed = true;
            }
        }
        if (currentWord.size() > 0 && dirUsed == false) {

            // send req
            string combined = url + currentWord;
            const char* result = combined.c_str();
            int code = getReturnCode(result);
            if (code != 0 && code != 404) {
                cout << "\033[38;2;255;255;100m" << result << "  -  " << code << endl;
                res.push_back(currentWord);
            }

        }
        times++;
        if (steak) {
            break;
        }
    }
    return res;
}

vector<string> fuzzWordlist(const char* url, const char* wordlist) {
    vector<string> res;
    vector<string> words;
    string currentWord;
    
    // read wordlist
    string line;
    ifstream MyReadFile(wordlist);
    while (getline (MyReadFile, line)) {
        words.push_back(line);
    }
    MyReadFile.close();

    // fuzz
    for (int i = 0; i < words.size(); i++) {
        // send req
        currentWord = words[i];
        string combined = url + currentWord;
        const char* result = combined.c_str();
        int code = getReturnCode(result);
        if (code != 0 && code != 404) {
            cout << "\033[38;2;255;255;100m" << result << "  -  " << code << endl;
           res.push_back(currentWord);
        }
        
    }

    return res;
}
