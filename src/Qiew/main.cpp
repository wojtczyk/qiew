/******************************************************************************
 * main.cpp
 *
 * Created on: Dec 6, 2004
 * Copyright Martin Wojtczyk <martin.wojtczyk@gmail.com>
 *
 * Qiew is a VRML and Inventor Viewer.
 * The project was started 06.12.2004 by Martin Wojtczyk
 ******************************************************************************/

#include <stdexcept>
#include <exception>
#include <string>
#include <iostream>
#include "config.h"
#include "Application.h"
#include "main.h"

#ifdef WIN32
#include <Windows.h>
#endif // WIN32

int main(int argc, char** argv)
{
	try
	{
		Application application(argc, argv);

		return application.exec();
	}
	catch (std::exception& e)
	{
		std::cout << e.what() << std::endl;
	}
};

#ifdef WIN32
int WINAPI
WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nShowCmd)
{
	return main(__argc, __argv);
}
#endif // WIN32
