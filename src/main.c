#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include <SDL2/SDL.h>

#include "logic.h"
#include "render.h"

#define SECONDS_TO_MICROSECONDS 1000000

void print_usage()
{
    printf("Usage: ./automata AUTOMATA\n");
    printf("     Langton's ant           -> langton\n");
    printf("     Conway's Game of Life   -> gameoflife\n");
    printf("     Brian's brain           -> briansbrain\n");
    printf("     Wireworld               -> wireworld\n");
    printf("     Falling Sand Simulator  -> sandsim\n");
}

int main(int argc, char **argv)
{
    int automata;//! automata defines the simulation we are running (see logic.h)
    char running_title[64] = {'\0'};
    char paused_title[64] = {'\0'};

    if (argc < 2) {
        print_usage();
        return EXIT_FAILURE;
    } else if (strcmp(argv[1], "langton") == 0) {
        automata = LANGTONS_ANT;
        strncat(running_title, "LANGTONS_ANT", 48);
        strncat(paused_title, "LANGTONS_ANT", 48);
    } else if (strcmp(argv[1], "gameoflife") == 0) {
        automata = GAME_OF_LIFE;
        strncat(running_title, "THE GAME OF LIFE", 48);
        strncat(paused_title, "THE GAME OF LIFE", 48);
    } else if (strcmp(argv[1], "briansbrain") == 0) {
        automata = BRIANS_BRAIN;
        strncat(running_title, "BRIAN'S BRAIN", 48);
        strncat(paused_title, "BRIAN'S BRAIN", 48);
    } else if (strcmp(argv[1], "wireworld") == 0) {
        automata = WIREWORLD;
        strncat(running_title, "WIREWORLD", 48);
        strncat(paused_title, "WIREWORLD", 48);
    } else if (strcmp(argv[1], "sandsim") == 0) {
        automata = FALLING_SAND_SIM;
        strncat(running_title, "FALLING SAND SIMULATOR", 48);
        strncat(paused_title, "FALLING SAND SIMULATOR", 48);
    } else {
        fprintf(stderr, "No such automata.\n");
        print_usage();
        return EXIT_FAILURE;
    }

    strncat(running_title, " - RUNNING", 48);
    strncat(paused_title, " - PAUSED", 48);

    if (SDL_Init(SDL_INIT_VIDEO) != 0) {
        fprintf(stderr, "SDL_INIT Error: %s\n", SDL_GetError());
        return EXIT_FAILURE;
    }

    SDL_Window *window = SDL_CreateWindow(running_title, SDL_WINDOWPOS_UNDEFINED,
                                                                                SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH,
                                                                                SCREEN_HEIGHT, SDL_WINDOW_SHOWN);

    if (window == NULL) {
        fprintf(stderr, "SDL_CreateWindow Error: %s\n", SDL_GetError());
        return EXIT_FAILURE;
    }

    SDL_Renderer *renderer = SDL_CreateRenderer(
            window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);

    if (renderer == NULL) {
        SDL_DestroyWindow(window);
        fprintf(stderr, "SDL_CreateRenderer Error: %s\n", SDL_GetError());
        return EXIT_FAILURE;
    }

    state_t state = {.mode = RUNNING_MODE};

     //! INIT BOARD AWITCHES THE GAME ACORDING TO THE AUTOMATA VARIABLES THAT WAS SELECTRED AT THE START
    // INIT BOARD
    switch (automata) {
        case LANGTONS_ANT:
            state.ant.x = N / 2;
            state.ant.y = N / 2;
            state.ant.dir = LEFT;
            break;

        case GAME_OF_LIFE:
            for (int x = 0; x < N; x++)
                for (int y = 0; y < N; y++)
                    state.board[x][y] = BLACK;

            // GLIDER
            state.board[N / 2][N / 2] = WHITE;
            state.board[N / 2 + 1][N / 2] = WHITE;
            state.board[N / 2 + 2][N / 2] = WHITE;
            state.board[N / 2 + 1][N / 2 - 2] = WHITE;
            state.board[N / 2 + 2][N / 2 - 1] = WHITE;
            break;

        case FALLING_SAND_SIM:
            for (int x = 0; x < N; x++)
                for (int y = 0; y < N-5; y++)
                    state.board[x][y] = AIR;

            for (int x = 0; x < N; x++)
                for (int y = N-3; y < N; y++)
                    state.board[x][y] = ROCK;               
            break;

        default:
            for (int x = 0; x < N; x++)
                for (int y = 0; y < N; y++)
                    state.board[x][y] = BLACK;

            break;
    }

    SDL_Event event;

    while (state.mode != QUIT_MODE) {
      //! while loop to search for events and handle them doing an action dependong on the game
        while (SDL_PollEvent(&event)) {
            switch (event.type) {
                case SDL_QUIT:
                    state.mode = QUIT_MODE;
                    break;

                case SDL_MOUSEBUTTONDOWN:
                    state.mode = PAUSED_MODE;
                    SDL_SetWindowTitle(window, paused_title);

                    int x = event.button.x / CELL_WIDTH;
                    int y = event.button.y / CELL_HEIGHT;
                    switch (automata) {
                        case GAME_OF_LIFE:
                            state.board[x][y] = (state.board[x][y] + 1) % 2;
                            break;
                        case BRIANS_BRAIN:
                            state.board[x][y] = (state.board[x][y] + 1) % 3;
                            break;
                        case WIREWORLD:
                            state.board[x][y] = (state.board[x][y] + 1) % 4;
                            break;
                        case FALLING_SAND_SIM:
                            state.board[x][y] = ((state.board[x][y] + 1 != 1 ? state.board[x][y] + 1 : state.board[x][y] + 2) + 3) % 7;
                            break;
                    }
                    break;

                case SDL_KEYDOWN:
                    if (event.key.keysym.sym == ' ') {
                        state.mode = RUNNING_MODE + PAUSED_MODE - state.mode;
                        SDL_SetWindowTitle(window, state.mode ? paused_title : running_title);
                    }
                    break;
            }
        }

        SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
        SDL_RenderClear(renderer);

        render_grid(renderer, &state);

        if (state.mode == RUNNING_MODE)
            usleep((1.0 / MOVES_PER_SECOND) * SECONDS_TO_MICROSECONDS);

        switch (automata) {
            case LANGTONS_ANT:
                langtons_ant(renderer, &state);
                break;

            case GAME_OF_LIFE:
                game_of_life(renderer, &state);
                break;

            case BRIANS_BRAIN:
                brians_brain(renderer, &state);
                break;

            case WIREWORLD:
                wireworld(renderer, &state);
                break;
            case FALLING_SAND_SIM:
                sand_sim(renderer, &state);
                break;
        }

        SDL_RenderPresent(renderer);
    }

    SDL_DestroyWindow(window);
    SDL_Quit();

    return EXIT_SUCCESS;
}
