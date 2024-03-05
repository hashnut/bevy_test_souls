#pragma once

namespace PD
{
	static constexpr uint64_t RELEASE_REVISION	= 51;
	static constexpr uint64_t SHIPPING_REVISION = 21;

	static uint64 GetPdlVersion()
	{
		uint64_t version = 0;
		version |= (uint64(SHIPPING_REVISION)	& 0xFFFF) << 0;
		version |= (uint64(RELEASE_REVISION)	& 0xFFFF) << 16;
		return version;
	}

	static constexpr const char* GetPdlVersionString()
	{
		return "51.21";
	}

#ifdef _SERVER
	static bool CheckPdlVersion(uint64 version)
	{
		uint64_t pdlVersion = GetPdlVersion();

	#ifndef _SHIPPING
		pdlVersion = (pdlVersion >> 16);
		pdlVersion &= 0xFFFF;

		version = (version >> 16);
	#endif

		return (version == pdlVersion);
	}
#endif

} // namespace PD
