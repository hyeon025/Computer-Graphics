#include <gl/glew.h>
#include <gl/freeglut.h>
#include <gl/freeglut_ext.h>
#include <iostream>
#include <random>
#include <map>

#define winW 800
#define winH 600
#define rectnum 10

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
size_t dragindex = -1;
float oriX, oriY;

struct Rect {
	float l, b, r, t;
	float color[3];
};

vector<Rect> rect;

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

	for (const auto& a : rect) {
		glColor3f(a.color[0], a.color[1], a.color[2]);
		glRectf(a.l, a.b, a.r, a.t);
	}
	glutSwapBuffers();	//화면 출력 (더블 버퍼링)
}
GLvoid Reshape(int w, int h)
{
	glViewport(0, 0, w, h);
}

GLvoid Keyboard(unsigned char key, int x, int y)
{
	switch (key) {
	case 'a':
	case 'A':
	{
		if (rect.size() < rectnum) {
			Rect r;
			float x = ran(dre);	//사각형 중점 (기본크기 0.2f)
			float y = ran(dre);
			r.l = x - 0.1f;
			r.t = y + 0.1f;
			r.r = x + 0.1f;
			r.b = y - 0.1f;

			r.color[0] = urd(dre);
			r.color[1] = urd(dre);
			r.color[2] = urd(dre);

			rect.push_back(r);

			break;
		}
	}
	}
	glutPostRedisplay();
}
GLvoid Mouse(int button, int state, int x, int y) {

	float ndcX = 2.0f * x / winW - 1.0f;
	float ndcY = -2.0f * y / winH + 1.0f;

	if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN) {

		for (int i = rect.size() - 1; i >= 0; --i)
		{
			auto& a = rect[i];
			if (ndcX > a.l && ndcX < a.r && ndcY > a.b && ndcY < a.t) {
				dragg = true;
				dragindex = i;

				oriX = ndcX;
				oriY = ndcY;

				break;
			}
		}
	}
	else if (button == GLUT_LEFT_BUTTON && state == GLUT_UP) {
		if (dragg) {
			dragg = false;
			for (size_t i = 0; i < rect.size(); ++i)
			{
				if (i == dragindex)
					continue;
				auto& a = rect[dragindex];
				auto& b = rect[i];

				if (a.l <= b.r && a.r >= b.l && a.b <= b.t && a.t >= b.b) {
					Rect newrect;

					newrect.l = min(a.l, b.l);
					newrect.r = max(a.r, b.r);
					newrect.b = min(a.b, b.b);
					newrect.t = max(a.t, b.t);

					newrect.color[0] = urd(dre);
					newrect.color[1] = urd(dre);
					newrect.color[2] = urd(dre);

					rect[dragindex] = newrect;
					rect.erase(rect.begin() + i);

					break;
				}
			}
			dragindex = -1;
			glutPostRedisplay();
		}
	}
	else if(button == GLUT_RIGHT_BUTTON && state == GLUT_DOWN) {
		float maxl{};
		float maxr{};
		float maxt{};
		float maxb{};
		for (int i = rect.size() - 1; i >= 0; --i) //네모 없애기
		{
			auto& a = rect[i];
			if (ndcX > a.l && ndcX < a.r && ndcY > a.b && ndcY < a.t && rect.size() < 30) {
				maxl = a.l;
				maxr = a.r;
				maxb = a.b;
				maxt = a.t;
				rect.erase(rect.begin() + i);

				//새로운 네모 2개 만들기
				for (size_t i = 0; i < 2; i++) {
					Rect newrect;
					newrect.color[0] = ran(dre);
					newrect.color[1] = ran(dre);
					newrect.color[2] = ran(dre);

					newrect.l = maxl + (maxr - maxl) * urd(dre) * 0.8f;
					newrect.r = newrect.l + (maxr - maxl) * urd(dre) * 0.8f;
					newrect.b = maxb + (maxt - maxb) * urd(dre) * 0.8f;
					newrect.t = newrect.b + (maxt - maxb) * urd(dre) * 0.8f;

					if (newrect.l < -1.0f) newrect.l = -1.0f;
					if (newrect.b < -1.0f) newrect.b = -1.0f;
					if (newrect.r > 1.0f) newrect.r = 1.0f;
					if (newrect.t > 1.0f) newrect.t = 1.0f;

					if(newrect.r - newrect.l < 0.05f)
						newrect.r = newrect.l + 0.05f;
					if (newrect.t - newrect.b < 0.05f)
						newrect.t = newrect.b + 0.05f;

					rect.push_back(newrect);
				}

				glutPostRedisplay();
				break;
			}
		}

		
	}
}

GLvoid Motion(int x, int y) {
	if (dragg) {
		float ndcX = 2.0f * x / winW - 1.0f;
		float ndcY = -2.0f * y / winH + 1.0f;

		float dx = ndcX - oriX;
		float dy = ndcY - oriY;

		auto& a = rect[dragindex];

		a.l += dx;
		a.t += dy;
		a.r += dx;
		a.b += dy;

		oriX = ndcX;
		oriY = ndcY;

		glutPostRedisplay();
	}
}