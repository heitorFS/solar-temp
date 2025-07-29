#include "server.h"

using namespace std;

#pragma region Main Methods

bool server::setupDatabase()
{
    ifstream f("C:/AEnAzume/database.db");
    if (f.good())
        return true;

    ofstream { "C:/AEnAzume/database.db" };

    sqlite3* db;
    int err = sqlite3_open("C:/AEnAzume/database.db", &db);

    string sql = 
        "create table colaboradores ("
        "   id INTEGER PRIMARY KEY AUTOINCREMENT,"
        "   nome VARCHAR(120) NOT NULL,"
        "   id_cargo UNSIGNED TINYINT NOT NULL,"
        "   email VARCHAR(254) NOT NULL,"
        "   telefone VARCHAR(11) NOT NULL,"
        "   cpf_cnpj VARCHAR(14) NOT NULL UNIQUE"
        ");"
        "create table companhias ("
        "   id INTEGER PRIMARY KEY AUTOINCREMENT,"
        "   nome VARCHAR(254) NOT NULL UNIQUE"
        ");"
        "create table origens ("
        "   id INTEGER PRIMARY KEY AUTOINCREMENT,"
        "   nome VARCHAR(254) NOT NULL UNIQUE,"
        "   id_companhia INTEGER NOT NULL,"
        "   FOREIGN KEY (id_companhia)"
        "       REFERENCES companhias (id)"
        ");"
        "CREATE TABLE clientes ("
        "   id INTEGER PRIMARY KEY,"
        "   nome VARCHAR(120) NOT NULL,"
        "   email VARCHAR(254) NOT NULL,"
        "   id_proprietario INTEGER NOT NULL,"
        "   cpf_cnpj VARCHAR(14) NOT NULL,"
        "   telefone VARCHAR(11) NOT NULL,"
        "   cep CHAR(8) NOT NULL,"
        "   endereco VARCHAR(254) NOT NULL,"
        "   numero INTEGER NOT NULL,"
        "   complemento VARCHAR(120),"
        "   id_origem INTEGER NOT NULL,"
        "   data_origem DATE,"
        "   extra_nome VARCHAR(120),"
        "   extra_cpf CHAR(11),"
        "   extra_rg VARCHAR(13),"
        "   extra_nacionalidade VARCHAR(20),"
        "   extra_profissao VARCHAR(120),"
        "   extra_renda DOUBLE,"
        "   observacoes VARCHAR(700),"
        "   FOREIGN KEY (id_proprietario)"
        "       REFERENCES colaboradores (id),"
        "   FOREIGN KEY (id_origem)"
        "       REFERENCES origens (id)"
        ");"
        "CREATE TABLE kits ("
        "   id INTEGER PRIMARY KEY,"
        "   fornecedor VARCHAR(300) NOT NULL,"
        "   valor DOUBLE NOT NULL,"
        "   tensao UNSIGNED TINYINT NOT NULL,"
        "   fixacao UNSIGNED TINYINT NOT NULL,"
        "   observacoes TEXT"
        ");"
        "CREATE TABLE modulos ("
        "   id INTEGER PRIMARY KEY,"
        "   id_kit INTEGER NOT NULL,"
        "   potencia INTEGER NOT NULL,"
        "   marca VARCHAR(300) NOT NULL,"
        "   quantidade INTEGER NOT NULL,"
        "   peso FLOAT NOT NULL,"
        "   comprimento INTEGER NOT NULL,"
        "   largura INTEGER NOT NULL,"
        "   garantia_defeito INTEGER NOT NULL,"
        "   garantia_eficiencia INTEGER NOT NULL,"
        "   FOREIGN KEY (id_kit)"
        "       REFERENCES kits (id)"
        ");"
        "CREATE TABLE inversores ("
        "   id INTEGER PRIMARY KEY,"
        "   id_kit INTEGER NOT NULL,"
        "   marca VARCHAR(300) NOT NULL,"
        "   modelo VARCHAR(300) NOT NULL,"
        "   monitoramento INTEGER NOT NULL,"
        "   potencia INTEGER NOT NULL,"
        "   quantidade INTEGER NOT NULL,"
        "   garantia INTEGER NOT NULL,"
        "   microinversor BOOLEAN NOT NULL,"
        "   FOREIGN KEY (id_kit)"
        "       REFERENCES kits (id)"
        ");"
        "CREATE TABLE itens ("
        "   id INTEGER PRIMARY KEY,"
        "   id_kit INTEGER NOT NULL,"
        "   nome VARCHAR(200) NOT NULL,"
        "   quantidade INTEGER NOT NULL,"
        "   fabricante VARCHAR(300) NOT NULL,"
        "   detalhes TEXT,"
        "   FOREIGN KEY (id_kit)"
        "       REFERENCES kits (id)"
        ");";

    char* errmsg;
    err = sqlite3_exec(db, sql.c_str(), NULL, NULL, &errmsg);
    sqlite3_close(db);
    if (err != SQLITE_OK)
        return false;

    return true;
}

