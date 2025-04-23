#define MS_CLASS "RTC::SCTP::ZeroChecksumAcceptableChunkParameter"
// #define MS_LOG_DEV_LEVEL 3

#include "RTC/SCTP/chunkParameters/ZeroChecksumAcceptableChunkParameter.hpp"
#include "Logger.hpp"
#include "MediaSoupErrors.hpp"

namespace RTC
{
	namespace SCTP
	{
		/* Class methods. */

		ZeroChecksumAcceptableChunkParameter* ZeroChecksumAcceptableChunkParameter::Parse(
		  const uint8_t* buffer, size_t bufferLength)
		{
			MS_TRACE();

			ChunkParameter::ChunkParameterType parameterType;
			uint16_t parameterLength;
			uint8_t padding;

			if (!ChunkParameter::IsChunkParameter(
			      buffer, bufferLength, parameterType, parameterLength, padding))
			{
				return nullptr;
			}

			if (parameterType != ChunkParameter::ChunkParameterType::ZERO_CHECKSUM_ACCEPTABLE)
			{
				MS_WARN_DEV("invalid Chunk Parameter type");

				return nullptr;
			}

			return ZeroChecksumAcceptableChunkParameter::ParseStrict(
			  buffer, bufferLength, parameterLength, padding);
		}

		ZeroChecksumAcceptableChunkParameter* ZeroChecksumAcceptableChunkParameter::Factory(
		  uint8_t* buffer, size_t bufferLength)
		{
			MS_TRACE();

			if (bufferLength < ZeroChecksumAcceptableChunkParameter::ZeroChecksumAcceptableChunkParameterHeaderLength)
			{
				MS_THROW_TYPE_ERROR("buffer too small");
			}

			auto* parameter = new ZeroChecksumAcceptableChunkParameter(buffer, bufferLength);

			parameter->InitializeHeader(
			  ChunkParameter::ChunkParameterType::ZERO_CHECKSUM_ACCEPTABLE,
			  ZeroChecksumAcceptableChunkParameter::ZeroChecksumAcceptableChunkParameterHeaderLength);

			// Initialize EDMID to zero.
			parameter->SetEdmid(0);

			// No need to invoke SetLength() since parent constructor invoked it.

			return parameter;
		}

		ZeroChecksumAcceptableChunkParameter* ZeroChecksumAcceptableChunkParameter::ParseStrict(
		  const uint8_t* buffer, size_t bufferLength, uint16_t parameterLength, uint8_t padding)
		{
			MS_TRACE();

			if (parameterLength != ZeroChecksumAcceptableChunkParameter::ZeroChecksumAcceptableChunkParameterHeaderLength)
			{
				MS_WARN_TAG(
				  sctp,
				  "ZeroChecksumAcceptableChunkParameter Length field must be %zu",
				  ZeroChecksumAcceptableChunkParameter::ZeroChecksumAcceptableChunkParameterHeaderLength);

				return nullptr;
			}

			auto* parameter =
			  new ZeroChecksumAcceptableChunkParameter(const_cast<uint8_t*>(buffer), bufferLength);

			// Mark the Parameter as frozen since we are parsing.
			parameter->Freeze();

			return parameter;
		}

		/* Instance methods. */

		ZeroChecksumAcceptableChunkParameter::ZeroChecksumAcceptableChunkParameter(
		  uint8_t* buffer, size_t bufferLength)
		  : ChunkParameter(buffer, bufferLength)
		{
			MS_TRACE();

			SetLength(
			  ZeroChecksumAcceptableChunkParameter::ZeroChecksumAcceptableChunkParameterHeaderLength);
		}

		ZeroChecksumAcceptableChunkParameter::~ZeroChecksumAcceptableChunkParameter()
		{
			MS_TRACE();
		}

		void ZeroChecksumAcceptableChunkParameter::Dump(int indentation) const
		{
			MS_TRACE();

			MS_DUMP_CLEAN(indentation, "<SCTP::ZeroChecksumAcceptableChunkParameter>");
			DumpCommon(indentation);
			MS_DUMP_CLEAN(indentation, "  edmid: %" PRIu32, GetEdmid());
			MS_DUMP_CLEAN(indentation, "</SCTP::ZeroChecksumAcceptableChunkParameter>");
		}

		ZeroChecksumAcceptableChunkParameter* ZeroChecksumAcceptableChunkParameter::Clone(
		  uint8_t* buffer, size_t bufferLength) const
		{
			MS_TRACE();

			auto* clonedParameter = new ZeroChecksumAcceptableChunkParameter(buffer, bufferLength);

			CloneInto(clonedParameter);

			return clonedParameter;
		}

		void ZeroChecksumAcceptableChunkParameter::SetEdmid(uint32_t value)
		{
			MS_TRACE();

			AssertNotFrozen();

			Utils::Byte::Set4Bytes(const_cast<uint8_t*>(GetBuffer()), 4, value);
		}

		ZeroChecksumAcceptableChunkParameter* ZeroChecksumAcceptableChunkParameter::SoftClone(
		  const uint8_t* buffer) const
		{
			MS_TRACE();

			auto* softClonedParameter =
			  new ZeroChecksumAcceptableChunkParameter(const_cast<uint8_t*>(buffer), GetLength());

			SoftCloneInto(softClonedParameter);

			return softClonedParameter;
		}
	} // namespace SCTP
} // namespace RTC
