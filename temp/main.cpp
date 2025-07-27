/*
		�ۼ��� : 20184009 ���ֿ�
		�ۼ��� : 22.05.23 ~ 22.06.15
		���α׷� �� : Model Viewer
		���α׷� ���� : obj������ ���� �о�� 4���� ��������
					   ����ϰ�, ȸ��, ��, ���� ����, ������ 
					   ����� �̿��Ͽ� ���� �� �� �ִ� ���α׷��̴�.
*/
#include <GL/glut.h>
#include <iostream>
#include <math.h>
#include <vector>		//�ؽ��� ������ �̸��� �����ϱ� ���� �迭�� �������
#include "MeshFactory.h"	//obj������ �ҷ����� �������
#include "ObjMesh.h"		//������Ʈ�� ������ ��� Ŭ����
#include "lodepng.h"		//PNG������ �ؽ��� ������ �д� �Լ�

using namespace std;

void Display();		//display�Լ��� �ݹ� �Լ�
void Reshape(int w, int h);		//reshape�Լ��� �ݹ� �Լ�
void Keyboard(int key, int x, int y);	//keyboard�Լ��� �ݹ� �Լ�
void Grid();	//������ �׸��� �Լ�
void PopupMenu(int index);	//�˾��޴� �Լ��� �ݹ� �Լ�
void InitL();	//������ �ʱ�ȭ�ϴ� �Լ�
void loadTexture(GLuint * texture, const char *path);//PNG���Ͽ��� �ؽ��ĸ� �ҷ����� �Լ�
void spin_Display(int key);	//ī�޶� ������ �Լ�
void MyKeyboard(unsigned char KeyPressed, int X, int Y);	//special�Լ��� �ݹ� �Լ�

float angle = 0.0;

bool light = true;		//���� Ȱ��ȭ ��� ����
bool texture = true;	//�ؽ��� Ȱ��ȭ ��� ����
bool line = true;

GLint menu;			//�˾� �޴� ����
ObjMesh objMesh;	//������Ʈ Ŭ������ ��ü
GLint w, h;			//���� ������ ��Ⱦ�� ��

//camera
float fov = 60;		//���� ������ �þ߰�
GLfloat light_position[] = { 10.0, 10.0, 10.0, 0.0 };	//������ġ


char f[] = "��E.obj";	//obj������ �̸�
std::vector<unsigned char> image2;	//�ؽ����� �����͸� �����ϴ� ����
GLuint textureID[1];	//�ؽ��� ������ �̸�

void Init(){
	CMeshFacotry::LoadObjModel(f, &objMesh);	

	glDepthFunc(GL_LESS); // Depth ���� ����
	glEnable(GL_DEPTH_TEST); // Depth Testing Ȱ��ȭ

	glClearColor(0.0, 0.0, 0.0, 1.0); //������ ��� ����

	InitL();	//���� �ʱ�ȭ
	loadTexture(&textureID[0], "Texture.png");	//�ؽ��� ������ �ҷ���
}

void InitL()	//���� �ʱ�ȭ
{
	GLfloat light_ambient[] = { 1.0, 1.0, 1.0, 1.0 };	// �ֺ���
	GLfloat light_diffuse[] = { 1.0, 1.0, 1.0, 1.0 };	// Ȯ�걤
	GLfloat light_specular[] = { 1.0, 1.0, 1.0, 1.0 };	// ��鱤

	// ���� ����
	glLightfv(GL_LIGHT0, GL_POSITION, light_position);
	glLightfv(GL_LIGHT0, GL_AMBIENT, light_ambient);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, light_diffuse);
	glLightfv(GL_LIGHT0, GL_SPECULAR, light_specular);

	glEnable(GL_LIGHTING);	//������ Ȱ��ȭ ��Ŵ
	glEnable(GL_LIGHT0);	//����� ������ ��ȣ�� ����
}

void loadTexture(GLuint * texture, const char *path) {	//PNG���Ͽ��� �ؽ��ĸ� �ҷ����� �Լ�
	std::vector<unsigned char> image;
	unsigned width, height;
	unsigned error = lodepng::decode(image, width, height, path);	//�ؽ��� ���� decoding
	if (!error)		//������ ���� ��� ���
		std::cout << "error" << error << ": " << lodepng_error_text(error) << std::endl;
	size_t u2 = 1; while (u2 < width) u2 *= 2;		
	size_t v2 = 1; while (v2 < height) v2 *= 2;		//���� ũ�� �ľ�
	image2 = std::vector<unsigned char>(u2*v2 * 4);	//������ ����� 4�� ���� ũ�� �Ҵ�
	for (size_t y = 0; y < height; y++)
		for (size_t x = 0; x < width; x++)
			for (size_t c = 0; c < 4; c++) {	//������ �����͸� ������
				image2[4 * u2 * y + 4 * x + c] = image[4 * width * y + 4 * x + c];
			}
	glGenTextures(1, texture);		//�ؽ��� �̸� ����
	glBindTexture(GL_TEXTURE_2D, *texture);		//���� �ؽ��ĸ� 2D�� ����
	glTexImage2D(GL_TEXTURE_2D, 0, 4, u2, v2, 0, GL_RGBA, GL_UNSIGNED_BYTE, &image2[0]);	//�ؽ��� ��ü�� �̹��� ����
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);		//2D�� �ؽ��ĸ� 3D�� ���� ���ν�Ŵ
}

