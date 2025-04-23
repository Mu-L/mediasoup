#include "common.hpp"
#include "MediaSoupErrors.hpp"
#include "RTC/SCTP/ChunkParameter.hpp"
#include "RTC/SCTP/chunkParameters/ZeroChecksumAcceptableChunkParameter.hpp"
#include "RTC/SCTP/common.hpp" // in worker/test/include/
#include <catch2/catch_test_macros.hpp>
#include <cstring> // std::memset()

using namespace RTC::SCTP;

SCENARIO("Zero Checksum Acceptable Chunk Parameter (32769)", "[sctp][serializable]")
{
	resetBuffers();

	SECTION("ZeroChecksumAcceptableChunkParameter::Parse() succeeds")
	{
		// clang-format off
		uint8_t buffer[] =
		{
			// Type:32769 (ZERO_CHECKSUM_ACCEPTABLE), Length: 8
			0x80, 0x01, 0x00, 0x08,
			// EDMID: 666777888
			0x27, 0xBE, 0x39, 0x20,
			// Extra bytes that should be ignored
			0xAA, 0xBB, 0xCC, 0xDD,
			0xAA, 0xBB, 0xCC
		};
		// clang-format on

		auto* parameter = ZeroChecksumAcceptableChunkParameter::Parse(buffer, sizeof(buffer));

		CHECK_PARAMETER(
		  /*parameter*/ parameter,
		  /*buffer*/ buffer,
		  /*bufferLength*/ sizeof(buffer),
		  /*length*/ 8,
		  /*frozen*/ true,
		  /*parameterType*/ ChunkParameter::ChunkParameterType::ZERO_CHECKSUM_ACCEPTABLE,
		  /*unknownType*/ false,
		  /*actionForUnknownParameterType*/ ChunkParameter::ActionForUnknownChunkParameterType::SKIP);

		REQUIRE(parameter->GetEdmid() == 666777888);

		/* Should throw if modifications are attempted when it's frozen. */

		REQUIRE_THROWS_AS(parameter->SetEdmid(1234), MediaSoupError);

		/* Serialize it. */

		parameter->Serialize(SerializeBuffer, sizeof(SerializeBuffer));

		std::memset(buffer, 0x00, sizeof(buffer));

		CHECK_PARAMETER(
		  /*parameter*/ parameter,
		  /*buffer*/ SerializeBuffer,
		  /*bufferLength*/ sizeof(SerializeBuffer),
		  /*length*/ 8,
		  /*frozen*/ false,
		  /*parameterType*/ ChunkParameter::ChunkParameterType::ZERO_CHECKSUM_ACCEPTABLE,
		  /*unknownType*/ false,
		  /*actionForUnknownParameterType*/ ChunkParameter::ActionForUnknownChunkParameterType::SKIP);

		REQUIRE(parameter->GetEdmid() == 666777888);

		/* Clone it. */

		auto* clonedParameter = parameter->Clone(CloneBuffer, sizeof(CloneBuffer));

		std::memset(SerializeBuffer, 0x00, sizeof(SerializeBuffer));

		delete parameter;

		CHECK_PARAMETER(
		  /*parameter*/ clonedParameter,
		  /*buffer*/ CloneBuffer,
		  /*bufferLength*/ sizeof(CloneBuffer),
		  /*length*/ 8,
		  /*frozen*/ false,
		  /*parameterType*/ ChunkParameter::ChunkParameterType::ZERO_CHECKSUM_ACCEPTABLE,
		  /*unknownType*/ false,
		  /*actionForUnknownParameterType*/ ChunkParameter::ActionForUnknownChunkParameterType::SKIP);

		REQUIRE(clonedParameter->GetEdmid() == 666777888);

		delete clonedParameter;
	}

	SECTION("ZeroChecksumAcceptableChunkParameter::Factory() succeeds")
	{
		auto* parameter =
		  ZeroChecksumAcceptableChunkParameter::Factory(FactoryBuffer, sizeof(FactoryBuffer));

		CHECK_PARAMETER(
		  /*parameter*/ parameter,
		  /*buffer*/ FactoryBuffer,
		  /*bufferLength*/ sizeof(FactoryBuffer),
		  /*length*/ 8,
		  /*frozen*/ false,
		  /*parameterType*/ ChunkParameter::ChunkParameterType::ZERO_CHECKSUM_ACCEPTABLE,
		  /*unknownType*/ false,
		  /*actionForUnknownParameterType*/ ChunkParameter::ActionForUnknownChunkParameterType::SKIP);

		REQUIRE(parameter->GetEdmid() == 0);

		/* Modify it. */

		parameter->SetEdmid(12345678);

		CHECK_PARAMETER(
		  /*parameter*/ parameter,
		  /*buffer*/ FactoryBuffer,
		  /*bufferLength*/ sizeof(FactoryBuffer),
		  /*length*/ 8,
		  /*frozen*/ false,
		  /*parameterType*/ ChunkParameter::ChunkParameterType::ZERO_CHECKSUM_ACCEPTABLE,
		  /*unknownType*/ false,
		  /*actionForUnknownParameterType*/ ChunkParameter::ActionForUnknownChunkParameterType::SKIP);

		REQUIRE(parameter->GetEdmid() == 12345678);

		/* Parse itself and compare. */

		auto* parsedParameter =
		  ZeroChecksumAcceptableChunkParameter::Parse(parameter->GetBuffer(), parameter->GetLength());

		delete parameter;

		CHECK_PARAMETER(
		  /*parameter*/ parsedParameter,
		  /*buffer*/ FactoryBuffer,
		  /*bufferLength*/ 8,
		  /*length*/ 8,
		  /*frozen*/ true,
		  /*parameterType*/ ChunkParameter::ChunkParameterType::ZERO_CHECKSUM_ACCEPTABLE,
		  /*unknownType*/ false,
		  /*actionForUnknownParameterType*/ ChunkParameter::ActionForUnknownChunkParameterType::SKIP);

		REQUIRE(parsedParameter->GetEdmid() == 12345678);

		delete parsedParameter;
	}
}
