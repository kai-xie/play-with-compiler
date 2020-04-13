# play-with-compiler

## Build and Test

1. Install [Bazel](https://docs.bazel.build/versions/master/install.html) or [Bazelisk](https://github.com/bazelbuild/bazelisk)
2. Build: run command:
   ``` bash
   bazel build //...
   ```
3. Test: now the project should build, then run the compiled executables, e.g.:
    ``` bash
    ./bazel-bin/src/simple_lexer_test
    ./bazel-bin/src/simple_parser_test
    ./bazel-bin/src/simple_script_test    
    ```

## Note

Currently available tests:

``` bash
./bazel-bin/src/simple_lexer_test

./bazel-bin/src/simple_parser_test
./bazel-bin/src/simple_parser_test -v=1     # print parsing log
./bazel-bin/src/simple_parser_test -v=2     # print more detailed parsing log

./bazel-bin/src/simple_script_test
./bazel-bin/src/simple_script_test -v     # print detailed parsing log
```
