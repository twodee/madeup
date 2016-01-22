'use strict';

goog.provide('Blockly.Blocks.madeup');
goog.require('Blockly.Blocks');

function setStatementExpression(block, isExpression) {
  block.unplug(true, true);
  if (isExpression) {
    block.setPreviousStatement(false);
    block.setNextStatement(false);
    block.setOutput(true);
  } else {
    block.setOutput(false);
    block.setPreviousStatement(true);
    block.setNextStatement(true);
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

Blockly.Blocks.madeup.STATEMENT_HUE = 210;
Blockly.Blocks.madeup.INTEGER_HUE = 45;
Blockly.Blocks.madeup.ARRAY_HUE = 90;
Blockly.Blocks.madeup.STRING_HUE = 260;
Blockly.Blocks.madeup.REAL_HUE = 160;
Blockly.Blocks.madeup.NUMBER_HUE = 180;
Blockly.Blocks.madeup.BOOLEAN_HUE = 20;
Blockly.Blocks.madeup.STRING_HUE = 180;
Blockly.Blocks.madeup.LOOP_HUE = 330;
Blockly.Blocks.madeup.UNKNOWN_TYPE_HUE = 330;

// ----------------------------------------------------------------------------
// https://blockly-demo.appspot.com/static/demos/blockfactory/index.html#23r8y7
Blockly.Blocks['madeup_math_sign'] = {
  init: function() {
    this.setHelpUrl('http://www.example.com/');
    this.setColour(Blockly.Blocks.madeup.NUMBER_HUE);
    this.appendValueInput("X").appendField("sign");
    this.setInputsInline(true);
    this.setOutput(true);
    this.setTooltip('');
  },
  customContextMenu: toggleStatementExpression,
  mutationToDom: mutationToDom,
  domToMutation: domModeToMutation
};

Blockly.Blocks['madeup_math_abs'] = {
  init: function() {
    this.setHelpUrl('http://www.example.com/');
    this.setColour(Blockly.Blocks.madeup.NUMBER_HUE);
    this.appendValueInput("X").appendField("abs");
    this.setInputsInline(true);
    this.setOutput(true);
    this.setTooltip('');
  },
  customContextMenu: toggleStatementExpression,
  mutationToDom: mutationToDom,
  domToMutation: domModeToMutation
};

// ----------------------------------------------------------------------------
// https://blockly-demo.appspot.com/static/demos/blockfactory/index.html#4fn5wu
Blockly.Blocks['madeup_io_print'] = {
  init: function() {
    this.setHelpUrl('http://www.example.com/');
    this.setColour(Blockly.Blocks.madeup.STATEMENT_HUE);
    // this.appendValueInput("MESSAGE").setCheck(["Boolean", "Integer", "String", "Real"]).appendField("print"); 
    this.appendValueInput("MESSAGE").setCheck(null).appendField("print");
    this.setPreviousStatement(true);
    this.setNextStatement(true);
    this.setTooltip('');
  },
  customContextMenu: toggleStatementExpression,
  mutationToDom: mutationToDom,
  domToMutation: domModeToMutation
};

Blockly.Blocks['madeup_io_debug'] = {
  init: function() {
    this.setHelpUrl('http://www.example.com/');
    this.setColour(Blockly.Blocks.madeup.STATEMENT_HUE);
    this.appendValueInput("MESSAGE")
        .setCheck(["Boolean", "Integer", "String", "Real"])
        .appendField("debug");
    this.setPreviousStatement(true);
    this.setNextStatement(true);
    this.setTooltip('');
  },
  customContextMenu: toggleStatementExpression,
  mutationToDom: mutationToDom,
  domToMutation: domModeToMutation
};

// ----------------------------------------------------------------------------
// https://blockly-demo.appspot.com/static/demos/blockfactory/index.html#t9hp6h
Blockly.Blocks['madeup_math_integer'] = {
  init: function() {
    this.setHelpUrl('http://www.example.com/');
    this.setColour(Blockly.Blocks.madeup.INTEGER_HUE);
    this.appendDummyInput().appendField(new Blockly.FieldTextInput("0"), "INTEGER");
    this.setOutput(true, "Integer");
    this.setTooltip('');
  },
  customContextMenu: toggleStatementExpression,
  mutationToDom: mutationToDom,
  domToMutation: domModeToMutation
};

Blockly.Blocks['madeup_math_real'] = {
  init: function() {
    this.setHelpUrl('http://www.example.com/');
    this.setColour(Blockly.Blocks.madeup.REAL_HUE);
    this.appendDummyInput().appendField(new Blockly.FieldTextInput("0.0"), "REAL");
    this.setOutput(true, "Real");
    this.setTooltip('');
  },
  customContextMenu: toggleStatementExpression,
  mutationToDom: mutationToDom,
  domToMutation: domModeToMutation
};

Blockly.Blocks['madeup_nothing'] = {
  init: function() {
    this.setHelpUrl('http://www.example.com/');
    this.setColour(30);
    this.appendDummyInput().appendField("nothing");
    this.setOutput(true);
    this.setTooltip('');
  },
  customContextMenu: toggleStatementExpression,
  mutationToDom: mutationToDom,
  domToMutation: domModeToMutation
};

// ----------------------------------------------------------------------------
// https://blockly-demo.appspot.com/static/demos/blockfactory/index.html#y9urpv
Blockly.Blocks['madeup_logic_boolean'] = {
  init: function() {
    this.setHelpUrl('http://www.example.com/');
    this.setColour(Blockly.Blocks.madeup.BOOLEAN_HUE);
    this.appendDummyInput().appendField(new Blockly.FieldDropdown([["true", "TRUE"], ["false", "FALSE"]]), "BOOLEAN");
    this.setOutput(true, "Boolean");
    this.setTooltip('');
  },
  customContextMenu: toggleStatementExpression,
  mutationToDom: mutationToDom,
  domToMutation: domModeToMutation
};

// ----------------------------------------------------------------------------
// https://blockly-demo.appspot.com/static/demos/blockfactory/index.html#bcrxzp
Blockly.Blocks['madeup_movement_moveto'] = {
  init: function() {
    this.setHelpUrl('http://www.example.com/');
    this.setColour(Blockly.Blocks.madeup.STATEMENT_HUE);
    this.appendValueInput("X")
        .setCheck(["Integer", "Real"])
        .setAlign(Blockly.ALIGN_RIGHT)
        .appendField("moveto x");
    this.appendValueInput("Y")
        .setCheck(["Integer", "Real"])
        .setAlign(Blockly.ALIGN_RIGHT)
        .appendField("y");
    this.appendValueInput("Z")
        .setCheck(["Integer", "Real"])
        .setAlign(Blockly.ALIGN_RIGHT)
        .appendField("z");
    this.setPreviousStatement(true);
    this.setNextStatement(true);
    this.setTooltip('');
  },
  customContextMenu: toggleStatementExpression,
  mutationToDom: mutationToDom,
  domToMutation: domModeToMutation
};

// ----------------------------------------------------------------------------
// https://blockly-demo.appspot.com/static/demos/blockfactory/index.html#sq5jy6
Blockly.Blocks['madeup_movement_move'] = {
  init: function() {
    this.setHelpUrl('http://www.example.com/');
    this.setColour(Blockly.Blocks.madeup.STATEMENT_HUE);
    this.appendValueInput("DISTANCE")
        .setCheck(["Integer", "Real"])
        .setAlign(Blockly.ALIGN_RIGHT)
        .appendField("move");
    this.setPreviousStatement(true);
    this.setNextStatement(true);
    this.setTooltip('');
  },
  customContextMenu: toggleStatementExpression,
  mutationToDom: mutationToDom,
  domToMutation: domModeToMutation
};

// ----------------------------------------------------------------------------
// https://blockly-demo.appspot.com/static/demos/blockfactory/index.html#wbikby
Blockly.Blocks['madeup_loop_repeat'] = {
  init: function() {
    this.setHelpUrl('http://www.example.com/');
    this.setColour(Blockly.Blocks.madeup.LOOP_HUE);
    this.appendValueInput("COUNT")
        .setCheck("Integer")
        .appendField("repeat");
    this.appendDummyInput().appendField("times");
    this.appendStatementInput("BLOCK");
    this.setInputsInline(true);
    this.setPreviousStatement(true);
    this.setNextStatement(true);
    this.setTooltip('');
  },
  customContextMenu: toggleStatementExpression,
  mutationToDom: mutationToDom,
  domToMutation: domModeToMutation
};

// ----------------------------------------------------------------------------
// https://blockly-demo.appspot.com/static/demos/blockfactory/index.html#qau5xu
Blockly.Blocks['madeup_loop_repeatwich'] = {
  init: function() {
    this.appendValueInput("COUNT")
        .appendField("repeat");
    this.appendDummyInput()
        .appendField("times");
    this.appendStatementInput("SURROUNDER");
    this.appendStatementInput("SURROUNDEE")
        .appendField("around");
    this.setInputsInline(true);
    this.setPreviousStatement(true);
    this.setNextStatement(true);
    this.setColour(330);
    this.setTooltip('');
    this.setHelpUrl('http://www.example.com/');
  },
  customContextMenu: toggleStatementExpression,
  mutationToDom: mutationToDom,
  domToMutation: domModeToMutation
};

// ----------------------------------------------------------------------------
// https://blockly-demo.appspot.com/static/demos/blockfactory/index.html#uc9f7i
Blockly.Blocks['madeup_movement_turn'] = {
  init: function() {
    this.setHelpUrl('http://www.example.com/');
    this.setColour(Blockly.Blocks.madeup.STATEMENT_HUE);
    this.appendValueInput("DEGREES")
        .setAlign(Blockly.ALIGN_RIGHT)
        .appendField(new Blockly.FieldDropdown([["yaw", "YAW"], ["pitch", "PITCH"], ["roll", "ROLL"]]), "TYPE");
    this.setPreviousStatement(true);
    this.setNextStatement(true);
    this.setTooltip('');
  },
  customContextMenu: toggleStatementExpression,
  mutationToDom: mutationToDom,
  domToMutation: domModeToMutation
};

// ----------------------------------------------------------------------------
// https://blockly-demo.appspot.com/static/demos/blockfactory/index.html#v6t7gt
Blockly.Blocks['madeup_math_sincostan'] = {
  init: function() {
    this.setHelpUrl('http://www.example.com/');
    this.setColour(Blockly.Blocks.madeup.REAL_HUE);
    this.appendValueInput("DEGREES")
        .appendField(new Blockly.FieldDropdown([["sin", "SIN"], ["cos", "COS"], ["tan", "TAN"]]), "F");
    this.setInputsInline(true);
    this.setOutput(true);
    this.setTooltip('');
  },
  customContextMenu: toggleStatementExpression,
  mutationToDom: mutationToDom,
  domToMutation: domModeToMutation
};

Blockly.Blocks['madeup_math_inverse_sincostan'] = {
  init: function() {
    this.setHelpUrl('http://www.example.com/');
    this.setColour(Blockly.Blocks.madeup.REAL_HUE);
    this.appendValueInput("RATIO")
        .appendField(new Blockly.FieldDropdown([["asin", "ASIN"], ["acos", "ACOS"], ["atan", "ATAN"]]), "F");
    this.setInputsInline(true);
    this.setOutput(true);
    this.setTooltip('');
  },
  customContextMenu: toggleStatementExpression,
  mutationToDom: mutationToDom,
  domToMutation: domModeToMutation
};

// ----------------------------------------------------------------------------
// https://blockly-demo.appspot.com/static/demos/blockfactory/index.html#xxgqk9
Blockly.Blocks['madeup_logic_junction'] = {
  init: function() {
    this.setHelpUrl('http://www.example.com/');
    this.setColour(Blockly.Blocks.madeup.BOOLEAN_HUE);
    this.appendValueInput("A");
    this.appendValueInput("B")
        .appendField(new Blockly.FieldDropdown([["and", "AND"], ["or", "OR"]]), "F");
    this.setInputsInline(true);
    this.setOutput(true);
    this.setTooltip('');
  },
  customContextMenu: toggleStatementExpression,
  mutationToDom: mutationToDom,
  domToMutation: domModeToMutation
};

// ----------------------------------------------------------------------------
// https://blockly-demo.appspot.com/static/demos/blockfactory/index.html#nzh2h5
Blockly.Blocks['madeup_logic_not'] = {
  init: function() {
    this.setHelpUrl('http://www.example.com/');
    this.setColour(Blockly.Blocks.madeup.BOOLEAN_HUE);
    this.appendValueInput("A").appendField("not");
    this.setInputsInline(true);
    this.setOutput(true);
    this.setTooltip('');
  },
  customContextMenu: toggleStatementExpression,
  mutationToDom: mutationToDom,
  domToMutation: domModeToMutation
};

// ----------------------------------------------------------------------------
// https://blockly-demo.appspot.com/static/demos/blockfactory/index.html#3xq2c7
Blockly.Blocks['madeup_movement_center'] = {
  init: function() {
    this.setHelpUrl('http://www.example.com/');
    this.setColour(Blockly.Blocks.madeup.STATEMENT_HUE);
    this.appendDummyInput().appendField("center");
    this.setPreviousStatement(true);
    this.setNextStatement(true);
    this.setTooltip('');
  },
  customContextMenu: toggleStatementExpression,
  mutationToDom: mutationToDom,
  domToMutation: domModeToMutation
};

Blockly.Blocks['madeup_movement_identity'] = {
  init: function() {
    this.setHelpUrl('http://www.example.com/');
    this.setColour(Blockly.Blocks.madeup.STATEMENT_HUE);
    this.appendDummyInput().appendField("identity");
    this.setPreviousStatement(true);
    this.setNextStatement(true);
    this.setTooltip('');
  },
  customContextMenu: toggleStatementExpression,
  mutationToDom: mutationToDom,
  domToMutation: domModeToMutation
};

Blockly.Blocks['madeup_movement_reframe'] = {
  init: function() {
    this.setHelpUrl('http://www.example.com/');
    this.setColour(Blockly.Blocks.madeup.STATEMENT_HUE);
    this.appendDummyInput().appendField("reframe");
    this.setPreviousStatement(true);
    this.setNextStatement(true);
    this.setTooltip('');
  },
  customContextMenu: toggleStatementExpression,
  mutationToDom: mutationToDom,
  domToMutation: domModeToMutation
};

Blockly.Blocks['madeup_movement_push'] = {
  init: function() {
    this.setHelpUrl('http://www.example.com/');
    this.setColour(Blockly.Blocks.madeup.STATEMENT_HUE);
    this.appendDummyInput().appendField("push");
    this.setPreviousStatement(true);
    this.setNextStatement(true);
    this.setTooltip('');
  },
  customContextMenu: toggleStatementExpression,
  mutationToDom: mutationToDom,
  domToMutation: domModeToMutation
};

Blockly.Blocks['madeup_movement_pop'] = {
  init: function() {
    this.setHelpUrl('http://www.example.com/');
    this.setColour(Blockly.Blocks.madeup.STATEMENT_HUE);
    this.appendDummyInput().appendField("pop");
    this.setPreviousStatement(true);
    this.setNextStatement(true);
    this.setTooltip('');
  },
  customContextMenu: toggleStatementExpression,
  mutationToDom: mutationToDom,
  domToMutation: domModeToMutation
};

Blockly.Blocks['madeup_movement_reverse'] = {
  init: function() {
    this.setHelpUrl('http://www.example.com/');
    this.setColour(Blockly.Blocks.madeup.STATEMENT_HUE);
    this.appendDummyInput().appendField("reverse");
    this.setPreviousStatement(true);
    this.setNextStatement(true);
    this.setTooltip('');
  },
  customContextMenu: toggleStatementExpression,
  mutationToDom: mutationToDom,
  domToMutation: domModeToMutation
};

// Blockly.Blocks['madeup_io_where'] = {
  // init: function() {
    // this.setHelpUrl('http://www.example.com/');
    // this.setColour(Blockly.Blocks.madeup.STATEMENT_HUE);
    // this.appendDummyInput().appendField("where");
    // this.setPreviousStatement(true);
    // this.setNextStatement(true);
    // this.setTooltip('');
  // }
// };

Blockly.Blocks['madeup_generate_dowel'] = {
  init: function() {
    this.setHelpUrl('http://www.example.com/');
    this.setColour(Blockly.Blocks.madeup.STATEMENT_HUE);
    this.appendValueInput("MAXBEND")
              .setCheck(["Integer", "Real"])
              .appendField("dowel")
              .appendField("maxBend");
    this.setPreviousStatement(true);
    this.setNextStatement(true);
    this.setTooltip('');
  },
  customContextMenu: toggleStatementExpression,
  mutationToDom: mutationToDom,
  domToMutation: domModeToMutation
};

Blockly.Blocks['madeup_generate_tube'] = {
  init: function() {
    this.setHelpUrl('http://www.example.com/');
    this.setColour(Blockly.Blocks.madeup.STATEMENT_HUE);
    this.appendValueInput("MAXBEND")
              .setCheck(["Integer", "Real"])
              .appendField("tube")
              .appendField("maxBend");
    this.setPreviousStatement(true);
    this.setNextStatement(true);
    this.setTooltip('');
  },
  customContextMenu: toggleStatementExpression,
  mutationToDom: mutationToDom,
  domToMutation: domModeToMutation
};

Blockly.Blocks['madeup_generate_spheres'] = {
  init: function() {
    this.setHelpUrl('http://www.example.com/');
    this.setColour(Blockly.Blocks.madeup.STATEMENT_HUE);
    this.appendDummyInput().appendField("spheres");
    this.setPreviousStatement(true);
    this.setNextStatement(true);
    this.setTooltip('');
  },
  customContextMenu: toggleStatementExpression,
  mutationToDom: mutationToDom,
  domToMutation: domModeToMutation
};

Blockly.Blocks['madeup_generate_forget'] = {
  init: function() {
    this.setHelpUrl('http://www.example.com/');
    this.setColour(Blockly.Blocks.madeup.STATEMENT_HUE);
    this.appendDummyInput().appendField("forget");
    this.setPreviousStatement(true);
    this.setNextStatement(true);
    this.setTooltip('');
  },
  customContextMenu: toggleStatementExpression,
  mutationToDom: mutationToDom,
  domToMutation: domModeToMutation
};

Blockly.Blocks['madeup_generate_polygon'] = {
  init: function() {
    this.setHelpUrl('http://www.example.com/');
    this.setColour(Blockly.Blocks.madeup.STATEMENT_HUE);
    this.appendDummyInput().appendField('polygon');
    this.setPreviousStatement(true);
    this.setNextStatement(true);
    this.setTooltip('');
  },
  customContextMenu: toggleStatementExpression,
  mutationToDom: mutationToDom,
  domToMutation: domModeToMutation
};

Blockly.Blocks['madeup_generate_boxes'] = {
  init: function() {
    this.setHelpUrl('http://www.example.com/');
    this.setColour(Blockly.Blocks.madeup.STATEMENT_HUE);
    this.appendDummyInput().appendField("boxes");
    this.setPreviousStatement(true);
    this.setNextStatement(true);
    this.setTooltip('');
  },
  customContextMenu: toggleStatementExpression,
  mutationToDom: mutationToDom,
  domToMutation: domModeToMutation
};

// ----------------------------------------------------------------------------
// https://blockly-demo.appspot.com/static/demos/blockfactory/index.html#auj6x8
Blockly.Blocks['madeup_generate_extrude'] = {
  init: function() {
    this.setHelpUrl('http://www.example.com/');
    this.setColour(Blockly.Blocks.madeup.STATEMENT_HUE);
    this.appendValueInput("X")
        .setCheck(["Integer", "Real"])
        .setAlign(Blockly.ALIGN_RIGHT)
        .appendField("extrude x");
    this.appendValueInput("Y")
        .setCheck(["Integer", "Real"])
        .setAlign(Blockly.ALIGN_RIGHT)
        .appendField("y");
    this.appendValueInput("Z")
        .setCheck(["Integer", "Real"])
        .setAlign(Blockly.ALIGN_RIGHT)
        .appendField("z");
    this.appendValueInput("LENGTH")
        .setCheck(["Integer", "Real"])
        .setAlign(Blockly.ALIGN_RIGHT)
        .appendField("length");
    this.setPreviousStatement(true);
    this.setNextStatement(true);
    this.setTooltip('');
  },
  customContextMenu: toggleStatementExpression,
  mutationToDom: mutationToDom,
  domToMutation: domModeToMutation
};

Blockly.Blocks['madeup_generate_revolve'] = {
  init: function() {
    this.setHelpUrl('http://www.example.com/');
    this.setColour(Blockly.Blocks.madeup.STATEMENT_HUE);
    this.appendValueInput("X")
        .setCheck(["Integer", "Real"])
        .setAlign(Blockly.ALIGN_RIGHT)
        .appendField("revolve x");
    this.appendValueInput("Y")
        .setCheck(["Integer", "Real"])
        .setAlign(Blockly.ALIGN_RIGHT)
        .appendField("y");
    this.appendValueInput("Z")
        .setCheck(["Integer", "Real"])
        .setAlign(Blockly.ALIGN_RIGHT)
        .appendField("z");
    this.appendValueInput("DEGREES")
        .setCheck(["Integer", "Real"])
        .setAlign(Blockly.ALIGN_RIGHT)
        .appendField("degrees");
    this.setPreviousStatement(true);
    this.setNextStatement(true);
    this.setTooltip('');
  },
  customContextMenu: toggleStatementExpression,
  mutationToDom: mutationToDom,
  domToMutation: domModeToMutation
};

// ----------------------------------------------------------------------------
// https://blockly-demo.appspot.com/static/demos/blockfactory/index.html#r6an6p
Blockly.Blocks['madeup_generate_surface'] = {
  init: function() {
    this.setHelpUrl('http://www.example.com/');
    this.setColour(Blockly.Blocks.madeup.STATEMENT_HUE);
    this.appendValueInput("COLUMNS")
        .setCheck("Integer")
        .appendField("surface columns");
    this.appendValueInput("ROWS")
        .setCheck("Integer")
        .setAlign(Blockly.ALIGN_RIGHT)
        .appendField("rows");
    this.setPreviousStatement(true);
    this.setNextStatement(true);
    this.setTooltip('');
  },
  customContextMenu: toggleStatementExpression,
  mutationToDom: mutationToDom,
  domToMutation: domModeToMutation
};

// ----------------------------------------------------------------------------
// https://blockly-demo.appspot.com/static/demos/blockfactory/index.html#jijkvq
Blockly.Blocks['madeup_math_binary_arithmetic_operator'] = {
  init: function() {
    this.setHelpUrl('http://www.example.com/');
    this.setColour(Blockly.Blocks.madeup.NUMBER_HUE);
    this.appendValueInput("A")
        .setCheck(["Integer", "Real"]);
    this.appendDummyInput()
        .appendField(new Blockly.FieldDropdown([["+", "+"], ["-", "-"], ["*", "*"], ["/", "/"], ["//", "//"], ["%", "%"], ["^", "^"]]), "OPERATOR");
    this.appendValueInput("B")
        .setCheck(["Integer", "Real"]);
    this.setInputsInline(true);
    this.setOutput(true);
    this.setTooltip('');
  },
  customContextMenu: toggleStatementExpression,
  mutationToDom: mutationToDom,
  domToMutation: domModeToMutation
};

// ----------------------------------------------------------------------------
// https://blockly-demo.appspot.com/static/demos/blockfactory/index.html#z4b83u
Blockly.Blocks['madeup_math_relational_operator'] = {
  init: function() {
    this.setHelpUrl('http://www.example.com/');
    this.setColour(Blockly.Blocks.madeup.BOOLEAN_HUE);
    this.appendValueInput("A").setCheck(["Integer", "Real"]);
    this.appendDummyInput().appendField(new Blockly.FieldDropdown([[">", ">"], [">=", ">="], ["<", "<"], ["<=", "<="], ["==", "=="], ["!=", "!="]]), "OPERATOR");
    this.appendValueInput("B").setCheck(["Integer", "Real"]);
    this.setInputsInline(true);
    this.setOutput(true, "Boolean");
    this.setTooltip('');
  },
  customContextMenu: toggleStatementExpression,
  mutationToDom: mutationToDom,
  domToMutation: domModeToMutation
};

// ----------------------------------------------------------------------------
// https://blockly-demo.appspot.com/static/demos/blockfactory/index.html#9gcbbm
Blockly.Blocks['madeup_math_unary_operator'] = {
  init: function() {
    this.setHelpUrl('http://www.example.com/');
    this.setColour(Blockly.Blocks.madeup.NUMBER_HUE);
    this.appendValueInput("A")
        .setCheck(["Integer", "Real"])
        .appendField("-");
    this.setInputsInline(true);
    this.setOutput(true);
    this.setTooltip('');
  },
  customContextMenu: toggleStatementExpression,
  mutationToDom: mutationToDom,
  domToMutation: domModeToMutation
};

// ----------------------------------------------------------------------------
// https://blockly-demo.appspot.com/static/demos/blockfactory/index.html#ggzags
Blockly.Blocks['madeup_math_minmax'] = {
  init: function() {
    this.setHelpUrl('http://www.example.com/');
    this.setColour(Blockly.Blocks.madeup.NUMBER_HUE);
    this.appendValueInput("A")
        .setCheck(["Integer", "Real"])
        .appendField(new Blockly.FieldDropdown([["min", "MIN"], ["max", "MAX"]]), "F");
    this.appendValueInput("B");
    this.setOutput(true, ["Integer", "Real"]);
    this.setTooltip('');
  },
  customContextMenu: toggleStatementExpression,
  mutationToDom: mutationToDom,
  domToMutation: domModeToMutation
};

// ----------------------------------------------------------------------------
// https://blockly-demo.appspot.com/static/demos/blockfactory/index.html#xpfdgx
Blockly.Blocks['madeup_math_random'] = {
  init: function() {
    this.appendValueInput("MIN")
        .setCheck("Integer")
        .setAlign(Blockly.ALIGN_RIGHT)
        .appendField("random")
        .appendField("min");
    this.appendValueInput("MAX")
        .setCheck("Integer")
        .setAlign(Blockly.ALIGN_RIGHT)
        .appendField("max");
    this.setInputsInline(false);
    this.setOutput(true, "Integer");
    this.setColour(Blockly.Blocks.madeup.NUMBER_HUE);
    this.setTooltip('');
    this.setHelpUrl('http://www.example.com/');
  },
  customContextMenu: toggleStatementExpression,
  mutationToDom: mutationToDom,
  domToMutation: domModeToMutation
};

// ----------------------------------------------------------------------------
// https://blockly-demo.appspot.com/static/demos/blockfactory/index.html#a6zzhx
Blockly.Blocks['madeup_math_log'] = {
  init: function() {
    this.setHelpUrl('http://www.example.com/');
    this.setColour(Blockly.Blocks.madeup.REAL_HUE);
    this.appendValueInput("BASE")
        .setCheck(["Integer", "Real"])
        .setAlign(Blockly.ALIGN_RIGHT)
        .appendField("log")
        .appendField("base");
    this.appendValueInput("X")
        .setCheck(["Integer", "Real"])
        .setAlign(Blockly.ALIGN_RIGHT)
        .appendField("x");
    this.setInputsInline(false);
    this.setOutput(true, "Real");
    this.setTooltip('');
  },
  customContextMenu: toggleStatementExpression,
  mutationToDom: mutationToDom,
  domToMutation: domModeToMutation
};

// ----------------------------------------------------------------------------
// https://blockly-demo.appspot.com/static/demos/blockfactory/index.html#3a3bw8
Blockly.Blocks['madeup_logic_if_expr'] = {
  init: function() {
    this.setHelpUrl('http://www.example.com/');
    this.setColour(Blockly.Blocks.madeup.UNKNOWN_TYPE_HUE);
    this.appendValueInput("CONDITION")
        .setCheck("Boolean")
        .appendField("if");
    this.appendValueInput("THEN")
        .appendField("then");
    this.appendValueInput("ELSE")
        .appendField("else");
    this.setInputsInline(true);
    this.setOutput(true);
    this.setTooltip('');
  },
  customContextMenu: toggleStatementExpression,
  mutationToDom: mutationToDom,
  domToMutation: domModeToMutation
};

// ----------------------------------------------------------------------------
// https://blockly-demo.appspot.com/static/demos/blockfactory/index.html#ns4a7v
Blockly.Blocks['madeup_logic_if_statement'] = {
  init: function() {
    this.setHelpUrl('http://www.example.com/');
    this.setColour(Blockly.Blocks.madeup.UNKNOWN_TYPE_HUE);
    this.appendValueInput("CONDITION")
        .setCheck("Boolean")
        .appendField("if");
    this.appendStatementInput("THEN")
        .appendField("then");
    this.setInputsInline(true);
    this.setPreviousStatement(true);
    this.setNextStatement(true);
    this.setTooltip('');
  },
  customContextMenu: toggleStatementExpression,
  mutationToDom: mutationToDom,
  domToMutation: domModeToMutation
};

Blockly.Blocks['madeup_logic_if_else_statement'] = {
  init: function() {
    this.setHelpUrl('http://www.example.com/');
    this.setColour(Blockly.Blocks.madeup.UNKNOWN_TYPE_HUE);
    this.appendValueInput("CONDITION")
        .setCheck("Boolean")
        .appendField("if");
    this.appendStatementInput("THEN")
        .appendField("then");
    this.appendStatementInput("ELSE")
        .appendField("else");
    this.setInputsInline(true);
    this.setPreviousStatement(true);
    this.setNextStatement(true);
    this.setTooltip('');
  },
  customContextMenu: toggleStatementExpression,
  mutationToDom: mutationToDom,
  domToMutation: domModeToMutation
};

// ----------------------------------------------------------------------------
// https://blockly-demo.appspot.com/static/demos/blockfactory/index.html#rixzgc
Blockly.Blocks['madeup_movement_rotate'] = {
  init: function() {
    this.setHelpUrl('http://www.example.com/');
    this.setColour(Blockly.Blocks.madeup.STATEMENT_HUE);
    this.appendValueInput("X")
        .setCheck(["Integer", "Real"])
        .setAlign(Blockly.ALIGN_RIGHT)
        .appendField("rotate x");
    this.appendValueInput("Y")
        .setCheck(["Integer", "Real"])
        .setAlign(Blockly.ALIGN_RIGHT)
        .appendField("y");
    this.appendValueInput("Z")
        .setCheck(["Integer", "Real"])
        .setAlign(Blockly.ALIGN_RIGHT)
        .appendField("z");
    this.appendValueInput("DEGREES")
        .setCheck(["Integer", "Real"])
        .setAlign(Blockly.ALIGN_RIGHT)
        .appendField("degrees");
    this.setPreviousStatement(true);
    this.setNextStatement(true);
    this.setTooltip('');
  },
  customContextMenu: toggleStatementExpression,
  mutationToDom: mutationToDom,
  domToMutation: domModeToMutation
};

Blockly.Blocks['madeup_movement_scale'] = {
  init: function() {
    this.setHelpUrl('http://www.example.com/');
    this.setColour(Blockly.Blocks.madeup.STATEMENT_HUE);
    this.appendValueInput("X")
        .setCheck(["Integer", "Real"])
        .setAlign(Blockly.ALIGN_RIGHT)
        .appendField("scale x");
    this.appendValueInput("Y")
        .setCheck(["Integer", "Real"])
        .setAlign(Blockly.ALIGN_RIGHT)
        .appendField("y");
    this.appendValueInput("Z")
        .setCheck(["Integer", "Real"])
        .setAlign(Blockly.ALIGN_RIGHT)
        .appendField("z");
    this.setPreviousStatement(true);
    this.setNextStatement(true);
    this.setTooltip('');
  },
  customContextMenu: toggleStatementExpression,
  mutationToDom: mutationToDom,
  domToMutation: domModeToMutation
};

Blockly.Blocks['madeup_movement_translate'] = {
  init: function() {
    this.setHelpUrl('http://www.example.com/');
    this.setColour(Blockly.Blocks.madeup.STATEMENT_HUE);
    this.appendValueInput("X")
        .setCheck(["Integer", "Real"])
        .setAlign(Blockly.ALIGN_RIGHT)
        .appendField("translate x");
    this.appendValueInput("Y")
        .setCheck(["Integer", "Real"])
        .setAlign(Blockly.ALIGN_RIGHT)
        .appendField("y");
    this.appendValueInput("Z")
        .setCheck(["Integer", "Real"])
        .setAlign(Blockly.ALIGN_RIGHT)
        .appendField("z");
    this.setPreviousStatement(true);
    this.setNextStatement(true);
    this.setTooltip('');
  },
  customContextMenu: toggleStatementExpression,
  mutationToDom: mutationToDom,
  domToMutation: domModeToMutation
};

// ----------------------------------------------------------------------------
// https://blockly-demo.appspot.com/static/demos/blockfactory/index.html#6fi4ac
Blockly.Blocks['madeup_loop_while'] = {
  init: function() {
    this.setHelpUrl('http://www.example.com/');
    this.setColour(Blockly.Blocks.madeup.LOOP_HUE);
    this.appendValueInput("CONDITION")
        .setCheck("Boolean")
        .appendField("while");
    this.appendStatementInput("BODY");
    this.setInputsInline(true);
    this.setPreviousStatement(true);
    this.setNextStatement(true);
    this.setTooltip('');
  },
  customContextMenu: toggleStatementExpression,
  mutationToDom: mutationToDom,
  domToMutation: domModeToMutation
};

// ----------------------------------------------------------------------------
// https://blockly-demo.appspot.com/static/demos/blockfactory/index.html#sc7hwf
Blockly.Blocks['madeup_loop_for_to'] = {
  init: function() {
    this.setHelpUrl('http://www.example.com/');
    this.setColour(Blockly.Blocks.madeup.LOOP_HUE);
    this.appendValueInput("STOP")
        .setCheck("Integer")
        .appendField("for")
        .appendField(new Blockly.FieldVariable("i"), "ITERATOR")
        .appendField("to");
    this.appendStatementInput("BODY");
    this.setInputsInline(true);
    this.setPreviousStatement(true);
    this.setNextStatement(true);
    this.setTooltip('');
  },
  customContextMenu: toggleStatementExpression,
  mutationToDom: mutationToDom,
  domToMutation: domModeToMutation
};

Blockly.Blocks['madeup_loop_for_to_by'] = {
  init: function() {
    this.setHelpUrl('http://www.example.com/');
    this.setColour(Blockly.Blocks.madeup.LOOP_HUE);
    this.appendValueInput("STOP")
        .setCheck("Integer")
        .appendField("for")
        .appendField(new Blockly.FieldVariable("i"), "ITERATOR")
        .appendField("to");
    this.appendValueInput("BY")
        .setCheck("Integer")
        .appendField("by");
    this.appendStatementInput("BODY");
    this.setInputsInline(true);
    this.setPreviousStatement(true);
    this.setNextStatement(true);
    this.setTooltip('');
  },
  customContextMenu: toggleStatementExpression,
  mutationToDom: mutationToDom,
  domToMutation: domModeToMutation
};

Blockly.Blocks['madeup_loop_for_through'] = {
  init: function() {
    this.setHelpUrl('http://www.example.com/');
    this.setColour(Blockly.Blocks.madeup.LOOP_HUE);
    this.appendValueInput("STOP")
        .setCheck("Integer")
        .appendField("for")
        .appendField(new Blockly.FieldVariable("i"), "ITERATOR")
        .appendField("through");
    this.appendStatementInput("BODY");
    this.setInputsInline(true);
    this.setPreviousStatement(true);
    this.setNextStatement(true);
    this.setTooltip('');
  },
  customContextMenu: toggleStatementExpression,
  mutationToDom: mutationToDom,
  domToMutation: domModeToMutation
};

Blockly.Blocks['madeup_loop_for_through_by'] = {
  init: function() {
    this.setHelpUrl('http://www.example.com/');
    this.setColour(Blockly.Blocks.madeup.LOOP_HUE);
    this.appendValueInput("STOP")
        .setCheck("Integer")
        .appendField("for")
        .appendField(new Blockly.FieldVariable("i"), "ITERATOR")
        .appendField("through");
    this.appendValueInput("BY")
        .setCheck("Integer")
        .appendField("by");
    this.appendStatementInput("BODY");
    this.setInputsInline(true);
    this.setPreviousStatement(true);
    this.setNextStatement(true);
    this.setTooltip('');
  },
  customContextMenu: toggleStatementExpression,
  mutationToDom: mutationToDom,
  domToMutation: domModeToMutation
};

// ----------------------------------------------------------------------------
// https://blockly-demo.appspot.com/static/demos/blockfactory/index.html#5nv8s2
Blockly.Blocks['madeup_loop_for_in'] = {
  init: function() {
    this.setHelpUrl('http://www.example.com/');
    this.setColour(Blockly.Blocks.madeup.LOOP_HUE);
    this.appendValueInput("START")
        .setCheck("Integer")
        .appendField("for")
        .appendField(new Blockly.FieldVariable("i"), "ITERATOR")
        .appendField("in");
    this.appendValueInput("STOP")
        .setCheck("Integer")
        .appendField("..");
    this.appendStatementInput("BODY");
    this.setInputsInline(true);
    this.setPreviousStatement(true);
    this.setNextStatement(true);
    this.setTooltip('');
  },
  customContextMenu: toggleStatementExpression,
  mutationToDom: mutationToDom,
  domToMutation: domModeToMutation
};

// ----------------------------------------------------------------------------
// https://blockly-demo.appspot.com/static/demos/blockfactory/index.html#dd8846
Blockly.Blocks['madeup_loop_for_in_by'] = {
  init: function() {
    this.appendValueInput("START")
        .setCheck("Integer")
        .appendField("for")
        .appendField(new Blockly.FieldVariable("i"), "ITERATOR")
        .appendField("in");
    this.appendValueInput("STOP")
        .setCheck("Integer")
        .appendField("..");
    this.appendValueInput("BY")
        .setCheck("Integer")
        .appendField("by");
    this.appendStatementInput("BODY");
    this.setInputsInline(true);
    this.setPreviousStatement(true);
    this.setNextStatement(true);
    this.setColour(330);
    this.setTooltip('');
    this.setHelpUrl('http://www.example.com/');
  },
  customContextMenu: toggleStatementExpression,
  mutationToDom: mutationToDom,
  domToMutation: domModeToMutation
};

// ----------------------------------------------------------------------------
// https://blockly-demo.appspot.com/static/demos/blockfactory/index.html#34x6ss
Blockly.Blocks['madeup_string'] = {
  init: function() {
    this.setColour(Blockly.Blocks.madeup.STRING_HUE);
    this.appendDummyInput().appendField(new Blockly.FieldTextInput("text"), "STRING");
    this.setOutput(true);
    this.setTooltip('');
    this.setHelpUrl('http://www.example.com/');
  },
  customContextMenu: toggleStatementExpression,
  mutationToDom: mutationToDom,
  domToMutation: domModeToMutation
};

// ----------------------------------------------------------------------------
// https://blockly-demo.appspot.com/static/demos/blockfactory/index.html#c8vgu2
Blockly.Blocks['madeup_eval'] = {
  init: function() {
    this.setColour(Blockly.Blocks.madeup.STATEMENT_HUE);
    this.appendValueInput("EXPR").appendField("eval");
    this.setPreviousStatement(true);
    this.setNextStatement(true);
    this.setTooltip('');
    this.setHelpUrl('http://www.example.com/');
  },
  customContextMenu: toggleStatementExpression,
  mutationToDom: mutationToDom,
  domToMutation: domModeToMutation
};

// ----------------------------------------------------------------------------

Blockly.Blocks['madeup_array_literal'] = {
  /**
   * Block for creating a list with any number of elements of any type.
   * @this Blockly.Block
   */
  init: function() {
    // this.setHelpUrl(Blockly.Msg.LISTS_CREATE_WITH_HELPURL);
    this.setColour(Blockly.Blocks.madeup.ARRAY_HUE);
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
    domModeToMutation(xmlElement);
    this.itemCount_ = parseInt(xmlElement.getAttribute('items'), 10);
    this.updateShape_();
  },
  /**
   * Populate the mutator's dialog with this block's components.
   * @param {!Blockly.Workspace} workspace Mutator's workspace.
   * @return {!Blockly.Block} Root block in mutator.
   * @this Blockly.Block
   */
  decompose: function(workspace) {
    var containerBlock =
        Blockly.Block.obtain(workspace, 'madeup_array_create_with_container');
    containerBlock.initSvg();
    var connection = containerBlock.getInput('STACK').connection;
    for (var i = 0; i < this.itemCount_; i++) {
      var itemBlock = Blockly.Block.obtain(workspace, 'madeup_array_element');
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
    this.itemCount_ = connections.length;
    this.updateShape_();
    // Reconnect any child blocks.
    for (var i = 0; i < this.itemCount_; i++) {
      if (connections[i]) {
        this.getInput('ELEMENT' + i).connection.connect(connections[i]);
      }
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
    // Delete everything.
    if (this.getInput('EMPTY')) {
      this.removeInput('EMPTY');
    } else {
      var i = 0;
      while (this.getInput('ELEMENT' + i)) {
        this.removeInput('ELEMENT' + i);
        i++;
      }
    }
    // Rebuild block.
    if (this.itemCount_ == 0) {
      this.appendDummyInput('EMPTY')
          .appendField('empty');
    } else {
      for (var i = 0; i < this.itemCount_; i++) {
        var input = this.appendValueInput('ELEMENT' + i);
        if (i == 0) {
          input.appendField('array');
        }
      }
    }
  },
  customContextMenu: toggleStatementExpression,
  mutationToDom: mutationToDom,
  domToMutation: domModeToMutation
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

// ----------------------------------------------------------------------------
// Remove the return blocks that Blockly automatically generates in the
// PROCEDURES node of the toolbox. Madeup doesn't support explicit return
// statements.
Blockly.Blocks['procedures_ifreturn'] = null;
Blockly.Blocks['procedures_defreturn'] = null;

// Blockly makes the function definition blocks for us. Let's tweak them
// so they can change betweeen statements and expressions.
(function() {
  var oldContextMenu = Blockly.Blocks['procedures_defnoreturn'].customContextMenu;
  Blockly.Blocks['procedures_defnoreturn'].customContextMenu = function(options) {
    oldContextMenu.call(this, options);
    toggleStatementExpression.call(this, options);
  };

  var oldDomToMutation = Blockly.Blocks['procedures_defnoreturn'].domToMutation;
  Blockly.Blocks['procedures_defnoreturn'].domToMutation = function(xmlElement) {
    oldDomToMutation.call(this, xmlElement);
    domModeToMutation.call(this, xmlElement);
  };

  var oldMutationToDom = Blockly.Blocks['procedures_defnoreturn'].mutationToDom;
  Blockly.Blocks['procedures_defnoreturn'].mutationToDom = function() {
    var container = oldMutationToDom.call(this);
    mutationModeToDom(this, container);
    return container;
  };
})();

// Blockly makes the function call blocks for us. Let's tweak them so they can
// change betweeen statements and expressions.
(function() {
  var oldContextMenu = Blockly.Blocks['procedures_callnoreturn'].customContextMenu;
  Blockly.Blocks['procedures_callnoreturn'].customContextMenu = function(options) {
    oldContextMenu.call(this, options);
    toggleStatementExpression.call(this, options);
  };

  var oldDomToMutation = Blockly.Blocks['procedures_callnoreturn'].domToMutation;
  Blockly.Blocks['procedures_callnoreturn'].domToMutation = function(xmlElement) {
    oldDomToMutation.call(this, xmlElement);
    domModeToMutation.call(this, xmlElement);
  };

  var oldMutationToDom = Blockly.Blocks['procedures_callnoreturn'].mutationToDom;
  Blockly.Blocks['procedures_callnoreturn'].mutationToDom = function() {
    var container = oldMutationToDom.call(this);
    mutationModeToDom(this, container);
    return container;
  };
})();
