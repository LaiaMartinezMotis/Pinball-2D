#ifndef __CHAIN_LIST_H__
#define __CHAIN_LIST_H__

// Pivot 0, 0
int scenario_points[318] = {
	5, 781,
	14, 596,
	25, 586,
	39, 577,
	39, 562,
	34, 550,
	25, 545,
	15, 538,
	5, 526,
	5, 376,
	11, 367,
	18, 358,
	27, 350,
	39, 342,
	52, 337,
	70, 334,
	65, 320,
	60, 308,
	54, 295,
	50, 279,
	48, 265,
	48, 251,
	47, 234,
	46, 213,
	33, 205,
	23, 191,
	22, 176,
	27, 161,
	39, 137,
	50, 119,
	40, 114,
	30, 106,
	23, 94,
	18, 85,
	17, 72,
	18, 57,
	22, 46,
	28, 39,
	37, 30,
	47, 24,
	57, 21,
	69, 20,
	78, 21,
	87, 24,
	94, 27,
	102, 33,
	108, 40,
	112, 47,
	116, 54,
	120, 63,
	131, 56,
	144, 51,
	156, 48,
	172, 43,
	190, 40,
	210, 38,
	229, 38,
	245, 39,
	262, 42,
	277, 45,
	295, 50,
	312, 57,
	330, 66,
	341, 73,
	353, 79,
	374, 96,
	386, 107,
	399, 124,
	408, 138,
	419, 156,
	426, 168,
	429, 182,
	432, 200,
	434, 222,
	432, 729,
	410, 728,
	408, 217,
	407, 206,
	404, 191,
	401, 180,
	396, 166,
	389, 152,
	382, 141,
	373, 130,
	364, 119,
	353, 109,
	340, 98,
	329, 91,
	316, 84,
	309, 79,
	298, 85,
	288, 92,
	276, 101,
	268, 106,
	268, 132,
	277, 138,
	284, 142,
	291, 147,
	298, 152,
	306, 159,
	314, 166,
	320, 174,
	326, 183,
	332, 195,
	335, 205,
	337, 218,
	337, 230,
	337, 243,
	335, 253,
	328, 267,
	323, 278,
	292, 332,
	292, 338,
	307, 347,
	314, 348,
	329, 329,
	341, 308,
	366, 256,
	368, 237,
	368, 217,
	366, 200,
	360, 180,
	354, 166,
	348, 155,
	337, 145,
	323, 133,
	314, 127,
	308, 118,
	315, 107,
	326, 107,
	336, 113,
	346, 121,
	356, 130,
	364, 140,
	373, 153,
	382, 170,
	389, 187,
	391, 201,
	393, 216,
	394, 231,
	392, 249,
	389, 262,
	382, 281,
	376, 296,
	370, 314,
	326, 420,
	326, 502,
	353, 528,
	395, 527,
	396, 778,
	373, 780,
	371, 743,
	267, 792,
	267, 830,
	176, 830,
	176, 792,
	38, 730,
	38, 782,
	8, 781
};

int wall_10000[14] = {
	102, 112,
	102, 180,
	106, 185,
	177, 136,
	178, 107,
	139, 81,
	102, 109
};

int wall_launch_ramp[56] = {
	41, 209,
	49, 211,
	57, 212,
	63, 216,
	64, 226,
	64, 242,
	65, 256,
	67, 268,
	69, 280,
	73, 290,
	80, 299,
	166, 346,
	154, 361,
	161, 371,
	171, 382,
	178, 393,
	188, 406,
	197, 422,
	175, 388,
	151, 359,
	136, 348,
	120, 340,
	104, 335,
	94, 334,
	82, 333,
	71, 333,
	55, 260,
	47, 219
};

int left_bumper[22] = {
	108, 615,
	107, 679,
	109, 686,
	113, 689,
	143, 708,
	149, 711,
	155, 708,
	154, 703,
	118, 612,
	114, 609,
	108, 612
};

int right_bumper[20] = {
	287, 707,
	322, 617,
	326, 613,
	332, 613,
	334, 618,
	334, 684,
	332, 689,
	293, 717,
	288, 715,
	286, 710
};

int left_slingshot[18] = {
	37, 613,
	43, 610,
	48, 615,
	48, 687,
	151, 740,
	144, 742,
	144, 750,
	37, 694,
	37, 617
};

