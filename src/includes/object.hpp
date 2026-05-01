#pragma once

#include <iostream>
#include <fstream>
#include <vector>
#include <array>

class Object {
    private:
        std::vector<float> vertexes;
        std::vector<float> vertexNormals;
        std::vector<float> vertexTexCoords;
        std::vector<unsigned int> faces;
        std::array<float, 3> Center;
        std::string material;
        std::string name;

        void calculateCenter();

    public:
        Object(std::string filepath);
        Object(const Object& obj);
        Object& operator=(const Object& obj);
        ~Object();

        std::vector<float> getVertexes(void);
        std::vector<float> getVertexNormals(void);
        std::vector<float> getVertexTexCoords(void);
        std::vector<unsigned int> getFaces(void);
        const std::array<float, 3> getCenter(void);
};