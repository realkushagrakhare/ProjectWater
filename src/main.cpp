#include "StdHeader.h"
#include <GL/glew.h>
#include <GL/freeglut.h>
#include "vsMathLib.h"
#include "vsShaderLib.h"
#include "Shader.h"
#include "Mesh.h"
#include "Water.h"
#include "Renderer.h"
#include "Cubemap.h"
#include "vec3.h"
#include "SOIL.h"
#include "Texture.h"
#include "Raytracer.h"
#include <sfml/OpenGL.hpp>
#include <SFML/Window.hpp>

VSMathLib *vsml;
Shader *shader;
Mesh *mesh;

Water *water;
Cubemap *cubemap;
Renderer *renderer;
float angleX = -25.f;
float angleY = -200.5f;

int paused = 0;
int pNo = 0;
int prevTime;
Vector planeNormal;
int mode = -1;
const int MODE_ADD_DROPS = 0;
const int MODE_MOVE_LIGHT = 1;
const int MODE_ORBIT_CAMERA = 2;
int oldX, oldY;
int winWidth, winHeight;
float min_interval = 1.0/75;

void resize(int w, int h) {
	float fov;
	// Prevent a divide by zero, when window is too short
	if(h == 0) h = 1;
	// set the viewport to be the entire window
	glViewport(0, 0, w, h);
    winWidth = w;
    winHeight = h;
	// set the projection matrix
	fov = (1.0f * w) / h;
	vsml->loadIdentity(VSMathLib::PROJECTION);
	vsml->perspective(45.f, fov, 0.1f, 100.0f);
}

void update(float seconds) {
    if (seconds > 1) return;

    water->stepSimulation();
    //water->stepSimulation();
    water->updateNormals();
    renderer->updateCaustics(water);
}


void draw(void) {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	vsml->loadIdentity(VSMathLib::VIEW);
	vsml->loadIdentity(VSMathLib::MODEL);
	vsml->translate(0, 0, -4);
    vsml->rotate(-angleX, 1, 0, 0);
    vsml->rotate(-angleY, 0, 1, 0);
    vsml->translate(0, 0.5, 0);

    glEnable(GL_DEPTH_TEST);
	renderer->renderCubeBox(water);
    renderer->renderWater(water, cubemap);
	renderer->renderCubeBox(water);
    glDisable(GL_DEPTH_TEST);
	glutSwapBuffers();
}


void processKeys(unsigned char key, int xx, int yy) {
	Vector n;
	switch(key) {
	case 27:
		glutLeaveMainLoop();
		break;
	case ' ':
	case 'p':
    case 'P':
        paused = !paused;
        break;
	case 'o':
	case 'O':
		renderer->toggleClear();
		break;
	case 'w':
	case 'W':
		renderer->addLightDir(Vector(0, 0, 0.05));
		break;
	case 's':
	case 'S':
		renderer->addLightDir(Vector(0, 0, -0.05));
		break;
	case 'a':
	case 'A':
		renderer->addLightDir(Vector(0.05, 0, 0));
		break;
	case 'd':
	case 'D':
		renderer->addLightDir(Vector(-0.05, 0, 0));
		break;
	case 'x':
	case 'X':
		renderer->setLightDir(Vector(0.00001, 0.999999, 0.00001));
    default:
        break;
	}

}

void onMouseMove(int x, int y){
    RayTracer tracer;
    Vector ray = tracer.getRayForPixel((float)x, (float)y);
    Vector pointOnPlane;
    switch (mode) {
    case MODE_ADD_DROPS: {
        pointOnPlane = tracer.eye.add(ray.multiply(- tracer.eye.y / ray.y));
        water->addDrop(pointOnPlane.x, pointOnPlane.z, 0.03, 0.01);
        if (paused) {
            water->updateNormals();
            renderer->updateCaustics(water);
        }
        break;
    }
    case MODE_ORBIT_CAMERA:
        angleY -= x - oldX;
        angleX -= y - oldY;
        angleX = std::max<float>(-89.999, std::min<float>(89.999, angleX));
        break;
    default:
        break;
    }
    oldX = x;
    oldY = y;
    if (paused) draw();
}

void onMouseDown(int x, int y) {
    oldX = x;
    oldY = y;
    RayTracer tracer;
    Vector ray = tracer.getRayForPixel((float)x, (float)y);
    Vector pointOnPlane = tracer.eye.add(ray.multiply(- tracer.eye.y / ray.y));
    
	if (fabs(pointOnPlane.x) < 1 && fabs(pointOnPlane.z) < 1) {
        mode = MODE_ADD_DROPS;
        onMouseMove(x, y);
    } 
    else {
        mode = MODE_ORBIT_CAMERA;
    }
}

