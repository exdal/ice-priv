#pragma once
#include <fstream>
#include <string>
#include <vector>

#ifdef ICESDK_WIN32
#define WIN32_LEAN_AND_MEAN
#include <Windows.h>
#include <dirent.h>
#elif ICESDK_LINUX
#include <sys/stat.h>
#include <unistd.h>
#include <iterator>
#include <dirent.h>
#include <stdlib.h>
#else
#error "Platform not implemented!"
#endif

namespace IceSDK
{
	namespace FileSystem
	{
		inline std::string JoinPath(const std::string &p1, const std::string &p2)
		{
			if (p1.length() <= 0)
				return p2;

			if (p2.length() <= 0)
				return p1;

			if (p1[p1.length() - 1] == '/' ||
				p1[p1.length() - 1] == '\\')
				return p1 + p2;

			return p1 + "/" + p2;
		}

		inline bool Exists(const std::string &pPath)
		{
#ifdef ICESDK_WIN32
			const auto dir_type = GetFileAttributesA(pPath.c_str());

			return dir_type != INVALID_FILE_ATTRIBUTES;
#elif ICESDK_LINUX
			return (access(pPath.c_str(), F_OK) != -1);
#else
#error "Platform not implemented!"
#endif
		}

		inline bool IsDirectory(const std::string &pPath)
		{
#ifdef ICESDK_WIN32
			return GetFileAttributes(pPath.c_str()) & FILE_ATTRIBUTE_DIRECTORY;
#elif ICESDK_LINUX
			struct stat st;
			stat(pPath.c_str(), &st);
			return S_ISDIR(st.st_mode);
#else
#error "Platform not implemented!"
#endif
			return false;
		}

		inline std::vector<std::string> ReadDirectory(const std::string &pPath, bool pRecursive = false)
		{
			std::vector<std::string> directories;

#if defined(ICESDK_WIN32) | defined(ICESDK_LINUX)
			DIR *dir;
			struct dirent *ent;
			if ((dir = opendir(pPath.c_str())) != nullptr)
			{
				while ((ent = readdir(dir)) != nullptr)
				{
					if (
						std::string(ent->d_name) == "." ||
						std::string(ent->d_name) == "..")
						continue;

					directories.push_back(FileSystem::JoinPath(pPath, ent->d_name));
				}
				closedir(dir);
			}
#else
#error "Platform not implemented!"
#endif

			if (pRecursive && !directories.empty())
			{
				for (const auto &_dir : directories)
				{
					if (FileSystem::IsDirectory(_dir))
					{
						for (const auto &__dir : ReadDirectory(_dir, true))
						{
							directories.push_back(__dir);
						}
					}
				}
			}

			return directories;
		}

		inline std::string ResolveFullPath(const std::string &pPath)
		{
#ifdef ICESDK_WIN32
			char buffer[4096];
			if (
				IS_ERROR(GetFullPathNameA(pPath.c_str(),
										  4096,
										  buffer,
										  nullptr)))
				return "";

			return buffer;
#elif ICESDK_LINUX
			char buffer[PATH_MAX + 1];

			realpath(pPath.c_str(), buffer);

			return std::string(buffer);
#else
#error "Platform not implemented!"
#endif
			return "";
		}

		inline std::vector<uint8_t> ReadBinaryFile(const std::string &pPath)
		{
			std::ifstream file(pPath, std::ios::binary | std::ios::in);
			if (file.fail())
				return {};

			file.unsetf(std::ios::skipws); // Read the file properly!

			file.seekg(0, std::ios::end);
			const auto fileSize = file.tellg();
			file.seekg(0, std::ios::beg);

			if (fileSize <= 0)
				return {};

			std::vector<uint8_t> data;
			data.reserve(fileSize);

			data.insert(data.begin(),
						std::istream_iterator<uint8_t>(file),
						std::istream_iterator<uint8_t>());

			return data;
		}

		inline void WriteBinaryFile(const std::string &pPath, std::vector<uint8_t> pData)
		{
			std::ofstream file(pPath, std::ios::binary | std::ios::out);
			file.unsetf(std::ios::skipws);

			file.write(reinterpret_cast<const char *>(&pData[0]), pData.size());
		}

		inline bool HasExtension(const std::string &pPath, const std::string &pExt)
		{
			if (pPath.length() < pExt.length())
				return false;

			return (0 == pPath.compare(pPath.length() - pExt.length(), pExt.length(), pExt));
		}

		inline void MkDir(const std::string &pPath)
		{
#ifdef ICESDK_WIN32
			CreateDirectory(pPath.c_str(), NULL);
#elif ICESDK_LINUX
			mkdir(pPath.c_str(), 0644);
#else
#error "Platform not implemented!"
#endif
		}

		inline std::string GetFileName(const std::string &pPath)
		{
			for (auto i = pPath.size(); i > 0; --i)
				if (pPath[i] == '/' || pPath[i] == '\\')
					return std::string(pPath.end() - (i - 1), pPath.end());

			return std::string(); // Not found...
		}
	} // namespace FileSystem
} // namespace IceSDK
