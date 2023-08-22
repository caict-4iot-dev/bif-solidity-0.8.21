abstract contract S {
    function longdata() internal virtual pure returns (bytes memory);
}

abstract contract X is S {
    function longdata() internal override pure returns (bytes memory) {
        return
            "xasopca.pngaibngidak.jbtnudak.cAP.BRRSMCPJAGPD KIAJDOMHUKR,SCPID"
            "xasopca.pngaibngidak.jbtnudak.cAP.BRRSMCPJAGPD KIAJDOMHUKR,SCPID"
            "M,SEYBDXCNTKIMNJGO;DUIAQBQUEHAKMPGIDSAJCOUKANJBCUEBKNA.GIAKMV.TI"
            "AJMO<KXBANJCPGUD ABKCJIDHA NKIMAJU,EKAMHSO;PYCAKUM,L.UCA MR;KITA"
            "M,SEYBDXCNTKIMNJGO;DUIAQBQUEHAKMPGIDSAJCOUKANJBCUEBKNA.GIAKMV.TI"
            "AJMO<KXBANJCPGUD ABKCJIDHA NKIMAJU,EKAMHSO;PYCAKUM,L.UCA MR;KITA"
            " .RPOKIDAS,.CKUMT.,ORKAD ,NOKIDHA .CGKIAD OVHAMS CUAOGT DAKN OIT"
            "xasopca.pngaibngidak.jbtnudak.cAP.BRRSMCPJAGPD KIAJDOMHUKR,SCPID"
            "M,SEYBDXCNTKIMNJGO;DUIAQBQUEHAKMPGIDSAJCOUKANJBCUEBKNA.GIAKMV.TI"
            "AJMO<KXBANJCPGUD ABKCJIDHA NKIMAJU,EKAMHSO;PYCAKUM,L.UCA MR;KITA"
            "apibakrpidbacnidkacjadtnpdkylca.,jda,r.kuadc,jdlkjd',c'dj, ncg d"
            "anosumantkudkc,djntudkantuadnc,ui,c.ud,.nujdncud,j.rsch'pkl.'pih";
        }
}

contract C is X {
    bytes data;
    function () pure returns (bytes memory) f;
    constructor() {
        data = super.longdata();
        f = super.longdata;
    }
    function test() public view returns (bool) {
        return keccak256(data) == keccak256(f());
    }
}
// ----
// test() -> true
