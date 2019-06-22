#include "CheckerBoard.h"

#include "VBOLayout.h"

CheckerBoard::CheckerBoard(int nx_, int ny_)
	: vao(0),
	vbo(0),
	ibo(0),
	nx(nx_),
	ny(ny_)
{
	Create();
}

CheckerBoard::~CheckerBoard()
{
	delete vao;
	delete vbo;
	delete ibo;
}

void CheckerBoard::Init()
{
	std::vector<float> vertices;
	std::vector<unsigned int> indices;
	mesh.GetVertices(vertices, false, true);
	mesh.GetIndices(indices);

	vao = new VAO;
	vbo = new VBO(vertices.data(), vertices.size() * sizeof(float));
	ibo = new IBO(indices.data(), indices.size());

	VBOLayout layout;
	layout.Push<float>(3);
	layout.Push<float>(4);
	vao->AddBuffer(*vbo, layout);
}

void CheckerBoard::Draw(QMatrix4x4 view_, QMatrix4x4 proj_, ShaderProgram& prog_)
{
	QOpenGLFunctions_4_5_Core *f = QOpenGLContext::currentContext()->
		versionFunctions<QOpenGLFunctions_4_5_Core>();

	QMatrix4x4 mvp = proj_ * view_;
	prog_.Bind();
	prog_.SetUniformMat4f("u_MVP", mvp.data());

	vao->Bind();
	ibo->Bind();
	f->glDrawElements(GL_TRIANGLES, ibo->GetCount(), GL_UNSIGNED_INT, 0);
}

void CheckerBoard::Create()
{
	float squareSize = 10.0f;
	for (int i = 0; i < nx; i++) {
		for (int j = 0; j < ny; j++) {
			TriMesh::Point p0((i + 0)*squareSize, (j + 0)*squareSize, 0);
			TriMesh::Point p1((i + 1)*squareSize, (j + 0)*squareSize, 0);
			TriMesh::Point p2((i + 1)*squareSize, (j + 1)*squareSize, 0);
			TriMesh::Point p3((i + 0)*squareSize, (j + 1)*squareSize, 0);

			std::vector< TriMesh::VertexHandle> vhs;
			if ((i + j) % 2 != 0) {
				vhs.push_back(mesh.add_vertex(p0));
				vhs.push_back(mesh.add_vertex(p1));
				vhs.push_back(mesh.add_vertex(p2));
				vhs.push_back(mesh.add_vertex(p3));
				for (int k = 0; k < 4; k++)
					mesh.set_color(vhs[k], TriMesh::Color(0, 0, 0, 1));

				mesh.add_face(vhs[0], vhs[1], vhs[2]);
				mesh.add_face(vhs[2], vhs[3], vhs[0]);
			}
			else {
				vhs.push_back(mesh.add_vertex(p0));
				vhs.push_back(mesh.add_vertex(p1));
				vhs.push_back(mesh.add_vertex(p2));
				vhs.push_back(mesh.add_vertex(p3));
				for (int k = 0; k < 4; k++)
					mesh.set_color(vhs[k], TriMesh::Color(1, 1, 1, 1));

				mesh.add_face(vhs[0], vhs[1], vhs[2]);
				mesh.add_face(vhs[2], vhs[3], vhs[0]);
			}
		}
	}
}
