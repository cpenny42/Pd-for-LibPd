//
//  soundfonts.c
//
//
//  Created by Chris Penny on 2/23/15.
//  Copyright (c) 2015 Comp150. All rights reserved.
//

#include "m_pd.h"
#include "fluidsynth.h"
#include <string.h>
#include <math.h>

enum MIDI_CC {
    BANK_SELECT_MSB = 0x00,
    MODULATION_MSB = 0x01,
    BREATH_MSB = 0x02,
    FOOT_MSB = 0x04,
    PORTAMENTO_TIME_MSB = 0x05,
    DATA_ENTRY_MSB = 0x06,
    VOLUME_MSB = 0x07,
    BALANCE_MSB = 0x08,
    PAN_MSB = 0x0A,
    EXPRESSION_MSB = 0x0B,
    EFFECTS1_MSB = 0x0C,
    EFFECTS2_MSB = 0x0D,
    GPC1_MSB = 0x10, /* general purpose controller */
    GPC2_MSB = 0x11,
    GPC3_MSB = 0x12,
    GPC4_MSB = 0x13,
    BANK_SELECT_LSB = 0x20,
    MODULATION_WHEEL_LSB = 0x21,
    BREATH_LSB = 0x22,
    FOOT_LSB = 0x24,
    PORTAMENTO_TIME_LSB = 0x25,
    DATA_ENTRY_LSB = 0x26,
    VOLUME_LSB = 0x27,
    BALANCE_LSB = 0x28,
    PAN_LSB = 0x2A,
    EXPRESSION_LSB = 0x2B,
    EFFECTS1_LSB = 0x2C,
    EFFECTS2_LSB = 0x2D,
    GPC1_LSB = 0x30,
    GPC2_LSB = 0x31,
    GPC3_LSB = 0x32,
    GPC4_LSB = 0x33,
    SUSTAIN_SWITCH = 0x40,
    PORTAMENTO_SWITCH = 0x41,
    SOSTENUTO_SWITCH = 0x42,
    SOFT_PEDAL_SWITCH = 0x43,
    LEGATO_SWITCH = 0x45,
    HOLD2_SWITCH = 0x45,
    SOUND_CTRL1 = 0x46,
    SOUND_CTRL2 = 0x47,
    SOUND_CTRL3 = 0x48,
    SOUND_CTRL4 = 0x49,
    SOUND_CTRL5 = 0x4A,
    SOUND_CTRL6 = 0x4B,
    SOUND_CTRL7 = 0x4C,
    SOUND_CTRL8 = 0x4D,
    SOUND_CTRL9 = 0x4E,
    SOUND_CTRL10 = 0x4F,
    GPC5 = 0x50,
    GPC6 = 0x51,
    GPC7 = 0x52,
    GPC8 = 0x53,
    PORTAMENTO_CTRL = 0x54,
    EFFECTS_DEPTH1 = 0x5B,
    EFFECTS_DEPTH2 = 0x5C,
    EFFECTS_DEPTH3 = 0x5D,
    EFFECTS_DEPTH4 = 0x5E,
    EFFECTS_DEPTH5 = 0x5F,
    DATA_ENTRY_INCR = 0x60,
    DATA_ENTRY_DECR = 0x61,
    NRPN_LSB = 0x62,
    NRPN_MSB = 0x63,
    RPN_LSB = 0x64,
    RPN_MSB = 0x65,
    ALL_SOUND_OFF = 0x78,
    ALL_CTRL_OFF = 0x79,
    LOCAL_CONTROL = 0x7A,
    ALL_NOTES_OFF = 0x7B,
    OMNI_OFF = 0x7C,
    OMNI_ON = 0x7D,
    POLY_OFF = 0x7E,
    POLY_ON = 0x7F
};

