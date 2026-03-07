/* ------------------------------------------------------------
name: "acoustic_fsk_demod"
Code generated with Faust 2.83.1 (https://faust.grame.fr)
Compilation options: -lang rust -fpga-mem-th 4 -ct 1 -es 1 -mcd 16 -mdd 1024 -mdy 33 -single -ftz 0
------------------------------------------------------------ */
#[cfg_attr(feature = "default-boxed", derive(default_boxed::DefaultBoxed))]
#[repr(C)]
pub struct mydsp {
	fSampleRate: i32,
	fConst0: F32,
	fConst1: F32,
	fConst2: F32,
	fHslider0: F32,
	fRec0: [F32;2],
	fConst3: F32,
	fHslider1: F32,
	fRec2: [F32;2],
	fConst4: F32,
	fHslider2: F32,
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
	fVec0: [F32;2],
	fRec5: [F32;3],
	fConst17: F32,
	fRec4: [F32;3],
	fRec3: [F32;3],
	fRec1: [F32;3],
	fHslider3: F32,
	fRec7: [F32;2],
	fRec8: [F32;3],
	fRec6: [F32;3],
	fHslider4: F32,
}

pub type FaustFloat = F32;
fn mydsp_faustpower2_f(value: F32) -> F32 {
	return value * value;
}
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
pub const FAUST_OUTPUTS: usize = 1;
pub const FAUST_ACTIVES: usize = 5;
pub const FAUST_PASSIVES: usize = 0;


impl mydsp {
		
