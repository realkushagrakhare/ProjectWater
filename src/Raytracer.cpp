#include "Raytracer.h"
#include "GLHeader.h"
#include "vsMathLib.h"

extern VSMathLib *vsml;

// the matrix is 4x4 and stored in an array, by column major.
void matrixMultiply(float *a, float *b, float *r) {
    r[0] = b[0]*a[0] + b[1]*a[4] + b[2]*a[8] + b[3]*a[12];
    r[1] = b[0]*a[1] + b[1]*a[5] + b[2]*a[9] + b[3]*a[13];
    r[2] = b[0]*a[2] + b[1]*a[6] + b[2]*a[10] + b[3]*a[14];
    r[3] = b[0]*a[3] + b[1]*a[7] + b[2]*a[11] + b[3]*a[15];

    r[4] = b[4]*a[0] + b[5]*a[4] + b[6]*a[8] + b[7]*a[12];
    r[5] = b[4]*a[1] + b[5]*a[5] + b[6]*a[9] + b[7]*a[13];
    r[6] = b[4]*a[2] + b[5]*a[6] + b[6]*a[10] + b[7]*a[14];
    r[7] = b[4]*a[3] + b[5]*a[7] + b[6]*a[11] + b[7]*a[15];

    r[8] = b[8]*a[0] + b[9]*a[4] + b[10]*a[8] + b[11]*a[12];
    r[9] = b[8]*a[1] + b[9]*a[5] + b[10]*a[9] + b[11]*a[13];
    r[10] = b[8]*a[2] + b[9]*a[6] + b[10]*a[10] + b[11]*a[14];
    r[11] = b[8]*a[3] + b[9]*a[7] + b[10]*a[11] + b[11]*a[15];

    r[12] = b[12]*a[0] + b[13]*a[4] + b[14]*a[8] + b[15]*a[12];
    r[13] = b[12]*a[1] + b[13]*a[5] + b[14]*a[9] + b[15]*a[13];
    r[14] = b[12]*a[2] + b[13]*a[6] + b[14]*a[10] + b[15]*a[14];
    r[15] = b[12]*a[3] + b[13]*a[7] + b[14]*a[11] + b[15]*a[15];
}

void matrixInverse(float *m, float *r) {
    r[0] = m[5]*m[10]*m[15] - m[5]*m[14]*m[11] - m[6]*m[9]*m[15] + m[6]*m[13]*m[11] + m[7]*m[9]*m[14] - m[7]*m[13]*m[10];
    r[1] = -m[1]*m[10]*m[15] + m[1]*m[14]*m[11] + m[2]*m[9]*m[15] - m[2]*m[13]*m[11] - m[3]*m[9]*m[14] + m[3]*m[13]*m[10];
    r[2] = m[1]*m[6]*m[15] - m[1]*m[14]*m[7] - m[2]*m[5]*m[15] + m[2]*m[13]*m[7] + m[3]*m[5]*m[14] - m[3]*m[13]*m[6];
    r[3] = -m[1]*m[6]*m[11] + m[1]*m[10]*m[7] + m[2]*m[5]*m[11] - m[2]*m[9]*m[7] - m[3]*m[5]*m[10] + m[3]*m[9]*m[6];

    r[4] = -m[4]*m[10]*m[15] + m[4]*m[14]*m[11] + m[6]*m[8]*m[15] - m[6]*m[12]*m[11] - m[7]*m[8]*m[14] + m[7]*m[12]*m[10];
    r[5] = m[0]*m[10]*m[15] - m[0]*m[14]*m[11] - m[2]*m[8]*m[15] + m[2]*m[12]*m[11] + m[3]*m[8]*m[14] - m[3]*m[12]*m[10];
    r[6] = -m[0]*m[6]*m[15] + m[0]*m[14]*m[7] + m[2]*m[4]*m[15] - m[2]*m[12]*m[7] - m[3]*m[4]*m[14] + m[3]*m[12]*m[6];
    r[7] = m[0]*m[6]*m[11] - m[0]*m[10]*m[7] - m[2]*m[4]*m[11] + m[2]*m[8]*m[7] + m[3]*m[4]*m[10] - m[3]*m[8]*m[6];

    r[8] = m[4]*m[9]*m[15] - m[4]*m[13]*m[11] - m[5]*m[8]*m[15] + m[5]*m[12]*m[11] + m[7]*m[8]*m[13] - m[7]*m[12]*m[9];
    r[9] = -m[0]*m[9]*m[15] + m[0]*m[13]*m[11] + m[1]*m[8]*m[15] - m[1]*m[12]*m[11] - m[3]*m[8]*m[13] + m[3]*m[12]*m[9];
    r[10] = m[0]*m[5]*m[15] - m[0]*m[13]*m[7] - m[1]*m[4]*m[15] + m[1]*m[12]*m[7] + m[3]*m[4]*m[13] - m[3]*m[12]*m[5];
    r[11] = -m[0]*m[5]*m[11] + m[0]*m[9]*m[7] + m[1]*m[4]*m[11] - m[1]*m[8]*m[7] - m[3]*m[4]*m[9] + m[3]*m[8]*m[5];

    r[12] = -m[4]*m[9]*m[14] + m[4]*m[13]*m[10] + m[5]*m[8]*m[14] - m[5]*m[12]*m[10] - m[6]*m[8]*m[13] + m[6]*m[12]*m[9];
    r[13] = m[0]*m[9]*m[14] - m[0]*m[13]*m[10] - m[1]*m[8]*m[14] + m[1]*m[12]*m[10] + m[2]*m[8]*m[13] - m[2]*m[12]*m[9];
    r[14] = -m[0]*m[5]*m[14] + m[0]*m[13]*m[6] + m[1]*m[4]*m[14] - m[1]*m[12]*m[6] - m[2]*m[4]*m[13] + m[2]*m[12]*m[5];
    r[15] = m[0]*m[5]*m[10] - m[0]*m[9]*m[6] - m[1]*m[4]*m[10] + m[1]*m[8]*m[6] + m[2]*m[4]*m[9] - m[2]*m[8]*m[5];

    float det = m[0]*r[0] + m[4]*r[1] + m[8]*r[2] + m[12]*r[3];;
    float invdet = 1.f / det;
    for (int i = 0; i < 16; i++) {
        r[i] *= invdet;
    }
}

