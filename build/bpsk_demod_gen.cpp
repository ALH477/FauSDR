/* ------------------------------------------------------------
name: "bpsk_demod"
Code generated with Faust 2.83.1 (https://faust.grame.fr)
Compilation options: -lang rust -fpga-mem-th 4 -ct 1 -es 1 -mcd 16 -mdd 1024 -mdy 33 -single -ftz 0
------------------------------------------------------------ */
#[cfg_attr(feature = "default-boxed", derive(default_boxed::DefaultBoxed))]
#[repr(C)]
pub struct mydsp {
	iRec0: [i32;2],
	iRec1: [i32;2],
	IOTA0: i32,
	fRec2: [F32;64],
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
	fHslider0: F32,
	fSampleRate: i32,
}

pub type FaustFloat = F32;
static fmydspSIG0Wave0: [F32;63] = [-4.99e-07,3.2e-08,2.364e-06,-3.902e-06,-1.013e-06,1.1437e-05,-1.3271e-05,-7.479e-06,3.7337e-05,-3.2618e-05,-3.0961e-05,9.9169e-05,-6.4865e-05,-9.8284e-05,0.000231999,-0.000107568,-0.000269222,0.000503715,-0.00014301,-0.000685631,0.001068726,-0.000109512,-0.001750678,0.002395231,0.000215126,-0.005085017,0.006802846,0.001919179,-0.024575477,0.056235954,-0.08436117,1.0956341,-0.08436117,0.056235954,-0.024575477,0.001919179,0.006802846,-0.005085017,0.000215126,0.002395231,-0.001750678,-0.000109512,0.001068726,-0.000685631,-0.00014301,0.000503715,-0.000269222,-0.000107568,0.000231999,-9.8284e-05,-6.4865e-05,9.9169e-05,-3.0961e-05,-3.2618e-05,3.7337e-05,-7.479e-06,-1.3271e-05,1.1437e-05,-1.013e-06,-3.902e-06,2.364e-06,3.2e-08,-4.99e-07];

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
static ftbl0mydspSIG0: std::sync::RwLock<[F32;63]>  = std::sync::RwLock::new([0.0;63]);
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
pub const FAUST_ACTIVES: usize = 1;
pub const FAUST_PASSIVES: usize = 0;


impl mydsp {
		
	pub fn new() -> mydsp { 
		mydsp {
			iRec0: [0;2],
			iRec1: [0;2],
			IOTA0: 0,
			fRec2: [0.0;64],
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
			fHslider0: 0.0,
			fSampleRate: 0,
		}
	}
	pub fn metadata(&self, m: &mut dyn Meta) { 
		m.declare("compile_options", r"-lang rust -fpga-mem-th 4 -ct 1 -es 1 -mcd 16 -mdd 1024 -mdy 33 -single -ftz 0");
		m.declare("filename", r"bpsk_demod.dsp");
		m.declare("filters.lib/lowpass0_highpass1", r"Copyright (C) 2003-2019 by Julius O. Smith III <jos@ccrma.stanford.edu>");
		m.declare("filters.lib/name", r"Faust Filters Library");
		m.declare("filters.lib/version", r"1.7.1");
		m.declare("maths.lib/author", r"GRAME");
		m.declare("maths.lib/copyright", r"GRAME");
		m.declare("maths.lib/license", r"LGPL with exception");
		m.declare("maths.lib/name", r"Faust Math Library");
		m.declare("maths.lib/version", r"2.9.0");
		m.declare("name", r"bpsk_demod");
		m.declare("oscillators.lib/name", r"Faust Oscillator Library");
		m.declare("oscillators.lib/version", r"1.6.0");
	}

	pub fn get_sample_rate(&self) -> i32 { self.fSampleRate as i32}
	
	pub fn class_init(sample_rate: i32) {
		// Obtaining locks on 1 static var(s)
		let mut ftbl0mydspSIG0_guard = ftbl0mydspSIG0.write().unwrap();
		let mut sig0: mydspSIG0 = newmydspSIG0();
		sig0.instance_initmydspSIG0(sample_rate);
		sig0.fillmydspSIG0(63, ftbl0mydspSIG0_guard.as_mut());
	}
	pub fn instance_reset_params(&mut self) {
		self.fHslider0 = 1.0;
	}
	pub fn instance_clear(&mut self) {
		for l0 in 0..2 {
			self.iRec0[l0 as usize] = 0;
		}
		for l1 in 0..2 {
			self.iRec1[l1 as usize] = 0;
		}
		self.IOTA0 = 0;
		for l2 in 0..64 {
			self.fRec2[l2 as usize] = 0.0;
		}
	}
	pub fn instance_constants(&mut self, sample_rate: i32) {
		// Obtaining locks on 1 static var(s)
		let ftbl0mydspSIG0_guard = ftbl0mydspSIG0.read().unwrap();
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
		ui_interface.open_vertical_box("bpsk_demod");
		ui_interface.add_horizontal_slider("bpsk_demod/output_gain", ParamIndex(0), 1.0, 0.0, 4.0, 0.01);
		ui_interface.close_box();
	}
	
