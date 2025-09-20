#include <gl/glew.h>
#include <gl/freeglut.h>
#include <gl/freeglut_ext.h>
#include <iostream>
#include <random>
#include <chrono>

#define winW 800
#define winH 600
#define rectnum 5

using namespace std;
using namespace std::chrono; //�ð� Ÿ�̸�

random_device rd;
default_random_engine dre{ rd() };
uniform_real_distribution ran{ -0.9f,0.9f };
uniform_real_distribution urd{ 0.0f,1.0f };

GLvoid drawScene(GLvoid);
GLvoid Reshape(int w, int h);
GLvoid Mouse(int button, int state, int x, int y);
GLvoid Keyboard(unsigned char key, int x, int y);
GLvoid glutTimerFunction(int value);

int ani_num = 0; //1:ƨ���, 2:�������
bool move_rect = true;

auto lastTime = high_resolution_clock::now(); //�ð� Ÿ�̸�
const float targetFPS = 100.0f;
const float frameTime = 1000.0f / targetFPS;

struct Rect {
	float l, b, r, t;
	float color[3];
	int dir = 1; // 1:����, 2;���Ʒ�, 3:����, 4:�޾Ʒ�
	int dir2 = 1; // 1:����, 2:��, 3:�����Ʒ�, 4:�޾Ʒ�
	int size_dir = 1; //1:���η� Ŀ��, 2:���η� �۾���, 3:���η� Ŀ��, 4:���η� �۾���
	bool following = false;
	float down_move = 0.0f;
};

vector<Rect> rect;	//�ʱ� �簢��
vector<Rect> anirect;	//�ִϸ��̼� �簢��
bool change_rect = false;
bool change_color = false;
bool follow = false;
int rect_num = -1;

void main(int argc, char** argv)
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA);
	glutInitWindowPosition(100, 100);	//������ ��ġ
	glutInitWindowSize(winW, winH);		//������ ũ��
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
	glutMouseFunc(Mouse);
	glutTimerFunc(16, glutTimerFunction, 1);
	glutMainLoop();
}
GLvoid drawScene()
{
	glClearColor(0.2f, 0.2f, 0.2f, 1.0f);	//������
	glClear(GL_COLOR_BUFFER_BIT);	//��ü ĥ�ϱ�
	// �׸��� �κ� ����: �׸��� ���úκ��� ���⿡ ���Եȴ�.

	for (const auto& a : anirect) {
		glColor3f(a.color[0], a.color[1], a.color[2]);
		glRectf(a.l, a.b, a.r, a.t);
	}

	glutSwapBuffers();	//ȭ�� ���
}
GLvoid Reshape(int w, int h)
{
	glViewport(0, 0, w, h);
}

GLvoid Keyboard(unsigned char key, int x, int y)
{
	switch (key) {
	case '1': //�밢�� ƨ��� �̵�/����
	{
		if (ani_num == 1) {
			ani_num = 0;
		}
		else if (ani_num == 0) {
			ani_num = 1;
		}
		else if(ani_num == 2){
			ani_num = 1;
		}
		break;
	}
	case '2':	//������� �̵�/����
	{
		if (ani_num == 2) {
			ani_num = 0;
		}
		else if (ani_num == 0) {
			ani_num = 2;
		}
		else if(ani_num == 1){
			ani_num = 2;
		}

		break;
	}
	case '3': {	//ũ�� ����/�� ����
		for (auto& a : anirect) {
			a.size_dir = rand() % 4 + 1;
		}
		if (!change_rect)
			change_rect = true;
		else
			change_rect = false;
		break;
	}
	case '4': {	//�� ����/�� ����
		if (!change_color)
			change_color = true;
		else
			change_color = false;
		break;
	}
	case '5': {
		if (!follow) {
			if (anirect.size() > 1) {
				follow = true;
				rect_num = rand() % anirect.size();
				for (size_t i = 0; i < anirect.size(); i++) {
					if (i != rect_num) {
						anirect[i].following = true;
					}
					else {
						anirect[i].following = false;
					}
				}
			}
		}
		else {
			follow = false;
			rect_num = -1;
			for (auto& a : anirect) {
				a.following = false;
			}
		}
		break;
	}
	case 's':
	case 'S': { //�ִϸ��̼� ����
		if(!move_rect)
			move_rect = true;
		else
			move_rect = false;
		break;
	}
	case 'm':
	case 'M': {	//���� ��ġ��
		anirect = rect;
		break;
	}
	case 'r':
	case 'R': {	//��� �簢�� ����
		ani_num = 0;
		move_rect = true;
		change_rect = false;
		change_color = false;
		rect.clear();
		anirect.clear();
		break;
	}
	case 'q':
	case 'Q': {
		glutLeaveMainLoop(); //����
	}
	}
	glutPostRedisplay();
}
GLvoid Mouse(int button, int state, int x, int y) {

	float ndcX = (2.0f * x) / winW - 1.0f; //��ǥ ��ȯ
	float ndcY = 1.0f - (2.0f * y) / winH;

	if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN) {
		if (rect.size() < rectnum) {
			Rect r;
			r.l = ndcX - 0.1f;
			r.t = ndcY + 0.1f;
			r.r = ndcX + 0.1f;
			r.b = ndcY - 0.1f;

			r.color[0] = urd(dre);
			r.color[1] = urd(dre);
			r.color[2] = urd(dre);
			r.dir2 = 1;

			rect.push_back(r);
			anirect.push_back(r);
		}
	}

	glutPostRedisplay();

}

