#ifndef INCLUDE_KITS_
#define INCLUDE_KITS_

#include "server_object.hpp"

#include <string>
#include <sqlite3.h>
#include <vector>

using namespace std;
using json = nlohmann::json;

namespace kits
{
    enum tensao
    {
        V127,
        V220,
        V380
    };

    enum fixacao
    {
        CERAMICA,
        METALICA_TRAPEZOIDAL,
        METALICA_ONDULADA,
        METALICA_55CM,
        FIBROCIMENTO_EST_MADEIRA,
        FIBROCIMENTO_EST_METALICA,
        LAJE,
        SOLO,
        MINI_TRILHO,
        KALHETAO,
        AMERICANO,
        CARPORT,
        ZIPADO,
        SEM_ESTRUTURA
    };

    enum monitoramento
    {
        WIFI,
        _4G,
        NENHUM
    };

    struct short_kit
    {
        uint64_t soma_potencia;
        double valor_kw;
    };

    #pragma region kit class

    struct kit
    {
        size_t id;
        string fornecedor;
        double valor;
        tensao id_tensao;
        fixacao id_fixacao;
        json modulos;
        json inversores;
        json itens;
        string observacoes;
        short_kit short_return;

        kit(size_t _id, string _fornecedor, double _valor, int _tensao, int _fixacao, string _observacoes)
            : id(_id), fornecedor(_fornecedor), valor(_valor), id_tensao((tensao) _tensao), id_fixacao((fixacao) _fixacao), observacoes(_observacoes)
        {}

        kit(string _fornecedor, double _valor, int _tensao, int _fixacao, string _observacoes)
            : fornecedor(_fornecedor), valor(_valor), id_tensao((tensao) _tensao), id_fixacao((fixacao) _fixacao), observacoes(_observacoes)
        {}

        kit(query_type select,sqlite3_stmt* stmt)
        {
            switch (select)
            {
                case query_type::SELECT_FULL:
                    id = sqlite3_column_int64(stmt, 0);
                    fornecedor = reinterpret_cast<char*>(const_cast<unsigned char*>(sqlite3_column_text(stmt, 1)));
                    valor = sqlite3_column_double(stmt, 2);
                    id_tensao = (tensao) sqlite3_column_int(stmt, 3);
                    id_fixacao = (fixacao) sqlite3_column_int(stmt, 4);
                    observacoes = reinterpret_cast<char*>(const_cast<unsigned char*>(sqlite3_column_text(stmt, 5)));
                    modulos = json::parse(reinterpret_cast<char*>(const_cast<unsigned char*>(sqlite3_column_text(stmt, 6))));
                    inversores = json::parse(reinterpret_cast<char*>(const_cast<unsigned char*>(sqlite3_column_text(stmt, 7))));
                    itens = json::parse(reinterpret_cast<char*>(const_cast<unsigned char*>(sqlite3_column_text(stmt, 8))));
                    break;
                case query_type::SELECT_SHORT:
                    id = sqlite3_column_int64(stmt, 0);
                    fornecedor = reinterpret_cast<char*>(const_cast<unsigned char*>(sqlite3_column_text(stmt, 1)));
                    valor = sqlite3_column_double(stmt, 2);
                    id_tensao = (tensao) sqlite3_column_int(stmt, 3);
                    id_fixacao = (fixacao) sqlite3_column_int(stmt, 4);
                    short_return.soma_potencia = sqlite3_column_int64(stmt, 5);
                    short_return.valor_kw = sqlite3_column_double(stmt, 6);
                    break;
            }
        }

        kit(){}

