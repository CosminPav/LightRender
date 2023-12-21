#include "Vector3D.h"
#include "Vector2D.h"

namespace Math
{
	class VertexMesh
	{
	public:

		Vector3D Position;
		Vector2D TexCoord;
		Vector3D Normal;
		Vector3D Tangent;
		Vector3D Bitangent;
	public:

		//Defalt constructor
		VertexMesh() : Position(), TexCoord(), Normal(), Tangent(), Bitangent()
		{}

		//overloaded constructor
		VertexMesh(const Vector3D& pos,const Vector2D& texcoord, const Vector3D& normal, const Vector3D& tangent, const Vector3D& binormal) 
			: Position(pos), TexCoord(texcoord), Normal(normal), Tangent(tangent), Bitangent(binormal)
		{}
		//Copy constructor
		VertexMesh(const VertexMesh& Vertex) : Position(Vertex.Position), TexCoord(Vertex.TexCoord), Normal(Vertex.Normal), Tangent(Vertex.Tangent), Bitangent(Vertex.Bitangent)
		{}

		~VertexMesh()
		{}
	};
}