	pub fn new() -> mydsp { 
		mydsp {
			fSampleRate: 0,
			fConst0: 0.0,
			fConst1: 0.0,
			fConst2: 0.0,
			fHslider0: 0.0,
			fRec0: [0.0;2],
			fConst3: 0.0,
			fHslider1: 0.0,
			fRec2: [0.0;2],
			fConst4: 0.0,
			fHslider2: 0.0,
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
			fVec0: [0.0;2],
			fRec5: [0.0;3],
			fConst17: 0.0,
			fRec4: [0.0;3],
			fRec3: [0.0;3],
			fRec1: [0.0;3],
			fHslider3: 0.0,
			fRec7: [0.0;2],
			fRec8: [0.0;3],
			fRec6: [0.0;3],
			fHslider4: 0.0,
		}
	}
	pub fn metadata(&self, m: &mut dyn Meta) { 
		m.declare("compile_options", r"-lang rust -fpga-mem-th 4 -ct 1 -es 1 -mcd 16 -mdd 1024 -mdy 33 -single -ftz 0");
		m.declare("filename", r"acoustic_fsk_demod.dsp");
		m.declare("filters.lib/fir:author", r"Julius O. Smith III");
		m.declare("filters.lib/fir:copyright", r"Copyright (C) 2003-2019 by Julius O. Smith III <jos@ccrma.stanford.edu>");
		m.declare("filters.lib/fir:license", r"MIT-style STK-4.3 license");
		m.declare("filters.lib/highpass:author", r"Julius O. Smith III");
		m.declare("filters.lib/highpass:copyright", r"Copyright (C) 2003-2019 by Julius O. Smith III <jos@ccrma.stanford.edu>");
		m.declare("filters.lib/iir:author", r"Julius O. Smith III");
		m.declare("filters.lib/iir:copyright", r"Copyright (C) 2003-2019 by Julius O. Smith III <jos@ccrma.stanford.edu>");
		m.declare("filters.lib/iir:license", r"MIT-style STK-4.3 license");
		m.declare("filters.lib/lowpass0_highpass1", r"Copyright (C) 2003-2019 by Julius O. Smith III <jos@ccrma.stanford.edu>");
		m.declare("filters.lib/lowpass0_highpass1:author", r"Julius O. Smith III");
		m.declare("filters.lib/lowpass:author", r"Julius O. Smith III");
		m.declare("filters.lib/lowpass:copyright", r"Copyright (C) 2003-2019 by Julius O. Smith III <jos@ccrma.stanford.edu>");
		m.declare("filters.lib/lowpass:license", r"MIT-style STK-4.3 license");
		m.declare("filters.lib/name", r"Faust Filters Library");
		m.declare("filters.lib/resonbp:author", r"Julius O. Smith III");
		m.declare("filters.lib/resonbp:copyright", r"Copyright (C) 2003-2019 by Julius O. Smith III <jos@ccrma.stanford.edu>");
		m.declare("filters.lib/resonbp:license", r"MIT-style STK-4.3 license");
		m.declare("filters.lib/tf2:author", r"Julius O. Smith III");
		m.declare("filters.lib/tf2:copyright", r"Copyright (C) 2003-2019 by Julius O. Smith III <jos@ccrma.stanford.edu>");
		m.declare("filters.lib/tf2:license", r"MIT-style STK-4.3 license");
		m.declare("filters.lib/tf2s:author", r"Julius O. Smith III");
		m.declare("filters.lib/tf2s:copyright", r"Copyright (C) 2003-2019 by Julius O. Smith III <jos@ccrma.stanford.edu>");
		m.declare("filters.lib/tf2s:license", r"MIT-style STK-4.3 license");
		m.declare("filters.lib/version", r"1.7.1");
		m.declare("maths.lib/author", r"GRAME");
		m.declare("maths.lib/copyright", r"GRAME");
		m.declare("maths.lib/license", r"LGPL with exception");
		m.declare("maths.lib/name", r"Faust Math Library");
		m.declare("maths.lib/version", r"2.9.0");
		m.declare("name", r"acoustic_fsk_demod");
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
		self.fHslider0 = 1.2e+03;
		self.fHslider1 = 3e+03;
		self.fHslider2 = 3.5;
		self.fHslider3 = 2e+03;
		self.fHslider4 = 4.0;
	}
	pub fn instance_clear(&mut self) {
		for l0 in 0..2 {
			self.fRec0[l0 as usize] = 0.0;
		}
		for l1 in 0..2 {
			self.fRec2[l1 as usize] = 0.0;
		}
		for l2 in 0..2 {
			self.fVec0[l2 as usize] = 0.0;
		}
		for l3 in 0..3 {
			self.fRec5[l3 as usize] = 0.0;
		}
		for l4 in 0..3 {
			self.fRec4[l4 as usize] = 0.0;
		}
		for l5 in 0..3 {
			self.fRec3[l5 as usize] = 0.0;
		}
		for l6 in 0..3 {
			self.fRec1[l6 as usize] = 0.0;
		}
		for l7 in 0..2 {
			self.fRec7[l7 as usize] = 0.0;
		}
		for l8 in 0..3 {
			self.fRec8[l8 as usize] = 0.0;
		}
		for l9 in 0..3 {
			self.fRec6[l9 as usize] = 0.0;
		}
	}
	pub fn instance_constants(&mut self, sample_rate: i32) {
		// Obtaining locks on 0 static var(s)
		self.fSampleRate = sample_rate;
		self.fConst0 = F32::min(1.92e+05, F32::max(1.0, (self.fSampleRate) as F32));
		self.fConst1 = 44.1 / self.fConst0;
		self.fConst2 = 1.0 - self.fConst1;
		self.fConst3 = 1.5707964 / self.fConst0;
		self.fConst4 = 3.1415927 / self.fConst0;
		self.fConst5 = F32::tan(12566.371 / self.fConst0);
		self.fConst6 = 2.0 * (1.0 - 1.0 / mydsp_faustpower2_f(self.fConst5));
		self.fConst7 = 1.0 / self.fConst5;
		self.fConst8 = (self.fConst7 + -1.4142135) / self.fConst5 + 1.0;
		self.fConst9 = 1.0 / ((self.fConst7 + 1.4142135) / self.fConst5 + 1.0);
		self.fConst10 = F32::tan(2513.2742 / self.fConst0);
		self.fConst11 = mydsp_faustpower2_f(self.fConst10);
		self.fConst12 = 2.0 * (1.0 - 1.0 / self.fConst11);
		self.fConst13 = 1.0 / self.fConst10;
		self.fConst14 = (self.fConst13 + -1.4142135) / self.fConst10 + 1.0;
		self.fConst15 = (self.fConst13 + 1.4142135) / self.fConst10 + 1.0;
		self.fConst16 = 1.0 / self.fConst15;
		self.fConst17 = 1.0 / (self.fConst11 * self.fConst15);
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
		ui_interface.open_vertical_box("acoustic_fsk_demod");
		ui_interface.add_horizontal_slider("acoustic_fsk_demod/bpf_q", ParamIndex(0), 3.5, 1.0, 2e+01, 0.1);
		ui_interface.add_horizontal_slider("acoustic_fsk_demod/f_mark", ParamIndex(1), 2e+03, 2e+02, 8e+03, 1.0);
		ui_interface.add_horizontal_slider("acoustic_fsk_demod/f_space", ParamIndex(2), 3e+03, 2e+02, 8e+03, 1.0);
		ui_interface.add_horizontal_slider("acoustic_fsk_demod/output_gain", ParamIndex(3), 4.0, 0.0, 2e+01, 0.1);
		ui_interface.add_horizontal_slider("acoustic_fsk_demod/symbol_rate", ParamIndex(4), 1.2e+03, 3e+02, 4.8e+03, 1.0);
		ui_interface.close_box();
	}
	
	pub fn get_param(&self, param: ParamIndex) -> Option<FaustFloat> {
		match param.0 {
			4 => Some(self.fHslider0),
			2 => Some(self.fHslider1),
			0 => Some(self.fHslider2),
			1 => Some(self.fHslider3),
			3 => Some(self.fHslider4),
			_ => None,
		}
	}
	
