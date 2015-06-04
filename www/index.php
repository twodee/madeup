<!DOCTYPE html>
<html lang="en">
<head>
  <title>Madeup</title>

  <!-- JQuery dependencies -->
  <link rel="stylesheet" href="jquery-ui/jquery-ui.css">
  <script src="jquery.js"></script>
  <script src="jquery-ui/jquery-ui.js"></script>
  <script src="jquery-cookie/src/jquery.cookie.js"></script>
  <link rel="stylesheet" media="screen" type="text/css" href="colorpicker/css/colorpicker.css" />
  <script type="text/javascript" src="colorpicker/js/colorpicker.js"></script>

  <!-- ThreeJS dependencies -->
  <script src="three-js/build/three.js"></script>
  <script src="three-js/examples/js/controls/OrbitControls.js"></script>

  <!-- Blockly dependencies -->
  <script src="blockly/blockly_compressed.js"></script>
  <script src="blockly/blocks_compressed.js"></script>
  <script src="blockly_blocks_madeup.js"></script>
  <script src="blockly_generator_madeup.js"></script>

  <!-- script src="keystrokes.js"></script -->

  <style type="text/css" media="screen">
html {
  height: 100%;
  width: 100%;
  overflow: hidden;
}

body {
  height: 100%;
  width: 100%;
  overflow: hidden;
  padding: 0;
  margin: 0;
}

#tools {
  background: #000000;
  color: #FFFFFF;
  float: left;
  width: 300px;
}

#tools_close {
  float: right;
}

#tools h2 {
  color: #999999;
}

#entire {
  float: left;
  width: 100%;
  padding: 0;
  margin: 0;
}

#left {
  float: left;
  padding: 0;
  width: 420px;
  height: auto;
  background-color: #000000;
}

#glcanvas {
  /* display: inline; */
  float: left;
  width: auto;
  overflow: hidden;
}

#text_editor { 
  margin: 0px;
  height: 300px;
  display: none;
}

#blocks_panel {
  background-color: green;
  margin: 0px;
}

#blocks_editor { 
  position: absolute;
  /* margin: 0px; */
  background-color: red;
}

#console {
  font-family: Courier;
  outline: none;
  background-color: #333333;
  color: #FFFFFF;
  height: 200px;
  overflow: auto;
  z-index: 1000;
}

#message {
  overflow: auto;
}

/* Fattens the handle for resizing the console. Needed because the text_editor seems 
   to snarf up its size of the boundary. */
#console > .ui-resizable-n {
  top: 0;
  height: 5px;
}

#widgets input[type="button"] {
  background-color: #333333;
  border: 0px;
  margin: 4px;
  color: white;
  font-size: 10pt;
  outline: none;
}

#widgets input[type="button"]:active {
  background-color: #999999;
}

.srclink:link {
  color: #FF9999; 
}

.srclink:visited {
  color: #000000; 
}

.srclink:hover {
  color: #FF9900; 
}

.srclink:active {
  color: #FF99FF; 
}

.settingslink {
  text-decoration: underline;
}

.settingslink:link {
  color: #FF9999; 
}

.settingslink:visited {
  color: #000000; 
}

.settingslink:hover {
  color: #FF9900; 
}

.settingslink:active {
  color: #FF99FF; 
}

#scrubber { 
  display: none;
  position: absolute;
  width: 100%;
  bottom: 0px;
  margin: 0px;
  padding: 0px;
  z-index: 100;
}

#widgets {
  position: relative;
  margin: 0px;
  padding: 0px;
}

#downloader {
  position: fixed;
  top: 600px;
  left: 0px;
  width: 420px;
}

    </style>

    <script type="text/javascript">
var GeometryMode = Object.freeze({
  PATH: 'PATH',
  SURFACE: 'SURFACE'
});

THREE.Object3D.prototype.clear = function() {
  var children = this.children;
  for (var i = children.length - 1; i >= 0; --i) {
    var child = children[i];
    child.clear();
    this.remove(child);
  }
}

