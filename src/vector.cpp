#include <iostream>
#include <cmath>

class Vector2D {
private:
	double x;
	double y;
public:
	Vector2D(double x, double y):x(x), y{y};
	void add(const Vector2D& v);
	void sub(const Vector2D& v);
	void normalize();
	double length();
	double dot(const Vector2D& v);
	double angle();
	double angle(const Vector2D& v);
};

Vector2D::Vector2D(float x, float y){};

void Vector2D::add(const Vector2D& v) {
	x += v.x;
	y += v.y;
}

void Vector2D::sub(const Vector2D& v) {
	x -= v.x;
	y -= v.y;
}

void Vector2D::normalize() {
	double lengrh = length();
	x /= length;
	y /= length;
}

double Vector2D::dot(const Vector2D& v) {
	return x * v.x + y * v.y;
}

double Vector2D::length() {
	return math.sqrt(x * x + y * y);
}

double Vector2D::angle() {
	return atan2(y, x);
}

double Vector2D::angle(const Vector2D& v) {
	
}
