#include "includes/object.hpp"
#include "includes/utils.hpp"
#include <limits>
#include <cmath>

std::tuple<int, int, int> read_vertex(std::vector<std::string> data, int i) {
    std::vector<std::string> indices = split_string(data[i], '/');
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

vec2 PlanarProject(const vec3& pos, const vec3& boundsMin, const vec3& boundsMax, int axis) {
    vec3 range = {
        boundsMax.v1 - boundsMin.v1,
        boundsMax.v2 - boundsMin.v2,
        boundsMax.v3 - boundsMin.v3
    };
    switch (axis) {
        case 0: // Project along X, use Y and Z
            return vec2{
                (range.v2 > 0.0f) ? (pos.v2 - boundsMin.v2) / range.v2 : 0.0f,
                (range.v3 > 0.0f) ? (pos.v3 - boundsMin.v3) / range.v3 : 0.0f
            };
        case 1: // Project along Y, use X and Z
            return vec2{
                (range.v1 > 0.0f) ? (pos.v1 - boundsMin.v1) / range.v1 : 0.0f,
                (range.v3 > 0.0f) ? (pos.v3 - boundsMin.v3) / range.v3 : 0.0f
            };
        case 2: // Project along Z, use X and Y
            return vec2{
                (range.v1 > 0.0f) ? (pos.v1 - boundsMin.v1) / range.v1 : 0.0f,
                (range.v2 > 0.0f) ? (pos.v2 - boundsMin.v2) / range.v2 : 0.0f
            };
        default:
            return vec2{0.0f, 0.0f};
    }
}

vec2 SphericalProject(const vec3& pos, const vec3& center) {
    vec3 dir = {
        pos.v1 - center.v1,
        pos.v2 - center.v2,
        pos.v3 - center.v3
    };

    float len = sqrtf(dir.v1*dir.v1 + dir.v2*dir.v2 + dir.v3*dir.v3);
    if (len > 0.0f) {
        dir.v1 /= len;
        dir.v2 /= len;
        dir.v3 /= len;
    }

    float u = 0.5f + (atan2f(dir.v3, dir.v1) / (2.0f * M_PI));
    float v = 0.5f - (asinf(dir.v2) / M_PI);
    return vec2{u, v};
}

vec2 CylindricalProject(const vec3& pos, const vec3& center, const vec3& boundsMin, const vec3& boundsMax) {
    vec3 dir = {
        pos.v1 - center.v1,
        0.0f,
        pos.v3 - center.v3
    };

    float u = 0.5f + (atan2f(dir.v3, dir.v1) / (2.0f * M_PI));
    float rangeY = boundsMax.v2 - boundsMin.v2;
    float v = (rangeY > 0.0f) ? (pos.v2 - boundsMin.v2) / rangeY : 0.0f;

    return vec2{u, v};
}

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

        this->boundsMin = {std::numeric_limits<float>::max(), std::numeric_limits<float>::max(),std::numeric_limits<float>::max()};
        this->boundsMax = {std::numeric_limits<float>::lowest(), std::numeric_limits<float>::lowest(), std::numeric_limits<float>::lowest()};
        calculateCenter(tempPositions);

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
                    this->planarUVsX.push_back(PlanarProject(tempPositions[pIdx - 1], this->boundsMin, this->boundsMax, 0));
                    this->planarUVsY.push_back(PlanarProject(tempPositions[pIdx - 1], this->boundsMin, this->boundsMax, 1));
                    this->planarUVsZ.push_back(PlanarProject(tempPositions[pIdx - 1], this->boundsMin, this->boundsMax, 2));
                    this->sphericalUVs.push_back(SphericalProject(tempPositions[pIdx - 1], this->Center));
                    this->cylindricalUVs.push_back(CylindricalProject(tempPositions[pIdx - 1], this->Center, this->boundsMin, this->boundsMax));
                    this->normals.push_back(nIdx >= 0 ? tempNormals[nIdx - 1] : vec3{0.0f, 1.0f, 0.0f});
                    this->indices.push_back(newIdx);
                } else {
                    indices.push_back(it->second);
                };
            };
        };
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

const std::vector<vec2>& Object::getPlanarUVsX(void)
{
    return (this->planarUVsX);
};

const std::vector<vec2>& Object::getPlanarUVsY(void)
{
    return (this->planarUVsY);
};

const std::vector<vec2>& Object::getPlanarUVsZ(void)
{
    return (this->planarUVsZ);
};

const std::vector<vec2>& Object::getSphericalUVs(void)
{
    return (this->sphericalUVs);
};

const std::vector<vec2>& Object::getCylindricalUVs(void)
{
    return (this->cylindricalUVs);
};

const std::vector<unsigned int>& Object::getIndices(void) {
    return (this->indices);
}

const vec3 Object::getCenter(void)
{
    return (this->Center);
};

void Object::calculateCenter(const std::vector<vec3> geometry)
{
    for (const vec3& v : geometry) {
        if (v.v1 > this->boundsMax.v1)
            this->boundsMax.v1 = v.v1;
        if (v.v1 < this->boundsMin.v1)
            this->boundsMin.v1 = v.v1;

        if (v.v2 > this->boundsMax.v2)
            this->boundsMax.v2 = v.v2;
        if (v.v2 < this->boundsMin.v2)
            this->boundsMin.v2 = v.v2;

        if (v.v3 > this->boundsMax.v3)
            this->boundsMax.v3 = v.v3;
        if (v.v3 < this->boundsMin.v3)
            this->boundsMin.v3 = v.v3;
    };

    this->Center = {0.0f, 0.0f, 0.0f};
    for (vec3 pos : geometry) {
        this->Center.v1 += pos.v1;
        this->Center.v2 += pos.v2;
        this->Center.v3 += pos.v3;
    };
    this->Center.v1 /= geometry.size();
    this->Center.v2 /= geometry.size();
    this->Center.v3 /= geometry.size();
};
