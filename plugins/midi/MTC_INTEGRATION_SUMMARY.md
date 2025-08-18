# MIDI MTC Integration with Show Editor - Implementation Summary

This document summarizes the complete implementation of MIDI MTC (MIDI Time Code) integration with the QLC+ show editor for timeline synchronization.

## Overview

The implementation provides seamless integration between external MIDI MTC timecode sources and QLC+ shows, enabling perfect synchronization between music, video, and lighting.

## Architecture

### 1. Core MTC Parser (`mtctimecode.h/cpp`)
- **MTCTimeCode class**: Handles parsing of MIDI MTC Quarter Frame messages
- **TimeCode struct**: Represents timecode with hours, minutes, seconds, frames, and frame rate
- **Frame rate support**: 24fps, 25fps, 30fps, 30fps drop frame
- **Automatic detection**: Frame rate and timecode parsing
- **BPM calculation**: Converts timecode to musical tempo

### 2. MIDI Plugin Integration
- **Enhanced MIDI input devices**: Support for MTC messages across all platforms (ALSA, Windows, macOS)
- **MTC signal processing**: Converts MTC timecode to QLC+ input signals
- **Beat system integration**: MTC timing integrates with existing QLC+ beat infrastructure
- **Configuration interface**: MTC enable/disable per MIDI input device

### 3. Show System Integration
- **New timing mode**: Added `Show::MTC` to the time division system
- **MTC-enabled shows**: Shows can be configured to use MTC timing
- **Real-time synchronization**: Show cursor position updates based on incoming MTC
- **BPM synchronization**: MTC-derived BPM drives show timing

### 4. Show Editor Integration
- **MTC timing mode**: Added to time division combo box
- **MTC status display**: Real-time MTC status and timecode display
- **MTC monitor widget**: Dedicated MTC monitoring interface in show editor
- **Timeline synchronization**: Show timeline automatically syncs to MTC

## Implementation Details

### MIDI MTC Message Flow
```
External MTC Source → MIDI Input Device → MTC Parser → Show Manager → Show Timeline
```

1. **MIDI Input**: Receives MTC Quarter Frame messages
2. **MTC Parsing**: Assembles complete timecode from 8 quarter frames
3. **Signal Conversion**: Converts timecode to QLC+ input signals
4. **Show Integration**: Updates show cursor position and timing
5. **Timeline Sync**: Show timeline follows external MTC source

### Show Timing Modes
- **Time**: Traditional millisecond-based timing
- **BPM 4/4**: Beat-based timing with 4/4 time signature
- **BPM 3/4**: Beat-based timing with 3/4 time signature  
- **BPM 2/4**: Beat-based timing with 2/4 time signature
- **MTC**: External MIDI timecode synchronization

### MTC Status Display
- **Status indicator**: Shows MTC On/Off status with color coding
- **Time display**: Real-time timecode display (HH:MM:SS:FF)
- **BPM display**: Calculated BPM from timecode changes
- **Sync button**: Manual synchronization trigger

## Usage Workflow

### 1. Enable MTC on MIDI Input
1. Open **Input/Output** → **MIDI**
2. Select MIDI input device
3. Set **MTC** column to **Enabled**
4. Device now listens for MTC messages

### 2. Configure Show for MTC
1. Open show in **Show Manager**
2. Set **Time division** to **MTC**
3. MTC is automatically enabled for the show
4. BPM field becomes active for MTC timing

### 3. Monitor MTC Status
1. **Show Editor**: MTC monitor widget shows real-time status
2. **Show Manager**: Toolbar displays MTC status and timecode
3. **Timeline**: Show cursor automatically follows MTC timecode

### 4. External MTC Source
1. Configure DAW/video editor to send MIDI MTC
2. Ensure MTC is enabled and running
3. QLC+ automatically syncs to incoming timecode
4. Perfect synchronization between audio/video and lighting

## Technical Features

### Cross-Platform Support
- **Linux (ALSA)**: Full MTC support via ALSA MIDI
- **Windows**: MTC support via Windows MIDI API
- **macOS**: MTC support via Core MIDI

### Real-Time Performance
- **Frame-accurate timing**: Sub-frame precision synchronization
- **Low latency**: Direct MIDI message processing
- **Automatic recovery**: Handles MTC interruptions gracefully

### Integration Points
- **Input/Output system**: MTC as external timing source
- **Beat system**: MTC-derived BPM drives beat timing
- **Show timing**: Direct timeline synchronization
- **Function timing**: All show functions sync to MTC

## Configuration Options

### MIDI Device Settings
- **MTC Enable/Disable**: Per-device MTC support
- **Channel mapping**: Automatic MTC signal routing
- **Frame rate detection**: Automatic frame rate detection

### Show Settings
- **Timing mode**: MTC timing mode selection
- **BPM synchronization**: Automatic BPM updates
- **Cursor sync**: Timeline cursor follows MTC

### Display Options
- **Status indicators**: Visual MTC status display
- **Time format**: Timecode display format
- **BPM display**: Real-time BPM calculation

## Benefits

### Professional Applications
- **Live performance**: Perfect sync with backing tracks
- **Video production**: Frame-accurate lighting sync
- **Broadcasting**: Live event synchronization
- **Theater**: Automated show timing

### Workflow Improvements
- **External timing**: Use professional timing sources
- **Automatic sync**: No manual timing adjustments
- **Real-time monitoring**: Live MTC status display
- **Seamless integration**: Works with existing QLC+ features

## Future Enhancements

### Potential Improvements
- **MTC output**: Generate MTC timecode
- **Advanced BPM detection**: More sophisticated tempo analysis
- **Multiple MTC sources**: Support for multiple timing sources
- **Enhanced monitoring**: Advanced MTC diagnostics
- **Network MTC**: Network-based timecode support

### Integration Opportunities
- **Video timeline**: Direct video timeline integration
- **Audio analysis**: Audio-based timing detection
- **Network protocols**: OSC, Art-Net timing support
- **Hardware integration**: Dedicated MTC hardware support

## Conclusion

The MIDI MTC integration provides a robust, professional-grade timing solution for QLC+ shows. It enables perfect synchronization between external timing sources and lighting systems, making QLC+ suitable for professional applications requiring precise timing control.

The implementation is designed to be:
- **Non-intrusive**: Works alongside existing timing systems
- **User-friendly**: Simple configuration and monitoring
- **Reliable**: Robust error handling and recovery
- **Extensible**: Foundation for future timing enhancements

This integration positions QLC+ as a professional lighting control solution capable of handling complex, time-critical applications with external timing requirements.

