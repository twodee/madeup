function hasWebGL() {
  try {
    var canvas = document.createElement("canvas");
    return !!window.WebGLRenderingContext && (canvas.getContext("webgl") || canvas.getContext("experimental-webgl"));
  } catch(e) { 
    return false;
  } 
}

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

  console.log('foo!');
  console.log(Blockly.Procedures.allProcedures(blocklyWorkspace));
  var procs = Blockly.Procedures.allProcedures(blocklyWorkspace)[0];
  // TODO

  // Find the procedure in the list with the given name.
  for (var i = 0; i < procs.length; ++i) {
    console.log("procedure " + i + ": " + procs[i][0]);
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
      list += '<a href="#" class="fileLink" onclick="load(\'' + mup.replace(/'/g, '\\&#39;').replace(/"/g, '\\&quot;') + '\')">- ' + mup + '</a><br/>';
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
var swatch = null;
var initialized = false;
var mupName = null;
var overallScene;
var modelScene;
var glyphScene;
var renderer, camera, controls;
var meshes = [];
var isAutopathify = true;
var nSecondsTillAutopathify = 1.0;
var showWireframe = false;
var showHeadings = true;
var showCounterclockwise = true;
var showClockwise = false;
var blocklyWorkspace = null;
var fontSize = 14;
var gridSpacing = 1.0;
var gridExtent = 10.0;
var isFlatShaded = true;
var isEditorText = true;
var tree = null;
var preview = undefined;
var isThemeDark = true;
var isSourceDirty = false;
var showPoints = true;

var isThemeDarkChanged = false;
var isShowWireframeChanged = false;
var isAxisChanged = [false, false, false];
var isGridChanged = [false, false, false];
var isAutopathifyChanged = false;
var isNSecondsTillAutopathifyChanged = false;
var isShowHeadingsChanged = false;
var isFontSizeChanged = false;
var isGridSpacingChanged = false;
var isGridExtentChanged = false;
var isShowCounterclockwiseChanged = false;
var isShowClockwiseChanged = false;
var isFlatShadedChanged = false;
var isShowPointsChanged = false;

function updateTitle() {
  $('#toggleFilePopup').attr('value', mupName + (isSourceDirty ? '*' : ''));
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
  if (isShowWireframeChanged) Cookies.set('showWireframe', showWireframe ? 1 : 0);
  if (isFlatShadedChanged) Cookies.set('isFlatShaded', isFlatShaded ? 1 : 0);
  if (isThemeDarkChanged) Cookies.set('isThemeDark', isThemeDark ? 1 : 0);
  if (isAxisChanged[0]) Cookies.set('axisX', $('#axisX').prop('checked') ? 1 : 0);
  if (isAxisChanged[1]) Cookies.set('axisY', $('#axisY').prop('checked') ? 1 : 0);
  if (isAxisChanged[2]) Cookies.set('axisZ', $('#axisZ').prop('checked') ? 1 : 0);
  if (isGridChanged[0]) Cookies.set('gridX', $('#gridX').prop('checked') ? 1 : 0);
  if (isGridChanged[1]) Cookies.set('gridY', $('#gridY').prop('checked') ? 1 : 0);
  if (isGridChanged[2]) Cookies.set('gridZ', $('#gridZ').prop('checked') ? 1 : 0);
  if (isAutopathifyChanged) Cookies.set('isAutopathify', $('#autopathify').prop('checked') ? 1 : 0);
  if (isNSecondsTillAutopathifyChanged) Cookies.set('nSecondsTillAutopathify', nSecondsTillAutopathify);
  if (isGridSpacingChanged) Cookies.set('gridSpacing', gridSpacing);
  if (isGridExtentChanged) Cookies.set('gridExtent', gridExtent);
  if (isShowPointsChanged) Cookies.set('showPoints', showPoints ? 1 : 0);
  Cookies.set('leftWidth', $('#left').width());
  Cookies.set('consoleHeight', $('#console').height());

  if (blocklyWorkspace) {
    var xml = Blockly.Xml.workspaceToDom(blocklyWorkspace);
    var xmlText = Blockly.Xml.domToText(xml);
    Cookies.set('lastBlocks', xmlText);
  }

  // Changes have been committed, so let's reset the dirty flags.
  isFontSizeChanged = false;
  isShowHeadingsChanged = false;
  isShowCounterclockwiseChanged = false;
  isShowClockwiseChanged = false;
  isFlatShadedChanged = false;
  isShowWireframeChanged = false;
  isThemeDarkChanged = false;
  isShowHeadingsChanged = false;
  isNSecondsTillAutopathifyChanged = false;
  isGridSpacingChanged = false;
  isGridExtentChanged = false;
  isShowPointsChanged = false;
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
      // $('#console').height(0); // TODO
      resize();
    }

    load('untitled');

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

    if (Cookies.get('showWireframe')) {
      showWireframe = parseInt(Cookies.get('showWireframe')) != 0;
    } else {
      showWireframe = false;
    }
    $('#showWireframe').prop('checked', showWireframe);

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
      updateCulling();

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

  $('#clear').click(function() {
    log('');
    textEditor.focus();
  });

  $('#smaller').click(function() {
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
    fit();
    textEditor.focus();
  });

  $('#cameraLeft').click(function() {
    viewFrom(0, -1);
    textEditor.focus();
  });

  $('#cameraRight').click(function() {
    viewFrom(0, 1);
    textEditor.focus();
  });

  $('#cameraBottom').click(function() {
    viewFrom(1, -1);
    textEditor.focus();
  });

  $('#cameraTop').click(function() {
    viewFrom(1, 1);
    textEditor.focus();
  });

  $('#cameraBack').click(function() {
    viewFrom(2, -1);
    textEditor.focus();
  });

  $('#cameraFront').click(function() {
    viewFrom(2, 1);
    textEditor.focus();
  });

  $('input[type=radio][name=editorMode]').change(function() {
    hideMenus(); // setEditor may pop open a dialog, which doesn't look good with a menu still open
    var editorMode = $(this).val();
    setEditor(editorMode != "Blocks");
  });

  $('input[type=radio][name=theme]').change(function() {
    var isDark = $(this).val() == 'isDark';
    setTheme(isDark);
  });

  $('#showHeadings').click(function() {
    isShowHeadingsChanged = true;
    showHeadings = this.checked;
    textEditor.focus();
    run(getSource(), GeometryMode.PATH);
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

  $('#showWireframe').click(function() {
    isShowWireframeChanged = true;
    showWireframe = this.checked;
    run(getSource(), GeometryMode.SURFACE);
  });

  $('#showPoints').click(function() {
    isShowPointsChanged = true;
    showPoints = this.checked;
    run(getSource(), GeometryMode.SURFACE);
  });

  function toggleMenu(id) {
    var buttonID = '#toggle' + id.charAt(1).toUpperCase() + id.substring(2);

    $(id).css('top', $(buttonID).offset().top + $(buttonID).innerHeight(true)) - 8; 
    $(id).css('left', $(buttonID).offset().left); 

    // Hilite the menu if it's opening.
    if (!$(id).is(":visible")) {
      $(buttonID).removeClass('closed').addClass('open');
    }

    $(id).slideToggle('fast', function() {
      // Unhilite the menu once it's closed.
      if (!$(id).is(":visible")) {
        $(buttonID).removeClass('open').addClass('closed');
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

  enableDownload(false);
  $('#download').click(function() {
    $('#tag').val(mupName);
    $('#source').val(getSource());
    isDownloading = true;
    $('#downloader').submit();
    textEditor.focus();
  });

  $('#docs').click(function() {
    window.open('docs/introduction.html', '_blank');
  });

  $('#magic').click(function() {
    var source = Blockly.Madeup.workspaceToCode(blocklyWorkspace);
    log(source);
  });

  $('#solidify').click(function() {
    log('Solidifying...'); 
    saveInCookies();
    run(getSource(), GeometryMode.SURFACE);
    textEditor.focus();
  });

  $('#pathify').click(function() {
    log('Pathifying...'); 
    saveInCookies();
    run(getSource(), GeometryMode.PATH);
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

  $('#fileSave').click(function() {
    hideMenus();

    if (!mupName) {
      mupName = prompt('Save under what name?');
    }

    if (mupName) {
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
    if (mupName && isSourceDirty && confirm('Save changes to ' + mupName + '?')) {
      save();
    }
    load('untitled');
  });

  $('#fileDelete').click(function() {
    var ok = confirm('Delete ' + mupName + '?');
    if (ok) {
      if (blocklyWorkspace) blocklyWorkspace.clear();
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

  $(document).on('click', function(event) {
    // Don't do any hiding when color picker clicked on.
    if ($(event.target).closest('.colorpicker').length != 0) {
      event.preventDefault();
      return;
      // return false;
    }

    // If a non-menu was clicked on, so let's close all the menus.
    if ($(event.target).closest('.togglePopup').length == 0 &&
        $(event.target).closest('.popup').length == 0) {
      hideMenus();
    }
    
    // Otherwise, if got a click on a menu toggle button, let's hide any other
    // menus. 
    else if ($(event.target).hasClass('togglePopup')) {
      // The menu button has ID toggleNamePopup. Strip off the toggle and lowercase
      // the name to get the menu's ID.
      var id = event.target.id.substring(6);
      var id = '#' + id.charAt(0).toLowerCase() + id.substring(1);
      hideMenus(id);
    }
 
    // Otherwise, if got a click on a menu itself, let's hide any other menus. 
    else if ($(event.target).hasClass('popup')) {
      hideMenus(event.target.id);
    }

    return true;
  });

  if (hasWebGL()) {
    init();
    animate();
  }
});

var onEditorChange = function(delta) {
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

  $('link[title="theme"]').attr('href', isThemeDark ? 'ide_skin_dark.css' : 'ide_skin_light.css');
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
    // Any blocks to convert to text?
    if (blocklyWorkspace.getTopBlocks().length > 0 && confirm('Convert your blocks to text?')) {
      var source = Blockly.Madeup.workspaceToCode(blocklyWorkspace);
      textEditor.setValue(source);
    }

    $('#blocksEditor').hide();
    $('#textEditor').show();
  }
  
  // We're heading to blocks.
  else {
    if (!blocklyWorkspace) {
      blocklyWorkspace = Blockly.inject('blocksCanvas', {
        toolbox: document.getElementById('toolbox'),
        zoom: {
          controls: true,
          wheel: false,
          startScale: 1.0,
          maxScale: 3,
          minScale: 0.3,
          scaleSpeed: 1.2
        }
      });
    }

    // Any text to convert to blocks?
    var source = textEditor.getValue();
    if (source.length > 0 && confirm('Convert your text to blocks?')) {
      blocklyWorkspace.clear();
      convertTextToBlocks(source);
    }

    $('#textEditor').hide();
    $('#blocksEditor').show();
  }

  resize();
  Blockly.fireUiEvent(window, 'resize');

  if (!isEditorText && blocklyWorkspace) {
    blocklyWorkspace.addChangeListener(onBlocksChanged);
  }
}

function load(mup) {
  hideMenus();
  if (mupName && isSourceDirty && confirm('Save changes to ' + mupName + '?')) {
    save();
  }

  // Clear the editors so they don't try to get converted.
  textEditor.setValue();
  if (blocklyWorkspace) blocklyWorkspace.clear();

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
      var xml = Blockly.Xml.textToDom(file.source);
      Blockly.Xml.domToWorkspace(blocklyWorkspace, xml);
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
    url: 'translate.php',
    data: JSON.stringify({ source: source }),
    contentType: 'application/json; charset=utf-8',
    dataType: 'json',
    success: function(data) {
      if (data['exit_status'] == 0) {
        tree = data['tree'];
        console.log(data['tree']);
        var block = parse(new Peeker(tree));
        block.initSvg();
        block.render();
      }
    },
    failure: function(errorMessage) {
      console.log('Failure. :(');
    }
  });
}

var allGeometry = undefined;
var timeOfLatestRun = undefined;

function run(source, mode) {
  timeOfLatestRun = new Date().getTime();

  $.ajax({
    type: 'POST',
    url: 'interpret.php',
    data: JSON.stringify(
      {
        timestamp: timeOfLatestRun,
        source: source,
        extension: 'json',
        geometry_mode: mode,
        shading_mode: isFlatShaded ? 'FLAT' : 'SMOOTH'
      }
    ),
    contentType: 'application/json; charset=utf-8',
    dataType: 'json',
    success: function(data) {
      // Only listen to responses to latest run.
      if (data['timestamp'] != timeOfLatestRun) {
        return;
      }

      var sansDebug = data['stdout'].replace(/^Debug:.*$\n/gm, '');
      if (sansDebug.length > 0) {
        console.log(sansDebug);
      }

      if (data['exit_status'] == 0) {
        tree = data['tree'];
        console.log(data['tree']);

        for (var i = 0; i < meshes.length; ++i) {
          modelScene.remove(meshes[i]);
        }
        meshes = [];

        log(sansDebug);
        
        if (mode == GeometryMode.SURFACE) {
          var loader = new THREE.JSONLoader();
          try {
            var model = loader.parse(JSON.parse(data['model']));
            var material = new THREE.MeshLambertMaterial({vertexColors: THREE.VertexColors, wireframe: showWireframe, wireframeLinewidth: 5});
            meshes[0] = new THREE.Mesh(model.geometry, material);
            model.geometry.computeFaceNormals();
            model.geometry.computeVertexNormals();
            allGeometry = model.geometry;
            enableDownload(true);
          } catch (err) {
            log('The geometry I got back had some funny stuff in it that I didn\'t know how to read.');
          }
        } else {
          var paths = [];
          try {
            paths = JSON.parse(data['model']);
          } catch (err) {
            log('The geometry I got back had some funny stuff in it that I didn\'t know how to read.');
          }

          allGeometry = new THREE.Geometry();
          if (showPoints) {
            var dotsGeometry = new THREE.Geometry();
          }

          for (var pi = 0; pi < paths.length; ++pi) {
            var geometry = new THREE.Geometry();
            for (var i = 0; i < paths[pi].vertices.length; ++i) {
              var v = new THREE.Vector3(paths[pi].vertices[i][0], paths[pi].vertices[i][1], paths[pi].vertices[i][2]);
              geometry.vertices.push(v);
              if (showPoints) {
                dotsGeometry.vertices.push(v);
              }
              allGeometry.vertices.push(v);
            }
            meshes[meshes.length] = new THREE.Line(geometry, new THREE.LineBasicMaterial({
              // color: 0x6666FF,
              color: 0xCC6600,
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

          if (showPoints) {
            meshes[meshes.length] = new THREE.PointCloud(dotsGeometry, new THREE.PointCloudMaterial({
              color: 0x000000,
              size: 8,
              sizeAttenuation: false
            }));
          }
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
        log(data['stdout'] + '\nYour model was taking a long time to build. It felt like it was never going to finish! So, I stopped trying. Sorry.');
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

  var constraint;
  if (camera.aspect >= 1) {
    var fovX = 2 * Math.atan(Math.tan(camera.fov * Math.PI / 180.0 * 0.5) * camera.aspect);
    constraint = Math.tan(fovX * 0.5);
  } else {
    constraint = Math.tan(camera.fov * Math.PI / 180.0 * 0.5);
  }

  var dimensions = bounds.size();
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
  var centroid = bounds.center();

  var xform = new THREE.Matrix4().makeTranslation(-centroid.x, -centroid.y, -centroid.z);
  modelScene.matrix = xform;

  var constraint;
  if (camera.aspect >= 1) {
    var fovX = 2 * Math.atan(Math.tan(camera.fov * Math.PI / 180.0 * 0.5) * camera.aspect);
    constraint = Math.tan(fovX * 0.5);
  } else {
    constraint = Math.tan(camera.fov * Math.PI / 180.0 * 0.5);
  }

  var dimensions = bounds.size();
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

  var width = window.innerWidth - $('#left').width();
  var height = window.innerHeight;
  if (renderer) renderer.setSize(width, height);
  camera.aspect = width / height;
  // camera.top = camera.left / camera.aspect;
  // camera.bottom = -camera.top;
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
}

function init() {
  camera = new THREE.PerspectiveCamera(45.0, 1.0, 0.1, 10000.0);
  // camera = new THREE.OrthographicCamera(-50, 50, 50, -50, -100, 100);
  camera.position.z = 30;

  var glcanvas = $("#glcanvas");
  renderer = new THREE.WebGLRenderer({antialias: true});

  // Scratch out upstream implementation, which affects global culling state
  // that I rely on.
  renderer.setMaterialFaces = function(material) {}

  renderer.setClearColor(0xFFFFFF, 1);
  document.getElementById("glcanvas").appendChild(renderer.domElement);

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
    $('.popup').hide();
    $('.togglePopup').removeClass('open').addClass('closed');
  } else {
    $('.popup').not(exceptID).hide();
    $('.togglePopup').not('#toggle' + exceptID.charAt(1).toUpperCase() + exceptID.substring(2)).removeClass('open').addClass('closed');
  }

  if (swatch) {
    swatch.ColorPickerHide();
  }
}
