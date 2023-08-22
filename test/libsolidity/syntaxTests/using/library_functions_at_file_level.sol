library L {
    function externalFunction(uint) external pure {}
    function publicFunction(uint) public pure {}
    function internalFunction(uint) internal pure {}
}

using {L.externalFunction} for uint;
using {L.publicFunction} for uint;
using {L.internalFunction} for uint;

contract C {
    function f() public pure {
        uint x;
        x.externalFunction();
        x.publicFunction();
        x.internalFunction();
    }
}
// ----
