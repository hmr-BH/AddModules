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
#include <vector>
#include <string>

#ifdef _WIN32
#   define WIN32_LEAN_AND_MEAN
#   include <windows.h>
#elif __APPLE__
#   include <mach-o/dyld.h>
#else
#   include <unistd.h>
#endif

namespace tools
{
    inline std::string executable_path()
    {
#ifdef _WIN32
        std::vector<char> buf(1024);
        DWORD len;
        while (true)
        {
            len = GetModuleFileNameA(nullptr, buf.data(), buf.size());
            if (len == 0) return "";
            if (len < buf.size()) break;
            buf.resize(buf.size() * 2);
        }
        return std::string(buf.data(), len);
#elif __APPLE__
        std::vector<char> buf(PATH_MAX);
        uint32_t size = buf.size();
        if (_NSGetExecutablePath(buf.data(), &size) != 0)
        {
            buf.resize(size);
            _NSGetExecutablePath(buf.data(), &size);
        }
        return std::filesystem::canonical(buf.data()).string();
#else
        std::vector<char> buf(256);
        ssize_t len;
        while (true)
        {
            len = readlink("/proc/self/exe", buf.data(), buf.size());
            if (len < 0) return "";
            if (static_cast<size_t>(len) < buf.size()) break;
            buf.resize(buf.size() * 2);
        }
        return std::string(buf.data(), len);
#endif
    }
} // namespace tools