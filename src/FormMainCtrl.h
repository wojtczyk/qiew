#ifndef _FORMMAINCTRL_H_
#define _FORMMAINCTRL_H_

#include <qobject.h>

class FormMain;
class SoSeparator;
class SoQtExaminerViewer;
class SoVRMLGroup;

class FormMainCtrl :public QObject
{
	Q_OBJECT

private slots:
	void slotFileOpenAction();
	void slotFileReloadAction();
	void slotFileTake_ScreenshotAction();
	void slotFileExitAction();
	void slotViewCoordinate_AxesAction(bool on);
	void slotViewFullScreenAction(bool on);
	void slotHelpAboutAction();

private:
	static const int STATUSBAR_MSG_DURATION;

	FormMain* formMain;
	SoSeparator* sceneRoot;
	SoVRMLGroup* axesRoot;
	SoQtExaminerViewer* viewer;
	std::string filename;
	
public:
	FormMainCtrl(FormMain* formMain, QObject* parent = 0, const char* name = NULL);
	virtual ~FormMainCtrl();
	
	void load(std::string& filename);
	void addAxes();
	void removeAxes();
};

#endif //_FORMMAINCTRL_H_
