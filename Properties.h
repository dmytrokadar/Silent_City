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
		try
		{
			std::ifstream file("data/settings.json");
			nlohmann::json data = nlohmann::json::parse(file);

			windowWidth = data.value("winW", 800);
			windowHeight = data.value("winH", 600);
			folder = data.value("skyboxF", "Skybox");
			bloodI = data.value("bloodBanner", "data/PainHud.png");
			bannerI = data.value("banner", "data/gameOver.png");
			
			auto knotsB = data.at("knotsBezier");
			for (auto& knot : knotsB) {
				knotsBezier.emplace_back(knot[0], knot[1], knot[2]);
			}

			auto handlesB = data.at("handlesBezier");
			for (auto& handle : handlesB) {
				handlesBezier.emplace_back(handle[0], handle[1], handle[2]);
			}

			auto knotsC = data.at("handlesCatmull");
			for (auto& knot : knotsC) {
				knotsCatmull.emplace_back(knot[0], knot[1], knot[2]);
			}

			auto handlesC = data.at("handlesCatmull");
			for (auto& handle : handlesC) {
				handlesCatmull.emplace_back(handle[0], handle[1], handle[2]);
			}

			std::cout << "windowWidth: " << windowWidth << std::endl;
			std::cout << "windowHeight: " << windowHeight << std::endl;
			std::cout << "folder: " << folder << std::endl;
		}
		catch (const std::exception& e)
		{
			std::cout << e.what();
		}
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

	std::vector < glm::vec3 >& getKnotsBezier() {
		return knotsBezier;
	}

	std::vector < glm::vec3 >& getHandlesBezier() {
		return handlesBezier;
	}

	std::vector < glm::vec3 >& getKnotsCatmull() {
		return knotsCatmull;
	}

	std::vector < glm::vec3 >& getHandlesCatmull() {
		return handlesCatmull;
	}

	std::string getBannerI() {
		return bannerI;
	}

	std::string getBloodI() {
		return bloodI;
	}
private:
	std::vector<std::string> images;
	std::string parentFolder = "data/";
	std::string folder = "Skybox";

	std::string bloodI = "data/PainHud.png";
	std::string bannerI = "data/gameOver.png";

	std::vector < glm::vec3 > knotsBezier;
	std::vector < glm::vec3 > handlesBezier;
	std::vector < glm::vec3 > knotsCatmull;
	std::vector < glm::vec3 > handlesCatmull;

	int windowHeight;
	int windowWidth;
};