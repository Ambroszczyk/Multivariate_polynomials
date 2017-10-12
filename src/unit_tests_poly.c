/** @file
   Implementacja testow jednostkowych
   */

#include <stdio.h>
#include <stdarg.h>
#include <setjmp.h>
#include <stdlib.h>
#include <memory.h>
#include "cmocka.h"
#include "parsing.h"

/**
 * bufor skoku, dla funkcji exit
 */
static jmp_buf jmp_at_exit;

/**
 * kod wyjścia funkcji exit
 */
static int exit_status;

/**
 * Pomocnicza atrapa funkcji main
 */
extern int poly_main();

/**
 * tworzy wielomian x_0
 * @return wielomian x_0
 */
Poly make_linear_poly() {
    Mono* m = MonoInitialize(1);
    m[0].exp = 1;
    m[0].p = PolyFromCoeff(1);
    Poly result;
    result.mono = m;
    result.size = 1;
    return result;
}

/**
 * Atrapa funkcji main
 */
int mock_main() {
    if (!setjmp(jmp_at_exit))
        return poly_main();
    return exit_status;
}

/**
 * Atrapa funkcji exit
 * @param[in, out] status : status
 */
void mock_exit(int status) {
    exit_status = status;
    longjmp(jmp_at_exit, 1);
}
/**
 *  Atrapa funkcji fprintf sprawdzająca poprawność wypisywania na stderr.
 */
int mock_fprintf(FILE* const file, const char *format, ...) CMOCKA_PRINTF_ATTRIBUTE(2, 3);

/**
 * Atrapa funkcji fprintf sprawdzająca poprawność wypisywania na stdout.
 */
int mock_printf(const char *format, ...) CMOCKA_PRINTF_ATTRIBUTE(1, 2);

/**
 * Pomocnicze bufor, do którego pisze atrapa funkcji printf i fprintf.
 */
static char fprintf_buffer[256];

/**
 *Pomocnicze bufor, do którego pisze atrapa funkcji printf i fprintf
 */
static char printf_buffer[256];

/**
 *pozycje zapisu w tych buforze, pozycja zapisu wskazuje bajt o wartości 0
 */
 static int fprintf_position = 0;

/**
 *pozycje zapisu w tych buforze, pozycja zapisu wskazuje bajt o wartości 0
 */
static int printf_position = 0;

/**
 * Atrapa funkcji fprintf sprawdzająca poprawność wypisywania na stderr.
 */
int mock_fprintf(FILE* const file, const char *format, ...) {
    int return_value;
    va_list args;

    assert_true(file == stderr);
    /* Poniższa asercja sprawdza też, czy fprintf_position jest nieujemne.
    W buforze musi zmieścić się kończący bajt o wartości 0. */
    assert_true((size_t)fprintf_position < sizeof(fprintf_buffer));

    va_start(args, format);
    return_value = vsnprintf(fprintf_buffer + fprintf_position,
                             sizeof(fprintf_buffer) - fprintf_position,
                             format,
                             args);
    va_end(args);

    fprintf_position += return_value;
    assert_true((size_t)fprintf_position < sizeof(fprintf_buffer));
    return return_value;
}

/**
 * Atrapa funkcji fprintf sprawdzająca poprawność wypisywania na stdout.
 */
int mock_printf(const char *format, ...) {
    int return_value;
    va_list args;

    /* Poniższa asercja sprawdza też, czy printf_position jest nieujemne.
    W buforze musi zmieścić się kończący bajt o wartości 0. */
    assert_true((size_t)printf_position < sizeof(printf_buffer));

    va_start(args, format);
    return_value = vsnprintf(printf_buffer + printf_position,
                             sizeof(printf_buffer) - printf_position,
                             format,
                             args);
    va_end(args);

    printf_position += return_value;
    assert_true((size_t)printf_position < sizeof(printf_buffer));
    return return_value;
}

/**
 *  Pomocniczy bufor, z którego korzystają atrapy funkcji operujących na stdin.
 */
static char input_stream_buffer[256];

/**
 * pozycja bufora
 */
static int input_stream_position = 0;

/**
 * koniec bufora
 */
static int input_stream_end = 0;

/**
 * licznik przeczytanych charów
 */
int read_char_count;

/**
 * Atrapa funkcji scanf używana do przechwycenia czytania z stdin.
 * @param[in] format : format
 */
int mock_scanf(const char *format, ...) {
    va_list fmt_args;
    int ret;

    va_start(fmt_args, format);
    ret = vsscanf(input_stream_buffer + input_stream_position, format, fmt_args);
    va_end(fmt_args);

    if (ret < 0) { /* ret == EOF */
        input_stream_position = input_stream_end;
    }
    else {
        assert_true(read_char_count >= 0);
        input_stream_position += read_char_count;
        if (input_stream_position > input_stream_end) {
            input_stream_position = input_stream_end;
        }
    }
    return ret;
}

