#include <Inventor/Qt/SoQt.h>
#include <Inventor/Qt/viewers/SoQtExaminerViewer.h>
#include <Inventor/SoDB.h>
#include <Inventor/nodes/SoSeparator.h>
#include <Inventor/nodes/SoFont.h>
#include <Inventor/nodes/SoText3.h>
#include <Inventor/VRMLnodes/SoVRMLCone.h>
#include <Inventor/VRMLnodes/SoVRMLCylinder.h>
#include <Inventor/VRMLnodes/SoVRMLGroup.h>
#include <Inventor/VRMLnodes/SoVRMLMaterial.h>
#include <Inventor/VRMLnodes/SoVRMLTransform.h>
#include <Inventor/misc/SoChildList.h>
#include <qfiledialog.h>
#include <qmessagebox.h>
#include <qmenubar.h>
#include <qstatusbar.h>
#include <qtoolbutton.h>
#include <qgl.h>
#include <qimage.h>
#include <qstrlist.h>
#include <string>
#include <exception>
#include <stdexcept>
#include <simhelper/soqt/SaveDialogGl.h>
#include "config.h"
#include "FormMain.h"
#include "FormMainCtrl.h"

using namespace std;
using namespace simhelper::soqt;

const int FormMainCtrl::STATUSBAR_MSG_DURATION = 5000;

FormMainCtrl::FormMainCtrl(FormMain* formMain, QObject* parent, const char* name)
	:QObject(parent, name)
{
	this->formMain = formMain;

	SoQt::init(formMain);

	sceneRoot = new SoSeparator;
	sceneRoot->ref();
	axesRoot = new SoVRMLTransform;
	sceneRoot->addChild(axesRoot);

	viewer = new SoQtExaminerViewer(formMain);
	viewer->setTransparencyType(SoGLRenderAction::SORTED_OBJECT_BLEND);
	viewer->setSceneGraph(sceneRoot);
	viewer->show();
	viewer->viewAll();
	viewer->setFeedbackVisibility(true);
	viewer->setFeedbackSize(10);
	formMain->setCentralWidget(viewer->getWidget());
	formMain->setCaption(PACKAGE_NAME " " PACKAGE_VERSION);

	// hide the toolbar by default
	formMain->toolbar->hide();

	QObject::connect((QObject*) this->formMain->fileOpenAction, SIGNAL(activated()), this, SLOT(slotFileOpenAction()));
	QObject::connect((QObject*) this->formMain->fileReloadAction, SIGNAL(activated()), this, SLOT(slotFileReloadAction()));
	QObject::connect((QObject*) this->formMain->fileTake_ScreenshotAction, SIGNAL(activated()), this, SLOT(slotFileTake_ScreenshotAction()));
	QObject::connect((QObject*) this->formMain->fileExitAction, SIGNAL(activated()), this, SLOT(slotFileExitAction()));
	QObject::connect((QObject*) this->formMain->viewCoordinate_AxesAction, SIGNAL(toggled(bool)), this, SLOT(slotViewCoordinate_AxesAction(bool)));
	QObject::connect((QObject*) this->formMain->viewFullScreenAction, SIGNAL(toggled(bool)), this, SLOT(slotViewFullScreenAction(bool)));
	QObject::connect((QObject*) this->formMain->helpAboutAction, SIGNAL(activated()), this, SLOT(slotHelpAboutAction()));
};

FormMainCtrl::~FormMainCtrl()
{
	sceneRoot->unref();
	// delete viewer; Do not delete the viewer widget! It is deleted by its parent
};

void FormMainCtrl::slotFileOpenAction()
{
	QString filter = "Supported Files (*.iv *.vrml *.wrl *.wrz *.wrl.gz);;VRML (*.wrl *.vrml *.wrz *.wrl.gz);;Open Inventor (*.iv)";
	QString openFile = QFileDialog::getOpenFileName(QString::null, filter, formMain);

	if (openFile != QString::null)
	{
		string filename = openFile.ascii(); 
		load(filename);
		viewer->resetToHomePosition();
		viewer->viewAll();
	}
};

void FormMainCtrl::slotFileReloadAction()
{
	load(this->filename);
};

void FormMainCtrl::slotFileTake_ScreenshotAction()
{
	try
	{
		SaveDialogGL::show(viewer);
	}
	catch (std::runtime_error& e)
	{
		formMain->statusBar()->message(e.what(), STATUSBAR_MSG_DURATION);
	};
};

