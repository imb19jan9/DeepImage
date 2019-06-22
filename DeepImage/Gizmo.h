#pragma once

#include "GizmoFrame.h"
#include "VAO.h"
#include "VBO.h"
#include "IBO.h"
#include "TriMesh.h"
#include "ShaderProgram.h"
#include "Model3D.h"

class Gizmo
{
protected:
	GizmoFrame frame;
	float screenFactor;

	VAO* vao;
	VBO* vbo;
	IBO* ibo;

	TriMesh mesh;

	bool dragging;

public:
	Gizmo();
	~Gizmo();

	virtual void Init();
	virtual void Draw(QMatrix4x4 view_, QMatrix4x4 proj_, ShaderProgram& prog_) = 0;

	virtual void MousePressed(QPoint p_, const qglviewer::Camera& cam_) = 0;
	virtual void MouseMoved(QPoint p_, const qglviewer::Camera& cam_) = 0;
	virtual void MouseReleased(QPoint p_, const qglviewer::Camera& cam_) = 0;

	virtual void AdjustScale(const qglviewer::Camera& cam_) = 0;
	void SetPosition(qglviewer::Vec pos_) { frame.setPosition(pos_); }

	virtual inline bool IsHover() = 0;

	inline qglviewer::ManipulatedFrame& GetFrame() { return frame; }

	void Followed(Model3D& model_) {
		frame.Followed(model_.GetFrame());
	}
	void UnFollowed(Model3D& model_) {
		frame.UnFollowed(model_.GetFrame());
	}
	void ClearFollower() {
		frame.ClearFollower();
	}
};

class GizmoTranslate : public Gizmo
{
private:
	enum TranslateType {
		X_AXIS, Y_AXIS, Z_AXIS, NONE
	};
	TranslateType translateType;

public:
	GizmoTranslate();
	~GizmoTranslate();

	virtual void Init();
	virtual void Draw(QMatrix4x4 view_, QMatrix4x4 proj_, ShaderProgram& prog_);

	virtual void MousePressed(QPoint p_, const qglviewer::Camera& cam_);
	virtual void MouseMoved(QPoint p_, const qglviewer::Camera& cam_);
	virtual void MouseReleased(QPoint p_, const qglviewer::Camera& cam_);

	virtual void AdjustScale(const qglviewer::Camera& cam_);

	virtual inline bool IsHover() { return translateType != NONE; }

private:
	void Create();

	void UpdateConstraint(QPoint p_, const qglviewer::Camera & cam_);
};

class GizmoRotate : public Gizmo
{
private:
	enum RotateType {
		X_AXIS, Y_AXIS, Z_AXIS, NONE
	};
	RotateType rotateType;

public:
	GizmoRotate();
	~GizmoRotate();

	virtual void Init();
	virtual void Draw(QMatrix4x4 view_, QMatrix4x4 proj_, ShaderProgram& prog_);

	virtual void MousePressed(QPoint p_, const qglviewer::Camera& cam_);
	virtual void MouseMoved(QPoint p_, const qglviewer::Camera& cam_);
	virtual void MouseReleased(QPoint p_, const qglviewer::Camera& cam_);

	virtual void AdjustScale(const qglviewer::Camera& cam_);

	virtual inline bool IsHover() { return rotateType != NONE; }

private:
	void Create();

	void UpdateConstraint(QPoint p_, const qglviewer::Camera & cam_);
};