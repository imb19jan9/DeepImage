#include "Model3D.h"

Model3D::Model3D()
	: vao(0),
	vbo(0),
	ibo(0),
	color(0, 0, 0, 1),
	scaleVec(1, 1, 1)
{
}

Model3D::~Model3D()
{
	delete vao;
	delete vbo;
	delete ibo;
}

void Model3D::Init()
{
	std::vector<float> vertices;
	std::vector<unsigned int> indices;
	mesh.GetVertices(vertices, true, false);
	mesh.GetIndices(indices);

	vao = new VAO;
	vbo = new VBO(vertices.data(), vertices.size() * sizeof(float));
	VBOLayout layout;
	layout.Push<float>(3);
	layout.Push<float>(3);
	vao->AddBuffer(*vbo, layout);

	ibo = new IBO(indices.data(), indices.size());
}

void Model3D::Draw()
{
	QOpenGLFunctions_4_5_Core *f = QOpenGLContext::currentContext()->
		versionFunctions<QOpenGLFunctions_4_5_Core>();

	vao->Bind();
	ibo->Bind();
	f->glDrawElements(GL_TRIANGLES, ibo->GetCount(), GL_UNSIGNED_INT, 0);
}

void Model3D::Load(const std::string & filePath_)
{
	mesh.Read(filePath_);
}

QMatrix4x4 Model3D::ModelMatrix()
{
	QMatrix4x4 scaleMatrix;
	scaleMatrix.scale(scaleVec);
	const double* dm = frame.worldMatrix();
	QMatrix4x4 modelMatrix;
	for (int i = 0; i < 16; i++)
		modelMatrix.data()[i] = dm[i];
	return modelMatrix * scaleMatrix;
}

qglviewer::Vec Model3D::CenterOfMass()
{
	TriMesh::Point com(0, 0, 0);
	for (TriMesh::VertexIter vit = mesh.vertices_begin();
		vit != mesh.vertices_end();
		vit++) {
		com += mesh.point(*vit);
	}
	com /= mesh.n_vertices();
	QVector3D scaled = QVector3D(com[0], com[1], com[2]) * scaleVec;
	return frame.inverseCoordinatesOf(
		qglviewer::Vec(scaled[0], scaled[1], scaled[2]));
}


