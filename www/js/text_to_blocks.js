function connectExpression(block, connectionName, expressionBlock) {
  setStatementExpression(expressionBlock, true);
  block.getInput(connectionName).connection.connect(expressionBlock.outputConnection);
}

function connectStatement(block, connectionName, expressionBlock) {
  if (expressionBlock) {
    setStatementExpression(expressionBlock, false);
    block.getInput(connectionName).connection.connect(expressionBlock.previousConnection);
  }
}

function Peeker(src) {
  console.log("src:", src);
  console.trace();
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

var sidToBlockType = {};
for (type in blockDefinitions) {
  if (blockDefinitions.hasOwnProperty(type)) {
    var config = blockDefinitions[type].config;
    if (config.hasOwnProperty('sid')) {
      if (Array.isArray(config.sid)) {
        config.sid.forEach(function(sid) {
          sidToBlockType[sid] = type;
        });
      } else {
        sidToBlockType[config.sid] = type;
      }
    }
  }
}

function parse(peeker, workspace) {
  var block = null;

  // assert '('
  if (peeker.peek() != '(') {
    console.trace();
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

  else if (token == 'call-with-names') {
    peeker.get(); // eat space
    id = peeker.getToken();

    var isBuiltin = sidToBlockType.hasOwnProperty(id);
    if (isBuiltin) {
      var type = sidToBlockType[id];
      var config = blockDefinitions[type].config;
      block = workspace.newBlock(type);

      if (Array.isArray(config.sid)) {
        block.setFieldValue(id, 'function'); 
      }

      if (config.hasOwnProperty('args0')) {
        var formals = config.args0.filter(function(formal) {
          return formal.type == 'input_value' || formal.type == 'input_statement';
        });

        // Connect any specified parameters.
        var formalsPresent = [];
        while (peeker.peek() == ' ') {
          peeker.get(); // eat space
          var formal = peeker.getToken();
          formalsPresent.push(formal);
          peeker.get(); // eat space
          var actual = parse(peeker, workspace);
          connectExpression(block, formal, actual);
        }

        // Remove any unspecified default parameters.
        formals.forEach(function(formal) {
          if (formal.hasOwnProperty('default') && !formalsPresent.includes(formal.name)) {
            block.enableParameter(formal.name, false);
          }
        });
      }
    } else {
      if (!Blockly.Procedures.isLegalName_(id, workspace)) {
        block = workspace.newBlock('procedures_callnoreturn');
        block.setFieldValue(id, 'NAME');
        var formals = getBlocklyProcedureFormals(workspace, id);
        var formalToPosition = {};

        // I used to use setProcedureParameters, but Blockly
        // moved that out of the API. Now we need a mutation.
        // Baah.
        var container = document.createElement('mutation'); 
        formals.forEach(function(formal, i) {
          var arg = document.createElement('arg'); 
          arg.setAttribute('name', formal);
          arg.setAttribute('paramId', formal);
          container.appendChild(arg);
          formalToPosition[formal] = i;
        });
        block.domToMutation(container);
 
        // Connect any specified parameters.
        while (peeker.peek() == ' ') {
          peeker.get(); // eat space
          var formal = peeker.getToken();
          peeker.get(); // eat space
          var actual = parse(peeker, workspace);
          connectExpression(block, 'ARG' + formalToPosition[formal], actual);
        }
      } else {
        var variable = workspace.createVariable(id);
        console.log("id:", id);
        block = workspace.newBlock('variables_get');
        block.setFieldValue(variable.getId(), 'VAR');
      }
    }
  }

  else if (token == 'call') {
    peeker.get(); // eat space
    id = peeker.getToken();

    var isBuiltin = sidToBlockType.hasOwnProperty(id);
    if (isBuiltin) {
      var type = sidToBlockType[id];
      var config = blockDefinitions[type].config;
      block = workspace.newBlock(type);
      console.log("config:", config);

      if (Array.isArray(config.sid)) {
        block.setFieldValue(id, 'function'); 
      }

      if (config.hasOwnProperty('args0')) {
        var i = 0;

        var args = config.args0.filter(function(arg) {
          return arg.type == 'input_value' || arg.type == 'input_statement';
        });
        console.log("args:", args);

        // Connect any specified parameters.
        while (i < args.length && peeker.peek() == ' ') {
          var arg = args[i];
          console.log("arg:", arg);
          var eaten = peeker.get(); // eat space
          console.log("eaten:", eaten);
          var actual = parse(peeker, workspace);
          connectExpression(block, arg.name, actual);
          ++i;
        }

        // Remove any unspecified default parameters.
        while (i < args.length) {
          var arg = args[i];
          if (arg.hasOwnProperty('default')) {
            block.enableParameter(arg.name, false);
          }
          ++i;
        }
      }
    } else {
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
        var variable = workspace.createVariable(id);
        block = workspace.newBlock('variables_get');
        block.setFieldValue(variable.getId(), 'VAR');
      }
    }
  }
  
  else if (token == 'return') {
    peeker.get(); // eat space
    var value = parse(peeker, workspace);
    block = workspace.newBlock('madeup_return');
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
      var variable = workspace.createVariable(id);
      var a = workspace.newBlock('variables_get');
      a.setFieldValue(variable.getId(), 'VAR');
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
      console.log("formal:", formal);
      formals.push(formal);
      if (peeker.peek() == ' ') {
        peeker.get(); // eat space
        var defaultActual = parse(peeker, workspace);
        defaultActual.dispose();
        console.error('Default parameters not supported in blocks yet.');
      }
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

    var variable = workspace.createVariable(id);
    block = workspace.newBlock('variables_set');
    block.setFieldValue(variable.getId(), 'VAR');
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
    console.trace();
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
