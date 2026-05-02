#include <stdio.h>
#include <stdlib.h>
#include <GL/glew.h>
#include <GL/freeglut.h>

#include "Shaders/LoadShaders.h"
GLuint h_ShaderProgram; // handle to shader program
GLint loc_ModelViewProjectionMatrix, loc_primitive_color; // indices of uniform variables

														  // include glm/*.hpp only if necessary
														  //#include <glm/glm.hpp> 
#include <glm/gtc/matrix_transform.hpp> //translate, rotate, scale, ortho, etc.
glm::mat4 ModelViewProjectionMatrix;
glm::mat4 ViewMatrix, ProjectionMatrix, ViewProjectionMatrix;

#define TO_RADIAN 0.01745329252f  
#define TO_DEGREE 57.295779513f
#define BUFFER_OFFSET(offset) ((GLvoid *) (offset))

#define LOC_VERTEX 0

int win_width = 0, win_height = 0;
float centerx = 0.0f, centery = 0.0f, rotate_angle = 0.0f;

#include "objects.h"

int leftbuttonpressed = 0;
int animation_mode = 1;
int halt = 0;
int draw_level_1 = 0, draw_level_2 = 0, draw_level_3 = 0, draw_level_4 = 0, draw_level_5 = 0;
unsigned int timestamp = 0;

float squareSize = 100.0f;

void timer(int value) {
	timestamp = (timestamp + 1) % UINT_MAX;
	glutPostRedisplay();
	if (animation_mode)
		glutTimerFunc(10, timer, 0);
}

void display(void) {

	//halt 구현
	float t1 = (halt >= 1) ? 0.0f : timestamp;
	float t2 = (halt >= 2) ? 0.0f : timestamp;
	float t3 = (halt >= 3) ? 0.0f : timestamp;
	float t4 = (halt >= 4) ? 0.0f : timestamp;
	float t5 = (halt >= 5) ? 0.0f : timestamp;

	glm::mat4 ModelMatrix;

	glClear(GL_COLOR_BUFFER_BIT);

	ModelMatrix = glm::mat4(1.0f);
	ModelViewProjectionMatrix = ViewProjectionMatrix * ModelMatrix;
	glUniformMatrix4fv(loc_ModelViewProjectionMatrix, 1, GL_FALSE, &ModelViewProjectionMatrix[0][0]);
	draw_axes();

	ModelMatrix = glm::translate(glm::mat4(1.0f), glm::vec3(centerx, centery, 0.0f));
	ModelViewProjectionMatrix = ViewProjectionMatrix * ModelMatrix;
	glUniformMatrix4fv(loc_ModelViewProjectionMatrix, 1, GL_FALSE, &ModelViewProjectionMatrix[0][0]);
	draw_snail(); //level 0, 마우스 따라 움직임(요구조건 2)

	//사각형 궤도
	float time = glutGet(GLUT_ELAPSED_TIME) / 1000.0f;
	float cycle = 4.0f;
	float t = fmod(time, cycle) / cycle;

	float x = 0.0f, y = 0.0f;
	if (t < 0.25f) {
		x = -squareSize + 8.0f * squareSize * t;
		y = squareSize;
	}
	else if (t < 0.5f) {
		x = squareSize;
		y = squareSize - 8.0f * squareSize * (t - 0.25f);
	}
	else if (t < 0.75f) { 
		x = squareSize - 8.0f * squareSize * (t - 0.5f);
		y = -squareSize;
	}
	else { 
		x = -squareSize;
		y = -squareSize + 8.0f * squareSize * (t - 0.75f);
	}

	if (halt >= 2) {
		x = squareSize;
		y = squareSize;
	}

	if (draw_level_1) {
		for (int i = 0; i < 4; i++) {
			glm::mat4 ModelMatrix_1 = glm::rotate(ModelMatrix, (i * 90.0f + t1 * 0.5f)*TO_RADIAN, glm::vec3(0.0f, 0.0f, 1.0f));
			ModelMatrix_1 = glm::translate(ModelMatrix_1, glm::vec3(300.0f, 0.0f, 0.0f));
			ModelViewProjectionMatrix = ViewProjectionMatrix * ModelMatrix_1;
			glUniformMatrix4fv(loc_ModelViewProjectionMatrix, 1, GL_FALSE, &ModelViewProjectionMatrix[0][0]);
			draw_tree(); //level 1, level0에 종속된 움직임, draw_level 로 on-off(요구조건 3)

			if (draw_level_2) {
				for (int j = 0; j < 1; j++) {
					glm::mat4 ModelMatrix_2 = glm::translate(ModelMatrix_1, glm::vec3(x, y, 0.0f)); //사각형 궤도 회전
					//ModelMatrix_2 = glm::scale(ModelMatrix_2, glm::vec3(0.4f, 0.4f, 1.0f));
					ModelViewProjectionMatrix = ViewProjectionMatrix * ModelMatrix_2;
					glUniformMatrix4fv(loc_ModelViewProjectionMatrix, 1, GL_FALSE, &ModelViewProjectionMatrix[0][0]);
					draw_sunflower(); //level 2

					if (draw_level_3) {
						for (int k = 0; k < 4; k++) {
							float sin_angle = (k * 90.0f + t3) * TO_RADIAN; //사인파형 궤도 회전
							float radius = 100.0f + 140.0f * sinf(t3 * 0.03f);
							
							float scale_factor = 1.5f + 1.0f * sinf(t3 * 0.08 + k); //회전하면서 크기 변화

							glm::mat4 ModelMatrix_3 = glm::rotate(ModelMatrix_2, sin_angle, glm::vec3(0.0f, 0.0f, 1.0f));
							ModelMatrix_3 = glm::translate(ModelMatrix_3, glm::vec3(radius, 0.0f, 0.0f)); 
							glm::mat4 ModelMatrix_3_d = glm::scale(ModelMatrix_3, glm::vec3(scale_factor, scale_factor, 1.0f)); 
							ModelViewProjectionMatrix = ViewProjectionMatrix * ModelMatrix_3_d; 
							glUniformMatrix4fv(loc_ModelViewProjectionMatrix, 1, GL_FALSE, &ModelViewProjectionMatrix[0][0]); 
							draw_butterfly(); //level 3
							
							if (draw_level_4) {
								for (int l = 0; l < 2; l++) {
									glm::mat4 ModelMatrix_4 = glm::rotate(ModelMatrix_3, (l * 30.0f + t4) * TO_RADIAN, glm::vec3(0.0f, 0.0f, 1.0f));
									ModelMatrix_4 = glm::translate(ModelMatrix_4, glm::vec3(100.0f, 0.0f, 0.0f));
									//ModelMatrix_4 = glm::scale(ModelMatrix_4, glm::vec3(3.0f, 3.0f, 1.0f));
									ModelViewProjectionMatrix = ViewProjectionMatrix * ModelMatrix_4;
									glUniformMatrix4fv(loc_ModelViewProjectionMatrix, 1, GL_FALSE, &ModelViewProjectionMatrix[0][0]);
									draw_bee(); //level 4

									if (draw_level_5) {
										for (int n = 0; n < 1; n++) {
											glm::mat4 ModelMatrix_5 = glm::rotate(ModelMatrix_4, (n + t5) * TO_RADIAN, glm::vec3(0.0f, 0.0f, 1.0f));
											ModelMatrix_5 = glm::translate(ModelMatrix_5, glm::vec3(50.0f, 0.0f, 0.0f));
											ModelMatrix_5 = glm::scale(ModelMatrix_5, glm::vec3(0.5f, 0.5f, 0.1f));
											ModelViewProjectionMatrix = ViewProjectionMatrix * ModelMatrix_5;
											glUniformMatrix4fv(loc_ModelViewProjectionMatrix, 1, GL_FALSE, &ModelViewProjectionMatrix[0][0]);
											draw_bird(); //level5, on-off 기능 있음 (요구조건 4)
										}
									}
								}
							}
						}
					}
				}
			}
		}
	}
	

	glFlush();
}