void FormMainCtrl::slotFileExitAction()
{
	formMain->close();
};

void FormMainCtrl::slotViewCoordinate_AxesAction(bool on)
{
	// set the toolbutton's state
	formMain->toolButtonCoordinate_Axes->setOn(on);
	
	if (on)
	{
		addAxes();
	}
	else
	{
		removeAxes();
	};
};

void FormMainCtrl::slotHelpAboutAction()
{
	QString aboutString = PACKAGE_STRING "\n\n"
		"by\tMartin Wojtczyk <" PACKAGE_BUGREPORT ">\n\n"
		"parts by\n"
		"\tMarkus Rickert\n"
		"\tJakob Vogel\n\n"
		"Some icons from eclipse and gnome :-)";
	QMessageBox::about(formMain, PACKAGE_NAME, aboutString);
};

void FormMainCtrl::slotViewFullScreenAction(bool on)
{
	if (on)
	{
		formMain->statusBar()->hide();
		formMain->menuBarMain->hide();
		formMain->toolbar->hide();
		formMain->showFullScreen();
		if ((bool) viewer->isDecoration() == true) viewer->setDecoration(false);
	}
	else
	{
		formMain->statusBar()->show();
		formMain->menuBarMain->show();
		formMain->showNormal();
		if (viewer->isDecoration() == false) viewer->setDecoration(true);
	};
	// set the viewer's state
	viewer->setFullScreen(on);
	// set the toolbutton's state
	formMain->toolButtonFullScreen->setOn(on);
};

void FormMainCtrl::load(std::string& filename)
{
	if (filename != "")
	{
		this->filename = filename;
		
		SoInput in;
		SoSeparator* soSeparator;
		
		if (in.openFile(filename.c_str()) == false)
		{
			QString error = "Error loading " + QString(filename.c_str());
			formMain->statusBar()->message(error, STATUSBAR_MSG_DURATION);
		}
		else
		{
			soSeparator = SoDB::readAll(&in);
			in.closeFile();

			SoDB::writelock();
			axesRoot->ref();                // save axesRoot node
			sceneRoot->removeAllChildren(); // remove everything, release memory for everything except axesRoot
//			axesRoot = new SoVRMLTransform;
			sceneRoot->addChild(axesRoot);  // reconnect axesRoot
			axesRoot->unref();              // remove the ref
			if (soSeparator != NULL)
			{
				sceneRoot->addChild(soSeparator);
				SoDB::writeunlock();
				viewer->viewAll();
			}
			else
			{
				SoDB::writeunlock();
				QString error = "Error loading " + QString(filename.c_str());
				formMain->statusBar()->message(error, STATUSBAR_MSG_DURATION);
			};
		};
	};
};

