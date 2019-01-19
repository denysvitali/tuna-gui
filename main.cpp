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

std::vector<Node*> capsules;
Node* cylinder = nullptr;
Node* box = nullptr;
Camera* camera1 = nullptr;
Camera* camera2 = nullptr;

bool lightning = true;
bool wireframe = false;
bool debug = false;

/////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////CALLBACKS///////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////

bool firstMouse = true;
double lastX = 0;
double lastY = 0;

float angleX = 0.0;

bool rotationEnabled = true;
glm::mat4 cylinder_initialMat;

double cameraPosX = 0.0;
double cameraPosY = 0.0;
double cameraPosZ = 0.0;
double cameraDistance = 200;

void loopCallback(){
	if(rotationEnabled){
		glm::vec4 v4 = camera1->getLookAtNode()->getRenderMatrix() * glm::vec4(0,0,0,1);

		cameraPosX = v4.x;
		cameraPosY = v4.y;
		cameraPosZ = v4.z;
		camera1->setPos(glm::vec3(cameraPosX + cameraDistance * glm::sin(glm::radians(angleX)),  cameraPosY + 20, cameraPosZ + cameraDistance * glm::cos(glm::radians(angleX))));
		angleX+= 2;
	}
}

void printSceneHierarchy(Node* node, int spacing = 0){
	if(node == nullptr){
		return;
	}

	for(int i=0; i<spacing; i++){
		std::cout << "\t";
	}
	std::cout << node->getName().data() << std::endl;

	if(node->getChildren().size() != 0){
		for(auto& c : node->getChildren()){
			printSceneHierarchy(c, spacing+1);
		}
	}
}
//	Callback used to set a flag on mouse release
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
//	Callbacks used to bind the mouse movements to the camera yaw and pitch movements
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

	glm::vec3 front = TunaGE::getCurrentCamera()->getFront();

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

	TunaGE::getCurrentCamera()->setFront(front);
	TunaGE::redisplay();
}