/**
 * Atrapa funkcji getchar używana do przechwycenia czytania z stdin.
 */
int mock_getchar() {
    if (input_stream_position < input_stream_end)
        return input_stream_buffer[input_stream_position++];
    else
        return EOF;
}

/**
 * Atrapa funkcji ungetc.
 * Obsługiwane jest tylko standardowe wejście.
 */
int mock_ungetc(int c, FILE *stream) {
    assert_true(stream == stdin);
    if (input_stream_position > 0)
        return input_stream_buffer[--input_stream_position] = c;
    else
        return EOF;
}

/**
 * Funkcja wołana przed każdym testem
 */
static int test_setup(void **state) {
    (void)state;

    memset(fprintf_buffer, 0, sizeof(fprintf_buffer));
    memset(printf_buffer, 0, sizeof(printf_buffer));
    printf_position = 0;
    fprintf_position = 0;

    /* Zwrócenie zera oznacza sukces. */
    return 0;
}

/**
 * Funkcja wołana po każdej grupie testów.
 */
static int test_group_teardown(void **state) {
    (void)state;

    return 0;
}

/**
 * Funkcja inicjująca dane wejściowe dla programu korzystającego ze stdin.
 */
static void init_input_stream(const char *str) {
    memset(input_stream_buffer, 0, sizeof(input_stream_buffer));
    input_stream_position = 0;
    input_stream_end = (int) strlen(str);
    assert_true((size_t)input_stream_end < sizeof(input_stream_buffer));
    strcpy(input_stream_buffer, str);
}

/**
 * test w przypadku gdy wielomian jest zerowy, count = 0
 * @param[in, out] state : status
 */
static void test_poly_zero_first(void **state) {
    (void) state;
    Poly p = PolyZero();
    Poly correct_result = PolyZero();
    unsigned count = 0;
    Poly dummy[1];

    Poly result = PolyCompose(&p, count, dummy);

    assert_true(PolyIsEq(&correct_result, &result));
}

/**
 * test w przypadku gdy wielomian jest zerowy, count = 1
 * @param[in, out] state : status
 */
static void test_poly_zero_second(void **state) {
    (void) state;
    Poly p = PolyZero();
    Poly correct_result = PolyZero();
    unsigned count = 1;
    Poly x[1] = {PolyFromCoeff(1)};

    Poly result = PolyCompose(&p, count, x);

    assert_true(PolyIsEq(&correct_result, &result));
}

/**
 * test w przypadku gdy wielomian jest stały, count = 0
 * @param[in, out] state : status
 */
static void test_poly_const_first(void **state) {
    (void) state;
    Poly p = PolyFromCoeff(1);
    Poly correct_result = PolyFromCoeff(1);
    unsigned count = 0;
    Poly dummy[1];

    Poly result = PolyCompose(&p, count, dummy);

    assert_true(PolyIsEq(&correct_result, &result));
}

/**
 * test w przypadku gdy wielomian jest stały, count = 1
 * @param[in, out] state : status
 */
static void test_poly_const_second(void **state) {
    (void) state;
    Poly p = PolyFromCoeff(1);
    Poly correct_result = PolyFromCoeff(1);
    unsigned count = 1;
    Poly x[1] = {PolyFromCoeff(2)};

    Poly result = PolyCompose(&p, count, x);

    assert_true(PolyIsEq(&correct_result, &result));
}

/**
 * test w przypadku gdy wielomian jest liniowy, count = 0
 * @param[in, out] state : status
 */
static void test_poly_linear_first(void **state) {
    (void) state;
    Poly p = make_linear_poly();
    Poly correct_result = PolyZero();
    unsigned count = 0;
    Poly dummy[1];

    Poly result = PolyCompose(&p, count, dummy);
    PolyDestroy(&p);

    assert_true(PolyIsEq(&correct_result, &result));
}

/**
 * test w przypadku gdy wielomian jest liniowy, count = 1
 * @param[in, out] state : status
 */
static void test_poly_linear_second(void **state) {
    (void) state;
    Poly p = make_linear_poly();
    Poly correct_result = PolyFromCoeff(1);
    unsigned count = 1;
    Poly x[1] = {PolyFromCoeff(1)};

    Poly result = PolyCompose(&p, count, x);
    PolyDestroy(&p);

    assert_true(PolyIsEq(&correct_result, &result));
}


/**
 * test w przypadku gdy wielomian jest liniowy, count = 1
 * @param[in, out] state : status
 */
static void test_poly_linear_third(void **state) {
    (void) state;
    Poly p = make_linear_poly();
    Poly correct_result = make_linear_poly();
    unsigned count = 1;
    Poly x[1] = {make_linear_poly()};

    Poly result = PolyCompose(&p, count, x);

    assert_true(PolyIsEq(&correct_result, &result));

    PolyDestroy(&p);
    PolyDestroy(&correct_result);
    PolyDestroy(&x[0]);
    PolyDestroy(&result);
}

