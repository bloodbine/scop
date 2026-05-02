#include "includes/object.hpp"
#include "includes/utils.hpp"
#include <limits>

std::tuple<int, int, int> read_vertex(std::vector<std::string> data, int i) {
    std::vector<std::string> indices = split_string(data[i], '/');
    // std::cout << indices[0] << " " << indices[1] << " " << indices[2] << std::endl;
    int v = std::stoi(indices[0]), vt = -1, vn = -1;
    if (indices.size() == 2) {
        if (data[1].find("//") == std::string::npos)
            vt = std::stoi(indices[1]);
        else
            vn = std::stoi(indices[1]);
    } else if (indices.size() == 3) {
        vt = std::stoi(indices[1]);
        vn = std::stoi(indices[2]);
    }
    std::tuple<int, int, int> vertex(v, vt, vn);
    return (vertex);
};

Object::Object(std::string filepath) {
    std::ifstream ObjFile(filepath);
    if (ObjFile.is_open()) {
        std::string line;
        std::vector<vec3> tempPositions;
        std::vector<vec3> tempNormals;
        std::vector<vec2> tempUVs;
        std::vector<std::vector<std::tuple<int,int,int>>> faces;

        while (std::getline(ObjFile, line)) {
            std::vector<std::string> data = split_string(line, ' ');
            if (data[0] == "v") {
                float x = std::stof(data[1]) ,y = std::stof(data[2]), z = std::stof(data[3]);
                tempPositions.push_back(vec3{x, y, z});
            } else if (data[0] == "vn") {
                float x = std::stof(data[1]) ,y = std::stof(data[2]), z = std::stof(data[3]);
                tempNormals.push_back(vec3{x, y, z});
            } else if (data[0] == "vt") {
                float x = std::stof(data[1]) ,y = std::stof(data[2]);
                tempUVs.push_back(vec2{x, y});
            } else if (data[0] == "f") {
                std::vector<std::tuple<int, int, int>> face;
                for (int i = 1; i <= 3; i++) {
                    std::tuple<int, int, int> vertex = read_vertex(data, i);
                    face.push_back(vertex);
                }
                faces.push_back(face);

                if (data.size() == 5) { // Smoothly handle triangulation of quads, ngons are unhandled
                    std::vector<std::tuple<int, int, int>> face_2;
                    std::tuple<int, int, int> vertex = read_vertex(data, 4);
                    face_2.push_back(face[0]);
                    face_2.push_back(face[2]);
                    face_2.push_back(vertex);
                    faces.push_back(face_2);
                };
            } else if (data[0] == "o") {
                this->name = data[1];
            };
        };
        ObjFile.close();

        std::cout << "Data Count Temp: " << tempPositions.size() << " " << tempNormals.size() << " " << tempUVs.size() << std::endl;

        std::map<VertexTriple, unsigned int> indexMap;
        for (const auto& face : faces) {
            for (const auto& triple : face) {
                int pIdx = std::get<0>(triple);
                int tIdx = std::get<1>(triple);
                int nIdx = std::get<2>(triple);

                VertexTriple key = {pIdx, tIdx, nIdx};
                auto it = indexMap.find(key);
                if (it == indexMap.end()) {
                    unsigned int newIdx = this->positions.size();
                    indexMap[key] = newIdx;
                    this->positions.push_back(tempPositions[pIdx - 1]);
                    this->UVs.push_back(tIdx >= 0 ? tempUVs[tIdx - 1] : vec2{0.0f, 0.0f});
                    this->normals.push_back(nIdx >= 0 ? tempNormals[nIdx - 1] : vec3{0.0f, 1.0f, 0.0f});
                    this->indices.push_back(newIdx);
                } else {
                    indices.push_back(it->second);
                };
            };
        };

        std::cout << "Data Count Final: " << this->positions.size() << " " << this->normals.size() << " " << this->UVs.size() << std::endl;

        calculateCenter();
    } else {
        std::cerr << "Unable to open file: " << filepath << std::endl;
    };
};

Object::Object(const Object& obj) {
    *this = obj;
};

Object& Object::operator=(const Object& obj) {
    this->positions.clear();
    this->positions = obj.positions;
    this->UVs.clear();
    this->UVs = obj.UVs;
    this->normals.clear();
    this->normals = obj.normals;
    this->indices.clear();
    this->indices = obj.indices;
    this->material = obj.material;
    this->name = obj.name;
    return *this;
};

Object::~Object() {
    return;
};

const std::vector<vec3>& Object::getPositions(void) {
    return (this->positions);
}

const std::vector<vec3>& Object::getNormals(void)
{
    return (this->normals);
}

const std::vector<vec2>& Object::getUVs(void)
{
    return (this->UVs);
};

const std::vector<unsigned int>& Object::getIndices(void) {
    return (this->indices);
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
    for (const vec3& v : positions) {

        if (v.x > maxX)
            maxX = v.x;
        if (v.x < minX)
            minX = v.x;

        if (v.y > maxY)
            maxY = v.y;
        if (v.y < minY)
            minY = v.y;

        if (v.z > maxZ)
            maxZ = v.z;
        if (v.z < minZ)
            minZ = v.z;

        i += 1;
    };
    Center[0] = (minX + maxX) / 2;
    Center[1] = (minY + maxY) / 2;
    Center[2] = (minZ + maxZ) / 2;
};
