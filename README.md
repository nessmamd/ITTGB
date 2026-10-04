# ITTGB

yay compilers !! this is nothing new it is just me remaking parts of the gcc compiler but with some variation 

## Lexer
so there will be v1 and v2. v1 is the bootstrap level with assembly and then v2 will be the upper layer C that will then run to support the source code. currently only print and let are the identifiers supported but all other operations are supported. 

for the parser, same thing with v1 and v2. the expr, term, primary, and unary will also be supported.


## Build and run

From this directory, run:

```sh
make run
```

This builds the lexer with the system C compiler and runs the sample input. To
only build it, use `make`; to remove the generated executable, use `make clean`.


