/******************************************************************************
 * Application.cpp
 *
 * Created on: Feb 8, 2011
 * Copyright Martin Wojtczyk <martin.wojtczyk@gmail.com>
 ******************************************************************************/

#include <QMainWindow>
#include <QFileOpenEvent>
#include <string>
#include "config.h"
#include "Application.h"
#include "MainWindowCtrl.h"
#include "ui_MainWindow.h"

Application::Application(int& argc, char** argv) :
	QApplication(argc, argv),
	mainWindow(NULL),
	uiMainWindow(NULL),
	mainWindowCtrl(NULL)
{
	mainWindow = new QMainWindow();
	uiMainWindow = new Ui::MainWindow();
	uiMainWindow->setupUi(mainWindow);

	mainWindowCtrl = new MainWindowCtrl(mainWindow, uiMainWindow);

	if (argc > 1)
	{
		std::string filename = argv[1];
		mainWindowCtrl->load(filename);
	};

	mainWindow->show();
};

Application::~Application()
{
};

// Overwrite the Event handler
bool Application::event(QEvent* event)
{
	switch (event->type())
	{
		// Take care of the Apple openFile event
		case QEvent::FileOpen:
		{
			std::string filename = static_cast<QFileOpenEvent *> (event)->file().toStdString();
			if (mainWindowCtrl != NULL)
			{
				mainWindowCtrl->load(filename);
			}
			return true;
		}

		default:
			return QApplication::event(event);
	}
};
