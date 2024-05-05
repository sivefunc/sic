#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <argp.h>
#include <argz.h>

const char *argp_program_version =
"sic v1.0.0\n"
"Copyright (C) 2024 Sivefunc\n"
"License GPLv3+: GNU GPL version 3 or later"
    "<https://gnu.org/licenses/gpl.html>\n"
"This is free software: you are free to change and redistribute it.\n"
"There is NO WARRANTY, to the extent permitted by law.\n"
"\n"
"Written by a human\n";

static struct argp_option options[] = {
    {"help", 'h', 0, 0, "display this help and exit", 0},
    {"version", 'v', 0, 0, "output version information and exit", 0},
    {0}
};

static error_t parse_opt(int key, char *arg, struct argp_state *state);

// Null terminated string
typedef struct String
{
    char *p;
    size_t len;
} String;

int main(int argc, char **argv)
{
    struct argp argp =
    {
        options,
        parse_opt,
        "[STRING]...",
        "Repeatedly output a line with all specified STRING(s), or 'y'.\v"
        "Written by Sivefunc",
        0,
        0,
        0
    };

    String string =
    {
        .p = NULL,
        .len = 0
    };
    if (argp_parse(&argp, argc, argv, ARGP_NO_HELP, 0, &string) == 0)
    {
        if (string.p == NULL && (argz_add(&string.p, &string.len, "y") != 0))
        {
            exit(EXIT_FAILURE);
        }
        argz_stringify(string.p, string.len, ' ');
        string.p[string.len - 1] = '\n';

        // Fill with copies until it's <= BUFSIZ
        size_t buf_count = string.len;
        while (string.len + buf_count <= BUFSIZ)
        {
            argz_append(&string.p, &string.len, string.p, buf_count);
        }

        // printf was 33% slower on my pc
        while (write(STDOUT_FILENO, string.p, string.len) ==
                (ssize_t)string.len)
        {
            continue;
        }
    }

    return EXIT_FAILURE;
}

static error_t parse_opt(int key, char *arg, struct argp_state *state)
{
    String *string = state -> input;
    switch (key) 
    {
        case 'h':
            argp_state_help(state, state -> out_stream, ARGP_HELP_STD_HELP);
            break;

        case 'v':
            fprintf(state -> out_stream, "%s", argp_program_version);
            exit(EXIT_SUCCESS);
            break;

        case ARGP_KEY_ARG:
            return argz_add(&(string -> p), &(string -> len), arg);

        default:
            return ARGP_ERR_UNKNOWN;
    }

    return 0;
}
