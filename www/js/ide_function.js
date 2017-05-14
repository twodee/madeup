function hasWebGL() {
  try {
    var canvas = document.createElement("canvas");
    return !!window.WebGLRenderingContext && (canvas.getContext("webgl") || canvas.getContext("experimental-webgl"));
  } catch(e) { 
    return false;
  } 
}

function showConsole(isShown) {
  $('#showConsole').prop('checked', isShown);
  if (isShown) {
    $('#console').height(100);
    resize();
  } else {
    $('#console').height(0);
    resize();
  }
}

var onMouseDown = (function() {
  var x = 0;
  var y = 0;
  var dx = 0;
  var dy = 0;

  function onMouseMove(event) {
    dx = event.x - x;
    dy = event.y - y;
    x = event.x;
    y = event.y;
  }

  function onMouseUp(event) {
    var velocity = Math.sqrt(dx * dx + dy * dy);

    if (event.button == 0 && !event.altKey && !event.metaKey && !event.ctrlKey && velocity > 3) {
      controls.staticMoving = false;
      controls.noPan = true;
      controls.noZoom = true;
      controls.rotateSpeed = velocity * 0.01;
    } else {
      controls.staticMoving = true;
    }

    renderer.domElement.removeEventListener('mousemove', onMouseMove);
    renderer.domElement.removeEventListener('mouseup', onMouseUp);
  }
  
  function onMouseDown(event) {
    controls.staticMoving = true;
    controls.noPan = false;
    controls.noZoom = false;
    x = event.x;
    y = event.y;
    dx = 0;
    dy = 0;
    controls.rotateSpeed = 3.0;
    renderer.domElement.addEventListener('mousemove', onMouseMove, false);
    renderer.domElement.addEventListener('mouseup', onMouseUp, false);
  };

  return onMouseDown;
})();

// Prime Blockly with some builtin variables. There's no API exposed for this,
// but Blockly.Variables.allVariables walks the blocks in the workspace and
// returns the names of all variables. Our builtin variables don't necessarily
// appear in the workspace (yet), so we hijack this function and add them
// manually.
(function() {
  var oldAllVariables = Blockly.Variables.allVariables;
  Blockly.Variables.allVariables = function(root) {
    var vars = oldAllVariables.call(this, root);
    vars.push('nsides');
    vars.push('.rgb');
    vars.push('.radius');
    vars.push('.innerRadius');
    vars.push('.outerRadius');
    return vars;
  };
})();

// Warn on leaving the page if there are unsaved changes. Downloading triggers
// this, even though we're not leaving the page, so we add a special flag to
// filter out these events.
window.onbeforeunload = function() {
  if (!isDownloading && mupName && isSourceDirty) {
    return 'You have unsaved changes.';
  }
  isDownloading = false;
};

