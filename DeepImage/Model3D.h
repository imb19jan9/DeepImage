#pragma once

#include <QVector4D>
#include <QVector3D>
#include <QMatrix4x4>
#include <QGLViewer/frame.h>

#include "VAO.h"
#include "VBO.h"
#include "IBO.h"
#include "TriMesh.h"

class Model3D
{
private:
	VAO* vao;
	VBO* vbo;
	IBO* ibo;

	TriMesh mesh;
	qglviewer::Frame frame;
	QVector3D scaleVec;

	QVector4D color;
	
public:
	Model3D();
	~Model3D();

	void Init();
	void Draw();

	void Load(const std::string& filePath_);
	QMatrix4x4 ModelMatrix();

	qglviewer::Vec CenterOfMass();

	inline void SetColor(QVector4D color_) { color = color_; }
	inline QVector4D GetColor() const { return color; }
	inline qglviewer::Frame& GetFrame() { return frame; }
};