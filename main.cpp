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

	float sensitivity = 0.2;
	xoffset *= sensitivity;
	yoffset *= sensitivity;

	glm::vec3 front = TunaGE::getCurrentCamera()->getCameraFront();

	glm::vec3 f2 = glm::normalize(front);
	double m_pitch = asin(f2.y);
	double m_yaw = atan2(f2.z/cos(m_pitch), f2.x/cos(m_pitch)) ;


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

	TunaGE::getCurrentCamera()->setCameraFront(front);
	TunaGE::redisplay();

	//glutPostWindowRedisplay(windowId);
}



void kbdCB(unsigned char c, int mouseX, int mouseY) {
	Camera* camera = TunaGE::getCurrentCamera();
	Node* root;
    switch (c) {
        case 'w':
            camera->setCameraPos(camera->getCameraPos() + (camera->getCameraSpeed() * camera->getCameraFront()));
            break;
        case 's':
            camera->setCameraPos(camera->getCameraPos() - (camera->getCameraSpeed() * camera->getCameraFront()));
            break;
        case 'd':
            camera->setCameraPos(camera->getCameraPos() + (glm::normalize(glm::cross(camera->getCameraFront(), camera->getCameraUp()))*camera->getCameraSpeed()));
            break;
        case 'a':
            camera->setCameraPos(camera->getCameraPos() - (glm::normalize(glm::cross(camera->getCameraFront(), camera->getCameraUp()))*camera->getCameraSpeed()));
            break;
		case 'c':
			TunaGE::renderList.switchCamera();
			break;
		case '6':
			root = TunaGE::renderList.getSceneRoot();
			if (root != nullptr) {
				root->getSceneElementByName("Capsule013")->setMatrix(glm::rotate(root->getSceneElementByName("Capsule013")->getMatrix(), glm::radians(1.0f), glm::vec3(1, 0, 0)));
				root->getSceneElementByName("Capsule013")->setMatrix(glm::rotate(root->getSceneElementByName("Capsule013")->getMatrix(), glm::radians(-0.5f), glm::vec3(0, 1, 0)));
				root->getSceneElementByName("Capsule014")->setMatrix(glm::rotate(root->getSceneElementByName("Capsule014")->getMatrix(), glm::radians(1.0f), glm::vec3(1, 0, 0)));
				TunaGE::renderList.clear();
				TunaGE::renderList.pass(root);
			}
			break;
		case 'z':
			root = TunaGE::renderList.getSceneRoot();
			if (root != nullptr) {
				root->getSceneElementByName("Capsule013")->setMatrix(glm::rotate(root->getSceneElementByName("Capsule013")->getMatrix(), glm::radians(-1.0f), glm::vec3(1, 0, 0)));
				root->getSceneElementByName("Capsule013")->setMatrix(glm::rotate(root->getSceneElementByName("Capsule013")->getMatrix(), glm::radians(0.5f), glm::vec3(0, 1, 0)));
				root->getSceneElementByName("Capsule014")->setMatrix(glm::rotate(root->getSceneElementByName("Capsule014")->getMatrix(), glm::radians(-1.0f), glm::vec3(1, 0, 0)));
				TunaGE::renderList.clear();
				TunaGE::renderList.pass(root);
			}
			break;
		case '7':
			root = TunaGE::renderList.getSceneRoot();
			if (root != nullptr) {
				root->getSceneElementByName("Capsule010")->setMatrix(glm::rotate(root->getSceneElementByName("Capsule010")->getMatrix(), glm::radians(1.5f), glm::vec3(1, 0, 0)));
				root->getSceneElementByName("Capsule011")->setMatrix(glm::rotate(root->getSceneElementByName("Capsule011")->getMatrix(), glm::radians(5.0f), glm::vec3(1, 0, 0)));
				root->getSceneElementByName("Capsule012")->setMatrix(glm::rotate(root->getSceneElementByName("Capsule012")->getMatrix(), glm::radians(2.0f), glm::vec3(1, 0, 0)));
				TunaGE::renderList.clear();
				TunaGE::renderList.pass(root);
			}
			break;
		case 'u':
			root = TunaGE::renderList.getSceneRoot();
			if (root != nullptr) {
				root->getSceneElementByName("Capsule010")->setMatrix(glm::rotate(root->getSceneElementByName("Capsule010")->getMatrix(), glm::radians(-1.5f), glm::vec3(1, 0, 0)));
				root->getSceneElementByName("Capsule011")->setMatrix(glm::rotate(root->getSceneElementByName("Capsule011")->getMatrix(), glm::radians(-5.0f), glm::vec3(1, 0, 0)));
				root->getSceneElementByName("Capsule012")->setMatrix(glm::rotate(root->getSceneElementByName("Capsule012")->getMatrix(), glm::radians(-2.0f), glm::vec3(1, 0, 0)));
				TunaGE::renderList.clear();
				TunaGE::renderList.pass(root);
			}
			break;
		case '8':
			root = TunaGE::renderList.getSceneRoot();
			if (root != nullptr) {
				root->getSceneElementByName("Capsule007")->setMatrix(glm::rotate(root->getSceneElementByName("Capsule007")->getMatrix(), glm::radians(1.5f), glm::vec3(1, 0, 0)));
				root->getSceneElementByName("Capsule008")->setMatrix(glm::rotate(root->getSceneElementByName("Capsule008")->getMatrix(), glm::radians(5.0f), glm::vec3(1, 0, 0)));
				root->getSceneElementByName("Capsule009")->setMatrix(glm::rotate(root->getSceneElementByName("Capsule009")->getMatrix(), glm::radians(2.0f), glm::vec3(1, 0, 0)));
				TunaGE::renderList.clear();
				TunaGE::renderList.pass(root);
			}
			break;
		case 'i':
			root = TunaGE::renderList.getSceneRoot();
			if (root != nullptr) {
				root->getSceneElementByName("Capsule007")->setMatrix(glm::rotate(root->getSceneElementByName("Capsule007")->getMatrix(), glm::radians(-1.5f), glm::vec3(1, 0, 0)));
				root->getSceneElementByName("Capsule008")->setMatrix(glm::rotate(root->getSceneElementByName("Capsule008")->getMatrix(), glm::radians(-5.0f), glm::vec3(1, 0, 0)));
				root->getSceneElementByName("Capsule009")->setMatrix(glm::rotate(root->getSceneElementByName("Capsule009")->getMatrix(), glm::radians(-2.0f), glm::vec3(1, 0, 0)));
				TunaGE::renderList.clear();
				TunaGE::renderList.pass(root);
			}
			break;
		case '9':
			root = TunaGE::renderList.getSceneRoot();
			if (root != nullptr) {
				root->getSceneElementByName("Capsule004")->setMatrix(glm::rotate(root->getSceneElementByName("Capsule004")->getMatrix(), glm::radians(1.2f), glm::vec3(1, 0, 0)));
				root->getSceneElementByName("Capsule005")->setMatrix(glm::rotate(root->getSceneElementByName("Capsule005")->getMatrix(), glm::radians(5.0f), glm::vec3(1, 0, 0)));
				root->getSceneElementByName("Capsule006")->setMatrix(glm::rotate(root->getSceneElementByName("Capsule006")->getMatrix(), glm::radians(1.8f), glm::vec3(1, 0, 0)));
				TunaGE::renderList.clear();
				TunaGE::renderList.pass(root);
			}
			break;
		case 'o':
			root = TunaGE::renderList.getSceneRoot();
			if (root != nullptr) {
				root->getSceneElementByName("Capsule004")->setMatrix(glm::rotate(root->getSceneElementByName("Capsule004")->getMatrix(), glm::radians(-1.2f), glm::vec3(1, 0, 0)));
				root->getSceneElementByName("Capsule005")->setMatrix(glm::rotate(root->getSceneElementByName("Capsule005")->getMatrix(), glm::radians(-5.0f), glm::vec3(1, 0, 0)));
				root->getSceneElementByName("Capsule006")->setMatrix(glm::rotate(root->getSceneElementByName("Capsule006")->getMatrix(), glm::radians(-1.8f), glm::vec3(1, 0, 0)));
				TunaGE::renderList.clear();
				TunaGE::renderList.pass(root);
			}
			break;
		case '0':
			root = TunaGE::renderList.getSceneRoot();
			if (root != nullptr) {
				root->getSceneElementByName("Capsule001")->setMatrix(glm::rotate(root->getSceneElementByName("Capsule001")->getMatrix(), glm::radians(1.0f), glm::vec3(1, 0, 0)));
				root->getSceneElementByName("Capsule002")->setMatrix(glm::rotate(root->getSceneElementByName("Capsule002")->getMatrix(), glm::radians(5.0f), glm::vec3(1, 0, 0)));
				root->getSceneElementByName("Capsule003")->setMatrix(glm::rotate(root->getSceneElementByName("Capsule003")->getMatrix(), glm::radians(1.7f), glm::vec3(1, 0, 0)));
				TunaGE::renderList.clear();
				TunaGE::renderList.pass(root);
			}
			break;
		case 'p':
			root = TunaGE::renderList.getSceneRoot();
			if (root != nullptr) {
				root->getSceneElementByName("Capsule001")->setMatrix(glm::rotate(root->getSceneElementByName("Capsule001")->getMatrix(), glm::radians(-1.0f), glm::vec3(1, 0, 0)));
				root->getSceneElementByName("Capsule002")->setMatrix(glm::rotate(root->getSceneElementByName("Capsule002")->getMatrix(), glm::radians(-5.0f), glm::vec3(1, 0, 0)));
				root->getSceneElementByName("Capsule003")->setMatrix(glm::rotate(root->getSceneElementByName("Capsule003")->getMatrix(), glm::radians(-1.7f), glm::vec3(1, 0, 0)));
				TunaGE::renderList.clear();
				TunaGE::renderList.pass(root);
			}
			break;
		default:
			break;
    }

    TunaGE::redisplay();
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

	Camera camera1{"camera 1"};
	Camera camera2{"camera 2"};

	camera1.setCameraPos(glm::vec3(142.0f, 135.0f, 0.0f));
	camera1.setCameraFront(glm::vec3(-1.0f, -0.30f, 0.01f));
	camera1.setCameraUp(glm::vec3(0.0f, 1.0f, 0.0f));
	camera1.setCameraSpeed(3);

	camera2.setCameraPos(glm::vec3(-0.0, 20.0f, -20.0f));
	camera2.setCameraFront(glm::vec3(0.01f, 0.0f, 0.9f));
	camera2.setCameraUp(glm::vec3(0.0f, 1.0f, 0.0f));
	camera2.setCameraSpeed(3);

    Node* rootest;

#if _WINDOWS
    rootest = TunaGE::loadOVO("D:/ProgettoGE/tuna/tuna-ge/assets/scenes/gauntlet.OVO");
#else
    rootest = TunaGE::loadOVO("../../tuna-ge/assets/scenes/gauntlet.ovo");
#endif

    rootest->link(&camera1);
	//((Mesh*)rootest->getSceneElementByName("Plane001"))->getMaterial().setAlpha(0.2f);
	rootest->getSceneElementByName("Cylinder001")->setFlipScene(true);
	rootest->getSceneElementByName("Omni001")->setFlipScene(true);
	rootest->getSceneElementByName("Spot001")->setFlipScene(true);
    TunaGE::renderList.pass(rootest);

	std::cout << "Library Version: " << TunaGE::version() << std::endl;
	std::cout << "GUI Version: " << version() << std::endl;
	TunaGE::loop();
}