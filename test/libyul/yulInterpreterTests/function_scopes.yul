{
    f(1)
    function f(i) {
        if i { g(1) }
        function g(j) {
            if j { h() }
            f(0)
            function h() {
                g(0)
            }
        }
        sstore(i, add(i, 7))
    }
}
// ----
// Trace:
// Memory dump:
// Storage dump:
//   0000000000000000000000000000000000000000000000000000000000000000: 0000000000000000000000000000000000000000000000000000000000000007
//   0000000000000000000000000000000000000000000000000000000000000001: 0000000000000000000000000000000000000000000000000000000000000008
