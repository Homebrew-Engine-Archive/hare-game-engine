#include "PCH.h"
#include "Mesh.h"

namespace hare_graphics
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

	u32 Mesh::getVertexCount()
	{
		return buffer.getSize() / sizeof(Vertex);
	}

	u32	Mesh::getPrimCount()
	{
		return getVertexCount() - 2;
	}

	RenderUnit::RenderOperationType	Mesh::getOperationType()
	{
		return RenderUnit::ROT_TRIANGLE_LIST;
	}

	void Mesh::setTextureUVMap(f32 ul, f32 vt, f32 ur, f32 vb)
	{

	}

	void Mesh::createMesh(u32 r, u32 c, u32 unitWidth, int left, int top, f32 z)
	{
		assert(r > 1 && c > 1);

		buffer.allocate(r * c * sizeof(Vertex));
		Vertex* v = getBuffer();
		for (u32 count1 = 0; count1 < r; ++count1){
			for (u32 count2 = 0; count2 < c; ++count2){
				v[count1 * c + count2].diffuse = -1;
				v[count1 * c + count2].x = unitWidth * count1 + left;
				v[count1 * c + count2].y = unitWidth * count2 + top;
				v[count1 * c + count2].z = z;
				v[count1 * c + count2].u = (f32)count2 / (c - 1);
				v[count1 * c + count2].v = (f32)count1 / (r - 1);
			}
		}
	}

	void Mesh::createUserMesh(u32 count)
	{
		assert(count > 3);
		buffer.allocate(count * sizeof(Vertex));
	}

}