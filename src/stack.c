/** @file
   Implementacja stosu trzymajacego wielomiany
   */


#include "stack.h"
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <limits.h>
#include "utils.h"


StackPtr StackInit() {
    StackPtr new_stack = malloc(sizeof(struct Stack));
    assert(new_stack != NULL);
    new_stack->prev = NULL;
    return new_stack;
}

bool StackEmpty(StackPtr stack) {
    if (stack->prev == NULL)
        return true;
    return false;
}

void StackPush(Poly *poly, StackPtr *stack) {
    StackPtr new_stack = malloc(sizeof(struct Stack));
    assert(new_stack != NULL);
    new_stack->poly = *poly;
    new_stack->prev = *stack;
    *stack = new_stack;
}

Poly StackTop(StackPtr stack) {
    return stack->poly;
}

Poly StackPop(StackPtr *stack) {
    StackPtr tem_stack;
    Poly poped_poly;
    if (!StackEmpty(*stack)) {
        poped_poly = (*stack)->poly;
        tem_stack = (*stack)->prev;
        free(*stack);
        *stack = tem_stack;
        return poped_poly;
    }
    return PolyZero();
}

void StackClear(StackPtr stack) {
    Poly p;
    while(!StackEmpty(stack)) {
        p = StackPop(&stack);
        PolyDestroy(&p);
    }
    free(stack);
}

void StackZero(StackPtr *stack) {
    Poly poly_help = PolyZero();
    StackPush(&poly_help, stack);
}

bool StackIsCoeff(StackPtr *stack) {
    if (StackEmpty(*stack))
        return false;
    else {
        Poly poly_help = StackTop(*stack);
        if (PolyIsCoeff(&poly_help))
            printf("1\n");
        else
            printf("0\n");
        return true;
    }
}

bool StackIsZero(StackPtr *stack) {
    if (StackEmpty(*stack))
        return false;
    else {
        Poly poly_help = StackTop(*stack);
        if (PolyIsZero(&poly_help))
            printf("1\n");
        else
            printf("0\n");
        return true;
    }
}

bool StackClone(StackPtr *stack) {
    if (StackEmpty(*stack))
        return false;
    else {
        Poly poly_help = StackTop(*stack);
        Poly new_poly = PolyClone(&poly_help);
        StackPush(&new_poly, stack);
        return true;
    }
}

bool StackAdd(StackPtr *stack) {
    Poly poly1, poly2, poly_add;
    if (StackEmpty(*stack) || StackEmpty((*stack)->prev))
        return false;
    else {
        poly1 = StackPop(stack);
        poly2 = StackPop(stack);
        poly_add = PolyAdd(&poly1, &poly2);
        PolyDestroy(&poly1);
        PolyDestroy(&poly2);
        StackPush(&poly_add, stack);
        return true;
    }
}

bool StackMul(StackPtr *stack) {
    Poly poly1, poly2, poly_mul;
    if (StackEmpty(*stack) || StackEmpty((*stack)->prev))
        return false;
    else {
        poly1 = StackPop(stack);
        poly2 = StackPop(stack);
        poly_mul = PolyMul(&poly1, &poly2);
        PolyDestroy(&poly1);
        PolyDestroy(&poly2);
        StackPush(&poly_mul, stack);
        return true;
    }
}

bool StackNeg(StackPtr *stack) {
    Poly poly_neg;
    Poly poly_help;
    if (StackEmpty(*stack))
        return false;
    else {
        poly_help = StackPop(stack);
        poly_neg = PolyNeg(&poly_help);
        PolyDestroy(&poly_help);
        StackPush(&poly_neg, stack);
        return true;
    }
}

bool StackSub(StackPtr *stack) {
    Poly poly_high, poly_low, poly_sub;
    if (StackEmpty(*stack) || StackEmpty((*stack)->prev))
        return false;
    else {
        poly_high = StackPop(stack);
        poly_low = StackPop(stack);
        poly_sub = PolySub(&poly_high, &poly_low);
        PolyDestroy(&poly_high);
        PolyDestroy(&poly_low);
        StackPush(&poly_sub, stack);
        return true;
    }
}

bool StackIsEq(StackPtr *stack) {
    Poly poly_high, poly_low;
    if (StackEmpty(*stack) || StackEmpty((*stack)->prev))
        return false;
    else {
        poly_high = StackTop(*stack);
        poly_low = StackTop((*stack)->prev);
        if (PolyIsEq(&poly_high, &poly_low))
            printf("1\n");
        else
            printf("0\n");
        return true;
    }
}

bool StackDeg(StackPtr *stack) {
    int deg;
    Poly poly_help;
    if (StackEmpty(*stack))
        return false;
    else {
        poly_help = StackTop(*stack);
        deg = PolyDeg(&poly_help);
        printf("%d\n", deg);
        return true;
    }
}

bool StackDegBy(StackPtr *stack, unsigned idx) {
    int deg;
    Poly poly_help;
    if (StackEmpty(*stack))
        return false;
    else {
        poly_help = StackTop(*stack);
        deg = PolyDegBy(&poly_help, idx);
        printf("%d\n", deg);
        return true;
    }
}

bool StackAt(StackPtr *stack, poly_coeff_t x) {
    Poly poly_at;
    Poly poly_help;
    if (StackEmpty(*stack))
        return false;
    else {
        poly_help = StackPop(stack);
        poly_at = PolyAt(&poly_help, x);
        PolyDestroy(&poly_help);
        StackPush(&poly_at, stack);
        return true;
    }
}

bool StackPopDel(StackPtr *stack) {
    if (StackEmpty(*stack))
        return false;
    else {
        Poly poly_pop = StackPop(stack);
        PolyDestroy(&poly_pop);
        return true;
    }
}

bool StackPrint(StackPtr  *stack) {
    if (StackEmpty(*stack))
        return false;
    else {
        Poly poly = StackTop(*stack);
        PolyPrint(&poly);
        printf("\n");
        return true;
    }
}

bool StackCompose(StackPtr *stack, unsigned count) {
    StackPtr tempo_stack = *stack;
    unsigned i = 0;
    Poly p;
    Poly result;
    Poly* poly_array;
    
    if (count == UINT_MAX)
        return false;

    //sprawdzamym, czy na stosie jest odpowiednio duzo elemantow
    while (i < count + 1) {
        if (StackEmpty(tempo_stack))
            return false;
        tempo_stack=tempo_stack->prev;
        i++;
    }

    poly_array = malloc(sizeof(Poly) * count);
    assert(poly_array != NULL);

    p = StackPop(stack);

    for (unsigned j = 0; j < count; j++)
        poly_array[j] = StackPop(stack);

    result = PolyCompose(&p, count, poly_array);
    StackPush(&result, stack);

    PolyDestroy(&p);
    for (unsigned j = 0; j < count; j++)
        PolyDestroy(&poly_array[j]);

    free(poly_array);
    return true;

}