        static string get_table(query_type select)
        {
            switch (select)
            {
                case query_type::SELECT_SHORT:
                    return "kits INNER JOIN modulos ON modulos.id_kit = kits.id GROUP BY kits.id";
                default:
                    return "kits";
            }
            return "kits";
        }
        static string get_fields(query_type select)
        {
            switch(select)
            {
                case query_type::SELECT_FULL:
                    return "id,"
                        "fornecedor,"
                        "valor,"
                        "tensao,"
                        "fixacao,"
                        "observacoes,"
                        "("
                        "    SELECT CONCAT("
                        "        '[',"
                        "        (SELECT GROUP_CONCAT("
                        "        '{\"id\": ' || m.id || ', ' ||"
                        "        '\"marca\": \"' || m.marca || '\", ' ||"
                        "        '\"potencia\": ' || m.potencia || ', ' ||"
                        "        '\"quantidade\": ' || m.quantidade || ', ' ||"
                        "        '\"comprimento\": ' || m.comprimento || ', ' ||"
                        "        '\"largura\": ' || m.largura || ', ' ||"
                        "        '\"peso\": ' || m.peso || ', ' ||"
                        "        '\"garantia_defeito\": ' || m.garantia_defeito || ', ' ||"
                        "        '\"garantia_eficiencia\": ' || m.garantia_eficiencia || '}'"
                        "        )),"
                        "        ']'"
                        "    )"
                        "    FROM modulos m"
                        "    WHERE m.id_kit = kits.id"
                        ") AS modulos,"
                        "("
                        "    SELECT CONCAT("
                        "        '[',"
                        "        (SELECT GROUP_CONCAT("
                        "        '{\"id\": ' || i.id || ', ' ||"
                        "        '\"marca\": \"' || i.marca || '\", ' ||"
                        "        '\"modelo\": \"' || i.modelo || '\", ' ||"
                        "        '\"potencia\": ' || i.potencia || ', ' ||"
                        "        '\"quantidade\": ' || i.quantidade || ', ' ||"
                        "        '\"garantia\": ' || i.garantia || ', ' ||"
                        "        '\"monitoramento\": ' || i.monitoramento || ', ' ||"
                        "        '\"microinversor\": ' || i.microinversor || '}'"
                        "        )),"
                        "        ']'"
                        "    )"
                        "    FROM inversores i"
                        "    WHERE i.id_kit = kits.id"
                        ") AS inversores,"
                        "("
                        "    SELECT CONCAT("
                        "        '[',"
                        "        (SELECT GROUP_CONCAT("
                        "        '{ \"id\": ' || it.id || ', ' ||"
                        "        '\"nome\": \"' || it.nome || '\", ' ||"
                        "        '\"fabricante\": \"' || it.fabricante || '\", ' ||"
                        "        '\"quantidade\": ' || it.quantidade || ', ' ||"
                        "        '\"detalhes\": \"' || it.detalhes || '\"}'"
                        "        )),"
                        "        ']'"
                        "    )"
                        "    FROM itens it"
                        "    WHERE it.id_kit = kits.id"
                        ") AS itens";
                case query_type::SELECT_SHORT:
                    return "kits.id, kits.fornecedor, kits.valor, kits.tensao, kits.fixacao, SUM(modulos.potencia * modulos.quantidade), kits.valor / SUM(modulos.potencia * modulos.quantidade)";
                default:                        
                    return "fornecedor, valor, tensao, fixacao, observacoes";
            }
        }
        static string get_template() { return "'{}', {}, {}, {}, '{}'"; }
    };

    #pragma endregion kit class

    #pragma region kit JSON serialization

    void to_json(nlohmann::json& j, const kit& obj)
    {
        j = 
        {
            { "id", obj.id }, { "fornecedor", obj.fornecedor }, { "valor", obj.valor },
            { "tensao", (int)obj.id_tensao }, { "fixacao", (int)obj.id_fixacao }, { "modulos", obj.modulos },
            { "inversores", obj.inversores }, { "itens", obj.itens }, { "observacoes", obj.observacoes },
            { "short_return", {
                { "soma_potencia", obj.short_return.soma_potencia },
                { "valor_kw", obj.short_return.valor_kw }
            }}
        };
    }

    void from_json(const nlohmann::json& j, kit& obj)
    {
        validate_from_json(j, "id", obj.id);            
        validate_from_json(j, "fornecedor", obj.fornecedor);
        validate_from_json(j, "valor", obj.valor);
        validate_from_json(j, "tensao", obj.id_tensao);
        validate_from_json(j, "fixacao", obj.id_fixacao);
        validate_from_json(j, "observacoes", obj.observacoes);
    }

    #pragma endregion kit JSON serialization

    #pragma region kit operations

