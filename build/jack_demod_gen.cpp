/* ------------------------------------------------------------
name: "jack_demod"
Code generated with Faust 2.83.1 (https://faust.grame.fr)
Compilation options: -lang rust -fpga-mem-th 4 -ct 1 -es 1 -mcd 16 -mdd 1024 -mdy 33 -single -ftz 0
------------------------------------------------------------ */
#[cfg_attr(feature = "default-boxed", derive(default_boxed::DefaultBoxed))]
#[repr(C)]
pub struct mydsp {
	fHslider0: F32,
	iRec0: [i32;2],
	iRec1: [i32;2],
	IOTA0: i32,
	fRec2: [F32;128],
	fConst0: F32,
	fConst1: F32,
	fConst2: F32,
	fConst3: F32,
	fConst4: F32,
	fConst5: F32,
	fConst6: F32,
	fConst7: F32,
	fConst8: F32,
	fConst9: F32,
	fConst10: F32,
	fConst11: F32,
	fConst12: F32,
	fConst13: F32,
	fConst14: F32,
	fConst15: F32,
	fConst16: F32,
	fConst17: F32,
	fConst18: F32,
	fConst19: F32,
	fConst20: F32,
	fConst21: F32,
	fConst22: F32,
	fConst23: F32,
	fConst24: F32,
	fConst25: F32,
	fConst26: F32,
	fConst27: F32,
	fConst28: F32,
	fConst29: F32,
	fConst30: F32,
	fConst31: F32,
	fConst32: F32,
	fConst33: F32,
	fConst34: F32,
	fConst35: F32,
	fConst36: F32,
	fConst37: F32,
	fConst38: F32,
	fConst39: F32,
	fConst40: F32,
	fConst41: F32,
	fConst42: F32,
	fConst43: F32,
	fConst44: F32,
	fConst45: F32,
	fConst46: F32,
	fConst47: F32,
	fConst48: F32,
	fConst49: F32,
	fConst50: F32,
	fConst51: F32,
	fConst52: F32,
	fConst53: F32,
	fConst54: F32,
	fConst55: F32,
	fConst56: F32,
	fConst57: F32,
	fConst58: F32,
	fConst59: F32,
	fConst60: F32,
	fConst61: F32,
	fConst62: F32,
	fConst63: F32,
	fConst64: F32,
	fConst65: F32,
	fConst66: F32,
	fConst67: F32,
	fConst68: F32,
	fConst69: F32,
	fConst70: F32,
	fConst71: F32,
	fConst72: F32,
	fConst73: F32,
	fConst74: F32,
	fConst75: F32,
	fConst76: F32,
	fConst77: F32,
	fConst78: F32,
	fConst79: F32,
	fConst80: F32,
	fConst81: F32,
	fConst82: F32,
	fConst83: F32,
	fConst84: F32,
	fConst85: F32,
	fConst86: F32,
	fConst87: F32,
	fConst88: F32,
	fConst89: F32,
	fConst90: F32,
	fConst91: F32,
	fConst92: F32,
	fConst93: F32,
	fConst94: F32,
	fConst95: F32,
	fConst96: F32,
	fConst97: F32,
	fConst98: F32,
	fConst99: F32,
	fConst100: F32,
	fConst101: F32,
	fConst102: F32,
	fConst103: F32,
	fConst104: F32,
	fConst105: F32,
	fConst106: F32,
	fConst107: F32,
	fConst108: F32,
	fConst109: F32,
	fConst110: F32,
	fConst111: F32,
	fConst112: F32,
	fConst113: F32,
	fConst114: F32,
	fConst115: F32,
	fConst116: F32,
	fConst117: F32,
	fConst118: F32,
	fConst119: F32,
	fConst120: F32,
	fConst121: F32,
	fConst122: F32,
	fConst123: F32,
	fConst124: F32,
	fConst125: F32,
	fConst126: F32,
	fConst127: F32,
	fConst128: F32,
	fConst129: F32,
	fConst130: F32,
	fConst131: F32,
	fConst132: F32,
	fConst133: F32,
	fConst134: F32,
	fConst135: F32,
	fConst136: F32,
	fConst137: F32,
	fConst138: F32,
	fConst139: F32,
	fConst140: F32,
	fConst141: F32,
	fConst142: F32,
	fConst143: F32,
	fConst144: F32,
	fConst145: F32,
	fConst146: F32,
	fConst147: F32,
	fConst148: F32,
	fConst149: F32,
	fConst150: F32,
	fConst151: F32,
	fConst152: F32,
	fConst153: F32,
	fConst154: F32,
	fConst155: F32,
	fConst156: F32,
	fConst157: F32,
	fConst158: F32,
	fConst159: F32,
	fConst160: F32,
	fConst161: F32,
	fConst162: F32,
	fConst163: F32,
	fConst164: F32,
	fConst165: F32,
	fConst166: F32,
	fConst167: F32,
	fConst168: F32,
	fConst169: F32,
	fConst170: F32,
	fConst171: F32,
	fConst172: F32,
	fConst173: F32,
	fConst174: F32,
	fConst175: F32,
	fConst176: F32,
	fConst177: F32,
	fConst178: F32,
	fConst179: F32,
	fConst180: F32,
	fConst181: F32,
	fConst182: F32,
	fConst183: F32,
	fHslider1: F32,
	fSampleRate: i32,
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

pub const FAUST_INPUTS: usize = 2;
pub const FAUST_OUTPUTS: usize = 1;
pub const FAUST_ACTIVES: usize = 2;
pub const FAUST_PASSIVES: usize = 0;


impl mydsp {
		
