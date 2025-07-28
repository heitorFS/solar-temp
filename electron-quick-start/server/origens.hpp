#ifndef INCLUDE_ORIGENS_
#define INCLUDE_ORIGENS_

#include "server_object.hpp"
#include "companhias.hpp"

#include <string>
#include <sqlite3.h>

using namespace std;
using companhia = companhias::companhia;

namespace origens
{
    #pragma region class

    struct origem
    {
        int id = 0;
        string nome;
        companhia origem_companhia;

        origem(int _id, string _nome, int id_companhia)
            : id(_id), nome(_nome)
        {
            origem_companhia = companhia();
            origem_companhia.id = id_companhia;
        }

        origem(string _nome, int id_companhia) : nome(_nome)
        {            
            origem_companhia = companhia();
            origem_companhia.id = id_companhia;
        }

        origem(query_type _,sqlite3_stmt* stmt)
        {
            origem_companhia = companhia();

            id = sqlite3_column_int(stmt, 0);
            nome = reinterpret_cast<char*>(const_cast<unsigned char*>(sqlite3_column_text(stmt, 1)));
            origem_companhia.id = sqlite3_column_int(stmt, 2);
            origem_companhia.nome = reinterpret_cast<char*>(const_cast<unsigned char*>(sqlite3_column_text(stmt, 3)));
        }

        origem() {}

        static string get_table(bool select) { if (select) return "origens INNER JOIN companhias ON id_companhia = companhias.id"; return "origens"; }
        static string get_fields(bool select)
        {
            if (select) return "origens.id, origens.nome, companhias.id, companhias.nome";
            return "nome, id_companhia";
        }
        static string get_template() { return "'{}', {}"; }
    };

    #pragma endregion class

    #pragma region JSON serialization
        
    void to_json(nlohmann::json& j, origem& obj)
    {
        j = {
            { "id", obj.id }, { "nome", obj.nome },
            { "companhia", {
                { "id", obj.origem_companhia.id }, { "nome", obj.origem_companhia.nome }
            }} 
        };
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

    Napi::Number create(const Napi::CallbackInfo& info)
    {
        Napi::Env env = info.Env();

        if (info.Length() < 2 || !info[0].IsString() || !info[1].IsNumber())
            Napi::TypeError::New(env, "nome::String, id::Number")
                .ThrowAsJavaScriptException();

        origem _origem(
            remove_single_quotes(info[0].As<Napi::String>().Utf8Value(), DUPLICATE),
            info[1].As<Napi::Number>().Int32Value()
        ); 

        json errs = json::array({});
        if (!validate_letters(_origem.nome)) errs.push_back(field_error("nome", field_error_code::INVALID_FIELD));

        validate_unique(errs, "origens", field<string>("nome", _origem.nome));

        if (errs.size() > 0)
        {
            Napi::Error::New(env, to_string(errs)).ThrowAsJavaScriptException();        
            return Napi::Number::New(env, -1);
        }

        int64_t id = crud::create<origem>(_origem.nome, _origem.origem_companhia.id);
        return Napi::Number::New(env, (double)id);
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