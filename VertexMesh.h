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
	public:

		//Defalt constructor
		VertexMesh() : Position(), TexCoord(), Normal()
		{}

		//overloaded constructor
		VertexMesh(Vector3D pos, Vector2D texcoord, Vector3D normal) : Position(pos), TexCoord(texcoord), Normal(normal)
		{}
		//Copy constructor
		VertexMesh(const VertexMesh& Vertex) : Position(Vertex.Position), TexCoord(Vertex.TexCoord), Normal(Vertex.Normal)
		{}

		~VertexMesh()
		{}
	};
}
