#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include "Renderer.h"
#include "VertexBuffer.h"
#include "IndexBuffer.h"
#include "VertexArray.h"
#include "VertexBufferLayout.h"
#include "Shader.h"
#include "Texture.h"

#include <iostream>

int main()
{
	if (!glfwInit())
		return -1;

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	GLFWwindow* window = glfwCreateWindow(640, 480, "Modern OpenGL", nullptr, nullptr);

	if (!window)
	{
		glfwTerminate();
		return -1;
	}

	glfwMakeContextCurrent(window);

	glfwSwapInterval(1);

	GLenum err = glewInit();

	if (err != GLEW_OK)
	{
		std::cerr << "Failed to initialize GLEW: " << glewGetErrorString(err) << '\n';

		glfwDestroyWindow(window);
		glfwTerminate();
		return -1;
	}

	float positions[] = {
	-0.5f, -0.5f, 0.0f, 0.0f,
	 0.5f,  -0.5f, 1.0f, 0.0f,
	 0.5f, 0.5f, 1.0f, 1.0f,
	 -0.5f, 0.5f, 0.0f, 1.0f
	};

	unsigned int indices[] =
	{
		0,1,2,
		2,3,0
	};

	GLCALL(glEnable(GL_BLEND));
	GLCALL(glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA));

	VertexArray va;
	VertexBuffer vb(positions, 4 * 4 * sizeof(float));

	VertexBufferLayout layout;
	layout.Push<float>(2);
	layout.Push<float>(2);
	va.AddBuffer(vb, layout);

	IndexBuffer ib(indices, std::size(indices));

	Shader shader("res/shaders/Basic.shader");
	shader.Bind();
	shader.SetUniform4f("u_Color", 1.0f, 0.3f, 0.8f, 1.0f);

	Texture texture("res/textures/ShriKrishna.png");
	texture.Bind();
	shader.SetUniform1i("u_Texture", 0);

	va.Unbind();
	vb.Unbind();
	ib.Unbind();
	shader.UnBind();

	Renderer renderer;

	float red_component = 0.0f;
	float increment = 0.05f;

	while (!glfwWindowShouldClose(window))
	{
		renderer.Clear();
		shader.Bind();
		shader.SetUniform4f("u_Color", red_component, 0.3f, 0.8f, 1.0f);

		renderer.Draw(va, ib, shader);

		if (red_component > 1.0f)
			increment = -0.05f;
		else if (red_component < 0.0f)
			increment = 0.05f;

		red_component += increment;

		glfwSwapBuffers(window);

		glfwPollEvents();
	}

	glfwDestroyWindow(window);
	glfwTerminate();

	return 0;
}