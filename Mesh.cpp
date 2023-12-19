#include "Mesh.h"
#include <vector>

#include "GraphicsEngine.h"
#include "RenderSystem.h"
#include "VertexMesh.h"

#include <locale>
#include <codecvt>
#include <Windows.h>
#define TINYOBJLOADER_IMPLEMENTATION
#include <tiny_obj_loader.h>

Mesh::Mesh(const wchar_t* FullPath) :Resource(FullPath)
{
	tinyobj::attrib_t attributes;
	std::vector<tinyobj::shape_t> shapes;
	std::vector<tinyobj::material_t> materials;

	std::string warn;
	std::string err;

	int size_needed = WideCharToMultiByte(CP_UTF8, 0, &FullPath[0], wcslen(FullPath), NULL, 0, NULL, NULL);
	std::string InputFile(size_needed, 0);
	WideCharToMultiByte(CP_UTF8, 0, &FullPath[0], wcslen(FullPath), &InputFile[0], size_needed, NULL, NULL);

	//std::string InputFile = //std::wstring_convert<std::codecvt_utf8<wchar_t>>().to_bytes(FullPath);

	std::string MTL_Dir = InputFile.substr(0, InputFile.find_last_of("\\/"));

	bool bRes = tinyobj::LoadObj(&attributes, &shapes, &materials, &warn, &err, InputFile.c_str(), MTL_Dir.c_str());

	if (!err.empty())
		throw std::exception("Mesh failed to load");
	if (!bRes) {
		throw std::exception("Mesh failed to load");
	}

	std::vector<Math::VertexMesh> VerticesList;
	std::vector<unsigned int> IndicesList;

	size_t SizeVertexIndexLists = 0;

	for (size_t i = 0; i < shapes.size(); ++i) {
		SizeVertexIndexLists += shapes[i].mesh.indices.size();
	}
	
	VerticesList.reserve(SizeVertexIndexLists);
	IndicesList.reserve(SizeVertexIndexLists);

	MaterialSlots.resize(materials.size());

	size_t GlobalIndexOffset = 0;

	for (size_t m = 0; m < materials.size(); ++m) {
		MaterialSlots[m].StartIndex = GlobalIndexOffset;
		MaterialSlots[m].MaterialId = m;

		//Go through each shape
		for (size_t s = 0; s < shapes.size(); ++s) {
			size_t IndexOffset = 0;

			//Go through each face of the shape 
			for (size_t f = 0; f < shapes[s].mesh.num_face_vertices.size(); ++f) {
				if (shapes[s].mesh.material_ids[f] != m) continue;
				unsigned char NumFaceVerts = shapes[s].mesh.num_face_vertices[f];

				for (unsigned char v = 0; v < NumFaceVerts; ++v) {
					tinyobj::index_t Index = shapes[s].mesh.indices[IndexOffset + v];

					tinyobj::real_t vx = attributes.vertices[Index.vertex_index * 3 + 0];
					tinyobj::real_t vy = attributes.vertices[Index.vertex_index * 3 + 1];
					tinyobj::real_t vz = attributes.vertices[Index.vertex_index * 3 + 2];

					tinyobj::real_t tx = attributes.texcoords[Index.texcoord_index * 2 + 0];
					tinyobj::real_t ty = attributes.texcoords[Index.texcoord_index * 2 + 1];

					//Get the normals
					tinyobj::real_t nx = attributes.normals[Index.normal_index * 3 + 0];
					tinyobj::real_t ny = attributes.normals[Index.normal_index * 3 + 1];
					tinyobj::real_t nz = attributes.normals[Index.normal_index * 3 + 2];

					Math::VertexMesh vMesh(Math::Vector3D(vx, vy, vz), Math::Vector2D(tx, ty), Math::Vector3D(nx, ny, nz));
					VerticesList.push_back(vMesh);

					IndicesList.push_back(static_cast<unsigned int>(GlobalIndexOffset) + v);
				}
				IndexOffset += NumFaceVerts;
				GlobalIndexOffset += NumFaceVerts;
			}
		}
		MaterialSlots[m].NumIndices = GlobalIndexOffset - MaterialSlots[m].StartIndex;
	}

	void* ShaderByteCode = nullptr;
	//the size of mem buffer in bytes
	size_t SizeOfShader = 0;
	GraphicsEngine::Get()->GetVertexMeshLayoutSharedByteCodeAndSize(&ShaderByteCode, &SizeOfShader);
	mVertexBuffer = GraphicsEngine::Get()->GetRenderSystem()->CreateVertexBuffer(&VerticesList[0], sizeof(Math::VertexMesh), static_cast<UINT>(VerticesList.size()), ShaderByteCode, static_cast<UINT>(SizeOfShader));

	mIndexBuffer = GraphicsEngine::Get()->GetRenderSystem()->MakeIndexBuffer(&IndicesList[0], (UINT)IndicesList.size());
}


Mesh::~Mesh()
{
	//delete this;
}
