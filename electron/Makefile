# SRC = forn index.php translate.php save.php interpret.php blockly_blocks_madeup.js text_to_blocks.js ide_editor.js ide_function.js ide_skin.css ide_skin_dark.css ide_skin_light.css favicon-16x16.png favicon-32x32.png keystrokes.js block_definitions.js vertex.png boot.php index.html
SRC = index.html
JQUERY_UI_VERSION = 1.11.2

.PHONY: madeup check

all: check $(DST)

libdeps: ace three-js jquery.js js-cookie jquery-ui blockly closure-library filesaver jquery_ui_touch_punch mousetrap meshline

ace:
	git clone https://github.com/ajaxorg/ace-builds.git ace

filesaver:
	git clone https://github.com/eligrey/FileSaver.js filesaver

repos/ace/src-noconflict/mode-madeup.js: 
	cp js/mode-madeup.js $@

js-cookie:
	git clone https://github.com/js-cookie/js-cookie.git js-cookie

jquery.js:
	curl -o jquery.js http://code.jquery.com/jquery-1.11.2.min.js 

jquery-ui:
	curl -o jquery-ui.zip http://jqueryui.com/resources/download/jquery-ui-$(JQUERY_UI_VERSION).zip
	unzip jquery-ui.zip
	ln -s jquery-ui-$(JQUERY_UI_VERSION) jquery-ui

three-js:
	git clone --depth=1 https://github.com/mrdoob/three.js.git three-js

meshline:
	git clone --depth=1 https://github.com/spite/THREE.MeshLine meshline

blockly:
	git clone https://github.com/twodee/blockly.git blockly

closure-library:
	git clone https://github.com/google/closure-library.git closure-library

jquery_ui_touch_punch:
	git clone https://github.com/furf/jquery-ui-touch-punch.git jquery_ui_touch_punch

mousetrap:
	git clone https://github.com/ccampbell/mousetrap.git mousetrap
