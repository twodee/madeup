// NodeList, as returned by querySelectorAll, doesn't support forEach
// at the moment, except in Chrome and Firefox.
var forEach = function (array, callback, scope) {
  for (var i = 0; i < array.length; i++) {
    callback.call(scope, i, array[i]); // passes back stuff we need
  }
};

var GeometryMode = Object.freeze({
  PATH: 'PATH',
  SURFACE: 'SURFACE'
});

function Mup(name, modifiedTime, driveID) {
  this.name = name;
  this.driveID = driveID;
  this.modifiedTime = modifiedTime;
  this.isDirty = false;
}

var now = new Date();
var sessionID = Math.random().toString(36).substr(2, 9) + '_' + now.toISOString();
var docEditors = [];
var docWorkspaces = [];
var Range = null;
var textEditor = null;
var gearSections = ['file', 'mups', 'editor', 'pathify', 'solidify', 'camera', 'grid', 'tutorial', 'lesson', 'about'];
var isDownloading = false;
var initialized = false;
var mup = null;
var overallScene;
var modelScene;
var pointScene;
var glyphScene;
var raycaster;
var mouse;
var renderer, camera, controls;
var meshes = [];
var lineMeshes = [];
var lineEssences = [];
var blocklyWorkspace = null;
var allGeometry = undefined;
var timeOfLatestRun = undefined;
var autopathifyTask = undefined;
var snapshotTask = undefined;
var settings = new Settings();
var lastBlocks = null;
var badModelMessage = 'Uh oh. I tried to generate a model for you, but it is broken. This can happen for a bunch of reasons: some faces may be too small, some vertices may be duplicated, and the mesh boolean operations may just be fickle.';
var isAutoSolidify = false;
var axes = [null, null, null];

function hasWebGL() {
  try {
    var canvas = document.createElement("canvas");
    return !!window.WebGLRenderingContext && (!!canvas.getContext("webgl") || !!canvas.getContext("experimental-webgl"));
  } catch(e) { 
    return false;
  } 
}

// https://github.com/devuxd/SeeCodeRun/wiki/Ace-code-editor
function showSlider(position, number, range) {
  position.pageY += textEditor.renderer.lineHeight;
  var div = document.getElementById('slider-popup');

  // If div hasn't been displayed before, let's display it.
  if (div === null) {
    div = document.createElement('slider-popup');
    div.setAttribute('id', 'slider-popup');
    div.innerHTML = '<input type="range" id="varslider">';
    document.body.appendChild(div);
  }

  div.style.left = (position.pageX - div.offsetWidth * 0.5) + 'px';
  div.style.top = position.pageY + 'px';
  div.style.display = 'block';

  var slider = document.getElementById('varslider');
  slider.min = number - 10;
  slider.max = number + 10;
  slider.step = number % 1 === 0 ? 1 : 'any';
  slider.value = number;

  slider.oninput = function(e) {
    var doc = textEditor.getSession().getDocument();
    if (number % 1 === 0) {
      var value = parseInt(slider.value);
      var replacement = '' + value;
    } else {
      var value = parseFloat(slider.value);
      var replacement = value.toFixed(2);
    }
    doc.replace(range, replacement);
    range.setEnd(range.end.row, range.start.column + replacement.length);
  }
}

function getLineMaterial(width, color) {
  return new MeshLineMaterial({
    lineWidth: width,
    color: new THREE.Color(color),
    useMap: false,
    useAlphaMap: false,
    opacity: 1,
    sizeAttenuation: false,
    resolution: new THREE.Vector2(renderer.getSize().width, renderer.getSize().height),
    near: camera.near,
    far: camera.far
  });
}

function restoreSettings(settings) {
  var json = localStorage.getItem('settings')
  if (json != null) {
    var stored = JSON.parse(json);
    for (var key in stored) {
      settings.set(key, stored[key]);
    }
  }
}

