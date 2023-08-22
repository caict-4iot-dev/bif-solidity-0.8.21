contract C {
	function f() public pure {
		uint x;
		uint i;
		while (i < 3) {
			++i;
			if (i > 1)
				continue;
			++x;
		}
		assert(x == 1);
	}
}
// ====
// SMTEngine: bmc
// SMTSolvers: z3
// BMCLoopIterations: 4
// ----
// Info 6002: BMC: 3 verification condition(s) proved safe! Enable the model checker option "show proved safe" to see all of them.
