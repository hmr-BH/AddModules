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
#include <string_view>

#include "commands.hpp"
#include "../execute/execute.hpp"

namespace commands {

    int install(const std::vector<std::string_view>& args,
                const cli::GlobalOptions& g)
    {
        bool upgrade = false;
        std::string_view index_url = "http://hmrbh.cn/swarmclone/modules/simple";
        std::string_view module_name;

        for (size_t i = 0; i < args.size(); ++i)
        {
            const auto& arg = args[i];

            if (arg == "--upgrade")
                upgrade = true;

            else if (arg.starts_with("--index-url"))
            {
                if (arg.size() > 11 && arg[11] == '=')
                    index_url = arg.substr(12);
                else if (i + 1 < args.size())
                    index_url = args[++i];
            }

            else if (arg == "-h" || arg == "--help")
            {
                std::println("{}", usageText);
                return 0;
            }

            else if (module_name.empty())
            {
                module_name = arg;
            }
            else
            {
                std::println(stderr, "Error: unexpected argument '{}'", arg);
                return 1;
            }
        }

        if (module_name.empty())
        {
            std::println(stderr, "Error: missing module name");
            return 1;
        }

        if (g.debug)
            std::println("Debug: install module '{}', upgrade={}, index={}, debug={}", module_name, upgrade, index_url, g.debug);

        execute::install_module(module_name, upgrade, index_url, g.debug);
        return 0;
    }

}  // namespace commands