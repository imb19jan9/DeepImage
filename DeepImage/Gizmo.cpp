#include "Gizmo.h"

static const float EPSILON = 1e-6;

static bool IntersectPlane(const qglviewer::Vec &n, const qglviewer::Vec &p0,
	const qglviewer::Vec &l0, const qglviewer::Vec &l, float &t)
{
	// assuming vectors are all normalized
	float denom = n * l;
	if (abs(denom) > EPSILON) {
		qglviewer::Vec p0l0 = p0 - l0;
		t = (p0l0 * n) / denom;
		return (t >= 0);
	}

	return false;
}

Gizmo::Gizmo()
	: vao(0),
	vbo(0),
	ibo(0),
	screenFactor(1.0f),
	dragging(false)
{

}

Gizmo::~Gizmo()
{
	delete vao;
	delete vbo;
	delete ibo;
}

void Gizmo::Init()
{
	std::vector<float> vertices;
	std::vector<unsigned int> indices;
	mesh.GetVertices(vertices, false, false);
	mesh.GetIndices(indices);

	vao = new VAO;
	vbo = new VBO(vertices.data(), vertices.size() * sizeof(float));
	VBOLayout layout;
	layout.Push<float>(3);
	vao->AddBuffer(*vbo, layout);

	ibo = new IBO(indices.data(), indices.size());
}

GizmoTranslate::GizmoTranslate()
	: translateType(NONE)
{
	Create();
}

GizmoTranslate::~GizmoTranslate()
{

}

void GizmoTranslate::Init()
{
	Gizmo::Init();
}

void GizmoTranslate::Draw(QMatrix4x4 view_, QMatrix4x4 proj_, ShaderProgram& prog_)
{
	QOpenGLFunctions_4_5_Core *f = QOpenGLContext::currentContext()->
		versionFunctions<QOpenGLFunctions_4_5_Core>();

	f->glClear(GL_DEPTH_BUFFER_BIT);

	QMatrix4x4 translation;
	translation.translate(1, 0, 0);
	QMatrix4x4 scaleMatrix;
	scaleMatrix.scale(screenFactor);
	const double* dm = frame.worldMatrix();
	QMatrix4x4 modelMatrix;
	for (int i = 0; i < 16; i++)
		modelMatrix.data()[i] = dm[i];
	QMatrix4x4 mvp = proj_ * view_ * modelMatrix * scaleMatrix * translation;

	prog_.Bind();

	//x-axis
	prog_.SetUniformMat4f("u_MVP", mvp.data());
	vao->Bind();
	ibo->Bind();
	if (translateType == X_AXIS)
		prog_.SetUniform4f("u_Color", 1, 1, 1, 1);
	else
		prog_.SetUniform4f("u_Color", 1, 0, 0, 1);
	f->glDrawElements(GL_TRIANGLES, ibo->GetCount(), GL_UNSIGNED_INT, 0);

	//y-axis
	QMatrix4x4 rotation1;
	rotation1.rotate(90, 0, 0, 1);
	mvp = proj_ * view_*modelMatrix*rotation1*scaleMatrix * translation;
	prog_.SetUniformMat4f("u_MVP", mvp.data());
	if (translateType == Y_AXIS)
		prog_.SetUniform4f("u_Color", 1, 1, 1, 1);
	else
		prog_.SetUniform4f("u_Color", 0, 1, 0, 1);
	f->glDrawElements(GL_TRIANGLES, ibo->GetCount(), GL_UNSIGNED_INT, 0);

	//z-axis
	QMatrix4x4 rotation2;
	rotation2.rotate(-90, 0, 1, 0);
	mvp = proj_ * view_*modelMatrix*rotation2*scaleMatrix * translation;
	prog_.SetUniformMat4f("u_MVP", mvp.data());
	if (translateType == Z_AXIS)
		prog_.SetUniform4f("u_Color", 1, 1, 1, 1);
	else
		prog_.SetUniform4f("u_Color", 0, 0, 1, 1);
	f->glDrawElements(GL_TRIANGLES, ibo->GetCount(), GL_UNSIGNED_INT, 0);
}

