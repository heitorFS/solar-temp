#define SERVER "solar-db.mysql.uhserver.com"
#define USERNAME "hsilva"
#define PASSWORD "slr668128*"
#define PORT 3306
#define DATABASE "solar_db"

#include <napi.h>
#include <iostream>
#include <mysql/mysql.h>
#include <string>

using namespace std;

namespace server {
    
    // CLASSES

    enum Cargos
    {
        VENDEDOR,
        PROJETISTA,
        INSTALADOR,
        FINANCEIRO,
        ADMINISTRATIVO
    };

    class Colaborador
    {
        public:
            int id;
            string nome;
            //u_int id_cargo
    };

    // METHODS

    int add (int x, int y);
    int getAllClientes();

    Napi::Number addWrapped(const Napi::CallbackInfo& info);
    Napi::Number getAllClientesWrapped(const Napi::CallbackInfo& info);

    Napi::Object Init(Napi::Env env, Napi::Object exports);
    NODE_API_MODULE(cppserver, Init);
}
