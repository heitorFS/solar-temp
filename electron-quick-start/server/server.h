#define DUPLICATE true
#define REMOVE false

#include <sqlite3.h>

#include "CpfCnpjValidacao.hpp"
#include <cstddef>
#include <fmt/core.h>
#include <fstream>
#include <functional>
#include <iostream>
#include <memory>
#include <napi.h>
#include <nlohmann/json.hpp>
#include <regex>
#include <string>
#include <vector>

using namespace std;

namespace server {

    #pragma region Prototypes

    template <int N, typename ...T>
    decltype(auto) get_argument(const T&... t);

    void formatQuery(string& base, string& query, string& table);

    template <typename T, typename... Args>
    void formatQuery(string& base, string& query, string& table, T& arg, Args&... args);

    #pragma endregion Prototypes

    #pragma region Classes

    #pragma region Helper Classes

    enum field_error_code
    {
        INVALID_FIELD,
        NOT_UNIQUE
    };

    struct field_error
    {
        string name;
        field_error_code err;

        field_error(string _name, field_error_code _err) : name(_name), err(_err)
        {}
    };

    struct date
    {
        static string dash_to_slash(string _d)
        {
            return fmt::format("{}/{}/{}", _d.substr(8, 2), _d.substr(5, 2), _d.substr(0, 4));
        }

        static bool validate_date(string _d)
        {
            if (regex_search(_d, regex("^([0-9]){4}-([0-9]){2}-([0-9]){2}$", regex_constants::ECMAScript)))
                return true;
            return false;
        }
    };

    #pragma endregion Helper Classes

    struct cliente
    {
        int id = 0;
        string nome;
        string email;
        int proprietario;
        string cpf_cnpj;
        string telefone;
        string cep;
        long numero;
        string complemento;
        int origem;
        string data_origem;
        string extra_nome;
        string extra_cpf;
        string extra_rg;
        string extra_nacionalidade;
        string extra_profissao;
        double extra_renda;
        string observacoes;

        cliente(int _id, string _nome, string _email, int _propietario, string _cpf_cnpj, string _telefone, string _cep, long _numero, string _complemento, int _origem, string _data_origem, string _extra_nome, string _extra_cpf, string _extra_rg, string _extra_nacionalidade, string _extra_profissao, double _extra_renda, string _observacoes)
        : id(_id), nome(_nome), email(_email), proprietario(_propietario), cpf_cnpj(_cpf_cnpj), telefone(_telefone), cep(_cep), numero(_numero), complemento(_complemento), origem(_origem), data_origem(_data_origem), extra_nome(_extra_nome), extra_cpf(_extra_cpf), extra_rg(_extra_rg), extra_nacionalidade(_extra_nacionalidade), extra_profissao(_extra_profissao), extra_renda(_extra_renda), observacoes(_observacoes)
        {}

        cliente(string _nome, string _email, int _propietario, string _cpf_cnpj, string _telefone, string _cep, long _numero, string _complemento, int _origem, string _data_origem, string _extra_nome, string _extra_cpf, string _extra_rg, string _extra_nacionalidade, string _extra_profissao, double _extra_renda, string _observacoes)
        : nome(_nome), email(_email), proprietario(_propietario), cpf_cnpj(_cpf_cnpj), telefone(_telefone), cep(_cep), numero(_numero), complemento(_complemento), origem(_origem), data_origem(_data_origem), extra_nome(_extra_nome), extra_cpf(_extra_cpf), extra_rg(_extra_rg), extra_nacionalidade(_extra_nacionalidade), extra_profissao(_extra_profissao), extra_renda(_extra_renda), observacoes(_observacoes)
        {}