    Napi::String get_all(const Napi::CallbackInfo& info)
    {
        Napi::Env env = info.Env();

        return Napi::String::New(env, crud::read<kit>(NULL, query_type::SELECT_SHORT));
    }

    Napi::String get(const Napi::CallbackInfo& info)
    {
        Napi::Env env = info.Env();

        if (info.Length() < 1 || !info[0].IsNumber())
            Napi::TypeError::New(env, "id::number")
                .ThrowAsJavaScriptException();

        string extra = fmt::format(" WHERE id = {}", info[0].As<Napi::Number>().Int32Value());
        return Napi::String::New(env, crud::read<kit>(&extra, query_type::SELECT_FULL));
    }

    Napi::Number create(const Napi::CallbackInfo& info)
    {
        Napi::Env env = info.Env();

        if (info.Length() < 1 || !info[0].IsString())
            Napi::TypeError::New(env, "fornecedor::String, id_tensao::Number, id_fixacao::Number, observacoes::String")
                .ThrowAsJavaScriptException();

        kit _kit = json::parse(remove_single_quotes(info[0].As<Napi::String>().Utf8Value(), DUPLICATE))
            .template get<kit>();
        
        json errs = json::array({});
        if (_kit.id_tensao < 0 || _kit.id_tensao > 2) errs.push_back(field_error("id_tensao", field_error_code::INVALID_FIELD));
        if (_kit.id_fixacao < 0 || _kit.id_fixacao > 13) errs.push_back(field_error("id_fixacao", field_error_code::INVALID_FIELD));
        
        if (errs.size() > 0)
        {
            Napi::Error::New(env, to_string(errs)).ThrowAsJavaScriptException();
            return Napi::Number::New(env, 0);
        }

        int64_t id = crud::create<kit>(_kit.fornecedor, _kit.valor, (int)_kit.id_tensao, (int)_kit.id_fixacao, _kit.observacoes);
        return Napi::Number::New(env, (double)id);
    }

    Napi::Boolean update(const Napi::CallbackInfo& info)
    {
        Napi::Env env = info.Env();

        if (info.Length() < 1 || !info[0].IsNumber())
            Napi::TypeError::New(env, "fornecedor::String, id_tensao::Number, id_fixacao::Number, observacoes::String")
                .ThrowAsJavaScriptException();

        kit _kit = json::parse(remove_single_quotes(info[0].As<Napi::String>().Utf8Value(), DUPLICATE))
            .template get<kit>();

        crud::update<kit>(_kit.id, field<string>("fornecedor", _kit.fornecedor), field<double>("valor", _kit.valor), field<int>("tensao", (int)_kit.id_tensao),
            field<int>("fixacao", _kit.id_fixacao), field<string>("observacoes", _kit.observacoes));
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
        if (validate_unique(errs, kit::get_table(DELETE), field<size_t>("id", id)))
        {
            Napi::Error::New(env, to_string(errs)).ThrowAsJavaScriptException();
            return Napi::Boolean::New(env, false);
        }
        crud::remove<kit>(field<size_t>("id", id));

        return Napi::Boolean::New(env, true);
    }

    #pragma endregion kit operations

    namespace modulos
    {
        #pragma region modulo class

        struct modulo
        {
            size_t id;
            size_t id_kit;
            int potencia;
            string marca;
            int quantidade;
            float peso;
            int comprimento;
            int largura;
            int garantia_defeito;
            int garantia_eficiencia;

            modulo(size_t _id, size_t _id_kit, int _potencia, string _marca, int _quantidade, float _peso, int _comprimento, int _largura, int _garantia_defeito, int _garantia_eficiencia)
                : id(_id), id_kit(_id_kit), potencia(_potencia), marca(_marca), quantidade(_quantidade), peso(_peso), comprimento(_comprimento), largura(_largura), garantia_defeito(_garantia_defeito), garantia_eficiencia(_garantia_eficiencia)
            {}

            modulo(size_t _id_kit, int _potencia, string _marca, int _quantidade, float _peso, int _comprimento, int _largura, int _garantia_defeito, int _garantia_eficiencia)
                : id_kit(_id_kit), potencia(_potencia), marca(_marca), quantidade(_quantidade), peso(_peso), comprimento(_comprimento), largura(_largura), garantia_defeito(_garantia_defeito), garantia_eficiencia(_garantia_eficiencia)
            {}

