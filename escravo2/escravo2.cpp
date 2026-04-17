#include "httplib.h"
#include <iostream>
#include <sstream>

using namespace httplib;

int main(){

    Server svr;

    svr.Get("/health", [](const Request&, Response& res) {
        res.set_content("OK", "text/plain");
    });

    svr.Post("/tamanho", [](const Request& req, Response& res){

        std::stringstream ss(req.body);
        std::string word;

        std::string menor, maior;

        while(ss >> word){

            if(menor.empty() || word.size() < menor.size())
                menor = word;

            if(maior.empty() || word.size() > maior.size())
                maior = word;
        }

        std::string json =
        "{ \"menor\": \"" + menor +
        "\", \"maior\": \"" + maior + "\"}";

        res.set_content(json,"application/json");

    });

    std::cout<<"Escravo 2 rodando, porta 8082\n";

    svr.listen("0.0.0.0",8082);
}