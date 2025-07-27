/*
		작성자 : 20184009 김주원
		작성일 : 22.05.23 ~ 22.06.15
		프로그램 명 : Model Viewer
		프로그램 설명 : obj파일의 모델을 읽어와 4개의 시점으로
					   출력하고, 회전, 줌, 재질 변경, 조명의 
					   기능을 이용하여 모델을 볼 수 있는 프로그램이다.
*/
#include <GL/glut.h>
#include <iostream>
#include <math.h>
#include <vector>		//텍스쳐 파일의 이름을 저장하기 위한 배열의 헤더파일
#include "MeshFactory.h"	//obj파일을 불러오는 헤더파일
#include "ObjMesh.h"		//오브젝트의 정보를 담는 클래스
#include "lodepng.h"		//PNG파일인 텍스쳐 파일을 읽는 함수

using namespace std;

void Display();		//display함수의 콜백 함수
void Reshape(int w, int h);		//reshape함수의 콜백 함수
void Keyboard(int key, int x, int y);	//keyboard함수의 콜백 함수
void Grid();	//발판을 그리는 함수
void PopupMenu(int index);	//팝업메뉴 함수의 콜백 함수
void InitL();	//조명을 초기화하는 함수
void loadTexture(GLuint * texture, const char *path);//PNG파일에서 텍스쳐를 불러오는 함수
void spin_Display(int key);	//카메라를 돌리는 함수
void MyKeyboard(unsigned char KeyPressed, int X, int Y);	//special함수의 콜백 함수

float angle = 0.0;

bool light = true;		//조명 활성화 토글 변수
bool texture = true;	//텍스쳐 활성화 토글 변수
bool line = true;

GLint menu;			//팝업 메뉴 변수
ObjMesh objMesh;	//오브젝트 클래스의 객체
GLint w, h;			//원근 투영의 종횡비 값

//camera
float fov = 60;		//원근 투영의 시야각
GLfloat light_position[] = { 10.0, 10.0, 10.0, 0.0 };	//조명위치


char f[] = "월E.obj";	//obj파일의 이름
std::vector<unsigned char> image2;	//텍스쳐의 데이터를 저장하는 벡터
GLuint textureID[1];	//텍스쳐 파일의 이름

void Init(){
	CMeshFacotry::LoadObjModel(f, &objMesh);	

	glDepthFunc(GL_LESS); // Depth 버퍼 설정
	glEnable(GL_DEPTH_TEST); // Depth Testing 활성화

	glClearColor(0.0, 0.0, 0.0, 1.0); //윈도우 배경 설정

	InitL();	//조명 초기화
	loadTexture(&textureID[0], "Texture.png");	//텍스쳐 파일을 불러옴
}

void InitL()	//조명 초기화
{
	GLfloat light_ambient[] = { 1.0, 1.0, 1.0, 1.0 };	// 주변광
	GLfloat light_diffuse[] = { 1.0, 1.0, 1.0, 1.0 };	// 확산광
	GLfloat light_specular[] = { 1.0, 1.0, 1.0, 1.0 };	// 경면광

	// 조명 설정
	glLightfv(GL_LIGHT0, GL_POSITION, light_position);
	glLightfv(GL_LIGHT0, GL_AMBIENT, light_ambient);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, light_diffuse);
	glLightfv(GL_LIGHT0, GL_SPECULAR, light_specular);

	glEnable(GL_LIGHTING);	//조명을 활성화 시킴
	glEnable(GL_LIGHT0);	//사용할 조명의 번호로 설정
}