void keyboard(unsigned char key, int x, int y) {
	switch (key) {
	case 27: // ESC key
		glutLeaveMainLoop(); // Incur destuction callback for cleanups.
		break;
	case 'q':
		animation_mode = 1 - animation_mode;
		if (animation_mode) glutTimerFunc(10, timer, 0);
		break;
	case 'w':
		halt = (halt + 1) % 6;
		glutPostRedisplay();
		break;
	case 'a':
		draw_level_1 = 1 - draw_level_1;
		glutPostRedisplay();
		break;
	case 's':
		draw_level_2 = 1 - draw_level_2;
		glutPostRedisplay();
		break;
	case 'd':
		draw_level_3 = 1 - draw_level_3;
		glutPostRedisplay();
		break;
	case 'f':
		draw_level_4 = 1 - draw_level_4;
		glutPostRedisplay();
		break;
	case 'g':
		draw_level_5 = 1 - draw_level_5;
		glutPostRedisplay();
		break;

	}
}

void mouse(int button, int state, int x, int y) {
	if ((button == GLUT_LEFT_BUTTON) && (state == GLUT_DOWN))
		leftbuttonpressed = 1;
	else if ((button == GLUT_LEFT_BUTTON) && (state == GLUT_UP))
		leftbuttonpressed = 0;
}

void motion(int x, int y) {
	if (leftbuttonpressed) {
		centerx = x - win_width / 2.0f, centery = (win_height - y) - win_height / 2.0f;
		glutPostRedisplay();
	}
}

