#include "protorpc/src/test.proto.h"

static const struct proto_field fields_TestPod[] = {
	{{1, "u"}, PROTO_U32, offsetof(TestPod, foo), 8, NULL, offsetof(TestPod, foo_type)},
	{{1, "i"}, PROTO_S32, offsetof(TestPod, foo), 16, NULL, offsetof(TestPod, foo_type)}
};
static const pb_string *by_name_TestPod[] = {
	&fields_TestPod[1].json_name,
	&fields_TestPod[0].json_name
};
const proto_message proto_TestPod = {
	sizeof(TestPod),
	2,
	fields_TestPod,
	by_name_TestPod
};

static const proto_enum_value values_TestEnum[] = {
	{{6, "ENUM_A"}, 0},
	{{6, "ENUM_B"}, 1},
	{{6, "ENUM_C"}, 2}
};
static const pb_string *by_name_TestEnum[] = {
	{&values_TestEnum[0].name},
	{&values_TestEnum[1].name},
	{&values_TestEnum[2].name}
};
const proto_enum proto_TestEnum = {
	3,
	values_TestEnum,
	by_name_TestEnum
};

static const struct proto_field fields_TestMessage[] = {
	{{1, "b"}, PROTO_BOOL, offsetof(TestMessage, b), 8, NULL, -1},
	{{3, "i32"}, PROTO_I32, offsetof(TestMessage, i32), 16, NULL, -1},
	{{3, "s32"}, PROTO_S32, offsetof(TestMessage, s32), 24, NULL, -1},
	{{4, "sf32"}, PROTO_SF32, offsetof(TestMessage, sf32), 37, NULL, -1},
	{{3, "u32"}, PROTO_U32, offsetof(TestMessage, u32), 40, NULL, -1},
	{{3, "f32"}, PROTO_F32, offsetof(TestMessage, f32), 53, NULL, -1},
	{{3, "i64"}, PROTO_I64, offsetof(TestMessage, i64), 56, NULL, -1},
	{{4, "sf64"}, PROTO_SF64, offsetof(TestMessage, sf64), 65, NULL, -1},
	{{3, "s64"}, PROTO_S64, offsetof(TestMessage, s64), 72, NULL, -1},
	{{3, "u64"}, PROTO_U64, offsetof(TestMessage, u64), 80, NULL, -1},
	{{3, "f64"}, PROTO_F64, offsetof(TestMessage, f64), 89, NULL, -1},
	{{1, "f"}, PROTO_FLOAT, offsetof(TestMessage, f), 101, NULL, -1},
	{{1, "d"}, PROTO_DOUBLE, offsetof(TestMessage, d), 105, NULL, -1},
	{{2, "by"}, PROTO_BYTES, offsetof(TestMessage, by), 114, NULL, -1},
	{{3, "str"}, PROTO_STRING, offsetof(TestMessage, str), 122, NULL, -1},
	{{2, "en"}, PROTO_ENUM, offsetof(TestMessage, en), 128, &proto_TestEnum, -1},
	{{3, "msg"}, PROTO_MESSAGE, offsetof(TestMessage, msg), 138, &proto_TestMessage, -1},
	{{3, "pod"}, PROTO_POD, offsetof(TestMessage, pod), 146, &proto_TestPod, -1},
	{{2, "rb"}, PROTO_LIST_BOOL, offsetof(TestMessage, rb), 170, NULL, -1},
	{{4, "ri32"}, PROTO_LIST_I32, offsetof(TestMessage, ri32), 178, NULL, -1},
	{{4, "rs32"}, PROTO_LIST_S32, offsetof(TestMessage, rs32), 186, NULL, -1},
	{{5, "rsf32"}, PROTO_LIST_SF32, offsetof(TestMessage, rsf32), 194, NULL, -1},
	{{4, "ru32"}, PROTO_LIST_U32, offsetof(TestMessage, ru32), 202, NULL, -1},
	{{4, "rf32"}, PROTO_LIST_F32, offsetof(TestMessage, rf32), 210, NULL, -1},
	{{4, "ri64"}, PROTO_LIST_I64, offsetof(TestMessage, ri64), 218, NULL, -1},
	{{5, "rsf64"}, PROTO_LIST_SF64, offsetof(TestMessage, rsf64), 226, NULL, -1},
	{{4, "rs64"}, PROTO_LIST_S64, offsetof(TestMessage, rs64), 234, NULL, -1},
	{{4, "ru64"}, PROTO_LIST_U64, offsetof(TestMessage, ru64), 1682, NULL, -1},
	{{4, "rf64"}, PROTO_LIST_F64, offsetof(TestMessage, rf64), 1690, NULL, -1},
	{{2, "rf"}, PROTO_LIST_FLOAT, offsetof(TestMessage, rf), 1698, NULL, -1},
	{{2, "rd"}, PROTO_LIST_DOUBLE, offsetof(TestMessage, rd), 1706, NULL, -1},
	{{3, "rby"}, PROTO_LIST_BYTES, offsetof(TestMessage, rby), 1714, NULL, -1},
	{{4, "rstr"}, PROTO_LIST_STRING, offsetof(TestMessage, rstr), 1722, NULL, -1},
	{{3, "ren"}, PROTO_LIST_ENUM, offsetof(TestMessage, ren), 1730, &proto_TestEnum, -1},
	{{4, "rmsg"}, PROTO_LIST_MESSAGE, offsetof(TestMessage, rmsg), 1738, &proto_TestMessage, -1},
	{{4, "rpod"}, PROTO_LIST_POD, offsetof(TestMessage, rpod), 1746, &proto_TestPod, -1},
	{{2, "mb"}, PROTO_LIST_POD, offsetof(TestMessage, mb), 1770, &proto_TestMessage_MbEntry, -1},
	{{4, "mi32"}, PROTO_LIST_POD, offsetof(TestMessage, mi32), 1778, &proto_TestMessage_Mi32Entry, -1},
	{{4, "ms32"}, PROTO_LIST_POD, offsetof(TestMessage, ms32), 1786, &proto_TestMessage_Ms32Entry, -1},
	{{5, "msf32"}, PROTO_LIST_POD, offsetof(TestMessage, msf32), 1794, &proto_TestMessage_Msf32Entry, -1},
	{{4, "mu32"}, PROTO_LIST_POD, offsetof(TestMessage, mu32), 1802, &proto_TestMessage_Mu32Entry, -1},
	{{4, "mf32"}, PROTO_LIST_POD, offsetof(TestMessage, mf32), 1810, &proto_TestMessage_Mf32Entry, -1},
	{{4, "mi64"}, PROTO_LIST_POD, offsetof(TestMessage, mi64), 1818, &proto_TestMessage_Mi64Entry, -1},
	{{5, "msf64"}, PROTO_LIST_POD, offsetof(TestMessage, msf64), 1826, &proto_TestMessage_Msf64Entry, -1},
	{{4, "ms64"}, PROTO_LIST_POD, offsetof(TestMessage, ms64), 1834, &proto_TestMessage_Ms64Entry, -1},
	{{4, "mu64"}, PROTO_LIST_POD, offsetof(TestMessage, mu64), 17682, &proto_TestMessage_Mu64Entry, -1},
	{{4, "mf64"}, PROTO_LIST_POD, offsetof(TestMessage, mf64), 17690, &proto_TestMessage_Mf64Entry, -1},
	{{2, "mf"}, PROTO_LIST_POD, offsetof(TestMessage, mf), 17698, &proto_TestMessage_MfEntry, -1},
	{{2, "md"}, PROTO_LIST_POD, offsetof(TestMessage, md), 17706, &proto_TestMessage_MdEntry, -1},
	{{3, "mby"}, PROTO_LIST_MESSAGE, offsetof(TestMessage, mby), 17714, &proto_TestMessage_MbyEntry, -1},
	{{4, "mstr"}, PROTO_LIST_MESSAGE, offsetof(TestMessage, mstr), 17722, &proto_TestMessage_MstrEntry, -1},
	{{3, "men"}, PROTO_LIST_POD, offsetof(TestMessage, men), 17730, &proto_TestMessage_MenEntry, -1},
	{{4, "mmsg"}, PROTO_LIST_MESSAGE, offsetof(TestMessage, mmsg), 17738, &proto_TestMessage_MmsgEntry, -1},
	{{4, "mpod"}, PROTO_LIST_POD, offsetof(TestMessage, mpod), 17746, &proto_TestMessage_MpodEntry, -1}
};
static const pb_string *by_name_TestMessage[] = {
	&fields_TestMessage[0].json_name,
	&fields_TestMessage[12].json_name,
	&fields_TestMessage[11].json_name,
	&fields_TestMessage[13].json_name,
	&fields_TestMessage[15].json_name,
	&fields_TestMessage[36].json_name,
	&fields_TestMessage[48].json_name,
	&fields_TestMessage[47].json_name,
	&fields_TestMessage[18].json_name,
	&fields_TestMessage[30].json_name,
	&fields_TestMessage[29].json_name,
	&fields_TestMessage[5].json_name,
	&fields_TestMessage[10].json_name,
	&fields_TestMessage[1].json_name,
	&fields_TestMessage[6].json_name,
	&fields_TestMessage[49].json_name,
	&fields_TestMessage[51].json_name,
	&fields_TestMessage[16].json_name,
	&fields_TestMessage[17].json_name,
	&fields_TestMessage[31].json_name,
	&fields_TestMessage[33].json_name,
	&fields_TestMessage[2].json_name,
	&fields_TestMessage[8].json_name,
	&fields_TestMessage[14].json_name,
	&fields_TestMessage[4].json_name,
	&fields_TestMessage[9].json_name,
	&fields_TestMessage[41].json_name,
	&fields_TestMessage[46].json_name,
	&fields_TestMessage[37].json_name,
	&fields_TestMessage[42].json_name,
	&fields_TestMessage[52].json_name,
	&fields_TestMessage[53].json_name,
	&fields_TestMessage[38].json_name,
	&fields_TestMessage[44].json_name,
	&fields_TestMessage[50].json_name,
	&fields_TestMessage[40].json_name,
	&fields_TestMessage[45].json_name,
	&fields_TestMessage[23].json_name,
	&fields_TestMessage[28].json_name,
	&fields_TestMessage[19].json_name,
	&fields_TestMessage[24].json_name,
	&fields_TestMessage[34].json_name,
	&fields_TestMessage[35].json_name,
	&fields_TestMessage[20].json_name,
	&fields_TestMessage[26].json_name,
	&fields_TestMessage[32].json_name,
	&fields_TestMessage[22].json_name,
	&fields_TestMessage[27].json_name,
	&fields_TestMessage[3].json_name,
	&fields_TestMessage[7].json_name,
	&fields_TestMessage[39].json_name,
	&fields_TestMessage[43].json_name,
	&fields_TestMessage[21].json_name,
	&fields_TestMessage[25].json_name
};
const proto_message proto_TestMessage = {
	sizeof(TestMessage),
	54,
	fields_TestMessage,
	by_name_TestMessage
};