void GizmoTranslate::MousePressed(QPoint p_, const qglviewer::Camera & cam_)
{
	if (translateType == NONE)
		return;
	switch (translateType)
	{
	case GizmoTranslate::X_AXIS:
		frame.SetWorldConstraint(GizmoFrame::TRANSLATE_ALONG_XAXIS);
		break;
	case GizmoTranslate::Y_AXIS:
		frame.SetWorldConstraint(GizmoFrame::TRANSLATE_ALONG_YAXIS);
		break;
	case GizmoTranslate::Z_AXIS:
		frame.SetWorldConstraint(GizmoFrame::TRANSLATE_ALONG_ZAXIS);
		break;
	case GizmoTranslate::NONE:
		break;
	default:
		break;
	}

	dragging = true;
}

void GizmoTranslate::MouseMoved(QPoint p_, const qglviewer::Camera & cam_)
{
	if (!dragging)
		UpdateConstraint(p_, cam_);
}

void GizmoTranslate::MouseReleased(QPoint p_, const qglviewer::Camera & cam_)
{
	if (translateType == NONE)
		return;

	dragging = false;
}

void GizmoTranslate::AdjustScale(const qglviewer::Camera & cam_)
{
	static const float SCALETODEPTH = 0.1f;

	qglviewer::Vec posW = frame.position();
	qglviewer::Vec posC = cam_.cameraCoordinatesOf(posW);
	screenFactor = -posC[2] * SCALETODEPTH;
}

void GizmoTranslate::Create()
{
	static const int SEGMENT_COUNT = 30;
	static const float INNER_RADIUS = 0.1f;
	static const float OUTER_RADIUS = 0.15f;
	static const float CYLINDER_LEN = 0.7f;

	TriMesh::VertexHandle vhEnd1 = mesh.add_vertex(TriMesh::Point(0, 0, 0));
	TriMesh::VertexHandle vhEnd2 = mesh.add_vertex(TriMesh::Point(1, 0, 0));

	std::vector<TriMesh::VertexHandle> vhs1, vhs2, vhs3;
	for (int i = 0; i < SEGMENT_COUNT; i++) {
		float angle = 2 * M_PI*(i / (float)SEGMENT_COUNT);
		float x1 = 0;
		float x2 = CYLINDER_LEN;
		float yi = INNER_RADIUS * cos(angle);
		float zi = INNER_RADIUS * sin(angle);
		float yo = OUTER_RADIUS * cos(angle);
		float zo = OUTER_RADIUS * sin(angle);

		vhs1.push_back(mesh.add_vertex(TriMesh::Point(x1, yi, zi)));
		vhs2.push_back(mesh.add_vertex(TriMesh::Point(x2, yi, zi)));
		vhs3.push_back(mesh.add_vertex(TriMesh::Point(x2, yo, zo)));
	}

	for (int i = 0; i < SEGMENT_COUNT; i++) {
		int nextIndex = (i + 1) % SEGMENT_COUNT;

		mesh.add_face(vhEnd1, vhs1[nextIndex], vhs1[i]);
		mesh.add_face(vhs1[i], vhs2[nextIndex], vhs2[i]);
		mesh.add_face(vhs1[i], vhs1[nextIndex], vhs2[nextIndex]);
		mesh.add_face(vhs2[i], vhs3[nextIndex], vhs3[i]);
		mesh.add_face(vhs2[i], vhs2[nextIndex], vhs3[nextIndex]);
		mesh.add_face(vhEnd2, vhs3[i], vhs3[nextIndex]);
	}
}

void GizmoTranslate::UpdateConstraint(QPoint p_, const qglviewer::Camera & cam_)
{
	static const float HOVERED_DIST = 15.f;
	qglviewer::Vec cursor(p_.x(), p_.y(), 0);

	qglviewer::Vec axisDir[3] = {
		qglviewer::Vec(1, 0, 0),
		qglviewer::Vec(0, 1, 0),
		qglviewer::Vec(0, 0, 1)
	};
	for (int i = 0; i < 3; i++) {
		qglviewer::Vec model1 = screenFactor * axisDir[i];
		qglviewer::Vec model2 = 2 * screenFactor*axisDir[i];
		qglviewer::Vec world1 = frame.inverseCoordinatesOf(model1);
		qglviewer::Vec world2 = frame.inverseCoordinatesOf(model2);
		qglviewer::Vec screen1 = cam_.projectedCoordinatesOf(world1);
		qglviewer::Vec screen2 = cam_.projectedCoordinatesOf(world2);
		screen1[2] = screen2[2] = 0.0;

		qglviewer::Vec disp = cursor - screen1;
		qglviewer::Vec dir = screen2 - screen1;
		float len = dir.normalize();

		if (abs(len) < EPSILON && disp.norm() < HOVERED_DIST) {
			translateType = static_cast<TranslateType>(i);
			return;
		}
		else {
			float t = qMax(0.0f, qMin(len, (float)(disp*dir)));
			qglviewer::Vec proj = screen1 + t * dir;
			float dist = (cursor - proj).norm();
			if (dist < HOVERED_DIST) {
				translateType = static_cast<TranslateType>(i);
				return;
			}
		}
	}

	translateType = static_cast<TranslateType>(3);
}

