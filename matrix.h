#ifndef MATRIX_H
#define MATRIX_H

#include <iostream>
#include <cmath>
#include <vector>
#include <string>
#include <fstream>
#include <strstream>
#include <sstream>


struct vec3d
{
    float x = 0;
    float y = 0;
    float z = 0;
    float w = 1; // Need a 4th term to perform sensible matrix vector multiplication
};

struct triangle
{
    vec3d p[3];
    wchar_t sym;
    short col;
};

struct mesh
{
    std::vector<triangle> tris;

    bool LoadFromObjectFile(std::string sFilename)
    {
        std::ifstream f(sFilename);
        if (!f.is_open())
            return false;

        // Local cache of verts
        std::vector<vec3d> verts;

        while (!f.eof())
        {
            char line[128];
            f.getline(line, 128);

            // std::strstream s; // depricated
            std::stringstream s;
            
            s << line;

            char junk;

            if (line[0] == 'v')
            {
                vec3d v;
                s >> junk >> v.x >> v.y >> v.z;
                verts.push_back(v);
            }

            if (line[0] == 'f')
            {
                int f[3];
                s >> junk >> f[0] >> f[1] >> f[2];
                tris.push_back({ verts[f[0] - 1], verts[f[1] - 1], verts[f[2] - 1] });

                // std::cout << "{{{" << tris[tris.size()-1].p[0].x << ", "<< tris[tris.size()-1].p[0].y << ", "<< tris[tris.size()-1].p[0].z << "}, {" << tris[tris.size()-1].p[1].x << ", "<< tris[tris.size()-1].p[1].y << ", "<< tris[tris.size()-1].p[1].z << "}, {" << tris[tris.size()-1].p[2].x << ", " << tris[tris.size()-1].p[2].y << ", " << tris[tris.size()-1].p[2].z << "}}}" << std::endl;
            }
        }

        // if (tris.size()>0) {
        //     // Dodanie przesunięcia do wszystkich wartości głębokości
        //     for (auto& tri : tris) {
        //         for (int i = 0; i < 3; i++) {
        //             tri.p[i].z += 2.0;
        //         }
        //     }            
        // }

        std::cout << tris.size() << std::endl;

        return true;
    }
};

struct mat4x4
{
    float m[4][4] = { 0 };
};

vec3d Matrix_MultiplyVector(mat4x4 &m, vec3d &i);
mat4x4 Matrix_MakeIdentity();
mat4x4 Matrix_MakeRotationX(float fAngleRad);
mat4x4 Matrix_MakeRotationY(float fAngleRad);
mat4x4 Matrix_MakeRotationZ(float fAngleRad);
mat4x4 Matrix_MakeTranslation(float x, float y, float z);
mat4x4 Matrix_MakeProjection(float fFovDegrees, float fAspectRatio, float fNear, float fFar);
mat4x4 Matrix_MultiplyMatrix(mat4x4 &m1, mat4x4 &m2);
mat4x4 Matrix_PointAt(vec3d &pos, vec3d &target, vec3d &up);
mat4x4 Matrix_QuickInverse(mat4x4 &m);

vec3d Vector_Add(vec3d &v1, vec3d &v2);
vec3d Vector_Sub(vec3d &v1, vec3d &v2);
vec3d Vector_Mul(vec3d &v1, float k);
vec3d Vector_Div(vec3d &v1, float k);
float Vector_DotProduct(vec3d &v1, vec3d &v2);
float Vector_Length(vec3d &v);
vec3d Vector_Normalise(vec3d &v);
vec3d Vector_CrossProduct(vec3d &v1, vec3d &v2);
vec3d Vector_IntersectPlane(vec3d &plane_p, vec3d &plane_n, vec3d &lineStart, vec3d &lineEnd);
int Triangle_ClipAgainstPlane(vec3d plane_p, vec3d plane_n, triangle &in_tri, triangle &out_tri1, triangle &out_tri2);

#endif // MATRIX_H
