#include "httplib.h"
#include <fstream>
#include <iostream>

using namespace httplib;

int main(){

    std::ifstream file("palavras.txt");

    std::string text(
        (std::istreambuf_iterator<char>(file)),
        std::istreambuf_iterator<char>()
    );

    Client cli("localhost",8080);

    auto res = cli.Post("/processar",text,"text/plain");

    if(res){
        std::cout<<res->body<<std::endl;
    }

}