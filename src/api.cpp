#include "api.hpp"
#include <curl/curl.h>
#include <iostream>
#include <nlohmann/detail/conversions/to_json.hpp>
#include <nlohmann/json.hpp>
#include <string>
#include <vector>

using std::string;
using std::vector;
using json = nlohmann::json;

size_t write_callback(char *ptr, size_t size, size_t nmemb, void *userdata) {
    size_t total = size * nmemb;

    std::string *response = static_cast<std::string *>(userdata);

    response->append(ptr, total);

    return total;
}

std::vector<Anime> parse_results(const std::string &response) {
    json data = json::parse(response);
    vector<Anime> results;

    for (const auto &anime : data["data"]) {
        string title = anime["attributes"]["canonicalTitle"];
        string id = anime["id"];

        if (anime["attributes"]["titles"].contains("en")) {
            title = anime["attributes"]["titles"]["en"];
        }

        Anime result{title, id};
        results.push_back(result);
    }
    return results;
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
