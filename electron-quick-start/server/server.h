#define SERVER "solar-db.mysql.uhserver.com"
#define USERNAME "hsilva"
#define PASSWORD "slr668128*"
#define PORT 3306
#define DATABASE "solar_db"

#include <napi.h>
#include <iostream>
#include <mysql/mysql.h>
#include <string>
#include <fmt/core.h>

using namespace std;

namespace server {
    
    // CLASSES     

    class Colaborador
    {
        public:
            int Id = 0;
            string Nome;
            int Id_cargo;
            string Email;
            string Telefone;
            string Cpf_cnpj;

            Colaborador(int id, string nome, int id_cargo, string email, string telefone, string cpf_cnpj)
            {
                Id = id;
                Nome = nome;
                Id_cargo = id_cargo;
                Email = email;
                Telefone = telefone;
                Cpf_cnpj = cpf_cnpj;
            }

            Colaborador(string nome, int id_cargo, string email, string telefone, string cpf_cnpj)
            {
                Nome = nome;
                Id_cargo = id_cargo;
                Email = email;
                Telefone = telefone;
                Cpf_cnpj = cpf_cnpj;
            }
    };

    // METHODS

    int add (int x, int y);
    int getAllClientes();
    bool createColaborador(server::Colaborador& colaborador);

    Napi::Number addWrapped(const Napi::CallbackInfo& info);
    Napi::Number getAllClientesWrapped(const Napi::CallbackInfo& info);
    Napi::Boolean createColaboradorWrapped(const Napi::CallbackInfo& info);

    Napi::Object Init(Napi::Env env, Napi::Object exports);
    NODE_API_MODULE(cppserver, Init);
}