static const proto_method method_TestService_rpc2 = {
	{23, "/twirp/TestService/rpc2"},
	0,
	&proto_TestPod,
	&proto_TestMessage
};
static const proto_method method_TestService_rpc1 = {
	{23, "/twirp/TestService/rpc1"},
	1,
	&proto_TestMessage,
	&proto_TestPod
};
static const pb_string *by_name_TestService[] = {
	&method_TestService_rpc1.path,
	&method_TestService_rpc2.path,
};
const proto_service proto_TestService = {
	2,
	by_name_TestService
};

static const struct proto_field fields_TestMessage_MbEntry[] = {
	{{3, "key"}, PROTO_U32, offsetof(TestMessage_MbEntry, key), 8, NULL, -1},
	{{5, "value"}, PROTO_BOOL, offsetof(TestMessage_MbEntry, value), 16, NULL, -1}
};
static const pb_string *by_name_TestMessage_MbEntry[] = {
	&fields_TestMessage_MbEntry[0].json_name,
	&fields_TestMessage_MbEntry[1].json_name
};
const proto_message proto_TestMessage_MbEntry = {
	sizeof(TestMessage_MbEntry),
	2,
	fields_TestMessage_MbEntry,
	by_name_TestMessage_MbEntry
};

static const struct proto_field fields_TestMessage_MdEntry[] = {
	{{3, "key"}, PROTO_U32, offsetof(TestMessage_MdEntry, key), 8, NULL, -1},
	{{5, "value"}, PROTO_DOUBLE, offsetof(TestMessage_MdEntry, value), 17, NULL, -1}
};
static const pb_string *by_name_TestMessage_MdEntry[] = {
	&fields_TestMessage_MdEntry[0].json_name,
	&fields_TestMessage_MdEntry[1].json_name
};
const proto_message proto_TestMessage_MdEntry = {
	sizeof(TestMessage_MdEntry),
	2,
	fields_TestMessage_MdEntry,
	by_name_TestMessage_MdEntry
};

