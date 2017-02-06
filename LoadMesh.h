#pragma once
#pragma comment ( lib, "D3D11.lib")
#pragma comment ( lib,"D3DX11.lib")

#include<D3D11.h>
#include<D3DX11.h>
#include<xnamath.h>
#include<iostream>
#include<fstream>
#include<string>

using namespace std;

struct IndexType
{
	int vertex;
	int normal;
	int tex;
};

struct VertexType
{
	XMFLOAT3 pos;
	XMFLOAT3 normal;
	XMFLOAT2 tex;
};

class LoadMesh
{
public:
	LoadMesh() 
	{
		verticesCount = 0;
		indexCount = 0;
		m_normal = NULL;
		m_tex = NULL;
		m_vertices = NULL;
	}
	~LoadMesh() {}

public:
	bool InializeMeshTri(char*filename)
	{
		isTriangel = true;
		ifstream file;
		string line;
		file.open(filename, ios::in);
		if (file.fail())
		{
			cout << "File Open Failed" << endl;
			return false;
		}

		while (getline(file, line))
		{
			const char*data = line.c_str();
			if ('v' == data[0] && ' ' == data[1])
				verticesCount++;
			else if ('v' == data[0] && 'n' == data[1] && ' ' == data[2])
				normalCount++;
			else if ('v' == data[0] && 't' == data[1] && ' ' == data[2])
				texCount++;
			else if ('f' == data[0] && ' ' == data[1])
				indexCount++;
		}
		file.close();

		cout << "vn " << verticesCount << "   tn " << texCount << "   nn " << normalCount << "  fn" << indexCount << endl;
		//Build Array
		m_vertices = new float[verticesCount * 3];
		m_normal = new float[normalCount * 3];
		m_tex = new float[texCount * 2];
		indexType = new IndexType[indexCount * 3 ];

		file.open(filename, ios::in);
		if (file.fail())
		{
			cout << "File Open Failed" << endl;
			return false;
		}

		char ch[9];
		char ch2[5];
		memset(ch, '\0', 9);
		memset(ch2, '\0', 5);
		int n = 0;
		int nv = 0;
		int nn = 0;
		int nt = 0;
		int ni = 0;
		int splitNum = 1;

		while (getline(file, line))
		{
			const char*data = line.c_str();
			if (data[0] == 'v'&&data[1] == ' ')
			{
				//Vertex
				for (int a = 2; a < line.length(); a++)
				{
					if (' ' == data[a])
					{
						m_vertices[nv++] = Char2Float(ch, 9);
						n = 0;
					}
					else
					{
						ch[n++] = data[a];
					}
					if ((a + 1) == line.length())
					{
						m_vertices[nv++] = Char2Float(ch, 9);
						n = 0;
					}
				}
			}
			else if (data[0] == 'v'&&data[1] == 'n')
			{
				//Normal
				for (int a = 3; a < line.length(); a++)
				{
					if (' ' == data[a])
					{
						m_normal[nn] = Char2Float(ch, 9);
						nn++;
						n = 0;
					}
					else
					{
						ch[n++] = data[a];
					}
					if ((a + 1) == line.length())
					{
						m_normal[nn] = Char2Float(ch, 9);
						nn++;
						n = 0;
					}
				}
			}
			else if (data[0] == 'v'&&data[1] == 't')
			{
				//Texcoord
				for (int a = 3; a < line.length(); a++)
				{
					if (' ' == data[a])
					{
						char temp = data[a];
						m_tex[nt] = Char2Float(ch, 9);
						nt++;
						n = 0;
					}
					else
					{
						ch[n++] = data[a];
					}
					if ((a + 1) == line.length())
					{
						m_tex[nt] = Char2Float(ch, 9);
						nt++;
						n = 0;
					}
				}
			}
			else if (data[0] == 'f'&&data[1] == ' ')
			{
				//Faces  
				for (int a = 2; a < line.length(); a++)
				{
					if (data[a] == '/')
					{
						n = 0;
						if (splitNum == 1)
						{
							indexType[ni].vertex = Char2Int(ch2, 2);
							memset(ch2, '\0', 5);
							splitNum++;
						}
						else if (splitNum == 2)
						{
							indexType[ni].tex = Char2Int(ch2, 2);
							memset(ch2, '\0', 5);
							splitNum = 1;
						}
					}
					else if (data[a] == ' ')
					{
						n = 0;
						indexType[ni].normal = Char2Int(ch2, 2);
						memset(ch2, '\0', 5);
						splitNum = 1;
						ni++;
					}
					else
					{
						ch2[n] = data[a];
						n++;
					}

					if ((a + 1) == line.length())
					{
						indexType[ni].normal = Char2Int(ch2, 2);
						memset(ch2, '\0', 5);
						splitNum = 1;
						ni++;
						n = 0;
						break;
					}
				}
			}
		}


		file.close();


		return true;
	}