            modulo(query_type _,sqlite3_stmt* stmt)
            {
                id = sqlite3_column_int64(stmt, 0);
                id_kit = sqlite3_column_int(stmt, 1);
                potencia = sqlite3_column_int(stmt, 2);
                marca = reinterpret_cast<char*>(const_cast<unsigned char*>(sqlite3_column_text(stmt, 3)));
                quantidade = sqlite3_column_int(stmt, 4);
                peso = (float)sqlite3_column_double(stmt, 5);
                comprimento = sqlite3_column_int(stmt, 6);
                largura = sqlite3_column_int(stmt, 7);
                garantia_defeito = sqlite3_column_int(stmt, 8);
                garantia_eficiencia = sqlite3_column_int(stmt, 9);
            }

            modulo(){}

            static string get_table(bool select) { return "modulos"; }
            static string get_fields(bool select)
            {
                if (select) return "*";
                return "id_kit, potencia, marca, quantidade, peso, comprimento, largura, garantia_defeito, garantia_eficiencia";
            }
            static string get_template() { return "{}, {}, '{}', {}, {}, {}, {}, {}, {}"; }
        };
        
        #pragma endregion modulo class

        #pragma region modulo JSON serialization

        void to_json(nlohmann::json& j, const modulo& obj)
        {
            j = 
            {
                { "id", obj.id }, { "id_kit", obj.id_kit }, { "potencia", obj.potencia },
                { "marca", obj.marca }, { "quantidade", obj.quantidade }, { "peso", obj.peso },
                { "comprimento", obj.comprimento }, { "largura", obj.largura }, { "garantia_defeito", obj.garantia_eficiencia },
                { "garantia_eficiencia", obj.garantia_eficiencia }
            };
        }

        void from_json(const nlohmann::json& j, modulo& obj)
        {
            validate_from_json(j, "id", obj.id);
            validate_from_json(j, "id_kit", obj.id_kit);
            validate_from_json(j, "potencia", obj.potencia);
            validate_from_json(j, "marca", obj.marca);
            validate_from_json(j, "quantidade", obj.quantidade);
            validate_from_json(j, "peso", obj.peso);
            validate_from_json(j, "comprimento", obj.comprimento);
            validate_from_json(j, "largura", obj.largura);
            validate_from_json(j, "garantia_defeito", obj.garantia_defeito);
            validate_from_json(j, "garantia_eficiencia", obj.garantia_eficiencia);
        }

        #pragma endregion modulo JSON serialization

        #pragma region modulo operations

        Napi::String get_all(const Napi::CallbackInfo& info)
        {
            Napi::Env env = info.Env();

            return Napi::String::New(env, crud::read<modulo>());
        }

        Napi::String get_by_kit(const Napi::CallbackInfo& info)
        {
            Napi::Env env = info.Env();

            if (info.Length() < 1 || !info[0].IsNumber())
                Napi::TypeError::New(env, "id::number")
                    .ThrowAsJavaScriptException();

            string extra = fmt::format(" WHERE id_kit = {}", info[0].As<Napi::Number>().Int32Value());
            return Napi::String::New(env, crud::read<modulo>(&extra));
        }

        Napi::String get(const Napi::CallbackInfo& info)
        {
            Napi::Env env = info.Env();

            if (info.Length() < 1 || !info[0].IsNumber())
                Napi::TypeError::New(env, "id::number")
                    .ThrowAsJavaScriptException();

            string extra = fmt::format(" WHERE id = {}", info[0].As<Napi::Number>().Int32Value());
            return Napi::String::New(env, crud::read<modulo>(&extra));
        }