static const struct proto_field fields_TestMessage_MfEntry[] = {
	{{3, "key"}, PROTO_U32, offsetof(TestMessage_MfEntry, key), 8, NULL, -1},
	{{5, "value"}, PROTO_FLOAT, offsetof(TestMessage_MfEntry, value), 21, NULL, -1}
};
static const pb_string *by_name_TestMessage_MfEntry[] = {
	&fields_TestMessage_MfEntry[0].json_name,
	&fields_TestMessage_MfEntry[1].json_name
};
const proto_message proto_TestMessage_MfEntry = {
	sizeof(TestMessage_MfEntry),
	2,
	fields_TestMessage_MfEntry,
	by_name_TestMessage_MfEntry
};

static const struct proto_field fields_TestMessage_MbyEntry[] = {
	{{3, "key"}, PROTO_U32, offsetof(TestMessage_MbyEntry, key), 8, NULL, -1},
	{{5, "value"}, PROTO_BYTES, offsetof(TestMessage_MbyEntry, value), 18, NULL, -1}
};
static const pb_string *by_name_TestMessage_MbyEntry[] = {
	&fields_TestMessage_MbyEntry[0].json_name,
	&fields_TestMessage_MbyEntry[1].json_name
};
const proto_message proto_TestMessage_MbyEntry = {
	sizeof(TestMessage_MbyEntry),
	2,
	fields_TestMessage_MbyEntry,
	by_name_TestMessage_MbyEntry
};

