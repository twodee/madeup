'use strict';

goog.require('Blockly.Blocks');

goog.provide('Blockly.Blocks.madeup');
goog.provide('Blockly.Madeup');

Blockly.Variables.allUsedVariables = (function (proxied) {
  return function (root) {
    var list = proxied.call(this, root);
    list.push('.rgb');
    list.push('.innerRadius');
    list.push('.outerRadius');
    list.push('.radius');
    list.push('nsides');
    console.log('myAllUsedVariables: ' + list);
    return list;
  };
})(Blockly.Variables.allUsedVariables);

function setStatementExpression(block, isExpression) {
  var isExpressionAlready = !!block.outputConnection;
  if (isExpression != isExpressionAlready) {
    block.unplug(true, true);
    if (isExpression) {
      block.setPreviousStatement(false);
      block.setNextStatement(false);
      block.setOutput(true);
      block.setColour(EXPRESSION_HUE);
    } else {
      block.setOutput(false);
      block.setPreviousStatement(true);
      block.setNextStatement(true);
      block.setColour(STATEMENT_HUE);
    }
  }
}

function toggleStatementExpression(options) {
  var isExpression = !!this.outputConnection;
  var block = this;
  var option = {
    enabled: true,
    text: isExpression ? 'Convert to Statement' : 'Convert to Expression',
    callback: function() {
      setStatementExpression(block, !isExpression);
    }
  };
  options.push(option);
}

function mutationModeToDom(block, container) {
  var isExpression = !!block.outputConnection;
  // Something forces lowercase attribute names. I'm not going to fight it.
  container.setAttribute('isexpression', isExpression);
}

function mutationToDom() {
  var container = document.createElement('mutation');
  mutationModeToDom(this, container);
  return container;
}

function domModeToMutation(element) {
  var isExpression = element.getAttribute('isexpression') == 'true';
  setStatementExpression(this, isExpression); 
}

Blockly.Blocks.madeup.EXPRESSION_HUE = 210;
Blockly.Blocks.madeup.LITERAL_HUE = 315;
Blockly.Blocks.madeup.STATEMENT_HUE = 180;

Blockly.Madeup = new Blockly.Generator('Madeup');
Blockly.Madeup.INDENT = '  ';            // 0 "" ...
Blockly.Madeup.ORDER_ATOMIC = 0;            // 0 "" ...
Blockly.Madeup.ORDER_ARRAY_OF = 2;
Blockly.Madeup.ORDER_ARRAY_BY = 3;
Blockly.Madeup.ORDER_EXPONENTIATION = 4;    // **
Blockly.Madeup.ORDER_MULTIPLICATIVE = 6;    // * / // %
Blockly.Madeup.ORDER_ADDITIVE = 7;          // + -
Blockly.Madeup.ORDER_RELATIONAL = 11;       // in, not in, is, is not,
Blockly.Madeup.ORDER_EQUALITY = 12;
Blockly.Madeup.ORDER_LOGICAL_NOT = 13;      // not
Blockly.Madeup.ORDER_LOGICAL_AND = 14;      // and
Blockly.Madeup.ORDER_LOGICAL_OR = 15;       // or
Blockly.Madeup.ORDER_CONDITIONAL = 16;      // if else
Blockly.Madeup.ORDER_FUNCTION_CALL_FIRST_PARAMETER = 30;     // ()
Blockly.Madeup.ORDER_FUNCTION_CALL_ONLY_PARAMETER = 30;     // ()
Blockly.Madeup.ORDER_UNARY_NEGATION = 31;        // + -
Blockly.Madeup.ORDER_FUNCTION_CALL_NOT_FIRST_PARAMETER = 32;     // ()
Blockly.Madeup.ORDER_NONE = 99;             // (...)
Blockly.Madeup.ORDER_FUNCTION_CALL = 33;     // ()
Blockly.Madeup.PROCEDURES_DEFRETURN_COMMENT = null;
Blockly.Madeup.PROCEDURES_DEFNORETURN_COMMENT = null;
Blockly.Madeup.addReservedWords('');

