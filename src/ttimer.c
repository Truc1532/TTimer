#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <unistd.h>
#include <termios.h>
#include <string.h>

typedef struct {
	uint64_t time;
	uint8_t paused;
} State;

void enableRawMode() {
	struct termios term;
	tcgetattr(STDIN_FILENO, &term);
	term.c_lflag &= ~(ECHO | ICANON);
	tcsetattr(STDIN_FILENO, TCSANOW, &term);
}

void disableRawMode() {
	struct termios term;
	tcgetattr(STDIN_FILENO, &term);
	term.c_lflag |= (ECHO | ICANON);
	tcsetattr(STDIN_FILENO, TCSANOW, &term);
}

int kbhit() {
	struct timeval tv = { 0L, 0L };
	fd_set fds;
	FD_ZERO(&fds);
	FD_SET(STDIN_FILENO, &fds);
	return select(1, &fds, NULL, NULL, &tv);
}

void displayTimer(uint64_t time) {
	printf("\033[2K\rMili seconds: %lu, Seconds: %lu, Minutes: %lu, Hours, %lu",
		time % 1000, (time / 1000) % 60, time / 60000, time / 3600000);
	fflush(stdout);
}

int main(int argc, char *argv[]) {
	State *state = (State *) calloc(1, sizeof(State));
	
	for (int i = 1; i < argc; i++) {
		if (strcmp(argv[i], "--version") == 0) {
			printf("TTimer (Terminal Timer) 1.0\n");
			free(state);
			return 0;
		} else if (strcmp(argv[i], "--help") == 0) {
			printf("r - reset the timer\n");
			printf("p - pause the timer\n");
			printf("q - exit\n");
			free(state);
			return 0;
		}
	}

	enableRawMode();

	while (1) {
		if (!state->paused) {
			state->time += 1;
			usleep(1000);
			displayTimer(state->time);
			fflush(stdout);
		} else {
			state->time += 0;
		}

		if (kbhit()) {
			char c = getchar();
			if (c == 'p') {
				state->paused = !state->paused;
			} else if (c == 'r') {
				state->time = 0;
				displayTimer(state->time);
			} else if (c == 'q') {
				break;
			}
		}
	}

	disableRawMode();
	printf("\n");
	free(state);
	return 0;
}



		
