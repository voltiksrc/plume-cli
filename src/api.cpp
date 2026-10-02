#include "api.hpp"
#include <curl/curl.h>
#include <iostream>
#include <nlohmann/json.hpp>
#include <string>

using std::string;
using json = nlohmann::json;

size_t write_callback(char *ptr, size_t size, size_t nmemb, void *userdata) {
    size_t total = size * nmemb;

    std::string *response = static_cast<std::string *>(userdata);

    response->append(ptr, total);

    return total;
}

void parse_results(const std::string &response) {
    json data = json::parse(response);

    int number = 1;

    for (const auto &anime : data["data"]) {
        std::string title = anime["attributes"]["canonicalTitle"];

        std::string english = title;

        if (anime["attributes"]["titles"].contains("en")) {
            title = anime["attributes"]["titles"]["en"];
        }

        std::cout << number << ". " << title << '\n';

        ++number;
    }
}

string request_page(const std::string &query) {
    CURL *curl = curl_easy_init();

    if (!curl) {
        std::cerr << "Failed to initialize curl.\n";
        return "";
    }

    std::string encoded_search;

    char *encoded = curl_easy_escape(curl, query.c_str(), 0);

    if (encoded) {
        encoded_search = encoded;
        curl_free(encoded);
    }
    std::string apiurl =
        "https://kitsu.io/api/edge/anime?filter[text]=" + encoded_search;

    // libcurl url options
    string response;
    curl_easy_setopt(curl, CURLOPT_URL, apiurl.c_str());

    curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, 1L);

    curl_easy_setopt(curl, CURLOPT_WRITEDATA, &response);

    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_callback);

    CURLcode result = curl_easy_perform(curl);
    if (result != CURLE_OK) {
        std::cerr << curl_easy_strerror(result) << '\n';
        curl_easy_cleanup(curl);
        return "";
    }
    long response_code;
    curl_easy_getinfo(curl, CURLINFO_RESPONSE_CODE, &response_code);
    if (response_code == 404) {
        std::cerr << "Page not found for: " << query << '\n';
        curl_easy_cleanup(curl);
        return "";
    }
    if (response_code != 200) {
        std::cerr << "API Request failed. HTTP: " << response_code << '\n';
        curl_easy_cleanup(curl);
        return "";
    }
    curl_easy_cleanup(curl);
    return response;
}
