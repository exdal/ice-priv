#include "pch.h"
#include "Utils/FileSystem.h"
#include "Utils/Logger.h"
#include <fstream>
#include <iostream>
#include <iterator>

#ifdef ICESDK_WIN32
#include <Windows.h>
#elif defined(ICESDK_LINUX) || defined(ICESDK_ANDROID)
#include <sys/stat.h>
#include <sys/types.h>
#include <dirent.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>

#else
#error "Platform not implemented!"
#endif

using namespace IceSDK;

std::string FileSystem::JoinPath(std::string_view p1, std::string_view p2) {
    if (p1.length() <= 0)
        return p2.data();

    if (p2.length() <= 0)
        return p1.data();

    if (p1[p1.length() - 1] == '/' || p1[p1.length() - 1] == '\\')
        return std::string(p1.data()) + p2.data();

    return std::string(p1) + "/" + p2.data();
}

bool FileSystem::Exists(std::string_view pPath) {
#ifdef ICESDK_WIN32
    const auto dir_type = GetFileAttributesA(pPath.data());

    return dir_type != INVALID_FILE_ATTRIBUTES;
#elif defined(ICESDK_LINUX) || defined(ICESDK_ANDROID)
    return (access(pPath.data(), F_OK) != -1);
#else
#error "Platform not implemented!"
#endif
    return false;
}

bool FileSystem::IsDirectory(std::string_view pPath) {
    if (!Exists(pPath))
        return false;

#ifdef ICESDK_WIN32
    return GetFileAttributes(pPath.data()) & FILE_ATTRIBUTE_DIRECTORY;
#elif defined(ICESDK_LINUX) || defined(ICESDK_ANDROID)
    struct stat st;
    stat(pPath.data(), &st);
    return S_ISDIR(st.st_mode);
#else
#error "Platform not implemented!"
#endif
    return false;
}

std::vector<std::string> FileSystem::ReadDirectory(std::string_view pPath, bool pRecursive /*= false */) {
    std::vector<std::string> directories;

#if defined(ICESDK_WIN32)
    HANDLE hFind;
    WIN32_FIND_DATA FindFileData;

    // Requires a * wildcard for some reason
    if ((hFind = FindFirstFile(FileSystem::JoinPath(pPath, "./*").data(), &FindFileData)) != INVALID_HANDLE_VALUE) {
        do {
            if (std::string_view(FindFileData.cFileName) == "." || std::string_view(FindFileData.cFileName) == "..")
                continue;

            directories.push_back(FileSystem::JoinPath(pPath, FindFileData.cFileName));
        } while (FindNextFile(hFind, &FindFileData));
        FindClose(hFind);
    }
#elif defined(ICESDK_LINUX) || defined(ICESDK_ANDROID)
    DIR *dir;
    struct dirent *ent;
    if ((dir = opendir(pPath.data())) != nullptr) {
        while ((ent = readdir(dir)) != nullptr) {
            if (std::string_view(ent->d_name) == "." || std::string_view(ent->d_name) == "..")
                continue;

            directories.push_back(FileSystem::JoinPath(pPath, ent->d_name));
        }
        closedir(dir);
    }
#else
#error "Platform not implemented!"
#endif

    if (pRecursive && !directories.empty()) {
        for (const auto &_dir : directories) {
            if (FileSystem::IsDirectory(_dir)) {
                for (const auto &__dir : ReadDirectory(_dir, true)) { directories.push_back(__dir); }
            }
        }
    }

    return directories;
}

std::string FileSystem::ResolveFullPath(std::string_view pPath) {
#ifdef ICESDK_WIN32
    char buffer[4096];
    if (IS_ERROR(GetFullPathNameA(pPath.data(), 4096, buffer, nullptr)))
        return "";

    return buffer;
#elif defined(ICESDK_LINUX) || defined(ICESDK_ANDROID)
    char buffer[PATH_MAX + 1];

    realpath(pPath.data(), buffer);

    return std::string(buffer);
#else
#error "Platform not implemented!"
#endif
    return "";
}

uint8_t *FileSystem::ReadBinaryFile(std::string_view path, uint32_t *size) {
    FILE *fp = fopen(path.data(), "rb");
    if (!fp) {
        ICESDK_ERROR("Failed to open %s", path.data());
        return nullptr;
    }

    fseek(fp, 0, SEEK_END);
    const uint32_t fileSize = ftell(fp);
    fseek(fp, 0, SEEK_SET);

    if (fileSize <= 0)
        return {};

    uint8_t *data = (uint8_t *)malloc(fileSize);

    if (!data) {
        // print out of mem error
        return nullptr;
    }
    fread(data, fileSize, 1, fp);

    *size = fileSize;
    return data;
}

void FileSystem::WriteBinaryFile(std::string_view path, uint8_t *data, uint32_t size) {
#if defined(ICESDK_LINUX) || defined(ICESDK_ANDROID)
    auto fp = open(path.data(), O_WRONLY | O_CREAT, umask(0755));
    if (fp < 0)
        ICESDK_ERROR("Failed to open %s", path.data());

    write(fp, data, size);

    close(fp);
#else
    FILE *fp = fopen(path.data(), "wb");
    if (!fp)
        ICESDK_ERROR("Failed to open %s", path.data());

    if (data)
        fwrite(data, size, 1, fp); // maybe we want to touch the file

    fclose(fp);
#endif

#if defined(ICESDK_LINUX) || defined(ICESDK_ANDROID)
    chmod(path.data(),
        umask(0755)); // set the correct permissions cause it's wrong
#endif
}

void FileSystem::MkDir(std::string_view pPath) {
#ifdef ICESDK_WIN32
    CreateDirectory(pPath.data(), NULL);
#elif defined(ICESDK_LINUX) || defined(ICESDK_ANDROID)
    if (mkdir(pPath.data(), umask(0755)))
        ICESDK_ERROR("Failed to create Directory! %s", pPath.data());

    chmod(pPath.data(), umask(0755)); // set the correct permissions cause
                                      // it's wrong for some reason
#else
#error "Platform not implemented!"
#endif
}

#include <iostream>

void FileSystem::Touch(std::string_view pPath) {
    WriteBinaryFile(pPath, nullptr, 0);
}

void FileSystem::Delete(std::string_view pPath) {
    remove(pPath.data());
}
