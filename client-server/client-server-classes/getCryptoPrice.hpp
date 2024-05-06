#include <iostream>
#include <string>
#include <curl/curl.h>

size_t WriteCallback(void *contents, size_t size, size_t nmemb, std::string *stream) {
    size_t realsize = size * nmemb;
    stream->append((char*)contents, realsize);
    return realsize;
}

std::string get_crypto_price(const std::string& symbol) {
    CURL *curl;
    CURLcode res;
    std::string readBuffer;
    std::string crypto_price = "-1.0";

    curl = curl_easy_init();
    if(curl) {
        std::string url = "https://min-api.cryptocompare.com/data/price?fsym=" + symbol + "&tsyms=USD";
        curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallback);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, &readBuffer);

        res = curl_easy_perform(curl);
        if(res == CURLE_OK) {
            size_t start_pos = readBuffer.find("USD\":");
            if (start_pos != std::string::npos) {
                start_pos += 5; // Move past "USD":
                size_t end_pos = readBuffer.find("}", start_pos);
                if (end_pos != std::string::npos) {
                    std::string price_str = readBuffer.substr(start_pos, end_pos - start_pos);
                    crypto_price = price_str;
                }
            }
        } else {
            std::cerr << "Ошибка при выполнении запроса: " << curl_easy_strerror(res) << std::endl;
        }

        curl_easy_cleanup(curl);
    }

    return crypto_price;
}