var scene, renderer, camera, controls;
var geoscene, pointer_scene;
var meshes = [];
var nSecondsTillPreview = 1.0;
var isWireframe = false;
var arrow_shafts = [];
var showHeadings = true;
var modelColor = 'FF0000';

$(document).ready(function() {
  scrubber = $('#scrubber')[0];

  /* $.getJSON("movie.json", function(data) { */
    /* movie = new KeystrokesMovie(data); */
  /* }); */

  $('#scrubber').bind('seeking', function() {
    show(scrubber.currentTime);
  });

  var fontSize = 14;

  var periodicID;
  $('#scrubber').bind('play', function() {
    periodicID = window.setInterval(function() {
      show(scrubber.currentTime);
    }, 100);
  });

  $('#scrubber').bind('pause', function() {
    window.clearInterval(periodicID);
  });

  $(window).load(function() {
    if ($.cookie('last')) {
      text_editor.setValue($.cookie('last'), -1);
    }

    if ($.cookie('fontSize')) {
      fontSize = parseInt($.cookie('fontSize'));
    } else {
      fontSize = 14;
    }
    text_editor.setFontSize(fontSize);
    $('#console')[0].style.fontSize = fontSize + 'px';

    if ($.cookie('showHeadings')) {
      showHeadings = parseInt($.cookie('showHeadings')) != 0;
    }
    $('#showHeadings').prop('checked', showHeadings);

    if ($.cookie('modelColor')) {
      modelColor = $.cookie('modelColor');
    }
    $('#modelColor').css('background-color', '#' + modelColor);

    if ($.cookie('isWireframe')) {
      isWireframe = parseInt($.cookie('isWireframe')) != 0;
    } else {
      isWireframe = false;
    }
    $('#isWireframe').prop('checked', isWireframe);
  });

  $(window).unload(function() {
    $.cookie('last', getSource());
    $.cookie('fontSize', fontSize);
    $.cookie('showHeadings', showHeadings ? 1 : 0);
    $.cookie('modelColor', modelColor);
    $.cookie('isWireframe', isWireframe ? 1 : 0);
  });

  $('#clear').click(function() {
    log('');
    text_editor.focus();
  });

  $('#smaller').click(function() {
    fontSize -= 2;
    $('#widgets input').each(function (i) {
      this.style.fontSize = (parseInt($(this).css('font-size')) - 1) + 'px';
    });
    text_editor.setFontSize(fontSize);
    $('#console')[0].style.fontSize = fontSize + 'px';
    text_editor.focus();
  });
  $('#bigger').click(function() {
    fontSize += 2;
    $('#widgets input').each(function (i) {
      this.style.fontSize = (parseInt($(this).css('font-size')) + 1) + 'px';
    });
    text_editor.setFontSize(fontSize);
    $('#console')[0].style.fontSize = fontSize + 'px';
    text_editor.focus();
  });
  /* $('#lastfit').click(function() { */
    /* controls.reset();  */
  /* }); */
  $('#fit').click(function() {
    fit();
    text_editor.focus();
  });
  $('#editor_mode').change(function() {
    var editor_mode = $(this).val();
    console.log(editor_mode);
    if (editor_mode == "Blocks") {
      console.log("switch to blocks");
      var workspace = Blockly.inject('blocks_editor', {toolbox: document.getElementById('toolbox')});
      workspace.addChangeListener(function() {
        var code = Blockly.Madeup.workspaceToCode(workspace);
        /* document.getElementById('source_code').value = code; */
        text_editor.setValue(code);
        console.log(code);
      });
    } else {
      console.log("switch to text");
    }
  });
  $('#showHeadings').click(function() {
    showHeadings = this.checked;
    text_editor.focus();
    render();
  });
  $('#autopreview').click(function() {
    console.log('checked');
    if (this.checked) {
      console.log('on');
      $('#nSecondsTillPreview').prop('disabled', false);
      schedulePreview();
    } else {
      console.log('off');
      $('#nSecondsTillPreview').prop('disabled', true);
      text_editor.getSession().off('change', onEditorChange);
      if (preview) {
        clearTimeout(preview); 
      }
      preview = undefined;
    }
  });
  $('#isWireframe').click(function() {
    isWireframe = this.checked;
    run(GeometryMode.SURFACE);
  });
  $('#tools_close').click(function() {
    $('#tools').hide();
    resize();
    render();
  });
  $('#tools_menu').click(function() {
    $('#tools').toggle();
    resize();
    render();
  });
  $('#download').click(function() {
    $('#source').val(getSource());
    $('#downloader').submit();
    text_editor.focus();
  });
  $('#run').click(function() {
    $.cookie('last', getSource());
    run(GeometryMode.SURFACE);
    text_editor.focus();
  });

  $('#left').resizable({
    handles: "e",
    resize: function(event, ui) {
      resize();
      Blockly.fireUiEvent(window, 'resize');
      render();
    } 
  });

  $('#console').resizable({
    handles: "n",
    resize: function(event, ui) {
      resize();
      Blockly.fireUiEvent(window, 'resize');
      // Need this because console is relatively positioned.
      $('#console').css('top', '0px');
    } 
  });

  $('#nSecondsTillPreview').val(nSecondsTillPreview + '');
  $('#nSecondsTillPreview').change(function () {
    nSecondsTillPreview = parseFloat($('#nSecondsTillPreview').val());
    text_editor.getSession().off('change', onEditorChange);
    if (preview) {
      clearTimeout(preview); 
    }
    preview = undefined;
    schedulePreview();
  });
  schedulePreview();

  $('#modelColor').ColorPicker({
    flat: false,
    color: modelColor,
    onSubmit: function(hsb, hex, rgb) {
      console.log(hex);
      modelColor = hex;
      $('#modelColor').css('background-color', '#' + modelColor);
      run(GeometryMode.SURFACE);
    }
  });
  $('#tools').hide();

  init();
  animate();
});

