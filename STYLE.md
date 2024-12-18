
The following are coding style guidelines for this project.

To automatically format all source code, install
[clang-format](https://clang.llvm.org/docs/ClangFormat.html) and
run `make format`.

## Standard compliance

Code is expected to be ANSI C compatible (C89). This means:

* All variables must be declared at the beginning of a block.
* Use `/* ... */` comments, not `// ...` comments.
* There is no bool type (as added in C99); `int` is used for boolean values.
* Other features added by later standards may not be used, such as inline
  functions, variable-length arrays or `long long`.

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

## Comments

Code should be commented using full English sentences. In general, try not to
document "what" the code is doing, but rather the "how" and "why". Most people,
for example, would agree that the following is an example of a comment that is
not useful:
```c
    /* Free the node */
    free(node);
```
The following is a comment that is more enlightening to the reader:
```c
    /* The node to be removed must be swapped with an "adjacent"
     * node, ie. one which has the closest key to this one. Find
     * a node to swap with. */
    swap_node = avl_tree_node_get_replacement(tree, node);
```
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

## Modularity

The project is structured in a modular way, such that each module (`.c` and
`.h` file) is independent of the others. The idea is that anyone should be able
to use any file in an "off the shelf" way by trivially copying those two files
into their project.
