#include "ModelManager.h"

static const int MODELS_OFFSET = 100;

void ModelManager::AddModel(Model3D * model_)
{
	models.push_back(model_);
}

QVector4D ModelManager::GetIndexColor(int idx_)
{
	int num = idx_ + MODELS_OFFSET;

	int r = num / 65536;
	int g = (num - r * 65536) / 256;
	int b = (num - r * 65536 - g * 256);
	return QVector4D(r / 255.0, g / 255.0, b / 255.0, 1.0);
}

int ModelManager::GetIndexFromColor(QVector4D color_)
{
	color_ = color_ * 255.0;
	int num = color_[2] + color_[1] * 256 + color_[0] * 256 * 256;

	return num - MODELS_OFFSET;
}

void ModelManager::ToggleSelection(int idx_, bool& onoff_)
{
	if (std::find(selecteds.begin(), selecteds.end(),
		models[idx_]) == selecteds.end()) {
		selecteds.push_back(models[idx_]);

		onoff_ = true;
	}
	else {
		selecteds.remove(models[idx_]);

		onoff_ = false;
	}
}

qglviewer::Vec ModelManager::GetSelectedsCOG()
{
	qglviewer::Vec pos(0, 0, 0);
	for (std::list<Model3D*>::iterator it = selecteds.begin();
		it != selecteds.end();
		it++)
		pos += (*it)->CenterOfMass();
	pos /= selecteds.size();
	return pos;
}