var onEditorChange = function(delta) {
  console.log('running delta');
  if (preview) {
    clearTimeout(preview); 
  }
  preview = setTimeout(function() {
    run(GeometryMode.PATH);
  }, nSecondsTillPreview * 1000);
}

var preview = undefined;
function schedulePreview() {
  text_editor.getSession().on('change', onEditorChange);
}

function getSource() {
  return text_editor.getValue();
}

var allGeometry = undefined;
function run(mode) {
  $.ajax({
    type: 'POST',
    url: 'interpret.php',
    data: JSON.stringify({source: getSource(), extension: 'json', mode: mode}),
    contentType: 'application/json; charset=utf-8',
    dataType: 'json',
    success: function(data) {
      var sansDebug = data['output'].replace(/^Debug:.*$\n/gm, '');
      if (sansDebug.length > 0) {
        console.log(sansDebug);
      }

      if (data['exit_status'] == 0) {
        for (var i = 0; i < meshes.length; ++i) {
          geoscene.remove(meshes[i]);
        }
        meshes = [];
        arrow_shafts = [];

        log(sansDebug);
        
        if (mode == GeometryMode.SURFACE) {
          var loader = new THREE.JSONLoader();
          var model = loader.parse(JSON.parse(data['model']));
          /* var material = new THREE.MeshLambertMaterial( { color: 0xf61f54, wireframe: isWireframe}); */
          var material = new THREE.MeshLambertMaterial( { color: parseInt(modelColor, 16), wireframe: isWireframe});
          material.side = THREE.DoubleSide;
          meshes[0] = new THREE.Mesh(model.geometry, material);
          allGeometry = model.geometry;
        } else {
          var paths = JSON.parse(data['model']);
          allGeometry = new THREE.Geometry();
          var dotsGeometry = new THREE.Geometry();

          for (var pi = 0; pi < paths.length; ++pi) {
            var geometry = new THREE.Geometry();
            for (var i = 0; i < paths[pi].vertices.length; ++i) {
              var v = new THREE.Vector3(paths[pi].vertices[i][0], paths[pi].vertices[i][1], paths[pi].vertices[i][2]);
              geometry.vertices.push(v);
              dotsGeometry.vertices.push(v);
              allGeometry.vertices.push(v);
            }
            meshes[meshes.length] = new THREE.Line(geometry, new THREE.LineBasicMaterial({
              color: 0x6666FF,
              linewidth: 3
            }));

            var nvertices = paths[pi].vertices.length;
            if (nvertices > 0) {
              var m = paths[pi].orientation;
              /* var height = 1; */
              /* var g2 = new THREE.CylinderGeometry(0, 0.5, height, 10, 10, false); */
              /* allGeometry.vertices = allGeometry.vertices.concat(g2.vertices); */

              /* g2.applyMatrix(new THREE.Matrix4().makeTranslation(0, height * 0.5, 0)); */
              /* g2.applyMatrix(new THREE.Matrix4().set(m[0], m[1], m[2], m[3], m[4], m[5], m[6], m[7], m[8], m[9], m[10], m[11], m[12], m[13], m[14], m[15])); */
              /* g2.applyMatrix(new THREE.Matrix4().makeRotationX(Math.PI / 2.0)); */
              /* var offset = new THREE.Vector3(paths[pi].vertices[paths[pi].vertices.length - 1][0], paths[pi].vertices[paths[pi].vertices.length - 1][1], paths[pi].vertices[paths[pi].vertices.length - 1][2]); */
              /* g2.applyMatrix(new THREE.Matrix4().makeTranslation(offset.x, offset.y, offset.z)); */

              /* meshes[meshes.length] = new THREE.Mesh(g2, new THREE.MeshLambertMaterial({ */
                /* color: 0x0000ff, */
              /* })); */

              // Get the last two points on the page.
              var ultimate = paths[pi].vertices[nvertices - 1];
              ultimate = new THREE.Vector3(ultimate[0], ultimate[1], ultimate[2]);

              /* if (nvertices > 1) { */
                /* var penultimate = paths[pi].vertices[nvertices - 2]; */
                /* penultimate = new THREE.Vector3(penultimate[0], penultimate[1], penultimate[2]); */

                /* // The arrowhead will be sized as a fraction of this last segment. */
                /* var diff = new THREE.Vector3().subVectors(ultimate, penultimate); */
                /* arrow_shafts[arrow_shafts.length] = diff.length() * 0.4; */
                /* console.log("diff"); */
                /* console.log(diff); */
              /* } else { */
              arrow_shafts[arrow_shafts.length] = 2.0;
              /* } */

              // Save the last point to help position the arrowhead.
              arrow_shafts[arrow_shafts.length] = ultimate;

              // Also get the heading, which we'll need to orient the
              // arrowhead.
              var dir = new THREE.Vector3(0, 1, 0);
              dir.applyMatrix4(new THREE.Matrix4().set(m[0], m[1], m[2], m[3], m[4], m[5], m[6], m[7], m[8], m[9], m[10], m[11], m[12], m[13], m[14], m[15]));
              dir.applyMatrix4(new THREE.Matrix4().makeRotationX(Math.PI / 2.0));
              console.log(dir);
              arrow_shafts[arrow_shafts.length] = dir;
            }
          }

          meshes[meshes.length] = new THREE.PointCloud(dotsGeometry, new THREE.PointCloudMaterial({
            color: 0x000000,
            size: 8,
            sizeAttenuation: false
          }));

        }

        /* if (isAutofit) { */
          /* allGeometry.computeBoundingBox(); */
          /* allGeometry.center(); */
          /* var bounds = allGeometry.boundingBox; */
          /* console.log(camera.fov + ' ' + camera.position.z + ' ' + bounds.max.y); */
          /* var verticalFitZ = bounds.max.y / Math.tan(camera.fov * Math.PI / 180.0 * 0.5); */
          /* var fovX = 2 * Math.atan(Math.tan(camera.fov * Math.PI / 180.0 * 0.5) * camera.aspect); */
          /* var horizontalFitZ = bounds.max.x / Math.tan(fovX * 0.5); */
          /* console.log("max z " + bounds.max.z); */
          /* console.log("min z " + bounds.min.z); */
          /* camera.position.z = bounds.max.z + (verticalFitZ > horizontalFitZ ? verticalFitZ : horizontalFitZ); */
          /* camera.updateProjectionMatrix(); */

          /* // Update orbit controls so that it will reset to the last fit -- instead of the camera's initial settings. */
          /* controls.position0 = camera.position.clone(); */
        /* } */

        for (var mi = 0; mi < meshes.length; ++mi) {
          /* meshes[mi].geometry.mergeVertices(); */
          /* meshes[mi].geometry.computeFaceNormals(); */
          /* meshes[mi].geometry.computeVertexNormals(); */
          geoscene.add(meshes[mi]);
          console.log("adding");
          console.log(meshes[mi]);
        }
        render();
      } else if (data['exit_status'] == 22) {
        console.log(data['exit_status']);
        log(data['output'] + '\nYour model was taking a long time to build. It felt like it was never going to finish! So, I stopped trying. Sorry.');
      } else {
        log(sansDebug);
      }
    },
    failure: function(error_message) {
      console.log('Failure. :(');
    }
  });
}

