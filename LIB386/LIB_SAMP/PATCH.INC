ENVELOPES	equ	6

HEADER_SIZE	equ	12
ID_SIZE		equ	10
DESC_SIZE 	equ	60
RESERVED_SIZE	equ	40
PATCH_HEADER_RESERVED_SIZE equ	36
LAYER_RESERVED_SIZE	   equ	40
PATCH_DATA_RESERVED_SIZE   equ	36


PatchHeader	STRUC

;PatchHeader

header		db	HEADER_SIZE dup (?)
gravis_id	db	ID_SIZE dup (?)
description	db	DESC_SIZE dup (?)
instruments	db	?
voices		db	?
channels	db	?
wave_forms	dw	?
master_volume	dw	?
data_size	dd	?
reserved_h	db	PATCH_HEADER_RESERVED_SIZE dup (?)

;InstrumentData

instrument	dw	?
instrument_name	db	16 dup (?)
instrument_size	dd	?
layers		db	?
reserved_i	db	RESERVED_SIZE dup (?)

;LayerData

layer_duplicate	db	?
layer		db	?
layer_size	dd	?
samples		db	?
reserved_l	db	LAYER_RESERVED_SIZE dup (?)

PatchHeader	ENDS


PatchData	STRUC

wave_name	db      7 dup (?)
fractions	db	?
wave_size	dd	?
start_loop	dd	?
end_loop	dd	?
sample_rate	dw	?
low_frequency	dd	?
high_frequency	dd	?
root_frequency	dd	?
tune		dw	?
balance		db	?
envelope_rate	db	ENVELOPES dup (?)
envelope_offset	db	ENVELOPES dup (?)
tremolo_sweep	db	?
tremolo_rate	db	?
tremolo_depth	db	?
vibrato_sweep	db	?
vibrato_rate	db	?
vibrato_depth	db	?
modes		db	?
scale_frequency	dw	?
scale_factor	dw	?
reserved_w	db	PATCH_DATA_RESERVED_SIZE dup (?)

PatchData	ENDS


CommonData	STRUC

Control		db	?
NumbSamples	db	?
DataOffset	dw	?

CommonData	ENDS


;SampleData	STRUC
;
;_wave_start	dd	?
;_wave_end	dd	?
;_start_loop	dd	?
;_end_loop	dd	?
;_sample_rate	dw	?
;_low_frequency	dd	?
;_high_frequency	dd	?
;_root_frequency	dd	?
;_balance	db	?
;_envelope_rate	db	ENVELOPES dup (?)
;_envelope_offset db	ENVELOPES dup (?)
;_tremolo_sweep	db	?
;_tremolo_rate	db	?
;_tremolo_depth	db	?
;_vibrato_sweep	db	?
;_vibrato_rate	db	?
;_vibrato_depth	db	?
;_modes		db	?
;_scale_frequency dw	?
;_scale_factor	dw	?
;
;SampleData	ENDS

wave_struct	STRUC
	_start_loop	dd	?
	_end_loop	dd	?
	_low_frequency	dd	?
	_high_frequency	dd	?
        _root_frequency	dd	?
        _mem		dd	?
	_scale_frequency dw	?
	_sample_rate	dw	?
        _scale_factor	dw	?
        _start_acc_low	dw	?
        _start_acc_high	dw	?
        _start_low	dw	?
        _start_high	dw	?
	_end_low	dw	?
        _end_high	dw	?
        _end_acc_low	dw	?
	_end_acc_high	dw	?
        _sample_ratio	dw	?
        _wave_size	dd	?
        _fractions	db	?
        _balance	db	?
        _envelope_rate	db	ENVELOPES dup (?)
        _envelope_offset db	ENVELOPES dup (?)
        _tremolo_sweep	db	?
        _tremolo_rate	db	?
        _tremolo_depth	db	?
        _vibrato_sweep	db	?
        _vibrato_rate	db	?
        _vibrato_depth	db	?
        _modes		db	?
wave_struct	ENDS

