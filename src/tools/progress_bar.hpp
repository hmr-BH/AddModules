#pragma once
#include <string>
#include <iostream>
#include <iomanip>
#include <sstream>

namespace tools {
    class ProgressBar {
    private:
        std::size_t total_{0};
        std::size_t current_{0};
        int current_file_{0};
        int total_files_{0};

        std::string format_bytes(std::size_t bytes) const {
            std::ostringstream oss;
            oss << std::fixed << std::setprecision(2);

            if (bytes >= 1024 * 1024) {
                oss << bytes / (1024.0 * 1024.0) << "MB";
            } else if (bytes >= 1024) {
                oss << bytes / 1024.0 << "KB";
            } else {
                oss << bytes << "B";
            }
            return oss.str();
        }

    public:
        void set_total_bytes(std::size_t bytes) { total_ = bytes; }
        void set_file_count(int current, int total) {
            current_file_ = current;
            total_files_ = total;
        }

        void add_bytes(std::size_t n) {
            current_ += n;
            display();
        }

        void display() const {
            const int bar_width = 50;
            float ratio = total_ > 0 ? static_cast<float>(current_) / total_ : 0.0f;
            int pos = static_cast<int>(bar_width * ratio);

            std::string line;
            line.reserve(100);

            line += '\r';
            line += '[';
            line.append(pos, '#');
            line.append(bar_width - pos, '.');
            line += "] ";
            line += std::to_string(static_cast<int>(ratio * 100.0)) + "% ";
            line += format_bytes(current_) + "/" + format_bytes(total_);
            line += " (" + std::to_string(current_file_) + "/" + std::to_string(total_files_) + ")";

            std::cout << line << std::flush;
        }
    };
} // namespace tools