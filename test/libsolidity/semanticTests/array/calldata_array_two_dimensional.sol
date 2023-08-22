pragma abicoder               v2;
contract C {
    function test(uint256[][2] calldata a) external returns (uint256) {
        return a.length;
    }
    function test(uint256[][2] calldata a, uint256 i) external returns (uint256) {
        return a[i].length;
    }
    function test(uint256[][2] calldata a, uint256 i, uint256 j) external returns (uint256) {
        return a[i][j];
    }
    function reenc(uint256[][2] calldata a, uint256 i, uint256 j) external returns (uint256) {
        return this.test(a, i, j);
    }
}
// ----
// test(uint256[][2]): 0x20, 0x40, 0xC0, 3, 0x0A01, 0x0A02, 0x0A03, 4, 0x0B01, 0x0B02, 0x0B03, 0x0B04 -> 2
// test(uint256[][2],uint256): 0x40, 0, 0x40, 0xC0, 3, 0x0A01, 0x0A02, 0x0A03, 4, 0x0B01, 0x0B02, 0x0B03, 0x0B04 -> 3
// test(uint256[][2],uint256): 0x40, 1, 0x40, 0xC0, 3, 0x0A01, 0x0A02, 0x0A03, 4, 0x0B01, 0x0B02, 0x0B03, 0x0B04 -> 4
// test(uint256[][2],uint256,uint256): 0x60, 0, 0, 0x40, 0xC0, 3, 0x0A01, 0x0A02, 0x0A03, 4, 0x0B01, 0x0B02, 0x0B03, 0x0B04 -> 0x0A01
// reenc(uint256[][2],uint256,uint256): 0x60, 0, 0, 0x40, 0xC0, 3, 0x0A01, 0x0A02, 0x0A03, 4, 0x0B01, 0x0B02, 0x0B03, 0x0B04 -> 0x0A01
// test(uint256[][2],uint256,uint256): 0x60, 0, 1, 0x40, 0xC0, 3, 0x0A01, 0x0A02, 0x0A03, 4, 0x0B01, 0x0B02, 0x0B03, 0x0B04 -> 0x0A02
// reenc(uint256[][2],uint256,uint256): 0x60, 0, 1, 0x40, 0xC0, 3, 0x0A01, 0x0A02, 0x0A03, 4, 0x0B01, 0x0B02, 0x0B03, 0x0B04 -> 0x0A02
// test(uint256[][2],uint256,uint256): 0x60, 0, 2, 0x40, 0xC0, 3, 0x0A01, 0x0A02, 0x0A03, 4, 0x0B01, 0x0B02, 0x0B03, 0x0B04 -> 0x0A03
// reenc(uint256[][2],uint256,uint256): 0x60, 0, 2, 0x40, 0xC0, 3, 0x0A01, 0x0A02, 0x0A03, 4, 0x0B01, 0x0B02, 0x0B03, 0x0B04 -> 0x0A03
// test(uint256[][2],uint256,uint256): 0x60, 1, 0, 0x40, 0xC0, 3, 0x0A01, 0x0A02, 0x0A03, 4, 0x0B01, 0x0B02, 0x0B03, 0x0B04 -> 0x0B01
// reenc(uint256[][2],uint256,uint256): 0x60, 1, 0, 0x40, 0xC0, 3, 0x0A01, 0x0A02, 0x0A03, 4, 0x0B01, 0x0B02, 0x0B03, 0x0B04 -> 0x0B01
// test(uint256[][2],uint256,uint256): 0x60, 1, 1, 0x40, 0xC0, 3, 0x0A01, 0x0A02, 0x0A03, 4, 0x0B01, 0x0B02, 0x0B03, 0x0B04 -> 0x0B02
// reenc(uint256[][2],uint256,uint256): 0x60, 1, 1, 0x40, 0xC0, 3, 0x0A01, 0x0A02, 0x0A03, 4, 0x0B01, 0x0B02, 0x0B03, 0x0B04 -> 0x0B02
// test(uint256[][2],uint256,uint256): 0x60, 1, 2, 0x40, 0xC0, 3, 0x0A01, 0x0A02, 0x0A03, 4, 0x0B01, 0x0B02, 0x0B03, 0x0B04 -> 0x0B03
// reenc(uint256[][2],uint256,uint256): 0x60, 1, 2, 0x40, 0xC0, 3, 0x0A01, 0x0A02, 0x0A03, 4, 0x0B01, 0x0B02, 0x0B03, 0x0B04 -> 0x0B03
// test(uint256[][2],uint256,uint256): 0x60, 1, 3, 0x40, 0xC0, 3, 0x0A01, 0x0A02, 0x0A03, 4, 0x0B01, 0x0B02, 0x0B03, 0x0B04 -> 0x0B04
// reenc(uint256[][2],uint256,uint256): 0x60, 1, 3, 0x40, 0xC0, 3, 0x0A01, 0x0A02, 0x0A03, 4, 0x0B01, 0x0B02, 0x0B03, 0x0B04 -> 0x0B04
// test(uint256[][2],uint256,uint256): 0x60, 0, 3, 0x40, 0xC0, 3, 0x0A01, 0x0A02, 0x0A03, 4, 0x0B01, 0x0B02, 0x0B03, 0x0B04 -> FAILURE, hex"4e487b71", 0x32
// test(uint256[][2],uint256,uint256): 0x60, 1, 4, 0x40, 0xC0, 3, 0x0A01, 0x0A02, 0x0A03, 4, 0x0B01, 0x0B02, 0x0B03, 0x0B04 -> FAILURE, hex"4e487b71", 0x32
// test(uint256[][2],uint256): 0x40, 2, 0x40, 0xC0, 3, 0x0A01, 0x0A02, 0x0A03, 4, 0x0B01, 0x0B02, 0x0B03, 0x0B04 -> FAILURE, hex"4e487b71", 0x32
