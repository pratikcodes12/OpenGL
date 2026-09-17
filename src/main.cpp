#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <cassert>

#define ASSERT(x) if (!(x)) __debugbreak();
#define GLCALL(x) GLClearError();\
	x;\
	ASSERT(GLLogCall(#x, __FILE__, __LINE__))

static void GLClearError()
{
	while (glGetError() != GL_NO_ERROR);
}

static bool GLLogCall(const char* function, const char* file, int line)
{
	while (GLenum error = glGetError())
	{
		std::cout << "[OpenGL Error] (" << error << ")" << function << " " << file << ":" << line << std::endl;
		return false;
	}
	return true;
}

struct ShaderProgramSource
{
	std::string vertexSource;
	std::string fragmentSource;
};

static ShaderProgramSource parseShader(const std::string& filepath)
{
	std::ifstream stream(filepath);
	enum class ShaderType
	{
		NONE = -1, VERTEX = 0, FRAGMENT = 1
	};

	std::stringstream ss[2];
	std::string line;
	ShaderType type = ShaderType::NONE;
	while (getline(stream, line))
	{
		if (line.find("#shader") != std::string::npos)
		{
			if (line.find("vertex") != std::string::npos)
				type = ShaderType::VERTEX;
			else if (line.find("fragment") != std::string::npos)
				type = ShaderType::FRAGMENT;
		}
		else
		{
			ss[int(type)] << line << '\n';
		}
	}

	return { ss[0].str(), ss[1].str() };
}

static unsigned int compileShader(unsigned int type, const std::string& source)
{
	unsigned int id = glCreateShader(type);
	const char* src = source.c_str();
	glShaderSource(id, 1, &src, nullptr);
	glCompileShader(id);

	// Error handling
	int result;
	glGetShaderiv(id, GL_COMPILE_STATUS, &result);
	if (result == GL_FALSE)
	{
		int length;
		glGetShaderiv(id, GL_INFO_LOG_LENGTH, &length);
		char* massage = (char*)alloca(length * sizeof(char));
		glGetShaderInfoLog(id, length, &length, massage);
		std::cout << "Failed to compile Shader!" << (type == GL_VERTEX_SHADER ? "Vertex" : "Fragment") << " shader compilation failed: " << std::endl;
		std::cout << massage << std::endl;
		glDeleteShader(id);
		return 0;
	}

	return id;
}

static unsigned int createShader(const std::string& vertexShader, const std::string& fragmentShader)
{
	unsigned int program = glCreateProgram();
	unsigned int vs = compileShader(GL_VERTEX_SHADER, vertexShader);
	unsigned int fs = compileShader(GL_FRAGMENT_SHADER, fragmentShader);

	glAttachShader(program, vs);
	glAttachShader(program, fs);
	glLinkProgram(program);
	glValidateProgram(program);

	glDeleteShader(vs);
	glDeleteShader(fs);

	return program;
}

int main()
{
	// Initialize GLFW
	if (!glfwInit())
		return -1;

	// Request an OpenGL 4.6 Core Profile context
	//glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	//glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
	//glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	// Create a window and OpenGL context
	GLFWwindow* window = glfwCreateWindow(640, 480, "Modern OpenGL", nullptr, nullptr);

	if (!window)
	{
		glfwTerminate();
		return -1;
	}

	// Make the OpenGL context current
	glfwMakeContextCurrent(window);

	glfwSwapInterval(1);

	GLenum err = glewInit();

	if (err != GLEW_OK)
	{
		std::cerr << "Failed to initialize GLEW: "
			<< glewGetErrorString(err) << '\n';

		glfwDestroyWindow(window);
		glfwTerminate();
		return -1;
	}

	float positions[] = {
	-0.5f, -0.5f,
	 0.5f,  -0.5f,
	 0.5f, 0.5f,
	 -0.5f, 0.5f,
	};

	unsigned int indices[] =
	{
		0,1,2,
		2,3,0
	};

	unsigned int vao;
	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);

	unsigned int buffer;
	glGenBuffers(1, &buffer);
	glBindBuffer(GL_ARRAY_BUFFER, buffer);
	glBufferData(GL_ARRAY_BUFFER, 6 * 2 * sizeof(float), positions, GL_STATIC_DRAW);

	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 2, 0);

	unsigned int ibo;
	glGenBuffers(1, &ibo);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, 6 * sizeof(unsigned int), indices, GL_STATIC_DRAW);

	ShaderProgramSource source = parseShader("res/shaders/Basic.shader");

	std::cout << "Vertex Shader Source:\n" << source.vertexSource << std::endl;
	std::cout << "Fragment Shader Source:\n" << source.fragmentSource << std::endl;

	unsigned int shader = createShader(source.vertexSource, source.fragmentSource);
	glUseProgram(shader);

	int location = glGetUniformLocation(shader, "u_Color");
	assert(location != -1);
	GLCALL(glUniform4f(location, 1.0f, 0.3f, 0.8f, 1.0f));

	float red_component = 0.0f;
	float increment = 0.05f;

	// Main loop
	while (!glfwWindowShouldClose(window))
	{
		// Clear the screen
		glClear(GL_COLOR_BUFFER_BIT);

		//glBegin(GL_TRIANGLES);
		//glVertex2f(-0.5f, -0.5f);
		//glVertex2f(0.0f, 0.5f);
		//glVertex2f(0.5f, -0.5f);
		//glEnd();

		GLCALL(glUniform4f(location, red_component, 0.3f, 0.8f, 1.0f));
		GLCALL(glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr));

		if (red_component > 1.0f)
			increment = -0.05f;
		else if (red_component < 0.0f)
			increment = 0.05f;

		red_component += increment;

		// Display the rendered frame
		glfwSwapBuffers(window);

		// Process keyboard/window events
		glfwPollEvents();
	}

	glDeleteProgram(shader);

	// Cleanup
	glfwDestroyWindow(window);
	glfwTerminate();

	return 0;
}