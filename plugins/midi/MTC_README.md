# MIDI MTC (MIDI Time Code) Support in QLC+

This document describes the new MIDI MTC support added to QLC+ for perfect synchronization between music and lights.

## Overview

MIDI MTC (MIDI Time Code) is a standard protocol that allows precise timing synchronization between audio/video equipment and lighting systems. This implementation enables QLC+ to receive MTC timecode from external sources (like DAWs, video editors, or dedicated timecode generators) and use it to drive show timing with frame-accurate precision.

## Features

- **Real-time MTC parsing**: Receives and parses MIDI MTC Quarter Frame messages
- **Multiple frame rate support**: Supports 24fps (film), 25fps (PAL), 30fps (NTSC), and 30fps drop frame
- **Automatic frame rate detection**: Automatically detects frame rate from incoming MTC data
- **BPM calculation**: Converts timecode to BPM for musical synchronization
- **Show integration**: Integrates with QLC+ show timing system
- **Cross-platform support**: Works on Windows, macOS, and Linux

## How It Works

### MTC Message Format

MIDI MTC uses Quarter Frame messages to transmit timecode data. Each complete timecode requires 8 Quarter Frame messages:

1. **Frame LSB** (bits 0-3)
2. **Frame MSB** (bits 4-5)
3. **Seconds LSB** (bits 0-3)
4. **Seconds MSB** (bits 4-5)
5. **Minutes LSB** (bits 0-3)
6. **Minutes MSB** (bits 4-5)
7. **Hours LSB** (bits 0-3)
8. **Hours MSB** (bits 4) + **SMPTE Type** (bits 5-6) + **Frame Rate** (bit 7)

### Integration with QLC+ Timing

The MTC system integrates with QLC+'s existing beat/timing infrastructure:

- MTC timecode is converted to milliseconds
- BPM is calculated from timecode changes
- Timing signals are emitted through the existing beat system
- Shows can be synchronized to external MTC sources

## Setup Instructions

### 1. Enable MTC on MIDI Input Device

1. Open QLC+ and go to **Input/Output** → **MIDI**
2. Select your MIDI input device
3. Check the **MTC** column and set it to **Enabled**
4. The device will now listen for MTC messages

### 2. Configure External MTC Source

Configure your DAW, video editor, or timecode generator to:
- Send MIDI MTC on the same MIDI port as QLC+
- Use the appropriate frame rate for your project
- Ensure MTC is enabled and running

### 3. Use in Shows

Once MTC is enabled and receiving timecode:

- **Show timing**: Shows will automatically sync to incoming MTC
- **Beat synchronization**: MTC-derived BPM will drive beat-based functions
- **Precise timing**: Frame-accurate synchronization for video/audio projects

## Technical Details

### MTC TimeCode Structure

```cpp
struct TimeCode
{
    int hours;           // 0-23
    int minutes;         // 0-59
    int seconds;         // 0-59
    int frames;          // 0-29 (depends on frame rate)
    FrameRate frameRate; // FPS_24, FPS_25, FPS_30, FPS_30DF
};
```

### Frame Rate Support

- **FPS_24**: Film standard (24 frames per second)
- **FPS_25**: PAL video standard (25 frames per second)
- **FPS_30**: NTSC video standard (30 frames per second)
- **FPS_30DF**: NTSC drop frame (30 frames per second, drop frame)

### BPM Calculation

The system calculates BPM from timecode changes assuming a 4/4 time signature. This provides musical synchronization for beat-based lighting effects.

## Troubleshooting

### Common Issues

1. **No MTC received**: Check that MTC is enabled on the MIDI input device
2. **Wrong frame rate**: Verify frame rate settings in your external MTC source
3. **Timing drift**: Ensure your MTC source is stable and accurate
4. **MIDI port conflicts**: Make sure only one application is using the MIDI port

### Debug Information

Enable debug output to see MTC parsing details:
- MTC Quarter Frame messages received
- Timecode updates
- Frame rate detection
- BPM calculations

## Use Cases

### Live Performance
- Synchronize lights with backing tracks
- Perfect timing for video projections
- Automated lighting sequences driven by music

### Studio/Recording
- Synchronize lighting with video recordings
- Precise timing for music videos
- Automated lighting for content creation

### Theater/Events
- Synchronize lighting with video content
- Automated sequences with external timing
- Multi-media performances

## Future Enhancements

Potential improvements for future versions:
- MTC output capability
- More sophisticated BPM detection
- Support for additional timecode formats
- Enhanced show timeline integration
- Real-time MTC monitoring interface

## Contributing

This MTC implementation is open source. Contributions are welcome:
- Bug reports and feature requests
- Code improvements and optimizations
- Documentation updates
- Testing on different platforms

## License

This implementation follows the same Apache 2.0 license as QLC+.
