#pragma once

// CPP
#include <ctime>

/**
 * Discord Manager for the Scripthook
 */
class DiscordManager
{
public:

	/**
	 * Called when the manager needs to be initialised
	 */
	void Open();

	void OnTick();

private:

	uint32_t uCurrentCityID = 0;
};
