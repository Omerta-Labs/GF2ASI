#pragma once

namespace EARS
{
	namespace Modules
	{
		class CrimeManager
		{
		public:

			// Trigger this to call off the police.
			void CalmPoliceTowardsCorleones();

			// Access the CrimeManager instance.
			static CrimeManager* GetInstance();
		};
	} // Modules
} // EARS