enum SOUNDFONTS_NRPN {
    START_ADDRS_OFFSET = 0,
    END_ADDRS_OFFSET = 1,
    STARTLOOP_ADDRS_OFFSET = 2,
    ENDLOOP_ADDRS_OFFSET = 3,
    START_ADDRS_COARSE_OFFSET = 4,
    MOD_LFO_TO_PITCH = 5,
    VIB_LFO_TO_PITCH = 6,
    MOD_ENV_TO_PITCH = 7,
    INITIAL_FILTER_FC = 8,
    INITIAL_FILTER_Q = 9,
    MOD_LFO_TO_FILTER_FC = 10,
    MOD_ENV_TO_FILTER_FC = 11,
    END_ADDRS_COARSE_OFFSET = 12,
    MOD_LFO_TO_VOLUME = 13,
    CHORUS_EFFECTS_SEND = 15,
    REVERB_EFFECTS_SEND = 16,
    PAN = 17,
    DELAY_MOD_LFO = 21,
    FREQ_MOD_LFO = 22,
    DELAY_VIB_LFO = 23,
    FREQ_VIB_LFO = 24,
    DELAY_MOD_ENV = 25,
    ATTACK_MOD_ENV = 26,
    HOLD_MOD_ENV = 27,
    DECAY_MOD_ENV = 28,
    SUSTAIN_MOD_ENV = 29,
    RELEASE_MOD_ENV = 30,
    KEYNUM_TO_MOD_ENV_HOLD = 31,
    KEYNUM_TO_MOD_ENV_DECAY = 32,
    DELAY_VOL_ENV = 33,
    ATTACK_VOL_ENV = 34,
    HOLD_VOL_ENV = 35,
    DECAY_VOL_ENV = 36,
    SUSTAIN_VOL_ENV = 37,
    RELEASE_VOL_ENV = 38,
    KEYNUM_TO_VOL_ENV_HOLD = 39,
    KEYNUM_TO_VOL_ENV_DECAY = 40,
    KEYRANGE = 43,
    VELRANGE = 44,
    STARTLOOP_ADDRS_COARSE_OFFSET = 45,
    KEYNUM = 46,
    VELOCITY = 47,
    INITIAL_ATTENUATION = 48,
    ENDLOOP_ADDRS_COARSE_OFFSET = 50,
    COARSE_TUNE = 51,
    FINE_TUNE = 52,
    SAMPLE_MODES = 54,
    SCALE_TUNING = 56,
    EXCLUSIVE_CLASS = 57,
    OVERRIDING_ROOT_KEY = 58
};

static t_class *soundfonts_class;

typedef struct _soundfonts {
    t_object x_obj;
    
    fluid_settings_t *settings;
    fluid_synth_t *synth;
    
    t_int chorus_nr;
    t_int chorus_level;
    t_int chorus_speed;
    t_int chorus_depth;
    t_int chorus_type;
    
    t_int reverb_size;
    t_int reverb_damping;
    t_int reverb_width;
    t_int reverb_level;
    
    t_int sfont_id;
    t_int polyphony;
    
    // Outlets
    t_outlet *x_outL;
    t_outlet *x_outR;
    
} t_soundfonts;

void *soundfonts_new(void)
{
    // Get Class Instance
    t_soundfonts *instance = (t_soundfonts *)pd_new(soundfonts_class);

    instance->settings = new_fluid_settings();
    instance->synth = new_fluid_synth(instance->settings);
    instance->sfont_id = -1;
    
    instance->chorus_nr = FLUID_CHORUS_DEFAULT_N;
    instance->chorus_level = FLUID_CHORUS_DEFAULT_LEVEL;
    instance->chorus_depth = FLUID_CHORUS_DEFAULT_DEPTH;
    instance->chorus_speed = FLUID_CHORUS_DEFAULT_SPEED;
    instance->chorus_type = FLUID_CHORUS_DEFAULT_TYPE;
    
    instance->reverb_damping = FLUID_REVERB_DEFAULT_DAMP;
    instance->reverb_level = FLUID_REVERB_DEFAULT_LEVEL;
    instance->reverb_size = FLUID_REVERB_DEFAULT_ROOMSIZE;
    instance->reverb_width = FLUID_REVERB_DEFAULT_WIDTH;
    
    // Spawn outlets
    instance->x_outL = outlet_new(&instance->x_obj, &s_signal);
    instance->x_outR = outlet_new(&instance->x_obj, &s_signal);
    
    return (void *)instance;
}