GizmoRotate::GizmoRotate()
{
	Create();
}

GizmoRotate::~GizmoRotate()
{
}

void GizmoRotate::Init()
{
	Gizmo::Init();
}

void GizmoRotate::Draw(QMatrix4x4 view_, QMatrix4x4 proj_, ShaderProgram& prog_)
{
	QOpenGLFunctions_4_5_Core *f = QOpenGLContext::currentContext()->
		versionFunctions<QOpenGLFunctions_4_5_Core>();

	f->glClear(GL_DEPTH_BUFFER_BIT);
	QMatrix4x4 scaleMatrix;
	scaleMatrix.scale(screenFactor);
	const double* dm = frame.worldMatrix();
	QMatrix4x4 modelMatrix;
	for (int i = 0; i < 16; i++)
		modelMatrix.data()[i] = dm[i];
	QMatrix4x4 mvp = proj_ * view_ * modelMatrix * scaleMatrix;

	prog_.Bind();

	//x-axis
	prog_.SetUniformMat4f("u_MVP", mvp.data());
	vao->Bind();
	ibo->Bind();
	if (rotateType == X_AXIS)
		prog_.SetUniform4f("u_Color", 1, 1, 1, 1);
	else
		prog_.SetUniform4f("u_Color", 1, 0, 0, 1);
	f->glDrawElements(GL_TRIANGLES, ibo->GetCount(), GL_UNSIGNED_INT, 0);

	//y-axis
	QMatrix4x4 rotation1;
	rotation1.rotate(90, 0, 0, 1);
	mvp = proj_ * view_*modelMatrix*rotation1*scaleMatrix;
	prog_.SetUniformMat4f("u_MVP", mvp.data());
	if (rotateType == Y_AXIS)
		prog_.SetUniform4f("u_Color", 1, 1, 1, 1);
	else
		prog_.SetUniform4f("u_Color", 0, 1, 0, 1);
	f->glDrawElements(GL_TRIANGLES, ibo->GetCount(), GL_UNSIGNED_INT, 0);

	//z-axis
	QMatrix4x4 rotation2;
	rotation2.rotate(-90, 0, 1, 0);
	mvp = proj_ * view_*modelMatrix*rotation2*scaleMatrix;
	prog_.SetUniformMat4f("u_MVP", mvp.data());
	if (rotateType == Z_AXIS)
		prog_.SetUniform4f("u_Color", 1, 1, 1, 1);
	else
		prog_.SetUniform4f("u_Color", 0, 0, 1, 1);
	f->glDrawElements(GL_TRIANGLES, ibo->GetCount(), GL_UNSIGNED_INT, 0);
}

void GizmoRotate::MousePressed(QPoint p_, const qglviewer::Camera & cam_)
{
	if (rotateType == NONE)
		return;
	switch (rotateType)
	{
	case GizmoRotate::X_AXIS:
		frame.SetLocalConstraint(GizmoFrame::ROTATE_ABOUT_XAXIS);
		break;
	case GizmoRotate::Y_AXIS:
		frame.SetLocalConstraint(GizmoFrame::ROTATE_ABOUT_YAXIS);
		break;
	case GizmoRotate::Z_AXIS:
		frame.SetLocalConstraint(GizmoFrame::ROTATE_ABOUT_ZAXIS);
		break;
	case GizmoRotate::NONE:
		break;
	default:
		break;
	}

	dragging = true;
}

void GizmoRotate::MouseMoved(QPoint p_, const qglviewer::Camera & cam_)
{
	if (!dragging)
		UpdateConstraint(p_, cam_);
}

