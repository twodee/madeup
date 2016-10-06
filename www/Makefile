SRC = forn index.php translate.php save.php interpret.php blockly_generator_madeup.js blockly_blocks_madeup.js text_to_blocks.js ide_editor.js ide_function.js ide_skin.css ide_skin_dark.css ide_skin_light.css favicon-16x16.png favicon-32x32.png keystrokes.js block_definitions.js
DST = $(addprefix $(WWW_ROOT)/, $(SRC))
JQUERY_UI_VERSION = 1.11.2

DST_BINARIES = $(WWW_ROOT)/$(notdir $(MERP))

.PHONY: madeup check

all: check $(DST) $(WWW_ROOT)/ace/src-noconflict/mode-madeup.js $(DST_BINARIES)

check:
	@if [ -z "$(WWW_ROOT)" ]; then \
		echo "WWW_ROOT is not set. Run \"make www\" from the build directory."; \
		exit 1; \
	fi

libdeps: $(WWW_ROOT)/ace $(WWW_ROOT)/three-js $(WWW_ROOT)/jquery.js $(WWW_ROOT)/js-cookie $(WWW_ROOT)/jquery-ui $(WWW_ROOT)/blockly $(WWW_ROOT)/closure-library $(WWW_ROOT)/filesaver $(WWW_ROOT)/jquery_ui_touch_punch $(WWW_ROOT)/mousetrap

init: libdeps madeup all

$(DST_BINARIES): $(MADEUP) $(TWODEE) $(MERP)
	$(SUDO) cp $^ $(WWW_ROOT)
	$(SUDO) chmod a+rX $@

$(WWW_ROOT)/ace:
	$(SUDO) git clone https://github.com/ajaxorg/ace-builds.git $(WWW_ROOT)/ace
	$(SUDO) chmod -R a+rX $(WWW_ROOT)/ace

# $(WWW_ROOT)/libigl:
	# $(SUDO) git clone https://github.com/ajaxorg/ace-builds.git $(WWW_ROOT)/ace
	# $(SUDO) chmod -R a+rX $(WWW_ROOT)/ace

$(WWW_ROOT)/filesaver:
	$(SUDO) git clone https://github.com/eligrey/FileSaver.js $(WWW_ROOT)/filesaver
	$(SUDO) chmod -R a+rX $(WWW_ROOT)/filesaver

$(WWW_ROOT)/ace/src-noconflict/mode-madeup.js: mode-madeup.js
	$(SUDO) cp $^ $@
	$(SUDO) chmod a+rX $@

$(WWW_ROOT)/js-cookie:
	$(SUDO) git clone https://github.com/js-cookie/js-cookie.git $(WWW_ROOT)/js-cookie
	$(SUDO) chmod -R a+rX $(WWW_ROOT)/js-cookie

$(WWW_ROOT)/jquery.js:
	$(SUDO) curl -o $(WWW_ROOT)/jquery.js http://code.jquery.com/jquery-1.11.2.min.js 
	$(SUDO) chmod -R a+rX $(WWW_ROOT)/jquery.js

$(WWW_ROOT)/jquery-ui:
	$(SUDO) curl -o $(WWW_ROOT)/jquery-ui.zip http://jqueryui.com/resources/download/jquery-ui-$(JQUERY_UI_VERSION).zip
	$(SUDO) unzip -d $(WWW_ROOT) $(WWW_ROOT)/jquery-ui.zip
	$(SUDO) ln -s $(WWW_ROOT)/jquery-ui-$(JQUERY_UI_VERSION) $(WWW_ROOT)/jquery-ui
	$(SUDO) chmod -R a+rX $(WWW_ROOT)/jquery-ui $(WWW_ROOT)/jquery-ui-$(JQUERY_UI_VERSION)

$(WWW_ROOT)/three-js:
	$(SUDO) git clone https://github.com/mrdoob/three.js.git $(WWW_ROOT)/three-js
	$(SUDO) chmod -R a+rX $(WWW_ROOT)/three-js

$(WWW_ROOT)/blockly:
	$(SUDO) git clone https://github.com/twodee/blockly.git $(WWW_ROOT)/blockly
	$(SUDO) chmod -R a+rX $(WWW_ROOT)/blockly

$(WWW_ROOT)/closure-library:
	$(SUDO) git clone https://github.com/google/closure-library.git $(WWW_ROOT)/closure-library
	$(SUDO) chmod -R a+rX $(WWW_ROOT)/closure-library

$(WWW_ROOT)/jquery_ui_touch_punch:
	$(SUDO) git clone https://github.com/furf/jquery-ui-touch-punch.git $(WWW_ROOT)/jquery_ui_touch_punch
	$(SUDO) chmod -R a+rX $(WWW_ROOT)/jquery_ui_touch_punch

$(WWW_ROOT)/mousetrap:
	$(SUDO) git clone https://github.com/ccampbell/mousetrap.git $(WWW_ROOT)/mousetrap
	$(SUDO) chmod -R a+rX $(WWW_ROOT)/mousetrap

$(DST): $(WWW_ROOT)/%: %
	$(SUDO) cp $^ $@
	$(SUDO) chmod a+rX $@
