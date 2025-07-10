#ifndef INCLUDE_ORIGENS_
#define INCLUDE_ORIGENS_

#include "server_object.hpp"

#include <string>
#include <sqlite3.h>

using namespace std;

namespace origens
{
    #pragma region class

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

        origem() {}

        static string get_table(bool select) { return "origens"; }
        static string get_fields(bool select) {
            if (select) return "*";
            return "nome";
        }
        static string get_template() { return "'{}'"; }
    };

    #pragma endregion class

    #pragma region JSON serialization
        
    void to_json(nlohmann::json& j, origem& obj)
    {
        j = {{ "id", obj.id }, { "nome", obj.nome }};
    }

    #pragma endregion JSON serialization

    #pragma region operations

    Napi::String get_all(const Napi::CallbackInfo& info)
    {
        Napi::Env env = info.Env();

        return Napi::String::New(env, crud::read<origem>());
    }

    Napi::String get(const Napi::CallbackInfo& info)
    {
        Napi::Env env = info.Env();

        if (info.Length() < 1 || !info[0].IsNumber())
            Napi::TypeError::New(env, "id::number")
                .ThrowAsJavaScriptException();

        string extra = fmt::format(" WHERE id = {}", info[0].As<Napi::Number>().Int32Value());
        return Napi::String::New(env, crud::read<origem>(&extra));
    }

    Napi::Boolean create(const Napi::CallbackInfo& info)
    {
        Napi::Env env = info.Env();

        if (info.Length() < 1 || !info[0].IsString())
            Napi::TypeError::New(env, "nome::String")
                .ThrowAsJavaScriptException();

        origem _origem(
            remove_single_quotes(info[0].As<Napi::String>().Utf8Value(), DUPLICATE)
        ); 

        json errs = json::array({});
        if (!validate_letters(_origem.nome)) errs.push_back(field_error("nome", field_error_code::INVALID_FIELD));
        validate_unique(errs, "origens", field<string>("nome", _origem.nome));

        if (errs.size() > 0)
        {
            Napi::Error::New(env, to_string(errs)).ThrowAsJavaScriptException();        
            return Napi::Boolean::New(env, false);
        }

        crud::create<origem>(_origem.nome);
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
        if (validate_unique(errs, "origens", field<size_t>("id", id)))
            Napi::Error::New(env, to_string(errs)).ThrowAsJavaScriptException();

        crud::remove<origem>(id);
        return Napi::Boolean::New(env, true);
    }
}

#endif // INCLUDE_ORIGENS_