	bool InitializeMeshQuad(char*filename)
	{
		isTriangel = false;
		ifstream file;
		char*buffer;
		int filesize = 0;
		string line;
		file.open(filename, ios::in);
		if (file.fail())
		{
			cout << "File Open Failed" << endl;
		}
		file.seekg(0, std::ios::end);
		filesize = (int)file.tellg();
		file.seekg(0, std::ios::beg);
		buffer = new char[filesize];
		//Write Obj file to Buffer
		file.read(buffer, filesize);
		int i = 0;
		//Get Vertex Numbers
		while (true)
		{
			if (buffer[i] == 'v')
			{
				if (buffer[i + 1] == ' ')
				{
					verticesCount++;
				}
				else if (buffer[i + 1] == 't')
				{
					texCount++;
				}
				else if (buffer[i + 1] == 'n')
					normalCount++;
			}
			else if (buffer[i] == 'f')
				if (buffer[i + 1] == ' ')
					indexCount++;
			i++;
			if (i == filesize)
				break;
		}
		cout << "vn " << verticesCount << "   tn " << texCount << "   nn " << normalCount <<"  fn"<<indexCount<< endl;
		i = 0;
		//Build Array
		m_vertices = new float[verticesCount * 3];
		m_normal = new float[normalCount * 3];
		m_tex = new float[texCount * 2];
		indexType = new IndexType[indexCount*4];

		//Reset File
		file.close();

		file.open(filename, ios::in);
		if (file.fail())
		{
			cout << "File Open Failed" << endl;
		}

		
			char ch[9];
			char ch2[2];
			memset(ch, '\0', 9);
			memset(ch2, '\0', 2);
			int n = 0;
			int nv = 0;
			int nn = 0;
			int nt = 0;
			int ni = 0;
			int splitNum = 1;

			while (getline(file, line))
			{
				const char*data = line.c_str();
				if (data[0] == 'v'&&data[1] == ' ')
				{
					//Vertex
					for (int a = 2; a < line.length(); a++)
					{
						if (' ' == data[a])
						{
							m_vertices[nv++] = Char2Float(ch, 9);
							n = 0;
						}
						else
						{
							ch[n++] = data[a];
						}
						if ((a + 1) == line.length())
						{
							m_vertices[nv++] = Char2Float(ch, 9);
							n = 0;
						}
					}
				}
				else if (data[0] == 'v'&&data[1] == 'n')
				{
					//Normal
					for (int a = 3; a < line.length(); a++)
					{
						if (' ' == data[a])
						{
							m_normal[nn] = Char2Float(ch, 9);
							nn++;
							n = 0;
						}
						else
						{
							ch[n++] = data[a];
						}
						if ((a + 1) == line.length())
						{
							m_normal[nn] = Char2Float(ch, 9);
							nn++;
							n = 0;
						}
					}
				}
				else if (data[0] == 'v'&&data[1] == 't')
				{
					//Texcoord
					for (int a = 3; a < line.length(); a++)
					{
						if (' ' == data[a])
						{
							char temp = data[a];
							m_tex[nt] = Char2Float(ch, 9);
							nt++;
							n = 0;
						}
						else
						{
							ch[n++] = data[a];
						}
						if ((a + 1) == line.length())
						{
							m_tex[nt] = Char2Float(ch, 9);
							nt++;
							n = 0;
						}
					}
				}
				else if (data[0] == 'f'&&data[1] == ' ')
				{
					cout << "FACE   " << line << endl;
					//Faces  (48) 
					for (int a = 2; a < line.length(); a++)
					{
						if (data[a] == '/')
						{
							n = 0;
							if (splitNum == 1)
							{
								indexType[ni].vertex = Char2Int(ch2, 2);
								memset(ch2, '\0', 2);
								splitNum++;
							}
							else if (splitNum == 2)
							{
								indexType[ni].tex = Char2Int(ch2, 2);
								memset(ch2, '\0', 2);
								splitNum = 1;
							}
						}
						else if (data[a] == ' ')
						{
							n = 0;
							indexType[ni].normal = Char2Int(ch2, 2);
							memset(ch2, '\0', 2);
							splitNum = 1;
							ni++;
						}
						else
						{
							ch2[n] = data[a];
							n++;
						}

						if ((a + 1) == line.length())
						{
							indexType[ni].normal = Char2Int(ch2, 2);
							memset(ch2, '\0', 2);
							splitNum = 1;
							ni++; 
							n = 0;
							break;
						}
					}
				}
			}
		

		file.close();

		return true;
	}

	bool Release()
	{
		delete m_normal;
		delete m_tex;
		delete m_vertices;
		m_normal = 0;
		m_tex = 0;
		m_vertices = 0;
		indexCount = 0;
		verticesCount = 0;
		return true;
	}

	bool GetVertices(VertexType *vertex,int &vertexLen,bool isTriangel)
	{
		if (isTriangel&&this->isTriangel)
			GetVerticesTri(vertex, vertexLen);
		else if (!isTriangel && !this->isTriangel)
			GetVerticesQuad(vertex, vertexLen);
		else if (isTriangel && !this->isTriangel)
		{
			cout << "Not Inialize to Triangel Face" <<endl;
			return false;
		}
		else
		{
			cout << "Not Inialize to Quad Face" << endl;
			return false;
		}
		return true;
	}