function persistSettings(settings) {
  localStorage.setItem('settings', settings.toJSON());
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

// Warn on leaving the page if there are unsaved changes. Downloading triggers
// this, even though we're not leaving the page, so we add a special flag to
// filter out these events.
window.addEventListener('beforeunload', function(e) {
  if (!isDownloading && mup.isDirty && needsUnsavedPrompt()) {
    var message = 'You have unsaved changes. Throw them away?';
    e.returnValue = message;
    return message;
  } else if (isDownloading) {
    isDownloading = false;
  }
});

THREE.Object3D.prototype.clear = function() {
  var children = this.children;
  for (var i = children.length - 1; i >= 0; --i) {
    var child = children[i];
    child.clear();
    this.remove(child);
  }
}

function enableDownload(enable) {
  if (enable) {
    $('.export3D').prop('disabled', false);
    $('.export3D').removeClass('unclickable').addClass('clickable');
  } else {
    $('.export3D').prop('disabled', true);
    $('.export3D').removeClass('clickable').addClass('unclickable');
  }
}

function generateDownloadable(filename, text) {
  var blob = new Blob([text], {type: "application/json"});
  saveAs(blob, filename);
}

function exportScreenshot() {
  renderer.domElement.toBlob(function(blob) {
    saveAs(blob, mup.name + '.png');
  });
  return false;
}

function promptForSaveAs() {
  platformPromptForSaveAs(function(name) {
    mup.name = name;
    save();
    updateTitle();
  });
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

function Settings() {
  function Setting(initialValue) {
    this.value = initialValue;
    this.isChanged = false;
  }

  var pairs = {};

  pairs.showMode = new Setting('solid');
  pairs.showHeadings = new Setting(true);
  pairs.showPoints = new Setting(true);
  pairs.pathifyNodeSize = new Setting(0.3);
  pairs.pathifyLineSize = new Setting(6);

  pairs.lightBothSides = new Setting(true);
  pairs.fontSize = new Setting(14);

  pairs.gridSpacing = new Setting(1.0);
  pairs.gridExtent = new Setting(10.0);

  pairs.isFlatShaded = new Setting(true);
  pairs.isAutorotate = new Setting(false);
  pairs.isEditorText = new Setting(true);
  pairs.isThemeDark = new Setting(true);

  pairs.sortMupsBy = new Setting('name');

  pairs.isAutopathify = new Setting(true);
  pairs.nSecondsTillAutopathify = new Setting(1.0);

  pairs.showAxisX = new Setting(false);
  pairs.showAxisY = new Setting(false);
  pairs.showAxisZ = new Setting(false);
  pairs.showGridX = new Setting(false);
  pairs.showGridY = new Setting(false);
  pairs.showGridZ = new Setting(false);

  this.toJSON = function() {
    var simpler = {};
    for (var key in pairs) {
      simpler[key] = pairs[key].value;
    }
    return JSON.stringify(simpler, null, 2); 
  }

  this.set = function(key, value) {
    if (this.has(key)) {
      var oldValue = pairs[key].value;
      pairs[key].value = value;
      pairs[key].isChanged = oldValue != value;
    } else {
      pairs[key] = new Setting(value);
      pairs[key].isChanged = true;
    }
  }

  this.get = function(key) {
    if (this.has(key)) {
      return pairs[key].value;
    } else {
      throw 'No setting ' + key + '!';
    }
  }

  this.has = function(key) {
    return pairs.hasOwnProperty(key);
  }

  this.keys = function() {
    return Object.keys(pairs);
  }

  this.isChanged = function(key) {
    if (this.has(key)) {
      return pairs[key].isChanged;
    } else {
      throw 'No setting ' + key + '!';
    }
  }

  this.dump = function() {
    this.keys().forEach(function(key) {
      console.log(key + ' -> ' + this.get(key) + '[' + this.get(key).constructor.name + ']');
    });
  }
}

function updateTitle() {
  // The name won't be set in certain situations -- namely, when it's embedded.
  // Showing it as dirty makes no sense, because it's not a file.
  if (mup.name != null) {
    document.title = (mup.isDirty ? '*' : '') + 'Madeup: ' + mup.name.replace(/^.*\//, '');
  }
  if (mup.isDirty) {
    $('#fileSave').prop('disabled', false);
    $('#fileSave').removeClass('unclickable').addClass('clickable');
    $('#dirty-suffix').text('*');
  } else {
    $('#fileSave').prop('disabled', true);
    $('#fileSave').removeClass('clickable').addClass('unclickable');
    $('#dirty-suffix').text('');
  }
}

function syncSettings() {
  // Embedded views have differing size requirements, so we don't try to
  // preserve their values long term.
  if (!isEmbedded) {
    settings.set('leftWidth', $('#left').width());
    settings.set('consoleHeight', $('#console').height());
  }

  if (blocklyWorkspace) {
    var xml = Blockly.Xml.workspaceToDom(blocklyWorkspace);
    var xmlText = Blockly.Xml.domToText(xml);
    settings.set('lastBlocks', xmlText);
  }

  // Save gear menu config.
  settings.set('showGearMenu', $('#right').css('display') != 'none');
  gearSections.forEach(function(tag) {
    settings.set('isOpen' + tag.charAt(0).toUpperCase() + tag.substring(1), $('#panel-section-' + tag + ' > .panel-section-label').hasClass('open-panel-section-label'));
  });

  persistSettings(settings);
}

// renderer isn't ready yet, so we need to wait for the window onload event.
$(window).on('load', function() {
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

  if (hasWebGL()) {
    init();
    animate();
  }

  // When we are embedded in an iframe, the wheel event will cause the
  // embedding context to scroll. That's not what we want, so we capture and
  // squelch any wheel events.
  if (isEmbedded) {
    document.body.addEventListener('wheel', function (e) {
      e.preventDefault();
    });
    // TODO: confirm that this works as intended.
  }

  configureDownloader();
  restoreSettings(settings);

  textEditor = ace.edit("textEditor");
  textEditor.$blockScrolling = Infinity;
  Range = ace.require('ace/range').Range;

  // Creates an undo manager implicitly.
  textEditor.setSession(ace.createEditSession('', 'ace/mode/madeup'));

  textEditor.setTheme("ace/theme/twilight");
  textEditor.setHighlightSelectedWord(false);
  textEditor.setHighlightActiveLine(false);
  textEditor.setOption("tabSize", 2);
  textEditor.setOption("useSoftTabs", true);

  // https://github.com/ajaxorg/ace/blob/master/lib/ace/commands/default_commands.js
  // https://ace.c9.io/demo/keyboard_shortcuts.html
  // textEditor.commands.removeCommand(['togglecomment', 'gotoline', 'showSettingsMenu']);
  textEditor.commands.bindKey('ctrl-s', null);
  textEditor.commands.bindKey('ctrl-/', null);
  textEditor.commands.bindKey('ctrl-?', null);
  textEditor.commands.bindKey('ctrl-shift-?', null);
  textEditor.commands.bindKey('ctrl-shift-/', null);
  textEditor.commands.bindKey('ctrl-,', null);
  textEditor.commands.bindKey('ctrl-[', null);
  textEditor.commands.bindKey('ctrl-]', null);
  textEditor.commands.bindKey('ctrl-l', null);
  textEditor.commands.bindKey('command-l', null);
  textEditor.commands.bindKey('ctrl-\'', 'togglecomment');

  textEditor.on('mousemove', function(e) {
    return true;
    var isSliderable = false;

    var position = e.getDocumentPosition();
    if (position) {
      var range = textEditor.getSession().getWordRange(position.row, position.column);
      var line = textEditor.getSession().getLine(position.row);

      var left = line.substring(0, position.column);
      var right = line.substring(position.column);

      var patterns = [
        [/\b(-?\d+\.\d+)$/, /^(\d*)/],
        [/\b(-?\d+\.)$/, /^(\d+)/],
        [/\b(-?\d+)$/, /^(\.\d+)/],
        [/\b(-)$/, /^(\d+\.\d+)/],
        [/\b()$/, /^(\d+\.\d+)/],
        [/\b(-?\d+)$/, /^(\d+)/],
        [/\b(-?\d+)$/, /^()/],
        [/\b(-?)$/, /^(\d+)/],
      ];

      var preCursor = null;
      var postCursor = null;
      var i = 0;
      while (i < patterns.length && (preCursor == null || postCursor == null)) {
        preCursor = left.match(patterns[i][0]);
        if (!left.match(/[A-Za-z_]$/)) {
          postCursor = right.match(patterns[i][1]);
        }
        ++i;
      }

      if (preCursor && postCursor) {
        var number = preCursor[1] + postCursor[1];
        var range = new Range(position.row, position.column - preCursor[1].length, position.row, position.column + postCursor[1].length);
        isSliderable = true;
        showSlider(textEditor.renderer.textToScreenCoordinates(position), parseFloat(number), range);
      }
    }

    if (!isSliderable) {
      var div = document.getElementById('slider-popup');
      if (div) {
        div.style.display = 'none';
      }
    }
  });

  if (isEmbedded) {
    $('#breakout').show();
  }

  // Preloads are forced to be text for the time being.
  if (source0) {
    settings.set('isEditorText', true);
  }

  $('#textEditor textarea').addClass('mousetrap');

  if (!isEmbedded) {
    if (settings.has('leftWidth')) {
      $('#left').width(settings.get('leftWidth'));
      resize();
    }

    if (settings.has('consoleHeight')) {
      $('#console').height(settings.get('consoleHeight'));
      $('#showConsole').prop('checked', $('#console').height() != 0);
      resize();
    }

    if (source0) {
      mup = new Mup('untitled');
    } else {
      load(new Mup('untitled'));
    }
  } else {
    mup = new Mup('untitled');
    if (isPresenting) {
      showConsole(false);
      $('#left').width(375);
      resize();
    } else {
      $('#left').width(250);
      resize();
    }
  }

  if (!isEmbedded && !isBlocksURL && settings.has('fontSize')) {
  } else if (isBlocksURL) {
    settings.set('fontSize', 24);
  } else if (isPresenting) {
    settings.set('fontSize', 28);
  } else {
    settings.set('fontSize', 12);
  }
  setFontSize(settings.get('fontSize'));

  $('#showHeadings').prop('checked', settings.get('showHeadings'));
  $('#isFlatShaded').prop('checked', settings.get('isFlatShaded'));
  $('#lightBothSides').prop('checked', settings.get('lightBothSides'));
  $('#showPoints').prop('checked', settings.get('showPoints'));
  $("#autopathify").prop('checked', settings.get('isAutopathify'));
  $('#showMode').val(settings.get('showMode'));
  $('#gridSpacing').val(settings.get('gridSpacing'));
  $('#gridExtent').val(settings.get('gridExtent'));

  $('#isAutorotate').prop('checked', settings.get('isAutorotate'));
  enableAutorotate(settings.get('isAutorotate'));

  if (settings.get('isEditorText')) {
    $("#isEditorText").prop('checked', true);
  } else {
    $("#isEditorBlocks").prop('checked', true);
  }

  if (settings.get('sortMupsBy') == 'date') {
    $("#sortMupsByDate").prop('checked', true);
  } else {
    $("#sortMupsByName").prop('checked', true);
  }

  setTheme(settings.get('isThemeDark'));
  if (settings.get('isThemeDark')) {
    $("#isDark").prop('checked', true);
  } else {
    $("#isLight").prop('checked', true);
  }

  $('#pathify-node-size').val(settings.get('pathifyNodeSize'));
  $('#pathify-node-size')[0].oninput = function () {
    settings.set('pathifyNodeSize', parseFloat(this.value));
    run(getSource(), GeometryMode.PATH);
  };

  // Line size
  $('#pathify-line-size').val(settings.get('pathifyLineSize'));
  $('#pathify-line-size')[0].oninput = function () {
    settings.set('pathifyLineSize', parseFloat(this.value));
    run(getSource(), GeometryMode.PATH);
  };

  // Showing gear menu?
  if (!lesson && settings.has('showGearMenu') && settings.get('showGearMenu')) {
    showGearMenu();
  }

  gearSections.forEach(function(tag) {
    var property = 'isOpen' + tag.charAt(0).toUpperCase() + tag.substring(1);
    if (settings.has(property) && settings.get(property)) {
      $('#panel-section-' + tag + ' > .panel-section-label').click();
    }
  });

  $('#nSecondsTillAutopathify').val(settings.get('nSecondsTillAutopathify'));
  $('#nSecondsTillAutopathify').change(function () {
    settings.set('nSecondsTillAutopathify', parseFloat(this.value));

    // Clear any pending.
    if (autopathifyTask) {
      clearTimeout(autopathifyTask); 
      autopathifyTask = undefined;
    }
  });

  // Only save cookies if they were successfully loaded.
  $(window).on('unload', function() {
    syncSettings();
    if (mup.isDirty && confirm('Save changes to ' + mup.name + '?')) {
      save();
    }
  });

  platformize();

  if (isBlocksURL) {
    setEditor(false);
    showConsole(false);
    resize();
  }

  $('#toolbar').css('display', 'block');

  if (source0) {
    textEditor.setValue(source0, 1);
  }

  if (isAutoSolidify) {
    run(getSource(), GeometryMode.SURFACE, fit);
  }

  $('#smaller').click(decreaseFontSize);
  $('#bigger').click(increaseFontSize);

  $(function() {
    var mouse = null;
    $('#fit-button').mousedown(function(e) {
      mouse = [e.clientX, e.clientY];
    }).mouseup(function(e) {
      var diff = [Math.abs(e.clientX - mouse[0]), Math.abs(e.clientY - mouse[1])];
      if (diff[0] + diff[1] < 5) {
        fit();
        focusEditor();
      }
    });
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

  $('#squareport').click(squareport);

  $('input[type=radio][name=editorMode]').change(function() {
    var editorMode = $(this).val();
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
    settings.set('showHeadings', this.checked);
    run(getSource(), GeometryMode.PATH);
  });

  $('#isFlatShaded').click(function() {
    settings.set('isFlatShaded', this.checked);
  });

  $('#isAutorotate').click(function() {
    enableAutorotate(this.checked);
  });

  $('#lightBothSides').click(function() {
    settings.set('lightBothSides', this.checked);
  });

  function enableAutorotate(isEnabled) {
    settings.set('isAutorotate', isEnabled);
    if (settings.get('isAutorotate')) {
      renderer.domElement.addEventListener('mousedown', onMouseDown, false);
    } else {
      renderer.domElement.removeEventListener('mousedown', onMouseDown);
      controls.staticMoving = true;
    }
  }

  var red = 0xB80000;
  var green = 0x006100;
  var blue = 0x0000FF;
  var colors = [red, green, blue];
  var grids = new Array(3);

  function generateAxis(d) {
    if (axes[d]) {
      removeAxis(d);
    }

    var geometry = new THREE.Geometry();
    var a = new THREE.Vector3(0, 0, 0);
    var b = new THREE.Vector3(0, 0, 0);
    a.setComponent(d, -settings.get('gridExtent'));
    b.setComponent(d, settings.get('gridExtent'));
    geometry.vertices.push(a);
    geometry.vertices.push(b);

    axes[d] = {
      geometry: geometry,
      color: colors[d],
      width: 4.0
    };

    clearLines();
    generateLines();
    render();
  }

  function removeAxis(d) {
    if (axes[d]) {
      glyphScene.remove(axes[d].mesh);
      axes[d] = null;
      render();
    }
  }

  function toggleAxis(d) {
    return function() {
      var axes = "XYZ";
      settings.set('showAxis' + axes.charAt(d), this.checked);
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

    function generateLines(i) {
      var a = new THREE.Vector3(0, 0, 0);
      var b = new THREE.Vector3(0, 0, 0);
      a.setComponent((d + 1) % 3, -settings.get('gridExtent'));
      b.setComponent((d + 1) % 3, settings.get('gridExtent'));
      a.setComponent((d + 2) % 3, i);
      b.setComponent((d + 2) % 3, i);
      geometry.vertices.push(a);
      geometry.vertices.push(b);

      a = new THREE.Vector3(0, 0, 0);
      b = new THREE.Vector3(0, 0, 0);
      a.setComponent((d + 1) % 3, i);
      b.setComponent((d + 1) % 3, i);
      a.setComponent((d + 2) % 3, -settings.get('gridExtent'));
      b.setComponent((d + 2) % 3, settings.get('gridExtent'));
      geometry.vertices.push(a);
      geometry.vertices.push(b);
    }

    var geometry = new THREE.Geometry();
    generateLines(0);
    for (var i = settings.get('gridSpacing'); i <= settings.get('gridExtent'); i += settings.get('gridSpacing')) {
      generateLines(i);
      generateLines(-i);
    }

    grids[d] = new THREE.LineSegments(geometry, new THREE.LineBasicMaterial({color: colors[d], linewidth: 1}));
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
      var labels = "XYZ";
      settings.set('showGrid' + labels.charAt(d), this.checked);
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

  $('#fitGrid').click(function() {
    if (allGeometry !== undefined) {
      allGeometry.computeBoundingBox();
      var bounds = allGeometry.boundingBox;
      var maxStretch = Math.max(
        Math.abs(bounds.min.x),
        Math.abs(bounds.min.y),
        Math.abs(bounds.min.z),
        Math.abs(bounds.max.x),
        Math.abs(bounds.max.y),
        Math.abs(bounds.max.z)
      );

      settings.set('gridExtent', maxStretch);
      $('#gridExtent').val(maxStretch);
      onGridExtentChange();
    }
  });

  function onGridExtentChange() {
    for (var d = 0; d < 3; ++d) {
      if (axes[d]) {
        generateAxis(d);
      }
      if (grids[d]) {
        generateGrid(d);
      }
    }
  }

  $('#gridExtent').change(function() {
    settings.set('gridExtent', parseFloat($(this).val()));
    onGridExtentChange();
  });

  $('#gridSpacing').change(function() {
    settings.set('gridSpacing', parseFloat($(this).val()));
    for (var d = 0; d < 3; ++d) {
      if (grids[d]) {
        generateGrid(d);
      }
    }
  });

  // Axes and grids
  var labels = "XYZ";
  for (var i = 0; i < 3; ++i) {
    var d = labels[i];

    if (settings.get('showAxis' + d)) {
      $('#axis' + d).prop('checked', true);
      generateAxis(i);
    }

    if (settings.get('showGrid' + d)) {
      $('#grid' + d).prop('checked', true);
      generateGrid(i);
    }
  }

  $('#autopathify').click(function() {
    settings.set('isAutopathify', this.checked);

    if (this.checked) {
      $('#nSecondsTillAutopathify').prop('disabled', false);
    } else {
      $('#nSecondsTillAutopathify').prop('disabled', true);
      if (autopathifyTask) {
        clearTimeout(autopathifyTask); 
        autopathifyTask = undefined;
      }
    }
  });

  $('#showMode').change(function() {
    settings.set('showMode', $('#showMode').find(":selected").val());
    run(getSource(), GeometryMode.PATH);
  });

  $('#showPoints').click(function() {
    clearPoints();
    settings.set('showPoints', this.checked);
    run(getSource(), GeometryMode.PATH);
  });

  $('#exportScreenshot').click(exportScreenshot);

  enableDownload(false);
  $('.export3D').click(function() {
    var id = this.id;
    var type = id.substring(id.length - 3).toLowerCase();
    $('#extension').val(type);
    $('#tag').val(mup.name);
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
    sendToChris(source);
  });

  $('#solidify-button').click(function() {
    syncSettings();
    run(getSource(), GeometryMode.SURFACE);
    focusEditor();
  });

  $('#pathify-button').click(function() {
    syncSettings();
    run(getSource(), GeometryMode.PATH);
    focusEditor();
  });

  $('#breakout').click(function() {
    // I tried using $.post for this, but the new window didn't show
    // the source or URL correctly.
    $('#breakout-form input[name=src]').val(getSource());
    $('#breakout-form').submit();
  });

  // This isn't UFW. jQuery will send a parameter to the callback. I
  // don't want that parameter in this case.
  $('#settings-button').click(function() {
    showGearMenu();
  });
  $('#close-settings-button').click(function() {
    hideGearMenu();
  });

  $('ul#settings > li > .panel-section-label').click(function() {
    $(this).toggleClass('open-panel-section-label');
    $(this).next().slideToggle(200, function() {
      refreshBlocklyWorkspaces();
    });
  });

  $('#panel-section-tutorial > .panel-section-label').click(function() {
    if ($('#panel-section-tutorial > .panel-section-content').is(':empty')) {
      $('#panel-section-tutorial > .panel-section-content').load('docs/html/index.html', function() {
        docify();
      });
    } else {
      docify();
    }
  });

  $('#panel-section-lesson > .panel-section-label').click(function() {
    if ($('#panel-section-lesson > .panel-section-content').is(':empty')) {
      $('#panel-section-lesson > .panel-section-content').load('docs/html/' + lesson + '.html', function() {
        docify();
      });
    } else {
      docify();
    }
  });

  $('#left').resizable({
    handles: "e",
    resize: function(event, ui) {
      window.dispatchEvent(new Event('resize'))
    } 
  });

  $('#right').resizable({
    handles: "w",
    minWidth: 300,
    resize: function(event, ui) {
      window.dispatchEvent(new Event('resize'))
    } 
  });

  $('#console').resizable({
    handles: "n",
    resize: function(event, ui) {
      window.dispatchEvent(new Event('resize'))
    } 
  });

  if (lesson) {
      showGearMenu(function() {
        $('#panel-section-lesson .panel-section-label').click();
      });
  } else {
    $('#panel-section-lesson').hide();
  }

  Mousetrap.bind('ctrl+s', save);
  Mousetrap.bind('ctrl+shift+s', promptForSaveAs);
  Mousetrap.bind('ctrl+,', pathify);
  Mousetrap.bind('ctrl+.', solidify);
  Mousetrap.bind('ctrl+/', fit);
  Mousetrap.bind('ctrl+shift+/', toggleGearMenu);
  Mousetrap.bind('ctrl+0', exportScreenshot);
  Mousetrap.bind('ctrl+t', logText);
  Mousetrap.bind('esc', focusParent);
  Mousetrap.bind('ctrl+[', decreaseFontSize);
  Mousetrap.bind('ctrl+]', increaseFontSize);
  Mousetrap.bind('ctrl+9', logAbtractSyntaxTree);

  // Form elements don't get events in the same way. We must explicitly bind.
  forEach(document.querySelectorAll('input, select, textarea'), function(i, element) {
    var mousetrap = new Mousetrap(element);
    mousetrap.bind('ctrl+s', save);
    mousetrap.bind('ctrl+shift+s', promptForSaveAs);
    mousetrap.bind('ctrl+,', pathify);
    mousetrap.bind('ctrl+.', solidify);
    mousetrap.bind('ctrl+/', fit);
    mousetrap.bind('ctrl+shift+/', toggleGearMenu);
    mousetrap.bind('ctrl+0', exportScreenshot);
    mousetrap.bind('ctrl+t', logText);
    mousetrap.bind('esc', focusParent);
    mousetrap.bind('ctrl+[', decreaseFontSize);
    mousetrap.bind('ctrl+]', increaseFontSize);
    mousetrap.bind('ctrl+9', logAbtractSyntaxTree);
  });
});

function logAbtractSyntaxTree() {
  textToAbstractSyntaxTree(getSource(),
    function(data) {
      console.log(data['tree']);
    },
    function() {
      console.log('Failure. :(');
    }
  );
}

function pathify() {
  run(getSource(), GeometryMode.PATH);
}

function solidify() {
  run(getSource(), GeometryMode.SURFACE);
}

function toggleGearMenu() {
  if ($('#right').css('display') == 'none') {
    showGearMenu();
  } else {
    hideGearMenu();
  }
}

function resizeGearMenu() {
  // If gear menu contents exceed its size, grow it.
  var settingsWidth = $('ul#settings').width();
  if (settingsWidth > $('#right').width()) {
    $('#right').width($('ul#settings').width());
  }
}

function logText() {
  if (!settings.get('isEditorText')) {
    var source = Blockly.Madeup.workspaceToCode(blocklyWorkspace);
    log(source);
  }
}

function focusParent() {
  parent.focus();
}

function increaseFontSize() {
  setFontSize(settings.get('fontSize') + 2);
  resize();
  return false;
}

function decreaseFontSize() {
  setFontSize(settings.get('fontSize') - 2);
  resize();
  return false;
}

function setFontSize(newSize) {
  settings.set('fontSize', newSize);
  textEditor.setFontSize(settings.get('fontSize'));
  docEditors.forEach(function(editor) {
    editor.setFontSize(settings.get('fontSize'));
  });
  $('#console')[0].style.fontSize = settings.get('fontSize') + 'px';
  $('ul#settings')[0].style.fontSize = (settings.get('fontSize') + 0) + 'px';
  $('#right input, #right select, #info3').css('font-size', newSize);
  resizeGearMenu();
}

function showGearMenu(callback) {
  if ($('#right').css('display') == 'none') {
    $('#settings-button').fadeToggle(100, function() {
      $('#right').toggle('slide', {direction: 'right', duration: 500}, function() {
        resizeGearMenu();
        resize();
        $('#right').css('overflow', 'visible');
        $('#close-settings-button').fadeToggle(500);
        if (callback) {
          callback();
        }
      });
    });
  }
}

function hideGearMenu() {
  $('#close-settings-button').fadeToggle(100, function() {
    $('#right').toggle('slide', {direction: 'right', duration: 500}, function() {
      resize();
      $('#settings-button').fadeToggle(500);
    });
  });
}

// Only triggered when blocks editor is active.
function onBlocksChanged() {
  var xml = Blockly.Xml.workspaceToDom(blocklyWorkspace);
  var currentBlocks = Blockly.Xml.domToText(xml);
  mup.isDirty = lastBlocks != currentBlocks;
  onSourceChanged();
}

// Only triggered when text editor is active.
function onTextChanged(delta) {
  mup.isDirty = true;
  onSourceChanged();
}

// Triggered on change to blocks or text source.
function onSourceChanged() {
  enableDownload(false);
  updateTitle();

  if (autopathifyTask) {
    clearTimeout(autopathifyTask); 
  }

  if (settings.get('isAutopathify')) {
    autopathifyTask = setTimeout(function() {
      run(getSource(), GeometryMode.PATH);
    }, settings.get('nSecondsTillAutopathify') * 1000);
  }

  // If this is a recorded lesson, schedule a snapshot to be taken in a few
  // seconds.
  //
  // The snapshot may get postponed if more events happen before the quantum.
  // We don't want to record absolutely every keystroke.
  if (snapshotTask) {
    clearTimeout(snapshotTask); 
  }

  if (lesson != null && isSnapshot) {
    snapshotTask = setTimeout(recordSnapshot, 2 * 1000);
  }
}

function recordSnapshot() {
  $.ajax({
    type: 'POST',
    url: 'snapshot.php',
    data: JSON.stringify({
      source: getSource(),
      lesson: lesson,
      id: sessionID
    }),
    contentType: 'application/json; charset=utf-8',
    success: function(data) {
    },
    error: function(e) {
      console.log(e);
    }
  });
}

function setTheme(isDark) {
  settings.set('isThemeDark', isDark);

  // Update radio buttons to reflect current editor.
  if (settings.get('isThemeDark')) {
    $("#isDark").prop('checked', true);
  } else {
    $("#isLight").prop('checked', true);
  }

  $('link[title="theme"]').attr('href', settings.get('isThemeDark') ? 'css/ide_skin_dark.css' : 'css/ide_skin_light.css');
  textEditor.setTheme(settings.get('isThemeDark') ? 'ace/theme/twilight' : 'ace/theme/katzenmilch');

  docEditors.forEach(function(editor) {
    editor.setTheme(settings.get('isThemeDark') ? 'ace/theme/twilight' : 'ace/theme/katzenmilch');
  });
}

function setEditor(isText) {
  settings.set('isEditorText', isText);

  // We're heading to text.
  if (settings.get('isEditorText')) {
    // If we have blocks, let's offer to convert them to text.
    if (blocklyWorkspace != null && blocklyWorkspace.getTopBlocks().length > 0) {
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
        media: 'repos/blockly/media/',
        trashcan: true,
        collapse: false,
        zoom: {
          controls: true,
          wheel: false,
          startScale: 1.5,
          maxScale: 5,
          minScale: 0.3,
          scaleSpeed: 1.2
        }
      });
      clearWorkspace();
    }

    // Any text to convert to blocks?
    if (textEditor != null && textEditor.getValue().length > 0) {
      $('<div title="Convert">Convert your text program to a blocks program?</div>').dialog({
        resizable: false,
        height: 'auto',
        width: 400,
        modal: true,
        buttons: {
          'Convert to blocks': function() {
            clearWorkspace();
            textToAbstractSyntaxTree(textEditor.getValue(),
              function(data) {
                if (data['exit_status'] == 0) {
                  var tree = data['tree'];
                  parse(new Peeker(tree), blocklyWorkspace);
                }
                ensureBuiltinVariables();
              },
              function() {
                console.log('Failure. :(');
              });
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

    ensureBuiltinVariables();
  }
 
  // We want to listen for change events in the editor for a few reasons: a
  // change may need to trigger an autopathify task, and a change may need to
  // be recorded if we're in experiment observation mode.
  if (isText) {
    if (textEditor) {
      textEditor.getSession().on('change', onTextChanged);
    }
    if (blocklyWorkspace) {
      blocklyWorkspace.removeChangeListener(onBlocksChanged);
    }
    $("#isEditorText").prop('checked', true);
  } else {
    if (textEditor) {
      textEditor.getSession().off('change', onTextChanged);
    }
    if (blocklyWorkspace) {
      blocklyWorkspace.addChangeListener(onBlocksChanged);
    }
    $("#isEditorBlocks").prop('checked', true);
  }
}

function switchEditors() {
  if (settings.get('isEditorText')) {
    $('#blocksEditor').hide();
    $('#textEditor').show();
    docsToText();
  } else {
    $('#textEditor').hide();
    $('#blocksEditor').show();
    docsToBlocks();
  }

  // resize();
  window.dispatchEvent(new Event('resize'))
}

function load(newMup) {
  if (mup && mup.isDirty && confirm('Save changes to ' + mup.name + '?')) {
    save();
  }

  // Clear the editors so they don't try to get converted.
  textEditor.setValue('');
  if (blocklyWorkspace) {
    clearWorkspace();
    // blocklyWorkspace.clear();
    // blocklyWorkspace.updateVariableStore();
  }

  mup = newMup;

  // Remove meshes and previews.
  clearModel();
  if (renderer) render();

  platformLoad(mup, function(source) {
    var isText;
    if (source.length == 0) {
      isText = settings.get('isEditorText');
    } else {
      isText = source.charAt(0) != '<';
    }

    setEditor(isText);
    if (settings.get('isEditorText')) {
      textEditor.session.setValue(source, -1);
    } else {
      clearWorkspace();
      // If source is blank, XML parse will fail.
      if (source.length > 0) {
        var xml = Blockly.Xml.textToDom(source);
        Blockly.Xml.domToWorkspace(xml, blocklyWorkspace);
        xml = Blockly.Xml.workspaceToDom(blocklyWorkspace);
        lastBlocks = Blockly.Xml.domToText(xml);
      }
      ensureBuiltinVariables();
      // blocklyWorkspace.updateVariableStore(); TODO do I need this?
    }

    // TODO toggle modes
    mup.isDirty = false;
    updateTitle();
  });
}

function save() {
  if (mup.name == null) {
    promptForSaveAs();
  } else {
    updateTitle();
    var mode = null;
    var source = null;
    if (settings.get('isEditorText')) {
      source = getSource();
      mode = 'text';
    } else {
      var xml = Blockly.Xml.workspaceToDom(blocklyWorkspace);
      source = Blockly.Xml.domToText(xml);
      mode = 'blocks';
    }

    platformSave(mup, source, mode, function() {
      mup.isDirty = false;
      updateTitle();
    });
  }

  return false;
}

function getSource() {
  if (settings.get('isEditorText')) {
    return textEditor.getValue();
  } else {
    return Blockly.Madeup.workspaceToCode(blocklyWorkspace);
  }
}

function run(source, mode, pingback) {
  // If an autopathify task is scheduled, unschedule it, as we have now met its
  // demands.
  if (autopathifyTask) {
    clearTimeout(autopathifyTask); 
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
    shading_mode: settings.get('isFlatShaded') ? 'FLAT' : 'SMOOTH'
  }, function(data) {
    onInterpret(data);
    if (pingback) {
      pingback();
    }
  }, function(errorMessage) {
    console.log('Failure. :(');
  });
}

function clearPoints() {
  for (var i = pointScene.children.length - 1; i >= 0; --i) {
    pointScene.remove(pointScene.children[i]);
  }
}

function clearLines() {
  for (var i = 0; i < lineMeshes.length; ++i) {
    modelScene.remove(lineMeshes[i]);
  }
  lineMeshes = [];

  for (var i = 0; i < 3; ++i) {
    if (axes[i] && axes[i].mesh) {
      glyphScene.remove(axes[i].mesh);
    }
  }

  // We intentionally don't remove the underlying line geometries. We only
  // want these old lines to be redrawn.
}

function clearModel() {
  for (var i = 0; i < meshes.length; ++i) {
    modelScene.remove(meshes[i]);
  }
  clearPoints();
  clearLines();
  meshes = [];
  lineEssences = [];
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
    clearModel();
    log(sansDebug);
    
    if (data.geometry_mode == GeometryMode.SURFACE) {
      var loader = new THREE.JSONLoader();
      try {
        var model = loader.parse(JSON.parse(data['model'].replace(/NaN/gi, 0)));

        if (settings.get('showMode') == 'triangles' || settings.get('showMode') == 'shaded_triangles') {
          var solidMaterial;
          if (settings.get('showMode') == 'shaded_triangles') {
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
          var frontMaterial = new THREE.MeshLambertMaterial({vertexColors: THREE.VertexColors, wireframe: settings.get('showMode') == 'wireframe', wireframeLinewidth: 5, side: THREE.FrontSide});
          var backMaterial;
          if (settings.get('lightBothSides')) {
            backMaterial = new THREE.MeshLambertMaterial({vertexColors: THREE.VertexColors, wireframe: settings.get('showMode') == 'wireframe', wireframeLinewidth: 5, side: THREE.BackSide});
          } else {
            backMaterial = new THREE.MeshBasicMaterial({color: 0x000000, wireframe: settings.get('showMode') == 'wireframe', wireframeLinewidth: 5, side: THREE.BackSide});
          }
          var material = [frontMaterial, backMaterial];
          meshes[0] = THREE.SceneUtils.createMultiMaterialObject(model.geometry, material);
        }

        allGeometry = model.geometry;
        enableDownload(true);
      } catch (err) {
        console.log(err);
        console.log(data['model']);
        log(badModelMessage);
      }
    } else {
      var sphereGeometry = new THREE.SphereGeometry(settings.get('pathifyNodeSize'), 20, 20);
      var sphereMaterial = new THREE.MeshBasicMaterial({color: 'rgb(0, 0, 0)'});

      var paths = [];
      try {
        paths = JSON.parse(data['model']);
      } catch (err) {
        log(badModelMessage);
      }

      allGeometry = new THREE.Geometry();

      for (var pi = 0; pi < paths.length; ++pi) {
        var path = paths[pi];
        var pathPositions = path.vertices;
 
        // Get a list of just the unique node positions. We don't really want
        // duplicates in the glyph drawing and raycasting.
        var nodePositions = [];
        var geometry = new THREE.Geometry();
        for (var i = 0; i < pathPositions.length; ++i) {
          var position = new THREE.Vector3(pathPositions[i][0], pathPositions[i][1], pathPositions[i][2]);

          geometry.vertices.push(position);
          allGeometry.vertices.push(position);

          var isUnique = true;
          for (var ni = 0; isUnique && ni < nodePositions.length; ++ni) {
            if (nodePositions[ni].equals(position)) {
              isUnique = false;
            }
          }

          if (isUnique) {
            nodePositions.push(position);
          }
        }

        // Remove the last item if that's where the arrowhead appears. We don't
        // want a dot there.
        if (settings.get('showHeadings') && pathPositions.length > 0) {
          var last = pathPositions[pathPositions.length - 1];
          var position = new THREE.Vector3(last[0], last[1], last[2]);
          if (position.equals(nodePositions[nodePositions.length - 1])) {
            nodePositions.pop();
          }
        }

        // Add the unique positions into the point cloud list.
        for (var i = 0; i < nodePositions.length; ++i) {
          var node = new THREE.Mesh(sphereGeometry, sphereMaterial);
          node.position.copy(nodePositions[i]);
          if (settings.get('showPoints')) {
            pointScene.add(node);
          }
        }

        // Decimate line because MeshLine doesn't handle miters. We just add
        // two more points to each line segment.
        if (geometry.vertices.length > 1) {
          var fracturedGeometry = new THREE.Geometry();
          var vertices = geometry.vertices;
  
          // Add each line segments' starting vertex, plus another one epsilon
          // away from it, plus another one epsilon away from the successor
          // vertex.
          for (var i = 0; i < vertices.length - 1; ++i) {
            fracturedGeometry.vertices.push(vertices[i]);
            var distance = vertices[i].distanceTo(vertices[i + 1]);
            if (distance > 0.01) {
              var direction = new THREE.Vector3();
              direction.subVectors(vertices[i + 1], vertices[i]);
              direction.normalize();

              var p = vertices[i].clone();
              p.addScaledVector(direction, 0.005);
              fracturedGeometry.vertices.push(p);

              p = vertices[i + 1].clone();
              p.addScaledVector(direction, -0.005);
              fracturedGeometry.vertices.push(p);
            }
          }

          // Add last one on.
          fracturedGeometry.vertices.push(vertices[vertices.length - 1]);

          lineEssences.push({
            geometry: fracturedGeometry,
            color: '#CC6600',
            width: settings.get('pathifyLineSize')
          });
        }

        var nvertices = pathPositions.length;
        if (settings.get('showHeadings') && nvertices > 0) {
          var m = path.orientation;
          
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
          var offset = new THREE.Vector3(pathPositions[pathPositions.length - 1][0], pathPositions[pathPositions.length - 1][1], pathPositions[pathPositions.length - 1][2]);

          g2.computeFaceNormals();
          allGeometry.vertices = allGeometry.vertices.concat(g2.vertices);

          var cursorMesh = new THREE.Mesh(g2, new THREE.MeshLambertMaterial({
            color: 0x0000ff,
          }));
          cursorMesh.position.copy(offset);
          pointScene.add(cursorMesh);
        }
      }
    }

    for (var mi = 0; mi < meshes.length; ++mi) {
      modelScene.add(meshes[mi]);
    }
    generateLines();

    render();
  } else if (data['exit_status'] == 22) {
    log(data['stdout'] + '\nYour model was taking a long time to build. It felt like it was never going to finish! So, I stopped trying. Sorry.');
  } else {
    log(sansDebug);
  }
}

function getLineMesh(lineEssence) {
  var polyline = new MeshLine();
  polyline.setGeometry(lineEssence.geometry);
  var material = getLineMaterial(lineEssence.width, lineEssence.color);
  var mesh = new THREE.Mesh(polyline.geometry, material);
  return mesh;
}

function generateLines() {
  for (var i = 0; i < lineEssences.length; ++i) {
    var mesh = getLineMesh(lineEssences[i]);
    lineMeshes.push(mesh);
    modelScene.add(mesh);
  }

  for (var i = 0; i < 3; ++i) {
    if (axes[i]) {
      if (axes[i].mesh) {
        glyphScene.remove(axes[i].mesh);
      }
      axes[i].mesh = getLineMesh(axes[i]);
      glyphScene.add(axes[i].mesh);
    }
  }
}

function fit() {
  // Must return false. Otherwise everything gets highlighted on Linux.
 
  if (allGeometry !== undefined) {
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

  return false;
}

function viewFrom(dim, sign) {
  if (allGeometry === undefined) {
    var bounds = new THREE.Box3(new THREE.Vector3(-7, -7, -7), new THREE.Vector3(7, 7, 7));
  } else {
    allGeometry.computeBoundingBox();
    var bounds = allGeometry.boundingBox;
  }

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
    if (settings.get('isEditorText')) {
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

  var nonChromeHeight = $(window).height();
  var flexHeight = nonChromeHeight - $('#console').height();

  var rightWidth = 0;
  if ($('#right').css('display') != 'none') {
    rightWidth = $('#right').outerWidth();
  }

  var width = $(window).width() - $('#left').width() - rightWidth;
  $('#glcanvas').css({left: $('#left').width(), right: rightWidth});
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

  if (textEditor) {
    textEditor.resize();
  }

  clearLines();
  generateLines();

  if (renderer) {
    render();
  }
}

function squareport() {
  var height = window.innerHeight;
  var rightWidth = 0;
  if ($('#right').css('display') != 'none') {
    rightWidth = $('#right').outerWidth();
  }

  var leftover = $(window).width() - height - rightWidth;
  $('#left').width(leftover);
  resize();
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
  renderer = new THREE.WebGLRenderer({
    antialias: true,
    preserveDrawingBuffer: true,
    alpha: true
  });

  renderer.setClearColor(0xFFFFFF, 0);
  // renderer.setClearColor(0xCCCCCC, 1);
  document.getElementById("glcanvas").appendChild(renderer.domElement);
  renderer.domElement.addEventListener('touchmove', function(e) {
    e.preventDefault();
  }, false);

  // controls = new THREE.OrbitControls(camera, renderer.domElement);  
  controls = new THREE.TrackballControls(camera, renderer.domElement);
  controls.addEventListener('change', render);

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

  pointScene = new THREE.Scene();
  modelScene.add(pointScene);

  glyphScene = new THREE.Scene();
  modelScene.add(glyphScene);

  raycaster = new THREE.Raycaster();
  mouse = new THREE.Vector2();

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
  $(glcanvas).mousemove(function(e) {
    var parent = glcanvas.offset();  
    var x = e.pageX - parent.left;
    var y = e.pageY - parent.top;
    mouse.x = x / glcanvas.width() * 2 - 1;
    mouse.y = -y / glcanvas.height() * 2 + 1;

    raycaster.setFromCamera(mouse, camera);
    var intersections = raycaster.intersectObjects(pointScene.children);
    var message = '';
    for (var i = 0; i < intersections.length; ++i) {
      message += intersections[i].object.position.x + ', ' + intersections[i].object.position.y + ', ' + intersections[i].object.position.z + '\n';
    }

    var oldMessage = $('#info3').text();
    $('#info3').text(message);

    // If we are transition between no message and message, let's smooth out
    // the entrance/exit.
    if (message.length == 0 && oldMessage.length > 0) {
      $('#info3').fadeOut(1000);
    } else if (message.length > 0 && oldMessage.length == 0) {
      $('#info3').fadeIn(1000);
    }
  });
  initialized = true;
  resize();
}

function clearWorkspace() {
  // console.trace("clear");
  blocklyWorkspace.clear();
}

function ensureBuiltinVariables() {
  ids = ['nsides', '.rgb', '.radius', '.innerRadius', '.outerRadius'];
  for (var i = 0; i < ids.length; ++i) {
    var id = ids[i];
    if (blocklyWorkspace.getVariable(id) == null) {
      blocklyWorkspace.createVariable(id);
    }
  }
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

function docsToText() {
  var switchers = document.querySelectorAll('.mup-switcher');
  forEach(switchers, function(i, switcher) {
    var textEditor = switcher.children[0];
    $(textEditor).height(150 + 'px');
    var blocksEditor = switcher.children[1];
    textEditor.style.display = 'block';
    blocksEditor.style.display = 'none';
  });
}

function refreshBlocklyWorkspaces() {
  docWorkspaces.forEach(function(workspace) {
    Blockly.svgResize(workspace);
  });
}

function docsToBlocks() {
  var switchers = document.querySelectorAll('.mup-switcher');
  forEach(switchers, function(i, switcher) {
    var textEditor = switcher.children[0];
    var blocksEditor = switcher.children[1];
    textEditor.style.display = 'none';
    blocksEditor.style.display = 'block';
  });
  refreshBlocklyWorkspaces();
  resizeGearMenu();
}

// document.addEventListener('DOMContentLoaded', function() {
function docify() {
  docEditors = [];
  docWorkspaces = [];

  // Inject Ace editors on all the code snippets.
  var editors = document.querySelectorAll('.text-editor');
  forEach(editors, function(i, element) {
    var editor = ace.edit(element);
    docEditors.push(editor);
    editor.$blockScrolling = Infinity;
    editor.renderer.$cursorLayer.element.style.display = "none";
    editor.setTheme(settings.get('isThemeDark') ? 'ace/theme/twilight' : 'ace/theme/katzenmilch');
    editor.getSession().setMode("ace/mode/madeup");
    editor.setOptions({
      maxLines: 15,
      showPrintMargin: false,
      highlightActiveLine: false,
      highlightGutterLine: false,
      readOnly: true,
      fontSize: settings.get('fontSize')
    });
    editor.renderer.setScrollMargin(10, 10);
    editor.resize();
  });

  // Inject Blockly workspaces on all the code snippets.
  var switchers = document.querySelectorAll('.mup-switcher');
  forEach(switchers, function(i, switcher) {
    var textEditor = switcher.children[0];
    var blocksEditor = switcher.children[1];

    // Purge old workspace.
    $(blocksEditor.children).slice(1).remove();

    var workspace = Blockly.inject(blocksEditor, {
      comments: false,
      toolbox: false,
      trashcan: false,
      readOnly: true,
      scrollbars: false,
      zoom: false
    });

    // Fit the container exactly around the blocks.
    var sExpression = blocksEditor.firstElementChild.innerHTML;
    parse(new Peeker(sExpression), workspace);
    var metrics = workspace.getMetrics();
    blocksEditor.style.height = metrics.contentHeight + 'px';
    blocksEditor.style.width = metrics.contentWidth + 'px';
    Blockly.svgResize(workspace);

    blocksEditor.style.display = 'none';

    docWorkspaces.push(workspace);
  });

  if (settings.get('isEditorText')) {
    docsToText();
  } else {
    docsToBlocks();
  }

  // Switch all anchor elements to load.
  var docsSelector = '#panel-section-tutorial > .panel-section-content';
  $(docsSelector + ' a').each(function(index, anchor) {
    anchor.onclick = function() {
      $(docsSelector).load(anchor.href, function() {
        if (!anchor.href.endsWith('docs/html/index.html')) {
          var html = '<a href="docs/html/index.html" class="toc">back to index</a>';
          $(docsSelector).prepend(html);
          $(docsSelector).append(html);
        }
        docify();
      });
      return false;
    };
  });

  $(function() {
    $('.togglee').css('display', 'none');
    $('.toggler').click(function() {
      if ($(this).next().css('display') == 'none') {
        $(this).next().slideDown();
      } else {
        $(this).next().slideUp();
      }
    });
  });
}