void soundfonts_free(t_soundfonts *instance)
{
    delete_fluid_settings(instance->settings);
    delete_fluid_synth(instance->synth);

    outlet_free(instance->x_outL);
    outlet_free(instance->x_outR);
}

t_int *soundfonts_perform(t_int *input)
{
    t_soundfonts *instance = (t_soundfonts *)(input[1]);
    t_sample *outL = (t_sample *)(input[2]);
    t_sample *outR = (t_sample *)(input[3]);
    int n = (int)(input[4]);
    
    fluid_synth_write_float(instance->synth, n, outL, 0, 1, outR, 0, 1);
    
    return input + 5;
}

void soundfonts_dsp(t_soundfonts *instance, t_signal **sp)
{
    dsp_add(soundfonts_perform, 4, instance,
            sp[0]->s_vec, sp[1]->s_vec, sp[0]->s_n);
}

void soundfonts_set(t_soundfonts *instance, t_symbol *soundfont)
{
    if ((instance != NULL) && (instance->synth != NULL)) {
//        post("setting to: %s", soundfont->s_name);
        instance->sfont_id = fluid_synth_sfload(instance->synth, soundfont->s_name, 1);
    } else {
        post("Unable to set soundfont due to error.");
    }
}

void soundfonts_note(t_soundfonts *instance, t_symbol *s, int argc, t_atom *argv)
{
    if ((argc < 2) || (instance->synth == NULL)) {
        post("soundfonts error: need 2 values to play note (note, velocity pair).");
        return;
    }
    
    float note = atom_getfloatarg(0, argc, argv);
    float velocity = atom_getfloatarg(1, argc, argv);
    
    if ( (note >= 0) && (note <= 127) && (velocity >= 0) && (velocity <= 127) ) {

        fluid_synth_noteon(instance->synth, 0, note, velocity);
        
    }
}

void soundfonts_polyphony(t_soundfonts *instance, t_floatarg poly)
{
    instance->polyphony = poly;
    fluid_synth_set_polyphony(instance->synth, poly);
}

void soundfonts_pitchbend(t_soundfonts *instance, t_floatarg bend)
{
    if ((bend <= 127) && (bend >= 0) ){
        // Fluidsynth pitchbend values go from 0 to 16383
        fluid_synth_pitch_bend(instance->synth, 0, bend * 129);
    }
}

void soundfonts_pitchbend_range(t_soundfonts *instance, t_floatarg range)
{
    if ((range <= 72) && (range >= 0)) {
        fluid_synth_pitch_wheel_sens(instance->synth, 0, range);
    }
}

void soundfonts_cc(t_soundfonts *instance, t_floatarg control, t_floatarg val)
{
    
    if((control <= 127) && (control >= 0) && (val <= 127) && (val >= 0))
    {
        fluid_synth_cc(instance->synth, 0, control, val);
    } else {
        post( "overflow - %f %f", control, val);
    }
}

void soundfonts_nrpn(t_soundfonts *instance, t_floatarg select, t_floatarg MSB, t_floatarg LSB)
{
    soundfonts_cc(instance, NRPN_MSB, 120);
    soundfonts_cc(instance, NRPN_LSB, select);
    soundfonts_cc(instance, DATA_ENTRY_LSB, LSB);
    soundfonts_cc(instance, DATA_ENTRY_MSB, MSB);
}

void soundfonts_nrpn_extract_msb_lsb(t_soundfonts *instance, int selector, t_floatarg amount)
{
    int scaled = (amount / 128) * 16384;
    
    int msb = scaled >> 7;
    int lsb = scaled & 127;
    soundfonts_nrpn(instance, selector, msb, lsb);
}

float_t soundfonts_scale_time(t_floatarg time_in_ms)
{
    return ( (( 1200 * (log(time_in_ms / 1000)/log(2)) ) / 16384) * 64 ) + 64;
}