var lastBlocks = null;
function onBlocksChanged() {
  var xml = Blockly.Xml.workspaceToDom(blocklyWorkspace);
  var currentBlocks = Blockly.Xml.domToText(xml);
  isSourceDirty = lastBlocks != currentBlocks;
  updateTitle();
  if (isAutopathify) {
    run(getSource(), GeometryMode.PATH);
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

function getBlocklyProcedureFormals(name) {
  // allProcedures gives back [procedures with return, procedures without
  // return]. We only have the latter.

  var procs = Blockly.Procedures.allProcedures(blocklyWorkspace)[0];
  // TODO

  // Find the procedure in the list with the given name.
  for (var i = 0; i < procs.length; ++i) {
    if (procs[i][0] == name) {
      return procs[i][1];
    }
  }

  throw 'No procedure named ' + name;
}

function populateFileMenu() {
  var list = '';
  var mups = Object.keys(window.localStorage).sort();
  for (var i = 0; i < mups.length; ++i) {
    var mup = mups[i];
    if (mup != 'untitled') {
      list += '<a href="#" class="menu-link" onclick="load(\'' + mup.replace(/'/g, '\\&#39;').replace(/"/g, '\\&quot;') + '\')">' + mup + '</a><br/>';
    }
  }
  $('#mups').html(list);
}

function enableDownload(enable) {
  if (enable) {
    $('#download').prop('disabled', false);
    $('#download').removeClass('unclickable').addClass('clickable');
  } else {
    $('#download').prop('disabled', true);
    $('#download').removeClass('clickable').addClass('unclickable');
  }
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

var isDownloading = false;
var initialized = false;
var mupName = null;
var overallScene;
var modelScene;
var glyphScene;
var renderer, camera, controls;
var meshes = [];
var isAutopathify = true;
var nSecondsTillAutopathify = 1.0;
var showMode = 'solid';
var showHeadings = true;
var lightBothSides = true;
var blocklyWorkspace = null;
var fontSize = 14;
var gridSpacing = 1.0;
var gridExtent = 10.0;
var isFlatShaded = true;
var isAutorotate = false;
var isEditorText = true;
var tree = null;
var preview = undefined;
var isThemeDark = true;
var isSourceDirty = false;
var showPoints = true;
var pathifyNodeSize = 0.3;
var pathifyLineSize = 6;

var isThemeDarkChanged = false;
var isShowModeChanged = false;
var isAxisChanged = [false, false, false];
var isGridChanged = [false, false, false];
var isAutopathifyChanged = false;
var isNSecondsTillAutopathifyChanged = false;
var isPathifyNodeSizeChanged = false;
var isPathifyLineSizeChanged = false;
var isShowHeadingsChanged = false;
var isFontSizeChanged = false;
var isGridSpacingChanged = false;
var isGridExtentChanged = false;
var isLightBothSidesChanged = false;
var isFlatShadedChanged = false;
var isAutorotateChanged = false;
var isShowPointsChanged = false;

function updateTitle() {
  // The name won't be set in certain situations -- namely, when it's embedded.
  // Showing it as dirty makes no sense, because it's not a file.
  if (mupName != null) {
    document.title = (isSourceDirty ? '*' : '') + 'Madeup: ' + mupName;
    document.title = (isSourceDirty ? '*' : '') + 'Madeup: ' + mupName;
  }
  if (isSourceDirty) {
    $('#fileSave').prop('disabled', false);
    $('#fileSave').removeClass('unclickable').addClass('clickable');
    $('#dirty-suffix').text('*');
  } else {
    $('#fileSave').prop('disabled', true);
    $('#fileSave').removeClass('clickable').addClass('unclickable');
    $('#dirty-suffix').text('');
  }
}

function saveInCookies() {
  // Cookies.set('lastMup', mupName); 
 
  // Only store a cookie if a setting has changed. If we unconditionally stored
  // these, then updates to the default value would not be seen by users, as
  // the old defaults persisted in the cookies would override the new ones.
  if (isShowHeadingsChanged) Cookies.set('showHeadings', showHeadings ? 1 : 0);
  if (isLightBothSidesChanged) Cookies.set('lightBothSides', lightBothSides ? 1 : 0);
  if (isShowModeChanged) Cookies.set('showMode', showMode);
  if (isFlatShadedChanged) Cookies.set('isFlatShaded', isFlatShaded ? 1 : 0);
  if (isAutorotateChanged) Cookies.set('isAutorotate', isAutorotate ? 1 : 0);
  if (isThemeDarkChanged) Cookies.set('isThemeDark', isThemeDark ? 1 : 0);
  if (isAxisChanged[0]) Cookies.set('axisX', $('#axisX').prop('checked') ? 1 : 0);
  if (isAxisChanged[1]) Cookies.set('axisY', $('#axisY').prop('checked') ? 1 : 0);
  if (isAxisChanged[2]) Cookies.set('axisZ', $('#axisZ').prop('checked') ? 1 : 0);
  if (isGridChanged[0]) Cookies.set('gridX', $('#gridX').prop('checked') ? 1 : 0);
  if (isGridChanged[1]) Cookies.set('gridY', $('#gridY').prop('checked') ? 1 : 0);
  if (isGridChanged[2]) Cookies.set('gridZ', $('#gridZ').prop('checked') ? 1 : 0);
  if (isAutopathifyChanged) Cookies.set('isAutopathify', $('#autopathify').prop('checked') ? 1 : 0);
  if (isNSecondsTillAutopathifyChanged) Cookies.set('nSecondsTillAutopathify', nSecondsTillAutopathify);
  if (isPathifyNodeSizeChanged) Cookies.set('pathifyNodeSize', pathifyNodeSize);
  if (isPathifyLineSizeChanged) Cookies.set('pathifyLineSize', pathifyLineSize);
  if (isGridSpacingChanged) Cookies.set('gridSpacing', gridSpacing);
  if (isGridExtentChanged) Cookies.set('gridExtent', gridExtent);
  if (isShowPointsChanged) Cookies.set('showPoints', showPoints ? 1 : 0);

  // Embedded views have differing size requirements, so we don't try to
  // preserve their values long term.
  if (!isEmbedded) {
    if (isFontSizeChanged) Cookies.set('fontSize', fontSize);
    Cookies.set('leftWidth', $('#left').width());
    Cookies.set('consoleHeight', $('#console').height());
  }

  if (blocklyWorkspace) {
    var xml = Blockly.Xml.workspaceToDom(blocklyWorkspace);
    var xmlText = Blockly.Xml.domToText(xml);
    Cookies.set('lastBlocks', xmlText);
  }

  // Changes have been committed, so let's reset the dirty flags.
  isFontSizeChanged = false;
  isShowHeadingsChanged = false;
  isLightBothSidesChanged = false;
  isShowClockwiseChanged = false;
  isFlatShadedChanged = false;
  isAutorotateChanged = false;
  isShowModeChanged = false;
  isThemeDarkChanged = false;
  isShowHeadingsChanged = false;
  isNSecondsTillAutopathifyChanged = false;
  isPathifyNodeSizeChanged = false;
  isPathifyLineSizeChanged = false;
  isGridSpacingChanged = false;
  isGridExtentChanged = false;
  isShowPointsChanged = false;
  for (var d = 0; d < 3; ++d) {
    isAxisChanged[d] = false;
    isGridChanged[d] = false;
  }
}

$(document).ready(function() {
  $(document).tooltip({
    show: {
      effect: 'slideDown',
      delay: 2000
    },
    position: {
      my: 'center top',
      at: 'center bottom'
    }
  });

  populateFileMenu();

  // When we are embedded in an iframe, the wheel event will cause the
  // embedding context to scroll. That's not what we want, so we capture and
  // squelch any wheel events.
  if (isEmbedded) {
    document.body.addEventListener('wheel', function (e) {
      e.preventDefault();
    });
    // TODO: confirm that this works as intended.
  }

  $(window).load(function() {
    $('#textEditor textarea').addClass('mousetrap');
    Cookies.defaults = {
      expires: 10 * 365
    };

    if (!isEmbedded) {
      if (Cookies.get('leftWidth')) {
        $('#left').width(Cookies.get('leftWidth'));
        resize();
      }

      if (Cookies.get('consoleHeight')) {
        $('#console').height(Cookies.get('consoleHeight'));
        $('#showConsole').prop('checked', $('#console').height() != 0);
        resize();
      }

      load('untitled');
    } else if (isPresenting) {
      showConsole(false);
      $('#left').width(375);
      resize();
    } else {
      $('#left').width(250);
      resize();
    }

    if (!isEmbedded && !isBlocky && Cookies.get('fontSize')) {
      fontSize = parseInt(Cookies.get('fontSize'));
    } else if (isBlocky) {
      fontSize = 24;
    } else if (isPresenting) {
      fontSize = 28;
    } else {
      fontSize = 20;
    }
    setFontSize(fontSize);

    if (Cookies.get('showHeadings')) {
      showHeadings = parseInt(Cookies.get('showHeadings')) != 0;
    }
    $('#showHeadings').prop('checked', showHeadings);

    if (Cookies.get('isFlatShaded')) {
      isFlatShaded = parseInt(Cookies.get('isFlatShaded')) != 0;
    }
    $('#isFlatShaded').prop('checked', isFlatShaded);

    if (Cookies.get('isAutorotate')) {
      isAutorotate = parseInt(Cookies.get('isAutorotate')) != 0;
    }
    $('#isAutorotate').prop('checked', isAutorotate);
    enableAutorotate(isAutorotate);

    if (Cookies.get('lightBothSides')) {
      lightBothSides = parseInt(Cookies.get('lightBothSides')) != 0;
    }
    $('#lightBothSides').prop('checked', lightBothSides);

    if (Cookies.get('showMode')) {
      showMode = Cookies.get('showMode');
    } else {
      showMode = 'solid';
    }
    $('#showMode').val(showMode);

    if (isEditorText) {
      $("#isEditorText").prop('checked', true);
    } else {
      $("#isEditorBlocks").prop('checked', true);
    }

    if (Cookies.get('isThemeDark')) {
      setTheme(parseInt(Cookies.get('isThemeDark')) != 0);
    }

    if (isThemeDark) {
      $("#isDark").prop('checked', true);
    } else {
      $("#isLight").prop('checked', true);
    }

    if (Cookies.get('showPoints')) {
      showPoints = parseInt(Cookies.get('showPoints')) != 0;
    }
    $("#showPoints").prop('checked', showPoints);

    if (Cookies.get('gridExtent')) {
      gridExtent = parseFloat(Cookies.get('gridExtent'));
    }
    $('#gridSpacing').val(gridSpacing + '');

    if (Cookies.get('gridSpacing')) {
      gridSpacing = parseFloat(Cookies.get('gridSpacing'));
    }
    $('#gridExtent').val(gridExtent + '');

    if (Cookies.get('isAutopathify')) {
      isAutopathify = parseInt(Cookies.get('isAutopathify')) != 0;
    }
    $("#autopathify").prop('checked', isAutopathify);

    if (Cookies.get('nSecondsTillAutopathify')) {
      nSecondsTillAutopathify = parseFloat(Cookies.get('nSecondsTillAutopathify'));
    }

    // Node size
    if (Cookies.get('pathifyNodeSize')) {
      pathifyNodeSize = parseFloat(Cookies.get('pathifyNodeSize'));
    }

    $('#pathify-node-size').val(pathifyNodeSize);
    $('#pathify-node-size')[0].oninput = function () {
      pathifyNodeSize = parseFloat($('#pathify-node-size').val());
      run(getSource(), GeometryMode.PATH);
    };

    // Line size
    if (Cookies.get('pathifyLineSize')) {
      pathifyLineSize = parseFloat(Cookies.get('pathifyLineSize'));
    }

    $('#pathify-line-size').val(pathifyLineSize);
    $('#pathify-line-size')[0].oninput = function () {
      pathifyLineSize = parseFloat($('#pathify-line-size').val());
      run(getSource(), GeometryMode.PATH);
    };

    $('#nSecondsTillAutopathify').val(nSecondsTillAutopathify + '');
    $('#nSecondsTillAutopathify').change(function () {
      if (!isAutopathify) return;

      nSecondsTillAutopathify = parseFloat($('#nSecondsTillAutopathify').val());
      isNSecondsTillAutopathifyChanged = true;
      textEditor.getSession().off('change', onEditorChange);
      if (preview) {
        clearTimeout(preview); 
      }
      preview = undefined;
      schedulePathify();
    });
    schedulePathify();

    // WebGL-dependent stuff.
    if (!hasWebGL()) {
      log('No WebGL.');
    } else {
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
      if (mupName && isSourceDirty && confirm('Save changes to ' + mupName + '?')) {
        save();
      }
    });
  });

  $('#smaller').click(function() {
    setFontSize(fontSize - 2);
    resize();
    // Blockly.fireUiEvent(window, 'resize'); TODO
  });

  $('#bigger').click(function() {
    setFontSize(fontSize + 2);
    resize();
    // Blockly.fireUiEvent(window, 'resize'); TODO
  });

  $('#fit-button').click(function() {
    fit();
    focusEditor();
  });

  $('#cameraLeft').click(function() {
    viewFrom(0, -1);
  });

  $('#cameraRight').click(function() {
    viewFrom(0, 1);
  });

  $('#cameraBottom').click(function() {
    viewFrom(1, -1);
  });

  $('#cameraTop').click(function() {
    viewFrom(1, 1);
  });

  $('#cameraBack').click(function() {
    viewFrom(2, -1);
  });

  $('#cameraFront').click(function() {
    viewFrom(2, 1);
  });

  $('input[type=radio][name=editorMode]').change(function() {
    var editorMode = $(this).val();
    console.log(editorMode);
    setEditor(editorMode != "Blocks");
  });

  $('input[type=radio][name=theme]').change(function() {
    var isDark = $(this).val() == 'isDark';
    setTheme(isDark);
  });

  $('#showConsole').click(function() {
    showConsole(this.checked);
  });

  $('#showHeadings').click(function() {
    isShowHeadingsChanged = true;
    showHeadings = this.checked;
    run(getSource(), GeometryMode.PATH);
  });

  $('#isFlatShaded').click(function() {
    isFlatShadedChanged = true;
    isFlatShaded = this.checked;
  });

  $('#isAutorotate').click(function() {
    isAutorotateChanged = true;
    enableAutorotate(this.checked);
  });

  $('#lightBothSides').click(function() {
    isLightBothSidesChanged = true;
    lightBothSides = this.checked;
  });

  function enableAutorotate(isEnabled) {
    isAutorotate = isEnabled;
    if (isAutorotate) {
      renderer.domElement.addEventListener('mousedown', onMouseDown, false);
    } else {
      renderer.domElement.removeEventListener('mousedown', onMouseDown);
      controls.staticMoving = true;
    }
  }

  function setFontSize(newSize) {
    if (newSize != fontSize) {
      fontSize = newSize;
      isFontSizeChanged = true;
    }

    textEditor.setFontSize(fontSize);
    $('#console')[0].style.fontSize = fontSize + 'px';
    $('ul#settings')[0].style.fontSize = (fontSize + 0) + 'px';
  }

  var red = 0xB80000;
  var green = 0x006100;
  var blue = 0x0000FF;
  var colors = [red, green, blue];
  var axes = new Array(3);
  var grids = new Array(3);

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
    // axes[d] = new THREE.Line(geometry, new THREE.LineBasicMaterial({
      // color: colors[d],
      // linewidth: 5
    // }), THREE.LineSegments);
    axes[d] = new MeshLine();
    axes[d].setGeometry(geometry);
    var material = new MeshLineMaterial({
      lineWidth: 0.1,
      color: new THREE.Color(colors[d]),
      useMap: false,
      useAlphaMap: false,
      opacity: 1,
      sizeAttenuation: !false,
      resolution: new THREE.Vector2(800, 600),
      near: camera.near,
      far: camera.far
    });
    glyphScene.add(new THREE.Mesh(axes[d].geometry, material));
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

    grids[d] = new THREE.Line(geometry, new THREE.LineBasicMaterial({color: colors[d], linewidth: 1}), THREE.LineSegments);
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

  $('#autopathify').click(function() {
    isAutopathifyChanged = true;
    isAutopathify = this.checked;

    if (this.checked) {
      $('#nSecondsTillAutopathify').prop('disabled', false);
      schedulePathify();
    } else {
      $('#nSecondsTillAutopathify').prop('disabled', true);
      textEditor.getSession().off('change', onEditorChange);
      if (preview) {
        clearTimeout(preview); 
      }
      preview = undefined;
    }
  });

  $('#showMode').change(function() {
    isShowModeChanged = true;
    showMode = $('#showMode').find(":selected").val();
    run(getSource(), GeometryMode.PATH);
  });

  $('#showPoints').click(function() {
    isShowPointsChanged = true;
    showPoints = this.checked;
    run(getSource(), GeometryMode.PATH);
  });

  enableDownload(false);
  $('#download').click(function() {
    $('#tag').val(mupName);
    $('#source').val(getSource());
    $('#timestamp').val(new Date().getTime());
    isDownloading = true;
    $('#downloader').submit();
    focusEditor();
  });

  $('#docs').click(function() {
    window.open('docs/introduction.html', '_blank');
    focusEditor();
  });

  $('#github').click(function() {
    window.open('https://github.com/twodee/madeup', '_blank');
    focusEditor();
  });

  $('#magic').click(function() {
    var source = getSource();

    var name = prompt("What's your name?");
    $.ajax({
      type: 'POST',
      url: madeupPrefix + '/save.php',
      data: JSON.stringify({
        name: name,
        source: source
      }),
      contentType: 'application/json; charset=utf-8',
      dataType: 'json',
      success: function(data) {
        console.log("Saved!");
      },
      failure: function(errorMessage) {
        console.log('Failure. :(');
      }
    });
  });

  $('#solidify-button').click(function() {
    saveInCookies();
    run(getSource(), GeometryMode.SURFACE);
    focusEditor();
  });

  $('#pathify-button').click(function() {
    saveInCookies();
    run(getSource(), GeometryMode.PATH);
    focusEditor();
  });

  $('#settings-button').click(showSettings);
  $('#close-settings-button').click(hideSettings);

  $('ul#settings > li > .panel-section-label').click(function() {
    $(this).toggleClass('open-panel-section-label');
    $(this).next().slideToggle(200);
  });

  $('#fileSaveAs').click(function() {
    var name = prompt('Save under what name?');
    if (name != null) {
      mupName = name;
      save();
      updateTitle();
    }
  });

  $('#fileSave').click(function() {
    if (!mupName) {
      mupName = prompt('Save under what name?');
    }

    if (mupName) {
      save();
      updateTitle();
    }
  });

  $('#exportArchive').click(function() {
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
    if (mupName && isSourceDirty && confirm('Save changes to ' + mupName + '?')) {
      save();
    }
    load('untitled');
  });

  $('#fileDelete').click(function() {
    var ok = confirm('Delete ' + mupName + '?');
    if (ok) {
      if (blocklyWorkspace) {
        blocklyWorkspace.clear();
        blocklyWorkspace.updateVariableList();
      }
      window.localStorage.removeItem(mupName);
      mupName = null;
      load('untitled');
    }
  });

  $('#left').resizable({
    handles: "e",
    resize: function(event, ui) {
      resize();
      window.dispatchEvent(new Event('resize'))
      render();
    } 
  });

  $('#right').resizable({
    handles: "w",
    minWidth: 300,
    resize: function(event, ui) {
      resize();
      window.dispatchEvent(new Event('resize'))
      render();
    } 
  });

  $('#console').resizable({
    handles: "n",
    resize: function(event, ui) {
      resize();
      window.dispatchEvent(new Event('resize'))
      // Need this because console is relatively positioned.
      $('#console').css('top', '0px');
    } 
  });

  if (hasWebGL()) {
    init();
    animate();
  }
});

function showSettings() {
  $('#settings-button').fadeToggle(100, function() {
    $('#right').toggle('slide', {direction: 'right', duration: 500}, function() {
      resize();
      $('#right').css('overflow', 'visible');
      $('#close-settings-button').fadeToggle(500);
    });
  });
}

function hideSettings() {
  $('#close-settings-button').fadeToggle(100, function() {
    $('#right').toggle('slide', {direction: 'right', duration: 500}, function() {
      resize();
      $('#settings-button').fadeToggle(500);
    });
  });
}

function onEditorChange(delta) {
  isSourceDirty = true;
  updateTitle();
  enableDownload(false);
  if (preview) {
    clearTimeout(preview); 
  }
  if (isAutopathify) {
    preview = setTimeout(function() {
      run(getSource(), GeometryMode.PATH);
    }, nSecondsTillAutopathify * 1000);
  }
}

function schedulePathify() {
  textEditor.getSession().on('change', onEditorChange);
}

function setTheme(isDark) {
  if (isThemeDark == isDark) return;
  isThemeDark = isDark;

  isThemeDarkChanged = true;

  // Update radio buttons to reflect current editor.
  if (isThemeDark) {
    $("#isDark").prop('checked', true);
  } else {
    $("#isLight").prop('checked', true);
  }

  $('link[title="theme"]').attr('href', isThemeDark ? 'css/ide_skin_dark.css' : 'css/ide_skin_light.css');
  textEditor.setTheme(isThemeDark ? 'ace/theme/twilight' : 'ace/theme/katzenmilch');
}

function setEditor(isText) {
  // Bail if we're already in the requested mode.
  if (isEditorText == isText) return;

  if (blocklyWorkspace) {
    blocklyWorkspace.removeChangeListener(onBlocksChanged);
  }

  isEditorText = isText;

  // Update radio buttons to reflect current editor.
  if (isEditorText) {
    $("#isEditorText").prop('checked', true);
  } else {
    $("#isEditorBlocks").prop('checked', true);
  }

  // We're heading to text.
  if (isEditorText) {
    // If we have blocks, let's offer to convert them to text.
    if (blocklyWorkspace.getTopBlocks().length > 0) {
      $('<div title="Convert">Convert your blocks program to text?</div>').dialog({
        resizable: false,
        height: 'auto',
        width: 400,
        modal: true,
        buttons: {
          'Convert to text': function() {
            // Any blocks to convert to text?
            var source = Blockly.Madeup.workspaceToCode(blocklyWorkspace);
            textEditor.setValue(source, -1);
            switchEditors();
            $(this).dialog('close');
          },
          'Blank editor': function() {
            switchEditors();
            $(this).dialog('close');
          }
        }
      });
    } else {
      switchEditors();
    }
  }
  
  // We're heading to blocks.
  else {
    if (!blocklyWorkspace) {
      blocklyWorkspace = Blockly.inject('blocksCanvas', {
        comments: false,
        toolbox: document.getElementById('toolbox'),
        trashcan: true,
        zoom: {
          controls: true,
          wheel: false,
          startScale: 1.5,
          maxScale: 5,
          minScale: 0.3,
          scaleSpeed: 1.2
        }
      });
      blocklyWorkspace.updateVariableList();
    }

    // Any text to convert to blocks?
    var source = textEditor.getValue();
    if (source.length > 0) {
      $('<div title="Convert">Convert your text program to a blocks program?</div>').dialog({
        resizable: false,
        height: 'auto',
        width: 400,
        modal: true,
        buttons: {
          'Convert to blocks': function() {
            blocklyWorkspace.clear();
            blocklyWorkspace.updateVariableList();
            convertTextToBlocks(source);
            switchEditors();
            $(this).dialog('close');
          },
          'Blank canvas': function() {
            switchEditors();
            $(this).dialog('close');
          }
        }
      });
    } else {
      switchEditors();
    }

  }

}

function switchEditors() {
  if (isEditorText) {
    $('#blocksEditor').hide();
    $('#textEditor').show();
  } else {
    $('#textEditor').hide();
    $('#blocksEditor').show();
  }

  resize();
  window.dispatchEvent(new Event('resize'))

  if (!isEditorText && blocklyWorkspace) {
    blocklyWorkspace.addChangeListener(onBlocksChanged);
  }
}

function load(mup) {
  if (mupName && isSourceDirty && confirm('Save changes to ' + mupName + '?')) {
    save();
  }

  // Clear the editors so they don't try to get converted.
  textEditor.setValue('');
  if (blocklyWorkspace) {
    blocklyWorkspace.clear();
    blocklyWorkspace.updateVariableList();
  }

  mupName = mup;

  for (var i = 0; i < meshes.length; ++i) {
    modelScene.remove(meshes[i]);
  }
  if (renderer) render();

  var json = window.localStorage.getItem(mup); 
  if (json) {
    var file = JSON.parse(window.localStorage.getItem(mup));
    setEditor(file.mode == 'text');
    if (isEditorText) {
      textEditor.session.setValue(file.source, -1);
    } else {
      blocklyWorkspace.clear();
      blocklyWorkspace.updateVariableList();
      var xml = Blockly.Xml.textToDom(file.source);
      console.log(xml);
      Blockly.Xml.domToWorkspace(xml, blocklyWorkspace);
      xml = Blockly.Xml.workspaceToDom(blocklyWorkspace);
      lastBlocks = Blockly.Xml.domToText(xml);
    }
  }

  // TODO toggle modes
  isSourceDirty = false;
  updateTitle();
}

function save() {
  if (mupName != null) {
    var mode = null;
    var source = null;
    if (isEditorText) {
      source = getSource();
      mode = 'text';
    } else {
      var xml = Blockly.Xml.workspaceToDom(blocklyWorkspace);
      source = Blockly.Xml.domToText(xml);
      mode = 'blocks';
    }

    var file = {
      'mode' : mode,
      'updated_at' : new Date().toString(),
      'source' : source
    };

    window.localStorage.setItem(mupName, JSON.stringify(file));
    isSourceDirty = false;
    updateTitle();

    $('#message').html('I saved your program. It is precious! Find it later under <image src="images/gear.png" id="gear-in-console" width="' + fontSize + 'pt"> / Mups / ' + mupName + '.');
  }
}

function getSource() {
  if (isEditorText) {
    return textEditor.getValue();
  } else {
    return Blockly.Madeup.workspaceToCode(blocklyWorkspace);
  }
}

function convertTextToBlocks(source) {
  $.ajax({
    type: 'POST',
    url: madeupPrefix + '/translate.php',
    data: JSON.stringify({ source: source }),
    contentType: 'application/json; charset=utf-8',
    dataType: 'json',
    success: function(data) {
      if (data['exit_status'] == 0) {
        tree = data['tree'];
        console.log(data['tree']);
        parse(new Peeker(tree));
      }
    },
    failure: function(errorMessage) {
      console.log('Failure. :(');
    }
  });
}

var allGeometry = undefined;
var timeOfLatestRun = undefined;

function run(source, mode, pingback) {
  // If a preview is scheduled, clear it.
  if (preview) {
    clearTimeout(preview); 
  }

  if (mode == GeometryMode.SURFACE) {
    log('Solidifying...'); 
  } else if (mode == GeometryMode.PATH) {
    log('Pathifying...'); 
  }

  timeOfLatestRun = new Date().getTime();

  interpret({
    timestamp: timeOfLatestRun,
    source: source,
    extension: 'json',
    geometry_mode: mode,
    shading_mode: isFlatShaded ? 'FLAT' : 'SMOOTH'
  }, function(data) {
    onInterpret(data);
    if (pingback) {
      pingback();
    }
  }, function(errorMessage) {
    console.log('Failure. :(');
  });
}

function onInterpret(data) {
  // Only listen to responses to latest run.
  if (data['timestamp'] != timeOfLatestRun) {
    return;
  }

  var sansDebug = data['stdout'].replace(/^Debug:.*$\n/gm, '');
  if (sansDebug.length > 0) {
    console.log(sansDebug);
  }

  if (data['exit_status'] == 0) {
    for (var i = 0; i < meshes.length; ++i) {
      modelScene.remove(meshes[i]);
    }
    meshes = [];

    log(sansDebug);
    
    if (data.geometry_mode == GeometryMode.SURFACE) {
      var loader = new THREE.JSONLoader();
      try {
        var model = loader.parse(JSON.parse(data['model']));

        if (showMode == 'triangles' || showMode == 'shaded_triangles') {
          var solidMaterial;
          if (showMode == 'shaded_triangles') {
            solidMaterial = new THREE.MeshLambertMaterial({color: 0xcccccc});
          } else {
            solidMaterial = new THREE.MeshBasicMaterial({color: 0xcccccc});
          }
          solidMaterial.side = THREE.DoubleSide;
          var wireframeMaterial = new THREE.MeshBasicMaterial({color: 0x333333, wireframe: true, transparent: true, wireframeLinewidth: 3}); 
          wireframeMaterial.side = THREE.DoubleSide;
          var material = [solidMaterial, wireframeMaterial];
          material.side = THREE.DoubleSide;
          meshes[0] = THREE.SceneUtils.createMultiMaterialObject(model.geometry, material);
        } else {
          var frontMaterial = new THREE.MeshLambertMaterial({vertexColors: THREE.VertexColors, wireframe: showMode == 'wireframe', wireframeLinewidth: 5, side: THREE.FrontSide});
          var backMaterial;
          if (lightBothSides) {
            backMaterial = new THREE.MeshLambertMaterial({vertexColors: THREE.VertexColors, wireframe: showMode == 'wireframe', wireframeLinewidth: 5, side: THREE.BackSide});
          } else {
            backMaterial = new THREE.MeshBasicMaterial({color: 0x000000, wireframe: showMode == 'wireframe', wireframeLinewidth: 5, side: THREE.BackSide});
          }
          var material = [frontMaterial, backMaterial];
          meshes[0] = THREE.SceneUtils.createMultiMaterialObject(model.geometry, material);
        }

        allGeometry = model.geometry;
        enableDownload(true);
      } catch (err) {
        console.log(err);
        console.log(data['model']);
        log('The geometry I got back had some funny stuff in it that I didn\'t know how to read.');
      }
    } else {
      var sphereGeometry = new THREE.SphereGeometry(pathifyNodeSize, 20, 20);
      var sphereMaterial = new THREE.MeshBasicMaterial({color: 'rgb(0, 0, 0)'});

      var paths = [];
      try {
        paths = JSON.parse(data['model']);
      } catch (err) {
        log('The geometry I got back had some funny stuff in it that I didn\'t know how to read.');
      }

      allGeometry = new THREE.Geometry();

      for (var pi = 0; pi < paths.length; ++pi) {
        var geometry = new THREE.Geometry();
 
        var nodeVertices = [];
        var iLastUnique = paths[pi].vertices.length - 1;
        for (var i = paths[pi].vertices.length - 1; i > 0; --i) {
          var curr = new THREE.Vector3(paths[pi].vertices[i][0], paths[pi].vertices[i][1], paths[pi].vertices[i][2]);
          var prev = new THREE.Vector3(paths[pi].vertices[i - 1][0], paths[pi].vertices[i - 1][1], paths[pi].vertices[i - 1][2]);
          if (!curr.equals(prev)) {
            break;
          } else {
            --iLastUnique;
          }
        }

        for (var i = 0; i < paths[pi].vertices.length; ++i) {
          var v = new THREE.Vector3(paths[pi].vertices[i][0], paths[pi].vertices[i][1], paths[pi].vertices[i][2]);
          geometry.vertices.push(v);
          if (!showHeadings || i < iLastUnique) {
            nodeVertices.push(v);
          }
          allGeometry.vertices.push(v);
        }

        var polyline = new MeshLine();
        polyline.setGeometry(geometry);
        var lineMaterialColorNotDepth = new MeshLineMaterial({
          lineWidth: pathifyLineSize,
          color: new THREE.Color('#CC6600'),
          useMap: false,
          useAlphaMap: false,
          opacity: 1,
          sizeAttenuation: false,
          resolution: new THREE.Vector2(800, 600),
          near: camera.near,
          far: camera.far
        });
        meshes.push(new THREE.Mesh(polyline.geometry, lineMaterialColorNotDepth));
        
        if (showPoints) {
          for (var vi = 0; vi < nodeVertices.length; ++vi) {
            var node = new THREE.Mesh(sphereGeometry, sphereMaterial);
            node.position.x = nodeVertices[vi].x;
            node.position.y = nodeVertices[vi].y;
            node.position.z = nodeVertices[vi].z;
            meshes.push(node);
          }
        }

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
    }

    for (var mi = 0; mi < meshes.length; ++mi) {
      modelScene.add(meshes[mi]);
    }

    render();
  } else if (data['exit_status'] == 22) {
    log(data['stdout'] + '\nYour model was taking a long time to build. It felt like it was never going to finish! So, I stopped trying. Sorry.');
  } else {
    log(sansDebug);
  }
}

function fit() {
  if (allGeometry === undefined) {
    return;
  }

  allGeometry.computeBoundingBox();

  var bounds = allGeometry.boundingBox;
  var centroid = bounds.getCenter();

  var xform = new THREE.Matrix4().makeTranslation(-centroid.x, -centroid.y, -centroid.z);
  modelScene.matrix = xform;

  var constraint;
  if (camera.aspect >= 1) {
    var fovX = 2 * Math.atan(Math.tan(camera.fov * Math.PI / 180.0 * 0.5) * camera.aspect);
    constraint = Math.tan(fovX * 0.5);
  } else {
    constraint = Math.tan(camera.fov * Math.PI / 180.0 * 0.5);
  }

  var dimensions = bounds.getSize();
  var maxSpan = Math.max(dimensions.x, Math.max(dimensions.y, dimensions.z));
  var fit = maxSpan / constraint;

  controls.reset(); 
  camera.position.z = bounds.max.z + fit;
  camera.updateProjectionMatrix();
}

function viewFrom(dim, sign) {
  if (allGeometry === undefined) {
    return;
  }

  allGeometry.computeBoundingBox();

  var bounds = allGeometry.boundingBox;
  var centroid = bounds.getCenter();

  var xform = new THREE.Matrix4().makeTranslation(-centroid.x, -centroid.y, -centroid.z);
  modelScene.matrix = xform;

  var constraint;
  if (camera.aspect >= 1) {
    var fovX = 2 * Math.atan(Math.tan(camera.fov * Math.PI / 180.0 * 0.5) * camera.aspect);
    constraint = Math.tan(fovX * 0.5);
  } else {
    constraint = Math.tan(camera.fov * Math.PI / 180.0 * 0.5);
  }

  var dimensions = bounds.getSize();
  var maxSpan = Math.max(dimensions.x, Math.max(dimensions.y, dimensions.z));
  var fit = maxSpan / constraint;

  controls.reset(); 
  if (dim == 0) {
    camera.up = new THREE.Vector3(0, 1, 0);
    camera.position.x = (bounds.max.x + fit) * sign;
    camera.position.y = 0;
    camera.position.z = 0;
  } else if (dim == 1) {
    camera.up = new THREE.Vector3(0, 0, -sign);
    camera.position.x = 0;
    camera.position.y = (bounds.max.y + fit) * sign;
    camera.position.z = 0;
  } else if (dim == 2) {
    camera.up = new THREE.Vector3(0, 1, 0);
    camera.position.x = 0;
    camera.position.y = 0;
    camera.position.z = (bounds.max.z + fit) * sign;
  }
  camera.updateProjectionMatrix();
}

function log(message) {
  // Replace indents with nbsps.
  message = message.replace(/^( +)/gm, function (match, spaces, offset, s) {
    return spaces.replace(/ /g, '\u00a0');
  });

  // $1 is the whole source span. $2 is the start. $3 is the end.
  var linkMessage = message.replace(/^((\d+)\((\d+)(?:-(\d+))?\)):\s*/gm, function(match, full, startLine, startIndex, stopIndex) {
    if (isEditorText) {
      return '<div style="color: #FF9999; display: inline;">Error on <a style="text-decoration: underline;" onclick="javascript:highlight(' + startIndex + ', ' + stopIndex + ')" class="srclink">line ' + startLine + '</a></div>: ';
    } else {
      // return '<div style="color: #FF9999; display: inline;">Error</div>';
      return '';
    }
  });


  $('#console #message').html(linkMessage.replace(/\n/g, '<br/>'));
}

function resize() {
  if (!initialized) return;

  controls.handleResize();

  var nonChromeHeight = $(window).height() - $('#keystrokes').height();
  var flexHeight = nonChromeHeight - $('#console').height();

  var rightWidth = 0;
  if ($('#right').css('display') != 'none') {
    rightWidth = $('#right').outerWidth();
  }

  var width = window.innerWidth - $('#left').width() - rightWidth;
  $('#glcanvas').width(width);
  var height = window.innerHeight;
  if (renderer) renderer.setSize(width, nonChromeHeight);
  camera.aspect = width / height;
  // camera.top = camera.left / camera.aspect;
  // camera.bottom = -camera.top;
  camera.updateProjectionMatrix();

  $("#textEditor").height(flexHeight);
  $("#blocksEditor").height(flexHeight);

  var blocklyArea = document.getElementById('blocksEditor');
  var blocklyDiv = document.getElementById('blocksCanvas');

  // Position blocklyDiv over blocklyArea.
  blocklyDiv.style.left = $('#blocksEditor').position().left + 'px';
  blocklyDiv.style.top = $('#blocksEditor').position().top + 'px';
  blocklyDiv.style.height = blocklyArea.offsetHeight + 'px';
  $("#blocksCanvas").width(blocklyArea.offsetWidth);

  if (blocklyWorkspace) {
    Blockly.svgResize(blocklyWorkspace);
  }

  textEditor.resize();
  if (renderer) {
    render();
  }
}

function highlight(startIndex, stopIndex) {
  var doc = textEditor.getSession().getDocument();
  var lines = doc.getAllLines();

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
  textEditor.centerSelection();
}

function init() {
  camera = new THREE.PerspectiveCamera(45.0, 1.0, 0.1, 10000.0);
  // camera = new THREE.OrthographicCamera(-50, 50, 50, -50, -100, 100);
  camera.position.z = 30;

  var glcanvas = $("#glcanvas");
  renderer = new THREE.WebGLRenderer({antialias: true});

  // Scratch out upstream implementation, which affects global culling state
  // that I rely on.
  // renderer.setMaterialFaces = function(material) {
    // console.log('dummy'); 
  // }

  renderer.setClearColor(0xFFFFFF, 1);
  // renderer.setClearColor(0xCCCCCC, 1);
  document.getElementById("glcanvas").appendChild(renderer.domElement);
  renderer.domElement.addEventListener('touchmove', function(e) {
    e.preventDefault();
  }, false);

  // controls = new THREE.OrbitControls(camera, renderer.domElement);  
  controls = new THREE.TrackballControls(camera, renderer.domElement);
  controls.addEventListener('change', render);

  // (function() {
    // var oldUpdate = controls.update;
    // controls.update = function() {
      // oldUpdate();
    // };
  // })();

  controls.rotateSpeed = 3.0;
  controls.zoomSpeed = 1.2;
  controls.panSpeed = 0.8;
  controls.noZoom = false;
  controls.noPan = false;
  controls.staticMoving = true;
  controls.dynamicDampingFactor = 0.0;

  // pointerScene = new THREE.Scene(); 
  modelScene = new THREE.Scene();
  modelScene.matrixAutoUpdate = false;

  glyphScene = new THREE.Scene();
  modelScene.add(glyphScene);

  overallScene = new THREE.Scene();
  overallScene.add(modelScene);
  overallScene.add(camera);

  // create a point light
  var pointLight = new THREE.PointLight(0xFFFFFF);

  // set its position
  pointLight.position.x = 90;
  pointLight.position.y = 150;
  pointLight.position.z = 100;

  // add to the scene
  camera.add(pointLight);

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

function focusEditor() {
  textEditor.focus();
}

Mousetrap.bind('ctrl+s', function(e) {
  save();
});

Mousetrap.bind('ctrl+,', function(e) {
  run(getSource(), GeometryMode.PATH);
});

Mousetrap.bind('ctrl+.', function(e) {
  run(getSource(), GeometryMode.SURFACE);
});

Mousetrap.bind('ctrl+/', function(e) {
  fit();
});

Mousetrap.bind('ctrl+shift+/', function(e) {
  if ($('#right').css('display') == 'none') {
    showSettings();
  } else {
    hideSettings();
  }
});

Mousetrap.bind('ctrl+t', function(e) {
  if (!isEditorText) {
    var source = Blockly.Madeup.workspaceToCode(blocklyWorkspace);
    log(source);
  }
});

Mousetrap.bind('esc', function(e) {
  parent.focus();
});
