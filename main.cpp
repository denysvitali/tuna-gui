#include <iostream>
#include <string>
#include <sstream>
#include <cstring>
#include <iomanip>

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

	std::string c13n = "Capsule013";
	std::string c14n = "Capsule014";

	std::vector<Node*> capsules;

	Node* root = TunaGE::renderList.getSceneRoot();
	if (root != nullptr) {
		for(int i=1; i<=15; i++){
			std::stringstream cSS{};
			cSS << "Capsule" << std::setfill('0') << std::setw(3) << i;
			capsules.push_back(root->getSceneElementByName(cSS.str().data()));
		}
	}


	Camera* camera = TunaGE::getCurrentCamera();
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
				capsules[12]->setMatrix(glm::rotate(capsules[12]->getMatrix(), glm::radians(1.0f), glm::vec3(1, 0, 0)));
				capsules[12]->setMatrix(glm::rotate(capsules[12]->getMatrix(), glm::radians(-0.5f), glm::vec3(0, 1, 0)));
				capsules[13]->setMatrix(glm::rotate(capsules[13]->getMatrix(), glm::radians(1.0f), glm::vec3(1, 0, 0)));
				TunaGE::renderList.clear();
				TunaGE::renderList.pass(root);
			}
			break;
		case 'z':
			root = TunaGE::renderList.getSceneRoot();
			if (root != nullptr) {
				capsules[12]->setMatrix(glm::rotate(capsules[12]->getMatrix(), glm::radians(-1.0f), glm::vec3(1, 0, 0)));
				capsules[12]->setMatrix(glm::rotate(capsules[12]->getMatrix(), glm::radians(0.5f), glm::vec3(0, 1, 0)));
				capsules[13]->setMatrix(glm::rotate(capsules[13]->getMatrix(), glm::radians(-1.0f), glm::vec3(1, 0, 0)));
				TunaGE::renderList.clear();
				TunaGE::renderList.pass(root);
			}
			break;
		case '7':
			root = TunaGE::renderList.getSceneRoot();
			if (root != nullptr) {
				capsules[9]->setMatrix(glm::rotate(capsules[9]->getMatrix(), glm::radians(1.5f), glm::vec3(1, 0, 0)));
				capsules[10]->setMatrix(glm::rotate(capsules[10]->getMatrix(), glm::radians(5.0f), glm::vec3(1, 0, 0)));
				capsules[11]->setMatrix(glm::rotate(capsules[11]->getMatrix(), glm::radians(2.0f), glm::vec3(1, 0, 0)));
				TunaGE::renderList.clear();
				TunaGE::renderList.pass(root);
			}
			break;
		case 'u':
			root = TunaGE::renderList.getSceneRoot();
			if (root != nullptr) {
				capsules[9]->setMatrix(glm::rotate(capsules[9]->getMatrix(), glm::radians(-1.5f), glm::vec3(1, 0, 0)));
				capsules[10]->setMatrix(glm::rotate(capsules[10]->getMatrix(), glm::radians(-5.0f), glm::vec3(1, 0, 0)));
				capsules[11]->setMatrix(glm::rotate(capsules[11]->getMatrix(), glm::radians(-2.0f), glm::vec3(1, 0, 0)));
				TunaGE::renderList.clear();
				TunaGE::renderList.pass(root);
			}
			break;
		case '8':
			root = TunaGE::renderList.getSceneRoot();
			if (root != nullptr) {
				capsules[6]->setMatrix(glm::rotate(capsules[6]->getMatrix(), glm::radians(1.5f), glm::vec3(1, 0, 0)));
				capsules[7]->setMatrix(glm::rotate(capsules[7]->getMatrix(), glm::radians(5.0f), glm::vec3(1, 0, 0)));
				capsules[8]->setMatrix(glm::rotate(capsules[8]->getMatrix(), glm::radians(2.0f), glm::vec3(1, 0, 0)));
				TunaGE::renderList.clear();
				TunaGE::renderList.pass(root);
			}
			break;
		case 'i':
			root = TunaGE::renderList.getSceneRoot();
			if (root != nullptr) {
				capsules[6]->setMatrix(glm::rotate(capsules[6]->getMatrix(), glm::radians(-1.5f), glm::vec3(1, 0, 0)));
				capsules[7]->setMatrix(glm::rotate(capsules[7]->getMatrix(), glm::radians(-5.0f), glm::vec3(1, 0, 0)));
				capsules[8]->setMatrix(glm::rotate(capsules[8]->getMatrix(), glm::radians(-2.0f), glm::vec3(1, 0, 0)));
				TunaGE::renderList.clear();
				TunaGE::renderList.pass(root);
			}
			break;
		case '9':
			root = TunaGE::renderList.getSceneRoot();
			if (root != nullptr) {
				capsules[3]->setMatrix(glm::rotate(capsules[3]->getMatrix(), glm::radians(1.2f), glm::vec3(1, 0, 0)));
				capsules[4]->setMatrix(glm::rotate(capsules[4]->getMatrix(), glm::radians(5.0f), glm::vec3(1, 0, 0)));
				capsules[5]->setMatrix(glm::rotate(capsules[5]->getMatrix(), glm::radians(1.8f), glm::vec3(1, 0, 0)));
				TunaGE::renderList.clear();
				TunaGE::renderList.pass(root);
			}
			break;
		case 'o':
			root = TunaGE::renderList.getSceneRoot();
			if (root != nullptr) {
				capsules[3]->setMatrix(glm::rotate(capsules[3]->getMatrix(), glm::radians(-1.2f), glm::vec3(1, 0, 0)));
				capsules[4]->setMatrix(glm::rotate(capsules[4]->getMatrix(), glm::radians(-5.0f), glm::vec3(1, 0, 0)));
				capsules[5]->setMatrix(glm::rotate(capsules[5]->getMatrix(), glm::radians(-1.8f), glm::vec3(1, 0, 0)));
				TunaGE::renderList.clear();
				TunaGE::renderList.pass(root);
			}
			break;
		case '0':
			root = TunaGE::renderList.getSceneRoot();
			if (root != nullptr) {
				capsules[0]->setMatrix(glm::rotate(capsules[0]->getMatrix(), glm::radians(1.0f), glm::vec3(1, 0, 0)));
				capsules[1]->setMatrix(glm::rotate(capsules[1]->getMatrix(), glm::radians(5.0f), glm::vec3(1, 0, 0)));
				capsules[2]->setMatrix(glm::rotate(capsules[2]->getMatrix(), glm::radians(1.7f), glm::vec3(1, 0, 0)));
				TunaGE::renderList.clear();
				TunaGE::renderList.pass(root);
			}
			break;
		case 'p':
			root = TunaGE::renderList.getSceneRoot();
			if (root != nullptr) {
				capsules[0]->setMatrix(glm::rotate(capsules[0]->getMatrix(), glm::radians(-1.0f), glm::vec3(1, 0, 0)));
				capsules[1]->setMatrix(glm::rotate(capsules[1]->getMatrix(), glm::radians(-5.0f), glm::vec3(1, 0, 0)));
				capsules[2]->setMatrix(glm::rotate(capsules[2]->getMatrix(), glm::radians(-1.7f), glm::vec3(1, 0, 0)));
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
	camera1.setCameraSpeed(2);

	camera2.setCameraPos(glm::vec3(-0.0, 20.0f, -20.0f));
	camera2.setCameraFront(glm::vec3(0.01f, 0.0f, 0.9f));
	camera2.setCameraUp(glm::vec3(0.0f, 1.0f, 0.0f));
	camera2.setCameraSpeed(2);

    Node* rootest;

#if _WINDOWS
    rootest = TunaGE::loadOVO("../tuna-ge/assets/scenes/gauntlet.OVO");
#else
    rootest = TunaGE::loadOVO("../../tuna-ge/assets/scenes/gauntlet.ovo");
#endif

    rootest->link(&camera1);

    TunaGE::renderList.pass(rootest);
	TunaGE::makeReflective(rootest->getSceneElementByName("Plane001"));


    TunaGE::setFrameRate(true);

	std::cout << "Library Version: " << TunaGE::version() << std::endl;
	std::cout << "GUI Version: " << version() << std::endl;
	TunaGE::loop();
}