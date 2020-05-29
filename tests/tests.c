# include <stdio.h>
# include <stdlib.h>
# include <stdbool.h>
# include <assert.h>
# include <ctype.h>
# include <string.h>

# include "../src/urvalid.h"

struct test_metrics {
    int passing;
    int failing;
};

char* trim_space(char *str) {
    char *end;
    /* skip leading whitespace */
    while (isspace(*str)) {
        str = str + 1;
    }
    /* remove trailing whitespace */
    end = str + strlen(str) - 1;
    while (end > str && isspace(*end)) {
        end = end - 1;
    }
    /* write null character */
    *(end+1) = '\0';
    return str;
}

int test_urls(bool should_pass, char *url) {

    url = trim_space(url);

    bool is_valid = is_valid_url(url);

    printf("Testing url: %s ", url);

    if (should_pass == is_valid) {

        printf("\033[0;32m"); // Switch output text color to green
        printf(" PASSED");
        printf("\033[0m\n"); // Switch back to default color

        return EXIT_SUCCESS;

    } else {

        printf("\033[0;31m"); // Red
        printf("FAILED");
        printf("\033[0m\n");

        if (should_pass){
            printf("\t%s is a VALID url, but was recognized as INVALID\n\n", url);
        } else {
            printf("\t%s is an INVALID url, but was recognized as VALID\n\n", url);

        }

        return EXIT_FAILURE;
    }
}

void check_url_status_from_file_lines(char *filename, bool url_validity, struct test_metrics *m){
    FILE *file = NULL;
    char *url = NULL;
    size_t len = 0;

    file = fopen(filename, "r");

    if (file == NULL) {
        exit(EXIT_FAILURE);
    };

    while (getline(&url, &len, file) != -1) {
        if (test_urls(url_validity, url) == EXIT_SUCCESS) {
            m->passing += 1;
        } else {
            m->failing += 1;
        }
    }

    if (url) {
        free(url);
    };

    fclose(file);
}

void test_urls_that_should_pass(struct test_metrics *m) {
    check_url_status_from_file_lines("should_pass.txt", true, m);
}

void test_urls_that_should_not_pass(struct test_metrics *m) {
    check_url_status_from_file_lines("should_not_pass.txt", false, m);
}

int main(void) {

    struct test_metrics metrics = { 0, 0 };

    test_urls_that_should_pass(&metrics);
    test_urls_that_should_not_pass(&metrics);

    printf("\n\nCompleted running %d tests: ", metrics.passing + metrics.failing);
    printf("\033[0;32m"); // Green
    printf("%d tests passed ", metrics.passing);
    printf("\033[0;31m"); // Red
    printf("%d tests failed", metrics.failing);
    printf("\033[0m\n");
}
