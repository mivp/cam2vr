# cam2vr
3D Cam to VR (Google cardboard)

**Ref**
- webvr-polyfill

## Workflow

3D camcorder --> decklink capture card --> this program: parse frame, perform vr warp and display (OpenGL) --> Blackmagic Web Presenter --> Youtube --> view by Google cardboard

## Compile

- MacOS/Linux
```
mkdir build
cd build
cmake ..
make
```
