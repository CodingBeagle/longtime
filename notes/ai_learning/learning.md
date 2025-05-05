# AI Learning

## Examples of silly mistakes

I let GitHub CoPilot autocomplete initialization code for the SDL library, which looked like this:
```c
if (SDL_Init(SDL_INIT_VIDEO) != 0) {
	std::cerr << "SDL_Init Error: " << SDL_GetError() << std::endl;
	return 1;
}
```

But of course, **SDL_Init** returns true on success, which converts to 1 in C++. See the problem?