
The following are coding style guidelines for this project.

Code is expected to be ANSI C compatible (C89).  Use `/* ... */` comments, not
`// ...` comments.

To automatically format all source code, install
[clang-format](https://clang.llvm.org/docs/ClangFormat.html) and
run `make format`.

## Blocks

The indentation rules are a variant on the K&R style.
Tabs are used for indentation, spaces for alignment, eg.
```c
for (...) {
------>|function_call(..., ...,
------>|              .....);
}
```
Braces belong on the end of lines, eg.
```c
while (...) {
	if (...) {
		...
	} else {
		...
	}
}
```
The one exception is functions, where the brace goes on the next line:
```c
void foo(...)
{
	...
}
```

## Naming

Types are named with [camel case](https://en.wikipedia.org/wiki/Camel_case),
eg. `HashTable`.
All others (functions, variables, struct fields, etc.) are named with [snake
case](https://en.wikipedia.org/wiki/Snake_case), eg. `hash_table_new`.

## Testing

No code will be accepted without unit tests. See the [test](test/) directory
for what these should look like. Run `make check` to invoke the unit tests.

Tests should [cover](https://en.wikipedia.org/wiki/Code_coverage) at least
95% of lines.  Configure the project using `./configure --enable-coverage` and
then run `make check`.

## Documentation

All public interfaces must be documented using
[Doxygen](https://www.doxygen.nl/). For example:
```c
/**
 * Retrieve the number of entries in a hash table.
 *
 * @param hash_table          The hash table.
 * @return                    The number of entries in the hash table.
 */
unsigned int hash_table_num_entries(HashTable *hash_table);
```
