#include <iostream>
#include <string>
#include <vector>

// Add GLAD for OpenGL function loading
// I am using the header-only version, for that to work you need to define GLAD_GL_IMPLEMENTATION in a single source file.
// If you don't do this, you will end up with missing symbols.
#define GLAD_GL_IMPLEMENTATION // Necessary for headeronly version.
#include <glad/gl.h>

#include <SDL3/SDL.h>
#include <SDL3/SDL_main.h>

SDL_Window* mainWindow;
SDL_GLContext glContext;

const char* vertexShaderSource = "#version 330 core\n"
    "layout (location = 0) in vec3 aPos;\n"
    "void main()\n"
    "{\n"
    "   gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
	"}\0";

const char* fragmentShaderSource = "#version 330 core\n"
	"out vec4 FragColor;\n"
	"void main()\n"
	"{\n"
	"   FragColor = vec4(1.0f, 0.5f, 0.2f, 1.0f);\n"
	"}\0";

float vertices[] = {
	-0.5f, -0.5f, 0.0f,
	 0.5f, -0.5f, 0.0f,
	 0.0f,  0.5f, 0.0f
};

// Function Prototypes
void PanicTerminate(std::string message);
void SdlCleanup();

unsigned int createAndCompileShader(GLenum shaderType, const char* shaderSource) {
	// Create and compile shader
	unsigned int shader = glCreateShader(shaderType);
	glShaderSource(shader, 1, &shaderSource, nullptr);
	glCompileShader(shader);

	// Check for compilation errors
	int success;
	char infoLog[512];
	glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
	if (!success) {
		// The returned info log is null-terminated so long as the buffer is large enough
		glGetShaderInfoLog(shader, 512, nullptr, infoLog);
		PanicTerminate("Shader Compilation Error: " + std::string(infoLog));
	}

	return shader;
}

unsigned int createShaderProgram(const std::vector<unsigned int>& shaders) {
	// Create a shader program
	unsigned int shaderProgram = glCreateProgram();

	// Attach all shaders to the program
	for (const auto& shader : shaders) {
		glAttachShader(shaderProgram, shader);
	}

	// Link the program
	glLinkProgram(shaderProgram);

	// Check for linking errors
	int success;
	char infoLog[512];
	glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
	if (!success) {
		glGetProgramInfoLog(shaderProgram, 512, nullptr, infoLog);
		PanicTerminate("Shader Program Linking Error: " + std::string(infoLog));
	}

	// Detach and delete shaders after linking, as they will no longer be required
	for (const auto& shader : shaders) {
		glDetachShader(shaderProgram, shader);
	}

	return shaderProgram;
}

