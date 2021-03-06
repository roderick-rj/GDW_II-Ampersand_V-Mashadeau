#include "areas/World.h"
#include "Utilities.h"

world::Area * world::World::getArea(std::string & AIN)
{
	world::Area * newArea = nullptr; // the new area to be made.
	AIN = ustd::toUpper(AIN); // makes the area name uppercase, just to make sure that it's in the right format.
	std::string tempStr = AIN; // a temporary string that holds the AIN value.

	// if the distinction 'AIN' couldn't be found, it's added to the string.
	if (AIN.find("AIN") == std::string::npos)
		tempStr = "AIN_" + AIN; // adds the 'AIN' prefix.

	// if a string was used instead of an underscore, it is turned into an underscore. If the string contains more than one space or underscore, then it is considered unusable.
	if (AIN.find(" ") != std::string::npos)
		tempStr.at(tempStr.find(" ")) = '_'; // changes the space ot an underscore.

	// the length of the string must be 7 (AIN_###). If it is not, then it is invalid, and a nullptr is returned.
	if (tempStr.length() != 7)
		return nullptr;

	// Goes through every area and sees if it can find what the player has asked for.
	//DEBUG
	if (AIN == "AIN_X00") newArea = new AIN_X00();
	else if (AIN == "AIN_X01") newArea = new AIN_X01();

	//CASTLE
	else if (AIN == "AIN_A00") newArea = new AIN_A00();
	else if (AIN == "AIN_A01") newArea = new AIN_A01();

	//SHADOW
	else if (AIN == "AIN_B00") newArea = new AIN_B00();
	else if (AIN == "AIN_B01") newArea = new AIN_B01();

	//FIRE
	else if (AIN == "AIN_C00") newArea = new AIN_C00();
	else if (AIN == "AIN_C01") newArea = new AIN_C01();

	//EARTH
	else if (AIN == "AIN_E00") newArea = new AIN_E00();
	else if (AIN == "AIN_E01") newArea = new AIN_E01();

	//AIR
	else if (AIN == "AIN_F00") newArea = new AIN_F00();
	else if (AIN == "AIN_F01") newArea = new AIN_F01();

	return newArea;
}
