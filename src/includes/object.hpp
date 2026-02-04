#include <GLFW/glfw3.h>
#include <iostream>
#include <fstream>
#include <vector>

struct Vertex {
    float x;
    float y;
    float z;
};

struct Face {
    int V1;
    int V2;
    int V3;
};

class Object {
    private:
        std::vector<Vertex> vertexes;
        std::vector<Face> faces;
        std::string material;
        std::string name;

    public:
        Object(std::string filepath);
        Object(const Object& obj);
        Object& operator=(const Object& obj);
        ~Object();

        std::vector<Vertex> getVertexes(void);
        std::vector<Face> getFaces(void);
};