function fit() {
  if (allGeometry === undefined) {
    return;
  }

  allGeometry.computeBoundingBox();
  /* allGeometry.center(); */
  var bounds = allGeometry.boundingBox;
  console.log(bounds);
  var centroid = bounds.center();
  console.log(centroid);

  console.log(geoscene.matrix);
  var xform = new THREE.Matrix4().makeTranslation(-centroid.x, -centroid.y, -centroid.z);
  geoscene.matrix = xform;
  console.log(xform);

  console.log(camera.fov + ' ' + camera.position.z + ' ' + bounds.max.y);
  var verticalFitZ = bounds.max.y / Math.tan(camera.fov * Math.PI / 180.0 * 0.5);
  var fovX = 2 * Math.atan(Math.tan(camera.fov * Math.PI / 180.0 * 0.5) * camera.aspect);
  var horizontalFitZ = bounds.max.x / Math.tan(fovX * 0.5);
  console.log("max z " + bounds.max.z);
  console.log("min z " + bounds.min.z);
  controls.reset(); 
  camera.position.z = bounds.max.z + (verticalFitZ > horizontalFitZ ? verticalFitZ : horizontalFitZ);
  camera.updateProjectionMatrix();

  // Update orbit controls so that it will reset to the last fit -- instead of the camera's initial settings.
  /* controls.position0 = camera.position.clone(); */
}

