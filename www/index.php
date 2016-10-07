<!DOCTYPE html>
<html lang="en">
<head>
  <title>Madeup</title>
  <meta charset="UTF-8">
  <meta name="viewport" content="width=device-width, initial-scale=1.0, maximum-scale=1.0, user-scalable=no">

  <link rel="icon" type="image/png" sizes="32x32" href="/favicon-32x32.png">
  <link rel="icon" type="image/png" sizes="16x16" href="/favicon-16x16.png">
  <link href="https://fonts.googleapis.com/css?family=Hammersmith+One" rel="stylesheet">

  <!-- JQuery dependencies -->
  <link rel="stylesheet" href="jquery-ui-1.11.2/jquery-ui.css"/>
  <script src="jquery.js"></script>
  <script src="jquery-ui-1.11.2/jquery-ui.js"></script>
  <script src="js-cookie/src/js.cookie.js"></script>
  <script src="jquery_ui_touch_punch/jquery.ui.touch-punch.min.js"></script>

  <script src="mousetrap/mousetrap.min.js"></script>

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
  <script src="block_definitions.js"></script>
  <script src="blockly_blocks_madeup.js"></script>
  <script src="blockly_generator_madeup.js"></script>

  <script>
    var isEmbedded = false;
    var keystrokesMoviePrefix = null;
  </script>

<?php
if (array_key_exists('src', $_REQUEST)) {
?>

  <script>
    isEmbedded = true;
  </script>

<?php
}
?>

<?php
if (array_key_exists('movie', $_REQUEST)) {
  $keystrokesMoviePrefix = $_REQUEST['movie'];
?>

  <script>
    keystrokesMoviePrefix = '<?= $keystrokesMoviePrefix ?>';
  </script>
  <script src="keystrokes.js"></script>

<?php
}
?>

  <!-- Madeup IDE dependencies -->
  <link rel="stylesheet" href="ide_skin.css"/>
  <link rel="stylesheet" href="ide_skin_dark.css" title="theme"/>
  <script src="text_to_blocks.js"></script>
  <script src="ide_function.js"></script>
</head>

<body>
  <!-- The Blockly toolbox __________________________________________________ -->
  <xml id="toolbox" style="display: none">
    <category name="Data">
      <block type="madeup_integer"></block>
      <block type="madeup_real"></block>
      <block type="madeup_string"></block>
      <block type="madeup_boolean"></block>
      <block type="madeup_where"></block>
      <block type="madeup_nothing"></block>
    </category>
    <category name="Variables" custom="VARIABLE"></category>
    <category name="Functions" custom="PROCEDURE"></category>
    <category name="Math">
      <block type="madeup_binary_arithmetic_operator"></block>
      <block type="madeup_unary_operator"></block>
      <block type="madeup_sincostan"></block>
      <block type="madeup_inverse_sincostan"></block>
      <block type="madeup_atan2"></block>
      <block type="madeup_minmax"></block>
      <block type="madeup_abs"></block>
      <block type="madeup_sign"></block>
      <block type="madeup_random"></block>
      <block type="madeup_log"></block>
    </category>
    <category name="Movement">
      <block type="madeup_moveto"></block>
      <block type="madeup_move"></block>
      <block type="madeup_turn"></block>
      <block type="madeup_center"></block>
      <block type="madeup_rotate"></block>
      <block type="madeup_scale"></block>
      <block type="madeup_translate"></block>
      <block type="madeup_identity"></block>
      <block type="madeup_reframe"></block>
      <block type="madeup_push"></block>
      <block type="madeup_pop"></block>
      <block type="madeup_coalesce"></block>
      <block type="madeup_reverse"></block>
    </category>
    <category name="Solidifiers">
      <block type="madeup_spheres"></block>
      <block type="madeup_boxes"></block>
      <block type="madeup_dowel">
        <value name="MAXBEND">
          <shadow type="madeup_real">
            <field name="REAL">360</field>
          </shadow>
        </value>
      </block>
      <block type="madeup_tube">
        <value name="MAXBEND">
          <shadow type="madeup_real">
            <field name="REAL">360</field>
          </shadow>
        </value>
      </block>
      <block type="madeup_extrude"></block>
      <block type="madeup_revolve"></block>
      <block type="madeup_polygon"></block>
      <block type="madeup_surface"></block>
      <block type="madeup_forget"></block>
      <block type="madeup_echo"></block>
    </category>
    <category name="Loops">
      <block type="madeup_repeat"></block>
      <block type="madeup_while"></block>
      <block type="madeup_for_upper"></block>
      <block type="madeup_for_upper_by"></block>
      <block type="madeup_for_in"></block>
      <block type="madeup_for_in_by"></block>
      <block type="madeup_repeat_around"></block>
    </category>
    <category name="I/O">
      <block type="madeup_print"></block>
      <block type="madeup_debug"></block>
    </category>
    <category name="Logic">
      <block type="madeup_binary_logic_operator"></block>
      <block type="madeup_not"></block>
      <block type="madeup_if_expr"></block>
      <block type="madeup_if_statement"></block>
      <block type="madeup_if_else_statement"></block>
      <block type="madeup_relational_operator"></block>
    </category>
    <category name="Arrays">
      <block type="madeup_array_literal"></block>
      <block type="madeup_array_by"></block>
      <block type="madeup_array_of"></block>
      <block type="madeup_subscript"></block>
      <block type="madeup_subscript_set"></block>
      <block type="madeup_subrange"></block>
      <block type="madeup_normalize"></block>
      <block type="madeup_magnitude"></block>
      <block type="madeup_cross"></block>
      <block type="madeup_dot"></block>
      <block type="madeup_size"></block>
    </category>
  </xml>

  <!-- The layout _________________________________________________________ -->