/**
 * test czytania z inputu gdy nie podano parametru
 * @param[in, out] state : status
 */
static void test_count_no_parameter(void **state) {
    (void) state;
    init_input_stream("COMPOSE \n");


    assert_int_equal(mock_main(), 0);
    assert_string_equal(printf_buffer, "");
    assert_string_equal(fprintf_buffer, "ERROR 1 WRONG COUNT\n");
}

/**
 * test czytania z inputu gdy COUNT = 0
 * @param[in, out] state : status
 */
static void test_count_minimal_value(void **state) {
    (void) state;
    init_input_stream("COMPOSE 0\n");


    assert_int_equal(mock_main(), 0);
    assert_string_equal(printf_buffer, "");
    assert_string_equal(fprintf_buffer, "ERROR 1 STACK UNDERFLOW\n");
}

/**
 * test czytania z inputu gdy COUNT = UINT_MAX
 * @param[in, out] state : status
 */
static void test_count_maximal_value(void **state) {
    (void) state;

    init_input_stream("COMPOSE 4294967295\n");


    assert_int_equal(mock_main(), 0);
    assert_string_equal(printf_buffer, "");
    assert_string_equal(fprintf_buffer, "ERROR 1 STACK UNDERFLOW\n");
}

/**
 * test czytania z inputu gdy COUNT = -1
 * @param[in, out] state : status
 */
static void test_count_minus_one(void **state) {
    (void) state;

    init_input_stream("COMPOSE -1\n");


    assert_int_equal(mock_main(), 0);
    assert_string_equal(printf_buffer, "");
    assert_string_equal(fprintf_buffer, "ERROR 1 WRONG COUNT\n");
}


/**
 * test czytania z inputu gdy COUNT = UINT_MAX + 1
 * @param[in, out] state : status
 */
static void test_count_unsigned_max_plus_one(void **state) {
    (void) state;

    init_input_stream("COMPOSE 4294967296\n");


    assert_int_equal(mock_main(), 0);
    assert_string_equal(printf_buffer, "");
    assert_string_equal(fprintf_buffer, "ERROR 1 WRONG COUNT\n");
}


/**
 * test czytania z inputu gdy COUNT = OGROMNA_LICZBA
 * @param[in, out] state : status
 */
static void test_count_sooo_big_number(void **state) {
    (void) state;

    init_input_stream("COMPOSE 12345678910111213141516\n");


    assert_int_equal(mock_main(), 0);
    assert_string_equal(printf_buffer, "");
    assert_string_equal(fprintf_buffer, "ERROR 1 WRONG COUNT\n");
}

/**
 * test czytania z inputu gdy COUNT = KOMBINACJI_LITER_I_CYFR
 * @param[in, out] state : status
 */
static void test_count_letters_and_numbers_combination(void **state) {
    (void) state;

    init_input_stream("COMPOSE 1A2B3C4D5E\n");


    assert_int_equal(mock_main(), 0);
    assert_string_equal(printf_buffer, "");
    assert_string_equal(fprintf_buffer, "ERROR 1 WRONG COUNT\n");
}

/**
 * test czytania z inputu gdy COUNT = KOMBINACJI_LITER
 * @param[in, out] state : status
 */
static void test_count_letters_combination(void **state) {
    (void) state;

    init_input_stream("COMPOSE ABCDEFGHIJ\n");


    assert_int_equal(mock_main(), 0);
    assert_string_equal(printf_buffer, "");
    assert_string_equal(fprintf_buffer, "ERROR 1 WRONG COUNT\n");
}

/**
 * głowna funkcja
  */
int main() {
    int result = 0;
    const struct CMUnitTest first_tests[] = {
            cmocka_unit_test(test_poly_zero_first),
            cmocka_unit_test(test_poly_zero_second),
            cmocka_unit_test(test_poly_const_first),
            cmocka_unit_test(test_poly_const_second),
            cmocka_unit_test(test_poly_linear_first),
            cmocka_unit_test(test_poly_linear_second),
            cmocka_unit_test(test_poly_linear_third)

    };

    const struct CMUnitTest second_tests[] = {
            cmocka_unit_test_setup(test_count_no_parameter, test_setup),
            cmocka_unit_test_setup(test_count_minimal_value, test_setup),
            cmocka_unit_test_setup(test_count_maximal_value, test_setup),
            cmocka_unit_test_setup(test_count_minus_one, test_setup),
            cmocka_unit_test_setup(test_count_unsigned_max_plus_one, test_setup),
            cmocka_unit_test_setup(test_count_sooo_big_number, test_setup),
            cmocka_unit_test_setup(test_count_letters_combination, test_setup),
            cmocka_unit_test_setup(test_count_letters_and_numbers_combination, test_setup)

    };

    result = cmocka_run_group_tests(first_tests, NULL, test_group_teardown);
    result += cmocka_run_group_tests(second_tests, NULL, test_group_teardown);
    return result;
}