int main(int argc, char **argv){
	glutInit(&argc, argv);		//GLUT�ʱ�ȭ
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_DEPTH);	//�ΰ� ���� ���
	glutInitWindowSize(810, 810);
	glutCreateWindow("OBJ file Loader");

	Init();
	
	//�ݹ� �Լ� ����
	glutSpecialFunc(Keyboard);		//ESCŰ �Է� �Լ�
	glutKeyboardFunc(MyKeyboard);	//ȭ��ǥ Ű �Է� �Լ�
	glutDisplayFunc(Display);	
	glutReshapeFunc(Reshape);		

	menu = glutCreateMenu(PopupMenu);	//�˾� �޴� ����
	glutAddMenuEntry("Light", 0);		//�� on/off
	glutAddMenuEntry("Texture", 1);		//�ؽ��� on/off
	glutAddMenuEntry("material1", 2);	//Meterial ����
	glutAddMenuEntry("material2", 3);
	glutAddMenuEntry("material3", 4);
	glutAddMenuEntry("Smooth", 5);		//Shading ����
	glutAddMenuEntry("Flat", 6);
	glutAddMenuEntry("Line_draw", 7);	//�� on/off
	glutAttachMenu(GLUT_RIGHT_BUTTON);

	glutMainLoop();

	return 0;
}

int Mat_N = 0;
int shadmodel = 0;

GLfloat mat1[4] = { 0.0215, 0.1745, 0.0215, 1.0 };	//Material��
GLfloat mat2[4] = { 0.135, 0.2225, 0.1575, 1.0 };
GLfloat mat3[4] = { 0.05375, 0.05, 0.06625, 1.0 };

void Display(void)		//display�Լ��� �ݹ� �Լ�
{
	
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); //color, depth ���� �ʱ�ȭ

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	if (texture) glEnable(GL_TEXTURE_2D);	//�ؽ��� Ȱ��ȭ/��Ȱ��ȭ
	else glDisable(GL_TEXTURE_2D);

	if (light) glEnable(GL_LIGHTING);		//���� Ȱ��ȭ/��Ȱ��ȭ
	else glDisable(GL_LIGHTING);

	switch (shadmodel) {	//Shading ����
	case 0:
		glShadeModel(GL_SMOOTH);
		break;
	case 1:
		glShadeModel(GL_FLAT);
		break;
	}

	switch (Mat_N) {	//Material ����
	case 1:
		glMaterialfv(GL_FRONT, GL_AMBIENT, mat1);
		break;
	case 2:
		glMaterialfv(GL_FRONT, GL_AMBIENT, mat2);
		break;
	case 3:
		glMaterialfv(GL_FRONT, GL_AMBIENT, mat3);
		break;
	}

	int i = 1;
	while (i < 5) {
		switch (i) {
		case 1:	//1��и�
			glPushMatrix();

			glMatrixMode(GL_PROJECTION);

			glLoadIdentity();
			gluPerspective(fov, (GLfloat)w / (GLfloat)h, 0.1, 100000.0);

			gluLookAt(35, 15, 0, 0, 6, 0, 0.0, 1.0, 0.0);	//ī�޶� ��ġ ����
			glViewport(410, 410, 400, 400);		//������ �� ����Ʈ ����

			glPushMatrix();
			glRotatef(angle, 0, 1, 0);	//ī�޶� ȸ��
			glTranslatef(0, 0, 143);
			objMesh.Render();	//������Ʈ ���

			glPopMatrix();

			glPopMatrix();

			break;

		case 2:	//2��и�
			glPushMatrix();

			glMatrixMode(GL_PROJECTION);

			glLoadIdentity();
			gluPerspective(fov, (GLfloat)w / (GLfloat)h, 0.1, 100000.0);

			gluLookAt(0, 120, 5, 0, 6, 0, 0.0, 1.0, 0.0);
			glViewport(0, 410, 400, 400);	//���� �� ����Ʈ ����

			glPushMatrix();

			glRotatef(angle, 0, 1, 0);

			Grid();		//���� ����

			glScalef(2.7f, 2.7f, 2.7f);		//ũ�� ����
			glTranslatef(0, 0, 143);
			objMesh.Render();

			glPopMatrix();

			glPopMatrix();

			break;

		case 3:	//3��и�
			glPushMatrix();

			glMatrixMode(GL_PROJECTION);

			glLoadIdentity();
			gluPerspective(fov, (GLfloat)w / (GLfloat)h, 0.1, 100000.0);

			glViewport(0, 0, 400, 400);		//���� �Ʒ� ����Ʈ ����
			gluLookAt(0.0f, 25.75f, 80.0f, 0, 25.75f, 0, 0.0, 1.0, 0.0);

			glPushMatrix();

			glScalef(2.7f, 2.7f, 2.7f);
			glRotatef(angle, 0, 1, 0);
			glTranslatef(0, 0, 143);
			objMesh.Render();

			glPopMatrix();

			glPopMatrix();

			break;

		case 4:	//4��и�
			glPushMatrix();

			glMatrixMode(GL_PROJECTION);

			glLoadIdentity();
			gluPerspective(fov, (GLfloat)w / (GLfloat)h, 0.1, 100000.0);

			gluLookAt(-70, 50, 80, 0, 6, 0, 0.0, 1.0, 0.0);
			glViewport(410, 0, 400, 400);		//������ �Ʒ� ����Ʈ ����

			glPushMatrix();

			glRotatef(angle, 0, 1, 0);
			Grid();

			glScalef(2.7f, 2.7f, 2.7f);
			glTranslatef(0, 0, 143);
			objMesh.Render();

			glPopMatrix();

			glPopMatrix();

			break;
		}
		i++;
	}
	glutSwapBuffers(); //���� swap
}

