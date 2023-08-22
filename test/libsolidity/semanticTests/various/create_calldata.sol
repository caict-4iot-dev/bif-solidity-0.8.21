contract C {
	bytes public s;
	constructor(uint256 x) {
		// Due to a bug in EVMHost, msg.data used to contain initcode and constructor arguments.
		s = msg.data;
		assert(msg.data.length == 0);
	}
}
// ----
// constructor(): 42 ->
// gas irOptimized: 146699
// gas legacy: 173845
// gas legacyOptimized: 137736
// s() -> 0x20, 0
