### Bhuban Chakma (IT-23054)

# Number Theory & Cryptography Workshop

## My Understanding of Day 1 and Day 2

> **Source material:**\
> - **Day 1.pdf** --- Maths and Physics Club, IIT Bombay, December 23,
> 2020\
> - **Day 2.pdf** --- Maths and Physics Club, IIT Bombay, December 24,
> 2020
>
> This README reorganizes the ideas presented in the two PDFs into a
> study guide. The explanations and Python examples are written to make
> the slide topics easier to understand and experiment with. Code
> verifies examples; it does not replace mathematical proofs.

------------------------------------------------------------------------

## Table of Contents

-   [Overview](#overview)
-   [Day 1 --- GCD, Modular Arithmetic and Number-Theoretic
    Tools](#day-1--gcd-modular-arithmetic-and-number-theoretic-tools)
    -   [1. Greatest Common Divisor](#1-greatest-common-divisor-gcd)
    -   [2. Euclid's Algorithm](#2-euclids-algorithm)
    -   [3. Extended Euclid's Algorithm and Bézout's
        Identity](#3-extended-euclids-algorithm-and-bézouts-identity)
    -   [4. Least Common Multiple](#4-least-common-multiple-lcm)
    -   [5. Diophantine Equations](#5-diophantine-equations)
    -   [6. Modular Arithmetic and Modular
        Division](#6-modular-arithmetic-and-modular-division)
    -   [7. Chinese Remainder Theorem](#7-chinese-remainder-theorem-crt)
    -   [8. Fast Modular Exponentiation](#8-fast-modular-exponentiation)
    -   [9. Fermat's Little Theorem](#9-fermats-little-theorem)
    -   [10. Euler's Totient Function and Euler's
        Theorem](#10-eulers-totient-function-and-eulers-theorem)
    -   [11. Day 1 Summary](#day-1-summary)
-   [Day 2 --- Prime Numbers and Primality
    Testing](#day-2--prime-numbers-and-primality-testing)
    -   [1. Prime Numbers](#1-prime-numbers)
    -   [2. Infinitude and Distribution of
        Primes](#2-infinitude-and-distribution-of-primes)
    -   [3. Prime-Related Conjectures](#3-prime-related-conjectures)
    -   [4. Generating and Testing
        Primes](#4-generating-and-testing-primes)
    -   [5. Fermat Test and
        Pseudoprimes](#5-fermat-test-and-pseudoprimes)
    -   [6. Carmichael Numbers](#6-carmichael-numbers)
    -   [7. Further Properties of
        Primes](#7-further-properties-of-primes)
    -   [8. Lucas's Theorem and the n−1
        Test](#8-lucass-theorem-and-the-n1-test)
    -   [9. Mersenne Primes and Perfect
        Numbers](#9-mersenne-primes-and-perfect-numbers)
    -   [10. Antiprimes](#10-antiprimes-highly-composite-numbers)
    -   [11. Day 2 Summary](#day-2-summary)
-   [How the Two Days Connect](#how-the-two-days-connect)
-   [Combined Python Practice](#combined-python-practice)
-   [Important Distinctions and
    Notes](#important-distinctions-and-notes)

------------------------------------------------------------------------

# Overview

The two sessions build a foundation in elementary number theory and show
why it matters in cryptography.

-   **Day 1** develops tools for working with integers: GCD, the
    Euclidean algorithm, modular arithmetic, modular inverses, the
    Chinese Remainder Theorem, modular exponentiation, Fermat's Little
    Theorem and Euler's theorem.
-   **Day 2** focuses on prime numbers: how primes are distributed, how
    to generate or test them, why primality tests can be fooled, and how
    prime numbers connect to perfect numbers and cryptography.

A recurring idea is that calculations with very large integers can often
be made manageable by working with their **remainders modulo an
integer**.

------------------------------------------------------------------------

# Day 1 --- GCD, Modular Arithmetic and Number-Theoretic Tools

## 1. Greatest Common Divisor (GCD)

The **greatest common divisor** of two integers (a) and (b), not both
zero, is the largest positive integer that divides both.

It is written as:

\[ `\gcd`{=tex}(a,b) \]

Examples:

\[ `\gcd`{=tex}(24,16)=8,`\qquad `{=tex}`\gcd`{=tex}(9,17)=1 \]

\[ `\gcd`{=tex}(239,0)=239 \]

If the GCD is 1, the two integers are called **coprime** or **relatively
prime**.

### Why GCD matters

The slides introduce two applications:

1.  Simplifying fractions.
2.  Finding modular inverses, which are important in cryptographic
    protocols.

For example:

\[ `\frac{31}{177}`{=tex}+`\frac{29}{59}`{=tex}
=`\frac{31\cdot59+177\cdot29}{177\cdot59}`{=tex}
=`\frac{6962}{10443}`{=tex} =`\frac23`{=tex} \]

Python can simplify fractions exactly:

``` python
from fractions import Fraction

answer = Fraction(31, 177) + Fraction(29, 59)
print(answer)  # 2/3
```

### Naive GCD algorithm

A direct approach checks every possible divisor from the smaller input
downwards. It is easy to understand but can be extremely slow for large
numbers.

``` python
def gcd_naive(a, b):
    assert a >= 0 and b >= 0 and (a != 0 or b != 0)

    for d in range(min(a, b), 0, -1):
        if a % d == 0 and b % d == 0:
            return d

print(gcd_naive(24, 16))  # 8
```

The worst case can require a number of checks proportional to the
smaller input. This motivates Euclid's algorithm.

------------------------------------------------------------------------

## 2. Euclid's Algorithm

### Euclid's lemma

For integers (a,b,d):

\[ d`\mid `{=tex}a `\text{ and }`{=tex} d`\mid `{=tex}b `\iff`{=tex}
d`\mid`{=tex}(a-b) `\text{ and }`{=tex} d`\mid `{=tex}b \]

The key consequence is:

\[ `\boxed{\gcd(a,b)=\gcd(b,a\bmod b)}`{=tex} \]

This allows us to replace a pair of numbers with a smaller pair without
changing their GCD.

### Algorithm

Repeat the following until the second number becomes zero:

\[ (a,b)`\leftarrow`{=tex}(b,a`\bmod `{=tex}b) \]

The remaining non-zero number is the GCD.

``` python
def gcd_euclid(a, b):
    a, b = abs(a), abs(b)

    while b != 0:
        a, b = b, a % b

    return a

print(gcd_euclid(24, 16))                 # 8
print(gcd_euclid(790933790547, 1849639579327))  # 3416723
```

### Trace an example

``` python
def gcd_trace(a, b):
    a, b = abs(a), abs(b)

    while b:
        print(f"gcd({a}, {b})")
        a, b = b, a % b

    print(f"gcd({a}, 0) = {a}")
    return a

gcd_trace(48, 18)
```

The remainder sequence is:

\[ 48=2(18)+12 \] \[ 18=1(12)+6 \] \[ 12=2(6)+0 \]

Therefore:

\[ `\gcd`{=tex}(48,18)=6 \]

### Why it is efficient

For (a`\ge `{=tex}b\>0), the remainder satisfies:

\[ a`\bmod `{=tex}b \< `\frac `{=tex}a2 \]

To see why:

-   If (b`\le `{=tex}a/2), then (a`\bmod `{=tex}b \< b`\le `{=tex}a/2).
-   If (b\>a/2), then the quotient is 1 and (a`\bmod `{=tex}b=a-b\<a/2).

Thus the numbers shrink rapidly. The slide gives a logarithmic iteration
bound, rather than the potentially enormous number of steps required by
naive subtraction or divisor checking.

------------------------------------------------------------------------

## 3. Extended Euclid's Algorithm and Bézout's Identity

The ordinary Euclidean algorithm finds the GCD. The **extended**
algorithm also finds integers (x,y) satisfying:

\[ `\boxed{ax+by=\gcd(a,b)}`{=tex} \]

This is Bézout's identity.

Examples from the slides:

\[ 2=10(-1)+6(2) \]

\[ 1=7(-2)+5(3) \]

\[ 23=391(-3)+299(4) \]

### Python implementation

``` python
def extended_gcd(a, b):
    """Return (g, x, y) such that a*x + b*y = g = gcd(a, b)."""
    if b == 0:
        return a, 1, 0

    g, x1, y1 = extended_gcd(b, a % b)

    x = y1
    y = x1 - (a // b) * y1

    return g, x, y


g, x, y = extended_gcd(391, 299)

print("gcd =", g)
print("x =", x)
print("y =", y)
print("check =", 391*x + 299*y)
```

Expected result:

``` text
gcd = 23
x = -3
y = 4
check = 23
```

### Modular inverse

An integer (x) is the modular inverse of (a) modulo (n) if:

\[ ax`\equiv1`{=tex}`\pmod `{=tex}n \]

Such an inverse exists **if and only if**:

\[ `\gcd`{=tex}(a,n)=1 \]

If extended Euclid gives:

\[ ax+ny=1 \]

then reducing modulo (n) gives (ax`\equiv1`{=tex}`\pmod `{=tex}n). Thus
(x`\bmod `{=tex}n) is the inverse.

``` python
def modular_inverse(a, n):
    g, x, _ = extended_gcd(a, n)

    if g != 1:
        return None

    return x % n


print(modular_inverse(3, 7))  # 5, because 3*5 = 15 ≡ 1 (mod 7)
```

------------------------------------------------------------------------

## 4. Least Common Multiple (LCM)

The **least common multiple** of positive integers (a,b) is the smallest
positive integer divisible by both.

\[ `\operatorname{lcm}`{=tex}(24,16)=48 \]

The important relationship is:

\[ `\boxed{\operatorname{lcm}(a,b)\cdot\gcd(a,b)=ab}`{=tex} \]

Therefore:

\[ `\boxed{\operatorname{lcm}(a,b)=\frac{ab}{\gcd(a,b)}}`{=tex} \]

### Python

``` python
def lcm(a, b):
    if a == 0 or b == 0:
        return 0
    return abs(a // gcd_euclid(a, b) * b)

print(lcm(24, 16))  # 48
print(lcm(9, 17))   # 153
```

Dividing before multiplying helps avoid unnecessary intermediate growth.

------------------------------------------------------------------------

## 5. Diophantine Equations

A **Diophantine equation** is an equation for which integer solutions
are required.

A central example is:

\[ ax+by=c,`\qquad `{=tex}x,y`\in`{=tex}`\mathbb `{=tex}Z \]

### Existence theorem

The equation has an integer solution if and only if:

\[ `\boxed{\gcd(a,b)\mid c}`{=tex} \]

Why?

-   Every common divisor of (a,b) must divide (ax+by), so it must divide
    (c).
-   Conversely, Bézout's identity gives (ax_0+by_0=d), where
    (d=`\gcd`{=tex}(a,b)). If (c=td), multiplying the identity by (t)
    gives a solution.

### Example

Solve:

\[ 10x+6y=14 \]

Since:

\[ `\gcd`{=tex}(10,6)=2,`\qquad 2`{=tex}`\mid14`{=tex} \]

solutions exist.

Extended Euclid gives:

\[ 2=10(-1)+6(2) \]

Multiplying by 7:

\[ 14=10(-7)+6(14) \]

So one solution is:

\[ (x_0,y_0)=(-7,14) \]

### All integer solutions

Let:

\[ d=`\gcd`{=tex}(a,b),`\quad `{=tex}a=dp,`\quad `{=tex}b=dq \]

If ((x_0,y_0)) is one solution, every solution is:

\[ `\boxed{x=x_0+tq,\qquad y=y_0-tp,\quad t\in\mathbb Z}`{=tex} \]

For (10x+6y=14), (d=2,p=5,q=3), so:

\[ x=-7+3t,`\qquad `{=tex}y=14-5t \]

### Python: find one solution

``` python
def solve_diophantine(a, b, c):
    g, x, y = extended_gcd(a, b)

    if c % g != 0:
        return None

    scale = c // g
    return scale * x, scale * y

print(solve_diophantine(10, 6, 14))
```

The returned pair is one solution. The formula above generates all
solutions.

------------------------------------------------------------------------

## 6. Modular Arithmetic and Modular Division

Congruence modulo (n) means two integers have the same remainder after
division by (n):

\[ a`\equiv `{=tex}b`\pmod `{=tex}n `\iff`{=tex} n`\mid`{=tex}(a-b) \]

For example:

\[ 17`\equiv5`{=tex}`\pmod{12}`{=tex} \]

because (17-5=12).

Addition and multiplication preserve congruence:

\[ a`\equiv `{=tex}b`\pmod `{=tex}n
`\Rightarrow `{=tex}a+c`\equiv `{=tex}b+c`\pmod `{=tex}n \]

\[ a`\equiv `{=tex}b, c`\equiv `{=tex}d`\pmod `{=tex}n
`\Rightarrow `{=tex}ac`\equiv `{=tex}bd`\pmod `{=tex}n \]

### Modular division

Division is not automatically allowed modulo (n). To divide by (a), we
need its modular inverse. This exists precisely when:

\[ `\gcd`{=tex}(a,n)=1 \]

For example, modulo 7, the inverse of 3 is 5:

\[ 3`\cdot5`{=tex}=15`\equiv1`{=tex}`\pmod7`{=tex} \]

Thus solving (3x`\equiv4`{=tex}`\pmod7`{=tex}):

\[ x`\equiv4`{=tex}`\cdot5`{=tex}=20`\equiv6`{=tex}`\pmod7`{=tex} \]

### Python

``` python
a, b, modulus = 3, 4, 7

inverse = modular_inverse(a, modulus)

if inverse is None:
    print("No unique modular division using an inverse.")
else:
    x = (b * inverse) % modulus
    print(x)  # 6
```

For a general congruence (ax`\equiv `{=tex}b`\pmod `{=tex}n), solutions
exist if and only if (`\gcd`{=tex}(a,n)`\mid `{=tex}b). If the GCD is
greater than 1, there may be multiple residue solutions.

------------------------------------------------------------------------

## 7. Chinese Remainder Theorem (CRT)

The Chinese Remainder Theorem combines remainder information from
different moduli.

### Theorem

If (`\gcd`{=tex}(a,b)=1), then for any remainders (r_a,r_b), there is a
solution to:

\[ n`\equiv `{=tex}r_a`\pmod `{=tex}a \] \[
n`\equiv `{=tex}r_b`\pmod `{=tex}b \]

The solution is unique modulo (ab).

### Example

Solve:

\[ n`\equiv2`{=tex}`\pmod3`{=tex} \] \[ n`\equiv3`{=tex}`\pmod5`{=tex}
\]

The numbers congruent to 2 modulo 3 are:

\[ 2,5,8,11,14,`\ldots`{=tex} \]

Among them, 8 is congruent to 3 modulo 5. Therefore:

\[ `\boxed{n\equiv8\pmod{15}}`{=tex} \]

All solutions are (8+15k), where (k) is an integer.

### Constructive formula from the slides

Since (`\gcd`{=tex}(a,b)=1), extended Euclid gives:

\[ ax+by=1 \]

Then:

\[ `\boxed{n=r_a(by)+r_b(ax)}`{=tex} \]

This has remainder (r_a) modulo (a) and remainder (r_b) modulo (b).

### Python

``` python
def chinese_remainder(a, ra, b, rb):
    if gcd_euclid(a, b) != 1:
        raise ValueError("This implementation requires coprime moduli.")

    _, x, y = extended_gcd(a, b)

    n = ra * b * y + rb * a * x
    return n % (a * b)

print(chinese_remainder(3, 2, 5, 3))  # 8
```

The slides also explain that the idea extends to more moduli when every
pair of moduli is coprime. CRT is useful for reconstructing a number
from its remainders and for computations involving large integers.

------------------------------------------------------------------------

## 8. Fast Modular Exponentiation

In cryptography, we frequently need to calculate:

\[ b\^e`\bmod `{=tex}m \]

Computing (b\^e) first can create an enormous integer. Instead, reduce
modulo (m) after every multiplication.

### Repeated multiplication

For (b=7,e=4,m=11):

\[ 1`\to7`{=tex}`\to49`{=tex}`\bmod11`{=tex}=5 \] \[
5`\cdot7`{=tex}=35`\bmod11`{=tex}=2 \] \[
2`\cdot7`{=tex}=14`\bmod11`{=tex}=3 \]

Therefore:

\[ 7\^4`\bmod11`{=tex}=3 \]

### Repeated squaring

Write the exponent in binary. For example:

\[ 13=8+4+1=(1101)\_2 \]

Calculate:

\[ b, b^2, b^4, b\^8 `\pmod `{=tex}m \]

Then multiply the powers corresponding to the 1-bits.

For (7\^{13}`\bmod11`{=tex}):

\[ 7^1`\equiv7`{=tex},`\quad
7`{=tex}^2`\equiv5`{=tex},`\quad
7`{=tex}^4`\equiv3`{=tex},`\quad
7`{=tex}^8`\equiv9`{=tex}`\pmod{11}`{=tex} \]

Thus:

\[ 7\^{13}`\equiv7`{=tex}`\cdot5`{=tex}`\cdot9`{=tex}
`\equiv2`{=tex}`\pmod{11}`{=tex} \]

### Python implementation

``` python
def fast_mod_exp(base, exponent, modulus):
    if modulus <= 0 or exponent < 0:
        raise ValueError("Use a positive modulus and non-negative exponent.")

    result = 1
    base %= modulus

    while exponent > 0:
        if exponent % 2 == 1:
            result = (result * base) % modulus

        base = (base * base) % modulus
        exponent //= 2

    return result

print(fast_mod_exp(7, 13, 11))  # 2

# Python's built-in modular exponentiation:
print(pow(7, 13, 11))           # 2
```

Repeated squaring needs a number of iterations proportional to
(`\log`{=tex}\_2 e), making it much faster than multiplying (b) by
itself (e) times.

------------------------------------------------------------------------

## 9. Fermat's Little Theorem

If (p) is prime and (p`\nmid `{=tex}a), then:

\[ `\boxed{a^{p-1}\equiv1\pmod p}`{=tex} \]

An equivalent form, valid for every integer (a), is:

\[ a\^p`\equiv `{=tex}a`\pmod `{=tex}p \]

This theorem is important for modular exponentiation and primality
testing.

### Example

For (a=2,p=7):

\[ 2\^6=64`\equiv1`{=tex}`\pmod7`{=tex} \]

### Python

``` python
a = 2
p = 7

print(pow(a, p - 1, p))  # 1
```

The theorem gives a necessary condition for primality, but its converse
is false: some composite numbers also satisfy the congruence. Day 2
develops this issue through pseudoprimes.

------------------------------------------------------------------------

## 10. Euler's Totient Function and Euler's Theorem

### Euler's totient function

The Euler totient function (`\phi`{=tex}(n)) counts the integers from 0
to (n-1) that are coprime with (n), following the convention used in the
slides.

Examples:

\[
`\phi`{=tex}(1)=1,`\quad `{=tex}`\phi`{=tex}(2)=1,`\quad `{=tex}`\phi`{=tex}(3)=2,`\quad `{=tex}`\phi`{=tex}(10)=4
\]

For a prime (p):

\[ `\boxed{\phi(p)=p-1}`{=tex} \]

For distinct primes (p,q):

\[ `\boxed{\phi(pq)=(p-1)(q-1)}`{=tex} \]

More generally, if the prime factorization is known:

\[ n=`\prod`{=tex}\_i p_i\^{e_i} \]

then:

\[ `\boxed{\phi(n)=n\prod_{p\mid n}\left(1-\frac1p\right)}`{=tex} \]

### Python

``` python
def euler_phi(n):
    if n < 1:
        raise ValueError("n must be positive")

    count = 0
    for k in range(n):
        if gcd_euclid(k, n) == 1:
            count += 1

    return count

for n in [1, 2, 3, 10, 12]:
    print(n, euler_phi(n))
```

For larger numbers, a factorization-based implementation is more
efficient. Python's `sympy` library also provides `totient`.

### Euler's theorem

If (`\gcd`{=tex}(a,n)=1), then:

\[ `\boxed{a^{\phi(n)}\equiv1\pmod n}`{=tex} \]

This generalizes Fermat's Little Theorem. When (n=p) is prime,
(`\phi`{=tex}(p)=p-1), so Euler's theorem becomes Fermat's Little
Theorem.

The slides connect Euler's theorem and fast modular exponentiation to
RSA encryption and decryption.

``` python
# Example: gcd(3, 10) = 1 and phi(10) = 4
print(pow(3, 4, 10))  # 1
```

------------------------------------------------------------------------

## Day 1 Summary

  --------------------------------------------------------------------------------------------
  Topic                               Main idea or formula
  ----------------------------------- --------------------------------------------------------
  GCD                                 Greatest positive common divisor

  Euclid's algorithm                  (`\gcd`{=tex}(a,b)=`\gcd`{=tex}(b,a`\bmod `{=tex}b))

  Bézout's identity                   (ax+by=`\gcd`{=tex}(a,b))

  Modular inverse                     Exists iff (`\gcd`{=tex}(a,n)=1)

  LCM                                 (`\operatorname{lcm}`{=tex}(a,b)=ab/`\gcd`{=tex}(a,b))

  Diophantine equation                (ax+by=c) is solvable iff
                                      (`\gcd`{=tex}(a,b)`\mid `{=tex}c)

  CRT                                 Coprime moduli combine into one residue modulo their
                                      product

  Fast exponentiation                 Repeated squaring computes (b\^e`\bmod `{=tex}m)
                                      efficiently

  Fermat                              (a\^{p-1}`\equiv1`{=tex}`\pmod `{=tex}p), if
                                      (p`\nmid `{=tex}a)

  Euler                               (a\^{`\phi`{=tex}(n)}`\equiv1`{=tex}`\pmod `{=tex}n), if
                                      (`\gcd`{=tex}(a,n)=1)
  --------------------------------------------------------------------------------------------

------------------------------------------------------------------------

# Day 2 --- Prime Numbers and Primality Testing

## 1. Prime Numbers

A **prime number** is a natural number greater than 1 that cannot be
expressed as a product of two smaller natural numbers.

Examples:

\[ 2,3,5,7,11,13,17,19,`\ldots`{=tex} \]

A number greater than 1 that is not prime is called composite.

Prime numbers are the building blocks of the positive integers. The
Fundamental Theorem of Arithmetic says that every integer greater than 1
has a unique prime factorization, apart from the order of the factors.

Example:

\[ 60=2\^2`\cdot3`{=tex}`\cdot5`{=tex} \]

The slides connect prime numbers to number-theoretic functions, modular
arithmetic, algebra and cryptography. In public-key cryptography,
multiplying two large primes is easy compared with recovering the prime
factors from their product using known practical methods.

------------------------------------------------------------------------

## 2. Infinitude and Distribution of Primes

### Are there infinitely many primes?

The slides present several approaches to proving that there are
infinitely many primes, including:

-   Euclid-style constructions involving a product of known primes plus
    or minus 1.
-   The fact that (n!+1) has a prime factor greater than (n).
-   The divergence of the harmonic series.
-   Proofs attributed in the slides to Goldbach and Filip Saidak.
-   A topological approach.

A simple Euclid-style argument illustrates the central idea.

Assume, for contradiction, that the complete list of primes is:

\[ p_1,p_2,`\ldots`{=tex},p_k \]

Construct:

\[ N=p_1p_2`\cdots `{=tex}p_k+1 \]

No listed prime divides (N), because division by any (p_i) leaves
remainder 1. But every integer greater than 1 has a prime divisor. This
gives a prime outside the supposed complete list, a contradiction.

Therefore, there are infinitely many primes.

### Primes in arithmetic progressions

The slides introduce Dirichlet's theorem:

If (a,b) are positive integers and:

\[ `\gcd`{=tex}(a,b)=1 \]

then there are infinitely many primes of the form:

\[ ak+b \]

The slides emphasize that the coprimality condition is necessary and
sufficient, and that the proof requires analytic number theory rather
than an elementary proof.

### Prime Number Theorem

Let (`\pi`{=tex}(x)) be the number of primes less than or equal to (x).
The Prime Number Theorem states:

\[ `\boxed{\pi(x)\sim\frac{x}{\ln x}}`{=tex} \]

The symbol (`\sim`{=tex}) means that the ratio of the two sides
approaches 1 as (x) grows.

It gives an approximation to the density of primes among large positive
integers.

------------------------------------------------------------------------

## 3. Prime-Related Conjectures

The Day 2 slides list several famous questions. A conjecture is a
statement believed to be true but not established by a proof.

### Goldbach's conjecture

Every even integer greater than 2 can be written as the sum of two
primes.

Examples:

\[ 10=3+7,`\qquad 28`{=tex}=11+17 \]

The PDF phrases the range as even numbers greater than 3; the usual
statement is even integers greater than 2.

### Other conjectures and questions in the slides

-   **Twin Prime Conjecture:** Are there infinitely many pairs of primes
    that differ by 2?
-   **Prime values of (n\^2+1):** Are there infinitely many primes of
    this form?
-   **Primes between consecutive squares:** Is there always a prime
    between (n\^2) and ((n+1)\^2)?
-   **Prime spirals:** A visual pattern arising from arranging integers
    in a spiral and marking primes.
-   **Waring's prime number conjecture:** The slides state that every
    odd number greater than 3 is either prime or a sum of three primes.

The slides also mention that every integer can be represented in binary,
and that every positive integer is a sum of at most four squares.

These topics illustrate how many basic-looking questions about primes
remain difficult.

------------------------------------------------------------------------

## 4. Generating and Testing Primes

Generating candidate primes and testing whether a number is prime are
different tasks.

### Polynomial examples

The slides discuss:

\[ n\^2+n+41 \]

This polynomial produces prime values for many small non-negative
integers, but it does not produce primes for every integer. For example,
at (n=40):

\[ 40^2+40+41=1681=41^2 \]

So it is composite.

### Sieve of Eratosthenes

To find all primes up to (N):

1.  List integers from 2 to (N).
2.  Start with the smallest unmarked number.
3.  Mark its multiples as composite.
4.  Continue until the next candidate exceeds (`\sqrt `{=tex}N).
5.  The unmarked numbers are prime.

Why only go up to (`\sqrt `{=tex}N)? Every composite number (n) has at
least one factor no greater than (`\sqrt `{=tex}n).

### Python: Sieve of Eratosthenes

``` python
def sieve(n):
    if n < 2:
        return []

    is_prime = [True] * (n + 1)
    is_prime[0] = is_prime[1] = False

    p = 2
    while p * p <= n:
        if is_prime[p]:
            for multiple in range(p * p, n + 1, p):
                is_prime[multiple] = False
        p += 1

    return [i for i in range(2, n + 1) if is_prime[i]]

print(sieve(50))
```

### Trial division

To test a single integer (n), try possible divisors up to
(`\sqrt `{=tex}n).

``` python
def is_prime_trial(n):
    if n < 2:
        return False
    if n == 2:
        return True
    if n % 2 == 0:
        return False

    d = 3
    while d * d <= n:
        if n % d == 0:
            return False
        d += 2

    return True

for number in [1, 2, 7, 21, 97, 100]:
    print(number, is_prime_trial(number))
```

The slides also mention optimizing trial division by checking only
primes, and the **wheel of 30**. Since numbers divisible by 2, 3 or 5
cannot be prime (except 2, 3 and 5 themselves), candidates greater than
5 can be restricted to residues:

\[ 1,7,11,13,17,19,23,29`\pmod{30}`{=tex} \]

------------------------------------------------------------------------

## 5. Fermat Test and Pseudoprimes

### Fermat's Little Theorem

If (p) is prime, then for any integer (a):

\[ a\^p`\equiv `{=tex}a`\pmod `{=tex}p \]

In particular, if (p`\nmid `{=tex}a):

\[ `\boxed{a^{p-1}\equiv1\pmod p}`{=tex} \]

This gives a probable-prime test:

1.  Choose a base (a).
2.  Compute (a\^{n-1}`\bmod `{=tex}n).
3.  If the result is not 1 (and the base is coprime to (n)), (n) is
    composite.
4.  If the result is 1, the test has not proved that (n) is prime.

### Fermat pseudoprime

A composite integer (n) that satisfies:

\[ a\^{n-1}`\equiv1`{=tex}`\pmod `{=tex}n \]

for a base (a) coprime to (n) is called a Fermat pseudoprime to that
base.

Examples from the slides:

-   \(341\) is a pseudoprime to base 2, but not base 3.
-   \(91\) is a pseudoprime to base 3, but not base 2.
-   \(561\) passes the Fermat congruence for every base coprime to it.

### Python: Fermat test

``` python
def fermat_test(n, a):
    if n < 2:
        return False

    if gcd_euclid(a, n) != 1:
        return False

    return pow(a, n - 1, n) == 1


for n, a in [(341, 2), (341, 3), (91, 3), (91, 2), (561, 2)]:
    print(f"n={n}, base={a}, passes={fermat_test(n, a)}")
```

A passing result means **probable prime for that base**, not proven
prime. Testing more bases reduces some risks, but does not eliminate
Carmichael numbers.

------------------------------------------------------------------------

## 6. Carmichael Numbers

A **Carmichael number** is a composite positive integer (n) such that:

\[ a\^n`\equiv `{=tex}a`\pmod `{=tex}n \]

for every integer (a).

The slides give the first examples:

\[ 561, 1105, 1729, 2465, 2821 \]

A characterization stated in the slides is:

A positive integer (n) is a Carmichael number if and only if it is
composite, squarefree, and for every prime divisor (p) of (n):

\[ `\boxed{p-1\mid n-1}`{=tex} \]

Squarefree means that no prime-square divides (n).

For example:

\[ 561=3`\cdot11`{=tex}`\cdot17`{=tex} \]

It is squarefree, and:

\[
2`\mid560`{=tex},`\quad10`{=tex}`\mid560`{=tex},`\quad16`{=tex}`\mid560`{=tex}
\]

Therefore it satisfies the stated characterization.

The slides also state that there are infinitely many Carmichael numbers.
Their existence explains why Fermat's test alone is not a complete
primality test.

### Python: inspect a Carmichael example

``` python
from math import prod

n = 561
factors = [3, 11, 17]

print("Product:", prod(factors))
print("Squarefree:", len(set(factors)) == len(factors))

for p in factors:
    print(f"{p}-1 divides {n}-1:", (n - 1) % (p - 1) == 0)

# Check Fermat congruence for several bases coprime to 561
for a in [2, 5, 7, 10]:
    if gcd_euclid(a, n) == 1:
        print(a, pow(a, n, n))
```

------------------------------------------------------------------------

## 7. Further Properties of Primes

The slides list several useful characterizations.

For an integer (p\>1), the following are equivalent to (p) being prime:

1.  Every non-zero residue modulo (p) has a multiplicative inverse.

2.  (`\phi`{=tex}(p)=p-1).

3.  The only solutions to:

    \[ x\^2`\equiv1`{=tex}`\pmod `{=tex}p \]

    are (x`\equiv1`{=tex}) and (x`\equiv-1`{=tex}`\pmod `{=tex}p).

For prime (p), multiplication by a non-zero residue permutes the
non-zero residues:

\[ a,2a,3a,`\ldots`{=tex},(p-1)a`\pmod `{=tex}p \]

when (p`\nmid `{=tex}a). The results are exactly the residues
(1,2,`\ldots`{=tex},p-1), in some order.

### Primitive roots

A primitive root modulo a prime (p) is a number (a) whose powers
generate every non-zero residue modulo (p).

Its order is (p-1):

\[ a\^{p-1}`\equiv1`{=tex}`\pmod `{=tex}p \]

and no smaller positive exponent gives 1.

The slides connect primitive roots with generators, order and
periodicity.

------------------------------------------------------------------------

## 8. Lucas's Theorem and the n−1 Test

### Lucas primality criterion

The slides state the following form:

Let (n\>1). If for every prime factor (q) of (n-1), there exists an
integer (a) such that:

\[ a\^{n-1}`\equiv1`{=tex}`\pmod `{=tex}n \]

and:

\[ a\^{(n-1)/q}`\not`{=tex}`\equiv1`{=tex}`\pmod `{=tex}n \]

then (n) is prime.

The idea is to show that the multiplicative order of a suitable residue
is large enough to force the group of invertible residues to have at
least (n-1) elements. Since there are at most (n-1) non-zero residues,
this forces (n) to be prime.

### The n−1 test

The slides also give a partial-factorization version. Suppose:

\[ n-1=FR,`\qquad `{=tex}F\>R,`\qquad`{=tex}`\gcd`{=tex}(F,R)=1 \]

and the factorization of (F) is known. If for every prime factor (q) of
(F), an integer (a\>1) can be found satisfying:

\[ a\^{n-1}`\equiv1`{=tex}`\pmod `{=tex}n \]

and:

\[ `\gcd`{=tex}`\left`{=tex}(a\^{(n-1)/q}-1,n`\right`{=tex})=1 \]

then (n) is prime.

The slides name Pepin's test, Proth's theorem, the Lucas--Lehmer test
and (n+1) tests as related primality-testing methods.

These are more advanced tests than trial division or the basic Fermat
test.

------------------------------------------------------------------------

## 9. Mersenne Primes and Perfect Numbers

### Mersenne numbers

A Mersenne number has the form:

\[ M_n=2\^n-1 \]

If (M_n) is prime, it is called a **Mersenne prime**.

The conjecture that every number of the form (2\^n-1) is prime is false.
For example:

\[ 2\^4-1=15=3`\cdot5`{=tex} \]

The slides list exponents such as:

\[ 2,3,5,7,13,17,19,31,61,89,107,127 \]

for which the corresponding Mersenne numbers are prime.

If (2\^n-1) is prime, then (n) must itself be prime. Otherwise, if
(n=uv) with (u,v\>1), then:

\[ 2^n-1=(2^u)\^v-1 \]

which is divisible by (2\^u-1), so it is composite.

### Perfect numbers

A positive integer is called **perfect** if it equals the sum of all its
positive divisors excluding itself.

Examples:

\[ 6=1+2+3 \]

\[ 28=1+2+4+7+14 \]

The first few perfect numbers listed in the slides are:

\[ 6, 28, 496, 8128 \]

### Connection to Mersenne primes

The Day 2 assignment gives the theorem:

\[ `\boxed{N=2^{n-1}(2^n-1)}`{=tex} \]

is an even perfect number if and only if (2\^n-1) is prime.

This is the Euclid--Euler characterization of even perfect numbers.

### Python: generate even perfect numbers

``` python
def even_perfect_numbers(max_exponent):
    results = []

    for n in range(2, max_exponent + 1):
        mersenne = 2**n - 1

        if is_prime_trial(mersenne):
            perfect = 2**(n - 1) * mersenne
            results.append((n, mersenne, perfect))

    return results

for exponent, mersenne, perfect in even_perfect_numbers(20):
    print(
        f"n={exponent}, M_n={mersenne}, "
        f"even perfect number={perfect}"
    )
```

------------------------------------------------------------------------

## 10. Antiprimes (Highly Composite Numbers)

An **antiprime**, also called a highly composite number, is a positive
integer with more positive divisors than every smaller positive integer.

The slides list examples:

\[ 1,2,4,6,12,24,36,48,60,`\ldots`{=tex},360 \]

### Number-of-divisors formula

If the prime factorization is:

\[ n=p_1^{a_1}p_2^{a_2}`\cdots `{=tex}p_k\^{a_k} \]

then the number of positive divisors is:

\[ `\boxed{d(n)=(a_1+1)(a_2+1)\cdots(a_k+1)}`{=tex} \]

Why? A divisor independently chooses an exponent from 0 through (a_i)
for each prime (p_i).

For example:

\[ 60=2^2`\cdot3`{=tex}^1`\cdot5`{=tex}\^1 \]

Therefore:

\[ d(60)=(2+1)(1+1)(1+1)=12 \]

The slides list further structural properties as questions for
investigation:

-   Prime factors of an antiprime are consecutive primes.
-   Exponents are non-increasing when primes are listed in increasing
    order.
-   A result attributed to Ramanujan: if (n) has (k) divisors, then the
    (k)-th antiprime has a particular exponent property.

These are presented as topics/properties to prove or explore in the
slides.

### Python: count divisors

``` python
def divisor_count(n):
    if n < 1:
        raise ValueError("n must be positive")

    count = 0
    for d in range(1, n + 1):
        if n % d == 0:
            count += 1

    return count

for n in [1, 2, 4, 6, 12, 24, 36, 48, 60]:
    print(n, divisor_count(n))
```

A simple way to find highly composite numbers up to a limit is to
compare each number's divisor count with the largest count seen so far.

``` python
def highly_composite_numbers(limit):
    result = []
    best_count = 0

    for n in range(1, limit + 1):
        count = divisor_count(n)

        if count > best_count:
            result.append((n, count))
            best_count = count

    return result

print(highly_composite_numbers(100))
```

This implementation is intended for learning and small limits, not
large-scale computation.

------------------------------------------------------------------------

## Day 2 Summary

  ----------------------------------------------------------------------------------
  Topic                               Main idea
  ----------------------------------- ----------------------------------------------
  Prime                               Integer greater than 1 with no non-trivial
                                      factorization

  Infinitely many primes              Euclid-style contradiction proves primes never
                                      end

  Dirichlet's theorem                 Coprime arithmetic progressions contain
                                      infinitely many primes

  Prime Number Theorem                (`\pi`{=tex}(x)`\sim `{=tex}x/`\ln `{=tex}x)

  Sieve                               Finds all primes up to a chosen limit

  Fermat test                         Uses (a\^{n-1}`\bmod `{=tex}n) as a
                                      compositeness/probable-prime test

  Pseudoprime                         Composite number passing a Fermat test for a
                                      base

  Carmichael number                   Composite number passing the Fermat congruence
                                      for every coprime base

  Primitive root                      Residue whose powers generate all non-zero
                                      residues modulo a prime

  Lucas criterion                     Uses multiplicative orders and factors of
                                      (n-1) to prove primality

  Mersenne prime                      Prime of the form (2\^n-1)

  Perfect number                      Equals the sum of its proper positive divisors

  Antiprime                           Has more divisors than every smaller positive
                                      integer
  ----------------------------------------------------------------------------------

------------------------------------------------------------------------

# How the Two Days Connect

The two sessions are not isolated topics. Day 1 supplies the
mathematical tools that Day 2 uses.

``` mermaid
flowchart TD
    A["Day 1: Integer arithmetic"] --> B["GCD and Euclid's algorithm"]
    B --> C["Extended Euclid and modular inverses"]
    C --> D["Congruences and modular division"]
    D --> E["Chinese Remainder Theorem"]
    D --> F["Fast modular exponentiation"]
    F --> G["Fermat's Little Theorem"]
    G --> H["Day 2: Primality testing"]
    H --> I["Pseudoprimes and Carmichael numbers"]
    H --> J["Lucas tests and Mersenne primes"]
    J --> K["Perfect numbers"]
```

The key connection is **modular arithmetic**. It allows us to work with
remainders rather than huge integers. Fermat's and Euler's theorems then
provide exponent identities that support primality testing and
public-key cryptography.

------------------------------------------------------------------------

# Combined Python Practice

The following cell brings together a few of the most useful functions
from both days. It is suitable for Google Colab.

``` python
from math import isqrt


# DAY 1: GCD
def gcd(a, b):
    a, b = abs(a), abs(b)

    while b:
        a, b = b, a % b

    return a


# DAY 1: EXTENDED GCD
def extended_gcd(a, b):
    if b == 0:
        return a, 1, 0

    g, x1, y1 = extended_gcd(b, a % b)

    return g, y1, x1 - (a // b) * y1


# DAY 1: MODULAR INVERSE
def mod_inverse(a, n):
    g, x, _ = extended_gcd(a, n)

    if g != 1:
        return None

    return x % n


# DAY 1: LCM
def lcm(a, b):
    if a == 0 or b == 0:
        return 0

    return abs((a // gcd(a, b)) * b)


# DAY 1: FAST MODULAR EXPONENTIATION
def mod_exp(a, e, m):
    if e < 0 or m <= 0:
        raise ValueError("Require e >= 0 and m > 0")

    result = 1
    a %= m

    while e:
        if e & 1:
            result = (result * a) % m

        a = (a * a) % m
        e >>= 1

    return result


# DAY 2: PRIME TEST BY TRIAL DIVISION
def is_prime(n):
    if n < 2:
        return False

    if n % 2 == 0:
        return n == 2

    for d in range(3, isqrt(n) + 1, 2):
        if n % d == 0:
            return False

    return True


# DAY 2: SIEVE OF ERATOSTHENES
def sieve(limit):
    if limit < 2:
        return []

    flags = [True] * (limit + 1)
    flags[0] = flags[1] = False

    for p in range(2, isqrt(limit) + 1):
        if flags[p]:
            for multiple in range(p * p, limit + 1, p):
                flags[multiple] = False

    return [n for n in range(2, limit + 1) if flags[n]]


# DAY 2: FERMAT TEST
def fermat_test(n, base):
    if n < 2 or gcd(n, base) != 1:
        return False

    return mod_exp(base, n - 1, n) == 1


# DEMONSTRATION
print("GCD:", gcd(24, 16))
print("Extended GCD:", extended_gcd(10, 6))
print("Inverse of 3 modulo 7:", mod_inverse(3, 7))
print("LCM:", lcm(24, 16))
print("7^13 mod 11:", mod_exp(7, 13, 11))

print("Primes up to 50:", sieve(50))
print("Is 97 prime?", is_prime(97))
print("Does 341 pass Fermat base 2?", fermat_test(341, 2))
print("Does 341 pass Fermat base 3?", fermat_test(341, 3))
```

------------------------------------------------------------------------

# Important Distinctions and Notes

1.  **A Fermat test is not a proof of primality.** A composite number
    can pass the test for one base, and Carmichael numbers pass it for
    every base coprime to the number.
2.  **A modular inverse does not always exist.** It exists exactly when
    the number and modulus are coprime.
3.  **CRT's simple product-modulus form requires coprime moduli.** The
    implementation above deliberately checks this condition.
4.  **The Prime Number Theorem is an asymptotic statement.** It is an
    approximation for large (x), not an exact formula for every (x).
5.  **The Sieve and trial division solve different tasks.** A sieve is
    useful for listing many primes up to a limit; trial division is
    useful for testing one reasonably sized integer.
6.  **The Day 2 assignment's pseudoprime problem is stronger than
    experimentation.** Python can generate and verify examples, but the
    claim that infinitely many pseudoprimes exist requires a
    mathematical construction and proof.
7.  **The Day 1 slides use the convention (`\phi`{=tex}(1)=1).** This
    README follows that convention in the totient explanation.

## Final Understanding

My overall understanding is that these two days teach how the structure
of integers can be used to solve computational problems.

Day 1 explains how to reduce large calculations to smaller remainder
calculations and how GCD, modular inverses, CRT and exponentiation work
together. Day 2 uses those tools to study prime numbers, test primality,
understand pseudoprimes and explore special classes such as Mersenne
primes, perfect numbers and antiprimes.

The most important takeaway is that **number theory is not only abstract
mathematics; its properties form the mathematical foundation of many
cryptographic operations.**