        cliente(sqlite3_stmt* stmt)
        {
            id = sqlite3_column_int(stmt, 0);
            nome = reinterpret_cast<char*>(const_cast<unsigned char*>(sqlite3_column_text(stmt, 1)));
            email = reinterpret_cast<char*>(const_cast<unsigned char*>(sqlite3_column_text(stmt, 2)));
            proprietario = sqlite3_column_int(stmt, 3);
            cpf_cnpj = reinterpret_cast<char*>(const_cast<unsigned char*>(sqlite3_column_text(stmt, 4)));
            telefone = reinterpret_cast<char*>(const_cast<unsigned char*>(sqlite3_column_text(stmt, 5)));
            cep = reinterpret_cast<char*>(const_cast<unsigned char*>(sqlite3_column_text(stmt, 6)));
            numero = sqlite3_column_int64(stmt, 7);
            complemento = reinterpret_cast<char*>(const_cast<unsigned char*>(sqlite3_column_text(stmt, 8)));
            origem = sqlite3_column_int(stmt, 9);
            data_origem = reinterpret_cast<char*>(const_cast<unsigned char*>(sqlite3_column_text(stmt, 10)));
            extra_nome = reinterpret_cast<char*>(const_cast<unsigned char*>(sqlite3_column_text(stmt, 11)));
            extra_cpf = reinterpret_cast<char*>(const_cast<unsigned char*>(sqlite3_column_text(stmt, 12)));
            extra_rg = reinterpret_cast<char*>(const_cast<unsigned char*>(sqlite3_column_text(stmt, 13)));
            extra_nacionalidade = reinterpret_cast<char*>(const_cast<unsigned char*>(sqlite3_column_text(stmt, 14)));
            extra_profissao = reinterpret_cast<char*>(const_cast<unsigned char*>(sqlite3_column_text(stmt, 15)));
            extra_renda = sqlite3_column_double(stmt, 16);
            observacoes = reinterpret_cast<char*>(const_cast<unsigned char*>(sqlite3_column_text(stmt, 17)));
        }

        static string get_table() { return "clientes"; }
        static string get_fields() { return "nome, email, proprietario, cpf_cnpj, telefone, cep, numero, complemento, origem, data_origem, extra_nome, extra_cpf, extra_rg, extra_nacionalidade, extra_profissao, extra_renda, observacoes"; }
        static string get_template() { return "'{}', '{}', {}, '{}', '{}', '{}', {}, '{}', {}, '{}', '{}', '{}', '{}', '{}', '{}', {}, '{}'"; }
    };

    struct colaborador
    {
        int id = 0;
        string nome;
        int id_cargo;
        string email;
        string telefone;
        string cpf_cnpj;

        colaborador(int _id, string _nome, int _id_cargo, string _email, string _telefone, string _cpf_cnpj)
        {
            id = _id;
            nome = _nome;
            id_cargo = _id_cargo;
            email = _email;
            telefone = _telefone;
            cpf_cnpj = _cpf_cnpj;
        }

        colaborador(string _nome, int _id_cargo, string _email, string _telefone, string _cpf_cnpj)
        {
            nome = _nome;
            id_cargo = _id_cargo;
            email = _email;
            telefone = _telefone;
            cpf_cnpj = _cpf_cnpj;
        }

        colaborador(sqlite3_stmt* stmt)
        {
            id = sqlite3_column_int(stmt, 0);
            nome = reinterpret_cast<char*>(const_cast<unsigned char*>(sqlite3_column_text(stmt, 1)));
            id_cargo = sqlite3_column_int(stmt, 2);
            email = reinterpret_cast<char*>(const_cast<unsigned char*>(sqlite3_column_text(stmt, 3)));
            telefone = reinterpret_cast<char*>(const_cast<unsigned char*>(sqlite3_column_text(stmt, 4)));
            cpf_cnpj = reinterpret_cast<char*>(const_cast<unsigned char*>(sqlite3_column_text(stmt, 5)));
        }

        static string get_table() { return "colaboradores"; }
        static string get_fields() {
            return "nome, id_cargo, email, telefone, cpf_cnpj";
        }
        static string get_template() { return "'{}', {}, '{}', '{}', '{}'"; }
    };

    struct origem
    {
        int id = 0;
        string nome;

        origem(int _id, const unsigned char* _nome)
        {
            id = _id;
            nome = reinterpret_cast<char*>(const_cast<unsigned char*>(_nome));
        }

        origem(string _nome) : nome(_nome)
        {}

        origem(sqlite3_stmt* stmt)
        {
            id = sqlite3_column_int(stmt, 0);
            nome = reinterpret_cast<char*>(const_cast<unsigned char*>(sqlite3_column_text(stmt, 1)));
        }