static const struct proto_field fields_TestMessage_MenEntry[] = {
	{{3, "key"}, PROTO_U32, offsetof(TestMessage_MenEntry, key), 8, NULL, -1},
	{{5, "value"}, PROTO_ENUM, offsetof(TestMessage_MenEntry, value), 16, &proto_TestEnum, -1}
};
static const pb_string *by_name_TestMessage_MenEntry[] = {
	&fields_TestMessage_MenEntry[0].json_name,
	&fields_TestMessage_MenEntry[1].json_name
};
const proto_message proto_TestMessage_MenEntry = {
	sizeof(TestMessage_MenEntry),
	2,
	fields_TestMessage_MenEntry,
	by_name_TestMessage_MenEntry
};

static const struct proto_field fields_TestMessage_Mf32Entry[] = {
	{{3, "key"}, PROTO_U32, offsetof(TestMessage_Mf32Entry, key), 8, NULL, -1},
	{{5, "value"}, PROTO_F32, offsetof(TestMessage_Mf32Entry, value), 21, NULL, -1}
};
static const pb_string *by_name_TestMessage_Mf32Entry[] = {
	&fields_TestMessage_Mf32Entry[0].json_name,
	&fields_TestMessage_Mf32Entry[1].json_name
};
const proto_message proto_TestMessage_Mf32Entry = {
	sizeof(TestMessage_Mf32Entry),
	2,
	fields_TestMessage_Mf32Entry,
	by_name_TestMessage_Mf32Entry
};

static const struct proto_field fields_TestMessage_Mf64Entry[] = {
	{{3, "key"}, PROTO_U32, offsetof(TestMessage_Mf64Entry, key), 8, NULL, -1},
	{{5, "value"}, PROTO_F64, offsetof(TestMessage_Mf64Entry, value), 17, NULL, -1}
};
static const pb_string *by_name_TestMessage_Mf64Entry[] = {
	&fields_TestMessage_Mf64Entry[0].json_name,
	&fields_TestMessage_Mf64Entry[1].json_name
};
const proto_message proto_TestMessage_Mf64Entry = {
	sizeof(TestMessage_Mf64Entry),
	2,
	fields_TestMessage_Mf64Entry,
	by_name_TestMessage_Mf64Entry
};

