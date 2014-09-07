# VPublisher

### Overview
**Over-the-top content (OTT)** refers to delivery of audio, video, and other media over the Internet without a multiple system operator being involved in the control or distribution of the content. **Adaptive bitrate streaming** is a technique used in streaming multimedia over computer networks. While in the past most video streaming technologies utilized streaming protocols such as RTP with RTSP, today's adaptive streaming technologies are almost exclusively based on HTTP[1] and designed to work efficiently over large distributed HTTP networks such as the Internet.

### Project Scope
VPublisher is a project intended to allows video publishing in a OTT environvemnt. It allows to acquire video from filesystem or acquisition board and publishes it in [HLS](http://en.wikipedia.org/wiki/HTTP_Live_Streaming) or [MPEG Dash](http://en.wikipedia.org/wiki/Dynamic_Adaptive_Streaming_over_HTTP) format.

### Technologies
* Qt 5
* C/C++
* HSL
* LibAV
* Video4Linux (DVB TV)

### Status & Notes
The project is still in its early stages and need other development efforts. It is intended to work under a Linux system. Tested with Ubuntu 12.04
