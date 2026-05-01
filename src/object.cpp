#include "includes/object.hpp"
#include "includes/utils.hpp"
#include <limits>

Object::Object(std::string filepath) {
    std::ifstream ObjFile(filepath);
    if (ObjFile.is_open()) {
        std::string line;
        while (std::getline(ObjFile, line)) {
            std::vector<std::string> split_data = split_string(line, ' ');
            if (split_data[0] == "v") {
                this->vertexes.push_back(std::stof(split_data[1]));
                this->vertexes.push_back(std::stof(split_data[2]));
                this->vertexes.push_back(std::stof(split_data[3]));
            } else if (split_data[0] == "vn") {
                this->vertexNormals.push_back(std::stof(split_data[1]));
                this->vertexNormals.push_back(std::stof(split_data[2]));
                this->vertexNormals.push_back(std::stof(split_data[3]));
            } else if (split_data[0] == "vt") {
                this->vertexTexCoords.push_back(std::stof(split_data[1]));
                this->vertexTexCoords.push_back(std::stof(split_data[2]));
                this->vertexTexCoords.push_back(std::stof(split_data[3]));
            } else if (split_data[0] == "f") {
                std::vector<std::string> split_index_1 = split_string(split_data[1], '/');
                this->faces.push_back(std::stoi(split_index_1[0]) - 1);
                this->faces.push_back(std::stoi(split_index_1[1]) - 1);
                this->faces.push_back(std::stoi(split_index_1[2]) - 1);
                std::vector<std::string> split_index_2 = split_string(split_data[2], '/');
                this->faces.push_back(std::stoi(split_index_2[0]) - 1);
                this->faces.push_back(std::stoi(split_index_2[1]) - 1);
                this->faces.push_back(std::stoi(split_index_2[2]) - 1);
                std::vector<std::string> split_index_3 = split_string(split_data[3], '/');
                this->faces.push_back(std::stoi(split_index_3[0]) - 1);
                this->faces.push_back(std::stoi(split_index_3[1]) - 1);
                this->faces.push_back(std::stoi(split_index_3[2]) - 1);

                if (split_data.size() == 5) { // Smoothly handle triangulation of quads, ngons are unhandled
                    this->faces.push_back(std::stoi(split_index_1[0]) - 1);
                    this->faces.push_back(std::stoi(split_index_1[1]) - 1);
                    this->faces.push_back(std::stoi(split_index_1[2]) - 1);
                    
                    this->faces.push_back(std::stoi(split_data[0]) - 1);
                    this->faces.push_back(std::stoi(split_data[1]) - 1);
                    this->faces.push_back(std::stoi(split_data[2]) - 1);

                    std::vector<std::string> split_index_4 = split_string(split_data[3], '/');
                    this->faces.push_back(std::stoi(split_data[0]) - 1);
                    this->faces.push_back(std::stoi(split_data[1]) - 1);
                    this->faces.push_back(std::stoi(split_data[2]) - 1);
                };
            } else if (split_data[0] == "o") {
                this->name = split_data[1];
            };
        };
        ObjFile.close();
        calculateCenter();
    } else {
        std::cerr << "Unable to open file: " << filepath << std::endl;
    };
};

Object::Object(const Object& obj) {
    *this = obj;
};

Object& Object::operator=(const Object& obj) {
    this->vertexes.clear();
    this->vertexes = obj.vertexes;
    this->faces.clear();
    this->faces = obj.faces;
    this->material = obj.material;
    this->name = obj.name;
    return *this;
};

Object::~Object() {
    return;
};

std::vector<float> Object::getVertexes(void) {
    return (this->vertexes);
}

std::vector<float> Object::getVertexNormals(void)
{
    return (this->vertexNormals);
}

std::vector<float> Object::getVertexTexCoords(void)
{
    return (this->vertexTexCoords);
};

std::vector<unsigned int> Object::getFaces(void) {
    return (this->faces);
}

const std::array<float, 3> Object::getCenter(void)
{
    return (this->Center);
};

void Object::calculateCenter()
{
    float minX = std::numeric_limits<float>::max();
    float minY = std::numeric_limits<float>::max();
    float minZ = std::numeric_limits<float>::max();
    float maxX = std::numeric_limits<float>::lowest();
    float maxY = std::numeric_limits<float>::lowest();
    float maxZ = std::numeric_limits<float>::lowest();

    unsigned int i = 0;
    for (const float& v : vertexes) {
        int posType = i % 3;
        switch (posType) {
            case 0: // X Coordinate
                if (v > maxX)
                    maxX = v;
                if (v < minX)
                    minX = v;
                break;
            case 1: // Y Coordinate
                if (v > maxY)
                    maxY = v;
                if (v < minY)
                    minY = v;
                break;
            case 2: // Z Coordinate
                if (v > maxZ)
                    maxZ = v;
                if (v < minZ)
                    minZ = v;
                break;
            };
        i += 1;
    };
    Center[0] = (minX + maxX) / 2;
    Center[1] = (minY + maxY) / 2;
    Center[2] = (minZ + maxZ) / 2;
};