        Napi::Number create(const Napi::CallbackInfo& info)
        {
            Napi::Env env = info.Env();

            if (info.Length() < 1 || !info[0].IsNumber())
                Napi::TypeError::New(env, "id_kit::Number, potencia::Number, marca::String, quantidade::Number, peso::Number, comprimento::Number, largura::Number, garantia_defeito::Number, garantia_eficiencia::Number")
                    .ThrowAsJavaScriptException();

            modulo _modulo = json::parse(remove_single_quotes(info[0].As<Napi::String>().Utf8Value(), DUPLICATE))
                .template get<modulo>();

            int64_t id = crud::create<modulo>(_modulo.id_kit, _modulo.potencia, _modulo.marca, _modulo.quantidade, _modulo.peso, _modulo.comprimento, _modulo.largura, _modulo.garantia_defeito, _modulo.garantia_eficiencia);
            return Napi::Number::New(env, (double)id);
        }

        Napi::Boolean update(const Napi::CallbackInfo& info)
        {
            Napi::Env env = info.Env();

            if (info.Length() < 1 || !info[0].IsNumber())
                Napi::TypeError::New(env, "id::Number, potencia::Number, marca::String, quantidade::Number, peso::Number, comprimento::Number, largura::Number, garantia_defeito::Number, garantia_eficiencia::Number")
                    .ThrowAsJavaScriptException();

            modulo _modulo = json::parse(remove_single_quotes(info[0].As<Napi::String>().Utf8Value(), DUPLICATE))
                .template get<modulo>();
            
            crud::update<modulo>(_modulo.id, field<int>("potencia", _modulo.potencia),
                field<string>("marca", _modulo.marca), field<int>("quantidade", _modulo.quantidade), field<float>("peso", _modulo.peso),
                field<int>("comprimento", _modulo.comprimento), field<int>("largura", _modulo.largura), field<int>("garantia_defeito", _modulo.garantia_defeito),
                field<int>("garantia_eficiencia", _modulo.garantia_eficiencia));
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
            if (validate_unique(errs, modulo::get_table(DELETE), field<size_t>("id", id)))
            {
                Napi::Error::New(env, to_string(errs)).ThrowAsJavaScriptException();
                return Napi::Boolean::New(env, false);
            }
            crud::remove<modulo>(field<size_t>("id", id));

            return Napi::Boolean::New(env, true);
        }

        Napi::Boolean remove_by_kit(const Napi::CallbackInfo& info)
        {
            Napi::Env env = info.Env();

            if (info.Length() < 1 || !info[0].IsNumber())
                Napi::TypeError::New(env, "id::number")
                    .ThrowAsJavaScriptException();

            json errs = json::array({});
            size_t id = info[0].As<Napi::Number>().Int32Value();
            if (validate_unique(errs, modulo::get_table(DELETE), field<size_t>("id_kit", id)))
            {
                Napi::Error::New(env, to_string(errs)).ThrowAsJavaScriptException();
                return Napi::Boolean::New(env, false);
            }
            crud::remove<modulo>(field<size_t>("id_kit", id));

            return Napi::Boolean::New(env, true);
        }

        #pragma endregion modulo operations
    }

    namespace inversores
    {
        #pragma region inversor class

        enum monitoramento
        {
            WIFI,
            _4G,
            NENHUM
        };

        struct inversor
        {
            size_t id;
            size_t id_kit;
            string marca;
            string modelo;
            monitoramento id_monitoramento;
            int potencia;
            int quantidade;
            int garantia;
            bool microinversor;

            inversor(size_t _id, size_t _id_kit, string _marca, string _modelo, size_t _id_monitoramento, int _potencia, int _quantidade, int _garantia, bool _microinversor)
                : id(_id), id_kit(_id_kit), marca(_marca), modelo(_modelo), id_monitoramento((monitoramento)_id_monitoramento), potencia(_potencia), quantidade(_quantidade), garantia(_garantia), microinversor(_microinversor)
            {}

            inversor(size_t _id_kit, string _marca, string _modelo, int _id_monitoramento, int _potencia, int _quantidade, int _garantia, bool _microinversor)
                : id_kit(_id_kit), marca(_marca), modelo(_modelo), id_monitoramento((monitoramento)_id_monitoramento), potencia(_potencia), quantidade(_quantidade), garantia(_garantia), microinversor(_microinversor)
            {}