// Unary negation has lower precedence than first parameter. Meaning a unary
// negation in a first parameter slot will be parenthesized. Unary negation has
// higher precedence than non-first parameter, meaning in a non-first parameter
// slot it will not be parenthesized.
//
// I'd like to avoid parenthesizing parameters when the function only has one
// parameter. But I can't do this if the operation is unary negation.
//
// lower numbers - higher precedence

/**
 * Initialise the database of variable names.
 * @param {!Blockly.Workspace} workspace Workspace to generate code from.
 */
Blockly.Madeup.init = function(workspace) {
  // Create a dictionary of definitions to be printed before the code.
  // Blockly.Madeup.definitions_ = Object.create(null);
  // Create a dictionary mapping desired function names in definitions_
  // to actual function names (to avoid collisions with user functions).
  // Blockly.Madeup.functionNames_ = Object.create(null);

  if (!Blockly.Madeup.variableDB_) {
    Blockly.Madeup.variableDB_ = new Blockly.Names(Blockly.Madeup.RESERVED_WORDS_);
  } else {
    Blockly.Madeup.variableDB_.reset();
  }

  // var defvars = [];
  // var variables = Blockly.Variables.allVariables(workspace);
  // for (var x = 0; x < variables.length; x++) {
   // defvars[x] = Blockly.Madeup.variableDB_.getName(variables[x],
       // Blockly.Variables.NAME_TYPE) + ' = None';
  // }
  // Blockly.Madeup.definitions_['variables'] = defvars.join('\n');
};

/**
 * Prepend the generated code with the variable definitions.
 * @param {string} code Generated code.
 * @return {string} Completed code.
 */
Blockly.Madeup.finish = function(code) {
  // Convert the definitions dictionary into a list.
  // var imports = [];
  // var definitions = [];
  // for (var name in Blockly.Madeup.definitions_) {
    // var def = Blockly.Madeup.definitions_[name];
    // if (def.match(/^(from\s+\S+\s+)?import\s+\S+/)) {
      // imports.push(def);
    // } else {
      // definitions.push(def);
    // }
  // }
  // var allDefs = imports.join('\n') + '\n\n' + definitions.join('\n\n');
  // return allDefs.replace(/\n\n+/g, '\n\n').replace(/\n*$/, '\n\n\n') + code;
  return code;
};

/**
 * Naked values are top-level blocks with outputs that aren't plugged into
 * anything.
 * @param {string} line Line of generated code.
 * @return {string} Legal line of code.
 */
Blockly.Madeup.scrubNakedValue = function(line) {
  return line + '\n';
};

/**
 * Common tasks for generating Python from blocks.
 * Handles comments for the specified block and any connected value blocks.
 * Calls any statements following this block.
 * @param {!Blockly.Block} block The current block.
 * @param {string} code The Python code created for this block.
 * @return {string} Python code with comments and subsequent blocks added.
 * @private
 */
Blockly.Madeup.scrub_ = function(block, code) {
  var commentCode = '';
  // Only collect comments for blocks that aren't inline.
  if (!block.outputConnection || !block.outputConnection.targetConnection) {
    // Collect comment for this block.
    var comment = block.getCommentText();
    if (comment) {
      commentCode += Blockly.Madeup.prefixLines(comment, '-- ') + '\n';
    }
    // Collect comments for all value arguments.
    // Don't collect comments for nested statements.
    for (var x = 0; x < block.inputList.length; x++) {
      if (block.inputList[x].type == Blockly.INPUT_VALUE) {
        var childBlock = block.inputList[x].connection.targetBlock();
        if (childBlock) {
          var comment = Blockly.Madeup.allNestedComments(childBlock);
          if (comment) {
            commentCode += Blockly.Madeup.prefixLines(comment, '-- ');
          }
        }
      }
    }
  }
  var nextBlock = block.nextConnection && block.nextConnection.targetBlock();
  var nextCode = Blockly.Madeup.blockToCode(nextBlock);
  return commentCode + code + nextCode;
};

