SRC = index.php interpret.php keystrokes.js blockly_generator_madeup.js blockly_blocks_madeup.js
DST = $(addprefix $(WWW_ROOT)/, $(SRC))
JQUERY_UI_VERSION = 1.11.2

# DST_BINARIES = $(WWW_ROOT)/$(notdir $(MADEUP)) $(WWW_ROOT)/$(notdir $(TWODEE)) $(WWW_ROOT)/$(notdir $(MERP)) 
DST_BINARIES = $(WWW_ROOT)/$(notdir $(MERP))

.PHONY: madeup

all: $(DST) $(WWW_ROOT)/ace/src-noconflict/mode-madeup.js $(DST_BINARIES)
libdeps: $(WWW_ROOT)/ace $(WWW_ROOT)/three-js $(WWW_ROOT)/jquery.js $(WWW_ROOT)/jquery-cookie $(WWW_ROOT)/jquery-ui $(WWW_ROOT)/colorpicker $(WWW_ROOT)/blockly
init: libdeps madeup all

run: all

$(DST_BINARIES): $(MADEUP) $(TWODEE) $(MERP)
	echo "$^ are getting copied"
	sudo cp $^ $(WWW_ROOT)
	sudo chmod a+rX $@

$(WWW_ROOT)/ace:
	sudo git clone https://github.com/ajaxorg/ace-builds.git $(WWW_ROOT)/ace
	sudo chmod -R a+rX $(WWW_ROOT)/ace

$(WWW_ROOT)/ace/src-noconflict/mode-madeup.js: mode-madeup.js
	sudo cp $^ $@
	sudo chmod a+rX $@

$(WWW_ROOT)/jquery-cookie:
	sudo git clone https://github.com/carhartl/jquery-cookie.git $(WWW_ROOT)/jquery-cookie
	sudo chmod -R a+rX $(WWW_ROOT)/jquery-cookie

$(WWW_ROOT)/jquery.js:
	sudo curl -o $(WWW_ROOT)/jquery.js http://code.jquery.com/jquery-1.11.2.min.js 
	sudo chmod -R a+rX $(WWW_ROOT)/jquery.js

$(WWW_ROOT)/jquery-ui:
	sudo curl -o $(WWW_ROOT)/jquery-ui.zip http://jqueryui.com/resources/download/jquery-ui-$(JQUERY_UI_VERSION).zip
	sudo unzip -d $(WWW_ROOT) $(WWW_ROOT)/jquery-ui.zip
	sudo ln -s $(WWW_ROOT)/jquery-ui-$(JQUERY_UI_VERSION) $(WWW_ROOT)/jquery-ui
	sudo chmod -R a+rX $(WWW_ROOT)/jquery-ui $(WWW_ROOT)/jquery-ui-$(JQUERY_UI_VERSION)

$(WWW_ROOT)/colorpicker:
	sudo mkdir -p $(WWW_ROOT)/colorpicker
	sudo curl -o $(WWW_ROOT)/colorpicker/colorpicker.zip http://www.eyecon.ro/colorpicker/colorpicker.zip
	sudo unzip -d $(WWW_ROOT)/colorpicker $(WWW_ROOT)/colorpicker/colorpicker.zip
	sudo chmod -R a+rX $(WWW_ROOT)/colorpicker

$(WWW_ROOT)/three-js:
	sudo git clone https://github.com/mrdoob/three.js.git $(WWW_ROOT)/three-js
	sudo chmod -R a+rX $(WWW_ROOT)/three-js

$(WWW_ROOT)/blockly:
	sudo git clone https://github.com/google/blockly.git $(WWW_ROOT)/blockly
	sudo chmod -R a+rX $(WWW_ROOT)/blockly

$(DST): $(WWW_ROOT)/%: %
	sudo cp $^ $@
	sudo chmod a+rX $@
