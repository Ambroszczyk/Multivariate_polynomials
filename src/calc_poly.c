#include <stdio.h>
#include "parsing.h"
#include "utils.h"



/**
 * funkcja po kolei wczytuje wiersze, a nastepnie probuje je wczytac
 * az do momentu otrzymania znaku konca pliku
 * @return
 */

/*
static Poly make_linear_poly() {
    Mono* m = MonoInitialize(1);
    m[0].exp = 1;
    m[0].p = PolyFromCoeff(1);
    Poly result;
    result.mono = m;
    result.size = 1;
    return result;
}
*/

int main() {
    
    int r = 1;
    int last_char = 1;
    StackPtr stack;
    stack = StackInit();

    while (last_char !=  EOF) {
        TryParseRow(r, &stack, &last_char);
        r++;
    }
    StackClear(stack);

    return 0;

    /*
    Poly x[100];
    Mono mono_tab[1];
    Mono mono_tab2[2];
    Mono mono_tab3[2];
    x[0] = PolyFromCoeff(3);
    x[1] = PolyFromCoeff(2);

    mono_tab[0] = MonoFromPoly(&x[0], 0);
    mono_tab2[0] = MonoFromPoly(&x[0], 1);
    mono_tab2[1] = MonoFromPoly(&x[1], 3);

    x[3] = PolyAddMonos(1, mono_tab);
    x[4] = PolyAddMonos(2, mono_tab2);
    mono_tab3[0] = MonoFromPoly(&x[3], 3);
    mono_tab3[1] = MonoFromPoly(&x[4], 2);
    x[5] = PolyAddMonos(2, mono_tab3);

    PolyPrint(&x[3]);
    printf("\n");
    PolyPrint(&x[4]);
    printf("\n");
    PolyPrint(&x[5]);

    x[6] = PolyCompose(&x[5], 1
            , &x[3]);
    printf("\n");
    PolyPrint(&x[6]);
     */
}
