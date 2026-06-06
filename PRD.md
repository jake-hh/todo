# PRD: Generic Dynamic Array (Vector)

## Problem Statement

The existing codebase contains a partial `Vector` implementation that is limited to `int` only,
uses non-standard method names, lacks exception handling, and is missing many required operations.
The assignment requires a fully generic, template-based dynamic array that matches the interface
of `std::vector` conceptually, with a prescribed API that must be followed exactly.

## Solution

Rewrite `Vector` as a C++ class template `Vector<T>` that stores elements of any single type,
grows automatically when needed, and exposes the exact method signatures specified in the
assignment. The existing scaffolding (growth factor constant, internal `ar`/`cap`/`len` fields,
the `resize` helper) provides a starting point, but the class must be converted to a template and
all methods must be renamed, added, or corrected.

## User Stories

1. As a developer, I want a default constructor `Vector()`, so that I can create an empty vector with no arguments.
2. As a developer, I want a constructor `Vector(unsigned size)`, so that I can pre-allocate memory for `size` elements without adding any elements.
3. As a developer, I want `T at(unsigned index)` to return the element at `index`, so that I can safely access elements with bounds checking.
4. As a developer, I want `at()` to throw `std::out_of_range` when `index >= size()`, so that out-of-bounds access is caught at runtime.
5. As a developer, I want `T operator[](unsigned index)` to return the element at `index` without bounds checking, so that I can access elements with minimal overhead when I know the index is valid.
6. As a developer, I want `bool isEmpty()` to return `true` when the vector has no elements, so that I can branch on emptiness cleanly.
7. As a developer, I want `unsigned size()` to return the number of stored elements, so that I can iterate and reason about the vector's contents.
8. As a developer, I want `unsigned capacity()` to return the number of elements for which memory has been reserved, so that I can understand the vector's memory footprint.
9. As a developer, I want `void reserve(unsigned size)` to pre-allocate memory for at least `size` elements without changing the element count, so that I can avoid repeated reallocations during bulk insertions.
10. As a developer, I want `reserve()` to do nothing if the current capacity already satisfies the request, so that unnecessary copies are avoided.
11. As a developer, I want `void resize(unsigned size)` to shrink the element count when `size < current size`, so that I can truncate the vector.
12. As a developer, I want `void resize(unsigned size)` to grow the element count with default-constructed elements when `size > current size`, so that the vector expands predictably.
13. As a developer, I want `void resize(unsigned size, T elem)` to fill new positions with `elem` when growing, so that I can initialize expanded slots to a known value.
14. As a developer, I want `resize()` to increase capacity as needed when growing, so that the underlying allocation is sufficient.
15. As a developer, I want `void clear()` to set the element count to 0 without releasing or reducing capacity, so that I can reuse the allocation.
16. As a developer, I want `void insert(unsigned index, T elem)` to insert `elem` at position `index`, shifting subsequent elements right, so that I can insert into the middle of the vector.
17. As a developer, I want `insert()` to throw `std::out_of_range` when `index > size()`, so that inserting at an invalid position is caught at runtime.
18. As a developer, I want `insert()` to grow capacity automatically if needed, so that it never silently fails due to insufficient space.
19. As a developer, I want `void erase(unsigned index)` to remove the element at `index` and shift subsequent elements left, so that I can delete individual elements.
20. As a developer, I want `erase(unsigned index)` to throw `std::out_of_range` when `index >= size()`, so that erasing at an invalid position is caught.
21. As a developer, I want `void erase(unsigned index, unsigned amount)` to remove `amount` consecutive elements starting at `index`, so that I can delete a range efficiently.
22. As a developer, I want `erase(unsigned index, unsigned amount)` to throw `std::out_of_range` when the range `[index, index+amount)` exceeds the valid range, so that partial out-of-bounds erases are caught.
23. As a developer, I want `void pushBack(T elem)` to append `elem` to the end, growing capacity automatically if needed, so that I can build the vector incrementally.
24. As a developer, I want `void popBack()` to remove the last element and decrease `size()` by 1, while leaving capacity unchanged, so that I can trim the tail without reallocating.
25. As a developer, I want the vector to work with any copyable type `T` (e.g., `int`, `double`, `std::string`), so that I am not restricted to a single element type.
26. As a developer, I want automatic capacity growth (e.g., doubling) when inserting beyond current capacity, so that amortized insertion is O(1).

