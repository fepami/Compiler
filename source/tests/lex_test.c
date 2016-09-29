#include <stdarg.h>
#include <stddef.h>
#include <setjmp.h>
#include <cmocka.h>

#include <stdlib.h>

#include "lex.h"
#include "token.h"

#define TEST_FILE_PATH "/home/felipe/workspace/Compiler/source/tests/lex_test.txt"

/* A test case that does nothing and succeeds. */
static void firstTokenTest(void **state) {
    (void) state; /* unused */
    Token* firstToken = NULL;
    FILE *fp = fopen (TEST_FILE_PATH, "r");
    assert_non_null(fp);

    assert_int_equal(nextToken(fp, &firstToken), 1);
    assert_non_null(firstToken);
    assert_int_equal(firstToken->tokenClass, CLASS_RESERVED_WORD);
    assert_string_equal(firstToken->value.stringValue, "int");

    freeToken(&firstToken);
    fclose(fp);
}

static void allTokenTest(void **state) {
    (void) state; /* unused */
    int count = 0;
    int ret = -1;
    Token* token = NULL;
    FILE *fp = fopen (TEST_FILE_PATH , "r");
    assert_non_null(fp);

    while(1){
        if((ret = nextToken(fp, &token)) == 0)
            break;
        assert_int_equal(ret, 1);
        assert_non_null(token);
//         printToken(token);
        freeToken(&token);
        count++;
    }

    assert_int_equal(count, 50);
    fclose(fp);
}

int main(void) {
    const struct CMUnitTest tests[] = {
        cmocka_unit_test(firstTokenTest),
        cmocka_unit_test(allTokenTest),
    };

    return cmocka_run_group_tests(tests, NULL, NULL);
}
