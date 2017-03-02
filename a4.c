/*
 * Names: Ian Kirkpatrick, Benjamin Groseclose, Nathan Johnson,
 * Class: CPS 360,                            Section: Spring 2017
 * Times: Tuesday, Thursday 9:30 - 11:00      Assignment: 04
 * Due: March 2, 2017                      Started: February 27, 2017
 * Credit: 15 points.
 *
 * Problem: Implement several boolean circuits using bitwise operations. Each
 * circuit is represented by a function. These are the software representations
 * of the circuits. The program should take input from the command line of
 * hexidecimal values. Each function will use the inputs as x, y, cary-in and/or
 * cary-out. They are refered to as <arg-a>, <arg-b>, <arg-c>, etc...
 */
#include<stdio.h>
#include<stdlib.h>

int main(int argc, char *argv[]) {
  void halfaddr(int a, int b, int *sum, int *outcary);
  void fulladdr(int a, int b, int incary, int *sum, int *outcary);
  void add4(int a, int b, int incary, int *sum, int *outcary);
  void magnitude4(int a, int *result);

  int a=8, b=7, s, oc;

  halfaddr(a, b, &s, &oc);
  printf("s: %d\nco: %d\n", s, oc);

  fulladdr(a, b, oc, &s, &oc);
  printf("s: %d\nco: %d\n", s, oc);

  add4(a, b, 0, &s, &oc);
  printf("s: %d\nco: %d\n", s, oc);

  magnitude4(0x0b, &s);
  printf("magnitude: %d\n", s);


  exit(0);
}

/* Extract the first bit from a using the & bitwise operation. This will ignore
 * any bits after the first bit and return the result. For example, if a is
 * 0000 0000 0010 0011, then this will return 0000 0000 0000 0001. If a is
 * 0000 0000 0010 0010, then this will return 0000 0000 0000 0000.
 */
unsigned int _1(int a) {
  return a & 1;
}

/* Extract the first 4 bits from a using the & bitwise operation. This will
 * ignore any bits after the first 4 bits and return the result. For example,
 * If a is 1001 1101 0010 1010, then this will return 0000 0000 0000 0010.
 */
unsigned int _4(int a) {
  return a & 0x0f;
}

/* Perform a half adder operation on a, and b. This is a software representation
 * of this circuit.
 *
 * A half adder works similar to an exclusive or where the main output (sum in
 * this case) will be set to 1 if only one of a or b is 1. Otherwise, sum will
 * be a zero. The difference, however, is that if both a and b are 1, then
 * outcary will be set to 1 because it caries over when a and b are combined.
 *
 * The boolean function for this C function is:
 *      sum = a'b + ab'
 *      outcary = ab
 *
 * NOTE: No matter what a and b are, even if they are signed and negative, the
 * only bits that will be evaluated here are the first bit in a and b. For
 * example, if a is 2 (0000 0010) then it will be evaluated as 0 because the
 * first bit is a 0. So 3 (0000 0011) will be evaluated as 1. Sum and outcary
 * will be returned with only the first bit set if any.
 */
void halfaddr(int a, int b, int *sum, int *outcary) {
  unsigned int _1(int a);
  *sum = ((_1(~a)) & _1(b)) | (_1(a) & (_1(~b)));
  *outcary = _1(a) & _1(b);
}

/* Perform a full adder operation on a, b, and incary. This is a software
 * representation of this circuit.
 *
 * A full adder works similar to an exclusive or where the main output (sum in
 * this case) will be set to 1 if only one of a, b, and incary is 1. The
 * difference here is in two cases. If all 3 a, b, and incary are 1, then sum
 * will be 1. Also, if at least two of the three inputs are 1, then outcary will
 * be set to 1. This simulates adding the two and extracting the caryover to the
 * next bit.
 *
 * The boolean function for this C function is:
 *      sum = a ^ b ^ incary
 *      outcary = ab + incary(a + b)
 *
 * NOTE: No matter what a and b are, even if they are signed and negative, the
 * only bits that will be evaluated here are the first bit in a and b. For
 * example, if a is 2 (0000 0010) then it will be evaluated as 0 because the
 * first bit is a 0. So 3 (0000 0011) will be evaluated as 1. Sum and outcary
 * will be returned with only the first bit set if any.
 */
void fulladdr(int a, int b, int incary, int *sum, int *outcary) {
  unsigned int _1(int a);
  *sum = _1(a) ^ _1(b) ^ _1(incary);
  *outcary = (_1(a) & _1(b)) | (_1(incary) & (_1(a) | _1(b)));
}

/* Add the first 4 bits in a and b using 4 full adders. This will take a carry
 * in from the last operation if any and use send that to the full adder. This
 * module will then proceed to use 4 total full adders to add the first 4 bits
 * and set sum to the result and outcary to the last carry over from the last
 * full adder.
 *
 * NOTE: No matter what a and b are, even if they are signed and negative, the
 * only bits that will be evaluated here are the first 4 bits in a and b. For
 * example, if a is 2 (1101 1101 0010) then it will be evaluated as
 * 0000 0000 0010. Sum and outcary will be returned in the same manner with only
 * the first four bits set.
 */
void add4(int a, int b, int incary, int *sum, int *outcary) {
  void fulladdr(int a, int b, int incary, int *sum, int *outcary);
  int ta, tb, tmps, tmpc;
  unsigned int _4(int a);
  *sum = 0;
  ta = _4(a);
  tb = _4(b);
  tmpc = incary;

  fulladdr(ta, tb, tmpc, &tmps, &tmpc);
  *sum = *sum | tmps;
  ta >>= 1;
  tb >>= 1;

  fulladdr(ta, tb, tmpc, &tmps, &tmpc);
  *sum = *sum | tmps<<1;
  ta >>= 1;
  tb >>= 1;

  fulladdr(ta, tb, tmpc, &tmps, &tmpc);
  *sum = *sum | tmps<<2;
  ta >>= 1;
  tb >>= 1;

  fulladdr(ta, tb, tmpc, &tmps, &tmpc);
  *sum = *sum | tmps<<3;

  *outcary = tmpc;
}

/* Returns the absolute value of a.
 * Problem: Find the magnitude of a. The magnitude of a is synonymous to the
 * absolute value of a. For example, if a = 8, abs(a) = 8. Also, if a = -8, then
 * abs(a) is still 8. Think of it as the distance away from 0 that a is. This
 * function is to implement the solution without any control-flow statements.
 * Additionally, it will use the add4 function and the xor operator within it's
 * solution.
 *
 * Solution: The basic idea or this function is to retrieve the 1's compliment
 * of a. This is done by creating a mask for a by bit shifting it 7 times to the
 * right. Note that only the first 4 bits on a will be evaluated. They will be
 * extracted and put into another variable. Next, add a to that mask using the
 * add4 function and then compare that value to mask again using an exclusive or
 * operator.
 */
void magnitude4(int a, int *result) {
  void add4(int a, int b, int incary, int *sum, int *outcary);
  unsigned int _1(int a);
  unsigned int _4(int a);
  int incary=0, sum, outcary;
  char mask;
  char ta = (char) a;

  char tmpmask = _4(ta) >> 3;
  char fmask = _1(tmpmask);
  fmask = (fmask << 1) + _1(tmpmask);
  fmask = (fmask << 1) + _1(tmpmask);
  fmask = (fmask << 1) + _1(tmpmask);
  fmask = (fmask & 0x0F) | (fmask << 4);

  mask = fmask;
  add4((int) mask, (int) ta, incary, &sum, &outcary);
  *result = _4((char) sum ^ mask);
}