static const struct proto_field fields_TestMessage_Mi32Entry[] = {
	{{3, "key"}, PROTO_U32, offsetof(TestMessage_Mi32Entry, key), 8, NULL, -1},
	{{5, "value"}, PROTO_I32, offsetof(TestMessage_Mi32Entry, value), 16, NULL, -1}
};
static const pb_string *by_name_TestMessage_Mi32Entry[] = {
	&fields_TestMessage_Mi32Entry[0].json_name,
	&fields_TestMessage_Mi32Entry[1].json_name
};
const proto_message proto_TestMessage_Mi32Entry = {
	sizeof(TestMessage_Mi32Entry),
	2,
	fields_TestMessage_Mi32Entry,
	by_name_TestMessage_Mi32Entry
};

static const struct proto_field fields_TestMessage_Mi64Entry[] = {
	{{3, "key"}, PROTO_U32, offsetof(TestMessage_Mi64Entry, key), 8, NULL, -1},
	{{5, "value"}, PROTO_I64, offsetof(TestMessage_Mi64Entry, value), 16, NULL, -1}
};
static const pb_string *by_name_TestMessage_Mi64Entry[] = {
	&fields_TestMessage_Mi64Entry[0].json_name,
	&fields_TestMessage_Mi64Entry[1].json_name
};
const proto_message proto_TestMessage_Mi64Entry = {
	sizeof(TestMessage_Mi64Entry),
	2,
	fields_TestMessage_Mi64Entry,
	by_name_TestMessage_Mi64Entry
};

static const struct proto_field fields_TestMessage_MmsgEntry[] = {
	{{3, "key"}, PROTO_U32, offsetof(TestMessage_MmsgEntry, key), 8, NULL, -1},
	{{5, "value"}, PROTO_MESSAGE, offsetof(TestMessage_MmsgEntry, value), 18, &proto_TestMessage, -1}
};
static const pb_string *by_name_TestMessage_MmsgEntry[] = {
	&fields_TestMessage_MmsgEntry[0].json_name,
	&fields_TestMessage_MmsgEntry[1].json_name
};
const proto_message proto_TestMessage_MmsgEntry = {
	sizeof(TestMessage_MmsgEntry),
	2,
	fields_TestMessage_MmsgEntry,
	by_name_TestMessage_MmsgEntry
};

static const struct proto_field fields_TestMessage_MpodEntry[] = {
	{{3, "key"}, PROTO_U32, offsetof(TestMessage_MpodEntry, key), 8, NULL, -1},
	{{5, "value"}, PROTO_POD, offsetof(TestMessage_MpodEntry, value), 18, &proto_TestPod, -1}
};
static const pb_string *by_name_TestMessage_MpodEntry[] = {
	&fields_TestMessage_MpodEntry[0].json_name,
	&fields_TestMessage_MpodEntry[1].json_name
};
const proto_message proto_TestMessage_MpodEntry = {
	sizeof(TestMessage_MpodEntry),
	2,
	fields_TestMessage_MpodEntry,
	by_name_TestMessage_MpodEntry
};

static const struct proto_field fields_TestMessage_Ms32Entry[] = {
	{{3, "key"}, PROTO_U32, offsetof(TestMessage_Ms32Entry, key), 8, NULL, -1},
	{{5, "value"}, PROTO_S32, offsetof(TestMessage_Ms32Entry, value), 16, NULL, -1}
};
static const pb_string *by_name_TestMessage_Ms32Entry[] = {
	&fields_TestMessage_Ms32Entry[0].json_name,
	&fields_TestMessage_Ms32Entry[1].json_name
};
const proto_message proto_TestMessage_Ms32Entry = {
	sizeof(TestMessage_Ms32Entry),
	2,
	fields_TestMessage_Ms32Entry,
	by_name_TestMessage_Ms32Entry
};

