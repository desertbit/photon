# Photon

Build powerful crossplatform Web Apps. This project is a simple alternative to [Electron](http://electron.atom.io/).
Useful for Go application.

```
proton http://localhost:8080
```

## Installation

The Qt libraries are required.

### Mac

```
brew install qt
brew link --force qt
```

## Workarounds

### Linux Nouveau crash

https://bugreports.qt.io/browse/QTBUG-41242

```
export LIBGL_ALWAYS_SOFTWARE = 1
photon URL
```
