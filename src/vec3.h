#pragma once

class Vector {
public:
    Vector(float _x = 0.f, float _y = 0.f, float _z = 0.f);
    Vector(const Vector &v);

    Vector & operator = (const Vector &rhs);
    bool operator < (const Vector &that) const;
    Vector unit() const;
    double length() const;
    float dot(const Vector &v) const;
    Vector add(const Vector &v) const;
    Vector add(const float f) const;
    Vector subtract(const Vector &v) const;
    Vector subtract(const float f) const;
    Vector multiply(const float f) const;
    Vector multiply(const Vector &v) const;

    void toArray(float a[3]) const;

    static Vector lerp(const Vector &a, const Vector &b, float fraction);
    static Vector fromAngles(float theta, float phi);

public:
    float x, y, z;
};