	pub fn set_param(&mut self, param: ParamIndex, value: FaustFloat) {
		match param.0 {
			4 => { self.fHslider0 = value }
			2 => { self.fHslider1 = value }
			0 => { self.fHslider2 = value }
			1 => { self.fHslider3 = value }
			3 => { self.fHslider4 = value }
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
		let [outputs0, .. ] = outputs.as_mut() else { panic!("wrong number of output buffers"); };
		let outputs0 = outputs0.as_mut()[..count].iter_mut();
		let mut fSlow0: F32 = self.fConst1 * self.fHslider0;
		let mut fSlow1: F32 = self.fConst1 * self.fHslider1;
		let mut fSlow2: F32 = 1.0 / self.fHslider2;
		let mut fSlow3: F32 = self.fConst1 * self.fHslider3;
		let mut fSlow4: F32 = self.fHslider4;
		let zipped_iterators = inputs0.zip(outputs0);
		for (input0, output0) in zipped_iterators {
			self.fRec0[0] = fSlow0 + self.fConst2 * self.fRec0[1];
			let mut fTemp0: F32 = F32::tan(self.fConst3 * self.fRec0[0]);
			let mut fTemp1: F32 = 1.0 / fTemp0;
			let mut fTemp2: F32 = (fTemp1 + 1.4142135) / fTemp0 + 1.0;
			let mut fTemp3: F32 = 1.0 - 1.0 / mydsp_faustpower2_f(fTemp0);
			let mut fTemp4: F32 = (fTemp1 + -1.4142135) / fTemp0 + 1.0;
			self.fRec2[0] = fSlow1 + self.fConst2 * self.fRec2[1];
			let mut fTemp5: F32 = F32::tan(self.fConst4 * self.fRec2[0]);
			let mut fTemp6: F32 = 1.0 / fTemp5;
			let mut fTemp7: F32 = (fSlow2 + fTemp6) / fTemp5 + 1.0;
			let mut fTemp8: F32 = mydsp_faustpower2_f(fTemp5);
			let mut fTemp9: F32 = *input0;
			self.fVec0[0] = fTemp9;
			self.fRec5[0] = fTemp9 - (self.fVec0[1] + self.fConst16 * (self.fConst14 * self.fRec5[2] + self.fConst12 * self.fRec5[1]));
			self.fRec4[0] = self.fConst17 * (self.fRec5[2] + (self.fRec5[0] - 2.0 * self.fRec5[1])) - self.fConst9 * (self.fConst8 * self.fRec4[2] + self.fConst6 * self.fRec4[1]);
			let mut fTemp10: F32 = self.fRec4[2] + self.fRec4[0] + 2.0 * self.fRec4[1];
			let mut fTemp11: F32 = self.fConst9 * (fTemp10 / (F32::max(F32::abs(self.fConst9 * fTemp10), 0.0) + 1e-06));
			self.fRec3[0] = fTemp11 - (self.fRec3[2] * ((fTemp6 - fSlow2) / fTemp5 + 1.0) + 2.0 * self.fRec3[1] * (1.0 - 1.0 / fTemp8)) / fTemp7;
			self.fRec1[0] = mydsp_faustpower2_f(self.fRec3[0] - self.fRec3[2]) / (fTemp8 * mydsp_faustpower2_f(fTemp7)) - (fTemp4 * self.fRec1[2] + 2.0 * fTemp3 * self.fRec1[1]) / fTemp2;
			self.fRec7[0] = fSlow3 + self.fConst2 * self.fRec7[1];
			let mut fTemp12: F32 = F32::tan(self.fConst4 * self.fRec7[0]);
			let mut fTemp13: F32 = 1.0 / fTemp12;
			let mut fTemp14: F32 = (fSlow2 + fTemp13) / fTemp12 + 1.0;
			let mut fTemp15: F32 = mydsp_faustpower2_f(fTemp12);
			self.fRec8[0] = fTemp11 - (self.fRec8[2] * ((fTemp13 - fSlow2) / fTemp12 + 1.0) + 2.0 * self.fRec8[1] * (1.0 - 1.0 / fTemp15)) / fTemp14;
			self.fRec6[0] = mydsp_faustpower2_f(self.fRec8[0] - self.fRec8[2]) / (fTemp15 * mydsp_faustpower2_f(fTemp14)) - (self.fRec6[2] * fTemp4 + 2.0 * self.fRec6[1] * fTemp3) / fTemp2;
			*output0 = fSlow4 * ((self.fRec6[2] + self.fRec6[0] + 2.0 * self.fRec6[1] - (self.fRec1[2] + self.fRec1[0] + 2.0 * self.fRec1[1])) / fTemp2);
			self.fRec0[1] = self.fRec0[0];
			self.fRec2[1] = self.fRec2[0];
			self.fVec0[1] = self.fVec0[0];
			self.fRec5[2] = self.fRec5[1];
			self.fRec5[1] = self.fRec5[0];
			self.fRec4[2] = self.fRec4[1];
			self.fRec4[1] = self.fRec4[0];
			self.fRec3[2] = self.fRec3[1];
			self.fRec3[1] = self.fRec3[0];
			self.fRec1[2] = self.fRec1[1];
			self.fRec1[1] = self.fRec1[0];
			self.fRec7[1] = self.fRec7[0];
			self.fRec8[2] = self.fRec8[1];
			self.fRec8[1] = self.fRec8[0];
			self.fRec6[2] = self.fRec6[1];
			self.fRec6[1] = self.fRec6[0];
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
