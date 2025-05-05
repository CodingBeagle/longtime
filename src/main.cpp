#include <iostream>

// Add GLAD for OpenGL function loading
// I am using the header-only version, for that to work you need to define GLAD_GL_IMPLEMENTATION in a single source file.
// If you don't do this, you will end up with missing symbols.
#define GLAD_GL_IMPLEMENTATION // Necessary for headeronly version.
#include <glad/gl.h>

#include <SDL3/SDL.h>
#include <SDL3/SDL_main.h>

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
	SDL_Window* mainWindow = SDL_CreateWindow(
		"Longtime",
		800, 600,
		SDL_WINDOW_OPENGL);

	if (mainWindow == nullptr) {
		std::cerr << "SDL_CreateWindow Error: " << SDL_GetError() << std::endl;
		SDL_Quit();
		return 1;
	}

	// Create OpenGL context for the window
	SDL_GLContext glContext = SDL_GL_CreateContext(mainWindow);

	if (glContext == nullptr) {
		std::cerr << "SDL_GL_CreateContext Error: " << SDL_GetError() << std::endl;
		SDL_DestroyWindow(mainWindow);
		SDL_Quit();
		return 1;
	}

	// Load OpenGL functions using GLAD
	if (!gladLoadGL((GLADloadfunc)SDL_GL_GetProcAddress)) {
		std::cerr << "Failed to initialize GLAD" << std::endl;
		SDL_GL_DestroyContext(glContext);
		SDL_DestroyWindow(mainWindow);
		SDL_Quit();
		return 1;
	}

	// Set OpenGL viewport dimensions
	glViewport(0, 0, 800, 600);

	// Main loop
	bool running = true;
	while (running) {
		SDL_Event event;
		while (SDL_PollEvent(&event)) {
			if (event.type == SDL_EVENT_QUIT) {
				running = false;
			}
		}

		// Clear the screen
		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		// Swap the buffers
		SDL_GL_SwapWindow(mainWindow);
	}

	// Clean up
	SDL_GL_DestroyContext(glContext);
	SDL_DestroyWindow(mainWindow);
	SDL_Quit();
	
	return 0;
}