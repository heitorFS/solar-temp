#ifndef INCLUDE_COLABORADORES_
#define INCLUDE_COLABORADORES_

#include "server_object.hpp"

#include <string>
#include <sqlite3.h>

using namespace std;

namespace colaboradores
{
    enum cargo
    {
        VENDEDOR,
        PROJETISTA,
        INSTALADOR,
        FINANCEIRO,
        ADMINISTRATIVO
    };

    #pragma region class

    struct colaborador
    {
        int id = 0;
        string nome;
        cargo id_cargo;
        string email;
        string telefone;
        string cpf_cnpj;

        colaborador(int _id, string _nome, int _id_cargo, string _email, string _telefone, string _cpf_cnpj)
        {
            id = _id;
            nome = _nome;
            id_cargo = (cargo)_id_cargo;
            email = _email;
            telefone = _telefone;
            cpf_cnpj = _cpf_cnpj;
        }

        colaborador(string _nome, int _id_cargo, string _email, string _telefone, string _cpf_cnpj)
        {
            nome = _nome;
            id_cargo = (cargo)_id_cargo;
            email = _email;
            telefone = _telefone;
            cpf_cnpj = _cpf_cnpj;
        }

        colaborador(query_type _, sqlite3_stmt* stmt)
        {
            id = sqlite3_column_int(stmt, 0);
            nome = reinterpret_cast<char*>(const_cast<unsigned char*>(sqlite3_column_text(stmt, 1)));
            id_cargo = (cargo)sqlite3_column_int(stmt, 2);
            email = reinterpret_cast<char*>(const_cast<unsigned char*>(sqlite3_column_text(stmt, 3)));
            telefone = reinterpret_cast<char*>(const_cast<unsigned char*>(sqlite3_column_text(stmt, 4)));
            cpf_cnpj = reinterpret_cast<char*>(const_cast<unsigned char*>(sqlite3_column_text(stmt, 5)));
        }

        colaborador(){}

        static string get_table(bool select) { return "colaboradores"; }
        static string get_fields(bool select)
        {
            if (select) return "*";
            return "nome, id_cargo, email, telefone, cpf_cnpj";
        }
        static string get_template() { return "'{}', {}, '{}', '{}', '{}'"; }
    };

    #pragma endregion class

    #pragma region JSON serialization
        
    void to_json(nlohmann::json& j, colaborador& obj)
    {
        j = {{ "id", obj.id }, { "nome", obj.nome }, { "id_cargo", obj.id_cargo },
        { "email", obj.email }, { "telefone", obj.telefone }, { "cpf_cnpj", obj.cpf_cnpj }};
    }

    #pragma endregion JSON serialization

    #pragma region operations

    Napi::String get_all(const Napi::CallbackInfo& info)
    {
        Napi::Env env = info.Env();

        sqlite3* db;
        int err = sqlite3_open("C:/AEnAzume/database.db", &db);

        return Napi::String::New(env, crud::read<colaborador>());
    }

    Napi::String get(const Napi::CallbackInfo& info)
    {
        Napi::Env env = info.Env();

        if (info.Length() < 1 || !info[0].IsNumber())
            Napi::TypeError::New(env, "id::number")
                .ThrowAsJavaScriptException();

        string extra = fmt::format(" WHERE id = {}", info[0].As<Napi::Number>().Int32Value());
        return Napi::String::New(env, crud::read<colaborador>(&extra));
    }

