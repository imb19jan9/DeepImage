#pragma once

#include <QGLViewer/manipulatedFrame.h>

class GizmoFrame;
class GizmoFrameConstraint : public qglviewer::Constraint
{
private:
	GizmoFrame* gizmoFrame;
	qglviewer::AxisPlaneConstraint *constraint;
	qglviewer::WorldConstraint world;
	qglviewer::LocalConstraint local;

public:
	GizmoFrameConstraint(GizmoFrame* gizmoFrame_);
	~GizmoFrameConstraint();

	inline void DefinedWRTWorld() { constraint = &world; }
	inline void DefinedWRTLocal() { constraint = &local; }

	void SetTranslationConstraint(qglviewer::AxisPlaneConstraint::Type type_,
		const qglviewer::Vec& direction_);
	void SetRotationConstraint(qglviewer::AxisPlaneConstraint::Type type_,
		const qglviewer::Vec& direction_);

private:
	virtual void constrainTranslation(qglviewer::Vec &translation,
		qglviewer::Frame *const frame);
	virtual void constrainRotation(qglviewer::Quaternion &rotation,
		qglviewer::Frame *const frame);
};

class GizmoFrame : public qglviewer::ManipulatedFrame
{
private:
	std::vector<qglviewer::Frame*> followers;
	GizmoFrameConstraint constraint;

public:
	GizmoFrame();
	~GizmoFrame();

	void Followed(qglviewer::Frame& frame_);
	void UnFollowed(qglviewer::Frame& frame_);
	inline void ClearFollower() {
		followers.clear();
	}

	void TranslateFollowers(qglviewer::Vec t_);
	void RotateFollowers(qglviewer::Vec axis_, qglviewer::Vec pos_, float angle_);

	enum ConstraintType {
		TRANSLATE_ALONG_XAXIS,
		TRANSLATE_ALONG_YAXIS,
		TRANSLATE_ALONG_ZAXIS,
		ROTATE_ABOUT_XAXIS,
		ROTATE_ABOUT_YAXIS,
		ROTATE_ABOUT_ZAXIS
	};
	void SetLocalConstraint(ConstraintType type_);
	void SetWorldConstraint(ConstraintType type_);
};