int main(int argc, char* argv[])
{
	// Initialize SDL
	if (SDL_Init(SDL_INIT_VIDEO) != true) {
		std::cerr << "SDL_Init Error: " << SDL_GetError() << std::endl;
		return 1;
	}

    // Specify OpenGL version before creating the OpenGL context
	// SDL_GL_SetAttribute is used to set an OpenGL window attribute before window creation
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3); // Set OpenGL major version to 4
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3); // Set OpenGL minor version to 6
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE); // Use the core profile

	// Create a window
	// SDL_WINDOW_OPENGL flag makes sure that the OpenGL library is dynamically loaded
	mainWindow = SDL_CreateWindow(
		"Longtime",
		800, 600,
		SDL_WINDOW_OPENGL);

	if (mainWindow == nullptr) {
		PanicTerminate("SDL_CreateWindow Error: " + std::string(SDL_GetError()));
	}

	// Create OpenGL context for the window
	glContext = SDL_GL_CreateContext(mainWindow);
	if (glContext == nullptr) {
		PanicTerminate("SDL_GL_CreateContext Error: " + std::string(SDL_GetError()));
	}

	// Load OpenGL functions using GLAD
	if (!gladLoadGL((GLADloadfunc)SDL_GL_GetProcAddress)) {
		PanicTerminate("Failed to initialize GLAD");
	}

	// Set OpenGL viewport dimensions
	glViewport(0, 0, 800, 600);

	// Enable OpenGL debug output
	glEnable(GL_DEBUG_OUTPUT);
	glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);

	// Debug callback function
	glDebugMessageCallback(
		[](GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length, const GLchar* message, const void* userParam) {
			std::cout << "[OpenGL Debug] "
					  << "Source: " << source << ", Type: " << type
					  << ", ID: " << id << ", Severity: " << severity
					  << "\nMessage: " << message << std::endl;
		},
		nullptr
	);

	// Optionally, filter messages by severity
	glDebugMessageControl(GL_DONT_CARE, GL_DONT_CARE, GL_DONT_CARE, 0, nullptr, GL_TRUE);

	// Create a Vertex Array Object (VAO)
	// A VAO is an object that stores:
	// - Calls to glEnableVertexAttribArray (or glDisableVertexAttribArray)
	// - Calls to glVertexAttribPointer
	// - VBOs associated with vertex attributes by calls to gLVertexAttribPointer
	// A VAO makes it easier to switch between different vertex attribute configurations
	unsigned int VAO{};
	glGenVertexArrays(1, &VAO);

	// After binding our VAO, we can "record" all necessary calls to set up the vertex attributes
	glBindVertexArray(VAO);

	// Generate vertex buffer object used to store vertex attributes on the GPU
	unsigned int VBO{};
	glGenBuffers(1, &VBO);

	// We bind the buffer to the GL_ARRAY_BUFFER target.
	// A target defines the role the buffer will have in the pipeline.
	// GL_ARRAY_BUFFER = Vertex attribute data.
	glBindBuffer(GL_ARRAY_BUFFER, VBO);

	// Copy the vertex data to the buffer's memory.
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	// We describe the output of our data for location 0 in the vertex shader.
	// Each component has 3 elements of type FLOAT.
	// The data is tightly packed, and there's 4 bytes times 3 for each component (3 floats).
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);

	// Vertex attributes are disabled by default and has to be enabled.
	glEnableVertexAttribArray(0);

	// Create vertex shader
	unsigned int vertexShader = createAndCompileShader(GL_VERTEX_SHADER, vertexShaderSource);

	// Create fragment shader
	unsigned int fragmentShader = createAndCompileShader(GL_FRAGMENT_SHADER, fragmentShaderSource);

	// Create shader program
	unsigned int shaderProgram = createShaderProgram({ vertexShader, fragmentShader });

	glUseProgram(shaderProgram);

	// Unbind the VAO
	// We can now use it later for drawing calls
	glBindVertexArray(0);

	// Shader cleanup
	// After creating and linking the shader program, the shaders are not needed any longer
	// Best practice is to clean up after yourself ;)
	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);

	// Unbind the shader program until we want to use it
	glUseProgram(0);

	// Main loop
	bool running = true;
	while (running) {

		// Input
		SDL_Event event;
		while (SDL_PollEvent(&event)) {
			if (event.type == SDL_EVENT_QUIT) {
				running = false;
			}

			// For the escape key, we want to use SDL Keycodes, in which we care not about location of the key on the keyboard
			// but about the actual symbol of the key. We want the escape key when exiting, no matter where it is located on the keyboard.
			if (event.type == SDL_EVENT_KEY_DOWN) {
				if (event.key.key == SDLK_ESCAPE) {
					running = false;
				}
			}
		}

		// Rendering

		// Clear the screen
		glClearColor(0.0f, 0.392f, 0.584f, 0.929f);
		glClear(GL_COLOR_BUFFER_BIT);

		glUseProgram(shaderProgram);
		glBindVertexArray(VAO);
		glDrawArrays(GL_TRIANGLES, 0, 3);

		// Swap the buffers
		SDL_GL_SwapWindow(mainWindow);
	}

	// Clean up
	SdlCleanup();
	
	return 0;
}

void PanicTerminate(std::string message)
{
	std::cerr << message << std::endl;
	SdlCleanup();
	std::terminate();
}

void SdlCleanup()
{
	if (glContext != nullptr) {
		SDL_GL_DestroyContext(glContext);
	}
	
	if (mainWindow != nullptr) {
		SDL_DestroyWindow(mainWindow);
	}

	SDL_Quit();
}