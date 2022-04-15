# paintforge
PaintForge, a paint program for Tapwave Zodiac based on X-Forge game engine

![Logo](https://github.com/jarikomppa/paintforge/blob/main/logo.png?raw=true)

## What?

This is a bunch of very old source code (from 2003-2004). I was working for Fathammer at the time, and I was the (de-facto) lead architect for the X-Forge Core, the hardware abstraction level of the mobile game engine.

If you haven't heard of Fathammer or X-Forge, I'll forgive you. These existed before iphone came to the market, and the engine ran on basically any mobile device at the time; phones, PDAs, even some smart remote controls.

In many ways Fathammer was ahead of its time, with the mobile gaming revolution always a couple years in the future. By the time iphone hit the market, Fathammer was no more - but I believe we played an important role in leading the world to where nobody thinks it's strange to play a real time 3d rendered game on their phone while waiting for the bus.

## Yes, But What?

Patience. I'm getting to it, eventually.

Anyway, one of the PDA.. "personal digital assistant", a term Apple coined with their doomed Newton device that didn't take off, but used by others whose devices did, namely Palm... yes, one of the PDAs we worked on was a Palm ecosystem splinter called Tapwave Zodiac.

I loved the Tapwave project. We cooperated with Tapwave so much that a lot of the time it felt like we were the same company, to the point where I shifted my day cycle so my day would overlap more with Tapwave in the US. To the point where when I wasn't around one day due to holiday or whatever, my counterpart at Tapwave got a bit pissed off. He did apologise afterwards, though.

## You're Still Rambling

Yes, I am. So, like I said, I was the one who designed and wrote large parts of the low level of X-Forge. I think it was a pretty nicely designed low level engine for the time; remember, the target devices had single-core 32 bit 200Mhz ARM CPUs without floating point support, and maybe a megabyte or two of RAM.

This was designed so that we could write support for various platforms fairly quickly; we might fly to some company's offices for a couple of days, port the core to their platform, compile a couple of our games for it and then fly back. In some cases we'd hear stories of our (very few) competitors struggling for months to do something similar.

Other folks at Fathammer worked on stuff on top of the core - the 3d engine and games. I also had nothing to do with the core's software rendering 3d pipeline, apart from maybe talking design in broad strokes. We also had people who concentrated on specific platforms, like the dreaded Symbian.

## Get To The Point Already

I wrote a lot of the documentation and also did a few small test games, like a simple 2d block pushing game, which was pretty easy to do in X-Forge.

Anyway, while playing around with the Tapwave Zodiac device - the first mobile device ever with analog joystick and touchscreen (it was a resistive screen so you had to use a stylus, but that's just how things worked back then), I figured that hey, this could have a paint program.

I'd use the stylus to draw, and the analog joystick to affect brush size and transparency (if I remember correctly).

It was a fun project, and I do recommend trying to make a simple photoshop clone yourself, as it's surprising how much you learn about photoshop's internals without ever seeing the internals. Some things just "click".

## So It's a Small Photoshop Then?

Well, kinda. Very limited. And very unfinished. Due to some communication issues at Fathammer I was told not to work on it, even though I used only publicly available materials and my own free time.

The company understood their mistake and apologised, even offered to reimburse me for whatever, but the inspiration and motivation to work on it was already gone, so I never continued on it.

Anyway, there's a bunch of hard-coded limitations, like the fact that it's 16 bit RGB color (5:6:5 if I remember correctly), just the Tapwave's resolution (480x320 iirc), number of layers designed to fit the Tapwave's memory. Oh, and it was very unfinished at the point where I gave up on it.

## How Do I Compile It?

Good luck.

## Seriously, How?

You'd need X-Forge SDK to compile against, at least. Other than that, I have no idea. I guess you could just look at the function calls and reimplement everything the program calls, if you have too much time on your hands.

Also, it's probably faster for you to just, you know, start from scratch.

## Why Release This?

It's more of "why not". There's probably very little value in it, but who knows, maybe if you bothered to read this far, you might get a kick out of it.

The most fun stuff is in canvas.cpp.

If you do find this interesting, feel free to send me some email. You're a smart person, I'm sure you'll find my email somewhere. Maybe use a search engine.
