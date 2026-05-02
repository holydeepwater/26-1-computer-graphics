#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <math.h>
#include <GL/glew.h>
#include <GL/freeglut.h>
#include "Definitions_26.h"

Window wd;
Status st;
EditorMode current_mode;

My_Polygon polygons[MAX_POLYGONS];
int n_polygons = 0;

My_Polygon current_polygon;
int selected_polygon_index = -1;

static int prev_x = 0, prev_y = 0;
static int mouse_down_on_selected_center = 0;
static int left_dragged = 0;
static int pressed_vertex_index = -1;
static int dragged_vertex_index = -1;

static float animation_scale_state = 1.0f;
static int animation_scale_direction = 1;

static const float polygon_color_table[10][3] = {
	{ 0.00f, 0.00f, 1.00f }, // blue
	{ 0.00f, 0.60f, 0.00f }, // green
	{ 0.80f, 0.00f, 0.80f }, // magenta
	{ 0.90f, 0.40f, 0.00f }, // orange
	{ 0.00f, 0.70f, 0.70f }, // cyan-ish
	{ 0.60f, 0.30f, 0.10f }, // brown
	{ 0.50f, 0.00f, 0.00f }, // dark red
	{ 0.40f, 0.20f, 0.70f }, // purple
	{ 0.70f, 0.70f, 0.00f }, // olive
	{ 0.20f, 0.20f, 0.20f }  // dark gray
};

static void set_polygon_color_by_index(My_Polygon* pg, int index) {
	pg->line_color[0] = polygon_color_table[index % 10][0];
	pg->line_color[1] = polygon_color_table[index % 10][1];
	pg->line_color[2] = polygon_color_table[index % 10][2];
}

static void set_background_color_by_mode(void) {
	switch (current_mode) {
	case MODE_IDLE:
		glClearColor(IDLE_BACKGROUND_COLOR, 1.0f);
		break;
	case MODE_CREATE:
		glClearColor(CREATE_BACKGROUND_COLOR, 1.0f);
		break;
	case MODE_SELECT:
		glClearColor(SELECT_BACKGROUND_COLOR, 1.0f);
		break;
	case MODE_ANIMATE:
		glClearColor(ANIMATE_BACKGROUND_COLOR, 1.0f);
		break;
	}
}

static int is_point_near_center(int x, int y, My_Polygon* pg) {
	float x_normalized = 2.0f * ((float)x) / wd.width - 1.0f;
	float y_normalized = 2.0f * ((float)wd.height - y) / wd.height - 1.0f;

	float dx = fabsf(x_normalized - pg->center_x);
	float dy = fabsf(y_normalized - pg->center_y) * (float)wd.width / (float)wd.height;

	return (dx < CENTER_SELECTION_SENSITIVITY) && (dy < CENTER_SELECTION_SENSITIVITY);
}

static int is_point_near_vertex(int x, int y, My_Polygon* pg, int vertex_index) {
	float x_normalized = 2.0f * ((float)x) / wd.width - 1.0f;
	float y_normalized = 2.0f * ((float)wd.height - y) / wd.height - 1.0f;

	float dx = fabsf(x_normalized - pg->point[vertex_index][0]);
	float dy = fabsf(y_normalized - pg->point[vertex_index][1]) * (float)wd.width / (float)wd.height;

	return (dx < CENTER_SELECTION_SENSITIVITY) && (dy < CENTER_SELECTION_SENSITIVITY);
}

static int find_vertex_by_click_except_center(int x, int y, My_Polygon* pg) {
	for (int i = 0; i < pg->n_points; i++) {
		if (is_point_near_vertex(x, y, pg, i)) {
			if (!is_point_near_center(x, y, pg)) {
				return i;
			}
		}
	}
	return -1;
}

static int find_polygon_by_center_click(int x, int y) {
	for (int i = n_polygons - 1; i >= 0; i--) {
		if (is_point_near_center(x, y, &polygons[i])) return i;
	}
	return -1;
}

static void remove_polygon_at(int index) {
	if (index < 0 || index >= n_polygons) return;

	for (int i = index; i < n_polygons - 1; i++) {
		polygons[i] = polygons[i + 1];
	}
	n_polygons--;

	if (n_polygons == 0) {
		selected_polygon_index = -1;
	}
	else if (selected_polygon_index >= n_polygons) {
		selected_polygon_index = n_polygons - 1;
	}
}

static void start_create_mode(void) {
	if (n_polygons >= MAX_POLYGONS) {
		fprintf(stderr, "*** Error: maximum number of polygons is %d.\n", MAX_POLYGONS);
		return;
	}

	current_polygon.n_points = 0;
	current_polygon.center_x = 0.0f;
	current_polygon.center_y = 0.0f;
	set_polygon_color_by_index(&current_polygon, n_polygons);
	current_mode = MODE_CREATE;
	glutPostRedisplay();
}

