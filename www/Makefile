SRC = forn index.php translate.php interpret.php blockly_generator_madeup.js blockly_blocks_madeup.js ide_editor.js ide_function.js ide_skin.css ide_skin_dark.css ide_skin_light.css favicon-16x16.png favicon-32x32.png 
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

libdeps: $(WWW_ROOT)/ace $(WWW_ROOT)/three-js $(WWW_ROOT)/jquery.js $(WWW_ROOT)/js-cookie $(WWW_ROOT)/jquery-ui $(WWW_ROOT)/blockly $(WWW_ROOT)/closure-library $(WWW_ROOT)/filesaver

init: libdeps madeup all

$(DST_BINARIES): $(MADEUP) $(TWODEE) $(MERP)
	sudo cp $^ $(WWW_ROOT)
	sudo chmod a+rX $@

$(WWW_ROOT)/ace:
	sudo git clone https://github.com/ajaxorg/ace-builds.git $(WWW_ROOT)/ace
	sudo chmod -R a+rX $(WWW_ROOT)/ace

# $(WWW_ROOT)/libigl:
	# sudo git clone https://github.com/ajaxorg/ace-builds.git $(WWW_ROOT)/ace
	# sudo chmod -R a+rX $(WWW_ROOT)/ace

$(WWW_ROOT)/filesaver:
	sudo git clone https://github.com/eligrey/FileSaver.js $(WWW_ROOT)/filesaver
	sudo chmod -R a+rX $(WWW_ROOT)/filesaver

$(WWW_ROOT)/ace/src-noconflict/mode-madeup.js: mode-madeup.js
	sudo cp $^ $@
	sudo chmod a+rX $@

$(WWW_ROOT)/js-cookie:
	sudo git clone https://github.com/js-cookie/js-cookie.git $(WWW_ROOT)/js-cookie
	sudo chmod -R a+rX $(WWW_ROOT)/js-cookie

$(WWW_ROOT)/jquery.js:
	sudo curl -o $(WWW_ROOT)/jquery.js http://code.jquery.com/jquery-1.11.2.min.js 
	sudo chmod -R a+rX $(WWW_ROOT)/jquery.js

$(WWW_ROOT)/jquery-ui:
	sudo curl -o $(WWW_ROOT)/jquery-ui.zip http://jqueryui.com/resources/download/jquery-ui-$(JQUERY_UI_VERSION).zip
	sudo unzip -d $(WWW_ROOT) $(WWW_ROOT)/jquery-ui.zip
	sudo ln -s $(WWW_ROOT)/jquery-ui-$(JQUERY_UI_VERSION) $(WWW_ROOT)/jquery-ui
	sudo chmod -R a+rX $(WWW_ROOT)/jquery-ui $(WWW_ROOT)/jquery-ui-$(JQUERY_UI_VERSION)

$(WWW_ROOT)/three-js:
	sudo git clone https://github.com/mrdoob/three.js.git $(WWW_ROOT)/three-js
	sudo chmod -R a+rX $(WWW_ROOT)/three-js

$(WWW_ROOT)/blockly:
	sudo git clone https://github.com/twodee/blockly.git $(WWW_ROOT)/blockly
	sudo chmod -R a+rX $(WWW_ROOT)/blockly

$(WWW_ROOT)/closure-library:
	sudo git clone https://github.com/google/closure-library.git $(WWW_ROOT)/closure-library
	sudo chmod -R a+rX $(WWW_ROOT)/closure-library

$(DST): $(WWW_ROOT)/%: %
	sudo cp $^ $@
	sudo chmod a+rX $@
