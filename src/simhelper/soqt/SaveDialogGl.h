#ifndef SIMHELPER_SOQT_SAVEDIALOGGL_H
#define SIMHELPER_SOQT_SAVEDIALOGGL_H

#include <qfiledialog.h>

class QLabel;
class QSpinBox;
class QCheckBox;
class QSlider;
class QVBox;
class QHBox;
class QFrame;

namespace simhelper
{

namespace soqt
{

class SaveDialogGL :public QFileDialog
{
private:
	QVBox *widgetBox;
	QFrame *separator;
	QHBox *qualityBox;
	QLabel *qualityLabel;
	QSlider *qualitySlider;
	QSpinBox *qualitySpinBox;
	QCheckBox *alphaBox;

	SaveDialogGL(QWidget*);
	~SaveDialogGL();

public:	
	static void show(SoQtGLWidget*) throw(std::exception);
};

};

};

#endif //SIMHELPER_SOQT_SAVEDIALOGGL_H