static void finish_create_mode(void) {
	if (current_polygon.n_points >= 3) {
		update_center_of_gravity(&current_polygon);
		polygons[n_polygons] = current_polygon;
		n_polygons++;

		current_polygon.n_points = 0;
		current_polygon.center_x = 0.0f;
		current_polygon.center_y = 0.0f;
		current_mode = MODE_IDLE;
		fprintf(stdout, "*** Polygon creation finished.\n");
	}
	else {
		fprintf(stderr, "*** Error: choose at least three points before pressing 'e'.\n");
	}
	glutPostRedisplay();
}

static void stop_drag_and_rotation_flags(void) {
	st.leftbuttonpressed = 0;
	st.rightbuttonpressed = 0;
	st.move_mode = 0;
	st.rotation_mode = 0;
	mouse_down_on_selected_center = 0;
	left_dragged = 0;
	pressed_vertex_index = -1;
	dragged_vertex_index = -1;
}

void timer(int value) {
	if (current_mode == MODE_ANIMATE && selected_polygon_index >= 0 && selected_polygon_index < n_polygons) {
		My_Polygon* pg = &polygons[selected_polygon_index];

		rotate_points_around_center_of_grivity(pg, -0.04f);

		if (animation_scale_direction > 0) {
			scale_points_around_center_of_grivity(pg, 1.005f);
			animation_scale_state *= 1.005f;
			if (animation_scale_state > 1.25f) animation_scale_direction = -1;
		}
		else {
			scale_points_around_center_of_grivity(pg, 0.995f);
			animation_scale_state *= 0.995f;
			if (animation_scale_state < 0.75f) animation_scale_direction = 1;
		}

		glutPostRedisplay();
		glutTimerFunc(ROTATION_STEP, timer, 0);
	}
}

void display(void) {
	set_background_color_by_mode();
	glClear(GL_COLOR_BUFFER_BIT);

	for (int i = 0; i < n_polygons; i++) {
		draw_lines_by_points(&polygons[i],
			polygons[i].line_color[0],
			polygons[i].line_color[1],
			polygons[i].line_color[2]);

		if (i == selected_polygon_index &&
			(current_mode == MODE_SELECT || current_mode == MODE_ANIMATE)) {
			draw_center_point(polygons[i].center_x, polygons[i].center_y,
				SELECTED_CENTER_POINT_COLOR);
		}
		else {
			draw_center_point(polygons[i].center_x, polygons[i].center_y,
				CENTER_POINT_COLOR);
		}
	}

	if (current_mode == MODE_CREATE && current_polygon.n_points > 0) {
		draw_open_lines_by_points(&current_polygon,
			current_polygon.line_color[0],
			current_polygon.line_color[1],
			current_polygon.line_color[2]);
	}

	glutSwapBuffers();
}

void keyboard(unsigned char key, int x, int y) {
	switch (key) {
	case 's':
	case 'S':
		if (current_mode == MODE_IDLE) {
			start_create_mode();
		}
		break;

	case 'e':
	case 'E':
		if (current_mode == MODE_CREATE) {
			finish_create_mode();
		}
		break;

	case 'c':
	case 'C':
		if (current_mode == MODE_SELECT && selected_polygon_index != -1) {
			remove_polygon_at(selected_polygon_index);
			selected_polygon_index = -1;
			current_mode = MODE_IDLE;
			stop_drag_and_rotation_flags();
			glutPostRedisplay();
		}
		break;

	case 'a':
	case 'A':
		if (current_mode == MODE_SELECT && selected_polygon_index != -1) {
			current_mode = MODE_ANIMATE;
			animation_scale_state = 1.0f;
			animation_scale_direction = 1;
			glutTimerFunc(ROTATION_STEP, timer, 0);
			glutPostRedisplay();
		}
		else if (current_mode == MODE_ANIMATE && selected_polygon_index != -1) {
			current_mode = MODE_SELECT;
			glutPostRedisplay();
		}
		break;

	case 'f':
	case 'F':
		if (current_mode == MODE_SELECT && selected_polygon_index != -1) {
			int r, g, b;

			printf("Enter RGB values (0~255): ");
			if (scanf("%d %d %d", &r, &g, &b) != 3) {
				fprintf(stderr, "*** Error: invalid RGB input.\n");
				int ch;
				while ((ch = getchar()) != '\n' && ch != EOF) {}
				break;
			}

			if (r < 0 || r > 255 || g < 0 || g > 255 || b < 0 || b > 255) {
				fprintf(stderr, "*** Error: RGB values must be between 0 and 255.\n");
				break;
			}

			polygons[selected_polygon_index].line_color[0] = r / 255.0f;
			polygons[selected_polygon_index].line_color[1] = g / 255.0f;
			polygons[selected_polygon_index].line_color[2] = b / 255.0f;

			glutPostRedisplay();
		}
		break;

	case 'q':
	case 'Q':
		glutLeaveMainLoop();
		break;
	}
}

