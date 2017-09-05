# cam2vr
3D Cam to VR (Google cardboard)

**Contact**: Toan Nguyen ([http://monash.edu/mivp](http://monash.edu/mivp))

**Ref**
- webvr-polyfill

## Workflow

3D camcorder --> decklink capture card --> this program: parse frame, perform vr warp and display (OpenGL) --> Blackmagic Web Presenter --> Youtube --> view by Google cardboard

## Compile

- Linux (tested with Qt5):
```
cd cam2vr
mkdir build
cd build
qmake ../cam2vr.pro
make
```
