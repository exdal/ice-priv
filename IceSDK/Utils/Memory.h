#pragma once

#include <memory>
#include <sstream>
#include <string>
#include <vector>

namespace IceSDK {
    namespace Memory {
        template <typename T>
        using Ptr = std::shared_ptr<T>;

        template <typename T>
        using WeakPtr = std::weak_ptr<T>;

        inline std::string SizeInText(const double pLen) {
            std::ostringstream out;
            out.precision(2);

            if (pLen < 1024.0) {
                out << pLen << " B";

                return out.str();
            }

            if (pLen < 1024.0 * 1024.0) {
                out << pLen / 1024.0 << " KB";

                return out.str();
            }

            if (pLen < 1024.0 * 1024.0 * 1024.0) {
                out << pLen / (1024.0 * 1024.0) << " MB";

                return out.str();
            }

            if (pLen < 1024.0 * 1024.0 * 1024.0 * 1024.0) {
                out << pLen / (1024.0 * 1024.0 * 1024.0) << " GB";

                return out.str();
            }

            return std::to_string(pLen) + " UK";
        }

        template <typename T>
        static void serialize(uint8_t *&data, T *val) {
            memcpy(val, data, sizeof(T));
            data += sizeof(T);
        }

        static void serialize_str8(uint8_t *&data, std::string &str) {
            str.assign((const char *)(data + 1), *(uint8_t *)data);
            data += 1 + str.length();
        }

        template <typename T>
        static void serialize(T data, std::vector<uint8_t> &binary, uint32_t size_ = 0) {
            uint32_t size = (size_ > 0) ? size_ : sizeof(T);
            binary.insert(binary.end(), (uint8_t *)&data, (uint8_t *)&data + size);
        }

        template <typename T>
        static void serialize(T *data, std::vector<uint8_t> &binary, uint32_t size_ = 0) {
            uint32_t size = (size_ > 0) ? size_ : sizeof(T);
            binary.insert(binary.end(), (uint8_t *)data, (uint8_t *)data + size);
        }

         static void serialize_str8(std::string data, std::vector<uint8_t> &binary) {
            uint8_t len = (uint8_t)data.length();
            binary.insert(binary.end(), (uint8_t *)&len, (uint8_t *)&len + 1);
            binary.insert(binary.end(), (uint8_t *)data.data(), (uint8_t *)data.data() + len);
        }

        static void serialize_str(std::string &data, std::vector<uint8_t> &binary) {
            uint16_t len = (uint16_t)data.length();
            binary.insert(binary.end(), (uint8_t *)&len, (uint8_t *)&len + 2);
            binary.insert(binary.end(), (uint8_t *)data.data(), (uint8_t *)data.data() + len);
        }
    } // namespace Memory
} // namespace IceSDK