float_t soundfonts_scale_freq(t_floatarg freq_in_hz)
{
    freq_in_hz = fabsf(freq_in_hz);
    return ( (( 1200 * (log(freq_in_hz / (8.176 / M_E))/log(2)) ) / 16384) * 64 ) + 64;
}

float_t soundfonts_scale_power(t_floatarg power)
{
    if (power >= .9944505)
        power = .9944505;
    if (power < 0)
        power = 0;
    
    int split = 80;    
    power = (127 - (power * 127));
    float_t transform;
    if (power <= split) {
        transform = powf(power / split, 2) * 1000;
    } else {
        transform = powf((power - split) / (127 - split), 5) * 4000 + 1000;
    }
    
    return transform;
}

void soundfonts_flush(t_soundfonts *instance)
{
    soundfonts_cc(instance, ALL_SOUND_OFF, 0);
}

void soundfonts_pan(t_soundfonts *instance, t_floatarg pan)
{
    soundfonts_nrpn(instance, PAN, pan, 0);
}

void soundfonts_volume(t_soundfonts *instance, t_floatarg vol)
{
    soundfonts_cc(instance, VOLUME_MSB, vol * 127);
}

void soundfonts_sustain(t_soundfonts *instance, t_floatarg sustain)
{
    soundfonts_cc(instance, SUSTAIN_SWITCH, (sustain >= 1) * 127);
}

void soundfonts_mod(t_soundfonts *instance, t_floatarg val)
{
    soundfonts_cc(instance, MODULATION_MSB, val);
}

void soundfonts_vol_delay(t_soundfonts *instance, t_floatarg delay)
{
    soundfonts_nrpn_extract_msb_lsb(instance, DELAY_VOL_ENV, soundfonts_scale_time(delay));
}

void soundfonts_vol_attack(t_soundfonts *instance, t_floatarg attack)
{
    soundfonts_nrpn_extract_msb_lsb(instance, ATTACK_VOL_ENV, soundfonts_scale_time(attack));
}

void soundfonts_vol_hold(t_soundfonts *instance, t_floatarg hold)
{
    soundfonts_nrpn_extract_msb_lsb(instance, HOLD_VOL_ENV, soundfonts_scale_time(hold));
}

void soundfonts_vol_decay(t_soundfonts *instance, t_floatarg decay)
{
    soundfonts_nrpn_extract_msb_lsb(instance, DECAY_VOL_ENV, soundfonts_scale_time(decay));
}

void soundfonts_vol_sustain(t_soundfonts *instance, t_floatarg sustain)
{
    soundfonts_nrpn_extract_msb_lsb(instance, SUSTAIN_VOL_ENV, soundfonts_scale_time(soundfonts_scale_power(sustain)));
}

void soundfonts_vol_release(t_soundfonts *instance, t_floatarg release)
{
    soundfonts_nrpn_extract_msb_lsb(instance, RELEASE_VOL_ENV, soundfonts_scale_time(release));
}

void soundfonts_mod_lfo_pitch(t_soundfonts *instance, t_floatarg amount)
{
    amount = (amount / 2.0) + 64;
    soundfonts_nrpn_extract_msb_lsb(instance, MOD_LFO_TO_PITCH, amount);
}

void soundfonts_mod_lfo_filter(t_soundfonts *instance, t_floatarg amount)
{
    amount = (amount / 2.0) + 64;
    soundfonts_nrpn_extract_msb_lsb(instance, MOD_LFO_TO_FILTER_FC, amount);
}

void soundfonts_mod_lfo_volume(t_soundfonts *instance, t_floatarg amount)
{
    amount = (amount / 2.0) + 64;
    soundfonts_nrpn_extract_msb_lsb(instance, MOD_LFO_TO_VOLUME, amount);
}

void soundfonts_mod_lfo_delay(t_soundfonts *instance, t_floatarg amount)
{
    soundfonts_nrpn_extract_msb_lsb(instance, DELAY_MOD_LFO, soundfonts_scale_time(amount));
}

