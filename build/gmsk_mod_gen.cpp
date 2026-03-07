/* ------------------------------------------------------------
name: "gmsk_mod"
Code generated with Faust 2.83.1 (https://faust.grame.fr)
Compilation options: -lang rust -fpga-mem-th 4 -ct 1 -es 1 -mcd 16 -mdd 1024 -mdy 33 -single -ftz 0
------------------------------------------------------------ */
#[cfg_attr(feature = "default-boxed", derive(default_boxed::DefaultBoxed))]
#[repr(C)]
pub struct mydsp {
	iVec0: [i32;2],
	fSampleRate: i32,
	fConst0: F32,
	fConst1: F32,
	fConst2: F32,
	fHslider0: F32,
	fRec1: [F32;2],
	fConst3: F32,
	fRec0: [F32;2],
	fHslider1: F32,
	fRec4: [F32;2],
	fHslider2: F32,
	fRec5: [F32;2],
	fConst4: F32,
	fVec1: [F32;2],
	fRec7: [F32;2],
	fRec6: [F32;2],
	fRec3: [F32;2],
	fConst5: F32,
	fRec2: [F32;2],
	fHslider3: F32,
}

pub type FaustFloat = F32;
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

pub const FAUST_INPUTS: usize = 1;
pub const FAUST_OUTPUTS: usize = 2;
pub const FAUST_ACTIVES: usize = 4;
pub const FAUST_PASSIVES: usize = 0;


impl mydsp {
		
	pub fn new() -> mydsp { 
		mydsp {
			iVec0: [0;2],
			fSampleRate: 0,
			fConst0: 0.0,
			fConst1: 0.0,
			fConst2: 0.0,
			fHslider0: 0.0,
			fRec1: [0.0;2],
			fConst3: 0.0,
			fRec0: [0.0;2],
			fHslider1: 0.0,
			fRec4: [0.0;2],
			fHslider2: 0.0,
			fRec5: [0.0;2],
			fConst4: 0.0,
			fVec1: [0.0;2],
			fRec7: [0.0;2],
			fRec6: [0.0;2],
			fRec3: [0.0;2],
			fConst5: 0.0,
			fRec2: [0.0;2],
			fHslider3: 0.0,
		}
	}
	pub fn metadata(&self, m: &mut dyn Meta) { 
		m.declare("compile_options", r"-lang rust -fpga-mem-th 4 -ct 1 -es 1 -mcd 16 -mdd 1024 -mdy 33 -single -ftz 0");
		m.declare("filename", r"gmsk_mod.dsp");
		m.declare("filters.lib/lowpass0_highpass1", r"Copyright (C) 2003-2019 by Julius O. Smith III <jos@ccrma.stanford.edu>");
		m.declare("filters.lib/lowpass0_highpass1:author", r"Julius O. Smith III");
		m.declare("filters.lib/lowpass:author", r"Julius O. Smith III");
		m.declare("filters.lib/lowpass:copyright", r"Copyright (C) 2003-2019 by Julius O. Smith III <jos@ccrma.stanford.edu>");
		m.declare("filters.lib/lowpass:license", r"MIT-style STK-4.3 license");
		m.declare("filters.lib/name", r"Faust Filters Library");
		m.declare("filters.lib/pole:author", r"Julius O. Smith III");
		m.declare("filters.lib/pole:copyright", r"Copyright (C) 2003-2019 by Julius O. Smith III <jos@ccrma.stanford.edu>");
		m.declare("filters.lib/pole:license", r"MIT-style STK-4.3 license");
		m.declare("filters.lib/tf1:author", r"Julius O. Smith III");
		m.declare("filters.lib/tf1:copyright", r"Copyright (C) 2003-2019 by Julius O. Smith III <jos@ccrma.stanford.edu>");
		m.declare("filters.lib/tf1:license", r"MIT-style STK-4.3 license");
		m.declare("filters.lib/tf1s:author", r"Julius O. Smith III");
		m.declare("filters.lib/tf1s:copyright", r"Copyright (C) 2003-2019 by Julius O. Smith III <jos@ccrma.stanford.edu>");
		m.declare("filters.lib/tf1s:license", r"MIT-style STK-4.3 license");
		m.declare("filters.lib/version", r"1.7.1");
		m.declare("maths.lib/author", r"GRAME");
		m.declare("maths.lib/copyright", r"GRAME");
		m.declare("maths.lib/license", r"LGPL with exception");
		m.declare("maths.lib/name", r"Faust Math Library");
		m.declare("maths.lib/version", r"2.9.0");
		m.declare("name", r"gmsk_mod");
		m.declare("oscillators.lib/name", r"Faust Oscillator Library");
		m.declare("oscillators.lib/version", r"1.6.0");
		m.declare("platform.lib/name", r"Generic Platform Library");
		m.declare("platform.lib/version", r"1.3.0");
		m.declare("signals.lib/name", r"Faust Signal Routing Library");
		m.declare("signals.lib/version", r"1.6.0");
	}

	pub fn get_sample_rate(&self) -> i32 { self.fSampleRate as i32}
	