#pragma endregion Main Methods

#pragma region Wrapped Methods

Napi::Boolean server::setupDatabaseWrapped(const Napi::CallbackInfo& info)
{
    Napi::Env env = info.Env(); 
    if (!setupDatabase())
        Napi::Error::New(env, "Não foi possível criar o banco de dados. Erro: ").ThrowAsJavaScriptException();
    return Napi::Boolean::New(env, true);
}

#pragma endregion Origens

#pragma endregion Wrapped Methods

Napi::Object server::Init(Napi::Env env, Napi::Object exports)
{
    exports.Set("setupDatabase", Napi::Function::New(env, server::setupDatabaseWrapped));

    exports.Set("getAllClientesShort", Napi::Function::New(env, clientes::get_all_short));
    exports.Set("getAllClientes", Napi::Function::New(env, clientes::get_all));
    exports.Set("getCliente", Napi::Function::New(env, clientes::get));
    exports.Set("createCliente", Napi::Function::New(env, clientes::create));
    exports.Set("updateCliente", Napi::Function::New(env, clientes::update));
    exports.Set("deleteCliente", Napi::Function::New(env, clientes::remove));

    exports.Set("getAllColaboradores", Napi::Function::New(env, colaboradores::get_all));
    exports.Set("getColaborador", Napi::Function::New(env, colaboradores::get));
    exports.Set("createColaborador", Napi::Function::New(env, colaboradores::create));
    exports.Set("updateColaborador", Napi::Function::New(env, colaboradores::update));
    exports.Set("deleteColaborador", Napi::Function::New(env, colaboradores::remove));

    exports.Set("getAllCompanhias", Napi::Function::New(env, companhias::get_all));
    exports.Set("getCompanhia", Napi::Function::New(env, companhias::get));
    exports.Set("createCompanhia", Napi::Function::New(env, companhias::create));
    exports.Set("updateCompanhia", Napi::Function::New(env, companhias::update));
    exports.Set("deleteCompanhia", Napi::Function::New(env, companhias::remove));

    exports.Set("getAllInversores", Napi::Function::New(env, kits::inversores::get_all));
    exports.Set("getInversoresByKit", Napi::Function::New(env, kits::inversores::get_by_kit));
    exports.Set("getInversor", Napi::Function::New(env, kits::inversores::get));
    exports.Set("createInversor", Napi::Function::New(env, kits::inversores::create));
    exports.Set("updateInversor", Napi::Function::New(env, kits::inversores::update));
    exports.Set("deleteInversor", Napi::Function::New(env, kits::inversores::remove));
    exports.Set("deleteInversoresByKit", Napi::Function::New(env, kits::inversores::remove_by_kit));

    exports.Set("getAllItens", Napi::Function::New(env, kits::itens::get_all));
    exports.Set("getItensByKit", Napi::Function::New(env, kits::itens::get_by_kit));
    exports.Set("getItem", Napi::Function::New(env, kits::itens::get));
    exports.Set("createItem", Napi::Function::New(env, kits::itens::create));
    exports.Set("updateItem", Napi::Function::New(env, kits::itens::update));
    exports.Set("deleteItem", Napi::Function::New(env, kits::itens::remove));
    exports.Set("deleteItensByKit", Napi::Function::New(env, kits::itens::remove_by_kit));

    exports.Set("getAllKits", Napi::Function::New(env, kits::get_all));
    exports.Set("getKit", Napi::Function::New(env, kits::get));
    exports.Set("createKit", Napi::Function::New(env, kits::create));
    exports.Set("updateKit", Napi::Function::New(env, kits::update));
    exports.Set("deleteKit", Napi::Function::New(env, kits::remove));

    exports.Set("getAllModulos", Napi::Function::New(env, kits::modulos::get_all));
    exports.Set("getModulosByKit", Napi::Function::New(env, kits::modulos::get_by_kit));
    exports.Set("getModulo", Napi::Function::New(env, kits::modulos::get));
    exports.Set("createModulo", Napi::Function::New(env, kits::modulos::create));
    exports.Set("updateModulo", Napi::Function::New(env, kits::modulos::update));
    exports.Set("deleteModulo", Napi::Function::New(env, kits::modulos::remove));
    exports.Set("deleteModulosByKit", Napi::Function::New(env, kits::modulos::remove_by_kit));

    exports.Set("getAllOrigens", Napi::Function::New(env, origens::get_all));
    exports.Set("getOrigem", Napi::Function::New(env, origens::get));
    exports.Set("createOrigem", Napi::Function::New(env, origens::create));
    exports.Set("deleteOrigem", Napi::Function::New(env, origens::remove));
    return exports;
}