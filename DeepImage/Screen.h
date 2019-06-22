#pragma once

#include <QGLViewer/qglviewer.h>

#include "ShaderProgram.h"
#include "Gizmo.h"
#include "FBO.h"
#include "ModelManager.h"
#include "CheckerBoard.h"

class Screen : public QGLViewer
{
private:
	PhongShader *phong;
	SolidColorShader *solid;
	VertexColorShader *vertexColor;

	FBO* fbo;

	Gizmo* gizmo;
	GizmoTranslate gizmoTranslate;
	GizmoRotate gizmoRotate;

	CheckerBoard checkerBoard;

	ModelManager* modelManager;

public:
	Screen(QWidget *parent = 0);
	~Screen();

	inline void SetModelManager(ModelManager& modelManager_) {
		modelManager = &modelManager_;
	}

	enum GizmoType {
		TRANSLATE, ROTATE
	};
	void SetGizmoType(GizmoType gizmoType_);

private:
	virtual void init();
	virtual void preDraw() {}
	virtual void draw();
	virtual void postDraw() {}
	virtual void resizeGL(int width_, int height_);

	virtual void mousePressEvent(QMouseEvent *e_);
	virtual void mouseMoveEvent(QMouseEvent *e_);
	virtual void mouseReleaseEvent(QMouseEvent *e_);

	virtual void wheelEvent(QWheelEvent *e_);
};