<div id="entire" style="z-index: 0">
  <div id="menu">
    <div style="display: inline-block; position: absolute: left: 0px;">
      <input id="toggleFilePopup" class="togglePopup" type="button" value="File"/>
      <input id="toggleEditorPopup" class="togglePopup" type="button" value="Editor"/>
      <input id="toggleDisplayPopup" class="togglePopup" type="button" value="Display"/>
      <input id="toggleAboutPopup" class="togglePopup" type="button" value="About"/>
    </div>

    <div style="display: inline-block; position: absolute; left: 0px; right: 0px; text-align: center; pointer-events: none;" class="fileLink">
    </div>

    <div style="display: inline-block; position: absolute; right: 0px;">
      <input id="solidify" type="button" value="Solidify"/>
      <input id="pathify" type="button" value="Pathify"/>
      <input id="fit" type="button" value="Fit"/>
    </div>

    <div id="aboutPopup" class="popup">
      <a href="#" id="docs" class="fileLink">Documentation</a><br>
      <a href="#" id="github" class="fileLink">Source Code</a><br>
      <a href="mailto:crjjrc@gmail.com" id="email" class="fileLink">Email Us</a><br>
    </div>

    <div id="filePopup" class="popup">
      <a href="#" id="fileSave" class="fileLink">Save</a><br/>
      <a href="#" id="fileSaveAs" class="fileLink">Save As</a><br/>
      <p></p>
      <a href="#" id="fileClose" class="fileLink">Close</a><br/>
      <p></p>
      <a href="#" id="magic" class="fileLink">Send to Chris</a><br/>
      <p></p> 
      <a href="#" id="download" class="fileLink">Export OBJ Model</a><br/>
      <p></p> 
      <a href="#" id="exportArchive" class="fileLink">Export Archive</a><br/>
      <a href="#" id="importArchive" class="fileLink">Import Archive</a><br/>
      <form id="importForm">
        <input type="file" id="archive">
        <a href="#" id="cancelImport" class="fileLink">Cancel Import</a>
      </form>
      <p></p> 
      <a href="#" id="fileDelete" class="fileLink">Delete</a><br/>
      <hr>
      <div id="mupsContainer"><div id="mups"></div></div>
    </div>

    <div id="editorPopup" class="popup">
      Text size: <a href="#" id="smaller" class="fileLink">smaller</a> or <a href="#" id="bigger" class="fileLink">bigger</a><br/>
      <input type="radio" name="editorMode" id="isEditorText" value="Text" checked="checked"/>Text or 
      <input type="radio" name="editorMode" id="isEditorBlocks" value="Blocks"/>Blocks<br/>
      <input type="radio" name="theme" id="isDark" value="isDark" checked="checked"/>Dark or 
      <input type="radio" name="theme" id="isLight" value="isLight"/>Light<br/>
      <input type="checkbox" id="showConsole" checked="checked"/>Show Console<br/>
      <input type="checkbox" id="autopathify" checked="checked"/>Auto-pathify <input type="text" id="nSecondsTillAutopathify" size="5"/> seconds after last edit<br/>
    </div>

    <div id="displayPopup" class="popup">
      Show <select name="showMode" id="showMode" class="popupDropdown">
        <option value="solid" selected>Solid</option> 
        <option value="triangles">Triangles</option> 
        <option value="shaded_triangles">Shaded Triangles</option> 
        <option value="wireframe">Wireframe</option> 
      </select><br/>
      <input type="checkbox" id="showHeadings"/>Show heading<br/>
      <input type="checkbox" id="showPoints"/>Show points<br/>
      <input type="checkbox" id="lightBothSides"/>Two-sided lighting<br/>
      <input type="checkbox" id="isFlatShaded"/>Flat shading<br/>
      <input type="checkbox" id="isAutorotate"/>Autorotate<br/>
      <hr>
      View From:
      <a href="#" id="cameraLeft" class="fileLink">-X</a> <a href="#" id="cameraRight" class="fileLink">+X</a>
      <a href="#" id="cameraBottom" class="fileLink">-Y</a> <a href="#" id="cameraTop" class="fileLink">+Y</a>
      <a href="#" id="cameraBack" class="fileLink">-Z</a> <a href="#" id="cameraFront" class="fileLink">+Z</a><br/>
      <hr>
      Grid:<br/>
      <input type="checkbox" id="axisX"/>X axis<br/>
      <input type="checkbox" id="axisY"/>Y axis<br/>
      <input type="checkbox" id="axisZ"/>Z axis<br/>
      <input type="checkbox" id="gridX"/>X = 0 plane<br/>
      <input type="checkbox" id="gridY"/>Y = 0 plane<br/>
      <input type="checkbox" id="gridZ"/>Z = 0 plane<br/>
      Grid Extent: <input type="text" id="gridExtent" size="5"/><br/>
      Grid Spacing: <input type="text" id="gridSpacing" size="5"/><br/>
    </div>
  </div>

  <div id="left">
    <div id="editorPane">
      <div id="textEditor" class="mousetrap"></div>
      <div id="blocksEditor">
        <div id="blocksCanvas"></div>
      </div>
    </div>
    <div id="console"><div id="message"></div></div>
  </div>

  <div id="glcanvas"></div>

