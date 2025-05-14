#pragma once

namespace EARS
{
	namespace Modules
	{
		class IScriptedSequenceTarget
		{
		public:

			// NB: This could be GetClassID FIRST
			// Then typical destructor

			virtual ~IScriptedSequenceTarget() = 0;

		private:
		};
	}
}