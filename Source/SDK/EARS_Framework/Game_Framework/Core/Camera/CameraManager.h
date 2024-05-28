#pragma once

namespace EARS
{
	namespace Framework
	{
		class CameraManager
		{
		public:

			/**
			 * Allows the Camera Manager the ability to update
			 * NB: Internally, this is a counter. Calling EnableUpdate() increments this Counter
			 */
			void EnableUpdate();

			/**
			 * Allows the Camera Manager the ability to update
			 * NB: Internally, this is a counter. Calling DisableUpdate() decrements this Counter
			 */
			void DisableUpdate();

			/**
			 * Fetch the instance of this class
			 */
			static CameraManager* GetInstance();

		private:
		};
	} // Framework
} // EARS
