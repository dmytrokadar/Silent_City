#include <iostream>
#include <nlohmann/json.hpp>

constexpr char WINDOW_TITLE[] = "PGR: Application Skeleton";

class Properties
{
public:
	std::string skyboxFaces[6];

	Properties() {
		std::vector<std::string> imagesL = {
			"/posx.jpg",
			"/negx.jpg",
			"/posy.jpg",
			"/negy.jpg",
			"/posz.jpg",
			"/negz.jpg"
		};

		images = imagesL;
		reloadProperties();


	}
	~Properties() {

	}

	void loadProperties() {
		// read file
		std::ifstream file("data/settings.json");
		nlohmann::json data = nlohmann::json::parse(file);

		windowWidth = data.value("winW", 800);
		windowHeight = data.value("winH", 600);
		folder = data.value("skyboxF", "Skybox");

		std::cout << "windowWidth: " << windowWidth << std::endl;
		std::cout << "windowHeight: " << windowHeight << std::endl;
		std::cout << "windowHeight: " << folder << std::endl;
	}
	void reloadProperties() {
		loadProperties();
		for (int i = 0; i < 6; i++) {
			skyboxFaces[i] = parentFolder+folder+images[i];
		}
	}

	int getWinH() {
		return windowHeight;
	}
	
	int getWinW() {
		return windowWidth;
	}
private:
	std::vector<std::string> images;
	std::string parentFolder = "data/";
	std::string folder;

	int windowHeight;
	int windowWidth;
};