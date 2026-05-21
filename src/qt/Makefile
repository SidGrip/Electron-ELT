.PHONY: FORCE
all: FORCE
	$(MAKE) -C .. electron_qt test_electron_qt
clean: FORCE
	$(MAKE) -C .. electron_qt_clean test_electron_qt_clean
check: FORCE
	$(MAKE) -C .. test_electron_qt_check
electron-qt electron-qt.exe: FORCE
	 $(MAKE) -C .. electron_qt
apk: FORCE
	$(MAKE) -C .. electron_qt_apk
