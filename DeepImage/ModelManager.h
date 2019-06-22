#pragma once

#include "Model3D.h"

class ModelManager
{
private:
	std::vector<Model3D*> models;
	std::list<Model3D*> selecteds;

public:
	void AddModel(Model3D* model_);

	QVector4D GetIndexColor(int idx_);
	int GetIndexFromColor(QVector4D color_);

	void ToggleSelection(int idx_, bool & onoff_);

	qglviewer::Vec GetSelectedsCOG();

	inline std::vector<Model3D*>& GetModels() { return models; }
	inline std::list<Model3D*>& GetSelecteds() { return selecteds; }

	inline bool HasSelected() { return !selecteds.empty(); }
};