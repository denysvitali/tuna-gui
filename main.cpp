#include <iostream>
#include <string>
#include <sstream>
#include <cstring>
#include <iomanip>
#include "Leap.h"

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
Camera* mainCamera = nullptr;

bool lightning = true;
bool culling = true;
bool wireframe = false;
bool debug = false;
bool fpsCounter = false;

/////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////CALLBACKS///////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////

bool firstMouse = true;
double lastX = 0;
double lastY = 0;

float angleX = 0.0;

bool rotationEnabled = false;
glm::mat4 cylinder_initialMat;

double cameraPosX = 0.0;
double cameraPosY = 0.0;
double cameraPosZ = 0.0;
double cameraDistance = 180;
double cameraDistance_Y = 20;

RGBColor textColor = RGBColor(255, 0, 0);

void loopCallback(){
	if(rotationEnabled){
		/*glm::vec4 v4 = camera->getLookAtNode()->getRenderMatrix() * glm::vec4(0,0,0,1);

		cameraPosX = v4.x;
		cameraPosY = v4.y;
		cameraPosZ = v4.z;
		camera->setPos(glm::vec3(cameraPosX + cameraDistance * glm::sin(glm::radians(angleX)),
				cameraPosY + cameraDistance_Y,
				cameraPosZ + cameraDistance * glm::cos(glm::radians(angleX))));
		angleX+= 2;*/
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
void mouseCallback(Mouse::Button button, Button::State state, int mouseX, int mouseY) {

	switch (state)
	{
		case Button::State::UP:
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
	float cameraSpeed = 0.3f;
	Node* root;
	float i;
	glm::vec3 posBefore;
	glm::vec3 absPos;
	glm::vec3 posAfter;
	glm::vec3 absPosLight;
	switch (c) {
    	case 'r':
			rotationEnabled = !rotationEnabled;
    		break;
        case 'w':
			rotationEnabled = false;
			posBefore = camera->getPos();
            camera->setPos(camera->getPos() + (cameraSpeed * camera->getAdditionalFront()));
			absPos = camera->getAbsolutePosition();
			posAfter = camera->getPos();
			
            break;
        case 's':
			rotationEnabled = false;
			posBefore = camera->getPos();
            camera->setPos(camera->getPos() - (cameraSpeed * camera->getAdditionalFront()));
			absPos = camera->getAbsolutePosition();
			posAfter = camera->getPos();
			
            break;
        case 'd':
			rotationEnabled = false;
			posBefore = camera->getPos();
            camera->setPos(camera->getPos() + (glm::normalize(glm::cross(camera->getAdditionalFront(), camera->getUp())) * cameraSpeed));
			absPos = camera->getAbsolutePosition();
			posAfter = camera->getPos();
			
            break;
        case 'a':
			rotationEnabled = false;
			posBefore = camera->getPos();
            camera->setPos(camera->getPos() - (glm::normalize(glm::cross(camera->getAdditionalFront(), camera->getUp())) * cameraSpeed));
			absPos = camera->getAbsolutePosition();
			posAfter = camera->getPos();
	
            break;
		case 'c':
			TunaGE::renderList.switchCamera();
			break;
		case '6':

            capsules[12]->setMatrix(glm::rotate(capsules[12]->getMatrix(), glm::radians(1.0f), glm::vec3(1, 0, 0)));
            capsules[12]->setMatrix(glm::rotate(capsules[12]->getMatrix(), glm::radians(-0.5f), glm::vec3(0, 1, 0)));
            capsules[13]->setMatrix(glm::rotate(capsules[13]->getMatrix(), glm::radians(1.0f), glm::vec3(1, 0, 0)));
			break;
		case 'z':

            capsules[12]->setMatrix(glm::rotate(capsules[12]->getMatrix(), glm::radians(-1.0f), glm::vec3(1, 0, 0)));
            capsules[12]->setMatrix(glm::rotate(capsules[12]->getMatrix(), glm::radians(0.5f), glm::vec3(0, 1, 0)));
            capsules[13]->setMatrix(glm::rotate(capsules[13]->getMatrix(), glm::radians(-1.0f), glm::vec3(1, 0, 0)));
			break;
		case '7':

            capsules[9]->setMatrix(glm::rotate(capsules[9]->getMatrix(), glm::radians(1.5f), glm::vec3(1, 0, 0)));
            capsules[10]->setMatrix(glm::rotate(capsules[10]->getMatrix(), glm::radians(5.0f), glm::vec3(1, 0, 0)));
            capsules[11]->setMatrix(glm::rotate(capsules[11]->getMatrix(), glm::radians(2.0f), glm::vec3(1, 0, 0)));
			break;
		case 'u':

            capsules[9]->setMatrix(glm::rotate(capsules[9]->getMatrix(), glm::radians(-1.5f), glm::vec3(1, 0, 0)));
            capsules[10]->setMatrix(glm::rotate(capsules[10]->getMatrix(), glm::radians(-5.0f), glm::vec3(1, 0, 0)));
            capsules[11]->setMatrix(glm::rotate(capsules[11]->getMatrix(), glm::radians(-2.0f), glm::vec3(1, 0, 0)));
			break;
		case '8':

            capsules[6]->setMatrix(glm::rotate(capsules[6]->getMatrix(), glm::radians(1.5f), glm::vec3(1, 0, 0)));
            capsules[7]->setMatrix(glm::rotate(capsules[7]->getMatrix(), glm::radians(5.0f), glm::vec3(1, 0, 0)));
            capsules[8]->setMatrix(glm::rotate(capsules[8]->getMatrix(), glm::radians(2.0f), glm::vec3(1, 0, 0)));
			break;
		case 'i':

            capsules[6]->setMatrix(glm::rotate(capsules[6]->getMatrix(), glm::radians(-1.5f), glm::vec3(1, 0, 0)));
            capsules[7]->setMatrix(glm::rotate(capsules[7]->getMatrix(), glm::radians(-5.0f), glm::vec3(1, 0, 0)));
            capsules[8]->setMatrix(glm::rotate(capsules[8]->getMatrix(), glm::radians(-2.0f), glm::vec3(1, 0, 0)));
			break;
		case '9':

            capsules[3]->setMatrix(glm::rotate(capsules[3]->getMatrix(), glm::radians(1.2f), glm::vec3(1, 0, 0)));
            capsules[4]->setMatrix(glm::rotate(capsules[4]->getMatrix(), glm::radians(5.0f), glm::vec3(1, 0, 0)));
            capsules[5]->setMatrix(glm::rotate(capsules[5]->getMatrix(), glm::radians(1.8f), glm::vec3(1, 0, 0)));
			break;
		case 'o':

            capsules[3]->setMatrix(glm::rotate(capsules[3]->getMatrix(), glm::radians(-1.2f), glm::vec3(1, 0, 0)));
            capsules[4]->setMatrix(glm::rotate(capsules[4]->getMatrix(), glm::radians(-5.0f), glm::vec3(1, 0, 0)));
            capsules[5]->setMatrix(glm::rotate(capsules[5]->getMatrix(), glm::radians(-1.8f), glm::vec3(1, 0, 0)));
			break;
		case '0':

            capsules[0]->setMatrix(glm::rotate(capsules[0]->getMatrix(), glm::radians(1.2f), glm::vec3(1, 0, 0)));
            capsules[1]->setMatrix(glm::rotate(capsules[1]->getMatrix(), glm::radians(5.0f), glm::vec3(1, 0, 0)));
            capsules[2]->setMatrix(glm::rotate(capsules[2]->getMatrix(), glm::radians(1.7f), glm::vec3(1, 0, 0)));
			break;
		case 'p':

            capsules[0]->setMatrix(glm::rotate(capsules[0]->getMatrix(), glm::radians(-1.2f), glm::vec3(1, 0, 0)));
            capsules[1]->setMatrix(glm::rotate(capsules[1]->getMatrix(), glm::radians(-5.0f), glm::vec3(1, 0, 0)));
            capsules[2]->setMatrix(glm::rotate(capsules[2]->getMatrix(), glm::radians(-1.7f), glm::vec3(1, 0, 0)));
			break;

		case 'g':
			culling = !culling;
			TunaGE::enableCulling(culling);
			break;

    	case 'h':
    		fpsCounter = !fpsCounter;
    		TunaGE::setFPSCounter(fpsCounter);
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
		
		default:
			break;
    }
    TunaGE::redisplay();
}

/////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////

// Cursor:
int mouseX, mouseY;
int scrollX, scrollY;
float hX, hY, hZ;

// Hand settings:
glm::mat4 handMatrix;
float scalingFactor = 2.0f;
const int nrOfCapsules = 16;      ///< The hand model is made of N capsules
glm::mat3 basePose[nrOfCapsules];   ///< Hand initial, neutral pose
Node *hand = nullptr;
Node *spheres[6];
Leap::FingerList fingers;
glm::vec4 leapPos = glm::vec4(-1,-1,-1,-1);

int checkFinger(int fingerIndex, int sphereIndex) {

	glm::vec3 knuckeJointPos = glm::vec3(fingers[fingerIndex].jointPosition(Leap::Finger::Joint::JOINT_DIP).x, fingers[fingerIndex].jointPosition(Leap::Finger::Joint::JOINT_DIP).y, fingers[fingerIndex].jointPosition(Leap::Finger::Joint::JOINT_DIP).z);
	glm::vec3 mediumJointPos = glm::vec3(fingers[fingerIndex].jointPosition(Leap::Finger::Joint::JOINT_MCP).x, fingers[fingerIndex].jointPosition(Leap::Finger::Joint::JOINT_MCP).y, fingers[fingerIndex].jointPosition(Leap::Finger::Joint::JOINT_MCP).z);
	glm::vec3 tipJointPos = glm::vec3(fingers[fingerIndex].jointPosition(Leap::Finger::Joint::JOINT_PIP).x, fingers[fingerIndex].jointPosition(Leap::Finger::Joint::JOINT_PIP).y, fingers[fingerIndex].jointPosition(Leap::Finger::Joint::JOINT_PIP).z);
	float dot = glm::dot(mediumJointPos - knuckeJointPos, tipJointPos - knuckeJointPos);
	float angle = glm::acos(dot / (glm::length(mediumJointPos - knuckeJointPos) * glm::length(tipJointPos - knuckeJointPos)));

	if (angle < 0.25) {
		hand->getSceneElementByName("Box001")->link(spheres[sphereIndex]);
		return 1;
	}
	else {
		spheres[sphereIndex]->unlink();
		if (angle > 0.75)
			return -1;
		return 0;
	}

}

class SampleListener : public Leap::Listener
{
	//////////
public: //
//////////

   /**
	* Get the final rotation matrix for a given segment.
	* @param hand Leap Motion hand object
	* @param fingers Leap Motion finger list object
	* @param index index of the segment
	* @param segment segment number
	* @return rotational (3x3) matrix for the given segment
	*/
	glm::mat3 segFinalRotation(const Leap::Hand &hand, const Leap::FingerList &fingers, int index, int segment)
	{
		// Node matrix:
		Leap::Matrix lpFingerMatrix;
		glm::mat3 fingerMatrix;
		lpFingerMatrix = fingers[index].bone((Leap::Bone::Type) segment).basis();
		memcpy(&fingerMatrix, &lpFingerMatrix.toArray3x3(), sizeof(glm::mat3));

		// Get parent matrix:
		Leap::Matrix lpParentMatrix;
		glm::mat3 parentMatrix;
		if (segment > 0)
			lpParentMatrix = fingers[index].bone((Leap::Bone::Type) (segment - 1)).basis();
		else
			lpParentMatrix = hand.basis();
		memcpy(&parentMatrix, &lpParentMatrix.toArray3x3(), sizeof(glm::mat3));
		parentMatrix = glm::inverse(parentMatrix);

		// Return matrix:
		return glm::inverse(parentMatrix * fingerMatrix);
	}

	/**
	 * Leap Motion update methods invoked when a hand is over the sensor.
	 * @param controller Leap Motion controller object
	 */
	void SampleListener::onFrame(const Leap::Controller &controller)
	{
		// Get the most recent frame and report some basic information:
		const Leap::Frame frame = controller.frame();
		Leap::HandList hands = frame.hands();

		// Iterate through available hands (if any):
		for (Leap::HandList::const_iterator hl = hands.begin(); hl != hands.end(); ++hl)
		{
			// Get the right hand only:
			const Leap::Hand rhand = *hl;
			if (rhand.isLeft())
				continue;

			// Hand position:
			hX = (rhand.palmPosition().x / scalingFactor);
			hY = (rhand.palmPosition().y / scalingFactor) - 40.0f;
			hZ = (rhand.palmPosition().z / scalingFactor);
		
			Leap::Matrix m = rhand.basis();
			memcpy(&handMatrix, &m.toArray4x4(), sizeof(glm::mat4));
			Leap::Vector wvec = rhand.palmPosition();
			handMatrix[3] = glm::vec4(wvec.x / scalingFactor, wvec.y / scalingFactor - 70.0f, wvec.z / scalingFactor, 1.0f); // Some harcoded offsets

			fingers = rhand.fingers();

			if(leapPos.w == -1)
				leapPos = glm::vec4(rhand.wristPosition().x, rhand.wristPosition().y, rhand.wristPosition().z, 1);

			glm::vec4 wristPos = (leapPos - glm::vec4(rhand.wristPosition().x, rhand.wristPosition().y, rhand.wristPosition().z, 1)) * 0.5f;
			wristPos.y *= -1;
			leapPos = glm::vec4(rhand.wristPosition().x, rhand.wristPosition().y, rhand.wristPosition().z, 1);
			
			int fingerSum = 0;
			fingerSum += checkFinger(1, 4);
			fingerSum += checkFinger(2, 3);
			fingerSum += checkFinger(3, 2);
			fingerSum += checkFinger(4, 1);

			handMatrix = glm::rotate(handMatrix,glm::radians(90.0f),glm::vec3(1,0,0));
			handMatrix = glm::rotate(handMatrix, glm::radians(180.0f), glm::vec3(0, 0, 1));
			
			if (fingerSum == 4) {
				hand->getSceneElementByName("Box001")->link(spheres[0]);
				hand->getSceneElementByName("Cylinder001")->setMatrix(handMatrix);
			}
			else {
				spheres[0]->unlink();
				if (fingerSum == -4) {
					TunaGE::getCurrentCamera()->setPos(TunaGE::getCurrentCamera()->getPos() + glm::vec3(wristPos*0.03f));
				}
				else {
					hand->getSceneElementByName("Cylinder001")->setMatrix(handMatrix);
				}
				
			}

			// Thumb:
			hand->getSceneElementByName("Capsule013")->setMatrix(basePose[13] * segFinalRotation(rhand, fingers, 0, Leap::Bone::TYPE_PROXIMAL));
			hand->getSceneElementByName("Capsule014")->setMatrix(basePose[14] * segFinalRotation(rhand, fingers, 0, Leap::Bone::TYPE_INTERMEDIATE));
			hand->getSceneElementByName("Capsule015")->setMatrix(basePose[15] * segFinalRotation(rhand, fingers, 0, Leap::Bone::TYPE_DISTAL));
	
			// Pinky:
			hand->getSceneElementByName("Capsule001")->setMatrix(basePose[1] * segFinalRotation(rhand, fingers, 4, Leap::Bone::TYPE_PROXIMAL));
			hand->getSceneElementByName("Capsule002")->setMatrix(basePose[2] * segFinalRotation(rhand, fingers, 4, Leap::Bone::TYPE_INTERMEDIATE));
			hand->getSceneElementByName("Capsule003")->setMatrix(basePose[3] * segFinalRotation(rhand, fingers, 4, Leap::Bone::TYPE_DISTAL));

			// Ring:
			hand->getSceneElementByName("Capsule004")->setMatrix(basePose[4] * segFinalRotation(rhand, fingers, 3, Leap::Bone::TYPE_PROXIMAL));
			hand->getSceneElementByName("Capsule005")->setMatrix(basePose[5] * segFinalRotation(rhand, fingers, 3, Leap::Bone::TYPE_INTERMEDIATE));
			hand->getSceneElementByName("Capsule006")->setMatrix(basePose[6] * segFinalRotation(rhand, fingers, 3, Leap::Bone::TYPE_DISTAL));

			// Middle:
			hand->getSceneElementByName("Capsule007")->setMatrix(basePose[7] * segFinalRotation(rhand, fingers, 2, Leap::Bone::TYPE_PROXIMAL));
			hand->getSceneElementByName("Capsule008")->setMatrix(basePose[8] * segFinalRotation(rhand, fingers, 2, Leap::Bone::TYPE_INTERMEDIATE));
			hand->getSceneElementByName("Capsule009")->setMatrix(basePose[9] * segFinalRotation(rhand, fingers, 2, Leap::Bone::TYPE_DISTAL));

			// Index:
			hand->getSceneElementByName("Capsule010")->setMatrix(basePose[10] * segFinalRotation(rhand, fingers, 1, Leap::Bone::TYPE_PROXIMAL));
			hand->getSceneElementByName("Capsule011")->setMatrix(basePose[11] * segFinalRotation(rhand, fingers, 1, Leap::Bone::TYPE_INTERMEDIATE));
			hand->getSceneElementByName("Capsule012")->setMatrix(basePose[12] * segFinalRotation(rhand, fingers, 1, Leap::Bone::TYPE_DISTAL));
		}
	}
};

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
	
	mainCamera = new Camera("mainCamera");
	
	mainCamera->setPos(glm::vec3(0.0f, 0.0f, 0.0f));
	mainCamera->setFront(glm::vec3(0.0f, 0.0f, -1.0f));
	mainCamera->setUp(glm::vec3(0.0f, 1.0f, 0.0f));
	mainCamera->setFarPlane(1000);

    Node* root;

    std::string sceneName = "Thumb.OVO";
    std::string path;

#if _WINDOWS
	path = "../../tuna-ge/assets/scenes/";
#else
	path = "../../tuna-ge/assets/scenes/";
#endif

	path = path + sceneName;
	root = TunaGE::loadOVO(path.data());
	Node* newNode = new Node();

	if(root == nullptr){
    	std::cerr << "Unable to open the scene file!" << std::endl;
		TunaGE::free();
    	exit(1);
    }
	newNode->link(root);
	
	root->getSceneElementByName("Group001")->setFlipScene(true);
	//Node* divano = root->getSceneElementByName("Group001")->unlink();
	root->getSceneElementByName("Plane002")->unlink();
	root->getSceneElementByName("Plane003")->unlink();
	root->getSceneElementByName("Plane004")->unlink();
	root->getSceneElementByName("Plane005")->unlink();
	root->getSceneElementByName("Plane006")->unlink();

	//((Mesh*)root->getSceneElementByName("Plane001"))->getMaterial()->setAlpha(0.2);

	printSceneHierarchy(root);

	root->link(mainCamera);

	//	Set the mirror flag on the Cylinder supporting the gauntlet, this will make it and all his subnodes mirror at y=0
	cylinder = root->getSceneElementByName("Cylinder001");
	cylinder->setFlipScene(false);

	root->getSceneElementByName("Teapot001")->setFlipScene(true);

	TunaGE::renderList.pass(newNode);

	for(int i=1; i<=15; i++){
			std::stringstream cSS{};
			cSS << "Capsule" << std::setfill('0') << std::setw(3) << i;
			capsules.push_back(root->getSceneElementByName(cSS.str().data()));
	}

	for (int i = 1; i <= 6; i++) {
		char name[256];
		sprintf(name, "Sphere00%d", i);
		spheres[i - 1] = root->getSceneElementByName(name);
	}
	// Bind initial pose by name:
	for (int c = 1; c <= 15; c++)
	{
		char name[256];
		if (c < 10)
			sprintf(name, "Capsule00%d", c);
		else
			sprintf(name, "Capsule0%d", c);

		basePose[c] = glm::mat3(1.0);
		for (int d = 0; d < 3; d++)
		{
			glm::vec4 v = root->getSceneElementByName(name)->getMatrix()[d];
			
			basePose[c][d] = glm::vec3(v.x, v.y, v.z);
		}
	}
	
	//	Show version
	std::cout << "Library Version: " << TunaGE::version().data() << std::endl;
	std::cout << "GUI Version: " << version() << std::endl;

	TunaGE::setLoopCallback(loopCallback);
	TunaGE::setFPSCounter(fpsCounter);
	TunaGE::enableCulling(culling);
	TunaGE::setLightning(lightning);
	TunaGE::setWireframe(wireframe);
	TunaGE::setDebug(debug);
	hand = root;
	// Load LeapMotion:
	Leap::Controller leapCtrl;
	SampleListener leapList;

	
	// Have the sample listener receive events from the controller:
	leapCtrl.addListener(leapList);
	

	//	Enter main loop
	TunaGE::loop();
	//	Deallocate resources and the scene
	TunaGE::free();
}