            inversor(query_type _,sqlite3_stmt* stmt)
            {
                id = sqlite3_column_int64(stmt, 0);
                id_kit = sqlite3_column_int(stmt, 1);
                marca = reinterpret_cast<char*>(const_cast<unsigned char*>(sqlite3_column_text(stmt, 2)));
                modelo = reinterpret_cast<char*>(const_cast<unsigned char*>(sqlite3_column_text(stmt, 3)));
                id_monitoramento = (monitoramento)sqlite3_column_int(stmt, 4);
                potencia = sqlite3_column_int(stmt, 5);
                quantidade = sqlite3_column_int(stmt, 6);
                garantia = sqlite3_column_int(stmt, 7);
                microinversor = sqlite3_column_int(stmt, 8);
            }

            inversor(){}

            static string get_table(bool select) { return "inversores"; }
            static string get_fields(bool select)
            {
                if (select) return "*";
                return "id_kit, marca, modelo, monitoramento, potencia, quantidade, garantia, microinversor";
            }
            static string get_template() { return "{}, '{}', '{}', {}, {}, {}, {}, {}"; }
        };
        
        #pragma endregion inversor class

        #pragma region inversor JSON serialization

        void to_json(nlohmann::json& j, const inversor& obj)
        {
            j = 
            {
                { "id", obj.id }, { "id_kit", obj.id_kit }, { "marca", obj.marca },
                { "modelo", obj.modelo }, { "monitoramento", obj.id_monitoramento }, { "potencia", obj.potencia },
                { "quantidade", obj.quantidade }, { "garantia", obj.garantia }, { "microinversor", obj.microinversor }
            };
        }

        void from_json(const nlohmann::json& j, inversor& obj)
        {
            validate_from_json(j, "id", obj.id);
            validate_from_json(j, "id_kit", obj.id_kit);
            validate_from_json(j, "marca", obj.marca);
            validate_from_json(j, "modelo", obj.modelo);
            validate_from_json(j, "monitoramento", obj.id_monitoramento);
            validate_from_json(j, "potencia", obj.potencia);
            validate_from_json(j, "quantidade", obj.quantidade);
            validate_from_json(j, "garantia", obj.garantia);
            validate_from_json(j, "microinversor", obj.microinversor);
        }

        #pragma endregion inversor JSON serialization

        #pragma region inversor operations

        Napi::String get_all(const Napi::CallbackInfo& info)
        {
            Napi::Env env = info.Env();

            return Napi::String::New(env, crud::read<inversor>());
        }

        Napi::String get_by_kit(const Napi::CallbackInfo& info)
        {
            Napi::Env env = info.Env();

            if (info.Length() < 1 || !info[0].IsNumber())
                Napi::TypeError::New(env, "id::number")
                    .ThrowAsJavaScriptException();

            string extra = fmt::format(" WHERE id_kit = {}", info[0].As<Napi::Number>().Int32Value());
            return Napi::String::New(env, crud::read<inversor>(&extra));
        }

        Napi::String get(const Napi::CallbackInfo& info)
        {
            Napi::Env env = info.Env();

            if (info.Length() < 1 || !info[0].IsNumber())
                Napi::TypeError::New(env, "id::number")
                    .ThrowAsJavaScriptException();

            string extra = fmt::format(" WHERE id = {}", info[0].As<Napi::Number>().Int32Value());
            return Napi::String::New(env, crud::read<inversor>(&extra));
        }

        Napi::Number create(const Napi::CallbackInfo& info)
        {
            Napi::Env env = info.Env();

            if (info.Length() < 1 || !info[0].IsString())
                Napi::TypeError::New(env, "id_kit::Number, marca::String, modelo::String, id_monitoramento::Number, potencia::Number, quantidade::Number, garantia::Number, microinversor::Number")
                    .ThrowAsJavaScriptException();

            inversor _inversor = json::parse(remove_single_quotes(info[0].As<Napi::String>().Utf8Value(), DUPLICATE))
                .template get<inversor>();

            int64_t id = crud::create<inversor>(_inversor.id_kit, _inversor.marca, _inversor.modelo, (int)_inversor.id_monitoramento, _inversor.potencia, _inversor.quantidade, _inversor.garantia, (int)_inversor.microinversor);
            return Napi::Number::New(env, (double)id);
        }

