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
    console.log('expecting (');
    return null;
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
      var msg = parse(peeker);
      block = blocklyWorkspace.newBlock('madeup_io_print');
      connectExpression(block, 'MESSAGE', msg);
    } else if (id == 'echo') {
      peeker.get(); // eat space
      var mesh = parse(peeker);
      block = blocklyWorkspace.newBlock('madeup_generate_echo');
      connectExpression(block, 'MESH', mesh);
    } else if (id == 'debug') {
      peeker.get(); // eat space
      var msg = parse(peeker);
      block = blocklyWorkspace.newBlock('madeup_io_debug');
      connectExpression(block, 'MESSAGE', msg);
    } else if (id == 'sign') {
      peeker.get(); // eat space
      var x = parse(peeker);
      block = blocklyWorkspace.newBlock('madeup_math_sign');
      connectExpression(block, 'X', x);
    } else if (id == 'moveto') {
      peeker.get(); // eat space
      var x = parse(peeker);
      peeker.get(); // eat space
      var y = parse(peeker);
      peeker.get(); // eat space
      var z = parse(peeker);
      block = blocklyWorkspace.newBlock('madeup_movement_moveto');
      connectExpression(block, 'X', x);
      connectExpression(block, 'Y', y);
      connectExpression(block, 'Z', z);
    } else if (id == 'move') {
      peeker.get(); // eat space
      var x = parse(peeker);
      block = blocklyWorkspace.newBlock('madeup_movement_move');
      connectExpression(block, 'DISTANCE', x);
    } else if (id == 'random') {
      peeker.get(); // eat space
      var min = parse(peeker);
      peeker.get(); // eat space
      var max = parse(peeker);
      block = blocklyWorkspace.newBlock('madeup_math_random');
      connectExpression(block, 'MIN', min);
      connectExpression(block, 'MAX', max);
    } else if (id == 'atan2') {
      peeker.get(); // eat space
      var opposite = parse(peeker);
      peeker.get(); // eat space
      var adjacent = parse(peeker);
      block = blocklyWorkspace.newBlock('madeup_math_atan2');
      connectExpression(block, 'OPPOSITE', opposite);
      connectExpression(block, 'ADJACENT', adjacent);
    } else if (id == 'yaw' || id == 'roll' || id == 'pitch') {
      peeker.get(); // eat space
      var degrees = parse(peeker);
      block = blocklyWorkspace.newBlock('madeup_movement_turn');
      block.setFieldValue(id, 'TYPE');
      connectExpression(block, 'DEGREES', degrees);
    } else if (id == 'coalesce') {
      peeker.get(); // eat space
      var threshold = parse(peeker);
      block = blocklyWorkspace.newBlock('madeup_path_coalesce');
      connectExpression(block, 'THRESHOLD', threshold);
    } else if (id == 'translate' || id == 'scale') {
      peeker.get(); // eat space
      var x = parse(peeker);
      peeker.get(); // eat space
      var y = parse(peeker);
      peeker.get(); // eat space
      var z = parse(peeker);
      block = blocklyWorkspace.newBlock('madeup_movement_' + id);
      connectExpression(block, 'X', x);
      connectExpression(block, 'Y', y);
      connectExpression(block, 'Z', z);
    } else if (id == 'spheres' || id == 'forget' || id == 'boxes' || id == 'polygon') {
      block = blocklyWorkspace.newBlock('madeup_generate_' + id);
    } else if (id == 'revolve') {
      peeker.get(); // eat space
      var x = parse(peeker);
      peeker.get(); // eat space
      var y = parse(peeker);
      peeker.get(); // eat space
      var z = parse(peeker);
      peeker.get(); // eat space
      var degrees = parse(peeker);
      block = blocklyWorkspace.newBlock('madeup_generate_revolve');
      connectExpression(block, 'X', x);
      connectExpression(block, 'Y', y);
      connectExpression(block, 'Z', z);
      connectExpression(block, 'DEGREES', degrees);
    } else if (id == 'extrude') {
      peeker.get(); // eat space
      var x = parse(peeker);
      peeker.get(); // eat space
      var y = parse(peeker);
      peeker.get(); // eat space
      var z = parse(peeker);
      peeker.get(); // eat space
      var length = parse(peeker);
      block = blocklyWorkspace.newBlock('madeup_generate_extrude');
      connectExpression(block, 'X', x);
      connectExpression(block, 'Y', y);
      connectExpression(block, 'Z', z);
      connectExpression(block, 'LENGTH', length);
    } else if (id == 'surface') {
      peeker.get(); // eat space
      var columns = parse(peeker);
      peeker.get(); // eat space
      var rows = parse(peeker);
      block = blocklyWorkspace.newBlock('madeup_generate_surface');
      connectExpression(block, 'COLUMNS', columns);
      connectExpression(block, 'ROWS', rows);
    } else if (id == 'dowel' || id == 'tube') {
      var maxBend;
      if (peeker.peek() == ' ') {
        peeker.get(); // eat space
        peeker.getToken(); // TODO consume maxBend
        peeker.get(); // eat space
        maxBend = parse(peeker);
      } else {
        maxBend = blocklyWorkspace.newBlock('madeup_math_integer');
        maxBend.setFieldValue(360, 'INTEGER');
        maxBend.initSvg();
        maxBend.render();
      }
      block = blocklyWorkspace.newBlock('madeup_generate_' + id);
      connectExpression(block, 'MAXBEND', maxBend);
    } else if (id == 'rotate') {
      peeker.get(); // eat space
      var x = parse(peeker);
      peeker.get(); // eat space
      var y = parse(peeker);
      peeker.get(); // eat space
      var z = parse(peeker);
      peeker.get(); // eat space
      var degrees = parse(peeker);
      block = blocklyWorkspace.newBlock('madeup_movement_' + id);
      connectExpression(block, 'X', x);
      connectExpression(block, 'Y', y);
      connectExpression(block, 'Z', z);
      connectExpression(block, 'DEGREES', degrees);
    } else if (id == 'sin' || id == 'cos' || id == 'tan') {
      peeker.get(); // eat space
      var degrees = parse(peeker);
      block = blocklyWorkspace.newBlock('madeup_math_sincostan');
      block.setFieldValue(id, 'F');
      connectExpression(block, 'DEGREES', degrees);
    } else if (id == 'asin' || id == 'acos' || id == 'atan') {
      peeker.get(); // eat space
      var ratio = parse(peeker);
      block = blocklyWorkspace.newBlock('madeup_math_inverse_sincostan');
      block.setFieldValue(id, 'F');
      connectExpression(block, 'RATIO', ratio);
    } else if (id == 'center' || id == 'identity' || id == 'reframe' || id == 'push' || id == 'pop' || id == 'reverse') {
      block = blocklyWorkspace.newBlock('madeup_movement_' + id);
    } else if (id == 'where') {
      block = blocklyWorkspace.newBlock('madeup_movement_where');
    } else if (id == 'log') {
      peeker.get(); // eat space
      var base = parse(peeker);
      peeker.get(); // eat space
      var x = parse(peeker);
      block = blocklyWorkspace.newBlock('madeup_math_log');
      connectExpression(block, 'BASE', base);
      connectExpression(block, 'X', x);
    } else if (id == 'min' || id == 'max') {
      peeker.get(); // eat space
      var a = parse(peeker);
      peeker.get(); // eat space
      var b = parse(peeker);
      block = blocklyWorkspace.newBlock('madeup_math_minmax');
      block.setFieldValue(id, 'F');
      connectExpression(block, 'A', a);
      connectExpression(block, 'B', b);
    } else if (id == 'normalize' || id == 'magnitude') {
      peeker.get(); // eat space
      var v = parse(peeker);
      block = blocklyWorkspace.newBlock('madeup_' + id);
      connectExpression(block, 'VECTOR', v);
    } else if (id == 'cross' || id == 'dot') {
      peeker.get(); // eat space
      var a = parse(peeker);
      peeker.get(); // eat space
      var b = parse(peeker);
      block = blocklyWorkspace.newBlock('madeup_' + id);
      connectExpression(block, 'A', a);
      connectExpression(block, 'B', b);
    } else {
      // is it a variable or a function?
      if (!Blockly.Procedures.isLegalName(id, blocklyWorkspace)) {
        block = blocklyWorkspace.newBlock('procedures_callnoreturn');
        block.setFieldValue(id, 'NAME');
        var formals = getBlocklyProcedureFormals(id);
        block.setProcedureParameters(formals, formals);
        var i = 0;
        while (peeker.peek() == ' ') {
          peeker.get(); // eat space
          var actualBlock = parse(peeker);
          connectExpression(block, 'ARG' + i, actualBlock);
          ++i;
        }
      } else {
        block = blocklyWorkspace.newBlock('variables_get');
        block.setFieldValue(id, 'VAR');
      }
    }
  }

  else if (token == 'repeat') {
    peeker.get(); // eat space
    var count = parse(peeker);
    peeker.get(); // eat space
    var body = parse(peeker);

    block = blocklyWorkspace.newBlock('madeup_loop_repeat');
    connectExpression(block, 'COUNT', count);
    connectStatement(block, 'BLOCK', body);
  }

  else if (token == 'repeatwich') {
    peeker.get(); // eat space
    var count = parse(peeker);
    peeker.get(); // eat space
    var surrounder = parse(peeker);
    peeker.get(); // eat space
    var surroundee = parse(peeker);

    block = blocklyWorkspace.newBlock('madeup_loop_repeatwich');
    connectExpression(block, 'COUNT', count);
    connectStatement(block, 'SURROUNDER', surrounder);
    connectStatement(block, 'SURROUNDEE', surroundee);
  }

  else if (token == 'while') {
    peeker.get(); // eat space
    var condition = parse(peeker);
    peeker.get(); // eat space
    var body = parse(peeker);
    block = blocklyWorkspace.newBlock('madeup_loop_while');
    connectExpression(block, 'CONDITION', condition);
    connectStatement(block, 'BODY', body);
  }

  else if (token == 'if') {
    peeker.get(); // eat space
    var condition = parse(peeker);
    peeker.get(); // eat space
    var thenBlock = parse(peeker);
    peeker.get(); // eat space
    var elseBlock = parse(peeker);
    block = blocklyWorkspace.newBlock('madeup_logic_if_else_statement');
    connectExpression(block, 'CONDITION', condition);
    connectStatement(block, 'THEN', thenBlock);
    connectStatement(block, 'ELSE', elseBlock);
  }

  else if (token == 'for-through') {
    peeker.get(); // eat space
    var id = peeker.getToken();
    peeker.get(); // eat space
    var stop = parse(peeker);
    peeker.get(); // eat space
    var body = parse(peeker);
    block = blocklyWorkspace.newBlock('madeup_loop_for_through');
    block.setFieldValue(id, 'ITERATOR');
    connectExpression(block, 'STOP', stop);
    connectStatement(block, 'BODY', body);
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
    block = blocklyWorkspace.newBlock('madeup_loop_for_through_by');
    block.setFieldValue(id, 'ITERATOR');
    connectExpression(block, 'STOP', stop);
    connectExpression(block, 'BY', by);
    connectStatement(block, 'BODY', body);
  }

  else if (token == 'for-to') {
    peeker.get(); // eat space
    var id = peeker.getToken();
    peeker.get(); // eat space
    var stop = parse(peeker);
    peeker.get(); // eat space
    var body = parse(peeker);
    block = blocklyWorkspace.newBlock('madeup_loop_for_to');
    block.setFieldValue(id, 'ITERATOR');
    connectExpression(block, 'STOP', stop);
    connectStatement(block, 'BODY', body);
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
    block = blocklyWorkspace.newBlock('madeup_loop_for_to_by');
    block.setFieldValue(id, 'ITERATOR');
    connectExpression(block, 'STOP', stop);
    connectExpression(block, 'BY', by);
    connectStatement(block, 'BODY', body);
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
    block = blocklyWorkspace.newBlock('madeup_loop_for_in');
    block.setFieldValue(id, 'ITERATOR');
    connectExpression(block, 'START', start);
    connectExpression(block, 'STOP', stop);
    connectStatement(block, 'BODY', body);
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
    block = blocklyWorkspace.newBlock('madeup_loop_for_in_by');
    block.setFieldValue(id, 'ITERATOR');
    connectExpression(block, 'START', start);
    connectExpression(block, 'STOP', stop);
    connectExpression(block, 'BY', by);
    connectStatement(block, 'BODY', body);
  }

  else if (token == '+' || token == '-' || token == '*' || token == '/' || token == '//' || token == '^' || token == '%') {
    peeker.get(); // eat space
    var a = parse(peeker);
    peeker.get(); // eat space
    var b = parse(peeker);
    block = blocklyWorkspace.newBlock('madeup_math_binary_arithmetic_operator');
    block.setFieldValue(token, 'OPERATOR');
    connectExpression(block, 'A', a);
    connectExpression(block, 'B', b);
  }

  else if (token == 'negate') {
    peeker.get(); // eat space
    var a = parse(peeker);
    block = blocklyWorkspace.newBlock('madeup_math_unary_operator');
    connectExpression(block, 'A', a);
  }

  else if (token == 'define') {
    peeker.get(); // eat space
    var id = peeker.getToken();
    block = blocklyWorkspace.newBlock('procedures_defnoreturn');
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
    connectStatement(block, 'STACK', body);
  }

  else if (token == 'define-variable') {
    peeker.get(); // eat space
    var id = peeker.getToken();
    peeker.get(); // eat space
    var rhs = parse(peeker);

    block = blocklyWorkspace.newBlock('variables_set');
    block.setFieldValue(id, 'VAR');
    connectExpression(block, 'VALUE', rhs);
  }

  else if (token == 'define-element') {
    peeker.get(); // eat space
    peeker.get(); // eat (
    peeker.getToken(); // eat SUBSCRIPT
    peeker.get(); // eat space

    var collection = parse(peeker);
    peeker.get(); // eat space
    var i = parse(peeker);
    peeker.get(); // eat )
    peeker.get(); // eat space
    var rhs = parse(peeker);

    block = blocklyWorkspace.newBlock('madeup_subscript_set');
    connectExpression(block, 'I', i);
    connectExpression(block, 'COLLECTION', collection);
    connectExpression(block, 'RHS', rhs);
  }

  else if (token == '>' || token == '>=' || token == '<' || token == '<=' || token == '==' || token == '!=') {
    peeker.get(); // eat space
    var a = parse(peeker);
    peeker.get(); // eat space
    var b = parse(peeker);
    block = blocklyWorkspace.newBlock('madeup_math_relational_operator');
    block.setFieldValue(token, 'OPERATOR');
    connectExpression(block, 'A', a);
    connectExpression(block, 'B', b);
  }

  else if (token == 'or' || token == 'and') {
    peeker.get(); // eat space
    var a = parse(peeker);
    peeker.get(); // eat space
    var b = parse(peeker);
    block = blocklyWorkspace.newBlock('madeup_logic_junction');
    block.setFieldValue(token, 'F');
    connectExpression(block, 'A', a);
    connectExpression(block, 'B', b);
  }

  else if (token == 'not') {
    peeker.get(); // eat space
    var a = parse(peeker);
    block = blocklyWorkspace.newBlock('madeup_logic_not');
    connectExpression(block, 'A', a);
  }

  else if (token == 'abs') {
    peeker.get(); // eat space
    var x = parse(peeker);
    block = blocklyWorkspace.newBlock('madeup_math_abs');
    connectExpression(block, 'X', x);
  }

  else if (token == 'unit') {
    block = blocklyWorkspace.newBlock('madeup_nothing');
  }

  else if (token == 'INTEGER') {
    peeker.get(); // eat space
    var i = peeker.getInteger();
    block = blocklyWorkspace.newBlock('madeup_math_integer');
    block.setFieldValue('' + i, 'INTEGER');
  }

  else if (token == 'BOOLEAN') {
    peeker.get(); // eat space
    var literal = peeker.getToken();
    block = blocklyWorkspace.newBlock('madeup_logic_boolean');
    block.setFieldValue(literal, 'BOOLEAN');
  }

  else if (token == 'REAL') {
    peeker.get(); // eat space
    var i = peeker.getReal();
    block = blocklyWorkspace.newBlock('madeup_math_real');
    block.setFieldValue('' + i, 'REAL');
  }

  else if (token == 'STRING') {
    peeker.get(); // eat space
    var s = '';
    while (peeker.peek() != ')') {
      s += peeker.get();
    }
    block = blocklyWorkspace.newBlock('madeup_string');
    block.setFieldValue(s, 'STRING');
  }

  else if (token == 'SUBSCRIPT') {
    peeker.get(); // eat space
    var collection = parse(peeker);
    peeker.get(); // eat space
    var i = parse(peeker);
    block = blocklyWorkspace.newBlock('madeup_subscript');
    connectExpression(block, 'I', i);
    connectExpression(block, 'COLLECTION', collection);
  }

  else if (token == 'SUBRANGE') {
    peeker.get(); // eat space
    var collection = parse(peeker);
    peeker.get(); // eat space
    var i = parse(peeker);
    peeker.get(); // eat space
    var j = parse(peeker);
    block = blocklyWorkspace.newBlock('madeup_subrange');
    connectExpression(block, 'I', i);
    connectExpression(block, 'J', j);
    connectExpression(block, 'COLLECTION', collection);
  }

  else if (token == 'ARRAYLITERAL') {
    var elements = [];
    while (peeker.peek() != ')') {
      peeker.get(); // eat space
      var element = parse(peeker);
      elements.push(element);
    }

    block = blocklyWorkspace.newBlock('madeup_array_literal');
    block.itemCount_ = elements.length;
    block.updateShape_();

    for (var i = 0; i < elements.length; ++i) {
      connectExpression(block, 'ELEMENT' + i, elements[i]);
    }
  }

  else if (token == 'MAKEARRAY') {
    peeker.get(); // eat space
    var count = parse(peeker);
    peeker.get(); // eat space
    var element = parse(peeker);

    block = blocklyWorkspace.newBlock(element.type == 'madeup_array_of' ? 'madeup_array_by' : 'madeup_array_of');
    connectExpression(block, 'N', count);
    connectExpression(block, 'ITEM', element);
  }

  // assert ')'
  if (peeker.peek() != ')') {
    throw 'expecting )';
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
