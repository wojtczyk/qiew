/******************************************************************************
 * Application.h
 *
 * Created on: Feb 8, 2011
 * Copyright Martin Wojtczyk <martin.wojtczyk@gmail.com>
 ******************************************************************************/

#ifndef APPLICATION_H
#define APPLICATION_H

#include <QApplication>

// forward declarations
class QMainWindow;
namespace Ui
{
	class MainWindow;
};
class MainWindowCtrl;

class Application :public QApplication
{
	Q_OBJECT

private:
	QMainWindow* mainWindow;
	Ui::MainWindow* uiMainWindow;
	MainWindowCtrl* mainWindowCtrl;

protected:
	bool event(QEvent *);

public:
	Application(int& argc, char** argv);
	virtual ~Application();
};

#endif /* APPLICATION_H */
