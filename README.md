# SKYLINE STREAM APPLICATION
An application computing the skyline of a stream of tuples.

Project of the course "Parallel and Distributed Systems: Paradigms and Models" at the Master Course in Computer Science, at the University of Pisa, Academic year: 2018 / 2019.

## PROJECT DESCRIPTION

The target is to develop an application computing the skyline of a stream of tuples. We consider tuples made of k items. Items at position k have type type<sub>k</sub>. For each type<sub>k</sub>, an order is defined by a ><sub>k</sub> relation. A tuple t<sub>1</sub> = <x<sub>1</sub>, ... , x<sub>k</sub>> dominates a tuple t<sub>2</sub> = <y<sub>1</sub>, ... , y<sub>k</sub>> if and only if at least one component of t<sub>1</sub> (x<sub>j</sub>) it is better than the corresponding component of t<sub>2</sub> (x<sub>j</sub> ><sub>k</sub> y<sub>j</sub>) and all the other components of t<sub>1</sub> (the x<sub>j</sub> such that i != j) are better or equal. (i.e. either x<sub>i</sub> ><sub>k</sub> y<sub>i</sub> or x<sub>i</sub> =<sub>k</sub> y<sub>i</sub>). The skyline of a set of tuples is the set of tuples that are not dominated by any other tuple.
The skyline stream application considers a (possibly infinite) stream of tuples. For each window of size n (n consecutive tuples), the application computes the skyline and outputs the skyline onto the output stream. Consecutive windows differ by exactly k item (sliding factor). As an example, the stream:

a, b, c, d, e, f, g, h, i, l, m, n, o

with n = 4 and k = 2 results in the following windows:

<a, c, e, g>,

<b, d, f, h>, 

<c, e, g, i>, 

<d, f, h, l>, ...

In order to execute the tests, a random stream of integer tuples of k items each must be used, with the classical integer comparison arithmetic.
