#pragma once

#include <OpenMesh/Core/IO/MeshIO.hh>
#include <OpenMesh/Core/Mesh/TriMesh_ArrayKernelT.hh>

struct HCCLTraits : public OpenMesh::DefaultTraits
{
	typedef OpenMesh::Vec4f Color;

	VertexAttributes(OpenMesh::Attributes::Normal | OpenMesh::Attributes::Color);
	FaceAttributes(OpenMesh::Attributes::Normal);
	EdgeAttributes(0);
};

class TriMesh : public OpenMesh::TriMesh_ArrayKernelT<HCCLTraits>
{
public:
	bool Read(std::string filePath_);
	bool Write(std::string filePath_);

	void GetVertices(std::vector<float>& vertices_,
		bool includeNormal_, bool includeColor_);
	void GetIndices(std::vector<unsigned int>& indices_);
};