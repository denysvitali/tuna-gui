#include <iostream>
#include <string>
#include <sstream>
#include <cstring>

#ifdef _WINDOWS
	#include <Windows.h>
#endif

#include "version.hh"
#include "tuna-ge.h"

using namespace tunagui;
using namespace tunage;

std::string version(){
    std::stringstream ss{};
    ss << "v" << GUI_MAJOR << "." << GUI_MINOR << "." << GUI_PATCH << "\t\t";
    ss << "Commit: " << Version::GIT_SHA1 << "\tMessage: " << Version::GIT_COMMIT_SUBJECT;
    return ss.str();
}

void doVersion(){
    std::cout << "Tuna GUI" << std::endl;
    std::cout << version() << std::endl;
}


/////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////CALLBACKS///////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////

bool firstMouse = true;
double lastX = 0;
double lastY = 0;

void mouseCallback(int button, int state, int mouseX, int mouseY) {

	switch (state)
	{
		case 1:
			firstMouse = true;
			break;
		default:
			break;
	}
}

void motionCallback(int mouseX, int mouseY){

	if (firstMouse) {
		lastX = mouseX;
		lastY = mouseY;
		firstMouse = false;
	}

	float xoffset = mouseX - lastX;
	float yoffset = lastY - mouseY;
	lastX = mouseX;
	lastY = mouseY;

	float sensitivity = 0.1;
	xoffset *= sensitivity;
	yoffset *= sensitivity;

	glm::vec3 front = TunaGE::getCurrentCamera().getCameraFront();

	glm::vec3 f2 = glm::normalize(front);
	double m_pitch = asin(f2.y);
	double m_yaw = atan2(front.z/cos(m_pitch), front.x/cos(m_pitch)) ;


	m_yaw += glm::radians(xoffset);
	m_pitch += glm::radians(yoffset);

	if (m_pitch > glm::radians(89.0f)) {
		m_pitch = glm::radians(89.0f);
	}
	if (m_pitch < glm::radians(-89.0f)) {
		m_pitch = glm::radians(-89.0f);
	}


	front.x = cos(m_yaw) * cos(m_pitch);
	front.y = sin(m_pitch);
	front.z = sin(m_yaw) * cos(m_pitch);

	std::cout << "Front: " << front.x << ", " << front.y << ", " << front.z << std::endl;
	TunaGE::getCurrentCamera().setCameraFront(front);

	//glutPostWindowRedisplay(windowId);
}



void kbdCB(unsigned char c, int mouseX, int mouseY) {
	Camera& camera = TunaGE::getCurrentCamera();
    switch (c) {
        case 'w':
            camera.setCameraPos(camera.getCameraPos() + (camera.getCameraSpeed() * camera.getCameraFront()));
            break;
        case 's':
            camera.setCameraPos(camera.getCameraPos() - (camera.getCameraSpeed() * camera.getCameraFront()));
            break;
        case 'd':
            camera.setCameraPos(camera.getCameraPos() + (glm::normalize(glm::cross(camera.getCameraFront(), camera.getCameraUp()))*camera.getCameraSpeed()));
            break;
        case 'a':
            camera.setCameraPos(camera.getCameraPos() - (glm::normalize(glm::cross(camera.getCameraFront(), camera.getCameraUp()))*camera.getCameraSpeed()));
            break;
		case 'c':
			TunaGE::renderList.switchCamera();
			break;
		default:
			break;
    }
}

/////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////


