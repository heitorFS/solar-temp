#ifndef INCLUDE_CLIENTES_
#define INCLUDE_CLIENTES_

#include "server_object.hpp"

#include "colaboradores.hpp"
#include "origens.hpp"

using namespace std;
using json = nlohmann::json;
using colaborador = colaboradores::colaborador;
using origem = origens::origem;

namespace clientes
{
    #pragma region class
    
    struct cliente
    {
        size_t id = 0;
        string nome;
        string email;
        colaborador proprietario;
        string cpf_cnpj;
        string telefone;
        string cep;
        string endereco;
        int64_t numero;
        string complemento;
        origem cliente_origem;
        string data_origem;
        string extra_nome;
        string extra_cpf;
        string extra_rg;
        string extra_nacionalidade;
        string extra_profissao;
        double extra_renda;
        string observacoes;

        cliente(size_t _id, string _nome, string _email, size_t id_proprietario, string _cpf_cnpj, string _telefone, string _cep, string _endereco, int64_t _numero, string _complemento, size_t id_origem, string _data_origem, string _extra_nome, string _extra_cpf, string _extra_rg, string _extra_nacionalidade, string _extra_profissao, double _extra_renda, string _observacoes)
        : id(_id), nome(_nome), email(_email), cpf_cnpj(_cpf_cnpj), telefone(_telefone), cep(_cep), endereco(_endereco), numero(_numero), complemento(_complemento), data_origem(_data_origem), extra_nome(_extra_nome), extra_cpf(_extra_cpf), extra_rg(_extra_rg), extra_nacionalidade(_extra_nacionalidade), extra_profissao(_extra_profissao), extra_renda(_extra_renda), observacoes(_observacoes)
        {
            proprietario = colaborador();
            proprietario.id = id_proprietario;
            cliente_origem = origem();
            cliente_origem.id = id_origem;
        }

        cliente(string _nome, string _email, size_t id_proprietario, string _cpf_cnpj, string _telefone, string _cep, string _endereco, int64_t _numero, string _complemento, size_t id_origem, string _data_origem, string _extra_nome, string _extra_cpf, string _extra_rg, string _extra_nacionalidade, string _extra_profissao, double _extra_renda, string _observacoes)
        : nome(_nome), email(_email), cpf_cnpj(_cpf_cnpj), telefone(_telefone), cep(_cep), endereco(_endereco), numero(_numero), complemento(_complemento), data_origem(_data_origem), extra_nome(_extra_nome), extra_cpf(_extra_cpf), extra_rg(_extra_rg), extra_nacionalidade(_extra_nacionalidade), extra_profissao(_extra_profissao), extra_renda(_extra_renda), observacoes(_observacoes)
        {
            proprietario = colaborador();
            proprietario.id = id_proprietario;
            cliente_origem = origem();
            cliente_origem.id = id_origem;
        }

