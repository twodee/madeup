function hasWebGL() {
  try {
    var canvas = document.createElement("canvas");
    return !!window.WebGLRenderingContext && (canvas.getContext("webgl") || canvas.getContext("experimental-webgl"));
  } catch(e) { 
    return false;
  } 
}

window.onbeforeunload = function() {
  if (mupName && isSourceDirty) {
    return 'You have unsaved changes.';
  }
};

var lastBlocks = null;
function onBlocksChanged() {
  var xml = Blockly.Xml.workspaceToDom(blocklyWorkspace);
  var currentBlocks = Blockly.Xml.domToText(xml);
  isSourceDirty = lastBlocks != currentBlocks;
  updateTitle();
  run(getSource(), GeometryMode.PATH);
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
  var procs = Blockly.Procedures.allProcedures(blocklyWorkspace)[1];

  // Find the procedure in the list with the given name.
  for (var i = 0; i < procs.length; ++i) {
    console.log("procedure " + i + ": " + procs[i][0]);
    if (procs[i][0] == name) {
      return procs[i][1];
    }
  }

  throw 'No procedure named ' + name;
}

function Peeker(src) {
  this.i = 0;
  this.src = src;
  this.peek = function() {
    return this.src.charAt(this.i);
  };
  this.get = function() {
    var c = this.peek();
    ++this.i;
    return c;
  };
  this.getToken = function() {
    var token = '';
    while (this.peek() != ' ' && this.peek() != ')') {
      token += this.get();
    }
    return token;
  }
  this.getInteger = function() {
    var isNegative = false;
    var num = '';

    if (this.peek() == '-') {
      isNegative = true;
      this.get(); 
    }

    while (/^\d+$/.test(this.peek())) {
      num = num + this.get();
    }

    num = parseInt(num);
    return isNegative ? -num : num;
  };
  this.getReal = function() {
    var isNegative = false;
    var num = '';

    if (this.peek() == '-') {
      isNegative = true;
      this.get(); 
    }

    while (/^\d+$/.test(this.peek())) {
      num = num + this.get();
    }

    if (this.peek() == '.') {
      num = num + this.get();
    }

    while (/^\d+$/.test(this.peek())) {
      num = num + this.get();
    }

    num = parseFloat(num);
    return isNegative ? -num : num;
  };
}

