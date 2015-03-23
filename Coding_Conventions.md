# Casing #
_Revise these?_
  * Class names should be in `CamelCase`.
  * Method names should be `concatenated_by_underscores`.
  * Macros names must be in `ALL_CAPS`.

# Namimg #
  * Method names should retain the original C method names if possible, e.g.
    * `igraph_vector_null` → `igraph::Vector.null`
    * `igraph_degree` → `igraph::Graph.degree`
    * `igraph_vs_as_vector` → `igraph::VertexSelector.as_vector`
  * If the functions serve similar purpose _and_ return the same type, then they may share the same name, e.g.
    * degree() return degree of all nodes, while degree(VertexSelector::single(6)) return degree of node 6 as a Vector.
    * But degree\_of(6) returns the degree of node 6 as an Integer, so it should take a different name.
  * Use the named constructor idiom unless default constructor really express its default meaning, e.g.
    * Use `igraph::Vector::seq(Integer, Integer)` instead of `igraph::Vector::Vector(Integer, Integer)`. An uninformed seeing the statement `Vector(5., 12.)` may think you are creating a vector with 2 elements.

# Types #
  * Use named enums instead of magic booleans to give a clear meaning to the readers.
  * Use typedef to specify the usage of a general type. For example, Vertex and Edge are both typedefs of the generic type Integer.
  * Return a wrapper type instead of STL type for containers. (e.g. Vector instead of `std::vector<T>`). Users should be instructed to switch between STL and igraph containers by iterators:
    * `std::vector<Integer> stl_vector = std::vector<Integer> (igraph_vector.begin(), igraph_vector.end());`
    * `Vector igraph_vector = Vector (stl_vector.begin(), stl_vector.end());`
  * Always keep in mind that the igraph Integer is typedef-ed to be a double, not an int.
  * Wrapper types must put the original type as the first member, and must not have virtual functions. This ensures conversions like these will succeed in most platforms:
    * ` Graph g = ...; igraph_is_connected( (igraph_t*)&g, ... ); `
> But such coding style should not be used.

# Language features #
  * Avoid unnecessary use of GCC C extensions, TR1 libraries or C++0x features, as these are not available in every compilers. If you really need these, you must justify that the use is beneficial to the user by increased readiness or performance. Always provide an alternative that gives the same result when these feature are absent. You can test for each of these special configurations with:
    * GCC: `#if __GNUC__`
    * C++0x: `#if XXINTRNL_CXX0X`
  * Avoid the use of non-standard library, such as boost libraries. If the functionality of some library is required, all source code should be added them in this project.
    * Avoid using boost features (e.g. `foreach`). Some people may not bother to download and configure boost.
  * Some of these features, however, have been dealt with, so you may freely use them. They are:
    * `__attribute__` (revert to nothing is GCC is not used.)
    * `std::move`
    * `gsl::Random` (revert to `rand()` if GSL is not included.)
  * All internal macro should be prefixed by `XXINTRNL_` so normal people won't see it. They should be used only in the file that defines it. `#undef` them after using if possible.
    * **FIXME**: `XXINTRNL_WRAPPER_CONSTRUCTOR_***` currently breaks this rule.
  * Unless you need `realloc()`, always allocate memory with `new[]`/`delete[]` or `alloca()`.

# Enums #
  * To avoid polluting the shallower namespaces, if an enum is used by methods in only one class, then that enum must be placed in that class, i.e., instead of
```
/** wrong **/
enum FileType {     // used nowhere else.
  Binary,
  ASCII
};
class File {
private:
  std::FILE* fptr;
public:
  File(const char* filename, FileType type); 
  // ...
};
```
> do it like this:
```
/** correct **/
class File {
private:
  std::FILE* fptr;
public:
  enum Type {     // used nowhere else.
    Binary,
    ASCII
  };
  File(const char* filename, Type type); 
  // ...
};
```
> Outside of the class you'd refer to the enum by File::Type and the values by File::Binary and File::ASCII.)

# Compiler support #
  * The following compilers should be supported:
    * Primary:
      * `g++-4.4` (4.4.0 20090327) (Lau's machine)
      * `g++-4.2` (4.2.1) (Kenny's machine)
      * `g++-4.1` (4.1.2 20071124) (MOSIX)
    * Secondary:
      * `llvm-g++-4.2`
      * Microsoft Visual C++ 2008
      * Microsoft Visual C++ 2005 (Barn)

# Documentation syntax #
  * Doxygen syntax should be used.

# License #
  * All new code are licensed in GPLv3.