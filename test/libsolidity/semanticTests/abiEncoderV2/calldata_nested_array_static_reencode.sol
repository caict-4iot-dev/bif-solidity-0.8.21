pragma abicoder v2;

contract C {
	function f(uint[3][] calldata a) public {
		abi.encode(a);
	}
	function f(uint[][3] calldata a) public {
		abi.encode(a);
	}
	function f(uint[2][2] calldata a) public {
		abi.encode(a);
	}
}
// ----
// f(uint256[3][]): 0x20, 1, 0x01 -> FAILURE
// f(uint256[3][]): 0x20, 1, 0x01, 0x02 -> FAILURE
// f(uint256[3][]): 0x20, 1, 0x01, 0x02, 0x03 ->
// f(uint256[][3]): 0x20, 0x60, 0x60, 0x60, 3, 0x01 -> FAILURE
// f(uint256[][3]): 0x20, 0x60, 0x60, 0x60, 3, 0x01, 0x02 -> FAILURE
// f(uint256[][3]): 0x20, 0x60, 0x60, 0x60, 3, 0x01, 0x02, 0x03 ->
// f(uint256[2][2]): 0x01 -> FAILURE
// f(uint256[2][2]): 0x01, 0x02 -> FAILURE
// f(uint256[2][2]): 0x01, 0x02, 0x03 -> FAILURE
// f(uint256[2][2]): 0x01, 0x02, 0x03, 0x04 ->
// f(uint256[2][2]): 0x01, 0x02, 0x03, 0x04, 0x05 ->
