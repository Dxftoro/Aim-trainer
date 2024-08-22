#pragma once
#include <gl/GL.h>
#include <string>
#include "player_controls.h"
#include "textures.h"

void DrawTextN(std::string text, unsigned int font, float scale, vec2 pos, vec3 color3 = vec3(1.0f, 1.0f, 1.0f)) {
	float verticies[4][2]{
		{-1.0f, 1.0f},
		{1.0f, 1.0f},
		{-1.0f, -1.0f},
		{1.0f, -1.0f}
	};
	float texture_coord[4][2]{
		{0.0f, 0.0f},
		{1.0f, 0.0f},
		{0.0f, 1.0f},
		{1.0f, 1.0f}
	};

	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, font);

	glPushMatrix();
	glEnableClientState(GL_TEXTURE_COORD_ARRAY);
	glEnableClientState(GL_VERTEX_ARRAY);

	glColor3f(color3.x, color3.y, color3.z);
	glVertexPointer(2, GL_FLOAT, 0, &verticies);
	glTexCoordPointer(2, GL_FLOAT, 0, &texture_coord);

	glTranslatef(pos.x, pos.y, 0.0f);
	glScalef(scale, scale, 0);

	for (int i = 0; i < text.size(); i++) {
		static float char_size = 1 / 16.0f;
		int x = text[i] & 0b1111, y = text[i] >> 4;
		struct {
			float left, right, top, bottom;
		} rect;

		rect.left = x * char_size;
		rect.right = rect.left + char_size;
		rect.top = y * char_size;
		rect.bottom = rect.top + char_size;

		texture_coord[0][0] = rect.left;
		texture_coord[0][1] = rect.bottom;

		texture_coord[1][0] = rect.right;
		texture_coord[1][1] = rect.bottom;

		texture_coord[2][0] = rect.left;
		texture_coord[2][1] = rect.top;

		texture_coord[3][0] = rect.right;
		texture_coord[3][1] = rect.top;

		glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
		glTranslatef(1.5f, 0.0f, 0.0f);
		if (text[i] == '\n') glTranslatef(-(i + 1) * 1.5f, -2.1f, 0.0f);
	}

	glDisableClientState(GL_VERTEX_ARRAY);
	glDisableClientState(GL_TEXTURE_COORD_ARRAY);
	glPopMatrix();

	glBindTexture(GL_TEXTURE_2D, 0);
}

void DrawText(std::string text, Font256 &font, float scale, vec2 pos, vec3 color3 = vec3(1.0f, 1.0f, 1.0f)) {
	float verticies[4][2]{
		{-1.0f, 1.0f},
		{1.0f, 1.0f}, //1 0
		{-1.0f, -1.0f},
		{1.0f, -1.0f} //3 0
	};
	float texture_coord[4][2]{
		{0.0f, 0.0f},
		{1.0f, 0.0f},
		{0.0f, 1.0f},
		{1.0f, 1.0f}
	};

	static float char_size = 1 / 16.0f;
	float sym_interval = 0.37f;

	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, font.texture_id);

	glPushMatrix();
	glEnableClientState(GL_TEXTURE_COORD_ARRAY);
	glEnableClientState(GL_VERTEX_ARRAY);

	glColor3f(color3.x, color3.y, color3.z);
	glVertexPointer(2, GL_FLOAT, 0, &verticies);
	glTexCoordPointer(2, GL_FLOAT, 0, &texture_coord);

	glTranslatef(pos.x, pos.y, 0.0f);
	glScalef(scale, scale, 0);

	for (int i = 0; i < text.size(); i++) {
		float char_width = font.width_array[text[i]];

		int x = text[i] & 0b1111, y = text[i] >> 4;

		struct {
			float left, right, top, bottom;
		} rect;

		rect.left = x * char_size;
		rect.right = rect.left + char_size * char_width;
		rect.top = y * char_size;
		rect.bottom = rect.top + char_size;

		texture_coord[0][0] = rect.left;
		texture_coord[0][1] = rect.bottom;

		texture_coord[1][0] = rect.right;
		texture_coord[1][1] = rect.bottom;

		texture_coord[2][0] = rect.left;
		texture_coord[2][1] = rect.top;

		texture_coord[3][0] = rect.right; 
		texture_coord[3][1] = rect.top;

		verticies[1][0] = char_width;
		verticies[3][0] = char_width;

		glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
		glTranslatef(char_width + sym_interval, 0.0f, 0.0f);
		if (text[i] == '\n') glTranslatef(-(i + 1) * (char_width + sym_interval), -2.1f, 0.0f);
	}

	glDisableClientState(GL_VERTEX_ARRAY);
	glDisableClientState(GL_TEXTURE_COORD_ARRAY);
	glPopMatrix();

	glBindTexture(GL_TEXTURE_2D, 0);
}

