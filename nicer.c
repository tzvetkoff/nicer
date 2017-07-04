#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <getopt.h>
#include <errno.h>

#define VERSION "0.1.0"

static void
usage(const char *prog_name, FILE *out)
{
    fputs("\n", out);
    fputs("Usage:\n", out);
    fprintf(out, "  %s [options] <command> [args...]\n", prog_name);

    fputs("\n", out);
    fputs("Options:\n", out);
    fputs("  -n N, --increment=N   set priority [0]\n", out);
    fputs("  -h, --help            display this help and exit\n", out);
    fputs("  -v, --version         print version information and exit\n", out);

    fputs("\n", out);
    exit(out == stderr ? EXIT_FAILURE : EXIT_SUCCESS);
}

int
main(int argc, char **argv)
{
    static int ch = 0, priority = 0;
    gid_t gid;
    uid_t uid;
    char *p;
    static const struct option longopts[] = {
        {"increment", required_argument, NULL, 'n'},
        {"help", no_argument, NULL, 'h'},
        {"version", no_argument, NULL, 'v'},
        {NULL, 0, NULL, 0}
    };

    /* parse command line options */
    while ((ch = getopt_long(argc, argv, "+n:hv", longopts, NULL)) != -1) {
        switch (ch) {
            case 'h':
                usage(argv[0], stdout);
            case 'v':
                puts(VERSION);
                return EXIT_SUCCESS;
            case 'n':
                priority = strtol(optarg, &p, 0);
                if (p == optarg) {
                    fprintf(stderr, "%s: invalid number: %s\n", argv[0],
                        optarg);
                    return EXIT_FAILURE;
                }
                break;
            default:
                usage(argv[0], stderr);
        }
    }

    /* no arguments? */
    if (argc - optind == 0) {
        fprintf(stderr,
            "%s: wrong number of arguments (given %d, expected >=%d)\n",
            argv[0], argc - optind, 1);
        usage(argv[0], stderr);
    }

    /* do we need to change priority? */
    if (priority) {
        /* do we need root? */
        if (priority < 0) {
            gid = getgid();
            uid = getuid();

            if (setgid(0) == -1) {
                fprintf(stderr, "%s: setgid(0): %s\n", argv[0],
                    strerror(errno));
                return EXIT_FAILURE;
            }
            if (setuid(0) == -1) {
                fprintf(stderr, "%s: setuid(0): %s\n", argv[0],
                    strerror(errno));
                return EXIT_FAILURE;
            }
        }

        /* try to set priority */
        if (nice(0) + nice(priority) != priority) {
            fprintf(stderr, "%s: nice(%d): cannot set priority\n", argv[0],
                priority);
            return EXIT_FAILURE;
        }

        /* restore original gid & uid if needed */
        if (priority < 0) {
            setgid(gid);
            setuid(uid);
        }
    }

    /* exec! */
    return execvp(argv[optind], argv + optind);
}
