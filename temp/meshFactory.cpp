#pragma once
#include <GL/glut.h>
#include "MeshFactory.h"
#include <windows.h>
#include "ObjMesh.h"

void CMeshFacotry::LoadObjModel(char* fileDir, ObjMesh* mesh)
{
	FILE* fp = fopen(fileDir, "r");		//obj파일 read
	if (fp == NULL) {
		printf("%s file can not open", fileDir);
		exit(1);
	}
	int numVertex = 0;		//정점 개수 변수
	int numFaces = 0;		//면 개수
	int numNormals = 0;		//정점 normal 좌표 개수
	int numTexcoords = 0;	//텍스쳐 좌표 개수

	float* vertices = NULL;
	float* normal = NULL;
	int* faces = NULL;
	int* textureFace = NULL;
	float* texCoords = NULL;
	int* norface = NULL;

	char line[256];

	while (!feof(fp)) {
		fgets(line, 256, fp);
		if (line[0] == 'v') {
			if (line[1] == 't') {		//texture 개수 파악
				numTexcoords++;
			}
			else if (line[1] == 'n') {	//normal 개수 파악
				numNormals++;
			}
			else if (line[1] == ' ')	//정점 개수 파악
			numVertex++;
		}
		else if (line[0] == 'f') {		//면 개수 파악
			numFaces++;
		}
	}
	rewind(fp);

	printf("numVertex : %d\n", numVertex);
	printf("numFaces : %d\n", numFaces);
	printf("numTexcoords : %d\n", numTexcoords);
	printf("numNormals : %d\n", numNormals);

	vertices = (float*)malloc(sizeof(float) * 3 * numVertex);		//정점 좌표 배열
	texCoords = (float*)malloc(sizeof(float) * 3 * numTexcoords);	//텍스쳐 좌표 배열
	normal = (float*)malloc(sizeof(float) * 3 * numNormals);		//normal 좌표 배열
	faces = (int*)malloc(sizeof(int) * 3 * numFaces);				//면의 정점 좌표 번호 배열
	textureFace = (int*)malloc(sizeof(int) * 3 * numFaces);			//면의 텍스쳐 좌표 번호 배열
	norface = (int*)malloc(sizeof(int) * 3 * numFaces);				//면의 normal 좌표 번호 배열

	int j = 0, k = 0, l = 0;
	int IdxFace = 0;
	int IdxTexCoord = 0;
	int Idxnormal = 0;
	float x, y, z;
	char temp = NULL;

	while (!feof(fp)) {
		fgets(line, 256, fp);
		if (line[0] == 'v') {		
			if (line[1] == 't') {		//texture 좌표 read
				fseek(fp, -(strlen(line) + 1), SEEK_CUR);	//읽은 문장의 앞으로 이동

				fscanf(fp, "%s %f %f %f", line, &x, &y, &z);
				texCoords[j++] = x;	texCoords[j++] = y;	texCoords[j++] = z;	//좌표값 저장
			}
			else if (line[1] == 'n') {	//normal 좌표 read
				fseek(fp, -(strlen(line) + 1), SEEK_CUR);	//읽은 문장의 앞으로 이동

				fscanf(fp, "%s %f %f %f", line, &x, &y, &z);
				normal[k++] = x;	normal[k++] = y;	normal[k++] = z;	//좌표값 저장
			}
			else if (line[1] == ' '){	//vertex 좌표 read
				fseek(fp, -(strlen(line) + 1), SEEK_CUR);	//읽은 문장의 앞으로 이동

				fscanf(fp, "%s %f %f %f", line, &x, &y, &z);
				vertices[l++] = x;	vertices[l++] = y;	vertices[l++] = z;	//좌표값 저장
			}
		}
		else if (line[0] == 'f') {		//면 정보 read
			fseek(fp, -(strlen(line) + 1), SEEK_CUR);		//한줄 앞으로

			for (int a = 0; a < 3; a++) {	//삼각형의 정점 3개 좌표 read
				fscanf(fp, "%c %f%c%f%c%f", &temp, &x, &temp, &y, &temp, &z);
				faces[IdxFace++] = x-1; textureFace[IdxTexCoord++] = y-1;  norface[Idxnormal++] = z-1;
			}
		}
	}

	//객체에 값 전달
	//정점 개수, 좌표
	mesh->m_numVertices = numVertex;
	mesh->m_vertices = vertices;

	//텍스쳐 개수, 좌표
	mesh->m_numTexCoords = numTexcoords;
	mesh->m_texCoords = texCoords;

	//노말 개수, 좌표
	mesh->m_numNormals = numNormals;
	mesh->m_normals = normal;

	//면의 정보(세 정점 번호, 텍스쳐 번호, normal 번호)
	mesh->m_numFaces = numFaces;
	mesh->m_Faces = faces;
	mesh->m_TextureFace = textureFace;
	mesh->m_norface = norface;	
}