function log(message) {
  // $1 is the whole source span. $2 is the start. $3 is the end.
  var link_message = message.replace(/^((\d+)\((\d+)(?:-(\d+))?\))/gm, function(match, full, startLine, startIndex, stopIndex) {
    return '<div style="color: #FF9999; display: inline;">Error on <a style="text-decoration: underline;" onclick="javascript:highlight(' + startIndex + ', ' + stopIndex + ')" class="srclink">line ' + startLine + /*':' + startIndex + ':' + stopIndex +*/ '</a></div>';
  });

  $('#console #message').html(link_message.replace(/\n/g, '<br/>'));
}

function resize() {
  var width = window.innerWidth - $('#left').width() - ($('#tools').is(':visible') ? $('#tools').width() : 0);
  var height = window.innerHeight;
  renderer.setSize(width, height);
  camera.aspect = width / height;
  camera.updateProjectionMatrix();
  $("#text_editor").height($(document).height() - ($('#scrubber').is(':visible') ? $('#scrubber').height() : 0) - $('#widgets').height() - ($('#console').is(':visible') ? $('#console').height() : 0));
  $("#blocks_panel").height($(document).height() - ($('#scrubber').is(':visible') ? $('#scrubber').height() : 0) - $('#widgets').height() - ($('#console').is(':visible') ? $('#console').height() : 0));

  var blocklyArea = document.getElementById('blocks_panel');
  var blocklyDiv = document.getElementById('blocks_editor');
  var element = blocklyArea;
  var x = 0;
  var y = 0;
  do {
    x += element.offsetLeft;
    y += element.offsetTop;
    element = element.offsetParent;
  } while (element);
  // Position blocklyDiv over blocklyArea.
  blocklyDiv.style.left = x + 'px';
  blocklyDiv.style.top = y + 'px';
  /* blocklyDiv.style.width = blocklyArea.offsetWidth + 'px'; */
  blocklyDiv.style.height = blocklyArea.offsetHeight + 'px';
  $("#blocks_editor").width(blocklyArea.offsetWidth);

  console.log(blocklyArea.offsetWidth);

  /* console.log($('#left').width()); */
  text_editor.resize();
}

