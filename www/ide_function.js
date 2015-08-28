function hasWebGL() {
  try {
    var canvas = document.createElement("canvas");
    return !!window.WebGLRenderingContext && (canvas.getContext("webgl") || canvas.getContext("experimental-webgl"));
  } catch(e) { 
    return false;
  } 
}

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

function populateFileMenu() {
  var list = '';
  var mups = Object.keys(window.localStorage).sort();
  for (var i = 0; i < mups.length; ++i) {
    var mup = mups[i];
    if (mup != 'untitled') {
      list += '<a href="#" class="fileLink" onclick="load(\'' + mup.replace(/'/g, '\\&#39;').replace(/"/g, '\\&quot;') + '\')">- ' + mup + '</a><br/>';
    }
  }
  $('#mups').html(list);
}

function generateDownloadable(filename, text) {
  var blob = new Blob([text], {type: "application/json"});
  saveAs(blob, filename);
}

function yyyymmdd() {
  var now = new Date();
  var year = now.getFullYear();
  var month = now.getMonth() + 1;
  var day = now.getDate();

  day = (day < 10 ? "0" : "") + day;
  month = (month < 10 ? "0" : "") + month;

  return year + '_' + month + '_' + day;
}

var swatch = null;
var initialized = false;
var mupName = null;
var overallScene;
var modelScene;
var glyphScene;
var renderer, camera, controls;
var meshes = [];
var nSecondsTillPreview = 1.0;
var showWireframe = false;
var showHeadings = true;
var showCounterclockwise = true;
var showClockwise = false;
var modelColor = 'FF0000';
var workspace = null;
var fontSize = 14;
var gridSpacing = 1.0;
var gridExtent = 10.0;
var isFlatShaded = true;
var isEditorText = true;

var isEditorTextChanged = false;
var isShowWireframeChanged = false;
var isAxisChanged = [false, false, false];
var isGridChanged = [false, false, false];
var isNSecondsTillPreviewChanged = false;
var isShowHeadingsChanged = false;
var isModelColorChanged = false;
var isFontSizeChanged = false;
var isGridSpacingChanged = false;
var isGridExtentChanged = false;
var isShowCounterclockwiseChanged = false;
var isShowClockwiseChanged = false;
var isFlatShadedChanged = false;

function updateTitle() {
  $('#toggleFilePopup').attr('value', mupName);
}

function saveInCookies() {
  // Cookies.set('lastMup', mupName); 

  // Only store a cookie if a setting has changed. If we unconditionally stored
  // these, then updates to the default value would not be seen by users, as
  // the old defaults persisted in the cookies would override the new ones.
  if (isFontSizeChanged) Cookies.set('fontSize', fontSize);
  if (isShowHeadingsChanged) Cookies.set('showHeadings', showHeadings ? 1 : 0);
  if (isShowCounterclockwiseChanged) Cookies.set('showCounterclockwise', showCounterclockwise ? 1 : 0);
  if (isShowClockwiseChanged) Cookies.set('showClockwise', showClockwise ? 1 : 0);
  if (isModelColorChanged) Cookies.set('modelColor', modelColor);
  if (isShowWireframeChanged) Cookies.set('showWireframe', showWireframe ? 1 : 0);
  if (isFlatShadedChanged) Cookies.set('isFlatShaded', isFlatShaded ? 1 : 0);
  if (isEditorTextChanged) Cookies.set('isEditorText', isEditorText ? 1 : 0);
  if (isAxisChanged[0]) Cookies.set('axisX', $('#axisX').prop('checked') ? 1 : 0);
  if (isAxisChanged[1]) Cookies.set('axisY', $('#axisY').prop('checked') ? 1 : 0);
  if (isAxisChanged[2]) Cookies.set('axisZ', $('#axisZ').prop('checked') ? 1 : 0);
  if (isGridChanged[0]) Cookies.set('gridX', $('#gridX').prop('checked') ? 1 : 0);
  if (isGridChanged[1]) Cookies.set('gridY', $('#gridY').prop('checked') ? 1 : 0);
  if (isGridChanged[2]) Cookies.set('gridZ', $('#gridZ').prop('checked') ? 1 : 0);
  if (isNSecondsTillPreviewChanged) Cookies.set('nSecondsTillPreview', nSecondsTillPreview);
  if (isGridSpacingChanged) Cookies.set('gridSpacing', gridSpacing);
  if (isGridExtentChanged) Cookies.set('gridExtent', gridExtent);
  Cookies.set('leftWidth', $('#left').width());
  Cookies.set('consoleHeight', $('#console').height());

  if (workspace) {
    var xml = Blockly.Xml.workspaceToDom(workspace);
    var xmlText = Blockly.Xml.domToText(xml);
    Cookies.set('lastBlocks', xmlText);
  }

  // Changes have been committed, so let's reset the dirty flags.
  isFontSizeChanged = false;
  isShowHeadingsChanged = false;
  isShowCounterclockwiseChanged = false;
  isShowClockwiseChanged = false;
  isFlatShadedChanged = false;
  isModelColorChanged = false;
  isShowWireframeChanged = false;
  isEditorTextChanged = false;
  isShowHeadingsChanged = false;
  isNSecondsTillPreviewChanged = false;
  isGridSpacingChanged = false;
  isGridExtentChanged = false;
  for (var d = 0; d < 3; ++d) {
    isAxisChanged[d] = false;
    isGridChanged[d] = false;
  }
}

$(document).ready(function() {
  $(window).load(function() {
    Cookies.defaults = {
      expires: 10 * 365
    };

    if (Cookies.get('leftWidth')) {
      $('#left').width(Cookies.get('leftWidth'));
      resize();
    }

    if (Cookies.get('consoleHeight')) {
      $('#console').height(Cookies.get('consoleHeight'));
      resize();
    }

    // if (Cookies.get('last')) { 
      // textEditor.setValue(Cookies.get('last'), -1); 
    // } 

    // if (Cookies.get('lastMup')) { 
      // load(Cookies.get('lastMup')); 
    // } else { 
    load('untitled');
    // } 

    if (Cookies.get('fontSize')) {
      fontSize = parseInt(Cookies.get('fontSize'));
    } else {
      fontSize = 14;
    }
    textEditor.setFontSize(fontSize);
    $('#console')[0].style.fontSize = fontSize + 'px';
    $('#menu input').each(function (i) {
      var delta = (fontSize - 14) / 2;
      this.style.fontSize = (parseInt($(this).css('font-size')) + delta) + 'px';
    });

    if (Cookies.get('showHeadings')) {
      showHeadings = parseInt(Cookies.get('showHeadings')) != 0;
    }
    $('#showHeadings').prop('checked', showHeadings);

    if (Cookies.get('isFlatShaded')) {
      isFlatShaded = parseInt(Cookies.get('isFlatShaded')) != 0;
    }
    $('#isFlatShaded').prop('checked', isFlatShaded);

    if (Cookies.get('showCounterclockwise')) {
      showCounterclockwise = parseInt(Cookies.get('showCounterclockwise')) != 0;
    }
    $('#showCounterclockwise').prop('checked', showCounterclockwise);

    if (Cookies.get('showClockwise')) {
      showClockwise = parseInt(Cookies.get('showClockwise')) != 0;
    }
    $('#showClockwise').prop('checked', showClockwise);

    if (Cookies.get('modelColor')) {
      modelColor = Cookies.get('modelColor');
    }
    $('#modelColor').css('background-color', '#' + modelColor);

    if (Cookies.get('showWireframe')) {
      showWireframe = parseInt(Cookies.get('showWireframe')) != 0;
    } else {
      showWireframe = false;
    }
    $('#showWireframe').prop('checked', showWireframe);

    if (Cookies.get('isEditorText')) {
      setEditor(parseInt(Cookies.get('isEditorText')) != 0);
    }

    if (isEditorText) {
      $("#isEditorText").prop('checked', true);
    } else {
      $("#isEditorBlocks").prop('checked', true);
    }

    if (Cookies.get('gridExtent')) {
      gridExtent = parseFloat(Cookies.get('gridExtent'));
    }
    $('#gridSpacing').val(gridSpacing + '');

    if (Cookies.get('gridSpacing')) {
      gridSpacing = parseFloat(Cookies.get('gridSpacing'));
    }
    $('#gridExtent').val(gridExtent + '');

    if (Cookies.get('nSecondsTillPreview')) {
      nSecondsTillPreview = parseFloat(Cookies.get('nSecondsTillPreview'));
    }

    $('#nSecondsTillPreview').val(nSecondsTillPreview + '');
    $('#nSecondsTillPreview').change(function () {
      nSecondsTillPreview = parseFloat($('#nSecondsTillPreview').val());
      isNSecondsTillPreviewChanged = true;
      textEditor.getSession().off('change', onEditorChange);
      if (preview) {
        clearTimeout(preview); 
      }
      preview = undefined;
      schedulePreview();
    });
    schedulePreview();

    // WebGL-dependent stuff.
    if (!hasWebGL()) {
      log('No WebGL.');
    } else {
      updateCulling();

      swatch = $('#modelColor').ColorPicker({
        flat: false,
        color: modelColor,
        onSubmit: function(hsb, hex, rgb) {
          modelColor = hex;
          isModelColorChanged = true;
          $('#modelColor').css('background-color', '#' + modelColor);
          run(GeometryMode.SURFACE);
        }
      });

      if (Cookies.get('axisX')) {
        if (parseInt(Cookies.get('axisX')) != 0) {
          $('#axisX').prop('checked', true);
          generateAxis(0);
        }
      }

      if (Cookies.get('axisY')) {
        if (parseInt(Cookies.get('axisY')) != 0) {
          $('#axisY').prop('checked', true);
          generateAxis(1);
        }
      }

      if (Cookies.get('axisZ')) {
        if (parseInt(Cookies.get('axisZ')) != 0) {
          $('#axisZ').prop('checked', true);
          generateAxis(2);
        }
      }

      if (Cookies.get('gridX')) {
        if (parseInt(Cookies.get('gridX')) != 0) {
          $('#gridX').prop('checked', true);
          generateGrid(0);
        }
      }

      if (Cookies.get('gridY')) {
        if (parseInt(Cookies.get('gridY')) != 0) {
          $('#gridY').prop('checked', true);
          generateGrid(1);
        }
      }

      if (Cookies.get('gridZ')) {
        if (parseInt(Cookies.get('gridZ')) != 0) {
          $('#gridZ').prop('checked', true);
          generateGrid(2);
        }
      }
    }

    // Only save cookies if they were successfully loaded.
    $(window).unload(function() {
      saveInCookies();
      save();
    });
  });

  $('#clear').click(function() {
    log('');
    textEditor.focus();
  });

  $('#smaller').click(function() {
    hideMenus();
    isFontSizeChanged = true;
    fontSize -= 2;
    $('#menu input').each(function (i) {
      this.style.fontSize = (parseInt($(this).css('font-size')) - 1) + 'px';
    });
    textEditor.setFontSize(fontSize);
    $('#console')[0].style.fontSize = fontSize + 'px';
    textEditor.focus();
  });

  $('#bigger').click(function() {
    hideMenus();
    isFontSizeChanged = true;
    fontSize += 2;
    $('#menu input').each(function (i) {
      this.style.fontSize = (parseInt($(this).css('font-size')) + 1) + 'px';
    });
    textEditor.setFontSize(fontSize);
    $('#console')[0].style.fontSize = fontSize + 'px';
    textEditor.focus();
  });

  $('#fit').click(function() {
    hideMenus();
    fit();
    textEditor.focus();
  });

  $('input[type=radio][name=editorMode]').change(function() {
    var editorMode = $(this).val();
    setEditor(editorMode != "Blocks");
  });

  $('#showHeadings').click(function() {
    isShowHeadingsChanged = true;
    showHeadings = this.checked;
    textEditor.focus();
    run(GeometryMode.PATH);
  });

  $('#isFlatShaded').click(function() {
    isFlatShadedChanged = true;
    isFlatShaded = this.checked;
    textEditor.focus();
    render();
  });

  $('#showCounterclockwise').click(function() {
    isShowCounterclockwiseChanged = true;
    showCounterclockwise = this.checked;
    textEditor.focus();
    updateCulling();
  });

  $('#showClockwise').click(function() {
    isShowClockwiseChanged = true;
    showClockwise = this.checked;
    textEditor.focus();
    updateCulling();
  });

  var red = 0xB80000;
  var green = 0x006100;
  var blue = 0x0000FF;
  var colors = [red, green, blue];
  var axes = new Array(3);
  var grids = new Array(3);

  function setEditor(isText) {
    if (isEditorText == isText) return;
    isEditorText = isText;
    isEditorTextChanged = true;

    if (isEditorText) {
      $('#blocksEditor').hide();
      $('#textEditor').show();
    } else {
      $('#textEditor').hide();
      $('#blocksEditor').show();

      if (!workspace) {
        workspace = Blockly.inject('blocksCanvas', {toolbox: document.getElementById('toolbox')});
        workspace.addChangeListener(function() {
          var code = Blockly.Madeup.workspaceToCode(workspace);
          textEditor.setValue(code);
          log(code);
        });
        if (Cookies.get('lastBlocks')) {
          var xml = Blockly.Xml.textToDom(Cookies.get('lastBlocks'));
          Blockly.Xml.domToWorkspace(workspace, xml);
        }
      } else {
      }
    }

    resize();
    Blockly.fireUiEvent(window, 'resize');
  }

  function generateAxis(d) {
    if (axes[d]) {
      removeAxis(d);
    }

    var geometry = new THREE.Geometry();
    var a = new THREE.Vector3(0, 0, 0);
    var b = new THREE.Vector3(0, 0, 0);
    a.setComponent(d, -gridExtent);
    b.setComponent(d, gridExtent);
    geometry.vertices.push(a);
    geometry.vertices.push(b);
    axes[d] = new THREE.Line(geometry, new THREE.LineBasicMaterial({
      color: colors[d],
      linewidth: 5
    }), THREE.LinePieces);
    glyphScene.add(axes[d]);
    render();
  }

  function removeAxis(d) {
    if (axes[d]) {
      glyphScene.remove(axes[d]);
      axes[d] = null;
      render();
    }
  }

  function toggleAxis(d) {
    return function() {
      isAxisChanged[d] = true;
      if (this.checked) {
        generateAxis(d);
      } else {
        removeAxis(d);
      }
    }
  }

  $('#axisX').click(toggleAxis(0));
  $('#axisY').click(toggleAxis(1));
  $('#axisZ').click(toggleAxis(2));
  
  function generateGrid(d) {
    if (grids[d]) {
      removeGrid(d);
    }

    var geometry = new THREE.Geometry();
    for (var i = -gridExtent; i <= gridExtent; i += gridSpacing) {
      var a = new THREE.Vector3(0, 0, 0);
      var b = new THREE.Vector3(0, 0, 0);
      a.setComponent((d + 1) % 3, -gridExtent);
      b.setComponent((d + 1) % 3, gridExtent);
      a.setComponent((d + 2) % 3, i);
      b.setComponent((d + 2) % 3, i);
      geometry.vertices.push(a);
      geometry.vertices.push(b);

      a = new THREE.Vector3(0, 0, 0);
      b = new THREE.Vector3(0, 0, 0);
      a.setComponent((d + 1) % 3, i);
      b.setComponent((d + 1) % 3, i);
      a.setComponent((d + 2) % 3, -gridExtent);
      b.setComponent((d + 2) % 3, gridExtent);
      geometry.vertices.push(a);
      geometry.vertices.push(b);
    }

    grids[d] = new THREE.Line(geometry, new THREE.LineBasicMaterial({color: colors[d], linewidth: 1}), THREE.LinePieces);
    glyphScene.add(grids[d]);
    render();
  }

  function removeGrid(d) {
    if (grids[d]) {
      glyphScene.remove(grids[d]);
      grids[d] = null;
      render();
    }
  }

  function toggleGrid(d) {
    return function() {
      isGridChanged[d] = true;
      if (this.checked) {
        generateGrid(d);
      } else {
        removeGrid(d);
      }
    }
  }

  $('#gridX').click(toggleGrid(0));
  $('#gridY').click(toggleGrid(1));
  $('#gridZ').click(toggleGrid(2));

  $('#gridExtent').change(function() {
    isGridExtentChanged = true;
    gridExtent = parseFloat($(this).val());
    for (var d = 0; d < 3; ++d) {
      if (axes[d]) {
        generateAxis(d);
      }
      if (grids[d]) {
        generateGrid(d);
      }
    }
  });

  $('#gridSpacing').change(function() {
    isGridSpacingChanged = true;
    gridSpacing = parseFloat($(this).val());
    for (var d = 0; d < 3; ++d) {
      if (grids[d]) {
        generateGrid(d);
      }
    }
  });

  $('#autopreview').click(function() {
    if (this.checked) {
      $('#nSecondsTillPreview').prop('disabled', false);
      schedulePreview();
    } else {
      $('#nSecondsTillPreview').prop('disabled', true);
      textEditor.getSession().off('change', onEditorChange);
      if (preview) {
        clearTimeout(preview); 
      }
      preview = undefined;
    }
  });

  $('#showWireframe').click(function() {
    isShowWireframeChanged = true;
    showWireframe = this.checked;
    run(GeometryMode.SURFACE);
  });

  function toggleMenu(id) {
    var buttonID = '#toggle' + id.charAt(1).toUpperCase() + id.substring(2);

    hideMenus(id);
    $(id).css('top', $(buttonID).offset().top + $(buttonID).innerHeight(true)) - 8; 
    $(id).css('left', $(buttonID).offset().left); 

    // Hilite the menu if it's opening.
    if (!$(id).is(":visible")) {
      $(buttonID).css('background-color', '#333333');
    }

    $(id).slideToggle('fast', function() {
      // Unhilite the menu once it's closed.
      if (!$(id).is(":visible")) {
        $(buttonID).css('background-color', '#000000');
      }
    });
  }

  $('#toggleFilePopup').click(function() {
    toggleMenu('#filePopup');
    populateFileMenu();
  });

  $('#toggleEditorPopup').click(function() {
    toggleMenu('#editorPopup');
  });

  $('#toggleGridPopup').click(function() {
    toggleMenu('#gridPopup');
  });

  $('#toggleDisplayPopup').click(function() {
    toggleMenu('#displayPopup');
  });

  $('#download').click(function() {
    hideMenus();
    $('#source').val(getSource());
    $('#downloader').submit();
    textEditor.focus();
  });

  $('#solidify').click(function() {
    hideMenus();
    log('Running...'); 
    saveInCookies();
    run(GeometryMode.SURFACE);
    textEditor.focus();
  });

  $('#fileSaveAs').click(function() {
    hideMenus();
    var name = prompt('Save under what name?');
    if (name != null) {
      mupName = name;
      save();
      updateTitle();
    }
  });

  $('#exportArchive').click(function() {
    hideMenus();

    var archive = new Object;
    for (var mup in window.localStorage) {
      if (mup != 'untitled') {
        var file = JSON.parse(window.localStorage.getItem(mup));
        archive[mup] = file;
      }
    }

    generateDownloadable('mups_archive_' + yyyymmdd() + '.json', JSON.stringify(archive));
  });

  $('#importForm').hide();
  $('#importArchive').click(function() {
    $('#importForm').show();
  });
  $('#cancelImport').click(function() {
    $('#importForm').hide();
  });
  $('#archive').change(function() {
    var archive = this.files[0];
    $('#importForm').hide();

    var reader = new FileReader();
    reader.onload = function(e) {
      var mups = JSON.parse(e.target.result);
      for (mup in mups) {
        window.localStorage.setItem(mup, JSON.stringify(mups[mup]));
        populateFileMenu();
      }
    };
    reader.readAsText(archive);
  });

  $('#fileClose').click(function() {
    hideMenus();
    save();
    load('untitled');
  });

  $('#fileDelete').click(function() {
    hideMenus();
    save();
    var ok = confirm('Delete ' + mupName + '?');
    if (ok) {
      window.localStorage.removeItem(mupName);
      mupName = null;
      load('untitled');
    }
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

  if (hasWebGL()) {
    init();
    animate();
  }
});

var onEditorChange = function(delta) {
  if (preview) {
    clearTimeout(preview); 
  }
  preview = setTimeout(function() {
    run(GeometryMode.PATH);
  }, nSecondsTillPreview * 1000);
}

var preview = undefined;
function schedulePreview() {
  textEditor.getSession().on('change', onEditorChange);
}

function load(mup) {
  console.log('new mup: ' + mup);

  hideMenus();
  save();
  mupName = mup;

  for (var i = 0; i < meshes.length; ++i) {
    modelScene.remove(meshes[i]);
  }
  if (renderer) render();

  var json = window.localStorage.getItem(mup); 
  if (json) {
    var file = JSON.parse(window.localStorage.getItem(mup));
    textEditor.session.setValue(file.source, -1);
  }

  // TODO toggle modes
  updateTitle();
}

function save() {
  if (mupName != null) {
    console.log('saving ' + mupName);
    var file = {
      'mode' : isEditorText ? 'text' : 'blocks',
      'updated_at' : new Date().toString(),
      'source' : getSource()
    };
    window.localStorage.setItem(mupName, JSON.stringify(file));
  }
}

function getSource() {
  return textEditor.getValue();
}

var allGeometry = undefined;
function run(mode) {
  $.ajax({
    type: 'POST',
    url: 'interpret.php',
    data: JSON.stringify(
      {
        source: getSource(),
        extension: 'json',
        mode: mode,
        shadingMode: isFlatShaded ? 'FLAT' : 'SMOOTH'
      }
    ),
    contentType: 'application/json; charset=utf-8',
    dataType: 'json',
    success: function(data) {
      var sansDebug = data['output'].replace(/^Debug:.*$\n/gm, '');
      if (sansDebug.length > 0) {
        console.log(sansDebug);
      }

      if (data['exit_status'] == 0) {
        for (var i = 0; i < meshes.length; ++i) {
          modelScene.remove(meshes[i]);
        }
        meshes = [];

        log(sansDebug);
        
        if (mode == GeometryMode.SURFACE) {
          var loader = new THREE.JSONLoader();
          var model = loader.parse(JSON.parse(data['model']));
          var material = new THREE.MeshLambertMaterial({color: parseInt(modelColor, 16), wireframe: showWireframe, wireframeLinewidth: 5});
          meshes[0] = new THREE.Mesh(model.geometry, material);
          model.geometry.computeFaceNormals();
          model.geometry.computeVertexNormals();
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
            if (showHeadings && nvertices > 0) {
              var m = paths[pi].orientation;
              
              var g2 = new THREE.Geometry();

              g2.vertices.push(
                new THREE.Vector3(-0.5, 0,  0),
                new THREE.Vector3( 0.5, 0,  0),
                new THREE.Vector3( 0, 0, -1),
                new THREE.Vector3( 0, 0.3, -0.2)
              );

              g2.faces.push(
                new THREE.Face3(1, 0, 2),
                new THREE.Face3(0, 1, 3),
                new THREE.Face3(3, 2, 0),
                new THREE.Face3(1, 2, 3)
              );

              var mm = new THREE.Matrix4().set(m[0], m[1], m[2], m[3], m[4], m[5], m[6], m[7], m[8], m[9], m[10], m[11], m[12], m[13], m[14], m[15]);
              var mmm = new THREE.Matrix4().getInverse(mm);
              g2.applyMatrix(mmm);
              var offset = new THREE.Vector3(paths[pi].vertices[paths[pi].vertices.length - 1][0], paths[pi].vertices[paths[pi].vertices.length - 1][1], paths[pi].vertices[paths[pi].vertices.length - 1][2]);
              g2.applyMatrix(new THREE.Matrix4().makeTranslation(offset.x, offset.y, offset.z));

              g2.computeFaceNormals();
              allGeometry.vertices = allGeometry.vertices.concat(g2.vertices);

              meshes[meshes.length] = new THREE.Mesh(g2, new THREE.MeshLambertMaterial({
                color: 0x0000ff,
              }));
              modelScene.add(meshes[meshes.length - 1]);
            }
          }

          meshes[meshes.length] = new THREE.PointCloud(dotsGeometry, new THREE.PointCloudMaterial({
            color: 0x000000,
            size: 8,
            sizeAttenuation: false
          }));
        }

        for (var mi = 0; mi < meshes.length; ++mi) {
          /* meshes[mi].geometry.mergeVertices(); */
          /* meshes[mi].geometry.computeFaceNormals(); */
          /* meshes[mi].geometry.computeVertexNormals(); */
          modelScene.add(meshes[mi]);
        }
        updateCulling();
        render();
      } else if (data['exit_status'] == 22) {
        log(data['output'] + '\nYour model was taking a long time to build. It felt like it was never going to finish! So, I stopped trying. Sorry.');
      } else {
        log(sansDebug);
      }
    },
    failure: function(errorMessage) {
      console.log('Failure. :(');
    }
  });
}

function fit() {
  if (allGeometry === undefined) {
    return;
  }

  allGeometry.computeBoundingBox();

  var bounds = allGeometry.boundingBox;
  var centroid = bounds.center();

  var xform = new THREE.Matrix4().makeTranslation(-centroid.x, -centroid.y, -centroid.z);
  modelScene.matrix = xform;

  var verticalFitZ = bounds.max.y / Math.tan(camera.fov * Math.PI / 180.0 * 0.5);
  var fovX = 2 * Math.atan(Math.tan(camera.fov * Math.PI / 180.0 * 0.5) * camera.aspect);
  var horizontalFitZ = bounds.max.x / Math.tan(fovX * 0.5);

  controls.reset(); 
  camera.position.z = bounds.max.z + (verticalFitZ > horizontalFitZ ? verticalFitZ : horizontalFitZ);
  camera.updateProjectionMatrix();
}

function log(message) {
  // $1 is the whole source span. $2 is the start. $3 is the end.
  var linkMessage = message.replace(/^((\d+)\((\d+)(?:-(\d+))?\))/gm, function(match, full, startLine, startIndex, stopIndex) {
    return '<div style="color: #FF9999; display: inline;">Error on <a style="text-decoration: underline;" onclick="javascript:highlight(' + startIndex + ', ' + stopIndex + ')" class="srclink">line ' + startLine + /*':' + startIndex + ':' + stopIndex +*/ '</a></div>';
  });

  $('#console #message').html(linkMessage.replace(/\n/g, '<br/>'));
}

function resize() {
  if (!initialized) return;

  controls.handleResize();

  var width = window.innerWidth - $('#left').width();
  var height = window.innerHeight;
  if (renderer) renderer.setSize(width, height);
  camera.aspect = width / height;
  camera.updateProjectionMatrix();
  $("#textEditor").height($(document).height() - $('#menu').height() - $('#console').height());
  $("#blocksEditor").height($(document).height() - $('#menu').height() - $('#console').height());

  var blocklyArea = document.getElementById('blocksEditor');
  var blocklyDiv = document.getElementById('blocksCanvas');
  // Position blocklyDiv over blocklyArea.
  blocklyDiv.style.left = $('#blocksEditor').position().left + 'px';
  blocklyDiv.style.top = $('#blocksEditor').position().top + 'px';
  blocklyDiv.style.height = blocklyArea.offsetHeight + 'px';
  $("#blocksCanvas").width(blocklyArea.offsetWidth);

  textEditor.resize();
  if (renderer) render();
}

function highlight(startIndex, stopIndex) {
  var doc = textEditor.getSession().getDocument();
  var lines = doc.getAllLines();
  //console.log(lines);

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

  textEditor.getSelection().setSelectionRange(new Range(start.row, start.column, stop.row, stop.column + 1));
}

function updateCulling() {
  var mode;
  if (showClockwise && showCounterclockwise) {
    mode = THREE.CullFaceNone;
  } else if (showCounterclockwise) {
    mode = THREE.CullFaceBack;
  } else if (showClockwise) {
    mode = THREE.CullFaceFront;
  } else {
    mode = THREE.CullFaceFrontBack;
  }
  renderer.setFaceCulling(mode, THREE.FrontFaceDirectionCCW);
  render();
  //run(GeometryMode.SURFACE);
}

function init() {
  camera = new THREE.PerspectiveCamera(45.0, 1.0, 0.1, 10000.0);
  camera.position.z = 30;

  var glcanvas = $("#glcanvas");
  renderer = new THREE.WebGLRenderer({antialias: true});

  // Scratch out upstream implementation, which affects global culling state
  // that I rely on.
  renderer.setMaterialFaces = function(material) {}

  renderer.setClearColor(0xFFFFFF, 1);
  document.getElementById("glcanvas").appendChild(renderer.domElement);

  // Hide menus when clicking off-menu. This is not UFW. I need to explicitly
  // omit the parameters.
  document.getElementById('left').addEventListener('mousedown', function() {hideMenus();});
  renderer.domElement.addEventListener('mousedown', function() {hideMenus();});

  // controls = new THREE.OrbitControls(camera, renderer.domElement);  
  controls = new THREE.TrackballControls(camera, renderer.domElement);
  controls.addEventListener('change', render);

  controls.rotateSpeed = 3.0;
  controls.zoomSpeed = 1.2;
  controls.panSpeed = 0.8;
  controls.noZoom = false;
  controls.noPan = false;
  controls.staticMoving = true;

  // pointerScene = new THREE.Scene(); 
  modelScene = new THREE.Scene();
  modelScene.matrixAutoUpdate = false;

  glyphScene = new THREE.Scene();
  modelScene.add(glyphScene);

  overallScene = new THREE.Scene();
  overallScene.add(modelScene);
  overallScene.add(camera);

  var radius = 50,
      segments = 16,
      rings = 16;

  // create a point light
  var pointLight = new THREE.PointLight(0xFFFFFF);

  // set its position
  pointLight.position.x = 10;
  pointLight.position.y = 50;
  pointLight.position.z = 130;

  // add to the scene
  camera.add(pointLight);

  updateCulling();

  window.addEventListener('resize', resize);
  initialized = true;
  resize();
}

function animate() {
  requestAnimationFrame(animate);
  controls.update();
}

function render() {
  renderer.render(overallScene, camera);
}

function hideMenus(exceptID) {
  if (exceptID === undefined) {
    $('.popups').hide();
    $('.togglePopup').css('background-color', '#000000');
  } else {
    $('.popups').not(exceptID).hide();
    $('.togglePopup').not('#toggle' + exceptID.charAt(1).toUpperCase() + exceptID.substring(2)).css('background-color', '#000000');
  }

  if (swatch) {
    swatch.ColorPickerHide();
  }
}
