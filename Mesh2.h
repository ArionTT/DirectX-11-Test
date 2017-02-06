#pragma once
#pragma comment ( lib, "D3D11.lib")
#pragma comment ( lib, "d3dcompiler.lib")
#pragma comment ( lib,"D3DX11.lib")

#include<D3D11.h>
#include<D3DX11.h>
#include<D3Dcompiler.h>
#include<xnamath.h>
#include<iostream>
#include"tiny_obj_loader.h"

struct VertexType
{
	XMFLOAT3 pos;
	XMFLOAT3 normal;
	XMFLOAT2 tex;
};

class Mesh2
{
public:
	Mesh2()
	{
	}
	~Mesh2()
	{
	}

	bool Initialize(std::string filename, std::string dir = "model/")
	{
		tinyobj::attrib_t attrib;
		std::vector<tinyobj::shape_t> shapes;
		std::vector<tinyobj::material_t> materials;

		std::string err;
		bool ret = tinyobj::LoadObj(&attrib, &shapes, &materials, &err, filename.c_str(), dir.c_str());

		if (!err.empty())
		{ // `err` may contain warning message.
			std::cerr << err << std::endl;
		}

		if (!ret)
		{
			return false;
		}

		int nv = 0;
		// Loop over shapes
		indicesCount = shapes[0].mesh.num_face_vertices.size()*3;
		verticesCount = shapes[0].mesh.num_face_vertices.size()*3;
		vertices = new VertexType[verticesCount];
		indices = new WORD[indicesCount];
		// Loop over faces(polygon)
		size_t index_offset = 0;
		for (size_t f = 0; f < shapes[0].mesh.num_face_vertices.size(); f++)
		{
			int fv = shapes[0].mesh.num_face_vertices[f];
			// Loop over vertices in the face.
			for (size_t v = 0; v < fv; v++)
			{
				// access to vertex
				tinyobj::index_t idx = shapes[0].mesh.indices[index_offset + v];
				vertices[nv].pos.x = attrib.vertices[3 * idx.vertex_index + 0];
				vertices[nv].pos.y = attrib.vertices[3 * idx.vertex_index + 1];
				vertices[nv].pos.z = attrib.vertices[3 * idx.vertex_index + 2];

				vertices[nv].normal.x = attrib.normals[3 * idx.normal_index + 0];
				vertices[nv].normal.y = attrib.normals[3 * idx.normal_index + 1];
				vertices[nv].normal.z = attrib.normals[3 * idx.normal_index + 2];

				vertices[nv].tex.x = attrib.texcoords[2 * idx.texcoord_index + 0];
				vertices[nv].tex.y = attrib.texcoords[2 * idx.texcoord_index + 1];

				std::cout << "Pos x : " << vertices[nv].pos.x << " y : " << vertices[nv].pos.y << " z : " << vertices[nv].pos.z << std::endl;

				nv++;
			}
			index_offset += fv;

			// per-face material
			shapes[0].mesh.material_ids[f];
		}

		for (int i = 0; i < indicesCount; i++)
		{
			indices[i] = i;
		}

		for (int i = 0; i < verticesCount; i++)
		{
			if (boundingRadius < vertices[i].pos.x)
				boundingRadius = vertices[i].pos.x;
			else if (boundingRadius < vertices[i].pos.y)
				boundingRadius = vertices[i].pos.y;
			else if (boundingRadius < vertices[i].pos.z)
				boundingRadius = vertices[i].pos.z;
		}


		return true;
	}

	int GetVertexCount()
	{
		return verticesCount;
	}

	int GetIndicesCount()
	{
		return indicesCount;
	}

	VertexType* GetVertices()
	{
		return vertices;
	}

	WORD* GetIndices()
	{
		return indices;
	}

	float GetBoundingRadius()
	{
		return boundingRadius;
	}


private:
	int verticesCount = 0;
	int indicesCount = 0;
	float boundingRadius = 5.0f;
	VertexType*vertices = NULL;
	WORD*indices = NULL;
};