	pub fn class_init(sample_rate: i32) {
		// Obtaining locks on 0 static var(s)
	}
	pub fn instance_reset_params(&mut self) {
		self.fHslider0 = 0.1;
		self.fHslider1 = 9.6e+03;
		self.fHslider2 = 0.3;
		self.fHslider3 = 0.9;
	}
	pub fn instance_clear(&mut self) {
		for l0 in 0..2 {
			self.iVec0[l0 as usize] = 0;
		}
		for l1 in 0..2 {
			self.fRec1[l1 as usize] = 0.0;
		}
		for l2 in 0..2 {
			self.fRec0[l2 as usize] = 0.0;
		}
		for l3 in 0..2 {
			self.fRec4[l3 as usize] = 0.0;
		}
		for l4 in 0..2 {
			self.fRec5[l4 as usize] = 0.0;
		}
		for l5 in 0..2 {
			self.fVec1[l5 as usize] = 0.0;
		}
		for l6 in 0..2 {
			self.fRec7[l6 as usize] = 0.0;
		}
		for l7 in 0..2 {
			self.fRec6[l7 as usize] = 0.0;
		}
		for l8 in 0..2 {
			self.fRec3[l8 as usize] = 0.0;
		}
		for l9 in 0..2 {
			self.fRec2[l9 as usize] = 0.0;
		}
	}
	pub fn instance_constants(&mut self, sample_rate: i32) {
		// Obtaining locks on 0 static var(s)
		self.fSampleRate = sample_rate;
		self.fConst0 = F32::min(1.92e+05, F32::max(1.0, (self.fSampleRate) as F32));
		self.fConst1 = 44.1 / self.fConst0;
		self.fConst2 = 1.0 - self.fConst1;
		self.fConst3 = 1.0 / self.fConst0;
		self.fConst4 = 3.1415927 / self.fConst0;
		self.fConst5 = 1.5707964 / self.fConst0;
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
		ui_interface.open_vertical_box("gmsk_mod");
		ui_interface.add_horizontal_slider("gmsk/bt_product", ParamIndex(0), 0.3, 0.1, 0.7, 0.01);
		ui_interface.add_horizontal_slider("gmsk/carrier_freq", ParamIndex(1), 0.1, 0.001, 0.5, 0.0001);
		ui_interface.add_horizontal_slider("gmsk/output_gain", ParamIndex(2), 0.9, 0.0, 1.0, 0.001);
		ui_interface.add_horizontal_slider("gmsk/symbol_rate", ParamIndex(3), 9.6e+03, 3e+02, 1.152e+05, 1.0);
		ui_interface.close_box();
	}
	
	pub fn get_param(&self, param: ParamIndex) -> Option<FaustFloat> {
		match param.0 {
			1 => Some(self.fHslider0),
			3 => Some(self.fHslider1),
			0 => Some(self.fHslider2),
			2 => Some(self.fHslider3),
			_ => None,
		}
	}
	
	pub fn set_param(&mut self, param: ParamIndex, value: FaustFloat) {
		match param.0 {
			1 => { self.fHslider0 = value }
			3 => { self.fHslider1 = value }
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
		
		// Obtaining locks on 0 static var(s)
		let [inputs0, .. ] = inputs.as_ref() else { panic!("wrong number of input buffers"); };
		let inputs0 = inputs0.as_ref()[..count].iter();
		let [outputs0, outputs1, .. ] = outputs.as_mut() else { panic!("wrong number of output buffers"); };
		let outputs0 = outputs0.as_mut()[..count].iter_mut();
		let outputs1 = outputs1.as_mut()[..count].iter_mut();
		let mut fSlow0: F32 = self.fConst1 * self.fHslider0;
		let mut fSlow1: F32 = self.fConst1 * self.fHslider1;
		let mut fSlow2: F32 = self.fConst1 * self.fHslider2;
		let mut fSlow3: F32 = self.fHslider3;
		let zipped_iterators = inputs0.zip(outputs0).zip(outputs1);
		for ((input0, output0), output1) in zipped_iterators {
			self.iVec0[0] = 1;
			self.fRec1[0] = fSlow0 + self.fConst2 * self.fRec1[1];
			let mut fTemp0: F32 = (if i32::wrapping_sub(1, self.iVec0[1]) != 0 {0.0} else {self.fRec0[1] + self.fConst3 * self.fRec1[0]});
			self.fRec0[0] = fTemp0 - F32::floor(fTemp0);
			self.fRec4[0] = fSlow1 + self.fConst2 * self.fRec4[1];
			self.fRec5[0] = fSlow2 + self.fConst2 * self.fRec5[1];
			let mut fTemp1: F32 = 1.0 / F32::tan(self.fConst4 * self.fRec5[0] * self.fRec4[0]);
			let mut fTemp2: F32 = fTemp1 + 1.0;
			let mut fTemp3: F32 = *input0;
			self.fVec1[0] = fTemp3;
			let mut fTemp4: F32 = 1.0 - fTemp1;
			self.fRec7[0] = -((fTemp4 * self.fRec7[1] - (fTemp3 + self.fVec1[1])) / fTemp2);
			self.fRec6[0] = -((fTemp4 * self.fRec6[1] - (self.fRec7[0] + self.fRec7[1])) / fTemp2);
			self.fRec3[0] = -((self.fRec3[1] * fTemp4 - (self.fRec6[0] + self.fRec6[1])) / fTemp2);
			self.fRec2[0] = 0.9999 * self.fRec2[1] + self.fConst5 * self.fRec4[0] * self.fRec3[0];
			let mut fTemp5: F32 = self.fRec2[0] + 6.2831855 * self.fRec0[0];
			*output0 = fSlow3 * F32::cos(fTemp5);
			*output1 = fSlow3 * F32::sin(fTemp5);
			self.iVec0[1] = self.iVec0[0];
			self.fRec1[1] = self.fRec1[0];
			self.fRec0[1] = self.fRec0[0];
			self.fRec4[1] = self.fRec4[0];
			self.fRec5[1] = self.fRec5[0];
			self.fVec1[1] = self.fVec1[0];
			self.fRec7[1] = self.fRec7[0];
			self.fRec6[1] = self.fRec6[0];
			self.fRec3[1] = self.fRec3[0];
			self.fRec2[1] = self.fRec2[0];
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
