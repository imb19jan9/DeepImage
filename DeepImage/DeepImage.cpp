#include "DeepImage.h"

#include <QFileDialog>

DeepImage::DeepImage(QWidget *parent)
	: QWidget(parent)
{
	ui.setupUi(this);
	ui.openGLWidget->SetModelManager(modelManager);

	connect(ui.pushButton, SIGNAL(clicked()), this, SLOT(ModelLoaded()));
	connect(ui.radioButton, SIGNAL(toggled(bool)), this, SLOT(GizmoChanged()));
	connect(ui.radioButton_2, SIGNAL(toggled(bool)), this, SLOT(GizmoChanged()));
}

void DeepImage::ModelLoaded()
{
	QString filePath = QFileDialog::getOpenFileName(this, tr("Open File"),
		"./",
		tr("Meshes (*.ply *.stl)"));
	if (filePath.isEmpty())
		return;

	Model3D* model = new Model3D;
	model->Load(filePath.toStdString());
	ui.openGLWidget->makeCurrent();
	model->Init();
	modelManager.AddModel(model);

	ui.openGLWidget->update();
}

void DeepImage::ModelDeleted()
{

}

void DeepImage::GizmoChanged()
{
	if (ui.radioButton->isChecked()) {
		ui.openGLWidget->SetGizmoType(Screen::TRANSLATE);
		ui.radioButton_2->setChecked(false);
	}
	else if (ui.radioButton_2->isChecked()) {
		ui.openGLWidget->SetGizmoType(Screen::ROTATE);
		ui.radioButton->setChecked(false);
	}

	ui.openGLWidget->update();
}