int right_slingshot[18] = {
	301, 738,
	307, 741,
	307, 747,
	370, 714,
	370, 617,
	366, 612,
	361, 617,
	361, 708,
	303, 737
};

int right_blocker[10] = {
	74, 615,
	74, 672,
	80, 672,
	80, 615,
	77, 615
};

int bump_light[51] = {
35, 234,
	30, 235,
	25, 237,
	17, 243,
	12, 251,
	9, 260,
	9, 265,
	9, 270,
	12, 276,
	15, 282,
	19, 287,
	27, 291,
	35, 293,
	43, 293,
	50, 290,
	56, 285,
	62, 278,
	65, 272,
	67, 265,
	67, 259,
	65, 252,
	62, 246,
	58, 240,
	52, 236,
	40, 234
};

int up_bumper[42] =
{
	57, 44,
	52, 47,
	47, 54,
	44, 62,
	44, 71,
	46, 79,
	50, 86,
	56, 90,
	63, 94,
	72, 95,
	81, 91,
	88, 85,
	93, 77,
	94, 68,
	93, 59,
	89, 52,
	84, 46,
	77, 42,
	70, 41,
	65, 41,
	60, 43
};

int middle_bumper_1[42] =
{
	177, 197,
	185, 199,
	192, 202,
	197, 206,
	201, 213,
	202, 221,
	201, 229,
	199, 236,
	194, 243,
	186, 247,
	178, 249,
	169, 248,
	161, 243,
	156, 238,
	153, 231,
	153, 222,
	155, 214,
	159, 207,
	163, 202,
	169, 199,
	174, 197
};

int middle_bumper_2[44] =
{
	254, 171,
	265, 173,
	272, 179,
	276, 184,
	279, 190,
	280, 197,
	279, 205,
	276, 211,
	272, 217,
	265, 221,
	258, 223,
	251, 223,
	244, 220,
	238, 216,
	233, 210,
	230, 203,
	230, 197,
	231, 189,
	234, 182,
	239, 176,
	244, 173,
	250, 171
};

int middle_bumper_3[44] =
{
	212, 271,
	220, 271,
	226, 273,
	231, 277,
	235, 282,
	238, 287,
	239, 296,
	238, 304,
	235, 311,
	231, 317,
	225, 321,
	217, 323,
	208, 323,
	202, 320,
	195, 315,
	191, 307,
	189, 299,
	189, 291,
	192, 283,
	198, 277,
	203, 273,
	208, 271
};

int up_blocker_1[18] = {
	202, 108,
	206, 103,
	211, 103,
	213, 108,
	213, 141,
	211, 146,
	206, 146,
	202, 141,
	202, 112,
};

int up_blocker_2[18] = {
	236, 108,
	239, 103,
	244, 103,
	246, 108,
	247, 141,
	243, 145,
	238, 145,
	236, 140,
	236, 111
};

int right_flipper[20] = {
	297, 743,
	291, 741,
	240, 762,
	236, 766,
	236, 771,
	240, 774,
	245, 775,
	297, 756,
	301, 752,
	300, 746
};

int left_flipper[22] = {
	147, 745,
	153, 741,
	159, 741,
	208, 762,
	212, 766,
	212, 771,
	208, 774,
	202, 774,
	149, 757,
	146, 752,
	146, 748
};

int plunger[10] = {
	413, 655,
	413, 666,
	430, 666,
	430, 654,
	413, 654
};

int launch_ramp_sensor[8] = {
	77, 304,
	156, 347,
	161, 339,
	85, 299
};

int right_bumper_coll[6] = {
	323, 614,
	284, 709,
	291, 714
};

int left_bumper_coll[6] = {
	120, 612,
	159, 705,
	152, 710
};

int ramp_sensor[6] = {
	335, 253,
	291, 325,
	297, 329
};

int purple_ramp[72] = {
	105, 462,
	96, 452,
	96, 431,
	93, 410,
	103, 403,
	113, 396,
	123, 405,
	136, 418,
	152, 436,
	145, 421,
	138, 408,
	130, 396,
	122, 387,
	112, 376,
	101, 370,
	92, 366,
	82, 362,
	69, 361,
	53, 367,
	46, 377,
	46, 389,
	54, 393,
	63, 396,
	74, 400,
	84, 404,
	91, 411,
	95, 421,
	95, 440,
	97, 539,
	94, 549,
	86, 559,
	82, 575,
	93, 558,
	101, 555,
	105, 550,
	105, 464
};


#endif // !__CHAIN_LIST_H__

