#include "includes/utils.hpp"

std::vector<std::string> split_string(const std::string src, char delim) {
    std::vector<std::string> split_strings;
    int last_start = 0;
    for (int i = 0; i <= (int)src.length(); i++) {
        if (src[i] == delim || src[i] == '\0') {
            split_strings.push_back(src.substr(last_start, i - last_start));
            last_start = i + 1;
        };
    };
    return (split_strings);
};

void mat4Mul(const float a[16], const float b[16], float out[16]) {
    for (int i = 0; i < 4; ++i) {           // column i
        for (int j = 0; j < 4; ++j) {       // row j
            out[j * 4 + i] = a[0*4+i]*b[j*4+0] + a[1*4+i]*b[j*4+1] + a[2*4+i]*b[j*4+2] + a[3*4+i]*b[j*4+3];
        }
    }
}

// Compute the inverse of a 3x3 matrix (column-major)
// Returns false if singular
bool mat3Inverse(const float m[9], float out[9]) {
    float det = m[0] * (m[4] * m[8] - m[5] * m[7]) -
                m[1] * (m[3] * m[8] - m[5] * m[6]) +
                m[2] * (m[3] * m[7] - m[4] * m[6]);
    if (det == 0.0f) return false;
    float invDet = 1.0f / det;

    out[0] = (m[4] * m[8] - m[5] * m[7]) * invDet;
    out[1] = (m[2] * m[7] - m[1] * m[8]) * invDet;
    out[2] = (m[1] * m[5] - m[2] * m[4]) * invDet;
    out[3] = (m[5] * m[6] - m[3] * m[8]) * invDet;
    out[4] = (m[0] * m[8] - m[2] * m[6]) * invDet;
    out[5] = (m[2] * m[3] - m[0] * m[5]) * invDet;
    out[6] = (m[3] * m[7] - m[4] * m[6]) * invDet;
    out[7] = (m[1] * m[6] - m[0] * m[7]) * invDet;
    out[8] = (m[0] * m[4] - m[1] * m[3]) * invDet;
    return true;
}

// Transpose a 3x3 matrix (column-major)
void mat3Transpose(const float in[9], float out[9]) {
    out[0] = in[0]; out[1] = in[3]; out[2] = in[6];
    out[3] = in[1]; out[4] = in[4]; out[5] = in[7];
    out[6] = in[2]; out[7] = in[5]; out[8] = in[8];
}

// Extract the upper-left 3x3 from a 4x4 matrix (column-major)
void mat4ToMat3(const float in4x4[16], float out3x3[9]) {
    out3x3[0] = in4x4[0]; out3x3[1] = in4x4[1]; out3x3[2] = in4x4[2];
    out3x3[3] = in4x4[4]; out3x3[4] = in4x4[5]; out3x3[5] = in4x4[6];
    out3x3[6] = in4x4[8]; out3x3[7] = in4x4[9]; out3x3[8] = in4x4[10];
}