/*
 * Statements return their source code. Expressions yield their code plus their lowest precedence.
 */

function generateInMode(block, code, precedence) {
  if (block.outputConnection) {
    return [code, precedence];
  } else {
    return code + '\n';
  }
}

// --------------------------------------------------------------------------- 

for (var block_type in block_definitions) {
  if (block_definitions.hasOwnProperty(block_type)) {
    (function (config) {
      Blockly.Blocks[block_type] = {
        init: function() {
          this.jsonInit(config);
          Blockly.addClass_(this.svgGroup_, 'wippo');
        },
        customContextMenu: toggleStatementExpression,
        mutationToDom: mutationToDom,
        domToMutation: domModeToMutation
      };
    })(block_definitions[block_type].config);

    Blockly.Madeup[block_type] = block_definitions[block_type].generator;
  }
}

// ----------------------------------------------------------------------------

Blockly.Blocks['madeup_array_literal'] = {
  /**
   * Block for creating a list with any number of elements of any type.
   * @this Blockly.Block
   */
  init: function() {
    // this.setHelpUrl(Blockly.Msg.LISTS_CREATE_WITH_HELPURL);
    this.setColour(Blockly.Blocks.madeup.EXPRESSION_HUE);
    this.itemCount_ = 3;
    this.updateShape_();
    this.setOutput(true, 'Array');
    this.setMutator(new Blockly.Mutator(['madeup_array_element']));
    this.setTooltip(Blockly.Msg.LISTS_CREATE_WITH_TOOLTIP);
  },
  /**
   * Create XML to represent list inputs.
   * @return {!Element} XML storage element.
   * @this Blockly.Block
   */
  mutationToDom: function() {
    var container = document.createElement('mutation');
    container.setAttribute('items', this.itemCount_);
    mutationModeToDom(this, container);
    return container;
  },
  /**
   * Parse XML to restore the list inputs.
   * @param {!Element} xmlElement XML storage element.
   * @this Blockly.Block
   */
  domToMutation: function(xmlElement) {
    this.itemCount_ = parseInt(xmlElement.getAttribute('items'), 10);
    this.updateShape_();
    domModeToMutation.call(this, xmlElement);
  },
  /**
   * Populate the mutator's dialog with this block's components.
   * @param {!Blockly.Workspace} workspace Mutator's workspace.
   * @return {!Blockly.Block} Root block in mutator.
   * @this Blockly.Block
   */
  decompose: function(workspace) {
    var containerBlock = workspace.newBlock('madeup_array_create_with_container');
    containerBlock.initSvg();
    var connection = containerBlock.getInput('STACK').connection;
    for (var i = 0; i < this.itemCount_; i++) {
      var itemBlock = workspace.newBlock('madeup_array_element');
      itemBlock.initSvg();
      connection.connect(itemBlock.previousConnection);
      connection = itemBlock.nextConnection;
    }
    return containerBlock;
  },
  /**
   * Reconfigure this block based on the mutator dialog's components.
   * @param {!Blockly.Block} containerBlock Root block in mutator.
   * @this Blockly.Block
   */
  compose: function(containerBlock) {
    var itemBlock = containerBlock.getInputTargetBlock('STACK');
    // Count number of inputs.
    var connections = [];
    while (itemBlock) {
      connections.push(itemBlock.valueConnection_);
      itemBlock = itemBlock.nextConnection && itemBlock.nextConnection.targetBlock();
    }
    // Disconnect any children that don't belong.
    for (var i = 0; i < this.itemCount_; i++) {
      var connection = this.getInput('ELEMENT' + i).connection.targetConnection;
      if (connection && connections.indexOf(connection) == -1) {
        connection.disconnect();
      }
    }
    this.itemCount_ = connections.length;
    this.updateShape_();
    // Reconnect any child blocks.
    for (var i = 0; i < this.itemCount_; i++) {
      Blockly.Mutator.reconnect(connections[i], this, 'ELEMENT' + i);
    }
  },
  /**
   * Store pointers to any connected child blocks.
   * @param {!Blockly.Block} containerBlock Root block in mutator.
   * @this Blockly.Block
   */
  saveConnections: function(containerBlock) {
    var itemBlock = containerBlock.getInputTargetBlock('STACK');
    var i = 0;
    while (itemBlock) {
      var input = this.getInput('ELEMENT' + i);
      itemBlock.valueConnection_ = input && input.connection.targetConnection;
      i++;
      itemBlock = itemBlock.nextConnection && itemBlock.nextConnection.targetBlock();
    }
  },
  /**
   * Modify this block to have the correct number of inputs.
   * @private
   * @this Blockly.Block
   */
  updateShape_: function() {
    if (this.itemCount_ && this.getInput('EMPTY')) {
      this.removeInput('EMPTY');
    } else if (!this.itemCount_ && !this.getInput('EMPTY')) {
      this.appendDummyInput('EMPTY')
          .appendField(Blockly.Msg.LISTS_CREATE_EMPTY_TITLE);
    }
    // ELEMENT new inputs.
    for (var i = 0; i < this.itemCount_; i++) {
      if (!this.getInput('ELEMENT' + i)) {
        var input = this.appendValueInput('ELEMENT' + i);
        if (i == 0) {
          input.appendField('array');
        }
      }
    }
    // Remove deleted inputs.
    while (this.getInput('ELEMENT' + i)) {
      this.removeInput('ELEMENT' + i);
      i++;
    }
  },
  customContextMenu: toggleStatementExpression,
  // mutationToDom: mutationToDom,
  // domToMutation: domModeToMutation
};

