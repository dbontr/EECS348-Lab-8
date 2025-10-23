// temp.c
// EECS 348 Lab 8 - Task 2
// Interactive temperature converter among C/F/K.
// Categorizes the *converted* temperature by Celsius thresholds and prints an advisory.
// Handles invalid inputs (non-numeric, scale typos, sub–absolute-zero).
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>

static int read_line(const char *prompt, char *buf, size_t n) {
    if (prompt) {
        fputs(prompt, stdout);
        fflush(stdout);
    }
    if (!fgets(buf, n, stdin)) return 0;
    // Strip trailing newline
    size_t len = strlen(buf);
    if (len && buf[len-1] == '\n') buf[len-1] = '\0';
    return 1;
}

static int parse_double(const char *s, double *out) {
    // Skip leading spaces
    while (isspace((unsigned char)*s)) s++;
    if (*s == '\0') return 0;
    char *end = NULL;
    double v = strtod(s, &end);
    if (end == s) return 0;
    while (isspace((unsigned char)*end)) end++;
    if (*end != '\0') return 0;
    *out = v;
    return 1;
}

static int to_upper_letter(const char *s) {
    // Return uppercase first non-space letter; else 0
    while (*s && isspace((unsigned char)*s)) s++;
    if (!*s) return 0;
    return toupper((unsigned char)*s);
}

static int is_below_absolute_zero(double value, char scale_upper) {
    if (scale_upper == 'C') return value < -273.15;
    if (scale_upper == 'F') return value < -459.67;
    if (scale_upper == 'K') return value < 0.0;
    return 0;
}

static double to_celsius(double value, char scale_upper) {
    switch (scale_upper) {
        case 'C': return value;
        case 'F': return (value - 32.0) * 5.0 / 9.0;
        case 'K': return value - 273.15;
        default:  return value; // unreachable if validated
    }
}

static double from_celsius(double c, char target_upper) {
    switch (target_upper) {
        case 'C': return c;
        case 'F': return c * 9.0 / 5.0 + 32.0;
        case 'K': return c + 273.15;
        default:  return c; // unreachable if validated
    }
}

static const char *categorize_celsius(double c) {
    // Handout intervals:
    // Freezing: (-inf, 0), Cold: [0,10), Comfortable: [10,25), Hot: [25,35), Extreme Heat: [35, inf)
    // Note: the PDF shows "Hot: 25°C to 35°C, [20, 35)" which appears to be a typo.
    if (c < 0.0)              return "Freezing";
    else if (c < 10.0)        return "Cold";
    else if (c < 25.0)        return "Comfortable";
    else if (c < 35.0)        return "Hot";
    else                      return "Extreme Heat";
}

static const char *advisory_for_category(const char *cat) {
    if (strcmp(cat, "Freezing") == 0)     return "Wear a heavy jacket and protect extremities.";
    if (strcmp(cat, "Cold") == 0)         return "Wear a jacket or layers.";
    if (strcmp(cat, "Comfortable") == 0)  return "Enjoy the weather.";
    if (strcmp(cat, "Hot") == 0)          return "Drink lots of water!";
    /* Extreme Heat */
    return "Limit outdoor activity; stay hydrated and indoors if possible.";
}

int main(void) {
    char buf[256];

    for (;;) {
        // Temperature value
        double value = 0.0;
        if (!read_line("Enter the temperature value: ", buf, sizeof buf)) {
            puts("\nInput stream ended. Exiting.");
            break;
        }
        if (!parse_double(buf, &value)) {
            puts("Invalid temperature number. Try again.\n");
            continue;
        }

        // Original scale
        if (!read_line("Enter the original scale (C, F, or K): ", buf, sizeof buf)) break;
        int src = to_upper_letter(buf);
        if (!(src == 'C' || src == 'F' || src == 'K')) {
            puts("Invalid scale. Use C, F, or K.\n");
            continue;
        }
        if (is_below_absolute_zero(value, (char)src)) {
            puts("Invalid: value is below absolute zero for that scale.\n");
            continue;
        }

        // Target scale
        if (!read_line("Enter the scale to convert to (C, F, or K): ", buf, sizeof buf)) break;
        int dst = to_upper_letter(buf);
        if (!(dst == 'C' || dst == 'F' || dst == 'K')) {
            puts("Invalid target scale. Use C, F, or K.\n");
            continue;
        }

        // Convert via Celsius
        double as_c = to_celsius(value, (char)src);
        double out  = from_celsius(as_c, (char)dst);

        printf("Converted temperature: %.2f %c\n", out, (char)dst);

        // Categorize *converted* temperature in Celsius
        const char *category = categorize_celsius(to_celsius(out, (char)dst));
        printf("Temperature category: %s\n", category);
        printf("Weather advisory: %s\n", advisory_for_category(category));

        // Optional repeat
        if (!read_line("\nConvert another? (y/n): ", buf, sizeof buf)) break;
        int ans = to_upper_letter(buf);
        if (ans != 'Y') break;
        putchar('\n');
    }
    return 0;
}
