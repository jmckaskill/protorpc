#include "test.proto.h"

static const struct proto_field fields_TestMessage[] = {
	{{1, "b"}, PROTO_BOOL, offsetof(struct TestMessage, b), 8, NULL, -1},
	{{3, "i32"}, PROTO_I32, offsetof(struct TestMessage, i32), 16, NULL, -1},
	{{3, "s32"}, PROTO_S32, offsetof(struct TestMessage, s32), 24, NULL, -1},
	{{4, "sf32"}, PROTO_SF32, offsetof(struct TestMessage, sf32), 37, NULL, -1},
	{{3, "u32"}, PROTO_U32, offsetof(struct TestMessage, u32), 40, NULL, -1},
	{{3, "f32"}, PROTO_F32, offsetof(struct TestMessage, f32), 53, NULL, -1},
	{{3, "i64"}, PROTO_I64, offsetof(struct TestMessage, i64), 56, NULL, -1},
	{{4, "sf64"}, PROTO_SF64, offsetof(struct TestMessage, sf64), 65, NULL, -1},
	{{3, "s64"}, PROTO_S64, offsetof(struct TestMessage, s64), 72, NULL, -1},
	{{3, "u64"}, PROTO_U64, offsetof(struct TestMessage, u64), 80, NULL, -1},
	{{3, "f64"}, PROTO_F64, offsetof(struct TestMessage, f64), 89, NULL, -1},
	{{1, "f"}, PROTO_FLOAT, offsetof(struct TestMessage, f), 101, NULL, -1},
	{{1, "d"}, PROTO_DOUBLE, offsetof(struct TestMessage, d), 105, NULL, -1},
	{{2, "by"}, PROTO_BYTES, offsetof(struct TestMessage, by), 114, NULL, -1},
	{{3, "str"}, PROTO_STRING, offsetof(struct TestMessage, str), 122, NULL, -1},
	{{2, "en"}, PROTO_ENUM, offsetof(struct TestMessage, en), 128, &pb_enum_TestEnum, -1},
	{{3, "msg"}, PROTO_MESSAGE, offsetof(struct TestMessage, msg), 138, &pb_type_TestMessage, -1},
	{{3, "pod"}, PROTO_POD, offsetof(struct TestMessage, pod), 146, &pb_type_TestPod, -1},
	{{2, "rb"}, PROTO_LIST_BOOL, offsetof(struct TestMessage, rb), 170, NULL, -1},
	{{4, "ri32"}, PROTO_LIST_I32, offsetof(struct TestMessage, ri32), 178, NULL, -1},
	{{4, "rs32"}, PROTO_LIST_S32, offsetof(struct TestMessage, rs32), 186, NULL, -1},
	{{5, "rsf32"}, PROTO_LIST_SF32, offsetof(struct TestMessage, rsf32), 194, NULL, -1},
	{{4, "ru32"}, PROTO_LIST_U32, offsetof(struct TestMessage, ru32), 202, NULL, -1},
	{{4, "rf32"}, PROTO_LIST_F32, offsetof(struct TestMessage, rf32), 210, NULL, -1},
	{{4, "ri64"}, PROTO_LIST_I64, offsetof(struct TestMessage, ri64), 218, NULL, -1},
	{{5, "rsf64"}, PROTO_LIST_SF64, offsetof(struct TestMessage, rsf64), 226, NULL, -1},
	{{4, "rs64"}, PROTO_LIST_S64, offsetof(struct TestMessage, rs64), 234, NULL, -1},
	{{4, "ru64"}, PROTO_LIST_U64, offsetof(struct TestMessage, ru64), 1682, NULL, -1},
	{{4, "rf64"}, PROTO_LIST_F64, offsetof(struct TestMessage, rf64), 1690, NULL, -1},
	{{2, "rf"}, PROTO_LIST_FLOAT, offsetof(struct TestMessage, rf), 1698, NULL, -1},
	{{2, "rd"}, PROTO_LIST_DOUBLE, offsetof(struct TestMessage, rd), 1706, NULL, -1},
	{{3, "rby"}, PROTO_LIST_BYTES, offsetof(struct TestMessage, rby), 1714, NULL, -1},
	{{4, "rstr"}, PROTO_LIST_STRING, offsetof(struct TestMessage, rstr), 1722, NULL, -1},
	{{3, "ren"}, PROTO_LIST_ENUM, offsetof(struct TestMessage, ren), 1730, &pb_enum_TestEnum, -1},
	{{4, "rmsg"}, PROTO_LIST_MESSAGE, offsetof(struct TestMessage, rmsg), 1738, &pb_type_TestMessage, -1},
	{{4, "rpod"}, PROTO_LIST_POD, offsetof(struct TestMessage, rpod), 1746, &pb_type_TestPod, -1},
	{{2, "mb"}, PROTO_LIST_POD, offsetof(struct TestMessage, mb), 1770, &pb_type_TestMessage_MbEntry, -1},
	{{4, "mi32"}, PROTO_LIST_POD, offsetof(struct TestMessage, mi32), 1778, &pb_type_TestMessage_Mi32Entry, -1},
	{{4, "ms32"}, PROTO_LIST_POD, offsetof(struct TestMessage, ms32), 1786, &pb_type_TestMessage_Ms32Entry, -1},
	{{5, "msf32"}, PROTO_LIST_POD, offsetof(struct TestMessage, msf32), 1794, &pb_type_TestMessage_Msf32Entry, -1},
	{{4, "mu32"}, PROTO_LIST_POD, offsetof(struct TestMessage, mu32), 1802, &pb_type_TestMessage_Mu32Entry, -1},
	{{4, "mf32"}, PROTO_LIST_POD, offsetof(struct TestMessage, mf32), 1810, &pb_type_TestMessage_Mf32Entry, -1},
	{{4, "mi64"}, PROTO_LIST_POD, offsetof(struct TestMessage, mi64), 1818, &pb_type_TestMessage_Mi64Entry, -1},
	{{5, "msf64"}, PROTO_LIST_POD, offsetof(struct TestMessage, msf64), 1826, &pb_type_TestMessage_Msf64Entry, -1},
	{{4, "ms64"}, PROTO_LIST_POD, offsetof(struct TestMessage, ms64), 1834, &pb_type_TestMessage_Ms64Entry, -1},
	{{4, "mu64"}, PROTO_LIST_POD, offsetof(struct TestMessage, mu64), 17682, &pb_type_TestMessage_Mu64Entry, -1},
	{{4, "mf64"}, PROTO_LIST_POD, offsetof(struct TestMessage, mf64), 17690, &pb_type_TestMessage_Mf64Entry, -1},
	{{2, "mf"}, PROTO_LIST_POD, offsetof(struct TestMessage, mf), 17698, &pb_type_TestMessage_MfEntry, -1},
	{{2, "md"}, PROTO_LIST_POD, offsetof(struct TestMessage, md), 17706, &pb_type_TestMessage_MdEntry, -1},
	{{3, "mby"}, PROTO_LIST_MESSAGE, offsetof(struct TestMessage, mby), 17714, &pb_type_TestMessage_MbyEntry, -1},
	{{4, "mstr"}, PROTO_LIST_MESSAGE, offsetof(struct TestMessage, mstr), 17722, &pb_type_TestMessage_MstrEntry, -1},
	{{3, "men"}, PROTO_LIST_POD, offsetof(struct TestMessage, men), 17730, &pb_type_TestMessage_MenEntry, -1},
	{{4, "mmsg"}, PROTO_LIST_MESSAGE, offsetof(struct TestMessage, mmsg), 17738, &pb_type_TestMessage_MmsgEntry, -1},
	{{4, "mpod"}, PROTO_LIST_POD, offsetof(struct TestMessage, mpod), 17746, &pb_type_TestMessage_MpodEntry, -1}
};
static const pb_string_t *by_name_TestMessage[] = {
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
const struct proto_message pb_type_TestMessage = {
	sizeof(struct TestMessage),
	sizeof(fields_TestMessage) / sizeof(struct proto_field),
	fields_TestMessage,
	by_name_TestMessage
};

static const struct proto_field fields_TestMessage_MbEntry[] = {
	{{3, "key"}, PROTO_U32, offsetof(struct TestMessage_MbEntry, key), 8, NULL, -1},
	{{5, "value"}, PROTO_BOOL, offsetof(struct TestMessage_MbEntry, value), 16, NULL, -1}
};
static const pb_string_t *by_name_TestMessage_MbEntry[] = {
	&fields_TestMessage_MbEntry[0].json_name,
	&fields_TestMessage_MbEntry[1].json_name
};
const struct proto_message pb_type_TestMessage_MbEntry = {
	sizeof(struct TestMessage_MbEntry),
	sizeof(fields_TestMessage_MbEntry) / sizeof(struct proto_field),
	fields_TestMessage_MbEntry,
	by_name_TestMessage_MbEntry
};

static const struct proto_field fields_TestMessage_Mu32Entry[] = {
	{{3, "key"}, PROTO_U32, offsetof(struct TestMessage_Mu32Entry, key), 8, NULL, -1},
	{{5, "value"}, PROTO_U32, offsetof(struct TestMessage_Mu32Entry, value), 16, NULL, -1}
};
static const pb_string_t *by_name_TestMessage_Mu32Entry[] = {
	&fields_TestMessage_Mu32Entry[0].json_name,
	&fields_TestMessage_Mu32Entry[1].json_name
};
const struct proto_message pb_type_TestMessage_Mu32Entry = {
	sizeof(struct TestMessage_Mu32Entry),
	sizeof(fields_TestMessage_Mu32Entry) / sizeof(struct proto_field),
	fields_TestMessage_Mu32Entry,
	by_name_TestMessage_Mu32Entry
};

static const struct proto_field fields_TestMessage_Mu64Entry[] = {
	{{3, "key"}, PROTO_U32, offsetof(struct TestMessage_Mu64Entry, key), 8, NULL, -1},
	{{5, "value"}, PROTO_U64, offsetof(struct TestMessage_Mu64Entry, value), 16, NULL, -1}
};
static const pb_string_t *by_name_TestMessage_Mu64Entry[] = {
	&fields_TestMessage_Mu64Entry[0].json_name,
	&fields_TestMessage_Mu64Entry[1].json_name
};
const struct proto_message pb_type_TestMessage_Mu64Entry = {
	sizeof(struct TestMessage_Mu64Entry),
	sizeof(fields_TestMessage_Mu64Entry) / sizeof(struct proto_field),
	fields_TestMessage_Mu64Entry,
	by_name_TestMessage_Mu64Entry
};

static const struct proto_field fields_TestMessage_Mi32Entry[] = {
	{{3, "key"}, PROTO_U32, offsetof(struct TestMessage_Mi32Entry, key), 8, NULL, -1},
	{{5, "value"}, PROTO_I32, offsetof(struct TestMessage_Mi32Entry, value), 16, NULL, -1}
};
static const pb_string_t *by_name_TestMessage_Mi32Entry[] = {
	&fields_TestMessage_Mi32Entry[0].json_name,
	&fields_TestMessage_Mi32Entry[1].json_name
};
const struct proto_message pb_type_TestMessage_Mi32Entry = {
	sizeof(struct TestMessage_Mi32Entry),
	sizeof(fields_TestMessage_Mi32Entry) / sizeof(struct proto_field),
	fields_TestMessage_Mi32Entry,
	by_name_TestMessage_Mi32Entry
};

static const struct proto_field fields_TestMessage_Mi64Entry[] = {
	{{3, "key"}, PROTO_U32, offsetof(struct TestMessage_Mi64Entry, key), 8, NULL, -1},
	{{5, "value"}, PROTO_I64, offsetof(struct TestMessage_Mi64Entry, value), 16, NULL, -1}
};
static const pb_string_t *by_name_TestMessage_Mi64Entry[] = {
	&fields_TestMessage_Mi64Entry[0].json_name,
	&fields_TestMessage_Mi64Entry[1].json_name
};
const struct proto_message pb_type_TestMessage_Mi64Entry = {
	sizeof(struct TestMessage_Mi64Entry),
	sizeof(fields_TestMessage_Mi64Entry) / sizeof(struct proto_field),
	fields_TestMessage_Mi64Entry,
	by_name_TestMessage_Mi64Entry
};

static const struct proto_field fields_TestMessage_Ms32Entry[] = {
	{{3, "key"}, PROTO_U32, offsetof(struct TestMessage_Ms32Entry, key), 8, NULL, -1},
	{{5, "value"}, PROTO_S32, offsetof(struct TestMessage_Ms32Entry, value), 16, NULL, -1}
};
static const pb_string_t *by_name_TestMessage_Ms32Entry[] = {
	&fields_TestMessage_Ms32Entry[0].json_name,
	&fields_TestMessage_Ms32Entry[1].json_name
};
const struct proto_message pb_type_TestMessage_Ms32Entry = {
	sizeof(struct TestMessage_Ms32Entry),
	sizeof(fields_TestMessage_Ms32Entry) / sizeof(struct proto_field),
	fields_TestMessage_Ms32Entry,
	by_name_TestMessage_Ms32Entry
};

static const struct proto_field fields_TestMessage_Ms64Entry[] = {
	{{3, "key"}, PROTO_U32, offsetof(struct TestMessage_Ms64Entry, key), 8, NULL, -1},
	{{5, "value"}, PROTO_S64, offsetof(struct TestMessage_Ms64Entry, value), 16, NULL, -1}
};
static const pb_string_t *by_name_TestMessage_Ms64Entry[] = {
	&fields_TestMessage_Ms64Entry[0].json_name,
	&fields_TestMessage_Ms64Entry[1].json_name
};
const struct proto_message pb_type_TestMessage_Ms64Entry = {
	sizeof(struct TestMessage_Ms64Entry),
	sizeof(fields_TestMessage_Ms64Entry) / sizeof(struct proto_field),
	fields_TestMessage_Ms64Entry,
	by_name_TestMessage_Ms64Entry
};

static const struct proto_field fields_TestMessage_Mf32Entry[] = {
	{{3, "key"}, PROTO_U32, offsetof(struct TestMessage_Mf32Entry, key), 8, NULL, -1},
	{{5, "value"}, PROTO_F32, offsetof(struct TestMessage_Mf32Entry, value), 21, NULL, -1}
};
static const pb_string_t *by_name_TestMessage_Mf32Entry[] = {
	&fields_TestMessage_Mf32Entry[0].json_name,
	&fields_TestMessage_Mf32Entry[1].json_name
};
const struct proto_message pb_type_TestMessage_Mf32Entry = {
	sizeof(struct TestMessage_Mf32Entry),
	sizeof(fields_TestMessage_Mf32Entry) / sizeof(struct proto_field),
	fields_TestMessage_Mf32Entry,
	by_name_TestMessage_Mf32Entry
};

static const struct proto_field fields_TestMessage_Mf64Entry[] = {
	{{3, "key"}, PROTO_U32, offsetof(struct TestMessage_Mf64Entry, key), 8, NULL, -1},
	{{5, "value"}, PROTO_F64, offsetof(struct TestMessage_Mf64Entry, value), 17, NULL, -1}
};
static const pb_string_t *by_name_TestMessage_Mf64Entry[] = {
	&fields_TestMessage_Mf64Entry[0].json_name,
	&fields_TestMessage_Mf64Entry[1].json_name
};
const struct proto_message pb_type_TestMessage_Mf64Entry = {
	sizeof(struct TestMessage_Mf64Entry),
	sizeof(fields_TestMessage_Mf64Entry) / sizeof(struct proto_field),
	fields_TestMessage_Mf64Entry,
	by_name_TestMessage_Mf64Entry
};

static const struct proto_field fields_TestMessage_Msf32Entry[] = {
	{{3, "key"}, PROTO_U32, offsetof(struct TestMessage_Msf32Entry, key), 8, NULL, -1},
	{{5, "value"}, PROTO_SF32, offsetof(struct TestMessage_Msf32Entry, value), 21, NULL, -1}
};
static const pb_string_t *by_name_TestMessage_Msf32Entry[] = {
	&fields_TestMessage_Msf32Entry[0].json_name,
	&fields_TestMessage_Msf32Entry[1].json_name
};
const struct proto_message pb_type_TestMessage_Msf32Entry = {
	sizeof(struct TestMessage_Msf32Entry),
	sizeof(fields_TestMessage_Msf32Entry) / sizeof(struct proto_field),
	fields_TestMessage_Msf32Entry,
	by_name_TestMessage_Msf32Entry
};

static const struct proto_field fields_TestMessage_Msf64Entry[] = {
	{{3, "key"}, PROTO_U32, offsetof(struct TestMessage_Msf64Entry, key), 8, NULL, -1},
	{{5, "value"}, PROTO_SF64, offsetof(struct TestMessage_Msf64Entry, value), 17, NULL, -1}
};
static const pb_string_t *by_name_TestMessage_Msf64Entry[] = {
	&fields_TestMessage_Msf64Entry[0].json_name,
	&fields_TestMessage_Msf64Entry[1].json_name
};
const struct proto_message pb_type_TestMessage_Msf64Entry = {
	sizeof(struct TestMessage_Msf64Entry),
	sizeof(fields_TestMessage_Msf64Entry) / sizeof(struct proto_field),
	fields_TestMessage_Msf64Entry,
	by_name_TestMessage_Msf64Entry
};

static const struct proto_field fields_TestMessage_MfEntry[] = {
	{{3, "key"}, PROTO_U32, offsetof(struct TestMessage_MfEntry, key), 8, NULL, -1},
	{{5, "value"}, PROTO_FLOAT, offsetof(struct TestMessage_MfEntry, value), 21, NULL, -1}
};
static const pb_string_t *by_name_TestMessage_MfEntry[] = {
	&fields_TestMessage_MfEntry[0].json_name,
	&fields_TestMessage_MfEntry[1].json_name
};
const struct proto_message pb_type_TestMessage_MfEntry = {
	sizeof(struct TestMessage_MfEntry),
	sizeof(fields_TestMessage_MfEntry) / sizeof(struct proto_field),
	fields_TestMessage_MfEntry,
	by_name_TestMessage_MfEntry
};

static const struct proto_field fields_TestMessage_MdEntry[] = {
	{{3, "key"}, PROTO_U32, offsetof(struct TestMessage_MdEntry, key), 8, NULL, -1},
	{{5, "value"}, PROTO_DOUBLE, offsetof(struct TestMessage_MdEntry, value), 17, NULL, -1}
};
static const pb_string_t *by_name_TestMessage_MdEntry[] = {
	&fields_TestMessage_MdEntry[0].json_name,
	&fields_TestMessage_MdEntry[1].json_name
};
const struct proto_message pb_type_TestMessage_MdEntry = {
	sizeof(struct TestMessage_MdEntry),
	sizeof(fields_TestMessage_MdEntry) / sizeof(struct proto_field),
	fields_TestMessage_MdEntry,
	by_name_TestMessage_MdEntry
};

static const struct proto_field fields_TestMessage_MbyEntry[] = {
	{{3, "key"}, PROTO_U32, offsetof(struct TestMessage_MbyEntry, key), 8, NULL, -1},
	{{5, "value"}, PROTO_BYTES, offsetof(struct TestMessage_MbyEntry, value), 18, NULL, -1}
};
static const pb_string_t *by_name_TestMessage_MbyEntry[] = {
	&fields_TestMessage_MbyEntry[0].json_name,
	&fields_TestMessage_MbyEntry[1].json_name
};
const struct proto_message pb_type_TestMessage_MbyEntry = {
	sizeof(struct TestMessage_MbyEntry),
	sizeof(fields_TestMessage_MbyEntry) / sizeof(struct proto_field),
	fields_TestMessage_MbyEntry,
	by_name_TestMessage_MbyEntry
};

static const struct proto_field fields_TestMessage_MstrEntry[] = {
	{{3, "key"}, PROTO_U32, offsetof(struct TestMessage_MstrEntry, key), 8, NULL, -1},
	{{5, "value"}, PROTO_STRING, offsetof(struct TestMessage_MstrEntry, value), 18, NULL, -1}
};
static const pb_string_t *by_name_TestMessage_MstrEntry[] = {
	&fields_TestMessage_MstrEntry[0].json_name,
	&fields_TestMessage_MstrEntry[1].json_name
};
const struct proto_message pb_type_TestMessage_MstrEntry = {
	sizeof(struct TestMessage_MstrEntry),
	sizeof(fields_TestMessage_MstrEntry) / sizeof(struct proto_field),
	fields_TestMessage_MstrEntry,
	by_name_TestMessage_MstrEntry
};

static const struct proto_field fields_TestMessage_MenEntry[] = {
	{{3, "key"}, PROTO_U32, offsetof(struct TestMessage_MenEntry, key), 8, NULL, -1},
	{{5, "value"}, PROTO_ENUM, offsetof(struct TestMessage_MenEntry, value), 16, &pb_enum_TestEnum, -1}
};
static const pb_string_t *by_name_TestMessage_MenEntry[] = {
	&fields_TestMessage_MenEntry[0].json_name,
	&fields_TestMessage_MenEntry[1].json_name
};
const struct proto_message pb_type_TestMessage_MenEntry = {
	sizeof(struct TestMessage_MenEntry),
	sizeof(fields_TestMessage_MenEntry) / sizeof(struct proto_field),
	fields_TestMessage_MenEntry,
	by_name_TestMessage_MenEntry
};

static const struct proto_field fields_TestMessage_MmsgEntry[] = {
	{{3, "key"}, PROTO_U32, offsetof(struct TestMessage_MmsgEntry, key), 8, NULL, -1},
	{{5, "value"}, PROTO_MESSAGE, offsetof(struct TestMessage_MmsgEntry, value), 18, &pb_type_TestMessage, -1}
};
static const pb_string_t *by_name_TestMessage_MmsgEntry[] = {
	&fields_TestMessage_MmsgEntry[0].json_name,
	&fields_TestMessage_MmsgEntry[1].json_name
};
const struct proto_message pb_type_TestMessage_MmsgEntry = {
	sizeof(struct TestMessage_MmsgEntry),
	sizeof(fields_TestMessage_MmsgEntry) / sizeof(struct proto_field),
	fields_TestMessage_MmsgEntry,
	by_name_TestMessage_MmsgEntry
};

static const struct proto_field fields_TestMessage_MpodEntry[] = {
	{{3, "key"}, PROTO_U32, offsetof(struct TestMessage_MpodEntry, key), 8, NULL, -1},
	{{5, "value"}, PROTO_POD, offsetof(struct TestMessage_MpodEntry, value), 18, &pb_type_TestPod, -1}
};
static const pb_string_t *by_name_TestMessage_MpodEntry[] = {
	&fields_TestMessage_MpodEntry[0].json_name,
	&fields_TestMessage_MpodEntry[1].json_name
};
const struct proto_message pb_type_TestMessage_MpodEntry = {
	sizeof(struct TestMessage_MpodEntry),
	sizeof(fields_TestMessage_MpodEntry) / sizeof(struct proto_field),
	fields_TestMessage_MpodEntry,
	by_name_TestMessage_MpodEntry
};

static const struct proto_field fields_TestPod[] = {
	{{1, "u"}, PROTO_U32, offsetof(struct TestPod, foo), 8, NULL, offsetof(struct TestPod, foo_type)},
	{{1, "i"}, PROTO_S32, offsetof(struct TestPod, foo), 16, NULL, offsetof(struct TestPod, foo_type)}
};
static const pb_string_t *by_name_TestPod[] = {
	&fields_TestPod[1].json_name,
	&fields_TestPod[0].json_name
};
const struct proto_message pb_type_TestPod = {
	sizeof(struct TestPod),
	sizeof(fields_TestPod) / sizeof(struct proto_field),
	fields_TestPod,
	by_name_TestPod
};

static const struct proto_enum_value values_TestEnum[] = {
	{{6, "ENUM_A"}, 0},
	{{6, "ENUM_B"}, 1},
	{{6, "ENUM_C"}, 2}
};
static const pb_string_t *by_name_TestEnum[] = {
	{&values_TestEnum[0].name},
	{&values_TestEnum[1].name},
	{&values_TestEnum[2].name}
};
const struct proto_enum pb_enum_TestEnum = {
	sizeof(values_TestEnum) / sizeof(struct proto_enum_value),
	values_TestEnum,
	by_name_TestEnum
};
