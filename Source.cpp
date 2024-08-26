#include <GLFW/glfw3.h>
#include <gl/GL.h>
#include <iostream>
#include <string>
#include <chrono>
#include <ctime>
#include <cmath>

#include "user_interface.h"
#include "textures.h"

#define S_WIDTH 550
#define S_HEIGHT 550

Font256 Verdana;

float target_side = 50;
float easy_side = 90;
float hard_side = 50;

std::string mode_name = "none";

//GAME UI
UIbutton target = UIbutton("TARGET", target_side, target_side);
UIbutton restart = UIbutton("RESTART", 200, 60);
UIbutton menu = UIbutton("MENU", 200, 30);

//MENU UI
UIbutton easy = UIbutton("EASY", 200, 50);
UIbutton hard = UIbutton("HARD", 200, 50);
UIframe mode_view = UIframe("CHOICE", 50, 50);

int target_count = 50;
int kills = 0;
int fails = 0;

bool in_game = false;
bool in_menu = true;

std::chrono::time_point<std::chrono::system_clock> beg;
std::chrono::time_point<std::chrono::system_clock> end;
float diff_sum = 0.0f;
float avg_diff = 0.0f;

void MoveTarget() {
	float new_x = rand() % (int)(S_WIDTH - target_side);
	float new_y = rand() % (int)(S_HEIGHT - target_side);

	target.SetPos(new_x, new_y);
}

void ResetProgress() {
	in_game = true;
	kills = 0;
	fails = 0;
	diff_sum = 0.0f;

	MoveTarget();
	beg = std::chrono::system_clock::now();
}

void MouseCallback(GLFWwindow* window, int button, int action, int mods) {
	double x, y;
	glfwGetCursorPos(window, &x, &y);

	if (button == GLFW_MOUSE_BUTTON_1 && action == GLFW_PRESS) {
		if (target.Pointed(x, y) && in_game) {

			end = std::chrono::system_clock::now();
			std::chrono::duration<float> diff = end - beg;
			diff_sum += diff.count();

			kills++;
			//std::cout << diff.count() << " " << kills << std::endl;
			if (kills >= target_count) {
				avg_diff = diff_sum / kills;
				in_game = false;
				//std::cout << "Average delay: " << avg_diff << std::endl;
			}
			else MoveTarget();

			beg = std::chrono::system_clock::now();
		}
		else if (in_game) fails++;

		else if (restart.Pointed(x, y) && !in_game && !in_menu) {
			ResetProgress();
		}
		else if (menu.Pointed(x, y) && !in_game && !in_menu) {
			in_menu = true;
		}

		else if (easy.Pointed(x, y) && in_menu) {
			target_side = easy_side;
			target.SetSize(easy_side, easy_side);
			mode_name = "Easy";

			in_menu = false;
			ResetProgress();
		}
		else if (hard.Pointed(x, y) && in_menu) {
			target_side = hard_side;
			target.SetSize(hard_side, hard_side);
			mode_name = "Hard";

			in_menu = false;
			ResetProgress();
		}
	}
}

