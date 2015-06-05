'use strict';

goog.provide('Blockly.Blocks.madeup');
goog.require('Blockly.Blocks');

Blockly.Blocks.madeup.STATEMENT_HUE = 210;
Blockly.Blocks.madeup.INTEGER_HUE = 45;
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
  }
};

Blockly.Blocks['madeup_math_abs'] = {
  init: function() {
    this.setHelpUrl('http://www.example.com/');
    this.setColour(Blockly.Blocks.madeup.NUMBER_HUE);
    this.appendValueInput("X").appendField("abs");
    this.setInputsInline(true);
    this.setOutput(true);
    this.setTooltip('');
  }
};

// ----------------------------------------------------------------------------
// https://blockly-demo.appspot.com/static/demos/blockfactory/index.html#4fn5wu
Blockly.Blocks['madeup_io_print'] = {
  init: function() {
    this.setHelpUrl('http://www.example.com/');
    this.setColour(Blockly.Blocks.madeup.STATEMENT_HUE);
    this.appendValueInput("MESSAGE").setCheck(["Boolean", "Integer", "String", "Real"]).appendField("print");
    this.setPreviousStatement(true);
    this.setNextStatement(true);
    this.setTooltip('');
  }
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
  }
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
  }
};

Blockly.Blocks['madeup_math_real'] = {
  init: function() {
    this.setHelpUrl('http://www.example.com/');
    this.setColour(Blockly.Blocks.madeup.REAL_HUE);
    this.appendDummyInput().appendField(new Blockly.FieldTextInput("0.0"), "REAL");
    this.setOutput(true, "Real");
    this.setTooltip('');
  }
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
  }
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
  }
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
  }
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
  }
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
  }
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
  }
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
  }
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
  }
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
  }
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
  }
};

Blockly.Blocks['madeup_movement_identity'] = {
  init: function() {
    this.setHelpUrl('http://www.example.com/');
    this.setColour(Blockly.Blocks.madeup.STATEMENT_HUE);
    this.appendDummyInput().appendField("identity");
    this.setPreviousStatement(true);
    this.setNextStatement(true);
    this.setTooltip('');
  }
};

Blockly.Blocks['madeup_io_where'] = {
  init: function() {
    this.setHelpUrl('http://www.example.com/');
    this.setColour(Blockly.Blocks.madeup.STATEMENT_HUE);
    this.appendDummyInput().appendField("where");
    this.setPreviousStatement(true);
    this.setNextStatement(true);
    this.setTooltip('');
  }
};

Blockly.Blocks['madeup_generate_dowel'] = {
  init: function() {
    this.setHelpUrl('http://www.example.com/');
    this.setColour(Blockly.Blocks.madeup.STATEMENT_HUE);
    this.appendDummyInput().appendField("dowel");
    this.setPreviousStatement(true);
    this.setNextStatement(true);
    this.setTooltip('');
  }
};

Blockly.Blocks['madeup_generate_ball'] = {
  init: function() {
    this.setHelpUrl('http://www.example.com/');
    this.setColour(Blockly.Blocks.madeup.STATEMENT_HUE);
    this.appendDummyInput().appendField("ball");
    this.setPreviousStatement(true);
    this.setNextStatement(true);
    this.setTooltip('');
  }
};

Blockly.Blocks['madeup_generate_box'] = {
  init: function() {
    this.setHelpUrl('http://www.example.com/');
    this.setColour(Blockly.Blocks.madeup.STATEMENT_HUE);
    this.appendDummyInput().appendField("box");
    this.setPreviousStatement(true);
    this.setNextStatement(true);
    this.setTooltip('');
  }
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
  }
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
  }
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
  }
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
  }
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
  }
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
  }
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
  }
};

// ----------------------------------------------------------------------------
// https://blockly-demo.appspot.com/static/demos/blockfactory/index.html#a6zzhx
Blockly.Blocks['madeup_math_log'] = {
  init: function() {
    this.setHelpUrl('http://www.example.com/');
    this.setColour(Blockly.Blocks.madeup.REAL_HUE);
    this.appendValueInput("BASE")
        .setCheck(["Integer", "Real"])
        .appendField("log");
    this.appendValueInput("X")
        .setCheck(["Integer", "Real"]);
    this.setInputsInline(true);
    this.setOutput(true, "Real");
    this.setTooltip('');
  }
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
  }
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
  }
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
  }
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
  }
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
  }
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
  }
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
  }
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
  }
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
  }
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
  }
};
