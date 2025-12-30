from mido import MidiFile, Message, MetaMessage

# Load MIDI file
mid = MidiFile("egypt.mid")

notes = []
durations = []

ticks_per_beat = mid.ticks_per_beat
tempo = 500000  # default microseconds per beat

note_start_time = {}

current_time = 0

for track in mid.tracks:
    current_time = 0
    for msg in track:
        current_time += msg.time
        if msg.type == 'set_tempo':
            tempo = msg.tempo
        elif msg.type == 'note_on' and msg.velocity > 0:
            note_start_time[msg.note] = current_time
        elif (msg.type == 'note_off') or (msg.type == 'note_on' and msg.velocity == 0):
            if msg.note in note_start_time:
                start = note_start_time.pop(msg.note)
                duration_sec = (current_time - start) * tempo / (ticks_per_beat * 1_000_000)
                duration_ms = int(duration_sec * 1000)
                notes.append(msg.note)
                durations.append(duration_ms)

# Generate C code
print("const int melody[] = {", ", ".join(map(str, notes)), "};")
print("const int durations[] = {", ", ".join(map(str, durations)), "};")
print(f"const size_t MELODY_LEN = {len(notes)};")