void loadTexture(GLuint * texture, const char *path) {	//PNG파일에서 텍스쳐를 불러오는 함수
	std::vector<unsigned char> image;
	unsigned width, height;
	unsigned error = lodepng::decode(image, width, height, path);	//텍스쳐 파일 decoding
	if (!error)		//문제가 있을 경우 경고
		std::cout << "error" << error << ": " << lodepng_error_text(error) << std::endl;
	size_t u2 = 1; while (u2 < width) u2 *= 2;		
	size_t v2 = 1; while (v2 < height) v2 *= 2;		//파일 크기 파악
	image2 = std::vector<unsigned char>(u2*v2 * 4);	//파일의 사이즈에 4를 곱해 크기 할당
	for (size_t y = 0; y < height; y++)
		for (size_t x = 0; x < width; x++)
			for (size_t c = 0; c < 4; c++) {	//파일의 데이터를 복사함
				image2[4 * u2 * y + 4 * x + c] = image[4 * width * y + 4 * x + c];
			}
	glGenTextures(1, texture);		//텍스쳐 이름 생성
	glBindTexture(GL_TEXTURE_2D, *texture);		//명명된 텍스쳐를 2D로 생성
	glTexImage2D(GL_TEXTURE_2D, 0, 4, u2, v2, 0, GL_RGBA, GL_UNSIGNED_BYTE, &image2[0]);	//텍스쳐 객체에 이미지 설정
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);		//2D인 텍스쳐를 3D에 맞춰 매핑시킴
}

int main(int argc, char **argv){
	glutInit(&argc, argv);		//GLUT초기화
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_DEPTH);	//두개 버퍼 사용
	glutInitWindowSize(810, 810);
	glutCreateWindow("OBJ file Loader");

	Init();
	
	//콜백 함수 적용
	glutSpecialFunc(Keyboard);		//ESC키 입력 함수
	glutKeyboardFunc(MyKeyboard);	//화살표 키 입력 함수
	glutDisplayFunc(Display);	
	glutReshapeFunc(Reshape);		

	menu = glutCreateMenu(PopupMenu);	//팝업 메뉴 설정
	glutAddMenuEntry("Light", 0);		//빛 on/off
	glutAddMenuEntry("Texture", 1);		//텍스쳐 on/off
	glutAddMenuEntry("material1", 2);	//Meterial 설정
	glutAddMenuEntry("material2", 3);
	glutAddMenuEntry("material3", 4);
	glutAddMenuEntry("Smooth", 5);		//Shading 설정
	glutAddMenuEntry("Flat", 6);
	glutAddMenuEntry("Line_draw", 7);	//선 on/off
	glutAttachMenu(GLUT_RIGHT_BUTTON);

	glutMainLoop();

	return 0;
}

int Mat_N = 0;
int shadmodel = 0;

GLfloat mat1[4] = { 0.0215, 0.1745, 0.0215, 1.0 };	//Material값
GLfloat mat2[4] = { 0.135, 0.2225, 0.1575, 1.0 };
GLfloat mat3[4] = { 0.05375, 0.05, 0.06625, 1.0 };