        Napi::Boolean update(const Napi::CallbackInfo& info)
        {
            Napi::Env env = info.Env();

            if (info.Length() < 1 || !info[0].IsString())
                Napi::TypeError::New(env, "id::Number, marca::String, modelo::String, id_monitoramento::Number, potencia::Number, quantidade::Number, garantia::Number, microinversor::Boolean")
                    .ThrowAsJavaScriptException();

            inversor _inversor = json::parse(remove_single_quotes(info[0].As<Napi::String>().Utf8Value(), DUPLICATE))
                .template get<inversor>();

            crud::update<inversor>(_inversor.id, field<string>("marca", _inversor.marca),
                field<string>("modelo", _inversor.modelo), field<int>("id_monitoramento", (int)_inversor.id_monitoramento), field<int>("potencia", _inversor.potencia),
                field<int>("quantidade", _inversor.quantidade), field<int>("garantia", _inversor.garantia), field<int>("microinversor", _inversor.microinversor ? 1 : 0));
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
            if (validate_unique(errs, inversor::get_table(DELETE), field<size_t>("id", id)))
            {
                Napi::Error::New(env, to_string(errs)).ThrowAsJavaScriptException();
                return Napi::Boolean::New(env, false);
            }
            crud::remove<inversor>(field<size_t>("id", id));

            return Napi::Boolean::New(env, true);
        }

        Napi::Boolean remove_by_kit(const Napi::CallbackInfo& info)
        {
            Napi::Env env = info.Env();

            if (info.Length() < 1 || !info[0].IsNumber())
                Napi::TypeError::New(env, "id::number")
                    .ThrowAsJavaScriptException();

            json errs = json::array({});
            size_t id = info[0].As<Napi::Number>().Int32Value();
            if (validate_unique(errs, inversor::get_table(DELETE), field<size_t>("id", id)))
            {
                Napi::Error::New(env, to_string(errs)).ThrowAsJavaScriptException();
                return Napi::Boolean::New(env, false);
            }
            crud::remove<inversor>(field<size_t>("id_kit", id));

            return Napi::Boolean::New(env, true);
        }

        #pragma endregion inversor operations
    }

    namespace itens
    {
        #pragma region item class

        struct item
        {
            size_t id;
            size_t id_kit;
            string nome;
            int quantidade;
            string fabricante;
            string detalhes;

            item(size_t _id, size_t _id_kit, string _nome, int _quantidade, string _fabricante, string _detalhes)
                : id(_id), id_kit(_id_kit), nome(_nome), quantidade(_quantidade), fabricante(_fabricante), detalhes(_detalhes)
            {}

            item(size_t _id_kit, string _nome, int _quantidade, string _fabricante, string _detalhes)
                : id_kit(_id_kit), nome(_nome), quantidade(_quantidade), fabricante(_fabricante), detalhes(_detalhes)
            {}

            item(query_type _,sqlite3_stmt* stmt)
            {
                id = sqlite3_column_int64(stmt, 0);
                id_kit = sqlite3_column_int(stmt, 1);
                nome = reinterpret_cast<char*>(const_cast<unsigned char*>(sqlite3_column_text(stmt, 2)));
                quantidade = sqlite3_column_int(stmt, 3);
                fabricante = reinterpret_cast<char*>(const_cast<unsigned char*>(sqlite3_column_text(stmt, 4)));
                detalhes = reinterpret_cast<char*>(const_cast<unsigned char*>(sqlite3_column_text(stmt, 5)));
            }

            item(){}

            static string get_table(bool select) { return "itens"; }
            static string get_fields(bool select)
            {
                if (select) return "*";
                return "id_kit, nome, quantidade, fabricante, detalhes";
            }
            static string get_template() { return "{}, '{}', {}, '{}', '{}'"; }
        };
        
        #pragma endregion item class

        #pragma region item JSON serialization

        void to_json(nlohmann::json& j, const item& obj)
        {
            j = 
            {
                { "id", obj.id }, { "id_kit", obj.id_kit }, { "nome", obj.nome },
                { "quantidade", obj.quantidade }, { "fabricante", obj.fabricante }, { "detalhes", obj.detalhes }
            };
        }

