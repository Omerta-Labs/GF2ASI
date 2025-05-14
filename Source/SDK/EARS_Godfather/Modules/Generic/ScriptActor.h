#pragma once

namespace EARS
{
	namespace Modules
	{
		class ScriptActorInterface
		{
		public:

			virtual ~ScriptActorInterface() = 0;
		};

		class SentientScriptActorInterface : public ScriptActorInterface
		{
		public:

			virtual ~SentientScriptActorInterface() = 0;
		};
	}
}
