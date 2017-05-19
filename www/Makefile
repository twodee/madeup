JS = $(addprefix js/,blockly_blocks_madeup.js text_to_blocks.js ide_editor.js ide_function.js keystrokes.js block_definitions.js)
CSS = $(addprefix css/,ide_skin.css ide_skin_dark.css ide_skin_light.css)
IMAGES = $(addprefix images/,favicon-16x16.png favicon-32x32.png pathify_icon.png solidify_icon.png fit.png gear.png close.png)
SRC = forn index.php translate.php save.php interpret.php index.html bridge.js $(JS) $(CSS) $(IMAGES)
DST = $(addprefix $(WWW_ROOT)/, $(SRC))
DEPS = ace three-js jquery.js js-cookie jquery-ui blockly closure-library filesaver jquery_ui_touch_punch mousetrap meshline
JQUERY_UI_VERSION = 1.12.1

DST_BINARIES = $(WWW_ROOT)/$(notdir $(MERP))

.PHONY: madeup check

all: check $(DST) $(WWW_ROOT)/repos/ace/src-noconflict/mode-madeup.js $(DST_BINARIES)

check:
	@if [ -z "$(WWW_ROOT)" ]; then \
		echo "WWW_ROOT is not set. Run \"make www\" from the build directory."; \
		exit 1; \
	fi

libdeps: $(addprefix $(WWW_ROOT)/repos/, $(DEPS))

init: libdeps madeup all

$(DST_BINARIES): $(MADEUP) $(TWODEE) $(MERP)
	$(SUDO) cp $^ $(WWW_ROOT)
	$(SUDO) chmod a+rX $@

$(WWW_ROOT)/repos/ace:
	$(SUDO) git clone --depth=1 https://github.com/ajaxorg/ace-builds.git $@
	$(SUDO) chmod -R a+rX $@

$(WWW_ROOT)/repos/filesaver:
	$(SUDO) git clone --depth=1 https://github.com/eligrey/FileSaver.js $@
	$(SUDO) chmod -R a+rX $@

$(WWW_ROOT)/repos/meshline:
	$(SUDO) git clone --depth=1 https://github.com/spite/THREE.MeshLine.git $@
	$(SUDO) chmod -R a+rX $@

$(WWW_ROOT)/repos/ace/src-noconflict/mode-madeup.js: js/mode-madeup.js
	$(SUDO) cp $^ $@
	$(SUDO) chmod a+rX $@

$(WWW_ROOT)/repos/js-cookie:
	$(SUDO) git clone --depth=1 https://github.com/js-cookie/js-cookie.git $@
	$(SUDO) chmod -R a+rX $@

$(WWW_ROOT)/repos/jquery.js:
	$(SUDO) curl -o $@ http://code.jquery.com/jquery-1.11.2.min.js 
	$(SUDO) chmod -R a+rX $@

$(WWW_ROOT)/repos/jquery-ui:
	$(SUDO) curl -o $(WWW_ROOT)/repos/jquery-ui.zip http://jqueryui.com/resources/download/jquery-ui-$(JQUERY_UI_VERSION).zip
	$(SUDO) unzip -d $(WWW_ROOT)/repos $(WWW_ROOT)/repos/jquery-ui.zip
	$(SUDO) ln -s $(WWW_ROOT)/repos/jquery-ui-$(JQUERY_UI_VERSION) $@
	$(SUDO) chmod -R a+rX $@ $(WWW_ROOT)/repos/jquery-ui-$(JQUERY_UI_VERSION)

$(WWW_ROOT)/repos/three-js:
	$(SUDO) git clone --depth=1 https://github.com/mrdoob/three.js.git $@
	$(SUDO) chmod -R a+rX $@

$(WWW_ROOT)/repos/blockly:
	$(SUDO) git clone --depth=1 https://github.com/twodee/blockly.git $@
	$(SUDO) chmod -R a+rX $@

$(WWW_ROOT)/repos/closure-library:
	$(SUDO) git clone --depth=1 https://github.com/google/closure-library.git $@
	$(SUDO) chmod -R a+rX $@

$(WWW_ROOT)/repos/jquery_ui_touch_punch:
	$(SUDO) git clone --depth=1 https://github.com/furf/jquery-ui-touch-punch.git $@
	$(SUDO) chmod -R a+rX $@

$(WWW_ROOT)/repos/mousetrap:
	$(SUDO) git clone --depth=1 https://github.com/ccampbell/mousetrap.git $@
	$(SUDO) chmod -R a+rX $@

$(WWW_ROOT)/js:
	mkdir $@
	chmod a+rX $@

$(WWW_ROOT)/css:
	mkdir $@
	chmod a+rX $@

$(WWW_ROOT)/images:
	mkdir $@
	chmod a+rX $@

$(WWW_ROOT)/repos:
	mkdir $@
	chmod a+rX $@

$(DST): $(WWW_ROOT)/%: % $(WWW_ROOT)/js $(WWW_ROOT)/css $(WWW_ROOT)/images $(WWW_ROOT)/repos
	echo $(SUDO) cp $* $@
	$(SUDO) cp $* $@
	$(SUDO) chmod a+rX $@