void GizmoRotate::MouseReleased(QPoint p_, const qglviewer::Camera & cam_)
{
	if (rotateType == NONE)
		return;

	dragging = false;
}

void GizmoRotate::AdjustScale(const qglviewer::Camera & cam_)
{
	static const float SCALETODEPTH = 0.2f;

	qglviewer::Vec posW = frame.position();
	qglviewer::Vec posC = cam_.cameraCoordinatesOf(posW);
	screenFactor = -posC[2] * SCALETODEPTH;
}

void GizmoRotate::Create()
{
	static const int TORUS_SEGMENT_COUNT = 30;
	static const int CIRCLE_SEGMENT_COUNT = 30;
	static const float TORUS_RADIUS = 1.0f;
	static const float CIRCLE_RADIUS = 0.05f;

	std::vector<qglviewer::Vec> circlePts;
	for (int i = 0; i < CIRCLE_SEGMENT_COUNT; i++) {
		float angle = 2 * M_PI*i / (float)CIRCLE_SEGMENT_COUNT;
		float x = CIRCLE_RADIUS * cos(angle);
		float y = CIRCLE_RADIUS * sin(angle) + TORUS_RADIUS;
		circlePts.push_back(qglviewer::Vec(x, y, 0));
	}

	std::vector<TriMesh::VertexHandle> vhs;
	for (int i = 0; i < TORUS_SEGMENT_COUNT; i++) {
		float angle = 2 * M_PI*i / (float)TORUS_SEGMENT_COUNT;
		qglviewer::Quaternion q(qglviewer::Vec(1, 0, 0), angle);
		for (int j = 0; j < CIRCLE_SEGMENT_COUNT; j++) {
			qglviewer::Vec pt = q.rotate(circlePts[j]);
			vhs.push_back(mesh.add_vertex(TriMesh::Point(pt[0], pt[1], pt[2])));
		}
	}

	for (int i = 0; i < TORUS_SEGMENT_COUNT; i++) {
		for (int j = 0; j < CIRCLE_SEGMENT_COUNT; j++) {
			mesh.add_face(vhs[i*CIRCLE_SEGMENT_COUNT + j],
				vhs[i*CIRCLE_SEGMENT_COUNT + (j + 1) % CIRCLE_SEGMENT_COUNT],
				vhs[((i + 1) % TORUS_SEGMENT_COUNT)*CIRCLE_SEGMENT_COUNT + j]);
			mesh.add_face(vhs[((i + 1) % TORUS_SEGMENT_COUNT)*CIRCLE_SEGMENT_COUNT + j],
				vhs[i*CIRCLE_SEGMENT_COUNT + (j + 1) % CIRCLE_SEGMENT_COUNT],
				vhs[((i + 1) % TORUS_SEGMENT_COUNT)*CIRCLE_SEGMENT_COUNT +
				(j + 1) % CIRCLE_SEGMENT_COUNT]);
		}
	}
}

void GizmoRotate::UpdateConstraint(QPoint p_, const qglviewer::Camera & cam_)
{
	static const float HOVERED_DIST = 15.f;

	qglviewer::Vec originW, dirW;
	cam_.convertClickToLine(p_, originW, dirW);
	qglviewer::Vec originM = frame.coordinatesOf(originW);
	qglviewer::Vec dirM = frame.transformOf(dirW);

	qglviewer::Vec cursor(p_.x(), p_.y(), 0);

	qglviewer::Vec planeNormal[3] = {
		qglviewer::Vec(1, 0, 0),
		qglviewer::Vec(0, 1, 0),
		qglviewer::Vec(0, 0, 1)
	};
	for (int i = 0; i < 3; i++) {
		float t;
		if (!IntersectPlane(planeNormal[i], qglviewer::Vec(0, 0, 0),
			originM, dirM, t))
			continue;

		qglviewer::Vec intersection = originM + t * dirM;
		qglviewer::Vec closestM = screenFactor * intersection / intersection.norm();
		qglviewer::Vec closestW = frame.inverseCoordinatesOf(closestM);
		qglviewer::Vec closestSC = cam_.projectedCoordinatesOf(closestW);
		closestSC[2] = 0;
		float dist = (cursor - closestSC).norm();
		if (dist < HOVERED_DIST) {
			rotateType = static_cast<RotateType>(i);
			return;
		}
	}

	rotateType = static_cast<RotateType>(3);
}