void soundfonts_mod_lfo_freq(t_soundfonts *instance, t_floatarg amount)
{
    soundfonts_nrpn_extract_msb_lsb(instance, FREQ_MOD_LFO, soundfonts_scale_freq(amount));
}

void soundfonts_mod_env_pitch(t_soundfonts *instance, t_floatarg amount)
{
    amount = (amount / 2.0) + 64;
    soundfonts_nrpn_extract_msb_lsb(instance, MOD_ENV_TO_PITCH, amount);
}

void soundfonts_mod_env_filter(t_soundfonts *instance, t_floatarg amount)
{
    amount = (amount / 2.0) + 64;
    soundfonts_nrpn_extract_msb_lsb(instance, MOD_ENV_TO_FILTER_FC, amount);
}

void soundfonts_mod_env_delay(t_soundfonts *instance, t_floatarg delay)
{
    soundfonts_nrpn_extract_msb_lsb(instance, DELAY_MOD_ENV, soundfonts_scale_time(delay));
}

void soundfonts_mod_env_attack(t_soundfonts *instance, t_floatarg attack)
{
    soundfonts_nrpn_extract_msb_lsb(instance, ATTACK_MOD_ENV, soundfonts_scale_time(attack));
}

void soundfonts_mod_env_hold(t_soundfonts *instance, t_floatarg hold)
{
    soundfonts_nrpn_extract_msb_lsb(instance, HOLD_MOD_ENV, soundfonts_scale_time(hold));
}

void soundfonts_mod_env_decay(t_soundfonts *instance, t_floatarg decay)
{
    soundfonts_nrpn_extract_msb_lsb(instance, DECAY_MOD_ENV, soundfonts_scale_time(decay));
}

void soundfonts_mod_env_sustain(t_soundfonts *instance, t_floatarg sustain)
{
    soundfonts_nrpn_extract_msb_lsb(instance, SUSTAIN_MOD_ENV, soundfonts_scale_time(soundfonts_scale_power(sustain)));
}

void soundfonts_mod_env_release(t_soundfonts *instance, t_floatarg release)
{
    soundfonts_nrpn_extract_msb_lsb(instance, RELEASE_MOD_ENV, soundfonts_scale_time(release));
}

void soundfonts_vib_lfo_pitch(t_soundfonts *instance, t_floatarg amount)
{
    amount = (amount / 2.0) + 64;
    soundfonts_nrpn_extract_msb_lsb(instance, VIB_LFO_TO_PITCH, amount);
}

void soundfonts_vib_lfo_delay(t_soundfonts *instance, t_floatarg amount)
{
    soundfonts_nrpn_extract_msb_lsb(instance, DELAY_VIB_LFO, amount);
}

void soundfonts_vib_lfo_freq(t_soundfonts *instance, t_floatarg amount)
{
    soundfonts_nrpn_extract_msb_lsb(instance, FREQ_VIB_LFO, soundfonts_scale_freq(amount));
}

void soundfonts_fc(t_soundfonts *instance, t_floatarg cutoff)
{
    soundfonts_nrpn_extract_msb_lsb(instance, INITIAL_FILTER_FC, cutoff);
}

void soundfonts_q(t_soundfonts *instance, t_floatarg q)
{
    soundfonts_nrpn_extract_msb_lsb(instance, INITIAL_FILTER_Q, soundfonts_scale_freq(q));
}

void soundfonts_vol_adsr(t_soundfonts *instance, t_floatarg a, t_floatarg d, t_floatarg s, t_floatarg r)
{
    soundfonts_vol_attack(instance, a);
    soundfonts_vol_decay(instance, d);
    soundfonts_vol_sustain(instance, s);
    soundfonts_vol_release(instance, r);
}

void soundfonts_mod_adsr(t_soundfonts *instance, t_floatarg a, t_floatarg d, t_floatarg s, t_floatarg r)
{
    soundfonts_mod_env_attack(instance, a);
    soundfonts_mod_env_decay(instance, d);
    soundfonts_mod_env_sustain(instance, s);
    soundfonts_mod_env_release(instance, r);
}

