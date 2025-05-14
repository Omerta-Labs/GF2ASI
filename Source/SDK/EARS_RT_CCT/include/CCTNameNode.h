#pragma once

#include <stdint.h>

// NB: Not sure whether this lives here
// (highly suspected that it does not)
struct ChrCntl_AnimNode_s
{

};

namespace EA
{
	namespace CCT
	{
		/**
		 * A small object associating a name to an AnimNode
		 * Stored within game files and loaded at runtime.
		 */
		struct CCTNameNode
		{
		public:

			CCTNameNode();
			CCTNameNode(const char* InName, ChrCntl_AnimNode_s* InNode);
			~CCTNameNode();

			// shift the CCTNameNode by a fixed offset
			void Relocate(int32_t InOffset);

			// getters
			const char* GetName() const { return m_Name; }
			const ChrCntl_AnimNode_s* GetNode() const { return m_Node; }

		private:

			const char* m_Name = nullptr;
			const ChrCntl_AnimNode_s* m_Node = nullptr;
		};
	} // CCT
} // EA
