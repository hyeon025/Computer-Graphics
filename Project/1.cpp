#include <gl/glew.h>
#include <gl/freeglut.h>
#include <gl/freeglut_ext.h>
#include <iostream>
#include <random>


using namespace std;

random_device rd;
default_random_engine dre{ rd() };
uniform_real_distribution ran{ 0.0,1.0 };

GLvoid drawScene(GLvoid);
GLvoid Reshape(int w, int h);
GLvoid Keyboard(unsigned char key, int x, int y);
GLvoid TimerFunction(int value);

float c_b = 1.0f, c_r = 1.0f, c_g = 1.0f;
bool timer = false;

void main(int argc, char** argv)
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA);
	glutInitWindowPosition(100, 100);	//������ ��ġ
	glutInitWindowSize(800, 600);		//������ ũ��
	glutCreateWindow("Example1");	//������ �̸�
	//--- GLEW �ʱ�ȭ�ϱ�
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
	glutMainLoop();
}
GLvoid drawScene()
{
	glClearColor(c_r, c_g, c_b, 1.0f);	//������
	glClear(GL_COLOR_BUFFER_BIT);	//��ü ĥ�ϱ�
	// �׸��� �κ� ����: �׸��� ���úκ��̿��⿡���Եȴ�.
	glutSwapBuffers();	//ȭ�� ���
}
GLvoid Reshape(int w, int h)
{
	glViewport(0, 0, w, h);
}
GLvoid Keyboard(unsigned char key, int x, int y)
{
	switch (key) {
	case 'c':
	case 'C':
		c_r = 0.0f, c_g = 1.0f, c_b = 1.0f;
		break;
	case 'm':
	case 'M':
		c_r = 1.0f, c_g = 0.0f, c_b = 1.0f;
		break;
	case 'y':
	case 'Y':
		c_r = 1.0f, c_g = 1.0f, c_b = 0.0f;
		break;
	case 'a':
	case 'A':	//������
		c_r = ran(dre), c_g = ran(dre), c_b = ran(dre);
		break;
	case 'w':
	case 'W':
		c_r = 1.0f, c_g = 1.0f, c_b = 1.0f;
		break;
	case 'k':
	case 'K':
		c_r = 0.0f, c_g = 0.0f, c_b = 0.0f;
		break;
	case 't':
	case 'T':
		if (!timer) {
			timer = true;
			glutTimerFunc(100, TimerFunction, 1);
		}
		break;
	case 's':
	case 'S':
		timer = false;
		break;
	case 'q':
	case 'Q':
		glutLeaveMainLoop();
		break;
	}
	glutPostRedisplay();
}

GLvoid TimerFunction(int value) {
	if (timer) {
		c_r = ran(dre);
		c_g = ran(dre);
		c_b = ran(dre);

		glutPostRedisplay();
	}
	glutTimerFunc(100, TimerFunction, 1);
}