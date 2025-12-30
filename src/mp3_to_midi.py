"""
mp3_to_midi.py
Convert MP3 audio file to a MIDI file (monophonic melody).
"""

import librosa
import numpy as np
import pretty_midi

# Load MP3 file
audio_file = "egypt.mp3"
y, sr = librosa.load(audio_file, sr=None)

# Pitch tracking using librosa
pitches, magnitudes = librosa.piptrack(y=y, sr=sr)

# Create MIDI object
midi = pretty_midi.PrettyMIDI()
instrument = pretty_midi.Instrument(program=pretty_midi.instrument_name_to_program("Acoustic Grand Piano"))

time_step = 0.02  # seconds per frame
current_time = 0.0

for t in range(pitches.shape[1]):
    pitch_slice = pitches[:, t]
    mag_slice = magnitudes[:, t]

    if mag_slice.max() > 0:
        pitch = pitch_slice[mag_slice.argmax()]
        if pitch > 0:
            midi_note = int(librosa.hz_to_midi(pitch))
            note = pretty_midi.Note(
                velocity=100,
                pitch=midi_note,
                start=current_time,
                end=current_time + time_step
            )
            instrument.notes.append(note)

    current_time += time_step

midi.instruments.append(instrument)
midi.write("anthem.mid")

print("MIDI file generated: anthem.mid")