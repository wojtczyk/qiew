/******************************************************************************
 * MainWindowCtrl.cpp
 *
 * Created on: Dec 6, 2004
 * Copyright Martin Wojtczyk <martin.wojtczyk@gmail.com>
 *
 * MainWindowCtrl is the Controller of the generated Qt GUI elements.
 ******************************************************************************/
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
#include <QMainWindow>
#include <QMessageBox>
#include <QFileDialog>
#include <QWidget>
#include <QGLWidget>
#include <string>
#include <exception>
#include <stdexcept>
#include "config.h"
#include "ui_MainWindow.h"
#include "MainWindowCtrl.h"

using namespace std;

MainWindowCtrl::MainWindowCtrl(QMainWindow* mainWindow,
	Ui::MainWindow* uiMainWindow)
	:QObject(mainWindow)
{
	this->mainWindow = mainWindow;
	this->uiMainWindow = uiMainWindow;

	SoQt::init(mainWindow);
	SoDB::init();
	
	sceneRoot = new SoSeparator;
	sceneRoot->ref();
	axesRoot = new SoVRMLTransform;
	sceneRoot->addChild(axesRoot);
	viewerWidget = new QWidget(mainWindow);
	mainWindow->setCentralWidget(viewerWidget);
	mainWindow->setWindowTitle(PACKAGE_NAME " " PACKAGE_VERSION);
	mainWindow->setStatusBar(NULL); // disable the status bar
	viewer = new SoQtExaminerViewer(viewerWidget);
	viewer->setTransparencyType(SoGLRenderAction::SORTED_OBJECT_BLEND);
	viewer->setSceneGraph(sceneRoot);
	viewer->setDecoration(false);
	addAxes();
	viewer->resetToHomePosition();
	viewer->viewAll();
	removeAxes();
	viewer->setFeedbackVisibility(true);
	viewer->setFeedbackSize(10);
	mainWindow->setFocusProxy(viewer->getWidget());

	QObject::connect((QObject*) this->uiMainWindow->actionOpen, SIGNAL(triggered()), this, SLOT(slotActionOpen()));
	QObject::connect((QObject*) this->uiMainWindow->actionReload, SIGNAL(triggered()), this, SLOT(slotActionReload()));
	QObject::connect((QObject*) this->uiMainWindow->actionTakeScreenshot, SIGNAL(triggered()), this, SLOT(slotActionTakeScreenshot()));
	QObject::connect((QObject*) this->uiMainWindow->actionExit, SIGNAL(triggered()), this, SLOT(slotActionExit()));
	QObject::connect((QObject*) this->uiMainWindow->actionCoordinateAxes, SIGNAL(toggled(bool)), this, SLOT(slotActionCoordinateAxes(bool)));
	QObject::connect((QObject*) this->uiMainWindow->actionFeedbackVisibility, SIGNAL(triggered()), this, SLOT(slotActionFeedbackVisibility()));
	QObject::connect((QObject*) this->uiMainWindow->actionFullScreen, SIGNAL(triggered()), this, SLOT(slotActionFullScreen()));
	QObject::connect((QObject*) this->uiMainWindow->actionAbout, SIGNAL(triggered()), this, SLOT(slotActionAbout()));

	// Add actions to the main window. This way the keyboard shortcuts are
	// available in Full Screen mode, too
//	mainWindow->addAction(uiMainWindow->actionOpen);
	mainWindow->addAction(uiMainWindow->actionReload);
//	mainWindow->addAction(uiMainWindow->actionTakeScreenshot);
	mainWindow->addAction(uiMainWindow->actionExit);
	mainWindow->addAction(uiMainWindow->actionCoordinateAxes);
	mainWindow->addAction(uiMainWindow->actionFeedbackVisibility);
	mainWindow->addAction(uiMainWindow->actionFullScreen);
};

MainWindowCtrl::~MainWindowCtrl()
{
	sceneRoot->unref();
	// delete viewer; Do not delete the viewer widget! It is deleted by its parent
};

