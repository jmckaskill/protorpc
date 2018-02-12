(function(proto){
	var com_example_TestEnum = {
		[0]: "ENUM_A",
		[1]: "ENUM_B",
		[2]: "ENUM_C",
	};
	var com_example_Empty = [
	];
	var com_example_EchoRequest = [
		10,14,"ping",
	];
	var com_example_TestMessage = [
		8,0,"b",
		16,2,"i32",
		24,3,"s32",
		37,9,"sf32",
		40,1,"u32",
		53,8,"f32",
		56,6,"i64",
		65,11,"sf64",
		72,7,"s64",
		80,5,"u64",
		89,10,"f64",
		101,12,"f",
		105,13,"d",
		114,15,"by",
		122,14,"str",
		128,36,"en",com_example_TestEnum,
		138,47,"msg",null,
		146,47,"pod",null,
		170,16,"rb",
		178,18,"ri32",
		186,19,"rs32",
		194,25,"rsf32",
		202,17,"ru32",
		210,24,"rf32",
		218,22,"ri64",
		226,27,"rsf64",
		234,23,"rs64",
		1682,21,"ru64",
		1690,26,"rf64",
		1698,28,"rf",
		1706,29,"rd",
		1714,31,"rby",
		1722,30,"rstr",
		1730,52,"ren",com_example_TestEnum,
		1738,63,"rmsg",null,
		1746,63,"rpod",null,
		1770,63,"mb",null,
		1778,63,"mi32",null,
		1786,63,"ms32",null,
		1794,63,"msf32",null,
		1802,63,"mu32",null,
		1810,63,"mf32",null,
		1818,63,"mi64",null,
		1826,63,"msf64",null,
		1834,63,"ms64",null,
		17682,63,"mu64",null,
		17690,63,"mf64",null,
		17698,63,"mf",null,
		17706,63,"md",null,
		17714,63,"mby",null,
		17722,63,"mstr",null,
		17730,63,"men",null,
		17738,63,"mmsg",null,
		17746,63,"mpod",null,
	];
	var com_example_TestMessage_MbEntry = [
		8,1,"key",
		16,0,"value",
	];
	var com_example_TestMessage_Mu32Entry = [
		8,1,"key",
		16,1,"value",
	];
	var com_example_TestMessage_Mu64Entry = [
		8,1,"key",
		16,5,"value",
	];
	var com_example_TestMessage_Mi32Entry = [
		8,1,"key",
		16,2,"value",
	];
	var com_example_TestMessage_Mi64Entry = [
		8,1,"key",
		16,6,"value",
	];
	var com_example_TestMessage_Ms32Entry = [
		8,1,"key",
		16,3,"value",
	];
	var com_example_TestMessage_Ms64Entry = [
		8,1,"key",
		16,7,"value",
	];
	var com_example_TestMessage_Mf32Entry = [
		8,1,"key",
		21,8,"value",
	];
	var com_example_TestMessage_Mf64Entry = [
		8,1,"key",
		17,10,"value",
	];
	var com_example_TestMessage_Msf32Entry = [
		8,1,"key",
		21,9,"value",
	];
	var com_example_TestMessage_Msf64Entry = [
		8,1,"key",
		17,11,"value",
	];
	var com_example_TestMessage_MfEntry = [
		8,1,"key",
		21,12,"value",
	];
	var com_example_TestMessage_MdEntry = [
		8,1,"key",
		17,13,"value",
	];
	var com_example_TestMessage_MbyEntry = [
		8,1,"key",
		18,15,"value",
	];
	var com_example_TestMessage_MstrEntry = [
		8,1,"key",
		18,14,"value",
	];
	var com_example_TestMessage_MenEntry = [
		8,1,"key",
		16,36,"value",com_example_TestEnum,
	];
	var com_example_TestMessage_MmsgEntry = [
		8,1,"key",
		18,47,"value",null,
	];
	var com_example_TestMessage_MpodEntry = [
		8,1,"key",
		18,47,"value",null,
	];
	var com_example_TestPod = [
		8,1,"u",
		16,3,"i",
	];
	com_example_TestMessage[52] = com_example_TestMessage;
	com_example_TestMessage[56] = com_example_TestPod;
	com_example_TestMessage[109] = com_example_TestMessage;
	com_example_TestMessage[113] = com_example_TestPod;
	com_example_TestMessage[117] = com_example_TestMessage_MbEntry;
	com_example_TestMessage[121] = com_example_TestMessage_Mi32Entry;
	com_example_TestMessage[125] = com_example_TestMessage_Ms32Entry;
	com_example_TestMessage[129] = com_example_TestMessage_Msf32Entry;
	com_example_TestMessage[133] = com_example_TestMessage_Mu32Entry;
	com_example_TestMessage[137] = com_example_TestMessage_Mf32Entry;
	com_example_TestMessage[141] = com_example_TestMessage_Mi64Entry;
	com_example_TestMessage[145] = com_example_TestMessage_Msf64Entry;
	com_example_TestMessage[149] = com_example_TestMessage_Ms64Entry;
	com_example_TestMessage[153] = com_example_TestMessage_Mu64Entry;
	com_example_TestMessage[157] = com_example_TestMessage_Mf64Entry;
	com_example_TestMessage[161] = com_example_TestMessage_MfEntry;
	com_example_TestMessage[165] = com_example_TestMessage_MdEntry;
	com_example_TestMessage[169] = com_example_TestMessage_MbyEntry;
	com_example_TestMessage[173] = com_example_TestMessage_MstrEntry;
	com_example_TestMessage[177] = com_example_TestMessage_MenEntry;
	com_example_TestMessage[181] = com_example_TestMessage_MmsgEntry;
	com_example_TestMessage[185] = com_example_TestMessage_MpodEntry;
	com_example_TestMessage_MmsgEntry[6] = com_example_TestMessage;
	com_example_TestMessage_MpodEntry[6] = com_example_TestPod;
	var com_example_Example = [
		"ping",com_example_EchoRequest,com_example_EchoRequest,
		"error",com_example_Empty,com_example_Empty,
	];
	var com_example_TestService = [
		"rpc2",com_example_TestPod,com_example_TestMessage,
		"rpc1",com_example_TestMessage,com_example_TestPod,
	];
	proto.register("com.example.", {
		Empty: com_example_Empty,
		EchoRequest: com_example_EchoRequest,
	},{
		Example: com_example_Example,
	});
	proto.register("com.example.", {
		TestMessage: com_example_TestMessage,
		TestPod: com_example_TestPod,
	},{
		TestService: com_example_TestService,
	});
})(proto);

