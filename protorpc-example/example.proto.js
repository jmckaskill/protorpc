(function(proto){
	var com_example_TestEnum = {
		[0]: "ENUM_A",
		[1]: "ENUM_B",
		[2]: "ENUM_C",
	};
	var com_example_Empty = {
	};
	var com_example_EchoRequest = {
		ping: [10,14],
	};
	var com_example_TestMessage = {
		b: [8,0],
		i32: [16,2],
		s32: [24,3],
		sf32: [37,9],
		u32: [40,1],
		f32: [53,8],
		i64: [56,6],
		sf64: [65,11],
		s64: [72,7],
		u64: [80,5],
		f64: [89,10],
		f: [101,12],
		d: [105,13],
		by: [114,15],
		str: [122,14],
		en: [128,36,com_example_TestEnum],
		msg: [138,47,0],
		pod: [146,47,0],
		rb: [170,16],
		ri32: [178,18],
		rs32: [186,19],
		rsf32: [194,25],
		ru32: [202,17],
		rf32: [210,24],
		ri64: [218,22],
		rsf64: [226,27],
		rs64: [234,23],
		ru64: [1682,21],
		rf64: [1690,26],
		rf: [1698,28],
		rd: [1706,29],
		rby: [1714,31],
		rstr: [1722,30],
		ren: [1730,52,com_example_TestEnum],
		rmsg: [1738,63,0],
		rpod: [1746,63,0],
		mb: [1770,63,0],
		mi32: [1778,63,0],
		ms32: [1786,63,0],
		msf32: [1794,63,0],
		mu32: [1802,63,0],
		mf32: [1810,63,0],
		mi64: [1818,63,0],
		msf64: [1826,63,0],
		ms64: [1834,63,0],
		mu64: [17682,63,0],
		mf64: [17690,63,0],
		mf: [17698,63,0],
		md: [17706,63,0],
		mby: [17714,63,0],
		mstr: [17722,63,0],
		men: [17730,63,0],
		mmsg: [17738,63,0],
		mpod: [17746,63,0],
	};
	var com_example_TestMessage_MbEntry = {
		key: [8,1],
		value: [16,0],
	};
	var com_example_TestMessage_Mu32Entry = {
		key: [8,1],
		value: [16,1],
	};
	var com_example_TestMessage_Mu64Entry = {
		key: [8,1],
		value: [16,5],
	};
	var com_example_TestMessage_Mi32Entry = {
		key: [8,1],
		value: [16,2],
	};
	var com_example_TestMessage_Mi64Entry = {
		key: [8,1],
		value: [16,6],
	};
	var com_example_TestMessage_Ms32Entry = {
		key: [8,1],
		value: [16,3],
	};
	var com_example_TestMessage_Ms64Entry = {
		key: [8,1],
		value: [16,7],
	};
	var com_example_TestMessage_Mf32Entry = {
		key: [8,1],
		value: [21,8],
	};
	var com_example_TestMessage_Mf64Entry = {
		key: [8,1],
		value: [17,10],
	};
	var com_example_TestMessage_Msf32Entry = {
		key: [8,1],
		value: [21,9],
	};
	var com_example_TestMessage_Msf64Entry = {
		key: [8,1],
		value: [17,11],
	};
	var com_example_TestMessage_MfEntry = {
		key: [8,1],
		value: [21,12],
	};
	var com_example_TestMessage_MdEntry = {
		key: [8,1],
		value: [17,13],
	};
	var com_example_TestMessage_MbyEntry = {
		key: [8,1],
		value: [18,15],
	};
	var com_example_TestMessage_MstrEntry = {
		key: [8,1],
		value: [18,14],
	};
	var com_example_TestMessage_MenEntry = {
		key: [8,1],
		value: [16,36,com_example_TestEnum],
	};
	var com_example_TestMessage_MmsgEntry = {
		key: [8,1],
		value: [18,47,0],
	};
	var com_example_TestMessage_MpodEntry = {
		key: [8,1],
		value: [18,47,0],
	};
	var com_example_TestPod = {
		u: [8,1],
		i: [16,3],
	};
	com_example_TestMessage.msg[2] = com_example_TestMessage;
	com_example_TestMessage.pod[2] = com_example_TestPod;
	com_example_TestMessage.rmsg[2] = com_example_TestMessage;
	com_example_TestMessage.rpod[2] = com_example_TestPod;
	com_example_TestMessage.mb[2] = com_example_TestMessage_MbEntry;
	com_example_TestMessage.mu32[2] = com_example_TestMessage_Mu32Entry;
	com_example_TestMessage.mu64[2] = com_example_TestMessage_Mu64Entry;
	com_example_TestMessage.mi32[2] = com_example_TestMessage_Mi32Entry;
	com_example_TestMessage.mi64[2] = com_example_TestMessage_Mi64Entry;
	com_example_TestMessage.ms32[2] = com_example_TestMessage_Ms32Entry;
	com_example_TestMessage.ms64[2] = com_example_TestMessage_Ms64Entry;
	com_example_TestMessage.mf32[2] = com_example_TestMessage_Mf32Entry;
	com_example_TestMessage.mf64[2] = com_example_TestMessage_Mf64Entry;
	com_example_TestMessage.msf32[2] = com_example_TestMessage_Msf32Entry;
	com_example_TestMessage.msf64[2] = com_example_TestMessage_Msf64Entry;
	com_example_TestMessage.mf[2] = com_example_TestMessage_MfEntry;
	com_example_TestMessage.md[2] = com_example_TestMessage_MdEntry;
	com_example_TestMessage.mby[2] = com_example_TestMessage_MbyEntry;
	com_example_TestMessage.mstr[2] = com_example_TestMessage_MstrEntry;
	com_example_TestMessage.men[2] = com_example_TestMessage_MenEntry;
	com_example_TestMessage.mmsg[2] = com_example_TestMessage_MmsgEntry;
	com_example_TestMessage.mpod[2] = com_example_TestMessage_MpodEntry;
	com_example_TestMessage_MmsgEntry.value[2] = com_example_TestMessage;
	com_example_TestMessage_MpodEntry.value[2] = com_example_TestPod;
	proto.com_example_Empty = com_example_Empty;
	proto.com_example_EchoRequest = com_example_EchoRequest;
	proto.com_example_TestMessage = com_example_TestMessage;
	proto.com_example_TestPod = com_example_TestPod;
	proto.com_example_Example = {
		_basePath: "/twirp/com.example.Example/",
		Echo: ["Echo", com_example_EchoRequest, com_example_EchoRequest],
		GenerateError: ["GenerateError", com_example_Empty, com_example_Empty],
	};
	proto.com_example_Example_EchoStream = [
		"/twirp/com.example.Example/EchoStream",
		com_example_EchoRequest,
		com_example_EchoRequest
	];
	proto.com_example_TestService = {
		_basePath: "/twirp/com.example.TestService/",
		rpc2: ["rpc2", com_example_TestPod, com_example_TestMessage],
		rpc1: ["rpc1", com_example_TestMessage, com_example_TestPod],
	};
})(proto);