//	Callback used to move the camera, move the fingers of the gauntlet and toggle some settings via keyboard
void kbdCB(unsigned char c, int mouseX, int mouseY) {
	Camera* camera = TunaGE::getCurrentCamera();
	float cameraSpeed = 4.0f;
	Node* root;
	float i;
	glm::vec3 posBefore;
	glm::vec3 relPos;
	glm::vec3 posAfter;
    switch (c) {
        case 'w':
			posBefore = camera->getPos();
            camera->setPos(camera->getPos() + (cameraSpeed * camera->getFront()));
			relPos = camera->getRelativePosition();
			posAfter = camera->getPos();
			if (relPos[1] < 1) {
				posAfter = glm::vec3(posAfter[0], posBefore[1], posAfter[2]);
				camera->setPos(posAfter);
			}
			if (relPos[1] > 400) {
				posAfter = glm::vec3(posAfter[0], posBefore[1], posAfter[2]);
				camera->setPos(posAfter);
			}
			if (relPos[2] > 200) {
				posAfter = glm::vec3(posAfter[0], posAfter[1], posBefore[2]);
				camera->setPos(posAfter);
			}
			if (relPos[2] < -200) {
				posAfter = glm::vec3(posAfter[0], posAfter[1], posBefore[2]);
				camera->setPos(posAfter);
			}
			if (relPos[0] > 200) {
				posAfter = glm::vec3(posBefore[0], posAfter[1], posAfter[2]);
				camera->setPos(posAfter);
			}
			if (relPos[0] < -200) {
				posAfter = glm::vec3(posBefore[0], posAfter[1], posAfter[2]);
				camera->setPos(posAfter);
			}

            break;
        case 's':
			posBefore = camera->getPos();
            camera->setPos(camera->getPos() - (cameraSpeed * camera->getFront()));
			relPos = camera->getRelativePosition();
			posAfter = camera->getPos();
			if (relPos[1] < 1) {
				posAfter = glm::vec3(posAfter[0], posBefore[1], posAfter[2]);
				camera->setPos(posAfter);
			}
			if (relPos[1] > 400) {
				posAfter = glm::vec3(posAfter[0], posBefore[1], posAfter[2]);
				camera->setPos(posAfter);
			}
			if (relPos[2] > 200) {
				posAfter = glm::vec3(posAfter[0], posAfter[1], posBefore[2]);
				camera->setPos(posAfter);
			}
			if (relPos[2] < -200) {
				posAfter = glm::vec3(posAfter[0], posAfter[1], posBefore[2]);
				camera->setPos(posAfter);
			}
			if (relPos[0] > 200) {
				posAfter = glm::vec3(posBefore[0], posAfter[1], posAfter[2]);
				camera->setPos(posAfter);
			}
			if (relPos[0] < -200) {
				posAfter = glm::vec3(posBefore[0], posAfter[1], posAfter[2]);
				camera->setPos(posAfter);
			}
            break;
        case 'd':
			posBefore = camera->getPos();
            camera->setPos(camera->getPos() + (glm::normalize(glm::cross(camera->getFront(), camera->getUp())) * cameraSpeed));
			relPos = camera->getRelativePosition();
			posAfter = camera->getPos();
			if (relPos[1] < 1) {
				posAfter = glm::vec3(posAfter[0], posBefore[1], posAfter[2]);
				camera->setPos(posAfter);
			}
			if (relPos[1] > 400) {
				posAfter = glm::vec3(posAfter[0], posBefore[1], posAfter[2]);
				camera->setPos(posAfter);
			}
			if (relPos[2] > 200) {
				posAfter = glm::vec3(posAfter[0], posAfter[1], posBefore[2]);
				camera->setPos(posAfter);
			}
			if (relPos[2] < -200) {
				posAfter = glm::vec3(posAfter[0], posAfter[1], posBefore[2]);
				camera->setPos(posAfter);
			}
			if (relPos[0] > 200) {
				posAfter = glm::vec3(posBefore[0], posAfter[1], posAfter[2]);
				camera->setPos(posAfter);
			}
			if (relPos[0] < -200) {
				posAfter = glm::vec3(posBefore[0], posAfter[1], posAfter[2]);
				camera->setPos(posAfter);
			}
            break;
        case 'a':
			posBefore = camera->getPos();
            camera->setPos(camera->getPos() - (glm::normalize(glm::cross(camera->getFront(), camera->getUp())) * cameraSpeed));
			relPos = camera->getRelativePosition();
			posAfter = camera->getPos();
			if (relPos[1] < 1) {
				posAfter = glm::vec3(posAfter[0], posBefore[1], posAfter[2]);
				camera->setPos(posAfter);
			}
			if (relPos[1] > 400) {
				posAfter = glm::vec3(posAfter[0], posBefore[1], posAfter[2]);
				camera->setPos(posAfter);
			}
			if (relPos[2] > 200) {
				posAfter = glm::vec3(posAfter[0], posAfter[1], posBefore[2]);
				camera->setPos(posAfter);
			}
			if (relPos[2] < -200) {
				posAfter = glm::vec3(posAfter[0], posAfter[1], posBefore[2]);
				camera->setPos(posAfter);
			}
			if (relPos[0] > 200) {
				posAfter = glm::vec3(posBefore[0], posAfter[1], posAfter[2]);
				camera->setPos(posAfter);
			}
			if (relPos[0] < -200) {
				posAfter = glm::vec3(posBefore[0], posAfter[1], posAfter[2]);
				camera->setPos(posAfter);
			}
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
				TunaGE::renderList.clearRenderElements();
				TunaGE::renderList.pass(root);
			}
			break;
		case 'z':
			root = TunaGE::renderList.getSceneRoot();
			if (root != nullptr) {
				capsules[12]->setMatrix(glm::rotate(capsules[12]->getMatrix(), glm::radians(-1.0f), glm::vec3(1, 0, 0)));
				capsules[12]->setMatrix(glm::rotate(capsules[12]->getMatrix(), glm::radians(0.5f), glm::vec3(0, 1, 0)));
				capsules[13]->setMatrix(glm::rotate(capsules[13]->getMatrix(), glm::radians(-1.0f), glm::vec3(1, 0, 0)));
				TunaGE::renderList.clearRenderElements();
				TunaGE::renderList.pass(root);
			}
			break;
		case '7':
			root = TunaGE::renderList.getSceneRoot();
			if (root != nullptr) {
				capsules[9]->setMatrix(glm::rotate(capsules[9]->getMatrix(), glm::radians(1.5f), glm::vec3(1, 0, 0)));
				capsules[10]->setMatrix(glm::rotate(capsules[10]->getMatrix(), glm::radians(5.0f), glm::vec3(1, 0, 0)));
				capsules[11]->setMatrix(glm::rotate(capsules[11]->getMatrix(), glm::radians(2.0f), glm::vec3(1, 0, 0)));
				TunaGE::renderList.clearRenderElements();
				TunaGE::renderList.pass(root);
			}
			break;
		case 'u':
			root = TunaGE::renderList.getSceneRoot();
			if (root != nullptr) {
				capsules[9]->setMatrix(glm::rotate(capsules[9]->getMatrix(), glm::radians(-1.5f), glm::vec3(1, 0, 0)));
				capsules[10]->setMatrix(glm::rotate(capsules[10]->getMatrix(), glm::radians(-5.0f), glm::vec3(1, 0, 0)));
				capsules[11]->setMatrix(glm::rotate(capsules[11]->getMatrix(), glm::radians(-2.0f), glm::vec3(1, 0, 0)));
				TunaGE::renderList.clearRenderElements();
				TunaGE::renderList.pass(root);
			}
			break;
		case '8':
			root = TunaGE::renderList.getSceneRoot();
			if (root != nullptr) {
				capsules[6]->setMatrix(glm::rotate(capsules[6]->getMatrix(), glm::radians(1.5f), glm::vec3(1, 0, 0)));
				capsules[7]->setMatrix(glm::rotate(capsules[7]->getMatrix(), glm::radians(5.0f), glm::vec3(1, 0, 0)));
				capsules[8]->setMatrix(glm::rotate(capsules[8]->getMatrix(), glm::radians(2.0f), glm::vec3(1, 0, 0)));
				TunaGE::renderList.clearRenderElements();
				TunaGE::renderList.pass(root);
			}
			break;
		case 'i':
			root = TunaGE::renderList.getSceneRoot();
			if (root != nullptr) {
				capsules[6]->setMatrix(glm::rotate(capsules[6]->getMatrix(), glm::radians(-1.5f), glm::vec3(1, 0, 0)));
				capsules[7]->setMatrix(glm::rotate(capsules[7]->getMatrix(), glm::radians(-5.0f), glm::vec3(1, 0, 0)));
				capsules[8]->setMatrix(glm::rotate(capsules[8]->getMatrix(), glm::radians(-2.0f), glm::vec3(1, 0, 0)));
				TunaGE::renderList.clearRenderElements();
				TunaGE::renderList.pass(root);
			}
			break;
		case '9':
			root = TunaGE::renderList.getSceneRoot();
			if (root != nullptr) {
				capsules[3]->setMatrix(glm::rotate(capsules[3]->getMatrix(), glm::radians(1.2f), glm::vec3(1, 0, 0)));
				capsules[4]->setMatrix(glm::rotate(capsules[4]->getMatrix(), glm::radians(5.0f), glm::vec3(1, 0, 0)));
				capsules[5]->setMatrix(glm::rotate(capsules[5]->getMatrix(), glm::radians(1.8f), glm::vec3(1, 0, 0)));
				TunaGE::renderList.clearRenderElements();
				TunaGE::renderList.pass(root);
			}
			break;
		case 'o':
			root = TunaGE::renderList.getSceneRoot();
			if (root != nullptr) {
				capsules[3]->setMatrix(glm::rotate(capsules[3]->getMatrix(), glm::radians(-1.2f), glm::vec3(1, 0, 0)));
				capsules[4]->setMatrix(glm::rotate(capsules[4]->getMatrix(), glm::radians(-5.0f), glm::vec3(1, 0, 0)));
				capsules[5]->setMatrix(glm::rotate(capsules[5]->getMatrix(), glm::radians(-1.8f), glm::vec3(1, 0, 0)));
				TunaGE::renderList.clearRenderElements();
				TunaGE::renderList.pass(root);
			}
			break;
		case '0':
			root = TunaGE::renderList.getSceneRoot();
			if (root != nullptr) {
				capsules[0]->setMatrix(glm::rotate(capsules[0]->getMatrix(), glm::radians(1.2f), glm::vec3(1, 0, 0)));
				capsules[1]->setMatrix(glm::rotate(capsules[1]->getMatrix(), glm::radians(5.0f), glm::vec3(1, 0, 0)));
				capsules[2]->setMatrix(glm::rotate(capsules[2]->getMatrix(), glm::radians(1.7f), glm::vec3(1, 0, 0)));
				TunaGE::renderList.clearRenderElements();
				TunaGE::renderList.pass(root);
			}
			break;
		case 'p':
			root = TunaGE::renderList.getSceneRoot();
			if (root != nullptr) {
				capsules[0]->setMatrix(glm::rotate(capsules[0]->getMatrix(), glm::radians(-1.2f), glm::vec3(1, 0, 0)));
				capsules[1]->setMatrix(glm::rotate(capsules[1]->getMatrix(), glm::radians(-5.0f), glm::vec3(1, 0, 0)));
				capsules[2]->setMatrix(glm::rotate(capsules[2]->getMatrix(), glm::radians(-1.7f), glm::vec3(1, 0, 0)));
				TunaGE::renderList.clearRenderElements();
				TunaGE::renderList.pass(root);
			}
			break;

    	case 'j':
    		debug = !debug;
    		TunaGE::setDebug(debug);
    		break;

		case 'k':
			lightning = !lightning;
			TunaGE::setLightning(lightning);
			break;
		case 'l':
			wireframe = !wireframe;
			TunaGE::setWireframe(wireframe);
			break;

		case 'v':
			root = TunaGE::renderList.getSceneRoot();
			root->getSceneElementByName("Omni001")->setMatrix(glm::translate(root->getSceneElementByName("Omni001")->getMatrix(), glm::vec3(-2, 0, 0)));
			TunaGE::renderList.clearRenderElements();
			TunaGE::renderList.pass(root);
			break;
		case 'b':
			root = TunaGE::renderList.getSceneRoot();
			root->getSceneElementByName("Omni001")->setMatrix(glm::translate(root->getSceneElementByName("Omni001")->getMatrix(), glm::vec3(2, 0, 0)));
			TunaGE::renderList.clearRenderElements();
			TunaGE::renderList.pass(root);
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

	//	Setting Callbacks
	TunaGE::setMotionCallback(motionCallback);
	TunaGE::setMouseCallback(mouseCallback);
	TunaGE::setKeyboardCallback(kbdCB);

	//	Init engine
	TunaGE::init();

	//	Creating cameras
	camera1 = new Camera("camera 1");
	camera2 = new Camera("camera 2");
	camera1->setPos(glm::vec3(142.0f, 135.0f, 0.0f));
	camera1->setFront(glm::vec3(-1.0f, -0.30f, 0.01f));
	camera1->setUp(glm::vec3(0.0f, 1.0f, 0.0f));
	camera1->setFarPlane(1000);
	camera2->setPos(glm::vec3(0.0, 20.0f, 30.0f));
	camera2->setFront(glm::vec3(0.0f, 0.0f, -1.0f));
	camera2->setUp(glm::vec3(0.0f, 1.0f, 0.0f));
	camera2->setFarPlane(1000);

    Node* root;
#if _WINDOWS
    root = TunaGE::loadOVO("../../tuna-ge/assets/scenes/gauntletTex.ovo");
#else
    root = TunaGE::loadOVO("../../tuna-ge/assets/scenes/gauntletTex.ovo");
#endif

    if(root == nullptr){
    	std::cerr << "Unable to open the scene file!" << std::endl;
    	exit(1);
    }

	//	Add cameras to the scene

	printSceneHierarchy(root);


    root->link(camera1);
	root->getSceneElementByName("Omni001")->link(camera2);

	//	Set the mirror flag on the Cylinder supporting the gauntlet, this will make it and all his subnodes mirror at y=0
	cylinder = root->getSceneElementByName("Cylinder001");
	cylinder->setFlipScene(true);
	cylinder_initialMat = cylinder->getMatrix();

	box = root->getSceneElementByName("Box001");

	TunaGE::renderList.pass(root);

	for(int i=1; i<=15; i++){
			std::stringstream cSS{};
			cSS << "Capsule" << std::setfill('0') << std::setw(3) << i;
			capsules.push_back(root->getSceneElementByName(cSS.str().data()));
	}

	camera1->lookAt(box);

	glm::vec4 v4 = camera1->getLookAtNode()->getRenderMatrix() * glm::vec4(0,0,0,1);

	cameraPosX = v4.x;
	cameraPosY = v4.y;
	cameraPosZ = v4.z;

	camera1->setPos(glm::vec3(cameraPosX, cameraPosY, cameraPosZ));
	camera1->setMode(CameraMode::LOOK_AT_NODE);

	//	Show version
	std::cout << "Library Version: " << TunaGE::version().data() << std::endl;
	std::cout << "GUI Version: " << version() << std::endl;

	TunaGE::setLoopCallback(loopCallback);


	TunaGE::setLightning(lightning);
	TunaGE::setWireframe(wireframe);
	TunaGE::setDebug(debug);

	//	Enter main loop
	TunaGE::loop();
	//	Deallocate resources and the scene
	TunaGE::free();
}