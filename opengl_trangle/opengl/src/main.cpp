#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <iostream>

static unsigned int CompileShader(unsigned int type, const std::string& source)
{
	unsigned int id = glCreateShader(type);
	const char * src = source.c_str();
	glShaderSource(id, 1, &src, nullptr);
	glCompileShader(id);
	// to do error handerling
	int result;
	glGetShaderiv(id, GL_COMPILE_STATUS, &result);
	if (result == GL_FALSE)
	{
		int length;
		glGetShaderiv(id, GL_INFO_LOG_LENGTH, &length);
		char* message = (char* ) alloca(length * sizeof(char));
		glGetShaderInfoLog(id, length, &length, message);
		std::cout << "Fail to compiel " << (type==GL_VERTEX_SHADER ? "vertex": "fragement") << "shader!" <<  std::endl;
		std::cout << message << std::endl;


	}

	return id;
}
static unsigned int CreateShader(const std::string& vertexShader, const std::string& fragmentShader)
{
	unsigned int program = glCreateProgram();
	unsigned int vs = CompileShader(GL_VERTEX_SHADER, vertexShader);
	unsigned int fs = CompileShader(GL_FRAGMENT_SHADER, fragmentShader);
	glAttachShader(program, vs);
	glAttachShader(program, fs);
	glValidateProgram(program);

	// importtank else still white.. 
	glLinkProgram(program);
	// important
	glDeleteShader(fs);
	glDeleteShader(vs);
	return program;
}

int main(void)
{
	GLFWwindow* window;

	/* Initialize the library */
	if (!glfwInit())
		return -1;



	/* Create a windowed mode window and its OpenGL context */
	window = glfwCreateWindow(640, 480, "Hello World", NULL, NULL);
	if (!window)
	{
		glfwTerminate();
		return -1;
	}

	/* Make the window's context current */
	glfwMakeContextCurrent(window);

	if (glewInit() != GLEW_OK)
		std::cout << "Error !" << std::endl;
	float positions[6] = {
					-0.5f,-0.5f,  //x coodinate y coodinate
					0.0f, 0.5f,
					0.5f, -0.5f
	};
	unsigned int buffer;

	std::cout << glGetString(GL_VERSION) << std::endl;


	glGenBuffers(1, &buffer);
	glBindBuffer(GL_ARRAY_BUFFER, buffer);   // which to be draw
	glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 6, positions, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 2, 0);

	glEnableVertexAttribArray(0);

	std::string vertexShader =
		"#version 410 core \n"
		"\n"
		"layout(location =0) in vec4 position;"
		"\n"
		"void main() \n"
		"{\n"
		"    gl_Position= position; \n"

		"}\n";


	std::string fragmentShader =
		"#version 410 core \n"
		"\n"
		"layout(location =0) out vec4 color; \n"
		"\n"
		"void main()\n"
		"{\n"
		"    color= vec4(1.0,0.5,0.0,1.0); \n" 

		"}\n";

	unsigned int shader = CreateShader(vertexShader, fragmentShader);
	
	glUseProgram(shader);
	/* Loop until the user closes the window */
	while (!glfwWindowShouldClose(window))
	{
		/* Render here */
		glClear(GL_COLOR_BUFFER_BIT);

		glDrawArrays(GL_TRIANGLES, 0, 3);  //
		//glDrawElements(GL_TRIANGLES,3,)


		
	/*	glBegin(GL_TRIANGLES);
		glVertex2f(-0.5f,-0.5f);
		glVertex2f(0.0f, 0.5f);
		glVertex2f(0.5f, -0.5f);
		glEnd();
		*/
		
		/* Swap front and back buffers */
		glfwSwapBuffers(window);

		/* Poll for and process events */
		glfwPollEvents();
	}

	glDeleteShader(shader);
	glfwTerminate();
	return 0;
}
