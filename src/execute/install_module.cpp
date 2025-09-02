/*
* Copyright (c) 2025 SwarmClone and Contributors
* Licensed under the Apache License, Version 2.0 (the "License");
* you may not use this file except in compliance with the License.
* You may obtain a copy of the License at
*
*     http://www.apache.org/licenses/LICENSE-2.0
*
* Unless required by applicable law or agreed to in writing, software
* distributed under the License is distributed on an "AS IS" BASIS,
* WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
* See the License for the specific language governing permissions and
* limitations under the License.
*/


#include <print>
#include <cpr/cpr.h>

#include "execute.hpp"
#include "../thirdparty/json.hpp"

bool execute::install_module(std::string_view module_name,
    bool upgrade, std::string_view index_url, bool debug) {

    std::println("Looking in index: {}", index_url);

    auto full_url = std::string(index_url) + "/" + std::string(module_name) + "/index.json";

    cpr::Response r = cpr::Get(cpr::Url{full_url},
        cpr::Authentication{"user", "pass", cpr::AuthMode::BASIC},
        cpr::Parameters{{"anon", "true"}, {"key", "value"}});

    std::println("Collecting {}", module_name);

    if (debug) {
        std::println("module-name:{}\n url:'{}'\n "
                     "upgrade:{}\n index-url:{}\n status-code:{}\n content:{}",
                     module_name, full_url, upgrade, index_url,
                     r.status_code, r.text);
    }

    if (r.status_code != 200) {
        std::println(stderr, "Error: Installation of module '{}' failed, \n"
                             "please confirm that the module is in the source '{}'\n"
                             "If you confirm the existence of the module, \n"
                             "please use the '--debug' parameter to get more information",
                             module_name, index_url);
        return false;
    }
    return true;
}