        cliente(query_type type, sqlite3_stmt* stmt)
        {
            proprietario = colaborador();
            cliente_origem = origem();
            
            switch (type)
            {
                case query_type::SELECT_FULL:
                    id = sqlite3_column_int64(stmt, 0);
                    nome = reinterpret_cast<char*>(const_cast<unsigned char*>(sqlite3_column_text(stmt, 1)));
                    email = reinterpret_cast<char*>(const_cast<unsigned char*>(sqlite3_column_text(stmt, 2)));
                    cpf_cnpj = reinterpret_cast<char*>(const_cast<unsigned char*>(sqlite3_column_text(stmt, 3)));
                    telefone = reinterpret_cast<char*>(const_cast<unsigned char*>(sqlite3_column_text(stmt, 4)));
                    cep = reinterpret_cast<char*>(const_cast<unsigned char*>(sqlite3_column_text(stmt, 5)));
                    endereco = reinterpret_cast<char*>(const_cast<unsigned char*>(sqlite3_column_text(stmt, 6)));
                    numero = sqlite3_column_int64(stmt, 7);
                    complemento = reinterpret_cast<char*>(const_cast<unsigned char*>(sqlite3_column_text(stmt, 8)));
                    data_origem = reinterpret_cast<char*>(const_cast<unsigned char*>(sqlite3_column_text(stmt, 9)));
                    extra_nome = reinterpret_cast<char*>(const_cast<unsigned char*>(sqlite3_column_text(stmt, 10)));
                    extra_cpf = reinterpret_cast<char*>(const_cast<unsigned char*>(sqlite3_column_text(stmt, 11)));
                    extra_rg = reinterpret_cast<char*>(const_cast<unsigned char*>(sqlite3_column_text(stmt, 12)));
                    extra_nacionalidade = reinterpret_cast<char*>(const_cast<unsigned char*>(sqlite3_column_text(stmt, 13)));
                    extra_profissao = reinterpret_cast<char*>(const_cast<unsigned char*>(sqlite3_column_text(stmt, 14)));
                    extra_renda = sqlite3_column_double(stmt, 15);
                    observacoes = reinterpret_cast<char*>(const_cast<unsigned char*>(sqlite3_column_text(stmt, 16)));
                    proprietario.id = sqlite3_column_int64(stmt, 17);
                    proprietario.nome = reinterpret_cast<char*>(const_cast<unsigned char*>(sqlite3_column_text(stmt, 18)));
                    proprietario.id_cargo = (colaboradores::cargo)sqlite3_column_int(stmt, 19);
                    proprietario.email = reinterpret_cast<char*>(const_cast<unsigned char*>(sqlite3_column_text(stmt, 20)));
                    proprietario.telefone = reinterpret_cast<char*>(const_cast<unsigned char*>(sqlite3_column_text(stmt, 21)));
                    proprietario.cpf_cnpj = reinterpret_cast<char*>(const_cast<unsigned char*>(sqlite3_column_text(stmt, 22)));
                    cliente_origem.id = sqlite3_column_int64(stmt, 23);
                    cliente_origem.nome = reinterpret_cast<char*>(const_cast<unsigned char*>(sqlite3_column_text(stmt, 24)));
                    cliente_origem.origem_companhia.id = sqlite3_column_int64(stmt, 25);
                    break;                
                default:
                    id = sqlite3_column_int64(stmt, 0);
                    nome = reinterpret_cast<char*>(const_cast<unsigned char*>(sqlite3_column_text(stmt, 1)));
                    email = reinterpret_cast<char*>(const_cast<unsigned char*>(sqlite3_column_text(stmt, 2)));
                    telefone = reinterpret_cast<char*>(const_cast<unsigned char*>(sqlite3_column_text(stmt, 3)));
                    proprietario.nome = reinterpret_cast<char*>(const_cast<unsigned char*>(sqlite3_column_text(stmt, 4)));
                    cliente_origem.nome = reinterpret_cast<char*>(const_cast<unsigned char*>(sqlite3_column_text(stmt, 5)));
                    break;
            }
        }

        cliente(){}

        static string get_table(bool select) { if (select) return "(clientes INNER JOIN colaboradores ON clientes.id_proprietario = colaboradores.id) INNER JOIN origens ON clientes.id_origem = origens.id"; return "clientes"; }
        static string get_fields(query_type select)
        {
            switch (select)
            {
                case query_type::SELECT_FULL:
                    return "clientes.id, clientes.nome, clientes.email, clientes.cpf_cnpj, clientes.telefone, clientes.cep, clientes.endereco, clientes.numero, clientes.complemento, clientes.data_origem, clientes.extra_nome, clientes.extra_cpf, clientes.extra_rg, clientes.extra_nacionalidade, clientes.extra_profissao, clientes.extra_renda, clientes.observacoes, colaboradores.id, colaboradores.nome, colaboradores.id_cargo, colaboradores.email, colaboradores.telefone, colaboradores.cpf_cnpj, origens.id, origens.nome, origens.id_companhia";
                case query_type::SELECT_SHORT:
                    return "clientes.id, clientes.nome, clientes.email, clientes.telefone, colaboradores.nome, origens.nome";
                default:
                    return "nome, email, id_proprietario, cpf_cnpj, telefone, cep, endereco, numero, complemento, id_origem, data_origem, extra_nome, extra_cpf, extra_rg, extra_nacionalidade, extra_profissao, extra_renda, observacoes";
            }
        }
        static string get_template() { return "'{}', '{}', {}, '{}', '{}', '{}', '{}', {}, '{}', {}, '{}', '{}', '{}', '{}', '{}', '{}', {}, '{}'"; }
    };

