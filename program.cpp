#include "program.h"

#include "display.h"

//							Program

	Program::Program()
	{
		srand(static_cast<unsigned int>(GameTimer::instance().timeSinceStart()));
		rand();
		Display &disp = Display::instance();
		while(Error::instance().error() == Error::err::allg_success) disp.update();
	}
	Program::~Program()
	{
	
	}

	Program &Program::instance()
	{
		static Program inst;
		return inst;
	}