function parse(peeker) {
  var block = null;

  // assert '('
  if (peeker.peek() != '(') {
    throw 'expecting (';
  } else {
    peeker.get();
  }

  // figure out contents
  var token = peeker.getToken();

  if (token == 'block') {
    if (peeker.peek() == ' ') {
      peeker.get(); // eat space
      block = parse(peeker);
      var prevStatement = block;
      while (peeker.peek() == ' ') {
        peeker.get(); // eat space
        var statement = parse(peeker);

        // If statement can have predecessor and predecessor can have a successor...
        if (statement.previousConnection && prevStatement.nextConnection) {
          prevStatement.nextConnection.connect(statement.previousConnection);
        }

        // If this statement can have a successor, set it up for connection to next.
        if (statement.nextConnection) {
          prevStatement = statement;
        }
      }
    }
  }

  else if (token == 'call' || token == 'call-with-names') {
    peeker.get(); // eat space
    id = peeker.getToken();
    if (id == 'print') {
      peeker.get(); // eat space
      var msg = parse(peeker);
      block = Blockly.Block.obtain(blocklyWorkspace, 'madeup_io_print');
      block.getInput('MESSAGE').connection.connect(msg.outputConnection);
    } else if (id == 'debug') {
      peeker.get(); // eat space
      var msg = parse(peeker);
      block = Blockly.Block.obtain(blocklyWorkspace, 'madeup_io_debug');
      block.getInput('MESSAGE').connection.connect(msg.outputConnection);
    } else if (id == 'sign') {
      peeker.get(); // eat space
      var x = parse(peeker);
      block = Blockly.Block.obtain(blocklyWorkspace, 'madeup_math_sign');
      block.getInput('X').connection.connect(x.outputConnection);
    } else if (id == 'moveto') {
      peeker.get(); // eat space
      var x = parse(peeker);
      peeker.get(); // eat space
      var y = parse(peeker);
      peeker.get(); // eat space
      var z = parse(peeker);
      block = Blockly.Block.obtain(blocklyWorkspace, 'madeup_movement_moveto');
      block.getInput('X').connection.connect(x.outputConnection);
      block.getInput('Y').connection.connect(y.outputConnection);
      block.getInput('Z').connection.connect(z.outputConnection);
    } else if (id == 'move') {
      peeker.get(); // eat space
      var x = parse(peeker);
      block = Blockly.Block.obtain(blocklyWorkspace, 'madeup_movement_move');
      block.getInput('DISTANCE').connection.connect(x.outputConnection);
    } else if (id == 'random') {
      peeker.get(); // eat space
      var min = parse(peeker);
      peeker.get(); // eat space
      var max = parse(peeker);
      block = Blockly.Block.obtain(blocklyWorkspace, 'madeup_math_random');
      block.getInput('MIN').connection.connect(min.outputConnection);
      block.getInput('MAX').connection.connect(max.outputConnection);
    } else if (id == 'yaw' || id == 'roll' || id == 'pitch') {
      peeker.get(); // eat space
      var degrees = parse(peeker);
      block = Blockly.Block.obtain(blocklyWorkspace, 'madeup_movement_turn');
      block.setFieldValue(id, 'TYPE');
      block.getInput('DEGREES').connection.connect(degrees.outputConnection);
    } else if (id == 'translate' || id == 'scale') {
      peeker.get(); // eat space
      var x = parse(peeker);
      peeker.get(); // eat space
      var y = parse(peeker);
      peeker.get(); // eat space
      var z = parse(peeker);
      block = Blockly.Block.obtain(blocklyWorkspace, 'madeup_movement_' + id);
      block.getInput('X').connection.connect(x.outputConnection);
      block.getInput('Y').connection.connect(y.outputConnection);
      block.getInput('Z').connection.connect(z.outputConnection);
    } else if (id == 'spheres' || id == 'forget' || id == 'boxes' || id == 'polygon') {
      block = Blockly.Block.obtain(blocklyWorkspace, 'madeup_generate_' + id);
    } else if (id == 'revolve') {
      peeker.get(); // eat space
      var x = parse(peeker);
      peeker.get(); // eat space
      var y = parse(peeker);
      peeker.get(); // eat space
      var z = parse(peeker);
      peeker.get(); // eat space
      var degrees = parse(peeker);
      block = Blockly.Block.obtain(blocklyWorkspace, 'madeup_generate_revolve');
      block.getInput('X').connection.connect(x.outputConnection);
      block.getInput('Y').connection.connect(y.outputConnection);
      block.getInput('Z').connection.connect(z.outputConnection);
      block.getInput('DEGREES').connection.connect(degrees.outputConnection);
    } else if (id == 'extrude') {
      peeker.get(); // eat space
      var x = parse(peeker);
      peeker.get(); // eat space
      var y = parse(peeker);
      peeker.get(); // eat space
      var z = parse(peeker);
      peeker.get(); // eat space
      var length = parse(peeker);
      block = Blockly.Block.obtain(blocklyWorkspace, 'madeup_generate_extrude');
      block.getInput('X').connection.connect(x.outputConnection);
      block.getInput('Y').connection.connect(y.outputConnection);
      block.getInput('Z').connection.connect(z.outputConnection);
      block.getInput('LENGTH').connection.connect(length.outputConnection);
    } else if (id == 'surface') {
      peeker.get(); // eat space
      var columns = parse(peeker);
      peeker.get(); // eat space
      var rows = parse(peeker);
      block = Blockly.Block.obtain(blocklyWorkspace, 'madeup_generate_surface');
      block.getInput('COLUMNS').connection.connect(columns.outputConnection);
      block.getInput('ROWS').connection.connect(rows.outputConnection);
    } else if (id == 'dowel' || id == 'tube') {
      var maxBend;
      if (peeker.peek() == ' ') {
        peeker.get(); // eat space
        maxBend = parse(peeker);
      } else {
        maxBend = Blockly.Block.obtain(blocklyWorkspace, 'madeup_math_integer');
        maxBend.setFieldValue(360, 'INTEGER');
        maxBend.initSvg();
        maxBend.render();
      }
      block = Blockly.Block.obtain(blocklyWorkspace, 'madeup_generate_' + id);
      block.getInput('MAXBEND').connection.connect(maxBend.outputConnection);
    } else if (id == 'rotate') {
      peeker.get(); // eat space
      var x = parse(peeker);
      peeker.get(); // eat space
      var y = parse(peeker);
      peeker.get(); // eat space
      var z = parse(peeker);
      peeker.get(); // eat space
      var degrees = parse(peeker);
      block = Blockly.Block.obtain(blocklyWorkspace, 'madeup_movement_' + id);
      block.getInput('X').connection.connect(x.outputConnection);
      block.getInput('Y').connection.connect(y.outputConnection);
      block.getInput('Z').connection.connect(z.outputConnection);
      block.getInput('DEGREES').connection.connect(degrees.outputConnection);
    } else if (id == 'sin' || id == 'cos' || id == 'tan') {
      peeker.get(); // eat space
      var degrees = parse(peeker);
      block = Blockly.Block.obtain(blocklyWorkspace, 'madeup_math_sincostan');
      block.setFieldValue(id, 'F');
      block.getInput('DEGREES').connection.connect(degrees.outputConnection);
    } else if (id == 'asin' || id == 'acos' || id == 'atan') {
      peeker.get(); // eat space
      var ratio = parse(peeker);
      block = Blockly.Block.obtain(blocklyWorkspace, 'madeup_math_inverse_sincostan');
      block.setFieldValue(id, 'F');
      block.getInput('RATIO').connection.connect(ratio.outputConnection);
    } else if (id == 'center' || id == 'identity' || id == 'reframe' || id == 'push' || id == 'pop' || id == 'reverse') {
      block = Blockly.Block.obtain(blocklyWorkspace, 'madeup_movement_' + id);
    } else if (id == 'where') {
      block = Blockly.Block.obtain(blocklyWorkspace, 'madeup_io_where');
    } else if (id == 'log') {
      peeker.get(); // eat space
      var base = parse(peeker);
      peeker.get(); // eat space
      var x = parse(peeker);
      block = Blockly.Block.obtain(blocklyWorkspace, 'madeup_math_log');
      block.getInput('BASE').connection.connect(base.outputConnection);
      block.getInput('X').connection.connect(x.outputConnection);
    } else if (id == 'min' || id == 'max') {
      peeker.get(); // eat space
      var a = parse(peeker);
      peeker.get(); // eat space
      var b = parse(peeker);
      block = Blockly.Block.obtain(blocklyWorkspace, 'madeup_math_minmax');
      block.setFieldValue(id, 'F');
      block.getInput('A').connection.connect(a.outputConnection);
      block.getInput('B').connection.connect(b.outputConnection);
    } else {
      // is it a variable or a function?
      if (!Blockly.Procedures.isLegalName(id, blocklyWorkspace)) {
        block = Blockly.Block.obtain(blocklyWorkspace, 'procedures_callnoreturn');
        block.setFieldValue(id, 'NAME');
        var formals = getBlocklyProcedureFormals(id);
        block.setProcedureParameters(formals, formals);
        var i = 0;
        while (peeker.peek() == ' ') {
          peeker.get(); // eat space
          var actualBlock = parse(peeker);
          block.getInput('ARG' + i).connection.connect(actualBlock.outputConnection);
          ++i;
        }
      } else {
        block = Blockly.Block.obtain(blocklyWorkspace, 'variables_get');
        block.setFieldValue(id, 'VAR');
      }
    }
  }

  else if (token == 'repeat') {
    peeker.get(); // eat space
    var count = parse(peeker);
    peeker.get(); // eat space
    var body = parse(peeker);

    block = Blockly.Block.obtain(blocklyWorkspace, 'madeup_loop_repeat');
    block.getInput('COUNT').connection.connect(count.outputConnection);
    block.getInput('BLOCK').connection.connect(body.previousConnection);
  }

  else if (token == 'repeatwich') {
    peeker.get(); // eat space
    var count = parse(peeker);
    peeker.get(); // eat space
    var surrounder = parse(peeker);
    peeker.get(); // eat space
    var surroundee = parse(peeker);

    block = Blockly.Block.obtain(blocklyWorkspace, 'madeup_loop_repeatwich');
    block.getInput('COUNT').connection.connect(count.outputConnection);
    block.getInput('SURROUNDER').connection.connect(surrounder.previousConnection);
    block.getInput('SURROUNDEE').connection.connect(surroundee.previousConnection);
  }

  else if (token == 'while') {
    peeker.get(); // eat space
    var condition = parse(peeker);
    peeker.get(); // eat space
    var body = parse(peeker);
    block = Blockly.Block.obtain(blocklyWorkspace, 'madeup_loop_while');
    block.getInput('CONDITION').connection.connect(condition.outputConnection);
    block.getInput('BODY').connection.connect(body.previousConnection);
  }

  else if (token == 'if') {
    peeker.get(); // eat space
    var condition = parse(peeker);
    peeker.get(); // eat space
    var thenBlock = parse(peeker);
    peeker.get(); // eat space
    var elseBlock = parse(peeker);
    block = Blockly.Block.obtain(blocklyWorkspace, 'madeup_logic_if_else_statement');
    block.getInput('CONDITION').connection.connect(condition.outputConnection);
    block.getInput('THEN').connection.connect(thenBlock.previousConnection);
    block.getInput('ELSE').connection.connect(elseBlock.previousConnection);
  }

  else if (token == 'for-through') {
    peeker.get(); // eat space
    var id = peeker.getToken();
    peeker.get(); // eat space
    var stop = parse(peeker);
    peeker.get(); // eat space
    var body = parse(peeker);
    block = Blockly.Block.obtain(blocklyWorkspace, 'madeup_loop_for_through');
    block.setFieldValue(id, 'ITERATOR');
    block.getInput('STOP').connection.connect(stop.outputConnection);
    block.getInput('BODY').connection.connect(body.previousConnection);
  }

  else if (token == 'for-through-by') {
    peeker.get(); // eat space
    var id = peeker.getToken();
    peeker.get(); // eat space
    var stop = parse(peeker);
    peeker.get(); // eat space
    var by = parse(peeker);
    peeker.get(); // eat space
    var body = parse(peeker);
    block = Blockly.Block.obtain(blocklyWorkspace, 'madeup_loop_for_through_by');
    block.setFieldValue(id, 'ITERATOR');
    block.getInput('STOP').connection.connect(stop.outputConnection);
    block.getInput('BY').connection.connect(by.outputConnection);
    block.getInput('BODY').connection.connect(body.previousConnection);
  }

  else if (token == 'for-to') {
    peeker.get(); // eat space
    var id = peeker.getToken();
    peeker.get(); // eat space
    var stop = parse(peeker);
    peeker.get(); // eat space
    var body = parse(peeker);
    block = Blockly.Block.obtain(blocklyWorkspace, 'madeup_loop_for_to');
    block.setFieldValue(id, 'ITERATOR');
    block.getInput('STOP').connection.connect(stop.outputConnection);
    block.getInput('BODY').connection.connect(body.previousConnection);
  }

  else if (token == 'for-to-by') {
    peeker.get(); // eat space
    var id = peeker.getToken();
    peeker.get(); // eat space
    var stop = parse(peeker);
    peeker.get(); // eat space
    var by = parse(peeker);
    peeker.get(); // eat space
    var body = parse(peeker);
    block = Blockly.Block.obtain(blocklyWorkspace, 'madeup_loop_for_to_by');
    block.setFieldValue(id, 'ITERATOR');
    block.getInput('STOP').connection.connect(stop.outputConnection);
    block.getInput('BY').connection.connect(by.outputConnection);
    block.getInput('BODY').connection.connect(body.previousConnection);
  }

  else if (token == 'for-in') {
    peeker.get(); // eat space
    var id = peeker.getToken();
    peeker.get(); // eat space
    var start = parse(peeker);
    peeker.get(); // eat space
    var stop = parse(peeker);
    peeker.get(); // eat space
    var body = parse(peeker);
    block = Blockly.Block.obtain(blocklyWorkspace, 'madeup_loop_for_in');
    block.setFieldValue(id, 'ITERATOR');
    block.getInput('START').connection.connect(start.outputConnection);
    block.getInput('STOP').connection.connect(stop.outputConnection);
    block.getInput('BODY').connection.connect(body.previousConnection);
  }

  else if (token == 'for-in-by') {
    peeker.get(); // eat space
    var id = peeker.getToken();
    peeker.get(); // eat space
    var start = parse(peeker);
    peeker.get(); // eat space
    var stop = parse(peeker);
    peeker.get(); // eat space
    var by = parse(peeker);
    peeker.get(); // eat space
    var body = parse(peeker);
    block = Blockly.Block.obtain(blocklyWorkspace, 'madeup_loop_for_in_by');
    block.setFieldValue(id, 'ITERATOR');
    block.getInput('START').connection.connect(start.outputConnection);
    block.getInput('STOP').connection.connect(stop.outputConnection);
    block.getInput('BY').connection.connect(by.outputConnection);
    block.getInput('BODY').connection.connect(body.previousConnection);
  }

  else if (token == '+' || token == '-' || token == '*' || token == '/' || token == '//' || token == '^' || token == '%') {
    peeker.get(); // eat space
    var a = parse(peeker);
    peeker.get(); // eat space
    var b = parse(peeker);
    block = Blockly.Block.obtain(blocklyWorkspace, 'madeup_math_binary_arithmetic_operator');
    block.setFieldValue(token, 'OPERATOR');
    block.getInput('A').connection.connect(a.outputConnection);
    block.getInput('B').connection.connect(b.outputConnection);
  }

  else if (token == 'negate') {
    peeker.get(); // eat space
    var a = parse(peeker);
    block = Blockly.Block.obtain(blocklyWorkspace, 'madeup_math_unary_operator');
    block.getInput('A').connection.connect(a.outputConnection);
  }

  else if (token == 'define') {
    peeker.get(); // eat space
    var id = peeker.getToken();
    block = Blockly.Block.obtain(blocklyWorkspace, 'procedures_defnoreturn');
    block.setFieldValue(id, 'NAME');
    peeker.get(); // eat space
    var formals = [];
    while (peeker.peek() != '(') {
      var formal = peeker.getToken();
      formals.push(formal);
      peeker.get(); // eat space
    }
    block.arguments_ = formals;
    block.updateParams_();
    var body = parse(peeker);
    block.getInput('STACK').connection.connect(body.previousConnection);
  }

  else if (token == 'define-variable') {
    peeker.get(); // eat space
    var id = peeker.getToken();
    peeker.get(); // eat space
    var rhs = parse(peeker);
    block = Blockly.Block.obtain(blocklyWorkspace, 'variables_set');
    block.setFieldValue(id, 'VAR');
    block.getInput('VALUE').connection.connect(rhs.outputConnection);
  }

  else if (token == '>' || token == '>=' || token == '<' || token == '<=' || token == '==' || token == '!=') {
    peeker.get(); // eat space
    var a = parse(peeker);
    peeker.get(); // eat space
    var b = parse(peeker);
    block = Blockly.Block.obtain(blocklyWorkspace, 'madeup_math_relational_operator');
    block.setFieldValue(token, 'OPERATOR');
    block.getInput('A').connection.connect(a.outputConnection);
    block.getInput('B').connection.connect(b.outputConnection);
  }

  else if (token == 'or' || token == 'and') {
    peeker.get(); // eat space
    var a = parse(peeker);
    peeker.get(); // eat space
    var b = parse(peeker);
    block = Blockly.Block.obtain(blocklyWorkspace, 'madeup_logic_junction');
    block.setFieldValue(token, 'F');
    block.getInput('A').connection.connect(a.outputConnection);
    block.getInput('B').connection.connect(b.outputConnection);
  }

  else if (token == 'not') {
    peeker.get(); // eat space
    var a = parse(peeker);
    block = Blockly.Block.obtain(blocklyWorkspace, 'madeup_logic_not');
    block.getInput('A').connection.connect(a.outputConnection);
  }

  else if (token == 'abs') {
    peeker.get(); // eat space
    var x = parse(peeker);
    block = Blockly.Block.obtain(blocklyWorkspace, 'madeup_math_abs');
    block.getInput('X').connection.connect(x.outputConnection);
  }

  else if (token == 'unit') {
    block = Blockly.Block.obtain(blocklyWorkspace, 'madeup_nothing');
  }

  else if (token == 'INTEGER') {
    peeker.get(); // eat space
    var i = peeker.getInteger();
    block = Blockly.Block.obtain(blocklyWorkspace, 'madeup_math_integer');
    block.setFieldValue('' + i, 'INTEGER');
  }

  else if (token == 'BOOLEAN') {
    peeker.get(); // eat space
    var literal = peeker.getToken();
    block = Blockly.Block.obtain(blocklyWorkspace, 'madeup_logic_boolean');
    block.setFieldValue(literal, 'BOOLEAN');
  }

  else if (token == 'REAL') {
    peeker.get(); // eat space
    var i = peeker.getReal();
    block = Blockly.Block.obtain(blocklyWorkspace, 'madeup_math_real');
    block.setFieldValue('' + i, 'REAL');
  }

  else if (token == 'STRING') {
    peeker.get(); // eat space
    var s = '';
    while (peeker.peek() != ')') {
      s += peeker.get();
    }
    block = Blockly.Block.obtain(blocklyWorkspace, 'madeup_string');
    block.setFieldValue(s, 'STRING');
  }

  // assert ')'
  if (peeker.peek() != ')') {
    throw 'expecting )';
  } else {
    peeker.get();
  }
  
  // Block might be null if empty body loop or conditional body.
  if (block) {
    block.initSvg();
    block.render();
  }

  return block;
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
    $('#download').css('color', '#FFFFFF');
  } else {
    $('#download').prop('disabled', true);
    $('#download').css('color', '#666666');
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
var blocklyWorkspace = null;
var fontSize = 14;
var gridSpacing = 1.0;
var gridExtent = 10.0;
var isFlatShaded = true;
var isEditorText = true;
var tree = null;
var isSourceDirty = false;

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
      // $('#console').height(200);
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
          run(getSource(), GeometryMode.SURFACE);
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

  $('input[type=radio][name=editorMode]').change(function() {
    hideMenus(); // setEditor may pop open a dialog, which doesn't look good with a menu still open
    var editorMode = $(this).val();
    setEditor(editorMode != "Blocks");
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
    run(getSource(), GeometryMode.SURFACE);
  });

  function toggleMenu(id) {
    var buttonID = '#toggle' + id.charAt(1).toUpperCase() + id.substring(2);

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

  enableDownload(false);
  $('#download').click(function() {
    $('#tag').val(mupName);
    $('#source').val(getSource());
    $('#downloader').submit();
    textEditor.focus();
  });

  $('#docs').click(function() {
    window.open('docs/introduction.html', '_blank');
  });

  $('#solidify').click(function() {
    log('Running...'); 
    saveInCookies();
    run(getSource(), GeometryMode.SURFACE);
    textEditor.focus();
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

    save();
    updateTitle();
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
  preview = setTimeout(function() {
    run(getSource(), GeometryMode.PATH);
  }, nSecondsTillPreview * 1000);
}

var preview = undefined;
function schedulePreview() {
  textEditor.getSession().on('change', onEditorChange);
}

function setEditor(isText) {
  // Bail if we're already in the requested mode.
  if (isEditorText == isText) return;

  if (blocklyWorkspace) {
    blocklyWorkspace.removeChangeListener(onBlocksChanged);
  }

  isEditorText = isText;
  isEditorTextChanged = true;

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
      blocklyWorkspace = Blockly.inject('blocksCanvas', {toolbox: document.getElementById('toolbox')});
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
function run(source, mode) {
  $.ajax({
    type: 'POST',
    url: 'interpret.php',
    data: JSON.stringify(
      {
        source: source,
        extension: 'json',
        geometry_mode: mode,
        shading_mode: isFlatShaded ? 'FLAT' : 'SMOOTH'
      }
    ),
    contentType: 'application/json; charset=utf-8',
    dataType: 'json',
    success: function(data) {
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
          var model = loader.parse(JSON.parse(data['model']));
          var material = new THREE.MeshLambertMaterial({color: parseInt(modelColor, 16), wireframe: showWireframe, wireframeLinewidth: 5});
          meshes[0] = new THREE.Mesh(model.geometry, material);
          model.geometry.computeFaceNormals();
          model.geometry.computeVertexNormals();
          allGeometry = model.geometry;
          enableDownload(true);
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
    $('.togglePopup').css('background-color', '#000000');
  } else {
    $('.popup').not(exceptID).hide();
    $('.togglePopup').not('#toggle' + exceptID.charAt(1).toUpperCase() + exceptID.substring(2)).css('background-color', '#000000');
  }

  if (swatch) {
    swatch.ColorPickerHide();
  }
}
