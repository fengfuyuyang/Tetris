/**
 * @Author: fengfuyuyang@outlook.com
 * @File Name: file_handler.h
 * @File Version: 0.1
 * @Create Time: 2024-07-17 11:13:57
 * 
 * Copyright (c) 2024 fengfuyuyang@outlook.com. All rights reserved.
 */

#pragma once

#include <windows.h>

#include <stdexcept>
#include <string>

class FileHandler {
public:
    // 获取文件完整路径
    // 注意：传入文件名应为可执行程序所在目录的相对路径，如 L"images\\image.png"
    static std::wstring GetFullPath(const std::wstring& filename) {
        std::wstring executable_directory = GetExecutableDirectory();
        return executable_directory + filename;
    }

    // 检查文件是否存在
    static bool FileExists(const std::wstring& filename) {
        DWORD attributes = GetFileAttributesW(filename.c_str());
        return (attributes != INVALID_FILE_ATTRIBUTES && !(attributes & FILE_ATTRIBUTE_DIRECTORY));
    }

    // 将路径转换为 std::string 格式
    static std::string ConvertToString(const std::wstring& wstr) {
        int required_size =
                WideCharToMultiByte(CP_UTF8, 0, wstr.c_str(), -1, nullptr, 0, nullptr, nullptr);
        if (required_size == 0) {
            throw std::runtime_error("Failed to convert wide string to multi-byte string.");
        }

        std::string str(required_size, 0);
        WideCharToMultiByte(CP_UTF8, 0, wstr.c_str(), -1, str.data(), required_size, nullptr,
                            nullptr);
        str.resize(required_size - 1); // 移除末尾的 null 终止符
        return str;
    }

private:
    // 获取当前可执行文件路径
    static std::wstring GetExecutableDirectory() {
        wchar_t buffer[MAX_PATH];
        GetModuleFileNameW(nullptr, buffer, MAX_PATH);
        std::wstring executable_path(buffer);
        std::wstring::size_type pos = executable_path.find_last_of(L"\\/");
        return executable_path.substr(0, pos + 1);
    }
};
