<!DOCTYPE html>
<html lang="en">
<head>
  <title>Madeup</title>

  <link rel="icon" type="image/png" sizes="32x32" href="/favicon-32x32.png">
  <link rel="icon" type="image/png" sizes="16x16" href="/favicon-16x16.png">

  <!-- JQuery dependencies -->
  <link rel="stylesheet" href="jquery-ui/jquery-ui.css"/>
  <script src="jquery.js"></script>
  <script src="jquery-ui/jquery-ui.js"></script>
  <script src="js-cookie/src/js.cookie.js"></script>
  <link rel="stylesheet" media="screen" type="text/css" href="colorpicker/css/colorpicker.css"/>
  <script type="text/javascript" src="colorpicker/js/colorpicker.js"></script>

  <!-- ThreeJS dependencies -->
  <script src="three-js/build/three.js"></script>
  <script src="three-js/examples/js/controls/OrbitControls.js"></script>
  <script src="three-js/examples/js/controls/TrackballControls.js"></script>

  <!-- Blockly dependencies -->
  <!-- <script src="blockly/blockly_compressed.js"></script> -->
  <script src="blockly/blockly_uncompressed.js"></script>

  <!-- <script src="blockly/blocks_compressed.js"></script> -->
  <script src="blockly/blocks/logic.js"></script>
  <script src="blockly/blocks/loops.js"></script>
  <script src="blockly/blocks/math.js"></script>
  <script src="blockly/blocks/text.js"></script>
  <script src="blockly/blocks/lists.js"></script>
  <script src="blockly/blocks/colour.js"></script>
  <script src="blockly/blocks/variables.js"></script>
  <script src="blockly/blocks/procedures.js"></script>

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
      <block type="madeup_string"></block>
      <block type="madeup_logic_boolean"></block>
      <block type="madeup_eval"></block>
      <block type="madeup_nothing"></block>
    </category>
    <category name="Loops">
      <block type="madeup_loop_repeat"></block>
      <block type="madeup_loop_while"></block>
      <block type="madeup_loop_for_to"></block>
      <block type="madeup_loop_for_to_by"></block>
      <block type="madeup_loop_for_through"></block>
      <block type="madeup_loop_for_through_by"></block>
      <block type="madeup_loop_for_in"></block>
      <block type="madeup_loop_for_in_by"></block>
      <block type="madeup_loop_repeatwich"></block>
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
      <block type="madeup_movement_reframe"></block>
      <block type="madeup_movement_push"></block>
      <block type="madeup_movement_pop"></block>
      <block type="madeup_movement_reverse"></block>
    </category>
    <category name="Generate">
      <block type="madeup_generate_dot"></block>
      <block type="madeup_generate_box"></block>
      <block type="madeup_generate_dowel"></block>
      <block type="madeup_generate_tube"></block>
      <block type="madeup_generate_extrude"></block>
      <block type="madeup_generate_revolve"></block>
      <block type="madeup_generate_surface"></block>
      <block type="madeup_generate_forget"></block>
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
      <block type="madeup_math_random"></block>
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
    <input id="toggleEditorPopup" type="button" value="Editor"/>

    <input style="float: right" id="download" type="button" value="Download Model"/>
    <input style="float: right" id="fit" type="button" value="Fit"/>
    <input style="float: right" id="toggleGridPopup" type="button" value="Grid"/>
    <input style="float: right" id="toggleDisplayPopup" type="button" value="Display"/>

    <div id="editorPopup" class="popups">
      <input type="radio" name="editorMode" id="isEditorText" value="Text" checked="checked"/>Text or 
      <input type="radio" name="editorMode" id="isEditorBlocks" value="Blocks"/>Blocks<br/>
      <input type="checkbox" id="autopreview" checked="checked"/>Preview <input type="text" id="nSecondsTillPreview" size="5"/> seconds after last edit<br/>
    </div>
    <div id="gridPopup" class="popups">
      <input type="checkbox" id="axisX"/>X axis<br/>
      <input type="checkbox" id="axisY"/>Y axis<br/>
      <input type="checkbox" id="axisZ"/>Z axis<br/>
      <input type="checkbox" id="gridX"/>X = 0 plane<br/>
      <input type="checkbox" id="gridY"/>Y = 0 plane<br/>
      <input type="checkbox" id="gridZ"/>Z = 0 plane<br/>
      Grid Extent: <input type="text" id="gridExtent" size="5"/><br/>
      Grid Spacing: <input type="text" id="gridSpacing" size="5"/><br/>
    </div>
    <div id="displayPopup" class="popups">
      <input type="checkbox" id="showWireframe"/>Show wireframe<br/>
      <input type="checkbox" id="showHeadings">Show heading</input><br/>
      <input type="checkbox" id="showCounterclockwise">Show counterclockwise faces</input><br/>
      <input type="checkbox" id="showClockwise">Show clockwise faces</input><br/>
      <input type="checkbox" id="isFlatShaded">Flat shading</input><br/>
      <div id="modelColor" style="width: 100px; display: inline-block; padding: 5px">Model color</div>
    </div>
    <br/>
  </div>

  <div id="left">
    <div id="editorPane">
      <div id="text_editor"></div>
      <div id="blocksEditor">
        <div id="blocksCanvas"></div>
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
    <input id="shadingMode" type="text" value="SMOOTH" name="shadingMode"/>
    <textarea id="source" name="source"></textarea>
  </form>

  <script src="ace/src-noconflict/ace.js" type="text/javascript" charset="utf-8"></script>
  <script src="ide_editor.js"></script>
</body>
</html>
