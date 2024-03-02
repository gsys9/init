# runtimed
A simple, dependency-oriented init system.

# Switch to C++
runtimed was recently rewritten in C++ from an
older Python version. There were many reasons for
this change; most of them were well-justified, and
one was petty.

1.  C++ will offer better performance than Python
    code in almost all possible situations.

2.  C++ code will produce a smaller binary than
    Python code, which does not have binaries at
    all, and relies on an interpreter that takes
    up between 30 and 100 MB.

3. Python does not offer `goto` functionality.
    This is the petty reason, and I am aware that
    many people dislike `goto` for some reason.
    `goto` was used in this program because I
    believe that it can make program flow simpler
    to understand.