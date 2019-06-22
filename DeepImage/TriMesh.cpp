#include "TriMesh.h"

bool TriMesh::Read(std::string filePath_)
{
	OpenMesh::IO::Options ropt;
	if (!OpenMesh::IO::read_mesh(*this, filePath_, ropt)) {
		std::cerr << "File Open Error: Error loading mesh from file "
			<< filePath_ << std::endl;
		return false;
	}

	// If the file did not provide vertex normals, then calculate them
	if (!ropt.check(OpenMesh::IO::Options::VertexNormal) &&
		has_face_normals() && has_vertex_normals()) {
		// let the mesh update the normals
		update_normals();
	}

	return true;
}

bool TriMesh::Write(std::string filePath_)
{
	OpenMesh::IO::Options wopt;
	if (!OpenMesh::IO::write_mesh(*this, filePath_, wopt)) {
		std::cerr << "Cannot write mesh to file"
			<< filePath_ << std::endl;
		return false;
	}

	return true;
}
void TriMesh::GetVertices(std::vector<float>& vertices_,
	bool includeNormal_, bool includeColor_)
{
	vertices_.clear();

	if (includeNormal_)
		update_normals();

	for (TriMesh::VertexIter vit = vertices_begin();
		vit != vertices_end();
		vit++) {
		TriMesh::Point p = point(*vit);
		vertices_.push_back(p[0]);
		vertices_.push_back(p[1]);
		vertices_.push_back(p[2]);

		if (includeNormal_) {
			TriMesh::Normal n = normal(*vit);
			vertices_.push_back(n[0]);
			vertices_.push_back(n[1]);
			vertices_.push_back(n[2]);
		}

		if (includeColor_) {
			TriMesh::Color c = color(*vit);
			vertices_.push_back(c[0]);
			vertices_.push_back(c[1]);
			vertices_.push_back(c[2]);
			vertices_.push_back(c[3]);
		}
	}
}

void TriMesh::GetIndices(std::vector<unsigned int>& indices_)
{
	indices_.clear();

	for (TriMesh::FaceIter fit = faces_begin();
		fit != faces_end();
		fit++) {
		for (TriMesh::FVIter fvit = fv_begin(*fit);
			fvit != fv_end(*fit);
			fvit++) {
			indices_.push_back(fvit->idx());
		}
	}
}