void wheel(int wheel, int direction, int x, int y) {
	if (current_mode != MODE_SELECT) return;
	if (selected_polygon_index < 0 || selected_polygon_index >= n_polygons) return;

	if (direction < 0) {
		scale_points_around_center_of_grivity(&polygons[selected_polygon_index], SCALE_UP_FACTOR);
	}
	else {
		scale_points_around_center_of_grivity(&polygons[selected_polygon_index], SCALE_DOWN_FACTOR);
	}
	glutPostRedisplay();
}

void mousepress(int button, int state, int x, int y) {
	if (current_mode == MODE_CREATE) {
		if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN) {
			int key_state = glutGetModifiers();
			if (key_state & GLUT_ACTIVE_SHIFT) {
				if (current_polygon.n_points < MAX_POSITIONS) {
					add_point(&current_polygon, &wd, x, y);
					glutPostRedisplay();
				}
				else {
					fprintf(stderr, "*** Error: maximum number of vertices is %d.\n", MAX_POSITIONS);
				}
			}
		}
		return;
	}

	if (current_mode == MODE_IDLE) {
		if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN) {
			int found_index = find_polygon_by_center_click(x, y);
			if (found_index != -1) {
				selected_polygon_index = found_index;
				current_mode = MODE_SELECT;
				stop_drag_and_rotation_flags();
				glutPostRedisplay();
			}
		}
		return;
	}

	if (current_mode == MODE_SELECT) {
		if (button == GLUT_LEFT_BUTTON) {
			if (state == GLUT_DOWN) {
				My_Polygon* pg = &polygons[selected_polygon_index];
				int key_state = glutGetModifiers();

				prev_x = x;
				prev_y = y;
				left_dragged = 0;
				st.leftbuttonpressed = 1;
				st.move_mode = 0;

				mouse_down_on_selected_center = is_point_near_center(x, y, pg);
				pressed_vertex_index = find_vertex_by_click_except_center(x, y, pg);
				dragged_vertex_index = -1;

				if ((key_state & GLUT_ACTIVE_CTRL) && !mouse_down_on_selected_center && pressed_vertex_index == -1) {
					insert_vertex(pg, &wd, x, y);
					st.leftbuttonpressed = 0;
					mouse_down_on_selected_center = 0;
					pressed_vertex_index = -1;
					dragged_vertex_index = -1;
					glutPostRedisplay();
					return;
				}
			}
			else if (state == GLUT_UP) {
				My_Polygon* pg = &polygons[selected_polygon_index];

				st.leftbuttonpressed = 0;
				st.move_mode = 0;

				if (pressed_vertex_index != -1) {
					if (dragged_vertex_index == -1) {
						remove_vertex_at(pg, pressed_vertex_index);

						if (pg->n_points < 3) {
							remove_polygon_at(selected_polygon_index);
							selected_polygon_index = -1;
							current_mode = MODE_IDLE;
							stop_drag_and_rotation_flags();
							glutPostRedisplay();
							return;
						}
					}
				}
				else if (!left_dragged && mouse_down_on_selected_center) {
					selected_polygon_index = -1;
					current_mode = MODE_IDLE;
				}

				mouse_down_on_selected_center = 0;
				pressed_vertex_index = -1;
				dragged_vertex_index = -1;
				glutPostRedisplay();
			}
		}
		else if (button == GLUT_RIGHT_BUTTON) {
			if (state == GLUT_DOWN) {
				st.rightbuttonpressed = 1;
				st.rotation_mode = 0;
				prev_x = x;
				prev_y = y;
			}
			else if (state == GLUT_UP) {
				st.rightbuttonpressed = 0;
				st.rotation_mode = 0;
				glutPostRedisplay();
			}
		}
	}
}

