#include <regex.h>        
#include <stdlib.h>
#include <stdio.h>

#define NUMM 3
#define DETAIL 0

void regicide(char * spec, char * s)
{
    regex_t regex;
    int reti;
    char msgbuf[100];

    regmatch_t matches[NUMM];

    /* Compile regular expression */
    reti = regcomp(&regex, spec, 0);
    if (reti) {
        regerror(reti, &regex, msgbuf, sizeof(msgbuf));
        fprintf(stderr, "Could not compile regex: %s\n", msgbuf);
        exit(1);
    }

    /* Execute regular expression */
    reti = regexec(&regex, s, NUMM, matches, 0);
    if (!reti) {
        if (DETAIL) {
            puts("MATCH");
            puts("vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv");
        }
        
        puts("---------");
        int j;
        for (j = matches[2].rm_so; j < matches[2].rm_eo; j++) {
            putchar(s[j]);
        }
        printf("\n");
        puts(s);

        if (DETAIL) {
            /* show matches */
            int i;
            for (i = 1; i < NUMM; i++) {
                printf("%d%d%d%d%d%d%d%d%d%d%d\n",i,i,i,i,i,i,i,i,i,i,i);
                printf("%d %d\n", matches[i].rm_so, matches[i].rm_eo);

                int j;
                for (j = matches[i].rm_so; j < matches[i].rm_eo; j++) {
                    putchar(s[j]);
                }
                printf("\n");
            }

            puts("^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^");
        }
    }
    else if (reti == REG_NOMATCH) {
        puts("NOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOO");
        puts("NOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOO");
        puts("NOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOO");
        puts("vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv");
        puts(s);
        puts("^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^");
    }
    else {
        regerror(reti, &regex, msgbuf, sizeof(msgbuf));
        fprintf(stderr, "Regex match failed: %s\n", msgbuf);
        exit(1);
    }

    /* Free compiled regular expression if you want to use the regex_t again */
    regfree(&regex);
}

void test(char * s)
{
    /*
     * Regex spec notes by multiline string line number:
     * 2. GET may be preceded by [CR]LF.
     * 3. Second subexpression \\([^[:space:]]*\\) is the resource path.
     * 4. Any number of headers are allowed.
     * 4. Line is header iff colon separated keyval strings followed by [CR]LF
     * 4. keyval strings contain no control characters (x01-x1f)
     * 4. This (liberally) passes the string ":\n" as a header due to the colon.
     */
    char * spec = "^"
                  "\\(\r\\{0,1\\}\n\\)*"
                  "GET \\([^[:space:]]*\\) HTTP/1\\.1\r\\{0,1\\}\n"
                  "\\([^[:cntrl:]]*:[^[:cntrl:]]*\r\\{0,1\\}\n\\)*"
                  "\\(\r\\{0,1\\}\n\\)\\{1,\\}"
                  "$";

    regicide(spec, s);
}


int main()
{
    //CANONICAL
    test("GET /someplace/file.html HTTP/1.1\r\n"
         "From: someuser@somewhere.org\r\n"
         "User-Agent: Bloatzilla/7.0\r\n"
         "\r\n");
    test("GET /someplace/file.html HTTP/1.1\r\n"
         "\r\n");
    test("GET /someplace/file.html HTTP/1.1\r\n"
         "From: someuser@somewhere.org\r\n"
         "User-Agent: Bloatzilla/7.0\n"
         "\r\n");
    test("GET balls HTTP/1.1\r\n\r\n");
    test("GET /someplace/file.html HTTP/1.1\r\n"
         "\n");
    test("GET /someplace/file.html HTTP/1.1\r\n"
         "From: someuser@somewhere.org\r\n"
         "User-Agent: Bloatzilla/7.0\n"
         "\n");
    test("\r\n\n\n\r\nGET /someplace/file.html HTTP/1.1\r\n"
         "From: someuser@somewhere.org\r\n"
         "User-Agent: Bloatzilla/7.0\n"
         "\n");
    test("GET /someplace/file.html HTTP/1.1\r\n"
         "From: someuser@somewhere.org\r\n"
         "User-Agent: Bloatzilla/7.0\n"
         "\n\n\r\n\r\n\n\n");
    test("GET b HTTP/1.1\r\n"
         "From: someuser@somewhere.org\r\n"
         ":\r\n"
         "\r\n");
    test("GET / HTTP/1.1\r\n"
         "From: someuser@somewhere.org\r\n"
         "User-Agent: Bloatzilla/7.0\r\n"
         "\r\n");


    return 0;
}
