#pragma once

#include <QtWidgets/QWidget>
#include "ui_DeepImage.h"
#include "ModelManager.h"

class DeepImage : public QWidget
{
	Q_OBJECT
private:
	ModelManager modelManager;

public:
	DeepImage(QWidget *parent = Q_NULLPTR);

	inline ModelManager& GetModelManager() { return modelManager; }

private:
	Ui::DeepImageClass ui;

private slots:
	void ModelLoaded();
	void ModelDeleted();
	void GizmoChanged();
};
