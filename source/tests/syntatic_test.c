#include <stdarg.h>
#include <stddef.h>
#include <setjmp.h>
#include <cmocka.h>

#include <stdlib.h>

#include "syntatic.h"
#include "token.h"

#define TEST_FILE_PATH "/home/felipe/workspace/Compiler/source/tests/syntatic_test.txt"


static void allSyntaticTest(void **state) {
    (void) state; /* unused */
    int count = 0;
    int ret = -1;
    Token* token = NULL;
    FILE *fp = fopen (TEST_FILE_PATH , "r");
    assert_non_null(fp);

    assert_int_equal(analyze(fp), 0);
    
    fclose(fp);
}

int main(void) {
    const struct CMUnitTest tests[] = {
        cmocka_unit_test(allSyntaticTest),
    };

    return cmocka_run_group_tests(tests, NULL, NULL);
}