    #pragma endregion class

    #pragma region JSON serialization
        
    void to_json(nlohmann::json& j, cliente& obj)
    {
        j = 
        {
            { "id", obj.id }, { "nome", obj.nome },  { "email", obj.email },
            { "proprietario",{
                { "id", obj.proprietario.id }, { "nome", obj.proprietario.nome }, { "id_cargo", obj.proprietario.id_cargo }, { "email", obj.proprietario.email }, { "telefone", obj.proprietario.telefone }, { "cpf_cnpj", obj.proprietario.cpf_cnpj }
            }}, { "cpf_cnpj", obj.cpf_cnpj }, { "telefone", obj.telefone },
            { "cep", obj.cep }, { "endereco", obj.endereco}, { "numero", obj.numero }, { "complemento", obj.complemento },
            { "origem", {
                { "id", obj.cliente_origem.id }, { "nome", obj.cliente_origem.nome }, {"id_companhia", obj.cliente_origem.origem_companhia.id}
            }}, { "data_origem", obj.data_origem }, { "extra_nome", obj.extra_nome },
            { "extra_cpf", obj.extra_cpf }, { "extra_rg", obj.extra_rg }, { "extra_nacionalidade", obj.extra_nacionalidade },
            { "extra_profissao", obj.extra_profissao }, { "extra_renda", obj.extra_renda }, { "observacoes", obj.observacoes }
        };
    }

    void from_json(const nlohmann::json& j, cliente& obj)
    {
        if (j.contains("id"))
            j.at("id").get_to(obj.id);
        j.at("nome").get_to(obj.nome);
        j.at("email").get_to(obj.email);
        j.at("proprietario").get_to(obj.proprietario.id);
        j.at("cpf_cnpj").get_to(obj.cpf_cnpj);
        j.at("telefone").get_to(obj.telefone);
        j.at("cep").get_to(obj.cep);
        j.at("endereco").get_to(obj.endereco);
        j.at("numero").get_to(obj.numero);
        j.at("complemento").get_to(obj.complemento);
        j.at("origem").get_to(obj.cliente_origem.id);
        j.at("data_origem").get_to(obj.data_origem);
        j.at("extra_nome").get_to(obj.extra_nome);
        j.at("extra_cpf").get_to(obj.extra_cpf);
        j.at("extra_rg").get_to(obj.extra_rg);
        j.at("extra_nacionalidade").get_to(obj.extra_nacionalidade);
        j.at("extra_profissao").get_to(obj.extra_profissao);
        j.at("extra_renda").get_to(obj.extra_renda);
        j.at("observacoes").get_to(obj.observacoes);
    }

    #pragma endregion JSON serialization

    #pragma region operations

    Napi::String get_all_short(const Napi::CallbackInfo& info)
    {
        Napi::Env env = info.Env();
        return Napi::String::New(env, crud::read<cliente>(NULL, query_type::SELECT_SHORT));
    }

    Napi::String get_all(const Napi::CallbackInfo& info)
    {
        Napi::Env env = info.Env();

        return Napi::String::New(env, crud::read<cliente>());
    }

