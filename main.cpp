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

int main(int argc, char** argv) {
	for (int i = 0; i < argc; i++) {
		if (strcmp(argv[i], "-v") == 0) {
			doVersion();
			return 0;
		}
	}
	TunaGE::init();
	TunaGE::enableOriginMarker();
	
	TunaGE::camera.setCameraPos(glm::vec3(0.0f, 4.0f, 10.0f));  // Camera is at (0,2,10), in World Space
	TunaGE::camera.setCameraFront(glm::vec3(0.0f, -0.8f, -1.0f)); // looks in the direction
	TunaGE::camera.setCameraUp(glm::vec3(0.0f, 1.0f, 0.0f)); // Head is up (set to 0,-1,0 to look upside-down)
	TunaGE::camera.setCameraSpeed(0.1f);
	TunaGE::camera.updateCamera();

	Mesh mesh{ "plane" };
	Material material{};

	RGBColor color = RGBColor::getColor("#1a1a1a");
	material.setAmbient(color.vec());
	material.setShininess(200);
	material.setSpecular(glm::vec3(0.5f, 0.5f, 0.5f));
	material.setDiffuse(glm::vec3(0.5f, 0.5f, 0.5f));


	Texture tex{"muro"};
	tex.loadFromFile("../../tuna-ge/assets/textures/paving_stones.jpg");
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
	
	Light light{ RGBColor{100, 255, 255}, "Light 1" };
	light.setMatrix(glm::translate(glm::mat4(1.0f), glm::vec3(1.0f, 1.0f, 0.0f)));
	light.setLight(1);
	light.setIntensity(1.0);
	light.setLightAmbient(glm::vec3(1.0f, 1.0f, 1.0f));
	light.setLightDiffuse(glm::vec3(0.4f, 0.4f, 0.4f));
	light.setLightSpecular(glm::vec3(1.0f, 1.0f, 1.0f));
	light.enable();

	mesh.link(&light);

	TunaGE::renderList.pass(root);

	std::cout << "Library Version: " << TunaGE::version() << std::endl;
	std::cout << "GUI Version: " << version() << std::endl;
	TunaGE::loop();
}