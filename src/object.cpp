#include "includes/object.hpp"
#include "includes/utils.hpp"

Object::Object(std::string filepath) {
    std::ifstream ObjFile(filepath);
    if (ObjFile.is_open()) {
        std::string line;
        while (std::getline(ObjFile, line)) {
            std::vector<std::string> split_data = split_string(line, ' ');
            if (split_data[0] == "v") {
                Vertex V;

                V.x = std::stof(split_data[1]);
                V.y = std::stof(split_data[2]);
                V.z = std::stof(split_data[3]);

                this->vertexes.push_back(V);
            } else if (split_data[0] == "f") {
                Face face1;

                face1.V1 = std::stoi(split_data[1]);
                face1.V2 = std::stoi(split_data[2]);
                face1.V3 = std::stoi(split_data[3]);

                this->faces.push_back(face1);
                if (split_data.size() == 5) {
                    Face face2;

                    face2.V1 = std::stoi(split_data[1]);
                    face2.V2 = std::stoi(split_data[3]);
                    face2.V3 = std::stoi(split_data[4]);

                    this->faces.push_back(face2);
                };
            } else if (split_data[0] == "o") {
                this->name = split_data[1];
            };
        };
        ObjFile.close();
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