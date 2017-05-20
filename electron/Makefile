JQUERY_UI_VERSION = 1.11.2

all: libdeps ace/src-noconflict/mode-madeup.js

libdeps: ace three-js jquery.js js-cookie jquery-ui blockly closure-library filesaver jquery_ui_touch_punch mousetrap meshline

ace:
	git clone --depth=1 https://github.com/ajaxorg/ace-builds.git ace

filesaver:
	git clone --depth=1 https://github.com/eligrey/FileSaver.js filesaver

ace/src-noconflict/mode-madeup.js: 
	cp ../js/mode-madeup.js $@

js-cookie:
	git clone --depth=1 https://github.com/js-cookie/js-cookie.git js-cookie

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
	git clone --depth=1 https://github.com/twodee/blockly.git blockly

closure-library:
	git clone --depth=1 https://github.com/google/closure-library.git closure-library

jquery_ui_touch_punch:
	git clone --depth=1 https://github.com/furf/jquery-ui-touch-punch.git jquery_ui_touch_punch

mousetrap:
	git clone --depth=1 https://github.com/ccampbell/mousetrap.git mousetrap
