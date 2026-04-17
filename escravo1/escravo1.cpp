#include "httplib.h"
#include <iostream>
#include <set>
#include <sstream>

using namespace httplib;

int main() {

    Server svr;

    svr.Get("/health", [](const Request&, Response& res) {
        res.set_content("OK", "text/plain");
    });

    svr.Post("/palavras", [](const Request& req, Response& res) {

        std::stringstream ss(req.body);
        std::string word;

        int total = 0;
        std::set<std::string> unique;

        while(ss >> word){
            total++;
            unique.insert(word);
        }

        std::string json =
        "{ \"total\": " + std::to_string(total) +
        ", \"unicas\": " + std::to_string(unique.size()) + "}";

        res.set_content(json, "application/json");
    });

    std::cout << "Escravo 1 rodando, porta 8081\n";

    svr.listen("0.0.0.0",8081);
}