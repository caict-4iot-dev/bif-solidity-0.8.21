function f() pure {
    assembly ("a", "b", "c", "c") {}
}
// ----
// Warning 4430: (24-56): Unknown inline assembly flag: "a"
// Warning 4430: (24-56): Unknown inline assembly flag: "b"
// Warning 4430: (24-56): Unknown inline assembly flag: "c"
// Warning 4430: (24-56): Unknown inline assembly flag: "c"
