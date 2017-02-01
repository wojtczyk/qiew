#include <Inventor/Qt/SoQt.h>
#include <Inventor/Qt/SoQtGLWidget.h>
#include <qfiledialog.h>
#include <qlabel.h>
#include <qspinbox.h>
#include <qcheckbox.h>
#include <qslider.h>
#include <qvbox.h>
#include <qhbox.h>
#include <qframe.h>
#include <qmessagebox.h>
#include <qgl.h>
#include <qimage.h>
#include <qstrlist.h>
#include <qfile.h>
#include <stdexcept>
#include <exception>
#include "SaveDialogGl.h"

namespace simhelper
{

namespace soqt
{

SaveDialogGL::SaveDialogGL(QWidget* parent) :QFileDialog(parent)
{
	widgetBox = new QVBox(this);
	widgetBox->setSpacing(5);

	separator = new QFrame(widgetBox);
	separator->setFrameStyle(QFrame::HLine | QFrame::Sunken);

	qualityBox = new QHBox(widgetBox);
	qualityBox->setSpacing(5);

	qualityLabel = new QLabel("Quality", qualityBox);
	qualitySlider = new QSlider(0, 100, 1, 100, Qt::Horizontal, qualityBox);

	qualitySpinBox = new QSpinBox(qualitySlider->minValue(), qualitySlider->maxValue(), 1, qualityBox);
	qualitySpinBox->setValue(qualitySlider->value());

	alphaBox = new QCheckBox("Save alpha channel", widgetBox);
	alphaBox->setChecked(true);

	addWidgets((QLabel*) 0, widgetBox, (QPushButton*) 0);

	QObject::connect(qualitySlider, SIGNAL(valueChanged(int)), qualitySpinBox, SLOT(setValue(int)));
	QObject::connect(qualitySpinBox, SIGNAL(valueChanged(int)), qualitySlider, SLOT(setValue(int)));
}

SaveDialogGL::~SaveDialogGL()
{
	QObject::disconnect(qualitySlider, SIGNAL(valueChanged(int)), qualitySpinBox, SLOT(setValue(int)));
	QObject::disconnect(qualitySpinBox, SIGNAL(valueChanged(int)), qualitySlider, SLOT(setValue(int)));

	delete separator;

	delete alphaBox;

	delete qualityLabel;
	delete qualitySlider;
	delete qualitySpinBox;
	delete qualityBox;

	delete widgetBox;
}

void SaveDialogGL::show(SoQtGLWidget* soQtGLWidget) throw(std::exception)
{
	QGLWidget* glWidget = (QGLWidget*) soQtGLWidget->getGLWidget();

	// Build filter string
	QStrList outputFormats = QImage::outputFormats();
	QString filter = "Images (";
	QString filter2 = "";
	for (const char* format = outputFormats.first(); format; format = outputFormats.next())
	{
		QString f(format);
		
		if (f == "JPEG")
		{
			filter += "*.jfif *.jpe *.jpeg *.jpg ";
			filter2 += ";;JPEG (*.jfif *.jpe *.jpeg *.jpg)";
		}
		else
		{
			filter += "*." + f.lower() + " ";
			filter2 += ";;" + f + " (*." + f.lower() + ")";
		};
	};
	filter.truncate(filter.length() - 1);
	filter += ")" + filter2;

	// Create & show dialog
	QFileDialog* dialog = new SaveDialogGL(glWidget);
	dialog->setMode(QFileDialog::AnyFile);
	dialog->setFilters(filter);
	dialog->setCaption("Screenshot");
	dialog->exec();

	// Check return code
	if ( dialog->result() == QDialog::Accepted )
	{
		// Check existence
		QFile file(dialog->selectedFile());
		QString warning = "Selected file (\"" + dialog->selectedFile() + "\") already exists. Overwrite?";
		if ( !file.exists() || QMessageBox::question(glWidget, "File exists!", warning, QMessageBox::Yes, QMessageBox::No | QMessageBox::Default | QMessageBox::Escape) == QMessageBox::Yes )
		{
			// Repaint GL widget and grab a frame
			glWidget->repaint();
			QImage image = glWidget->grabFrameBuffer(((SaveDialogGL*)dialog)->alphaBox->isChecked());

			QString saveFile = dialog->selectedFile();
			if (saveFile != QString::null)
			{
				QString format = saveFile.right(saveFile.length() - saveFile.findRev('.') - 1).upper();
				if ((format == "JFIF") || (format == "JPE") || (format == "JPG"))
				{
					format = "JPEG";
				};
				
				bool saved = image.save(saveFile, format, ((SaveDialogGL*)dialog)->qualitySpinBox->value());
			
				if ((saved == false))
				{
					throw std::runtime_error("Error writing " + saveFile);
				};
			}
		}
	}

	// Destroy dialog
	delete dialog;
};

};

};