	pub fn get_param(&self, param: ParamIndex) -> Option<FaustFloat> {
		match param.0 {
			0 => Some(self.fHslider0),
			_ => None,
		}
	}
	
	pub fn set_param(&mut self, param: ParamIndex, value: FaustFloat) {
		match param.0 {
			0 => { self.fHslider0 = value }
			_ => {}
		}
	}
	
	pub fn compute(
		&mut self,
		count: usize,
		inputs: &[impl AsRef<[FaustFloat]>],
		outputs: &mut[impl AsMut<[FaustFloat]>],
	) {
		
		// Obtaining locks on 1 static var(s)
		let ftbl0mydspSIG0_guard = ftbl0mydspSIG0.read().unwrap();
		let [inputs0, inputs1, .. ] = inputs.as_ref() else { panic!("wrong number of input buffers"); };
		let inputs0 = inputs0.as_ref()[..count].iter();
		let inputs1 = inputs1.as_ref()[..count].iter();
		let [outputs0, .. ] = outputs.as_mut() else { panic!("wrong number of output buffers"); };
		let outputs0 = outputs0.as_mut()[..count].iter_mut();
		let mut fSlow0: F32 = self.fHslider0;
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
			self.fRec2[(self.IOTA0 & 63) as usize] = fTemp5;
			*output0 = fSlow0 * (self.fConst62 * self.fRec2[(self.IOTA0 & 63) as usize] + self.fConst61 * self.fRec2[((i32::wrapping_sub(self.IOTA0, 1)) & 63) as usize] + self.fConst60 * self.fRec2[((i32::wrapping_sub(self.IOTA0, 2)) & 63) as usize] + self.fConst59 * self.fRec2[((i32::wrapping_sub(self.IOTA0, 3)) & 63) as usize] + self.fConst58 * self.fRec2[((i32::wrapping_sub(self.IOTA0, 4)) & 63) as usize] + self.fConst57 * self.fRec2[((i32::wrapping_sub(self.IOTA0, 5)) & 63) as usize] + self.fConst56 * self.fRec2[((i32::wrapping_sub(self.IOTA0, 6)) & 63) as usize] + self.fConst55 * self.fRec2[((i32::wrapping_sub(self.IOTA0, 7)) & 63) as usize] + self.fConst54 * self.fRec2[((i32::wrapping_sub(self.IOTA0, 8)) & 63) as usize] + self.fConst53 * self.fRec2[((i32::wrapping_sub(self.IOTA0, 9)) & 63) as usize] + self.fConst52 * self.fRec2[((i32::wrapping_sub(self.IOTA0, 10)) & 63) as usize] + self.fConst51 * self.fRec2[((i32::wrapping_sub(self.IOTA0, 11)) & 63) as usize] + self.fConst50 * self.fRec2[((i32::wrapping_sub(self.IOTA0, 12)) & 63) as usize] + self.fConst49 * self.fRec2[((i32::wrapping_sub(self.IOTA0, 13)) & 63) as usize] + self.fConst48 * self.fRec2[((i32::wrapping_sub(self.IOTA0, 14)) & 63) as usize] + self.fConst47 * self.fRec2[((i32::wrapping_sub(self.IOTA0, 15)) & 63) as usize] + self.fConst46 * self.fRec2[((i32::wrapping_sub(self.IOTA0, 16)) & 63) as usize] + self.fConst45 * self.fRec2[((i32::wrapping_sub(self.IOTA0, 17)) & 63) as usize] + self.fConst44 * self.fRec2[((i32::wrapping_sub(self.IOTA0, 18)) & 63) as usize] + self.fConst43 * self.fRec2[((i32::wrapping_sub(self.IOTA0, 19)) & 63) as usize] + self.fConst42 * self.fRec2[((i32::wrapping_sub(self.IOTA0, 20)) & 63) as usize] + self.fConst41 * self.fRec2[((i32::wrapping_sub(self.IOTA0, 21)) & 63) as usize] + self.fConst40 * self.fRec2[((i32::wrapping_sub(self.IOTA0, 22)) & 63) as usize] + self.fConst39 * self.fRec2[((i32::wrapping_sub(self.IOTA0, 23)) & 63) as usize] + self.fConst38 * self.fRec2[((i32::wrapping_sub(self.IOTA0, 24)) & 63) as usize] + self.fConst37 * self.fRec2[((i32::wrapping_sub(self.IOTA0, 25)) & 63) as usize] + self.fConst36 * self.fRec2[((i32::wrapping_sub(self.IOTA0, 26)) & 63) as usize] + self.fConst35 * self.fRec2[((i32::wrapping_sub(self.IOTA0, 27)) & 63) as usize] + self.fConst34 * self.fRec2[((i32::wrapping_sub(self.IOTA0, 28)) & 63) as usize] + self.fConst33 * self.fRec2[((i32::wrapping_sub(self.IOTA0, 29)) & 63) as usize] + self.fConst32 * self.fRec2[((i32::wrapping_sub(self.IOTA0, 30)) & 63) as usize] + self.fConst31 * self.fRec2[((i32::wrapping_sub(self.IOTA0, 31)) & 63) as usize] + self.fConst30 * self.fRec2[((i32::wrapping_sub(self.IOTA0, 32)) & 63) as usize] + self.fConst29 * self.fRec2[((i32::wrapping_sub(self.IOTA0, 33)) & 63) as usize] + self.fConst28 * self.fRec2[((i32::wrapping_sub(self.IOTA0, 34)) & 63) as usize] + self.fConst27 * self.fRec2[((i32::wrapping_sub(self.IOTA0, 35)) & 63) as usize] + self.fConst26 * self.fRec2[((i32::wrapping_sub(self.IOTA0, 36)) & 63) as usize] + self.fConst25 * self.fRec2[((i32::wrapping_sub(self.IOTA0, 37)) & 63) as usize] + self.fConst24 * self.fRec2[((i32::wrapping_sub(self.IOTA0, 38)) & 63) as usize] + self.fConst23 * self.fRec2[((i32::wrapping_sub(self.IOTA0, 39)) & 63) as usize] + self.fConst22 * self.fRec2[((i32::wrapping_sub(self.IOTA0, 40)) & 63) as usize] + self.fConst21 * self.fRec2[((i32::wrapping_sub(self.IOTA0, 41)) & 63) as usize] + self.fConst20 * self.fRec2[((i32::wrapping_sub(self.IOTA0, 42)) & 63) as usize] + self.fConst19 * self.fRec2[((i32::wrapping_sub(self.IOTA0, 43)) & 63) as usize] + self.fConst18 * self.fRec2[((i32::wrapping_sub(self.IOTA0, 44)) & 63) as usize] + self.fConst17 * self.fRec2[((i32::wrapping_sub(self.IOTA0, 45)) & 63) as usize] + self.fConst16 * self.fRec2[((i32::wrapping_sub(self.IOTA0, 46)) & 63) as usize] + self.fConst15 * self.fRec2[((i32::wrapping_sub(self.IOTA0, 47)) & 63) as usize] + self.fConst14 * self.fRec2[((i32::wrapping_sub(self.IOTA0, 48)) & 63) as usize] + self.fConst13 * self.fRec2[((i32::wrapping_sub(self.IOTA0, 49)) & 63) as usize] + self.fConst12 * self.fRec2[((i32::wrapping_sub(self.IOTA0, 50)) & 63) as usize] + self.fConst11 * self.fRec2[((i32::wrapping_sub(self.IOTA0, 51)) & 63) as usize] + self.fConst10 * self.fRec2[((i32::wrapping_sub(self.IOTA0, 52)) & 63) as usize] + self.fConst9 * self.fRec2[((i32::wrapping_sub(self.IOTA0, 53)) & 63) as usize] + self.fConst8 * self.fRec2[((i32::wrapping_sub(self.IOTA0, 54)) & 63) as usize] + self.fConst7 * self.fRec2[((i32::wrapping_sub(self.IOTA0, 55)) & 63) as usize] + self.fConst6 * self.fRec2[((i32::wrapping_sub(self.IOTA0, 56)) & 63) as usize] + self.fConst5 * self.fRec2[((i32::wrapping_sub(self.IOTA0, 57)) & 63) as usize] + self.fConst4 * self.fRec2[((i32::wrapping_sub(self.IOTA0, 58)) & 63) as usize] + self.fConst3 * self.fRec2[((i32::wrapping_sub(self.IOTA0, 59)) & 63) as usize] + self.fConst2 * self.fRec2[((i32::wrapping_sub(self.IOTA0, 60)) & 63) as usize] + self.fConst1 * self.fRec2[((i32::wrapping_sub(self.IOTA0, 61)) & 63) as usize] + self.fConst0 * self.fRec2[((i32::wrapping_sub(self.IOTA0, 62)) & 63) as usize]);
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
