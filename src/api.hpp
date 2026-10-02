#pragma once
#include <string>
#include <vector>

using std::string;
struct Anime {
    string title;
    string id;
};

std::vector<Anime> parse_results(const std::string &response);
std::string request_page(const std::string &query);
