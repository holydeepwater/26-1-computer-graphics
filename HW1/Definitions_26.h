#pragma once

#define MAX_POSITIONS 128
#define MAX_POLYGONS 10

#define IDLE_BACKGROUND_COLOR     0.93f, 0.95f, 0.97f
#define CREATE_BACKGROUND_COLOR   0.90f, 0.96f, 0.90f
#define SELECT_BACKGROUND_COLOR   0.98f, 0.94f, 0.86f
#define ANIMATE_BACKGROUND_COLOR  0.90f, 0.90f, 0.98f

#define POINT_COLOR 0.0f, 0.0f, 0.0f
#define CENTER_POINT_COLOR 0.0f, 0.0f, 0.0f
#define SELECTED_CENTER_POINT_COLOR 1.0f, 0.0f, 0.0f

#define ROTATION_STEP 16 // ms
#define SCALE_UP_FACTOR 1.02f
#define SCALE_DOWN_FACTOR (1.0f / SCALE_UP_FACTOR)
#define CENTER_SELECTION_SENSITIVITY 0.03f

typedef struct {
	int width, height;
	int initial_anchor_x, initial_anchor_y;
} Window;

typedef struct {
	int leftbuttonpressed;
	int rightbuttonpressed;
	int move_mode;
	int rotation_mode;
} Status;

typedef struct {
	float point[MAX_POSITIONS][2];
	int n_points;
	float center_x, center_y;
	float line_color[3];
} My_Polygon;

typedef enum {
	MODE_IDLE,
	MODE_CREATE,
	MODE_SELECT,
	MODE_ANIMATE
} EditorMode;

void add_point(My_Polygon* pg, Window* wd, int x, int y);
void draw_lines_by_points(My_Polygon* pg, float line_color_r, float line_color_g, float line_color_b);
void draw_open_lines_by_points(My_Polygon* pg, float line_color_r, float line_color_g, float line_color_b);
void draw_center_point(float x, float y, float r, float g, float b);
void update_center_of_gravity(My_Polygon* pg);
void move_points(My_Polygon* pg, float del_x, float del_y);
void rotate_points_around_center_of_grivity(My_Polygon* pg, float angle_radians);
void scale_points_around_center_of_grivity(My_Polygon* pg, float scale_factor);
void remove_vertex_at(My_Polygon* pg, int vertex_index);
void insert_vertex(My_Polygon* pg, Window* wd, int x, int y);