class UIframe {
private:
	std::string name;
	float vert[4][2]{
		{0.0f, 0.0f},
		{100.0f, 0.0f},
		{100.0f, 30.0f},
		{0.0f, 30.0f}
	};

	vec2 pos = vec2(0.0f, 0.0f);
	vec2 size = vec2(100.0f, 30.0f);
	vec3 colors = vec3(1.0f, 1.0f, 1.0f);

	void UpdateMatrix() {
		vert[0][0] = pos.x;
		vert[0][1] = pos.y;

		vert[1][0] = pos.x + size.x;
		vert[1][1] = pos.y;

		vert[2][0] = pos.x + size.x;
		vert[2][1] = pos.y + size.y;

		vert[3][0] = pos.x;
		vert[3][1] = pos.y + size.y;
	}

public:
	UIframe() {}
	
	UIframe(std::string name, float frame_width, float frame_height) {
		this->name = name;
		size = vec2(frame_width, frame_height);
		UpdateMatrix();
	}

	void SetPos(float x, float y) {
		pos = vec2(x, y);
		UpdateMatrix();
	}

	void SetSize(float width, float height) {
		size = vec2(width, height);
		UpdateMatrix();
	}

	void SetColor(float r, float g, float b) {
		this->colors.x = r;
		this->colors.y = g;
		this->colors.z = b;
	}

	std::string Name() {
		return name;
	}

	vec2 GetPos() {
		return this->pos;
	}

	vec2 GetSize() {
		return this->size;
	}

	void Draw() {
		glEnableClientState(GL_VERTEX_ARRAY);

		glColor3f(colors.x, colors.y, colors.z);
		glVertexPointer(2, GL_FLOAT, 0, &(this->vert));
		glDrawArrays(GL_TRIANGLE_FAN, 0, 4);

		glDisableClientState(GL_VERTEX_ARRAY);
	}
};

class UIbutton {
private:
	std::string name;
	float vert[4][2] {
		{0.0f, 0.0f},
		{100.0f, 0.0f},
		{100.0f, 30.0f},
		{0.0f, 30.0f}
	};

	bool pointed = false;
	unsigned int font = 0;

	vec2 pos = vec2(0.0f, 0.0f);
	vec2 size = vec2(100.0f, 30.0f);
	vec3 colors = vec3(1.0f, 1.0f, 0.0f);

	void UpdateMatrix() {
		vert[0][0] = pos.x;
		vert[0][1] = pos.y;

		vert[1][0] = pos.x + size.x;
		vert[1][1] = pos.y;

		vert[2][0] = pos.x + size.x;
		vert[2][1] = pos.y + size.y;

		vert[3][0] = pos.x;
		vert[3][1] = pos.y + size.y;
	}

public:
	UIbutton() {}

	UIbutton(std::string name, float button_width, float button_height) {
		this->name = name;
		size = vec2(button_width, button_height);
		UpdateMatrix();
	}

	void SetPos(float x, float y) {
		pos = vec2(x, y);
		UpdateMatrix();
	}

	void SetSize(float width, float height) {
		size = vec2(width, height);
		UpdateMatrix();
	}

	void SetColor(float r, float g, float b) {
		this->colors.x = r;
		this->colors.y = g;
		this->colors.z = b;
	}

	void SetFont(unsigned int texture_id) {
		this->font = texture_id;
	}

	std::string Name() {
		return name;
	}

	vec2 GetPos() {
		return this->pos;
	}

	vec2 GetSize() {
		return this->size;
	}

	bool Pointed(float x, float y) {
		this->pointed = (x > this->vert[0][0]) && (x < this->vert[2][0]) &&
			(y > this->vert[0][1]) && (y < this->vert[2][1]);

		return pointed;
	}

	void Draw() {
		glEnableClientState(GL_VERTEX_ARRAY);

		if (!this->pointed) glColor3f(this->colors.x, this->colors.y, this->colors.z);
		else glColor3f(0.8f, 0.8f, 0.8f);

		glVertexPointer(2, GL_FLOAT, 0, &(this->vert));
		glDrawArrays(GL_TRIANGLE_FAN, 0, 4);

		glDisableClientState(GL_VERTEX_ARRAY);
	}
};