void soundfonts_coarse_tune(t_soundfonts *instance, t_floatarg tune)
{
    soundfonts_nrpn_extract_msb_lsb(instance, COARSE_TUNE, tune);
}

void soundfonts_fine_tune(t_soundfonts *instance, t_floatarg tune)
{
    soundfonts_nrpn_extract_msb_lsb(instance, FINE_TUNE, tune);
}

void soundfonts_velocity_range(t_soundfonts *instance, t_floatarg low, t_floatarg high)
{
    soundfonts_nrpn(instance, VELRANGE, low, high);
}

void soundfonts_key_range(t_soundfonts *instance, t_floatarg low, t_floatarg high)
{
    soundfonts_nrpn(instance, KEYRANGE, low, high);
}

void soundfonts_chorus_amount(t_soundfonts *instance, t_floatarg amount)
{
    soundfonts_cc(instance, EFFECTS_DEPTH3, amount);
//    soundfonts_nrpn_extract_msb_lsb(instance, CHORUS_EFFECTS_SEND, amount);
}

void soundfonts_chorus_level(t_soundfonts *instance, t_floatarg amount)
{
    fluid_synth_set_chorus(instance->synth, instance->chorus_nr, amount, instance->chorus_speed, instance->chorus_depth, instance->chorus_type);
    instance->chorus_level = amount;
}

void soundfonts_chorus_n(t_soundfonts *instance, t_floatarg amount)
{
    fluid_synth_set_chorus(instance->synth, amount, instance->chorus_level, instance->chorus_speed, instance->chorus_depth, instance->chorus_type);
    instance->chorus_nr = amount;
}

void soundfonts_chorus_speed(t_soundfonts *instance, t_floatarg amount)
{
    fluid_synth_set_chorus(instance->synth, instance->chorus_nr, instance->chorus_level, amount, instance->chorus_depth, instance->chorus_type);
    instance->chorus_speed = amount;
}

void soundfonts_chorus_depth(t_soundfonts *instance, t_floatarg amount)
{
    fluid_synth_set_chorus(instance->synth, instance->chorus_nr, instance->chorus_level, instance->chorus_speed, amount, instance->chorus_type);
    instance->chorus_depth = amount;
}

void soundfonts_chorus_type(t_soundfonts *instance, t_floatarg type)
{
    fluid_synth_set_chorus(instance->synth, instance->chorus_nr, instance->chorus_level, instance->chorus_speed, instance->chorus_depth, type);
    instance->chorus_type = type;
}

void soundfonts_reverb_amount(t_soundfonts *instance, t_floatarg amount)
{
    soundfonts_cc(instance, EFFECTS_DEPTH1, amount);
//    soundfonts_nrpn_extract_msb_lsb(instance, REVERB_EFFECTS_SEND, amount);
}

void soundfonts_reverb_size(t_soundfonts *instance, t_floatarg amount)
{
    fluid_synth_set_reverb(instance->synth, amount, instance->reverb_damping, instance->reverb_width, instance->reverb_level);
    instance->reverb_size = amount;
}

void soundfonts_reverb_damping(t_soundfonts *instance, t_floatarg amount)
{
    fluid_synth_set_reverb(instance->synth, instance->reverb_size, amount, instance->reverb_width, instance->reverb_level);
    instance->reverb_damping = amount;
}

void soundfonts_reverb_width(t_soundfonts *instance, t_floatarg amount)
{
    fluid_synth_set_reverb(instance->synth, instance->reverb_size, instance->reverb_damping, amount, instance->reverb_level);
    instance->reverb_width = amount;
}

void soundfonts_reverb_level(t_soundfonts *instance, t_floatarg amount)
{
    fluid_synth_set_reverb(instance->synth, instance->reverb_size, instance->reverb_damping, instance->reverb_width, amount);
    instance->reverb_level = amount;
}


