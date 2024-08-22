#pragma once
#include <cmath>

struct vec2 {
	float x = 0.0f, y = 0.0f;

	vec2() {};
	vec2 (float x, float y) {
		this->x = x;
		this->y = y;
	}
};

struct vec3 {
	float x = 0.0f, y = 0.0f, z = 0.0f;

	vec3() {};
	vec3(float x, float y, float z) {
		this->x = x;
		this->y = y;
		this->z = z;
	}
};

class Player {
public:
	vec2 pos{0.0f, 0.0f};

	float x_angle = 90;
	float z_angle = 0;

	float walk_speed = 6.0f;
	float sprint_modifier = 6.0f;
	float rotate_speed = 250.0f;

	Player() {}
	Player(vec2 pos) {
		this->pos = pos;
	}

	void Move(float &speed, float &angle, float &elapsed_time) {
		this->pos.x += sin(angle) * speed * elapsed_time;
		this->pos.y += cos(angle) * speed * elapsed_time;
	}

	void Rotate(float xr, float zr) {
		this->x_angle += xr;
		this->x_angle = this->x_angle < 0 ? 0 : this->x_angle;
		this->x_angle = this->x_angle > 180 ? 180 : this->x_angle;

		this->z_angle += zr;
		if (this->z_angle < 0) this->z_angle += 360;
		if (this->z_angle > 360) this->z_angle -= 360;
	}
};