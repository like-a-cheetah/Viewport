#pragma once
// ObjMesh.h: interface for the ObjMesh class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_OBJMESH_H__796231AB_E521_4A69_8FDA_D1E69EE353C0__INCLUDED_)
#define AFX_OBJMESH_H__796231AB_E521_4A69_8FDA_D1E69EE353C0__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class ObjMesh
{
public:
	ObjMesh();
	virtual ~ObjMesh();
	void Render();
	void SetColor(float r, float g, float b);

	float* m_vertices;		//���� ��ǥ �迭 
	float* m_texCoords;		//�ؽ��� ��ǥ �迭
	float* m_normals;		//normal ��ǥ �迭

	int* m_Faces;			//���� �� ���� ��ǥ ��ȣ �迭
	int* m_TextureFace;		//���� �ؽ��� ��ǥ ��ȣ �迭
	int* m_norface;			//���� noraml ��ǥ ��ȣ �迭

	float m_Color[3];		//rgb�� �迭

	int m_numVertices;		//���� ����
	int m_numTexCoords;		//�ؽ��� ����
	int m_numNormals;		//normal ����
	int m_numFaces;			//�� ����

	bool line;		//������Ʈ �� ��� ����
};

#endif // !defined(AFX_OBJMESH_H__796231AB_E521_4A69_8FDA_D1E69EE353C0__INCLUDED_)


