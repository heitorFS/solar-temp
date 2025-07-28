#ifndef INCLUDE_COMPANHIAS_
#define INCLUDE_COMPANHIAS_

#include "server_object.hpp"

#include <string>
#include <sqlite3.h>

using namespace std;

namespace companhias
{
    #pragma region class

    struct companhia
    {
        int id = 0;
        string nome;

        companhia(int _id, string _nome)
            : id(_id), nome(_nome) {}

        companhia(string _nome)
            : nome(_nome) {}

        companhia(query_type _,sqlite3_stmt* stmt)
        {
            id = sqlite3_column_int(stmt, 0);
            nome = reinterpret_cast<char*>(const_cast<unsigned char*>(sqlite3_column_text(stmt, 1)));
        }

        companhia(){}

        static string get_table(bool select) { return "companhias"; }
        static string get_fields(bool select)
        {
            if (select) return "*";
            return "nome";
        }
        static string get_template() { return "'{}'"; }
    };

    #pragma endregion class

    #pragma region JSON serialization
        
    void to_json(nlohmann::json& j, companhia& obj)
    {
        j = {{ "id", obj.id }, { "nome", obj.nome }};
    }

    #pragma endregion JSON serialization

    #pragma region operations

    Napi::String get_all(const Napi::CallbackInfo& info)
    {
        Napi::Env env = info.Env();

        sqlite3* db;
        int err = sqlite3_open("C:/AEnAzume/database.db", &db);

        return Napi::String::New(env, crud::read<companhia>());
    }

    Napi::String get(const Napi::CallbackInfo& info)
    {
        Napi::Env env = info.Env();

        if (info.Length() < 1 || !info[0].IsNumber())
            Napi::TypeError::New(env, "id::number")
                .ThrowAsJavaScriptException();

        string extra = fmt::format(" WHERE id = {}", info[0].As<Napi::Number>().Int32Value());
        return Napi::String::New(env, crud::read<companhia>(&extra));
    }

    Napi::Number create(const Napi::CallbackInfo& info)
    {
        Napi::Env env = info.Env();

        if (info.Length() < 1 || !info[0].IsString())
            Napi::TypeError::New(env, "nome::String")
                .ThrowAsJavaScriptException();

        companhia _companhia(
            remove_single_quotes(info[0].As<Napi::String>().Utf8Value(), DUPLICATE)
        ); 

        json errs = json::array({});
        if (!validate_letters(_companhia.nome)) errs.push_back(field_error("nome", field_error_code::INVALID_FIELD));
        
        validate_unique(errs, companhia::get_table(VALIDATE), field<string>("nome", _companhia.nome));

        if (errs.size() > 0)
        {
            Napi::Error::New(env, to_string(errs)).ThrowAsJavaScriptException();        
            return Napi::Number::New(env, -1);
        }

        int64_t id = crud::create<companhia>(_companhia.nome);
        return Napi::Number::New(env, (double)id);
    }

    Napi::Boolean update(const Napi::CallbackInfo& info)
    {
        Napi::Env env = info.Env();

        if (info.Length() < 1 || !info[0].IsString())
            Napi::TypeError::New(env, "nome::String")
                .ThrowAsJavaScriptException();

        companhia _companhia(
            remove_single_quotes(info[0].As<Napi::String>().Utf8Value(), DUPLICATE)
        ); 

        json errs = json::array({});
        if (!validate_letters(_companhia.nome)) errs.push_back(field_error("nome", field_error_code::INVALID_FIELD));
        
        validate_unique(errs, companhia::get_table(VALIDATE), field<string>("nome", _companhia.nome));

        if (errs.size() > 0)
        {
            Napi::Error::New(env, to_string(errs)).ThrowAsJavaScriptException();        
            return Napi::Boolean::New(env, false);
        }

        crud::update<companhia>(_companhia.id, field<string>("nome", _companhia.nome));

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
        if (validate_unique(errs, companhia::get_table(VALIDATE), field<size_t>("id", id)))
        {
            Napi::Error::New(env, to_string(errs)).ThrowAsJavaScriptException();        
            return Napi::Boolean::New(env, false);
        }

        crud::remove<companhia>(id);
        return Napi::Boolean::New(env, true);
    }
}

#endif // INCLUDE_COMPANHIAS_