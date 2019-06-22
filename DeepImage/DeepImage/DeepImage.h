#pragma once

#include <QtWidgets/QWidget>
#include "ui_DeepImage.h"

class DeepImage : public QWidget
{
	Q_OBJECT

public:
	DeepImage(QWidget *parent = Q_NULLPTR);

private:
	Ui::DeepImageClass ui;
};
