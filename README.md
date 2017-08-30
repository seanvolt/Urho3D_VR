![Urho3D logo](https://raw.githubusercontent.com/urho3d/Urho3D/master/bin/Data/Textures/LogoLarge.png)

# Urho3D_VR

**Urho3D_VR** is a fork of Urho3D that adds support for OpenVR, with the goal of getting integrated into the proper Urho3D repository in the future. This fork includes a HelloVR sample.

## Current Issues:
- I don't have enough experience with CMake to get the project to build right now. The CMake integration in our internal project was created by a team member that is currently unavailable, and I have been unable to port it correctly. The rest of the code works in our own project, so if someone can get this project to build and compile using the OpenVR library, I can continue working on this fork.
- Right now the VR code is integrated as a subsystem. I feel that it's best to trust others that know Urho3D's architecture better than I do, so I'd love suggestions as to how this should be organized architecturally.
- Regarding implementation, the sample currently is sending hand and head nodes to the VR subsystem, which then creates the cameras and moves the head and hands every frame. Is there a more desirable way of doing this?
- I haven't yet ported our code over for getting button input from the controller. I'd like to know what's the best way of going about this - Should developers talk directly to OpenVR for controller input, or should the VR subsystem provide a complete interface to separate the OpenVR library?
- I've been unable to get the correct projection matrices for the eye's cameras, so it's currently estimated with a 'hack.'
- Haptic feedback does not work.

## License
Licensed under the MIT license, see [LICENSE](https://github.com/urho3d/Urho3D/blob/master/LICENSE) for details.

## Credits
Urho3D development, contributions and bugfixes by:
- Lasse Öörni
- Wei Tjong Yao
- Aster Jian
- Ricardo Abreu
- Vivienne Anthony
- Colin Barrett
- Erik Beran
- Gauthier Billot
- Loic Blot
- Danny Boisvert
- Sergey Bosko
- Lisandro Bruzzo
- Thomas Böhm
- Carlo Carollo
- Pete Chown
- Christian Clavet
- Sebastian Delatorre (primitivewaste)
- Stanislav Demyanovich
- Rainer Deyke
- Josh Engebretson
- Simon Flores
- Manuel Freiberger
- Chris Friesen
- Alex Fuller
- Konstantin Guschin
- Henrik Heino
- Mika Heinonen
- Victor Holt
- Johnathan Jenkins
- Jukka Jylänki
- Graham King
- Jason Kinzer
- Cameron Kline
- Jan Korous
- Eugene Kozlov
- Gunnar Kriik
- Aliaksandr Kryvashein
- Artem Kulyk
- Rokas Kupstys
- Ali Kämäräinen
- Sergey Lapin
- Pete Leigh
- Pengfei Li
- Arnis Lielturks
- Frode 'Modanung' Lindeijer
- Thorbjørn Lindeijer
- Nathanial Lydick
- Xavier Maupeu
- Iain Merrick
- Justin Miller
- Jonne Nauha
- Huy Nguyen
- Paul Noome
- David Palacios
- Alex Parlett
- Jordan Patterson
- Georgii Pelageikin
- Anton Petrov
- Vladimir Pobedinsky
- Franck Poulain
- Pranjal Raihan
- Svyatoslav Razmyslov
- Mariusz Richtscheid
- Nick Royer
- Jonathan Sandusky
- Miika Santala
- Anatoly Sennov
- Matan Shukry
- Bengt Soderstrom
- Hualin Song
- James Thomas
- Joshua Tippetts
- Konstantin Tomashevich
- Yusuf Umar
- Mateus Vendramini
- Daniel Wiberg
- Steven Zhang
- AGreatFish
- BlueMagnificent
- CG-SS
- Enhex
- Fastran
- Firegorilla
- Gordon-F
- Lumak
- Magic.Lixin
- Mike3D
- MonkeyFirst
- Ner'zhul
- Newb I the Newbd
- OvermindDL1
- PredatorMF
- Scellow
- Skrylar
- TheComet93
- Y-way
- 1vanK
- andmar1x
- amadeus_osa
- atship
- att
- celeron55
- cosmy1
- damu
- dragonCASTjosh
- feltech
- fredakilla
- gleblebedev
- hdunderscore
- lhinuz
- lvshiling
- marynate
- meshonline
- mightyCelu
- neat3d
- nemerle
- ninjastone
- orefkov
- proller
- raould
- rasteron
- reattiva
- rifai
- rikorin
- skaiware
- ssinai1
- svifylabs
- szamq
- thebluefish
- tommy3
- yushli
Urho3D VR development contributed by: 
- Sean Voltaire

Urho3D uses the following third-party libraries:
- AngelScript 2.31.2 (http://www.angelcode.com/angelscript)
- Boost 1.64.0 (http://www.boost.org) - only used for AngelScript generic bindings
- Box2D 2.3.2 WIP (http://box2d.org)
- Bullet 2.86.1 (http://www.bulletphysics.org)
- Civetweb 1.7 (https://github.com/civetweb/civetweb)
- FreeType 2.8 (https://www.freetype.org)
- GLEW 1.13.0 (http://glew.sourceforge.net)
- jo_jpeg 1.52 (http://www.jonolick.com/uploads/7/9/2/1/7921194/jo_jpeg.cpp)
- kNet (https://github.com/juj/kNet)
- libcpuid 0.4.0 (https://github.com/anrieff/libcpuid)
- Lua 5.1 (https://www.lua.org)
- LuaJIT 2.1.0+ (http://www.luajit.org)
- LZ4 1.7.5 (https://github.com/lz4/lz4)
- MojoShader (https://icculus.org/mojoshader)
- Mustache 1.0 (https://mustache.github.io, https://github.com/kainjow/Mustache)
- nanodbc 2.12.4 (https://lexicalunit.github.io/nanodbc)
- Open Asset Import Library 3.2 (http://assimp.sourceforge.net)
- pugixml 1.7 (http://pugixml.org)
- rapidjson 1.1.0 (https://github.com/miloyip/rapidjson)
- Recast/Detour (https://github.com/memononen/recastnavigation)
- SDL 2.0.5 (https://www.libsdl.org)
- SQLite 3.18.0 (https://www.sqlite.org)
- StanHull (https://codesuppository.blogspot.com/2006/03/john-ratcliffs-code-suppository-blog.html)
- stb_image 2.12 (https://nothings.org)
- stb_image_write 1.02 (https://nothings.org)
- stb_rect_pack 0.08 (https://nothings.org)
- stb_vorbis 1.09 (https://nothings.org)
- tolua++ 1.0.93 (defunct - http://www.codenix.com/~tolua)
- WebP (https://chromium.googlesource.com/webm/libwebp)

DXT / ETC1 / PVRTC decompression code based on the Squish library and the Oolong
Engine.
Jack and mushroom models from the realXtend project. (https://www.realxtend.org)
Ninja model and terrain, water, smoke, flare and status bar textures from OGRE.
BlueHighway font from Larabie Fonts.
Anonymous Pro font by Mark Simonson.
NinjaSnowWar sounds by Veli-Pekka Tätilä.
PBR textures from Substance Share. (https://share.allegorithmic.com)
IBL textures from HDRLab's sIBL Archive.
Dieselpunk Moto model by allexandr007.
Mutant & Kachujin models from Mixamo.
License / copyright information included with the assets as necessary. All other assets (including shaders) by Urho3D authors and licensed similarly as the engine itself.