int main(int argc, char** argv) {
	for (int i = 0; i < argc; i++) {
		if (strcmp(argv[i], "-v") == 0) {
			doVersion();
			return 0;
		}
	}

	TunaGE::setMotionCallback(motionCallback);
	TunaGE::setMouseCallback(mouseCallback);
	TunaGE::setKeyboardCallback(kbdCB);

	TunaGE::init();
	TunaGE::enableOriginMarker();


	Camera camera1{"camera 1"};
	Camera camera2{"camera 2"};

	camera1.setCameraPos(glm::vec3(-0.0, 20.0f, -20.0f));  // Camera is at (0,2,10), in World Space
	camera1.setCameraFront(glm::vec3(0.01f, 0.0f, 0.9f)); // looks in the direction
	camera1.setCameraUp(glm::vec3(0.0f, 1.0f, 0.0f)); // Head is up (set to 0,-1,0 to look upside-down)
	camera1.setCameraSpeed(2);

	camera2.setCameraPos(glm::vec3(-0.0, 20.0f, -20.0f));  // Camera is at (0,2,10), in World Space
	camera2.setCameraFront(glm::vec3(0.01f, 0.0f, 0.9f)); // looks in the direction
	camera2.setCameraUp(glm::vec3(0.0f, 1.0f, 0.0f)); // Head is up (set to 0,-1,0 to look upside-down)
	camera2.setCameraSpeed(2);

	Mesh mesh{ "plane" };

	Material material{};
	material.setAmbient(glm::vec3(0.0f, 0.0f, 0.0f));
	material.setShininess(120);
	material.setSpecular(glm::vec3(1.0f, 1.0f, 1.0f));
	material.setDiffuse(glm::vec3(0.5f, 0.5f, 0.5f));

	Texture tex{"a bad time"};

#if _WINDOWS
	tex.loadFromFile("../tuna-ge/assets/textures/sans.png");
#else
	tex.loadFromFile("../../tuna-ge/assets/textures/sans.png");

#endif

	material.setTexture(&tex);

	mesh.setMaterial(material);
	mesh.setMatrix(glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, -10.0f, 0.0f)));

	float width = 1;
	Vertex v1{ -width, 0, 0, 0, 1, 0, 0, 0 };
	Vertex v2{ width, 0, 0, 0, 1, 0, 1, 1 };
	Vertex v3{ 0, 0, -width, 0, 1, 0 , 1, 0 };

	Vertex v4{ 0, 0, width, 0, 1, 0 , 0, 1 };
	Vertex v5{ width, 0, 0, 0, 1, 0 , 1, 1 };
	Vertex v6{ -width, 0, 0, 0, 1, 0 , 0 , 0 };

	mesh.addVertex(v1);
	mesh.addVertex(v2);
	mesh.addVertex(v3);
	mesh.addVertex(v4);
	mesh.addVertex(v5);
	mesh.addVertex(v6);

	Node root{"root"};
	root.link(&mesh);
	root.link(&camera1);
	
	Light light{ RGBColor{100, 255, 255}, "Light 1" };
	light.setMatrix(glm::translate(glm::mat4(1.0f), glm::vec3(1.0f, 1.0f, 0.0f)));
	light.setLight(1);
	light.setIntensity(1);
	light.setLightAmbient(glm::vec3(0.0f, 1.0f, 0.0f));
	light.setLightDiffuse(glm::vec3(0.0f, 1.0f, 0.0f));
	light.setLightSpecular(glm::vec3(0.0f, 1.0f, 0.0f));
	light.enable();

	mesh.link(&light);

    Light light2{ RGBColor{100, 255, 255}, "Light 2" };
    light2.setMatrix(glm::translate(glm::mat4(1.0f), glm::vec3(-2.0f, 0.0f, 0.0f)));
    light2.setLight(2);
    light2.setIntensity(1.0);
    light2.setLightAmbient(glm::vec3(1.0f, 1.0f, 1.0f));
    light2.setLightDiffuse(glm::vec3(1.0f, 1.0f, 1.0f));
    light2.setLightSpecular(glm::vec3(1.0f, 1.0f, 1.0f));
    light2.enable();

    light.link(&light2);
    light2.link(&camera2);

    Light light3{ RGBColor{100, 255, 255}, "Light 3" };
    light3.setMatrix(glm::translate(glm::mat4(1.0f), glm::vec3(-2.0f, 0.0f, -2.0f)));
    light3.setLight(3);
    light3.setIntensity(1.0);
    light3.setLightAmbient(glm::vec3(1.0f, 0.0f, 0.0f));
    light3.setLightDiffuse(glm::vec3(1.0f, 0.0f, 0.0f));
    light3.setLightSpecular(glm::vec3(1.0f, 0.0f, 0.0f));
    light3.enable();

    light.link(&light3);

    Light light4{ RGBColor{100, 255, 255}, "Light 4" };
    light4.setMatrix(glm::translate(glm::mat4(1.0f), glm::vec3(-0.8f, -0.8f, 0.4f)));
    light4.setLight(4);
    light4.setIntensity(1.0);
    light4.setLightAmbient(glm::vec3(0.0f, 0.0f, 1.0f));
    light4.setLightDiffuse(glm::vec3(0.0f, 0.0f, 1.0f));
    light4.setLightSpecular(glm::vec3(0.0f, 0.0f, 1.0f));
    light4.enable();

    light.link(&light4);

    Node* rootest;
#if _WINDOWS
    rootest = TunaGE::loadOVO("../tuna-ge/assets/scenes/simple3dScene.ovo");
#else
    rootest = TunaGE::loadOVO("../../tuna-ge/assets/scenes/simple3dScene.ovo");
#endif

    rootest->link(&camera1);
    rootest->getChildren().front()->link(&camera2);
    TunaGE::renderList.pass(rootest);

//	TunaGE::renderList.pass(&root);

	std::cout << "Library Version: " << TunaGE::version() << std::endl;
	std::cout << "GUI Version: " << version() << std::endl;
	TunaGE::loop();
}