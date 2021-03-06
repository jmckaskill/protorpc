(function (proto) {
    var test_proto = new Uint8Array([
        // b = true
        0x08, 1,

        // i32 = -23 = 0xFFFFFFE9 = xF,x7F,x7F,x7F,x69
        0x10, 0xE9, 0xFF, 0xFF, 0xFF, 0xF,

        // s32 = -1234 = 2467 = 0x9A3 = 0x13,0x23
        0x18, 0xA3, 0x13,

        // sf32 = -34757 = 0xFFFF783B
        0x25, 0x3B, 0x78, 0xFF, 0xFF,

        // u32 = 1
        0x28, 1,

        // f32 = 34757 = 0x000087C5
        0x35, 0xC5, 0x87, 0, 0,

        // i64 = -3434565678781212898 = 0xD055F8B6567F571E = 1E 2E 7D 33 65 16 7E 2A 50 01
        0x38, 0x9E, 0xAE, 0xFD, 0xB3, 0xE5, 0x96, 0xFE, 0xAA, 0xD0, 0x01,
        //34
        // sf64 = -1234567890123456789 = 0x EEDD EF0B 8216 7EEB
        0x41, 0xEB, 0x7E, 0x16, 0x82, 0x0B, 0xEF, 0xDD, 0xEE,

        // s64 = -23585 = 47169 = xB841 = x02,x70,x41
        0x48, 0xC1, 0xF0, 0x02,

        // u64 = 10234 = 0x27FA = 0x4F,0x7A
        0x50, 0xFA, 0x4F,
        //50
        // f64 = 1234567890123456789 = 0x 1122 10F4 7DE9 8115
        0x59, 0x15, 0x81, 0xE9, 0x7D, 0xF4, 0x10, 0x22, 0x11,

        // f = 314 = 0x439d0000
        0x65, 0, 0, 0x9D, 0x43,
        // 64

        // d = 3.141 = 0x4009 20C4 9BA5 E354
        0x69, 0x54, 0xE3, 0xA5, 0x9B, 0xC4, 0x20, 0x09, 0x40,
        //73
        // by = "abcde" = x61 x62 x63 x64 x65
        0x72, 5, 0x61, 0x62, 0x63, 0x64, 0x65,

        // str = "abc$¢€𝌆" = x61 x62 x63 x24 xC2A2 xE282AC xF09D8C86
        0x7A, 13, 0x61, 0x62, 0x63, 0x24, 0xC2, 0xA2, 0xE2, 0x82, 0xAC, 0xF0, 0x9D, 0x8C, 0x86,
        //95
        // en = ENUM_C = 2
        0x80, 1, 2,

        // msg = {b = true}
        0x8A, 1, 2, 0x8, 1,
        //103
        // pod = {i = -12}, -12 zigzag = 23
        0x92, 1, 2, 0x10, 23,
        //108
        // rb = [false,true,false]
        0xAA, 1, 3, 0, 1, 0,
        //114
        // ri32 = [-1,0,1] = [0xFFFFFFFF,0,1] = [xF x7F x7F x7F x7F, 0, 1]
        0xB2, 1, 7, 0xFF, 0xFF, 0xFF, 0xFF, 0x0F, 0, 1,
        //124
        // rs32 = [-10,0,10] = [19,0,20]
        0xBA, 1, 3, 19, 0, 20,
        //130
        // rsf32 = [-10,20,0] = [0xFFFFFFF6,20,0]
        0xC2, 1, 12, 0xF6, 0xFF, 0xFF, 0xFF, 20, 0, 0, 0, 0, 0, 0, 0,
        //145
        // ru32 = [1,2,3]
        0xCA, 1, 3, 1, 2, 3,
        //151
        // rf32 = [10,20,30]
        0xD2, 1, 12, 10, 0, 0, 0, 20, 0, 0, 0, 30, 0, 0, 0,

        // ri64 = [-2,0,2] = [0xFFFFFFFFFFFFFFFE,0,2] = [x01 x7F x7F x7F x7F x7F x7F x7F x7F x7E, 0, 2]
        0xDA, 1, 12, 0xFE, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0x01, 0, 2,

        // rsf64 = [-100, 0, 100] = [0xFFFFFFFFFFFFFF9C, 0, 100]
        0xE2, 1, 24, 0x9C, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0, 0, 0, 0, 0, 0, 0, 0, 100, 0, 0, 0, 0, 0, 0, 0,

        // rs64 = [-20,0,20] = [39,0,40]
        0xEA, 1, 3, 39, 0, 40,

        // ru64 = [3,4,5]
        0x92, 0xD, 3, 3, 4, 5,

        // rf64 = [30,40,50]
        0x9A, 0xD, 24, 30, 0, 0, 0, 0, 0, 0, 0, 40, 0, 0, 0, 0, 0, 0, 0, 50, 0, 0, 0, 0, 0, 0, 0,

        // rf = [3.5] = [0x40600000]
        0xA2, 0xD, 4, 0, 0, 0x60, 0x40,

        // rd = [1.1,2.2,3.3] = [0x3FF1 9999 9999 999A, 0x4001 9999 9999 999A, 0x400A 6666 6666 6666]
        0xAA, 0xD, 24,
        0x9A, 0x99, 0x99, 0x99, 0x99, 0x99, 0xF1, 0x3F,
        0x9A, 0x99, 0x99, 0x99, 0x99, 0x99, 0x01, 0x40,
        0x66, 0x66, 0x66, 0x66, 0x66, 0x66, 0x0A, 0x40,

        // rby = ["defgh", "abcde"] = [x64 x65 x66 x67 x68, x61 x62 x63 x64 x65]
        0xB2, 0xD, 5, 0x64, 0x65, 0x66, 0x67, 0x68,
        0xB2, 0xD, 5, 0x61, 0x62, 0x63, 0x64, 0x65,

        // rstr = ["ghikj","lmnop"] = [x67 x68 x69 x6b x6a, x6c x6d x6e x6f x70]
        0xBA, 0xD, 5, 0x67, 0x68, 0x69, 0x6B, 0x6A,
        0xBA, 0xD, 5, 0x6C, 0x6D, 0x6E, 0x6F, 0x70,

        // ren = [ENUM_C,ENUM_B,ENUM_A] = [2,1,0]
        0xC2, 0xD, 3, 2, 1, 0,

        // rmsg = [{b = true},{u64 = 10234},{}]; 10234 = 0x27FA = 0x4F,0x7A
        0xCA, 0xD, 2, 0x8, 1,
        0xCA, 0xD, 3, 0x50, 0xFA, 0x4F,
        0xCA, 0xD, 0,

        // rpod = [{u = 1},{i = -1}]
        0xD2, 0xD, 2, 0x8, 1,
        0xD2, 0xD, 2, 0x10, 1,
    ]).buffer;

    function expect(a, b) {
        console.assert(a == b, a, b);
    }

    function isEmpty(a) {
        for (var prop in a) {
            return false;
        }
        return true;
    }

    function test_message(m) {
        expect(true, m.b);
        expect(-23, m.i32);
        expect(-1234, m.s32);
        expect(-34757, m.sf32);
        expect(1, m.u32);
        expect(34757, m.f32);
        expect("-3434565678781212898", m.i64);
        expect("-1234567890123456789", m.sf64);
        expect("-23585", m.s64);
        expect("10234", m.u64);
        expect("1234567890123456789", m.f64);
        expect(314, m.f);
        expect(3.141, m.d);
        expect(5, m.by.byteLength);
        expect("abcde", proto.utf8to16(new DataView(m.by), 0, 5));
        expect("abc$¢€𝌆", m.str);
        expect(3, m.rb.length);
        expect(false, m.rb[0]);
        expect(true, m.rb[1]);
        expect(false, m.rb[2]);
        expect(3, m.ru32.length);
        expect(1, m.ru32[0]);
        expect(2, m.ru32[1]);
        expect(3, m.ru32[2]);
        expect(3, m.ru64.length);
        expect(3, m.ru64[0]);
        expect(4, m.ru64[1]);
        expect(5, m.ru64[2]);
        expect(3, m.ri32.length);
        expect(-1, m.ri32[0]);
        expect(0, m.ri32[1]);
        expect(1, m.ri32[2]);
        expect(3, m.ri64.length);
        expect(-2, m.ri64[0]);
        expect(0, m.ri64[1]);
        expect(2, m.ri64[2]);
        expect(3, m.rs32.length);
        expect(-10, m.rs32[0]);
        expect(0, m.rs32[1]);
        expect(10, m.rs32[2]);
        expect(3, m.rs64.length);
        expect(-20, m.rs64[0]);
        expect(0, m.rs64[1]);
        expect(20, m.rs64[2]);
        expect(3, m.rf32.length);
        expect(10, m.rf32[0]);
        expect(20, m.rf32[1]);
        expect(30, m.rf32[2]);
        expect(3, m.rf64.length);
        expect(30, m.rf64[0]);
        expect(40, m.rf64[1]);
        expect(50, m.rf64[2]);
        expect(3, m.rsf32.length);
        expect(-10, m.rsf32[0]);
        expect(20, m.rsf32[1]);
        expect(0, m.rsf32[2]);
        expect(3, m.rsf64.length);
        expect(-100, m.rsf64[0]);
        expect(0, m.rsf64[1]);
        expect(100, m.rsf64[2]);
        expect(1, m.rf.length);
        expect(3.5, m.rf[0]);
        expect(3, m.rd.length);
        expect(1.1, m.rd[0]);
        expect(2.2, m.rd[1]);
        expect(3.3, m.rd[2]);
        expect(2, m.rby.length);
        expect(5, m.rby[0].byteLength);
        expect("defgh", proto.utf8to16(new DataView(m.rby[0]), 0, 5));
        expect(5, m.rby[1].byteLength);
        expect("abcde", proto.utf8to16(new DataView(m.rby[1]), 0, 5));
        expect(2, m.rstr.length);
        expect("ghikj", m.rstr[0]);
        expect("lmnop", m.rstr[1]);
        expect(3, m.ren.length);
        expect("ENUM_C", m.ren[0]);
        expect("ENUM_B", m.ren[1]);
        expect("ENUM_A", m.ren[2]);
        expect(true, m.msg.b);
        expect(-12, m.pod.i);
        expect(3, m.rmsg.length);
        expect(true, m.rmsg[0].b);
        expect(10234, m.rmsg[1].u64);
        expect(true, isEmpty(m.rmsg[2]));
        expect(2, m.rpod.length);
        expect(1, m.rpod[0].u);
        expect(-1, m.rpod[1].i);
    }

    var decoded = proto.decode(proto.com_example_TestMessage, test_proto);
    console.log("test decoder", decoded);
    test_message(decoded);
    console.log("test decoder - finished");

    var encoded = proto.encode(proto.com_example_TestMessage, decoded);
    console.log("test encoder", encoded);
    expect(encoded.byteLength, test_proto.byteLength);
    for (var i = 0; i < encoded.byteLength; i++) {
        //console.log("check byte ", i);
        expect(new DataView(encoded).getUint8(i), new DataView(test_proto).getUint8(i));
    }
    console.log("test encoder - finished");

    var host = window.location.origin;
    var svc = proto.new_client(host, proto.com_example_Example);

    svc.Echo({
        ping: "hello world",
    }).then(resp => {
        console.log("response", resp)
    }).catch(err => {
        console.log("error", err);
    });

    svc.GenerateError({}).catch(err => {
        console.log("expected error", err);
    });

    var stream = proto.new_stream(host, proto.com_example_Example_EchoStream);
    stream.send({ping: "hello world"});
    stream.onmessage = (msg) => {
        console.log(msg);
    };
    stream.onclose = (msg) => {
        console.log(msg);
    };
    setInterval(() => stream.send({ping: "123"}), 1000);
})(proto);