	bool GetIndices(WORD*indices,int &indicesLen,bool isTriangel)
	{
		if (isTriangel&&this->isTriangel)
			GetIndicesTri(indices, indicesLen);
		else if (!isTriangel && !this->isTriangel)
			GetIndicesQuad(indices, indicesLen);
		else if (isTriangel && !this->isTriangel)
		{
			cout << "Not Inialize to Triangel Face" << endl;
			return false;
		}
		else
		{
			cout << "Not Inialize to Quad Face" << endl;
			return false;
		}
		return true;
	}

	unsigned int GetIndicesNum()
	{
		return indexCount;
	}

	unsigned int GetVerticesNum()
	{
		return verticesCount;
	}

	//#Convert (char*)string to float
	float Char2Float(char*num,int numLen)
	{
		float n = 0;
		float n1 = 0;
		int tag = -1;
		bool minus = false;
		for (int i = numLen-1; i >= 0; i--)
		{
			float t = 0;
			if (num[i] == '0')
			{
				t = 0;
				n = (n + t) / 10;
			}
			else if (num[i] == '1')
			{
				t = 1;
				n = (n + t) / 10;
			}
			else if (num[i] == '2')
			{
				t = 2;
				n = (n + t) / 10;
			}
			else if (num[i] == '3')
			{
				t = 3;
				n = (n + t) / 10;
			}
			else if (num[i] == '4')
			{
				t = 4;
				n = (n + t) / 10;
			}
			else if (num[i] == '5')
			{
				t = 5;
				n = (n + t) / 10;
			}
			else if (num[i] == '6')
			{
				t = 6;
				n = (n + t) / 10;
			}
			else if (num[i] == '7')
			{
				t = 7;
				n = (n + t) / 10;
			}
			else if (num[i] == '8')
			{
				t = 8;
				n = (n + t) / 10;
			}
			else if (num[i] == '9')
			{
				t = 9;
				n = (n + t) / 10;
			}
			else if (num[i] == '.')
			{
				if (num[0] != '-')
				{
					for (int j = 0; j < i; j++)
					{
						n1 *= 10;
						n1 += (int)num[j]-48;
					}
				}
				else
				{
					for (int j = 1; j < i; j++)
					{
						n1 *= 10;
						n1 += (int)num[j]-48;
					}
				}
				break;
			}
		}
		n += n1;
		if (num[0] == '-')
		{
			n *= -1.0f;
		}
			
		return n;
	}

	//#Not complete version
	float Char2FloatV2(char*num, int numLen)
	{
		float sumInt = 0;
		float sumFloat = 0;
		int dotPos;
		for (int i = numLen - 1; i >= 0; i--)
		{
			if (num[i] == '.')
			{
				dotPos = i;
				break;
			}
		}
		for (int i = numLen - 1; i > dotPos; i--)
		{
			sumFloat /= 10;
			sumFloat += (int)num[i] - 47;
		}
		sumFloat;
		for (int i = 0; i < dotPos; i++)
		{
			sumInt *= 10;
			sumInt += (int)num[i] - 47;
		}

		return sumFloat + sumInt;
	}

	//#Convert (char*)string to int
	int Char2Int(char*num, int numLen)
	{
		int sum= 0;
		int n = 0;
		bool minus = false;
		if (num[0] == '-')
		{
			minus = true;
			n = 1;
		}
		for (int i = n; i < numLen; i++)
		{
			if (num[i] == '\0')
				break;
			sum *= 10;
			sum += (int)num[i]-48;
		}
		if (minus)
			sum *= -1;
		return sum;
	}
	
	bool GetVerticesTri(VertexType *&vertex, int &vertexLen)
	{
		for (int i = 0; i < vertexLen; i++)
		{
			//position
			vertex[i].pos.x = m_vertices[(indexType[i].vertex - 1) * 3];
			vertex[i].pos.y = m_vertices[(indexType[i].vertex - 1) * 3 + 1];
			vertex[i].pos.z = m_vertices[(indexType[i].vertex - 1) * 3 + 2];
			//texcoord
			vertex[i].tex.x = m_tex[(indexType[i].tex - 1) * 2];
			vertex[i].tex.y = m_tex[(indexType[i].tex - 1) * 2 + 1];
			//normal
			vertex[i].normal.x = m_normal[(indexType[i].normal - 1) * 3];
			vertex[i].normal.y = m_normal[(indexType[i].normal - 1) * 3 + 1];
			vertex[i].normal.z = m_normal[(indexType[i].normal - 1) * 3 + 2];
		}

		return true;
	}

	bool GetIndicesTri(WORD*indices, int &indicesLen)
	{
		for (int i = 0; i < indicesLen; i++)
		{
			indices[i] = i;
		}
		return true;
	}

	bool GetVerticesQuad(VertexType *vertex, int &vertexLen)
	{
		return true;
	}

	bool GetIndicesQuad(WORD*indices, int &indicesLen)
	{
		return true;
	}

public:
	float*m_vertices;
	float*m_tex;
	float*m_normal;
	int normalCount;
	int texCount;
	unsigned int verticesCount;
	unsigned int indexCount;
	bool isTriangel=true;

	IndexType*indexType = NULL;
};
