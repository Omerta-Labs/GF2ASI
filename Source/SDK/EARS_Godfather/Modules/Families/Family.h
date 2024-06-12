#pragma once

namespace EARS
{
	namespace Modules
	{
		class Sentient;

		class Family
		{
		public:

			/**
			 * Add a Medic into this Family;
			 * Internally does some magic to ensure that killed members
			 * enter a 'revivable state', rather than instantly die
			 */
			void AddMedic(EARS::Modules::Sentient* NewMedic);

		private:
		};
	}
}
