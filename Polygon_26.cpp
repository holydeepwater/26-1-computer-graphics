#include <stdio.h>
#include <math.h>
#include <GL/glew.h>
#include <GL/freeglut.h>
#include "Definitions_26.h"

void add_point(My_Polygon* pg, Window* wd, int x, int y) {
	if (pg->n_points >= MAX_POSITIONS) return;

	pg->point[pg->n_points][0] = 2.0f * ((float)x) / wd->width - 1.0f;
	pg->point[pg->n_points][1] = 2.0f * ((float)wd->height - y) / wd->height - 1.0f;
	pg->n_points++;
}

void draw_lines_by_points(My_Polygon* pg, float line_color_r, float line_color_g, float line_color_b) {
	glColor3f(POINT_COLOR);
	glBegin(GL_POINTS);
	for (int i = 0; i < pg->n_points; i++) {
		glVertex2f(pg->point[i][0], pg->point[i][1]);
	}
	glEnd();

	if (pg->n_points >= 2) {
		glColor3f(line_color_r, line_color_g, line_color_b);
		glBegin(GL_LINE_LOOP);
		for (int i = 0; i < pg->n_points; i++) {
			glVertex2f(pg->point[i][0], pg->point[i][1]);
		}
		glEnd();
	}
}

void draw_open_lines_by_points(My_Polygon* pg, float line_color_r, float line_color_g, float line_color_b) {
	glColor3f(POINT_COLOR);
	glBegin(GL_POINTS);
	for (int i = 0; i < pg->n_points; i++) {
		glVertex2f(pg->point[i][0], pg->point[i][1]);
	}
	glEnd();

	if (pg->n_points >= 2) {
		glColor3f(line_color_r, line_color_g, line_color_b);
		glBegin(GL_LINE_STRIP);
		for (int i = 0; i < pg->n_points; i++) {
			glVertex2f(pg->point[i][0], pg->point[i][1]);
		}
		glEnd();
	}
}

void draw_center_point(float x, float y, float r, float g, float b) {
	glColor3f(r, g, b);
	glBegin(GL_POINTS);
	glVertex2f(x, y);
	glEnd();
}

void update_center_of_gravity(My_Polygon* pg) {
	pg->center_x = 0.0f;
	pg->center_y = 0.0f;

	if (pg->n_points == 0) return;

	for (int i = 0; i < pg->n_points; i++) {
		pg->center_x += pg->point[i][0];
		pg->center_y += pg->point[i][1];
	}

	pg->center_x /= (float)pg->n_points;
	pg->center_y /= (float)pg->n_points;
}

void move_points(My_Polygon* pg, float del_x, float del_y) {
	for (int i = 0; i < pg->n_points; i++) {
		pg->point[i][0] += del_x;
		pg->point[i][1] += del_y;
	}
	pg->center_x += del_x;
	pg->center_y += del_y;
}

void rotate_points_around_center_of_grivity(My_Polygon* pg, float angle_radians) {
	float c = cosf(angle_radians);
	float s = sinf(angle_radians);

	for (int i = 0; i < pg->n_points; i++) {
		float translated_x = pg->point[i][0] - pg->center_x;
		float translated_y = pg->point[i][1] - pg->center_y;

		float rotated_x = c * translated_x - s * translated_y;
		float rotated_y = s * translated_x + c * translated_y;

		pg->point[i][0] = rotated_x + pg->center_x;
		pg->point[i][1] = rotated_y + pg->center_y;
	}
}

void scale_points_around_center_of_grivity(My_Polygon* pg, float scale_factor) {
	for (int i = 0; i < pg->n_points; i++) {
		float translated_x = pg->point[i][0] - pg->center_x;
		float translated_y = pg->point[i][1] - pg->center_y;

		pg->point[i][0] = scale_factor * translated_x + pg->center_x;
		pg->point[i][1] = scale_factor * translated_y + pg->center_y;
	}
}

void remove_vertex_at(My_Polygon* pg, int vertex_index) {
	if (vertex_index < 0 || vertex_index >= pg->n_points) return;

	for (int i = vertex_index; i < pg->n_points - 1; i++) {
		pg->point[i][0] = pg->point[i + 1][0];
		pg->point[i][1] = pg->point[i + 1][1];
	}
	pg->n_points--;

	if (pg->n_points > 0) {
		update_center_of_gravity(pg);
	}
	else {
		pg->center_x = 0.0f;
		pg->center_y = 0.0f;
	}
}

void insert_vertex(My_Polygon* pg, Window* wd, int x, int y) {
	if (pg->n_points >= MAX_POSITIONS) return;

	float new_x = 2.0f * ((float)x) / wd->width - 1.0f;
	float new_y = 2.0f * ((float)wd->height - y) / wd->height - 1.0f;

	pg->point[pg->n_points][0] = new_x;
	pg->point[pg->n_points][1] = new_y;
	pg->n_points++;

	update_center_of_gravity(pg);
}