void Reshape(int width, int height)	//reshape�Լ��� �ݹ� �Լ�
{
	w = width;		//��Ⱦ�� ���� �缳��
	h = height;
}

void Keyboard(int key, int a, int s)	//special�Լ��� �ݹ� �Լ�
{
	switch (key) {	//ȭ��ǥ Ű �Է� ó��
	case GLUT_KEY_LEFT:		//ī�޶� �·� ȸ��
		spin_Display(key);
		break;

	case GLUT_KEY_RIGHT:	//ī�޶� ��� ȸ��
		spin_Display(key);
		break;
					
	case GLUT_KEY_DOWN:		//�� �ƿ�
		fov += 0.5f;
		glutPostRedisplay();
		break;
		
	case GLUT_KEY_UP:		//�� ��
		fov -= 0.5f;
		glutPostRedisplay();
		break;				
	}
}

void MyKeyboard(unsigned char KeyPressed, int X, int Y) {
	switch (KeyPressed) {	//keyboard�ݹ� �Լ�
	case 27:		//esc �Է½� ����
		exit(0);
		break;
	}
}

void Grid()		//������ �׸��� �Լ�
{
	glPushMatrix();
	glColor3f(1, 1, 1);
	for (int i = -50; i < 50; i++){
		glBegin(GL_LINES);
		glNormal3f(0, 1, 0);
		glVertex3f(i, 0, -50);
		glVertex3f(i, 0, 50);
		glEnd();
	}

	for (int i = -50; i < 50; i++){
		glBegin(GL_LINES);
		glNormal3f(0, 1, 0);
		glVertex3f(-50, 0, i);
		glVertex3f(50, 0, i);
		glEnd();
	}

	glPopMatrix();
}

void PopupMenu(int index)	//�˾� ó�� �Լ�
{
	switch (index) {
	case 0:		//���� on/off
		light = !light;
		break;
	case 1:		//�ؽ��� on/off
		texture = !texture;
		break;
	case 2:		//meterial ��ȣ ����
		Mat_N = 1;
		break;
	case 3:		
		Mat_N = 2;
		break;
	case 4:
		Mat_N = 3;
		break;
	case 5:		//Smooth shading
		shadmodel = 0;
		break;
	case 6:		//Flat shading
		shadmodel = 1;
		break;
	case 7:		//������Ʈ ���� on/off
		objMesh.line = !objMesh.line;
		break;
	}
	glutPostRedisplay();
}

void spin_Display(int key) {		//���� ��ȯ �Լ�
	if(key == GLUT_KEY_RIGHT)
		angle -= 10;		//0.05�� ���� ��ȯ
	else if(key == GLUT_KEY_LEFT)
		angle += 10;		//0.05�� ���� ��ȯ
	if (angle > 360)			//�ѹ����� ���Ҵٸ� 
		angle = angle - 360;	//�ٽ� 0������ ȸ��
	else if(angle < 0)
		angle = angle + 360;
	glutPostRedisplay();
}