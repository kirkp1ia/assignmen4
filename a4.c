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
  void paritychk4(int a, int *rslt);
  void partiy3gen(int a, int *outparity);
  void mux2by1(int a, int b, int select, int *out);
  void mux4by1(int a, int b, int select, int *out);

  int a=7, b=0xa8, s, oc, parity, out;

  halfaddr(a, b, &s, &oc);
  printf("s: %d\nco: %d\n", s, oc);

  fulladdr(a, b, oc, &s, &oc);
  printf("s: %d\nco: %d\n", s, oc);

  add4(a, b, 0, &s, &oc);
  printf("s: %d\nco: %d\n", s, oc);

  magnitude4(a, &s);
  printf("magnitude: %d\n", s);

  partiy3gen(a, &parity);
  printf("parity: %d\n", parity);

  paritychk4(a, &parity);
  printf("parity: %d\n", parity);

  mux2by1(a, b, 2, &out);
  printf("out: %d\n", out);

  mux4by1(a, b, 5, &out);
  printf("out: %d\n", out);

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

/* Problem: Set *outparity to 1 or 0 based off whether the 3 bits of a given
 * hold a even or odd number of 1s respectively. This is done for error checking.
 *
 * Solution: Recieves int a and the pointer to outparity. We create 3 intergers
 * (x, y, z). We then set z to equal a & 1 to extract the 0s place. We then set
 * y to equal a & 2 to extract the 1s place. Also bit shift y one place to the
 * right to  to get the 1 or 0 in the 0s place. We then set x to equal a & 4 to
 * extract the 2s place. We also bit shift right x two places to get x to be
 * either 1 or 0 in the 0s place. Then set *outparity to be a 1 or a 0 based on
 * the boolean function that we set it to.
 *
 * The boolean function for this C function is:
 *       *outparity = (~x & ~(y^x))
 *
 * Sets outparity to 1 if a has an even number of bits set to on. Otherwise, it
 * will be set to 0.
 */
void partiy3gen(int a, int *outparity){
  unsigned int _1(int a);
  int x, y, z, xp, yxorz_p, yxorz;

  z = a & 1;
  y = a & 2;
  y = y >> 1;
  x = a & 4;
  x = x >> 2;

  xp = _1(~x);
  yxorz_p = _1(~(_1(y)^_1(z)));
  yxorz = _1(_1(y)^_1(z));
  *outparity = _1( _1(xp & yxorz_p) | _1(x & yxorz) );
}

/* Problem: Set *rslt to 1  or 0 based of whether the 3 bits that follow the
 * parity bit (0s place) of a, is accurately represented by the parity bit. For
 * example if 1110 is given it will set *rslt to 1 because 0 is the parity bit
 * meaning that the 3 follow bits will have a odd number of 1's. Which is
 * correct.
 *
 * Solution: Recieves int a and the pointer to rslt. We create 4 intergers
 * (p, x, y ,z). We set p to equal a & 1, to set it to the parity bit. We then
 * set z to equal a & 2 to extract the 0s place.We then set y to equal a & 4 to
 * extract the 1s place. Also bit shift y two place to the right to  to get the
 * 1 or 0 in the 0s place. We then set x to equal a & 8 to extract the 2s place.
 * We also bit shift right x three places to get x to be either 1 or 0 in the 0s
 * place. We then set *rslt to equal 1 or 0 based off the boolean function that
 * compares p the parity bit and the boolean function that will return the
 * correct parity bit. If p = 1 and the boolean function returns 0 then we know
 * the parity bit was wrong and will return a 0 for false. This also will work
 * if the parity by get it correct and will return 1 for true.
 *
 * The boolean function for this C function is:
 *         *rslt = p & (~x & ~(y^z));
 *
 * Sets rslt to 1 if a has an odd number of bits set to 1. Otherwise, set rslt
 * to 0.
 */
void paritychk4(int a, int *rslt){
  int p, x, y, z, xp, yxorz_p, yxorz, actparity;

  p = a & 1;
  z = a & 2;
  z = z >> 1;
  y = a & 4;
  y = y >> 2;
  x = a & 8;
  x = x >> 3;

  xp = _1(~x);
  yxorz_p = _1(~(_1(y)^_1(z)));
  yxorz = _1(_1(y)^_1(z));
  actparity = _1( _1(xp & yxorz_p) | _1(x & yxorz) );
  *rslt = _1(_1(p) & actparity);
}

/* Problem: Set *out to equal a or b based off the whether the select variable
 * is 1 or 0. Returns a when select = 1, and b when select = 0;
 *
 * Solution: We recieved the three values a, b, and select. We then declare c as
 * an array of intergers the size of 2. We do this so that we can set b to the 0
 * index and a to the 1 index. Before we set the values in the array we first
 * extract the 4 lower order bits of each a and b. Then set b to c[0], and a to
 * c[1]. Then we extract 1 or 0 from the first bit is select to use that as the
 * selection variable. We set *out to c[select]. Because this will now set *out
 * to b if select = 0. Also will set *out to a when select = 1;
 */
void mux2by1(int a, int b, int select, int *out){
  int c[2];

  c[1] = a & 15;
  c[0] = b & 15;
  select = select & 1;

  *out = c[select];
}

/* Problem: Simulate a 4 by 1 multiplexer using the 2x1 multiplexer previously
 * in sequence. Use the first lower order bits in b as the four inputs. The
 * first 3 lower order bits of select will be used as the 3 select bits for the
 * three multiplexers that will be used. The parameter a is not used. The
 * assignment does not include it in what to use.
 *
 * Solution: Extract the lower order bits from b and set them to their
 * individual values. Then set the lower 3 bits in select to their individual
 * bits. Now put the first two bits (left to right) in the inputs into a 2 by 1
 * multiplexer (Using the function written above) with the first bit in the
 * select int. Do this for the next two inputs and the next select bit. Now the
 * two results from those multiplexers are put into a third multiplexer with the
 * third select bit. The result of this multiplexer is set to the parameter out.
 */
void mux4by1(int a, int b, int select, int *out) {
  void mux2by1(int a, int b, int select, int *out);
  int w, x, y, z, s0, s1, s2, muxwx, muxyz;

  z = (b&1);
  y = (b&2) >> 1;
  x = (b&4) >> 2;
  w = (b&8) >> 3;

  s0 = (select&1);
  s1 = (select&2) >> 1;
  s2 = (select&4) >> 2;

  mux2by1(w, x, s0, &muxwx);
  mux2by1(y, z, s1, &muxyz);

  mux2by1(muxwx, muxyz, s2, out);
}