void Display(void)		//display함수의 콜백 함수
{
	
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); //color, depth 버퍼 초기화

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	if (texture) glEnable(GL_TEXTURE_2D);	//텍스쳐 활성화/비활성화
	else glDisable(GL_TEXTURE_2D);

	if (light) glEnable(GL_LIGHTING);		//조명 활성화/비활성화
	else glDisable(GL_LIGHTING);

	switch (shadmodel) {	//Shading 설정
	case 0:
		glShadeModel(GL_SMOOTH);
		break;
	case 1:
		glShadeModel(GL_FLAT);
		break;
	}

	switch (Mat_N) {	//Material 설정
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
		case 1:	//1사분면
			glPushMatrix();

			glMatrixMode(GL_PROJECTION);

			glLoadIdentity();
			gluPerspective(fov, (GLfloat)w / (GLfloat)h, 0.1, 100000.0);

			gluLookAt(35, 15, 0, 0, 6, 0, 0.0, 1.0, 0.0);	//카메라 위치 설정
			glViewport(410, 410, 400, 400);		//오른쪽 위 뷰포트 생성

			glPushMatrix();
			glRotatef(angle, 0, 1, 0);	//카메라 회전
			glTranslatef(0, 0, 143);
			objMesh.Render();	//오브젝트 출력

			glPopMatrix();

			glPopMatrix();

			break;

		case 2:	//2사분면
			glPushMatrix();

			glMatrixMode(GL_PROJECTION);

			glLoadIdentity();
			gluPerspective(fov, (GLfloat)w / (GLfloat)h, 0.1, 100000.0);

			gluLookAt(0, 120, 5, 0, 6, 0, 0.0, 1.0, 0.0);
			glViewport(0, 410, 400, 400);	//왼쪽 위 뷰포트 생성

			glPushMatrix();

			glRotatef(angle, 0, 1, 0);

			Grid();		//발판 생성

			glScalef(2.7f, 2.7f, 2.7f);		//크기 설정
			glTranslatef(0, 0, 143);
			objMesh.Render();

			glPopMatrix();

			glPopMatrix();

			break;

		case 3:	//3사분면
			glPushMatrix();

			glMatrixMode(GL_PROJECTION);

			glLoadIdentity();
			gluPerspective(fov, (GLfloat)w / (GLfloat)h, 0.1, 100000.0);

			glViewport(0, 0, 400, 400);		//왼쪽 아래 뷰포트 생성
			gluLookAt(0.0f, 25.75f, 80.0f, 0, 25.75f, 0, 0.0, 1.0, 0.0);

			glPushMatrix();

			glScalef(2.7f, 2.7f, 2.7f);
			glRotatef(angle, 0, 1, 0);
			glTranslatef(0, 0, 143);
			objMesh.Render();

			glPopMatrix();

			glPopMatrix();

			break;

		case 4:	//4사분면
			glPushMatrix();

			glMatrixMode(GL_PROJECTION);

			glLoadIdentity();
			gluPerspective(fov, (GLfloat)w / (GLfloat)h, 0.1, 100000.0);

			gluLookAt(-70, 50, 80, 0, 6, 0, 0.0, 1.0, 0.0);
			glViewport(410, 0, 400, 400);		//오른쪽 아래 뷰포트 생성

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
	glutSwapBuffers(); //버퍼 swap
}

void Reshape(int width, int height)	//reshape함수의 콜백 함수
{
	w = width;		//종횡비 값을 재설정
	h = height;
}

void Keyboard(int key, int a, int s)	//special함수의 콜백 함수
{
	switch (key) {	//화살표 키 입력 처리
	case GLUT_KEY_LEFT:		//카메라 좌로 회전
		spin_Display(key);
		break;

	case GLUT_KEY_RIGHT:	//카메라 우로 회전
		spin_Display(key);
		break;
					
	case GLUT_KEY_DOWN:		//줌 아웃
		fov += 0.5f;
		glutPostRedisplay();
		break;
		
	case GLUT_KEY_UP:		//줌 인
		fov -= 0.5f;
		glutPostRedisplay();
		break;				
	}
}

void MyKeyboard(unsigned char KeyPressed, int X, int Y) {
	switch (KeyPressed) {	//keyboard콜백 함수
	case 27:		//esc 입력시 종료
		exit(0);
		break;
	}
}

void Grid()		//발판을 그리는 함수
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

void PopupMenu(int index)	//팝업 처리 함수
{
	switch (index) {
	case 0:		//조명 on/off
		light = !light;
		break;
	case 1:		//텍스쳐 on/off
		texture = !texture;
		break;
	case 2:		//meterial 번호 설정
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
	case 7:		//오브젝트 라인 on/off
		objMesh.line = !objMesh.line;
		break;
	}
	glutPostRedisplay();
}

void spin_Display(int key) {		//각도 변환 함수
	if(key == GLUT_KEY_RIGHT)
		angle -= 10;		//0.05씩 각도 변환
	else if(key == GLUT_KEY_LEFT)
		angle += 10;		//0.05씩 각도 변환
	if (angle > 360)			//한바퀴를 돌았다면 
		angle = angle - 360;	//다시 0도부터 회전
	else if(angle < 0)
		angle = angle + 360;
	glutPostRedisplay();
}