#pragma once

// C++
#include <stdint.h>

namespace EARS
{
	namespace Modules
	{
		/**
		 * One "CEC" - a named set of controller event bindings.
		 *
		 * Shipped as an RWS resource of type "CEC", loaded by ControllerDataManager. The
		 * resource is a flat image with no relocations:
		 *
		 *     [ControllerEventConfig (0xC0)][CtrlEventDesc[m_EventConfigSize] (0x80 each)]
		 *
		 * so m_pEventDesc is simply the blob base + 0xC0. A mod can therefore build one of
		 * these in its own memory and hand it to ControllerManager::AddConfigResource - the
		 * loader does nothing that cannot be done directly.
		 *
		 * Constraints the engine relies on:
		 *  - the CtrlEventDesc array MUST be sorted ascending by eventHashID, because
		 *    ControllerManager::GetEventList binary-searches it;
		 *  - duplicate eventHashIDs are legal and intentional - every matching entry is
		 *    tested, which is how one event gets alternative bindings;
		 *  - m_EventConfigSize must not exceed ControllerManager::MAX_EVENTS.
		 */
		class ControllerEventConfig
		{
		public:

			static constexpr uint8_t cCurrentParserVersion = 1;
			static constexpr uint8_t cMaxConfigNameLength = 33;
			static constexpr uint8_t cMaxConfigLocationLength = 146;

			/**
			 * One binding: a named event and the pad pattern(s) that raise it.
			 *
			 * A "sequence" is up to cMaxEventSequenceLength ordered PadPatterns, each with
			 * its own duration window, so taps, holds and multi-step combos are all data.
			 */
			class CtrlEventDesc
			{
			public:

				static constexpr int8_t cMaxEventSequenceLength = 7;
				static constexpr uint8_t cMaxEventNameLength = 26;
				static constexpr int8_t cMaxEventCategoryLength = 13;

				/** One step of a sequence: which buttons and stick directions must hold, and for how long. */
				class PadPattern
				{
				public:

					static constexpr int cMaxAnalogSticks = 2;

					// Cardinal direction window per stick, indexed by ControllerManager::eStickIndex.
					int8_t sticksMin[cMaxAnalogSticks] = { 0, 0 };	// 0x00
					int8_t sticksMax[cMaxAnalogSticks] = { 0, 0 };	// 0x02
					int16_t minDurationInMs = 0;					// 0x04
					int16_t maxDurationInMs = 0;					// 0x06
					uint16_t buttonRequired = 0;					// 0x08 - EARS::Framework::ButtonMask bits that must be down
					uint16_t buttonDiscarded = 0;					// 0x0A - EARS::Framework::ButtonMask bits that must be up
				};

				static_assert(sizeof(PadPattern) == 0xC, "ControllerEventConfig::CtrlEventDesc::PadPattern must equal 0xC");

				uint32_t eventHashID = 0;							// 0x00 - EARS::Common::HashString_SDBM(eventName)
				int8_t sequenceLength = 0;							// 0x04 - how many entries of sequence[] are used
				char eventName[cMaxEventNameLength] = {};			// 0x05
				char eventCategory[cMaxEventCategoryLength] = {};	// 0x1F
				PadPattern sequence[cMaxEventSequenceLength] = {};	// 0x2C
			};

			static_assert(sizeof(CtrlEventDesc) == 0x80, "ControllerEventConfig::CtrlEventDesc must equal 0x80");

			int8_t GetConfigVersion() const { return m_EventConfigVersion; }
			uint16_t GetConfigSize() const { return m_EventConfigSize; }
			uint32_t GetNameHash() const { return m_ConfigNameHash; }
			int8_t GetRefCount() const { return m_ReferenceCount; }
			const char* GetName() const { return m_ConfigName; }
			const char* GetConfigFileLocation() const { return m_ConfigFileLocation; }

			const CtrlEventDesc* GetEventDescArray() const { return m_pEventDesc; }
			const CtrlEventDesc& GetEventDesc(uint16_t Index) const { return m_pEventDesc[Index]; }
			CtrlEventDesc& GetEventDescWriteable(uint16_t Index) { return m_pEventDesc[Index]; }

			void AddRef() { ++m_ReferenceCount; }
			void SubtractRef() { --m_ReferenceCount; }

			/* Point at the descriptor array and record how many entries it holds.
			 * The array must already be sorted ascending by eventHashID. */
			void SetEventDescArray(uint16_t InSize, CtrlEventDesc* InDescs)
			{
				m_EventConfigSize = InSize;
				m_pEventDesc = InDescs;
			}

			/* Copy the name in and recompute m_ConfigNameHash from it. This is the key
			 * ControllerManager::PushConfiguration looks configs up by. */
			void SetNameAndHash(const char* InName);

			void SetConfigResourceLocation(const char* InLocation);

			/* Bring a freshly allocated blob up to the state LoadResource leaves it in:
			 * version stamped, refcount cleared, descriptor array pointed at blob + 0xC0. */
			void InitAsResourceImage();

		private:

			int8_t m_EventConfigVersion = cCurrentParserVersion;			// 0x00
			uint16_t m_EventConfigSize = 0;								// 0x02 - number of CtrlEventDesc entries
			uint32_t m_ConfigNameHash = 0;								// 0x04
			int8_t m_ReferenceCount = 0;								// 0x08
			char m_ConfigName[cMaxConfigNameLength] = {};				// 0x09
			char m_ConfigFileLocation[cMaxConfigLocationLength] = {};	// 0x2A
			CtrlEventDesc* m_pEventDesc = nullptr;						// 0xBC
		};

		static_assert(sizeof(EARS::Modules::ControllerEventConfig) == 0xC0, "EARS::Modules::ControllerEventConfig must equal 0xC0");
	} // Modules
} // EARS
