/** @file
   Interfejs błędów parsowania
   */

#ifndef WIELOMIANY_ERROR_H
#define WIELOMIANY_ERROR_H
/**
 * typ trzymajace informacje odnosnie bledow
 */
enum error {
    NO_ERROR,
    STACK_UNDERFLOW,
    W_VARIABLE,
    W_COMMAND,
    W_VALUE,
    NO_COMMAND,
    W_COUNT
};

/**
 * typ błędowy
 */
typedef enum error error_t;

/**
 * WYpisuje informacje o błędzie
 * @param[in] error : rodzaj błędu
 * @param[in] r : numer wiersza błędu
 */
void ErrorPrint(error_t error, int r);

#endif //WIELOMIANY_ERROR_H