void onMouseUp(int x, int y) {
    mode = -1;
} 

void processMouseButtons(int button, int state, int xx, int yy) {
	// start tracking the mouse
	if (state == GLUT_DOWN && button == GLUT_LEFT_BUTTON)  {
		onMouseDown(xx, yy);
	}
    else if (state == GLUT_UP && button == GLUT_LEFT_BUTTON) {
        onMouseUp(xx, yy);
    }
}

// Track mouse motion while buttons are pressed

void processMouseMotion(int xx, int yy) {
	onMouseMove(xx, yy);
}

void initOpenGL()
{
	glEnable(GL_DEPTH_TEST);
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
}


void initVSL() {
	vsml = VSMathLib::getInstance();
}

float rand_float() {
    return (float)rand() / (float)RAND_MAX;
}

void initScene() {
    water = new Water;
    cubemap = Cubemap::fromImages(
        "./images/xneg.jpg", 
        "./images/xpos.jpg",
        "./images/ypos.jpg",
        "./images/ypos.jpg",
        "./images/zneg.jpg",
        "./images/zpos.jpg"
    );
    renderer = new Renderer;
    prevTime = glutGet(GLUT_ELAPSED_TIME);
	//prevTime = 16;
}


void idle(){
    int nextTime = glutGet(GLUT_ELAPSED_TIME);
    float interval = float(nextTime - prevTime) / 1000.f;
    if (interval < min_interval) return;
    if (!paused) {
        update(interval);
        draw();
    }
    prevTime = nextTime;
}


int main(int argc, char **argv) {

//  GLUT initialization
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DEPTH|GLUT_DOUBLE|GLUT_RGBA|GLUT_MULTISAMPLE);

	glutInitContextVersion (3, 3);
	glutInitContextProfile (GLUT_CORE_PROFILE );
	//glutInitContextFlags(GLUT_DEBUG);

	glutInitWindowPosition(100,100);
	glutInitWindowSize(512,512);
	glutCreateWindow("Project Water");

//  Callback Registration
	glutDisplayFunc(draw);
	glutReshapeFunc(resize);
	glutIdleFunc(idle);

//	Mouse and Keyboard Callbacks
	glutKeyboardFunc(processKeys);
	glutMouseFunc(processMouseButtons);
	glutMotionFunc(processMouseMotion);


//	return from main loop
	glutSetOption(GLUT_ACTION_ON_WINDOW_CLOSE, GLUT_ACTION_GLUTMAINLOOP_RETURNS);

//	Init GLEW
	glewExperimental = GL_TRUE;
	glewInit();

	initOpenGL();
	initVSL();
    initScene();

	//  GLUT main loop
	glutMainLoop();

	return(0);

}

void mousePress(sf::Event& event) {
	int xx = sf::Mouse::getPosition().x, yy = sf::Mouse::getPosition().y;
	if (event.type == sf::Event::MouseButtonPressed && event.type == sf::Mouse::Left)
		onMouseDown(xx, yy);
	else if (event.type == sf::Event::MouseButtonReleased && event.type == sf::Mouse::Left)
		onMouseUp(xx, yy);
	else if (event.type == sf::Event::MouseMoved)
		onMouseMove(xx, yy);
}

void keyPress(sf::Event& event) {
	Vector n;
	if(event.type == sf::Keyboard::P)
		paused = !paused;
	else if(event.type == sf::Keyboard::O)
		renderer->toggleClear();
	else if (event.type == sf::Keyboard::W)
		renderer->addLightDir(Vector(0, 0, 0.05));
	else if (event.type == sf::Keyboard::S)
		renderer->addLightDir(Vector(0, 0, -0.05));
	else if (event.type == sf::Keyboard::A)
		renderer->addLightDir(Vector(0.05, 0, 0));
	else if (event.type == sf::Keyboard::D)
		renderer->addLightDir(Vector(-0.05, 0, 0));
	else if (event.type == sf::Keyboard::X)
		renderer->setLightDir(Vector(0.00001, 0.999999, 0.00001));

}

int main2()
{
	sf::ContextSettings window_settings;
	window_settings.depthBits = 24;
	window_settings.stencilBits = 8;
	window_settings.antialiasingLevel = 2;
	sf::Window window(sf::VideoMode(512, 512, 32), "Project Water", sf::Style::Default, window_settings);
	initOpenGL();
	initVSL();
	initScene();

	while (window.isOpen())
	{
		sf::Event event;
		while (window.pollEvent(event))
		{
			if (event.type == sf::Event::Closed)
				window.close();
			keyPress(event);
			mousePress(event);
		}


		draw();
		window.display();
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	}

	return 0;
}

