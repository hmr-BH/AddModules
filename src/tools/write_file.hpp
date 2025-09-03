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
#include <filesystem>
#include <fstream>
#include <string_view>
#include <system_error>

namespace tools {
    enum class WriteMode { Text, Binary };

    inline std::error_code write_file(std::string_view path,
                                      std::string_view content,
                                      WriteMode mode = WriteMode::Text) noexcept {
        namespace fs = std::filesystem;
        std::error_code ec;

        fs::path file{path};
        fs::path parent = file.parent_path();
        if (!parent.empty()) {
            fs::create_directories(parent, ec);
            if (ec) return ec;
        }

        fs::path tmp = file;
        tmp += ".tmp";

        std::ios::openmode openmode = std::ios::out;
        if (mode == WriteMode::Binary) openmode |= std::ios::binary;
        std::ofstream ofs(tmp, openmode);
        if (!ofs) return std::make_error_code(std::errc::io_error);

        ofs.write(content.data(), static_cast<std::streamsize>(content.size()));
        if (!ofs) {
            fs::remove(tmp, ec);
            return std::make_error_code(std::errc::io_error);
        }
        ofs.close();

        fs::rename(tmp, file, ec);
        return ec;
    }
} // namespace tools