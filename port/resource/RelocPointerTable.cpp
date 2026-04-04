#include "RelocPointerTable.h"

#include <cstdlib>
#include <cstring>
#include <spdlog/spdlog.h>

/**
 * Flat array mapping token → pointer.
 * Index 0 is reserved (NULL token). Valid tokens start at 1.
 *
 * Initial capacity: 256K entries (~2 MB). Grows by doubling if needed.
 * Typical usage per scene is well under 100K tokens.
 */

static void **sPointerTable = nullptr;
static uint32_t sNextToken = 1;
static uint32_t sCapacity = 0;

static constexpr uint32_t INITIAL_CAPACITY = 256 * 1024;

static void ensureCapacity(void)
{
	if (sPointerTable == nullptr)
	{
		sCapacity = INITIAL_CAPACITY;
		sPointerTable = (void **)calloc(sCapacity, sizeof(void *));
	}
	else if (sNextToken >= sCapacity)
	{
		uint32_t newCapacity = sCapacity * 2;
		spdlog::info("RelocPointerTable: growing {} -> {} entries", sCapacity, newCapacity);
		sPointerTable = (void **)realloc(sPointerTable, newCapacity * sizeof(void *));
		memset(sPointerTable + sCapacity, 0, (newCapacity - sCapacity) * sizeof(void *));
		sCapacity = newCapacity;
	}
}

extern "C" {

uint32_t portRelocRegisterPointer(void *ptr)
{
	if (ptr == nullptr)
	{
		return 0;
	}

	ensureCapacity();

	uint32_t token = sNextToken++;
	sPointerTable[token] = ptr;
	return token;
}

void *portRelocResolvePointer(uint32_t token)
{
	if (token == 0)
	{
		return nullptr;
	}

	if (token >= sNextToken)
	{
		spdlog::error("RelocPointerTable: invalid token {} (max={})", token, sNextToken - 1);
		return nullptr;
	}

	return sPointerTable[token];
}

void portRelocResetPointerTable(void)
{
	sNextToken = 1;

	if (sPointerTable != nullptr)
	{
		memset(sPointerTable, 0, sCapacity * sizeof(void *));
	}
}

} // extern "C"
