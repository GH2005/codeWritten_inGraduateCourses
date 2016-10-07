egcd.cpp: C++ implementation of the Extended-GCD algorithm. It takes
as input two non-negative integers a and b, such that a >= b >=0 and
a> 0, and finds three integers x, y and g such that ax + by =g where
g is the gcd of a and b.  

divisionOfBigNumbers.cpp: division of big numbers.
My algorithm to perform this task is emulating the “school method” of division. For example, if n = 29583, m = 30, then I’ll find the counts of 30000, 3000, 300, 30 separately in n, and these are digits of q. After subtracting 30000, 3000, 300, 30 from n multiple times that are their corresponding counts, the remainder r is got.

In the code, the actual method to do this division is treating integers as c-style character strings. Both n and m can be up to 50 digits long, but should not have leading zeros.

The time complexity of this algorithm is O(nm) that pushes the number of subtractions for getting each quotient digit to the worst case, nine.

