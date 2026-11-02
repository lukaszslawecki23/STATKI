#define SDL_MAIN_USE_CALLBACKS 1
#include <SDL3/SDL.h>
#include <SDL3/SDL_main.h>
#include "board.hpp"

struct AppState {
    SDL_Window* window = nullptr;
    SDL_Renderer* renderer = nullptr;

    Board playerBoard{190, 90};
};

SDL_AppResult SDL_AppInit(void** appstate, int argc, char* argv[]) {
    if (!SDL_Init(SDL_INIT_VIDEO)) {
        SDL_Log("Błąd inicjalizacji SDL: %s", SDL_GetError());
        return SDL_APP_FAILURE;
    }

    AppState* state = new AppState();
    if (!SDL_CreateWindowAndRenderer("Statki", 1920, 1080, SDL_LOGICAL_PRESENTATION_LETTERBOX, &state->window, &state->renderer)) {
        SDL_Log("Błąd tworzenia okna: %s", SDL_GetError());
        return SDL_APP_FAILURE;
    }

    *appstate = state;
    return SDL_APP_CONTINUE;
}

SDL_AppResult SDL_AppEvent(void* appstate, SDL_Event* event) {
    AppState* state = static_cast<AppState*>(appstate);

    if (event->type == SDL_EVENT_QUIT) {
        return SDL_APP_SUCCESS;
    }

    if (event->type == SDL_EVENT_MOUSE_BUTTON_DOWN && event->button.button == SDL_BUTTON_LEFT)
    {
        state->playerBoard.handleClick(event->button.x, event->button.y);
    }
    return SDL_APP_CONTINUE;
}

SDL_AppResult SDL_AppIterate(void* appstate) {
    AppState* state = static_cast<AppState*>(appstate);

    SDL_SetRenderDrawColor(state->renderer, 25, 130, 200, SDL_ALPHA_OPAQUE);
    SDL_RenderClear(state->renderer);

    state->playerBoard.render(state->renderer);

    SDL_RenderPresent(state->renderer);
    return SDL_APP_CONTINUE;
}

void SDL_AppQuit(void* appstate, SDL_AppResult result) {
    AppState* state = static_cast<AppState*>(appstate);
    if (state) {
        SDL_DestroyRenderer(state->renderer);
        SDL_DestroyWindow(state->window);
        delete state;
    }
}