        void from_json(const nlohmann::json& j, item& obj)
        {
            validate_from_json(j, "id", obj.id);
            validate_from_json(j, "id_kit", obj.id_kit);
            validate_from_json(j, "nome", obj.nome);
            validate_from_json(j, "quantidade", obj.quantidade);
            validate_from_json(j, "fabricante", obj.fabricante);
            validate_from_json(j, "detalhes", obj.detalhes);
        }

        #pragma endregion item JSON serialization

        #pragma region item operations

        Napi::String get_all(const Napi::CallbackInfo& info)
        {
            Napi::Env env = info.Env();

            return Napi::String::New(env, crud::read<item>());
        }

        Napi::String get_by_kit(const Napi::CallbackInfo& info)
        {
            Napi::Env env = info.Env();

            if (info.Length() < 1 || !info[0].IsNumber())
                Napi::TypeError::New(env, "id::number")
                    .ThrowAsJavaScriptException();

            string extra = fmt::format(" WHERE id_kit = {}", info[0].As<Napi::Number>().Int32Value());
            return Napi::String::New(env, crud::read<item>(&extra));
        }

        Napi::String get(const Napi::CallbackInfo& info)
        {
            Napi::Env env = info.Env();

            if (info.Length() < 1 || !info[0].IsNumber())
                Napi::TypeError::New(env, "id::number")
                    .ThrowAsJavaScriptException();

            string extra = fmt::format(" WHERE id = {}", info[0].As<Napi::Number>().Int32Value());
            return Napi::String::New(env, crud::read<item>(&extra));
        }

        Napi::Number create(const Napi::CallbackInfo& info)
        {
            Napi::Env env = info.Env();

            if (info.Length() < 1 || !info[0].IsString())
                Napi::TypeError::New(env, "id_kit::Number, nome::String, quantidade::Number, fabricante::String, detalhes::String")
                    .ThrowAsJavaScriptException();

            item _item = json::parse(remove_single_quotes(info[0].As<Napi::String>().Utf8Value(), DUPLICATE))
                .template get<item>();

            int64_t id = crud::create<item>(_item.id_kit, _item.nome, _item.quantidade, _item.fabricante, _item.detalhes);
            return Napi::Number::New(env, (double)id);
        }

        Napi::Boolean update(const Napi::CallbackInfo& info)
        {
            Napi::Env env = info.Env();

            if (info.Length() < 1 || !info[0].IsString())
                Napi::TypeError::New(env, "id::Number, nome::String, quantidade::Number, fabricante::String, detalhes::String")
                    .ThrowAsJavaScriptException();

            item _item = json::parse(remove_single_quotes(info[0].As<Napi::String>().Utf8Value(), DUPLICATE))
                .template get<item>();

            crud::update<item>(_item.id, field<string>("nome", _item.nome), field<int>("quantidade", _item.quantidade),
                field<string>("fabricante", _item.fabricante), field<string>("detalhes", _item.detalhes));
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
            if (validate_unique(errs, item::get_table(DELETE), field<size_t>("id", id)))
            {
                Napi::Error::New(env, to_string(errs)).ThrowAsJavaScriptException();
                return Napi::Boolean::New(env, false);
            }
            crud::remove<item>(field<size_t>("id", id));

            return Napi::Boolean::New(env, true);
        }

        Napi::Boolean remove_by_kit(const Napi::CallbackInfo& info)
        {
            Napi::Env env = info.Env();

            if (info.Length() < 1 || !info[0].IsNumber())
                Napi::TypeError::New(env, "id::number")
                    .ThrowAsJavaScriptException();

            json errs = json::array({});
            size_t id = info[0].As<Napi::Number>().Int32Value();
            if (validate_unique(errs, item::get_table(DELETE), field<size_t>("id", id)))
            {
                Napi::Error::New(env, to_string(errs)).ThrowAsJavaScriptException();
                return Napi::Boolean::New(env, false);
            }
            crud::remove<item>(field<size_t>("id_kit", id));

            return Napi::Boolean::New(env, true);
        }

        #pragma endregion item operations
    }
}

#endif //INCLUDE_KITS_
