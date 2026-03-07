/* ------------------------------------------------------------
name: "jack_mod"
Code generated with Faust 2.83.1 (https://faust.grame.fr)
Compilation options: -lang rust -fpga-mem-th 4 -ct 1 -es 1 -mcd 16 -mdd 1024 -mdy 33 -single -ftz 0
------------------------------------------------------------ */
#[cfg_attr(feature = "default-boxed", derive(default_boxed::DefaultBoxed))]
#[repr(C)]
pub struct mydsp {
	fHslider0: F32,
	IOTA0: i32,
	fVec0: [F32;64],
	fConst0: F32,
	fVec1: [F32;64],
	fConst1: F32,
	fVec2: [F32;64],
	fConst2: F32,
	fVec3: [F32;64],
	fConst3: F32,
	fVec4: [F32;64],
	fConst4: F32,
	fVec5: [F32;64],
	fConst5: F32,
	fVec6: [F32;64],
	fConst6: F32,
	fVec7: [F32;64],
	fConst7: F32,
	fVec8: [F32;64],
	fConst8: F32,
	fVec9: [F32;64],
	fConst9: F32,
	fVec10: [F32;64],
	fConst10: F32,
	fVec11: [F32;64],
	fConst11: F32,
	fVec12: [F32;64],
	fConst12: F32,
	fVec13: [F32;64],
	fConst13: F32,
	fVec14: [F32;64],
	fConst14: F32,
	fVec15: [F32;64],
	fConst15: F32,
	fVec16: [F32;64],
	fConst16: F32,
	fVec17: [F32;64],
	fConst17: F32,
	fVec18: [F32;64],
	fConst18: F32,
	fVec19: [F32;64],
	fConst19: F32,
	fVec20: [F32;64],
	fConst20: F32,
	fVec21: [F32;64],
	fConst21: F32,
	fVec22: [F32;64],
	fConst22: F32,
	fVec23: [F32;64],
	fConst23: F32,
	fVec24: [F32;64],
	fConst24: F32,
	fVec25: [F32;64],
	fConst25: F32,
	fVec26: [F32;64],
	fConst26: F32,
	fVec27: [F32;64],
	fConst27: F32,
	fVec28: [F32;64],
	fConst28: F32,
	fVec29: [F32;64],
	fConst29: F32,
	fVec30: [F32;64],
	fConst30: F32,
	fVec31: [F32;32],
	fConst31: F32,
	fVec32: [F32;32],
	fConst32: F32,
	fVec33: [F32;32],
	fConst33: F32,
	fVec34: [F32;32],
	fConst34: F32,
	fVec35: [F32;32],
	fConst35: F32,
	fVec36: [F32;32],
	fConst36: F32,
	fVec37: [F32;32],
	fConst37: F32,
	fVec38: [F32;32],
	fConst38: F32,
	fVec39: [F32;32],
	fConst39: F32,
	fVec40: [F32;32],
	fConst40: F32,
	fVec41: [F32;32],
	fConst41: F32,
	fVec42: [F32;32],
	fConst42: F32,
	fVec43: [F32;32],
	fConst43: F32,
	fVec44: [F32;32],
	fConst44: F32,
	fVec45: [F32;32],
	fConst45: F32,
	fVec46: [F32;32],
	fConst46: F32,
	fVec47: [F32;16],
	fConst47: F32,
	fVec48: [F32;15],
	fConst48: F32,
	fVec49: [F32;14],
	fConst49: F32,
	fVec50: [F32;13],
	fConst50: F32,
	fVec51: [F32;12],
	fConst51: F32,
	fVec52: [F32;11],
	fConst52: F32,
	fVec53: [F32;10],
	fConst53: F32,
	fVec54: [F32;9],
	fConst54: F32,
	fVec55: [F32;8],
	fConst55: F32,
	fVec56: [F32;7],
	fConst56: F32,
	fVec57: [F32;6],
	fConst57: F32,
	fVec58: [F32;5],
	fConst58: F32,
	fVec59: [F32;4],
	fConst59: F32,
	fVec60: [F32;3],
	fConst60: F32,
	fVec61: [F32;2],
	iVec62: [i32;2],
	fConst61: F32,
	fConst62: F32,
	fVec63: [F32;128],
	fConst63: F32,
	fVec64: [F32;128],
	fConst64: F32,
	fVec65: [F32;128],
	fConst65: F32,
	fVec66: [F32;128],
	fConst66: F32,
	fVec67: [F32;128],
	fConst67: F32,
	fVec68: [F32;128],
	fConst68: F32,
	fVec69: [F32;128],
	fConst69: F32,
	fVec70: [F32;128],
	fConst70: F32,
	fVec71: [F32;128],
	fConst71: F32,
	fVec72: [F32;128],
	fConst72: F32,
	fVec73: [F32;128],
	fConst73: F32,
	fVec74: [F32;128],
	fConst74: F32,
	fVec75: [F32;128],
	fConst75: F32,
	fVec76: [F32;128],
	fConst76: F32,
	fVec77: [F32;128],
	fConst77: F32,
	fVec78: [F32;128],
	fConst78: F32,
	fVec79: [F32;128],
	fConst79: F32,
	fVec80: [F32;128],
	fConst80: F32,
	fVec81: [F32;128],
	fConst81: F32,
	fVec82: [F32;128],
	fConst82: F32,
	fVec83: [F32;128],
	fConst83: F32,
	fVec84: [F32;128],
	fConst84: F32,
	fVec85: [F32;128],
	fConst85: F32,
	fVec86: [F32;128],
	fConst86: F32,
	fVec87: [F32;128],
	fConst87: F32,
	fVec88: [F32;128],
	fConst88: F32,
	fVec89: [F32;128],
	fConst89: F32,
	fVec90: [F32;128],
	fConst90: F32,
	fVec91: [F32;128],
	fConst91: F32,
	fVec92: [F32;128],
	fConst92: F32,
	fVec93: [F32;128],
	fConst93: F32,
	fVec94: [F32;128],
	fConst94: F32,
	fVec95: [F32;128],
	fConst95: F32,
	fVec96: [F32;128],
	fConst96: F32,
	fVec97: [F32;128],
	fConst97: F32,
	fVec98: [F32;128],
	fConst98: F32,
	fVec99: [F32;128],
	fConst99: F32,
	fVec100: [F32;128],
	fConst100: F32,
	fVec101: [F32;128],
	fConst101: F32,
	fVec102: [F32;128],
	fConst102: F32,
	fVec103: [F32;128],
	fConst103: F32,
	fVec104: [F32;128],
	fConst104: F32,
	fVec105: [F32;128],
	fConst105: F32,
	fVec106: [F32;128],
	fConst106: F32,
	fVec107: [F32;128],
	fConst107: F32,
	fVec108: [F32;128],
	fConst108: F32,
	fVec109: [F32;128],
	fConst109: F32,
	fVec110: [F32;128],
	fConst110: F32,
	fVec111: [F32;128],
	fConst111: F32,
	fVec112: [F32;128],
	fConst112: F32,
	fVec113: [F32;128],
	fConst113: F32,
	fVec114: [F32;128],
	fConst114: F32,
	fVec115: [F32;128],
	fConst115: F32,
	fVec116: [F32;128],
	fConst116: F32,
	fVec117: [F32;128],
	fConst117: F32,
	fVec118: [F32;128],
	fConst118: F32,
	fVec119: [F32;128],
	fConst119: F32,
	fVec120: [F32;64],
	fConst120: F32,
	fVec121: [F32;64],
	fConst121: F32,
	fVec122: [F32;64],
	fConst122: F32,
	fVec123: [F32;64],
	fConst123: F32,
	fVec124: [F32;64],
	fConst124: F32,
	fVec125: [F32;64],
	fConst125: F32,
	fVec126: [F32;64],
	fConst126: F32,
	fVec127: [F32;64],
	fConst127: F32,
	fVec128: [F32;64],
	fConst128: F32,
	fVec129: [F32;64],
	fConst129: F32,
	fVec130: [F32;64],
	fConst130: F32,
	fVec131: [F32;64],
	fConst131: F32,
	fVec132: [F32;64],
	fConst132: F32,
	fVec133: [F32;64],
	fConst133: F32,
	fVec134: [F32;64],
	fConst134: F32,
	fVec135: [F32;64],
	fConst135: F32,
	fVec136: [F32;64],
	fConst136: F32,
	fVec137: [F32;64],
	fConst137: F32,
	fVec138: [F32;64],
	fConst138: F32,
	fVec139: [F32;64],
	fConst139: F32,
	fVec140: [F32;64],
	fConst140: F32,
	fVec141: [F32;64],
	fConst141: F32,
	fVec142: [F32;64],
	fConst142: F32,
	fVec143: [F32;64],
	fConst143: F32,
	fVec144: [F32;64],
	fConst144: F32,
	fVec145: [F32;64],
	fConst145: F32,
	fVec146: [F32;64],
	fConst146: F32,
	fVec147: [F32;64],
	fConst147: F32,
	fVec148: [F32;64],
	fConst148: F32,
	fVec149: [F32;64],
	fConst149: F32,
	fVec150: [F32;64],
	fConst150: F32,
	fVec151: [F32;64],
	fConst151: F32,
	fVec152: [F32;32],
	fConst152: F32,
	fVec153: [F32;32],
	fConst153: F32,
	fVec154: [F32;32],
	fConst154: F32,
	fVec155: [F32;32],
	fConst155: F32,
	fVec156: [F32;32],
	fConst156: F32,
	fVec157: [F32;32],
	fConst157: F32,
	fVec158: [F32;32],
	fConst158: F32,
	fVec159: [F32;32],
	fConst159: F32,
	fVec160: [F32;32],
	fConst160: F32,
	fVec161: [F32;32],
	fConst161: F32,
	fVec162: [F32;32],
	fConst162: F32,
	fVec163: [F32;32],
	fConst163: F32,
	fVec164: [F32;32],
	fConst164: F32,
	fVec165: [F32;32],
	fConst165: F32,
	fVec166: [F32;32],
	fConst166: F32,
	fVec167: [F32;32],
	fConst167: F32,
	fVec168: [F32;16],
	fConst168: F32,
	fVec169: [F32;15],
	fConst169: F32,
	fVec170: [F32;14],
	fConst170: F32,
	fVec171: [F32;13],
	fConst171: F32,
	fVec172: [F32;12],
	fConst172: F32,
	fVec173: [F32;11],
	fConst173: F32,
	fVec174: [F32;10],
	fConst174: F32,
	fVec175: [F32;9],
	fConst175: F32,
	fVec176: [F32;8],
	fConst176: F32,
	fVec177: [F32;7],
	fConst177: F32,
	fVec178: [F32;6],
	fConst178: F32,
	fVec179: [F32;5],
	fConst179: F32,
	fVec180: [F32;4],
	fConst180: F32,
	fVec181: [F32;3],
	fConst181: F32,
	fVec182: [F32;2],
	fConst182: F32,
	fConst183: F32,
	fSampleRate: i32,
	fConst184: F32,
	fConst185: F32,
	fConst186: F32,
	fHslider1: F32,
	fRec0: [F32;2],
	fHslider2: F32,
	fRec4: [F32;2],
	fConst187: F32,
	fRec3: [F32;2],
	fRec2: [F32;2],
	fRec1: [F32;2],
	fVec183: [F32;64],
	fVec184: [F32;64],
	fVec185: [F32;64],
	fVec186: [F32;64],
	fVec187: [F32;64],
	fVec188: [F32;64],
	fVec189: [F32;64],
	fVec190: [F32;64],
	fVec191: [F32;64],
	fVec192: [F32;64],
	fVec193: [F32;64],
	fVec194: [F32;64],
	fVec195: [F32;64],
	fVec196: [F32;64],
	fVec197: [F32;64],
	fVec198: [F32;64],
	fVec199: [F32;64],
	fVec200: [F32;64],
	fVec201: [F32;64],
	fVec202: [F32;64],
	fVec203: [F32;64],
	fVec204: [F32;64],
	fVec205: [F32;64],
	fVec206: [F32;64],
	fVec207: [F32;64],
	fVec208: [F32;64],
	fVec209: [F32;64],
	fVec210: [F32;64],
	fVec211: [F32;64],
	fVec212: [F32;64],
	fVec213: [F32;64],
	fVec214: [F32;32],
	fVec215: [F32;32],
	fVec216: [F32;32],
	fVec217: [F32;32],
	fVec218: [F32;32],
	fVec219: [F32;32],
	fVec220: [F32;32],
	fVec221: [F32;32],
	fVec222: [F32;32],
	fVec223: [F32;32],
	fVec224: [F32;32],
	fVec225: [F32;32],
	fVec226: [F32;32],
	fVec227: [F32;32],
	fVec228: [F32;32],
	fVec229: [F32;32],
	fVec230: [F32;16],
	fVec231: [F32;15],
	fVec232: [F32;14],
	fVec233: [F32;13],
	fVec234: [F32;12],
	fVec235: [F32;11],
	fVec236: [F32;10],
	fVec237: [F32;9],
	fVec238: [F32;8],
	fVec239: [F32;7],
	fVec240: [F32;6],
	fVec241: [F32;5],
	fVec242: [F32;4],
	fVec243: [F32;3],
	fVec244: [F32;2],
	fVec245: [F32;128],
	fVec246: [F32;128],
	fVec247: [F32;128],
	fVec248: [F32;128],
	fVec249: [F32;128],
	fVec250: [F32;128],
	fVec251: [F32;128],
	fVec252: [F32;128],
	fVec253: [F32;128],
	fVec254: [F32;128],
	fVec255: [F32;128],
	fVec256: [F32;128],
	fVec257: [F32;128],
	fVec258: [F32;128],
	fVec259: [F32;128],
	fVec260: [F32;128],
	fVec261: [F32;128],
	fVec262: [F32;128],
	fVec263: [F32;128],
	fVec264: [F32;128],
	fVec265: [F32;128],
	fVec266: [F32;128],
	fVec267: [F32;128],
	fVec268: [F32;128],
	fVec269: [F32;128],
	fVec270: [F32;128],
	fVec271: [F32;128],
	fVec272: [F32;128],
	fVec273: [F32;128],
	fVec274: [F32;128],
	fVec275: [F32;128],
	fVec276: [F32;128],
	fVec277: [F32;128],
	fVec278: [F32;128],
	fVec279: [F32;128],
	fVec280: [F32;128],
	fVec281: [F32;128],
	fVec282: [F32;128],
	fVec283: [F32;128],
	fVec284: [F32;128],
	fVec285: [F32;128],
	fVec286: [F32;128],
	fVec287: [F32;128],
	fVec288: [F32;128],
	fVec289: [F32;128],
	fVec290: [F32;128],
	fVec291: [F32;128],
	fVec292: [F32;128],
	fVec293: [F32;128],
	fVec294: [F32;128],
	fVec295: [F32;128],
	fVec296: [F32;128],
	fVec297: [F32;128],
	fVec298: [F32;128],
	fVec299: [F32;128],
	fVec300: [F32;128],
	fVec301: [F32;128],
	fVec302: [F32;64],
	fVec303: [F32;64],
	fVec304: [F32;64],
	fVec305: [F32;64],
	fVec306: [F32;64],
	fVec307: [F32;64],
	fVec308: [F32;64],
	fVec309: [F32;64],
	fVec310: [F32;64],
	fVec311: [F32;64],
	fVec312: [F32;64],
	fVec313: [F32;64],
	fVec314: [F32;64],
	fVec315: [F32;64],
	fVec316: [F32;64],
	fVec317: [F32;64],
	fVec318: [F32;64],
	fVec319: [F32;64],
	fVec320: [F32;64],
	fVec321: [F32;64],
	fVec322: [F32;64],
	fVec323: [F32;64],
	fVec324: [F32;64],
	fVec325: [F32;64],
	fVec326: [F32;64],
	fVec327: [F32;64],
	fVec328: [F32;64],
	fVec329: [F32;64],
	fVec330: [F32;64],
	fVec331: [F32;64],
	fVec332: [F32;64],
	fVec333: [F32;64],
	fVec334: [F32;32],
	fVec335: [F32;32],
	fVec336: [F32;32],
	fVec337: [F32;32],
	fVec338: [F32;32],
	fVec339: [F32;32],
	fVec340: [F32;32],
	fVec341: [F32;32],
	fVec342: [F32;32],
	fVec343: [F32;32],
	fVec344: [F32;32],
	fVec345: [F32;32],
	fVec346: [F32;32],
	fVec347: [F32;32],
	fVec348: [F32;32],
	fVec349: [F32;32],
	fVec350: [F32;16],
	fVec351: [F32;15],
	fVec352: [F32;14],
	fVec353: [F32;13],
	fVec354: [F32;12],
	fVec355: [F32;11],
	fVec356: [F32;10],
	fVec357: [F32;9],
	fVec358: [F32;8],
	fVec359: [F32;7],
	fVec360: [F32;6],
	fVec361: [F32;5],
	fVec362: [F32;4],
	fVec363: [F32;3],
	fVec364: [F32;2],
	fRec5: [F32;2],
	fRec6: [F32;2],
	fHslider3: F32,
	fVec365: [F32;64],
	fVec366: [F32;64],
	fVec367: [F32;64],
	fVec368: [F32;64],
	fVec369: [F32;64],
	fVec370: [F32;64],
	fVec371: [F32;64],
	fVec372: [F32;64],
	fVec373: [F32;64],
	fVec374: [F32;64],
	fVec375: [F32;64],
	fVec376: [F32;64],
	fVec377: [F32;64],
	fVec378: [F32;64],
	fVec379: [F32;64],
	fVec380: [F32;64],
	fVec381: [F32;64],
	fVec382: [F32;64],
	fVec383: [F32;64],
	fVec384: [F32;64],
	fVec385: [F32;64],
	fVec386: [F32;64],
	fVec387: [F32;64],
	fVec388: [F32;64],
	fVec389: [F32;64],
	fVec390: [F32;64],
	fVec391: [F32;64],
	fVec392: [F32;64],
	fVec393: [F32;64],
	fVec394: [F32;64],
	fVec395: [F32;64],
	fVec396: [F32;32],
	fVec397: [F32;32],
	fVec398: [F32;32],
	fVec399: [F32;32],
	fVec400: [F32;32],
	fVec401: [F32;32],
	fVec402: [F32;32],
	fVec403: [F32;32],
	fVec404: [F32;32],
	fVec405: [F32;32],
	fVec406: [F32;32],
	fVec407: [F32;32],
	fVec408: [F32;32],
	fVec409: [F32;32],
	fVec410: [F32;32],
	fVec411: [F32;32],
	fVec412: [F32;16],
	fVec413: [F32;15],
	fVec414: [F32;14],
	fVec415: [F32;13],
	fVec416: [F32;12],
	fVec417: [F32;11],
	fVec418: [F32;10],
	fVec419: [F32;9],
	fVec420: [F32;8],
	fVec421: [F32;7],
	fVec422: [F32;6],
	fVec423: [F32;5],
	fVec424: [F32;4],
	fVec425: [F32;3],
	fVec426: [F32;2],
	fVec427: [F32;128],
	fVec428: [F32;128],
	fVec429: [F32;128],
	fVec430: [F32;128],
	fVec431: [F32;128],
	fVec432: [F32;128],
	fVec433: [F32;128],
	fVec434: [F32;128],
	fVec435: [F32;128],
	fVec436: [F32;128],
	fVec437: [F32;128],
	fVec438: [F32;128],
	fVec439: [F32;128],
	fVec440: [F32;128],
	fVec441: [F32;128],
	fVec442: [F32;128],
	fVec443: [F32;128],
	fVec444: [F32;128],
	fVec445: [F32;128],
	fVec446: [F32;128],
	fVec447: [F32;128],
	fVec448: [F32;128],
	fVec449: [F32;128],
	fVec450: [F32;128],
	fVec451: [F32;128],
	fVec452: [F32;128],
	fVec453: [F32;128],
	fVec454: [F32;128],
	fVec455: [F32;128],
	fVec456: [F32;128],
	fVec457: [F32;128],
	fVec458: [F32;128],
	fVec459: [F32;128],
	fVec460: [F32;128],
	fVec461: [F32;128],
	fVec462: [F32;128],
	fVec463: [F32;128],
	fVec464: [F32;128],
	fVec465: [F32;128],
	fVec466: [F32;128],
	fVec467: [F32;128],
	fVec468: [F32;128],
	fVec469: [F32;128],
	fVec470: [F32;128],
	fVec471: [F32;128],
	fVec472: [F32;128],
	fVec473: [F32;128],
	fVec474: [F32;128],
	fVec475: [F32;128],
	fVec476: [F32;128],
	fVec477: [F32;128],
	fVec478: [F32;128],
	fVec479: [F32;128],
	fVec480: [F32;128],
	fVec481: [F32;128],
	fVec482: [F32;128],
	fVec483: [F32;128],
	fVec484: [F32;64],
	fVec485: [F32;64],
	fVec486: [F32;64],
	fVec487: [F32;64],
	fVec488: [F32;64],
	fVec489: [F32;64],
	fVec490: [F32;64],
	fVec491: [F32;64],
	fVec492: [F32;64],
	fVec493: [F32;64],
	fVec494: [F32;64],
	fVec495: [F32;64],
	fVec496: [F32;64],
	fVec497: [F32;64],
	fVec498: [F32;64],
	fVec499: [F32;64],
	fVec500: [F32;64],
	fVec501: [F32;64],
	fVec502: [F32;64],
	fVec503: [F32;64],
	fVec504: [F32;64],
	fVec505: [F32;64],
	fVec506: [F32;64],
	fVec507: [F32;64],
	fVec508: [F32;64],
	fVec509: [F32;64],
	fVec510: [F32;64],
	fVec511: [F32;64],
	fVec512: [F32;64],
	fVec513: [F32;64],
	fVec514: [F32;64],
	fVec515: [F32;64],
	fVec516: [F32;32],
	fVec517: [F32;32],
	fVec518: [F32;32],
	fVec519: [F32;32],
	fVec520: [F32;32],
	fVec521: [F32;32],
	fVec522: [F32;32],
	fVec523: [F32;32],
	fVec524: [F32;32],
	fVec525: [F32;32],
	fVec526: [F32;32],
	fVec527: [F32;32],
	fVec528: [F32;32],
	fVec529: [F32;32],
	fVec530: [F32;32],
	fVec531: [F32;32],
	fVec532: [F32;16],
	fVec533: [F32;15],
	fVec534: [F32;14],
	fVec535: [F32;13],
	fVec536: [F32;12],
	fVec537: [F32;11],
	fVec538: [F32;10],
	fVec539: [F32;9],
	fVec540: [F32;8],
	fVec541: [F32;7],
	fVec542: [F32;6],
	fVec543: [F32;5],
	fVec544: [F32;4],
	fVec545: [F32;3],
	fVec546: [F32;2],
	fVec547: [F32;64],
	fVec548: [F32;64],
	fVec549: [F32;64],
	fVec550: [F32;64],
	fVec551: [F32;64],
	fVec552: [F32;64],
	fVec553: [F32;64],
	fVec554: [F32;64],
	fVec555: [F32;64],
	fVec556: [F32;64],
	fVec557: [F32;64],
	fVec558: [F32;64],
	fVec559: [F32;64],
	fVec560: [F32;64],
	fVec561: [F32;64],
	fVec562: [F32;64],
	fVec563: [F32;64],
	fVec564: [F32;64],
	fVec565: [F32;64],
	fVec566: [F32;64],
	fVec567: [F32;64],
	fVec568: [F32;64],
	fVec569: [F32;64],
	fVec570: [F32;64],
	fVec571: [F32;64],
	fVec572: [F32;64],
	fVec573: [F32;64],
	fVec574: [F32;64],
	fVec575: [F32;64],
	fVec576: [F32;64],
	fVec577: [F32;64],
	fVec578: [F32;32],
	fVec579: [F32;32],
	fVec580: [F32;32],
	fVec581: [F32;32],
	fVec582: [F32;32],
	fVec583: [F32;32],
	fVec584: [F32;32],
	fVec585: [F32;32],
	fVec586: [F32;32],
	fVec587: [F32;32],
	fVec588: [F32;32],
	fVec589: [F32;32],
	fVec590: [F32;32],
	fVec591: [F32;32],
	fVec592: [F32;32],
	fVec593: [F32;32],
	fVec594: [F32;16],
	fVec595: [F32;15],
	fVec596: [F32;14],
	fVec597: [F32;13],
	fVec598: [F32;12],
	fVec599: [F32;11],
	fVec600: [F32;10],
	fVec601: [F32;9],
	fVec602: [F32;8],
	fVec603: [F32;7],
	fVec604: [F32;6],
	fVec605: [F32;5],
	fVec606: [F32;4],
	fVec607: [F32;3],
	fVec608: [F32;2],
	fVec609: [F32;128],
	fVec610: [F32;128],
	fVec611: [F32;128],
	fVec612: [F32;128],
	fVec613: [F32;128],
	fVec614: [F32;128],
	fVec615: [F32;128],
	fVec616: [F32;128],
	fVec617: [F32;128],
	fVec618: [F32;128],
	fVec619: [F32;128],
	fVec620: [F32;128],
	fVec621: [F32;128],
	fVec622: [F32;128],
	fVec623: [F32;128],
	fVec624: [F32;128],
	fVec625: [F32;128],
	fVec626: [F32;128],
	fVec627: [F32;128],
	fVec628: [F32;128],
	fVec629: [F32;128],
	fVec630: [F32;128],
	fVec631: [F32;128],
	fVec632: [F32;128],
	fVec633: [F32;128],
	fVec634: [F32;128],
	fVec635: [F32;128],
	fVec636: [F32;128],
	fVec637: [F32;128],
	fVec638: [F32;128],
	fVec639: [F32;128],
	fVec640: [F32;128],
	fVec641: [F32;128],
	fVec642: [F32;128],
	fVec643: [F32;128],
	fVec644: [F32;128],
	fVec645: [F32;128],
	fVec646: [F32;128],
	fVec647: [F32;128],
	fVec648: [F32;128],
	fVec649: [F32;128],
	fVec650: [F32;128],
	fVec651: [F32;128],
	fVec652: [F32;128],
	fVec653: [F32;128],
	fVec654: [F32;128],
	fVec655: [F32;128],
	fVec656: [F32;128],
	fVec657: [F32;128],
	fVec658: [F32;128],
	fVec659: [F32;128],
	fVec660: [F32;128],
	fVec661: [F32;128],
	fVec662: [F32;128],
	fVec663: [F32;128],
	fVec664: [F32;128],
	fVec665: [F32;128],
	fVec666: [F32;64],
	fVec667: [F32;64],
	fVec668: [F32;64],
	fVec669: [F32;64],
	fVec670: [F32;64],
	fVec671: [F32;64],
	fVec672: [F32;64],
	fVec673: [F32;64],
	fVec674: [F32;64],
	fVec675: [F32;64],
	fVec676: [F32;64],
	fVec677: [F32;64],
	fVec678: [F32;64],
	fVec679: [F32;64],
	fVec680: [F32;64],
	fVec681: [F32;64],
	fVec682: [F32;64],
	fVec683: [F32;64],
	fVec684: [F32;64],
	fVec685: [F32;64],
	fVec686: [F32;64],
	fVec687: [F32;64],
	fVec688: [F32;64],
	fVec689: [F32;64],
	fVec690: [F32;64],
	fVec691: [F32;64],
	fVec692: [F32;64],
	fVec693: [F32;64],
	fVec694: [F32;64],
	fVec695: [F32;64],
	fVec696: [F32;64],
	fVec697: [F32;64],
	fVec698: [F32;32],
	fVec699: [F32;32],
	fVec700: [F32;32],
	fVec701: [F32;32],
	fVec702: [F32;32],
	fVec703: [F32;32],
	fVec704: [F32;32],
	fVec705: [F32;32],
	fVec706: [F32;32],
	fVec707: [F32;32],
	fVec708: [F32;32],
	fVec709: [F32;32],
	fVec710: [F32;32],
	fVec711: [F32;32],
	fVec712: [F32;32],
	fVec713: [F32;32],
	fVec714: [F32;16],
	fVec715: [F32;15],
	fVec716: [F32;14],
	fVec717: [F32;13],
	fVec718: [F32;12],
	fVec719: [F32;11],
	fVec720: [F32;10],
	fVec721: [F32;9],
	fVec722: [F32;8],
	fVec723: [F32;7],
	fVec724: [F32;6],
	fVec725: [F32;5],
	fVec726: [F32;4],
	fVec727: [F32;3],
	fVec728: [F32;2],
}

pub type FaustFloat = F32;
static fmydspSIG0Wave0: [F32;63] = [-4.9476e-05,-0.000137191,-0.000250882,-0.000320806,-0.000213637,0.000256791,0.001283741,0.002982887,0.005278224,0.007784676,0.009728348,0.009952689,0.007053161,-0.000338348,-0.013133927,-0.031293433,-0.05335193,-0.07609794,-0.09453024,-0.10218869,-0.09189054,-0.056816965,0.008196073,0.10538281,0.23304498,0.3850622,0.5510849,0.7174632,0.8688237,0.99007213,1.0684999,1.0956339,1.0684999,0.99007213,0.8688237,0.7174632,0.5510849,0.3850622,0.23304498,0.10538281,0.008196073,-0.056816965,-0.09189054,-0.10218869,-0.09453024,-0.07609794,-0.05335193,-0.031293433,-0.013133927,-0.000338348,0.007053161,0.009952689,0.009728348,0.007784676,0.005278224,0.002982887,0.001283741,0.000256791,-0.000213637,-0.000320806,-0.000250882,-0.000137191,-4.9476e-05];

pub struct mydspSIG0 {
	fmydspSIG0Wave0_idx: i32,
}

impl mydspSIG0 {
	
	fn get_num_inputsmydspSIG0(&self) -> i32 {
		return 0;
	}
	fn get_num_outputsmydspSIG0(&self) -> i32 {
		return 1;
	}
	
	pub fn instance_initmydspSIG0(&mut self, sample_rate: i32) {
		self.fmydspSIG0Wave0_idx = 0;
	}
	
	pub fn fillmydspSIG0(&mut self, count: i32, table: &mut[FaustFloat]) {
		for i1 in 0..count {
			table[i1 as usize] = fmydspSIG0Wave0[self.fmydspSIG0Wave0_idx as usize];
			self.fmydspSIG0Wave0_idx = (i32::wrapping_add(1, self.fmydspSIG0Wave0_idx)) % 63;
		}
	}

}


pub fn newmydspSIG0() -> mydspSIG0 { 
	mydspSIG0 {
		fmydspSIG0Wave0_idx: 0,
	}
}
static fmydspSIG1Wave0: [F32;121] = [-5.9532e-05,-9.7442e-05,-0.000139292,-0.000177924,-0.000202471,-0.00019815,-0.000146458,-2.5871e-05,0.000186855,0.000514744,0.000978014,0.001590904,0.00235805,0.003270614,0.004302442,0.005406583,0.006512571,0.007524868,0.008322896,0.008763009,0.008682746,0.007907528,0.006259895,0.003571143,-0.000304906,-0.005476507,-0.011997645,-0.019851603,-0.028935434,-0.03904646,-0.049871873,-0.060982633,-0.071832575,-0.08176354,-0.09001724,-0.09575378,-0.09807714,-0.096066706,-0.088814184,-0.07546441,-0.05525859,-0.027577769,0.008015382,0.051738802,0.103559494,0.16317017,0.22997592,0.3030922,0.3813551,0.46334386,0.5474159,0.6317529,0.71441644,0.79341125,0.86675376,0.9325421,0.9890257,1.0346707,1.0682179,1.0887312,1.0956339,1.0887312,1.0682179,1.0346707,0.9890257,0.9325421,0.86675376,0.79341125,0.71441644,0.6317529,0.5474159,0.46334386,0.3813551,0.3030922,0.22997592,0.16317017,0.103559494,0.051738802,0.008015382,-0.027577769,-0.05525859,-0.07546441,-0.088814184,-0.096066706,-0.09807714,-0.09575378,-0.09001724,-0.08176354,-0.071832575,-0.060982633,-0.049871873,-0.03904646,-0.028935434,-0.019851603,-0.011997645,-0.005476507,-0.000304906,0.003571143,0.006259895,0.007907528,0.008682746,0.008763009,0.008322896,0.007524868,0.006512571,0.005406583,0.004302442,0.003270614,0.00235805,0.001590904,0.000978014,0.000514744,0.000186855,-2.5871e-05,-0.000146458,-0.00019815,-0.000202471,-0.000177924,-0.000139292,-9.7442e-05,-5.9532e-05];

pub struct mydspSIG1 {
	fmydspSIG1Wave0_idx: i32,
}

impl mydspSIG1 {
	
	fn get_num_inputsmydspSIG1(&self) -> i32 {
		return 0;
	}
	fn get_num_outputsmydspSIG1(&self) -> i32 {
		return 1;
	}
	
	pub fn instance_initmydspSIG1(&mut self, sample_rate: i32) {
		self.fmydspSIG1Wave0_idx = 0;
	}
	
	pub fn fillmydspSIG1(&mut self, count: i32, table: &mut[FaustFloat]) {
		for i2 in 0..count {
			table[i2 as usize] = fmydspSIG1Wave0[self.fmydspSIG1Wave0_idx as usize];
			self.fmydspSIG1Wave0_idx = (i32::wrapping_add(1, self.fmydspSIG1Wave0_idx)) % 121;
		}
	}

}


pub fn newmydspSIG1() -> mydspSIG1 { 
	mydspSIG1 {
		fmydspSIG1Wave0_idx: 0,
	}
}
static ftbl0mydspSIG0: std::sync::RwLock<[F32;63]>  = std::sync::RwLock::new([0.0;63]);
static ftbl1mydspSIG1: std::sync::RwLock<[F32;121]>  = std::sync::RwLock::new([0.0;121]);
mod ffi {
	use std::os::raw::c_float;
	// Conditionally compile the link attribute only on non-Windows platforms
	#[cfg_attr(not(target_os = "windows"), link(name = "m"))]
	unsafe extern "C" {
		pub fn remainderf(from: c_float, to: c_float) -> c_float;
		pub fn rintf(val: c_float) -> c_float;
	}
}
fn remainder_f32(from: f32, to: f32) -> f32 {
	unsafe { ffi::remainderf(from, to) }
}
fn rint_f32(val: f32) -> f32 {
	unsafe { ffi::rintf(val) }
}

pub const FAUST_INPUTS: usize = 736;
pub const FAUST_OUTPUTS: usize = 2;
pub const FAUST_ACTIVES: usize = 4;
pub const FAUST_PASSIVES: usize = 0;


impl mydsp {
		
	pub fn new() -> mydsp { 
		mydsp {
			fHslider0: 0.0,
			IOTA0: 0,
			fVec0: [0.0;64],
			fConst0: 0.0,
			fVec1: [0.0;64],
			fConst1: 0.0,
			fVec2: [0.0;64],
			fConst2: 0.0,
			fVec3: [0.0;64],
			fConst3: 0.0,
			fVec4: [0.0;64],
			fConst4: 0.0,
			fVec5: [0.0;64],
			fConst5: 0.0,
			fVec6: [0.0;64],
			fConst6: 0.0,
			fVec7: [0.0;64],
			fConst7: 0.0,
			fVec8: [0.0;64],
			fConst8: 0.0,
			fVec9: [0.0;64],
			fConst9: 0.0,
			fVec10: [0.0;64],
			fConst10: 0.0,
			fVec11: [0.0;64],
			fConst11: 0.0,
			fVec12: [0.0;64],
			fConst12: 0.0,
			fVec13: [0.0;64],
			fConst13: 0.0,
			fVec14: [0.0;64],
			fConst14: 0.0,
			fVec15: [0.0;64],
			fConst15: 0.0,
			fVec16: [0.0;64],
			fConst16: 0.0,
			fVec17: [0.0;64],
			fConst17: 0.0,
			fVec18: [0.0;64],
			fConst18: 0.0,
			fVec19: [0.0;64],
			fConst19: 0.0,
			fVec20: [0.0;64],
			fConst20: 0.0,
			fVec21: [0.0;64],
			fConst21: 0.0,
			fVec22: [0.0;64],
			fConst22: 0.0,
			fVec23: [0.0;64],
			fConst23: 0.0,
			fVec24: [0.0;64],
			fConst24: 0.0,
			fVec25: [0.0;64],
			fConst25: 0.0,
			fVec26: [0.0;64],
			fConst26: 0.0,
			fVec27: [0.0;64],
			fConst27: 0.0,
			fVec28: [0.0;64],
			fConst28: 0.0,
			fVec29: [0.0;64],
			fConst29: 0.0,
			fVec30: [0.0;64],
			fConst30: 0.0,
			fVec31: [0.0;32],
			fConst31: 0.0,
			fVec32: [0.0;32],
			fConst32: 0.0,
			fVec33: [0.0;32],
			fConst33: 0.0,
			fVec34: [0.0;32],
			fConst34: 0.0,
			fVec35: [0.0;32],
			fConst35: 0.0,
			fVec36: [0.0;32],
			fConst36: 0.0,
			fVec37: [0.0;32],
			fConst37: 0.0,
			fVec38: [0.0;32],
			fConst38: 0.0,
			fVec39: [0.0;32],
			fConst39: 0.0,
			fVec40: [0.0;32],
			fConst40: 0.0,
			fVec41: [0.0;32],
			fConst41: 0.0,
			fVec42: [0.0;32],
			fConst42: 0.0,
			fVec43: [0.0;32],
			fConst43: 0.0,
			fVec44: [0.0;32],
			fConst44: 0.0,
			fVec45: [0.0;32],
			fConst45: 0.0,
			fVec46: [0.0;32],
			fConst46: 0.0,
			fVec47: [0.0;16],
			fConst47: 0.0,
			fVec48: [0.0;15],
			fConst48: 0.0,
			fVec49: [0.0;14],
			fConst49: 0.0,
			fVec50: [0.0;13],
			fConst50: 0.0,
			fVec51: [0.0;12],
			fConst51: 0.0,
			fVec52: [0.0;11],
			fConst52: 0.0,
			fVec53: [0.0;10],
			fConst53: 0.0,
			fVec54: [0.0;9],
			fConst54: 0.0,
			fVec55: [0.0;8],
			fConst55: 0.0,
			fVec56: [0.0;7],
			fConst56: 0.0,
			fVec57: [0.0;6],
			fConst57: 0.0,
			fVec58: [0.0;5],
			fConst58: 0.0,
			fVec59: [0.0;4],
			fConst59: 0.0,
			fVec60: [0.0;3],
			fConst60: 0.0,
			fVec61: [0.0;2],
			iVec62: [0;2],
			fConst61: 0.0,
			fConst62: 0.0,
			fVec63: [0.0;128],
			fConst63: 0.0,
			fVec64: [0.0;128],
			fConst64: 0.0,
			fVec65: [0.0;128],
			fConst65: 0.0,
			fVec66: [0.0;128],
			fConst66: 0.0,
			fVec67: [0.0;128],
			fConst67: 0.0,
			fVec68: [0.0;128],
			fConst68: 0.0,
			fVec69: [0.0;128],
			fConst69: 0.0,
			fVec70: [0.0;128],
			fConst70: 0.0,
			fVec71: [0.0;128],
			fConst71: 0.0,
			fVec72: [0.0;128],
			fConst72: 0.0,
			fVec73: [0.0;128],
			fConst73: 0.0,
			fVec74: [0.0;128],
			fConst74: 0.0,
			fVec75: [0.0;128],
			fConst75: 0.0,
			fVec76: [0.0;128],
			fConst76: 0.0,
			fVec77: [0.0;128],
			fConst77: 0.0,
			fVec78: [0.0;128],
			fConst78: 0.0,
			fVec79: [0.0;128],
			fConst79: 0.0,
			fVec80: [0.0;128],
			fConst80: 0.0,
			fVec81: [0.0;128],
			fConst81: 0.0,
			fVec82: [0.0;128],
			fConst82: 0.0,
			fVec83: [0.0;128],
			fConst83: 0.0,
			fVec84: [0.0;128],
			fConst84: 0.0,
			fVec85: [0.0;128],
			fConst85: 0.0,
			fVec86: [0.0;128],
			fConst86: 0.0,
			fVec87: [0.0;128],
			fConst87: 0.0,
			fVec88: [0.0;128],
			fConst88: 0.0,
			fVec89: [0.0;128],
			fConst89: 0.0,
			fVec90: [0.0;128],
			fConst90: 0.0,
			fVec91: [0.0;128],
			fConst91: 0.0,
			fVec92: [0.0;128],
			fConst92: 0.0,
			fVec93: [0.0;128],
			fConst93: 0.0,
			fVec94: [0.0;128],
			fConst94: 0.0,
			fVec95: [0.0;128],
			fConst95: 0.0,
			fVec96: [0.0;128],
			fConst96: 0.0,
			fVec97: [0.0;128],
			fConst97: 0.0,
			fVec98: [0.0;128],
			fConst98: 0.0,
			fVec99: [0.0;128],
			fConst99: 0.0,
			fVec100: [0.0;128],
			fConst100: 0.0,
			fVec101: [0.0;128],
			fConst101: 0.0,
			fVec102: [0.0;128],
			fConst102: 0.0,
			fVec103: [0.0;128],
			fConst103: 0.0,
			fVec104: [0.0;128],
			fConst104: 0.0,
			fVec105: [0.0;128],
			fConst105: 0.0,
			fVec106: [0.0;128],
			fConst106: 0.0,
			fVec107: [0.0;128],
			fConst107: 0.0,
			fVec108: [0.0;128],
			fConst108: 0.0,
			fVec109: [0.0;128],
			fConst109: 0.0,
			fVec110: [0.0;128],
			fConst110: 0.0,
			fVec111: [0.0;128],
			fConst111: 0.0,
			fVec112: [0.0;128],
			fConst112: 0.0,
			fVec113: [0.0;128],
			fConst113: 0.0,
			fVec114: [0.0;128],
			fConst114: 0.0,
			fVec115: [0.0;128],
			fConst115: 0.0,
			fVec116: [0.0;128],
			fConst116: 0.0,
			fVec117: [0.0;128],
			fConst117: 0.0,
			fVec118: [0.0;128],
			fConst118: 0.0,
			fVec119: [0.0;128],
			fConst119: 0.0,
			fVec120: [0.0;64],
			fConst120: 0.0,
			fVec121: [0.0;64],
			fConst121: 0.0,
			fVec122: [0.0;64],
			fConst122: 0.0,
			fVec123: [0.0;64],
			fConst123: 0.0,
			fVec124: [0.0;64],
			fConst124: 0.0,
			fVec125: [0.0;64],
			fConst125: 0.0,
			fVec126: [0.0;64],
			fConst126: 0.0,
			fVec127: [0.0;64],
			fConst127: 0.0,
			fVec128: [0.0;64],
			fConst128: 0.0,
			fVec129: [0.0;64],
			fConst129: 0.0,
			fVec130: [0.0;64],
			fConst130: 0.0,
			fVec131: [0.0;64],
			fConst131: 0.0,
			fVec132: [0.0;64],
			fConst132: 0.0,
			fVec133: [0.0;64],
			fConst133: 0.0,
			fVec134: [0.0;64],
			fConst134: 0.0,
			fVec135: [0.0;64],
			fConst135: 0.0,
			fVec136: [0.0;64],
			fConst136: 0.0,
			fVec137: [0.0;64],
			fConst137: 0.0,
			fVec138: [0.0;64],
			fConst138: 0.0,
			fVec139: [0.0;64],
			fConst139: 0.0,
			fVec140: [0.0;64],
			fConst140: 0.0,
			fVec141: [0.0;64],
			fConst141: 0.0,
			fVec142: [0.0;64],
			fConst142: 0.0,
			fVec143: [0.0;64],
			fConst143: 0.0,
			fVec144: [0.0;64],
			fConst144: 0.0,
			fVec145: [0.0;64],
			fConst145: 0.0,
			fVec146: [0.0;64],
			fConst146: 0.0,
			fVec147: [0.0;64],
			fConst147: 0.0,
			fVec148: [0.0;64],
			fConst148: 0.0,
			fVec149: [0.0;64],
			fConst149: 0.0,
			fVec150: [0.0;64],
			fConst150: 0.0,
			fVec151: [0.0;64],
			fConst151: 0.0,
			fVec152: [0.0;32],
			fConst152: 0.0,
			fVec153: [0.0;32],
			fConst153: 0.0,
			fVec154: [0.0;32],
			fConst154: 0.0,
			fVec155: [0.0;32],
			fConst155: 0.0,
			fVec156: [0.0;32],
			fConst156: 0.0,
			fVec157: [0.0;32],
			fConst157: 0.0,
			fVec158: [0.0;32],
			fConst158: 0.0,
			fVec159: [0.0;32],
			fConst159: 0.0,
			fVec160: [0.0;32],
			fConst160: 0.0,
			fVec161: [0.0;32],
			fConst161: 0.0,
			fVec162: [0.0;32],
			fConst162: 0.0,
			fVec163: [0.0;32],
			fConst163: 0.0,
			fVec164: [0.0;32],
			fConst164: 0.0,
			fVec165: [0.0;32],
			fConst165: 0.0,
			fVec166: [0.0;32],
			fConst166: 0.0,
			fVec167: [0.0;32],
			fConst167: 0.0,
			fVec168: [0.0;16],
			fConst168: 0.0,
			fVec169: [0.0;15],
			fConst169: 0.0,
			fVec170: [0.0;14],
			fConst170: 0.0,
			fVec171: [0.0;13],
			fConst171: 0.0,
			fVec172: [0.0;12],
			fConst172: 0.0,
			fVec173: [0.0;11],
			fConst173: 0.0,
			fVec174: [0.0;10],
			fConst174: 0.0,
			fVec175: [0.0;9],
			fConst175: 0.0,
			fVec176: [0.0;8],
			fConst176: 0.0,
			fVec177: [0.0;7],
			fConst177: 0.0,
			fVec178: [0.0;6],
			fConst178: 0.0,
			fVec179: [0.0;5],
			fConst179: 0.0,
			fVec180: [0.0;4],
			fConst180: 0.0,
			fVec181: [0.0;3],
			fConst181: 0.0,
			fVec182: [0.0;2],
			fConst182: 0.0,
			fConst183: 0.0,
			fSampleRate: 0,
			fConst184: 0.0,
			fConst185: 0.0,
			fConst186: 0.0,
			fHslider1: 0.0,
			fRec0: [0.0;2],
			fHslider2: 0.0,
			fRec4: [0.0;2],
			fConst187: 0.0,
			fRec3: [0.0;2],
			fRec2: [0.0;2],
			fRec1: [0.0;2],
			fVec183: [0.0;64],
			fVec184: [0.0;64],
			fVec185: [0.0;64],
			fVec186: [0.0;64],
			fVec187: [0.0;64],
			fVec188: [0.0;64],
			fVec189: [0.0;64],
			fVec190: [0.0;64],
			fVec191: [0.0;64],
			fVec192: [0.0;64],
			fVec193: [0.0;64],
			fVec194: [0.0;64],
			fVec195: [0.0;64],
			fVec196: [0.0;64],
			fVec197: [0.0;64],
			fVec198: [0.0;64],
			fVec199: [0.0;64],
			fVec200: [0.0;64],
			fVec201: [0.0;64],
			fVec202: [0.0;64],
			fVec203: [0.0;64],
			fVec204: [0.0;64],
			fVec205: [0.0;64],
			fVec206: [0.0;64],
			fVec207: [0.0;64],
			fVec208: [0.0;64],
			fVec209: [0.0;64],
			fVec210: [0.0;64],
			fVec211: [0.0;64],
			fVec212: [0.0;64],
			fVec213: [0.0;64],
			fVec214: [0.0;32],
			fVec215: [0.0;32],
			fVec216: [0.0;32],
			fVec217: [0.0;32],
			fVec218: [0.0;32],
			fVec219: [0.0;32],
			fVec220: [0.0;32],
			fVec221: [0.0;32],
			fVec222: [0.0;32],
			fVec223: [0.0;32],
			fVec224: [0.0;32],
			fVec225: [0.0;32],
			fVec226: [0.0;32],
			fVec227: [0.0;32],
			fVec228: [0.0;32],
			fVec229: [0.0;32],
			fVec230: [0.0;16],
			fVec231: [0.0;15],
			fVec232: [0.0;14],
			fVec233: [0.0;13],
			fVec234: [0.0;12],
			fVec235: [0.0;11],
			fVec236: [0.0;10],
			fVec237: [0.0;9],
			fVec238: [0.0;8],
			fVec239: [0.0;7],
			fVec240: [0.0;6],
			fVec241: [0.0;5],
			fVec242: [0.0;4],
			fVec243: [0.0;3],
			fVec244: [0.0;2],
			fVec245: [0.0;128],
			fVec246: [0.0;128],
			fVec247: [0.0;128],
			fVec248: [0.0;128],
			fVec249: [0.0;128],
			fVec250: [0.0;128],
			fVec251: [0.0;128],
			fVec252: [0.0;128],
			fVec253: [0.0;128],
			fVec254: [0.0;128],
			fVec255: [0.0;128],
			fVec256: [0.0;128],
			fVec257: [0.0;128],
			fVec258: [0.0;128],
			fVec259: [0.0;128],
			fVec260: [0.0;128],
			fVec261: [0.0;128],
			fVec262: [0.0;128],
			fVec263: [0.0;128],
			fVec264: [0.0;128],
			fVec265: [0.0;128],
			fVec266: [0.0;128],
			fVec267: [0.0;128],
			fVec268: [0.0;128],
			fVec269: [0.0;128],
			fVec270: [0.0;128],
			fVec271: [0.0;128],
			fVec272: [0.0;128],
			fVec273: [0.0;128],
			fVec274: [0.0;128],
			fVec275: [0.0;128],
			fVec276: [0.0;128],
			fVec277: [0.0;128],
			fVec278: [0.0;128],
			fVec279: [0.0;128],
			fVec280: [0.0;128],
			fVec281: [0.0;128],
			fVec282: [0.0;128],
			fVec283: [0.0;128],
			fVec284: [0.0;128],
			fVec285: [0.0;128],
			fVec286: [0.0;128],
			fVec287: [0.0;128],
			fVec288: [0.0;128],
			fVec289: [0.0;128],
			fVec290: [0.0;128],
			fVec291: [0.0;128],
			fVec292: [0.0;128],
			fVec293: [0.0;128],
			fVec294: [0.0;128],
			fVec295: [0.0;128],
			fVec296: [0.0;128],
			fVec297: [0.0;128],
			fVec298: [0.0;128],
			fVec299: [0.0;128],
			fVec300: [0.0;128],
			fVec301: [0.0;128],
			fVec302: [0.0;64],
			fVec303: [0.0;64],
			fVec304: [0.0;64],
			fVec305: [0.0;64],
			fVec306: [0.0;64],
			fVec307: [0.0;64],
			fVec308: [0.0;64],
			fVec309: [0.0;64],
			fVec310: [0.0;64],
			fVec311: [0.0;64],
			fVec312: [0.0;64],
			fVec313: [0.0;64],
			fVec314: [0.0;64],
			fVec315: [0.0;64],
			fVec316: [0.0;64],
			fVec317: [0.0;64],
			fVec318: [0.0;64],
			fVec319: [0.0;64],
			fVec320: [0.0;64],
			fVec321: [0.0;64],
			fVec322: [0.0;64],
			fVec323: [0.0;64],
			fVec324: [0.0;64],
			fVec325: [0.0;64],
			fVec326: [0.0;64],
			fVec327: [0.0;64],
			fVec328: [0.0;64],
			fVec329: [0.0;64],
			fVec330: [0.0;64],
			fVec331: [0.0;64],
			fVec332: [0.0;64],
			fVec333: [0.0;64],
			fVec334: [0.0;32],
			fVec335: [0.0;32],
			fVec336: [0.0;32],
			fVec337: [0.0;32],
			fVec338: [0.0;32],
			fVec339: [0.0;32],
			fVec340: [0.0;32],
			fVec341: [0.0;32],
			fVec342: [0.0;32],
			fVec343: [0.0;32],
			fVec344: [0.0;32],
			fVec345: [0.0;32],
			fVec346: [0.0;32],
			fVec347: [0.0;32],
			fVec348: [0.0;32],
			fVec349: [0.0;32],
			fVec350: [0.0;16],
			fVec351: [0.0;15],
			fVec352: [0.0;14],
			fVec353: [0.0;13],
			fVec354: [0.0;12],
			fVec355: [0.0;11],
			fVec356: [0.0;10],
			fVec357: [0.0;9],
			fVec358: [0.0;8],
			fVec359: [0.0;7],
			fVec360: [0.0;6],
			fVec361: [0.0;5],
			fVec362: [0.0;4],
			fVec363: [0.0;3],
			fVec364: [0.0;2],
			fRec5: [0.0;2],
			fRec6: [0.0;2],
			fHslider3: 0.0,
			fVec365: [0.0;64],
			fVec366: [0.0;64],
			fVec367: [0.0;64],
			fVec368: [0.0;64],
			fVec369: [0.0;64],
			fVec370: [0.0;64],
			fVec371: [0.0;64],
			fVec372: [0.0;64],
			fVec373: [0.0;64],
			fVec374: [0.0;64],
			fVec375: [0.0;64],
			fVec376: [0.0;64],
			fVec377: [0.0;64],
			fVec378: [0.0;64],
			fVec379: [0.0;64],
			fVec380: [0.0;64],
			fVec381: [0.0;64],
			fVec382: [0.0;64],
			fVec383: [0.0;64],
			fVec384: [0.0;64],
			fVec385: [0.0;64],
			fVec386: [0.0;64],
			fVec387: [0.0;64],
			fVec388: [0.0;64],
			fVec389: [0.0;64],
			fVec390: [0.0;64],
			fVec391: [0.0;64],
			fVec392: [0.0;64],
			fVec393: [0.0;64],
			fVec394: [0.0;64],
			fVec395: [0.0;64],
			fVec396: [0.0;32],
			fVec397: [0.0;32],
			fVec398: [0.0;32],
			fVec399: [0.0;32],
			fVec400: [0.0;32],
			fVec401: [0.0;32],
			fVec402: [0.0;32],
			fVec403: [0.0;32],
			fVec404: [0.0;32],
			fVec405: [0.0;32],
			fVec406: [0.0;32],
			fVec407: [0.0;32],
			fVec408: [0.0;32],
			fVec409: [0.0;32],
			fVec410: [0.0;32],
			fVec411: [0.0;32],
			fVec412: [0.0;16],
			fVec413: [0.0;15],
			fVec414: [0.0;14],
			fVec415: [0.0;13],
			fVec416: [0.0;12],
			fVec417: [0.0;11],
			fVec418: [0.0;10],
			fVec419: [0.0;9],
			fVec420: [0.0;8],
			fVec421: [0.0;7],
			fVec422: [0.0;6],
			fVec423: [0.0;5],
			fVec424: [0.0;4],
			fVec425: [0.0;3],
			fVec426: [0.0;2],
			fVec427: [0.0;128],
			fVec428: [0.0;128],
			fVec429: [0.0;128],
			fVec430: [0.0;128],
			fVec431: [0.0;128],
			fVec432: [0.0;128],
			fVec433: [0.0;128],
			fVec434: [0.0;128],
			fVec435: [0.0;128],
			fVec436: [0.0;128],
			fVec437: [0.0;128],
			fVec438: [0.0;128],
			fVec439: [0.0;128],
			fVec440: [0.0;128],
			fVec441: [0.0;128],
			fVec442: [0.0;128],
			fVec443: [0.0;128],
			fVec444: [0.0;128],
			fVec445: [0.0;128],
			fVec446: [0.0;128],
			fVec447: [0.0;128],
			fVec448: [0.0;128],
			fVec449: [0.0;128],
			fVec450: [0.0;128],
			fVec451: [0.0;128],
			fVec452: [0.0;128],
			fVec453: [0.0;128],
			fVec454: [0.0;128],
			fVec455: [0.0;128],
			fVec456: [0.0;128],
			fVec457: [0.0;128],
			fVec458: [0.0;128],
			fVec459: [0.0;128],
			fVec460: [0.0;128],
			fVec461: [0.0;128],
			fVec462: [0.0;128],
			fVec463: [0.0;128],
			fVec464: [0.0;128],
			fVec465: [0.0;128],
			fVec466: [0.0;128],
			fVec467: [0.0;128],
			fVec468: [0.0;128],
			fVec469: [0.0;128],
			fVec470: [0.0;128],
			fVec471: [0.0;128],
			fVec472: [0.0;128],
			fVec473: [0.0;128],
			fVec474: [0.0;128],
			fVec475: [0.0;128],
			fVec476: [0.0;128],
			fVec477: [0.0;128],
			fVec478: [0.0;128],
			fVec479: [0.0;128],
			fVec480: [0.0;128],
			fVec481: [0.0;128],
			fVec482: [0.0;128],
			fVec483: [0.0;128],
			fVec484: [0.0;64],
			fVec485: [0.0;64],
			fVec486: [0.0;64],
			fVec487: [0.0;64],
			fVec488: [0.0;64],
			fVec489: [0.0;64],
			fVec490: [0.0;64],
			fVec491: [0.0;64],
			fVec492: [0.0;64],
			fVec493: [0.0;64],
			fVec494: [0.0;64],
			fVec495: [0.0;64],
			fVec496: [0.0;64],
			fVec497: [0.0;64],
			fVec498: [0.0;64],
			fVec499: [0.0;64],
			fVec500: [0.0;64],
			fVec501: [0.0;64],
			fVec502: [0.0;64],
			fVec503: [0.0;64],
			fVec504: [0.0;64],
			fVec505: [0.0;64],
			fVec506: [0.0;64],
			fVec507: [0.0;64],
			fVec508: [0.0;64],
			fVec509: [0.0;64],
			fVec510: [0.0;64],
			fVec511: [0.0;64],
			fVec512: [0.0;64],
			fVec513: [0.0;64],
			fVec514: [0.0;64],
			fVec515: [0.0;64],
			fVec516: [0.0;32],
			fVec517: [0.0;32],
			fVec518: [0.0;32],
			fVec519: [0.0;32],
			fVec520: [0.0;32],
			fVec521: [0.0;32],
			fVec522: [0.0;32],
			fVec523: [0.0;32],
			fVec524: [0.0;32],
			fVec525: [0.0;32],
			fVec526: [0.0;32],
			fVec527: [0.0;32],
			fVec528: [0.0;32],
			fVec529: [0.0;32],
			fVec530: [0.0;32],
			fVec531: [0.0;32],
			fVec532: [0.0;16],
			fVec533: [0.0;15],
			fVec534: [0.0;14],
			fVec535: [0.0;13],
			fVec536: [0.0;12],
			fVec537: [0.0;11],
			fVec538: [0.0;10],
			fVec539: [0.0;9],
			fVec540: [0.0;8],
			fVec541: [0.0;7],
			fVec542: [0.0;6],
			fVec543: [0.0;5],
			fVec544: [0.0;4],
			fVec545: [0.0;3],
			fVec546: [0.0;2],
			fVec547: [0.0;64],
			fVec548: [0.0;64],
			fVec549: [0.0;64],
			fVec550: [0.0;64],
			fVec551: [0.0;64],
			fVec552: [0.0;64],
			fVec553: [0.0;64],
			fVec554: [0.0;64],
			fVec555: [0.0;64],
			fVec556: [0.0;64],
			fVec557: [0.0;64],
			fVec558: [0.0;64],
			fVec559: [0.0;64],
			fVec560: [0.0;64],
			fVec561: [0.0;64],
			fVec562: [0.0;64],
			fVec563: [0.0;64],
			fVec564: [0.0;64],
			fVec565: [0.0;64],
			fVec566: [0.0;64],
			fVec567: [0.0;64],
			fVec568: [0.0;64],
			fVec569: [0.0;64],
			fVec570: [0.0;64],
			fVec571: [0.0;64],
			fVec572: [0.0;64],
			fVec573: [0.0;64],
			fVec574: [0.0;64],
			fVec575: [0.0;64],
			fVec576: [0.0;64],
			fVec577: [0.0;64],
			fVec578: [0.0;32],
			fVec579: [0.0;32],
			fVec580: [0.0;32],
			fVec581: [0.0;32],
			fVec582: [0.0;32],
			fVec583: [0.0;32],
			fVec584: [0.0;32],
			fVec585: [0.0;32],
			fVec586: [0.0;32],
			fVec587: [0.0;32],
			fVec588: [0.0;32],
			fVec589: [0.0;32],
			fVec590: [0.0;32],
			fVec591: [0.0;32],
			fVec592: [0.0;32],
			fVec593: [0.0;32],
			fVec594: [0.0;16],
			fVec595: [0.0;15],
			fVec596: [0.0;14],
			fVec597: [0.0;13],
			fVec598: [0.0;12],
			fVec599: [0.0;11],
			fVec600: [0.0;10],
			fVec601: [0.0;9],
			fVec602: [0.0;8],
			fVec603: [0.0;7],
			fVec604: [0.0;6],
			fVec605: [0.0;5],
			fVec606: [0.0;4],
			fVec607: [0.0;3],
			fVec608: [0.0;2],
			fVec609: [0.0;128],
			fVec610: [0.0;128],
			fVec611: [0.0;128],
			fVec612: [0.0;128],
			fVec613: [0.0;128],
			fVec614: [0.0;128],
			fVec615: [0.0;128],
			fVec616: [0.0;128],
			fVec617: [0.0;128],
			fVec618: [0.0;128],
			fVec619: [0.0;128],
			fVec620: [0.0;128],
			fVec621: [0.0;128],
			fVec622: [0.0;128],
			fVec623: [0.0;128],
			fVec624: [0.0;128],
			fVec625: [0.0;128],
			fVec626: [0.0;128],
			fVec627: [0.0;128],
			fVec628: [0.0;128],
			fVec629: [0.0;128],
			fVec630: [0.0;128],
			fVec631: [0.0;128],
			fVec632: [0.0;128],
			fVec633: [0.0;128],
			fVec634: [0.0;128],
			fVec635: [0.0;128],
			fVec636: [0.0;128],
			fVec637: [0.0;128],
			fVec638: [0.0;128],
			fVec639: [0.0;128],
			fVec640: [0.0;128],
			fVec641: [0.0;128],
			fVec642: [0.0;128],
			fVec643: [0.0;128],
			fVec644: [0.0;128],
			fVec645: [0.0;128],
			fVec646: [0.0;128],
			fVec647: [0.0;128],
			fVec648: [0.0;128],
			fVec649: [0.0;128],
			fVec650: [0.0;128],
			fVec651: [0.0;128],
			fVec652: [0.0;128],
			fVec653: [0.0;128],
			fVec654: [0.0;128],
			fVec655: [0.0;128],
			fVec656: [0.0;128],
			fVec657: [0.0;128],
			fVec658: [0.0;128],
			fVec659: [0.0;128],
			fVec660: [0.0;128],
			fVec661: [0.0;128],
			fVec662: [0.0;128],
			fVec663: [0.0;128],
			fVec664: [0.0;128],
			fVec665: [0.0;128],
			fVec666: [0.0;64],
			fVec667: [0.0;64],
			fVec668: [0.0;64],
			fVec669: [0.0;64],
			fVec670: [0.0;64],
			fVec671: [0.0;64],
			fVec672: [0.0;64],
			fVec673: [0.0;64],
			fVec674: [0.0;64],
			fVec675: [0.0;64],
			fVec676: [0.0;64],
			fVec677: [0.0;64],
			fVec678: [0.0;64],
			fVec679: [0.0;64],
			fVec680: [0.0;64],
			fVec681: [0.0;64],
			fVec682: [0.0;64],
			fVec683: [0.0;64],
			fVec684: [0.0;64],
			fVec685: [0.0;64],
			fVec686: [0.0;64],
			fVec687: [0.0;64],
			fVec688: [0.0;64],
			fVec689: [0.0;64],
			fVec690: [0.0;64],
			fVec691: [0.0;64],
			fVec692: [0.0;64],
			fVec693: [0.0;64],
			fVec694: [0.0;64],
			fVec695: [0.0;64],
			fVec696: [0.0;64],
			fVec697: [0.0;64],
			fVec698: [0.0;32],
			fVec699: [0.0;32],
			fVec700: [0.0;32],
			fVec701: [0.0;32],
			fVec702: [0.0;32],
			fVec703: [0.0;32],
			fVec704: [0.0;32],
			fVec705: [0.0;32],
			fVec706: [0.0;32],
			fVec707: [0.0;32],
			fVec708: [0.0;32],
			fVec709: [0.0;32],
			fVec710: [0.0;32],
			fVec711: [0.0;32],
			fVec712: [0.0;32],
			fVec713: [0.0;32],
			fVec714: [0.0;16],
			fVec715: [0.0;15],
			fVec716: [0.0;14],
			fVec717: [0.0;13],
			fVec718: [0.0;12],
			fVec719: [0.0;11],
			fVec720: [0.0;10],
			fVec721: [0.0;9],
			fVec722: [0.0;8],
			fVec723: [0.0;7],
			fVec724: [0.0;6],
			fVec725: [0.0;5],
			fVec726: [0.0;4],
			fVec727: [0.0;3],
			fVec728: [0.0;2],
		}
	}
	pub fn metadata(&self, m: &mut dyn Meta) { 
		m.declare("compile_options", r"-lang rust -fpga-mem-th 4 -ct 1 -es 1 -mcd 16 -mdd 1024 -mdy 33 -single -ftz 0");
		m.declare("filename", r"jack_mod.dsp");
		m.declare("filters.lib/lowpass0_highpass1", r"Copyright (C) 2003-2019 by Julius O. Smith III <jos@ccrma.stanford.edu>");
		m.declare("filters.lib/name", r"Faust Filters Library");
		m.declare("filters.lib/nlf2:author", r"Julius O. Smith III");
		m.declare("filters.lib/nlf2:copyright", r"Copyright (C) 2003-2019 by Julius O. Smith III <jos@ccrma.stanford.edu>");
		m.declare("filters.lib/nlf2:license", r"MIT-style STK-4.3 license");
		m.declare("filters.lib/version", r"1.7.1");
		m.declare("maths.lib/author", r"GRAME");
		m.declare("maths.lib/copyright", r"GRAME");
		m.declare("maths.lib/license", r"LGPL with exception");
		m.declare("maths.lib/name", r"Faust Math Library");
		m.declare("maths.lib/version", r"2.9.0");
		m.declare("name", r"jack_mod");
		m.declare("oscillators.lib/name", r"Faust Oscillator Library");
		m.declare("oscillators.lib/version", r"1.6.0");
		m.declare("platform.lib/name", r"Generic Platform Library");
		m.declare("platform.lib/version", r"1.3.0");
		m.declare("signals.lib/name", r"Faust Signal Routing Library");
		m.declare("signals.lib/version", r"1.6.0");
	}

	pub fn get_sample_rate(&self) -> i32 { self.fSampleRate as i32}
	
	pub fn class_init(sample_rate: i32) {
		// Obtaining locks on 2 static var(s)
		let mut ftbl0mydspSIG0_guard = ftbl0mydspSIG0.write().unwrap();
		let mut ftbl1mydspSIG1_guard = ftbl1mydspSIG1.write().unwrap();
		let mut sig0: mydspSIG0 = newmydspSIG0();
		sig0.instance_initmydspSIG0(sample_rate);
		sig0.fillmydspSIG0(63, ftbl0mydspSIG0_guard.as_mut());
		let mut sig1: mydspSIG1 = newmydspSIG1();
		sig1.instance_initmydspSIG1(sample_rate);
		sig1.fillmydspSIG1(121, ftbl1mydspSIG1_guard.as_mut());
	}
	pub fn instance_reset_params(&mut self) {
		self.fHslider0 = 0.0;
		self.fHslider1 = 3.14159;
		self.fHslider2 = 0.08333;
		self.fHslider3 = 0.8;
	}
	pub fn instance_clear(&mut self) {
		self.IOTA0 = 0;
		for l0 in 0..64 {
			self.fVec0[l0 as usize] = 0.0;
		}
		for l1 in 0..64 {
			self.fVec1[l1 as usize] = 0.0;
		}
		for l2 in 0..64 {
			self.fVec2[l2 as usize] = 0.0;
		}
		for l3 in 0..64 {
			self.fVec3[l3 as usize] = 0.0;
		}
		for l4 in 0..64 {
			self.fVec4[l4 as usize] = 0.0;
		}
		for l5 in 0..64 {
			self.fVec5[l5 as usize] = 0.0;
		}
		for l6 in 0..64 {
			self.fVec6[l6 as usize] = 0.0;
		}
		for l7 in 0..64 {
			self.fVec7[l7 as usize] = 0.0;
		}
		for l8 in 0..64 {
			self.fVec8[l8 as usize] = 0.0;
		}
		for l9 in 0..64 {
			self.fVec9[l9 as usize] = 0.0;
		}
		for l10 in 0..64 {
			self.fVec10[l10 as usize] = 0.0;
		}
		for l11 in 0..64 {
			self.fVec11[l11 as usize] = 0.0;
		}
		for l12 in 0..64 {
			self.fVec12[l12 as usize] = 0.0;
		}
		for l13 in 0..64 {
			self.fVec13[l13 as usize] = 0.0;
		}
		for l14 in 0..64 {
			self.fVec14[l14 as usize] = 0.0;
		}
		for l15 in 0..64 {
			self.fVec15[l15 as usize] = 0.0;
		}
		for l16 in 0..64 {
			self.fVec16[l16 as usize] = 0.0;
		}
		for l17 in 0..64 {
			self.fVec17[l17 as usize] = 0.0;
		}
		for l18 in 0..64 {
			self.fVec18[l18 as usize] = 0.0;
		}
		for l19 in 0..64 {
			self.fVec19[l19 as usize] = 0.0;
		}
		for l20 in 0..64 {
			self.fVec20[l20 as usize] = 0.0;
		}
		for l21 in 0..64 {
			self.fVec21[l21 as usize] = 0.0;
		}
		for l22 in 0..64 {
			self.fVec22[l22 as usize] = 0.0;
		}
		for l23 in 0..64 {
			self.fVec23[l23 as usize] = 0.0;
		}
		for l24 in 0..64 {
			self.fVec24[l24 as usize] = 0.0;
		}
		for l25 in 0..64 {
			self.fVec25[l25 as usize] = 0.0;
		}
		for l26 in 0..64 {
			self.fVec26[l26 as usize] = 0.0;
		}
		for l27 in 0..64 {
			self.fVec27[l27 as usize] = 0.0;
		}
		for l28 in 0..64 {
			self.fVec28[l28 as usize] = 0.0;
		}
		for l29 in 0..64 {
			self.fVec29[l29 as usize] = 0.0;
		}
		for l30 in 0..64 {
			self.fVec30[l30 as usize] = 0.0;
		}
		for l31 in 0..32 {
			self.fVec31[l31 as usize] = 0.0;
		}
		for l32 in 0..32 {
			self.fVec32[l32 as usize] = 0.0;
		}
		for l33 in 0..32 {
			self.fVec33[l33 as usize] = 0.0;
		}
		for l34 in 0..32 {
			self.fVec34[l34 as usize] = 0.0;
		}
		for l35 in 0..32 {
			self.fVec35[l35 as usize] = 0.0;
		}
		for l36 in 0..32 {
			self.fVec36[l36 as usize] = 0.0;
		}
		for l37 in 0..32 {
			self.fVec37[l37 as usize] = 0.0;
		}
		for l38 in 0..32 {
			self.fVec38[l38 as usize] = 0.0;
		}
		for l39 in 0..32 {
			self.fVec39[l39 as usize] = 0.0;
		}
		for l40 in 0..32 {
			self.fVec40[l40 as usize] = 0.0;
		}
		for l41 in 0..32 {
			self.fVec41[l41 as usize] = 0.0;
		}
		for l42 in 0..32 {
			self.fVec42[l42 as usize] = 0.0;
		}
		for l43 in 0..32 {
			self.fVec43[l43 as usize] = 0.0;
		}
		for l44 in 0..32 {
			self.fVec44[l44 as usize] = 0.0;
		}
		for l45 in 0..32 {
			self.fVec45[l45 as usize] = 0.0;
		}
		for l46 in 0..32 {
			self.fVec46[l46 as usize] = 0.0;
		}
		for l47 in 0..16 {
			self.fVec47[l47 as usize] = 0.0;
		}
		for l48 in 0..15 {
			self.fVec48[l48 as usize] = 0.0;
		}
		for l49 in 0..14 {
			self.fVec49[l49 as usize] = 0.0;
		}
		for l50 in 0..13 {
			self.fVec50[l50 as usize] = 0.0;
		}
		for l51 in 0..12 {
			self.fVec51[l51 as usize] = 0.0;
		}
		for l52 in 0..11 {
			self.fVec52[l52 as usize] = 0.0;
		}
		for l53 in 0..10 {
			self.fVec53[l53 as usize] = 0.0;
		}
		for l54 in 0..9 {
			self.fVec54[l54 as usize] = 0.0;
		}
		for l55 in 0..8 {
			self.fVec55[l55 as usize] = 0.0;
		}
		for l56 in 0..7 {
			self.fVec56[l56 as usize] = 0.0;
		}
		for l57 in 0..6 {
			self.fVec57[l57 as usize] = 0.0;
		}
		for l58 in 0..5 {
			self.fVec58[l58 as usize] = 0.0;
		}
		for l59 in 0..4 {
			self.fVec59[l59 as usize] = 0.0;
		}
		for l60 in 0..3 {
			self.fVec60[l60 as usize] = 0.0;
		}
		for l61 in 0..2 {
			self.fVec61[l61 as usize] = 0.0;
		}
		for l62 in 0..2 {
			self.iVec62[l62 as usize] = 0;
		}
		for l63 in 0..128 {
			self.fVec63[l63 as usize] = 0.0;
		}
		for l64 in 0..128 {
			self.fVec64[l64 as usize] = 0.0;
		}
		for l65 in 0..128 {
			self.fVec65[l65 as usize] = 0.0;
		}
		for l66 in 0..128 {
			self.fVec66[l66 as usize] = 0.0;
		}
		for l67 in 0..128 {
			self.fVec67[l67 as usize] = 0.0;
		}
		for l68 in 0..128 {
			self.fVec68[l68 as usize] = 0.0;
		}
		for l69 in 0..128 {
			self.fVec69[l69 as usize] = 0.0;
		}
		for l70 in 0..128 {
			self.fVec70[l70 as usize] = 0.0;
		}
		for l71 in 0..128 {
			self.fVec71[l71 as usize] = 0.0;
		}
		for l72 in 0..128 {
			self.fVec72[l72 as usize] = 0.0;
		}
		for l73 in 0..128 {
			self.fVec73[l73 as usize] = 0.0;
		}
		for l74 in 0..128 {
			self.fVec74[l74 as usize] = 0.0;
		}
		for l75 in 0..128 {
			self.fVec75[l75 as usize] = 0.0;
		}
		for l76 in 0..128 {
			self.fVec76[l76 as usize] = 0.0;
		}
		for l77 in 0..128 {
			self.fVec77[l77 as usize] = 0.0;
		}
		for l78 in 0..128 {
			self.fVec78[l78 as usize] = 0.0;
		}
		for l79 in 0..128 {
			self.fVec79[l79 as usize] = 0.0;
		}
		for l80 in 0..128 {
			self.fVec80[l80 as usize] = 0.0;
		}
		for l81 in 0..128 {
			self.fVec81[l81 as usize] = 0.0;
		}
		for l82 in 0..128 {
			self.fVec82[l82 as usize] = 0.0;
		}
		for l83 in 0..128 {
			self.fVec83[l83 as usize] = 0.0;
		}
		for l84 in 0..128 {
			self.fVec84[l84 as usize] = 0.0;
		}
		for l85 in 0..128 {
			self.fVec85[l85 as usize] = 0.0;
		}
		for l86 in 0..128 {
			self.fVec86[l86 as usize] = 0.0;
		}
		for l87 in 0..128 {
			self.fVec87[l87 as usize] = 0.0;
		}
		for l88 in 0..128 {
			self.fVec88[l88 as usize] = 0.0;
		}
		for l89 in 0..128 {
			self.fVec89[l89 as usize] = 0.0;
		}
		for l90 in 0..128 {
			self.fVec90[l90 as usize] = 0.0;
		}
		for l91 in 0..128 {
			self.fVec91[l91 as usize] = 0.0;
		}
		for l92 in 0..128 {
			self.fVec92[l92 as usize] = 0.0;
		}
		for l93 in 0..128 {
			self.fVec93[l93 as usize] = 0.0;
		}
		for l94 in 0..128 {
			self.fVec94[l94 as usize] = 0.0;
		}
		for l95 in 0..128 {
			self.fVec95[l95 as usize] = 0.0;
		}
		for l96 in 0..128 {
			self.fVec96[l96 as usize] = 0.0;
		}
		for l97 in 0..128 {
			self.fVec97[l97 as usize] = 0.0;
		}
		for l98 in 0..128 {
			self.fVec98[l98 as usize] = 0.0;
		}
		for l99 in 0..128 {
			self.fVec99[l99 as usize] = 0.0;
		}
		for l100 in 0..128 {
			self.fVec100[l100 as usize] = 0.0;
		}
		for l101 in 0..128 {
			self.fVec101[l101 as usize] = 0.0;
		}
		for l102 in 0..128 {
			self.fVec102[l102 as usize] = 0.0;
		}
		for l103 in 0..128 {
			self.fVec103[l103 as usize] = 0.0;
		}
		for l104 in 0..128 {
			self.fVec104[l104 as usize] = 0.0;
		}
		for l105 in 0..128 {
			self.fVec105[l105 as usize] = 0.0;
		}
		for l106 in 0..128 {
			self.fVec106[l106 as usize] = 0.0;
		}
		for l107 in 0..128 {
			self.fVec107[l107 as usize] = 0.0;
		}
		for l108 in 0..128 {
			self.fVec108[l108 as usize] = 0.0;
		}
		for l109 in 0..128 {
			self.fVec109[l109 as usize] = 0.0;
		}
		for l110 in 0..128 {
			self.fVec110[l110 as usize] = 0.0;
		}
		for l111 in 0..128 {
			self.fVec111[l111 as usize] = 0.0;
		}
		for l112 in 0..128 {
			self.fVec112[l112 as usize] = 0.0;
		}
		for l113 in 0..128 {
			self.fVec113[l113 as usize] = 0.0;
		}
		for l114 in 0..128 {
			self.fVec114[l114 as usize] = 0.0;
		}
		for l115 in 0..128 {
			self.fVec115[l115 as usize] = 0.0;
		}
		for l116 in 0..128 {
			self.fVec116[l116 as usize] = 0.0;
		}
		for l117 in 0..128 {
			self.fVec117[l117 as usize] = 0.0;
		}
		for l118 in 0..128 {
			self.fVec118[l118 as usize] = 0.0;
		}
		for l119 in 0..128 {
			self.fVec119[l119 as usize] = 0.0;
		}
		for l120 in 0..64 {
			self.fVec120[l120 as usize] = 0.0;
		}
		for l121 in 0..64 {
			self.fVec121[l121 as usize] = 0.0;
		}
		for l122 in 0..64 {
			self.fVec122[l122 as usize] = 0.0;
		}
		for l123 in 0..64 {
			self.fVec123[l123 as usize] = 0.0;
		}
		for l124 in 0..64 {
			self.fVec124[l124 as usize] = 0.0;
		}
		for l125 in 0..64 {
			self.fVec125[l125 as usize] = 0.0;
		}
		for l126 in 0..64 {
			self.fVec126[l126 as usize] = 0.0;
		}
		for l127 in 0..64 {
			self.fVec127[l127 as usize] = 0.0;
		}
		for l128 in 0..64 {
			self.fVec128[l128 as usize] = 0.0;
		}
		for l129 in 0..64 {
			self.fVec129[l129 as usize] = 0.0;
		}
		for l130 in 0..64 {
			self.fVec130[l130 as usize] = 0.0;
		}
		for l131 in 0..64 {
			self.fVec131[l131 as usize] = 0.0;
		}
		for l132 in 0..64 {
			self.fVec132[l132 as usize] = 0.0;
		}
		for l133 in 0..64 {
			self.fVec133[l133 as usize] = 0.0;
		}
		for l134 in 0..64 {
			self.fVec134[l134 as usize] = 0.0;
		}
		for l135 in 0..64 {
			self.fVec135[l135 as usize] = 0.0;
		}
		for l136 in 0..64 {
			self.fVec136[l136 as usize] = 0.0;
		}
		for l137 in 0..64 {
			self.fVec137[l137 as usize] = 0.0;
		}
		for l138 in 0..64 {
			self.fVec138[l138 as usize] = 0.0;
		}
		for l139 in 0..64 {
			self.fVec139[l139 as usize] = 0.0;
		}
		for l140 in 0..64 {
			self.fVec140[l140 as usize] = 0.0;
		}
		for l141 in 0..64 {
			self.fVec141[l141 as usize] = 0.0;
		}
		for l142 in 0..64 {
			self.fVec142[l142 as usize] = 0.0;
		}
		for l143 in 0..64 {
			self.fVec143[l143 as usize] = 0.0;
		}
		for l144 in 0..64 {
			self.fVec144[l144 as usize] = 0.0;
		}
		for l145 in 0..64 {
			self.fVec145[l145 as usize] = 0.0;
		}
		for l146 in 0..64 {
			self.fVec146[l146 as usize] = 0.0;
		}
		for l147 in 0..64 {
			self.fVec147[l147 as usize] = 0.0;
		}
		for l148 in 0..64 {
			self.fVec148[l148 as usize] = 0.0;
		}
		for l149 in 0..64 {
			self.fVec149[l149 as usize] = 0.0;
		}
		for l150 in 0..64 {
			self.fVec150[l150 as usize] = 0.0;
		}
		for l151 in 0..64 {
			self.fVec151[l151 as usize] = 0.0;
		}
		for l152 in 0..32 {
			self.fVec152[l152 as usize] = 0.0;
		}
		for l153 in 0..32 {
			self.fVec153[l153 as usize] = 0.0;
		}
		for l154 in 0..32 {
			self.fVec154[l154 as usize] = 0.0;
		}
		for l155 in 0..32 {
			self.fVec155[l155 as usize] = 0.0;
		}
		for l156 in 0..32 {
			self.fVec156[l156 as usize] = 0.0;
		}
		for l157 in 0..32 {
			self.fVec157[l157 as usize] = 0.0;
		}
		for l158 in 0..32 {
			self.fVec158[l158 as usize] = 0.0;
		}
		for l159 in 0..32 {
			self.fVec159[l159 as usize] = 0.0;
		}
		for l160 in 0..32 {
			self.fVec160[l160 as usize] = 0.0;
		}
		for l161 in 0..32 {
			self.fVec161[l161 as usize] = 0.0;
		}
		for l162 in 0..32 {
			self.fVec162[l162 as usize] = 0.0;
		}
		for l163 in 0..32 {
			self.fVec163[l163 as usize] = 0.0;
		}
		for l164 in 0..32 {
			self.fVec164[l164 as usize] = 0.0;
		}
		for l165 in 0..32 {
			self.fVec165[l165 as usize] = 0.0;
		}
		for l166 in 0..32 {
			self.fVec166[l166 as usize] = 0.0;
		}
		for l167 in 0..32 {
			self.fVec167[l167 as usize] = 0.0;
		}
		for l168 in 0..16 {
			self.fVec168[l168 as usize] = 0.0;
		}
		for l169 in 0..15 {
			self.fVec169[l169 as usize] = 0.0;
		}
		for l170 in 0..14 {
			self.fVec170[l170 as usize] = 0.0;
		}
		for l171 in 0..13 {
			self.fVec171[l171 as usize] = 0.0;
		}
		for l172 in 0..12 {
			self.fVec172[l172 as usize] = 0.0;
		}
		for l173 in 0..11 {
			self.fVec173[l173 as usize] = 0.0;
		}
		for l174 in 0..10 {
			self.fVec174[l174 as usize] = 0.0;
		}
		for l175 in 0..9 {
			self.fVec175[l175 as usize] = 0.0;
		}
		for l176 in 0..8 {
			self.fVec176[l176 as usize] = 0.0;
		}
		for l177 in 0..7 {
			self.fVec177[l177 as usize] = 0.0;
		}
		for l178 in 0..6 {
			self.fVec178[l178 as usize] = 0.0;
		}
		for l179 in 0..5 {
			self.fVec179[l179 as usize] = 0.0;
		}
		for l180 in 0..4 {
			self.fVec180[l180 as usize] = 0.0;
		}
		for l181 in 0..3 {
			self.fVec181[l181 as usize] = 0.0;
		}
		for l182 in 0..2 {
			self.fVec182[l182 as usize] = 0.0;
		}
		for l183 in 0..2 {
			self.fRec0[l183 as usize] = 0.0;
		}
		for l184 in 0..2 {
			self.fRec4[l184 as usize] = 0.0;
		}
		for l185 in 0..2 {
			self.fRec3[l185 as usize] = 0.0;
		}
		for l186 in 0..2 {
			self.fRec2[l186 as usize] = 0.0;
		}
		for l187 in 0..2 {
			self.fRec1[l187 as usize] = 0.0;
		}
		for l188 in 0..64 {
			self.fVec183[l188 as usize] = 0.0;
		}
		for l189 in 0..64 {
			self.fVec184[l189 as usize] = 0.0;
		}
		for l190 in 0..64 {
			self.fVec185[l190 as usize] = 0.0;
		}
		for l191 in 0..64 {
			self.fVec186[l191 as usize] = 0.0;
		}
		for l192 in 0..64 {
			self.fVec187[l192 as usize] = 0.0;
		}
		for l193 in 0..64 {
			self.fVec188[l193 as usize] = 0.0;
		}
		for l194 in 0..64 {
			self.fVec189[l194 as usize] = 0.0;
		}
		for l195 in 0..64 {
			self.fVec190[l195 as usize] = 0.0;
		}
		for l196 in 0..64 {
			self.fVec191[l196 as usize] = 0.0;
		}
		for l197 in 0..64 {
			self.fVec192[l197 as usize] = 0.0;
		}
		for l198 in 0..64 {
			self.fVec193[l198 as usize] = 0.0;
		}
		for l199 in 0..64 {
			self.fVec194[l199 as usize] = 0.0;
		}
		for l200 in 0..64 {
			self.fVec195[l200 as usize] = 0.0;
		}
		for l201 in 0..64 {
			self.fVec196[l201 as usize] = 0.0;
		}
		for l202 in 0..64 {
			self.fVec197[l202 as usize] = 0.0;
		}
		for l203 in 0..64 {
			self.fVec198[l203 as usize] = 0.0;
		}
		for l204 in 0..64 {
			self.fVec199[l204 as usize] = 0.0;
		}
		for l205 in 0..64 {
			self.fVec200[l205 as usize] = 0.0;
		}
		for l206 in 0..64 {
			self.fVec201[l206 as usize] = 0.0;
		}
		for l207 in 0..64 {
			self.fVec202[l207 as usize] = 0.0;
		}
		for l208 in 0..64 {
			self.fVec203[l208 as usize] = 0.0;
		}
		for l209 in 0..64 {
			self.fVec204[l209 as usize] = 0.0;
		}
		for l210 in 0..64 {
			self.fVec205[l210 as usize] = 0.0;
		}
		for l211 in 0..64 {
			self.fVec206[l211 as usize] = 0.0;
		}
		for l212 in 0..64 {
			self.fVec207[l212 as usize] = 0.0;
		}
		for l213 in 0..64 {
			self.fVec208[l213 as usize] = 0.0;
		}
		for l214 in 0..64 {
			self.fVec209[l214 as usize] = 0.0;
		}
		for l215 in 0..64 {
			self.fVec210[l215 as usize] = 0.0;
		}
		for l216 in 0..64 {
			self.fVec211[l216 as usize] = 0.0;
		}
		for l217 in 0..64 {
			self.fVec212[l217 as usize] = 0.0;
		}
		for l218 in 0..64 {
			self.fVec213[l218 as usize] = 0.0;
		}
		for l219 in 0..32 {
			self.fVec214[l219 as usize] = 0.0;
		}
		for l220 in 0..32 {
			self.fVec215[l220 as usize] = 0.0;
		}
		for l221 in 0..32 {
			self.fVec216[l221 as usize] = 0.0;
		}
		for l222 in 0..32 {
			self.fVec217[l222 as usize] = 0.0;
		}
		for l223 in 0..32 {
			self.fVec218[l223 as usize] = 0.0;
		}
		for l224 in 0..32 {
			self.fVec219[l224 as usize] = 0.0;
		}
		for l225 in 0..32 {
			self.fVec220[l225 as usize] = 0.0;
		}
		for l226 in 0..32 {
			self.fVec221[l226 as usize] = 0.0;
		}
		for l227 in 0..32 {
			self.fVec222[l227 as usize] = 0.0;
		}
		for l228 in 0..32 {
			self.fVec223[l228 as usize] = 0.0;
		}
		for l229 in 0..32 {
			self.fVec224[l229 as usize] = 0.0;
		}
		for l230 in 0..32 {
			self.fVec225[l230 as usize] = 0.0;
		}
		for l231 in 0..32 {
			self.fVec226[l231 as usize] = 0.0;
		}
		for l232 in 0..32 {
			self.fVec227[l232 as usize] = 0.0;
		}
		for l233 in 0..32 {
			self.fVec228[l233 as usize] = 0.0;
		}
		for l234 in 0..32 {
			self.fVec229[l234 as usize] = 0.0;
		}
		for l235 in 0..16 {
			self.fVec230[l235 as usize] = 0.0;
		}
		for l236 in 0..15 {
			self.fVec231[l236 as usize] = 0.0;
		}
		for l237 in 0..14 {
			self.fVec232[l237 as usize] = 0.0;
		}
		for l238 in 0..13 {
			self.fVec233[l238 as usize] = 0.0;
		}
		for l239 in 0..12 {
			self.fVec234[l239 as usize] = 0.0;
		}
		for l240 in 0..11 {
			self.fVec235[l240 as usize] = 0.0;
		}
		for l241 in 0..10 {
			self.fVec236[l241 as usize] = 0.0;
		}
		for l242 in 0..9 {
			self.fVec237[l242 as usize] = 0.0;
		}
		for l243 in 0..8 {
			self.fVec238[l243 as usize] = 0.0;
		}
		for l244 in 0..7 {
			self.fVec239[l244 as usize] = 0.0;
		}
		for l245 in 0..6 {
			self.fVec240[l245 as usize] = 0.0;
		}
		for l246 in 0..5 {
			self.fVec241[l246 as usize] = 0.0;
		}
		for l247 in 0..4 {
			self.fVec242[l247 as usize] = 0.0;
		}
		for l248 in 0..3 {
			self.fVec243[l248 as usize] = 0.0;
		}
		for l249 in 0..2 {
			self.fVec244[l249 as usize] = 0.0;
		}
		for l250 in 0..128 {
			self.fVec245[l250 as usize] = 0.0;
		}
		for l251 in 0..128 {
			self.fVec246[l251 as usize] = 0.0;
		}
		for l252 in 0..128 {
			self.fVec247[l252 as usize] = 0.0;
		}
		for l253 in 0..128 {
			self.fVec248[l253 as usize] = 0.0;
		}
		for l254 in 0..128 {
			self.fVec249[l254 as usize] = 0.0;
		}
		for l255 in 0..128 {
			self.fVec250[l255 as usize] = 0.0;
		}
		for l256 in 0..128 {
			self.fVec251[l256 as usize] = 0.0;
		}
		for l257 in 0..128 {
			self.fVec252[l257 as usize] = 0.0;
		}
		for l258 in 0..128 {
			self.fVec253[l258 as usize] = 0.0;
		}
		for l259 in 0..128 {
			self.fVec254[l259 as usize] = 0.0;
		}
		for l260 in 0..128 {
			self.fVec255[l260 as usize] = 0.0;
		}
		for l261 in 0..128 {
			self.fVec256[l261 as usize] = 0.0;
		}
		for l262 in 0..128 {
			self.fVec257[l262 as usize] = 0.0;
		}
		for l263 in 0..128 {
			self.fVec258[l263 as usize] = 0.0;
		}
		for l264 in 0..128 {
			self.fVec259[l264 as usize] = 0.0;
		}
		for l265 in 0..128 {
			self.fVec260[l265 as usize] = 0.0;
		}
		for l266 in 0..128 {
			self.fVec261[l266 as usize] = 0.0;
		}
		for l267 in 0..128 {
			self.fVec262[l267 as usize] = 0.0;
		}
		for l268 in 0..128 {
			self.fVec263[l268 as usize] = 0.0;
		}
		for l269 in 0..128 {
			self.fVec264[l269 as usize] = 0.0;
		}
		for l270 in 0..128 {
			self.fVec265[l270 as usize] = 0.0;
		}
		for l271 in 0..128 {
			self.fVec266[l271 as usize] = 0.0;
		}
		for l272 in 0..128 {
			self.fVec267[l272 as usize] = 0.0;
		}
		for l273 in 0..128 {
			self.fVec268[l273 as usize] = 0.0;
		}
		for l274 in 0..128 {
			self.fVec269[l274 as usize] = 0.0;
		}
		for l275 in 0..128 {
			self.fVec270[l275 as usize] = 0.0;
		}
		for l276 in 0..128 {
			self.fVec271[l276 as usize] = 0.0;
		}
		for l277 in 0..128 {
			self.fVec272[l277 as usize] = 0.0;
		}
		for l278 in 0..128 {
			self.fVec273[l278 as usize] = 0.0;
		}
		for l279 in 0..128 {
			self.fVec274[l279 as usize] = 0.0;
		}
		for l280 in 0..128 {
			self.fVec275[l280 as usize] = 0.0;
		}
		for l281 in 0..128 {
			self.fVec276[l281 as usize] = 0.0;
		}
		for l282 in 0..128 {
			self.fVec277[l282 as usize] = 0.0;
		}
		for l283 in 0..128 {
			self.fVec278[l283 as usize] = 0.0;
		}
		for l284 in 0..128 {
			self.fVec279[l284 as usize] = 0.0;
		}
		for l285 in 0..128 {
			self.fVec280[l285 as usize] = 0.0;
		}
		for l286 in 0..128 {
			self.fVec281[l286 as usize] = 0.0;
		}
		for l287 in 0..128 {
			self.fVec282[l287 as usize] = 0.0;
		}
		for l288 in 0..128 {
			self.fVec283[l288 as usize] = 0.0;
		}
		for l289 in 0..128 {
			self.fVec284[l289 as usize] = 0.0;
		}
		for l290 in 0..128 {
			self.fVec285[l290 as usize] = 0.0;
		}
		for l291 in 0..128 {
			self.fVec286[l291 as usize] = 0.0;
		}
		for l292 in 0..128 {
			self.fVec287[l292 as usize] = 0.0;
		}
		for l293 in 0..128 {
			self.fVec288[l293 as usize] = 0.0;
		}
		for l294 in 0..128 {
			self.fVec289[l294 as usize] = 0.0;
		}
		for l295 in 0..128 {
			self.fVec290[l295 as usize] = 0.0;
		}
		for l296 in 0..128 {
			self.fVec291[l296 as usize] = 0.0;
		}
		for l297 in 0..128 {
			self.fVec292[l297 as usize] = 0.0;
		}
		for l298 in 0..128 {
			self.fVec293[l298 as usize] = 0.0;
		}
		for l299 in 0..128 {
			self.fVec294[l299 as usize] = 0.0;
		}
		for l300 in 0..128 {
			self.fVec295[l300 as usize] = 0.0;
		}
		for l301 in 0..128 {
			self.fVec296[l301 as usize] = 0.0;
		}
		for l302 in 0..128 {
			self.fVec297[l302 as usize] = 0.0;
		}
		for l303 in 0..128 {
			self.fVec298[l303 as usize] = 0.0;
		}
		for l304 in 0..128 {
			self.fVec299[l304 as usize] = 0.0;
		}
		for l305 in 0..128 {
			self.fVec300[l305 as usize] = 0.0;
		}
		for l306 in 0..128 {
			self.fVec301[l306 as usize] = 0.0;
		}
		for l307 in 0..64 {
			self.fVec302[l307 as usize] = 0.0;
		}
		for l308 in 0..64 {
			self.fVec303[l308 as usize] = 0.0;
		}
		for l309 in 0..64 {
			self.fVec304[l309 as usize] = 0.0;
		}
		for l310 in 0..64 {
			self.fVec305[l310 as usize] = 0.0;
		}
		for l311 in 0..64 {
			self.fVec306[l311 as usize] = 0.0;
		}
		for l312 in 0..64 {
			self.fVec307[l312 as usize] = 0.0;
		}
		for l313 in 0..64 {
			self.fVec308[l313 as usize] = 0.0;
		}
		for l314 in 0..64 {
			self.fVec309[l314 as usize] = 0.0;
		}
		for l315 in 0..64 {
			self.fVec310[l315 as usize] = 0.0;
		}
		for l316 in 0..64 {
			self.fVec311[l316 as usize] = 0.0;
		}
		for l317 in 0..64 {
			self.fVec312[l317 as usize] = 0.0;
		}
		for l318 in 0..64 {
			self.fVec313[l318 as usize] = 0.0;
		}
		for l319 in 0..64 {
			self.fVec314[l319 as usize] = 0.0;
		}
		for l320 in 0..64 {
			self.fVec315[l320 as usize] = 0.0;
		}
		for l321 in 0..64 {
			self.fVec316[l321 as usize] = 0.0;
		}
		for l322 in 0..64 {
			self.fVec317[l322 as usize] = 0.0;
		}
		for l323 in 0..64 {
			self.fVec318[l323 as usize] = 0.0;
		}
		for l324 in 0..64 {
			self.fVec319[l324 as usize] = 0.0;
		}
		for l325 in 0..64 {
			self.fVec320[l325 as usize] = 0.0;
		}
		for l326 in 0..64 {
			self.fVec321[l326 as usize] = 0.0;
		}
		for l327 in 0..64 {
			self.fVec322[l327 as usize] = 0.0;
		}
		for l328 in 0..64 {
			self.fVec323[l328 as usize] = 0.0;
		}
		for l329 in 0..64 {
			self.fVec324[l329 as usize] = 0.0;
		}
		for l330 in 0..64 {
			self.fVec325[l330 as usize] = 0.0;
		}
		for l331 in 0..64 {
			self.fVec326[l331 as usize] = 0.0;
		}
		for l332 in 0..64 {
			self.fVec327[l332 as usize] = 0.0;
		}
		for l333 in 0..64 {
			self.fVec328[l333 as usize] = 0.0;
		}
		for l334 in 0..64 {
			self.fVec329[l334 as usize] = 0.0;
		}
		for l335 in 0..64 {
			self.fVec330[l335 as usize] = 0.0;
		}
		for l336 in 0..64 {
			self.fVec331[l336 as usize] = 0.0;
		}
		for l337 in 0..64 {
			self.fVec332[l337 as usize] = 0.0;
		}
		for l338 in 0..64 {
			self.fVec333[l338 as usize] = 0.0;
		}
		for l339 in 0..32 {
			self.fVec334[l339 as usize] = 0.0;
		}
		for l340 in 0..32 {
			self.fVec335[l340 as usize] = 0.0;
		}
		for l341 in 0..32 {
			self.fVec336[l341 as usize] = 0.0;
		}
		for l342 in 0..32 {
			self.fVec337[l342 as usize] = 0.0;
		}
		for l343 in 0..32 {
			self.fVec338[l343 as usize] = 0.0;
		}
		for l344 in 0..32 {
			self.fVec339[l344 as usize] = 0.0;
		}
		for l345 in 0..32 {
			self.fVec340[l345 as usize] = 0.0;
		}
		for l346 in 0..32 {
			self.fVec341[l346 as usize] = 0.0;
		}
		for l347 in 0..32 {
			self.fVec342[l347 as usize] = 0.0;
		}
		for l348 in 0..32 {
			self.fVec343[l348 as usize] = 0.0;
		}
		for l349 in 0..32 {
			self.fVec344[l349 as usize] = 0.0;
		}
		for l350 in 0..32 {
			self.fVec345[l350 as usize] = 0.0;
		}
		for l351 in 0..32 {
			self.fVec346[l351 as usize] = 0.0;
		}
		for l352 in 0..32 {
			self.fVec347[l352 as usize] = 0.0;
		}
		for l353 in 0..32 {
			self.fVec348[l353 as usize] = 0.0;
		}
		for l354 in 0..32 {
			self.fVec349[l354 as usize] = 0.0;
		}
		for l355 in 0..16 {
			self.fVec350[l355 as usize] = 0.0;
		}
		for l356 in 0..15 {
			self.fVec351[l356 as usize] = 0.0;
		}
		for l357 in 0..14 {
			self.fVec352[l357 as usize] = 0.0;
		}
		for l358 in 0..13 {
			self.fVec353[l358 as usize] = 0.0;
		}
		for l359 in 0..12 {
			self.fVec354[l359 as usize] = 0.0;
		}
		for l360 in 0..11 {
			self.fVec355[l360 as usize] = 0.0;
		}
		for l361 in 0..10 {
			self.fVec356[l361 as usize] = 0.0;
		}
		for l362 in 0..9 {
			self.fVec357[l362 as usize] = 0.0;
		}
		for l363 in 0..8 {
			self.fVec358[l363 as usize] = 0.0;
		}
		for l364 in 0..7 {
			self.fVec359[l364 as usize] = 0.0;
		}
		for l365 in 0..6 {
			self.fVec360[l365 as usize] = 0.0;
		}
		for l366 in 0..5 {
			self.fVec361[l366 as usize] = 0.0;
		}
		for l367 in 0..4 {
			self.fVec362[l367 as usize] = 0.0;
		}
		for l368 in 0..3 {
			self.fVec363[l368 as usize] = 0.0;
		}
		for l369 in 0..2 {
			self.fVec364[l369 as usize] = 0.0;
		}
		for l370 in 0..2 {
			self.fRec5[l370 as usize] = 0.0;
		}
		for l371 in 0..2 {
			self.fRec6[l371 as usize] = 0.0;
		}
		for l372 in 0..64 {
			self.fVec365[l372 as usize] = 0.0;
		}
		for l373 in 0..64 {
			self.fVec366[l373 as usize] = 0.0;
		}
		for l374 in 0..64 {
			self.fVec367[l374 as usize] = 0.0;
		}
		for l375 in 0..64 {
			self.fVec368[l375 as usize] = 0.0;
		}
		for l376 in 0..64 {
			self.fVec369[l376 as usize] = 0.0;
		}
		for l377 in 0..64 {
			self.fVec370[l377 as usize] = 0.0;
		}
		for l378 in 0..64 {
			self.fVec371[l378 as usize] = 0.0;
		}
		for l379 in 0..64 {
			self.fVec372[l379 as usize] = 0.0;
		}
		for l380 in 0..64 {
			self.fVec373[l380 as usize] = 0.0;
		}
		for l381 in 0..64 {
			self.fVec374[l381 as usize] = 0.0;
		}
		for l382 in 0..64 {
			self.fVec375[l382 as usize] = 0.0;
		}
		for l383 in 0..64 {
			self.fVec376[l383 as usize] = 0.0;
		}
		for l384 in 0..64 {
			self.fVec377[l384 as usize] = 0.0;
		}
		for l385 in 0..64 {
			self.fVec378[l385 as usize] = 0.0;
		}
		for l386 in 0..64 {
			self.fVec379[l386 as usize] = 0.0;
		}
		for l387 in 0..64 {
			self.fVec380[l387 as usize] = 0.0;
		}
		for l388 in 0..64 {
			self.fVec381[l388 as usize] = 0.0;
		}
		for l389 in 0..64 {
			self.fVec382[l389 as usize] = 0.0;
		}
		for l390 in 0..64 {
			self.fVec383[l390 as usize] = 0.0;
		}
		for l391 in 0..64 {
			self.fVec384[l391 as usize] = 0.0;
		}
		for l392 in 0..64 {
			self.fVec385[l392 as usize] = 0.0;
		}
		for l393 in 0..64 {
			self.fVec386[l393 as usize] = 0.0;
		}
		for l394 in 0..64 {
			self.fVec387[l394 as usize] = 0.0;
		}
		for l395 in 0..64 {
			self.fVec388[l395 as usize] = 0.0;
		}
		for l396 in 0..64 {
			self.fVec389[l396 as usize] = 0.0;
		}
		for l397 in 0..64 {
			self.fVec390[l397 as usize] = 0.0;
		}
		for l398 in 0..64 {
			self.fVec391[l398 as usize] = 0.0;
		}
		for l399 in 0..64 {
			self.fVec392[l399 as usize] = 0.0;
		}
		for l400 in 0..64 {
			self.fVec393[l400 as usize] = 0.0;
		}
		for l401 in 0..64 {
			self.fVec394[l401 as usize] = 0.0;
		}
		for l402 in 0..64 {
			self.fVec395[l402 as usize] = 0.0;
		}
		for l403 in 0..32 {
			self.fVec396[l403 as usize] = 0.0;
		}
		for l404 in 0..32 {
			self.fVec397[l404 as usize] = 0.0;
		}
		for l405 in 0..32 {
			self.fVec398[l405 as usize] = 0.0;
		}
		for l406 in 0..32 {
			self.fVec399[l406 as usize] = 0.0;
		}
		for l407 in 0..32 {
			self.fVec400[l407 as usize] = 0.0;
		}
		for l408 in 0..32 {
			self.fVec401[l408 as usize] = 0.0;
		}
		for l409 in 0..32 {
			self.fVec402[l409 as usize] = 0.0;
		}
		for l410 in 0..32 {
			self.fVec403[l410 as usize] = 0.0;
		}
		for l411 in 0..32 {
			self.fVec404[l411 as usize] = 0.0;
		}
		for l412 in 0..32 {
			self.fVec405[l412 as usize] = 0.0;
		}
		for l413 in 0..32 {
			self.fVec406[l413 as usize] = 0.0;
		}
		for l414 in 0..32 {
			self.fVec407[l414 as usize] = 0.0;
		}
		for l415 in 0..32 {
			self.fVec408[l415 as usize] = 0.0;
		}
		for l416 in 0..32 {
			self.fVec409[l416 as usize] = 0.0;
		}
		for l417 in 0..32 {
			self.fVec410[l417 as usize] = 0.0;
		}
		for l418 in 0..32 {
			self.fVec411[l418 as usize] = 0.0;
		}
		for l419 in 0..16 {
			self.fVec412[l419 as usize] = 0.0;
		}
		for l420 in 0..15 {
			self.fVec413[l420 as usize] = 0.0;
		}
		for l421 in 0..14 {
			self.fVec414[l421 as usize] = 0.0;
		}
		for l422 in 0..13 {
			self.fVec415[l422 as usize] = 0.0;
		}
		for l423 in 0..12 {
			self.fVec416[l423 as usize] = 0.0;
		}
		for l424 in 0..11 {
			self.fVec417[l424 as usize] = 0.0;
		}
		for l425 in 0..10 {
			self.fVec418[l425 as usize] = 0.0;
		}
		for l426 in 0..9 {
			self.fVec419[l426 as usize] = 0.0;
		}
		for l427 in 0..8 {
			self.fVec420[l427 as usize] = 0.0;
		}
		for l428 in 0..7 {
			self.fVec421[l428 as usize] = 0.0;
		}
		for l429 in 0..6 {
			self.fVec422[l429 as usize] = 0.0;
		}
		for l430 in 0..5 {
			self.fVec423[l430 as usize] = 0.0;
		}
		for l431 in 0..4 {
			self.fVec424[l431 as usize] = 0.0;
		}
		for l432 in 0..3 {
			self.fVec425[l432 as usize] = 0.0;
		}
		for l433 in 0..2 {
			self.fVec426[l433 as usize] = 0.0;
		}
		for l434 in 0..128 {
			self.fVec427[l434 as usize] = 0.0;
		}
		for l435 in 0..128 {
			self.fVec428[l435 as usize] = 0.0;
		}
		for l436 in 0..128 {
			self.fVec429[l436 as usize] = 0.0;
		}
		for l437 in 0..128 {
			self.fVec430[l437 as usize] = 0.0;
		}
		for l438 in 0..128 {
			self.fVec431[l438 as usize] = 0.0;
		}
		for l439 in 0..128 {
			self.fVec432[l439 as usize] = 0.0;
		}
		for l440 in 0..128 {
			self.fVec433[l440 as usize] = 0.0;
		}
		for l441 in 0..128 {
			self.fVec434[l441 as usize] = 0.0;
		}
		for l442 in 0..128 {
			self.fVec435[l442 as usize] = 0.0;
		}
		for l443 in 0..128 {
			self.fVec436[l443 as usize] = 0.0;
		}
		for l444 in 0..128 {
			self.fVec437[l444 as usize] = 0.0;
		}
		for l445 in 0..128 {
			self.fVec438[l445 as usize] = 0.0;
		}
		for l446 in 0..128 {
			self.fVec439[l446 as usize] = 0.0;
		}
		for l447 in 0..128 {
			self.fVec440[l447 as usize] = 0.0;
		}
		for l448 in 0..128 {
			self.fVec441[l448 as usize] = 0.0;
		}
		for l449 in 0..128 {
			self.fVec442[l449 as usize] = 0.0;
		}
		for l450 in 0..128 {
			self.fVec443[l450 as usize] = 0.0;
		}
		for l451 in 0..128 {
			self.fVec444[l451 as usize] = 0.0;
		}
		for l452 in 0..128 {
			self.fVec445[l452 as usize] = 0.0;
		}
		for l453 in 0..128 {
			self.fVec446[l453 as usize] = 0.0;
		}
		for l454 in 0..128 {
			self.fVec447[l454 as usize] = 0.0;
		}
		for l455 in 0..128 {
			self.fVec448[l455 as usize] = 0.0;
		}
		for l456 in 0..128 {
			self.fVec449[l456 as usize] = 0.0;
		}
		for l457 in 0..128 {
			self.fVec450[l457 as usize] = 0.0;
		}
		for l458 in 0..128 {
			self.fVec451[l458 as usize] = 0.0;
		}
		for l459 in 0..128 {
			self.fVec452[l459 as usize] = 0.0;
		}
		for l460 in 0..128 {
			self.fVec453[l460 as usize] = 0.0;
		}
		for l461 in 0..128 {
			self.fVec454[l461 as usize] = 0.0;
		}
		for l462 in 0..128 {
			self.fVec455[l462 as usize] = 0.0;
		}
		for l463 in 0..128 {
			self.fVec456[l463 as usize] = 0.0;
		}
		for l464 in 0..128 {
			self.fVec457[l464 as usize] = 0.0;
		}
		for l465 in 0..128 {
			self.fVec458[l465 as usize] = 0.0;
		}
		for l466 in 0..128 {
			self.fVec459[l466 as usize] = 0.0;
		}
		for l467 in 0..128 {
			self.fVec460[l467 as usize] = 0.0;
		}
		for l468 in 0..128 {
			self.fVec461[l468 as usize] = 0.0;
		}
		for l469 in 0..128 {
			self.fVec462[l469 as usize] = 0.0;
		}
		for l470 in 0..128 {
			self.fVec463[l470 as usize] = 0.0;
		}
		for l471 in 0..128 {
			self.fVec464[l471 as usize] = 0.0;
		}
		for l472 in 0..128 {
			self.fVec465[l472 as usize] = 0.0;
		}
		for l473 in 0..128 {
			self.fVec466[l473 as usize] = 0.0;
		}
		for l474 in 0..128 {
			self.fVec467[l474 as usize] = 0.0;
		}
		for l475 in 0..128 {
			self.fVec468[l475 as usize] = 0.0;
		}
		for l476 in 0..128 {
			self.fVec469[l476 as usize] = 0.0;
		}
		for l477 in 0..128 {
			self.fVec470[l477 as usize] = 0.0;
		}
		for l478 in 0..128 {
			self.fVec471[l478 as usize] = 0.0;
		}
		for l479 in 0..128 {
			self.fVec472[l479 as usize] = 0.0;
		}
		for l480 in 0..128 {
			self.fVec473[l480 as usize] = 0.0;
		}
		for l481 in 0..128 {
			self.fVec474[l481 as usize] = 0.0;
		}
		for l482 in 0..128 {
			self.fVec475[l482 as usize] = 0.0;
		}
		for l483 in 0..128 {
			self.fVec476[l483 as usize] = 0.0;
		}
		for l484 in 0..128 {
			self.fVec477[l484 as usize] = 0.0;
		}
		for l485 in 0..128 {
			self.fVec478[l485 as usize] = 0.0;
		}
		for l486 in 0..128 {
			self.fVec479[l486 as usize] = 0.0;
		}
		for l487 in 0..128 {
			self.fVec480[l487 as usize] = 0.0;
		}
		for l488 in 0..128 {
			self.fVec481[l488 as usize] = 0.0;
		}
		for l489 in 0..128 {
			self.fVec482[l489 as usize] = 0.0;
		}
		for l490 in 0..128 {
			self.fVec483[l490 as usize] = 0.0;
		}
		for l491 in 0..64 {
			self.fVec484[l491 as usize] = 0.0;
		}
		for l492 in 0..64 {
			self.fVec485[l492 as usize] = 0.0;
		}
		for l493 in 0..64 {
			self.fVec486[l493 as usize] = 0.0;
		}
		for l494 in 0..64 {
			self.fVec487[l494 as usize] = 0.0;
		}
		for l495 in 0..64 {
			self.fVec488[l495 as usize] = 0.0;
		}
		for l496 in 0..64 {
			self.fVec489[l496 as usize] = 0.0;
		}
		for l497 in 0..64 {
			self.fVec490[l497 as usize] = 0.0;
		}
		for l498 in 0..64 {
			self.fVec491[l498 as usize] = 0.0;
		}
		for l499 in 0..64 {
			self.fVec492[l499 as usize] = 0.0;
		}
		for l500 in 0..64 {
			self.fVec493[l500 as usize] = 0.0;
		}
		for l501 in 0..64 {
			self.fVec494[l501 as usize] = 0.0;
		}
		for l502 in 0..64 {
			self.fVec495[l502 as usize] = 0.0;
		}
		for l503 in 0..64 {
			self.fVec496[l503 as usize] = 0.0;
		}
		for l504 in 0..64 {
			self.fVec497[l504 as usize] = 0.0;
		}
		for l505 in 0..64 {
			self.fVec498[l505 as usize] = 0.0;
		}
		for l506 in 0..64 {
			self.fVec499[l506 as usize] = 0.0;
		}
		for l507 in 0..64 {
			self.fVec500[l507 as usize] = 0.0;
		}
		for l508 in 0..64 {
			self.fVec501[l508 as usize] = 0.0;
		}
		for l509 in 0..64 {
			self.fVec502[l509 as usize] = 0.0;
		}
		for l510 in 0..64 {
			self.fVec503[l510 as usize] = 0.0;
		}
		for l511 in 0..64 {
			self.fVec504[l511 as usize] = 0.0;
		}
		for l512 in 0..64 {
			self.fVec505[l512 as usize] = 0.0;
		}
		for l513 in 0..64 {
			self.fVec506[l513 as usize] = 0.0;
		}
		for l514 in 0..64 {
			self.fVec507[l514 as usize] = 0.0;
		}
		for l515 in 0..64 {
			self.fVec508[l515 as usize] = 0.0;
		}
		for l516 in 0..64 {
			self.fVec509[l516 as usize] = 0.0;
		}
		for l517 in 0..64 {
			self.fVec510[l517 as usize] = 0.0;
		}
		for l518 in 0..64 {
			self.fVec511[l518 as usize] = 0.0;
		}
		for l519 in 0..64 {
			self.fVec512[l519 as usize] = 0.0;
		}
		for l520 in 0..64 {
			self.fVec513[l520 as usize] = 0.0;
		}
		for l521 in 0..64 {
			self.fVec514[l521 as usize] = 0.0;
		}
		for l522 in 0..64 {
			self.fVec515[l522 as usize] = 0.0;
		}
		for l523 in 0..32 {
			self.fVec516[l523 as usize] = 0.0;
		}
		for l524 in 0..32 {
			self.fVec517[l524 as usize] = 0.0;
		}
		for l525 in 0..32 {
			self.fVec518[l525 as usize] = 0.0;
		}
		for l526 in 0..32 {
			self.fVec519[l526 as usize] = 0.0;
		}
		for l527 in 0..32 {
			self.fVec520[l527 as usize] = 0.0;
		}
		for l528 in 0..32 {
			self.fVec521[l528 as usize] = 0.0;
		}
		for l529 in 0..32 {
			self.fVec522[l529 as usize] = 0.0;
		}
		for l530 in 0..32 {
			self.fVec523[l530 as usize] = 0.0;
		}
		for l531 in 0..32 {
			self.fVec524[l531 as usize] = 0.0;
		}
		for l532 in 0..32 {
			self.fVec525[l532 as usize] = 0.0;
		}
		for l533 in 0..32 {
			self.fVec526[l533 as usize] = 0.0;
		}
		for l534 in 0..32 {
			self.fVec527[l534 as usize] = 0.0;
		}
		for l535 in 0..32 {
			self.fVec528[l535 as usize] = 0.0;
		}
		for l536 in 0..32 {
			self.fVec529[l536 as usize] = 0.0;
		}
		for l537 in 0..32 {
			self.fVec530[l537 as usize] = 0.0;
		}
		for l538 in 0..32 {
			self.fVec531[l538 as usize] = 0.0;
		}
		for l539 in 0..16 {
			self.fVec532[l539 as usize] = 0.0;
		}
		for l540 in 0..15 {
			self.fVec533[l540 as usize] = 0.0;
		}
		for l541 in 0..14 {
			self.fVec534[l541 as usize] = 0.0;
		}
		for l542 in 0..13 {
			self.fVec535[l542 as usize] = 0.0;
		}
		for l543 in 0..12 {
			self.fVec536[l543 as usize] = 0.0;
		}
		for l544 in 0..11 {
			self.fVec537[l544 as usize] = 0.0;
		}
		for l545 in 0..10 {
			self.fVec538[l545 as usize] = 0.0;
		}
		for l546 in 0..9 {
			self.fVec539[l546 as usize] = 0.0;
		}
		for l547 in 0..8 {
			self.fVec540[l547 as usize] = 0.0;
		}
		for l548 in 0..7 {
			self.fVec541[l548 as usize] = 0.0;
		}
		for l549 in 0..6 {
			self.fVec542[l549 as usize] = 0.0;
		}
		for l550 in 0..5 {
			self.fVec543[l550 as usize] = 0.0;
		}
		for l551 in 0..4 {
			self.fVec544[l551 as usize] = 0.0;
		}
		for l552 in 0..3 {
			self.fVec545[l552 as usize] = 0.0;
		}
		for l553 in 0..2 {
			self.fVec546[l553 as usize] = 0.0;
		}
		for l554 in 0..64 {
			self.fVec547[l554 as usize] = 0.0;
		}
		for l555 in 0..64 {
			self.fVec548[l555 as usize] = 0.0;
		}
		for l556 in 0..64 {
			self.fVec549[l556 as usize] = 0.0;
		}
		for l557 in 0..64 {
			self.fVec550[l557 as usize] = 0.0;
		}
		for l558 in 0..64 {
			self.fVec551[l558 as usize] = 0.0;
		}
		for l559 in 0..64 {
			self.fVec552[l559 as usize] = 0.0;
		}
		for l560 in 0..64 {
			self.fVec553[l560 as usize] = 0.0;
		}
		for l561 in 0..64 {
			self.fVec554[l561 as usize] = 0.0;
		}
		for l562 in 0..64 {
			self.fVec555[l562 as usize] = 0.0;
		}
		for l563 in 0..64 {
			self.fVec556[l563 as usize] = 0.0;
		}
		for l564 in 0..64 {
			self.fVec557[l564 as usize] = 0.0;
		}
		for l565 in 0..64 {
			self.fVec558[l565 as usize] = 0.0;
		}
		for l566 in 0..64 {
			self.fVec559[l566 as usize] = 0.0;
		}
		for l567 in 0..64 {
			self.fVec560[l567 as usize] = 0.0;
		}
		for l568 in 0..64 {
			self.fVec561[l568 as usize] = 0.0;
		}
		for l569 in 0..64 {
			self.fVec562[l569 as usize] = 0.0;
		}
		for l570 in 0..64 {
			self.fVec563[l570 as usize] = 0.0;
		}
		for l571 in 0..64 {
			self.fVec564[l571 as usize] = 0.0;
		}
		for l572 in 0..64 {
			self.fVec565[l572 as usize] = 0.0;
		}
		for l573 in 0..64 {
			self.fVec566[l573 as usize] = 0.0;
		}
		for l574 in 0..64 {
			self.fVec567[l574 as usize] = 0.0;
		}
		for l575 in 0..64 {
			self.fVec568[l575 as usize] = 0.0;
		}
		for l576 in 0..64 {
			self.fVec569[l576 as usize] = 0.0;
		}
		for l577 in 0..64 {
			self.fVec570[l577 as usize] = 0.0;
		}
		for l578 in 0..64 {
			self.fVec571[l578 as usize] = 0.0;
		}
		for l579 in 0..64 {
			self.fVec572[l579 as usize] = 0.0;
		}
		for l580 in 0..64 {
			self.fVec573[l580 as usize] = 0.0;
		}
		for l581 in 0..64 {
			self.fVec574[l581 as usize] = 0.0;
		}
		for l582 in 0..64 {
			self.fVec575[l582 as usize] = 0.0;
		}
		for l583 in 0..64 {
			self.fVec576[l583 as usize] = 0.0;
		}
		for l584 in 0..64 {
			self.fVec577[l584 as usize] = 0.0;
		}
		for l585 in 0..32 {
			self.fVec578[l585 as usize] = 0.0;
		}
		for l586 in 0..32 {
			self.fVec579[l586 as usize] = 0.0;
		}
		for l587 in 0..32 {
			self.fVec580[l587 as usize] = 0.0;
		}
		for l588 in 0..32 {
			self.fVec581[l588 as usize] = 0.0;
		}
		for l589 in 0..32 {
			self.fVec582[l589 as usize] = 0.0;
		}
		for l590 in 0..32 {
			self.fVec583[l590 as usize] = 0.0;
		}
		for l591 in 0..32 {
			self.fVec584[l591 as usize] = 0.0;
		}
		for l592 in 0..32 {
			self.fVec585[l592 as usize] = 0.0;
		}
		for l593 in 0..32 {
			self.fVec586[l593 as usize] = 0.0;
		}
		for l594 in 0..32 {
			self.fVec587[l594 as usize] = 0.0;
		}
		for l595 in 0..32 {
			self.fVec588[l595 as usize] = 0.0;
		}
		for l596 in 0..32 {
			self.fVec589[l596 as usize] = 0.0;
		}
		for l597 in 0..32 {
			self.fVec590[l597 as usize] = 0.0;
		}
		for l598 in 0..32 {
			self.fVec591[l598 as usize] = 0.0;
		}
		for l599 in 0..32 {
			self.fVec592[l599 as usize] = 0.0;
		}
		for l600 in 0..32 {
			self.fVec593[l600 as usize] = 0.0;
		}
		for l601 in 0..16 {
			self.fVec594[l601 as usize] = 0.0;
		}
		for l602 in 0..15 {
			self.fVec595[l602 as usize] = 0.0;
		}
		for l603 in 0..14 {
			self.fVec596[l603 as usize] = 0.0;
		}
		for l604 in 0..13 {
			self.fVec597[l604 as usize] = 0.0;
		}
		for l605 in 0..12 {
			self.fVec598[l605 as usize] = 0.0;
		}
		for l606 in 0..11 {
			self.fVec599[l606 as usize] = 0.0;
		}
		for l607 in 0..10 {
			self.fVec600[l607 as usize] = 0.0;
		}
		for l608 in 0..9 {
			self.fVec601[l608 as usize] = 0.0;
		}
		for l609 in 0..8 {
			self.fVec602[l609 as usize] = 0.0;
		}
		for l610 in 0..7 {
			self.fVec603[l610 as usize] = 0.0;
		}
		for l611 in 0..6 {
			self.fVec604[l611 as usize] = 0.0;
		}
		for l612 in 0..5 {
			self.fVec605[l612 as usize] = 0.0;
		}
		for l613 in 0..4 {
			self.fVec606[l613 as usize] = 0.0;
		}
		for l614 in 0..3 {
			self.fVec607[l614 as usize] = 0.0;
		}
		for l615 in 0..2 {
			self.fVec608[l615 as usize] = 0.0;
		}
		for l616 in 0..128 {
			self.fVec609[l616 as usize] = 0.0;
		}
		for l617 in 0..128 {
			self.fVec610[l617 as usize] = 0.0;
		}
		for l618 in 0..128 {
			self.fVec611[l618 as usize] = 0.0;
		}
		for l619 in 0..128 {
			self.fVec612[l619 as usize] = 0.0;
		}
		for l620 in 0..128 {
			self.fVec613[l620 as usize] = 0.0;
		}
		for l621 in 0..128 {
			self.fVec614[l621 as usize] = 0.0;
		}
		for l622 in 0..128 {
			self.fVec615[l622 as usize] = 0.0;
		}
		for l623 in 0..128 {
			self.fVec616[l623 as usize] = 0.0;
		}
		for l624 in 0..128 {
			self.fVec617[l624 as usize] = 0.0;
		}
		for l625 in 0..128 {
			self.fVec618[l625 as usize] = 0.0;
		}
		for l626 in 0..128 {
			self.fVec619[l626 as usize] = 0.0;
		}
		for l627 in 0..128 {
			self.fVec620[l627 as usize] = 0.0;
		}
		for l628 in 0..128 {
			self.fVec621[l628 as usize] = 0.0;
		}
		for l629 in 0..128 {
			self.fVec622[l629 as usize] = 0.0;
		}
		for l630 in 0..128 {
			self.fVec623[l630 as usize] = 0.0;
		}
		for l631 in 0..128 {
			self.fVec624[l631 as usize] = 0.0;
		}
		for l632 in 0..128 {
			self.fVec625[l632 as usize] = 0.0;
		}
		for l633 in 0..128 {
			self.fVec626[l633 as usize] = 0.0;
		}
		for l634 in 0..128 {
			self.fVec627[l634 as usize] = 0.0;
		}
		for l635 in 0..128 {
			self.fVec628[l635 as usize] = 0.0;
		}
		for l636 in 0..128 {
			self.fVec629[l636 as usize] = 0.0;
		}
		for l637 in 0..128 {
			self.fVec630[l637 as usize] = 0.0;
		}
		for l638 in 0..128 {
			self.fVec631[l638 as usize] = 0.0;
		}
		for l639 in 0..128 {
			self.fVec632[l639 as usize] = 0.0;
		}
		for l640 in 0..128 {
			self.fVec633[l640 as usize] = 0.0;
		}
		for l641 in 0..128 {
			self.fVec634[l641 as usize] = 0.0;
		}
		for l642 in 0..128 {
			self.fVec635[l642 as usize] = 0.0;
		}
		for l643 in 0..128 {
			self.fVec636[l643 as usize] = 0.0;
		}
		for l644 in 0..128 {
			self.fVec637[l644 as usize] = 0.0;
		}
		for l645 in 0..128 {
			self.fVec638[l645 as usize] = 0.0;
		}
		for l646 in 0..128 {
			self.fVec639[l646 as usize] = 0.0;
		}
		for l647 in 0..128 {
			self.fVec640[l647 as usize] = 0.0;
		}
		for l648 in 0..128 {
			self.fVec641[l648 as usize] = 0.0;
		}
		for l649 in 0..128 {
			self.fVec642[l649 as usize] = 0.0;
		}
		for l650 in 0..128 {
			self.fVec643[l650 as usize] = 0.0;
		}
		for l651 in 0..128 {
			self.fVec644[l651 as usize] = 0.0;
		}
		for l652 in 0..128 {
			self.fVec645[l652 as usize] = 0.0;
		}
		for l653 in 0..128 {
			self.fVec646[l653 as usize] = 0.0;
		}
		for l654 in 0..128 {
			self.fVec647[l654 as usize] = 0.0;
		}
		for l655 in 0..128 {
			self.fVec648[l655 as usize] = 0.0;
		}
		for l656 in 0..128 {
			self.fVec649[l656 as usize] = 0.0;
		}
		for l657 in 0..128 {
			self.fVec650[l657 as usize] = 0.0;
		}
		for l658 in 0..128 {
			self.fVec651[l658 as usize] = 0.0;
		}
		for l659 in 0..128 {
			self.fVec652[l659 as usize] = 0.0;
		}
		for l660 in 0..128 {
			self.fVec653[l660 as usize] = 0.0;
		}
		for l661 in 0..128 {
			self.fVec654[l661 as usize] = 0.0;
		}
		for l662 in 0..128 {
			self.fVec655[l662 as usize] = 0.0;
		}
		for l663 in 0..128 {
			self.fVec656[l663 as usize] = 0.0;
		}
		for l664 in 0..128 {
			self.fVec657[l664 as usize] = 0.0;
		}
		for l665 in 0..128 {
			self.fVec658[l665 as usize] = 0.0;
		}
		for l666 in 0..128 {
			self.fVec659[l666 as usize] = 0.0;
		}
		for l667 in 0..128 {
			self.fVec660[l667 as usize] = 0.0;
		}
		for l668 in 0..128 {
			self.fVec661[l668 as usize] = 0.0;
		}
		for l669 in 0..128 {
			self.fVec662[l669 as usize] = 0.0;
		}
		for l670 in 0..128 {
			self.fVec663[l670 as usize] = 0.0;
		}
		for l671 in 0..128 {
			self.fVec664[l671 as usize] = 0.0;
		}
		for l672 in 0..128 {
			self.fVec665[l672 as usize] = 0.0;
		}
		for l673 in 0..64 {
			self.fVec666[l673 as usize] = 0.0;
		}
		for l674 in 0..64 {
			self.fVec667[l674 as usize] = 0.0;
		}
		for l675 in 0..64 {
			self.fVec668[l675 as usize] = 0.0;
		}
		for l676 in 0..64 {
			self.fVec669[l676 as usize] = 0.0;
		}
		for l677 in 0..64 {
			self.fVec670[l677 as usize] = 0.0;
		}
		for l678 in 0..64 {
			self.fVec671[l678 as usize] = 0.0;
		}
		for l679 in 0..64 {
			self.fVec672[l679 as usize] = 0.0;
		}
		for l680 in 0..64 {
			self.fVec673[l680 as usize] = 0.0;
		}
		for l681 in 0..64 {
			self.fVec674[l681 as usize] = 0.0;
		}
		for l682 in 0..64 {
			self.fVec675[l682 as usize] = 0.0;
		}
		for l683 in 0..64 {
			self.fVec676[l683 as usize] = 0.0;
		}
		for l684 in 0..64 {
			self.fVec677[l684 as usize] = 0.0;
		}
		for l685 in 0..64 {
			self.fVec678[l685 as usize] = 0.0;
		}
		for l686 in 0..64 {
			self.fVec679[l686 as usize] = 0.0;
		}
		for l687 in 0..64 {
			self.fVec680[l687 as usize] = 0.0;
		}
		for l688 in 0..64 {
			self.fVec681[l688 as usize] = 0.0;
		}
		for l689 in 0..64 {
			self.fVec682[l689 as usize] = 0.0;
		}
		for l690 in 0..64 {
			self.fVec683[l690 as usize] = 0.0;
		}
		for l691 in 0..64 {
			self.fVec684[l691 as usize] = 0.0;
		}
		for l692 in 0..64 {
			self.fVec685[l692 as usize] = 0.0;
		}
		for l693 in 0..64 {
			self.fVec686[l693 as usize] = 0.0;
		}
		for l694 in 0..64 {
			self.fVec687[l694 as usize] = 0.0;
		}
		for l695 in 0..64 {
			self.fVec688[l695 as usize] = 0.0;
		}
		for l696 in 0..64 {
			self.fVec689[l696 as usize] = 0.0;
		}
		for l697 in 0..64 {
			self.fVec690[l697 as usize] = 0.0;
		}
		for l698 in 0..64 {
			self.fVec691[l698 as usize] = 0.0;
		}
		for l699 in 0..64 {
			self.fVec692[l699 as usize] = 0.0;
		}
		for l700 in 0..64 {
			self.fVec693[l700 as usize] = 0.0;
		}
		for l701 in 0..64 {
			self.fVec694[l701 as usize] = 0.0;
		}
		for l702 in 0..64 {
			self.fVec695[l702 as usize] = 0.0;
		}
		for l703 in 0..64 {
			self.fVec696[l703 as usize] = 0.0;
		}
		for l704 in 0..64 {
			self.fVec697[l704 as usize] = 0.0;
		}
		for l705 in 0..32 {
			self.fVec698[l705 as usize] = 0.0;
		}
		for l706 in 0..32 {
			self.fVec699[l706 as usize] = 0.0;
		}
		for l707 in 0..32 {
			self.fVec700[l707 as usize] = 0.0;
		}
		for l708 in 0..32 {
			self.fVec701[l708 as usize] = 0.0;
		}
		for l709 in 0..32 {
			self.fVec702[l709 as usize] = 0.0;
		}
		for l710 in 0..32 {
			self.fVec703[l710 as usize] = 0.0;
		}
		for l711 in 0..32 {
			self.fVec704[l711 as usize] = 0.0;
		}
		for l712 in 0..32 {
			self.fVec705[l712 as usize] = 0.0;
		}
		for l713 in 0..32 {
			self.fVec706[l713 as usize] = 0.0;
		}
		for l714 in 0..32 {
			self.fVec707[l714 as usize] = 0.0;
		}
		for l715 in 0..32 {
			self.fVec708[l715 as usize] = 0.0;
		}
		for l716 in 0..32 {
			self.fVec709[l716 as usize] = 0.0;
		}
		for l717 in 0..32 {
			self.fVec710[l717 as usize] = 0.0;
		}
		for l718 in 0..32 {
			self.fVec711[l718 as usize] = 0.0;
		}
		for l719 in 0..32 {
			self.fVec712[l719 as usize] = 0.0;
		}
		for l720 in 0..32 {
			self.fVec713[l720 as usize] = 0.0;
		}
		for l721 in 0..16 {
			self.fVec714[l721 as usize] = 0.0;
		}
		for l722 in 0..15 {
			self.fVec715[l722 as usize] = 0.0;
		}
		for l723 in 0..14 {
			self.fVec716[l723 as usize] = 0.0;
		}
		for l724 in 0..13 {
			self.fVec717[l724 as usize] = 0.0;
		}
		for l725 in 0..12 {
			self.fVec718[l725 as usize] = 0.0;
		}
		for l726 in 0..11 {
			self.fVec719[l726 as usize] = 0.0;
		}
		for l727 in 0..10 {
			self.fVec720[l727 as usize] = 0.0;
		}
		for l728 in 0..9 {
			self.fVec721[l728 as usize] = 0.0;
		}
		for l729 in 0..8 {
			self.fVec722[l729 as usize] = 0.0;
		}
		for l730 in 0..7 {
			self.fVec723[l730 as usize] = 0.0;
		}
		for l731 in 0..6 {
			self.fVec724[l731 as usize] = 0.0;
		}
		for l732 in 0..5 {
			self.fVec725[l732 as usize] = 0.0;
		}
		for l733 in 0..4 {
			self.fVec726[l733 as usize] = 0.0;
		}
		for l734 in 0..3 {
			self.fVec727[l734 as usize] = 0.0;
		}
		for l735 in 0..2 {
			self.fVec728[l735 as usize] = 0.0;
		}
	}
	pub fn instance_constants(&mut self, sample_rate: i32) {
		// Obtaining locks on 2 static var(s)
		let ftbl0mydspSIG0_guard = ftbl0mydspSIG0.read().unwrap();
		let ftbl1mydspSIG1_guard = ftbl1mydspSIG1.read().unwrap();
		self.fSampleRate = sample_rate;
		self.fConst0 = ftbl0mydspSIG0_guard[61];
		self.fConst1 = ftbl0mydspSIG0_guard[62];
		self.fConst2 = ftbl0mydspSIG0_guard[60];
		self.fConst3 = ftbl0mydspSIG0_guard[59];
		self.fConst4 = ftbl0mydspSIG0_guard[58];
		self.fConst5 = ftbl0mydspSIG0_guard[57];
		self.fConst6 = ftbl0mydspSIG0_guard[56];
		self.fConst7 = ftbl0mydspSIG0_guard[55];
		self.fConst8 = ftbl0mydspSIG0_guard[54];
		self.fConst9 = ftbl0mydspSIG0_guard[53];
		self.fConst10 = ftbl0mydspSIG0_guard[52];
		self.fConst11 = ftbl0mydspSIG0_guard[51];
		self.fConst12 = ftbl0mydspSIG0_guard[50];
		self.fConst13 = ftbl0mydspSIG0_guard[49];
		self.fConst14 = ftbl0mydspSIG0_guard[48];
		self.fConst15 = ftbl0mydspSIG0_guard[47];
		self.fConst16 = ftbl0mydspSIG0_guard[46];
		self.fConst17 = ftbl0mydspSIG0_guard[45];
		self.fConst18 = ftbl0mydspSIG0_guard[44];
		self.fConst19 = ftbl0mydspSIG0_guard[43];
		self.fConst20 = ftbl0mydspSIG0_guard[42];
		self.fConst21 = ftbl0mydspSIG0_guard[41];
		self.fConst22 = ftbl0mydspSIG0_guard[40];
		self.fConst23 = ftbl0mydspSIG0_guard[39];
		self.fConst24 = ftbl0mydspSIG0_guard[38];
		self.fConst25 = ftbl0mydspSIG0_guard[37];
		self.fConst26 = ftbl0mydspSIG0_guard[36];
		self.fConst27 = ftbl0mydspSIG0_guard[35];
		self.fConst28 = ftbl0mydspSIG0_guard[34];
		self.fConst29 = ftbl0mydspSIG0_guard[33];
		self.fConst30 = ftbl0mydspSIG0_guard[32];
		self.fConst31 = ftbl0mydspSIG0_guard[31];
		self.fConst32 = ftbl0mydspSIG0_guard[30];
		self.fConst33 = ftbl0mydspSIG0_guard[29];
		self.fConst34 = ftbl0mydspSIG0_guard[28];
		self.fConst35 = ftbl0mydspSIG0_guard[27];
		self.fConst36 = ftbl0mydspSIG0_guard[26];
		self.fConst37 = ftbl0mydspSIG0_guard[25];
		self.fConst38 = ftbl0mydspSIG0_guard[24];
		self.fConst39 = ftbl0mydspSIG0_guard[23];
		self.fConst40 = ftbl0mydspSIG0_guard[22];
		self.fConst41 = ftbl0mydspSIG0_guard[21];
		self.fConst42 = ftbl0mydspSIG0_guard[20];
		self.fConst43 = ftbl0mydspSIG0_guard[19];
		self.fConst44 = ftbl0mydspSIG0_guard[18];
		self.fConst45 = ftbl0mydspSIG0_guard[17];
		self.fConst46 = ftbl0mydspSIG0_guard[16];
		self.fConst47 = ftbl0mydspSIG0_guard[15];
		self.fConst48 = ftbl0mydspSIG0_guard[14];
		self.fConst49 = ftbl0mydspSIG0_guard[13];
		self.fConst50 = ftbl0mydspSIG0_guard[12];
		self.fConst51 = ftbl0mydspSIG0_guard[11];
		self.fConst52 = ftbl0mydspSIG0_guard[10];
		self.fConst53 = ftbl0mydspSIG0_guard[9];
		self.fConst54 = ftbl0mydspSIG0_guard[8];
		self.fConst55 = ftbl0mydspSIG0_guard[7];
		self.fConst56 = ftbl0mydspSIG0_guard[6];
		self.fConst57 = ftbl0mydspSIG0_guard[5];
		self.fConst58 = ftbl0mydspSIG0_guard[4];
		self.fConst59 = ftbl0mydspSIG0_guard[3];
		self.fConst60 = ftbl0mydspSIG0_guard[2];
		self.fConst61 = ftbl0mydspSIG0_guard[1];
		self.fConst62 = ftbl0mydspSIG0_guard[0];
		self.fConst63 = ftbl1mydspSIG1_guard[119];
		self.fConst64 = ftbl1mydspSIG1_guard[120];
		self.fConst65 = ftbl1mydspSIG1_guard[118];
		self.fConst66 = ftbl1mydspSIG1_guard[117];
		self.fConst67 = ftbl1mydspSIG1_guard[116];
		self.fConst68 = ftbl1mydspSIG1_guard[115];
		self.fConst69 = ftbl1mydspSIG1_guard[114];
		self.fConst70 = ftbl1mydspSIG1_guard[113];
		self.fConst71 = ftbl1mydspSIG1_guard[112];
		self.fConst72 = ftbl1mydspSIG1_guard[111];
		self.fConst73 = ftbl1mydspSIG1_guard[110];
		self.fConst74 = ftbl1mydspSIG1_guard[109];
		self.fConst75 = ftbl1mydspSIG1_guard[108];
		self.fConst76 = ftbl1mydspSIG1_guard[107];
		self.fConst77 = ftbl1mydspSIG1_guard[106];
		self.fConst78 = ftbl1mydspSIG1_guard[105];
		self.fConst79 = ftbl1mydspSIG1_guard[104];
		self.fConst80 = ftbl1mydspSIG1_guard[103];
		self.fConst81 = ftbl1mydspSIG1_guard[102];
		self.fConst82 = ftbl1mydspSIG1_guard[101];
		self.fConst83 = ftbl1mydspSIG1_guard[100];
		self.fConst84 = ftbl1mydspSIG1_guard[99];
		self.fConst85 = ftbl1mydspSIG1_guard[98];
		self.fConst86 = ftbl1mydspSIG1_guard[97];
		self.fConst87 = ftbl1mydspSIG1_guard[96];
		self.fConst88 = ftbl1mydspSIG1_guard[95];
		self.fConst89 = ftbl1mydspSIG1_guard[94];
		self.fConst90 = ftbl1mydspSIG1_guard[93];
		self.fConst91 = ftbl1mydspSIG1_guard[92];
		self.fConst92 = ftbl1mydspSIG1_guard[91];
		self.fConst93 = ftbl1mydspSIG1_guard[90];
		self.fConst94 = ftbl1mydspSIG1_guard[89];
		self.fConst95 = ftbl1mydspSIG1_guard[88];
		self.fConst96 = ftbl1mydspSIG1_guard[87];
		self.fConst97 = ftbl1mydspSIG1_guard[86];
		self.fConst98 = ftbl1mydspSIG1_guard[85];
		self.fConst99 = ftbl1mydspSIG1_guard[84];
		self.fConst100 = ftbl1mydspSIG1_guard[83];
		self.fConst101 = ftbl1mydspSIG1_guard[82];
		self.fConst102 = ftbl1mydspSIG1_guard[81];
		self.fConst103 = ftbl1mydspSIG1_guard[80];
		self.fConst104 = ftbl1mydspSIG1_guard[79];
		self.fConst105 = ftbl1mydspSIG1_guard[78];
		self.fConst106 = ftbl1mydspSIG1_guard[77];
		self.fConst107 = ftbl1mydspSIG1_guard[76];
		self.fConst108 = ftbl1mydspSIG1_guard[75];
		self.fConst109 = ftbl1mydspSIG1_guard[74];
		self.fConst110 = ftbl1mydspSIG1_guard[73];
		self.fConst111 = ftbl1mydspSIG1_guard[72];
		self.fConst112 = ftbl1mydspSIG1_guard[71];
		self.fConst113 = ftbl1mydspSIG1_guard[70];
		self.fConst114 = ftbl1mydspSIG1_guard[69];
		self.fConst115 = ftbl1mydspSIG1_guard[68];
		self.fConst116 = ftbl1mydspSIG1_guard[67];
		self.fConst117 = ftbl1mydspSIG1_guard[66];
		self.fConst118 = ftbl1mydspSIG1_guard[65];
		self.fConst119 = ftbl1mydspSIG1_guard[64];
		self.fConst120 = ftbl1mydspSIG1_guard[63];
		self.fConst121 = ftbl1mydspSIG1_guard[62];
		self.fConst122 = ftbl1mydspSIG1_guard[61];
		self.fConst123 = ftbl1mydspSIG1_guard[60];
		self.fConst124 = ftbl1mydspSIG1_guard[59];
		self.fConst125 = ftbl1mydspSIG1_guard[58];
		self.fConst126 = ftbl1mydspSIG1_guard[57];
		self.fConst127 = ftbl1mydspSIG1_guard[56];
		self.fConst128 = ftbl1mydspSIG1_guard[55];
		self.fConst129 = ftbl1mydspSIG1_guard[54];
		self.fConst130 = ftbl1mydspSIG1_guard[53];
		self.fConst131 = ftbl1mydspSIG1_guard[52];
		self.fConst132 = ftbl1mydspSIG1_guard[51];
		self.fConst133 = ftbl1mydspSIG1_guard[50];
		self.fConst134 = ftbl1mydspSIG1_guard[49];
		self.fConst135 = ftbl1mydspSIG1_guard[48];
		self.fConst136 = ftbl1mydspSIG1_guard[47];
		self.fConst137 = ftbl1mydspSIG1_guard[46];
		self.fConst138 = ftbl1mydspSIG1_guard[45];
		self.fConst139 = ftbl1mydspSIG1_guard[44];
		self.fConst140 = ftbl1mydspSIG1_guard[43];
		self.fConst141 = ftbl1mydspSIG1_guard[42];
		self.fConst142 = ftbl1mydspSIG1_guard[41];
		self.fConst143 = ftbl1mydspSIG1_guard[40];
		self.fConst144 = ftbl1mydspSIG1_guard[39];
		self.fConst145 = ftbl1mydspSIG1_guard[38];
		self.fConst146 = ftbl1mydspSIG1_guard[37];
		self.fConst147 = ftbl1mydspSIG1_guard[36];
		self.fConst148 = ftbl1mydspSIG1_guard[35];
		self.fConst149 = ftbl1mydspSIG1_guard[34];
		self.fConst150 = ftbl1mydspSIG1_guard[33];
		self.fConst151 = ftbl1mydspSIG1_guard[32];
		self.fConst152 = ftbl1mydspSIG1_guard[31];
		self.fConst153 = ftbl1mydspSIG1_guard[30];
		self.fConst154 = ftbl1mydspSIG1_guard[29];
		self.fConst155 = ftbl1mydspSIG1_guard[28];
		self.fConst156 = ftbl1mydspSIG1_guard[27];
		self.fConst157 = ftbl1mydspSIG1_guard[26];
		self.fConst158 = ftbl1mydspSIG1_guard[25];
		self.fConst159 = ftbl1mydspSIG1_guard[24];
		self.fConst160 = ftbl1mydspSIG1_guard[23];
		self.fConst161 = ftbl1mydspSIG1_guard[22];
		self.fConst162 = ftbl1mydspSIG1_guard[21];
		self.fConst163 = ftbl1mydspSIG1_guard[20];
		self.fConst164 = ftbl1mydspSIG1_guard[19];
		self.fConst165 = ftbl1mydspSIG1_guard[18];
		self.fConst166 = ftbl1mydspSIG1_guard[17];
		self.fConst167 = ftbl1mydspSIG1_guard[16];
		self.fConst168 = ftbl1mydspSIG1_guard[15];
		self.fConst169 = ftbl1mydspSIG1_guard[14];
		self.fConst170 = ftbl1mydspSIG1_guard[13];
		self.fConst171 = ftbl1mydspSIG1_guard[12];
		self.fConst172 = ftbl1mydspSIG1_guard[11];
		self.fConst173 = ftbl1mydspSIG1_guard[10];
		self.fConst174 = ftbl1mydspSIG1_guard[9];
		self.fConst175 = ftbl1mydspSIG1_guard[8];
		self.fConst176 = ftbl1mydspSIG1_guard[7];
		self.fConst177 = ftbl1mydspSIG1_guard[6];
		self.fConst178 = ftbl1mydspSIG1_guard[5];
		self.fConst179 = ftbl1mydspSIG1_guard[4];
		self.fConst180 = ftbl1mydspSIG1_guard[3];
		self.fConst181 = ftbl1mydspSIG1_guard[2];
		self.fConst182 = ftbl1mydspSIG1_guard[1];
		self.fConst183 = ftbl1mydspSIG1_guard[0];
		self.fConst184 = F32::min(1.92e+05, F32::max(1.0, (self.fSampleRate) as F32));
		self.fConst185 = 44.1 / self.fConst184;
		self.fConst186 = 1.0 - self.fConst185;
		self.fConst187 = 6.2831855 / self.fConst184;
	}
	pub fn instance_init(&mut self, sample_rate: i32) {
		self.instance_constants(sample_rate);
		self.instance_reset_params();
		self.instance_clear();
	}
	pub fn init(&mut self, sample_rate: i32) {
		mydsp::class_init(sample_rate);
		self.instance_init(sample_rate);
	}
	
	pub fn build_user_interface(&self, ui_interface: &mut dyn UI<FaustFloat>) {
		Self::build_user_interface_static(ui_interface);
	}
	
	pub fn build_user_interface_static(ui_interface: &mut dyn UI<FaustFloat>) {
		ui_interface.open_vertical_box("jack_mod");
		ui_interface.add_horizontal_slider("jack_mod/carrier_freq", ParamIndex(0), 0.08333, 0.01, 0.4, 1e-05);
		ui_interface.add_horizontal_slider("jack_mod/mod_index", ParamIndex(1), 3.14159, 0.0, 6.28318, 0.001);
		ui_interface.add_horizontal_slider("jack_mod/output_gain", ParamIndex(2), 0.8, 0.0, 1.0, 0.001);
		ui_interface.add_horizontal_slider("jack_mod/rrc_mode", ParamIndex(3), 0.0, 0.0, 1.0, 1.0);
		ui_interface.close_box();
	}
	
	pub fn get_param(&self, param: ParamIndex) -> Option<FaustFloat> {
		match param.0 {
			3 => Some(self.fHslider0),
			1 => Some(self.fHslider1),
			0 => Some(self.fHslider2),
			2 => Some(self.fHslider3),
			_ => None,
		}
	}
	
	pub fn set_param(&mut self, param: ParamIndex, value: FaustFloat) {
		match param.0 {
			3 => { self.fHslider0 = value }
			1 => { self.fHslider1 = value }
			0 => { self.fHslider2 = value }
			2 => { self.fHslider3 = value }
			_ => {}
		}
	}
	
	pub fn compute(
		&mut self,
		count: usize,
		inputs: &[impl AsRef<[FaustFloat]>],
		outputs: &mut[impl AsMut<[FaustFloat]>],
	) {
		
		// Obtaining locks on 2 static var(s)
		let ftbl0mydspSIG0_guard = ftbl0mydspSIG0.read().unwrap();
		let ftbl1mydspSIG1_guard = ftbl1mydspSIG1.read().unwrap();
		let [inputs0, inputs1, inputs2, inputs3, inputs4, inputs5, inputs6, inputs7, inputs8, inputs9, inputs10, inputs11, inputs12, inputs13, inputs14, inputs15, inputs16, inputs17, inputs18, inputs19, inputs20, inputs21, inputs22, inputs23, inputs24, inputs25, inputs26, inputs27, inputs28, inputs29, inputs30, inputs31, inputs32, inputs33, inputs34, inputs35, inputs36, inputs37, inputs38, inputs39, inputs40, inputs41, inputs42, inputs43, inputs44, inputs45, inputs46, inputs47, inputs48, inputs49, inputs50, inputs51, inputs52, inputs53, inputs54, inputs55, inputs56, inputs57, inputs58, inputs59, inputs60, inputs61, inputs62, inputs63, inputs64, inputs65, inputs66, inputs67, inputs68, inputs69, inputs70, inputs71, inputs72, inputs73, inputs74, inputs75, inputs76, inputs77, inputs78, inputs79, inputs80, inputs81, inputs82, inputs83, inputs84, inputs85, inputs86, inputs87, inputs88, inputs89, inputs90, inputs91, inputs92, inputs93, inputs94, inputs95, inputs96, inputs97, inputs98, inputs99, inputs100, inputs101, inputs102, inputs103, inputs104, inputs105, inputs106, inputs107, inputs108, inputs109, inputs110, inputs111, inputs112, inputs113, inputs114, inputs115, inputs116, inputs117, inputs118, inputs119, inputs120, inputs121, inputs122, inputs123, inputs124, inputs125, inputs126, inputs127, inputs128, inputs129, inputs130, inputs131, inputs132, inputs133, inputs134, inputs135, inputs136, inputs137, inputs138, inputs139, inputs140, inputs141, inputs142, inputs143, inputs144, inputs145, inputs146, inputs147, inputs148, inputs149, inputs150, inputs151, inputs152, inputs153, inputs154, inputs155, inputs156, inputs157, inputs158, inputs159, inputs160, inputs161, inputs162, inputs163, inputs164, inputs165, inputs166, inputs167, inputs168, inputs169, inputs170, inputs171, inputs172, inputs173, inputs174, inputs175, inputs176, inputs177, inputs178, inputs179, inputs180, inputs181, inputs182, inputs183, inputs184, inputs185, inputs186, inputs187, inputs188, inputs189, inputs190, inputs191, inputs192, inputs193, inputs194, inputs195, inputs196, inputs197, inputs198, inputs199, inputs200, inputs201, inputs202, inputs203, inputs204, inputs205, inputs206, inputs207, inputs208, inputs209, inputs210, inputs211, inputs212, inputs213, inputs214, inputs215, inputs216, inputs217, inputs218, inputs219, inputs220, inputs221, inputs222, inputs223, inputs224, inputs225, inputs226, inputs227, inputs228, inputs229, inputs230, inputs231, inputs232, inputs233, inputs234, inputs235, inputs236, inputs237, inputs238, inputs239, inputs240, inputs241, inputs242, inputs243, inputs244, inputs245, inputs246, inputs247, inputs248, inputs249, inputs250, inputs251, inputs252, inputs253, inputs254, inputs255, inputs256, inputs257, inputs258, inputs259, inputs260, inputs261, inputs262, inputs263, inputs264, inputs265, inputs266, inputs267, inputs268, inputs269, inputs270, inputs271, inputs272, inputs273, inputs274, inputs275, inputs276, inputs277, inputs278, inputs279, inputs280, inputs281, inputs282, inputs283, inputs284, inputs285, inputs286, inputs287, inputs288, inputs289, inputs290, inputs291, inputs292, inputs293, inputs294, inputs295, inputs296, inputs297, inputs298, inputs299, inputs300, inputs301, inputs302, inputs303, inputs304, inputs305, inputs306, inputs307, inputs308, inputs309, inputs310, inputs311, inputs312, inputs313, inputs314, inputs315, inputs316, inputs317, inputs318, inputs319, inputs320, inputs321, inputs322, inputs323, inputs324, inputs325, inputs326, inputs327, inputs328, inputs329, inputs330, inputs331, inputs332, inputs333, inputs334, inputs335, inputs336, inputs337, inputs338, inputs339, inputs340, inputs341, inputs342, inputs343, inputs344, inputs345, inputs346, inputs347, inputs348, inputs349, inputs350, inputs351, inputs352, inputs353, inputs354, inputs355, inputs356, inputs357, inputs358, inputs359, inputs360, inputs361, inputs362, inputs363, inputs364, inputs365, inputs366, inputs367, inputs368, inputs369, inputs370, inputs371, inputs372, inputs373, inputs374, inputs375, inputs376, inputs377, inputs378, inputs379, inputs380, inputs381, inputs382, inputs383, inputs384, inputs385, inputs386, inputs387, inputs388, inputs389, inputs390, inputs391, inputs392, inputs393, inputs394, inputs395, inputs396, inputs397, inputs398, inputs399, inputs400, inputs401, inputs402, inputs403, inputs404, inputs405, inputs406, inputs407, inputs408, inputs409, inputs410, inputs411, inputs412, inputs413, inputs414, inputs415, inputs416, inputs417, inputs418, inputs419, inputs420, inputs421, inputs422, inputs423, inputs424, inputs425, inputs426, inputs427, inputs428, inputs429, inputs430, inputs431, inputs432, inputs433, inputs434, inputs435, inputs436, inputs437, inputs438, inputs439, inputs440, inputs441, inputs442, inputs443, inputs444, inputs445, inputs446, inputs447, inputs448, inputs449, inputs450, inputs451, inputs452, inputs453, inputs454, inputs455, inputs456, inputs457, inputs458, inputs459, inputs460, inputs461, inputs462, inputs463, inputs464, inputs465, inputs466, inputs467, inputs468, inputs469, inputs470, inputs471, inputs472, inputs473, inputs474, inputs475, inputs476, inputs477, inputs478, inputs479, inputs480, inputs481, inputs482, inputs483, inputs484, inputs485, inputs486, inputs487, inputs488, inputs489, inputs490, inputs491, inputs492, inputs493, inputs494, inputs495, inputs496, inputs497, inputs498, inputs499, inputs500, inputs501, inputs502, inputs503, inputs504, inputs505, inputs506, inputs507, inputs508, inputs509, inputs510, inputs511, inputs512, inputs513, inputs514, inputs515, inputs516, inputs517, inputs518, inputs519, inputs520, inputs521, inputs522, inputs523, inputs524, inputs525, inputs526, inputs527, inputs528, inputs529, inputs530, inputs531, inputs532, inputs533, inputs534, inputs535, inputs536, inputs537, inputs538, inputs539, inputs540, inputs541, inputs542, inputs543, inputs544, inputs545, inputs546, inputs547, inputs548, inputs549, inputs550, inputs551, inputs552, inputs553, inputs554, inputs555, inputs556, inputs557, inputs558, inputs559, inputs560, inputs561, inputs562, inputs563, inputs564, inputs565, inputs566, inputs567, inputs568, inputs569, inputs570, inputs571, inputs572, inputs573, inputs574, inputs575, inputs576, inputs577, inputs578, inputs579, inputs580, inputs581, inputs582, inputs583, inputs584, inputs585, inputs586, inputs587, inputs588, inputs589, inputs590, inputs591, inputs592, inputs593, inputs594, inputs595, inputs596, inputs597, inputs598, inputs599, inputs600, inputs601, inputs602, inputs603, inputs604, inputs605, inputs606, inputs607, inputs608, inputs609, inputs610, inputs611, inputs612, inputs613, inputs614, inputs615, inputs616, inputs617, inputs618, inputs619, inputs620, inputs621, inputs622, inputs623, inputs624, inputs625, inputs626, inputs627, inputs628, inputs629, inputs630, inputs631, inputs632, inputs633, inputs634, inputs635, inputs636, inputs637, inputs638, inputs639, inputs640, inputs641, inputs642, inputs643, inputs644, inputs645, inputs646, inputs647, inputs648, inputs649, inputs650, inputs651, inputs652, inputs653, inputs654, inputs655, inputs656, inputs657, inputs658, inputs659, inputs660, inputs661, inputs662, inputs663, inputs664, inputs665, inputs666, inputs667, inputs668, inputs669, inputs670, inputs671, inputs672, inputs673, inputs674, inputs675, inputs676, inputs677, inputs678, inputs679, inputs680, inputs681, inputs682, inputs683, inputs684, inputs685, inputs686, inputs687, inputs688, inputs689, inputs690, inputs691, inputs692, inputs693, inputs694, inputs695, inputs696, inputs697, inputs698, inputs699, inputs700, inputs701, inputs702, inputs703, inputs704, inputs705, inputs706, inputs707, inputs708, inputs709, inputs710, inputs711, inputs712, inputs713, inputs714, inputs715, inputs716, inputs717, inputs718, inputs719, inputs720, inputs721, inputs722, inputs723, inputs724, inputs725, inputs726, inputs727, inputs728, inputs729, inputs730, inputs731, inputs732, inputs733, inputs734, inputs735, .. ] = inputs.as_ref() else { panic!("wrong number of input buffers"); };
		let inputs0 = inputs0.as_ref()[..count].iter();
		let inputs1 = inputs1.as_ref()[..count].iter();
		let inputs2 = inputs2.as_ref()[..count].iter();
		let inputs3 = inputs3.as_ref()[..count].iter();
		let inputs4 = inputs4.as_ref()[..count].iter();
		let inputs5 = inputs5.as_ref()[..count].iter();
		let inputs6 = inputs6.as_ref()[..count].iter();
		let inputs7 = inputs7.as_ref()[..count].iter();
		let inputs8 = inputs8.as_ref()[..count].iter();
		let inputs9 = inputs9.as_ref()[..count].iter();
		let inputs10 = inputs10.as_ref()[..count].iter();
		let inputs11 = inputs11.as_ref()[..count].iter();
		let inputs12 = inputs12.as_ref()[..count].iter();
		let inputs13 = inputs13.as_ref()[..count].iter();
		let inputs14 = inputs14.as_ref()[..count].iter();
		let inputs15 = inputs15.as_ref()[..count].iter();
		let inputs16 = inputs16.as_ref()[..count].iter();
		let inputs17 = inputs17.as_ref()[..count].iter();
		let inputs18 = inputs18.as_ref()[..count].iter();
		let inputs19 = inputs19.as_ref()[..count].iter();
		let inputs20 = inputs20.as_ref()[..count].iter();
		let inputs21 = inputs21.as_ref()[..count].iter();
		let inputs22 = inputs22.as_ref()[..count].iter();
		let inputs23 = inputs23.as_ref()[..count].iter();
		let inputs24 = inputs24.as_ref()[..count].iter();
		let inputs25 = inputs25.as_ref()[..count].iter();
		let inputs26 = inputs26.as_ref()[..count].iter();
		let inputs27 = inputs27.as_ref()[..count].iter();
		let inputs28 = inputs28.as_ref()[..count].iter();
		let inputs29 = inputs29.as_ref()[..count].iter();
		let inputs30 = inputs30.as_ref()[..count].iter();
		let inputs31 = inputs31.as_ref()[..count].iter();
		let inputs32 = inputs32.as_ref()[..count].iter();
		let inputs33 = inputs33.as_ref()[..count].iter();
		let inputs34 = inputs34.as_ref()[..count].iter();
		let inputs35 = inputs35.as_ref()[..count].iter();
		let inputs36 = inputs36.as_ref()[..count].iter();
		let inputs37 = inputs37.as_ref()[..count].iter();
		let inputs38 = inputs38.as_ref()[..count].iter();
		let inputs39 = inputs39.as_ref()[..count].iter();
		let inputs40 = inputs40.as_ref()[..count].iter();
		let inputs41 = inputs41.as_ref()[..count].iter();
		let inputs42 = inputs42.as_ref()[..count].iter();
		let inputs43 = inputs43.as_ref()[..count].iter();
		let inputs44 = inputs44.as_ref()[..count].iter();
		let inputs45 = inputs45.as_ref()[..count].iter();
		let inputs46 = inputs46.as_ref()[..count].iter();
		let inputs47 = inputs47.as_ref()[..count].iter();
		let inputs48 = inputs48.as_ref()[..count].iter();
		let inputs49 = inputs49.as_ref()[..count].iter();
		let inputs50 = inputs50.as_ref()[..count].iter();
		let inputs51 = inputs51.as_ref()[..count].iter();
		let inputs52 = inputs52.as_ref()[..count].iter();
		let inputs53 = inputs53.as_ref()[..count].iter();
		let inputs54 = inputs54.as_ref()[..count].iter();
		let inputs55 = inputs55.as_ref()[..count].iter();
		let inputs56 = inputs56.as_ref()[..count].iter();
		let inputs57 = inputs57.as_ref()[..count].iter();
		let inputs58 = inputs58.as_ref()[..count].iter();
		let inputs59 = inputs59.as_ref()[..count].iter();
		let inputs60 = inputs60.as_ref()[..count].iter();
		let inputs61 = inputs61.as_ref()[..count].iter();
		let inputs62 = inputs62.as_ref()[..count].iter();
		let inputs63 = inputs63.as_ref()[..count].iter();
		let inputs64 = inputs64.as_ref()[..count].iter();
		let inputs65 = inputs65.as_ref()[..count].iter();
		let inputs66 = inputs66.as_ref()[..count].iter();
		let inputs67 = inputs67.as_ref()[..count].iter();
		let inputs68 = inputs68.as_ref()[..count].iter();
		let inputs69 = inputs69.as_ref()[..count].iter();
		let inputs70 = inputs70.as_ref()[..count].iter();
		let inputs71 = inputs71.as_ref()[..count].iter();
		let inputs72 = inputs72.as_ref()[..count].iter();
		let inputs73 = inputs73.as_ref()[..count].iter();
		let inputs74 = inputs74.as_ref()[..count].iter();
		let inputs75 = inputs75.as_ref()[..count].iter();
		let inputs76 = inputs76.as_ref()[..count].iter();
		let inputs77 = inputs77.as_ref()[..count].iter();
		let inputs78 = inputs78.as_ref()[..count].iter();
		let inputs79 = inputs79.as_ref()[..count].iter();
		let inputs80 = inputs80.as_ref()[..count].iter();
		let inputs81 = inputs81.as_ref()[..count].iter();
		let inputs82 = inputs82.as_ref()[..count].iter();
		let inputs83 = inputs83.as_ref()[..count].iter();
		let inputs84 = inputs84.as_ref()[..count].iter();
		let inputs85 = inputs85.as_ref()[..count].iter();
		let inputs86 = inputs86.as_ref()[..count].iter();
		let inputs87 = inputs87.as_ref()[..count].iter();
		let inputs88 = inputs88.as_ref()[..count].iter();
		let inputs89 = inputs89.as_ref()[..count].iter();
		let inputs90 = inputs90.as_ref()[..count].iter();
		let inputs91 = inputs91.as_ref()[..count].iter();
		let inputs92 = inputs92.as_ref()[..count].iter();
		let inputs93 = inputs93.as_ref()[..count].iter();
		let inputs94 = inputs94.as_ref()[..count].iter();
		let inputs95 = inputs95.as_ref()[..count].iter();
		let inputs96 = inputs96.as_ref()[..count].iter();
		let inputs97 = inputs97.as_ref()[..count].iter();
		let inputs98 = inputs98.as_ref()[..count].iter();
		let inputs99 = inputs99.as_ref()[..count].iter();
		let inputs100 = inputs100.as_ref()[..count].iter();
		let inputs101 = inputs101.as_ref()[..count].iter();
		let inputs102 = inputs102.as_ref()[..count].iter();
		let inputs103 = inputs103.as_ref()[..count].iter();
		let inputs104 = inputs104.as_ref()[..count].iter();
		let inputs105 = inputs105.as_ref()[..count].iter();
		let inputs106 = inputs106.as_ref()[..count].iter();
		let inputs107 = inputs107.as_ref()[..count].iter();
		let inputs108 = inputs108.as_ref()[..count].iter();
		let inputs109 = inputs109.as_ref()[..count].iter();
		let inputs110 = inputs110.as_ref()[..count].iter();
		let inputs111 = inputs111.as_ref()[..count].iter();
		let inputs112 = inputs112.as_ref()[..count].iter();
		let inputs113 = inputs113.as_ref()[..count].iter();
		let inputs114 = inputs114.as_ref()[..count].iter();
		let inputs115 = inputs115.as_ref()[..count].iter();
		let inputs116 = inputs116.as_ref()[..count].iter();
		let inputs117 = inputs117.as_ref()[..count].iter();
		let inputs118 = inputs118.as_ref()[..count].iter();
		let inputs119 = inputs119.as_ref()[..count].iter();
		let inputs120 = inputs120.as_ref()[..count].iter();
		let inputs121 = inputs121.as_ref()[..count].iter();
		let inputs122 = inputs122.as_ref()[..count].iter();
		let inputs123 = inputs123.as_ref()[..count].iter();
		let inputs124 = inputs124.as_ref()[..count].iter();
		let inputs125 = inputs125.as_ref()[..count].iter();
		let inputs126 = inputs126.as_ref()[..count].iter();
		let inputs127 = inputs127.as_ref()[..count].iter();
		let inputs128 = inputs128.as_ref()[..count].iter();
		let inputs129 = inputs129.as_ref()[..count].iter();
		let inputs130 = inputs130.as_ref()[..count].iter();
		let inputs131 = inputs131.as_ref()[..count].iter();
		let inputs132 = inputs132.as_ref()[..count].iter();
		let inputs133 = inputs133.as_ref()[..count].iter();
		let inputs134 = inputs134.as_ref()[..count].iter();
		let inputs135 = inputs135.as_ref()[..count].iter();
		let inputs136 = inputs136.as_ref()[..count].iter();
		let inputs137 = inputs137.as_ref()[..count].iter();
		let inputs138 = inputs138.as_ref()[..count].iter();
		let inputs139 = inputs139.as_ref()[..count].iter();
		let inputs140 = inputs140.as_ref()[..count].iter();
		let inputs141 = inputs141.as_ref()[..count].iter();
		let inputs142 = inputs142.as_ref()[..count].iter();
		let inputs143 = inputs143.as_ref()[..count].iter();
		let inputs144 = inputs144.as_ref()[..count].iter();
		let inputs145 = inputs145.as_ref()[..count].iter();
		let inputs146 = inputs146.as_ref()[..count].iter();
		let inputs147 = inputs147.as_ref()[..count].iter();
		let inputs148 = inputs148.as_ref()[..count].iter();
		let inputs149 = inputs149.as_ref()[..count].iter();
		let inputs150 = inputs150.as_ref()[..count].iter();
		let inputs151 = inputs151.as_ref()[..count].iter();
		let inputs152 = inputs152.as_ref()[..count].iter();
		let inputs153 = inputs153.as_ref()[..count].iter();
		let inputs154 = inputs154.as_ref()[..count].iter();
		let inputs155 = inputs155.as_ref()[..count].iter();
		let inputs156 = inputs156.as_ref()[..count].iter();
		let inputs157 = inputs157.as_ref()[..count].iter();
		let inputs158 = inputs158.as_ref()[..count].iter();
		let inputs159 = inputs159.as_ref()[..count].iter();
		let inputs160 = inputs160.as_ref()[..count].iter();
		let inputs161 = inputs161.as_ref()[..count].iter();
		let inputs162 = inputs162.as_ref()[..count].iter();
		let inputs163 = inputs163.as_ref()[..count].iter();
		let inputs164 = inputs164.as_ref()[..count].iter();
		let inputs165 = inputs165.as_ref()[..count].iter();
		let inputs166 = inputs166.as_ref()[..count].iter();
		let inputs167 = inputs167.as_ref()[..count].iter();
		let inputs168 = inputs168.as_ref()[..count].iter();
		let inputs169 = inputs169.as_ref()[..count].iter();
		let inputs170 = inputs170.as_ref()[..count].iter();
		let inputs171 = inputs171.as_ref()[..count].iter();
		let inputs172 = inputs172.as_ref()[..count].iter();
		let inputs173 = inputs173.as_ref()[..count].iter();
		let inputs174 = inputs174.as_ref()[..count].iter();
		let inputs175 = inputs175.as_ref()[..count].iter();
		let inputs176 = inputs176.as_ref()[..count].iter();
		let inputs177 = inputs177.as_ref()[..count].iter();
		let inputs178 = inputs178.as_ref()[..count].iter();
		let inputs179 = inputs179.as_ref()[..count].iter();
		let inputs180 = inputs180.as_ref()[..count].iter();
		let inputs181 = inputs181.as_ref()[..count].iter();
		let inputs182 = inputs182.as_ref()[..count].iter();
		let inputs183 = inputs183.as_ref()[..count].iter();
		let inputs184 = inputs184.as_ref()[..count].iter();
		let inputs185 = inputs185.as_ref()[..count].iter();
		let inputs186 = inputs186.as_ref()[..count].iter();
		let inputs187 = inputs187.as_ref()[..count].iter();
		let inputs188 = inputs188.as_ref()[..count].iter();
		let inputs189 = inputs189.as_ref()[..count].iter();
		let inputs190 = inputs190.as_ref()[..count].iter();
		let inputs191 = inputs191.as_ref()[..count].iter();
		let inputs192 = inputs192.as_ref()[..count].iter();
		let inputs193 = inputs193.as_ref()[..count].iter();
		let inputs194 = inputs194.as_ref()[..count].iter();
		let inputs195 = inputs195.as_ref()[..count].iter();
		let inputs196 = inputs196.as_ref()[..count].iter();
		let inputs197 = inputs197.as_ref()[..count].iter();
		let inputs198 = inputs198.as_ref()[..count].iter();
		let inputs199 = inputs199.as_ref()[..count].iter();
		let inputs200 = inputs200.as_ref()[..count].iter();
		let inputs201 = inputs201.as_ref()[..count].iter();
		let inputs202 = inputs202.as_ref()[..count].iter();
		let inputs203 = inputs203.as_ref()[..count].iter();
		let inputs204 = inputs204.as_ref()[..count].iter();
		let inputs205 = inputs205.as_ref()[..count].iter();
		let inputs206 = inputs206.as_ref()[..count].iter();
		let inputs207 = inputs207.as_ref()[..count].iter();
		let inputs208 = inputs208.as_ref()[..count].iter();
		let inputs209 = inputs209.as_ref()[..count].iter();
		let inputs210 = inputs210.as_ref()[..count].iter();
		let inputs211 = inputs211.as_ref()[..count].iter();
		let inputs212 = inputs212.as_ref()[..count].iter();
		let inputs213 = inputs213.as_ref()[..count].iter();
		let inputs214 = inputs214.as_ref()[..count].iter();
		let inputs215 = inputs215.as_ref()[..count].iter();
		let inputs216 = inputs216.as_ref()[..count].iter();
		let inputs217 = inputs217.as_ref()[..count].iter();
		let inputs218 = inputs218.as_ref()[..count].iter();
		let inputs219 = inputs219.as_ref()[..count].iter();
		let inputs220 = inputs220.as_ref()[..count].iter();
		let inputs221 = inputs221.as_ref()[..count].iter();
		let inputs222 = inputs222.as_ref()[..count].iter();
		let inputs223 = inputs223.as_ref()[..count].iter();
		let inputs224 = inputs224.as_ref()[..count].iter();
		let inputs225 = inputs225.as_ref()[..count].iter();
		let inputs226 = inputs226.as_ref()[..count].iter();
		let inputs227 = inputs227.as_ref()[..count].iter();
		let inputs228 = inputs228.as_ref()[..count].iter();
		let inputs229 = inputs229.as_ref()[..count].iter();
		let inputs230 = inputs230.as_ref()[..count].iter();
		let inputs231 = inputs231.as_ref()[..count].iter();
		let inputs232 = inputs232.as_ref()[..count].iter();
		let inputs233 = inputs233.as_ref()[..count].iter();
		let inputs234 = inputs234.as_ref()[..count].iter();
		let inputs235 = inputs235.as_ref()[..count].iter();
		let inputs236 = inputs236.as_ref()[..count].iter();
		let inputs237 = inputs237.as_ref()[..count].iter();
		let inputs238 = inputs238.as_ref()[..count].iter();
		let inputs239 = inputs239.as_ref()[..count].iter();
		let inputs240 = inputs240.as_ref()[..count].iter();
		let inputs241 = inputs241.as_ref()[..count].iter();
		let inputs242 = inputs242.as_ref()[..count].iter();
		let inputs243 = inputs243.as_ref()[..count].iter();
		let inputs244 = inputs244.as_ref()[..count].iter();
		let inputs245 = inputs245.as_ref()[..count].iter();
		let inputs246 = inputs246.as_ref()[..count].iter();
		let inputs247 = inputs247.as_ref()[..count].iter();
		let inputs248 = inputs248.as_ref()[..count].iter();
		let inputs249 = inputs249.as_ref()[..count].iter();
		let inputs250 = inputs250.as_ref()[..count].iter();
		let inputs251 = inputs251.as_ref()[..count].iter();
		let inputs252 = inputs252.as_ref()[..count].iter();
		let inputs253 = inputs253.as_ref()[..count].iter();
		let inputs254 = inputs254.as_ref()[..count].iter();
		let inputs255 = inputs255.as_ref()[..count].iter();
		let inputs256 = inputs256.as_ref()[..count].iter();
		let inputs257 = inputs257.as_ref()[..count].iter();
		let inputs258 = inputs258.as_ref()[..count].iter();
		let inputs259 = inputs259.as_ref()[..count].iter();
		let inputs260 = inputs260.as_ref()[..count].iter();
		let inputs261 = inputs261.as_ref()[..count].iter();
		let inputs262 = inputs262.as_ref()[..count].iter();
		let inputs263 = inputs263.as_ref()[..count].iter();
		let inputs264 = inputs264.as_ref()[..count].iter();
		let inputs265 = inputs265.as_ref()[..count].iter();
		let inputs266 = inputs266.as_ref()[..count].iter();
		let inputs267 = inputs267.as_ref()[..count].iter();
		let inputs268 = inputs268.as_ref()[..count].iter();
		let inputs269 = inputs269.as_ref()[..count].iter();
		let inputs270 = inputs270.as_ref()[..count].iter();
		let inputs271 = inputs271.as_ref()[..count].iter();
		let inputs272 = inputs272.as_ref()[..count].iter();
		let inputs273 = inputs273.as_ref()[..count].iter();
		let inputs274 = inputs274.as_ref()[..count].iter();
		let inputs275 = inputs275.as_ref()[..count].iter();
		let inputs276 = inputs276.as_ref()[..count].iter();
		let inputs277 = inputs277.as_ref()[..count].iter();
		let inputs278 = inputs278.as_ref()[..count].iter();
		let inputs279 = inputs279.as_ref()[..count].iter();
		let inputs280 = inputs280.as_ref()[..count].iter();
		let inputs281 = inputs281.as_ref()[..count].iter();
		let inputs282 = inputs282.as_ref()[..count].iter();
		let inputs283 = inputs283.as_ref()[..count].iter();
		let inputs284 = inputs284.as_ref()[..count].iter();
		let inputs285 = inputs285.as_ref()[..count].iter();
		let inputs286 = inputs286.as_ref()[..count].iter();
		let inputs287 = inputs287.as_ref()[..count].iter();
		let inputs288 = inputs288.as_ref()[..count].iter();
		let inputs289 = inputs289.as_ref()[..count].iter();
		let inputs290 = inputs290.as_ref()[..count].iter();
		let inputs291 = inputs291.as_ref()[..count].iter();
		let inputs292 = inputs292.as_ref()[..count].iter();
		let inputs293 = inputs293.as_ref()[..count].iter();
		let inputs294 = inputs294.as_ref()[..count].iter();
		let inputs295 = inputs295.as_ref()[..count].iter();
		let inputs296 = inputs296.as_ref()[..count].iter();
		let inputs297 = inputs297.as_ref()[..count].iter();
		let inputs298 = inputs298.as_ref()[..count].iter();
		let inputs299 = inputs299.as_ref()[..count].iter();
		let inputs300 = inputs300.as_ref()[..count].iter();
		let inputs301 = inputs301.as_ref()[..count].iter();
		let inputs302 = inputs302.as_ref()[..count].iter();
		let inputs303 = inputs303.as_ref()[..count].iter();
		let inputs304 = inputs304.as_ref()[..count].iter();
		let inputs305 = inputs305.as_ref()[..count].iter();
		let inputs306 = inputs306.as_ref()[..count].iter();
		let inputs307 = inputs307.as_ref()[..count].iter();
		let inputs308 = inputs308.as_ref()[..count].iter();
		let inputs309 = inputs309.as_ref()[..count].iter();
		let inputs310 = inputs310.as_ref()[..count].iter();
		let inputs311 = inputs311.as_ref()[..count].iter();
		let inputs312 = inputs312.as_ref()[..count].iter();
		let inputs313 = inputs313.as_ref()[..count].iter();
		let inputs314 = inputs314.as_ref()[..count].iter();
		let inputs315 = inputs315.as_ref()[..count].iter();
		let inputs316 = inputs316.as_ref()[..count].iter();
		let inputs317 = inputs317.as_ref()[..count].iter();
		let inputs318 = inputs318.as_ref()[..count].iter();
		let inputs319 = inputs319.as_ref()[..count].iter();
		let inputs320 = inputs320.as_ref()[..count].iter();
		let inputs321 = inputs321.as_ref()[..count].iter();
		let inputs322 = inputs322.as_ref()[..count].iter();
		let inputs323 = inputs323.as_ref()[..count].iter();
		let inputs324 = inputs324.as_ref()[..count].iter();
		let inputs325 = inputs325.as_ref()[..count].iter();
		let inputs326 = inputs326.as_ref()[..count].iter();
		let inputs327 = inputs327.as_ref()[..count].iter();
		let inputs328 = inputs328.as_ref()[..count].iter();
		let inputs329 = inputs329.as_ref()[..count].iter();
		let inputs330 = inputs330.as_ref()[..count].iter();
		let inputs331 = inputs331.as_ref()[..count].iter();
		let inputs332 = inputs332.as_ref()[..count].iter();
		let inputs333 = inputs333.as_ref()[..count].iter();
		let inputs334 = inputs334.as_ref()[..count].iter();
		let inputs335 = inputs335.as_ref()[..count].iter();
		let inputs336 = inputs336.as_ref()[..count].iter();
		let inputs337 = inputs337.as_ref()[..count].iter();
		let inputs338 = inputs338.as_ref()[..count].iter();
		let inputs339 = inputs339.as_ref()[..count].iter();
		let inputs340 = inputs340.as_ref()[..count].iter();
		let inputs341 = inputs341.as_ref()[..count].iter();
		let inputs342 = inputs342.as_ref()[..count].iter();
		let inputs343 = inputs343.as_ref()[..count].iter();
		let inputs344 = inputs344.as_ref()[..count].iter();
		let inputs345 = inputs345.as_ref()[..count].iter();
		let inputs346 = inputs346.as_ref()[..count].iter();
		let inputs347 = inputs347.as_ref()[..count].iter();
		let inputs348 = inputs348.as_ref()[..count].iter();
		let inputs349 = inputs349.as_ref()[..count].iter();
		let inputs350 = inputs350.as_ref()[..count].iter();
		let inputs351 = inputs351.as_ref()[..count].iter();
		let inputs352 = inputs352.as_ref()[..count].iter();
		let inputs353 = inputs353.as_ref()[..count].iter();
		let inputs354 = inputs354.as_ref()[..count].iter();
		let inputs355 = inputs355.as_ref()[..count].iter();
		let inputs356 = inputs356.as_ref()[..count].iter();
		let inputs357 = inputs357.as_ref()[..count].iter();
		let inputs358 = inputs358.as_ref()[..count].iter();
		let inputs359 = inputs359.as_ref()[..count].iter();
		let inputs360 = inputs360.as_ref()[..count].iter();
		let inputs361 = inputs361.as_ref()[..count].iter();
		let inputs362 = inputs362.as_ref()[..count].iter();
		let inputs363 = inputs363.as_ref()[..count].iter();
		let inputs364 = inputs364.as_ref()[..count].iter();
		let inputs365 = inputs365.as_ref()[..count].iter();
		let inputs366 = inputs366.as_ref()[..count].iter();
		let inputs367 = inputs367.as_ref()[..count].iter();
		let inputs368 = inputs368.as_ref()[..count].iter();
		let inputs369 = inputs369.as_ref()[..count].iter();
		let inputs370 = inputs370.as_ref()[..count].iter();
		let inputs371 = inputs371.as_ref()[..count].iter();
		let inputs372 = inputs372.as_ref()[..count].iter();
		let inputs373 = inputs373.as_ref()[..count].iter();
		let inputs374 = inputs374.as_ref()[..count].iter();
		let inputs375 = inputs375.as_ref()[..count].iter();
		let inputs376 = inputs376.as_ref()[..count].iter();
		let inputs377 = inputs377.as_ref()[..count].iter();
		let inputs378 = inputs378.as_ref()[..count].iter();
		let inputs379 = inputs379.as_ref()[..count].iter();
		let inputs380 = inputs380.as_ref()[..count].iter();
		let inputs381 = inputs381.as_ref()[..count].iter();
		let inputs382 = inputs382.as_ref()[..count].iter();
		let inputs383 = inputs383.as_ref()[..count].iter();
		let inputs384 = inputs384.as_ref()[..count].iter();
		let inputs385 = inputs385.as_ref()[..count].iter();
		let inputs386 = inputs386.as_ref()[..count].iter();
		let inputs387 = inputs387.as_ref()[..count].iter();
		let inputs388 = inputs388.as_ref()[..count].iter();
		let inputs389 = inputs389.as_ref()[..count].iter();
		let inputs390 = inputs390.as_ref()[..count].iter();
		let inputs391 = inputs391.as_ref()[..count].iter();
		let inputs392 = inputs392.as_ref()[..count].iter();
		let inputs393 = inputs393.as_ref()[..count].iter();
		let inputs394 = inputs394.as_ref()[..count].iter();
		let inputs395 = inputs395.as_ref()[..count].iter();
		let inputs396 = inputs396.as_ref()[..count].iter();
		let inputs397 = inputs397.as_ref()[..count].iter();
		let inputs398 = inputs398.as_ref()[..count].iter();
		let inputs399 = inputs399.as_ref()[..count].iter();
		let inputs400 = inputs400.as_ref()[..count].iter();
		let inputs401 = inputs401.as_ref()[..count].iter();
		let inputs402 = inputs402.as_ref()[..count].iter();
		let inputs403 = inputs403.as_ref()[..count].iter();
		let inputs404 = inputs404.as_ref()[..count].iter();
		let inputs405 = inputs405.as_ref()[..count].iter();
		let inputs406 = inputs406.as_ref()[..count].iter();
		let inputs407 = inputs407.as_ref()[..count].iter();
		let inputs408 = inputs408.as_ref()[..count].iter();
		let inputs409 = inputs409.as_ref()[..count].iter();
		let inputs410 = inputs410.as_ref()[..count].iter();
		let inputs411 = inputs411.as_ref()[..count].iter();
		let inputs412 = inputs412.as_ref()[..count].iter();
		let inputs413 = inputs413.as_ref()[..count].iter();
		let inputs414 = inputs414.as_ref()[..count].iter();
		let inputs415 = inputs415.as_ref()[..count].iter();
		let inputs416 = inputs416.as_ref()[..count].iter();
		let inputs417 = inputs417.as_ref()[..count].iter();
		let inputs418 = inputs418.as_ref()[..count].iter();
		let inputs419 = inputs419.as_ref()[..count].iter();
		let inputs420 = inputs420.as_ref()[..count].iter();
		let inputs421 = inputs421.as_ref()[..count].iter();
		let inputs422 = inputs422.as_ref()[..count].iter();
		let inputs423 = inputs423.as_ref()[..count].iter();
		let inputs424 = inputs424.as_ref()[..count].iter();
		let inputs425 = inputs425.as_ref()[..count].iter();
		let inputs426 = inputs426.as_ref()[..count].iter();
		let inputs427 = inputs427.as_ref()[..count].iter();
		let inputs428 = inputs428.as_ref()[..count].iter();
		let inputs429 = inputs429.as_ref()[..count].iter();
		let inputs430 = inputs430.as_ref()[..count].iter();
		let inputs431 = inputs431.as_ref()[..count].iter();
		let inputs432 = inputs432.as_ref()[..count].iter();
		let inputs433 = inputs433.as_ref()[..count].iter();
		let inputs434 = inputs434.as_ref()[..count].iter();
		let inputs435 = inputs435.as_ref()[..count].iter();
		let inputs436 = inputs436.as_ref()[..count].iter();
		let inputs437 = inputs437.as_ref()[..count].iter();
		let inputs438 = inputs438.as_ref()[..count].iter();
		let inputs439 = inputs439.as_ref()[..count].iter();
		let inputs440 = inputs440.as_ref()[..count].iter();
		let inputs441 = inputs441.as_ref()[..count].iter();
		let inputs442 = inputs442.as_ref()[..count].iter();
		let inputs443 = inputs443.as_ref()[..count].iter();
		let inputs444 = inputs444.as_ref()[..count].iter();
		let inputs445 = inputs445.as_ref()[..count].iter();
		let inputs446 = inputs446.as_ref()[..count].iter();
		let inputs447 = inputs447.as_ref()[..count].iter();
		let inputs448 = inputs448.as_ref()[..count].iter();
		let inputs449 = inputs449.as_ref()[..count].iter();
		let inputs450 = inputs450.as_ref()[..count].iter();
		let inputs451 = inputs451.as_ref()[..count].iter();
		let inputs452 = inputs452.as_ref()[..count].iter();
		let inputs453 = inputs453.as_ref()[..count].iter();
		let inputs454 = inputs454.as_ref()[..count].iter();
		let inputs455 = inputs455.as_ref()[..count].iter();
		let inputs456 = inputs456.as_ref()[..count].iter();
		let inputs457 = inputs457.as_ref()[..count].iter();
		let inputs458 = inputs458.as_ref()[..count].iter();
		let inputs459 = inputs459.as_ref()[..count].iter();
		let inputs460 = inputs460.as_ref()[..count].iter();
		let inputs461 = inputs461.as_ref()[..count].iter();
		let inputs462 = inputs462.as_ref()[..count].iter();
		let inputs463 = inputs463.as_ref()[..count].iter();
		let inputs464 = inputs464.as_ref()[..count].iter();
		let inputs465 = inputs465.as_ref()[..count].iter();
		let inputs466 = inputs466.as_ref()[..count].iter();
		let inputs467 = inputs467.as_ref()[..count].iter();
		let inputs468 = inputs468.as_ref()[..count].iter();
		let inputs469 = inputs469.as_ref()[..count].iter();
		let inputs470 = inputs470.as_ref()[..count].iter();
		let inputs471 = inputs471.as_ref()[..count].iter();
		let inputs472 = inputs472.as_ref()[..count].iter();
		let inputs473 = inputs473.as_ref()[..count].iter();
		let inputs474 = inputs474.as_ref()[..count].iter();
		let inputs475 = inputs475.as_ref()[..count].iter();
		let inputs476 = inputs476.as_ref()[..count].iter();
		let inputs477 = inputs477.as_ref()[..count].iter();
		let inputs478 = inputs478.as_ref()[..count].iter();
		let inputs479 = inputs479.as_ref()[..count].iter();
		let inputs480 = inputs480.as_ref()[..count].iter();
		let inputs481 = inputs481.as_ref()[..count].iter();
		let inputs482 = inputs482.as_ref()[..count].iter();
		let inputs483 = inputs483.as_ref()[..count].iter();
		let inputs484 = inputs484.as_ref()[..count].iter();
		let inputs485 = inputs485.as_ref()[..count].iter();
		let inputs486 = inputs486.as_ref()[..count].iter();
		let inputs487 = inputs487.as_ref()[..count].iter();
		let inputs488 = inputs488.as_ref()[..count].iter();
		let inputs489 = inputs489.as_ref()[..count].iter();
		let inputs490 = inputs490.as_ref()[..count].iter();
		let inputs491 = inputs491.as_ref()[..count].iter();
		let inputs492 = inputs492.as_ref()[..count].iter();
		let inputs493 = inputs493.as_ref()[..count].iter();
		let inputs494 = inputs494.as_ref()[..count].iter();
		let inputs495 = inputs495.as_ref()[..count].iter();
		let inputs496 = inputs496.as_ref()[..count].iter();
		let inputs497 = inputs497.as_ref()[..count].iter();
		let inputs498 = inputs498.as_ref()[..count].iter();
		let inputs499 = inputs499.as_ref()[..count].iter();
		let inputs500 = inputs500.as_ref()[..count].iter();
		let inputs501 = inputs501.as_ref()[..count].iter();
		let inputs502 = inputs502.as_ref()[..count].iter();
		let inputs503 = inputs503.as_ref()[..count].iter();
		let inputs504 = inputs504.as_ref()[..count].iter();
		let inputs505 = inputs505.as_ref()[..count].iter();
		let inputs506 = inputs506.as_ref()[..count].iter();
		let inputs507 = inputs507.as_ref()[..count].iter();
		let inputs508 = inputs508.as_ref()[..count].iter();
		let inputs509 = inputs509.as_ref()[..count].iter();
		let inputs510 = inputs510.as_ref()[..count].iter();
		let inputs511 = inputs511.as_ref()[..count].iter();
		let inputs512 = inputs512.as_ref()[..count].iter();
		let inputs513 = inputs513.as_ref()[..count].iter();
		let inputs514 = inputs514.as_ref()[..count].iter();
		let inputs515 = inputs515.as_ref()[..count].iter();
		let inputs516 = inputs516.as_ref()[..count].iter();
		let inputs517 = inputs517.as_ref()[..count].iter();
		let inputs518 = inputs518.as_ref()[..count].iter();
		let inputs519 = inputs519.as_ref()[..count].iter();
		let inputs520 = inputs520.as_ref()[..count].iter();
		let inputs521 = inputs521.as_ref()[..count].iter();
		let inputs522 = inputs522.as_ref()[..count].iter();
		let inputs523 = inputs523.as_ref()[..count].iter();
		let inputs524 = inputs524.as_ref()[..count].iter();
		let inputs525 = inputs525.as_ref()[..count].iter();
		let inputs526 = inputs526.as_ref()[..count].iter();
		let inputs527 = inputs527.as_ref()[..count].iter();
		let inputs528 = inputs528.as_ref()[..count].iter();
		let inputs529 = inputs529.as_ref()[..count].iter();
		let inputs530 = inputs530.as_ref()[..count].iter();
		let inputs531 = inputs531.as_ref()[..count].iter();
		let inputs532 = inputs532.as_ref()[..count].iter();
		let inputs533 = inputs533.as_ref()[..count].iter();
		let inputs534 = inputs534.as_ref()[..count].iter();
		let inputs535 = inputs535.as_ref()[..count].iter();
		let inputs536 = inputs536.as_ref()[..count].iter();
		let inputs537 = inputs537.as_ref()[..count].iter();
		let inputs538 = inputs538.as_ref()[..count].iter();
		let inputs539 = inputs539.as_ref()[..count].iter();
		let inputs540 = inputs540.as_ref()[..count].iter();
		let inputs541 = inputs541.as_ref()[..count].iter();
		let inputs542 = inputs542.as_ref()[..count].iter();
		let inputs543 = inputs543.as_ref()[..count].iter();
		let inputs544 = inputs544.as_ref()[..count].iter();
		let inputs545 = inputs545.as_ref()[..count].iter();
		let inputs546 = inputs546.as_ref()[..count].iter();
		let inputs547 = inputs547.as_ref()[..count].iter();
		let inputs548 = inputs548.as_ref()[..count].iter();
		let inputs549 = inputs549.as_ref()[..count].iter();
		let inputs550 = inputs550.as_ref()[..count].iter();
		let inputs551 = inputs551.as_ref()[..count].iter();
		let inputs552 = inputs552.as_ref()[..count].iter();
		let inputs553 = inputs553.as_ref()[..count].iter();
		let inputs554 = inputs554.as_ref()[..count].iter();
		let inputs555 = inputs555.as_ref()[..count].iter();
		let inputs556 = inputs556.as_ref()[..count].iter();
		let inputs557 = inputs557.as_ref()[..count].iter();
		let inputs558 = inputs558.as_ref()[..count].iter();
		let inputs559 = inputs559.as_ref()[..count].iter();
		let inputs560 = inputs560.as_ref()[..count].iter();
		let inputs561 = inputs561.as_ref()[..count].iter();
		let inputs562 = inputs562.as_ref()[..count].iter();
		let inputs563 = inputs563.as_ref()[..count].iter();
		let inputs564 = inputs564.as_ref()[..count].iter();
		let inputs565 = inputs565.as_ref()[..count].iter();
		let inputs566 = inputs566.as_ref()[..count].iter();
		let inputs567 = inputs567.as_ref()[..count].iter();
		let inputs568 = inputs568.as_ref()[..count].iter();
		let inputs569 = inputs569.as_ref()[..count].iter();
		let inputs570 = inputs570.as_ref()[..count].iter();
		let inputs571 = inputs571.as_ref()[..count].iter();
		let inputs572 = inputs572.as_ref()[..count].iter();
		let inputs573 = inputs573.as_ref()[..count].iter();
		let inputs574 = inputs574.as_ref()[..count].iter();
		let inputs575 = inputs575.as_ref()[..count].iter();
		let inputs576 = inputs576.as_ref()[..count].iter();
		let inputs577 = inputs577.as_ref()[..count].iter();
		let inputs578 = inputs578.as_ref()[..count].iter();
		let inputs579 = inputs579.as_ref()[..count].iter();
		let inputs580 = inputs580.as_ref()[..count].iter();
		let inputs581 = inputs581.as_ref()[..count].iter();
		let inputs582 = inputs582.as_ref()[..count].iter();
		let inputs583 = inputs583.as_ref()[..count].iter();
		let inputs584 = inputs584.as_ref()[..count].iter();
		let inputs585 = inputs585.as_ref()[..count].iter();
		let inputs586 = inputs586.as_ref()[..count].iter();
		let inputs587 = inputs587.as_ref()[..count].iter();
		let inputs588 = inputs588.as_ref()[..count].iter();
		let inputs589 = inputs589.as_ref()[..count].iter();
		let inputs590 = inputs590.as_ref()[..count].iter();
		let inputs591 = inputs591.as_ref()[..count].iter();
		let inputs592 = inputs592.as_ref()[..count].iter();
		let inputs593 = inputs593.as_ref()[..count].iter();
		let inputs594 = inputs594.as_ref()[..count].iter();
		let inputs595 = inputs595.as_ref()[..count].iter();
		let inputs596 = inputs596.as_ref()[..count].iter();
		let inputs597 = inputs597.as_ref()[..count].iter();
		let inputs598 = inputs598.as_ref()[..count].iter();
		let inputs599 = inputs599.as_ref()[..count].iter();
		let inputs600 = inputs600.as_ref()[..count].iter();
		let inputs601 = inputs601.as_ref()[..count].iter();
		let inputs602 = inputs602.as_ref()[..count].iter();
		let inputs603 = inputs603.as_ref()[..count].iter();
		let inputs604 = inputs604.as_ref()[..count].iter();
		let inputs605 = inputs605.as_ref()[..count].iter();
		let inputs606 = inputs606.as_ref()[..count].iter();
		let inputs607 = inputs607.as_ref()[..count].iter();
		let inputs608 = inputs608.as_ref()[..count].iter();
		let inputs609 = inputs609.as_ref()[..count].iter();
		let inputs610 = inputs610.as_ref()[..count].iter();
		let inputs611 = inputs611.as_ref()[..count].iter();
		let inputs612 = inputs612.as_ref()[..count].iter();
		let inputs613 = inputs613.as_ref()[..count].iter();
		let inputs614 = inputs614.as_ref()[..count].iter();
		let inputs615 = inputs615.as_ref()[..count].iter();
		let inputs616 = inputs616.as_ref()[..count].iter();
		let inputs617 = inputs617.as_ref()[..count].iter();
		let inputs618 = inputs618.as_ref()[..count].iter();
		let inputs619 = inputs619.as_ref()[..count].iter();
		let inputs620 = inputs620.as_ref()[..count].iter();
		let inputs621 = inputs621.as_ref()[..count].iter();
		let inputs622 = inputs622.as_ref()[..count].iter();
		let inputs623 = inputs623.as_ref()[..count].iter();
		let inputs624 = inputs624.as_ref()[..count].iter();
		let inputs625 = inputs625.as_ref()[..count].iter();
		let inputs626 = inputs626.as_ref()[..count].iter();
		let inputs627 = inputs627.as_ref()[..count].iter();
		let inputs628 = inputs628.as_ref()[..count].iter();
		let inputs629 = inputs629.as_ref()[..count].iter();
		let inputs630 = inputs630.as_ref()[..count].iter();
		let inputs631 = inputs631.as_ref()[..count].iter();
		let inputs632 = inputs632.as_ref()[..count].iter();
		let inputs633 = inputs633.as_ref()[..count].iter();
		let inputs634 = inputs634.as_ref()[..count].iter();
		let inputs635 = inputs635.as_ref()[..count].iter();
		let inputs636 = inputs636.as_ref()[..count].iter();
		let inputs637 = inputs637.as_ref()[..count].iter();
		let inputs638 = inputs638.as_ref()[..count].iter();
		let inputs639 = inputs639.as_ref()[..count].iter();
		let inputs640 = inputs640.as_ref()[..count].iter();
		let inputs641 = inputs641.as_ref()[..count].iter();
		let inputs642 = inputs642.as_ref()[..count].iter();
		let inputs643 = inputs643.as_ref()[..count].iter();
		let inputs644 = inputs644.as_ref()[..count].iter();
		let inputs645 = inputs645.as_ref()[..count].iter();
		let inputs646 = inputs646.as_ref()[..count].iter();
		let inputs647 = inputs647.as_ref()[..count].iter();
		let inputs648 = inputs648.as_ref()[..count].iter();
		let inputs649 = inputs649.as_ref()[..count].iter();
		let inputs650 = inputs650.as_ref()[..count].iter();
		let inputs651 = inputs651.as_ref()[..count].iter();
		let inputs652 = inputs652.as_ref()[..count].iter();
		let inputs653 = inputs653.as_ref()[..count].iter();
		let inputs654 = inputs654.as_ref()[..count].iter();
		let inputs655 = inputs655.as_ref()[..count].iter();
		let inputs656 = inputs656.as_ref()[..count].iter();
		let inputs657 = inputs657.as_ref()[..count].iter();
		let inputs658 = inputs658.as_ref()[..count].iter();
		let inputs659 = inputs659.as_ref()[..count].iter();
		let inputs660 = inputs660.as_ref()[..count].iter();
		let inputs661 = inputs661.as_ref()[..count].iter();
		let inputs662 = inputs662.as_ref()[..count].iter();
		let inputs663 = inputs663.as_ref()[..count].iter();
		let inputs664 = inputs664.as_ref()[..count].iter();
		let inputs665 = inputs665.as_ref()[..count].iter();
		let inputs666 = inputs666.as_ref()[..count].iter();
		let inputs667 = inputs667.as_ref()[..count].iter();
		let inputs668 = inputs668.as_ref()[..count].iter();
		let inputs669 = inputs669.as_ref()[..count].iter();
		let inputs670 = inputs670.as_ref()[..count].iter();
		let inputs671 = inputs671.as_ref()[..count].iter();
		let inputs672 = inputs672.as_ref()[..count].iter();
		let inputs673 = inputs673.as_ref()[..count].iter();
		let inputs674 = inputs674.as_ref()[..count].iter();
		let inputs675 = inputs675.as_ref()[..count].iter();
		let inputs676 = inputs676.as_ref()[..count].iter();
		let inputs677 = inputs677.as_ref()[..count].iter();
		let inputs678 = inputs678.as_ref()[..count].iter();
		let inputs679 = inputs679.as_ref()[..count].iter();
		let inputs680 = inputs680.as_ref()[..count].iter();
		let inputs681 = inputs681.as_ref()[..count].iter();
		let inputs682 = inputs682.as_ref()[..count].iter();
		let inputs683 = inputs683.as_ref()[..count].iter();
		let inputs684 = inputs684.as_ref()[..count].iter();
		let inputs685 = inputs685.as_ref()[..count].iter();
		let inputs686 = inputs686.as_ref()[..count].iter();
		let inputs687 = inputs687.as_ref()[..count].iter();
		let inputs688 = inputs688.as_ref()[..count].iter();
		let inputs689 = inputs689.as_ref()[..count].iter();
		let inputs690 = inputs690.as_ref()[..count].iter();
		let inputs691 = inputs691.as_ref()[..count].iter();
		let inputs692 = inputs692.as_ref()[..count].iter();
		let inputs693 = inputs693.as_ref()[..count].iter();
		let inputs694 = inputs694.as_ref()[..count].iter();
		let inputs695 = inputs695.as_ref()[..count].iter();
		let inputs696 = inputs696.as_ref()[..count].iter();
		let inputs697 = inputs697.as_ref()[..count].iter();
		let inputs698 = inputs698.as_ref()[..count].iter();
		let inputs699 = inputs699.as_ref()[..count].iter();
		let inputs700 = inputs700.as_ref()[..count].iter();
		let inputs701 = inputs701.as_ref()[..count].iter();
		let inputs702 = inputs702.as_ref()[..count].iter();
		let inputs703 = inputs703.as_ref()[..count].iter();
		let inputs704 = inputs704.as_ref()[..count].iter();
		let inputs705 = inputs705.as_ref()[..count].iter();
		let inputs706 = inputs706.as_ref()[..count].iter();
		let inputs707 = inputs707.as_ref()[..count].iter();
		let inputs708 = inputs708.as_ref()[..count].iter();
		let inputs709 = inputs709.as_ref()[..count].iter();
		let inputs710 = inputs710.as_ref()[..count].iter();
		let inputs711 = inputs711.as_ref()[..count].iter();
		let inputs712 = inputs712.as_ref()[..count].iter();
		let inputs713 = inputs713.as_ref()[..count].iter();
		let inputs714 = inputs714.as_ref()[..count].iter();
		let inputs715 = inputs715.as_ref()[..count].iter();
		let inputs716 = inputs716.as_ref()[..count].iter();
		let inputs717 = inputs717.as_ref()[..count].iter();
		let inputs718 = inputs718.as_ref()[..count].iter();
		let inputs719 = inputs719.as_ref()[..count].iter();
		let inputs720 = inputs720.as_ref()[..count].iter();
		let inputs721 = inputs721.as_ref()[..count].iter();
		let inputs722 = inputs722.as_ref()[..count].iter();
		let inputs723 = inputs723.as_ref()[..count].iter();
		let inputs724 = inputs724.as_ref()[..count].iter();
		let inputs725 = inputs725.as_ref()[..count].iter();
		let inputs726 = inputs726.as_ref()[..count].iter();
		let inputs727 = inputs727.as_ref()[..count].iter();
		let inputs728 = inputs728.as_ref()[..count].iter();
		let inputs729 = inputs729.as_ref()[..count].iter();
		let inputs730 = inputs730.as_ref()[..count].iter();
		let inputs731 = inputs731.as_ref()[..count].iter();
		let inputs732 = inputs732.as_ref()[..count].iter();
		let inputs733 = inputs733.as_ref()[..count].iter();
		let inputs734 = inputs734.as_ref()[..count].iter();
		let inputs735 = inputs735.as_ref()[..count].iter();
		let [outputs0, outputs1, .. ] = outputs.as_mut() else { panic!("wrong number of output buffers"); };
		let outputs0 = outputs0.as_mut()[..count].iter_mut();
		let outputs1 = outputs1.as_mut()[..count].iter_mut();
		let mut iSlow0: i32 = (self.fHslider0) as i32;
		let mut fSlow1: F32 = self.fConst185 * self.fHslider1;
		let mut fSlow2: F32 = self.fConst185 * self.fHslider2;
		let mut fSlow3: F32 = self.fHslider3;
		let zipped_iterators = inputs0.zip(inputs1).zip(inputs2).zip(inputs3).zip(inputs4).zip(inputs5).zip(inputs6).zip(inputs7).zip(inputs8).zip(inputs9).zip(inputs10).zip(inputs11).zip(inputs12).zip(inputs13).zip(inputs14).zip(inputs15).zip(inputs16).zip(inputs17).zip(inputs18).zip(inputs19).zip(inputs20).zip(inputs21).zip(inputs22).zip(inputs23).zip(inputs24).zip(inputs25).zip(inputs26).zip(inputs27).zip(inputs28).zip(inputs29).zip(inputs30).zip(inputs31).zip(inputs32).zip(inputs33).zip(inputs34).zip(inputs35).zip(inputs36).zip(inputs37).zip(inputs38).zip(inputs39).zip(inputs40).zip(inputs41).zip(inputs42).zip(inputs43).zip(inputs44).zip(inputs45).zip(inputs46).zip(inputs47).zip(inputs48).zip(inputs49).zip(inputs50).zip(inputs51).zip(inputs52).zip(inputs53).zip(inputs54).zip(inputs55).zip(inputs56).zip(inputs57).zip(inputs58).zip(inputs59).zip(inputs60).zip(inputs61).zip(inputs62).zip(inputs63).zip(inputs64).zip(inputs65).zip(inputs66).zip(inputs67).zip(inputs68).zip(inputs69).zip(inputs70).zip(inputs71).zip(inputs72).zip(inputs73).zip(inputs74).zip(inputs75).zip(inputs76).zip(inputs77).zip(inputs78).zip(inputs79).zip(inputs80).zip(inputs81).zip(inputs82).zip(inputs83).zip(inputs84).zip(inputs85).zip(inputs86).zip(inputs87).zip(inputs88).zip(inputs89).zip(inputs90).zip(inputs91).zip(inputs92).zip(inputs93).zip(inputs94).zip(inputs95).zip(inputs96).zip(inputs97).zip(inputs98).zip(inputs99).zip(inputs100).zip(inputs101).zip(inputs102).zip(inputs103).zip(inputs104).zip(inputs105).zip(inputs106).zip(inputs107).zip(inputs108).zip(inputs109).zip(inputs110).zip(inputs111).zip(inputs112).zip(inputs113).zip(inputs114).zip(inputs115).zip(inputs116).zip(inputs117).zip(inputs118).zip(inputs119).zip(inputs120).zip(inputs121).zip(inputs122).zip(inputs123).zip(inputs124).zip(inputs125).zip(inputs126).zip(inputs127).zip(inputs128).zip(inputs129).zip(inputs130).zip(inputs131).zip(inputs132).zip(inputs133).zip(inputs134).zip(inputs135).zip(inputs136).zip(inputs137).zip(inputs138).zip(inputs139).zip(inputs140).zip(inputs141).zip(inputs142).zip(inputs143).zip(inputs144).zip(inputs145).zip(inputs146).zip(inputs147).zip(inputs148).zip(inputs149).zip(inputs150).zip(inputs151).zip(inputs152).zip(inputs153).zip(inputs154).zip(inputs155).zip(inputs156).zip(inputs157).zip(inputs158).zip(inputs159).zip(inputs160).zip(inputs161).zip(inputs162).zip(inputs163).zip(inputs164).zip(inputs165).zip(inputs166).zip(inputs167).zip(inputs168).zip(inputs169).zip(inputs170).zip(inputs171).zip(inputs172).zip(inputs173).zip(inputs174).zip(inputs175).zip(inputs176).zip(inputs177).zip(inputs178).zip(inputs179).zip(inputs180).zip(inputs181).zip(inputs182).zip(inputs183).zip(inputs184).zip(inputs185).zip(inputs186).zip(inputs187).zip(inputs188).zip(inputs189).zip(inputs190).zip(inputs191).zip(inputs192).zip(inputs193).zip(inputs194).zip(inputs195).zip(inputs196).zip(inputs197).zip(inputs198).zip(inputs199).zip(inputs200).zip(inputs201).zip(inputs202).zip(inputs203).zip(inputs204).zip(inputs205).zip(inputs206).zip(inputs207).zip(inputs208).zip(inputs209).zip(inputs210).zip(inputs211).zip(inputs212).zip(inputs213).zip(inputs214).zip(inputs215).zip(inputs216).zip(inputs217).zip(inputs218).zip(inputs219).zip(inputs220).zip(inputs221).zip(inputs222).zip(inputs223).zip(inputs224).zip(inputs225).zip(inputs226).zip(inputs227).zip(inputs228).zip(inputs229).zip(inputs230).zip(inputs231).zip(inputs232).zip(inputs233).zip(inputs234).zip(inputs235).zip(inputs236).zip(inputs237).zip(inputs238).zip(inputs239).zip(inputs240).zip(inputs241).zip(inputs242).zip(inputs243).zip(inputs244).zip(inputs245).zip(inputs246).zip(inputs247).zip(inputs248).zip(inputs249).zip(inputs250).zip(inputs251).zip(inputs252).zip(inputs253).zip(inputs254).zip(inputs255).zip(inputs256).zip(inputs257).zip(inputs258).zip(inputs259).zip(inputs260).zip(inputs261).zip(inputs262).zip(inputs263).zip(inputs264).zip(inputs265).zip(inputs266).zip(inputs267).zip(inputs268).zip(inputs269).zip(inputs270).zip(inputs271).zip(inputs272).zip(inputs273).zip(inputs274).zip(inputs275).zip(inputs276).zip(inputs277).zip(inputs278).zip(inputs279).zip(inputs280).zip(inputs281).zip(inputs282).zip(inputs283).zip(inputs284).zip(inputs285).zip(inputs286).zip(inputs287).zip(inputs288).zip(inputs289).zip(inputs290).zip(inputs291).zip(inputs292).zip(inputs293).zip(inputs294).zip(inputs295).zip(inputs296).zip(inputs297).zip(inputs298).zip(inputs299).zip(inputs300).zip(inputs301).zip(inputs302).zip(inputs303).zip(inputs304).zip(inputs305).zip(inputs306).zip(inputs307).zip(inputs308).zip(inputs309).zip(inputs310).zip(inputs311).zip(inputs312).zip(inputs313).zip(inputs314).zip(inputs315).zip(inputs316).zip(inputs317).zip(inputs318).zip(inputs319).zip(inputs320).zip(inputs321).zip(inputs322).zip(inputs323).zip(inputs324).zip(inputs325).zip(inputs326).zip(inputs327).zip(inputs328).zip(inputs329).zip(inputs330).zip(inputs331).zip(inputs332).zip(inputs333).zip(inputs334).zip(inputs335).zip(inputs336).zip(inputs337).zip(inputs338).zip(inputs339).zip(inputs340).zip(inputs341).zip(inputs342).zip(inputs343).zip(inputs344).zip(inputs345).zip(inputs346).zip(inputs347).zip(inputs348).zip(inputs349).zip(inputs350).zip(inputs351).zip(inputs352).zip(inputs353).zip(inputs354).zip(inputs355).zip(inputs356).zip(inputs357).zip(inputs358).zip(inputs359).zip(inputs360).zip(inputs361).zip(inputs362).zip(inputs363).zip(inputs364).zip(inputs365).zip(inputs366).zip(inputs367).zip(inputs368).zip(inputs369).zip(inputs370).zip(inputs371).zip(inputs372).zip(inputs373).zip(inputs374).zip(inputs375).zip(inputs376).zip(inputs377).zip(inputs378).zip(inputs379).zip(inputs380).zip(inputs381).zip(inputs382).zip(inputs383).zip(inputs384).zip(inputs385).zip(inputs386).zip(inputs387).zip(inputs388).zip(inputs389).zip(inputs390).zip(inputs391).zip(inputs392).zip(inputs393).zip(inputs394).zip(inputs395).zip(inputs396).zip(inputs397).zip(inputs398).zip(inputs399).zip(inputs400).zip(inputs401).zip(inputs402).zip(inputs403).zip(inputs404).zip(inputs405).zip(inputs406).zip(inputs407).zip(inputs408).zip(inputs409).zip(inputs410).zip(inputs411).zip(inputs412).zip(inputs413).zip(inputs414).zip(inputs415).zip(inputs416).zip(inputs417).zip(inputs418).zip(inputs419).zip(inputs420).zip(inputs421).zip(inputs422).zip(inputs423).zip(inputs424).zip(inputs425).zip(inputs426).zip(inputs427).zip(inputs428).zip(inputs429).zip(inputs430).zip(inputs431).zip(inputs432).zip(inputs433).zip(inputs434).zip(inputs435).zip(inputs436).zip(inputs437).zip(inputs438).zip(inputs439).zip(inputs440).zip(inputs441).zip(inputs442).zip(inputs443).zip(inputs444).zip(inputs445).zip(inputs446).zip(inputs447).zip(inputs448).zip(inputs449).zip(inputs450).zip(inputs451).zip(inputs452).zip(inputs453).zip(inputs454).zip(inputs455).zip(inputs456).zip(inputs457).zip(inputs458).zip(inputs459).zip(inputs460).zip(inputs461).zip(inputs462).zip(inputs463).zip(inputs464).zip(inputs465).zip(inputs466).zip(inputs467).zip(inputs468).zip(inputs469).zip(inputs470).zip(inputs471).zip(inputs472).zip(inputs473).zip(inputs474).zip(inputs475).zip(inputs476).zip(inputs477).zip(inputs478).zip(inputs479).zip(inputs480).zip(inputs481).zip(inputs482).zip(inputs483).zip(inputs484).zip(inputs485).zip(inputs486).zip(inputs487).zip(inputs488).zip(inputs489).zip(inputs490).zip(inputs491).zip(inputs492).zip(inputs493).zip(inputs494).zip(inputs495).zip(inputs496).zip(inputs497).zip(inputs498).zip(inputs499).zip(inputs500).zip(inputs501).zip(inputs502).zip(inputs503).zip(inputs504).zip(inputs505).zip(inputs506).zip(inputs507).zip(inputs508).zip(inputs509).zip(inputs510).zip(inputs511).zip(inputs512).zip(inputs513).zip(inputs514).zip(inputs515).zip(inputs516).zip(inputs517).zip(inputs518).zip(inputs519).zip(inputs520).zip(inputs521).zip(inputs522).zip(inputs523).zip(inputs524).zip(inputs525).zip(inputs526).zip(inputs527).zip(inputs528).zip(inputs529).zip(inputs530).zip(inputs531).zip(inputs532).zip(inputs533).zip(inputs534).zip(inputs535).zip(inputs536).zip(inputs537).zip(inputs538).zip(inputs539).zip(inputs540).zip(inputs541).zip(inputs542).zip(inputs543).zip(inputs544).zip(inputs545).zip(inputs546).zip(inputs547).zip(inputs548).zip(inputs549).zip(inputs550).zip(inputs551).zip(inputs552).zip(inputs553).zip(inputs554).zip(inputs555).zip(inputs556).zip(inputs557).zip(inputs558).zip(inputs559).zip(inputs560).zip(inputs561).zip(inputs562).zip(inputs563).zip(inputs564).zip(inputs565).zip(inputs566).zip(inputs567).zip(inputs568).zip(inputs569).zip(inputs570).zip(inputs571).zip(inputs572).zip(inputs573).zip(inputs574).zip(inputs575).zip(inputs576).zip(inputs577).zip(inputs578).zip(inputs579).zip(inputs580).zip(inputs581).zip(inputs582).zip(inputs583).zip(inputs584).zip(inputs585).zip(inputs586).zip(inputs587).zip(inputs588).zip(inputs589).zip(inputs590).zip(inputs591).zip(inputs592).zip(inputs593).zip(inputs594).zip(inputs595).zip(inputs596).zip(inputs597).zip(inputs598).zip(inputs599).zip(inputs600).zip(inputs601).zip(inputs602).zip(inputs603).zip(inputs604).zip(inputs605).zip(inputs606).zip(inputs607).zip(inputs608).zip(inputs609).zip(inputs610).zip(inputs611).zip(inputs612).zip(inputs613).zip(inputs614).zip(inputs615).zip(inputs616).zip(inputs617).zip(inputs618).zip(inputs619).zip(inputs620).zip(inputs621).zip(inputs622).zip(inputs623).zip(inputs624).zip(inputs625).zip(inputs626).zip(inputs627).zip(inputs628).zip(inputs629).zip(inputs630).zip(inputs631).zip(inputs632).zip(inputs633).zip(inputs634).zip(inputs635).zip(inputs636).zip(inputs637).zip(inputs638).zip(inputs639).zip(inputs640).zip(inputs641).zip(inputs642).zip(inputs643).zip(inputs644).zip(inputs645).zip(inputs646).zip(inputs647).zip(inputs648).zip(inputs649).zip(inputs650).zip(inputs651).zip(inputs652).zip(inputs653).zip(inputs654).zip(inputs655).zip(inputs656).zip(inputs657).zip(inputs658).zip(inputs659).zip(inputs660).zip(inputs661).zip(inputs662).zip(inputs663).zip(inputs664).zip(inputs665).zip(inputs666).zip(inputs667).zip(inputs668).zip(inputs669).zip(inputs670).zip(inputs671).zip(inputs672).zip(inputs673).zip(inputs674).zip(inputs675).zip(inputs676).zip(inputs677).zip(inputs678).zip(inputs679).zip(inputs680).zip(inputs681).zip(inputs682).zip(inputs683).zip(inputs684).zip(inputs685).zip(inputs686).zip(inputs687).zip(inputs688).zip(inputs689).zip(inputs690).zip(inputs691).zip(inputs692).zip(inputs693).zip(inputs694).zip(inputs695).zip(inputs696).zip(inputs697).zip(inputs698).zip(inputs699).zip(inputs700).zip(inputs701).zip(inputs702).zip(inputs703).zip(inputs704).zip(inputs705).zip(inputs706).zip(inputs707).zip(inputs708).zip(inputs709).zip(inputs710).zip(inputs711).zip(inputs712).zip(inputs713).zip(inputs714).zip(inputs715).zip(inputs716).zip(inputs717).zip(inputs718).zip(inputs719).zip(inputs720).zip(inputs721).zip(inputs722).zip(inputs723).zip(inputs724).zip(inputs725).zip(inputs726).zip(inputs727).zip(inputs728).zip(inputs729).zip(inputs730).zip(inputs731).zip(inputs732).zip(inputs733).zip(inputs734).zip(inputs735).zip(outputs0).zip(outputs1);
		for (((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((input0, input1), input2), input3), input4), input5), input6), input7), input8), input9), input10), input11), input12), input13), input14), input15), input16), input17), input18), input19), input20), input21), input22), input23), input24), input25), input26), input27), input28), input29), input30), input31), input32), input33), input34), input35), input36), input37), input38), input39), input40), input41), input42), input43), input44), input45), input46), input47), input48), input49), input50), input51), input52), input53), input54), input55), input56), input57), input58), input59), input60), input61), input62), input63), input64), input65), input66), input67), input68), input69), input70), input71), input72), input73), input74), input75), input76), input77), input78), input79), input80), input81), input82), input83), input84), input85), input86), input87), input88), input89), input90), input91), input92), input93), input94), input95), input96), input97), input98), input99), input100), input101), input102), input103), input104), input105), input106), input107), input108), input109), input110), input111), input112), input113), input114), input115), input116), input117), input118), input119), input120), input121), input122), input123), input124), input125), input126), input127), input128), input129), input130), input131), input132), input133), input134), input135), input136), input137), input138), input139), input140), input141), input142), input143), input144), input145), input146), input147), input148), input149), input150), input151), input152), input153), input154), input155), input156), input157), input158), input159), input160), input161), input162), input163), input164), input165), input166), input167), input168), input169), input170), input171), input172), input173), input174), input175), input176), input177), input178), input179), input180), input181), input182), input183), input184), input185), input186), input187), input188), input189), input190), input191), input192), input193), input194), input195), input196), input197), input198), input199), input200), input201), input202), input203), input204), input205), input206), input207), input208), input209), input210), input211), input212), input213), input214), input215), input216), input217), input218), input219), input220), input221), input222), input223), input224), input225), input226), input227), input228), input229), input230), input231), input232), input233), input234), input235), input236), input237), input238), input239), input240), input241), input242), input243), input244), input245), input246), input247), input248), input249), input250), input251), input252), input253), input254), input255), input256), input257), input258), input259), input260), input261), input262), input263), input264), input265), input266), input267), input268), input269), input270), input271), input272), input273), input274), input275), input276), input277), input278), input279), input280), input281), input282), input283), input284), input285), input286), input287), input288), input289), input290), input291), input292), input293), input294), input295), input296), input297), input298), input299), input300), input301), input302), input303), input304), input305), input306), input307), input308), input309), input310), input311), input312), input313), input314), input315), input316), input317), input318), input319), input320), input321), input322), input323), input324), input325), input326), input327), input328), input329), input330), input331), input332), input333), input334), input335), input336), input337), input338), input339), input340), input341), input342), input343), input344), input345), input346), input347), input348), input349), input350), input351), input352), input353), input354), input355), input356), input357), input358), input359), input360), input361), input362), input363), input364), input365), input366), input367), input368), input369), input370), input371), input372), input373), input374), input375), input376), input377), input378), input379), input380), input381), input382), input383), input384), input385), input386), input387), input388), input389), input390), input391), input392), input393), input394), input395), input396), input397), input398), input399), input400), input401), input402), input403), input404), input405), input406), input407), input408), input409), input410), input411), input412), input413), input414), input415), input416), input417), input418), input419), input420), input421), input422), input423), input424), input425), input426), input427), input428), input429), input430), input431), input432), input433), input434), input435), input436), input437), input438), input439), input440), input441), input442), input443), input444), input445), input446), input447), input448), input449), input450), input451), input452), input453), input454), input455), input456), input457), input458), input459), input460), input461), input462), input463), input464), input465), input466), input467), input468), input469), input470), input471), input472), input473), input474), input475), input476), input477), input478), input479), input480), input481), input482), input483), input484), input485), input486), input487), input488), input489), input490), input491), input492), input493), input494), input495), input496), input497), input498), input499), input500), input501), input502), input503), input504), input505), input506), input507), input508), input509), input510), input511), input512), input513), input514), input515), input516), input517), input518), input519), input520), input521), input522), input523), input524), input525), input526), input527), input528), input529), input530), input531), input532), input533), input534), input535), input536), input537), input538), input539), input540), input541), input542), input543), input544), input545), input546), input547), input548), input549), input550), input551), input552), input553), input554), input555), input556), input557), input558), input559), input560), input561), input562), input563), input564), input565), input566), input567), input568), input569), input570), input571), input572), input573), input574), input575), input576), input577), input578), input579), input580), input581), input582), input583), input584), input585), input586), input587), input588), input589), input590), input591), input592), input593), input594), input595), input596), input597), input598), input599), input600), input601), input602), input603), input604), input605), input606), input607), input608), input609), input610), input611), input612), input613), input614), input615), input616), input617), input618), input619), input620), input621), input622), input623), input624), input625), input626), input627), input628), input629), input630), input631), input632), input633), input634), input635), input636), input637), input638), input639), input640), input641), input642), input643), input644), input645), input646), input647), input648), input649), input650), input651), input652), input653), input654), input655), input656), input657), input658), input659), input660), input661), input662), input663), input664), input665), input666), input667), input668), input669), input670), input671), input672), input673), input674), input675), input676), input677), input678), input679), input680), input681), input682), input683), input684), input685), input686), input687), input688), input689), input690), input691), input692), input693), input694), input695), input696), input697), input698), input699), input700), input701), input702), input703), input704), input705), input706), input707), input708), input709), input710), input711), input712), input713), input714), input715), input716), input717), input718), input719), input720), input721), input722), input723), input724), input725), input726), input727), input728), input729), input730), input731), input732), input733), input734), input735), output0), output1) in zipped_iterators {
			self.fVec0[(self.IOTA0 & 63) as usize] = *input245;
			self.fVec1[(self.IOTA0 & 63) as usize] = *input246;
			self.fVec2[(self.IOTA0 & 63) as usize] = *input244;
			self.fVec3[(self.IOTA0 & 63) as usize] = *input243;
			self.fVec4[(self.IOTA0 & 63) as usize] = *input242;
			self.fVec5[(self.IOTA0 & 63) as usize] = *input241;
			self.fVec6[(self.IOTA0 & 63) as usize] = *input240;
			self.fVec7[(self.IOTA0 & 63) as usize] = *input239;
			self.fVec8[(self.IOTA0 & 63) as usize] = *input238;
			self.fVec9[(self.IOTA0 & 63) as usize] = *input237;
			self.fVec10[(self.IOTA0 & 63) as usize] = *input236;
			self.fVec11[(self.IOTA0 & 63) as usize] = *input235;
			self.fVec12[(self.IOTA0 & 63) as usize] = *input234;
			self.fVec13[(self.IOTA0 & 63) as usize] = *input233;
			self.fVec14[(self.IOTA0 & 63) as usize] = *input232;
			self.fVec15[(self.IOTA0 & 63) as usize] = *input231;
			self.fVec16[(self.IOTA0 & 63) as usize] = *input230;
			self.fVec17[(self.IOTA0 & 63) as usize] = *input229;
			self.fVec18[(self.IOTA0 & 63) as usize] = *input228;
			self.fVec19[(self.IOTA0 & 63) as usize] = *input227;
			self.fVec20[(self.IOTA0 & 63) as usize] = *input226;
			self.fVec21[(self.IOTA0 & 63) as usize] = *input225;
			self.fVec22[(self.IOTA0 & 63) as usize] = *input224;
			self.fVec23[(self.IOTA0 & 63) as usize] = *input223;
			self.fVec24[(self.IOTA0 & 63) as usize] = *input222;
			self.fVec25[(self.IOTA0 & 63) as usize] = *input221;
			self.fVec26[(self.IOTA0 & 63) as usize] = *input220;
			self.fVec27[(self.IOTA0 & 63) as usize] = *input219;
			self.fVec28[(self.IOTA0 & 63) as usize] = *input218;
			self.fVec29[(self.IOTA0 & 63) as usize] = *input217;
			self.fVec30[(self.IOTA0 & 63) as usize] = *input216;
			self.fVec31[(self.IOTA0 & 31) as usize] = *input215;
			self.fVec32[(self.IOTA0 & 31) as usize] = *input214;
			self.fVec33[(self.IOTA0 & 31) as usize] = *input213;
			self.fVec34[(self.IOTA0 & 31) as usize] = *input212;
			self.fVec35[(self.IOTA0 & 31) as usize] = *input211;
			self.fVec36[(self.IOTA0 & 31) as usize] = *input210;
			self.fVec37[(self.IOTA0 & 31) as usize] = *input209;
			self.fVec38[(self.IOTA0 & 31) as usize] = *input208;
			self.fVec39[(self.IOTA0 & 31) as usize] = *input207;
			self.fVec40[(self.IOTA0 & 31) as usize] = *input206;
			self.fVec41[(self.IOTA0 & 31) as usize] = *input205;
			self.fVec42[(self.IOTA0 & 31) as usize] = *input204;
			self.fVec43[(self.IOTA0 & 31) as usize] = *input203;
			self.fVec44[(self.IOTA0 & 31) as usize] = *input202;
			self.fVec45[(self.IOTA0 & 31) as usize] = *input201;
			self.fVec46[(self.IOTA0 & 31) as usize] = *input200;
			self.fVec47[0] = *input199;
			self.fVec48[0] = *input198;
			self.fVec49[0] = *input197;
			self.fVec50[0] = *input196;
			self.fVec51[0] = *input195;
			self.fVec52[0] = *input194;
			self.fVec53[0] = *input193;
			self.fVec54[0] = *input192;
			self.fVec55[0] = *input191;
			self.fVec56[0] = *input190;
			self.fVec57[0] = *input189;
			self.fVec58[0] = *input188;
			self.fVec59[0] = *input187;
			self.fVec60[0] = *input186;
			self.fVec61[0] = *input185;
			self.iVec62[0] = 1;
			self.fVec63[(self.IOTA0 & 127) as usize] = *input366;
			self.fVec64[(self.IOTA0 & 127) as usize] = *input367;
			self.fVec65[(self.IOTA0 & 127) as usize] = *input365;
			self.fVec66[(self.IOTA0 & 127) as usize] = *input364;
			self.fVec67[(self.IOTA0 & 127) as usize] = *input363;
			self.fVec68[(self.IOTA0 & 127) as usize] = *input362;
			self.fVec69[(self.IOTA0 & 127) as usize] = *input361;
			self.fVec70[(self.IOTA0 & 127) as usize] = *input360;
			self.fVec71[(self.IOTA0 & 127) as usize] = *input359;
			self.fVec72[(self.IOTA0 & 127) as usize] = *input358;
			self.fVec73[(self.IOTA0 & 127) as usize] = *input357;
			self.fVec74[(self.IOTA0 & 127) as usize] = *input356;
			self.fVec75[(self.IOTA0 & 127) as usize] = *input355;
			self.fVec76[(self.IOTA0 & 127) as usize] = *input354;
			self.fVec77[(self.IOTA0 & 127) as usize] = *input353;
			self.fVec78[(self.IOTA0 & 127) as usize] = *input352;
			self.fVec79[(self.IOTA0 & 127) as usize] = *input351;
			self.fVec80[(self.IOTA0 & 127) as usize] = *input350;
			self.fVec81[(self.IOTA0 & 127) as usize] = *input349;
			self.fVec82[(self.IOTA0 & 127) as usize] = *input348;
			self.fVec83[(self.IOTA0 & 127) as usize] = *input347;
			self.fVec84[(self.IOTA0 & 127) as usize] = *input346;
			self.fVec85[(self.IOTA0 & 127) as usize] = *input345;
			self.fVec86[(self.IOTA0 & 127) as usize] = *input344;
			self.fVec87[(self.IOTA0 & 127) as usize] = *input343;
			self.fVec88[(self.IOTA0 & 127) as usize] = *input342;
			self.fVec89[(self.IOTA0 & 127) as usize] = *input341;
			self.fVec90[(self.IOTA0 & 127) as usize] = *input340;
			self.fVec91[(self.IOTA0 & 127) as usize] = *input339;
			self.fVec92[(self.IOTA0 & 127) as usize] = *input338;
			self.fVec93[(self.IOTA0 & 127) as usize] = *input337;
			self.fVec94[(self.IOTA0 & 127) as usize] = *input336;
			self.fVec95[(self.IOTA0 & 127) as usize] = *input335;
			self.fVec96[(self.IOTA0 & 127) as usize] = *input334;
			self.fVec97[(self.IOTA0 & 127) as usize] = *input333;
			self.fVec98[(self.IOTA0 & 127) as usize] = *input332;
			self.fVec99[(self.IOTA0 & 127) as usize] = *input331;
			self.fVec100[(self.IOTA0 & 127) as usize] = *input330;
			self.fVec101[(self.IOTA0 & 127) as usize] = *input329;
			self.fVec102[(self.IOTA0 & 127) as usize] = *input328;
			self.fVec103[(self.IOTA0 & 127) as usize] = *input327;
			self.fVec104[(self.IOTA0 & 127) as usize] = *input326;
			self.fVec105[(self.IOTA0 & 127) as usize] = *input325;
			self.fVec106[(self.IOTA0 & 127) as usize] = *input324;
			self.fVec107[(self.IOTA0 & 127) as usize] = *input323;
			self.fVec108[(self.IOTA0 & 127) as usize] = *input322;
			self.fVec109[(self.IOTA0 & 127) as usize] = *input321;
			self.fVec110[(self.IOTA0 & 127) as usize] = *input320;
			self.fVec111[(self.IOTA0 & 127) as usize] = *input319;
			self.fVec112[(self.IOTA0 & 127) as usize] = *input318;
			self.fVec113[(self.IOTA0 & 127) as usize] = *input317;
			self.fVec114[(self.IOTA0 & 127) as usize] = *input316;
			self.fVec115[(self.IOTA0 & 127) as usize] = *input315;
			self.fVec116[(self.IOTA0 & 127) as usize] = *input314;
			self.fVec117[(self.IOTA0 & 127) as usize] = *input313;
			self.fVec118[(self.IOTA0 & 127) as usize] = *input312;
			self.fVec119[(self.IOTA0 & 127) as usize] = *input311;
			self.fVec120[(self.IOTA0 & 63) as usize] = *input310;
			self.fVec121[(self.IOTA0 & 63) as usize] = *input309;
			self.fVec122[(self.IOTA0 & 63) as usize] = *input308;
			self.fVec123[(self.IOTA0 & 63) as usize] = *input307;
			self.fVec124[(self.IOTA0 & 63) as usize] = *input306;
			self.fVec125[(self.IOTA0 & 63) as usize] = *input305;
			self.fVec126[(self.IOTA0 & 63) as usize] = *input304;
			self.fVec127[(self.IOTA0 & 63) as usize] = *input303;
			self.fVec128[(self.IOTA0 & 63) as usize] = *input302;
			self.fVec129[(self.IOTA0 & 63) as usize] = *input301;
			self.fVec130[(self.IOTA0 & 63) as usize] = *input300;
			self.fVec131[(self.IOTA0 & 63) as usize] = *input299;
			self.fVec132[(self.IOTA0 & 63) as usize] = *input298;
			self.fVec133[(self.IOTA0 & 63) as usize] = *input297;
			self.fVec134[(self.IOTA0 & 63) as usize] = *input296;
			self.fVec135[(self.IOTA0 & 63) as usize] = *input295;
			self.fVec136[(self.IOTA0 & 63) as usize] = *input294;
			self.fVec137[(self.IOTA0 & 63) as usize] = *input293;
			self.fVec138[(self.IOTA0 & 63) as usize] = *input292;
			self.fVec139[(self.IOTA0 & 63) as usize] = *input291;
			self.fVec140[(self.IOTA0 & 63) as usize] = *input290;
			self.fVec141[(self.IOTA0 & 63) as usize] = *input289;
			self.fVec142[(self.IOTA0 & 63) as usize] = *input288;
			self.fVec143[(self.IOTA0 & 63) as usize] = *input287;
			self.fVec144[(self.IOTA0 & 63) as usize] = *input286;
			self.fVec145[(self.IOTA0 & 63) as usize] = *input285;
			self.fVec146[(self.IOTA0 & 63) as usize] = *input284;
			self.fVec147[(self.IOTA0 & 63) as usize] = *input283;
			self.fVec148[(self.IOTA0 & 63) as usize] = *input282;
			self.fVec149[(self.IOTA0 & 63) as usize] = *input281;
			self.fVec150[(self.IOTA0 & 63) as usize] = *input280;
			self.fVec151[(self.IOTA0 & 63) as usize] = *input279;
			self.fVec152[(self.IOTA0 & 31) as usize] = *input278;
			self.fVec153[(self.IOTA0 & 31) as usize] = *input277;
			self.fVec154[(self.IOTA0 & 31) as usize] = *input276;
			self.fVec155[(self.IOTA0 & 31) as usize] = *input275;
			self.fVec156[(self.IOTA0 & 31) as usize] = *input274;
			self.fVec157[(self.IOTA0 & 31) as usize] = *input273;
			self.fVec158[(self.IOTA0 & 31) as usize] = *input272;
			self.fVec159[(self.IOTA0 & 31) as usize] = *input271;
			self.fVec160[(self.IOTA0 & 31) as usize] = *input270;
			self.fVec161[(self.IOTA0 & 31) as usize] = *input269;
			self.fVec162[(self.IOTA0 & 31) as usize] = *input268;
			self.fVec163[(self.IOTA0 & 31) as usize] = *input267;
			self.fVec164[(self.IOTA0 & 31) as usize] = *input266;
			self.fVec165[(self.IOTA0 & 31) as usize] = *input265;
			self.fVec166[(self.IOTA0 & 31) as usize] = *input264;
			self.fVec167[(self.IOTA0 & 31) as usize] = *input263;
			self.fVec168[0] = *input262;
			self.fVec169[0] = *input261;
			self.fVec170[0] = *input260;
			self.fVec171[0] = *input259;
			self.fVec172[0] = *input258;
			self.fVec173[0] = *input257;
			self.fVec174[0] = *input256;
			self.fVec175[0] = *input255;
			self.fVec176[0] = *input254;
			self.fVec177[0] = *input253;
			self.fVec178[0] = *input252;
			self.fVec179[0] = *input251;
			self.fVec180[0] = *input250;
			self.fVec181[0] = *input249;
			self.fVec182[0] = *input248;
			self.fRec0[0] = fSlow1 + self.fConst186 * self.fRec0[1];
			let mut fTemp0: F32 = (i32::wrapping_sub(1, self.iVec62[1])) as F32;
			self.fRec4[0] = fSlow2 + self.fConst186 * self.fRec4[1];
			self.fRec3[0] = self.fRec3[1] - self.fConst187 * self.fRec4[0] * self.fRec1[1];
			self.fRec2[0] = self.fConst187 * self.fRec4[0] * self.fRec3[0] + fTemp0 + self.fRec2[1];
			self.fRec1[0] = self.fRec2[0];
			self.fVec183[(self.IOTA0 & 63) as usize] = *input61;
			self.fVec184[(self.IOTA0 & 63) as usize] = *input62;
			self.fVec185[(self.IOTA0 & 63) as usize] = *input60;
			self.fVec186[(self.IOTA0 & 63) as usize] = *input59;
			self.fVec187[(self.IOTA0 & 63) as usize] = *input58;
			self.fVec188[(self.IOTA0 & 63) as usize] = *input57;
			self.fVec189[(self.IOTA0 & 63) as usize] = *input56;
			self.fVec190[(self.IOTA0 & 63) as usize] = *input55;
			self.fVec191[(self.IOTA0 & 63) as usize] = *input54;
			self.fVec192[(self.IOTA0 & 63) as usize] = *input53;
			self.fVec193[(self.IOTA0 & 63) as usize] = *input52;
			self.fVec194[(self.IOTA0 & 63) as usize] = *input51;
			self.fVec195[(self.IOTA0 & 63) as usize] = *input50;
			self.fVec196[(self.IOTA0 & 63) as usize] = *input49;
			self.fVec197[(self.IOTA0 & 63) as usize] = *input48;
			self.fVec198[(self.IOTA0 & 63) as usize] = *input47;
			self.fVec199[(self.IOTA0 & 63) as usize] = *input46;
			self.fVec200[(self.IOTA0 & 63) as usize] = *input45;
			self.fVec201[(self.IOTA0 & 63) as usize] = *input44;
			self.fVec202[(self.IOTA0 & 63) as usize] = *input43;
			self.fVec203[(self.IOTA0 & 63) as usize] = *input42;
			self.fVec204[(self.IOTA0 & 63) as usize] = *input41;
			self.fVec205[(self.IOTA0 & 63) as usize] = *input40;
			self.fVec206[(self.IOTA0 & 63) as usize] = *input39;
			self.fVec207[(self.IOTA0 & 63) as usize] = *input38;
			self.fVec208[(self.IOTA0 & 63) as usize] = *input37;
			self.fVec209[(self.IOTA0 & 63) as usize] = *input36;
			self.fVec210[(self.IOTA0 & 63) as usize] = *input35;
			self.fVec211[(self.IOTA0 & 63) as usize] = *input34;
			self.fVec212[(self.IOTA0 & 63) as usize] = *input33;
			self.fVec213[(self.IOTA0 & 63) as usize] = *input32;
			self.fVec214[(self.IOTA0 & 31) as usize] = *input31;
			self.fVec215[(self.IOTA0 & 31) as usize] = *input30;
			self.fVec216[(self.IOTA0 & 31) as usize] = *input29;
			self.fVec217[(self.IOTA0 & 31) as usize] = *input28;
			self.fVec218[(self.IOTA0 & 31) as usize] = *input27;
			self.fVec219[(self.IOTA0 & 31) as usize] = *input26;
			self.fVec220[(self.IOTA0 & 31) as usize] = *input25;
			self.fVec221[(self.IOTA0 & 31) as usize] = *input24;
			self.fVec222[(self.IOTA0 & 31) as usize] = *input23;
			self.fVec223[(self.IOTA0 & 31) as usize] = *input22;
			self.fVec224[(self.IOTA0 & 31) as usize] = *input21;
			self.fVec225[(self.IOTA0 & 31) as usize] = *input20;
			self.fVec226[(self.IOTA0 & 31) as usize] = *input19;
			self.fVec227[(self.IOTA0 & 31) as usize] = *input18;
			self.fVec228[(self.IOTA0 & 31) as usize] = *input17;
			self.fVec229[(self.IOTA0 & 31) as usize] = *input16;
			self.fVec230[0] = *input15;
			self.fVec231[0] = *input14;
			self.fVec232[0] = *input13;
			self.fVec233[0] = *input12;
			self.fVec234[0] = *input11;
			self.fVec235[0] = *input10;
			self.fVec236[0] = *input9;
			self.fVec237[0] = *input8;
			self.fVec238[0] = *input7;
			self.fVec239[0] = *input6;
			self.fVec240[0] = *input5;
			self.fVec241[0] = *input4;
			self.fVec242[0] = *input3;
			self.fVec243[0] = *input2;
			self.fVec244[0] = *input1;
			self.fVec245[(self.IOTA0 & 127) as usize] = *input182;
			self.fVec246[(self.IOTA0 & 127) as usize] = *input183;
			self.fVec247[(self.IOTA0 & 127) as usize] = *input181;
			self.fVec248[(self.IOTA0 & 127) as usize] = *input180;
			self.fVec249[(self.IOTA0 & 127) as usize] = *input179;
			self.fVec250[(self.IOTA0 & 127) as usize] = *input178;
			self.fVec251[(self.IOTA0 & 127) as usize] = *input177;
			self.fVec252[(self.IOTA0 & 127) as usize] = *input176;
			self.fVec253[(self.IOTA0 & 127) as usize] = *input175;
			self.fVec254[(self.IOTA0 & 127) as usize] = *input174;
			self.fVec255[(self.IOTA0 & 127) as usize] = *input173;
			self.fVec256[(self.IOTA0 & 127) as usize] = *input172;
			self.fVec257[(self.IOTA0 & 127) as usize] = *input171;
			self.fVec258[(self.IOTA0 & 127) as usize] = *input170;
			self.fVec259[(self.IOTA0 & 127) as usize] = *input169;
			self.fVec260[(self.IOTA0 & 127) as usize] = *input168;
			self.fVec261[(self.IOTA0 & 127) as usize] = *input167;
			self.fVec262[(self.IOTA0 & 127) as usize] = *input166;
			self.fVec263[(self.IOTA0 & 127) as usize] = *input165;
			self.fVec264[(self.IOTA0 & 127) as usize] = *input164;
			self.fVec265[(self.IOTA0 & 127) as usize] = *input163;
			self.fVec266[(self.IOTA0 & 127) as usize] = *input162;
			self.fVec267[(self.IOTA0 & 127) as usize] = *input161;
			self.fVec268[(self.IOTA0 & 127) as usize] = *input160;
			self.fVec269[(self.IOTA0 & 127) as usize] = *input159;
			self.fVec270[(self.IOTA0 & 127) as usize] = *input158;
			self.fVec271[(self.IOTA0 & 127) as usize] = *input157;
			self.fVec272[(self.IOTA0 & 127) as usize] = *input156;
			self.fVec273[(self.IOTA0 & 127) as usize] = *input155;
			self.fVec274[(self.IOTA0 & 127) as usize] = *input154;
			self.fVec275[(self.IOTA0 & 127) as usize] = *input153;
			self.fVec276[(self.IOTA0 & 127) as usize] = *input152;
			self.fVec277[(self.IOTA0 & 127) as usize] = *input151;
			self.fVec278[(self.IOTA0 & 127) as usize] = *input150;
			self.fVec279[(self.IOTA0 & 127) as usize] = *input149;
			self.fVec280[(self.IOTA0 & 127) as usize] = *input148;
			self.fVec281[(self.IOTA0 & 127) as usize] = *input147;
			self.fVec282[(self.IOTA0 & 127) as usize] = *input146;
			self.fVec283[(self.IOTA0 & 127) as usize] = *input145;
			self.fVec284[(self.IOTA0 & 127) as usize] = *input144;
			self.fVec285[(self.IOTA0 & 127) as usize] = *input143;
			self.fVec286[(self.IOTA0 & 127) as usize] = *input142;
			self.fVec287[(self.IOTA0 & 127) as usize] = *input141;
			self.fVec288[(self.IOTA0 & 127) as usize] = *input140;
			self.fVec289[(self.IOTA0 & 127) as usize] = *input139;
			self.fVec290[(self.IOTA0 & 127) as usize] = *input138;
			self.fVec291[(self.IOTA0 & 127) as usize] = *input137;
			self.fVec292[(self.IOTA0 & 127) as usize] = *input136;
			self.fVec293[(self.IOTA0 & 127) as usize] = *input135;
			self.fVec294[(self.IOTA0 & 127) as usize] = *input134;
			self.fVec295[(self.IOTA0 & 127) as usize] = *input133;
			self.fVec296[(self.IOTA0 & 127) as usize] = *input132;
			self.fVec297[(self.IOTA0 & 127) as usize] = *input131;
			self.fVec298[(self.IOTA0 & 127) as usize] = *input130;
			self.fVec299[(self.IOTA0 & 127) as usize] = *input129;
			self.fVec300[(self.IOTA0 & 127) as usize] = *input128;
			self.fVec301[(self.IOTA0 & 127) as usize] = *input127;
			self.fVec302[(self.IOTA0 & 63) as usize] = *input126;
			self.fVec303[(self.IOTA0 & 63) as usize] = *input125;
			self.fVec304[(self.IOTA0 & 63) as usize] = *input124;
			self.fVec305[(self.IOTA0 & 63) as usize] = *input123;
			self.fVec306[(self.IOTA0 & 63) as usize] = *input122;
			self.fVec307[(self.IOTA0 & 63) as usize] = *input121;
			self.fVec308[(self.IOTA0 & 63) as usize] = *input120;
			self.fVec309[(self.IOTA0 & 63) as usize] = *input119;
			self.fVec310[(self.IOTA0 & 63) as usize] = *input118;
			self.fVec311[(self.IOTA0 & 63) as usize] = *input117;
			self.fVec312[(self.IOTA0 & 63) as usize] = *input116;
			self.fVec313[(self.IOTA0 & 63) as usize] = *input115;
			self.fVec314[(self.IOTA0 & 63) as usize] = *input114;
			self.fVec315[(self.IOTA0 & 63) as usize] = *input113;
			self.fVec316[(self.IOTA0 & 63) as usize] = *input112;
			self.fVec317[(self.IOTA0 & 63) as usize] = *input111;
			self.fVec318[(self.IOTA0 & 63) as usize] = *input110;
			self.fVec319[(self.IOTA0 & 63) as usize] = *input109;
			self.fVec320[(self.IOTA0 & 63) as usize] = *input108;
			self.fVec321[(self.IOTA0 & 63) as usize] = *input107;
			self.fVec322[(self.IOTA0 & 63) as usize] = *input106;
			self.fVec323[(self.IOTA0 & 63) as usize] = *input105;
			self.fVec324[(self.IOTA0 & 63) as usize] = *input104;
			self.fVec325[(self.IOTA0 & 63) as usize] = *input103;
			self.fVec326[(self.IOTA0 & 63) as usize] = *input102;
			self.fVec327[(self.IOTA0 & 63) as usize] = *input101;
			self.fVec328[(self.IOTA0 & 63) as usize] = *input100;
			self.fVec329[(self.IOTA0 & 63) as usize] = *input99;
			self.fVec330[(self.IOTA0 & 63) as usize] = *input98;
			self.fVec331[(self.IOTA0 & 63) as usize] = *input97;
			self.fVec332[(self.IOTA0 & 63) as usize] = *input96;
			self.fVec333[(self.IOTA0 & 63) as usize] = *input95;
			self.fVec334[(self.IOTA0 & 31) as usize] = *input94;
			self.fVec335[(self.IOTA0 & 31) as usize] = *input93;
			self.fVec336[(self.IOTA0 & 31) as usize] = *input92;
			self.fVec337[(self.IOTA0 & 31) as usize] = *input91;
			self.fVec338[(self.IOTA0 & 31) as usize] = *input90;
			self.fVec339[(self.IOTA0 & 31) as usize] = *input89;
			self.fVec340[(self.IOTA0 & 31) as usize] = *input88;
			self.fVec341[(self.IOTA0 & 31) as usize] = *input87;
			self.fVec342[(self.IOTA0 & 31) as usize] = *input86;
			self.fVec343[(self.IOTA0 & 31) as usize] = *input85;
			self.fVec344[(self.IOTA0 & 31) as usize] = *input84;
			self.fVec345[(self.IOTA0 & 31) as usize] = *input83;
			self.fVec346[(self.IOTA0 & 31) as usize] = *input82;
			self.fVec347[(self.IOTA0 & 31) as usize] = *input81;
			self.fVec348[(self.IOTA0 & 31) as usize] = *input80;
			self.fVec349[(self.IOTA0 & 31) as usize] = *input79;
			self.fVec350[0] = *input78;
			self.fVec351[0] = *input77;
			self.fVec352[0] = *input76;
			self.fVec353[0] = *input75;
			self.fVec354[0] = *input74;
			self.fVec355[0] = *input73;
			self.fVec356[0] = *input72;
			self.fVec357[0] = *input71;
			self.fVec358[0] = *input70;
			self.fVec359[0] = *input69;
			self.fVec360[0] = *input68;
			self.fVec361[0] = *input67;
			self.fVec362[0] = *input66;
			self.fVec363[0] = *input65;
			self.fVec364[0] = *input64;
			let mut fTemp1: F32 = self.fConst187 * self.fRec4[0];
			let mut fTemp2: F32 = F32::cos(fTemp1);
			let mut fTemp3: F32 = F32::sin(fTemp1);
			self.fRec5[0] = self.fRec6[1] * fTemp3 + self.fRec5[1] * fTemp2;
			self.fRec6[0] = fTemp0 + self.fRec6[1] * fTemp2 - fTemp3 * self.fRec5[1];
			*output0 = fSlow3 * (self.fRec6[0] * F32::cos(self.fRec0[0] * (if iSlow0 != 0 {*input63 * self.fConst183 + self.fConst182 * self.fVec364[1] + self.fConst181 * self.fVec363[2] + self.fConst180 * self.fVec362[3] + self.fConst179 * self.fVec361[4] + self.fConst178 * self.fVec360[5] + self.fConst177 * self.fVec359[6] + self.fConst176 * self.fVec358[7] + self.fConst175 * self.fVec357[8] + self.fConst174 * self.fVec356[9] + self.fConst173 * self.fVec355[10] + self.fConst172 * self.fVec354[11] + self.fConst171 * self.fVec353[12] + self.fConst170 * self.fVec352[13] + self.fConst169 * self.fVec351[14] + self.fConst168 * self.fVec350[15] + self.fConst167 * self.fVec349[((i32::wrapping_sub(self.IOTA0, 16)) & 31) as usize] + self.fConst166 * self.fVec348[((i32::wrapping_sub(self.IOTA0, 17)) & 31) as usize] + self.fConst165 * self.fVec347[((i32::wrapping_sub(self.IOTA0, 18)) & 31) as usize] + self.fConst164 * self.fVec346[((i32::wrapping_sub(self.IOTA0, 19)) & 31) as usize] + self.fConst163 * self.fVec345[((i32::wrapping_sub(self.IOTA0, 20)) & 31) as usize] + self.fConst162 * self.fVec344[((i32::wrapping_sub(self.IOTA0, 21)) & 31) as usize] + self.fConst161 * self.fVec343[((i32::wrapping_sub(self.IOTA0, 22)) & 31) as usize] + self.fConst160 * self.fVec342[((i32::wrapping_sub(self.IOTA0, 23)) & 31) as usize] + self.fConst159 * self.fVec341[((i32::wrapping_sub(self.IOTA0, 24)) & 31) as usize] + self.fConst158 * self.fVec340[((i32::wrapping_sub(self.IOTA0, 25)) & 31) as usize] + self.fConst157 * self.fVec339[((i32::wrapping_sub(self.IOTA0, 26)) & 31) as usize] + self.fConst156 * self.fVec338[((i32::wrapping_sub(self.IOTA0, 27)) & 31) as usize] + self.fConst155 * self.fVec337[((i32::wrapping_sub(self.IOTA0, 28)) & 31) as usize] + self.fConst154 * self.fVec336[((i32::wrapping_sub(self.IOTA0, 29)) & 31) as usize] + self.fConst153 * self.fVec335[((i32::wrapping_sub(self.IOTA0, 30)) & 31) as usize] + self.fConst152 * self.fVec334[((i32::wrapping_sub(self.IOTA0, 31)) & 31) as usize] + self.fConst151 * self.fVec333[((i32::wrapping_sub(self.IOTA0, 32)) & 63) as usize] + self.fConst150 * self.fVec332[((i32::wrapping_sub(self.IOTA0, 33)) & 63) as usize] + self.fConst149 * self.fVec331[((i32::wrapping_sub(self.IOTA0, 34)) & 63) as usize] + self.fConst148 * self.fVec330[((i32::wrapping_sub(self.IOTA0, 35)) & 63) as usize] + self.fConst147 * self.fVec329[((i32::wrapping_sub(self.IOTA0, 36)) & 63) as usize] + self.fConst146 * self.fVec328[((i32::wrapping_sub(self.IOTA0, 37)) & 63) as usize] + self.fConst145 * self.fVec327[((i32::wrapping_sub(self.IOTA0, 38)) & 63) as usize] + self.fConst144 * self.fVec326[((i32::wrapping_sub(self.IOTA0, 39)) & 63) as usize] + self.fConst143 * self.fVec325[((i32::wrapping_sub(self.IOTA0, 40)) & 63) as usize] + self.fConst142 * self.fVec324[((i32::wrapping_sub(self.IOTA0, 41)) & 63) as usize] + self.fConst141 * self.fVec323[((i32::wrapping_sub(self.IOTA0, 42)) & 63) as usize] + self.fConst140 * self.fVec322[((i32::wrapping_sub(self.IOTA0, 43)) & 63) as usize] + self.fConst139 * self.fVec321[((i32::wrapping_sub(self.IOTA0, 44)) & 63) as usize] + self.fConst138 * self.fVec320[((i32::wrapping_sub(self.IOTA0, 45)) & 63) as usize] + self.fConst137 * self.fVec319[((i32::wrapping_sub(self.IOTA0, 46)) & 63) as usize] + self.fConst136 * self.fVec318[((i32::wrapping_sub(self.IOTA0, 47)) & 63) as usize] + self.fConst135 * self.fVec317[((i32::wrapping_sub(self.IOTA0, 48)) & 63) as usize] + self.fConst134 * self.fVec316[((i32::wrapping_sub(self.IOTA0, 49)) & 63) as usize] + self.fConst133 * self.fVec315[((i32::wrapping_sub(self.IOTA0, 50)) & 63) as usize] + self.fConst132 * self.fVec314[((i32::wrapping_sub(self.IOTA0, 51)) & 63) as usize] + self.fConst131 * self.fVec313[((i32::wrapping_sub(self.IOTA0, 52)) & 63) as usize] + self.fConst130 * self.fVec312[((i32::wrapping_sub(self.IOTA0, 53)) & 63) as usize] + self.fConst129 * self.fVec311[((i32::wrapping_sub(self.IOTA0, 54)) & 63) as usize] + self.fConst128 * self.fVec310[((i32::wrapping_sub(self.IOTA0, 55)) & 63) as usize] + self.fConst127 * self.fVec309[((i32::wrapping_sub(self.IOTA0, 56)) & 63) as usize] + self.fConst126 * self.fVec308[((i32::wrapping_sub(self.IOTA0, 57)) & 63) as usize] + self.fConst125 * self.fVec307[((i32::wrapping_sub(self.IOTA0, 58)) & 63) as usize] + self.fConst124 * self.fVec306[((i32::wrapping_sub(self.IOTA0, 59)) & 63) as usize] + self.fConst123 * self.fVec305[((i32::wrapping_sub(self.IOTA0, 60)) & 63) as usize] + self.fConst122 * self.fVec304[((i32::wrapping_sub(self.IOTA0, 61)) & 63) as usize] + self.fConst121 * self.fVec303[((i32::wrapping_sub(self.IOTA0, 62)) & 63) as usize] + self.fConst120 * self.fVec302[((i32::wrapping_sub(self.IOTA0, 63)) & 63) as usize] + self.fConst119 * self.fVec301[((i32::wrapping_sub(self.IOTA0, 64)) & 127) as usize] + self.fConst118 * self.fVec300[((i32::wrapping_sub(self.IOTA0, 65)) & 127) as usize] + self.fConst117 * self.fVec299[((i32::wrapping_sub(self.IOTA0, 66)) & 127) as usize] + self.fConst116 * self.fVec298[((i32::wrapping_sub(self.IOTA0, 67)) & 127) as usize] + self.fConst115 * self.fVec297[((i32::wrapping_sub(self.IOTA0, 68)) & 127) as usize] + self.fConst114 * self.fVec296[((i32::wrapping_sub(self.IOTA0, 69)) & 127) as usize] + self.fConst113 * self.fVec295[((i32::wrapping_sub(self.IOTA0, 70)) & 127) as usize] + self.fConst112 * self.fVec294[((i32::wrapping_sub(self.IOTA0, 71)) & 127) as usize] + self.fConst111 * self.fVec293[((i32::wrapping_sub(self.IOTA0, 72)) & 127) as usize] + self.fConst110 * self.fVec292[((i32::wrapping_sub(self.IOTA0, 73)) & 127) as usize] + self.fConst109 * self.fVec291[((i32::wrapping_sub(self.IOTA0, 74)) & 127) as usize] + self.fConst108 * self.fVec290[((i32::wrapping_sub(self.IOTA0, 75)) & 127) as usize] + self.fConst107 * self.fVec289[((i32::wrapping_sub(self.IOTA0, 76)) & 127) as usize] + self.fConst106 * self.fVec288[((i32::wrapping_sub(self.IOTA0, 77)) & 127) as usize] + self.fConst105 * self.fVec287[((i32::wrapping_sub(self.IOTA0, 78)) & 127) as usize] + self.fConst104 * self.fVec286[((i32::wrapping_sub(self.IOTA0, 79)) & 127) as usize] + self.fConst103 * self.fVec285[((i32::wrapping_sub(self.IOTA0, 80)) & 127) as usize] + self.fConst102 * self.fVec284[((i32::wrapping_sub(self.IOTA0, 81)) & 127) as usize] + self.fConst101 * self.fVec283[((i32::wrapping_sub(self.IOTA0, 82)) & 127) as usize] + self.fConst100 * self.fVec282[((i32::wrapping_sub(self.IOTA0, 83)) & 127) as usize] + self.fConst99 * self.fVec281[((i32::wrapping_sub(self.IOTA0, 84)) & 127) as usize] + self.fConst98 * self.fVec280[((i32::wrapping_sub(self.IOTA0, 85)) & 127) as usize] + self.fConst97 * self.fVec279[((i32::wrapping_sub(self.IOTA0, 86)) & 127) as usize] + self.fConst96 * self.fVec278[((i32::wrapping_sub(self.IOTA0, 87)) & 127) as usize] + self.fConst95 * self.fVec277[((i32::wrapping_sub(self.IOTA0, 88)) & 127) as usize] + self.fConst94 * self.fVec276[((i32::wrapping_sub(self.IOTA0, 89)) & 127) as usize] + self.fConst93 * self.fVec275[((i32::wrapping_sub(self.IOTA0, 90)) & 127) as usize] + self.fConst92 * self.fVec274[((i32::wrapping_sub(self.IOTA0, 91)) & 127) as usize] + self.fConst91 * self.fVec273[((i32::wrapping_sub(self.IOTA0, 92)) & 127) as usize] + self.fConst90 * self.fVec272[((i32::wrapping_sub(self.IOTA0, 93)) & 127) as usize] + self.fConst89 * self.fVec271[((i32::wrapping_sub(self.IOTA0, 94)) & 127) as usize] + self.fConst88 * self.fVec270[((i32::wrapping_sub(self.IOTA0, 95)) & 127) as usize] + self.fConst87 * self.fVec269[((i32::wrapping_sub(self.IOTA0, 96)) & 127) as usize] + self.fConst86 * self.fVec268[((i32::wrapping_sub(self.IOTA0, 97)) & 127) as usize] + self.fConst85 * self.fVec267[((i32::wrapping_sub(self.IOTA0, 98)) & 127) as usize] + self.fConst84 * self.fVec266[((i32::wrapping_sub(self.IOTA0, 99)) & 127) as usize] + self.fConst83 * self.fVec265[((i32::wrapping_sub(self.IOTA0, 100)) & 127) as usize] + self.fConst82 * self.fVec264[((i32::wrapping_sub(self.IOTA0, 101)) & 127) as usize] + self.fConst81 * self.fVec263[((i32::wrapping_sub(self.IOTA0, 102)) & 127) as usize] + self.fConst80 * self.fVec262[((i32::wrapping_sub(self.IOTA0, 103)) & 127) as usize] + self.fConst79 * self.fVec261[((i32::wrapping_sub(self.IOTA0, 104)) & 127) as usize] + self.fConst78 * self.fVec260[((i32::wrapping_sub(self.IOTA0, 105)) & 127) as usize] + self.fConst77 * self.fVec259[((i32::wrapping_sub(self.IOTA0, 106)) & 127) as usize] + self.fConst76 * self.fVec258[((i32::wrapping_sub(self.IOTA0, 107)) & 127) as usize] + self.fConst75 * self.fVec257[((i32::wrapping_sub(self.IOTA0, 108)) & 127) as usize] + self.fConst74 * self.fVec256[((i32::wrapping_sub(self.IOTA0, 109)) & 127) as usize] + self.fConst73 * self.fVec255[((i32::wrapping_sub(self.IOTA0, 110)) & 127) as usize] + self.fConst72 * self.fVec254[((i32::wrapping_sub(self.IOTA0, 111)) & 127) as usize] + self.fConst71 * self.fVec253[((i32::wrapping_sub(self.IOTA0, 112)) & 127) as usize] + self.fConst70 * self.fVec252[((i32::wrapping_sub(self.IOTA0, 113)) & 127) as usize] + self.fConst69 * self.fVec251[((i32::wrapping_sub(self.IOTA0, 114)) & 127) as usize] + self.fConst68 * self.fVec250[((i32::wrapping_sub(self.IOTA0, 115)) & 127) as usize] + self.fConst67 * self.fVec249[((i32::wrapping_sub(self.IOTA0, 116)) & 127) as usize] + self.fConst66 * self.fVec248[((i32::wrapping_sub(self.IOTA0, 117)) & 127) as usize] + self.fConst65 * self.fVec247[((i32::wrapping_sub(self.IOTA0, 118)) & 127) as usize] + self.fConst64 * self.fVec246[((i32::wrapping_sub(self.IOTA0, 120)) & 127) as usize] + self.fConst63 * self.fVec245[((i32::wrapping_sub(self.IOTA0, 119)) & 127) as usize]} else {*input0 * self.fConst62 + self.fConst61 * self.fVec244[1] + self.fConst60 * self.fVec243[2] + self.fConst59 * self.fVec242[3] + self.fConst58 * self.fVec241[4] + self.fConst57 * self.fVec240[5] + self.fConst56 * self.fVec239[6] + self.fConst55 * self.fVec238[7] + self.fConst54 * self.fVec237[8] + self.fConst53 * self.fVec236[9] + self.fConst52 * self.fVec235[10] + self.fConst51 * self.fVec234[11] + self.fConst50 * self.fVec233[12] + self.fConst49 * self.fVec232[13] + self.fConst48 * self.fVec231[14] + self.fConst47 * self.fVec230[15] + self.fConst46 * self.fVec229[((i32::wrapping_sub(self.IOTA0, 16)) & 31) as usize] + self.fConst45 * self.fVec228[((i32::wrapping_sub(self.IOTA0, 17)) & 31) as usize] + self.fConst44 * self.fVec227[((i32::wrapping_sub(self.IOTA0, 18)) & 31) as usize] + self.fConst43 * self.fVec226[((i32::wrapping_sub(self.IOTA0, 19)) & 31) as usize] + self.fConst42 * self.fVec225[((i32::wrapping_sub(self.IOTA0, 20)) & 31) as usize] + self.fConst41 * self.fVec224[((i32::wrapping_sub(self.IOTA0, 21)) & 31) as usize] + self.fConst40 * self.fVec223[((i32::wrapping_sub(self.IOTA0, 22)) & 31) as usize] + self.fConst39 * self.fVec222[((i32::wrapping_sub(self.IOTA0, 23)) & 31) as usize] + self.fConst38 * self.fVec221[((i32::wrapping_sub(self.IOTA0, 24)) & 31) as usize] + self.fConst37 * self.fVec220[((i32::wrapping_sub(self.IOTA0, 25)) & 31) as usize] + self.fConst36 * self.fVec219[((i32::wrapping_sub(self.IOTA0, 26)) & 31) as usize] + self.fConst35 * self.fVec218[((i32::wrapping_sub(self.IOTA0, 27)) & 31) as usize] + self.fConst34 * self.fVec217[((i32::wrapping_sub(self.IOTA0, 28)) & 31) as usize] + self.fConst33 * self.fVec216[((i32::wrapping_sub(self.IOTA0, 29)) & 31) as usize] + self.fConst32 * self.fVec215[((i32::wrapping_sub(self.IOTA0, 30)) & 31) as usize] + self.fConst31 * self.fVec214[((i32::wrapping_sub(self.IOTA0, 31)) & 31) as usize] + self.fConst30 * self.fVec213[((i32::wrapping_sub(self.IOTA0, 32)) & 63) as usize] + self.fConst29 * self.fVec212[((i32::wrapping_sub(self.IOTA0, 33)) & 63) as usize] + self.fConst28 * self.fVec211[((i32::wrapping_sub(self.IOTA0, 34)) & 63) as usize] + self.fConst27 * self.fVec210[((i32::wrapping_sub(self.IOTA0, 35)) & 63) as usize] + self.fConst26 * self.fVec209[((i32::wrapping_sub(self.IOTA0, 36)) & 63) as usize] + self.fConst25 * self.fVec208[((i32::wrapping_sub(self.IOTA0, 37)) & 63) as usize] + self.fConst24 * self.fVec207[((i32::wrapping_sub(self.IOTA0, 38)) & 63) as usize] + self.fConst23 * self.fVec206[((i32::wrapping_sub(self.IOTA0, 39)) & 63) as usize] + self.fConst22 * self.fVec205[((i32::wrapping_sub(self.IOTA0, 40)) & 63) as usize] + self.fConst21 * self.fVec204[((i32::wrapping_sub(self.IOTA0, 41)) & 63) as usize] + self.fConst20 * self.fVec203[((i32::wrapping_sub(self.IOTA0, 42)) & 63) as usize] + self.fConst19 * self.fVec202[((i32::wrapping_sub(self.IOTA0, 43)) & 63) as usize] + self.fConst18 * self.fVec201[((i32::wrapping_sub(self.IOTA0, 44)) & 63) as usize] + self.fConst17 * self.fVec200[((i32::wrapping_sub(self.IOTA0, 45)) & 63) as usize] + self.fConst16 * self.fVec199[((i32::wrapping_sub(self.IOTA0, 46)) & 63) as usize] + self.fConst15 * self.fVec198[((i32::wrapping_sub(self.IOTA0, 47)) & 63) as usize] + self.fConst14 * self.fVec197[((i32::wrapping_sub(self.IOTA0, 48)) & 63) as usize] + self.fConst13 * self.fVec196[((i32::wrapping_sub(self.IOTA0, 49)) & 63) as usize] + self.fConst12 * self.fVec195[((i32::wrapping_sub(self.IOTA0, 50)) & 63) as usize] + self.fConst11 * self.fVec194[((i32::wrapping_sub(self.IOTA0, 51)) & 63) as usize] + self.fConst10 * self.fVec193[((i32::wrapping_sub(self.IOTA0, 52)) & 63) as usize] + self.fConst9 * self.fVec192[((i32::wrapping_sub(self.IOTA0, 53)) & 63) as usize] + self.fConst8 * self.fVec191[((i32::wrapping_sub(self.IOTA0, 54)) & 63) as usize] + self.fConst7 * self.fVec190[((i32::wrapping_sub(self.IOTA0, 55)) & 63) as usize] + self.fConst6 * self.fVec189[((i32::wrapping_sub(self.IOTA0, 56)) & 63) as usize] + self.fConst5 * self.fVec188[((i32::wrapping_sub(self.IOTA0, 57)) & 63) as usize] + self.fConst4 * self.fVec187[((i32::wrapping_sub(self.IOTA0, 58)) & 63) as usize] + self.fConst3 * self.fVec186[((i32::wrapping_sub(self.IOTA0, 59)) & 63) as usize] + self.fConst2 * self.fVec185[((i32::wrapping_sub(self.IOTA0, 60)) & 63) as usize] + self.fConst1 * self.fVec184[((i32::wrapping_sub(self.IOTA0, 62)) & 63) as usize] + self.fConst0 * self.fVec183[((i32::wrapping_sub(self.IOTA0, 61)) & 63) as usize]})) - self.fRec1[0] * F32::sin(self.fRec0[0] * (if iSlow0 != 0 {*input247 * self.fConst183 + self.fConst182 * self.fVec182[1] + self.fConst181 * self.fVec181[2] + self.fConst180 * self.fVec180[3] + self.fConst179 * self.fVec179[4] + self.fConst178 * self.fVec178[5] + self.fConst177 * self.fVec177[6] + self.fConst176 * self.fVec176[7] + self.fConst175 * self.fVec175[8] + self.fConst174 * self.fVec174[9] + self.fConst173 * self.fVec173[10] + self.fConst172 * self.fVec172[11] + self.fConst171 * self.fVec171[12] + self.fConst170 * self.fVec170[13] + self.fConst169 * self.fVec169[14] + self.fConst168 * self.fVec168[15] + self.fConst167 * self.fVec167[((i32::wrapping_sub(self.IOTA0, 16)) & 31) as usize] + self.fConst166 * self.fVec166[((i32::wrapping_sub(self.IOTA0, 17)) & 31) as usize] + self.fConst165 * self.fVec165[((i32::wrapping_sub(self.IOTA0, 18)) & 31) as usize] + self.fConst164 * self.fVec164[((i32::wrapping_sub(self.IOTA0, 19)) & 31) as usize] + self.fConst163 * self.fVec163[((i32::wrapping_sub(self.IOTA0, 20)) & 31) as usize] + self.fConst162 * self.fVec162[((i32::wrapping_sub(self.IOTA0, 21)) & 31) as usize] + self.fConst161 * self.fVec161[((i32::wrapping_sub(self.IOTA0, 22)) & 31) as usize] + self.fConst160 * self.fVec160[((i32::wrapping_sub(self.IOTA0, 23)) & 31) as usize] + self.fConst159 * self.fVec159[((i32::wrapping_sub(self.IOTA0, 24)) & 31) as usize] + self.fConst158 * self.fVec158[((i32::wrapping_sub(self.IOTA0, 25)) & 31) as usize] + self.fConst157 * self.fVec157[((i32::wrapping_sub(self.IOTA0, 26)) & 31) as usize] + self.fConst156 * self.fVec156[((i32::wrapping_sub(self.IOTA0, 27)) & 31) as usize] + self.fConst155 * self.fVec155[((i32::wrapping_sub(self.IOTA0, 28)) & 31) as usize] + self.fConst154 * self.fVec154[((i32::wrapping_sub(self.IOTA0, 29)) & 31) as usize] + self.fConst153 * self.fVec153[((i32::wrapping_sub(self.IOTA0, 30)) & 31) as usize] + self.fConst152 * self.fVec152[((i32::wrapping_sub(self.IOTA0, 31)) & 31) as usize] + self.fConst151 * self.fVec151[((i32::wrapping_sub(self.IOTA0, 32)) & 63) as usize] + self.fConst150 * self.fVec150[((i32::wrapping_sub(self.IOTA0, 33)) & 63) as usize] + self.fConst149 * self.fVec149[((i32::wrapping_sub(self.IOTA0, 34)) & 63) as usize] + self.fConst148 * self.fVec148[((i32::wrapping_sub(self.IOTA0, 35)) & 63) as usize] + self.fConst147 * self.fVec147[((i32::wrapping_sub(self.IOTA0, 36)) & 63) as usize] + self.fConst146 * self.fVec146[((i32::wrapping_sub(self.IOTA0, 37)) & 63) as usize] + self.fConst145 * self.fVec145[((i32::wrapping_sub(self.IOTA0, 38)) & 63) as usize] + self.fConst144 * self.fVec144[((i32::wrapping_sub(self.IOTA0, 39)) & 63) as usize] + self.fConst143 * self.fVec143[((i32::wrapping_sub(self.IOTA0, 40)) & 63) as usize] + self.fConst142 * self.fVec142[((i32::wrapping_sub(self.IOTA0, 41)) & 63) as usize] + self.fConst141 * self.fVec141[((i32::wrapping_sub(self.IOTA0, 42)) & 63) as usize] + self.fConst140 * self.fVec140[((i32::wrapping_sub(self.IOTA0, 43)) & 63) as usize] + self.fConst139 * self.fVec139[((i32::wrapping_sub(self.IOTA0, 44)) & 63) as usize] + self.fConst138 * self.fVec138[((i32::wrapping_sub(self.IOTA0, 45)) & 63) as usize] + self.fConst137 * self.fVec137[((i32::wrapping_sub(self.IOTA0, 46)) & 63) as usize] + self.fConst136 * self.fVec136[((i32::wrapping_sub(self.IOTA0, 47)) & 63) as usize] + self.fConst135 * self.fVec135[((i32::wrapping_sub(self.IOTA0, 48)) & 63) as usize] + self.fConst134 * self.fVec134[((i32::wrapping_sub(self.IOTA0, 49)) & 63) as usize] + self.fConst133 * self.fVec133[((i32::wrapping_sub(self.IOTA0, 50)) & 63) as usize] + self.fConst132 * self.fVec132[((i32::wrapping_sub(self.IOTA0, 51)) & 63) as usize] + self.fConst131 * self.fVec131[((i32::wrapping_sub(self.IOTA0, 52)) & 63) as usize] + self.fConst130 * self.fVec130[((i32::wrapping_sub(self.IOTA0, 53)) & 63) as usize] + self.fConst129 * self.fVec129[((i32::wrapping_sub(self.IOTA0, 54)) & 63) as usize] + self.fConst128 * self.fVec128[((i32::wrapping_sub(self.IOTA0, 55)) & 63) as usize] + self.fConst127 * self.fVec127[((i32::wrapping_sub(self.IOTA0, 56)) & 63) as usize] + self.fConst126 * self.fVec126[((i32::wrapping_sub(self.IOTA0, 57)) & 63) as usize] + self.fConst125 * self.fVec125[((i32::wrapping_sub(self.IOTA0, 58)) & 63) as usize] + self.fConst124 * self.fVec124[((i32::wrapping_sub(self.IOTA0, 59)) & 63) as usize] + self.fConst123 * self.fVec123[((i32::wrapping_sub(self.IOTA0, 60)) & 63) as usize] + self.fConst122 * self.fVec122[((i32::wrapping_sub(self.IOTA0, 61)) & 63) as usize] + self.fConst121 * self.fVec121[((i32::wrapping_sub(self.IOTA0, 62)) & 63) as usize] + self.fConst120 * self.fVec120[((i32::wrapping_sub(self.IOTA0, 63)) & 63) as usize] + self.fConst119 * self.fVec119[((i32::wrapping_sub(self.IOTA0, 64)) & 127) as usize] + self.fConst118 * self.fVec118[((i32::wrapping_sub(self.IOTA0, 65)) & 127) as usize] + self.fConst117 * self.fVec117[((i32::wrapping_sub(self.IOTA0, 66)) & 127) as usize] + self.fConst116 * self.fVec116[((i32::wrapping_sub(self.IOTA0, 67)) & 127) as usize] + self.fConst115 * self.fVec115[((i32::wrapping_sub(self.IOTA0, 68)) & 127) as usize] + self.fConst114 * self.fVec114[((i32::wrapping_sub(self.IOTA0, 69)) & 127) as usize] + self.fConst113 * self.fVec113[((i32::wrapping_sub(self.IOTA0, 70)) & 127) as usize] + self.fConst112 * self.fVec112[((i32::wrapping_sub(self.IOTA0, 71)) & 127) as usize] + self.fConst111 * self.fVec111[((i32::wrapping_sub(self.IOTA0, 72)) & 127) as usize] + self.fConst110 * self.fVec110[((i32::wrapping_sub(self.IOTA0, 73)) & 127) as usize] + self.fConst109 * self.fVec109[((i32::wrapping_sub(self.IOTA0, 74)) & 127) as usize] + self.fConst108 * self.fVec108[((i32::wrapping_sub(self.IOTA0, 75)) & 127) as usize] + self.fConst107 * self.fVec107[((i32::wrapping_sub(self.IOTA0, 76)) & 127) as usize] + self.fConst106 * self.fVec106[((i32::wrapping_sub(self.IOTA0, 77)) & 127) as usize] + self.fConst105 * self.fVec105[((i32::wrapping_sub(self.IOTA0, 78)) & 127) as usize] + self.fConst104 * self.fVec104[((i32::wrapping_sub(self.IOTA0, 79)) & 127) as usize] + self.fConst103 * self.fVec103[((i32::wrapping_sub(self.IOTA0, 80)) & 127) as usize] + self.fConst102 * self.fVec102[((i32::wrapping_sub(self.IOTA0, 81)) & 127) as usize] + self.fConst101 * self.fVec101[((i32::wrapping_sub(self.IOTA0, 82)) & 127) as usize] + self.fConst100 * self.fVec100[((i32::wrapping_sub(self.IOTA0, 83)) & 127) as usize] + self.fConst99 * self.fVec99[((i32::wrapping_sub(self.IOTA0, 84)) & 127) as usize] + self.fConst98 * self.fVec98[((i32::wrapping_sub(self.IOTA0, 85)) & 127) as usize] + self.fConst97 * self.fVec97[((i32::wrapping_sub(self.IOTA0, 86)) & 127) as usize] + self.fConst96 * self.fVec96[((i32::wrapping_sub(self.IOTA0, 87)) & 127) as usize] + self.fConst95 * self.fVec95[((i32::wrapping_sub(self.IOTA0, 88)) & 127) as usize] + self.fConst94 * self.fVec94[((i32::wrapping_sub(self.IOTA0, 89)) & 127) as usize] + self.fConst93 * self.fVec93[((i32::wrapping_sub(self.IOTA0, 90)) & 127) as usize] + self.fConst92 * self.fVec92[((i32::wrapping_sub(self.IOTA0, 91)) & 127) as usize] + self.fConst91 * self.fVec91[((i32::wrapping_sub(self.IOTA0, 92)) & 127) as usize] + self.fConst90 * self.fVec90[((i32::wrapping_sub(self.IOTA0, 93)) & 127) as usize] + self.fConst89 * self.fVec89[((i32::wrapping_sub(self.IOTA0, 94)) & 127) as usize] + self.fConst88 * self.fVec88[((i32::wrapping_sub(self.IOTA0, 95)) & 127) as usize] + self.fConst87 * self.fVec87[((i32::wrapping_sub(self.IOTA0, 96)) & 127) as usize] + self.fConst86 * self.fVec86[((i32::wrapping_sub(self.IOTA0, 97)) & 127) as usize] + self.fConst85 * self.fVec85[((i32::wrapping_sub(self.IOTA0, 98)) & 127) as usize] + self.fConst84 * self.fVec84[((i32::wrapping_sub(self.IOTA0, 99)) & 127) as usize] + self.fConst83 * self.fVec83[((i32::wrapping_sub(self.IOTA0, 100)) & 127) as usize] + self.fConst82 * self.fVec82[((i32::wrapping_sub(self.IOTA0, 101)) & 127) as usize] + self.fConst81 * self.fVec81[((i32::wrapping_sub(self.IOTA0, 102)) & 127) as usize] + self.fConst80 * self.fVec80[((i32::wrapping_sub(self.IOTA0, 103)) & 127) as usize] + self.fConst79 * self.fVec79[((i32::wrapping_sub(self.IOTA0, 104)) & 127) as usize] + self.fConst78 * self.fVec78[((i32::wrapping_sub(self.IOTA0, 105)) & 127) as usize] + self.fConst77 * self.fVec77[((i32::wrapping_sub(self.IOTA0, 106)) & 127) as usize] + self.fConst76 * self.fVec76[((i32::wrapping_sub(self.IOTA0, 107)) & 127) as usize] + self.fConst75 * self.fVec75[((i32::wrapping_sub(self.IOTA0, 108)) & 127) as usize] + self.fConst74 * self.fVec74[((i32::wrapping_sub(self.IOTA0, 109)) & 127) as usize] + self.fConst73 * self.fVec73[((i32::wrapping_sub(self.IOTA0, 110)) & 127) as usize] + self.fConst72 * self.fVec72[((i32::wrapping_sub(self.IOTA0, 111)) & 127) as usize] + self.fConst71 * self.fVec71[((i32::wrapping_sub(self.IOTA0, 112)) & 127) as usize] + self.fConst70 * self.fVec70[((i32::wrapping_sub(self.IOTA0, 113)) & 127) as usize] + self.fConst69 * self.fVec69[((i32::wrapping_sub(self.IOTA0, 114)) & 127) as usize] + self.fConst68 * self.fVec68[((i32::wrapping_sub(self.IOTA0, 115)) & 127) as usize] + self.fConst67 * self.fVec67[((i32::wrapping_sub(self.IOTA0, 116)) & 127) as usize] + self.fConst66 * self.fVec66[((i32::wrapping_sub(self.IOTA0, 117)) & 127) as usize] + self.fConst65 * self.fVec65[((i32::wrapping_sub(self.IOTA0, 118)) & 127) as usize] + self.fConst64 * self.fVec64[((i32::wrapping_sub(self.IOTA0, 120)) & 127) as usize] + self.fConst63 * self.fVec63[((i32::wrapping_sub(self.IOTA0, 119)) & 127) as usize]} else {*input184 * self.fConst62 + self.fConst61 * self.fVec61[1] + self.fConst60 * self.fVec60[2] + self.fConst59 * self.fVec59[3] + self.fConst58 * self.fVec58[4] + self.fConst57 * self.fVec57[5] + self.fConst56 * self.fVec56[6] + self.fConst55 * self.fVec55[7] + self.fConst54 * self.fVec54[8] + self.fConst53 * self.fVec53[9] + self.fConst52 * self.fVec52[10] + self.fConst51 * self.fVec51[11] + self.fConst50 * self.fVec50[12] + self.fConst49 * self.fVec49[13] + self.fConst48 * self.fVec48[14] + self.fConst47 * self.fVec47[15] + self.fConst46 * self.fVec46[((i32::wrapping_sub(self.IOTA0, 16)) & 31) as usize] + self.fConst45 * self.fVec45[((i32::wrapping_sub(self.IOTA0, 17)) & 31) as usize] + self.fConst44 * self.fVec44[((i32::wrapping_sub(self.IOTA0, 18)) & 31) as usize] + self.fConst43 * self.fVec43[((i32::wrapping_sub(self.IOTA0, 19)) & 31) as usize] + self.fConst42 * self.fVec42[((i32::wrapping_sub(self.IOTA0, 20)) & 31) as usize] + self.fConst41 * self.fVec41[((i32::wrapping_sub(self.IOTA0, 21)) & 31) as usize] + self.fConst40 * self.fVec40[((i32::wrapping_sub(self.IOTA0, 22)) & 31) as usize] + self.fConst39 * self.fVec39[((i32::wrapping_sub(self.IOTA0, 23)) & 31) as usize] + self.fConst38 * self.fVec38[((i32::wrapping_sub(self.IOTA0, 24)) & 31) as usize] + self.fConst37 * self.fVec37[((i32::wrapping_sub(self.IOTA0, 25)) & 31) as usize] + self.fConst36 * self.fVec36[((i32::wrapping_sub(self.IOTA0, 26)) & 31) as usize] + self.fConst35 * self.fVec35[((i32::wrapping_sub(self.IOTA0, 27)) & 31) as usize] + self.fConst34 * self.fVec34[((i32::wrapping_sub(self.IOTA0, 28)) & 31) as usize] + self.fConst33 * self.fVec33[((i32::wrapping_sub(self.IOTA0, 29)) & 31) as usize] + self.fConst32 * self.fVec32[((i32::wrapping_sub(self.IOTA0, 30)) & 31) as usize] + self.fConst31 * self.fVec31[((i32::wrapping_sub(self.IOTA0, 31)) & 31) as usize] + self.fConst30 * self.fVec30[((i32::wrapping_sub(self.IOTA0, 32)) & 63) as usize] + self.fConst29 * self.fVec29[((i32::wrapping_sub(self.IOTA0, 33)) & 63) as usize] + self.fConst28 * self.fVec28[((i32::wrapping_sub(self.IOTA0, 34)) & 63) as usize] + self.fConst27 * self.fVec27[((i32::wrapping_sub(self.IOTA0, 35)) & 63) as usize] + self.fConst26 * self.fVec26[((i32::wrapping_sub(self.IOTA0, 36)) & 63) as usize] + self.fConst25 * self.fVec25[((i32::wrapping_sub(self.IOTA0, 37)) & 63) as usize] + self.fConst24 * self.fVec24[((i32::wrapping_sub(self.IOTA0, 38)) & 63) as usize] + self.fConst23 * self.fVec23[((i32::wrapping_sub(self.IOTA0, 39)) & 63) as usize] + self.fConst22 * self.fVec22[((i32::wrapping_sub(self.IOTA0, 40)) & 63) as usize] + self.fConst21 * self.fVec21[((i32::wrapping_sub(self.IOTA0, 41)) & 63) as usize] + self.fConst20 * self.fVec20[((i32::wrapping_sub(self.IOTA0, 42)) & 63) as usize] + self.fConst19 * self.fVec19[((i32::wrapping_sub(self.IOTA0, 43)) & 63) as usize] + self.fConst18 * self.fVec18[((i32::wrapping_sub(self.IOTA0, 44)) & 63) as usize] + self.fConst17 * self.fVec17[((i32::wrapping_sub(self.IOTA0, 45)) & 63) as usize] + self.fConst16 * self.fVec16[((i32::wrapping_sub(self.IOTA0, 46)) & 63) as usize] + self.fConst15 * self.fVec15[((i32::wrapping_sub(self.IOTA0, 47)) & 63) as usize] + self.fConst14 * self.fVec14[((i32::wrapping_sub(self.IOTA0, 48)) & 63) as usize] + self.fConst13 * self.fVec13[((i32::wrapping_sub(self.IOTA0, 49)) & 63) as usize] + self.fConst12 * self.fVec12[((i32::wrapping_sub(self.IOTA0, 50)) & 63) as usize] + self.fConst11 * self.fVec11[((i32::wrapping_sub(self.IOTA0, 51)) & 63) as usize] + self.fConst10 * self.fVec10[((i32::wrapping_sub(self.IOTA0, 52)) & 63) as usize] + self.fConst9 * self.fVec9[((i32::wrapping_sub(self.IOTA0, 53)) & 63) as usize] + self.fConst8 * self.fVec8[((i32::wrapping_sub(self.IOTA0, 54)) & 63) as usize] + self.fConst7 * self.fVec7[((i32::wrapping_sub(self.IOTA0, 55)) & 63) as usize] + self.fConst6 * self.fVec6[((i32::wrapping_sub(self.IOTA0, 56)) & 63) as usize] + self.fConst5 * self.fVec5[((i32::wrapping_sub(self.IOTA0, 57)) & 63) as usize] + self.fConst4 * self.fVec4[((i32::wrapping_sub(self.IOTA0, 58)) & 63) as usize] + self.fConst3 * self.fVec3[((i32::wrapping_sub(self.IOTA0, 59)) & 63) as usize] + self.fConst2 * self.fVec2[((i32::wrapping_sub(self.IOTA0, 60)) & 63) as usize] + self.fConst1 * self.fVec1[((i32::wrapping_sub(self.IOTA0, 62)) & 63) as usize] + self.fConst0 * self.fVec0[((i32::wrapping_sub(self.IOTA0, 61)) & 63) as usize]})));
			self.fVec365[(self.IOTA0 & 63) as usize] = *input614;
			self.fVec366[(self.IOTA0 & 63) as usize] = *input613;
			self.fVec367[(self.IOTA0 & 63) as usize] = *input612;
			self.fVec368[(self.IOTA0 & 63) as usize] = *input611;
			self.fVec369[(self.IOTA0 & 63) as usize] = *input610;
			self.fVec370[(self.IOTA0 & 63) as usize] = *input609;
			self.fVec371[(self.IOTA0 & 63) as usize] = *input608;
			self.fVec372[(self.IOTA0 & 63) as usize] = *input607;
			self.fVec373[(self.IOTA0 & 63) as usize] = *input606;
			self.fVec374[(self.IOTA0 & 63) as usize] = *input605;
			self.fVec375[(self.IOTA0 & 63) as usize] = *input604;
			self.fVec376[(self.IOTA0 & 63) as usize] = *input603;
			self.fVec377[(self.IOTA0 & 63) as usize] = *input602;
			self.fVec378[(self.IOTA0 & 63) as usize] = *input601;
			self.fVec379[(self.IOTA0 & 63) as usize] = *input600;
			self.fVec380[(self.IOTA0 & 63) as usize] = *input599;
			self.fVec381[(self.IOTA0 & 63) as usize] = *input598;
			self.fVec382[(self.IOTA0 & 63) as usize] = *input597;
			self.fVec383[(self.IOTA0 & 63) as usize] = *input596;
			self.fVec384[(self.IOTA0 & 63) as usize] = *input595;
			self.fVec385[(self.IOTA0 & 63) as usize] = *input594;
			self.fVec386[(self.IOTA0 & 63) as usize] = *input593;
			self.fVec387[(self.IOTA0 & 63) as usize] = *input592;
			self.fVec388[(self.IOTA0 & 63) as usize] = *input591;
			self.fVec389[(self.IOTA0 & 63) as usize] = *input590;
			self.fVec390[(self.IOTA0 & 63) as usize] = *input589;
			self.fVec391[(self.IOTA0 & 63) as usize] = *input588;
			self.fVec392[(self.IOTA0 & 63) as usize] = *input587;
			self.fVec393[(self.IOTA0 & 63) as usize] = *input586;
			self.fVec394[(self.IOTA0 & 63) as usize] = *input585;
			self.fVec395[(self.IOTA0 & 63) as usize] = *input584;
			self.fVec396[(self.IOTA0 & 31) as usize] = *input583;
			self.fVec397[(self.IOTA0 & 31) as usize] = *input582;
			self.fVec398[(self.IOTA0 & 31) as usize] = *input581;
			self.fVec399[(self.IOTA0 & 31) as usize] = *input580;
			self.fVec400[(self.IOTA0 & 31) as usize] = *input579;
			self.fVec401[(self.IOTA0 & 31) as usize] = *input578;
			self.fVec402[(self.IOTA0 & 31) as usize] = *input577;
			self.fVec403[(self.IOTA0 & 31) as usize] = *input576;
			self.fVec404[(self.IOTA0 & 31) as usize] = *input575;
			self.fVec405[(self.IOTA0 & 31) as usize] = *input574;
			self.fVec406[(self.IOTA0 & 31) as usize] = *input573;
			self.fVec407[(self.IOTA0 & 31) as usize] = *input572;
			self.fVec408[(self.IOTA0 & 31) as usize] = *input571;
			self.fVec409[(self.IOTA0 & 31) as usize] = *input570;
			self.fVec410[(self.IOTA0 & 31) as usize] = *input569;
			self.fVec411[(self.IOTA0 & 31) as usize] = *input568;
			self.fVec412[0] = *input567;
			self.fVec413[0] = *input566;
			self.fVec414[0] = *input565;
			self.fVec415[0] = *input564;
			self.fVec416[0] = *input563;
			self.fVec417[0] = *input562;
			self.fVec418[0] = *input561;
			self.fVec419[0] = *input560;
			self.fVec420[0] = *input559;
			self.fVec421[0] = *input558;
			self.fVec422[0] = *input557;
			self.fVec423[0] = *input556;
			self.fVec424[0] = *input555;
			self.fVec425[0] = *input554;
			self.fVec426[0] = *input553;
			self.fVec427[(self.IOTA0 & 127) as usize] = *input735;
			self.fVec428[(self.IOTA0 & 127) as usize] = *input734;
			self.fVec429[(self.IOTA0 & 127) as usize] = *input733;
			self.fVec430[(self.IOTA0 & 127) as usize] = *input732;
			self.fVec431[(self.IOTA0 & 127) as usize] = *input731;
			self.fVec432[(self.IOTA0 & 127) as usize] = *input730;
			self.fVec433[(self.IOTA0 & 127) as usize] = *input729;
			self.fVec434[(self.IOTA0 & 127) as usize] = *input728;
			self.fVec435[(self.IOTA0 & 127) as usize] = *input727;
			self.fVec436[(self.IOTA0 & 127) as usize] = *input726;
			self.fVec437[(self.IOTA0 & 127) as usize] = *input725;
			self.fVec438[(self.IOTA0 & 127) as usize] = *input724;
			self.fVec439[(self.IOTA0 & 127) as usize] = *input723;
			self.fVec440[(self.IOTA0 & 127) as usize] = *input722;
			self.fVec441[(self.IOTA0 & 127) as usize] = *input721;
			self.fVec442[(self.IOTA0 & 127) as usize] = *input720;
			self.fVec443[(self.IOTA0 & 127) as usize] = *input719;
			self.fVec444[(self.IOTA0 & 127) as usize] = *input718;
			self.fVec445[(self.IOTA0 & 127) as usize] = *input717;
			self.fVec446[(self.IOTA0 & 127) as usize] = *input716;
			self.fVec447[(self.IOTA0 & 127) as usize] = *input715;
			self.fVec448[(self.IOTA0 & 127) as usize] = *input714;
			self.fVec449[(self.IOTA0 & 127) as usize] = *input713;
			self.fVec450[(self.IOTA0 & 127) as usize] = *input712;
			self.fVec451[(self.IOTA0 & 127) as usize] = *input711;
			self.fVec452[(self.IOTA0 & 127) as usize] = *input710;
			self.fVec453[(self.IOTA0 & 127) as usize] = *input709;
			self.fVec454[(self.IOTA0 & 127) as usize] = *input708;
			self.fVec455[(self.IOTA0 & 127) as usize] = *input707;
			self.fVec456[(self.IOTA0 & 127) as usize] = *input706;
			self.fVec457[(self.IOTA0 & 127) as usize] = *input705;
			self.fVec458[(self.IOTA0 & 127) as usize] = *input704;
			self.fVec459[(self.IOTA0 & 127) as usize] = *input703;
			self.fVec460[(self.IOTA0 & 127) as usize] = *input702;
			self.fVec461[(self.IOTA0 & 127) as usize] = *input701;
			self.fVec462[(self.IOTA0 & 127) as usize] = *input700;
			self.fVec463[(self.IOTA0 & 127) as usize] = *input699;
			self.fVec464[(self.IOTA0 & 127) as usize] = *input698;
			self.fVec465[(self.IOTA0 & 127) as usize] = *input697;
			self.fVec466[(self.IOTA0 & 127) as usize] = *input696;
			self.fVec467[(self.IOTA0 & 127) as usize] = *input695;
			self.fVec468[(self.IOTA0 & 127) as usize] = *input694;
			self.fVec469[(self.IOTA0 & 127) as usize] = *input693;
			self.fVec470[(self.IOTA0 & 127) as usize] = *input692;
			self.fVec471[(self.IOTA0 & 127) as usize] = *input691;
			self.fVec472[(self.IOTA0 & 127) as usize] = *input690;
			self.fVec473[(self.IOTA0 & 127) as usize] = *input689;
			self.fVec474[(self.IOTA0 & 127) as usize] = *input688;
			self.fVec475[(self.IOTA0 & 127) as usize] = *input687;
			self.fVec476[(self.IOTA0 & 127) as usize] = *input686;
			self.fVec477[(self.IOTA0 & 127) as usize] = *input685;
			self.fVec478[(self.IOTA0 & 127) as usize] = *input684;
			self.fVec479[(self.IOTA0 & 127) as usize] = *input683;
			self.fVec480[(self.IOTA0 & 127) as usize] = *input682;
			self.fVec481[(self.IOTA0 & 127) as usize] = *input681;
			self.fVec482[(self.IOTA0 & 127) as usize] = *input680;
			self.fVec483[(self.IOTA0 & 127) as usize] = *input679;
			self.fVec484[(self.IOTA0 & 63) as usize] = *input678;
			self.fVec485[(self.IOTA0 & 63) as usize] = *input677;
			self.fVec486[(self.IOTA0 & 63) as usize] = *input676;
			self.fVec487[(self.IOTA0 & 63) as usize] = *input675;
			self.fVec488[(self.IOTA0 & 63) as usize] = *input674;
			self.fVec489[(self.IOTA0 & 63) as usize] = *input673;
			self.fVec490[(self.IOTA0 & 63) as usize] = *input672;
			self.fVec491[(self.IOTA0 & 63) as usize] = *input671;
			self.fVec492[(self.IOTA0 & 63) as usize] = *input670;
			self.fVec493[(self.IOTA0 & 63) as usize] = *input669;
			self.fVec494[(self.IOTA0 & 63) as usize] = *input668;
			self.fVec495[(self.IOTA0 & 63) as usize] = *input667;
			self.fVec496[(self.IOTA0 & 63) as usize] = *input666;
			self.fVec497[(self.IOTA0 & 63) as usize] = *input665;
			self.fVec498[(self.IOTA0 & 63) as usize] = *input664;
			self.fVec499[(self.IOTA0 & 63) as usize] = *input663;
			self.fVec500[(self.IOTA0 & 63) as usize] = *input662;
			self.fVec501[(self.IOTA0 & 63) as usize] = *input661;
			self.fVec502[(self.IOTA0 & 63) as usize] = *input660;
			self.fVec503[(self.IOTA0 & 63) as usize] = *input659;
			self.fVec504[(self.IOTA0 & 63) as usize] = *input658;
			self.fVec505[(self.IOTA0 & 63) as usize] = *input657;
			self.fVec506[(self.IOTA0 & 63) as usize] = *input656;
			self.fVec507[(self.IOTA0 & 63) as usize] = *input655;
			self.fVec508[(self.IOTA0 & 63) as usize] = *input654;
			self.fVec509[(self.IOTA0 & 63) as usize] = *input653;
			self.fVec510[(self.IOTA0 & 63) as usize] = *input652;
			self.fVec511[(self.IOTA0 & 63) as usize] = *input651;
			self.fVec512[(self.IOTA0 & 63) as usize] = *input650;
			self.fVec513[(self.IOTA0 & 63) as usize] = *input649;
			self.fVec514[(self.IOTA0 & 63) as usize] = *input648;
			self.fVec515[(self.IOTA0 & 63) as usize] = *input647;
			self.fVec516[(self.IOTA0 & 31) as usize] = *input646;
			self.fVec517[(self.IOTA0 & 31) as usize] = *input645;
			self.fVec518[(self.IOTA0 & 31) as usize] = *input644;
			self.fVec519[(self.IOTA0 & 31) as usize] = *input643;
			self.fVec520[(self.IOTA0 & 31) as usize] = *input642;
			self.fVec521[(self.IOTA0 & 31) as usize] = *input641;
			self.fVec522[(self.IOTA0 & 31) as usize] = *input640;
			self.fVec523[(self.IOTA0 & 31) as usize] = *input639;
			self.fVec524[(self.IOTA0 & 31) as usize] = *input638;
			self.fVec525[(self.IOTA0 & 31) as usize] = *input637;
			self.fVec526[(self.IOTA0 & 31) as usize] = *input636;
			self.fVec527[(self.IOTA0 & 31) as usize] = *input635;
			self.fVec528[(self.IOTA0 & 31) as usize] = *input634;
			self.fVec529[(self.IOTA0 & 31) as usize] = *input633;
			self.fVec530[(self.IOTA0 & 31) as usize] = *input632;
			self.fVec531[(self.IOTA0 & 31) as usize] = *input631;
			self.fVec532[0] = *input630;
			self.fVec533[0] = *input629;
			self.fVec534[0] = *input628;
			self.fVec535[0] = *input627;
			self.fVec536[0] = *input626;
			self.fVec537[0] = *input625;
			self.fVec538[0] = *input624;
			self.fVec539[0] = *input623;
			self.fVec540[0] = *input622;
			self.fVec541[0] = *input621;
			self.fVec542[0] = *input620;
			self.fVec543[0] = *input619;
			self.fVec544[0] = *input618;
			self.fVec545[0] = *input617;
			self.fVec546[0] = *input616;
			self.fVec547[(self.IOTA0 & 63) as usize] = *input429;
			self.fVec548[(self.IOTA0 & 63) as usize] = *input430;
			self.fVec549[(self.IOTA0 & 63) as usize] = *input428;
			self.fVec550[(self.IOTA0 & 63) as usize] = *input427;
			self.fVec551[(self.IOTA0 & 63) as usize] = *input426;
			self.fVec552[(self.IOTA0 & 63) as usize] = *input425;
			self.fVec553[(self.IOTA0 & 63) as usize] = *input424;
			self.fVec554[(self.IOTA0 & 63) as usize] = *input423;
			self.fVec555[(self.IOTA0 & 63) as usize] = *input422;
			self.fVec556[(self.IOTA0 & 63) as usize] = *input421;
			self.fVec557[(self.IOTA0 & 63) as usize] = *input420;
			self.fVec558[(self.IOTA0 & 63) as usize] = *input419;
			self.fVec559[(self.IOTA0 & 63) as usize] = *input418;
			self.fVec560[(self.IOTA0 & 63) as usize] = *input417;
			self.fVec561[(self.IOTA0 & 63) as usize] = *input416;
			self.fVec562[(self.IOTA0 & 63) as usize] = *input415;
			self.fVec563[(self.IOTA0 & 63) as usize] = *input414;
			self.fVec564[(self.IOTA0 & 63) as usize] = *input413;
			self.fVec565[(self.IOTA0 & 63) as usize] = *input412;
			self.fVec566[(self.IOTA0 & 63) as usize] = *input411;
			self.fVec567[(self.IOTA0 & 63) as usize] = *input410;
			self.fVec568[(self.IOTA0 & 63) as usize] = *input409;
			self.fVec569[(self.IOTA0 & 63) as usize] = *input408;
			self.fVec570[(self.IOTA0 & 63) as usize] = *input407;
			self.fVec571[(self.IOTA0 & 63) as usize] = *input406;
			self.fVec572[(self.IOTA0 & 63) as usize] = *input405;
			self.fVec573[(self.IOTA0 & 63) as usize] = *input404;
			self.fVec574[(self.IOTA0 & 63) as usize] = *input403;
			self.fVec575[(self.IOTA0 & 63) as usize] = *input402;
			self.fVec576[(self.IOTA0 & 63) as usize] = *input401;
			self.fVec577[(self.IOTA0 & 63) as usize] = *input400;
			self.fVec578[(self.IOTA0 & 31) as usize] = *input399;
			self.fVec579[(self.IOTA0 & 31) as usize] = *input398;
			self.fVec580[(self.IOTA0 & 31) as usize] = *input397;
			self.fVec581[(self.IOTA0 & 31) as usize] = *input396;
			self.fVec582[(self.IOTA0 & 31) as usize] = *input395;
			self.fVec583[(self.IOTA0 & 31) as usize] = *input394;
			self.fVec584[(self.IOTA0 & 31) as usize] = *input393;
			self.fVec585[(self.IOTA0 & 31) as usize] = *input392;
			self.fVec586[(self.IOTA0 & 31) as usize] = *input391;
			self.fVec587[(self.IOTA0 & 31) as usize] = *input390;
			self.fVec588[(self.IOTA0 & 31) as usize] = *input389;
			self.fVec589[(self.IOTA0 & 31) as usize] = *input388;
			self.fVec590[(self.IOTA0 & 31) as usize] = *input387;
			self.fVec591[(self.IOTA0 & 31) as usize] = *input386;
			self.fVec592[(self.IOTA0 & 31) as usize] = *input385;
			self.fVec593[(self.IOTA0 & 31) as usize] = *input384;
			self.fVec594[0] = *input383;
			self.fVec595[0] = *input382;
			self.fVec596[0] = *input381;
			self.fVec597[0] = *input380;
			self.fVec598[0] = *input379;
			self.fVec599[0] = *input378;
			self.fVec600[0] = *input377;
			self.fVec601[0] = *input376;
			self.fVec602[0] = *input375;
			self.fVec603[0] = *input374;
			self.fVec604[0] = *input373;
			self.fVec605[0] = *input372;
			self.fVec606[0] = *input371;
			self.fVec607[0] = *input370;
			self.fVec608[0] = *input369;
			self.fVec609[(self.IOTA0 & 127) as usize] = *input550;
			self.fVec610[(self.IOTA0 & 127) as usize] = *input551;
			self.fVec611[(self.IOTA0 & 127) as usize] = *input549;
			self.fVec612[(self.IOTA0 & 127) as usize] = *input548;
			self.fVec613[(self.IOTA0 & 127) as usize] = *input547;
			self.fVec614[(self.IOTA0 & 127) as usize] = *input546;
			self.fVec615[(self.IOTA0 & 127) as usize] = *input545;
			self.fVec616[(self.IOTA0 & 127) as usize] = *input544;
			self.fVec617[(self.IOTA0 & 127) as usize] = *input543;
			self.fVec618[(self.IOTA0 & 127) as usize] = *input542;
			self.fVec619[(self.IOTA0 & 127) as usize] = *input541;
			self.fVec620[(self.IOTA0 & 127) as usize] = *input540;
			self.fVec621[(self.IOTA0 & 127) as usize] = *input539;
			self.fVec622[(self.IOTA0 & 127) as usize] = *input538;
			self.fVec623[(self.IOTA0 & 127) as usize] = *input537;
			self.fVec624[(self.IOTA0 & 127) as usize] = *input536;
			self.fVec625[(self.IOTA0 & 127) as usize] = *input535;
			self.fVec626[(self.IOTA0 & 127) as usize] = *input534;
			self.fVec627[(self.IOTA0 & 127) as usize] = *input533;
			self.fVec628[(self.IOTA0 & 127) as usize] = *input532;
			self.fVec629[(self.IOTA0 & 127) as usize] = *input531;
			self.fVec630[(self.IOTA0 & 127) as usize] = *input530;
			self.fVec631[(self.IOTA0 & 127) as usize] = *input529;
			self.fVec632[(self.IOTA0 & 127) as usize] = *input528;
			self.fVec633[(self.IOTA0 & 127) as usize] = *input527;
			self.fVec634[(self.IOTA0 & 127) as usize] = *input526;
			self.fVec635[(self.IOTA0 & 127) as usize] = *input525;
			self.fVec636[(self.IOTA0 & 127) as usize] = *input524;
			self.fVec637[(self.IOTA0 & 127) as usize] = *input523;
			self.fVec638[(self.IOTA0 & 127) as usize] = *input522;
			self.fVec639[(self.IOTA0 & 127) as usize] = *input521;
			self.fVec640[(self.IOTA0 & 127) as usize] = *input520;
			self.fVec641[(self.IOTA0 & 127) as usize] = *input519;
			self.fVec642[(self.IOTA0 & 127) as usize] = *input518;
			self.fVec643[(self.IOTA0 & 127) as usize] = *input517;
			self.fVec644[(self.IOTA0 & 127) as usize] = *input516;
			self.fVec645[(self.IOTA0 & 127) as usize] = *input515;
			self.fVec646[(self.IOTA0 & 127) as usize] = *input514;
			self.fVec647[(self.IOTA0 & 127) as usize] = *input513;
			self.fVec648[(self.IOTA0 & 127) as usize] = *input512;
			self.fVec649[(self.IOTA0 & 127) as usize] = *input511;
			self.fVec650[(self.IOTA0 & 127) as usize] = *input510;
			self.fVec651[(self.IOTA0 & 127) as usize] = *input509;
			self.fVec652[(self.IOTA0 & 127) as usize] = *input508;
			self.fVec653[(self.IOTA0 & 127) as usize] = *input507;
			self.fVec654[(self.IOTA0 & 127) as usize] = *input506;
			self.fVec655[(self.IOTA0 & 127) as usize] = *input505;
			self.fVec656[(self.IOTA0 & 127) as usize] = *input504;
			self.fVec657[(self.IOTA0 & 127) as usize] = *input503;
			self.fVec658[(self.IOTA0 & 127) as usize] = *input502;
			self.fVec659[(self.IOTA0 & 127) as usize] = *input501;
			self.fVec660[(self.IOTA0 & 127) as usize] = *input500;
			self.fVec661[(self.IOTA0 & 127) as usize] = *input499;
			self.fVec662[(self.IOTA0 & 127) as usize] = *input498;
			self.fVec663[(self.IOTA0 & 127) as usize] = *input497;
			self.fVec664[(self.IOTA0 & 127) as usize] = *input496;
			self.fVec665[(self.IOTA0 & 127) as usize] = *input495;
			self.fVec666[(self.IOTA0 & 63) as usize] = *input494;
			self.fVec667[(self.IOTA0 & 63) as usize] = *input493;
			self.fVec668[(self.IOTA0 & 63) as usize] = *input492;
			self.fVec669[(self.IOTA0 & 63) as usize] = *input491;
			self.fVec670[(self.IOTA0 & 63) as usize] = *input490;
			self.fVec671[(self.IOTA0 & 63) as usize] = *input489;
			self.fVec672[(self.IOTA0 & 63) as usize] = *input488;
			self.fVec673[(self.IOTA0 & 63) as usize] = *input487;
			self.fVec674[(self.IOTA0 & 63) as usize] = *input486;
			self.fVec675[(self.IOTA0 & 63) as usize] = *input485;
			self.fVec676[(self.IOTA0 & 63) as usize] = *input484;
			self.fVec677[(self.IOTA0 & 63) as usize] = *input483;
			self.fVec678[(self.IOTA0 & 63) as usize] = *input482;
			self.fVec679[(self.IOTA0 & 63) as usize] = *input481;
			self.fVec680[(self.IOTA0 & 63) as usize] = *input480;
			self.fVec681[(self.IOTA0 & 63) as usize] = *input479;
			self.fVec682[(self.IOTA0 & 63) as usize] = *input478;
			self.fVec683[(self.IOTA0 & 63) as usize] = *input477;
			self.fVec684[(self.IOTA0 & 63) as usize] = *input476;
			self.fVec685[(self.IOTA0 & 63) as usize] = *input475;
			self.fVec686[(self.IOTA0 & 63) as usize] = *input474;
			self.fVec687[(self.IOTA0 & 63) as usize] = *input473;
			self.fVec688[(self.IOTA0 & 63) as usize] = *input472;
			self.fVec689[(self.IOTA0 & 63) as usize] = *input471;
			self.fVec690[(self.IOTA0 & 63) as usize] = *input470;
			self.fVec691[(self.IOTA0 & 63) as usize] = *input469;
			self.fVec692[(self.IOTA0 & 63) as usize] = *input468;
			self.fVec693[(self.IOTA0 & 63) as usize] = *input467;
			self.fVec694[(self.IOTA0 & 63) as usize] = *input466;
			self.fVec695[(self.IOTA0 & 63) as usize] = *input465;
			self.fVec696[(self.IOTA0 & 63) as usize] = *input464;
			self.fVec697[(self.IOTA0 & 63) as usize] = *input463;
			self.fVec698[(self.IOTA0 & 31) as usize] = *input462;
			self.fVec699[(self.IOTA0 & 31) as usize] = *input461;
			self.fVec700[(self.IOTA0 & 31) as usize] = *input460;
			self.fVec701[(self.IOTA0 & 31) as usize] = *input459;
			self.fVec702[(self.IOTA0 & 31) as usize] = *input458;
			self.fVec703[(self.IOTA0 & 31) as usize] = *input457;
			self.fVec704[(self.IOTA0 & 31) as usize] = *input456;
			self.fVec705[(self.IOTA0 & 31) as usize] = *input455;
			self.fVec706[(self.IOTA0 & 31) as usize] = *input454;
			self.fVec707[(self.IOTA0 & 31) as usize] = *input453;
			self.fVec708[(self.IOTA0 & 31) as usize] = *input452;
			self.fVec709[(self.IOTA0 & 31) as usize] = *input451;
			self.fVec710[(self.IOTA0 & 31) as usize] = *input450;
			self.fVec711[(self.IOTA0 & 31) as usize] = *input449;
			self.fVec712[(self.IOTA0 & 31) as usize] = *input448;
			self.fVec713[(self.IOTA0 & 31) as usize] = *input447;
			self.fVec714[0] = *input446;
			self.fVec715[0] = *input445;
			self.fVec716[0] = *input444;
			self.fVec717[0] = *input443;
			self.fVec718[0] = *input442;
			self.fVec719[0] = *input441;
			self.fVec720[0] = *input440;
			self.fVec721[0] = *input439;
			self.fVec722[0] = *input438;
			self.fVec723[0] = *input437;
			self.fVec724[0] = *input436;
			self.fVec725[0] = *input435;
			self.fVec726[0] = *input434;
			self.fVec727[0] = *input433;
			self.fVec728[0] = *input432;
			*output1 = fSlow3 * (self.fRec6[0] * F32::sin(self.fRec0[0] * (if iSlow0 != 0 {*input431 * self.fConst183 + self.fConst182 * self.fVec728[1] + self.fConst181 * self.fVec727[2] + self.fConst180 * self.fVec726[3] + self.fConst179 * self.fVec725[4] + self.fConst178 * self.fVec724[5] + self.fConst177 * self.fVec723[6] + self.fConst176 * self.fVec722[7] + self.fConst175 * self.fVec721[8] + self.fConst174 * self.fVec720[9] + self.fConst173 * self.fVec719[10] + self.fConst172 * self.fVec718[11] + self.fConst171 * self.fVec717[12] + self.fConst170 * self.fVec716[13] + self.fConst169 * self.fVec715[14] + self.fConst168 * self.fVec714[15] + self.fConst167 * self.fVec713[((i32::wrapping_sub(self.IOTA0, 16)) & 31) as usize] + self.fConst166 * self.fVec712[((i32::wrapping_sub(self.IOTA0, 17)) & 31) as usize] + self.fConst165 * self.fVec711[((i32::wrapping_sub(self.IOTA0, 18)) & 31) as usize] + self.fConst164 * self.fVec710[((i32::wrapping_sub(self.IOTA0, 19)) & 31) as usize] + self.fConst163 * self.fVec709[((i32::wrapping_sub(self.IOTA0, 20)) & 31) as usize] + self.fConst162 * self.fVec708[((i32::wrapping_sub(self.IOTA0, 21)) & 31) as usize] + self.fConst161 * self.fVec707[((i32::wrapping_sub(self.IOTA0, 22)) & 31) as usize] + self.fConst160 * self.fVec706[((i32::wrapping_sub(self.IOTA0, 23)) & 31) as usize] + self.fConst159 * self.fVec705[((i32::wrapping_sub(self.IOTA0, 24)) & 31) as usize] + self.fConst158 * self.fVec704[((i32::wrapping_sub(self.IOTA0, 25)) & 31) as usize] + self.fConst157 * self.fVec703[((i32::wrapping_sub(self.IOTA0, 26)) & 31) as usize] + self.fConst156 * self.fVec702[((i32::wrapping_sub(self.IOTA0, 27)) & 31) as usize] + self.fConst155 * self.fVec701[((i32::wrapping_sub(self.IOTA0, 28)) & 31) as usize] + self.fConst154 * self.fVec700[((i32::wrapping_sub(self.IOTA0, 29)) & 31) as usize] + self.fConst153 * self.fVec699[((i32::wrapping_sub(self.IOTA0, 30)) & 31) as usize] + self.fConst152 * self.fVec698[((i32::wrapping_sub(self.IOTA0, 31)) & 31) as usize] + self.fConst151 * self.fVec697[((i32::wrapping_sub(self.IOTA0, 32)) & 63) as usize] + self.fConst150 * self.fVec696[((i32::wrapping_sub(self.IOTA0, 33)) & 63) as usize] + self.fConst149 * self.fVec695[((i32::wrapping_sub(self.IOTA0, 34)) & 63) as usize] + self.fConst148 * self.fVec694[((i32::wrapping_sub(self.IOTA0, 35)) & 63) as usize] + self.fConst147 * self.fVec693[((i32::wrapping_sub(self.IOTA0, 36)) & 63) as usize] + self.fConst146 * self.fVec692[((i32::wrapping_sub(self.IOTA0, 37)) & 63) as usize] + self.fConst145 * self.fVec691[((i32::wrapping_sub(self.IOTA0, 38)) & 63) as usize] + self.fConst144 * self.fVec690[((i32::wrapping_sub(self.IOTA0, 39)) & 63) as usize] + self.fConst143 * self.fVec689[((i32::wrapping_sub(self.IOTA0, 40)) & 63) as usize] + self.fConst142 * self.fVec688[((i32::wrapping_sub(self.IOTA0, 41)) & 63) as usize] + self.fConst141 * self.fVec687[((i32::wrapping_sub(self.IOTA0, 42)) & 63) as usize] + self.fConst140 * self.fVec686[((i32::wrapping_sub(self.IOTA0, 43)) & 63) as usize] + self.fConst139 * self.fVec685[((i32::wrapping_sub(self.IOTA0, 44)) & 63) as usize] + self.fConst138 * self.fVec684[((i32::wrapping_sub(self.IOTA0, 45)) & 63) as usize] + self.fConst137 * self.fVec683[((i32::wrapping_sub(self.IOTA0, 46)) & 63) as usize] + self.fConst136 * self.fVec682[((i32::wrapping_sub(self.IOTA0, 47)) & 63) as usize] + self.fConst135 * self.fVec681[((i32::wrapping_sub(self.IOTA0, 48)) & 63) as usize] + self.fConst134 * self.fVec680[((i32::wrapping_sub(self.IOTA0, 49)) & 63) as usize] + self.fConst133 * self.fVec679[((i32::wrapping_sub(self.IOTA0, 50)) & 63) as usize] + self.fConst132 * self.fVec678[((i32::wrapping_sub(self.IOTA0, 51)) & 63) as usize] + self.fConst131 * self.fVec677[((i32::wrapping_sub(self.IOTA0, 52)) & 63) as usize] + self.fConst130 * self.fVec676[((i32::wrapping_sub(self.IOTA0, 53)) & 63) as usize] + self.fConst129 * self.fVec675[((i32::wrapping_sub(self.IOTA0, 54)) & 63) as usize] + self.fConst128 * self.fVec674[((i32::wrapping_sub(self.IOTA0, 55)) & 63) as usize] + self.fConst127 * self.fVec673[((i32::wrapping_sub(self.IOTA0, 56)) & 63) as usize] + self.fConst126 * self.fVec672[((i32::wrapping_sub(self.IOTA0, 57)) & 63) as usize] + self.fConst125 * self.fVec671[((i32::wrapping_sub(self.IOTA0, 58)) & 63) as usize] + self.fConst124 * self.fVec670[((i32::wrapping_sub(self.IOTA0, 59)) & 63) as usize] + self.fConst123 * self.fVec669[((i32::wrapping_sub(self.IOTA0, 60)) & 63) as usize] + self.fConst122 * self.fVec668[((i32::wrapping_sub(self.IOTA0, 61)) & 63) as usize] + self.fConst121 * self.fVec667[((i32::wrapping_sub(self.IOTA0, 62)) & 63) as usize] + self.fConst120 * self.fVec666[((i32::wrapping_sub(self.IOTA0, 63)) & 63) as usize] + self.fConst119 * self.fVec665[((i32::wrapping_sub(self.IOTA0, 64)) & 127) as usize] + self.fConst118 * self.fVec664[((i32::wrapping_sub(self.IOTA0, 65)) & 127) as usize] + self.fConst117 * self.fVec663[((i32::wrapping_sub(self.IOTA0, 66)) & 127) as usize] + self.fConst116 * self.fVec662[((i32::wrapping_sub(self.IOTA0, 67)) & 127) as usize] + self.fConst115 * self.fVec661[((i32::wrapping_sub(self.IOTA0, 68)) & 127) as usize] + self.fConst114 * self.fVec660[((i32::wrapping_sub(self.IOTA0, 69)) & 127) as usize] + self.fConst113 * self.fVec659[((i32::wrapping_sub(self.IOTA0, 70)) & 127) as usize] + self.fConst112 * self.fVec658[((i32::wrapping_sub(self.IOTA0, 71)) & 127) as usize] + self.fConst111 * self.fVec657[((i32::wrapping_sub(self.IOTA0, 72)) & 127) as usize] + self.fConst110 * self.fVec656[((i32::wrapping_sub(self.IOTA0, 73)) & 127) as usize] + self.fConst109 * self.fVec655[((i32::wrapping_sub(self.IOTA0, 74)) & 127) as usize] + self.fConst108 * self.fVec654[((i32::wrapping_sub(self.IOTA0, 75)) & 127) as usize] + self.fConst107 * self.fVec653[((i32::wrapping_sub(self.IOTA0, 76)) & 127) as usize] + self.fConst106 * self.fVec652[((i32::wrapping_sub(self.IOTA0, 77)) & 127) as usize] + self.fConst105 * self.fVec651[((i32::wrapping_sub(self.IOTA0, 78)) & 127) as usize] + self.fConst104 * self.fVec650[((i32::wrapping_sub(self.IOTA0, 79)) & 127) as usize] + self.fConst103 * self.fVec649[((i32::wrapping_sub(self.IOTA0, 80)) & 127) as usize] + self.fConst102 * self.fVec648[((i32::wrapping_sub(self.IOTA0, 81)) & 127) as usize] + self.fConst101 * self.fVec647[((i32::wrapping_sub(self.IOTA0, 82)) & 127) as usize] + self.fConst100 * self.fVec646[((i32::wrapping_sub(self.IOTA0, 83)) & 127) as usize] + self.fConst99 * self.fVec645[((i32::wrapping_sub(self.IOTA0, 84)) & 127) as usize] + self.fConst98 * self.fVec644[((i32::wrapping_sub(self.IOTA0, 85)) & 127) as usize] + self.fConst97 * self.fVec643[((i32::wrapping_sub(self.IOTA0, 86)) & 127) as usize] + self.fConst96 * self.fVec642[((i32::wrapping_sub(self.IOTA0, 87)) & 127) as usize] + self.fConst95 * self.fVec641[((i32::wrapping_sub(self.IOTA0, 88)) & 127) as usize] + self.fConst94 * self.fVec640[((i32::wrapping_sub(self.IOTA0, 89)) & 127) as usize] + self.fConst93 * self.fVec639[((i32::wrapping_sub(self.IOTA0, 90)) & 127) as usize] + self.fConst92 * self.fVec638[((i32::wrapping_sub(self.IOTA0, 91)) & 127) as usize] + self.fConst91 * self.fVec637[((i32::wrapping_sub(self.IOTA0, 92)) & 127) as usize] + self.fConst90 * self.fVec636[((i32::wrapping_sub(self.IOTA0, 93)) & 127) as usize] + self.fConst89 * self.fVec635[((i32::wrapping_sub(self.IOTA0, 94)) & 127) as usize] + self.fConst88 * self.fVec634[((i32::wrapping_sub(self.IOTA0, 95)) & 127) as usize] + self.fConst87 * self.fVec633[((i32::wrapping_sub(self.IOTA0, 96)) & 127) as usize] + self.fConst86 * self.fVec632[((i32::wrapping_sub(self.IOTA0, 97)) & 127) as usize] + self.fConst85 * self.fVec631[((i32::wrapping_sub(self.IOTA0, 98)) & 127) as usize] + self.fConst84 * self.fVec630[((i32::wrapping_sub(self.IOTA0, 99)) & 127) as usize] + self.fConst83 * self.fVec629[((i32::wrapping_sub(self.IOTA0, 100)) & 127) as usize] + self.fConst82 * self.fVec628[((i32::wrapping_sub(self.IOTA0, 101)) & 127) as usize] + self.fConst81 * self.fVec627[((i32::wrapping_sub(self.IOTA0, 102)) & 127) as usize] + self.fConst80 * self.fVec626[((i32::wrapping_sub(self.IOTA0, 103)) & 127) as usize] + self.fConst79 * self.fVec625[((i32::wrapping_sub(self.IOTA0, 104)) & 127) as usize] + self.fConst78 * self.fVec624[((i32::wrapping_sub(self.IOTA0, 105)) & 127) as usize] + self.fConst77 * self.fVec623[((i32::wrapping_sub(self.IOTA0, 106)) & 127) as usize] + self.fConst76 * self.fVec622[((i32::wrapping_sub(self.IOTA0, 107)) & 127) as usize] + self.fConst75 * self.fVec621[((i32::wrapping_sub(self.IOTA0, 108)) & 127) as usize] + self.fConst74 * self.fVec620[((i32::wrapping_sub(self.IOTA0, 109)) & 127) as usize] + self.fConst73 * self.fVec619[((i32::wrapping_sub(self.IOTA0, 110)) & 127) as usize] + self.fConst72 * self.fVec618[((i32::wrapping_sub(self.IOTA0, 111)) & 127) as usize] + self.fConst71 * self.fVec617[((i32::wrapping_sub(self.IOTA0, 112)) & 127) as usize] + self.fConst70 * self.fVec616[((i32::wrapping_sub(self.IOTA0, 113)) & 127) as usize] + self.fConst69 * self.fVec615[((i32::wrapping_sub(self.IOTA0, 114)) & 127) as usize] + self.fConst68 * self.fVec614[((i32::wrapping_sub(self.IOTA0, 115)) & 127) as usize] + self.fConst67 * self.fVec613[((i32::wrapping_sub(self.IOTA0, 116)) & 127) as usize] + self.fConst66 * self.fVec612[((i32::wrapping_sub(self.IOTA0, 117)) & 127) as usize] + self.fConst65 * self.fVec611[((i32::wrapping_sub(self.IOTA0, 118)) & 127) as usize] + self.fConst64 * self.fVec610[((i32::wrapping_sub(self.IOTA0, 120)) & 127) as usize] + self.fConst63 * self.fVec609[((i32::wrapping_sub(self.IOTA0, 119)) & 127) as usize]} else {*input368 * self.fConst62 + self.fConst61 * self.fVec608[1] + self.fConst60 * self.fVec607[2] + self.fConst59 * self.fVec606[3] + self.fConst58 * self.fVec605[4] + self.fConst57 * self.fVec604[5] + self.fConst56 * self.fVec603[6] + self.fConst55 * self.fVec602[7] + self.fConst54 * self.fVec601[8] + self.fConst53 * self.fVec600[9] + self.fConst52 * self.fVec599[10] + self.fConst51 * self.fVec598[11] + self.fConst50 * self.fVec597[12] + self.fConst49 * self.fVec596[13] + self.fConst48 * self.fVec595[14] + self.fConst47 * self.fVec594[15] + self.fConst46 * self.fVec593[((i32::wrapping_sub(self.IOTA0, 16)) & 31) as usize] + self.fConst45 * self.fVec592[((i32::wrapping_sub(self.IOTA0, 17)) & 31) as usize] + self.fConst44 * self.fVec591[((i32::wrapping_sub(self.IOTA0, 18)) & 31) as usize] + self.fConst43 * self.fVec590[((i32::wrapping_sub(self.IOTA0, 19)) & 31) as usize] + self.fConst42 * self.fVec589[((i32::wrapping_sub(self.IOTA0, 20)) & 31) as usize] + self.fConst41 * self.fVec588[((i32::wrapping_sub(self.IOTA0, 21)) & 31) as usize] + self.fConst40 * self.fVec587[((i32::wrapping_sub(self.IOTA0, 22)) & 31) as usize] + self.fConst39 * self.fVec586[((i32::wrapping_sub(self.IOTA0, 23)) & 31) as usize] + self.fConst38 * self.fVec585[((i32::wrapping_sub(self.IOTA0, 24)) & 31) as usize] + self.fConst37 * self.fVec584[((i32::wrapping_sub(self.IOTA0, 25)) & 31) as usize] + self.fConst36 * self.fVec583[((i32::wrapping_sub(self.IOTA0, 26)) & 31) as usize] + self.fConst35 * self.fVec582[((i32::wrapping_sub(self.IOTA0, 27)) & 31) as usize] + self.fConst34 * self.fVec581[((i32::wrapping_sub(self.IOTA0, 28)) & 31) as usize] + self.fConst33 * self.fVec580[((i32::wrapping_sub(self.IOTA0, 29)) & 31) as usize] + self.fConst32 * self.fVec579[((i32::wrapping_sub(self.IOTA0, 30)) & 31) as usize] + self.fConst31 * self.fVec578[((i32::wrapping_sub(self.IOTA0, 31)) & 31) as usize] + self.fConst30 * self.fVec577[((i32::wrapping_sub(self.IOTA0, 32)) & 63) as usize] + self.fConst29 * self.fVec576[((i32::wrapping_sub(self.IOTA0, 33)) & 63) as usize] + self.fConst28 * self.fVec575[((i32::wrapping_sub(self.IOTA0, 34)) & 63) as usize] + self.fConst27 * self.fVec574[((i32::wrapping_sub(self.IOTA0, 35)) & 63) as usize] + self.fConst26 * self.fVec573[((i32::wrapping_sub(self.IOTA0, 36)) & 63) as usize] + self.fConst25 * self.fVec572[((i32::wrapping_sub(self.IOTA0, 37)) & 63) as usize] + self.fConst24 * self.fVec571[((i32::wrapping_sub(self.IOTA0, 38)) & 63) as usize] + self.fConst23 * self.fVec570[((i32::wrapping_sub(self.IOTA0, 39)) & 63) as usize] + self.fConst22 * self.fVec569[((i32::wrapping_sub(self.IOTA0, 40)) & 63) as usize] + self.fConst21 * self.fVec568[((i32::wrapping_sub(self.IOTA0, 41)) & 63) as usize] + self.fConst20 * self.fVec567[((i32::wrapping_sub(self.IOTA0, 42)) & 63) as usize] + self.fConst19 * self.fVec566[((i32::wrapping_sub(self.IOTA0, 43)) & 63) as usize] + self.fConst18 * self.fVec565[((i32::wrapping_sub(self.IOTA0, 44)) & 63) as usize] + self.fConst17 * self.fVec564[((i32::wrapping_sub(self.IOTA0, 45)) & 63) as usize] + self.fConst16 * self.fVec563[((i32::wrapping_sub(self.IOTA0, 46)) & 63) as usize] + self.fConst15 * self.fVec562[((i32::wrapping_sub(self.IOTA0, 47)) & 63) as usize] + self.fConst14 * self.fVec561[((i32::wrapping_sub(self.IOTA0, 48)) & 63) as usize] + self.fConst13 * self.fVec560[((i32::wrapping_sub(self.IOTA0, 49)) & 63) as usize] + self.fConst12 * self.fVec559[((i32::wrapping_sub(self.IOTA0, 50)) & 63) as usize] + self.fConst11 * self.fVec558[((i32::wrapping_sub(self.IOTA0, 51)) & 63) as usize] + self.fConst10 * self.fVec557[((i32::wrapping_sub(self.IOTA0, 52)) & 63) as usize] + self.fConst9 * self.fVec556[((i32::wrapping_sub(self.IOTA0, 53)) & 63) as usize] + self.fConst8 * self.fVec555[((i32::wrapping_sub(self.IOTA0, 54)) & 63) as usize] + self.fConst7 * self.fVec554[((i32::wrapping_sub(self.IOTA0, 55)) & 63) as usize] + self.fConst6 * self.fVec553[((i32::wrapping_sub(self.IOTA0, 56)) & 63) as usize] + self.fConst5 * self.fVec552[((i32::wrapping_sub(self.IOTA0, 57)) & 63) as usize] + self.fConst4 * self.fVec551[((i32::wrapping_sub(self.IOTA0, 58)) & 63) as usize] + self.fConst3 * self.fVec550[((i32::wrapping_sub(self.IOTA0, 59)) & 63) as usize] + self.fConst2 * self.fVec549[((i32::wrapping_sub(self.IOTA0, 60)) & 63) as usize] + self.fConst1 * self.fVec548[((i32::wrapping_sub(self.IOTA0, 62)) & 63) as usize] + self.fConst0 * self.fVec547[((i32::wrapping_sub(self.IOTA0, 61)) & 63) as usize]})) + self.fRec1[0] * F32::cos(self.fRec0[0] * (if iSlow0 != 0 {*input615 * self.fConst183 + self.fVec546[1] * self.fConst182 + self.fVec545[2] * self.fConst181 + self.fVec544[3] * self.fConst180 + self.fVec543[4] * self.fConst179 + self.fVec542[5] * self.fConst178 + self.fVec541[6] * self.fConst177 + self.fVec540[7] * self.fConst176 + self.fVec539[8] * self.fConst175 + self.fVec538[9] * self.fConst174 + self.fVec537[10] * self.fConst173 + self.fVec536[11] * self.fConst172 + self.fVec535[12] * self.fConst171 + self.fVec534[13] * self.fConst170 + self.fVec533[14] * self.fConst169 + self.fVec532[15] * self.fConst168 + self.fVec531[((i32::wrapping_sub(self.IOTA0, 16)) & 31) as usize] * self.fConst167 + self.fVec530[((i32::wrapping_sub(self.IOTA0, 17)) & 31) as usize] * self.fConst166 + self.fVec529[((i32::wrapping_sub(self.IOTA0, 18)) & 31) as usize] * self.fConst165 + self.fVec528[((i32::wrapping_sub(self.IOTA0, 19)) & 31) as usize] * self.fConst164 + self.fVec527[((i32::wrapping_sub(self.IOTA0, 20)) & 31) as usize] * self.fConst163 + self.fVec526[((i32::wrapping_sub(self.IOTA0, 21)) & 31) as usize] * self.fConst162 + self.fVec525[((i32::wrapping_sub(self.IOTA0, 22)) & 31) as usize] * self.fConst161 + self.fVec524[((i32::wrapping_sub(self.IOTA0, 23)) & 31) as usize] * self.fConst160 + self.fVec523[((i32::wrapping_sub(self.IOTA0, 24)) & 31) as usize] * self.fConst159 + self.fVec522[((i32::wrapping_sub(self.IOTA0, 25)) & 31) as usize] * self.fConst158 + self.fVec521[((i32::wrapping_sub(self.IOTA0, 26)) & 31) as usize] * self.fConst157 + self.fVec520[((i32::wrapping_sub(self.IOTA0, 27)) & 31) as usize] * self.fConst156 + self.fVec519[((i32::wrapping_sub(self.IOTA0, 28)) & 31) as usize] * self.fConst155 + self.fVec518[((i32::wrapping_sub(self.IOTA0, 29)) & 31) as usize] * self.fConst154 + self.fVec517[((i32::wrapping_sub(self.IOTA0, 30)) & 31) as usize] * self.fConst153 + self.fVec516[((i32::wrapping_sub(self.IOTA0, 31)) & 31) as usize] * self.fConst152 + self.fVec515[((i32::wrapping_sub(self.IOTA0, 32)) & 63) as usize] * self.fConst151 + self.fVec514[((i32::wrapping_sub(self.IOTA0, 33)) & 63) as usize] * self.fConst150 + self.fVec513[((i32::wrapping_sub(self.IOTA0, 34)) & 63) as usize] * self.fConst149 + self.fVec512[((i32::wrapping_sub(self.IOTA0, 35)) & 63) as usize] * self.fConst148 + self.fVec511[((i32::wrapping_sub(self.IOTA0, 36)) & 63) as usize] * self.fConst147 + self.fVec510[((i32::wrapping_sub(self.IOTA0, 37)) & 63) as usize] * self.fConst146 + self.fVec509[((i32::wrapping_sub(self.IOTA0, 38)) & 63) as usize] * self.fConst145 + self.fVec508[((i32::wrapping_sub(self.IOTA0, 39)) & 63) as usize] * self.fConst144 + self.fVec507[((i32::wrapping_sub(self.IOTA0, 40)) & 63) as usize] * self.fConst143 + self.fVec506[((i32::wrapping_sub(self.IOTA0, 41)) & 63) as usize] * self.fConst142 + self.fVec505[((i32::wrapping_sub(self.IOTA0, 42)) & 63) as usize] * self.fConst141 + self.fVec504[((i32::wrapping_sub(self.IOTA0, 43)) & 63) as usize] * self.fConst140 + self.fVec503[((i32::wrapping_sub(self.IOTA0, 44)) & 63) as usize] * self.fConst139 + self.fVec502[((i32::wrapping_sub(self.IOTA0, 45)) & 63) as usize] * self.fConst138 + self.fVec501[((i32::wrapping_sub(self.IOTA0, 46)) & 63) as usize] * self.fConst137 + self.fVec500[((i32::wrapping_sub(self.IOTA0, 47)) & 63) as usize] * self.fConst136 + self.fVec499[((i32::wrapping_sub(self.IOTA0, 48)) & 63) as usize] * self.fConst135 + self.fVec498[((i32::wrapping_sub(self.IOTA0, 49)) & 63) as usize] * self.fConst134 + self.fVec497[((i32::wrapping_sub(self.IOTA0, 50)) & 63) as usize] * self.fConst133 + self.fVec496[((i32::wrapping_sub(self.IOTA0, 51)) & 63) as usize] * self.fConst132 + self.fVec495[((i32::wrapping_sub(self.IOTA0, 52)) & 63) as usize] * self.fConst131 + self.fVec494[((i32::wrapping_sub(self.IOTA0, 53)) & 63) as usize] * self.fConst130 + self.fVec493[((i32::wrapping_sub(self.IOTA0, 54)) & 63) as usize] * self.fConst129 + self.fVec492[((i32::wrapping_sub(self.IOTA0, 55)) & 63) as usize] * self.fConst128 + self.fVec491[((i32::wrapping_sub(self.IOTA0, 56)) & 63) as usize] * self.fConst127 + self.fVec490[((i32::wrapping_sub(self.IOTA0, 57)) & 63) as usize] * self.fConst126 + self.fVec489[((i32::wrapping_sub(self.IOTA0, 58)) & 63) as usize] * self.fConst125 + self.fVec488[((i32::wrapping_sub(self.IOTA0, 59)) & 63) as usize] * self.fConst124 + self.fVec487[((i32::wrapping_sub(self.IOTA0, 60)) & 63) as usize] * self.fConst123 + self.fVec486[((i32::wrapping_sub(self.IOTA0, 61)) & 63) as usize] * self.fConst122 + self.fVec485[((i32::wrapping_sub(self.IOTA0, 62)) & 63) as usize] * self.fConst121 + self.fVec484[((i32::wrapping_sub(self.IOTA0, 63)) & 63) as usize] * self.fConst120 + self.fVec483[((i32::wrapping_sub(self.IOTA0, 64)) & 127) as usize] * self.fConst119 + self.fVec482[((i32::wrapping_sub(self.IOTA0, 65)) & 127) as usize] * self.fConst118 + self.fVec481[((i32::wrapping_sub(self.IOTA0, 66)) & 127) as usize] * self.fConst117 + self.fVec480[((i32::wrapping_sub(self.IOTA0, 67)) & 127) as usize] * self.fConst116 + self.fVec479[((i32::wrapping_sub(self.IOTA0, 68)) & 127) as usize] * self.fConst115 + self.fVec478[((i32::wrapping_sub(self.IOTA0, 69)) & 127) as usize] * self.fConst114 + self.fVec477[((i32::wrapping_sub(self.IOTA0, 70)) & 127) as usize] * self.fConst113 + self.fVec476[((i32::wrapping_sub(self.IOTA0, 71)) & 127) as usize] * self.fConst112 + self.fVec475[((i32::wrapping_sub(self.IOTA0, 72)) & 127) as usize] * self.fConst111 + self.fVec474[((i32::wrapping_sub(self.IOTA0, 73)) & 127) as usize] * self.fConst110 + self.fVec473[((i32::wrapping_sub(self.IOTA0, 74)) & 127) as usize] * self.fConst109 + self.fVec472[((i32::wrapping_sub(self.IOTA0, 75)) & 127) as usize] * self.fConst108 + self.fVec471[((i32::wrapping_sub(self.IOTA0, 76)) & 127) as usize] * self.fConst107 + self.fVec470[((i32::wrapping_sub(self.IOTA0, 77)) & 127) as usize] * self.fConst106 + self.fVec469[((i32::wrapping_sub(self.IOTA0, 78)) & 127) as usize] * self.fConst105 + self.fVec468[((i32::wrapping_sub(self.IOTA0, 79)) & 127) as usize] * self.fConst104 + self.fVec467[((i32::wrapping_sub(self.IOTA0, 80)) & 127) as usize] * self.fConst103 + self.fVec466[((i32::wrapping_sub(self.IOTA0, 81)) & 127) as usize] * self.fConst102 + self.fVec465[((i32::wrapping_sub(self.IOTA0, 82)) & 127) as usize] * self.fConst101 + self.fVec464[((i32::wrapping_sub(self.IOTA0, 83)) & 127) as usize] * self.fConst100 + self.fVec463[((i32::wrapping_sub(self.IOTA0, 84)) & 127) as usize] * self.fConst99 + self.fVec462[((i32::wrapping_sub(self.IOTA0, 85)) & 127) as usize] * self.fConst98 + self.fVec461[((i32::wrapping_sub(self.IOTA0, 86)) & 127) as usize] * self.fConst97 + self.fVec460[((i32::wrapping_sub(self.IOTA0, 87)) & 127) as usize] * self.fConst96 + self.fVec459[((i32::wrapping_sub(self.IOTA0, 88)) & 127) as usize] * self.fConst95 + self.fVec458[((i32::wrapping_sub(self.IOTA0, 89)) & 127) as usize] * self.fConst94 + self.fVec457[((i32::wrapping_sub(self.IOTA0, 90)) & 127) as usize] * self.fConst93 + self.fVec456[((i32::wrapping_sub(self.IOTA0, 91)) & 127) as usize] * self.fConst92 + self.fVec455[((i32::wrapping_sub(self.IOTA0, 92)) & 127) as usize] * self.fConst91 + self.fVec454[((i32::wrapping_sub(self.IOTA0, 93)) & 127) as usize] * self.fConst90 + self.fVec453[((i32::wrapping_sub(self.IOTA0, 94)) & 127) as usize] * self.fConst89 + self.fVec452[((i32::wrapping_sub(self.IOTA0, 95)) & 127) as usize] * self.fConst88 + self.fVec451[((i32::wrapping_sub(self.IOTA0, 96)) & 127) as usize] * self.fConst87 + self.fVec450[((i32::wrapping_sub(self.IOTA0, 97)) & 127) as usize] * self.fConst86 + self.fVec449[((i32::wrapping_sub(self.IOTA0, 98)) & 127) as usize] * self.fConst85 + self.fVec448[((i32::wrapping_sub(self.IOTA0, 99)) & 127) as usize] * self.fConst84 + self.fVec447[((i32::wrapping_sub(self.IOTA0, 100)) & 127) as usize] * self.fConst83 + self.fVec446[((i32::wrapping_sub(self.IOTA0, 101)) & 127) as usize] * self.fConst82 + self.fVec445[((i32::wrapping_sub(self.IOTA0, 102)) & 127) as usize] * self.fConst81 + self.fVec444[((i32::wrapping_sub(self.IOTA0, 103)) & 127) as usize] * self.fConst80 + self.fVec443[((i32::wrapping_sub(self.IOTA0, 104)) & 127) as usize] * self.fConst79 + self.fVec442[((i32::wrapping_sub(self.IOTA0, 105)) & 127) as usize] * self.fConst78 + self.fVec441[((i32::wrapping_sub(self.IOTA0, 106)) & 127) as usize] * self.fConst77 + self.fVec440[((i32::wrapping_sub(self.IOTA0, 107)) & 127) as usize] * self.fConst76 + self.fVec439[((i32::wrapping_sub(self.IOTA0, 108)) & 127) as usize] * self.fConst75 + self.fVec438[((i32::wrapping_sub(self.IOTA0, 109)) & 127) as usize] * self.fConst74 + self.fVec437[((i32::wrapping_sub(self.IOTA0, 110)) & 127) as usize] * self.fConst73 + self.fVec436[((i32::wrapping_sub(self.IOTA0, 111)) & 127) as usize] * self.fConst72 + self.fVec435[((i32::wrapping_sub(self.IOTA0, 112)) & 127) as usize] * self.fConst71 + self.fVec434[((i32::wrapping_sub(self.IOTA0, 113)) & 127) as usize] * self.fConst70 + self.fVec433[((i32::wrapping_sub(self.IOTA0, 114)) & 127) as usize] * self.fConst69 + self.fVec432[((i32::wrapping_sub(self.IOTA0, 115)) & 127) as usize] * self.fConst68 + self.fVec431[((i32::wrapping_sub(self.IOTA0, 116)) & 127) as usize] * self.fConst67 + self.fVec430[((i32::wrapping_sub(self.IOTA0, 117)) & 127) as usize] * self.fConst66 + self.fVec429[((i32::wrapping_sub(self.IOTA0, 118)) & 127) as usize] * self.fConst65 + self.fVec428[((i32::wrapping_sub(self.IOTA0, 119)) & 127) as usize] * self.fConst63 + self.fVec427[((i32::wrapping_sub(self.IOTA0, 120)) & 127) as usize] * self.fConst64} else {*input552 * self.fConst62 + self.fVec426[1] * self.fConst61 + self.fVec425[2] * self.fConst60 + self.fVec424[3] * self.fConst59 + self.fVec423[4] * self.fConst58 + self.fVec422[5] * self.fConst57 + self.fVec421[6] * self.fConst56 + self.fVec420[7] * self.fConst55 + self.fVec419[8] * self.fConst54 + self.fVec418[9] * self.fConst53 + self.fVec417[10] * self.fConst52 + self.fVec416[11] * self.fConst51 + self.fVec415[12] * self.fConst50 + self.fVec414[13] * self.fConst49 + self.fVec413[14] * self.fConst48 + self.fVec412[15] * self.fConst47 + self.fVec411[((i32::wrapping_sub(self.IOTA0, 16)) & 31) as usize] * self.fConst46 + self.fVec410[((i32::wrapping_sub(self.IOTA0, 17)) & 31) as usize] * self.fConst45 + self.fVec409[((i32::wrapping_sub(self.IOTA0, 18)) & 31) as usize] * self.fConst44 + self.fVec408[((i32::wrapping_sub(self.IOTA0, 19)) & 31) as usize] * self.fConst43 + self.fVec407[((i32::wrapping_sub(self.IOTA0, 20)) & 31) as usize] * self.fConst42 + self.fVec406[((i32::wrapping_sub(self.IOTA0, 21)) & 31) as usize] * self.fConst41 + self.fVec405[((i32::wrapping_sub(self.IOTA0, 22)) & 31) as usize] * self.fConst40 + self.fVec404[((i32::wrapping_sub(self.IOTA0, 23)) & 31) as usize] * self.fConst39 + self.fVec403[((i32::wrapping_sub(self.IOTA0, 24)) & 31) as usize] * self.fConst38 + self.fVec402[((i32::wrapping_sub(self.IOTA0, 25)) & 31) as usize] * self.fConst37 + self.fVec401[((i32::wrapping_sub(self.IOTA0, 26)) & 31) as usize] * self.fConst36 + self.fVec400[((i32::wrapping_sub(self.IOTA0, 27)) & 31) as usize] * self.fConst35 + self.fVec399[((i32::wrapping_sub(self.IOTA0, 28)) & 31) as usize] * self.fConst34 + self.fVec398[((i32::wrapping_sub(self.IOTA0, 29)) & 31) as usize] * self.fConst33 + self.fVec397[((i32::wrapping_sub(self.IOTA0, 30)) & 31) as usize] * self.fConst32 + self.fVec396[((i32::wrapping_sub(self.IOTA0, 31)) & 31) as usize] * self.fConst31 + self.fVec395[((i32::wrapping_sub(self.IOTA0, 32)) & 63) as usize] * self.fConst30 + self.fVec394[((i32::wrapping_sub(self.IOTA0, 33)) & 63) as usize] * self.fConst29 + self.fVec393[((i32::wrapping_sub(self.IOTA0, 34)) & 63) as usize] * self.fConst28 + self.fVec392[((i32::wrapping_sub(self.IOTA0, 35)) & 63) as usize] * self.fConst27 + self.fVec391[((i32::wrapping_sub(self.IOTA0, 36)) & 63) as usize] * self.fConst26 + self.fVec390[((i32::wrapping_sub(self.IOTA0, 37)) & 63) as usize] * self.fConst25 + self.fVec389[((i32::wrapping_sub(self.IOTA0, 38)) & 63) as usize] * self.fConst24 + self.fVec388[((i32::wrapping_sub(self.IOTA0, 39)) & 63) as usize] * self.fConst23 + self.fVec387[((i32::wrapping_sub(self.IOTA0, 40)) & 63) as usize] * self.fConst22 + self.fVec386[((i32::wrapping_sub(self.IOTA0, 41)) & 63) as usize] * self.fConst21 + self.fVec385[((i32::wrapping_sub(self.IOTA0, 42)) & 63) as usize] * self.fConst20 + self.fVec384[((i32::wrapping_sub(self.IOTA0, 43)) & 63) as usize] * self.fConst19 + self.fVec383[((i32::wrapping_sub(self.IOTA0, 44)) & 63) as usize] * self.fConst18 + self.fVec382[((i32::wrapping_sub(self.IOTA0, 45)) & 63) as usize] * self.fConst17 + self.fVec381[((i32::wrapping_sub(self.IOTA0, 46)) & 63) as usize] * self.fConst16 + self.fVec380[((i32::wrapping_sub(self.IOTA0, 47)) & 63) as usize] * self.fConst15 + self.fVec379[((i32::wrapping_sub(self.IOTA0, 48)) & 63) as usize] * self.fConst14 + self.fVec378[((i32::wrapping_sub(self.IOTA0, 49)) & 63) as usize] * self.fConst13 + self.fVec377[((i32::wrapping_sub(self.IOTA0, 50)) & 63) as usize] * self.fConst12 + self.fVec376[((i32::wrapping_sub(self.IOTA0, 51)) & 63) as usize] * self.fConst11 + self.fVec375[((i32::wrapping_sub(self.IOTA0, 52)) & 63) as usize] * self.fConst10 + self.fVec374[((i32::wrapping_sub(self.IOTA0, 53)) & 63) as usize] * self.fConst9 + self.fVec373[((i32::wrapping_sub(self.IOTA0, 54)) & 63) as usize] * self.fConst8 + self.fVec372[((i32::wrapping_sub(self.IOTA0, 55)) & 63) as usize] * self.fConst7 + self.fVec371[((i32::wrapping_sub(self.IOTA0, 56)) & 63) as usize] * self.fConst6 + self.fVec370[((i32::wrapping_sub(self.IOTA0, 57)) & 63) as usize] * self.fConst5 + self.fVec369[((i32::wrapping_sub(self.IOTA0, 58)) & 63) as usize] * self.fConst4 + self.fVec368[((i32::wrapping_sub(self.IOTA0, 59)) & 63) as usize] * self.fConst3 + self.fVec367[((i32::wrapping_sub(self.IOTA0, 60)) & 63) as usize] * self.fConst2 + self.fVec366[((i32::wrapping_sub(self.IOTA0, 61)) & 63) as usize] * self.fConst0 + self.fVec365[((i32::wrapping_sub(self.IOTA0, 62)) & 63) as usize] * self.fConst1})));
			self.IOTA0 = i32::wrapping_add(self.IOTA0, 1);
			for j0 in (1..=15).rev() {
				self.fVec47[j0 as usize] = self.fVec47[(i32::wrapping_sub(j0, 1)) as usize];
			}
			for j1 in (1..=14).rev() {
				self.fVec48[j1 as usize] = self.fVec48[(i32::wrapping_sub(j1, 1)) as usize];
			}
			for j2 in (1..=13).rev() {
				self.fVec49[j2 as usize] = self.fVec49[(i32::wrapping_sub(j2, 1)) as usize];
			}
			for j3 in (1..=12).rev() {
				self.fVec50[j3 as usize] = self.fVec50[(i32::wrapping_sub(j3, 1)) as usize];
			}
			for j4 in (1..=11).rev() {
				self.fVec51[j4 as usize] = self.fVec51[(i32::wrapping_sub(j4, 1)) as usize];
			}
			for j5 in (1..=10).rev() {
				self.fVec52[j5 as usize] = self.fVec52[(i32::wrapping_sub(j5, 1)) as usize];
			}
			for j6 in (1..=9).rev() {
				self.fVec53[j6 as usize] = self.fVec53[(i32::wrapping_sub(j6, 1)) as usize];
			}
			for j7 in (1..=8).rev() {
				self.fVec54[j7 as usize] = self.fVec54[(i32::wrapping_sub(j7, 1)) as usize];
			}
			for j8 in (1..=7).rev() {
				self.fVec55[j8 as usize] = self.fVec55[(i32::wrapping_sub(j8, 1)) as usize];
			}
			for j9 in (1..=6).rev() {
				self.fVec56[j9 as usize] = self.fVec56[(i32::wrapping_sub(j9, 1)) as usize];
			}
			for j10 in (1..=5).rev() {
				self.fVec57[j10 as usize] = self.fVec57[(i32::wrapping_sub(j10, 1)) as usize];
			}
			for j11 in (1..=4).rev() {
				self.fVec58[j11 as usize] = self.fVec58[(i32::wrapping_sub(j11, 1)) as usize];
			}
			for j12 in (1..=3).rev() {
				self.fVec59[j12 as usize] = self.fVec59[(i32::wrapping_sub(j12, 1)) as usize];
			}
			self.fVec60[2] = self.fVec60[1];
			self.fVec60[1] = self.fVec60[0];
			self.fVec61[1] = self.fVec61[0];
			self.iVec62[1] = self.iVec62[0];
			for j13 in (1..=15).rev() {
				self.fVec168[j13 as usize] = self.fVec168[(i32::wrapping_sub(j13, 1)) as usize];
			}
			for j14 in (1..=14).rev() {
				self.fVec169[j14 as usize] = self.fVec169[(i32::wrapping_sub(j14, 1)) as usize];
			}
			for j15 in (1..=13).rev() {
				self.fVec170[j15 as usize] = self.fVec170[(i32::wrapping_sub(j15, 1)) as usize];
			}
			for j16 in (1..=12).rev() {
				self.fVec171[j16 as usize] = self.fVec171[(i32::wrapping_sub(j16, 1)) as usize];
			}
			for j17 in (1..=11).rev() {
				self.fVec172[j17 as usize] = self.fVec172[(i32::wrapping_sub(j17, 1)) as usize];
			}
			for j18 in (1..=10).rev() {
				self.fVec173[j18 as usize] = self.fVec173[(i32::wrapping_sub(j18, 1)) as usize];
			}
			for j19 in (1..=9).rev() {
				self.fVec174[j19 as usize] = self.fVec174[(i32::wrapping_sub(j19, 1)) as usize];
			}
			for j20 in (1..=8).rev() {
				self.fVec175[j20 as usize] = self.fVec175[(i32::wrapping_sub(j20, 1)) as usize];
			}
			for j21 in (1..=7).rev() {
				self.fVec176[j21 as usize] = self.fVec176[(i32::wrapping_sub(j21, 1)) as usize];
			}
			for j22 in (1..=6).rev() {
				self.fVec177[j22 as usize] = self.fVec177[(i32::wrapping_sub(j22, 1)) as usize];
			}
			for j23 in (1..=5).rev() {
				self.fVec178[j23 as usize] = self.fVec178[(i32::wrapping_sub(j23, 1)) as usize];
			}
			for j24 in (1..=4).rev() {
				self.fVec179[j24 as usize] = self.fVec179[(i32::wrapping_sub(j24, 1)) as usize];
			}
			for j25 in (1..=3).rev() {
				self.fVec180[j25 as usize] = self.fVec180[(i32::wrapping_sub(j25, 1)) as usize];
			}
			self.fVec181[2] = self.fVec181[1];
			self.fVec181[1] = self.fVec181[0];
			self.fVec182[1] = self.fVec182[0];
			self.fRec0[1] = self.fRec0[0];
			self.fRec4[1] = self.fRec4[0];
			self.fRec3[1] = self.fRec3[0];
			self.fRec2[1] = self.fRec2[0];
			self.fRec1[1] = self.fRec1[0];
			for j26 in (1..=15).rev() {
				self.fVec230[j26 as usize] = self.fVec230[(i32::wrapping_sub(j26, 1)) as usize];
			}
			for j27 in (1..=14).rev() {
				self.fVec231[j27 as usize] = self.fVec231[(i32::wrapping_sub(j27, 1)) as usize];
			}
			for j28 in (1..=13).rev() {
				self.fVec232[j28 as usize] = self.fVec232[(i32::wrapping_sub(j28, 1)) as usize];
			}
			for j29 in (1..=12).rev() {
				self.fVec233[j29 as usize] = self.fVec233[(i32::wrapping_sub(j29, 1)) as usize];
			}
			for j30 in (1..=11).rev() {
				self.fVec234[j30 as usize] = self.fVec234[(i32::wrapping_sub(j30, 1)) as usize];
			}
			for j31 in (1..=10).rev() {
				self.fVec235[j31 as usize] = self.fVec235[(i32::wrapping_sub(j31, 1)) as usize];
			}
			for j32 in (1..=9).rev() {
				self.fVec236[j32 as usize] = self.fVec236[(i32::wrapping_sub(j32, 1)) as usize];
			}
			for j33 in (1..=8).rev() {
				self.fVec237[j33 as usize] = self.fVec237[(i32::wrapping_sub(j33, 1)) as usize];
			}
			for j34 in (1..=7).rev() {
				self.fVec238[j34 as usize] = self.fVec238[(i32::wrapping_sub(j34, 1)) as usize];
			}
			for j35 in (1..=6).rev() {
				self.fVec239[j35 as usize] = self.fVec239[(i32::wrapping_sub(j35, 1)) as usize];
			}
			for j36 in (1..=5).rev() {
				self.fVec240[j36 as usize] = self.fVec240[(i32::wrapping_sub(j36, 1)) as usize];
			}
			for j37 in (1..=4).rev() {
				self.fVec241[j37 as usize] = self.fVec241[(i32::wrapping_sub(j37, 1)) as usize];
			}
			for j38 in (1..=3).rev() {
				self.fVec242[j38 as usize] = self.fVec242[(i32::wrapping_sub(j38, 1)) as usize];
			}
			self.fVec243[2] = self.fVec243[1];
			self.fVec243[1] = self.fVec243[0];
			self.fVec244[1] = self.fVec244[0];
			for j39 in (1..=15).rev() {
				self.fVec350[j39 as usize] = self.fVec350[(i32::wrapping_sub(j39, 1)) as usize];
			}
			for j40 in (1..=14).rev() {
				self.fVec351[j40 as usize] = self.fVec351[(i32::wrapping_sub(j40, 1)) as usize];
			}
			for j41 in (1..=13).rev() {
				self.fVec352[j41 as usize] = self.fVec352[(i32::wrapping_sub(j41, 1)) as usize];
			}
			for j42 in (1..=12).rev() {
				self.fVec353[j42 as usize] = self.fVec353[(i32::wrapping_sub(j42, 1)) as usize];
			}
			for j43 in (1..=11).rev() {
				self.fVec354[j43 as usize] = self.fVec354[(i32::wrapping_sub(j43, 1)) as usize];
			}
			for j44 in (1..=10).rev() {
				self.fVec355[j44 as usize] = self.fVec355[(i32::wrapping_sub(j44, 1)) as usize];
			}
			for j45 in (1..=9).rev() {
				self.fVec356[j45 as usize] = self.fVec356[(i32::wrapping_sub(j45, 1)) as usize];
			}
			for j46 in (1..=8).rev() {
				self.fVec357[j46 as usize] = self.fVec357[(i32::wrapping_sub(j46, 1)) as usize];
			}
			for j47 in (1..=7).rev() {
				self.fVec358[j47 as usize] = self.fVec358[(i32::wrapping_sub(j47, 1)) as usize];
			}
			for j48 in (1..=6).rev() {
				self.fVec359[j48 as usize] = self.fVec359[(i32::wrapping_sub(j48, 1)) as usize];
			}
			for j49 in (1..=5).rev() {
				self.fVec360[j49 as usize] = self.fVec360[(i32::wrapping_sub(j49, 1)) as usize];
			}
			for j50 in (1..=4).rev() {
				self.fVec361[j50 as usize] = self.fVec361[(i32::wrapping_sub(j50, 1)) as usize];
			}
			for j51 in (1..=3).rev() {
				self.fVec362[j51 as usize] = self.fVec362[(i32::wrapping_sub(j51, 1)) as usize];
			}
			self.fVec363[2] = self.fVec363[1];
			self.fVec363[1] = self.fVec363[0];
			self.fVec364[1] = self.fVec364[0];
			self.fRec5[1] = self.fRec5[0];
			self.fRec6[1] = self.fRec6[0];
			for j52 in (1..=15).rev() {
				self.fVec412[j52 as usize] = self.fVec412[(i32::wrapping_sub(j52, 1)) as usize];
			}
			for j53 in (1..=14).rev() {
				self.fVec413[j53 as usize] = self.fVec413[(i32::wrapping_sub(j53, 1)) as usize];
			}
			for j54 in (1..=13).rev() {
				self.fVec414[j54 as usize] = self.fVec414[(i32::wrapping_sub(j54, 1)) as usize];
			}
			for j55 in (1..=12).rev() {
				self.fVec415[j55 as usize] = self.fVec415[(i32::wrapping_sub(j55, 1)) as usize];
			}
			for j56 in (1..=11).rev() {
				self.fVec416[j56 as usize] = self.fVec416[(i32::wrapping_sub(j56, 1)) as usize];
			}
			for j57 in (1..=10).rev() {
				self.fVec417[j57 as usize] = self.fVec417[(i32::wrapping_sub(j57, 1)) as usize];
			}
			for j58 in (1..=9).rev() {
				self.fVec418[j58 as usize] = self.fVec418[(i32::wrapping_sub(j58, 1)) as usize];
			}
			for j59 in (1..=8).rev() {
				self.fVec419[j59 as usize] = self.fVec419[(i32::wrapping_sub(j59, 1)) as usize];
			}
			for j60 in (1..=7).rev() {
				self.fVec420[j60 as usize] = self.fVec420[(i32::wrapping_sub(j60, 1)) as usize];
			}
			for j61 in (1..=6).rev() {
				self.fVec421[j61 as usize] = self.fVec421[(i32::wrapping_sub(j61, 1)) as usize];
			}
			for j62 in (1..=5).rev() {
				self.fVec422[j62 as usize] = self.fVec422[(i32::wrapping_sub(j62, 1)) as usize];
			}
			for j63 in (1..=4).rev() {
				self.fVec423[j63 as usize] = self.fVec423[(i32::wrapping_sub(j63, 1)) as usize];
			}
			for j64 in (1..=3).rev() {
				self.fVec424[j64 as usize] = self.fVec424[(i32::wrapping_sub(j64, 1)) as usize];
			}
			self.fVec425[2] = self.fVec425[1];
			self.fVec425[1] = self.fVec425[0];
			self.fVec426[1] = self.fVec426[0];
			for j65 in (1..=15).rev() {
				self.fVec532[j65 as usize] = self.fVec532[(i32::wrapping_sub(j65, 1)) as usize];
			}
			for j66 in (1..=14).rev() {
				self.fVec533[j66 as usize] = self.fVec533[(i32::wrapping_sub(j66, 1)) as usize];
			}
			for j67 in (1..=13).rev() {
				self.fVec534[j67 as usize] = self.fVec534[(i32::wrapping_sub(j67, 1)) as usize];
			}
			for j68 in (1..=12).rev() {
				self.fVec535[j68 as usize] = self.fVec535[(i32::wrapping_sub(j68, 1)) as usize];
			}
			for j69 in (1..=11).rev() {
				self.fVec536[j69 as usize] = self.fVec536[(i32::wrapping_sub(j69, 1)) as usize];
			}
			for j70 in (1..=10).rev() {
				self.fVec537[j70 as usize] = self.fVec537[(i32::wrapping_sub(j70, 1)) as usize];
			}
			for j71 in (1..=9).rev() {
				self.fVec538[j71 as usize] = self.fVec538[(i32::wrapping_sub(j71, 1)) as usize];
			}
			for j72 in (1..=8).rev() {
				self.fVec539[j72 as usize] = self.fVec539[(i32::wrapping_sub(j72, 1)) as usize];
			}
			for j73 in (1..=7).rev() {
				self.fVec540[j73 as usize] = self.fVec540[(i32::wrapping_sub(j73, 1)) as usize];
			}
			for j74 in (1..=6).rev() {
				self.fVec541[j74 as usize] = self.fVec541[(i32::wrapping_sub(j74, 1)) as usize];
			}
			for j75 in (1..=5).rev() {
				self.fVec542[j75 as usize] = self.fVec542[(i32::wrapping_sub(j75, 1)) as usize];
			}
			for j76 in (1..=4).rev() {
				self.fVec543[j76 as usize] = self.fVec543[(i32::wrapping_sub(j76, 1)) as usize];
			}
			for j77 in (1..=3).rev() {
				self.fVec544[j77 as usize] = self.fVec544[(i32::wrapping_sub(j77, 1)) as usize];
			}
			self.fVec545[2] = self.fVec545[1];
			self.fVec545[1] = self.fVec545[0];
			self.fVec546[1] = self.fVec546[0];
			for j78 in (1..=15).rev() {
				self.fVec594[j78 as usize] = self.fVec594[(i32::wrapping_sub(j78, 1)) as usize];
			}
			for j79 in (1..=14).rev() {
				self.fVec595[j79 as usize] = self.fVec595[(i32::wrapping_sub(j79, 1)) as usize];
			}
			for j80 in (1..=13).rev() {
				self.fVec596[j80 as usize] = self.fVec596[(i32::wrapping_sub(j80, 1)) as usize];
			}
			for j81 in (1..=12).rev() {
				self.fVec597[j81 as usize] = self.fVec597[(i32::wrapping_sub(j81, 1)) as usize];
			}
			for j82 in (1..=11).rev() {
				self.fVec598[j82 as usize] = self.fVec598[(i32::wrapping_sub(j82, 1)) as usize];
			}
			for j83 in (1..=10).rev() {
				self.fVec599[j83 as usize] = self.fVec599[(i32::wrapping_sub(j83, 1)) as usize];
			}
			for j84 in (1..=9).rev() {
				self.fVec600[j84 as usize] = self.fVec600[(i32::wrapping_sub(j84, 1)) as usize];
			}
			for j85 in (1..=8).rev() {
				self.fVec601[j85 as usize] = self.fVec601[(i32::wrapping_sub(j85, 1)) as usize];
			}
			for j86 in (1..=7).rev() {
				self.fVec602[j86 as usize] = self.fVec602[(i32::wrapping_sub(j86, 1)) as usize];
			}
			for j87 in (1..=6).rev() {
				self.fVec603[j87 as usize] = self.fVec603[(i32::wrapping_sub(j87, 1)) as usize];
			}
			for j88 in (1..=5).rev() {
				self.fVec604[j88 as usize] = self.fVec604[(i32::wrapping_sub(j88, 1)) as usize];
			}
			for j89 in (1..=4).rev() {
				self.fVec605[j89 as usize] = self.fVec605[(i32::wrapping_sub(j89, 1)) as usize];
			}
			for j90 in (1..=3).rev() {
				self.fVec606[j90 as usize] = self.fVec606[(i32::wrapping_sub(j90, 1)) as usize];
			}
			self.fVec607[2] = self.fVec607[1];
			self.fVec607[1] = self.fVec607[0];
			self.fVec608[1] = self.fVec608[0];
			for j91 in (1..=15).rev() {
				self.fVec714[j91 as usize] = self.fVec714[(i32::wrapping_sub(j91, 1)) as usize];
			}
			for j92 in (1..=14).rev() {
				self.fVec715[j92 as usize] = self.fVec715[(i32::wrapping_sub(j92, 1)) as usize];
			}
			for j93 in (1..=13).rev() {
				self.fVec716[j93 as usize] = self.fVec716[(i32::wrapping_sub(j93, 1)) as usize];
			}
			for j94 in (1..=12).rev() {
				self.fVec717[j94 as usize] = self.fVec717[(i32::wrapping_sub(j94, 1)) as usize];
			}
			for j95 in (1..=11).rev() {
				self.fVec718[j95 as usize] = self.fVec718[(i32::wrapping_sub(j95, 1)) as usize];
			}
			for j96 in (1..=10).rev() {
				self.fVec719[j96 as usize] = self.fVec719[(i32::wrapping_sub(j96, 1)) as usize];
			}
			for j97 in (1..=9).rev() {
				self.fVec720[j97 as usize] = self.fVec720[(i32::wrapping_sub(j97, 1)) as usize];
			}
			for j98 in (1..=8).rev() {
				self.fVec721[j98 as usize] = self.fVec721[(i32::wrapping_sub(j98, 1)) as usize];
			}
			for j99 in (1..=7).rev() {
				self.fVec722[j99 as usize] = self.fVec722[(i32::wrapping_sub(j99, 1)) as usize];
			}
			for j100 in (1..=6).rev() {
				self.fVec723[j100 as usize] = self.fVec723[(i32::wrapping_sub(j100, 1)) as usize];
			}
			for j101 in (1..=5).rev() {
				self.fVec724[j101 as usize] = self.fVec724[(i32::wrapping_sub(j101, 1)) as usize];
			}
			for j102 in (1..=4).rev() {
				self.fVec725[j102 as usize] = self.fVec725[(i32::wrapping_sub(j102, 1)) as usize];
			}
			for j103 in (1..=3).rev() {
				self.fVec726[j103 as usize] = self.fVec726[(i32::wrapping_sub(j103, 1)) as usize];
			}
			self.fVec727[2] = self.fVec727[1];
			self.fVec727[1] = self.fVec727[0];
			self.fVec728[1] = self.fVec728[0];
		}
		
	}

}

impl FaustDsp for mydsp {
	type T = FaustFloat;
	fn new() -> Self where Self: Sized {
		Self::new()
	}
	fn metadata(&self, m: &mut dyn Meta) {
		self.metadata(m)
	}
	fn get_sample_rate(&self) -> i32 {
		self.get_sample_rate()
	}
	fn get_num_inputs(&self) -> i32 {
		FAUST_INPUTS as i32
	}
	fn get_num_outputs(&self) -> i32 {
		FAUST_OUTPUTS as i32
	}
	fn class_init(sample_rate: i32) where Self: Sized {
		Self::class_init(sample_rate);
	}
	fn instance_reset_params(&mut self) {
		self.instance_reset_params()
	}
	fn instance_clear(&mut self) {
		self.instance_clear()
	}
	fn instance_constants(&mut self, sample_rate: i32) {
		self.instance_constants(sample_rate)
	}
	fn instance_init(&mut self, sample_rate: i32) {
		self.instance_init(sample_rate)
	}
	fn init(&mut self, sample_rate: i32) {
		self.init(sample_rate)
	}
	fn build_user_interface(&self, ui_interface: &mut dyn UI<Self::T>) {
		self.build_user_interface(ui_interface)
	}
	fn build_user_interface_static(ui_interface: &mut dyn UI<Self::T>) where Self: Sized {
		Self::build_user_interface_static(ui_interface);
	}
	fn get_param(&self, param: ParamIndex) -> Option<Self::T> {
		self.get_param(param)
	}
	fn set_param(&mut self, param: ParamIndex, value: Self::T) {
		self.set_param(param, value)
	}
	fn compute(&mut self, count: i32, inputs: &[&[Self::T]], outputs: &mut [&mut [Self::T]]) {
		self.compute(count as usize, inputs, outputs)
	}
}
