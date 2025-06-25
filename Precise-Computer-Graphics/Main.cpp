#include<iostream>
#include<glad/glad.h>
#include<GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <vector>

const char* vertexShaderSource = R"glsl(
#version 330 core
layout(location = 0) in vec3 aPos;
layout(location = 1) in vec3 aColor;

out vec3 ourColor;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main()
{
    gl_Position = projection * view * model * vec4(aPos, 1.0);
    ourColor = aColor;
}

)glsl";

const char* fragmentShaderSource = R"glsl(
#version 330 core
in vec3 ourColor;
out vec4 FragColor;

void main()
{
    FragColor = vec4(ourColor, 1.0);
}
)glsl";


glm::vec3 cameraPos = glm::vec3(0.0f, 0.0f, 3.0f);
glm::vec3 cameraFront = glm::vec3(0.0f, 0.0f, -1.0f);
glm::vec3 cameraUp = glm::vec3(0.0f, 1.0f, 0.0f);

glm::vec3 cameraOffset(0.0f, 0.0f, 0.0f);


float yaw = -90.0f, pitch = 0.0f;
float lastX = 400, lastY = 300;
bool firstMouse = true;
float fov = 45.0f;


float axisVertices[] = {
	// X axis (red)
	0.0f, 0.0f, 0.0f,   1.0f, 0.0f, 0.0f,
	1.0f, 0.0f, 0.0f,   1.0f, 0.0f, 0.0f,

	// Y axis (green)
	0.0f, 0.0f, 0.0f,   0.0f, 1.0f, 0.0f,
	0.0f, 1.0f, 0.0f,   0.0f, 1.0f, 0.0f,

	// Z axis (blue)
	0.0f, 0.0f, 0.0f,   0.0f, 0.0f, 1.0f,
	0.0f, 0.0f, 1.0f,   0.0f, 0.0f, 1.0f,
};




void processInput(GLFWwindow* window, float deltaTime)
{
	float cameraSpeed = 2.5f * deltaTime;
	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
		cameraPos += cameraSpeed * cameraFront;
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
		cameraPos -= cameraSpeed * cameraFront;
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
		cameraPos -= glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraSpeed;
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
		cameraPos += glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraSpeed;
}

void mouse_callback(GLFWwindow* window, double xpos, double ypos)
{
	static bool dragging = false;
	static float lastX = 0, lastY = 0;

	if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_MIDDLE) == GLFW_PRESS)
	{
		if (!dragging)
		{
			dragging = true;
			lastX = xpos;
			lastY = ypos;
			return;
		}

		float xoffset = xpos - lastX;
		float yoffset = ypos - lastY;

		lastX = xpos;
		lastY = ypos;

		float sensitivity = 0.005f;

		// Przesuwaj scenê w kierunku X/Y kamery
		cameraOffset -= glm::vec3(xoffset * sensitivity, -yoffset * sensitivity, 0.0f);
	}
	else
	{
		dragging = false;
	}
}

std::vector<float> generateGrid(int halfSize, float spacing = 1.0f) {
	std::vector<float> vertices;

	for (int i = -halfSize; i <= halfSize; ++i) {
		float coord = i * spacing;

		// linie równoleg³e do X
		vertices.insert(vertices.end(), {
			-halfSize * spacing, 0.0f, coord,  0.5f, 0.5f, 0.5f,
			 halfSize * spacing, 0.0f, coord,  0.5f, 0.5f, 0.5f
			});

		// linie równoleg³e do Z
		vertices.insert(vertices.end(), {
			coord, 0.0f, -halfSize * spacing,  0.5f, 0.5f, 0.5f,
			coord, 0.0f,  halfSize * spacing,  0.5f, 0.5f, 0.5f
			});
	}

	return vertices;
}



