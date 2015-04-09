#include "common.h"

int main(void)
{
  const char *str = "Try not. Do, or do not. There is no try.";
  char target = 'T';
  const char *result = str;
 
  while((result = strchr(result, target)) != 0) { // was oorspronkelijk NULL
    printf("Found '%c' starting at '%s'\n", target, result);
    ++result; // Increment result, otherwise we'll find target at the same location
  }
}


