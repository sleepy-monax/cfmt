#include "fmt.h"

#include <stdio.h>


int main(int argc, char const *argv[])
{
    print_format("Hi! My name is {} and I'm {} years old!\n", "Nicolas", 21);

    return 0;
}