<?php
if (array_key_exists('movie', $_REQUEST)) {
?>

  <div id="keystrokes">
    <audio controls="controls" id="scrubber">
      <source src="<?= $keystrokesMoviePrefix ?>.wav" type="audio/wav">
      <source src="<?= $keystrokesMoviePrefix ?>.ogg" type="audio/ogg">
    </audio>
  </div>

<?php
}
?>

</div>

<!-- Hidden Download Form _________________________________________________ -->
<form id="downloader" action="interpret.php" method="post" style="display: none">
  <input id="extension" type="text" value="obj" name="extension"/>
  <input id="tag" type="text" value="model" name="tag"/>
  <input id="geometry_mode" type="text" value="SURFACE" name="geometry_mode"/>
  <input id="shading_mode" type="text" value="SMOOTH" name="shading_mode"/>
  <input id="timestamp" type="text" name="timestamp"/>
  <textarea id="source" name="source"></textarea>
</form>

<script src="ace/src-noconflict/ace.js" type="text/javascript" charset="utf-8"></script>
<script src="ide_editor.js"></script>

<!-- Blocks Switch ________________________________________________________ -->
<?php
if (strpos($_SERVER['HTTP_HOST'], 'to.madeup') !== false) {
?>

<script>
  $(document).ready(function() {
    setEditor(false);
    showConsole(false);
  });
</script>

<?php
}
?>

<!-- Source Loader ________________________________________________________ -->
<?php
if (array_key_exists('src', $_REQUEST)) {
  $src = str_replace(array("\r\n", "\n", "\r"), "\\n", $_REQUEST['src']);
?>

  <script>
    textEditor.setValue('<?= $src ?>', 1);
    run(getSource(), GeometryMode.SURFACE, fit);
  </script>

<?php
}
?>

</body>
</html>
