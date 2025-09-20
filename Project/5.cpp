#include <gl/glew.h>
#include <gl/freeglut.h>
#include <gl/freeglut_ext.h>
#include <iostream>
#include <random>

#define winW 800
#define winH 600
#define rectnum 30

using namespace std;

random_device rd;
default_random_engine dre{ rd() };
uniform_real_distribution ran{ -0.9f,0.9f };
uniform_real_distribution urd{ 0.0f,1.0f };

GLvoid drawScene(GLvoid);
GLvoid Reshape(int w, int h);
GLvoid Mouse(int button, int state, int x, int y);
GLvoid Keyboard(unsigned char key, int x, int y);
GLvoid Motion(int x, int y);

bool dragg = false;
float oriX, oriY;

struct Rect {
	float x, y;
	float color[3];
	bool exist{ true };
};

struct Erase {
	float x, y;
	float size;
	float color[3];
};

vector<Rect> rect;
Erase eraser;

void main(int argc, char** argv)
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA);
	glutInitWindowPosition(100, 100);	//윈도우 위치
	glutInitWindowSize(winW, winH);		//윈도우 크기
	glutCreateWindow("Example1");	//윈도우 이름
	//--- GLEW 초기화하기
	glewExperimental = GL_TRUE;
	if (glewInit() != GLEW_OK)
	{
		std::cerr << "Unable to initialize GLEW" << std::endl;
		exit(EXIT_FAILURE);
	}
	else
		std::cout << "GLEW Initialized\n";

	for (size_t i = 0; i < rectnum; ++i) {
		Rect a;
		a.x = ran(dre);
		a.y = ran(dre);
		a.color[0] = urd(dre);
		a.color[1] = urd(dre);
		a.color[2] = urd(dre);
		a.exist = true;
		rect.push_back(a);
	}

	glutDisplayFunc(drawScene);
	glutReshapeFunc(Reshape);
	glutKeyboardFunc(Keyboard);
	glutMouseFunc(Mouse);
	glutMotionFunc(Motion);
	glutMainLoop();
}
GLvoid drawScene()
{
	glClearColor(0.3f, 0.3f, 0.3f, 1.0f);	//바탕색
	glClear(GL_COLOR_BUFFER_BIT);	//전체 칠하기
	// 그리기 부분 구현: 그리기 관련부분이 여기에 포함된다.

	for (auto& a : rect) {
		glColor3f(a.color[0], a.color[1], a.color[2]);
		glRectf(a.x - 0.05f, a.y - 0.05f, a.x + 0.05f, a.y + 0.05f);

	}

	if (dragg) {
		glColor3f(eraser.color[0], eraser.color[1], eraser.color[2]);
		glRectf(eraser.x - eraser.size, eraser.y - eraser.size, eraser.x + eraser.size, eraser.y + eraser.size);

	}
	glutSwapBuffers();	//화면 출력
}
GLvoid Reshape(int w, int h)
{
	glViewport(0, 0, w, h);
}

GLvoid Keyboard(unsigned char key, int x, int y)
{
	switch (key) {
	case 'r':
	case 'R':	//사각형 초기화
	{
		for (auto& a : rect) {
			a.x = ran(dre);
			a.y = ran(dre);
			a.color[0] = urd(dre);
			a.color[1] = urd(dre);
			a.color[2] = urd(dre);
			a.exist = true;
		}

	}
	}
	glutPostRedisplay();
}
GLvoid Mouse(int button, int state, int x, int y) {

	float ndcX = 2.0f * x / winW - 1.0f;
	float ndcY = -2.0f * y / winH + 1.0f;

	if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN) {
		dragg = true;
		eraser.color[0] = 0.0f;
		eraser.color[1] = 0.0f;
		eraser.color[2] = 0.0f;
		eraser.size = 0.1f;

	}
	else if (button == GLUT_LEFT_BUTTON && state == GLUT_UP) {
		if (dragg) {
			dragg = false;

			glutPostRedisplay();
		}
	}
	else if (button == GLUT_RIGHT_BUTTON && state == GLUT_DOWN) {
		if (rect.size() < rectnum) {
			Rect r;
			r.x = ndcX;
			r.y = ndcY;
			r.color[0] = urd(dre);
			r.color[1] = urd(dre);
			r.color[2] = urd(dre);
			r.exist = true;
			rect.push_back(r);

			eraser.size -= 0.01f;
			if (eraser.size < 0.1f) eraser.size = 0.1f;
			glutPostRedisplay();
		}


	}
}

GLvoid Motion(int x, int y) {
	if (dragg) {
		float ndcX = 2.0f * x / winW - 1.0f;
		float ndcY = -2.0f * y / winH + 1.0f;

		float dx = ndcX - oriX;
		float dy = ndcY - oriY;

		eraser.x += dx;
		eraser.y += dy;

		for (auto& a : rect) {
			if (a.x - 0.05f <= eraser.x + eraser.size && a.x + 0.05f >= eraser.x - eraser.size
				&& a.y - 0.05f <= eraser.y + eraser.size && a.y + 0.05f >= eraser.y - eraser.size && a.exist)	//겹치는 경우
			{
				a.exist = false;
				eraser.size += 0.01f;
				eraser.color[0] = a.color[0];
				eraser.color[1] = a.color[1];
				eraser.color[2] = a.color[2];
				rect.erase(rect.begin() + (&a - &rect[0]));
				break;
			}
		}

		oriX = ndcX;
		oriY = ndcY;

		glutPostRedisplay();
	}
}