        static string get_table() { return "origens"; }
        static string get_fields() {
            return "nome";
        }
        static string get_template() { return "'{}'"; }
    };

    #pragma endregion Classes

    #pragma region JSON Serialization  
    
    void to_json(nlohmann::json& j, field_error& obj)
    {
        j = {{ "name", obj.name }, { "error_type", obj.err }};
    }
    
    void to_json(nlohmann::json& j, cliente& obj)
    {
        j = 
        {
            { "id", obj.id }, { "nome", obj.nome },  { "email", obj.email },
            { "proprietario", obj.proprietario }, { "cpf_cnpj", obj.cpf_cnpj }, { "telefone", obj.telefone },
            { "cep", obj.cep }, { "numero", obj.numero }, { "complemento", obj.complemento },
            { "origem", obj.origem }, { "data_origem", obj.data_origem }, { "extra_nome", obj.extra_nome },
            { "extra_cpf", obj.extra_cpf }, { "extra_rg", obj.extra_rg }, { "extra_nacionalidade", obj.extra_nacionalidade },
            { "extra_profissao", obj.extra_profissao }, { "extra_renda", obj.extra_renda }, { "observacoes", obj.observacoes }
        };
    }
    
    void to_json(nlohmann::json& j, colaborador& obj)
    {
        j = {{ "id", obj.id }, { "nome", obj.nome }, { "id_cargo", obj.id_cargo }, { "email", obj.email }, { "telefone", obj.telefone }, { "cpf_cnpj", obj.cpf_cnpj }};
    }
    
    void to_json(nlohmann::json& j, origem& obj)
    {
        j = {{ "id", obj.id }, { "nome", obj.nome }};
    }

    #pragma endregion JSON Serialization

    #pragma region Methods

    string remove_single_quotes(string txt, bool duplicate)
    {
        size_t location = txt.find('\'');
        while (location != string::npos)
        {
            txt.replace(location, 1, duplicate ? "''" : "");
            location = txt.find('\'', location + 1);
        }

        return txt;
    }

    template <int N, typename ...T>
    decltype(auto) get_argument(const T&... t)
    {
        return get<N>(forward_as_tuple(t...));
    }

    void formatQuery(string& base, string& query, string& table)
    {
        base.erase(base.length() - 1);
    }

    template <typename T, typename... Args>
    void formatQuery(string& base, string& query, string& table, T& arg, Args&... args)
    {
        base += fmt::format(typeid(arg.value) == typeid(string) ? query : remove_single_quotes(query, REMOVE), arg.name, arg.value, arg.extra != NULL ? fmt::format(" AND {}", arg.extra) : "");
        formatQuery(base, query, table, args...);
    }

    static int close_connection(void* _db, int count, char** data, char** columns)
    {
        sqlite3 *db = (sqlite3*)_db;
        return sqlite3_close(db);
    }

    int add (int x, int y);
    bool setupDatabase();

    Napi::Number addWrapped(const Napi::CallbackInfo& info);
    Napi::Boolean setupDatabaseWrapped(const Napi::CallbackInfo& info);

    Napi::String getAllClientesWrapped(const Napi::CallbackInfo& info);
    Napi::Boolean createClienteWrapped(const Napi::CallbackInfo& info);
    Napi::Boolean updateClienteWrapped(const Napi::CallbackInfo& info);
    Napi::Boolean deleteClienteWrapped(const Napi::CallbackInfo& info);

    Napi::String getAllColaboradoresWrapped(const Napi::CallbackInfo& info);
    Napi::Boolean createColaboradorWrapped(const Napi::CallbackInfo& info);
    Napi::Boolean updateColaboradorWrapped(const Napi::CallbackInfo& info);
    Napi::Boolean deleteColaboradorWrapped(const Napi::CallbackInfo& info);
    
    Napi::String getAllOrigensWrapped(const Napi::CallbackInfo& info);
    Napi::Boolean createOrigemWrapped(const Napi::CallbackInfo& info);
    Napi::Boolean deleteOrigemWrapped(const Napi::CallbackInfo& info);

    #pragma endregion Methods

    Napi::Object Init(Napi::Env env, Napi::Object exports);
    NODE_API_MODULE(cppserver, Init);
}