void MainWindowCtrl::slotActionOpen()
{
	QString filter = "Supported Files (*.iv *.vrml *.wrl *.wrz *.wrl.gz);;VRML (*.wrl *.vrml *.wrz *.wrl.gz);;Open Inventor (*.iv)";
	QString openFile = QFileDialog::getOpenFileName(mainWindow, "Open Model", QString::null, filter);

	if (openFile != QString::null)
	{
		string filename = openFile.toStdString(); 
		load(filename);
		viewer->resetToHomePosition();
		viewer->viewAll();
	}
};

void MainWindowCtrl::slotActionReload()
{
	load(this->filename);
};

void MainWindowCtrl::slotActionTakeScreenshot()
{
	QGLWidget* glWidget = (QGLWidget*) viewer->getGLWidget();
	
	QString filter = "Supported Image Formats (*.png *.jpg *.jpeg *.bmp *.ppm *.tiff *.xbm *.xpm)";
	filter += ";;Portable Network Graphics (*.png)";
	filter += ";;Joint Photographic Experts Group (*.jpg *.jpeg)";
	filter += ";;Windows Bitmap (*.bmp)";
	filter += ";;Portable Pixmap (*.ppm)";
	filter += ";;Tagged Image File Format (*.tiff)";
	filter += ";;X11 Bitmap(*.xbm)";
	filter += ";;X11 Pixmap(*.xpm)";

	QString saveFile = QFileDialog::getSaveFileName(mainWindow, "Save Screenshot", QString::null, filter);

	if (!(saveFile.isNull() || saveFile.isEmpty()))
	{
		// Repaint GL widget and grab a frame
		glWidget->repaint();
		QImage image = glWidget->grabFrameBuffer(true);

		bool saved = image.save(saveFile, 0, 100);
	
		if (saved == false)
		{
			QString error = "Error saving " + saveFile;
			QMessageBox::warning(mainWindow, "Error", error);
		};
	};
};

void MainWindowCtrl::slotActionExit()
{
	mainWindow->close();
};

void MainWindowCtrl::slotActionCoordinateAxes(bool on)
{
	if (on)
	{
		addAxes();
	}
	else
	{
		removeAxes();
	};
};

void MainWindowCtrl::slotActionFeedbackVisibility()
{
	if (viewer->isFeedbackVisible())
	{
		viewer->setFeedbackVisibility(false);
	}
	else
	{
		viewer->setFeedbackVisibility(true);
	}
};

void MainWindowCtrl::slotActionAbout()
{
	QString aboutString = PACKAGE_STRING "\n\n"
		"by " PACKAGE_BUGREPORT;
	QMessageBox::about(mainWindow, PACKAGE_NAME, aboutString);
};

void MainWindowCtrl::slotActionFullScreen()
{
	if (!mainWindow->isFullScreen())
	{
		mainWindow->menuBar()->hide();
		mainWindow->showFullScreen();
		// viewer->setDecoration(false);
		// viewer->setFullScreen(true);
	}
	else
	{
		mainWindow->menuBar()->show();
		mainWindow->showNormal();
		// viewer->setDecoration(true);
		// viewer->setFullScreen(false);
	};
};

void MainWindowCtrl::load(std::string& filename)
{
	if (filename != "")
	{
		this->filename = filename;
		
		SoInput in;
		SoSeparator* soSeparator;
		
		if (in.openFile(filename.c_str()) == false)
		{
			QString error = "Error loading " + QString(filename.c_str());
			QMessageBox::warning(mainWindow, "Error", error);
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
				QMessageBox::warning(mainWindow, "Error", error);
			};
		};
	};
};

void MainWindowCtrl::addAxes()
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

void MainWindowCtrl::removeAxes()
{
	SoDB::writelock();
	sceneRoot->removeChild(axesRoot);
	axesRoot = new SoVRMLTransform;
	sceneRoot->addChild(axesRoot);
	SoDB::writeunlock();
};