void glutTimerFunction(int value) {

	auto currentTime = high_resolution_clock::now(); //Ÿ�̸� �ð�
	auto elapsed = duration_cast<milliseconds>(currentTime - lastTime).count();
	lastTime = currentTime;

	float deltaTime = elapsed / 1000.0f;
	float speed = 0.1f;
	float move = speed * deltaTime;

	if (move_rect) {
		if (change_rect) { //��� �ٲٱ�
			for (auto& a: anirect) {
				switch (a.size_dir) {
				case 1: { 
					a.l -= move;
					a.r += move;
					if (a.l <= -1.0f)
						a.l = -1.0f;
					if (a.r >= 1.0f)
						a.r = 1.0f;
					if (a.r - a.l >= 0.5f) {
						a.size_dir = 2;
					}
					break;
				}
				case 2: {
					a.l += move;
					a.r -= move;
					if (a.l <= -1.0f)
						a.l = -1.0f;
					if (a.r >= 1.0f)
						a.r = 1.0f;

					if (a.r - a.l <= 0.2f)
						a.size_dir = 3;
					break;
				}
				case 3: {
					a.t += move;
					a.b -= move;

					if (a.t >= 1.0f)
						a.t = 1.0f;
					if (a.b <= -1.0f)
						a.b = -1.0f;

					if (a.t - a.b >= 0.5f)
						a.size_dir = 4;
					break;
				}
				case 4: {
					a.t -= move;
					a.b += move;

					if (a.t >= 1.0f)
						a.t = 1.0f;
					if (a.b <= -1.0f)
						a.b = -1.0f;

					if (a.t - a.b <= 0.2f)
						a.size_dir = 1;
					break;
				}
				}
			}
		}

		if (ani_num != 0) {
			switch (ani_num) {
			case 1: { //ƨ���
				for (size_t i = 0; i < anirect.size();++i) {
					if (follow && i != rect_num) {
						continue;
					}
					switch (anirect[i].dir) {
					case 1: { //����
						anirect[i].l += move;
						anirect[i].r += move;
						anirect[i].t += move;
						anirect[i].b += move;
						if (anirect[i].r >= 1.0f) 
							anirect[i].dir = 3;
						if (anirect[i].t >= 1.0f) 
							anirect[i].dir = 2;
						break;
					}
					case 2: { //���Ʒ�
						anirect[i].l += move;
						anirect[i].r += move;
						anirect[i].t -= move;
						anirect[i].b -= move;
						if (anirect[i].r >= 1.0f) anirect[i].dir = 4;
						if (anirect[i].b <= -1.0f) anirect[i].dir = 1;
						break;
					}
					case 3: { //����
						anirect[i].l -= move;
						anirect[i].r -= move;
						anirect[i].t += move;
						anirect[i].b += move;
						if (anirect[i].l <= -1.0f) anirect[i].dir = 1;
						if (anirect[i].t >= 1.0f) anirect[i].dir = 4;
						break;
					}
					case 4: { //�޾Ʒ�
						anirect[i].l -= move;
						anirect[i].r -= move;
						anirect[i].t -= move;
						anirect[i].b -= move;
						if (anirect[i].l <= -1.0f) anirect[i].dir = 2;
						if (anirect[i].b <= -1.0f) anirect[i].dir = 3;
						break;
					}
					}
				}
				break;
			}
			case 2: { //�������
				for (size_t i = 0; i < anirect.size();++i) {
					if (follow && i != rect_num) {
						continue;
					}
					switch (anirect[i].dir2) {
					case 1: { //��
						anirect[i].l += move;
						anirect[i].r += move;

						if (anirect[i].r >= 1.0f)
							anirect[i].dir2 = 3;
						break;
					}
					case 2: { //��
						anirect[i].l -= move;
						anirect[i].r -= move;

						if (anirect[i].l <= -1.0f)
							anirect[i].dir2 = 4;
						break;
					}
					case 3: { //�����Ʒ�
						anirect[i].t -= move;
						anirect[i].b -= move;
						anirect[i].down_move += move;

						if (anirect[i].down_move >= 0.2f) {
							anirect[i].dir2 = 2;
							anirect[i].down_move = 0.0f;
						}

						if (anirect[i].b <= -1.0f) {
							float height = anirect[i].t - anirect[i].b;
							anirect[i].t = 1.0f;
							anirect[i].b = anirect[i].t - height;
							anirect[i].dir2 = 1;
						}

						break;
					}
					case 4: { //�޾Ʒ�
						anirect[i].t -= move;
						anirect[i].b -= move;
						anirect[i].down_move += move;

						if (anirect[i].down_move >= 0.2f) {
							anirect[i].dir2 = 1;
							anirect[i].down_move = 0.0f;
						}

						if (anirect[i].b <= -1.0f) {
							float height = anirect[i].t - anirect[i].b;
							anirect[i].t = 1.0f;
							anirect[i].b = anirect[i].t - height;
							anirect[i].dir2 = 1;
						}
						break;
					}
					}
				}
				break;
			}
			}
		}
		if (follow && rect_num >= 0 && rect_num < anirect.size()) {
			float follow_speed = 2.0f;

			vector<int> followers;
			for (size_t i = 0; i < anirect.size(); i++) {
				if (i != rect_num) {
					followers.push_back(i);
				}
			}
			for (size_t i = 0; i < followers.size(); i++) {
				int target_idx;
				if (i == 0) {
					target_idx = rect_num;
				}
				else
					target_idx = followers[i - 1];


				float target_num_x = (anirect[target_idx].l + anirect[target_idx].r) / 2;
				float target_num_y = (anirect[target_idx].t + anirect[target_idx].b) / 2;

				float follower_rect_x = (anirect[followers[i]].l + anirect[followers[i]].r) / 2;
				float follower_rect_y = (anirect[followers[i]].t + anirect[followers[i]].b) / 2;

				float width = anirect[followers[i]].r - anirect[followers[i]].l;
				float height = anirect[followers[i]].t - anirect[followers[i]].b;

				float dx = target_num_x - follower_rect_x;
				float dy = target_num_y - follower_rect_y;

				float dist = sqrt(dx * dx + dy * dy);

				if (dist > 0.2f) {
					float new_rect_x = follower_rect_x + dx / dist * follow_speed * move;
					float new_rect_y = follower_rect_y + dy / dist * follow_speed * move;

					anirect[followers[i]].l = new_rect_x - width / 2;
					anirect[followers[i]].r = new_rect_x + width / 2;
					anirect[followers[i]].b = new_rect_y - height / 2;
					anirect[followers[i]].t = new_rect_y + height / 2;

					if (anirect[followers[i]].l < -1.0f) {
						anirect[followers[i]].l = -1.0f;
						anirect[followers[i]].r = anirect[followers[i]].l + width;
					}
					if (anirect[followers[i]].r > 1.0f) {
						anirect[followers[i]].r = 1.0f;
						anirect[followers[i]].l = anirect[followers[i]].r - width;
					}
					if (anirect[followers[i]].b < -1.0f) {
						anirect[followers[i]].b = -1.0f;
						anirect[followers[i]].t = anirect[followers[i]].b + height;
					}
					if (anirect[followers[i]].t > 1.0f) {
						anirect[followers[i]].t = 1.0f;
						anirect[followers[i]].b = anirect[followers[i]].t - height;
					}
				}

			}

		}

		if (change_color) {
			for (auto& a : anirect) {
				a.color[0] = urd(dre);
				a.color[1] = urd(dre);
				a.color[2] = urd(dre);
			}
		}
	}

	glutTimerFunc(16, glutTimerFunction, 1);
	glutPostRedisplay();
	
	
}