void FormMainCtrl::addAxes()
{
	SoVRMLTransform* axes = new SoVRMLTransform();
	axes->scale.setValue(0.25f, 0.25f, 0.25f);
	axesRoot->addChild(axes);
	
	SoVRMLCylinder* cylinder = new SoVRMLCylinder();
	cylinder->height = 0.18f / 0.27f;
	cylinder->radius = 0.01f / 0.27f;
	
	SoSFVec3f cylinderTransform;
	cylinderTransform.setValue(0.0f, cylinder->height.getValue() / 2.0f, 0.0f);
	
	SoVRMLCone* cone = new SoVRMLCone();
	cone->bottomRadius = 0.03f / 0.27f;
	cone->height = 0.09f / 0.27f;
	
	SoSFVec3f coneTransform;
	coneTransform.setValue(0.0f, cylinder->height.getValue() / 2.0f + cone->height.getValue() / 2.0f, 0.0f);
	
	SoSFVec3f textScale;
	textScale.setValue(0.01f / 0.27f, 0.01f / 0.27f, 0.01f / 0.27f);
	
	SoSFVec3f textTranslation;
	textTranslation.setValue(0.1f / 0.27f, cylinder->height.getValue() / 2.0f + cone->height.getValue() / 2.0f, 0.0f);
	
	SoFont* font = new SoFont();
	font->name.setValue("Arial, Helvetica, Sans");
	axes->addChild(font);
	
	// x
	SoVRMLTransform* xAxis = new SoVRMLTransform();
	xAxis->rotation.setValue(SbVec3f(1.0f, 1.0f, 0.0f), (float) -M_PI);
	axes->addChild(xAxis);
	
	SoVRMLMaterial* xAxisMaterial = new SoVRMLMaterial();
	xAxisMaterial->diffuseColor.setValue(1.0f, 0.0f, 0.0f);
	xAxis->addChild(xAxisMaterial);
	
	SoVRMLTransform* xCylinderTransform = new SoVRMLTransform();
	xCylinderTransform->translation = cylinderTransform;
	xAxis->addChild(xCylinderTransform);
	xCylinderTransform->addChild(cylinder);
	
	SoVRMLTransform* xConeTransform = new SoVRMLTransform();
	xConeTransform->translation = coneTransform;
	xCylinderTransform->addChild(xConeTransform);
	xConeTransform->addChild(cone);
	
	SoVRMLTransform* xTextTransform = new SoVRMLTransform();
	xTextTransform->scale = textScale;
	xTextTransform->translation.setValue(-0.15f / 0.27f, textTranslation.getValue()[1], textTranslation.getValue()[2]);
	xAxis->addChild(xTextTransform);
	
	SoText3* xText = new SoText3();
	xText->parts.setValue(SoText3::ALL);
	xText->string.setValue("x");
	xTextTransform->addChild(xText);
	
	// y
	SoVRMLTransform* yAxis = new SoVRMLTransform();
	yAxis->rotation.setValue(SbVec3f(1.0f, 0.0f, 0.0f), 2.0f * (float) M_PI);
	axes->addChild(yAxis);
	
	SoVRMLMaterial* yAxisMaterial = new SoVRMLMaterial();
	yAxisMaterial->diffuseColor.setValue(0.0f, 1.0f, 0.0f);
	yAxis->addChild(yAxisMaterial);
	
	SoVRMLTransform* yCylinderTransform = new SoVRMLTransform();
	yCylinderTransform->translation = cylinderTransform;
	yAxis->addChild(yCylinderTransform);
	yCylinderTransform->addChild(cylinder);
	
	SoVRMLTransform* yConeTransform = new SoVRMLTransform();
	yConeTransform->translation = coneTransform;
	yCylinderTransform->addChild(yConeTransform);
	yConeTransform->addChild(cone);
	
	SoVRMLTransform* yTextTransform = new SoVRMLTransform();
	yTextTransform->scale = textScale;
	yTextTransform->translation = textTranslation;
	yAxis->addChild(yTextTransform);
	
	SoText3* yText = new SoText3();
	yText->parts.setValue(SoText3::ALL);
	yText->string.setValue("y");
	yTextTransform->addChild(yText);
	
	// z
	SoVRMLTransform* zAxis = new SoVRMLTransform();
	zAxis->rotation.setValue(SbVec3f(0.0f, 1.0f, 1.0f), (float) M_PI);
	axes->addChild(zAxis);
	
	SoVRMLMaterial* zAxisMaterial = new SoVRMLMaterial();
	zAxisMaterial->diffuseColor.setValue(0.0f, 0.0f, 1.0f);
	zAxis->addChild(zAxisMaterial);
	
	SoVRMLTransform* zCylinderTransform = new SoVRMLTransform();
	zCylinderTransform->translation = cylinderTransform;
	zAxis->addChild(zCylinderTransform);
	zCylinderTransform->addChild(cylinder);
	
	SoVRMLTransform* zConeTransform = new SoVRMLTransform();
	zConeTransform->translation = coneTransform;
	zCylinderTransform->addChild(zConeTransform);
	zConeTransform->addChild(cone);
	
	SoVRMLTransform* zTextTransform = new SoVRMLTransform();
	zTextTransform->scale = textScale;
	zTextTransform->translation = textTranslation;
	zAxis->addChild(zTextTransform);
	
	SoText3* zText = new SoText3();
	zText->parts.setValue(SoText3::ALL);
	zText->string.setValue("z");
	zTextTransform->addChild(zText);
};

void FormMainCtrl::removeAxes()
{
//	axesRoot->removeAllChildren();

	SoDB::writelock();
	sceneRoot->removeChild(axesRoot);
	axesRoot = new SoVRMLTransform;
	sceneRoot->addChild(axesRoot);
	SoDB::writeunlock();
};
