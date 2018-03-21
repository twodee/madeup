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
      connectExpression(block, 'message', msg);
    } else if (id == 'echo') {
      peeker.get(); // eat space
      var mesh = parse(peeker, workspace);
      block = workspace.newBlock('madeup_echo');
      connectExpression(block, 'object', mesh);
    } else if (id == 'transform') {
      peeker.get(); // eat space
      var mesh = parse(peeker, workspace);
      block = workspace.newBlock('madeup_transform');
      connectExpression(block, 'object', mesh);
    } else if (id == 'path') {
      block = workspace.newBlock('madeup_path');
    } else if (id == 'debug') {
      peeker.get(); // eat space
      var msg = parse(peeker, workspace);
      block = workspace.newBlock('madeup_debug');
      connectExpression(block, 'message', msg);
    } else if (id == 'sign') {
      peeker.get(); // eat space
      var x = parse(peeker, workspace);
      block = workspace.newBlock('madeup_sign');
      connectExpression(block, 'x', x);
    } else if (id == 'moveto') {
      peeker.get(); // eat space
      var x = parse(peeker, workspace);
      peeker.get(); // eat space
      var y = parse(peeker, workspace);
      if (peeker.peek() == ' ') {
        peeker.get(); // eat space
        var z = parse(peeker, workspace);
        block = workspace.newBlock('madeup_moveto');
        connectExpression(block, 'x', x);
        connectExpression(block, 'y', y);
        connectExpression(block, 'z', z);
      } else {
        block = workspace.newBlock('madeup_moveto_xy');
        connectExpression(block, 'x', x);
        connectExpression(block, 'y', y);
      }
    } else if (id == 'polarto') {
      peeker.get(); // eat space
      var radius = parse(peeker, workspace);
      peeker.get(); // eat space
      var angle = parse(peeker, workspace);
      block = workspace.newBlock('madeup_polarto');
      connectExpression(block, 'radius', radius);
      connectExpression(block, 'angle', angle);
    } else if (id == 'move') {
      peeker.get(); // eat space
      var x = parse(peeker, workspace);
      block = workspace.newBlock('madeup_move');
      connectExpression(block, 'distance', x);
    } else if (id == 'random') {
      peeker.get(); // eat space
      var min = parse(peeker, workspace);
      peeker.get(); // eat space
      var max = parse(peeker, workspace);
      block = workspace.newBlock('madeup_random');
      connectExpression(block, 'min', min);
      connectExpression(block, 'max', max);
    } else if (id == 'random01') {
      block = workspace.newBlock('madeup_random01');
    } else if (id == 'atan2') {
      peeker.get(); // eat space
      var opposite = parse(peeker, workspace);
      peeker.get(); // eat space
      var adjacent = parse(peeker, workspace);
      block = workspace.newBlock('madeup_atan2');
      connectExpression(block, 'opposite', opposite);
      connectExpression(block, 'adjacent', adjacent);
    } else if (id == 'yaw' || id == 'roll' || id == 'pitch') {
      peeker.get(); // eat space
      var degrees = parse(peeker, workspace);
      block = workspace.newBlock('madeup_turn');
      block.setFieldValue(id, 'function');
      connectExpression(block, 'degrees', degrees);
    } else if (id == 'coalesce') {
      peeker.get(); // eat space
      var threshold = parse(peeker, workspace);
      block = workspace.newBlock('madeup_coalesce');
      connectExpression(block, 'threshold', threshold);
    } else if (id == 'mirror') {
      peeker.get(); // eat space
      var axis = parse(peeker, workspace);
      peeker.get(); // eat space
      var path = parse(peeker, workspace);
      peeker.get(); // eat space
      var point = parse(peeker, workspace);
      block = workspace.newBlock('madeup_mirror');
      connectExpression(block, 'axis', axis);
      connectExpression(block, 'path', path);
      connectExpression(block, 'point', point);
    } else if (id == 'center') {
      peeker.get(); // eat space
      var object = parse(peeker, workspace);
      block = workspace.newBlock('madeup_center');
      connectExpression(block, 'object', object);
    } else if (id == 'dilate') {
      peeker.get(); // eat space
      var path = parse(peeker, workspace);
      peeker.get(); // eat space
      var distance = parse(peeker, workspace);
      block = workspace.newBlock('madeup_dilate');
      connectExpression(block, 'path', path);
      connectExpression(block, 'distance', distance);
    } else if (id == 'translate' || id == 'scale') {
      peeker.get(); // eat space
      var x = parse(peeker, workspace);
      peeker.get(); // eat space
      var y = parse(peeker, workspace);
      peeker.get(); // eat space
      var z = parse(peeker, workspace);
      block = workspace.newBlock('madeup_' + id);
      connectExpression(block, 'x', x);
      connectExpression(block, 'y', y);
      connectExpression(block, 'z', z);
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
      connectExpression(block, 'x', x);
      connectExpression(block, 'y', y);
      connectExpression(block, 'z', z);
      connectExpression(block, 'degrees', degrees);
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
      connectExpression(block, 'x', x);
      connectExpression(block, 'y', y);
      connectExpression(block, 'z', z);
      connectExpression(block, 'length', length);
    } else if (id == 'surface') {
      peeker.get(); // eat space
      var columns = parse(peeker, workspace);
      peeker.get(); // eat space
      var rows = parse(peeker, workspace);
      block = workspace.newBlock('madeup_surface');
      connectExpression(block, 'columns', columns);
      connectExpression(block, 'rows', rows);
    } else if (id == 'dowel' || id == 'tube') {
      var maxBend;
      if (peeker.peek() == ' ') {
        peeker.get(); // eat space
        peeker.getToken(); // TODO consume maxBend
        peeker.get(); // eat space
        maxBend = parse(peeker, workspace);
      } else {
        maxBend = workspace.newBlock('madeup_integer');
        maxBend.setFieldValue(360, 'integer');
        maxBend.initSvg();
        maxBend.render();
      }
      block = workspace.newBlock('madeup_' + id);
      connectExpression(block, 'maxbend', maxBend);
    } else if (id == 'loft') {
      var paths;
      peeker.get(); // eat space
      paths = parse(peeker, workspace);
      block = workspace.newBlock('madeup_loft');
      connectExpression(block, 'paths', paths);
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
      connectExpression(block, 'x', x);
      connectExpression(block, 'y', y);
      connectExpression(block, 'z', z);
      connectExpression(block, 'degrees', degrees);
    } else if (id == 'sin' || id == 'cos' || id == 'tan') {
      peeker.get(); // eat space
      var degrees = parse(peeker, workspace);
      block = workspace.newBlock('madeup_sincostan');
      block.setFieldValue(id, 'function');
      connectExpression(block, 'degrees', degrees);
    } else if (id == 'asin' || id == 'acos' || id == 'atan') {
      peeker.get(); // eat space
      var ratio = parse(peeker, workspace);
      block = workspace.newBlock('madeup_inverse_sincostan');
      block.setFieldValue(id, 'function');
      connectExpression(block, 'ratio', ratio);
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
      connectExpression(block, 'view', view);
    } else if (id == 'log') {
      peeker.get(); // eat space
      var base = parse(peeker, workspace);
      peeker.get(); // eat space
      var x = parse(peeker, workspace);
      block = workspace.newBlock('madeup_log');
      connectExpression(block, 'base', base);
      connectExpression(block, 'x', x);
    } else if (id == 'min' || id == 'max') {
      peeker.get(); // eat space
      var a = parse(peeker, workspace);
      peeker.get(); // eat space
      var b = parse(peeker, workspace);
      block = workspace.newBlock('madeup_minmax');
      block.setFieldValue(id, 'function');
      connectExpression(block, 'a', a);
      connectExpression(block, 'b', b);
    } else if (id == 'normalize' || id == 'magnitude') {
      peeker.get(); // eat space
      var v = parse(peeker, workspace);
      block = workspace.newBlock('madeup_' + id);
      connectExpression(block, 'vector', v);
    } else if (id == 'cross' || id == 'dot') {
      peeker.get(); // eat space
      var a = parse(peeker, workspace);
      peeker.get(); // eat space
      var b = parse(peeker, workspace);
      block = workspace.newBlock('madeup_' + id);
      connectExpression(block, 'a', a);
      connectExpression(block, 'b', b);
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
          connectExpression(block, 'arg' + i, actualBlock);
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
    connectExpression(block, 'value', value);
  }

  else if (token == 'repeat') {
    peeker.get(); // eat space
    var count = parse(peeker, workspace);
    peeker.get(); // eat space
    var body = parse(peeker, workspace);

    block = workspace.newBlock('madeup_repeat');
    connectExpression(block, 'count', count);
    connectStatement(block, 'body', body);
  }

  else if (token == 'repeatwich') {
    peeker.get(); // eat space
    var count = parse(peeker, workspace);
    peeker.get(); // eat space
    var surrounder = parse(peeker, workspace);
    peeker.get(); // eat space
    var surroundee = parse(peeker, workspace);

    block = workspace.newBlock('madeup_repeat_around');
    connectExpression(block, 'count', count);
    connectStatement(block, 'surrounder', surrounder);
    connectStatement(block, 'surroundee', surroundee);
  }

  else if (token == 'while') {
    peeker.get(); // eat space
    var condition = parse(peeker, workspace);
    peeker.get(); // eat space
    var body = parse(peeker, workspace);
    block = workspace.newBlock('madeup_while');
    connectExpression(block, 'condition', condition);
    connectStatement(block, 'body', body);
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
      connectExpression(block, 'condition', condition);
      connectStatement(block, 'then', thenBlock);
      connectStatement(block, 'else', elseBlock);
    } else {
      block = workspace.newBlock('madeup_if_statement');
      connectExpression(block, 'condition', condition);
      connectStatement(block, 'then', thenBlock);
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
    connectExpression(block, 'condition', condition);
    connectExpression(block, 'then', thenBlock);
    connectExpression(block, 'else', elseBlock);
  }

  else if (token == 'for-through') {
    peeker.get(); // eat space
    var iterator = parse(peeker, workspace);
    peeker.get(); // eat space
    var stop = parse(peeker, workspace);
    peeker.get(); // eat space
    var body = parse(peeker, workspace);
    block = workspace.newBlock('madeup_for_upper');
    block.setFieldValue('through', 'clusivity');
    connectExpression(block, 'iterator', iterator);
    connectExpression(block, 'stop', stop);
    connectStatement(block, 'body', body);
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
    block.setFieldValue('through', 'clusivity');
    connectExpression(block, 'iterator', iterator);
    connectExpression(block, 'stop', stop);
    connectExpression(block, 'by', by);
    connectStatement(block, 'body', body);
  }

  else if (token == 'for-to') {
    peeker.get(); // eat space
    var iterator = parse(peeker, workspace);
    peeker.get(); // eat space
    var stop = parse(peeker, workspace);
    peeker.get(); // eat space
    var body = parse(peeker, workspace);
    block = workspace.newBlock('madeup_for_upper');
    block.setFieldValue('to', 'clusivity');
    connectExpression(block, 'iterator', iterator);
    connectExpression(block, 'stop', stop);
    connectStatement(block, 'body', body);
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
    block.setFieldValue('to', 'clusivity');
    connectExpression(block, 'iterator', iterator);
    connectExpression(block, 'stop', stop);
    connectExpression(block, 'by', by);
    connectStatement(block, 'body', body);
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
    connectExpression(block, 'iterator', iterator);
    connectExpression(block, 'start', start);
    connectExpression(block, 'stop', stop);
    connectStatement(block, 'body', body);
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
    connectExpression(block, 'iterator', iterator);
    connectExpression(block, 'start', start);
    connectExpression(block, 'stop', stop);
    connectExpression(block, 'by', by);
    connectStatement(block, 'body', body);
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
        block.setFieldValue('-', 'operator');
        connectExpression(block, 'a', a);
        connectExpression(block, 'b', b);
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
      block.setFieldValue('-', 'operator');
      connectExpression(block, 'a', a);
      connectExpression(block, 'b', b);
    }
  }

  else if (token == '+' || token == '-' || token == '*' || token == '/' || token == '//' || token == '^' || token == '%') {
    peeker.get(); // eat space
    var a = parse(peeker, workspace);
    peeker.get(); // eat space
    var b = parse(peeker, workspace);
    block = workspace.newBlock('madeup_binary_arithmetic_operator');
    block.setFieldValue(token, 'operator');
    connectExpression(block, 'a', a);
    connectExpression(block, 'b', b);
  }

  else if (token == 'negate') {
    peeker.get(); // eat space
    var a = parse(peeker, workspace);
    block = workspace.newBlock('madeup_unary_operator');
    connectExpression(block, 'a', a);
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
    connectStatement(block, 'stack', body);
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
    connectExpression(block, 'i', i);
    connectExpression(block, 'collection', collection);
    connectExpression(block, 'rhs', rhs);
  }

  else if (token == '>' || token == '>=' || token == '<' || token == '<=' || token == '==' || token == '!=') {
    peeker.get(); // eat space
    var a = parse(peeker, workspace);
    peeker.get(); // eat space
    var b = parse(peeker, workspace);
    block = workspace.newBlock('madeup_relational_operator');
    block.setFieldValue(token, 'operator');
    connectExpression(block, 'a', a);
    connectExpression(block, 'b', b);
  }

  else if (token == 'or' || token == 'and') {
    peeker.get(); // eat space
    var a = parse(peeker, workspace);
    peeker.get(); // eat space
    var b = parse(peeker, workspace);
    block = workspace.newBlock('madeup_binary_logic_operator');
    block.setFieldValue(token, 'function');
    connectExpression(block, 'a', a);
    connectExpression(block, 'b', b);
  }

  else if (token == 'not') {
    peeker.get(); // eat space
    var a = parse(peeker, workspace);
    block = workspace.newBlock('madeup_not');
    connectExpression(block, 'a', a);
  }

  else if (token == 'abs') {
    peeker.get(); // eat space
    var x = parse(peeker, workspace);
    block = workspace.newBlock('madeup_abs');
    connectExpression(block, 'x', x);
  }

  else if (token == 'unit') {
    block = workspace.newBlock('madeup_nothing');
  }

  else if (token == 'integer') {
    peeker.get(); // eat space
    var i = peeker.getInteger();
    block = workspace.newBlock('madeup_integer');
    block.setFieldValue('' + i, 'integer');
  }

  else if (token == 'boolean') {
    peeker.get(); // eat space
    var literal = peeker.getToken();
    block = workspace.newBlock('madeup_boolean');
    block.setFieldValue(literal, 'boolean');
  }

  else if (token == 'real') {
    peeker.get(); // eat space
    var i = peeker.getReal();
    block = workspace.newBlock('madeup_real');
    block.setFieldValue('' + i, 'real');
  }

  else if (token == 'string') {
    peeker.get(); // eat space
    var s = '';
    while (peeker.peek() != ')') {
      s += peeker.get();
    }
    block = workspace.newBlock('madeup_string');
    block.setFieldValue(s, 'string');
  }

  else if (token == 'subscript') {
    peeker.get(); // eat space
    var collection = parse(peeker, workspace);
    peeker.get(); // eat space
    var i = parse(peeker, workspace);
    block = workspace.newBlock('madeup_subscript');
    connectExpression(block, 'i', i);
    connectExpression(block, 'collection', collection);
  }

  else if (token == 'subrange') {
    peeker.get(); // eat space
    var collection = parse(peeker, workspace);
    peeker.get(); // eat space
    var i = parse(peeker, workspace);
    peeker.get(); // eat space
    var j = parse(peeker, workspace);
    block = workspace.newBlock('madeup_subrange');
    connectExpression(block, 'i', i);
    connectExpression(block, 'j', j);
    connectExpression(block, 'collection', collection);
  }

  else if (token == 'arrayliteral') {
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
      connectExpression(block, 'element' + i, elements[i]);
    }
  }

  else if (token == 'makearray') {
    peeker.get(); // eat space
    var count = parse(peeker, workspace);
    peeker.get(); // eat space
    var element = parse(peeker, workspace);

    block = workspace.newBlock(element.type == 'madeup_array_of' ? 'madeup_array_by' : 'madeup_array_of');
    connectExpression(block, 'n', count);
    connectExpression(block, 'item', element);
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
