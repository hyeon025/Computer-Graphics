#include <gl/glew.h>
#include <gl/freeglut.h>
#include <gl/freeglut_ext.h>
#include <iostream>
#include <random>

#define winW 800
#define winH 600

using namespace std;

random_device rd;
default_random_engine dre{ rd() };
uniform_real_distribution ran{ 0.0,1.0 };

GLvoid drawScene(GLvoid);
GLvoid Reshape(int w, int h);
GLvoid Mouse(int button, int state, int x, int y);

float c_b = 1.0f, c_r = 1.0f, c_g = 0.5f;
float color[4][3] = { { 1,0,0 }, { 0,1,0 }, { 0,0,1 }, { 1,1,0 } };

struct Rect {
	float left, right, bottom, top;
};

Rect rect[4] = {
	{-1.0f, 0.0f, -1.0f, 0.0f},
	{0.0f, 1.0f, -1.0f, 0.0f},
	{-1.0f, 0.0f, 0.0f, 1.0f},
	{0.0f, 1.0f, 0.0f, 1.0f}
};

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
	glutDisplayFunc(drawScene);
	glutReshapeFunc(Reshape);
	glutMouseFunc(Mouse);
	glutMainLoop();
}
GLvoid drawScene()
{
	glClearColor(c_r, c_g, c_b, 1.0f);	//바탕색
	glClear(GL_COLOR_BUFFER_BIT);	//전체 칠하기
	// 그리기 부분 구현: 그리기 관련부분이 여기에 포함된다.


	for (size_t i = 0; i < 4; i++) {
		glColor3fv(color[i]);
		glRectf(rect[i].left, rect[i].bottom, rect[i].right, rect[i].top);
	}

	glutSwapBuffers();	//화면 출력
}
GLvoid Reshape(int w, int h)
{
	glViewport(0, 0, w, h);
}
GLvoid Mouse(int button, int state, int x, int y) {

	float ndcX = 2.0f * x / winW - 1.0f;
	float ndcY = -2.0f * y / winH + 1.0f;

	if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN) { //좌클릭
		bool inrectL = false;
		for (size_t i = 0; i < 4; i++)
		{
			if (rect[i].left < ndcX && rect[i].right > ndcX && rect[i].bottom < ndcY && rect[i].top > ndcY) {
				for (size_t j = 0; j < 3; j++) {
					color[i][j] = ran(dre);
				}
				inrectL = true;

				break;
			}
		}

		if (!inrectL) {
			c_r = ran(dre);
			c_g = ran(dre);
			c_b = ran(dre);
		}
	}

	if (button == GLUT_RIGHT_BUTTON && state == GLUT_DOWN) { //우클릭
		bool inrectR = false;

		for (size_t i = 0; i < 4; i++)
		{
			if (rect[i].left < ndcX && rect[i].right > ndcX && rect[i].bottom < ndcY && rect[i].top > ndcY) {
				rect[i].left += 0.1f;
				rect[i].right -= 0.1f;
				rect[i].bottom += 0.1f;
				rect[i].top -= 0.1f;
				inrectR = true;

				break;
			}
		}
		if (!inrectR) {
			if (ndcX <= 0.0f && ndcY <= 0.0f) {
				rect[0].left -= 0.1f;
				rect[0].right += 0.1f;
				rect[0].bottom -= 0.1f;
				rect[0].top += 0.1f;
			}
			else if (ndcX >= 0.0f && ndcY <= 0.0f) {
				rect[1].left -= 0.1f;
				rect[1].right += 0.1f;
				rect[1].bottom -= 0.1f;
				rect[1].top += 0.1f;
			}
			else if (ndcX <= 0.0f && ndcY >= 0.0f) {
				rect[2].left -= 0.1f;
				rect[2].right += 0.1f;
				rect[2].bottom -= 0.1f;
				rect[2].top += 0.1f;
			}
			else if (ndcX >= 0.0f && ndcY >= 0.0f) {
				rect[3].left -= 0.1f;
				rect[3].right += 0.1f;
				rect[3].bottom -= 0.1f;
				rect[3].top += 0.1f;
			}
		}
	}
}