function highlight(startIndex, stopIndex) {
  var doc = text_editor.getSession().getDocument();
  var lines = doc.getAllLines();
  console.log(lines);

  if (stopIndex === undefined) {
    stopIndex = startIndex;
  }

  var indexToRowColumn = function (targetIndex) {
    var row = 0;
    var indexSoFar = 0; 
    
    while (row < lines.length && indexSoFar + lines[row].length < targetIndex) {
      indexSoFar += lines[row].length;
      ++indexSoFar; // linebreak
      ++row;
    }

    var column = targetIndex - indexSoFar;

    return {row: row, column: column};
  }
    
  var start = indexToRowColumn(startIndex);
  var stop = indexToRowColumn(stopIndex);

  text_editor.getSelection().setSelectionRange(new Range(start.row, start.column, stop.row, stop.column + 1));
}

function init() {
  THREE.Camera.prototype.getWorldRight = function () {
    var quaternion = new THREE.Quaternion();
    return function (optionalTarget) {
      var result = optionalTarget || new THREE.Vector3();
      this.getWorldQuaternion(quaternion);
      return result.set(1, 0, 0).applyQuaternion(quaternion);
    }
  }();

  camera = new THREE.PerspectiveCamera(45.0, 1.0, 0.1, 10000.0);
  camera.position.z = 30;

  var glcanvas = $("#glcanvas");
  renderer = new THREE.WebGLRenderer();
  renderer.setClearColor(0xFFFFFF, 1);
  document.getElementById("glcanvas").appendChild(renderer.domElement);

  controls = new THREE.OrbitControls(camera, renderer.domElement);
  /* controls.autoRotate = true; */
  controls.addEventListener('change', function() {
    render();
  });

  window.addEventListener('resize', resize);
  resize();

  pointer_scene = new THREE.Scene();
  geoscene = new THREE.Scene();
  geoscene.matrixAutoUpdate = false;
  scene = new THREE.Scene();
  geoscene.add(pointer_scene);
  scene.add(geoscene);
  scene.add(camera);
  console.log("adding");
  console.log(camera);

  var radius = 50,
      segments = 16,
      rings = 16;

  var sphereMaterial =
    new THREE.MeshLambertMaterial(
      {
        color: 0xCC0000
      });

  // create a point light
  var pointLight = new THREE.PointLight(0xFFFFFF);

  // set its position
  pointLight.position.x = 10;
  pointLight.position.y = 50;
  pointLight.position.z = 130;

  // add to the scene
  camera.add(pointLight);

  render();
}

function animate() {
  requestAnimationFrame(animate);
  controls.update();
}

function render() {
  pointer_scene.clear();

  // Keep arrowheads as screen-aligned as possible.
  if (showHeadings) {
    for (var i = 0; i < arrow_shafts.length; i += 3) {
      var arrow_vector = arrow_shafts[i + 2];
      console.log(arrow_vector);
      var screen_away = new THREE.Vector3().crossVectors(arrow_vector, camera.getWorldDirection()).normalize();
      console.log(arrow_shafts[i]);

      var arrow_vector_inverse = new THREE.Vector3().copy(arrow_vector).negate();
      var left = new THREE.Vector3().addVectors(screen_away, arrow_vector_inverse).multiplyScalar(arrow_shafts[i]);
      var right = new THREE.Vector3().addVectors(new THREE.Vector3().copy(screen_away).negate(), arrow_vector_inverse).multiplyScalar(arrow_shafts[i]);

      var geometry = new THREE.Geometry();
      geometry.vertices.push(new THREE.Vector3().addVectors(arrow_shafts[i + 1], left));
      geometry.vertices.push(arrow_shafts[i + 1]);
      geometry.vertices.push(new THREE.Vector3().addVectors(arrow_shafts[i + 1], right));
      var mesh = new THREE.Line(geometry, new THREE.LineBasicMaterial({
        color: 0x0000FF,
        linewidth: 8
      }));
      pointer_scene.add(mesh);
    }
  }

  renderer.render(scene, camera);
}
  </script>
