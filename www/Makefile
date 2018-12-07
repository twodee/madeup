JS = $(addprefix js/,text_to_blocks.js core.js keystrokes.js)
CSS = $(addprefix css/,ide_skin.css ide_skin_dark.css ide_skin_light.css)
IMAGES = $(addprefix images/,favicon-16x16.png favicon-32x32.png pathify_icon.png solidify_icon.png fit.png gear.png close.png)
PRIVACY = $(addprefix privacy/,index.html)
SRC = forn index.php translate.php save.php snapshot.php interpret.php index.html bridge.js $(JS) $(CSS) $(IMAGES) $(PRIVACY)
DST = $(addprefix $(WWW_ROOT)/, $(SRC))
DEPS = ace gapi.js three-js jquery.js js-cookie jquery-ui blockly closure-library filesaver jquery_ui_touch_punch mousetrap meshline
JQUERY_UI_VERSION = 1.12.1
MINIFIED_DEPS = $(addprefix $(WWW_ROOT)/,deps.min.js mode-madeup.js theme-twilight.js theme-katzenmilch.js)
DOCS = $(addprefix $(WWW_ROOT)/,$(wildcard docs/html/*.html) $(wildcard docs/html/images/*))

DST_BINARIES = $(WWW_ROOT)/$(notdir $(MERP))

.PHONY: madeup check

all: check $(DST) $(WWW_ROOT)/docs/html $(WWW_ROOT)/docs/html/images $(DOCS) $(DOCIMAGES) $(WWW_ROOT)/repos/ace/src-noconflict/mode-madeup.js $(DST_BINARIES) $(MINIFIED_DEPS)

check:
	@if [ -z "$(WWW_ROOT)" ]; then \
		echo "WWW_ROOT is not set. Run \"make www\" from the build directory."; \
		exit 1; \
	fi

libdeps: $(WWW_ROOT)/repos $(addprefix $(WWW_ROOT)/repos/, $(DEPS))

init: libdeps madeup all

$(WWW_ROOT)/docs/html $(WWW_ROOT)/docs/html/images:
	$(SUDO) mkdir -p $(WWW_ROOT)/docs/html
	$(SUDO) chmod g+rX $(WWW_ROOT)/docs/html
	$(SUDO) mkdir -p $(WWW_ROOT)/docs/html/images
	$(SUDO) chmod g+rX $(WWW_ROOT)/docs/html/images

$(WWW_ROOT)/docs/html/%: docs/html/%
	@echo cp -r $< $@
	cp -r $< $@
	chmod -R g+rX $@

$(DST_BINARIES): $(MADEUP) $(TWODEE) $(MERP)
	$(SUDO) cp $^ $(WWW_ROOT)
	$(SUDO) chmod g+rX $@

$(WWW_ROOT)/repos/ace:
	$(SUDO) git clone --depth=1 https://github.com/ajaxorg/ace-builds.git $@
	$(SUDO) chmod -R g+rX $@

$(WWW_ROOT)/repos/gapi.js:
	$(SUDO) curl -o $@ https://apis.google.com/js/api.js
	$(SUDO) chmod g+r $@

$(WWW_ROOT)/repos/filesaver:
	$(SUDO) git clone --depth=1 https://github.com/eligrey/FileSaver.js $@
	$(SUDO) chmod -R g+rX $@

$(WWW_ROOT)/repos/meshline:
	$(SUDO) git clone --depth=1 https://github.com/spite/THREE.MeshLine.git $@
	$(SUDO) chmod -R g+rX $@

$(WWW_ROOT)/repos/ace/src-noconflict/mode-madeup.js: js/mode-madeup.js
	$(SUDO) cp $^ $@
	$(SUDO) chmod g+rX $@

$(WWW_ROOT)/repos/js-cookie:
	$(SUDO) git clone --depth=1 https://github.com/js-cookie/js-cookie.git $@
	$(SUDO) chmod -R g+rX $@

$(WWW_ROOT)/repos/jquery.js:
	$(SUDO) curl -o $@ https://code.jquery.com/jquery-3.2.1.min.js
	$(SUDO) chmod -R g+rX $@

$(WWW_ROOT)/repos/jquery-ui:
	$(SUDO) curl -o $(WWW_ROOT)/repos/jquery-ui.zip http://jqueryui.com/resources/download/jquery-ui-$(JQUERY_UI_VERSION).zip
	$(SUDO) unzip -d $(WWW_ROOT)/repos $(WWW_ROOT)/repos/jquery-ui.zip
	$(SUDO) ln -s $(WWW_ROOT)/repos/jquery-ui-$(JQUERY_UI_VERSION) $@
	$(SUDO) chmod -R g+rX $@ $(WWW_ROOT)/repos/jquery-ui-$(JQUERY_UI_VERSION)

$(WWW_ROOT)/repos/three-js:
	$(SUDO) git clone --depth=1 https://github.com/mrdoob/three.js.git $@
	$(SUDO) chmod -R g+rX $@

$(WWW_ROOT)/repos/blockly:
	$(SUDO) git clone --depth=1 https://github.com/twodee/blockly.git $@
	$(SUDO) chmod -R g+rX $@

$(WWW_ROOT)/repos/closure-library:
	$(SUDO) git clone --depth=1 https://github.com/google/closure-library.git $@
	$(SUDO) chmod -R g+rX $@

$(WWW_ROOT)/repos/jquery_ui_touch_punch:
	$(SUDO) git clone --depth=1 https://github.com/furf/jquery-ui-touch-punch.git $@
	$(SUDO) chmod -R g+rX $@

$(WWW_ROOT)/repos/mousetrap:
	$(SUDO) git clone --depth=1 https://github.com/ccampbell/mousetrap.git $@
	$(SUDO) chmod -R g+rX $@

$(WWW_ROOT)/deps.min.js: $(addprefix $(WWW_ROOT)/,repos/gapi.js repos/jquery.js repos/jquery-ui/jquery-ui.js repos/jquery_ui_touch_punch/jquery.ui.touch-punch.js repos/js-cookie/src/js.cookie.js repos/mousetrap/mousetrap.min.js repos/three-js/build/three.js repos/three-js/examples/js/controls/TrackballControls.js repos/meshline/src/THREE.MeshLine.js repos/filesaver/FileSaver.min.js repos/ace/src-noconflict/ace.js js/text_to_blocks.js js/core.js bridge.js)
	@echo uglifying...
	cd $(WWW_ROOT) && $(SUDO) uglifyjs $^ > $@
	$(SUDO) chmod g+r $@

$(WWW_ROOT)/mode-madeup.js:
	cd $(WWW_ROOT) && $(SUDO) ln -s repos/ace/src-noconflict/mode-madeup.js .

$(WWW_ROOT)/theme-twilight.js:
	cd $(WWW_ROOT) && $(SUDO) ln -s repos/ace/src-noconflict/theme-twilight.js .

$(WWW_ROOT)/theme-katzenmilch.js:
	cd $(WWW_ROOT) && $(SUDO) ln -s repos/ace/src-noconflict/theme-katzenmilch.js .

$(WWW_ROOT)/js:
	mkdir $@
	chmod g+rX $@

$(WWW_ROOT)/css:
	mkdir $@
	chmod g+rX $@

$(WWW_ROOT)/images:
	mkdir $@
	chmod g+rX $@

$(WWW_ROOT)/privacy:
	mkdir $@
	chmod g+rX $@

$(WWW_ROOT)/repos:
	mkdir $@
	chmod g+rX $@

$(DST): $(WWW_ROOT)/%: % $(WWW_ROOT)/js $(WWW_ROOT)/css $(WWW_ROOT)/images $(WWW_ROOT)/privacy $(WWW_ROOT)/repos
	echo $(SUDO) cp $* $@
	$(SUDO) cp $* $@
	$(SUDO) chmod g+rX $@
