#pragma once

namespace EARS
{
	namespace Modules
	{
		class CrimeManager
		{
		public:

			// Trigger this member of the family to automatically join the Players crew.
			void CalmPoliceTowardsCorleones();

			// Access the CrimeManager instance.
			static CrimeManager* GetInstance();

		private:

			float m_TimeScale = 0.0f;
		};
	} // Modules
} // EARS
