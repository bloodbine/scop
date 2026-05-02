#pragma once

#include <iostream>
#include <fstream>
#include <vector>
#include <tuple>
#include <map>
#include <array>

struct VertexTriple {
    int p, t, n;
    bool operator<(const VertexTriple& other) const {
        if (p != other.p) return p < other.p;
        if (t != other.t) return t < other.t;
        return n < other.n;
    };
};

struct vec3
{
    float x;
    float y;
    float z;
};

struct vec2 {
    float x;
    float y;
};


class Object {
    private:
        std::vector<vec3> positions;
        std::vector<vec3> normals;
        std::vector<vec2> UVs;
        std::vector<unsigned int> indices;
        std::array<float, 3> Center;
        std::string material;
        std::string name;

        void calculateCenter();

    public:
        Object(std::string filepath);
        Object(const Object& obj);
        Object& operator=(const Object& obj);
        ~Object();

        const std::vector<vec3>& getPositions(void);
        const std::vector<vec3>& getNormals(void);
        const std::vector<vec2>& getUVs(void);
        const std::vector<unsigned int>& getIndices(void);
        const std::array<float, 3> getCenter(void);
};