    Napi::Number create(const Napi::CallbackInfo& info)
    {
        Napi::Env env = info.Env();

        if (info.Length() < 5 || !info[0].IsString() || !info[1].IsNumber() || !info[2].IsString() || !info[3].IsString() || !info[4].IsString())
            Napi::TypeError::New(env, "nome::String, id_cargo::Number, email::String, telefone::String, cpf_cnpj::String expected")
                .ThrowAsJavaScriptException();

        colaborador _colaborador(
            remove_single_quotes(info[0].As<Napi::String>().Utf8Value(), DUPLICATE),
            info[1].As<Napi::Number>().Int32Value(),
            remove_single_quotes(info[2].As<Napi::String>().Utf8Value(), DUPLICATE),
            remove_single_quotes(info[3].As<Napi::String>().Utf8Value(), DUPLICATE),
            remove_single_quotes(info[4].As<Napi::String>().Utf8Value(), DUPLICATE)
        ); 

        json errs = json::array({});
        if (!validate_letters(_colaborador.nome)) errs.push_back(field_error("nome", field_error_code::INVALID_FIELD));
        if (_colaborador.id_cargo < 0 || _colaborador.id_cargo > 4) errs.push_back(field_error("cargo", field_error_code::INVALID_FIELD));
        if (!validate_email(_colaborador.email)) errs.push_back(field_error("email", field_error_code::INVALID_FIELD));
        if (!validate_numbers(_colaborador.telefone)) errs.push_back(field_error("telefone", field_error_code::INVALID_FIELD));
        if (!validate_numbers(_colaborador.cpf_cnpj) || !cpfcnpj_validacao::validate_cpfcnpj(_colaborador.cpf_cnpj.c_str())) errs.push_back(field_error("cpf_cnpj", field_error_code::INVALID_FIELD));

        validate_unique(errs, colaborador::get_table(VALIDATE), field<string>("cpf_cnpj", _colaborador.cpf_cnpj));
        if (errs.size() > 0)
        {
            Napi::Error::New(env, to_string(errs)).ThrowAsJavaScriptException();
            return Napi::Number::New(env, -1);
        }
        
        int64_t id = crud::create<colaborador>(_colaborador.nome, (int)_colaborador.id_cargo, _colaborador.email, _colaborador.telefone, _colaborador.cpf_cnpj);        
        return Napi::Number::New(env, (double)id);
    }

    Napi::Boolean update(const Napi::CallbackInfo& info)
    {
        Napi::Env env = info.Env();

        if (info.Length() < 6 || !info[0].IsNumber() || !info[1].IsString() || !info[2].IsNumber() || !info[3].IsString() || !info[4].IsString() || !info[5].IsString())
            Napi::TypeError::New(env, "id::Number, nome::String, id_cargo::Number, email::String, telefone::String, cpf_cnpj::String expected")
                .ThrowAsJavaScriptException();
                
        colaborador _colaborador(
            info[0].As<Napi::Number>().Int32Value(),
            remove_single_quotes(info[1].As<Napi::String>().Utf8Value(), DUPLICATE),
            info[2].As<Napi::Number>().Int32Value(),
            remove_single_quotes(info[3].As<Napi::String>().Utf8Value(), DUPLICATE),
            remove_single_quotes(info[4].As<Napi::String>().Utf8Value(), DUPLICATE),
            remove_single_quotes(info[5].As<Napi::String>().Utf8Value(), DUPLICATE)
        ); 

        json errs = json::array({});
        if (!validate_letters(_colaborador.nome)) errs.push_back(field_error("nome", field_error_code::INVALID_FIELD));
        if (_colaborador.id_cargo < 1 || _colaborador.id_cargo > 5) errs.push_back(field_error("cargo", field_error_code::INVALID_FIELD));
        if (!validate_email(_colaborador.email)) errs.push_back(field_error("email", field_error_code::INVALID_FIELD));
        if (!validate_numbers(_colaborador.telefone)) errs.push_back(field_error("telefone", field_error_code::INVALID_FIELD));
        if (!validate_numbers(_colaborador.cpf_cnpj) || !cpfcnpj_validacao::validate_cpfcnpj(_colaborador.cpf_cnpj.c_str())) errs.push_back(field_error("cpf_cnpj", field_error_code::INVALID_FIELD));

        validate_unique(errs, colaborador::get_table(VALIDATE), field<string>("cpf_cnpj", _colaborador.cpf_cnpj, fmt::format("id != {}", _colaborador.id).c_str()));
            
        if (errs.size() > 0)
            Napi::Error::New(env, to_string(errs)).ThrowAsJavaScriptException();

        crud::update<colaborador>(_colaborador.id, field<string>("nome", _colaborador.nome), field<int>("id_cargo",  (int)_colaborador.id_cargo),
            field<string>("email", _colaborador.email), field<string>("telefone", _colaborador.telefone),
            field<string>("cpf_cnpj", _colaborador.cpf_cnpj));

        return Napi::Boolean::New(env, true);
    }

    Napi::Boolean remove(const Napi::CallbackInfo& info)
    {
        Napi::Env env = info.Env();

        if (info.Length() < 1 || !info[0].IsNumber())
            Napi::TypeError::New(env, "id::number")
                .ThrowAsJavaScriptException();

        json errs = json::array({});
        size_t id = info[0].As<Napi::Number>().Int32Value();
        if (validate_unique(errs, colaborador::get_table(VALIDATE), field<size_t>("id", id)))
            Napi::Error::New(env, to_string(errs)).ThrowAsJavaScriptException();
        crud::remove<colaborador>(id);

        return Napi::Boolean::New(env, true);
    }
}

#endif // INCLUDE_COLABORADORES_