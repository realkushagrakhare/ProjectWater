#include "vec3.h"

#include "StdHeader.h"

Vector::Vector(float _x /* = 0.f */, float _y /* = 0.f */, float _z /* = 0.f */)
    : x(_x), y(_y), z(_z)
{}

Vector::Vector(const Vector &v) 
    : x(v.x), y(v.y), z(v.z)
{}

Vector & Vector::operator=(const Vector &rhs) {
    if (this != &rhs) {
        x = rhs.x;
        y = rhs.y;
        z = rhs.z;
    }
    return *this;
}

bool Vector::operator< (const Vector &that) const{
    return (x < that.x) || (x == that.x && y < that.y) || (x == that.x && y == that.y && z < that.z);
}

double Vector::length() const{
    return sqrt(x*x + y*y + z*z);
}

Vector Vector::unit() const{
    double invlen = 1./length();
    return Vector(x*invlen, y*invlen, z*invlen);
}

void Vector::toArray (float a[3]) const {
    a[0] = x;
    a[1] = y;
    a[2] = z;
}

float Vector::dot(const Vector &v) const {
    return x*v.x + y*v.y + z*v.z;
}

Vector Vector::add(const float f) const {
    return Vector(x + f, y + f, z + f);
}    

Vector Vector::add(const Vector &v) const {
    return Vector(x + v.x, y + v.y, z + v.z);
}

Vector Vector::subtract(const float f) const {
    return Vector(x - f, y - f, z - f);
}

Vector Vector::subtract(const Vector &v) const {
    return Vector(x - v.x,  y - v.y, z - v.z);
}

Vector Vector::lerp(const Vector &a, const Vector &b, float t) {
    float x = a.x + (b.x - a.x) * t;
    float y = a.y + (b.y - a.y) * t;
    float z = a.z + (b.z - a.z) * t;
    return Vector(x, y, z);
}

Vector Vector::multiply(const float f) const {
    return Vector(x*f, y*f, z*f);
}

Vector Vector::multiply(const Vector &v) const {
    return Vector(x*v.x, y*v.y, z*v.z);
}

Vector Vector::fromAngles(float theta, float phi) {
    return Vector(cos(theta) * cos(phi), sin(phi), sin(theta) * cos(phi));
}