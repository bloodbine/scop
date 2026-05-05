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
    float v1;
    float v2;
    float v3;
};

struct vec2 {
    float v1;
    float v2;
};

class Object {
    private:
        std::vector<vec3> positions;
        std::vector<vec3> normals;
        std::vector<vec2> UVs;
        std::vector<vec2> planarUVsX;
        std::vector<vec2> planarUVsY;
        std::vector<vec2> planarUVsZ;
        std::vector<vec2> sphericalUVs;
        std::vector<vec2> cylindricalUVs;
        vec3 boundsMin;
        vec3 boundsMax;
        std::vector<unsigned int> indices;
        vec3 Center;
        std::string material;
        std::string name;

        void calculateCenter(const std::vector<vec3> geometry);

    public:
        Object(std::string filepath);
        Object(const Object& obj);
        Object& operator=(const Object& obj);
        ~Object();

        const std::vector<vec3>& getPositions(void);
        const std::vector<vec3>& getNormals(void);
        const std::vector<vec2>& getUVs(void);
        const std::vector<vec2>& getPlanarUVsX(void);
        const std::vector<vec2>& getPlanarUVsY(void);
        const std::vector<vec2>& getPlanarUVsZ(void);
        const std::vector<vec2>& getSphericalUVs(void);
        const std::vector<vec2>& getCylindricalUVs(void);
        const std::vector<unsigned int>& getIndices(void);
        const vec3 getCenter(void);
};