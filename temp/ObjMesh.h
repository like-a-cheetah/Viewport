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

	float* m_vertices;		//정점 좌표 배열 
	float* m_texCoords;		//텍스쳐 좌표 배열
	float* m_normals;		//normal 좌표 배열

	int* m_Faces;			//면의 세 정점 좌표 번호 배열
	int* m_TextureFace;		//면의 텍스쳐 좌표 번호 배열
	int* m_norface;			//면의 noraml 좌표 번호 배열

	float m_Color[3];		//rgb값 배열

	int m_numVertices;		//정점 개수
	int m_numTexCoords;		//텍스쳐 개수
	int m_numNormals;		//normal 개수
	int m_numFaces;			//면 개수

	bool line;		//오브젝트 선 토글 변수
};

#endif // !defined(AFX_OBJMESH_H__796231AB_E521_4A69_8FDA_D1E69EE353C0__INCLUDED_)


