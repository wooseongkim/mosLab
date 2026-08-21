#ifndef MOSLAB_TEST_UTIL_H
#define MOSLAB_TEST_UTIL_H

#include <stdio.h>
#include <string.h>

typedef struct test_report {
    int passed;
    int failed;
} test_report_t;

#define TEST_EXPECT(report, name, condition)                                      \
    do {                                                                         \
        if (condition) {                                                         \
            (report)->passed++;                                                  \
            printf("[PASS] %s\n", (name));                                      \
        } else {                                                                 \
            (report)->failed++;                                                  \
            printf("[FAIL] %s\n", (name));                                      \
        }                                                                        \
    } while (0)

#define TEST_EXPECT_STATUS(report, name, actual, expected)                       \
    TEST_EXPECT((report), (name), ((actual) == (expected)))

static int test_finish(const char *label, const test_report_t *report) {
    printf("%s RESULT: %d passed, %d failed\n",
           label,
           report->passed,
           report->failed);
    return report->failed == 0 ? 0 : 1;
}

#endif
