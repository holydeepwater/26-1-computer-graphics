#pragma once
GLfloat axes[4][2];
GLfloat axes_color[3] = { 0.0f, 0.0f, 0.0f };
GLuint VBO_axes, VAO_axes;

void prepare_axes(void) { // Draw axes in their MC.
	axes[0][0] = -win_width / 2.5f; axes[0][1] = 0.0f;
	axes[1][0] = win_width / 2.5f; axes[1][1] = 0.0f;
	axes[2][0] = 0.0f; axes[2][1] = -win_height / 2.5f;
	axes[3][0] = 0.0f; axes[3][1] = win_height / 2.5f;

	// Initialize vertex buffer object.
	glGenBuffers(1, &VBO_axes);

	glBindBuffer(GL_ARRAY_BUFFER, VBO_axes);
	glBufferData(GL_ARRAY_BUFFER, sizeof(axes), axes, GL_STATIC_DRAW);

	// Initialize vertex array object.
	glGenVertexArrays(1, &VAO_axes);
	glBindVertexArray(VAO_axes);

	glBindBuffer(GL_ARRAY_BUFFER, VBO_axes);
	glVertexAttribPointer(LOC_VERTEX, 2, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(0));

	glEnableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
}

void update_axes(void) {
	axes[0][0] = -win_width / 2.25f; axes[1][0] = win_width / 2.25f;
	axes[2][1] = -win_height / 2.25f;
	axes[3][1] = win_height / 2.25f;

	glBindBuffer(GL_ARRAY_BUFFER, VBO_axes);
	glBufferData(GL_ARRAY_BUFFER, sizeof(axes), axes, GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void draw_axes(void) {
	glUniform3fv(loc_primitive_color, 1, axes_color);
	glBindVertexArray(VAO_axes);
	glDrawArrays(GL_LINES, 0, 4);
	glBindVertexArray(0);
}

//house
#define HOUSE_ROOF 0
#define HOUSE_BODY 1
#define HOUSE_CHIMNEY 2
#define HOUSE_DOOR 3
#define HOUSE_WINDOW 4

GLfloat roof[3][2] = { { -12.0, 0.0 },{ 0.0, 12.0 },{ 12.0, 0.0 } };
GLfloat house_body[4][2] = { { -12.0, -14.0 },{ -12.0, 0.0 },{ 12.0, 0.0 },{ 12.0, -14.0 } };
GLfloat chimney[4][2] = { { 6.0, 6.0 },{ 6.0, 14.0 },{ 10.0, 14.0 },{ 10.0, 2.0 } };
GLfloat door[4][2] = { { -8.0, -14.0 },{ -8.0, -8.0 },{ -4.0, -8.0 },{ -4.0, -14.0 } };
GLfloat window[4][2] = { { 4.0, -6.0 },{ 4.0, -2.0 },{ 8.0, -2.0 },{ 8.0, -6.0 } };

GLfloat house_color[5][3] = {
	{ 200 / 255.0f, 39 / 255.0f, 42 / 255.0f },
	{ 235 / 255.0f, 225 / 255.0f, 196 / 255.0f },
	{ 255 / 255.0f, 0 / 255.0f, 0 / 255.0f },
	{ 233 / 255.0f, 113 / 255.0f, 23 / 255.0f },
	{ 44 / 255.0f, 180 / 255.0f, 49 / 255.0f }
};

GLuint VBO_house, VAO_house;
void prepare_house() {
	GLsizeiptr buffer_size = sizeof(roof) + sizeof(house_body) + sizeof(chimney) + sizeof(door)
		+ sizeof(window);

	// Initialize vertex buffer object.
	glGenBuffers(1, &VBO_house);

	glBindBuffer(GL_ARRAY_BUFFER, VBO_house);
	glBufferData(GL_ARRAY_BUFFER, buffer_size, NULL, GL_STATIC_DRAW); // allocate buffer object memory

	glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(roof), roof);
	glBufferSubData(GL_ARRAY_BUFFER, sizeof(roof), sizeof(house_body), house_body);
	glBufferSubData(GL_ARRAY_BUFFER, sizeof(roof) + sizeof(house_body), sizeof(chimney), chimney);
	glBufferSubData(GL_ARRAY_BUFFER, sizeof(roof) + sizeof(house_body) + sizeof(chimney), sizeof(door), door);
	glBufferSubData(GL_ARRAY_BUFFER, sizeof(roof) + sizeof(house_body) + sizeof(chimney) + sizeof(door),
		sizeof(window), window);

	// Initialize vertex array object.
	glGenVertexArrays(1, &VAO_house);
	glBindVertexArray(VAO_house);

	glBindBuffer(GL_ARRAY_BUFFER, VBO_house);
	glVertexAttribPointer(LOC_VERTEX, 2, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(0));

	glEnableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
}

void draw_house() {
	glBindVertexArray(VAO_house);

	glUniform3fv(loc_primitive_color, 1, house_color[HOUSE_ROOF]);
	glDrawArrays(GL_TRIANGLE_FAN, 0, 3);

	glUniform3fv(loc_primitive_color, 1, house_color[HOUSE_BODY]);
	glDrawArrays(GL_TRIANGLE_FAN, 3, 4);

	glUniform3fv(loc_primitive_color, 1, house_color[HOUSE_CHIMNEY]);
	glDrawArrays(GL_TRIANGLE_FAN, 7, 4);

	glUniform3fv(loc_primitive_color, 1, house_color[HOUSE_DOOR]);
	glDrawArrays(GL_TRIANGLE_FAN, 11, 4);

	glUniform3fv(loc_primitive_color, 1, house_color[HOUSE_WINDOW]);
	glDrawArrays(GL_TRIANGLE_FAN, 15, 4);

	glBindVertexArray(0);
}

// snail
#define SNAIL_SHELL 0
#define SNAIL_SHELL_IN 1
#define SNAIL_BODY 2
#define SNAIL_HEAD 3
#define SNAIL_EYE_L 4
#define SNAIL_EYE_R 5
#define SNAIL_HORN_L 6
#define SNAIL_HORN_R 7

GLfloat snail_shell[4][2] = {
	{ -14.0f, 8.0f }, { -14.0f, -8.0f }, { 2.0f, -8.0f }, { 2.0f, 8.0f }
};

GLfloat snail_shell_in[4][2] = {
	{ -10.0f, 4.0f }, { -10.0f, -4.0f }, { -2.0f, -4.0f }, { -2.0f, 4.0f }
};

GLfloat snail_body[4][2] = {
	{ -4.0f, -8.0f }, { -4.0f, -14.0f }, { 16.0f, -14.0f }, { 16.0f, -8.0f }
};

GLfloat snail_head[4][2] = {
	{ 10.0f, -8.0f }, { 10.0f, -2.0f }, { 18.0f, -2.0f }, { 18.0f, -8.0f }
};

GLfloat snail_eye_l[4][2] = {
	{ 12.0f, 3.0f }, { 12.0f, 1.0f }, { 13.5f, 1.0f }, { 13.5f, 3.0f }
};

GLfloat snail_eye_r[4][2] = {
	{ 15.0f, 3.0f }, { 15.0f, 1.0f }, { 16.5f, 1.0f }, { 16.5f, 3.0f }
};

GLfloat snail_horn_l[4][2] = {
	{ 11.0f, -2.0f }, { 11.0f, 2.0f }, { 12.0f, 2.0f }, { 12.0f, -2.0f }
};

GLfloat snail_horn_r[4][2] = {
	{ 16.0f, -2.0f }, { 16.0f, 2.0f }, { 17.0f, 2.0f }, { 17.0f, -2.0f }
};

GLfloat snail_color[8][3] = {
	{ 170 / 255.0f, 120 / 255.0f, 70 / 255.0f },
	{ 220 / 255.0f, 180 / 255.0f, 120 / 255.0f },
	{ 210 / 255.0f, 190 / 255.0f, 150 / 255.0f },
	{ 210 / 255.0f, 190 / 255.0f, 150 / 255.0f },
	{ 20 / 255.0f, 20 / 255.0f, 20 / 255.0f },
	{ 20 / 255.0f, 20 / 255.0f, 20 / 255.0f },
	{ 210 / 255.0f, 190 / 255.0f, 150 / 255.0f },
	{ 210 / 255.0f, 190 / 255.0f, 150 / 255.0f }
};

GLuint VBO_snail, VAO_snail;

void prepare_snail() {
	int size = sizeof(snail_shell);
	GLsizeiptr buffer_size = size * 8;

	glGenBuffers(1, &VBO_snail);
	glBindBuffer(GL_ARRAY_BUFFER, VBO_snail);
	glBufferData(GL_ARRAY_BUFFER, buffer_size, NULL, GL_STATIC_DRAW);

	glBufferSubData(GL_ARRAY_BUFFER, size * 0, size, snail_shell);
	glBufferSubData(GL_ARRAY_BUFFER, size * 1, size, snail_shell_in);
	glBufferSubData(GL_ARRAY_BUFFER, size * 2, size, snail_body);
	glBufferSubData(GL_ARRAY_BUFFER, size * 3, size, snail_head);
	glBufferSubData(GL_ARRAY_BUFFER, size * 4, size, snail_eye_l);
	glBufferSubData(GL_ARRAY_BUFFER, size * 5, size, snail_eye_r);
	glBufferSubData(GL_ARRAY_BUFFER, size * 6, size, snail_horn_l);
	glBufferSubData(GL_ARRAY_BUFFER, size * 7, size, snail_horn_r);

	glGenVertexArrays(1, &VAO_snail);
	glBindVertexArray(VAO_snail);

	glBindBuffer(GL_ARRAY_BUFFER, VBO_snail);
	glVertexAttribPointer(LOC_VERTEX, 2, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(0));

	glEnableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
}

void draw_snail() {
	glBindVertexArray(VAO_snail);

	for (int i = 0; i < 8; i++) {
		glUniform3fv(loc_primitive_color, 1, snail_color[i]);
		glDrawArrays(GL_TRIANGLE_FAN, i * 4, 4);
	}

	glBindVertexArray(0);
}

// butterfly
#define BUTTERFLY_WING_L_UP 0
#define BUTTERFLY_WING_L_DOWN 1
#define BUTTERFLY_WING_R_UP 2
#define BUTTERFLY_WING_R_DOWN 3
#define BUTTERFLY_BODY 4
#define BUTTERFLY_HEAD 5
#define BUTTERFLY_ANTENNA_L 6
#define BUTTERFLY_ANTENNA_R 7

GLfloat butterfly_wing_l_up[4][2] = {
	{ -2.0f, 8.0f }, { -14.0f, 14.0f }, { -18.0f, 5.0f }, { -5.0f, 1.0f }
};

GLfloat butterfly_wing_l_down[4][2] = {
	{ -3.0f, 0.0f }, { -15.0f, -4.0f }, { -11.0f, -13.0f }, { -2.0f, -7.0f }
};

GLfloat butterfly_wing_r_up[4][2] = {
	{ 2.0f, 8.0f }, { 14.0f, 14.0f }, { 18.0f, 5.0f }, { 5.0f, 1.0f }
};

GLfloat butterfly_wing_r_down[4][2] = {
	{ 3.0f, 0.0f }, { 15.0f, -4.0f }, { 11.0f, -13.0f }, { 2.0f, -7.0f }
};

GLfloat butterfly_body[4][2] = {
	{ -2.0f, 8.0f }, { -2.0f, -10.0f }, { 2.0f, -10.0f }, { 2.0f, 8.0f }
};

GLfloat butterfly_head[4][2] = {
	{ -3.0f, 13.0f }, { -3.0f, 8.0f }, { 3.0f, 8.0f }, { 3.0f, 13.0f }
};

GLfloat butterfly_antenna_l[4][2] = {
	{ -2.0f, 13.0f }, { -8.0f, 18.0f }, { -7.0f, 19.0f }, { -1.0f, 14.0f }
};

GLfloat butterfly_antenna_r[4][2] = {
	{ 2.0f, 13.0f }, { 8.0f, 18.0f }, { 7.0f, 19.0f }, { 1.0f, 14.0f }
};

GLfloat butterfly_color[8][3] = {
	{ 255 / 255.0f, 120 / 255.0f, 180 / 255.0f },
	{ 255 / 255.0f, 170 / 255.0f, 210 / 255.0f },
	{ 255 / 255.0f, 120 / 255.0f, 180 / 255.0f },
	{ 255 / 255.0f, 170 / 255.0f, 210 / 255.0f },
	{ 60 / 255.0f, 40 / 255.0f, 40 / 255.0f },
	{ 80 / 255.0f, 50 / 255.0f, 50 / 255.0f },
	{ 60 / 255.0f, 40 / 255.0f, 40 / 255.0f },
	{ 60 / 255.0f, 40 / 255.0f, 40 / 255.0f }
};

GLuint VBO_butterfly, VAO_butterfly;

void prepare_butterfly() {
	int size = sizeof(butterfly_wing_l_up);
	GLsizeiptr buffer_size = size * 8;

	glGenBuffers(1, &VBO_butterfly);

	glBindBuffer(GL_ARRAY_BUFFER, VBO_butterfly);
	glBufferData(GL_ARRAY_BUFFER, buffer_size, NULL, GL_STATIC_DRAW);

	glBufferSubData(GL_ARRAY_BUFFER, size * 0, size, butterfly_wing_l_up);
	glBufferSubData(GL_ARRAY_BUFFER, size * 1, size, butterfly_wing_l_down);
	glBufferSubData(GL_ARRAY_BUFFER, size * 2, size, butterfly_wing_r_up);
	glBufferSubData(GL_ARRAY_BUFFER, size * 3, size, butterfly_wing_r_down);
	glBufferSubData(GL_ARRAY_BUFFER, size * 4, size, butterfly_body);
	glBufferSubData(GL_ARRAY_BUFFER, size * 5, size, butterfly_head);
	glBufferSubData(GL_ARRAY_BUFFER, size * 6, size, butterfly_antenna_l);
	glBufferSubData(GL_ARRAY_BUFFER, size * 7, size, butterfly_antenna_r);

	glGenVertexArrays(1, &VAO_butterfly);
	glBindVertexArray(VAO_butterfly);

	glBindBuffer(GL_ARRAY_BUFFER, VBO_butterfly);
	glVertexAttribPointer(LOC_VERTEX, 2, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(0));

	glEnableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
}

void draw_butterfly() {
	glBindVertexArray(VAO_butterfly);

	for (int i = 0; i < 8; i++) {
		glUniform3fv(loc_primitive_color, 1, butterfly_color[i]);
		glDrawArrays(GL_TRIANGLE_FAN, i * 4, 4);
	}

	glBindVertexArray(0);
}

// tree
#define TREE_TRUNK 0
#define TREE_LEAF_TOP 1
#define TREE_LEAF_LEFT 2
#define TREE_LEAF_RIGHT 3
#define TREE_FRUIT1 4
#define TREE_FRUIT2 5

GLfloat tree_trunk[4][2] = {
	{ -3.0f, -12.0f }, { -3.0f, -24.0f }, { 3.0f, -24.0f }, { 3.0f, -12.0f }
};

GLfloat tree_leaf_top[4][2] = {
	{ -8.0f, 8.0f }, { -8.0f, -4.0f }, { 8.0f, -4.0f }, { 8.0f, 8.0f }
};

GLfloat tree_leaf_left[4][2] = {
	{ -15.0f, 2.0f }, { -15.0f, -10.0f }, { -1.0f, -10.0f }, { -1.0f, 2.0f }
};

GLfloat tree_leaf_right[4][2] = {
	{ 1.0f, 2.0f }, { 1.0f, -10.0f }, { 15.0f, -10.0f }, { 15.0f, 2.0f }
};

GLfloat tree_fruit1[4][2] = {
	{ -7.0f, 1.0f }, { -7.0f, -2.0f }, { -4.0f, -2.0f }, { -4.0f, 1.0f }
};

GLfloat tree_fruit2[4][2] = {
	{ 5.0f, -3.0f }, { 5.0f, -6.0f }, { 8.0f, -6.0f }, { 8.0f, -3.0f }
};

GLfloat tree_color[6][3] = {
	{ 120 / 255.0f, 70 / 255.0f, 30 / 255.0f },
	{ 40 / 255.0f, 150 / 255.0f, 70 / 255.0f },
	{ 30 / 255.0f, 130 / 255.0f, 60 / 255.0f },
	{ 30 / 255.0f, 130 / 255.0f, 60 / 255.0f },
	{ 220 / 255.0f, 40 / 255.0f, 40 / 255.0f },
	{ 220 / 255.0f, 40 / 255.0f, 40 / 255.0f }
};

GLuint VBO_tree, VAO_tree;

void prepare_tree() {
	int size = sizeof(tree_trunk);
	GLsizeiptr buffer_size = size * 6;

	glGenBuffers(1, &VBO_tree);

	glBindBuffer(GL_ARRAY_BUFFER, VBO_tree);
	glBufferData(GL_ARRAY_BUFFER, buffer_size, NULL, GL_STATIC_DRAW);

	glBufferSubData(GL_ARRAY_BUFFER, size * 0, size, tree_trunk);
	glBufferSubData(GL_ARRAY_BUFFER, size * 1, size, tree_leaf_top);
	glBufferSubData(GL_ARRAY_BUFFER, size * 2, size, tree_leaf_left);
	glBufferSubData(GL_ARRAY_BUFFER, size * 3, size, tree_leaf_right);
	glBufferSubData(GL_ARRAY_BUFFER, size * 4, size, tree_fruit1);
	glBufferSubData(GL_ARRAY_BUFFER, size * 5, size, tree_fruit2);

	glGenVertexArrays(1, &VAO_tree);
	glBindVertexArray(VAO_tree);

	glBindBuffer(GL_ARRAY_BUFFER, VBO_tree);
	glVertexAttribPointer(LOC_VERTEX, 2, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(0));

	glEnableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
}

void draw_tree() {
	glBindVertexArray(VAO_tree);

	for (int i = 0; i < 6; i++) {
		glUniform3fv(loc_primitive_color, 1, tree_color[i]);
		glDrawArrays(GL_TRIANGLE_FAN, i * 4, 4);
	}

	glBindVertexArray(0);
}

// bee
#define BEE_BODY 0
#define BEE_STRIPE1 1
#define BEE_STRIPE2 2
#define BEE_HEAD 3
#define BEE_WING_L 4
#define BEE_WING_R 5
#define BEE_ANTENNA_L 6
#define BEE_ANTENNA_R 7

GLfloat bee_body[4][2] = {
	{ -10.0f, 4.0f }, { -10.0f, -6.0f }, { 8.0f, -6.0f }, { 8.0f, 4.0f }
};

GLfloat bee_stripe1[4][2] = {
	{ -5.0f, 4.0f }, { -5.0f, -6.0f }, { -3.0f, -6.0f }, { -3.0f, 4.0f }
};

GLfloat bee_stripe2[4][2] = {
	{ 1.0f, 4.0f }, { 1.0f, -6.0f }, { 3.0f, -6.0f }, { 3.0f, 4.0f }
};

GLfloat bee_head[4][2] = {
	{ 8.0f, 3.0f }, { 8.0f, -5.0f }, { 14.0f, -5.0f }, { 14.0f, 3.0f }
};

GLfloat bee_wing_l[4][2] = {
	{ -6.0f, 5.0f }, { -10.0f, 13.0f }, { -2.0f, 13.0f }, { 0.0f, 5.0f }
};

GLfloat bee_wing_r[4][2] = {
	{ 0.0f, 5.0f }, { 3.0f, 12.0f }, { 10.0f, 10.0f }, { 5.0f, 4.0f }
};

GLfloat bee_antenna_l[4][2] = {
	{ 11.0f, 3.0f }, { 9.0f, 8.0f }, { 10.0f, 9.0f }, { 12.0f, 3.5f }
};

GLfloat bee_antenna_r[4][2] = {
	{ 13.0f, 3.0f }, { 16.0f, 8.0f }, { 17.0f, 7.0f }, { 14.0f, 2.5f }
};

GLfloat bee_color[8][3] = {
	{ 255 / 255.0f, 210 / 255.0f, 30 / 255.0f },
	{ 30 / 255.0f, 30 / 255.0f, 30 / 255.0f },
	{ 30 / 255.0f, 30 / 255.0f, 30 / 255.0f },
	{ 40 / 255.0f, 40 / 255.0f, 40 / 255.0f },
	{ 180 / 255.0f, 230 / 255.0f, 255 / 255.0f },
	{ 180 / 255.0f, 230 / 255.0f, 255 / 255.0f },
	{ 30 / 255.0f, 30 / 255.0f, 30 / 255.0f },
	{ 30 / 255.0f, 30 / 255.0f, 30 / 255.0f }
};

GLuint VBO_bee, VAO_bee;

void prepare_bee() {
	int size = sizeof(bee_body);
	GLsizeiptr buffer_size = size * 8;

	glGenBuffers(1, &VBO_bee);

	glBindBuffer(GL_ARRAY_BUFFER, VBO_bee);
	glBufferData(GL_ARRAY_BUFFER, buffer_size, NULL, GL_STATIC_DRAW);

	glBufferSubData(GL_ARRAY_BUFFER, size * 0, size, bee_body);
	glBufferSubData(GL_ARRAY_BUFFER, size * 1, size, bee_stripe1);
	glBufferSubData(GL_ARRAY_BUFFER, size * 2, size, bee_stripe2);
	glBufferSubData(GL_ARRAY_BUFFER, size * 3, size, bee_head);
	glBufferSubData(GL_ARRAY_BUFFER, size * 4, size, bee_wing_l);
	glBufferSubData(GL_ARRAY_BUFFER, size * 5, size, bee_wing_r);
	glBufferSubData(GL_ARRAY_BUFFER, size * 6, size, bee_antenna_l);
	glBufferSubData(GL_ARRAY_BUFFER, size * 7, size, bee_antenna_r);

	glGenVertexArrays(1, &VAO_bee);
	glBindVertexArray(VAO_bee);

	glBindBuffer(GL_ARRAY_BUFFER, VBO_bee);
	glVertexAttribPointer(LOC_VERTEX, 2, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(0));

	glEnableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
}

void draw_bee() {
	glBindVertexArray(VAO_bee);

	for (int i = 0; i < 8; i++) {
		glUniform3fv(loc_primitive_color, 1, bee_color[i]);
		glDrawArrays(GL_TRIANGLE_FAN, i * 4, 4);
	}

	glBindVertexArray(0);
}

// bird 
#define BIRD_BODY 0
#define BIRD_WING_L 1
#define BIRD_WING_R 2
#define BIRD_HEAD 3
#define BIRD_BEAK 4
#define BIRD_EYE 5
#define BIRD_TAIL 6
#define BIRD_LEG1 7
#define BIRD_LEG2 8

GLfloat bird_body[4][2] = {
	{ -15.0f, 6.0f }, { -15.0f, -9.0f }, { 12.0f, -9.0f }, { 12.0f, 6.0f }
};

GLfloat bird_wing_l[3][2] = {
	{ -6.0f, 4.5f }, { -21.0f, 15.0f }, { -15.0f, 0.0f }
};

GLfloat bird_wing_r[3][2] = {
	{ 3.0f, 4.5f }, { 15.0f, 13.5f }, { 12.0f, 0.0f }
};

GLfloat bird_head[4][2] = {
	{ 9.0f, 12.0f }, { 9.0f, 3.0f }, { 18.0f, 3.0f }, { 18.0f, 12.0f }
};

GLfloat bird_beak[3][2] = {
	{ 18.0f, 9.0f }, { 25.5f, 7.5f }, { 18.0f, 6.0f }
};

GLfloat bird_eye[4][2] = {
	{ 13.5f, 9.75f }, { 13.5f, 8.25f }, { 15.0f, 8.25f }, { 15.0f, 9.75f }
};

GLfloat bird_tail[3][2] = {
	{ -15.0f, 3.0f }, { -25.5f, 9.0f }, { -19.5f, -4.5f }
};

GLfloat bird_leg1[4][2] = {
	{ -4.5f, -9.0f }, { -4.5f, -16.5f }, { -3.0f, -16.5f }, { -3.0f, -9.0f }
};

GLfloat bird_leg2[4][2] = {
	{ 4.5f, -9.0f }, { 4.5f, -16.5f }, { 6.0f, -16.5f }, { 6.0f, -9.0f }
};

GLfloat bird_color[9][3] = {
	{ 80 / 255.0f, 170 / 255.0f, 230 / 255.0f },
	{ 120 / 255.0f, 200 / 255.0f, 250 / 255.0f },
	{ 120 / 255.0f, 200 / 255.0f, 250 / 255.0f },
	{ 80 / 255.0f, 170 / 255.0f, 230 / 255.0f },
	{ 255 / 255.0f, 170 / 255.0f, 30 / 255.0f },
	{ 20 / 255.0f, 20 / 255.0f, 20 / 255.0f },
	{ 50 / 255.0f, 130 / 255.0f, 200 / 255.0f },
	{ 90 / 255.0f, 70 / 255.0f, 50 / 255.0f },
	{ 90 / 255.0f, 70 / 255.0f, 50 / 255.0f }
};

GLuint VBO_bird, VAO_bird;

void prepare_bird() {
	GLsizeiptr buffer_size =
		sizeof(bird_body) +
		sizeof(bird_wing_l) +
		sizeof(bird_wing_r) +
		sizeof(bird_head) +
		sizeof(bird_beak) +
		sizeof(bird_eye) +
		sizeof(bird_tail) +
		sizeof(bird_leg1) +
		sizeof(bird_leg2);

	glGenBuffers(1, &VBO_bird);
	glBindBuffer(GL_ARRAY_BUFFER, VBO_bird);
	glBufferData(GL_ARRAY_BUFFER, buffer_size, NULL, GL_STATIC_DRAW);

	GLintptr offset = 0;

	glBufferSubData(GL_ARRAY_BUFFER, offset, sizeof(bird_body), bird_body);
	offset += sizeof(bird_body);

	glBufferSubData(GL_ARRAY_BUFFER, offset, sizeof(bird_wing_l), bird_wing_l);
	offset += sizeof(bird_wing_l);

	glBufferSubData(GL_ARRAY_BUFFER, offset, sizeof(bird_wing_r), bird_wing_r);
	offset += sizeof(bird_wing_r);

	glBufferSubData(GL_ARRAY_BUFFER, offset, sizeof(bird_head), bird_head);
	offset += sizeof(bird_head);

	glBufferSubData(GL_ARRAY_BUFFER, offset, sizeof(bird_beak), bird_beak);
	offset += sizeof(bird_beak);

	glBufferSubData(GL_ARRAY_BUFFER, offset, sizeof(bird_eye), bird_eye);
	offset += sizeof(bird_eye);

	glBufferSubData(GL_ARRAY_BUFFER, offset, sizeof(bird_tail), bird_tail);
	offset += sizeof(bird_tail);

	glBufferSubData(GL_ARRAY_BUFFER, offset, sizeof(bird_leg1), bird_leg1);
	offset += sizeof(bird_leg1);

	glBufferSubData(GL_ARRAY_BUFFER, offset, sizeof(bird_leg2), bird_leg2);

	glGenVertexArrays(1, &VAO_bird);
	glBindVertexArray(VAO_bird);

	glBindBuffer(GL_ARRAY_BUFFER, VBO_bird);
	glVertexAttribPointer(LOC_VERTEX, 2, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(0));

	glEnableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
}

void draw_bird() {
	glBindVertexArray(VAO_bird);

	glUniform3fv(loc_primitive_color, 1, bird_color[BIRD_BODY]);
	glDrawArrays(GL_TRIANGLE_FAN, 0, 4);

	glUniform3fv(loc_primitive_color, 1, bird_color[BIRD_WING_L]);
	glDrawArrays(GL_TRIANGLE_FAN, 4, 3);

	glUniform3fv(loc_primitive_color, 1, bird_color[BIRD_WING_R]);
	glDrawArrays(GL_TRIANGLE_FAN, 7, 3);

	glUniform3fv(loc_primitive_color, 1, bird_color[BIRD_HEAD]);
	glDrawArrays(GL_TRIANGLE_FAN, 10, 4);

	glUniform3fv(loc_primitive_color, 1, bird_color[BIRD_BEAK]);
	glDrawArrays(GL_TRIANGLE_FAN, 14, 3);

	glUniform3fv(loc_primitive_color, 1, bird_color[BIRD_EYE]);
	glDrawArrays(GL_TRIANGLE_FAN, 17, 4);

	glUniform3fv(loc_primitive_color, 1, bird_color[BIRD_TAIL]);
	glDrawArrays(GL_TRIANGLE_FAN, 21, 3);

	glUniform3fv(loc_primitive_color, 1, bird_color[BIRD_LEG1]);
	glDrawArrays(GL_TRIANGLE_FAN, 24, 4);

	glUniform3fv(loc_primitive_color, 1, bird_color[BIRD_LEG2]);
	glDrawArrays(GL_TRIANGLE_FAN, 28, 4);

	glBindVertexArray(0);
}

// sunflower
#define SUNFLOWER_PETAL_TOP 0
#define SUNFLOWER_PETAL_LEFT 1
#define SUNFLOWER_PETAL_RIGHT 2
#define SUNFLOWER_PETAL_BOTTOM 3
#define SUNFLOWER_CENTER 4
#define SUNFLOWER_STEM 5
#define SUNFLOWER_LEAF1 6
#define SUNFLOWER_LEAF2 7

GLfloat sunflower_petal_top[4][2] = {
	{ -4.5f, 21.0f }, { -4.5f, 13.5f }, { 4.5f, 13.5f }, { 4.5f, 21.0f }
};

GLfloat sunflower_petal_left[4][2] = {
	{ -12.0f, 13.5f }, { -12.0f, 4.5f }, { -4.5f, 4.5f }, { -4.5f, 13.5f }
};

GLfloat sunflower_petal_right[4][2] = {
	{ 4.5f, 13.5f }, { 4.5f, 4.5f }, { 12.0f, 4.5f }, { 12.0f, 13.5f }
};

GLfloat sunflower_petal_bottom[4][2] = {
	{ -4.5f, 4.5f }, { -4.5f, -3.0f }, { 4.5f, -3.0f }, { 4.5f, 4.5f }
};

GLfloat sunflower_center[4][2] = {
	{ -6.0f, 13.5f }, { -6.0f, 3.0f }, { 6.0f, 3.0f }, { 6.0f, 13.5f }
};

GLfloat sunflower_stem[4][2] = {
	{ -1.5f, -3.0f }, { -1.5f, -21.0f }, { 1.5f, -21.0f }, { 1.5f, -3.0f }
};

GLfloat sunflower_leaf1[4][2] = {
	{ 1.5f, -12.0f }, { 1.5f, -15.0f }, { 10.5f, -15.0f }, { 10.5f, -12.0f }
};

GLfloat sunflower_leaf2[4][2] = {
	{ -1.5f, -16.5f }, { -1.5f, -19.5f }, { -10.5f, -19.5f }, { -10.5f, -16.5f }
};

GLfloat sunflower_color[8][3] = {
	{ 255 / 255.0f, 180 / 255.0f, 0 / 255.0f },
	{ 255 / 255.0f, 180 / 255.0f, 0 / 255.0f },
	{ 255 / 255.0f, 180 / 255.0f, 0 / 255.0f },
	{ 255 / 255.0f, 180 / 255.0f, 0 / 255.0f },
	{ 80 / 255.0f, 70 / 255.0f, 60 / 255.0f },
	{ 40 / 255.0f, 130 / 255.0f, 70 / 255.0f },
	{ 40 / 255.0f, 130 / 255.0f, 70 / 255.0f },
	{ 40 / 255.0f, 130 / 255.0f, 70 / 255.0f }
};

GLuint VBO_sunflower, VAO_sunflower;

void prepare_sunflower() {
	int size = sizeof(sunflower_petal_top);
	GLsizeiptr buffer_size = size * 8;

	glGenBuffers(1, &VBO_sunflower);

	glBindBuffer(GL_ARRAY_BUFFER, VBO_sunflower);
	glBufferData(GL_ARRAY_BUFFER, buffer_size, NULL, GL_STATIC_DRAW);

	glBufferSubData(GL_ARRAY_BUFFER, size * 0, size, sunflower_petal_top);
	glBufferSubData(GL_ARRAY_BUFFER, size * 1, size, sunflower_petal_left);
	glBufferSubData(GL_ARRAY_BUFFER, size * 2, size, sunflower_petal_right);
	glBufferSubData(GL_ARRAY_BUFFER, size * 3, size, sunflower_petal_bottom);
	glBufferSubData(GL_ARRAY_BUFFER, size * 4, size, sunflower_center);
	glBufferSubData(GL_ARRAY_BUFFER, size * 5, size, sunflower_stem);
	glBufferSubData(GL_ARRAY_BUFFER, size * 6, size, sunflower_leaf1);
	glBufferSubData(GL_ARRAY_BUFFER, size * 7, size, sunflower_leaf2);

	glGenVertexArrays(1, &VAO_sunflower);
	glBindVertexArray(VAO_sunflower);

	glBindBuffer(GL_ARRAY_BUFFER, VBO_sunflower);
	glVertexAttribPointer(LOC_VERTEX, 2, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(0));

	glEnableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
}

void draw_sunflower() {
	glBindVertexArray(VAO_sunflower);

	for (int i = 0; i < 8; i++) {
		glUniform3fv(loc_primitive_color, 1, sunflower_color[i]);
		glDrawArrays(GL_TRIANGLE_FAN, i * 4, 4);
	}

	glBindVertexArray(0);
}
