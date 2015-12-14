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

  <!-- ThreeJS dependencies -->
  <script src="three-js/build/three.js"></script>
  <script src="three-js/examples/js/controls/TrackballControls.js"></script>

  <script src="filesaver/FileSaver.min.js"></script>

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
  <link rel="stylesheet" href="ide_skin_dark.css" title="theme"/>
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
      <block type="madeup_generate_spheres"></block>
      <block type="madeup_generate_boxes"></block>
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
<div id="entire" style="z-index: 0">
  <div id="menu">
    <div style="display: inline-block; position: absolute: left: 0px;">
      <input id="solidify" type="button" value="Solidify"/>
      <input id="smaller" type="button" value="-abc"/>
      <input id="bigger" type="button" value="+abc"/>
      <input id="toggleEditorPopup" class="togglePopup" type="button" value="Editor"/>
      <input id="docs" type="button" value="Docs"/>
      <input id="magic" type="button" value="Magic"/>
    </div>

    <div style="display: inline-block; position: absolute; left: 0px; right: 0px; text-align: center; pointer-events: none;" class="fileLink">
      <input style="color: yellow" id="toggleFilePopup" class="togglePopup" type="button" value="File"/>
    </div>

    <div style="display: inline-block; position: absolute; right: 0px;">
      <input id="toggleDisplayPopup" class="togglePopup" type="button" value="Display"/>
      <input id="toggleGridPopup" class="togglePopup" type="button" value="Grid"/>
      <input id="fit" type="button" value="Fit"/>
      <input id="download" type="button" value="Download"/>
    </div>

    <div id="filePopup" class="popup">
      <a href="#" id="fileSave" class="fileLink">Save</a><br/>
      <a href="#" id="fileSaveAs" class="fileLink">Save As</a><br/>
      <a href="#" id="fileClose" class="fileLink">Close</a><br/>
      <p></p> 
      <a href="#" id="exportArchive" class="fileLink">Export Archive</a><br/>
      <a href="#" id="importArchive" class="fileLink">Import Archive</a><br/>
      <form id="importForm">
        <input type="file" id="archive"></input>
        <a href="#" id="cancelImport" class="fileLink">Cancel Import</a>
      </form>
      <p></p> 
      <a href="#" id="fileDelete" class="fileLink">Delete</a><br/>
      <hr>
      <div id="mupsContainer"><div id="mups"></div></div>
    </div>

    <div id="editorPopup" class="popup">
      <input type="radio" name="editorMode" id="isEditorText" value="Text" checked="checked"/>Text or 
      <input type="radio" name="editorMode" id="isEditorBlocks" value="Blocks"/>Blocks<br/>
      <input type="radio" name="theme" id="isDark" value="isDark" checked="checked"/>Dark or 
      <input type="radio" name="theme" id="isLight" value="isLight"/>Light<br/>
      <input type="checkbox" id="autopreview" checked="checked"/>Preview <input type="text" id="nSecondsTillPreview" size="5"/> seconds after last edit<br/>
    </div>

    <div id="gridPopup" class="popup">
      <input type="checkbox" id="axisX"/>X axis<br/>
      <input type="checkbox" id="axisY"/>Y axis<br/>
      <input type="checkbox" id="axisZ"/>Z axis<br/>
      <input type="checkbox" id="gridX"/>X = 0 plane<br/>
      <input type="checkbox" id="gridY"/>Y = 0 plane<br/>
      <input type="checkbox" id="gridZ"/>Z = 0 plane<br/>
      Grid Extent: <input type="text" id="gridExtent" size="5"/><br/>
      Grid Spacing: <input type="text" id="gridSpacing" size="5"/><br/>
    </div>

    <div id="displayPopup" class="popup">
      <input type="checkbox" id="showWireframe"/>Show wireframe<br/>
      <input type="checkbox" id="showHeadings"/>Show heading<br/>
      <input type="checkbox" id="showCounterclockwise"/>Show counterclockwise faces<br/>
      <input type="checkbox" id="showClockwise"/>Show clockwise faces<br/>
      <input type="checkbox" id="isFlatShaded"/>Flat shading<br/>
    </div>
  </div>

  <div id="left">
    <div id="editorPane">
      <div id="textEditor"></div>
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
    <input id="tag" type="text" value="model" name="tag"/>
    <input id="geometry_mode" type="text" value="SURFACE" name="geometry_mode"/>
    <input id="shading_mode" type="text" value="SMOOTH" name="shading_mode"/>
    <textarea id="source" name="source"></textarea>
  </form>

  <script src="ace/src-noconflict/ace.js" type="text/javascript" charset="utf-8"></script>
  <script src="ide_editor.js"></script>
</body>
</html>
