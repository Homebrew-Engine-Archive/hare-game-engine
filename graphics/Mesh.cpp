#include "PCH.h"
#include "Mesh.h"

namespace hare
{
	Mesh::Mesh()
	{

	}

	Mesh::~Mesh()
	{

	}

	Vertex* Mesh::getBuffer()
	{
		return (Vertex*)buffer.getData();
	}

	uint32 Mesh::getVertexCount()
	{
		return buffer.getSize() / sizeof(Vertex);
	}

	uint32	Mesh::getPrimCount()
	{
		return getVertexCount() - 2;
	}

	RenderUnit::RenderOperationType	Mesh::getOperationType()
	{
		return RenderUnit::ROT_TRIANGLE_LIST;
	}

	void Mesh::setTextureUVMap(float ul, float vt, float ur, float vb)
	{

	}

	void Mesh::createMesh(uint32 r, uint32 c, uint32 unitWidth, int left, int top, float z)
	{
		assert(r > 1 && c > 1);

		buffer.allocate(r * c * sizeof(Vertex));
		Vertex* v = getBuffer();
		for (uint32 count1 = 0; count1 < r; ++count1){
			for (uint32 count2 = 0; count2 < c; ++count2){
				v[count1 * c + count2].diffuse = -1;
				v[count1 * c + count2].x = (float)unitWidth * count1 + left;
				v[count1 * c + count2].y = (float)unitWidth * count2 + top;
				v[count1 * c + count2].z = z;
				v[count1 * c + count2].u = (float)count2 / (c - 1);
				v[count1 * c + count2].v = (float)count1 / (r - 1);
			}
		}
	}

	void Mesh::createUserMesh(uint32 count)
	{
		assert(count > 3);
		buffer.allocate(count * sizeof(Vertex));
	}
}
