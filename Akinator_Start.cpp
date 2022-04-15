#include "Akinator.h"

int main()
{
	Akinator play;
	play.setAkinator_Reader();
	play.Game();
	play.getPrintInFile();
	play.getDump();

	return 0;
} 