static const struct proto_field fields_TestMessage_Ms64Entry[] = {
	{{3, "key"}, PROTO_U32, offsetof(TestMessage_Ms64Entry, key), 8, NULL, -1},
	{{5, "value"}, PROTO_S64, offsetof(TestMessage_Ms64Entry, value), 16, NULL, -1}
};
static const pb_string *by_name_TestMessage_Ms64Entry[] = {
	&fields_TestMessage_Ms64Entry[0].json_name,
	&fields_TestMessage_Ms64Entry[1].json_name
};
const proto_message proto_TestMessage_Ms64Entry = {
	sizeof(TestMessage_Ms64Entry),
	2,
	fields_TestMessage_Ms64Entry,
	by_name_TestMessage_Ms64Entry
};

static const struct proto_field fields_TestMessage_MstrEntry[] = {
	{{3, "key"}, PROTO_U32, offsetof(TestMessage_MstrEntry, key), 8, NULL, -1},
	{{5, "value"}, PROTO_STRING, offsetof(TestMessage_MstrEntry, value), 18, NULL, -1}
};
static const pb_string *by_name_TestMessage_MstrEntry[] = {
	&fields_TestMessage_MstrEntry[0].json_name,
	&fields_TestMessage_MstrEntry[1].json_name
};
const proto_message proto_TestMessage_MstrEntry = {
	sizeof(TestMessage_MstrEntry),
	2,
	fields_TestMessage_MstrEntry,
	by_name_TestMessage_MstrEntry
};

static const struct proto_field fields_TestMessage_Mu32Entry[] = {
	{{3, "key"}, PROTO_U32, offsetof(TestMessage_Mu32Entry, key), 8, NULL, -1},
	{{5, "value"}, PROTO_U32, offsetof(TestMessage_Mu32Entry, value), 16, NULL, -1}
};
static const pb_string *by_name_TestMessage_Mu32Entry[] = {
	&fields_TestMessage_Mu32Entry[0].json_name,
	&fields_TestMessage_Mu32Entry[1].json_name
};
const proto_message proto_TestMessage_Mu32Entry = {
	sizeof(TestMessage_Mu32Entry),
	2,
	fields_TestMessage_Mu32Entry,
	by_name_TestMessage_Mu32Entry
};

static const struct proto_field fields_TestMessage_Mu64Entry[] = {
	{{3, "key"}, PROTO_U32, offsetof(TestMessage_Mu64Entry, key), 8, NULL, -1},
	{{5, "value"}, PROTO_U64, offsetof(TestMessage_Mu64Entry, value), 16, NULL, -1}
};
static const pb_string *by_name_TestMessage_Mu64Entry[] = {
	&fields_TestMessage_Mu64Entry[0].json_name,
	&fields_TestMessage_Mu64Entry[1].json_name
};
const proto_message proto_TestMessage_Mu64Entry = {
	sizeof(TestMessage_Mu64Entry),
	2,
	fields_TestMessage_Mu64Entry,
	by_name_TestMessage_Mu64Entry
};

static const struct proto_field fields_TestMessage_Msf32Entry[] = {
	{{3, "key"}, PROTO_U32, offsetof(TestMessage_Msf32Entry, key), 8, NULL, -1},
	{{5, "value"}, PROTO_SF32, offsetof(TestMessage_Msf32Entry, value), 21, NULL, -1}
};
static const pb_string *by_name_TestMessage_Msf32Entry[] = {
	&fields_TestMessage_Msf32Entry[0].json_name,
	&fields_TestMessage_Msf32Entry[1].json_name
};
const proto_message proto_TestMessage_Msf32Entry = {
	sizeof(TestMessage_Msf32Entry),
	2,
	fields_TestMessage_Msf32Entry,
	by_name_TestMessage_Msf32Entry
};

static const struct proto_field fields_TestMessage_Msf64Entry[] = {
	{{3, "key"}, PROTO_U32, offsetof(TestMessage_Msf64Entry, key), 8, NULL, -1},
	{{5, "value"}, PROTO_SF64, offsetof(TestMessage_Msf64Entry, value), 17, NULL, -1}
};
static const pb_string *by_name_TestMessage_Msf64Entry[] = {
	&fields_TestMessage_Msf64Entry[0].json_name,
	&fields_TestMessage_Msf64Entry[1].json_name
};
const proto_message proto_TestMessage_Msf64Entry = {
	sizeof(TestMessage_Msf64Entry),
	2,
	fields_TestMessage_Msf64Entry,
	by_name_TestMessage_Msf64Entry
};

