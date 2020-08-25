#pragma once

#include <memory>
#include <string>

namespace IceSDK
{
	namespace Memory
	{
		template <typename T>
		using Ptr = std::shared_ptr<T>;

		template <typename T>
		using WeakPtr = std::weak_ptr<T>;

		inline std::string SizeInText(double len)
		{
			if (len < 1024.0)
				return std::to_string(len) + " B";

			if (len < 1024.0 * 1024.0)
				return std::to_string(len / (1024.0)) + " KB";

			if (len < 1024.0 * 1024.0 * 1024.0)
				return std::to_string(len / (1024.0 * 1024.0)) + " MB";

			if (len < 1024.0 * 1024.0 * 1024.0 * 1024.0)
				return std::to_string(len / (1024.0 * 1024.0 * 1024.0)) + " GB";

			return std::to_string(len) + " UK";
		}
	} // namespace Memory
} // namespace IceSDK
