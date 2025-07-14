#include "clientes.hpp"
#include "colaboradores.hpp"
#include "companhias.hpp"
#include "origens.hpp"

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

    template <int N, typename ...T>
    decltype(auto) get_argument(const T&... t);

    void formatQuery(string& base, string& query, string& table);

    template <typename T, typename... Args>
    void formatQuery(string& base, string& query, string& table, T& arg, Args&... args);

    bool setupDatabase();
    Napi::Boolean setupDatabaseWrapped(const Napi::CallbackInfo&);

    Napi::Object Init(Napi::Env env, Napi::Object exports);
    NODE_API_MODULE(cppserver, Init);
}
