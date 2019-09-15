# play-with-compiler

## Build and Test

1. Install [Bazel](https://docs.bazel.build/versions/master/install.html)
2. Build: run command:
   ``` bash
   bazel build //...
   ```
3. Test: now the project should build, then run the compiled executables, e.g.:
    ``` bash
    ./bazel-bin/src/simple_lexer_test
    ./bazel-bin/src/simple_parser_test
    ```

## Note

Currently available tests:

``` bash
./bazel-bin/src/simple_lexer_test
./bazel-bin/src/simple_parser_test
```
