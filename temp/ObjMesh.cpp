// ObjMesh.cpp: implementation of the ObjMesh class.
//
//////////////////////////////////////////////////////////////////////

#include "ObjMesh.h"
#include <STDIO.H>
#include <stdlib.h>
#include <gl/glut.h>
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////


ObjMesh::ObjMesh()	//생성자
{
	m_vertices = NULL;
	m_texCoords = NULL;
	m_normals = NULL;

	m_Faces = NULL;
	m_TextureFace = NULL;

	m_numTexCoords = m_numVertices = m_numFaces = m_numNormals = 0;

	m_Color[0] = 1;
	m_Color[1] = 1;
	m_Color[2] = 1;

	line = true;
}

ObjMesh::~ObjMesh()	//소멸자
{
	 	if(m_vertices != NULL)
	 		free(m_vertices);
	 	if(m_normals != NULL)
	 		free(m_normals);
	 	if(m_Faces != NULL)
	 		free(m_Faces);
	 	if(m_texCoords != NULL)
	 		free(m_texCoords);
	 	if(m_TextureFace != NULL)
	 		free(m_TextureFace);
}

void ObjMesh::Render()	//오브젝트 랜더링 함수
{
	glPushMatrix();

	glColor3fv(m_Color);

	//면 그리기
	int j = 0;
	for (int i = 0; i < m_numFaces*3; i += 3)
	{
		//텍스쳐 번호 배열(m_TextureFace)로 해당 면의 텍스쳐 좌표값 가져옴
		float p1[2] = { m_texCoords[m_TextureFace[i + 0] * 3 + 0], m_texCoords[m_TextureFace[i + 0] * 3 + 1] };
		float p2[2] = { m_texCoords[m_TextureFace[i + 1] * 3 + 0], m_texCoords[m_TextureFace[i + 1] * 3 + 1] };
		float p3[2] = { m_texCoords[m_TextureFace[i + 2] * 3 + 0], m_texCoords[m_TextureFace[i + 2] * 3 + 1] };

		//normal 번호 배열(m_norface)로 해당 면의 normal 좌표값 가져옴, 3으로 나누어 중심의 법선벡터 계산
		float x = (m_normals[m_norface[i + 0] * 3 + 0] + m_normals[m_norface[i + 1] * 3 + 0] + m_normals[m_norface[i + 2] * 3 + 0]) / 3;
		float y = (m_normals[m_norface[i + 0] * 3 + 1] + m_normals[m_norface[i + 1] * 3 + 1] + m_normals[m_norface[i + 2] * 3 + 1]) / 3;
		float z = (m_normals[m_norface[i + 0] * 3 + 2] + m_normals[m_norface[i + 1] * 3 + 2] + m_normals[m_norface[i + 2] * 3 + 2]) / 3;

		//텍스쳐, normal, 세 점으로 구성된 면을 매핑시켜 그림

		glBegin(GL_TRIANGLES);
		glNormal3f(x, y, z);
		glTexCoord2fv(p1);
		glVertex3f(m_vertices[m_Faces[i + 0] * 3 + 0], m_vertices[m_Faces[i + 0] * 3 + 1], m_vertices[m_Faces[i + 0] * 3 + 2]);

		glTexCoord2fv(p2);
		glVertex3f(m_vertices[m_Faces[i + 1] * 3 + 0], m_vertices[m_Faces[i + 1] * 3 + 1], m_vertices[m_Faces[i + 1] * 3 + 2]);

		glTexCoord2fv(p3);
		glVertex3f(m_vertices[m_Faces[i + 2] * 3 + 0], m_vertices[m_Faces[i + 2] * 3 + 1], m_vertices[m_Faces[i + 2] * 3 + 2]);
		glEnd();
	}

	//외곽선 그리기
	if (line) {
		glScalef(1.0001, 1.0001, 1.0001);
		glColor3d(0, 0, 0);
		for (int i = 0; i < m_numFaces * 3; i += 3)
		{
			glBegin(GL_LINES);
			glVertex3f(m_vertices[m_Faces[i + 0] * 3 + 0], m_vertices[m_Faces[i + 0] * 3 + 1], m_vertices[m_Faces[i + 0] * 3 + 2]);
			glVertex3f(m_vertices[m_Faces[i + 1] * 3 + 0], m_vertices[m_Faces[i + 1] * 3 + 1], m_vertices[m_Faces[i + 1] * 3 + 2]);

			glVertex3f(m_vertices[m_Faces[i + 1] * 3 + 0], m_vertices[m_Faces[i + 1] * 3 + 1], m_vertices[m_Faces[i + 1] * 3 + 2]);
			glVertex3f(m_vertices[m_Faces[i + 2] * 3 + 0], m_vertices[m_Faces[i + 2] * 3 + 1], m_vertices[m_Faces[i + 2] * 3 + 2]);

			glVertex3f(m_vertices[m_Faces[i + 2] * 3 + 0], m_vertices[m_Faces[i + 2] * 3 + 1], m_vertices[m_Faces[i + 2] * 3 + 2]);
			glVertex3f(m_vertices[m_Faces[i + 0] * 3 + 0], m_vertices[m_Faces[i + 0] * 3 + 1], m_vertices[m_Faces[i + 0] * 3 + 2]);
			glEnd();
		}
	}

	glPopMatrix();
}

void ObjMesh::SetColor(float r, float g, float b)
{
	m_Color[0] = r;
	m_Color[1] = g;
	m_Color[2] = b;
}

