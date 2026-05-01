#pragma once

#include <iostream>
#include <vector>

std::vector<std::string> split_string(const std::string src, char delim);
void mat4Mul(const float a[16], const float b[16], float out[16]);
bool mat3Inverse(const float m[9], float out[9]);
void mat3Transpose(const float in[9], float out[9]);
void mat4ToMat3(const float in4x4[16], float out3x3[9]);