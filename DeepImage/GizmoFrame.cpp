#include "GizmoFrame.h"

GizmoFrameConstraint::GizmoFrameConstraint(GizmoFrame * gizmoFrame_)
	: gizmoFrame(gizmoFrame_),
	constraint(0)
{
}

GizmoFrameConstraint::~GizmoFrameConstraint()
{
}

void GizmoFrameConstraint::SetTranslationConstraint(qglviewer::AxisPlaneConstraint::Type type_, const qglviewer::Vec & direction_)
{
	constraint->setTranslationConstraint(type_, direction_);
}

void GizmoFrameConstraint::SetRotationConstraint(qglviewer::AxisPlaneConstraint::Type type_, const qglviewer::Vec & direction_)
{
	constraint->setRotationConstraint(type_, direction_);
}

void GizmoFrameConstraint::constrainTranslation(qglviewer::Vec & translation, qglviewer::Frame * const frame)
{
	constraint->constrainTranslation(translation, frame);

	gizmoFrame->TranslateFollowers(
		gizmoFrame->inverseTransformOf(translation));
}

void GizmoFrameConstraint::constrainRotation(qglviewer::Quaternion & rotation, qglviewer::Frame * const frame)
{
	constraint->constrainRotation(rotation, frame);

	const qglviewer::Vec axis = frame->inverseTransformOf(rotation.axis());
	const qglviewer::Vec pos = frame->position();
	const float angle = rotation.angle();

	gizmoFrame->RotateFollowers(axis, pos, angle);
}

GizmoFrame::GizmoFrame()
	: constraint(this)
{
	setConstraint(&constraint);
}

GizmoFrame::~GizmoFrame()
{
}

void GizmoFrame::Followed(qglviewer::Frame & frame_)
{
	followers.push_back(&frame_);
}

void GizmoFrame::UnFollowed(qglviewer::Frame & frame_)
{
	std::vector<qglviewer::Frame*>::iterator it = 
		std::find(followers.begin(), followers.end(), &frame_);
	if (it != followers.end()) {
		followers.erase(it);
	}
}

void GizmoFrame::TranslateFollowers(qglviewer::Vec t_)
{
	for (int i = 0; i < followers.size(); i++) {
		followers[i]->translate(t_);
	}
}

void GizmoFrame::RotateFollowers(qglviewer::Vec axis_, qglviewer::Vec pos_, float angle_)
{
	for (int i = 0; i < followers.size(); i++) {
		qglviewer::Quaternion quatM(followers[i]->transformOf(axis_), angle_);
		followers[i]->rotate(quatM);

		qglviewer::Quaternion quatW(axis_, angle_);
		followers[i]->setPosition(
			pos_ + quatW.rotate(followers[i]->position() - pos_));
	}
}

void GizmoFrame::SetLocalConstraint(ConstraintType type_)
{
	constraint.DefinedWRTLocal();
	switch (type_)
	{
	case GizmoFrame::TRANSLATE_ALONG_XAXIS:
		constraint.SetTranslationConstraint(
			qglviewer::AxisPlaneConstraint::AXIS,
			qglviewer::Vec(1, 0, 0));
		break;
	case GizmoFrame::TRANSLATE_ALONG_YAXIS:
		constraint.SetTranslationConstraint(
			qglviewer::AxisPlaneConstraint::AXIS,
			qglviewer::Vec(0, 1, 0));
		break;
	case GizmoFrame::TRANSLATE_ALONG_ZAXIS:
		constraint.SetTranslationConstraint(
			qglviewer::AxisPlaneConstraint::AXIS,
			qglviewer::Vec(0, 0, 1));
		break;
	case GizmoFrame::ROTATE_ABOUT_XAXIS:
		constraint.SetRotationConstraint(
			qglviewer::AxisPlaneConstraint::AXIS,
			qglviewer::Vec(1, 0, 0));
		break;
	case GizmoFrame::ROTATE_ABOUT_YAXIS:
		constraint.SetRotationConstraint(
			qglviewer::AxisPlaneConstraint::AXIS,
			qglviewer::Vec(0, 1, 0));
		break;
	case GizmoFrame::ROTATE_ABOUT_ZAXIS:
		constraint.SetRotationConstraint(
			qglviewer::AxisPlaneConstraint::AXIS,
			qglviewer::Vec(0, 0, 1));
		break;
	default:
		break;
	}
}

void GizmoFrame::SetWorldConstraint(ConstraintType type_)
{
	constraint.DefinedWRTWorld();
	switch (type_)
	{
	case GizmoFrame::TRANSLATE_ALONG_XAXIS:
		constraint.SetTranslationConstraint(
			qglviewer::AxisPlaneConstraint::AXIS,
			qglviewer::Vec(1, 0, 0));
		break;
	case GizmoFrame::TRANSLATE_ALONG_YAXIS:
		constraint.SetTranslationConstraint(
			qglviewer::AxisPlaneConstraint::AXIS,
			qglviewer::Vec(0, 1, 0));
		break;
	case GizmoFrame::TRANSLATE_ALONG_ZAXIS:
		constraint.SetTranslationConstraint(
			qglviewer::AxisPlaneConstraint::AXIS,
			qglviewer::Vec(0, 0, 1));
		break;
	case GizmoFrame::ROTATE_ABOUT_XAXIS:
		constraint.SetRotationConstraint(
			qglviewer::AxisPlaneConstraint::AXIS,
			qglviewer::Vec(1, 0, 0));
		break;
	case GizmoFrame::ROTATE_ABOUT_YAXIS:
		constraint.SetRotationConstraint(
			qglviewer::AxisPlaneConstraint::AXIS,
			qglviewer::Vec(0, 1, 0));
		break;
	case GizmoFrame::ROTATE_ABOUT_ZAXIS:
		constraint.SetRotationConstraint(
			qglviewer::AxisPlaneConstraint::AXIS,
			qglviewer::Vec(0, 0, 1));
		break;
	default:
		break;
	}
}