void reshape(int width, int height) {
	win_width = width, win_height = height;

	glViewport(0, 0, win_width, win_height);
	ProjectionMatrix = glm::ortho(-win_width / 2.0, win_width / 2.0,
		-win_height / 2.0, win_height / 2.0, -1000.0, 1000.0);
	ViewProjectionMatrix = ProjectionMatrix * ViewMatrix;

	update_axes();

	glutPostRedisplay();
}

void cleanup(void) {
	glDeleteVertexArrays(1, &VAO_axes);
	glDeleteBuffers(1, &VBO_axes);

	glDeleteVertexArrays(1, &VAO_snail);
	glDeleteBuffers(1, &VBO_snail);

	glDeleteVertexArrays(1, &VAO_sunflower);
	glDeleteBuffers(1, &VBO_sunflower);

	glDeleteVertexArrays(1, &VAO_butterfly);
	glDeleteBuffers(1, &VBO_butterfly);

	glDeleteVertexArrays(1, &VAO_tree);
	glDeleteBuffers(1, &VBO_tree);

	glDeleteVertexArrays(1, &VAO_bee);
	glDeleteBuffers(1, &VBO_bee);

	glDeleteVertexArrays(1, &VAO_bird);
	glDeleteBuffers(1, &VBO_bird);

	// Delete others here too!!!
}

void register_callbacks(void) {
	glutDisplayFunc(display);
	glutKeyboardFunc(keyboard);
	glutMouseFunc(mouse);
	glutMotionFunc(motion);
	glutReshapeFunc(reshape);
	glutTimerFunc(10, timer, 0);
	glutCloseFunc(cleanup);
}

void prepare_shader_program(void) {
	ShaderInfo shader_info[3] = {
		{ GL_VERTEX_SHADER, "Shaders/simple.vert" },
		{ GL_FRAGMENT_SHADER, "Shaders/simple.frag" },
		{ GL_NONE, NULL }
	};

	h_ShaderProgram = LoadShaders(shader_info);
	glUseProgram(h_ShaderProgram);

	loc_ModelViewProjectionMatrix = glGetUniformLocation(h_ShaderProgram, "u_ModelViewProjectionMatrix");
	loc_primitive_color = glGetUniformLocation(h_ShaderProgram, "u_primitive_color");
}

void initialize_OpenGL(void) {
	glEnable(GL_MULTISAMPLE);
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

	glClearColor(44 / 255.0f, 180 / 255.0f, 49 / 255.0f, 1.0f);
	ViewMatrix = glm::mat4(1.0f);
}

void prepare_scene(void) {
	prepare_axes();
	prepare_snail();
	prepare_sunflower();
	prepare_butterfly();
	prepare_tree();
	prepare_bee();
	prepare_bird();
}

void initialize_renderer(void) {
	register_callbacks();
	prepare_shader_program();
	initialize_OpenGL();
	prepare_scene();
}

void initialize_glew(void) {
	GLenum error;

	glewExperimental = GL_TRUE;

	error = glewInit();
	if (error != GLEW_OK) {
		fprintf(stderr, "Error: %s\n", glewGetErrorString(error));
		exit(-1);
	}
	fprintf(stdout, "*********************************************************\n");
	fprintf(stdout, " - GLEW version supported: %s\n", glewGetString(GLEW_VERSION));
	fprintf(stdout, " - OpenGL renderer: %s\n", glGetString(GL_RENDERER));
	fprintf(stdout, " - OpenGL version supported: %s\n", glGetString(GL_VERSION));
	fprintf(stdout, "*********************************************************\n\n");
}

void greetings(char *program_name, char messages[][256], int n_message_lines) {
	fprintf(stdout, "**************************************************************\n\n");
	fprintf(stdout, "  PROGRAM NAME: %s\n\n", program_name);
	fprintf(stdout, "    This program was coded for CSE4170 students\n");
	fprintf(stdout, "      of Dept. of Comp. Sci. & Eng., Sogang University.\n\n");

	for (int i = 0; i < n_message_lines; i++)
		fprintf(stdout, "%s\n", messages[i]);
	fprintf(stdout, "\n**************************************************************\n\n");

	initialize_glew();
}

#define N_MESSAGE_LINES 1
void main(int argc, char *argv[]) {
	char program_name[64] = "Sogang CSE4170 20241620 HW2";
	char messages[N_MESSAGE_LINES][256] = {
		"    - Keys used: 'ESC', 'q', 'w', 'a', 's', 'd', 'f', 'g' "
		"    - Mouse used: L-click and move"
	};

	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_RGBA | GLUT_MULTISAMPLE);
	glutInitWindowSize(1200, 800);
	glutInitContextVersion(3, 3);
	glutInitContextProfile(GLUT_CORE_PROFILE);
	glutCreateWindow(program_name);

	greetings(program_name, messages, N_MESSAGE_LINES);
	initialize_renderer();

	glutSetOption(GLUT_ACTION_ON_WINDOW_CLOSE, GLUT_ACTION_GLUTMAINLOOP_RETURNS);
	glutMainLoop();
}