void soundfonts_setup(void)
{
    soundfonts_class = class_new(gensym("soundfonts"),
                                       (t_newmethod)soundfonts_new,
                                       0,
                                       sizeof(t_soundfonts),
                                       CLASS_DEFAULT,
                                       0);
    
    class_addmethod(soundfonts_class, (t_method)soundfonts_dsp, gensym("dsp"), 0);
    class_addmethod(soundfonts_class, (t_method)soundfonts_set, gensym("set"), A_DEFSYMBOL, 0);
    
    class_addmethod(soundfonts_class, (t_method)soundfonts_polyphony, gensym("polyphony"), A_FLOAT, 0);
    class_addmethod(soundfonts_class, (t_method)soundfonts_cc, gensym("cc"), A_FLOAT, A_FLOAT, 0);
    class_addmethod(soundfonts_class, (t_method)soundfonts_nrpn, gensym("nrpn"), A_FLOAT, A_FLOAT, A_FLOAT, 0);

    class_addmethod(soundfonts_class, (t_method)soundfonts_flush, gensym("flush"), 0);
    class_addmethod(soundfonts_class, (t_method)soundfonts_sustain, gensym("sustain"), A_FLOAT, 0);
    class_addmethod(soundfonts_class, (t_method)soundfonts_volume, gensym("volume"), A_FLOAT, 0);
    class_addmethod(soundfonts_class, (t_method)soundfonts_pan, gensym("pan"), A_FLOAT, 0);
    class_addmethod(soundfonts_class, (t_method)soundfonts_mod, gensym("mod"), A_FLOAT, 0);
    
    class_addmethod(soundfonts_class, (t_method)soundfonts_vol_delay, gensym("volume_delay"), A_FLOAT, 0);
    class_addmethod(soundfonts_class, (t_method)soundfonts_vol_attack, gensym("volume_attack"), A_FLOAT, 0);
    class_addmethod(soundfonts_class, (t_method)soundfonts_vol_hold, gensym("volume_hold"), A_FLOAT, 0);
    class_addmethod(soundfonts_class, (t_method)soundfonts_vol_decay, gensym("volume_decay"), A_FLOAT, 0);
    class_addmethod(soundfonts_class, (t_method)soundfonts_vol_sustain, gensym("volume_sustain"), A_FLOAT, 0);
    class_addmethod(soundfonts_class, (t_method)soundfonts_vol_release, gensym("volume_release"), A_FLOAT, 0);
    
    class_addmethod(soundfonts_class, (t_method)soundfonts_mod_lfo_pitch, gensym("mod_lfo_pitch"), A_FLOAT, 0);
    class_addmethod(soundfonts_class, (t_method)soundfonts_mod_lfo_filter, gensym("mod_lfo_filter"), A_FLOAT, 0);
    class_addmethod(soundfonts_class, (t_method)soundfonts_mod_lfo_volume, gensym("mod_lfo_volume"), A_FLOAT, 0);
    class_addmethod(soundfonts_class, (t_method)soundfonts_mod_lfo_delay, gensym("mod_lfo_delay"), A_FLOAT, 0);
    class_addmethod(soundfonts_class, (t_method)soundfonts_mod_lfo_freq, gensym("mod_lfo_frequency"), A_FLOAT, 0);
  
    class_addmethod(soundfonts_class, (t_method)soundfonts_mod_env_pitch, gensym("mod_env_pitch"), A_FLOAT, 0);
    class_addmethod(soundfonts_class, (t_method)soundfonts_mod_env_filter, gensym("mod_env_filter"), A_FLOAT, 0);
    class_addmethod(soundfonts_class, (t_method)soundfonts_mod_env_delay, gensym("mod_env_delay"), A_FLOAT, 0);
    class_addmethod(soundfonts_class, (t_method)soundfonts_mod_env_attack, gensym("mod_env_attack"), A_FLOAT, 0);
    class_addmethod(soundfonts_class, (t_method)soundfonts_mod_env_hold, gensym("mod_env_hold"), A_FLOAT, 0);
    class_addmethod(soundfonts_class, (t_method)soundfonts_mod_env_decay, gensym("mod_env_decay"), A_FLOAT, 0);
    class_addmethod(soundfonts_class, (t_method)soundfonts_mod_env_sustain, gensym("mod_env_sustain"), A_FLOAT, 0);
    class_addmethod(soundfonts_class, (t_method)soundfonts_mod_env_release, gensym("mod_env_release"), A_FLOAT, 0);
    
    class_addmethod(soundfonts_class, (t_method)soundfonts_vib_lfo_delay, gensym("vib_lfo_delay"), A_FLOAT, 0);
    class_addmethod(soundfonts_class, (t_method)soundfonts_vib_lfo_freq, gensym("vib_lfo_frequency"), A_FLOAT, 0);
    class_addmethod(soundfonts_class, (t_method)soundfonts_vib_lfo_pitch, gensym("vib_lfo_amount"), A_FLOAT, 0);
    
    class_addmethod(soundfonts_class, (t_method)soundfonts_fc, gensym("filter_frequency"), A_FLOAT, 0);
    class_addmethod(soundfonts_class, (t_method)soundfonts_q, gensym("filter_q"), A_FLOAT, 0);
    
    class_addmethod(soundfonts_class, (t_method)soundfonts_vol_adsr, gensym("volume_adsr"), A_FLOAT, A_FLOAT, A_FLOAT, A_FLOAT, 0);
    class_addmethod(soundfonts_class, (t_method)soundfonts_mod_adsr, gensym("mod_adsr"), A_FLOAT, A_FLOAT, A_FLOAT, A_FLOAT, 0);
    
    class_addmethod(soundfonts_class, (t_method)soundfonts_coarse_tune, gensym("coarse_tune"), A_FLOAT, 0);
    class_addmethod(soundfonts_class, (t_method)soundfonts_fine_tune, gensym("fine_tune"), A_FLOAT, 0);
    class_addmethod(soundfonts_class, (t_method)soundfonts_velocity_range, gensym("velocity_range"), A_FLOAT, A_FLOAT, 0);
    class_addmethod(soundfonts_class, (t_method)soundfonts_key_range, gensym("key_range"), A_FLOAT, A_FLOAT, 0);
    
    class_addmethod(soundfonts_class, (t_method)soundfonts_chorus_amount, gensym("chorus_amount"), A_FLOAT, 0);
    class_addmethod(soundfonts_class, (t_method)soundfonts_chorus_depth, gensym("chorus_depth"), A_FLOAT, 0);
    class_addmethod(soundfonts_class, (t_method)soundfonts_chorus_level, gensym("chorus_level"), A_FLOAT, 0);
    class_addmethod(soundfonts_class, (t_method)soundfonts_chorus_speed, gensym("chorus_speed"), A_FLOAT, 0);
    class_addmethod(soundfonts_class, (t_method)soundfonts_chorus_n, gensym("chorus_n"), A_FLOAT, 0);
    class_addmethod(soundfonts_class, (t_method)soundfonts_chorus_type, gensym("chorus_type"), A_FLOAT, 0);
    
    class_addmethod(soundfonts_class, (t_method)soundfonts_reverb_amount, gensym("reverb_amount"), A_FLOAT, 0);
    class_addmethod(soundfonts_class, (t_method)soundfonts_reverb_damping, gensym("reverb_damping"), A_FLOAT, 0);
    class_addmethod(soundfonts_class, (t_method)soundfonts_reverb_level, gensym("reverb_level"), A_FLOAT, 0);
    class_addmethod(soundfonts_class, (t_method)soundfonts_reverb_size, gensym("reverb_size"), A_FLOAT, 0);
    class_addmethod(soundfonts_class, (t_method)soundfonts_reverb_width, gensym("reverb_width"), A_FLOAT, 0);
    
    class_addmethod(soundfonts_class, (t_method)soundfonts_pitchbend, gensym("pitchbend"), A_FLOAT, 0);
    class_addmethod(soundfonts_class, (t_method)soundfonts_pitchbend_range, gensym("pitchbend_range"), A_FLOAT, 0);
    
    
    class_addlist(soundfonts_class, soundfonts_note);
}

