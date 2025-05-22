#include "utilities.hpp"
#include <IndustryStandard/PeImage.h>
#include <Guid/MemoryAttributesTable.h>
#include "core/patch/patch.hpp"
#include <dependencies/pe/pe.hpp>
#include <core/hook/hook.hpp>

void utils::copy_memory(void* destination, void* source, UINTN size)
{
	auto dst = static_cast<std::uint8_t*>(destination);
	auto src = static_cast<std::uint8_t*>(source);

	for (auto i = 0; i < size; i++)
	{
		dst[i] = src[i];
	}
}

std::uint64_t utils::compare_memory(std::uint8_t* source1, std::uint8_t* source2, std::uint64_t size)
{
	for (std::uint64_t i = 0; i < size; ++i)
	{
		if (source1[i] != source2[i])
		{
			return 1;
		}
	}

	return 0;
}