</head>

<body>
  <!-- The Blockly toolbox __________________________________________________ -->
  <xml id="toolbox" style="display: none">
    <category name="Loops">
      <block type="madeup_loop_repeat"></block>
      <block type="madeup_loop_while"></block>
      <block type="madeup_loop_for_to"></block>
      <block type="madeup_loop_for_through"></block>
      <block type="madeup_loop_for_in"></block>
    </category>
    <category name="Logic">
      <block type="madeup_logic_boolean"></block>
      <block type="madeup_logic_junction"></block>
      <block type="madeup_logic_not"></block>
      <block type="madeup_logic_if_expr"></block>
      <block type="madeup_logic_if_statement"></block>
      <block type="madeup_logic_if_else_statement"></block>
    </category>
    <category name="I/O">
      <block type="madeup_io_print"></block>
      <block type="madeup_io_debug"></block>
      <block type="madeup_io_where"></block>
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
    <category name="Math">
      <block type="madeup_math_integer"></block>
      <block type="madeup_math_real"></block>
      <block type="madeup_math_binary_arithmetic_operator"></block>
      <block type="madeup_math_relational_operator"></block>
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
  </xml>

  <div id="entire">
    <div id="left">
      <div id="widgets">
        <input id="run" type="button" value="Run" />
        <input id="smaller" type="button" value="-" />
        <input id="bigger" type="button" value="+" />
        <input id="tools_menu" type="button" value="Tools" />
        <input id="fit" type="button" value="Fit" />
        <select id="editor_mode">
          <option>Text</option>
          <option>Blocks</option>
        </select>
        <br />
      </div>
      <div id="editor_pane">
        <div id="text_editor"></div>
        <div id="blocks_panel"></div>
        <div id="blocks_editor"></div>
      </div>
      <div id="console"><div id="message"></div></div>
   
      <script src="ace/src-noconflict/ace.js" type="text/javascript" charset="utf-8"></script>
      <script>
var text_editor = ace.edit("text_editor");
var Range = ace.require('ace/range').Range;
text_editor.setTheme("ace/theme/twilight");
text_editor.getSession().setMode("ace/mode/madeup");
text_editor.setHighlightSelectedWord(false);
text_editor.setHighlightActiveLine(false);
      </script>
    </div>
    <div>
      <div id="glcanvas" bgcolor="#00FF00"></div>
      <div id="tools" title="Tools">
        <span id="tools_close">close</span>
        <div style="padding: 10px;">
          <h2>Model</h2>
          <a id="download" class="settingslink">Download model</a><br/>
          <input type="checkbox" id="autopreview" checked="checked"/>Preview <input type="text" id="nSecondsTillPreview" size="2"/> seconds after last edit<br/>
          <input type="checkbox" id="isWireframe" />Wireframe<br/>
          Model color: <div id="modelColor" style="width: 50px; height: 18px; display: inline-block"></div>

          <h2>View</h2>
          <input type="checkbox" id="showHeadings">Show Headings</input><br/>
        </div>
      </div>
    </div>
  </div>

  <form id="downloader" action="interpret.php" method="post" style="display: none">
    <input id="extension" type="text" value="obj" name="extension" />
    <input id="mode" type="text" value="SURFACE" name="mode" />
    <textarea id="source" name="source"></textarea>
  </form>
</body>
</html>
