// score.c
// EECS 348 Lab 8 - Task 1
// Enumerate all play-combination sums for a given NFL score.
// Re-prompts until the user enters 1 to stop.
// Handles invalid input (non-numeric, negative, 0, etc.).
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>

static int read_int_line(const char *prompt, int *out_value) {
    char buf[256];
    for (;;) {
        if (prompt) {
            fputs(prompt, stdout);
            fflush(stdout);
        }
        if (!fgets(buf, sizeof buf, stdin)) {
            return 0; // EOF or error
        }
        // Trim leading spaces
        char *p = buf;
        while (isspace((unsigned char)*p)) p++;

        // Empty line? reprompt
        if (*p == '\0' || *p == '\n') {
            fprintf(stderr, "Empty input. Try again.\n");
            continue;
        }

        // Parse integer; ensure no junk after number except whitespace
        char *end = NULL;
        long val = strtol(p, &end, 10);
        if (end == p) {
            fprintf(stderr, "Invalid number. Try again.\n");
            continue;
        }
        while (isspace((unsigned char)*end)) end++;
        if (*end != '\0' && *end != '\n') {
            fprintf(stderr, "Unexpected characters after number. Try again.\n");
            continue;
        }
        if (val < -2147483648L || val > 2147483647L) {
            fprintf(stderr, "Number out of range. Try again.\n");
            continue;
        }
        *out_value = (int)val;
        return 1;
    }
}

int main(void) {
    const int TD2 = 8; // TD + 2pt
    const int TDK = 7; // TD + FG (PAT kick)
    const int TD  = 6; // plain TD
    const int FG  = 3; // field goal
    const int SF  = 2; // safety

    for (;;) {
        int score = 0;
        if (!read_int_line("Enter the NFL score (Enter 1 to stop): ", &score)) {
            puts("\nInput stream ended. Exiting.");
            break;
        }
        if (score == 1) {
            puts("Goodbye.");
            break;
        }
        if (score < 0) {
            puts("Invalid: negative score is not allowed. Try again.");
            continue;
        }
        if (score == 0) {
            puts("Possible combinations of scoring plays if a team’s score is 0:");
            puts("0 TD + 2pt, 0 TD + FG, 0 TD, 0 3pt FG, 0 Safety");
            continue;
        }
        if (score < 2) {
            puts("Invalid: scores less than 2 (except 1 to quit) are not allowed. Try again.");
            continue;
        }

        printf("Possible combinations of scoring plays if a team’s score is %d:\n", score);

        long count = 0;
        for (int td2 = 0; td2 <= score / TD2; ++td2) {
            int rem_after_td2 = score - td2 * TD2;
            for (int tdk = 0; tdk <= rem_after_td2 / TDK; ++tdk) {
                int rem_after_tdk = rem_after_td2 - tdk * TDK;
                for (int tdn = 0; tdn <= rem_after_tdk / TD; ++tdn) {
                    int rem_after_td = rem_after_tdk - tdn * TD;
                    for (int fg = 0; fg <= rem_after_td / FG; ++fg) {
                        int rem_after_fg = rem_after_td - fg * FG;
                        if (rem_after_fg % SF == 0) {
                            int saf = rem_after_fg / SF;
                            // Match the order and naming shown in the sample.
                            printf("%d TD + 2pt, %d TD + FG, %d TD, %d 3pt FG, %d Safety\n",
                                   td2, tdk, tdn, fg, saf);
                            ++count;
                        }
                    }
                }
            }
        }
        if (count == 0) {
            puts("No combinations found.");
        }
        putchar('\n');
    }
    return 0;
}
