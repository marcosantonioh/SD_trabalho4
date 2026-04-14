#include "httplib.h"
#include <thread>
#include <iostream>

using namespace httplib;

int main(){

    Server svr;

    svr.Post("/processar", [](const Request& req, Response& res){

        std::string texto = req.body;

        std::string resp1, resp2;

        std::thread t1([&](){
            Client cli("slave1",8081);
            auto r = cli.Post("/palavras",texto,"text/plain");
            if(r) resp1 = r->body;
        });

        std::thread t2([&](){
            Client cli("slave2",8082);
            auto r = cli.Post("/tamanho",texto,"text/plain");
            if(r) resp2 = r->body;
        });

        t1.join();
        t2.join();

        std::string final =
        "{ \"analise_palavras\": " + resp1 +
        ", \"analise_tamanho\": " + resp2 + "}";

        res.set_content(final,"application/json");
    });

    std::cout<<"Mestre rodando, porta 8080\n";

    svr.listen("0.0.0.0",8080);
}