    Napi::String get(const Napi::CallbackInfo& info)
    {
        Napi::Env env = info.Env();

        if (info.Length() < 1 || !info[0].IsNumber())
            Napi::TypeError::New(env, "id::number")
                .ThrowAsJavaScriptException();

        string extra = fmt::format(" WHERE clientes.id = {}", info[0].As<Napi::Number>().Int32Value());
        return Napi::String::New(env, crud::read<cliente>(&extra));
    }

    Napi::Number create(const Napi::CallbackInfo& info)
    {
        Napi::Env env = info.Env();

        if (info.Length() < 1 || !info[0].IsString())
            Napi::TypeError::New(env, "nome::String, email::String, id_proprietario::Number, cpf_cnpj::String, telefone::String, cep::String, numero::Number, complemento::String, id_origem::Number, data_origem::String, extra_nome::String, extra_cpf::String, extra_rg::String, extra_nacionalidade::String, extra_profissao::String, extra_renda::Number, observacoes::String expected")
                .ThrowAsJavaScriptException();

        cliente _cliente = json::parse(remove_single_quotes(info[0].As<Napi::String>().Utf8Value(), DUPLICATE))
            .template get<cliente>();

        json errs = json::array({});
        if (!validate_letters(_cliente.nome)) errs.push_back(field_error("nome", field_error_code::INVALID_FIELD));
        if (!validate_email(_cliente.email)) errs.push_back(field_error("email", field_error_code::INVALID_FIELD));
        if (!validate_numbers(_cliente.cpf_cnpj) || !cpfcnpj_validacao::validate_cpfcnpj(_cliente.cpf_cnpj.c_str())) errs.push_back(field_error("cpf_cnpj", field_error_code::INVALID_FIELD));
        if (!validate_numbers(_cliente.telefone)) errs.push_back(field_error("telefone", field_error_code::INVALID_FIELD));
        if (!validate_numbers(_cliente.cep)) errs.push_back(field_error("cep", field_error_code::INVALID_FIELD));
        if (!validate_letters(_cliente.extra_nome)) errs.push_back(field_error("extra_nome", field_error_code::INVALID_FIELD));
        if (_cliente.extra_cpf.length() > 0 && (!validate_numbers(_cliente.extra_cpf) || !cpfcnpj_validacao::validate_cpfcnpj(_cliente.extra_cpf.c_str()))) errs.push_back(field_error("extra_cpf", field_error_code::INVALID_FIELD));
        if (!validate_numbers(_cliente.extra_rg)) errs.push_back(field_error("extra_rg", field_error_code::INVALID_FIELD));
        if (!validate_letters(_cliente.extra_nacionalidade)) errs.push_back(field_error("extra_nacionalidade", field_error_code::INVALID_FIELD));
        if (!validate_letters(_cliente.extra_profissao)) errs.push_back(field_error("extra_profissao", field_error_code::INVALID_FIELD));

        validate_unique(errs, cliente::get_table(VALIDATE), _cliente.id, field<string>("cpf_cnpj", _cliente.cpf_cnpj));
        
        if (errs.size() > 0)
        {
            Napi::Error::New(env, to_string(errs)).ThrowAsJavaScriptException();
            return Napi::Number::New(env, -1);
        }

        int64_t id = crud::create<cliente>(_cliente.nome, _cliente.email, _cliente.proprietario.id, _cliente.cpf_cnpj, _cliente.telefone, _cliente.cep, _cliente.endereco, _cliente.numero, _cliente.complemento, _cliente.cliente_origem.id, _cliente.data_origem, _cliente.extra_nome, _cliente.extra_cpf, _cliente.extra_rg, _cliente.extra_nacionalidade, _cliente.extra_profissao, _cliente.extra_renda, _cliente.observacoes);
        return Napi::Number::New(env, (double)id);
    }

