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


#include <string>
#include <vector>
#include <print>
#include <iostream>

#include "parser.hpp"
#include "../commands/commands.hpp"
#include "../tools/executable_path.hpp"

cli::GlobalOptions parse_global(const std::vector<std::string_view>& args,
                                std::vector<std::string_view>& remaining)
{
    cli::GlobalOptions g;
    for (auto arg : args) {
        if (arg == "-h" || arg == "--help")           { g.help   = true; }
        else if (arg == "-q" || arg == "--quiet")     { g.quiet = true; }
        else if (arg == "--version")                  { g.version = true; }
        else if (arg == "--debug")                    { g.debug  = true; }
        else if (arg == "--about")                    { g.about = true; }
        else                                          { remaining.push_back(arg); }
    }
    return g;
}

void print_version()
{
    std::println("add-module 1.0.0 from {}", tools::executable_path());
}

void print_usage()
{
    std::println(usageText);
}

void print_about()
{
    std::println(aboutText, tools::executable_path());
}

int cli::dispatch(int argc, char *argv[])
{
    std::vector<std::string_view> args(argv + 1, argv + argc);
    std::vector<std::string_view> rem;
    const GlobalOptions g = parse_global(args, rem);

    if (g.about) { print_about(); return 0; }
    if (g.version) { print_version(); return 0; }
    if (g.help && rem.empty()) { print_usage(); return 0; }

    if (rem.empty())
    {
        std::println(stderr, "Warning: no parameters were found");
        print_usage();
        return 1;
    }

    const std::string_view cmd = rem[0];
    const std::vector sub_args(rem.begin() + 1, rem.end());

    if (cmd == "install")    return commands::install(sub_args, g);
    if (cmd == "uninstall")  return commands::uninstall(sub_args, g);
    if (cmd == "list")       return commands::list(sub_args, g);
    if (cmd == "show")       return commands::show(sub_args, g);
    if (cmd == "help")       return commands::help();
    if (cmd == "about")      return commands::about();

    std::println(stderr, "Error: unknown command: '{}'", cmd);
    print_usage();
    return 0;
}