Vector matrixTransformPoint(float *m, const Vector &v) {
    float x = m[0] * v.x + m[4] * v.y + m[8] * v.z + m[12];
    float y = m[1] * v.x + m[5] * v.y + m[9] * v.z + m[13];
    float z = m[2] * v.x + m[6] * v.y + m[10] * v.z + m[14];
    float w = m[3] * v.x + m[7] * v.y + m[11] * v.z + m[15];
    return Vector(x/w, y/w, z/w);
}

Vector unProject(int winX, int winY, int winZ, int viewport[4]) {
    Vector point( float(winX - viewport[0]) / float(viewport[2]) * 2 - 1, float(winY - viewport[1]) / float(viewport[3]) * 2 - 1, float(winZ * 2 - 1));
    float temp[16], temp2[16];
    float *projection = vsml->get(VSMathLib::PROJECTION);
    float *modelview = vsml->get(VSMathLib::VIEW_MODEL);
    matrixMultiply(projection, modelview, temp);
    matrixInverse(temp, temp2);
    return matrixTransformPoint(temp2, point);
}


RayTracer::RayTracer() {
    glGetIntegerv(GL_VIEWPORT, viewport);
    float *m = vsml->get(VSMathLib::VIEW_MODEL);
    
    Vector axisX(m[0], m[1], m[2]);
    Vector axisY(m[4], m[5], m[6]);
    Vector axisZ(m[8], m[9], m[10]);
    Vector offset(m[12], m[13], m[14]);
    eye.x = -1.f * offset.dot(axisX);
    eye.y = -1.f * offset.dot(axisY);
    eye.z = -1.f * offset.dot(axisZ);

    int minX = viewport[0], maxX = minX + viewport[2];
    int minY = viewport[1], maxY = minY + viewport[3];
    ray00 = unProject(minX, minY, 1, viewport).subtract(eye);
    ray10 = unProject(maxX, minY, 1, viewport).subtract(eye);
    ray01 = unProject(minX, maxY, 1, viewport).subtract(eye);
    ray11 = unProject(maxX, maxY, 1, viewport).subtract(eye);
}

Vector RayTracer::getRayForPixel(float x, float y) {
    x = (x - (float)viewport[0]) / (float)viewport[2];
    y = 1 - (y - (float)viewport[1]) / (float)viewport[3];
    Vector ray0 = Vector::lerp(ray00, ray10, x);
    Vector ray1 = Vector::lerp(ray01, ray11, x);
    return Vector::lerp(ray0, ray1, y).unit();
}
