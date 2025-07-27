#pragma once
#include <GL/glut.h>
#include "MeshFactory.h"
#include <windows.h>
#include "ObjMesh.h"

void CMeshFacotry::LoadObjModel(char* fileDir, ObjMesh* mesh)
{
	FILE* fp = fopen(fileDir, "r");		//obj���� read
	if (fp == NULL) {
		printf("%s file can not open", fileDir);
		exit(1);
	}
	int numVertex = 0;		//���� ���� ����
	int numFaces = 0;		//�� ����
	int numNormals = 0;		//���� normal ��ǥ ����
	int numTexcoords = 0;	//�ؽ��� ��ǥ ����

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
			if (line[1] == 't') {		//texture ���� �ľ�
				numTexcoords++;
			}
			else if (line[1] == 'n') {	//normal ���� �ľ�
				numNormals++;
			}
			else if (line[1] == ' ')	//���� ���� �ľ�
			numVertex++;
		}
		else if (line[0] == 'f') {		//�� ���� �ľ�
			numFaces++;
		}
	}
	rewind(fp);

	printf("numVertex : %d\n", numVertex);
	printf("numFaces : %d\n", numFaces);
	printf("numTexcoords : %d\n", numTexcoords);
	printf("numNormals : %d\n", numNormals);

	vertices = (float*)malloc(sizeof(float) * 3 * numVertex);		//���� ��ǥ �迭
	texCoords = (float*)malloc(sizeof(float) * 3 * numTexcoords);	//�ؽ��� ��ǥ �迭
	normal = (float*)malloc(sizeof(float) * 3 * numNormals);		//normal ��ǥ �迭
	faces = (int*)malloc(sizeof(int) * 3 * numFaces);				//���� ���� ��ǥ ��ȣ �迭
	textureFace = (int*)malloc(sizeof(int) * 3 * numFaces);			//���� �ؽ��� ��ǥ ��ȣ �迭
	norface = (int*)malloc(sizeof(int) * 3 * numFaces);				//���� normal ��ǥ ��ȣ �迭

	int j = 0, k = 0, l = 0;
	int IdxFace = 0;
	int IdxTexCoord = 0;
	int Idxnormal = 0;
	float x, y, z;
	char temp = NULL;

	while (!feof(fp)) {
		fgets(line, 256, fp);
		if (line[0] == 'v') {		
			if (line[1] == 't') {		//texture ��ǥ read
				fseek(fp, -(strlen(line) + 1), SEEK_CUR);	//���� ������ ������ �̵�

				fscanf(fp, "%s %f %f %f", line, &x, &y, &z);
				texCoords[j++] = x;	texCoords[j++] = y;	texCoords[j++] = z;	//��ǥ�� ����
			}
			else if (line[1] == 'n') {	//normal ��ǥ read
				fseek(fp, -(strlen(line) + 1), SEEK_CUR);	//���� ������ ������ �̵�

				fscanf(fp, "%s %f %f %f", line, &x, &y, &z);
				normal[k++] = x;	normal[k++] = y;	normal[k++] = z;	//��ǥ�� ����
			}
			else if (line[1] == ' '){	//vertex ��ǥ read
				fseek(fp, -(strlen(line) + 1), SEEK_CUR);	//���� ������ ������ �̵�

				fscanf(fp, "%s %f %f %f", line, &x, &y, &z);
				vertices[l++] = x;	vertices[l++] = y;	vertices[l++] = z;	//��ǥ�� ����
			}
		}
		else if (line[0] == 'f') {		//�� ���� read
			fseek(fp, -(strlen(line) + 1), SEEK_CUR);		//���� ������

			for (int a = 0; a < 3; a++) {	//�ﰢ���� ���� 3�� ��ǥ read
				fscanf(fp, "%c %f%c%f%c%f", &temp, &x, &temp, &y, &temp, &z);
				faces[IdxFace++] = x-1; textureFace[IdxTexCoord++] = y-1;  norface[Idxnormal++] = z-1;
			}
		}
	}

	//��ü�� �� ����
	//���� ����, ��ǥ
	mesh->m_numVertices = numVertex;
	mesh->m_vertices = vertices;

	//�ؽ��� ����, ��ǥ
	mesh->m_numTexCoords = numTexcoords;
	mesh->m_texCoords = texCoords;

	//�븻 ����, ��ǥ
	mesh->m_numNormals = numNormals;
	mesh->m_normals = normal;

	//���� ����(�� ���� ��ȣ, �ؽ��� ��ȣ, normal ��ȣ)
	mesh->m_numFaces = numFaces;
	mesh->m_Faces = faces;
	mesh->m_TextureFace = textureFace;
	mesh->m_norface = norface;	
}
