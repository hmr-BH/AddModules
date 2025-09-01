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


#pragma once
#include <string>

constexpr std::string_view usageText = R"(
Usage:
    add-module <command> [options]

Subcommands:
    install             Install a module
    uninstall           Uninstall a module
    list                List all installed modules
    show                Display information about a specific module
    help                Show command help
    about               Show 'about' information

Global options:
    -h/--help           Show command help
    -q/--quiet          Automatically accept all prompts
    --debug             Show detailed debug logs
    --version           Display version information
    --about             Show 'about' information

Subcommand-specific options:
    install
        --upgrade       Install and upgrade the specified module
        --index-url     Specify an index/mirror URL
)";

constexpr std::string_view aboutText =R"(
========================================================================
    add-module 1.0.0 ( {} )
    A lightweight package manager for SwarmClone AI Framework.
    Copyright © 2025 SwarmClone and Contributors
    Apache-2.0 License
    Homepage: https://github.com/SwarmClone/AddModule
    Report bugs: https://github.com/SwarmClone/AddModule/issues
========================================================================
)";

namespace cli
{
    struct GlobalOptions {
        bool help = false;
        bool version = false;
        bool quiet = false;
        bool debug = false;
        bool about = false;
    };

    int dispatch(int argc, char* argv[]);

}