# UInputKeyboard

UInputKeyboard is a daemon which reads data from the **NanoVolumeKnob**
over serial port and sends received key codes to the `uinput` device driver in order to generates the associated keyboard's events.

Note: the UInputKeyboard is **only compatible with Linux**

## Build - Install
```
$ qmake
$ make
$ sudo make install
```

By default, the UInputKeyboard daemon will be installed in `/usr/local/bin`.
If you want to change the installation path, type:

```
$ qmake "PREFIX=/SOME/PATH"
$ make
$ sudo make install
```

Example:

By running `qmake "PREFIX=/home/john/"`, the UInputKeyboard daemon binary will be installed in `/home/john/bin/`

## Usage

### Start the daemon
```
sudo service uinputkeyboard start
```

### Stop the daemon
```
sudo service uinputkeyboard stop
```

### Restart the daemon
```
sudo service uinputkeyboard restart
```

### Daemon status
```
sudo service uinputkeyboard status
```
