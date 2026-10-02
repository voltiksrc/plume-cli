#include "api.hpp"
#include <iostream>
#include <string>

using std::string;

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

    parse_results(response);
}
