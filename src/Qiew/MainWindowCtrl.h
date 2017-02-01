/******************************************************************************
 * MainWindowCtrl.h
 *
 * Created on: Dec 6, 2004
 * Copyright Martin Wojtczyk <martin.wojtczyk@gmail.com>
 *
 * MainWindowCtrl is the Controller of the generated Qt GUI elements.
 ******************************************************************************/
#ifndef FORMMAINCTRL_H
#define FORMMAINCTRL_H

#include <QObject>
#include <string>

// forward declarations
class QMainWindow;
namespace Ui
{
	class MainWindow;
};
class QWidget;
class SoSeparator;
class SoQtExaminerViewer;
class SoVRMLGroup;

class MainWindowCtrl :public QObject
{
	Q_OBJECT

private slots:
	void slotActionOpen();
	void slotActionReload();
	void slotActionTakeScreenshot();
	void slotActionExit();
	void slotActionCoordinateAxes(bool on);
	void slotActionFeedbackVisibility();
	void slotActionFullScreen();
	void slotActionAbout();

private:
	QMainWindow* mainWindow;
	Ui::MainWindow* uiMainWindow;
	QWidget* viewerWidget;
	SoSeparator* sceneRoot;
	SoVRMLGroup* axesRoot;
	SoQtExaminerViewer* viewer;
	std::string filename;
	
public:
	MainWindowCtrl(QMainWindow* mainWindow, Ui::MainWindow* uiMainWindow);
	virtual ~MainWindowCtrl();
	
	void load(std::string& filename);
	void addAxes();
	void removeAxes();
};

#endif //FORMMAINCTRL_H
