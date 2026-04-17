#include "httplib.h"
#include <thread>
#include <iostream>

using namespace httplib;

int main(){

    Server svr;

    svr.Post("/processar", [](const Request& req, Response& res){

        std::string texto = req.body;

        std::string resp1, resp2;

        //verifica /health dos escravos
        Client check1("escravo1",8081);
        auto h1 = check1.Get("/health");

        Client check2("escravo2",8082);
        auto h2 = check2.Get("/health");

        if(!h1 || !h2){
            res.set_content("Algum escravo indisponível","text/plain");
            return;
        }

        //Cria threads
        std::thread t1([&](){
            Client cli("escravo1",8081);
            auto r = cli.Post("/palavras",texto,"text/plain");
            if(r){
                resp1 = r->body;
            }else{
                resp1 = "{\"erro\": \"escravo1 indisponivel\"}";
            }
        });

        std::thread t2([&](){
            Client cli("escravo2",8082);
            auto r = cli.Post("/tamanho",texto,"text/plain");
            if(r){
                resp2 = r->body;
            }else{
                resp2 = "{\"erro\": \"escravo2 indisponivel\"}";
            }
        });

        t1.join();
        t2.join();

        std::string final =
        "{\"analise_palavras\": " + resp1 +
        ", \"analise_tamanho\": " + resp2 + "}";
        
        res.set_content(final,"application/json");
    });

    std::cout<<"Mestre rodando, porta 8080\n";

    svr.listen("0.0.0.0",8080);
}