void mousemove(int x, int y) {
	if (current_mode != MODE_SELECT) return;
	if (selected_polygon_index < 0 || selected_polygon_index >= n_polygons) return;

	if (st.leftbuttonpressed) {
		float delx = 2.0f * ((float)x - prev_x) / wd.width;
		float dely = 2.0f * ((float)prev_y - y) / wd.height;

		prev_x = x;
		prev_y = y;

		if (pressed_vertex_index != -1) {
			My_Polygon* pg = &polygons[selected_polygon_index];

			if (fabsf(delx) > 0.0f || fabsf(dely) > 0.0f) {
				dragged_vertex_index = pressed_vertex_index;
				pg->point[dragged_vertex_index][0] += delx;
				pg->point[dragged_vertex_index][1] += dely;
				update_center_of_gravity(pg);
				left_dragged = 1;
			}
		}
		else {
			move_points(&polygons[selected_polygon_index], delx, dely);
			st.move_mode = 1;
			left_dragged = 1;
		}

		glutPostRedisplay();
	}

	if (st.rightbuttonpressed) {
		float dx = (float)(x - prev_x);
		prev_x = x;
		prev_y = y;

		if (dx != 0.0f) {
			float angle = -dx * 0.01f;
			rotate_points_around_center_of_grivity(&polygons[selected_polygon_index], angle);
			st.rotation_mode = 1;
			glutPostRedisplay();
		}
	}
}

void reshape(int width, int height) {
	fprintf(stdout, "### The new window size is %dx%d.\n", width, height);
	wd.width = width;
	wd.height = height;
	glViewport(0, 0, wd.width, wd.height);
}

void close(void) {
	fprintf(stdout, "\n^^^ The control is at the close callback function now.\n\n");
}

void initialize_polygon_editor(void) {
	wd.width = 800;
	wd.height = 600;
	wd.initial_anchor_x = 500;
	wd.initial_anchor_y = 200;

	st.leftbuttonpressed = 0;
	st.rightbuttonpressed = 0;
	st.move_mode = 0;
	st.rotation_mode = 0;

	current_mode = MODE_IDLE;
	n_polygons = 0;
	selected_polygon_index = -1;

	current_polygon.n_points = 0;
	current_polygon.center_x = 0.0f;
	current_polygon.center_y = 0.0f;
}

void register_callbacks(void) {
	glutDisplayFunc(display);
	glutKeyboardFunc(keyboard);
	glutMouseWheelFunc(wheel);
	glutMouseFunc(mousepress);
	glutMotionFunc(mousemove);
	glutReshapeFunc(reshape);
	glutCloseFunc(close);
}

void initialize_renderer(void) {
	register_callbacks();

	glPointSize(7.0f);
	glLineWidth(2.0f);
	glClearColor(IDLE_BACKGROUND_COLOR, 1.0f);
}

void initialize_glew(void) {
	GLenum error;

	glewExperimental = TRUE;
	error = glewInit();
	if (error != GLEW_OK) {
		fprintf(stderr, "Error: %s\n", glewGetErrorString(error));
		exit(-1);
	}
}

void greetings(char* program_name, char messages[][256], int n_message_lines) {
	fprintf(stdout, "**************************************************************\n\n");
	fprintf(stdout, "  PROGRAM NAME: %s\n\n", program_name);

	for (int i = 0; i < n_message_lines; i++) {
		fprintf(stdout, "%s\n", messages[i]);
	}
	fprintf(stdout, "\n**************************************************************\n\n");

	initialize_glew();
}

#define N_MESSAGE_LINES 10
int main(int argc, char* argv[]) {
	char program_name[64] = "Sogang CSE4170 Polygon Editor HW1";
	char messages[N_MESSAGE_LINES][256] = {
		"    - Keys used: 's', 'e', 'c', 'a', 'f', 'q'",
		"    - Create mode: SHIFT + left click to add vertices",
		"    - Idle mode: left click centroid to select a polygon",
		"    - Select mode: left drag anywhere to move the polygon",
		"    - Select mode: right drag to rotate the polygon",
		"    - Select mode: wheel down to enlarge, wheel up to shrink",
		"    - Select mode: click a vertex and release to remove it",
		"    - Select mode: click and drag a vertex to move it",
		"    - Select mode: CTRL + left click empty space to add a new vertex",
		"    - Select mode: press 'f' to change polygon color by RGB input"
	};

	glutInit(&argc, argv);
	initialize_polygon_editor();

	glutInitContextVersion(4, 0);
	glutInitContextProfile(GLUT_COMPATIBILITY_PROFILE);

	glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE);

	glutInitWindowSize(wd.width, wd.height);
	glutInitWindowPosition(wd.initial_anchor_x, wd.initial_anchor_y);
	glutCreateWindow(program_name);

	greetings(program_name, messages, N_MESSAGE_LINES);
	initialize_renderer();

	glutSetOption(GLUT_ACTION_ON_WINDOW_CLOSE, GLUT_ACTION_GLUTMAINLOOP_RETURNS);

	glutMainLoop();
	return 0;
}
