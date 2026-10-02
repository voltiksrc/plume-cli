#include "api.hpp"
#include <iostream>
#include <string>
#include <vector>

using std::string;
using std::vector;

int main(int argc, char *argv[]) {
    if (argc < 2) {
        std::cout << "usage: plume <anime>\n";
        return 1;
    }

    string search;

    for (int i = 1; i < argc; ++i) {
        search += argv[i];

        if (i < argc - 1) {
            search += " ";
        }
    }
    string response = request_page(search);

    if (response.empty()) {
        return 1;
    }

    vector<Anime> results = parse_results(response);
    int number = 1;

    for (const auto &anime : results) {
        std::cout << number << ". " << anime.title << '\n';
        ++number;
    }
    string choice;
    std::cout << "\n";
    std::cout << "Enter the anime you want to watch: ";
    std::getline(std::cin, choice);
    int selected = std::stoi(choice);
    if (selected < 1) {
        std::cerr << "Out of reach!\n";
        return 1;
    }
    if (selected > results.size()) {
        std::cerr << "Out of reach!\n";
        return 1;
    }
    string anititle = results[selected - 1].title;
    std::cout << "You picked: " << anititle << '\n';
}
