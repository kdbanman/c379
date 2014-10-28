#include <regex.h>        
#include <stdlib.h>
#include <stdio.h>

#define NOMATCH 0
#define MATCH 1

void regicide(char * spec, char * s, int report)
{
    regex_t regex;
    int reti;
    char msgbuf[100];

    /* Compile regular expression */
    reti = regcomp(&regex, spec, 0);
    if (reti) {
        regerror(reti, &regex, msgbuf, sizeof(msgbuf));
        fprintf(stderr, "Could not compile regex: %s\n", msgbuf);
        exit(1);
    }

    /* Execute regular expression */
    reti = regexec(&regex, s, 0, NULL, 0);
    if (!reti) {
        puts("MATCH");
        if (report == MATCH) {
            puts("vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv");
            puts(s);
            puts("^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^");
        }
    }
    else if (reti == REG_NOMATCH) {
        puts("NOO");
        if (report == NOMATCH) {
            puts("vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv");
            puts(s);
            puts("^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^");
        }
    }
    else {
        regerror(reti, &regex, msgbuf, sizeof(msgbuf));
        fprintf(stderr, "Regex match failed: %s\n", msgbuf);
        exit(1);
    }

    /* Free compiled regular expression if you want to use the regex_t again */
    regfree(&regex);
}

void testHeader(char * s, int report)
{
    char * spec = "^"
                  "\\([^[:cntrl:]]*:[^[:cntrl:]]*\r\\{0,1\\}\n\\)*"
                  "$";

    regicide(spec, s, report);
}

void test(char * s, int report)
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

    regicide(spec, s, report);
}

int main()
{
    puts("\n------------------------\nFAIL\n------------------------\n");

    test("bad", MATCH);
    test("GET balls HTTP/1e1\r\n", MATCH);
    test("GET balls HTTP/1.1", MATCH);
    test("GET balls HTTP/1.1\n", MATCH);
    test("GET balls HTTP/1.1\r\n", MATCH);
    test("GET /someplace/file.html HTTP/1.1\r\n", MATCH);
    test("GET /someplace/file.html HTTP/1.1\r\n"
         "From: someuser@somewhere.org\r\n"
         "User-Agent: Bloatzilla/7.0\r\n", MATCH);
    test("GET /someplace/file.html HTTP/1.1\r\n"
         "From: someuser@somewhere.org\r\n"
         "User-Agent: Bloatzilla/7.0\r\n"
         "\n\r", MATCH);

    puts("\n------------------------\nPASS\n------------------------\n");

    testHeader("From: someuser@somewhere.org\r\n", NOMATCH);

    //CANONICAL
    test("GET /someplace/file.html HTTP/1.1\r\n"
         "From: someuser@somewhere.org\r\n"
         "User-Agent: Bloatzilla/7.0\r\n"
         "\r\n", NOMATCH);
    test("GET /someplace/file.html HTTP/1.1\r\n"
         "\r\n", NOMATCH);
    test("GET /someplace/file.html HTTP/1.1\r\n"
         "From: someuser@somewhere.org\r\n"
         "User-Agent: Bloatzilla/7.0\n"
         "\r\n", NOMATCH);
    test("GET balls HTTP/1.1\r\n\r\n", NOMATCH);
    test("GET /someplace/file.html HTTP/1.1\r\n"
         "\n", NOMATCH);
    test("GET /someplace/file.html HTTP/1.1\r\n"
         "From: someuser@somewhere.org\r\n"
         "User-Agent: Bloatzilla/7.0\n"
         "\n", NOMATCH);
    test("\r\n\n\n\r\nGET /someplace/file.html HTTP/1.1\r\n"
         "From: someuser@somewhere.org\r\n"
         "User-Agent: Bloatzilla/7.0\n"
         "\n", NOMATCH);
    test("GET /someplace/file.html HTTP/1.1\r\n"
         "From: someuser@somewhere.org\r\n"
         "User-Agent: Bloatzilla/7.0\n"
         "\n\n\r\n\r\n\n\n", NOMATCH);
    test("GET b HTTP/1.1\r\n"
         "From: someuser@somewhere.org\r\n"
         ":\r\n"
         "\r\n", NOMATCH);
    test("GET / HTTP/1.1\r\n"
         "From: someuser@somewhere.org\r\n"
         "User-Agent: Bloatzilla/7.0\r\n"
         "\r\n", NOMATCH);


    return 0;
}