## Implementation Decisions

### Class template

- `Vector` becomes `template <typename T> class Vector`.
- Internal storage is `T *ar` (raw owning pointer).
- Internal fields: `unsigned cap` (capacity), `unsigned len` (size).
- Growth strategy: double current capacity (with a floor at `INITIAL_CAP = 16`) when more space is needed — same as the existing `GROWTH_FACTOR = 2` constant.

### Constructors

- `Vector()`: allocates `INITIAL_CAP` elements, sets `len = 0`.
- `Vector(unsigned size)`: allocates exactly `size` elements (or `INITIAL_CAP` if `size == 0`), sets `len = 0`.

### Memory management

- The class must have a destructor that calls `delete[] ar`.
- Internal reallocation helper (currently `resize` in old code) is replaced by a private `grow(unsigned newCap)` method to avoid name collision with the public `resize`.
- `grow()` allocates a new buffer, moves/copies elements, deletes the old buffer, and updates `cap`.
- `reserve(unsigned size)` calls `grow(size)` only when `size > cap`.

### Method contracts

| Method | Throws | Notes |
|---|---|---|
| `at(unsigned index)` | `std::out_of_range` if `index >= len` | — |
| `operator[](unsigned index)` | never | UB if out of range |
| `insert(unsigned index, T elem)` | `std::out_of_range` if `index > len` | index == len is valid (append) |
| `erase(unsigned index)` | `std::out_of_range` if `index >= len` | — |
| `erase(unsigned index, unsigned amount)` | `std::out_of_range` if `index + amount > len` | — |
| `popBack()` | undefined / no-op if empty | match `std::vector` behavior |

### File layout

- All code stays in `Vector.h++` (header-only template).
- `main.c++` is updated to exercise the new API.

## Testing Decisions

A good test:
- Exercises the **public API only** — never inspects private fields directly.
- Tests the **observable state** after an operation (return values, thrown exceptions, subsequent `size()`/`capacity()`/`at()` calls).
- Tests **boundary conditions**: empty vector, single element, index = 0, index = last, index = one-past-last (insert only).

### Modules to test

**`Vector<T>` (the only module)**

Scenarios to cover:
- Default constructor: `size() == 0`, `isEmpty() == true`, `capacity() >= 0`.
- Size constructor: `size() == 0`, `capacity() == requested`.
- `pushBack` / `size` / `at` / `operator[]`: basic round-trip.
- `pushBack` past initial capacity triggers growth; capacity increases, all existing elements preserved.
- `reserve`: capacity increases when requested > current; no-op when requested ≤ current; size unchanged.
- `resize(n)` shrink: size decreases, capacity unchanged.
- `resize(n)` grow with default: new slots accessible via `at()`.
- `resize(n, elem)` grow with fill value: new slots equal `elem`.
- `clear`: size = 0, capacity unchanged.
- `insert` at head, middle, tail: elements shift correctly.
- `insert` out of range: throws `std::out_of_range`.
- `erase` single: elements shift correctly, size decreases.
- `erase` single out of range: throws.
- `erase` range: correct elements removed.
- `erase` range out of range: throws.
- `popBack`: size decreases, capacity unchanged.
- `at` out of range: throws `std::out_of_range`.
- Works with `double` and `std::string` (template instantiation check).

## Out of Scope

- Thread safety.

## Further Notes

- The existing `delete ar` in `Vector.h++:56` is a bug — it must be `delete[] ar` for array allocations.
- `std::move` used on raw pointers performs element-wise moves, which is correct for trivially movable types but should be replaced with `std::copy` or a manual loop when supporting non-trivial types (e.g., `std::string`). The safest drop-in is `std::copy`.
- The `test()` method in the existing code should be removed; it has no place in the final implementation.
- `unsigned` indices mean callers cannot pass negative values — no negative-index checks are needed.