void DrawUI(GLFWwindow* window) {
	int width, height;
	double x, y;

	glfwGetWindowSize(window, &width, &height);
	glfwGetCursorPos(window, &x, &y);

	target.Pointed(x, y);
	restart.Pointed(x, y);
	menu.Pointed(x, y);
	easy.Pointed(x, y);
	hard.Pointed(x, y);

	glPushMatrix();

	glLoadIdentity();
	glOrtho(0, width, height, 0, -1, 1);

	if (in_game) {
		target.Draw();
		DrawText("Remaining: " + std::to_string(target_count - kills), Verdana, 8.0f, vec2(15.0f, 16.0f));
	}
	else if (in_menu) {
		easy.Draw();
		hard.Draw();

		vec2 easy_pos = easy.GetPos();
		vec2 hard_pos = hard.GetPos();

		DrawText("(v1.1) Made by Ftoro | Powered by C++ & OpenGL", Verdana, 6.0f, vec2(15.0f, 10.0f));
		DrawText("AIM TRAINER", Verdana, 21.0f, vec2(70.0f, 130.0f));
		DrawText("Easy mode", Verdana, 15.0f, vec2(easy_pos.x + 26.0f, easy_pos.y + 22.0f));
		DrawText("Hard mode", Verdana, 15.0f, vec2(hard_pos.x + 26.0f, hard_pos.y + 22.0f));

		if (easy.Pointed(x, y)) {
			mode_view.SetSize(easy_side, easy_side);
			mode_view.SetPos(S_WIDTH - easy_side - 70, S_HEIGHT / 2 - (easy_side / 2));
			mode_view.Draw();
		}
		if (hard.Pointed(x, y)) {
			mode_view.SetSize(hard_side, hard_side);
			mode_view.SetPos(S_WIDTH - ((easy_side - hard_side) / 2) - hard_side - 70, S_HEIGHT / 2 - (hard_side / 2));
			mode_view.Draw();
		}
	}
	else {
		restart.Draw();
		menu.Draw();

		int avg_diff_integer = (int)(avg_diff * 1000);

		DrawText("RESTART", Verdana, 15.0f, vec2(S_WIDTH / 2 - 56.0f, S_HEIGHT / 2 - 1.5f));
		DrawText("GO TO MENU", Verdana, 10.0f, vec2(S_WIDTH / 2 - 56.0f, S_HEIGHT / 2 + 64.5f));

		DrawText("Mode: " + mode_name, Verdana, 15.0f, vec2(45.0f, 50.0f));
		DrawText("Fails: " + std::to_string(fails), Verdana, 15.0f, vec2(45.0f, 90.0f));
		DrawText(
			"Average time: " + std::to_string(avg_diff_integer) + " ms",
			Verdana, 15.0f, vec2(45.0f, 130.0f)
		);

		if (avg_diff_integer <= 230) {
			DrawText("HOW TF DO YOU AIMING SO FAST???", Verdana, 12.0f, vec2(40.0f, 180.0f), vec3(1.0f, 0.0f, 0.0f));
		}
	}

	glPopMatrix();
}

void LoadTextures() {
	LoadFont("resources\\Verdana_B_alpha1.png", Verdana, 0);
}

int WinMain() {
	srand(time(0));
	GLFWwindow* window;

	if (!glfwInit()) return -1;

	window = glfwCreateWindow(S_WIDTH, S_HEIGHT, "Aim trainer", NULL, NULL);
	glfwSetWindowSizeLimits(window, S_WIDTH, S_HEIGHT, S_WIDTH, S_HEIGHT);

	GLFWimage images[1];
	images[0].pixels = stbi_load("resources\\AimTrainer.png", &images[0].width, &images[0].height, 0, 4);
	glfwSetWindowIcon(window, 1, images);
	stbi_image_free(images[0].pixels);

	if (!window) {
		glfwTerminate();
		return -1;
	}

	glfwMakeContextCurrent(window);
	glfwSetMouseButtonCallback(window, MouseCallback);

	LoadTextures();

	easy.SetColor(0.0f, 0.9f, 0.0f);
	easy.SetPos(50, S_HEIGHT / 2 - 70);

	hard.SetColor(0.9f, 0.0f, 0.0f);
	hard.SetPos(50, S_HEIGHT / 2 + 20);

	mode_view.SetColor(1.0f, 1.0f, 0.0f);

	restart.SetPos(S_WIDTH / 2 - 100, S_HEIGHT / 2 - 30);
	restart.SetColor(0.0f, 0.9f, 0.0f);
	
	menu.SetPos(S_WIDTH / 2 - 100, S_HEIGHT / 2 + 50);
	menu.SetColor(0.9f, 0.6f, 0.0f);

	MoveTarget();
	beg = std::chrono::system_clock::now();

	glEnable(GL_ALPHA_TEST);
	glAlphaFunc(GL_GREATER, 0.3f);

	while (!glfwWindowShouldClose(window)) {
		glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		DrawUI(window);

		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	std::cout << "Window closed" << std::endl;

	glfwTerminate();
	return 0;
}