Blockly.Blocks['madeup_array_create_with_container'] = {
  /**
   * Mutator block for list container.
   * @this Blockly.Block
   */
  init: function() {
    this.setColour(Blockly.Blocks.lists.HUE);
    this.appendDummyInput()
        .appendField('array');
    this.appendStatementInput('STACK');
    this.contextMenu = false;
  },
  customContextMenu: toggleStatementExpression,
  mutationToDom: mutationToDom,
  domToMutation: domModeToMutation
};

Blockly.Blocks['madeup_array_element'] = {
  init: function() {
    this.setColour(Blockly.Blocks.lists.HUE);
    this.appendDummyInput()
        .appendField('element');
    this.setPreviousStatement(true);
    this.setNextStatement(true);
    // this.setTooltip(Blockly.Msg.LISTS_CREATE_WITH_ITEM_TOOLTIP);
    this.contextMenu = false;
  },
  customContextMenu: toggleStatementExpression,
  mutationToDom: mutationToDom,
  domToMutation: domModeToMutation
};

// --------------------------------------------------------------------------- 
// GENERATORS FOR BUILTINS

Blockly.Madeup['variables_get'] = function(block) {
  var code = Blockly.Madeup.variableDB_.getName(block.getFieldValue('VAR'), Blockly.Variables.NAME_TYPE);
  return generateInMode(block, code, Blockly.Madeup.ORDER_ATOMIC);
};

Blockly.Madeup['variables_set'] = function(block) {
  var argument0 = Blockly.Madeup.valueToCode(block, 'VALUE', Blockly.Madeup.ORDER_NONE) || '0';
  var varName = Blockly.Madeup.variableDB_.getName(block.getFieldValue('VAR'), Blockly.Variables.NAME_TYPE);
  var code = varName + ' = ' + argument0;
  return generateInMode(block, code, Blockly.Madeup.ORDER_ATOMIC);
};

Blockly.Madeup['procedures_defnoreturn'] = function(block) {
  var funcName = Blockly.Madeup.variableDB_.getName(block.getFieldValue('NAME'), Blockly.Procedures.NAME_TYPE);
  var branch = Blockly.Madeup.statementToCode(block, 'STACK');

  var args = [];
  for (var x = 0; x < block.arguments_.length; x++) {
    args[x] = Blockly.Madeup.variableDB_.getName(block.arguments_[x], Blockly.Variables.NAME_TYPE);
  }

  var code = 'to ' + funcName + ' ' + args.join(' ') + '\n' + branch + 'end';
  return generateInMode(block, code, Blockly.Madeup.ORDER_ATOMIC);
};

