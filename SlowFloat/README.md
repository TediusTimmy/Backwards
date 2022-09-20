SlowFloat
=========

SlowFloat is a very simple single-precision decimal floating point library. The author got tired of ((0.1 * 10.0) != 1.0) evaluating true.

SlowFloat has nine decimal digits of precision. It uses just C++ with no inline assembly. It just requires uint64_t to be a type. Yes, it is slow, but it also gets rid of certain idiosyncrasies in doing math with a computer.
