# automatic-nested — Automatic Recursive DEBUG Fallback

Nested/user types auto-recurse: no silent `<<` / `<unformattable>` fallback
for printable aggregates.

## Dispatch (§7 `debug_print_value`, precedence order)

1. Scalars, strings, optionals, pointers, pairs/tuples, containers, enums — as before.
2. `IsDebugReflectable` (explicit `DEBUG_FIELDS` / `DEBUG_STRUCT` / `DEBUG_AUTO`) wins.
3. `IsAutoReflectable` (bare public aggregate) recurses generically — single path,
   no per-type code; reuses depth-32 / cycle-128 guards, const/ref/value preserved.
4. Streamable types use `operator<<`; anything else prints `<unformattable TypeName>`.

With-DEBUG nests print byte-identical to standalone explicit format; formatting
(`Type {`, `name: value`, 2-space indent) is unchanged.

## Compile-time notice

`debug_derive::uses_generic_auto_v<T>` is true when `T` prints through the
generic auto path. Pin or forbid it per type:

```cpp
static_assert(!debug_derive::uses_generic_auto_v<MyType>); // fails, naming MyType, if it relies on the fallback
```

// ponytail: trait instead of a per-instantiation pragma/[[deprecated]] hook —
// those either fire unconditionally or break -Wall -Wextra -pedantic builds.
// Unsupported (non-aggregate, unregistered) types keep the runtime marker
// `<unformattable TypeName>`, which likewise names the type.

## Limits (unchanged)

Aggregates only (no private members, user ctors, virtuals/bitfields, unions),
up to 64 fields. Non-aggregate with zero args prints `Type {}` — list fields
explicitly instead.

## Tests

`tests/test_auto.cpp`: `NestedPrimitiveMember`, `NestedWithExplicitDebug`,
`NestedWithoutDebug`, `ThreeLevelNesting`, `MixedPrimitiveAndUserDefined`
(fixtures in `tests/test_types.hpp`); demo section 10 in `examples/demo.cpp`.
