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
#include <filesystem>

#include "execute.hpp"
#include "../thirdparty/json.hpp"
#include "../tools/write_file.hpp"
#include "../tools/progress_bar.hpp"

namespace fs = std::filesystem;

bool execute::install_module(std::string_view module_name,
                             bool /*upgrade*/,
                             std::string_view index_url,
                             bool debug)
{
    std::println("Looking in index: {}", index_url);

    const auto full_url =
        std::string(index_url) + "/" + std::string(module_name) + "/index.json";

    cpr::Response r = cpr::Get(cpr::Url{full_url},
                               cpr::Authentication{"user", "pass", cpr::AuthMode::BASIC},
                               cpr::Parameters{{"anon", "true"}});

    std::println("Collecting '{}'", module_name);

    if (debug) {
        std::println("module-name:{}\nurl:'{}'\nstatus-code:{}\ncontent:{}",
                     module_name, full_url, r.status_code, r.text);
    }

    if (r.status_code != 200) {
        std::println(stderr,
                     "Error: Installation of module '{}' failed.\n"
                     "Please confirm the module exists in '{}'\n"
                     "Use --debug for more information.",
                     module_name, index_url);
        return false;
    }

    const auto module_index = nlohmann::json::parse(r.text);
    const auto& python_files = module_index["python_files"];
    if (!python_files.is_array() || python_files.empty()) {
        std::println(stderr, "No python_files found in index.");
        return false;
    }

    std::size_t total_bytes = 0;
    for (const auto& file : python_files) {
        const auto file_url =
            std::string(index_url) + "/" + std::string(module_name) +
            "/python_files/" + file.get<std::string>();
        cpr::Response head = cpr::Head(cpr::Url{file_url});
        if (head.status_code == 200) {
            if (auto it = head.header.find("Content-Length"); it != head.header.end()) {
                total_bytes += std::stoull(it->second);
            }
        }
    }

    const fs::path dir_path = fs::path("swarmclone") / module_name;
    std::error_code ec;
    fs::create_directories(dir_path, ec);
    if (ec) {
        std::println(stderr, "Failed to create directory '{}': {}",
                     dir_path.string(), ec.message());
        return false;
    }

    tools::ProgressBar bar;
    bar.set_total_bytes(total_bytes);

    int file_count = 0;

    for (const auto& file : python_files) {
        file_count++;
        bar.set_file_count(file_count, python_files.size());

        const auto file_name = file.get<std::string>();
        const auto file_url =
            std::string(index_url) + "/" + std::string(module_name) +
            "/python_files/" + file_name;

        std::string buffer;

        cpr::Response resp = cpr::Download(
            cpr::WriteCallback{
                [&](const std::string_view& data, intptr_t /*userdata*/) -> bool {
                    buffer.append(data);
                    bar.add_bytes(data.size());
                    return true;
                }
            },
            cpr::Url{file_url});

        if (resp.status_code != 200) {
            std::println(stderr, "\nFailed to download {} (status {})",
                         file_url, resp.status_code);
            return false;
        }

        const fs::path file_path = dir_path / file_name;
        if (auto ec_write = tools::write_file(file_path.string(),
                                              buffer,
                                              tools::WriteMode::Binary))
        {
            std::println(stderr, "\nFailed to write '{}': {}",
                         file_path.string(), ec_write.message());
            return false;
        }
    }

    std::println("\n Module '{}' installed successfully into '{}'",
                 module_name, dir_path.string());
    return true;
}