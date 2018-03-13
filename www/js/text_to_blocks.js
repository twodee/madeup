function connectExpression(block, connectionName, expressionBlock) {
  setStatementExpression(expressionBlock, true);
  block.getInput(connectionName).connection.connect(expressionBlock.outputConnection);
}

function connectStatement(block, connectionName, expressionBlock) {
  setStatementExpression(expressionBlock, false);
  block.getInput(connectionName).connection.connect(expressionBlock.previousConnection);
}

function Peeker(src) {
  this.i = 0;
  this.src = src;
  console.log("src:", src);
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

function parse(peeker, workspace) {
  var block = null;

  // assert '('
  if (peeker.peek() != '(') {
    console.log('expecting (, but saw ' + peeker.peek());
    return null;
  } else {
    peeker.get();
  }

  // figure out contents
  var token = peeker.getToken();

  if (token == 'block') {
    if (peeker.peek() == ' ') {
      peeker.get(); // eat space
      block = parse(peeker, workspace);
      var prevStatement = block;
      while (peeker.peek() == ' ') {
        peeker.get(); // eat space
        var statement = parse(peeker, workspace);

        setStatementExpression(statement, false);
        prevStatement.nextConnection.connect(statement.previousConnection);
        prevStatement = statement;
      }
    }
  }

  else if (token == 'call' || token == 'call-with-names') {
    peeker.get(); // eat space
    id = peeker.getToken();
    if (id == 'print') {
      peeker.get(); // eat space
      var msg = parse(peeker, workspace);
      block = workspace.newBlock('madeup_print');
      connectExpression(block, 'MESSAGE', msg);
    } else if (id == 'echo') {
      peeker.get(); // eat space
      var mesh = parse(peeker, workspace);
      block = workspace.newBlock('madeup_echo');
      connectExpression(block, 'OBJECT', mesh);
    } else if (id == 'transform') {
      peeker.get(); // eat space
      var mesh = parse(peeker, workspace);
      block = workspace.newBlock('madeup_transform');
      connectExpression(block, 'OBJECT', mesh);
    } else if (id == 'path') {
      block = workspace.newBlock('madeup_path');
    } else if (id == 'debug') {
      peeker.get(); // eat space
      var msg = parse(peeker, workspace);
      block = workspace.newBlock('madeup_debug');
      connectExpression(block, 'MESSAGE', msg);
    } else if (id == 'sign') {
      peeker.get(); // eat space
      var x = parse(peeker, workspace);
      block = workspace.newBlock('madeup_sign');
      connectExpression(block, 'X', x);
    } else if (id == 'moveto') {
      peeker.get(); // eat space
      var x = parse(peeker, workspace);
      peeker.get(); // eat space
      var y = parse(peeker, workspace);
      if (peeker.peek() == ' ') {
        peeker.get(); // eat space
        var z = parse(peeker, workspace);
        block = workspace.newBlock('madeup_moveto');
        connectExpression(block, 'X', x);
        connectExpression(block, 'Y', y);
        connectExpression(block, 'Z', z);
      } else {
        block = workspace.newBlock('madeup_moveto_xy');
        connectExpression(block, 'X', x);
        connectExpression(block, 'Y', y);
      }
    } else if (id == 'polarto') {
      peeker.get(); // eat space
      var radius = parse(peeker, workspace);
      peeker.get(); // eat space
      var angle = parse(peeker, workspace);
      block = workspace.newBlock('madeup_polarto');
      connectExpression(block, 'RADIUS', radius);
      connectExpression(block, 'ANGLE', angle);
    } else if (id == 'move') {
      peeker.get(); // eat space
      var x = parse(peeker, workspace);
      block = workspace.newBlock('madeup_move');
      connectExpression(block, 'DISTANCE', x);
    } else if (id == 'random') {
      peeker.get(); // eat space
      var min = parse(peeker, workspace);
      peeker.get(); // eat space
      var max = parse(peeker, workspace);
      block = workspace.newBlock('madeup_random');
      connectExpression(block, 'MIN', min);
      connectExpression(block, 'MAX', max);
    } else if (id == 'random01') {
      block = workspace.newBlock('madeup_random01');
    } else if (id == 'atan2') {
      peeker.get(); // eat space
      var opposite = parse(peeker, workspace);
      peeker.get(); // eat space
      var adjacent = parse(peeker, workspace);
      block = workspace.newBlock('madeup_atan2');
      connectExpression(block, 'OPPOSITE', opposite);
      connectExpression(block, 'ADJACENT', adjacent);
    } else if (id == 'yaw' || id == 'roll' || id == 'pitch') {
      peeker.get(); // eat space
      var degrees = parse(peeker, workspace);
      block = workspace.newBlock('madeup_turn');
      block.setFieldValue(id, 'FUNCTION');
      connectExpression(block, 'DEGREES', degrees);
    } else if (id == 'coalesce') {
      peeker.get(); // eat space
      var threshold = parse(peeker, workspace);
      block = workspace.newBlock('madeup_coalesce');
      connectExpression(block, 'THRESHOLD', threshold);
    } else if (id == 'mirror') {
      // peeker.get(); // eat space
      // var path = parse(peeker, workspace);
      peeker.get(); // eat space
      var axis = parse(peeker, workspace);
      // peeker.get(); // eat space
      // var point = parse(peeker, workspace);
      block = workspace.newBlock('madeup_mirror');
      // connectExpression(block, 'PATH', path);
      connectExpression(block, 'AXIS', axis);
      // connectExpression(block, 'POINT', point);
    } else if (id == 'center') {
      peeker.get(); // eat space
      var object = parse(peeker, workspace);
      block = workspace.newBlock('madeup_center');
      connectExpression(block, 'OBJECT', object);
    } else if (id == 'dilate') {
      peeker.get(); // eat space
      var path = parse(peeker, workspace);
      peeker.get(); // eat space
      var distance = parse(peeker, workspace);
      block = workspace.newBlock('madeup_dilate');
      connectExpression(block, 'PATH', path);
      connectExpression(block, 'DISTANCE', distance);
    } else if (id == 'translate' || id == 'scale') {
      peeker.get(); // eat space
      var x = parse(peeker, workspace);
      peeker.get(); // eat space
      var y = parse(peeker, workspace);
      peeker.get(); // eat space
      var z = parse(peeker, workspace);
      block = workspace.newBlock('madeup_' + id);
      connectExpression(block, 'X', x);
      connectExpression(block, 'Y', y);
      connectExpression(block, 'Z', z);
    } else if (id == 'spheres' || id == 'forget' || id == 'boxes' || id == 'polygon') {
      block = workspace.newBlock('madeup_' + id);
    } else if (id == 'add' || id == 'subtract') {
      block = workspace.newBlock('madeup_mode_' + id);
    } else if (id == 'revolve') {
      peeker.get(); // eat space
      var x = parse(peeker, workspace);
      peeker.get(); // eat space
      var y = parse(peeker, workspace);
      peeker.get(); // eat space
      var z = parse(peeker, workspace);
      peeker.get(); // eat space
      var degrees = parse(peeker, workspace);
      block = workspace.newBlock('madeup_revolve');
      connectExpression(block, 'X', x);
      connectExpression(block, 'Y', y);
      connectExpression(block, 'Z', z);
      connectExpression(block, 'DEGREES', degrees);
    } else if (id == 'extrude') {
      peeker.get(); // eat space
      var x = parse(peeker, workspace);
      peeker.get(); // eat space
      var y = parse(peeker, workspace);
      peeker.get(); // eat space
      var z = parse(peeker, workspace);
      peeker.get(); // eat space
      var length = parse(peeker, workspace);
      block = workspace.newBlock('madeup_extrude');
      connectExpression(block, 'X', x);
      connectExpression(block, 'Y', y);
      connectExpression(block, 'Z', z);
      connectExpression(block, 'LENGTH', length);
    } else if (id == 'surface') {
      peeker.get(); // eat space
      var columns = parse(peeker, workspace);
      peeker.get(); // eat space
      var rows = parse(peeker, workspace);
      block = workspace.newBlock('madeup_surface');
      connectExpression(block, 'COLUMNS', columns);
      connectExpression(block, 'ROWS', rows);
    } else if (id == 'dowel' || id == 'tube') {
      var maxBend;
      if (peeker.peek() == ' ') {
        peeker.get(); // eat space
        peeker.getToken(); // TODO consume maxBend
        peeker.get(); // eat space
        maxBend = parse(peeker, workspace);
      } else {
        maxBend = workspace.newBlock('madeup_integer');
        maxBend.setFieldValue(360, 'INTEGER');
        maxBend.initSvg();
        maxBend.render();
      }
      block = workspace.newBlock('madeup_' + id);
      connectExpression(block, 'MAXBEND', maxBend);
    } else if (id == 'loft') {
      var paths;
      peeker.get(); // eat space
      paths = parse(peeker, workspace);
      block = workspace.newBlock('madeup_loft');
      connectExpression(block, 'PATHS', paths);
    } else if (id == 'rotate') {
      peeker.get(); // eat space
      var x = parse(peeker, workspace);
      peeker.get(); // eat space
      var y = parse(peeker, workspace);
      peeker.get(); // eat space
      var z = parse(peeker, workspace);
      peeker.get(); // eat space
      var degrees = parse(peeker, workspace);
      block = workspace.newBlock('madeup_' + id);
      connectExpression(block, 'X', x);
      connectExpression(block, 'Y', y);
      connectExpression(block, 'Z', z);
      connectExpression(block, 'DEGREES', degrees);
    } else if (id == 'sin' || id == 'cos' || id == 'tan') {
      peeker.get(); // eat space
      var degrees = parse(peeker, workspace);
      block = workspace.newBlock('madeup_sincostan');
      block.setFieldValue(id, 'FUNCTION');
      connectExpression(block, 'DEGREES', degrees);
    } else if (id == 'asin' || id == 'acos' || id == 'atan') {
      peeker.get(); // eat space
      var ratio = parse(peeker, workspace);
      block = workspace.newBlock('madeup_inverse_sincostan');
      block.setFieldValue(id, 'FUNCTION');
      connectExpression(block, 'RATIO', ratio);
    } else if (id == 'identity' || id == 'reframe' || id == 'push' || id == 'pop' || id == 'reverse' || id == 'home') {
      block = workspace.newBlock('madeup_' + id);
    } else if (id == 'where') {
      block = workspace.newBlock('madeup_where');
    } else if (id == 'view') {
      block = workspace.newBlock('madeup_view');
    } else if (id == 'look') {
      peeker.get(); // eat space
      var view = parse(peeker, workspace);
      block = workspace.newBlock('madeup_look');
      connectExpression(block, 'VIEW', view);
    } else if (id == 'log') {
      peeker.get(); // eat space
      var base = parse(peeker, workspace);
      peeker.get(); // eat space
      var x = parse(peeker, workspace);
      block = workspace.newBlock('madeup_log');
      connectExpression(block, 'BASE', base);
      connectExpression(block, 'X', x);
    } else if (id == 'min' || id == 'max') {
      peeker.get(); // eat space
      var a = parse(peeker, workspace);
      peeker.get(); // eat space
      var b = parse(peeker, workspace);
      block = workspace.newBlock('madeup_minmax');
      block.setFieldValue(id, 'FUNCTION');
      connectExpression(block, 'A', a);
      connectExpression(block, 'B', b);
    } else if (id == 'normalize' || id == 'magnitude') {
      peeker.get(); // eat space
      var v = parse(peeker, workspace);
      block = workspace.newBlock('madeup_' + id);
      connectExpression(block, 'VECTOR', v);
    } else if (id == 'cross' || id == 'dot') {
      peeker.get(); // eat space
      var a = parse(peeker, workspace);
      peeker.get(); // eat space
      var b = parse(peeker, workspace);
      block = workspace.newBlock('madeup_' + id);
      connectExpression(block, 'A', a);
      connectExpression(block, 'B', b);
    } else {
      // is it a variable or a function?
      if (!Blockly.Procedures.isLegalName_(id, workspace)) {
        block = workspace.newBlock('procedures_callnoreturn');
        block.setFieldValue(id, 'NAME');
        var formals = getBlocklyProcedureFormals(workspace, id);

        // I used to use setProcedureParameters, but Blockly
        // moved that out of the API. Now we need a mutation.
        // Baah.
        var container = document.createElement('mutation'); 
        formals.forEach(function(formal) {
          var arg = document.createElement('arg'); 
          arg.setAttribute('name', formal);
          arg.setAttribute('paramId', formal);
          container.appendChild(arg);
        });
        block.domToMutation(container);

        // block.setProcedureParameters_(formals, formals);
 
        var i = 0;
        while (peeker.peek() == ' ') {
          peeker.get(); // eat space
          var actualBlock = parse(peeker, workspace);
          connectExpression(block, 'ARG' + i, actualBlock);
          ++i;
        }
      } else {
        block = workspace.newBlock('variables_get');
        block.setFieldValue(id, 'VAR');
      }
    }
  }
  
  else if (token == 'return') {
    peeker.get(); // eat space
    var value = parse(peeker, workspace);
    block = workspace.newBlock('madeup_return');
    console.log("value:", value);
    console.log("block:", block);
    connectExpression(block, 'VALUE', value);
  }

  else if (token == 'repeat') {
    peeker.get(); // eat space
    var count = parse(peeker, workspace);
    peeker.get(); // eat space
    var body = parse(peeker, workspace);

    block = workspace.newBlock('madeup_repeat');
    connectExpression(block, 'COUNT', count);
    connectStatement(block, 'BODY', body);
  }

  else if (token == 'repeatwich') {
    peeker.get(); // eat space
    var count = parse(peeker, workspace);
    peeker.get(); // eat space
    var surrounder = parse(peeker, workspace);
    peeker.get(); // eat space
    var surroundee = parse(peeker, workspace);

    block = workspace.newBlock('madeup_repeat_around');
    connectExpression(block, 'COUNT', count);
    connectStatement(block, 'SURROUNDER', surrounder);
    connectStatement(block, 'SURROUNDEE', surroundee);
  }

  else if (token == 'while') {
    peeker.get(); // eat space
    var condition = parse(peeker, workspace);
    peeker.get(); // eat space
    var body = parse(peeker, workspace);
    block = workspace.newBlock('madeup_while');
    connectExpression(block, 'CONDITION', condition);
    connectStatement(block, 'BODY', body);
  }

  else if (token == 'if') {
    peeker.get(); // eat space
    var condition = parse(peeker, workspace);
    peeker.get(); // eat space
    var thenBlock = parse(peeker, workspace);
    peeker.get(); // eat space
    var elseBlock = parse(peeker, workspace);
    if (elseBlock) {
      block = workspace.newBlock('madeup_if_else_statement');
      connectExpression(block, 'CONDITION', condition);
      connectStatement(block, 'THEN', thenBlock);
      connectStatement(block, 'ELSE', elseBlock);
    } else {
      block = workspace.newBlock('madeup_if_statement');
      connectExpression(block, 'CONDITION', condition);
      connectStatement(block, 'THEN', thenBlock);
    }
  }

  else if (token == 'if-ternary') {
    peeker.get(); // eat space
    var condition = parse(peeker, workspace);
    peeker.get(); // eat space
    var thenBlock = parse(peeker, workspace);
    peeker.get(); // eat space
    var elseBlock = parse(peeker, workspace);

    block = workspace.newBlock('madeup_if_expr');
    connectExpression(block, 'CONDITION', condition);
    connectExpression(block, 'THEN', thenBlock);
    connectExpression(block, 'ELSE', elseBlock);
  }

  else if (token == 'for-through') {
    peeker.get(); // eat space
    var iterator = parse(peeker, workspace);
    peeker.get(); // eat space
    var stop = parse(peeker, workspace);
    peeker.get(); // eat space
    var body = parse(peeker, workspace);
    block = workspace.newBlock('madeup_for_upper');
    block.setFieldValue('through', 'CLUSIVITY');
    connectExpression(block, 'ITERATOR', iterator);
    connectExpression(block, 'STOP', stop);
    connectStatement(block, 'BODY', body);
  }

  else if (token == 'for-through-by') {
    peeker.get(); // eat space
    var iterator = parse(peeker, workspace);
    peeker.get(); // eat space
    var stop = parse(peeker, workspace);
    peeker.get(); // eat space
    var by = parse(peeker, workspace);
    peeker.get(); // eat space
    var body = parse(peeker, workspace);
    block = workspace.newBlock('madeup_for_upper_by');
    block.setFieldValue('through', 'CLUSIVITY');
    connectExpression(block, 'ITERATOR', iterator);
    connectExpression(block, 'STOP', stop);
    connectExpression(block, 'BY', by);
    connectStatement(block, 'BODY', body);
  }

  else if (token == 'for-to') {
    peeker.get(); // eat space
    var iterator = parse(peeker, workspace);
    peeker.get(); // eat space
    var stop = parse(peeker, workspace);
    peeker.get(); // eat space
    var body = parse(peeker, workspace);
    block = workspace.newBlock('madeup_for_upper');
    block.setFieldValue('to', 'CLUSIVITY');
    connectExpression(block, 'ITERATOR', iterator);
    connectExpression(block, 'STOP', stop);
    connectStatement(block, 'BODY', body);
  }

  else if (token == 'for-to-by') {
    peeker.get(); // eat space
    var iterator = parse(peeker, workspace);
    peeker.get(); // eat space
    var stop = parse(peeker, workspace);
    peeker.get(); // eat space
    var by = parse(peeker, workspace);
    peeker.get(); // eat space
    var body = parse(peeker, workspace);
    block = workspace.newBlock('madeup_for_upper_by');
    block.setFieldValue('to', 'CLUSIVITY');
    connectExpression(block, 'ITERATOR', iterator);
    connectExpression(block, 'STOP', stop);
    connectExpression(block, 'BY', by);
    connectStatement(block, 'BODY', body);
  }

  else if (token == 'for-in') {
    peeker.get(); // eat space
    var iterator = parse(peeker, workspace);
    peeker.get(); // eat space
    var start = parse(peeker, workspace);
    peeker.get(); // eat space
    var stop = parse(peeker, workspace);
    peeker.get(); // eat space
    var body = parse(peeker, workspace);
    block = workspace.newBlock('madeup_for_in');
    connectExpression(block, 'ITERATOR', iterator);
    connectExpression(block, 'START', start);
    connectExpression(block, 'STOP', stop);
    connectStatement(block, 'BODY', body);
  }

  else if (token == 'for-in-by') {
    peeker.get(); // eat space
    var iterator = parse(peeker, workspace);
    peeker.get(); // eat space
    var start = parse(peeker, workspace);
    peeker.get(); // eat space
    var stop = parse(peeker, workspace);
    peeker.get(); // eat space
    var by = parse(peeker, workspace);
    peeker.get(); // eat space
    var body = parse(peeker, workspace);
    block = workspace.newBlock('madeup_for_in_by');
    connectExpression(block, 'ITERATOR', iterator);
    connectExpression(block, 'START', start);
    connectExpression(block, 'STOP', stop);
    connectExpression(block, 'BY', by);
    connectStatement(block, 'BODY', body);
  }

  else if (token == '-?') {
    peeker.get(); // eat space
    var id = peeker.getToken();
    peeker.get(); // eat space
    var b = parse(peeker, workspace);

    // if id takes parameters
    // else
    //   make subtraction
    //

    if (!Blockly.Procedures.isLegalName_(id, workspace)) {
      block = workspace.newBlock('procedures_callnoreturn');
      block.setFieldValue(id, 'NAME');
      var formals = getBlocklyProcedureFormals(workspace, id);
      if (formals.length == 0) { // foo() - b
        var a = block;
        a.initSvg();
        a.render();

        block = workspace.newBlock('madeup_binary_arithmetic_operator');
        block.setFieldValue('-', 'OPERATOR');
        connectExpression(block, 'A', a);
        connectExpression(block, 'B', b);
      } else { // foo(-b)
        var container = document.createElement('mutation'); 
        formals.forEach(function(formal) {
          var arg = document.createElement('arg'); 
          arg.setAttribute('name', formal);
          arg.setAttribute('paramId', formal);
          container.appendChild(arg);
        });
        block.domToMutation(container);
        connectExpression(block, 'ARG0', b);
      }
    } else { // foo - a
      var a = workspace.newBlock('variables_get');
      a.setFieldValue(id, 'VAR');
      a.initSvg();
      a.render();

      block = workspace.newBlock('madeup_binary_arithmetic_operator');
      block.setFieldValue('-', 'OPERATOR');
      connectExpression(block, 'A', a);
      connectExpression(block, 'B', b);
    }
  }

  else if (token == '+' || token == '-' || token == '*' || token == '/' || token == '//' || token == '^' || token == '%') {
    peeker.get(); // eat space
    var a = parse(peeker, workspace);
    peeker.get(); // eat space
    var b = parse(peeker, workspace);
    block = workspace.newBlock('madeup_binary_arithmetic_operator');
    block.setFieldValue(token, 'OPERATOR');
    connectExpression(block, 'A', a);
    connectExpression(block, 'B', b);
  }

  else if (token == 'negate') {
    peeker.get(); // eat space
    var a = parse(peeker, workspace);
    block = workspace.newBlock('madeup_unary_operator');
    connectExpression(block, 'A', a);
  }

  else if (token == 'define') {
    peeker.get(); // eat space
    var id = peeker.getToken();
    block = workspace.newBlock('procedures_defnoreturn');
    block.setFieldValue(id, 'NAME');
    peeker.get(); // eat space
    var formals = [];
    peeker.get(); // eat (
    while (peeker.peek() == '(') {
      peeker.get(); // eat (
      var formal = peeker.getToken();
      formals.push(formal);
      peeker.get(); // eat )
      if (peeker.peek() == ' ') {
        peeker.get(); // eat space
      }
    }
    peeker.get(); // eat )
    peeker.get(); // eat space
    block.arguments_ = formals;
    block.updateParams_();
    var body = parse(peeker, workspace);
    connectStatement(block, 'STACK', body);
  }

  else if (token == 'define-variable') {
    peeker.get(); // eat space
    var id = peeker.getToken();
    peeker.get(); // eat space
    var rhs = parse(peeker, workspace);

    block = workspace.newBlock('variables_set');
    block.setFieldValue(id, 'VAR');
    connectExpression(block, 'VALUE', rhs);
  }

  else if (token == 'define-element') {
    peeker.get(); // eat space
    peeker.get(); // eat (
    peeker.getToken(); // eat SUBSCRIPT
    peeker.get(); // eat space

    var collection = parse(peeker, workspace);
    peeker.get(); // eat space
    var i = parse(peeker, workspace);
    peeker.get(); // eat )
    peeker.get(); // eat space
    var rhs = parse(peeker, workspace);

    block = workspace.newBlock('madeup_subscript_set');
    connectExpression(block, 'I', i);
    connectExpression(block, 'COLLECTION', collection);
    connectExpression(block, 'RHS', rhs);
  }

  else if (token == '>' || token == '>=' || token == '<' || token == '<=' || token == '==' || token == '!=') {
    peeker.get(); // eat space
    var a = parse(peeker, workspace);
    peeker.get(); // eat space
    var b = parse(peeker, workspace);
    block = workspace.newBlock('madeup_relational_operator');
    block.setFieldValue(token, 'OPERATOR');
    connectExpression(block, 'A', a);
    connectExpression(block, 'B', b);
  }

  else if (token == 'or' || token == 'and') {
    peeker.get(); // eat space
    var a = parse(peeker, workspace);
    peeker.get(); // eat space
    var b = parse(peeker, workspace);
    block = workspace.newBlock('madeup_binary_logic_operator');
    block.setFieldValue(token, 'FUNCTION');
    connectExpression(block, 'A', a);
    connectExpression(block, 'B', b);
  }

  else if (token == 'not') {
    peeker.get(); // eat space
    var a = parse(peeker, workspace);
    block = workspace.newBlock('madeup_not');
    connectExpression(block, 'A', a);
  }

  else if (token == 'abs') {
    peeker.get(); // eat space
    var x = parse(peeker, workspace);
    block = workspace.newBlock('madeup_abs');
    connectExpression(block, 'X', x);
  }

  else if (token == 'unit') {
    block = workspace.newBlock('madeup_nothing');
  }

  else if (token == 'INTEGER') {
    peeker.get(); // eat space
    var i = peeker.getInteger();
    block = workspace.newBlock('madeup_integer');
    block.setFieldValue('' + i, 'INTEGER');
  }

  else if (token == 'BOOLEAN') {
    peeker.get(); // eat space
    var literal = peeker.getToken();
    block = workspace.newBlock('madeup_boolean');
    block.setFieldValue(literal, 'BOOLEAN');
  }

  else if (token == 'REAL') {
    peeker.get(); // eat space
    var i = peeker.getReal();
    block = workspace.newBlock('madeup_real');
    block.setFieldValue('' + i, 'REAL');
  }

  else if (token == 'STRING') {
    peeker.get(); // eat space
    var s = '';
    while (peeker.peek() != ')') {
      s += peeker.get();
    }
    block = workspace.newBlock('madeup_string');
    block.setFieldValue(s, 'STRING');
  }

  else if (token == 'SUBSCRIPT') {
    peeker.get(); // eat space
    var collection = parse(peeker, workspace);
    peeker.get(); // eat space
    var i = parse(peeker, workspace);
    block = workspace.newBlock('madeup_subscript');
    connectExpression(block, 'I', i);
    connectExpression(block, 'COLLECTION', collection);
  }

  else if (token == 'SUBRANGE') {
    peeker.get(); // eat space
    var collection = parse(peeker, workspace);
    peeker.get(); // eat space
    var i = parse(peeker, workspace);
    peeker.get(); // eat space
    var j = parse(peeker, workspace);
    block = workspace.newBlock('madeup_subrange');
    connectExpression(block, 'I', i);
    connectExpression(block, 'J', j);
    connectExpression(block, 'COLLECTION', collection);
  }

  else if (token == 'ARRAYLITERAL') {
    var elements = [];
    while (peeker.peek() != ')') {
      peeker.get(); // eat space
      var element = parse(peeker, workspace);
      elements.push(element);
    }

    block = workspace.newBlock('madeup_array_literal');
    block.itemCount_ = elements.length;
    block.updateShape_();

    for (var i = 0; i < elements.length; ++i) {
      connectExpression(block, 'ELEMENT' + i, elements[i]);
    }
  }

  else if (token == 'MAKEARRAY') {
    peeker.get(); // eat space
    var count = parse(peeker, workspace);
    peeker.get(); // eat space
    var element = parse(peeker, workspace);

    block = workspace.newBlock(element.type == 'madeup_array_of' ? 'madeup_array_by' : 'madeup_array_of');
    connectExpression(block, 'N', count);
    connectExpression(block, 'ITEM', element);
  }

  // assert ')'
  if (peeker.peek() != ')') {
    throw 'expecting ) but saw ' + peeker.peek();
  } else {
    peeker.get();
  }
  
  // Block might be null if empty loop or conditional body.
  if (block) {
    block.initSvg();
    block.render();
  }

  return block;
}
