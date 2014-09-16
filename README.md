# SimpleParser

... is a simple parser for resolving mathematical terms. 

The term is parsed by generating a binary expression tree using the [Shunting-Yard](http://en.wikipedia.org/wiki/Shunting-yard_algorithm) algorithm.
The implementation itself does not use any external libraries and relies fully on the features provided by the C++ language and the standard library.

## Current features:

- Calculating terms with basic operators while respecting the priority of each operator
- Support for parentheses
- Support for alphabetic constants
- Export of the expression tree as [Graphviz](http://www.graphviz.org/) dot for visualization

## Requirements:

- C++ compiler with support for C++14
- [GoogleTest](http://code.google.com/p/googletest/) for tests
