<!DOCTYPE html>
<html lang="en">
<head>
  <title>Madeup</title>

  <!-- JQuery dependencies -->
  <link rel="stylesheet" href="jquery-ui/jquery-ui.css"/>
  <script src="jquery.js"></script>
  <script src="jquery-ui/jquery-ui.js"></script>
  <script src="jquery-cookie/src/jquery.cookie.js"></script>
  <link rel="stylesheet" media="screen" type="text/css" href="colorpicker/css/colorpicker.css"/>
  <script type="text/javascript" src="colorpicker/js/colorpicker.js"></script>

  <!-- ThreeJS dependencies -->
  <script src="three-js/build/three.js"></script>
  <script src="three-js/examples/js/controls/OrbitControls.js"></script>

  <!-- Blockly dependencies -->
  <script src="blockly/blockly_compressed.js"></script>
  <script src="blockly/blocks_compressed.js"></script>
  <script src="blockly/msg/js/en.js"></script>
  <script src="blockly_blocks_madeup.js"></script>
  <script src="blockly_generator_madeup.js"></script>

  <!-- Madeup IDE dependencies -->
  <link rel="stylesheet" href="ide_skin.css"/>
  <script src="ide_function.js"></script>
</head>

<body>
  <!-- The Blockly toolbox __________________________________________________ -->
  <xml id="toolbox" style="display: none">
    <category name="Primitives">
      <block type="madeup_math_integer"></block>
      <block type="madeup_math_real"></block>
      <block type="madeup_logic_boolean"></block>
    </category>
    <category name="Loops">
      <block type="madeup_loop_repeat"></block>
      <block type="madeup_loop_while"></block>
      <block type="madeup_loop_for_to"></block>
      <block type="madeup_loop_for_through"></block>
      <block type="madeup_loop_for_in"></block>
    </category>
    <category name="Movement">
      <block type="madeup_movement_moveto"></block>
      <block type="madeup_movement_move"></block>
      <block type="madeup_movement_turn"></block>
      <block type="madeup_movement_center"></block>
      <block type="madeup_movement_rotate"></block>
      <block type="madeup_movement_scale"></block>
      <block type="madeup_movement_translate"></block>
      <block type="madeup_movement_identity"></block>
    </category>
    <category name="Generate">
      <block type="madeup_generate_ball"></block>
      <block type="madeup_generate_box"></block>
      <block type="madeup_generate_dowel"></block>
      <block type="madeup_generate_extrude"></block>
      <block type="madeup_generate_revolve"></block>
      <block type="madeup_generate_surface"></block>
    </category>
    <category name="I/O">
      <block type="madeup_io_print"></block>
      <block type="madeup_io_debug"></block>
      <block type="madeup_io_where"></block>
    </category>
    <category name="Math">
      <block type="madeup_math_binary_arithmetic_operator"></block>
      <block type="madeup_math_unary_operator"></block>
      <block type="madeup_math_sincostan"></block>
      <block type="madeup_math_inverse_sincostan"></block>
      <block type="madeup_math_minmax"></block>
      <block type="madeup_math_abs"></block>
      <block type="madeup_math_sign"></block>
      <block type="madeup_math_log"></block>
    </category>
    <category name="Variables" custom="VARIABLE"></category>
    <category name="Functions" custom="PROCEDURE"></category>
    <category name="Logic">
      <block type="madeup_logic_junction"></block>
      <block type="madeup_logic_not"></block>
      <block type="madeup_logic_if_expr"></block>
      <block type="madeup_logic_if_statement"></block>
      <block type="madeup_logic_if_else_statement"></block>
      <block type="madeup_math_relational_operator"></block>
    </category>
  </xml>

  <!-- The layout _________________________________________________________ -->
<div id="entire">
  <div id="menu">
    <input id="run" type="button" value="Run"/>
    <input id="smaller" type="button" value="-"/>
    <input id="bigger" type="button" value="+"/>
    <input id="toggle_editor_popup" type="button" value="Editor"/>

    <input style="float: right" id="download" type="button" value="Download Model"/>
    <input style="float: right" id="fit" type="button" value="Fit"/>
    <input style="float: right" id="toggle_grid_popup" type="button" value="Grid"/>
    <input style="float: right" id="toggle_display_popup" type="button" value="Display"/>

    <div id="editor_popup" class="popups">
      Mode:<br/>
      <input type="radio" name="editor_mode" value="Text" checked="checked"/>Text<br/>
      <input type="radio" name="editor_mode" value="Blocks"/>Blocks<br/>
      <input type="checkbox" id="autopreview" checked="checked"/>Preview <input type="text" id="nSecondsTillPreview" size="2"/> seconds after last edit<br/>
    </div>
    <div id="grid_popup" class="popups">
      <input type="checkbox" id="grid_xy"/>XY<br/>
      <input type="checkbox" id="grid_yz"/>YZ<br/>
      <input type="checkbox" id="grid_xz"/>XZ<br/>
    </div>
    <div id="display_popup" class="popups">
      <input type="checkbox" id="isWireframe"/>Wireframe<br/>
      Model color: <div id="modelColor" style="width: 50px; height: 18px; display: inline-block"></div>
      <input type="checkbox" id="showHeadings">Show Headings</input><br/>
    </div>
    <br/>
  </div>

  <div id="left">
    <div id="editor_pane">
      <div id="text_editor"></div>
      <div id="blocks_editor">
        <div id="blocks_canvas"></div>
      </div>
    </div>
    <div id="console"><div id="message"></div></div>
  </div>

  <div id="glcanvas" bgcolor="#00FF00"></div>
</div>

  <!-- Setup ______________________________________________________________ -->
  <form id="downloader" action="interpret.php" method="post" style="display: none">
    <input id="extension" type="text" value="obj" name="extension"/>
    <input id="mode" type="text" value="SURFACE" name="mode"/>
    <textarea id="source" name="source"></textarea>
  </form>

  <script src="ace/src-noconflict/ace.js" type="text/javascript" charset="utf-8"></script>
  <script src="ide_editor.js"></script>
</body>
</html>