Blockly.Madeup['procedures_callnoreturn'] = function(block) {
  var funcName = Blockly.Madeup.variableDB_.getName(block.getFieldValue('NAME'), Blockly.Procedures.NAME_TYPE);
  var args = [];
  var precedence;
  for (var i = 0; i < block.arguments_.length; i++) {
    if (block.arguments_.length == 1) {
      precedence = Blockly.Madeup.ORDER_FUNCTION_CALL_ONLY_PARAMETER;
    } else if (i == 0) {
      precedence = Blockly.Madeup.ORDER_FUNCTION_CALL_FIRST_PARAMETER;
    } else {
      precedence = Blockly.Madeup.ORDER_FUNCTION_CALL_NOT_FIRST_PARAMETER;
    }
    args[i] = Blockly.Madeup.valueToCode(block, 'ARG' + i, precedence) || '';
  }
  var code = funcName;
  if (args.length) {
    code += ' ' + args.join(', ');
  }
  return generateInMode(block, code, Blockly.Madeup.ORDER_FUNCTION_CALL);
};

Blockly.Madeup['madeup_array_literal'] = function(block) {
  var elements = [];
  for (var i = 0; i < block.itemCount_; i++) {
    elements[i] = Blockly.Madeup.valueToCode(block, 'ELEMENT' + i, Blockly.Madeup.ORDER_ATOMIC) || '';
  }
  var code = '{' + elements.join(', ') + '}';
  return generateInMode(block, code, Blockly.Madeup.ORDER_ATOMIC);
};

// ----------------------------------------------------------------------------
// Remove the return blocks that Blockly automatically generates in the
// PROCEDURES node of the toolbox. Madeup doesn't support explicit return
// statements.
Blockly.Blocks['procedures_ifreturn'] = null;
Blockly.Blocks['procedures_defreturn'] = null;
// Blockly.Blocks['procedures_callreturn'] = null;

// When you create a new variable, Blockly wants to create a setter, an rvalue,
// and a changer. We don't want to the changer.
Blockly.Blocks['math_change'] = null;

// Blockly.Blocks['procedures_callreturn'].defType_ = 'procedures_defnoreturn';

// Blockly makes the function definition blocks for us. Let's tweak them
// so they can change betweeen statements and expressions.
function extendBuiltin(id) {
  var oldContextMenu = Blockly.Blocks[id].customContextMenu;
  Blockly.Blocks[id].customContextMenu = function(options) {
    oldContextMenu.call(this, options);
    toggleStatementExpression.call(this, options);
  };

  var oldDomToMutation = Blockly.Blocks[id].domToMutation;
  Blockly.Blocks[id].domToMutation = function(xmlElement) {
    oldDomToMutation.call(this, xmlElement);
    domModeToMutation.call(this, xmlElement);
  };

  // Not every block has a mutation.
  var oldMutationToDom = Blockly.Blocks[id].mutationToDom;
  if (oldMutationToDom) {
    Blockly.Blocks[id].mutationToDom = function() {
      var container = oldMutationToDom.call(this);
      mutationModeToDom(this, container);
      return container;
    };
  }
}

extendBuiltin('procedures_defnoreturn');
extendBuiltin('procedures_callnoreturn');
extendBuiltin('variables_get');
extendBuiltin('variables_set');

Blockly.Blocks.variables.GET_HUE = LITERAL_HUE;
Blockly.Blocks.variables.SET_HUE = STATEMENT_HUE;
Blockly.Blocks.procedures.DEF_NO_RETURN_HUE = STATEMENT_HUE;
Blockly.Blocks.procedures.DEF_RETURN_HUE = STATEMENT_HUE;
Blockly.Blocks.procedures.CALL_NO_RETURN_HUE = STATEMENT_HUE;
Blockly.Blocks.procedures.CALL_RETURN_HUE = EXPRESSION_HUE;