	pub fn new() -> mydsp { 
		mydsp {
			fHslider0: 0.0,
			iRec0: [0;2],
			iRec1: [0;2],
			IOTA0: 0,
			fRec2: [0.0;128],
			fConst0: 0.0,
			fConst1: 0.0,
			fConst2: 0.0,
			fConst3: 0.0,
			fConst4: 0.0,
			fConst5: 0.0,
			fConst6: 0.0,
			fConst7: 0.0,
			fConst8: 0.0,
			fConst9: 0.0,
			fConst10: 0.0,
			fConst11: 0.0,
			fConst12: 0.0,
			fConst13: 0.0,
			fConst14: 0.0,
			fConst15: 0.0,
			fConst16: 0.0,
			fConst17: 0.0,
			fConst18: 0.0,
			fConst19: 0.0,
			fConst20: 0.0,
			fConst21: 0.0,
			fConst22: 0.0,
			fConst23: 0.0,
			fConst24: 0.0,
			fConst25: 0.0,
			fConst26: 0.0,
			fConst27: 0.0,
			fConst28: 0.0,
			fConst29: 0.0,
			fConst30: 0.0,
			fConst31: 0.0,
			fConst32: 0.0,
			fConst33: 0.0,
			fConst34: 0.0,
			fConst35: 0.0,
			fConst36: 0.0,
			fConst37: 0.0,
			fConst38: 0.0,
			fConst39: 0.0,
			fConst40: 0.0,
			fConst41: 0.0,
			fConst42: 0.0,
			fConst43: 0.0,
			fConst44: 0.0,
			fConst45: 0.0,
			fConst46: 0.0,
			fConst47: 0.0,
			fConst48: 0.0,
			fConst49: 0.0,
			fConst50: 0.0,
			fConst51: 0.0,
			fConst52: 0.0,
			fConst53: 0.0,
			fConst54: 0.0,
			fConst55: 0.0,
			fConst56: 0.0,
			fConst57: 0.0,
			fConst58: 0.0,
			fConst59: 0.0,
			fConst60: 0.0,
			fConst61: 0.0,
			fConst62: 0.0,
			fConst63: 0.0,
			fConst64: 0.0,
			fConst65: 0.0,
			fConst66: 0.0,
			fConst67: 0.0,
			fConst68: 0.0,
			fConst69: 0.0,
			fConst70: 0.0,
			fConst71: 0.0,
			fConst72: 0.0,
			fConst73: 0.0,
			fConst74: 0.0,
			fConst75: 0.0,
			fConst76: 0.0,
			fConst77: 0.0,
			fConst78: 0.0,
			fConst79: 0.0,
			fConst80: 0.0,
			fConst81: 0.0,
			fConst82: 0.0,
			fConst83: 0.0,
			fConst84: 0.0,
			fConst85: 0.0,
			fConst86: 0.0,
			fConst87: 0.0,
			fConst88: 0.0,
			fConst89: 0.0,
			fConst90: 0.0,
			fConst91: 0.0,
			fConst92: 0.0,
			fConst93: 0.0,
			fConst94: 0.0,
			fConst95: 0.0,
			fConst96: 0.0,
			fConst97: 0.0,
			fConst98: 0.0,
			fConst99: 0.0,
			fConst100: 0.0,
			fConst101: 0.0,
			fConst102: 0.0,
			fConst103: 0.0,
			fConst104: 0.0,
			fConst105: 0.0,
			fConst106: 0.0,
			fConst107: 0.0,
			fConst108: 0.0,
			fConst109: 0.0,
			fConst110: 0.0,
			fConst111: 0.0,
			fConst112: 0.0,
			fConst113: 0.0,
			fConst114: 0.0,
			fConst115: 0.0,
			fConst116: 0.0,
			fConst117: 0.0,
			fConst118: 0.0,
			fConst119: 0.0,
			fConst120: 0.0,
			fConst121: 0.0,
			fConst122: 0.0,
			fConst123: 0.0,
			fConst124: 0.0,
			fConst125: 0.0,
			fConst126: 0.0,
			fConst127: 0.0,
			fConst128: 0.0,
			fConst129: 0.0,
			fConst130: 0.0,
			fConst131: 0.0,
			fConst132: 0.0,
			fConst133: 0.0,
			fConst134: 0.0,
			fConst135: 0.0,
			fConst136: 0.0,
			fConst137: 0.0,
			fConst138: 0.0,
			fConst139: 0.0,
			fConst140: 0.0,
			fConst141: 0.0,
			fConst142: 0.0,
			fConst143: 0.0,
			fConst144: 0.0,
			fConst145: 0.0,
			fConst146: 0.0,
			fConst147: 0.0,
			fConst148: 0.0,
			fConst149: 0.0,
			fConst150: 0.0,
			fConst151: 0.0,
			fConst152: 0.0,
			fConst153: 0.0,
			fConst154: 0.0,
			fConst155: 0.0,
			fConst156: 0.0,
			fConst157: 0.0,
			fConst158: 0.0,
			fConst159: 0.0,
			fConst160: 0.0,
			fConst161: 0.0,
			fConst162: 0.0,
			fConst163: 0.0,
			fConst164: 0.0,
			fConst165: 0.0,
			fConst166: 0.0,
			fConst167: 0.0,
			fConst168: 0.0,
			fConst169: 0.0,
			fConst170: 0.0,
			fConst171: 0.0,
			fConst172: 0.0,
			fConst173: 0.0,
			fConst174: 0.0,
			fConst175: 0.0,
			fConst176: 0.0,
			fConst177: 0.0,
			fConst178: 0.0,
			fConst179: 0.0,
			fConst180: 0.0,
			fConst181: 0.0,
			fConst182: 0.0,
			fConst183: 0.0,
			fHslider1: 0.0,
			fSampleRate: 0,
		}
	}
	pub fn metadata(&self, m: &mut dyn Meta) { 
		m.declare("compile_options", r"-lang rust -fpga-mem-th 4 -ct 1 -es 1 -mcd 16 -mdd 1024 -mdy 33 -single -ftz 0");
		m.declare("filename", r"jack_demod.dsp");
		m.declare("filters.lib/lowpass0_highpass1", r"Copyright (C) 2003-2019 by Julius O. Smith III <jos@ccrma.stanford.edu>");
		m.declare("filters.lib/name", r"Faust Filters Library");
		m.declare("filters.lib/version", r"1.7.1");
		m.declare("maths.lib/author", r"GRAME");
		m.declare("maths.lib/copyright", r"GRAME");
		m.declare("maths.lib/license", r"LGPL with exception");
		m.declare("maths.lib/name", r"Faust Math Library");
		m.declare("maths.lib/version", r"2.9.0");
		m.declare("name", r"jack_demod");
		m.declare("oscillators.lib/name", r"Faust Oscillator Library");
		m.declare("oscillators.lib/version", r"1.6.0");
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
		self.fHslider1 = 1.0;
	}
	pub fn instance_clear(&mut self) {
		for l0 in 0..2 {
			self.iRec0[l0 as usize] = 0;
		}
		for l1 in 0..2 {
			self.iRec1[l1 as usize] = 0;
		}
		self.IOTA0 = 0;
		for l2 in 0..128 {
			self.fRec2[l2 as usize] = 0.0;
		}
	}
	pub fn instance_constants(&mut self, sample_rate: i32) {
		// Obtaining locks on 2 static var(s)
		let ftbl0mydspSIG0_guard = ftbl0mydspSIG0.read().unwrap();
		let ftbl1mydspSIG1_guard = ftbl1mydspSIG1.read().unwrap();
		self.fSampleRate = sample_rate;
		self.fConst0 = ftbl0mydspSIG0_guard[62];
		self.fConst1 = ftbl0mydspSIG0_guard[61];
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
		self.fConst63 = ftbl1mydspSIG1_guard[120];
		self.fConst64 = ftbl1mydspSIG1_guard[119];
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
		ui_interface.open_vertical_box("jack_demod");
		ui_interface.add_horizontal_slider("jack_demod/output_gain", ParamIndex(0), 1.0, 0.0, 4.0, 0.01);
		ui_interface.add_horizontal_slider("jack_demod/rrc_mode", ParamIndex(1), 0.0, 0.0, 1.0, 1.0);
		ui_interface.close_box();
	}
	