int main()
{
	glfwInit();

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	GLfloat vertices[] = {
		-0.5f, -0.5f * float(sqrt(3)), 0.0f, // Bottom Left
		 0.5f, -0.5f * float(sqrt(3)), 0.0f, // Bottom Right
		 0.0f,  0.5f * float(sqrt(3)) * 2/3, 0.0f  // Top
	};

	GLFWwindow* window = glfwCreateWindow(800, 600, "OpenGL Demo", NULL, NULL);
	if (window == NULL) {
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);

	gladLoadGL();

	glViewport(0, 0, 800, 600);
	glEnable(GL_DEPTH_TEST);


	GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
	glCompileShader(vertexShader);

	GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
	glCompileShader(fragmentShader);


	GLuint shaderProgram = glCreateProgram();
	glAttachShader(shaderProgram, vertexShader);
	glAttachShader(shaderProgram, fragmentShader);

	glLinkProgram(shaderProgram);

	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);


	GLuint VBO, VAO;

	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	
	glBindVertexArray(VAO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);

	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid*)0);
	glEnableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
	
	glClearColor(0.12f, 0.13f, 0.13f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT);
	//
	GLuint axisVAO, axisVBO;
	glGenVertexArrays(1, &axisVAO);
	glGenBuffers(1, &axisVBO);

	glBindVertexArray(axisVAO);
	glBindBuffer(GL_ARRAY_BUFFER, axisVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(axisVertices), axisVertices, GL_STATIC_DRAW);

	// pozycja
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	// kolor
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);

	std::vector<float> gridVertices = generateGrid(20); // 41 linii w ka¿d¹ stronê

	GLuint gridVAO, gridVBO;
	glGenVertexArrays(1, &gridVAO);
	glGenBuffers(1, &gridVBO);

	glBindVertexArray(gridVAO);
	glBindBuffer(GL_ARRAY_BUFFER, gridVBO);
	glBufferData(GL_ARRAY_BUFFER, gridVertices.size() * sizeof(float), gridVertices.data(), GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);



	glfwSwapBuffers(window);


	
	glfwSetCursorPosCallback(window, mouse_callback);
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);


	float deltaTime = 0.0f;
	float lastFrame = 0.0f;

	while (!glfwWindowShouldClose(window))
	{
		float currentFrame = glfwGetTime();
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;

		processInput(window, deltaTime);

		// === Poprawna kolejnoœæ ===
		glClearColor(0.12f, 0.13f, 0.13f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		// Matryce
		glm::mat4 model = glm::mat4(1.0f);
		glm::mat4 view = glm::lookAt(cameraPos + cameraOffset, cameraPos + cameraOffset + cameraFront, cameraUp);
		glm::mat4 projection = glm::perspective(glm::radians(fov), 800.0f / 600.0f, 0.1f, 100.0f);

		// Za³aduj do shaderów
		glUseProgram(shaderProgram);
		glUniformMatrix4fv(glGetUniformLocation(shaderProgram, "model"), 1, GL_FALSE, glm::value_ptr(model));
		glUniformMatrix4fv(glGetUniformLocation(shaderProgram, "view"), 1, GL_FALSE, glm::value_ptr(view));
		glUniformMatrix4fv(glGetUniformLocation(shaderProgram, "projection"), 1, GL_FALSE, glm::value_ptr(projection));

		// Rysuj siatkê
		glBindVertexArray(gridVAO);
		glDrawArrays(GL_LINES, 0, gridVertices.size() / 6);

		// Rysuj osie
		glBindVertexArray(axisVAO);
		glDrawArrays(GL_LINES, 0, 6);

		// Rysuj trójk¹t
		glBindVertexArray(VAO);
		glDrawArrays(GL_TRIANGLES, 0, 3);

		glfwSwapBuffers(window);
		glfwPollEvents();

		if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
			glfwSetWindowShouldClose(window, true);
	}

	glDeleteBuffers(1, &VBO);
	glDeleteVertexArrays(1, &VAO);
	glDeleteProgram(shaderProgram);
	glfwDestroyWindow(window);
	glfwTerminate();
	return 0;
}