    Napi::Boolean update(const Napi::CallbackInfo& info)
    {
        Napi::Env env = info.Env();

        if (info.Length() < 1 || !info[0].IsString())
            Napi::TypeError::New(env, "nome::String, email::String, id_proprietario::Number, cpf_cnpj::String, telefone::String, cep::String, endereco::String, numero::Number, complemento::String, id_origem::Number, data_origem::String, extra_nome::String, extra_cpf::String, extra_rg::String, extra_nacionalidade::String, extra_profissao::String, extra_renda::Number, observacoes::String expected")
                .ThrowAsJavaScriptException();

        cliente _cliente = json::parse(remove_single_quotes(info[0].As<Napi::String>().Utf8Value(), DUPLICATE))
            .template get<cliente>();

        json errs = json::array({});
        if (!validate_letters(_cliente.nome)) errs.push_back(field_error("nome", field_error_code::INVALID_FIELD));
        if (!validate_email(_cliente.email)) errs.push_back(field_error("email", field_error_code::INVALID_FIELD));
        if (!validate_numbers(_cliente.cpf_cnpj) || !cpfcnpj_validacao::validate_cpfcnpj(_cliente.cpf_cnpj.c_str())) errs.push_back(field_error("cpf_cnpj", field_error_code::INVALID_FIELD));
        if (!validate_numbers(_cliente.telefone)) errs.push_back(field_error("telefone", field_error_code::INVALID_FIELD));
        if (!validate_numbers(_cliente.cep)) errs.push_back(field_error("cep", field_error_code::INVALID_FIELD));
        if (!validate_letters(_cliente.extra_nome)) errs.push_back(field_error("extra_nome", field_error_code::INVALID_FIELD));
        if (_cliente.extra_cpf.length() > 0 && (!validate_numbers(_cliente.extra_cpf) || !cpfcnpj_validacao::validate_cpfcnpj(_cliente.extra_cpf.c_str()))) errs.push_back(field_error("extra_cpf", field_error_code::INVALID_FIELD));
        if (!validate_numbers(_cliente.extra_rg)) errs.push_back(field_error("extra_rg", field_error_code::INVALID_FIELD));
        if (!validate_letters(_cliente.extra_nacionalidade)) errs.push_back(field_error("extra_nacionalidade", field_error_code::INVALID_FIELD));
        if (!validate_letters(_cliente.extra_profissao)) errs.push_back(field_error("extra_profissao", field_error_code::INVALID_FIELD));

        validate_unique(errs, cliente::get_table(INSERT), 0, field<string>("cpf_cnpj", _cliente.cpf_cnpj));
        
        if (errs.size() > 0)
        {
            Napi::Error::New(env, to_string(errs)).ThrowAsJavaScriptException();
            return Napi::Boolean::New(env, false);
        }

        crud::update<cliente>(_cliente.id, field<string>("nome", _cliente.nome), field<string>("email",  _cliente.email),
            field<size_t>("id_proprietario", _cliente.proprietario.id), field<string>("cpf_cnpj", _cliente.cpf_cnpj),
            field<string>("telefone", _cliente.telefone), field<string>("cep", _cliente.cep),
            field<string>("endereco", _cliente.endereco), field<size_t>("id_origem", _cliente.cliente_origem.id),
            field<string>("data_origem", _cliente.data_origem), field<string>("extra_nome", _cliente.extra_nome),
            field<string>("extra_rg", _cliente.extra_rg), field<string>("extra_nacionalidade", _cliente.extra_nacionalidade),
            field<string>("extra_profissao", _cliente.extra_profissao), field<double>("extra_renda", _cliente.extra_renda),
            field<string>("observacoes", _cliente.observacoes));
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
        if (validate_unique(errs, cliente::get_table(DELETE), 0, field<size_t>("id", id)))
        {
            Napi::Error::New(env, to_string(errs)).ThrowAsJavaScriptException();
            return Napi::Boolean::New(env, false);
        }
        crud::remove<cliente>(field<size_t>("id", id));

        return Napi::Boolean::New(env, true);
    }

    #pragma endregion operations
}

#endif // INCLUDE_CLIENTES_