	pub fn get_param(&self, param: ParamIndex) -> Option<FaustFloat> {
		match param.0 {
			1 => Some(self.fHslider0),
			0 => Some(self.fHslider1),
			_ => None,
		}
	}
	
	pub fn set_param(&mut self, param: ParamIndex, value: FaustFloat) {
		match param.0 {
			1 => { self.fHslider0 = value }
			0 => { self.fHslider1 = value }
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
		let [inputs0, inputs1, .. ] = inputs.as_ref() else { panic!("wrong number of input buffers"); };
		let inputs0 = inputs0.as_ref()[..count].iter();
		let inputs1 = inputs1.as_ref()[..count].iter();
		let [outputs0, .. ] = outputs.as_mut() else { panic!("wrong number of output buffers"); };
		let outputs0 = outputs0.as_mut()[..count].iter_mut();
		let mut iSlow0: i32 = (self.fHslider0) as i32;
		let mut fSlow1: F32 = self.fHslider1;
		let zipped_iterators = inputs0.zip(inputs1).zip(outputs0);
		for ((input0, input1), output0) in zipped_iterators {
			self.iRec0[0] = self.iRec0[1];
			self.iRec1[0] = self.iRec1[1];
			let mut fTemp0: F32 = (self.iRec1[1]) as F32;
			let mut fTemp1: F32 = *input0;
			let mut fTemp2: F32 = F32::sin(fTemp1);
			let mut fTemp3: F32 = (self.iRec0[1]) as F32;
			let mut fTemp4: F32 = F32::cos(fTemp1);
			let mut fTemp5: F32 = fTemp4 * fTemp3 + fTemp2 * fTemp0;
			self.fRec2[(self.IOTA0 & 127) as usize] = fTemp5;
			let mut fTemp6: F32 = self.fRec2[((i32::wrapping_sub(self.IOTA0, 62)) & 127) as usize];
			let mut fTemp7: F32 = self.fRec2[((i32::wrapping_sub(self.IOTA0, 61)) & 127) as usize];
			let mut fTemp8: F32 = self.fRec2[((i32::wrapping_sub(self.IOTA0, 60)) & 127) as usize];
			let mut fTemp9: F32 = self.fRec2[((i32::wrapping_sub(self.IOTA0, 59)) & 127) as usize];
			let mut fTemp10: F32 = self.fRec2[((i32::wrapping_sub(self.IOTA0, 58)) & 127) as usize];
			let mut fTemp11: F32 = self.fRec2[((i32::wrapping_sub(self.IOTA0, 57)) & 127) as usize];
			let mut fTemp12: F32 = self.fRec2[((i32::wrapping_sub(self.IOTA0, 56)) & 127) as usize];
			let mut fTemp13: F32 = self.fRec2[((i32::wrapping_sub(self.IOTA0, 55)) & 127) as usize];
			let mut fTemp14: F32 = self.fRec2[((i32::wrapping_sub(self.IOTA0, 54)) & 127) as usize];
			let mut fTemp15: F32 = self.fRec2[((i32::wrapping_sub(self.IOTA0, 53)) & 127) as usize];
			let mut fTemp16: F32 = self.fRec2[((i32::wrapping_sub(self.IOTA0, 52)) & 127) as usize];
			let mut fTemp17: F32 = self.fRec2[((i32::wrapping_sub(self.IOTA0, 51)) & 127) as usize];
			let mut fTemp18: F32 = self.fRec2[((i32::wrapping_sub(self.IOTA0, 50)) & 127) as usize];
			let mut fTemp19: F32 = self.fRec2[((i32::wrapping_sub(self.IOTA0, 49)) & 127) as usize];
			let mut fTemp20: F32 = self.fRec2[((i32::wrapping_sub(self.IOTA0, 48)) & 127) as usize];
			let mut fTemp21: F32 = self.fRec2[((i32::wrapping_sub(self.IOTA0, 47)) & 127) as usize];
			let mut fTemp22: F32 = self.fRec2[((i32::wrapping_sub(self.IOTA0, 46)) & 127) as usize];
			let mut fTemp23: F32 = self.fRec2[((i32::wrapping_sub(self.IOTA0, 45)) & 127) as usize];
			let mut fTemp24: F32 = self.fRec2[((i32::wrapping_sub(self.IOTA0, 44)) & 127) as usize];
			let mut fTemp25: F32 = self.fRec2[((i32::wrapping_sub(self.IOTA0, 43)) & 127) as usize];
			let mut fTemp26: F32 = self.fRec2[((i32::wrapping_sub(self.IOTA0, 42)) & 127) as usize];
			let mut fTemp27: F32 = self.fRec2[((i32::wrapping_sub(self.IOTA0, 41)) & 127) as usize];
			let mut fTemp28: F32 = self.fRec2[((i32::wrapping_sub(self.IOTA0, 40)) & 127) as usize];
			let mut fTemp29: F32 = self.fRec2[((i32::wrapping_sub(self.IOTA0, 39)) & 127) as usize];
			let mut fTemp30: F32 = self.fRec2[((i32::wrapping_sub(self.IOTA0, 38)) & 127) as usize];
			let mut fTemp31: F32 = self.fRec2[((i32::wrapping_sub(self.IOTA0, 37)) & 127) as usize];
			let mut fTemp32: F32 = self.fRec2[((i32::wrapping_sub(self.IOTA0, 36)) & 127) as usize];
			let mut fTemp33: F32 = self.fRec2[((i32::wrapping_sub(self.IOTA0, 35)) & 127) as usize];
			let mut fTemp34: F32 = self.fRec2[((i32::wrapping_sub(self.IOTA0, 34)) & 127) as usize];
			let mut fTemp35: F32 = self.fRec2[((i32::wrapping_sub(self.IOTA0, 33)) & 127) as usize];
			let mut fTemp36: F32 = self.fRec2[((i32::wrapping_sub(self.IOTA0, 32)) & 127) as usize];
			let mut fTemp37: F32 = self.fRec2[((i32::wrapping_sub(self.IOTA0, 31)) & 127) as usize];
			let mut fTemp38: F32 = self.fRec2[((i32::wrapping_sub(self.IOTA0, 30)) & 127) as usize];
			let mut fTemp39: F32 = self.fRec2[((i32::wrapping_sub(self.IOTA0, 29)) & 127) as usize];
			let mut fTemp40: F32 = self.fRec2[((i32::wrapping_sub(self.IOTA0, 28)) & 127) as usize];
			let mut fTemp41: F32 = self.fRec2[((i32::wrapping_sub(self.IOTA0, 27)) & 127) as usize];
			let mut fTemp42: F32 = self.fRec2[((i32::wrapping_sub(self.IOTA0, 26)) & 127) as usize];
			let mut fTemp43: F32 = self.fRec2[((i32::wrapping_sub(self.IOTA0, 25)) & 127) as usize];
			let mut fTemp44: F32 = self.fRec2[((i32::wrapping_sub(self.IOTA0, 24)) & 127) as usize];
			let mut fTemp45: F32 = self.fRec2[((i32::wrapping_sub(self.IOTA0, 23)) & 127) as usize];
			let mut fTemp46: F32 = self.fRec2[((i32::wrapping_sub(self.IOTA0, 22)) & 127) as usize];
			let mut fTemp47: F32 = self.fRec2[((i32::wrapping_sub(self.IOTA0, 21)) & 127) as usize];
			let mut fTemp48: F32 = self.fRec2[((i32::wrapping_sub(self.IOTA0, 20)) & 127) as usize];
			let mut fTemp49: F32 = self.fRec2[((i32::wrapping_sub(self.IOTA0, 19)) & 127) as usize];
			let mut fTemp50: F32 = self.fRec2[((i32::wrapping_sub(self.IOTA0, 18)) & 127) as usize];
			let mut fTemp51: F32 = self.fRec2[((i32::wrapping_sub(self.IOTA0, 17)) & 127) as usize];
			let mut fTemp52: F32 = self.fRec2[((i32::wrapping_sub(self.IOTA0, 16)) & 127) as usize];
			let mut fTemp53: F32 = self.fRec2[((i32::wrapping_sub(self.IOTA0, 15)) & 127) as usize];
			let mut fTemp54: F32 = self.fRec2[((i32::wrapping_sub(self.IOTA0, 14)) & 127) as usize];
			let mut fTemp55: F32 = self.fRec2[((i32::wrapping_sub(self.IOTA0, 13)) & 127) as usize];
			let mut fTemp56: F32 = self.fRec2[((i32::wrapping_sub(self.IOTA0, 12)) & 127) as usize];
			let mut fTemp57: F32 = self.fRec2[((i32::wrapping_sub(self.IOTA0, 11)) & 127) as usize];
			let mut fTemp58: F32 = self.fRec2[((i32::wrapping_sub(self.IOTA0, 10)) & 127) as usize];
			let mut fTemp59: F32 = self.fRec2[((i32::wrapping_sub(self.IOTA0, 9)) & 127) as usize];
			let mut fTemp60: F32 = self.fRec2[((i32::wrapping_sub(self.IOTA0, 8)) & 127) as usize];
			let mut fTemp61: F32 = self.fRec2[((i32::wrapping_sub(self.IOTA0, 7)) & 127) as usize];
			let mut fTemp62: F32 = self.fRec2[((i32::wrapping_sub(self.IOTA0, 6)) & 127) as usize];
			let mut fTemp63: F32 = self.fRec2[((i32::wrapping_sub(self.IOTA0, 5)) & 127) as usize];
			let mut fTemp64: F32 = self.fRec2[((i32::wrapping_sub(self.IOTA0, 4)) & 127) as usize];
			let mut fTemp65: F32 = self.fRec2[((i32::wrapping_sub(self.IOTA0, 3)) & 127) as usize];
			let mut fTemp66: F32 = self.fRec2[((i32::wrapping_sub(self.IOTA0, 2)) & 127) as usize];
			let mut fTemp67: F32 = self.fRec2[((i32::wrapping_sub(self.IOTA0, 1)) & 127) as usize];
			let mut fTemp68: F32 = self.fRec2[(self.IOTA0 & 127) as usize];
			*output0 = fSlow1 * (if iSlow0 != 0 {self.fConst183 * fTemp68 + self.fConst182 * fTemp67 + self.fConst181 * fTemp66 + self.fConst180 * fTemp65 + self.fConst179 * fTemp64 + self.fConst178 * fTemp63 + self.fConst177 * fTemp62 + self.fConst176 * fTemp61 + self.fConst175 * fTemp60 + self.fConst174 * fTemp59 + self.fConst173 * fTemp58 + self.fConst172 * fTemp57 + self.fConst171 * fTemp56 + self.fConst170 * fTemp55 + self.fConst169 * fTemp54 + self.fConst168 * fTemp53 + self.fConst167 * fTemp52 + self.fConst166 * fTemp51 + self.fConst165 * fTemp50 + self.fConst164 * fTemp49 + self.fConst163 * fTemp48 + self.fConst162 * fTemp47 + self.fConst161 * fTemp46 + self.fConst160 * fTemp45 + self.fConst159 * fTemp44 + self.fConst158 * fTemp43 + self.fConst157 * fTemp42 + self.fConst156 * fTemp41 + self.fConst155 * fTemp40 + self.fConst154 * fTemp39 + self.fConst153 * fTemp38 + self.fConst152 * fTemp37 + self.fConst151 * fTemp36 + self.fConst150 * fTemp35 + self.fConst149 * fTemp34 + self.fConst148 * fTemp33 + self.fConst147 * fTemp32 + self.fConst146 * fTemp31 + self.fConst145 * fTemp30 + self.fConst144 * fTemp29 + self.fConst143 * fTemp28 + self.fConst142 * fTemp27 + self.fConst141 * fTemp26 + self.fConst140 * fTemp25 + self.fConst139 * fTemp24 + self.fConst138 * fTemp23 + self.fConst137 * fTemp22 + self.fConst136 * fTemp21 + self.fConst135 * fTemp20 + self.fConst134 * fTemp19 + self.fConst133 * fTemp18 + self.fConst132 * fTemp17 + self.fConst131 * fTemp16 + self.fConst130 * fTemp15 + self.fConst129 * fTemp14 + self.fConst128 * fTemp13 + self.fConst127 * fTemp12 + self.fConst126 * fTemp11 + self.fConst125 * fTemp10 + self.fConst124 * fTemp9 + self.fConst123 * fTemp8 + self.fConst122 * fTemp7 + self.fConst121 * fTemp6 + self.fConst120 * self.fRec2[((i32::wrapping_sub(self.IOTA0, 63)) & 127) as usize] + self.fConst119 * self.fRec2[((i32::wrapping_sub(self.IOTA0, 64)) & 127) as usize] + self.fConst118 * self.fRec2[((i32::wrapping_sub(self.IOTA0, 65)) & 127) as usize] + self.fConst117 * self.fRec2[((i32::wrapping_sub(self.IOTA0, 66)) & 127) as usize] + self.fConst116 * self.fRec2[((i32::wrapping_sub(self.IOTA0, 67)) & 127) as usize] + self.fConst115 * self.fRec2[((i32::wrapping_sub(self.IOTA0, 68)) & 127) as usize] + self.fConst114 * self.fRec2[((i32::wrapping_sub(self.IOTA0, 69)) & 127) as usize] + self.fConst113 * self.fRec2[((i32::wrapping_sub(self.IOTA0, 70)) & 127) as usize] + self.fConst112 * self.fRec2[((i32::wrapping_sub(self.IOTA0, 71)) & 127) as usize] + self.fConst111 * self.fRec2[((i32::wrapping_sub(self.IOTA0, 72)) & 127) as usize] + self.fConst110 * self.fRec2[((i32::wrapping_sub(self.IOTA0, 73)) & 127) as usize] + self.fConst109 * self.fRec2[((i32::wrapping_sub(self.IOTA0, 74)) & 127) as usize] + self.fConst108 * self.fRec2[((i32::wrapping_sub(self.IOTA0, 75)) & 127) as usize] + self.fConst107 * self.fRec2[((i32::wrapping_sub(self.IOTA0, 76)) & 127) as usize] + self.fConst106 * self.fRec2[((i32::wrapping_sub(self.IOTA0, 77)) & 127) as usize] + self.fConst105 * self.fRec2[((i32::wrapping_sub(self.IOTA0, 78)) & 127) as usize] + self.fConst104 * self.fRec2[((i32::wrapping_sub(self.IOTA0, 79)) & 127) as usize] + self.fConst103 * self.fRec2[((i32::wrapping_sub(self.IOTA0, 80)) & 127) as usize] + self.fConst102 * self.fRec2[((i32::wrapping_sub(self.IOTA0, 81)) & 127) as usize] + self.fConst101 * self.fRec2[((i32::wrapping_sub(self.IOTA0, 82)) & 127) as usize] + self.fConst100 * self.fRec2[((i32::wrapping_sub(self.IOTA0, 83)) & 127) as usize] + self.fConst99 * self.fRec2[((i32::wrapping_sub(self.IOTA0, 84)) & 127) as usize] + self.fConst98 * self.fRec2[((i32::wrapping_sub(self.IOTA0, 85)) & 127) as usize] + self.fConst97 * self.fRec2[((i32::wrapping_sub(self.IOTA0, 86)) & 127) as usize] + self.fConst96 * self.fRec2[((i32::wrapping_sub(self.IOTA0, 87)) & 127) as usize] + self.fConst95 * self.fRec2[((i32::wrapping_sub(self.IOTA0, 88)) & 127) as usize] + self.fConst94 * self.fRec2[((i32::wrapping_sub(self.IOTA0, 89)) & 127) as usize] + self.fConst93 * self.fRec2[((i32::wrapping_sub(self.IOTA0, 90)) & 127) as usize] + self.fConst92 * self.fRec2[((i32::wrapping_sub(self.IOTA0, 91)) & 127) as usize] + self.fConst91 * self.fRec2[((i32::wrapping_sub(self.IOTA0, 92)) & 127) as usize] + self.fConst90 * self.fRec2[((i32::wrapping_sub(self.IOTA0, 93)) & 127) as usize] + self.fConst89 * self.fRec2[((i32::wrapping_sub(self.IOTA0, 94)) & 127) as usize] + self.fConst88 * self.fRec2[((i32::wrapping_sub(self.IOTA0, 95)) & 127) as usize] + self.fConst87 * self.fRec2[((i32::wrapping_sub(self.IOTA0, 96)) & 127) as usize] + self.fConst86 * self.fRec2[((i32::wrapping_sub(self.IOTA0, 97)) & 127) as usize] + self.fConst85 * self.fRec2[((i32::wrapping_sub(self.IOTA0, 98)) & 127) as usize] + self.fConst84 * self.fRec2[((i32::wrapping_sub(self.IOTA0, 99)) & 127) as usize] + self.fConst83 * self.fRec2[((i32::wrapping_sub(self.IOTA0, 100)) & 127) as usize] + self.fConst82 * self.fRec2[((i32::wrapping_sub(self.IOTA0, 101)) & 127) as usize] + self.fConst81 * self.fRec2[((i32::wrapping_sub(self.IOTA0, 102)) & 127) as usize] + self.fConst80 * self.fRec2[((i32::wrapping_sub(self.IOTA0, 103)) & 127) as usize] + self.fConst79 * self.fRec2[((i32::wrapping_sub(self.IOTA0, 104)) & 127) as usize] + self.fConst78 * self.fRec2[((i32::wrapping_sub(self.IOTA0, 105)) & 127) as usize] + self.fConst77 * self.fRec2[((i32::wrapping_sub(self.IOTA0, 106)) & 127) as usize] + self.fConst76 * self.fRec2[((i32::wrapping_sub(self.IOTA0, 107)) & 127) as usize] + self.fConst75 * self.fRec2[((i32::wrapping_sub(self.IOTA0, 108)) & 127) as usize] + self.fConst74 * self.fRec2[((i32::wrapping_sub(self.IOTA0, 109)) & 127) as usize] + self.fConst73 * self.fRec2[((i32::wrapping_sub(self.IOTA0, 110)) & 127) as usize] + self.fConst72 * self.fRec2[((i32::wrapping_sub(self.IOTA0, 111)) & 127) as usize] + self.fConst71 * self.fRec2[((i32::wrapping_sub(self.IOTA0, 112)) & 127) as usize] + self.fConst70 * self.fRec2[((i32::wrapping_sub(self.IOTA0, 113)) & 127) as usize] + self.fConst69 * self.fRec2[((i32::wrapping_sub(self.IOTA0, 114)) & 127) as usize] + self.fConst68 * self.fRec2[((i32::wrapping_sub(self.IOTA0, 115)) & 127) as usize] + self.fConst67 * self.fRec2[((i32::wrapping_sub(self.IOTA0, 116)) & 127) as usize] + self.fConst66 * self.fRec2[((i32::wrapping_sub(self.IOTA0, 117)) & 127) as usize] + self.fConst65 * self.fRec2[((i32::wrapping_sub(self.IOTA0, 118)) & 127) as usize] + self.fConst64 * self.fRec2[((i32::wrapping_sub(self.IOTA0, 119)) & 127) as usize] + self.fConst63 * self.fRec2[((i32::wrapping_sub(self.IOTA0, 120)) & 127) as usize]} else {self.fConst62 * fTemp68 + self.fConst61 * fTemp67 + self.fConst60 * fTemp66 + self.fConst59 * fTemp65 + self.fConst58 * fTemp64 + self.fConst57 * fTemp63 + self.fConst56 * fTemp62 + self.fConst55 * fTemp61 + self.fConst54 * fTemp60 + self.fConst53 * fTemp59 + self.fConst52 * fTemp58 + self.fConst51 * fTemp57 + self.fConst50 * fTemp56 + self.fConst49 * fTemp55 + self.fConst48 * fTemp54 + self.fConst47 * fTemp53 + self.fConst46 * fTemp52 + self.fConst45 * fTemp51 + self.fConst44 * fTemp50 + self.fConst43 * fTemp49 + self.fConst42 * fTemp48 + self.fConst41 * fTemp47 + self.fConst40 * fTemp46 + self.fConst39 * fTemp45 + self.fConst38 * fTemp44 + self.fConst37 * fTemp43 + self.fConst36 * fTemp42 + self.fConst35 * fTemp41 + self.fConst34 * fTemp40 + self.fConst33 * fTemp39 + self.fConst32 * fTemp38 + self.fConst31 * fTemp37 + self.fConst30 * fTemp36 + self.fConst29 * fTemp35 + self.fConst28 * fTemp34 + self.fConst27 * fTemp33 + self.fConst26 * fTemp32 + self.fConst25 * fTemp31 + self.fConst24 * fTemp30 + self.fConst23 * fTemp29 + self.fConst22 * fTemp28 + self.fConst21 * fTemp27 + self.fConst20 * fTemp26 + self.fConst19 * fTemp25 + self.fConst18 * fTemp24 + self.fConst17 * fTemp23 + self.fConst16 * fTemp22 + self.fConst15 * fTemp21 + self.fConst14 * fTemp20 + self.fConst13 * fTemp19 + self.fConst12 * fTemp18 + self.fConst11 * fTemp17 + self.fConst10 * fTemp16 + self.fConst9 * fTemp15 + self.fConst8 * fTemp14 + self.fConst7 * fTemp13 + self.fConst6 * fTemp12 + self.fConst5 * fTemp11 + self.fConst4 * fTemp10 + self.fConst3 * fTemp9 + self.fConst2 * fTemp8 + self.fConst1 * fTemp7 + self.fConst0 * fTemp6});
			self.iRec0[1] = self.iRec0[0];
			self.iRec1[1] = self.iRec1[0];
			self.IOTA0 = i32::wrapping_add(self.IOTA0, 1);
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
