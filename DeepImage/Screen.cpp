#include "Screen.h"

#include <QGLViewer/manipulatedCameraFrame.h>
#include <QMouseEvent>

Screen::Screen(QWidget * parent)
	: QGLViewer(parent),
	phong(0),
	solid(0),
	vertexColor(0),
	fbo(0),
	checkerBoard(10, 10),
	gizmo(&gizmoTranslate)
{

}

Screen::~Screen()
{
	delete phong;
	delete solid;
	delete vertexColor;
	delete fbo;
}

void Screen::SetGizmoType(GizmoType gizmoType_)
{
	switch (gizmoType_)
	{
	case TRANSLATE:
		gizmo = &gizmoTranslate;
		break;
	case ROTATE:
		gizmo = &gizmoRotate;
		break;
	default:
		break;
	}

	gizmo->ClearFollower();
	std::list<Model3D*> selecteds = modelManager->GetSelecteds();
	for (std::list<Model3D*>::iterator it = selecteds.begin();
		it != selecteds.end();
		it++)
		gizmo->Followed(**it);
	qglviewer::Vec pos = modelManager->GetSelectedsCOG();
	gizmo->SetPosition(pos);
	gizmo->AdjustScale(*camera());
}

void Screen::init()
{
	QOpenGLFunctions_4_5_Core *f = QOpenGLContext::currentContext()->
		versionFunctions<QOpenGLFunctions_4_5_Core>();

	f->glDisable(GL_LIGHT0);
	f->glDisable(GL_LIGHTING);
	f->glDisable(GL_COLOR_MATERIAL);

	setSceneRadius(50);
	setSceneCenter(qglviewer::Vec(50, 50, 0));
	showEntireScene();
	camera()->frame()->setSpinningSensitivity(1000);

	setMouseTracking(true);

	phong = new PhongShader;
	solid = new SolidColorShader;
	vertexColor = new VertexColorShader;

	gizmoTranslate.Init();
	gizmoRotate.Init();

	checkerBoard.Init();
}

void Screen::draw()
{
	QOpenGLFunctions_4_5_Core *f = QOpenGLContext::currentContext()->
		versionFunctions<QOpenGLFunctions_4_5_Core>();

	f->glClearColor(0.7f, 0.7f, 0.7f, 1.0f);
	f->glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	QMatrix4x4 proj, view;
	camera()->getModelViewMatrix(view.data());
	camera()->getProjectionMatrix(proj.data());

	checkerBoard.Draw(view, proj, *vertexColor);

	std::vector<Model3D*>& models = modelManager->GetModels();
	std::list<Model3D*>& selecteds = modelManager->GetSelecteds();
	for (int i = 0; i < models.size(); i++) {
		if (std::find(selecteds.begin(), selecteds.end(),
			models[i]) != selecteds.end())
			models[i]->SetColor(QVector4D(0.0, 1.0, 0.0, 1.0));
		else
			models[i]->SetColor(QVector4D(0.0, 0.0, 0.0, 1.0));

		phong->Predraw(view, proj, *models[i]);
		models[i]->Draw();
	}

	if (modelManager->HasSelected())
		gizmo->Draw(view, proj, *solid);

	fbo->Bind();
	f->glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	f->glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	for (int i = 0; i < models.size(); i++) {
		QVector4D color = modelManager->GetIndexColor(i);
		models[i]->SetColor(color);
		solid->Predraw(view, proj, *models[i]);
		models[i]->Draw();
	}

	fbo->Unbind();
}

void Screen::resizeGL(int width_, int height_)
{
	QGLViewer::resizeGL(width_, height_);

	gizmo->AdjustScale(*camera());

	delete fbo;
	fbo = new FBO(width_, height_);
}

void Screen::mousePressEvent(QMouseEvent * e_)
{
	gizmo->MousePressed(e_->pos(), *camera());

	if (!gizmo->IsHover()) {
		makeCurrent();
		QOpenGLFunctions_4_5_Core *f = QOpenGLContext::currentContext()->
			versionFunctions<QOpenGLFunctions_4_5_Core>();
		fbo->Bind();
		float rgb[3];
		QPoint cursor(e_->pos());
		f->glReadPixels(cursor.x(), height() - cursor.y(), 1, 1, GL_RGB, GL_FLOAT, rgb);
		fbo->Unbind();

		QVector4D color(rgb[0], rgb[1], rgb[2], 1.0);
		int idx = modelManager->GetIndexFromColor(color);
		if (idx >= 0) {
			std::vector<Model3D*>& models = modelManager->GetModels();
			bool onoff;
			modelManager->ToggleSelection(idx, onoff);
			qglviewer::Vec pos = modelManager->GetSelectedsCOG();
			gizmo->SetPosition(pos);
			gizmo->AdjustScale(*camera());
			if (onoff) 
				gizmo->Followed(*models[idx]);
			else
				gizmo->UnFollowed(*models[idx]);
		}	
	}

	QGLViewer::mousePressEvent(e_);
	update();
}

void Screen::mouseMoveEvent(QMouseEvent * e_)
{
	QPoint cursor(e_->pos());
	gizmo->MouseMoved(cursor, *camera());
	//FIXME : should not depend on QGLviewer functions.
	//need to handle mouse interaction with my own implementation.
	if (gizmo->IsHover()) {
		setManipulatedFrame(&gizmo->GetFrame());
		if (gizmo == &gizmoTranslate)
			setMouseBinding(Qt::NoModifier, Qt::LeftButton, FRAME,
				QGLViewer::MouseAction::TRANSLATE);
		else if (gizmo == &gizmoRotate)
			setMouseBinding(Qt::NoModifier, Qt::LeftButton, FRAME,
				QGLViewer::MouseAction::ROTATE);
	}
	else {
		setManipulatedFrame(0);
		setMouseBinding(Qt::NoModifier, Qt::LeftButton, CAMERA,
			QGLViewer::MouseAction::ROTATE);
	}

	QGLViewer::mouseMoveEvent(e_);
	update();
}

void Screen::mouseReleaseEvent(QMouseEvent * e_)
{
	gizmo->MouseReleased(e_->pos(), *camera());

	QGLViewer::mouseReleaseEvent(e_);
}

void Screen::wheelEvent(QWheelEvent * e_)
{
	QGLViewer::wheelEvent(e_);

	gizmo->AdjustScale(*camera());
	update();
}
