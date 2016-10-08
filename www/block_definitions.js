var EXPRESSION_HUE = 210;
var LITERAL_HUE = 315;
var STATEMENT_HUE = 180;

var block_definitions = {
  'madeup_tube': {
    config:
      {
        "message0": "tube maxBend %1",
        "args0": [
          { "type": "input_value", "align": "RIGHT", "name": "MAXBEND", "check": ["Real", "Integer"] },
        ],
        "inputsInline": false,
        "previousStatement": null,
        "nextStatement": null,
        "colour": STATEMENT_HUE,
        "tooltip": "",
        "helpUrl": "http://www.example.com/"
      },
    generator:
      function (block) {
        var value_maxBend = Blockly.Madeup.valueToCode(block, 'MAXBEND', Blockly.Madeup.ORDER_FUNCTION_CALL_ONLY_PARAMETER);
        var code = 'tube maxBend:' + value_maxBend;
        return generateInMode(block, code, Blockly.Madeup.ORDER_ATOMIC);
      }
  },
  'madeup_spheres': {
    config:
      {
        "message0": "spheres",
        "previousStatement": null,
        "nextStatement": null,
        "colour": STATEMENT_HUE,
        "tooltip": "",
        "helpUrl": "http://www.example.com/"
      },
    generator:
      function (block) {
        var code = 'spheres';
        return generateInMode(block, code, Blockly.Madeup.ORDER_ATOMIC);
      }
  },
  'madeup_forget': {
    config:
      {
        "message0": "forget",
        "previousStatement": null,
        "nextStatement": null,
        "colour": STATEMENT_HUE,
        "tooltip": "",
        "helpUrl": "http://www.example.com/"
      },
    generator:
      function (block) {
        var code = 'forget';
        return generateInMode(block, code, Blockly.Madeup.ORDER_ATOMIC);
      }
  },
  'madeup_polygon': {
    config:
      {
        "message0": "polygon",
        "previousStatement": null,
        "nextStatement": null,
        "colour": STATEMENT_HUE,
        "tooltip": "",
        "helpUrl": "http://www.example.com/"
      },
    generator:
      function (block) {
        var code = 'polygon';
        return generateInMode(block, code, Blockly.Madeup.ORDER_ATOMIC);
      }
  },
  'madeup_boxes': {
    config:
      {
        "message0": "boxes",
        "previousStatement": null,
        "nextStatement": null,
        "colour": STATEMENT_HUE,
        "tooltip": "",
        "helpUrl": "http://www.example.com/"
      },
    generator:
      function (block) {
        var code = 'boxes';
        return generateInMode(block, code, Blockly.Madeup.ORDER_ATOMIC);
      }
  },
  'madeup_extrude': {
    config:
      {
        "message0": "extrude x %1 y %2 z %3 length %4",
        "args0": [
          { "type": "input_value", "align": "RIGHT", "name": "X", "check": ["Real", "Integer"] },
          { "type": "input_value", "align": "RIGHT", "name": "Y", "check": ["Real", "Integer"] },
          { "type": "input_value", "align": "RIGHT", "name": "Z", "check": ["Real", "Integer"] },
          { "type": "input_value", "align": "RIGHT", "name": "LENGTH", "check": ["Real", "Integer"] },
        ],
        "inputsInline": false,
        "previousStatement": null,
        "nextStatement": null,
        "colour": STATEMENT_HUE,
        "tooltip": "",
        "helpUrl": "http://www.example.com/"
      },
    generator:
      function (block) {
        var value_x = Blockly.Madeup.valueToCode(block, 'X', Blockly.Madeup.ORDER_FUNCTION_CALL_FIRST_PARAMETER);
        var value_y = Blockly.Madeup.valueToCode(block, 'Y', Blockly.Madeup.ORDER_FUNCTION_CALL_NOT_FIRST_PARAMETER);
        var value_z = Blockly.Madeup.valueToCode(block, 'Z', Blockly.Madeup.ORDER_FUNCTION_CALL_NOT_FIRST_PARAMETER);
        var value_length = Blockly.Madeup.valueToCode(block, 'LENGTH', Blockly.Madeup.ORDER_FUNCTION_CALL_NOT_FIRST_PARAMETER);
        var code = 'extrude ' + value_x + ', ' + value_y + ', ' + value_z + ', ' + value_length;
        return generateInMode(block, code, Blockly.Madeup.ORDER_ATOMIC);
      }
  },
  'madeup_revolve': {
    config:
      {
        "message0": "revolve x %1 y %2 z %3 length %4",
        "args0": [
          { "type": "input_value", "align": "RIGHT", "name": "X", "check": ["Real", "Integer"] },
          { "type": "input_value", "align": "RIGHT", "name": "Y", "check": ["Real", "Integer"] },
          { "type": "input_value", "align": "RIGHT", "name": "Z", "check": ["Real", "Integer"] },
          { "type": "input_value", "align": "RIGHT", "name": "DEGREES", "check": ["Real", "Integer"] },
        ],
        "inputsInline": false,
        "previousStatement": null,
        "nextStatement": null,
        "colour": STATEMENT_HUE,
        "tooltip": "",
        "helpUrl": "http://www.example.com/"
      },
    generator:
      function (block) {
        var value_x = Blockly.Madeup.valueToCode(block, 'X', Blockly.Madeup.ORDER_FUNCTION_CALL_FIRST_PARAMETER);
        var value_y = Blockly.Madeup.valueToCode(block, 'Y', Blockly.Madeup.ORDER_FUNCTION_CALL_NOT_FIRST_PARAMETER);
        var value_z = Blockly.Madeup.valueToCode(block, 'Z', Blockly.Madeup.ORDER_FUNCTION_CALL_NOT_FIRST_PARAMETER);
        var value_degrees = Blockly.Madeup.valueToCode(block, 'DEGREES', Blockly.Madeup.ORDER_FUNCTION_CALL_NOT_FIRST_PARAMETER);
        var code = 'revolve ' + value_x + ', ' + value_y + ', ' + value_z + ', ' + value_degrees;
        return generateInMode(block, code, Blockly.Madeup.ORDER_ATOMIC);
      }
  },
  'madeup_unary_operator': {
    config:
      {
        "message0": "-%1",
        "args0": [
          { "type": "input_value", "name": "A", "check": ["Integer", "Real"] },
        ],
        "inputsInline": true,
        "output": ["Integer", "Real"],
        "colour": EXPRESSION_HUE,
        "tooltip": "",
        "helpUrl": "http://www.example.com/"
      },
    generator:
      function (block) {
        var value_a = Blockly.Madeup.valueToCode(block, 'A', Blockly.Madeup.ORDER_UNARY_NEGATION);
        var code = '-' + value_a;
        return generateInMode(block, code, Blockly.Madeup.ORDER_UNARY_NEGATION);
      }
  },
  'madeup_minmax': {
    config:
      {
        "message0": "%1 %2 %3",
        "args0": [
          {
            "type": "field_dropdown",
            "name": "FUNCTION",
            "options": [
              [ "min", "MIN" ],
              [ "max", "MAX" ],
            ]
          },
          { "type": "input_value", "name": "A", "check": ["Integer", "Real"] },
          { "type": "input_value", "name": "B", "check": ["Integer", "Real"] }
        ],
        "inputsInline": false,
        "output": ["Integer", "Real"],
        "colour": EXPRESSION_HUE,
        "tooltip": "",
        "helpUrl": "http://www.example.com/"
      },
    generator:
      function (block) {
        var value_a = Blockly.Madeup.valueToCode(block, 'A', Blockly.Madeup.ORDER_FUNCTION_CALL_FIRST_PARAMETER);
        var dropdown_f = block.getFieldValue('FUNCTION');
        var value_b = Blockly.Madeup.valueToCode(block, 'B', Blockly.Madeup.ORDER_FUNCTION_CALL_NOT_FIRST_PARAMETER);
        var code = dropdown_f.toLowerCase() + ' ' + value_a + ', ' + value_b;
        return generateInMode(block, code, Blockly.Madeup.ORDER_FUNCTION_CALL);
      }
  },
  'madeup_random': {
    config:
      {
        "message0": "random min %1 max %2",
        "args0": [
          { "type": "input_value", "align": "RIGHT", "name": "MIN", "check": ["Integer"] },
          { "type": "input_value", "align": "RIGHT", "name": "MAX", "check": ["Integer"] }
        ],
        "inputsInline": false,
        "output": ["Integer"],
        "colour": EXPRESSION_HUE,
        "tooltip": "",
        "helpUrl": "http://www.example.com/"
      },
    generator:
      function (block) {
        var value_min = Blockly.Madeup.valueToCode(block, 'MIN', Blockly.Madeup.ORDER_FUNCTION_CALL_FIRST_PARAMETER);
        var value_max = Blockly.Madeup.valueToCode(block, 'MAX', Blockly.Madeup.ORDER_FUNCTION_CALL_NOT_FIRST_PARAMETER);
        var code = 'random ' + value_min + ', ' + value_max;
        return generateInMode(block, code, Blockly.Madeup.ORDER_FUNCTION_CALL);
      }
  },
  'madeup_atan2': {
    config:
      {
        "message0": "atan2 opposite %1 adjacent %2",
        "args0": [
          { "type": "input_value", "align": "RIGHT", "name": "OPPOSITE", "check": ["Integer", "Real"] },
          { "type": "input_value", "align": "RIGHT", "name": "ADJACENT", "check": ["Integer", "Real"] }
        ],
        "inputsInline": false,
        "output": ["Real", "Integer"],
        "colour": EXPRESSION_HUE,
        "tooltip": "",
        "helpUrl": "http://www.example.com/"
      },
    generator:
      function (block) {
        var value_opposite = Blockly.Madeup.valueToCode(block, 'OPPOSITE', Blockly.Madeup.ORDER_ATOMIC);
        var value_adjacent = Blockly.Madeup.valueToCode(block, 'ADJACENT', Blockly.Madeup.ORDER_ATOMIC);
        var code = 'atan2 ' + value_opposite + ', ' + value_adjacent;
        return generateInMode(block, code, Blockly.Madeup.ORDER_ATOMIC);
      }
  },
  'madeup_log': {
    config:
      {
        "message0": "log base %1 x %2",
        "args0": [
          { "type": "input_value", "align": "RIGHT", "name": "BASE", "check": ["Integer", "Real"] },
          { "type": "input_value", "align": "RIGHT", "name": "X", "check": ["Integer", "Real"] }
        ],
        "inputsInline": false,
        "output": ["Real"],
        "colour": EXPRESSION_HUE,
        "tooltip": "",
        "helpUrl": "http://www.example.com/"
      },
    generator:
      function (block) {
        var value_base = Blockly.Madeup.valueToCode(block, 'BASE', Blockly.Madeup.ORDER_FUNCTION_CALL_FIRST_PARAMETER);
        var value_x = Blockly.Madeup.valueToCode(block, 'X', Blockly.Madeup.ORDER_FUNCTION_CALL_NOT_FIRST_PARAMETER);
        var code = 'log ' + value_base + ', ' + value_x;
        return generateInMode(block, code, Blockly.Madeup.ORDER_FUNCTION_CALL);
      }
  },
  'madeup_if_expr': {
    config:
      {
        "message0": "if %1 then %2 else %3",
        "args0": [
          { "type": "input_value", "name": "CONDITION", "check": ["Boolean"] },
          { "type": "input_value", "name": "THEN" },
          { "type": "input_value", "name": "ELSE" }
        ],
        "inputsInline": true,
        "output": null,
        "colour": EXPRESSION_HUE,
        "tooltip": "",
        "helpUrl": "http://www.example.com/"
      },
    generator:
      function (block) {
        var value_condition = Blockly.Madeup.valueToCode(block, 'CONDITION', Blockly.Madeup.ORDER_NONE);
        var value_then = Blockly.Madeup.valueToCode(block, 'THEN', Blockly.Madeup.ORDER_CONDITIONAL);
        var value_else = Blockly.Madeup.valueToCode(block, 'ELSE', Blockly.Madeup.ORDER_CONDITIONAL);
        var code = 'if ' + value_condition + ' then ' + value_then + ' else ' + value_else;
        return generateInMode(block, code, Blockly.Madeup.ORDER_CONDITIONAL);
      }
  },
  'madeup_if_statement': {
    config:
      {
        "message0": "if %1 then %2",
        "args0": [
          { "type": "input_value", "name": "CONDITION", "check": "Boolean" },
          { "type": "input_statement", "name": "THEN" }
        ],
        "inputsInline": true,
        "previousStatement": null,
        "nextStatement": null,
        "colour": STATEMENT_HUE,
        "tooltip": "",
        "helpUrl": "http://www.example.com/"
      },
    generator:
      function (block) {
        var value_condition = Blockly.Madeup.valueToCode(block, 'CONDITION', Blockly.Madeup.ORDER_CONDITIONAL);
        var statements_then = Blockly.Madeup.statementToCode(block, 'THEN');
        var code = 'if ' + value_condition + '\n' + statements_then + 'end';
        return generateInMode(block, code, Blockly.Madeup.ORDER_ATOMIC);
      }
  },
  'madeup_if_else_statement': {
    config:
      {
        "message0": "if %1 then %2 else %3",
        "args0": [
          { "type": "input_value", "name": "CONDITION", "check": "Boolean" },
          { "type": "input_statement", "name": "THEN" },
          { "type": "input_statement", "name": "ELSE" },
        ],
        "inputsInline": true,
        "previousStatement": null,
        "nextStatement": null,
        "colour": STATEMENT_HUE,
        "tooltip": "",
        "helpUrl": "http://www.example.com/"
      },
    generator:
      function (block) {
        var value_condition = Blockly.Madeup.valueToCode(block, 'CONDITION', Blockly.Madeup.ORDER_CONDITIONAL);
        var statements_then = Blockly.Madeup.statementToCode(block, 'THEN');
        var statements_else = Blockly.Madeup.statementToCode(block, 'ELSE');
        var code = 'if ' + value_condition + '\n' + statements_then + 'else\n' + statements_else + 'end';
        return generateInMode(block, code, Blockly.Madeup.ORDER_ATOMIC);
      }
  },
  'madeup_rotate': {
    config:
      {
        "message0": "rotate x %1 y %2 z %3 degrees %4",
        "args0": [
          { "type": "input_value", "align": "RIGHT", "name": "X", "check": ["Real", "Integer"] },
          { "type": "input_value", "align": "RIGHT", "name": "Y", "check": ["Real", "Integer"] },
          { "type": "input_value", "align": "RIGHT", "name": "Z", "check": ["Real", "Integer"] },
          { "type": "input_value", "align": "RIGHT", "name": "DEGREES", "check": ["Real", "Integer"] },
        ],
        "inputsInline": false,
        "previousStatement": null,
        "nextStatement": null,
        "colour": STATEMENT_HUE,
        "tooltip": "",
        "helpUrl": "http://www.example.com/"
      },
    generator:
      function (block) {
        var value_x = Blockly.Madeup.valueToCode(block, 'X', Blockly.Madeup.ORDER_FUNCTION_CALL_FIRST_PARAMETER);
        var value_y = Blockly.Madeup.valueToCode(block, 'Y', Blockly.Madeup.ORDER_FUNCTION_CALL_NOT_FIRST_PARAMETER);
        var value_z = Blockly.Madeup.valueToCode(block, 'Z', Blockly.Madeup.ORDER_FUNCTION_CALL_NOT_FIRST_PARAMETER);
        var value_degrees = Blockly.Madeup.valueToCode(block, 'DEGREES', Blockly.Madeup.ORDER_FUNCTION_CALL_NOT_FIRST_PARAMETER);
        var code = 'rotate ' + value_x + ', ' + value_y + ', ' + value_z + ', ' + value_degrees;
        return generateInMode(block, code, Blockly.Madeup.ORDER_ATOMIC);
      }
  },
  'madeup_scale': {
    config:
      {
        "message0": "scale x %1 y %2 z %3",
        "args0": [
          { "type": "input_value", "align": "RIGHT", "name": "X", "check": ["Real", "Integer"] },
          { "type": "input_value", "align": "RIGHT", "name": "Y", "check": ["Real", "Integer"] },
          { "type": "input_value", "align": "RIGHT", "name": "Z", "check": ["Real", "Integer"] }
        ],
        "inputsInline": false,
        "previousStatement": null,
        "nextStatement": null,
        "colour": STATEMENT_HUE,
        "tooltip": "",
        "helpUrl": "http://www.example.com/"
      },
    generator:
      function (block) {
        var value_x = Blockly.Madeup.valueToCode(block, 'X', Blockly.Madeup.ORDER_FUNCTION_CALL_FIRST_PARAMETER);
        var value_y = Blockly.Madeup.valueToCode(block, 'Y', Blockly.Madeup.ORDER_FUNCTION_CALL_NOT_FIRST_PARAMETER);
        var value_z = Blockly.Madeup.valueToCode(block, 'Z', Blockly.Madeup.ORDER_FUNCTION_CALL_NOT_FIRST_PARAMETER);
        var code = 'scale ' + value_x + ', ' + value_y + ', ' + value_z;
        return generateInMode(block, code, Blockly.Madeup.ORDER_ATOMIC);
      }
  },
  'madeup_translate': {
    config:
      {
        "message0": "translate x %1 y %2 z %3",
        "args0": [
          { "type": "input_value", "align": "RIGHT", "name": "X", "check": ["Real", "Integer"] },
          { "type": "input_value", "align": "RIGHT", "name": "Y", "check": ["Real", "Integer"] },
          { "type": "input_value", "align": "RIGHT", "name": "Z", "check": ["Real", "Integer"] }
        ],
        "inputsInline": false,
        "previousStatement": null,
        "nextStatement": null,
        "colour": STATEMENT_HUE,
        "tooltip": "",
        "helpUrl": "http://www.example.com/"
      },
    generator:
      function (block) {
        var value_x = Blockly.Madeup.valueToCode(block, 'X', Blockly.Madeup.ORDER_FUNCTION_CALL_FIRST_PARAMETER);
        var value_y = Blockly.Madeup.valueToCode(block, 'Y', Blockly.Madeup.ORDER_FUNCTION_CALL_NOT_FIRST_PARAMETER);
        var value_z = Blockly.Madeup.valueToCode(block, 'Z', Blockly.Madeup.ORDER_FUNCTION_CALL_NOT_FIRST_PARAMETER);
        var code = 'translate ' + value_x + ', ' + value_y + ', ' + value_z;
        return generateInMode(block, code, Blockly.Madeup.ORDER_ATOMIC);
      }
  },
  'madeup_string': {
    config:
      {
        "message0": "%1",
        "args0": [
          { "type": "field_input", "name": "STRING", "text": "text" }
        ],
        "inputsInline": false,
        "output": "String",
        "colour": LITERAL_HUE,
        "tooltip": "",
        "helpUrl": "http://www.example.com/"
      },
    generator:
      function (block) {
        var text_string = block.getFieldValue('STRING');
        var code = '"' + text_string + '"';
        return generateInMode(block, code, Blockly.Madeup.ORDER_ATOMIC);
      }
  },
  'madeup_sign': {
    config:
      {
        "message0": "sign %1",
        "args0": [
          { "type": "input_value", "name": "X", "check": ["Integer", "Real"] }
        ],
        "inputsInline": true,
        "output": "Number",
        "colour": EXPRESSION_HUE,
        "tooltip": "",
        "helpUrl": "http://www.example.com/"
      },
    generator:
      function (block) {
        var value_x = Blockly.Madeup.valueToCode(block, 'X', Blockly.Madeup.ORDER_FUNCTION_CALL_ONLY_PARAMETER);
        var code = 'sign ' + value_x;
        return generateInMode(block, code, Blockly.Madeup.ORDER_FUNCTION_CALL);
      }
  },
  'madeup_abs': {
    config:
      {
        "message0": "abs %1",
        "args0": [
          { "type": "input_value", "name": "X", "check": ["Integer", "Real"] }
        ],
        "inputsInline": true,
        "output": "Number",
        "colour": EXPRESSION_HUE,
        "tooltip": "",
        "helpUrl": "http://www.example.com/"
      },
    generator:
      function (block) {
        var value_x = Blockly.Madeup.valueToCode(block, 'X', Blockly.Madeup.ORDER_NONE);
        var code = '|' + value_x + '|';
        return generateInMode(block, code, Blockly.Madeup.ORDER_ATOMIC);
      }
  },
  'madeup_binary_arithmetic_operator': {
    config:
      {
        "message0": "%1 %2 %3",
        "args0": [
          { "type": "input_value", "name": "A", "check": ["Integer", "Real"] },
          {
            "type": "field_dropdown",
            "name": "OPERATOR",
            "options": [
              [ "+", "+" ],
              [ "-", "-" ],
              [ "*", "*" ],
              [ "/", "/" ],
              [ "//", "//" ],
              [ "%", "%" ]
            ]
          },
          { "type": "input_value", "name": "B", "check": ["Integer", "Real"] }
        ],
        "inputsInline": true,
        "output": ["Integer", "Real"],
        "colour": EXPRESSION_HUE,
        "tooltip": "",
        "helpUrl": "http://www.example.com/"
      },
    generator:
      function (block) {
        var dropdown_operator = block.getFieldValue('OPERATOR');

        var precedence;
        if (dropdown_operator == '+' || dropdown_operator == '-') {
          precedence = Blockly.Madeup.ORDER_ADDITIVE;
        } else if (dropdown_operator == '*' || dropdown_operator == '/' || dropdown_operator == '//' || dropdown_operator == '%') {
          precedence = Blockly.Madeup.ORDER_MULTIPLICATIVE;
        } else {
          precedence = Blockly.Madeup.ORDER_EXPONENTIATION;
        } 

        var value_a = Blockly.Madeup.valueToCode(block, 'A', precedence);
        var value_b = Blockly.Madeup.valueToCode(block, 'B', precedence);
        var code = value_a + ' ' + dropdown_operator + ' ' + value_b;
        return generateInMode(block, code, precedence);
      }
  },
  'madeup_relational_operator': {
    config:
      {
        "message0": "%1 %2 %3",
        "args0": [
          { "type": "input_value", "name": "A", "check": ["Integer", "Real"] },
          {
            "type": "field_dropdown",
            "name": "OPERATOR",
            "options": [
              [ ">", ">" ],
              [ ">=", ">=" ],
              [ "<", "<" ],
              [ "<=", "<=" ],
              [ "==", "==" ],
              [ "!=", "!=" ]
            ]
          },
          { "type": "input_value", "name": "B", "check": ["Integer", "Real"] }
        ],
        "inputsInline": true,
        "output": "Boolean",
        "colour": EXPRESSION_HUE,
        "tooltip": "",
        "helpUrl": "http://www.example.com/"
      },
    generator:
      function (block) {
        var dropdown_operator = block.getFieldValue('OPERATOR');
        var precedence;
        if (dropdown_operator == '==' || dropdown_operator == '!=') {
          precedence = Blockly.Madeup.ORDER_EQUALITY;
        } else {
          precedence = Blockly.Madeup.ORDER_RELATIONAL;
        }
        var value_a = Blockly.Madeup.valueToCode(block, 'A', precedence);
        var value_b = Blockly.Madeup.valueToCode(block, 'B', precedence);
        var code = value_a + ' ' + dropdown_operator + ' ' + value_b;
        return generateInMode(block, code, precedence);
      }
  },
  'madeup_print': {
    config:
      {
        "message0": "print %1",
        "colour": STATEMENT_HUE,
        "args0": [ { "type": "input_value", "name": "MESSAGE" } ],
        "inputsInline": false,
        "previousStatement": null,
        "nextStatement": null,
        "tooltip": "",
        "helpUrl": "http://www.example.com/"
      },
    generator:
      function (block) {
        var value_message = Blockly.Madeup.valueToCode(block, 'MESSAGE', Blockly.Madeup.ORDER_FUNCTION_CALL_ONLY_PARAMETER);
        var code = 'print ' + value_message;
        return generateInMode(block, code, Blockly.Madeup.ORDER_ATOMIC);
      }
  },
  'madeup_debug': {
    config:
      {
        "message0": "debug %1",
        "colour": STATEMENT_HUE,
        "args0": [ { "type": "input_value", "name": "MESSAGE" } ],
        "inputsInline": false,
        "previousStatement": null,
        "nextStatement": null,
        "tooltip": "",
        "helpUrl": "http://www.example.com/"
      },
    generator:
      function (block) {
        var value_message = Blockly.Madeup.valueToCode(block, 'MESSAGE', Blockly.Madeup.ORDER_FUNCTION_CALL_ONLY_PARAMETER);
        var code = 'debug ' + value_message;
        return generateInMode(block, code, Blockly.Madeup.ORDER_ATOMIC);
      }
  },
  'madeup_integer': {
    config:
      {
        "message0": "%1",
        "args0": [
          { "type": "field_input", "name": "INTEGER", "text": "0" }
        ],
        "inputsInline": false,
        "output": "Integer",
        "colour": LITERAL_HUE,
        "tooltip": "",
        "helpUrl": "http://www.example.com/"
      },
    generator:
      function (block) {
        var code = block.getFieldValue('INTEGER');
        return generateInMode(block, code, Blockly.Madeup.ORDER_ATOMIC);
      }
  },
  'madeup_real': {
    config:
      {
        "message0": "%1",
        "args0": [
          { "type": "field_input", "name": "REAL", "text": "0.0" }
        ],
        "inputsInline": false,
        "output": "Real",
        "colour": LITERAL_HUE,
        "tooltip": "",
        "helpUrl": "http://www.example.com/"
      },
    generator:
      function (block) {
        var code = block.getFieldValue('REAL');
        return generateInMode(block, code, Blockly.Madeup.ORDER_ATOMIC);
      }
  },
  'madeup_nothing': {
    config:
      {
        "message0": "nothing",
        "output": "Nothing",
        "colour": LITERAL_HUE,
        "tooltip": "",
        "helpUrl": "http://www.example.com/"
      },
    generator:
      function (block) {
        var code = 'nothing';
        return generateInMode(block, code, Blockly.Madeup.ORDER_ATOMIC);
      }
  },
  'madeup_boolean': {
    config:
      {
        "message0": "%1",
        "args0": [
          {
            "type": "field_dropdown",
            "name": "BOOLEAN",
            "options": [
              [ "true", "TRUE" ],
              [ "false", "FALSE" ]
            ]
          },
        ],
        "inputsInline": true,
        "output": "Boolean",
        "colour": LITERAL_HUE,
        "tooltip": "",
        "helpUrl": "http://www.example.com/"
      },
    generator:
      function (block) {
        var dropdown_boolean = block.getFieldValue('BOOLEAN');
        var code = dropdown_boolean.toLowerCase();
        return generateInMode(block, code, Blockly.Madeup.ORDER_ATOMIC);
      }
  },
  'madeup_moveto': {
    config:
      {
        "message0": "moveto x %1 y %2 z %3",
        "args0": [
          { "type": "input_value", "align": "RIGHT", "name": "X", "check": ["Real", "Integer"] },
          { "type": "input_value", "align": "RIGHT", "name": "Y", "check": ["Real", "Integer"] },
          { "type": "input_value", "align": "RIGHT", "name": "Z", "check": ["Real", "Integer"] }
        ],
        "inputsInline": false,
        "previousStatement": null,
        "nextStatement": null,
        "colour": STATEMENT_HUE,
        "tooltip": "",
        "helpUrl": "http://www.example.com/"
      },
    generator:
      function (block) {
        var value_x = Blockly.Madeup.valueToCode(block, 'X', Blockly.Madeup.ORDER_FUNCTION_CALL_FIRST_PARAMETER);
        var value_y = Blockly.Madeup.valueToCode(block, 'Y', Blockly.Madeup.ORDER_FUNCTION_CALL_NOT_FIRST_PARAMETER);
        var value_z = Blockly.Madeup.valueToCode(block, 'Z', Blockly.Madeup.ORDER_FUNCTION_CALL_NOT_FIRST_PARAMETER);
        var code = 'moveto ' + value_x + ', ' + value_y + ', ' + value_z;
        return generateInMode(block, code, Blockly.Madeup.ORDER_ATOMIC);
      }
  },
  'madeup_move': {
    config:
      {
        "message0": "move %1",
        "args0": [
          { "type": "input_value", "name": "DISTANCE", "check": ["Real", "Integer"] },
        ],
        "inputsInline": true,
        "previousStatement": null,
        "nextStatement": null,
        "colour": STATEMENT_HUE,
        "tooltip": "",
        "helpUrl": "http://www.example.com/"
      },
    generator:
      function (block) {
        var value_distance = Blockly.Madeup.valueToCode(block, 'DISTANCE', Blockly.Madeup.ORDER_FUNCTION_CALL_ONLY_PARAMETER);
        var code = 'move ' + value_distance;
        return generateInMode(block, code, Blockly.Madeup.ORDER_ATOMIC);
      }
  },
  'madeup_turn': {
    config:
      {
        "message0": "%1 %2",
        "args0": [
          {
            "type": "field_dropdown",
            "name": "FUNCTION",
            "options": [
              [ "yaw", "YAW" ],
              [ "pitch", "PITCH" ],
              [ "roll", "ROLL" ],
            ]
          },
          { "type": "input_value", "name": "DEGREES", "check": ["Real", "Integer"] },
        ],
        "inputsInline": true,
        "previousStatement": null,
        "nextStatement": null,
        "colour": STATEMENT_HUE,
        "tooltip": "",
        "helpUrl": "http://www.example.com/"
      },
    generator:
      function (block) {
        var value_degrees = Blockly.Madeup.valueToCode(block, 'DEGREES', Blockly.Madeup.ORDER_FUNCTION_CALL_ONLY_PARAMETER);
        var dropdown_type = block.getFieldValue('FUNCTION');
        var code = dropdown_type.toLowerCase() + ' ' + value_degrees;
        return generateInMode(block, code, Blockly.Madeup.ORDER_ATOMIC);
      }
  },
  'madeup_sincostan': {
    config:
      {
        "message0": "%1 %2",
        "args0": [
          {
            "type": "field_dropdown",
            "name": "FUNCTION",
            "options": [
              [ "sin", "SIN" ],
              [ "cos", "COS" ],
              [ "tan", "TAN" ],
            ]
          },
          { "type": "input_value", "name": "DEGREES", "check": ["Integer", "Real"] },
        ],
        "inputsInline": false,
        "output": ["Integer", "Real"],
        "colour": EXPRESSION_HUE,
        "tooltip": "",
        "helpUrl": "http://www.example.com/"
      },
    generator:
      function (block) {
        var value_degrees = Blockly.Madeup.valueToCode(block, 'DEGREES', Blockly.Madeup.ORDER_FUNCTION_CALL_ONLY_PARAMETER);
        var dropdown_f = block.getFieldValue('FUNCTION');
        var code = dropdown_f.toLowerCase() + ' ' + value_degrees;
        return generateInMode(block, code, Blockly.Madeup.ORDER_FUNCTION_CALL);
      }
  },
  'madeup_inverse_sincostan': {
    config:
      {
        "message0": "%1 %2",
        "args0": [
          {
            "type": "field_dropdown",
            "name": "FUNCTION",
            "options": [
              [ "asin", "ASIN" ],
              [ "acos", "ACOS" ],
              [ "atan", "ATAN" ],
            ]
          },
          { "type": "input_value", "name": "DEGREES", "check": ["Integer", "Real"] },
        ],
        "inputsInline": false,
        "output": ["Integer", "Real"],
        "colour": EXPRESSION_HUE,
        "tooltip": "",
        "helpUrl": "http://www.example.com/"
      },
    generator:
      function (block) {
        var value_ratio = Blockly.Madeup.valueToCode(block, 'RATIO', Blockly.Madeup.ORDER_FUNCTION_CALL_ONLY_PARAMETER);
        var dropdown_f = block.getFieldValue('FUNCTION');
        var code = dropdown_f.toLowerCase() + ' ' + value_ratio;
        return generateInMode(block, code, Blockly.Madeup.ORDER_FUNCTION_CALL);
      }
  },
  'madeup_binary_logic_operator': {
    config:
      {
        "message0": "%1 %2 %3",
        "args0": [
          { "type": "input_value", "name": "A", "check": ["Boolean"] },
          {
            "type": "field_dropdown",
            "name": "FUNCTION",
            "options": [
              [ "and", "AND" ],
              [ "or", "OR" ],
            ]
          },
          { "type": "input_value", "name": "B", "check": ["Boolean"] }
        ],
        "inputsInline": true,
        "output": ["Boolean"],
        "colour": EXPRESSION_HUE,
        "tooltip": "",
        "helpUrl": "http://www.example.com/"
      },
    generator:
      function (block) {
        var dropdown_f = block.getFieldValue('FUNCTION');
        var precedence;
        if (dropdown_f == 'and') {
          precedence = Blockly.Madeup.ORDER_LOGICAL_AND;
        } else {
          precedence = Blockly.Madeup.ORDER_LOGICAL_OR;
        }
        var value_a = Blockly.Madeup.valueToCode(block, 'A', precedence);
        var value_b = Blockly.Madeup.valueToCode(block, 'B', precedence);
        var code = value_a + ' ' + dropdown_f.toLowerCase() + ' ' + value_b;
        return generateInMode(block, code, precedence);
      }
  },
  'madeup_not': {
    config:
      {
        "message0": "not %1",
        "args0": [
          { "type": "input_value", "name": "A", "check": ["Boolean"] },
        ],
        "inputsInline": true,
        "output": ["Boolean"],
        "colour": EXPRESSION_HUE,
        "tooltip": "",
        "helpUrl": "http://www.example.com/"
      },
    generator:
      function (block) {
        var value_a = Blockly.Madeup.valueToCode(block, 'A', Blockly.Madeup.ORDER_LOGICAL_NOT);
        var code = 'not ' + value_a;
        return generateInMode(block, code, Blockly.Madeup.ORDER_LOGICAL_NOT);
      }
  },
  'madeup_center': {
    config:
      {
        "message0": "center",
        "previousStatement": null,
        "nextStatement": null,
        "colour": STATEMENT_HUE,
        "tooltip": "",
        "helpUrl": "http://www.example.com/"
      },
    generator:
      function (block) {
        var code = 'center';
        return generateInMode(block, code, Blockly.Madeup.ORDER_ATOMIC);
      }
  },
  'madeup_identity': {
    config:
      {
        "message0": "identity",
        "previousStatement": null,
        "nextStatement": null,
        "colour": STATEMENT_HUE,
        "tooltip": "",
        "helpUrl": "http://www.example.com/"
      },
    generator:
      function (block) {
        var code = 'identity';
        return generateInMode(block, code, Blockly.Madeup.ORDER_ATOMIC);
      }
  },
  'madeup_reframe': {
    config:
      {
        "message0": "reframe",
        "previousStatement": null,
        "nextStatement": null,
        "colour": STATEMENT_HUE,
        "tooltip": "",
        "helpUrl": "http://www.example.com/"
      },
    generator:
      function (block) {
        var code = 'reframe';
        return generateInMode(block, code, Blockly.Madeup.ORDER_ATOMIC);
      }
  },
  'madeup_push': {
    config:
      {
        "message0": "push",
        "previousStatement": null,
        "nextStatement": null,
        "colour": STATEMENT_HUE,
        "tooltip": "",
        "helpUrl": "http://www.example.com/"
      },
    generator:
      function (block) {
        var code = 'push';
        return generateInMode(block, code, Blockly.Madeup.ORDER_ATOMIC);
      }
  },
  'madeup_pop': {
    config:
      {
        "message0": "pop",
        "previousStatement": null,
        "nextStatement": null,
        "colour": STATEMENT_HUE,
        "tooltip": "",
        "helpUrl": "http://www.example.com/"
      },
    generator:
      function (block) {
        var code = 'pop';
        return generateInMode(block, code, Blockly.Madeup.ORDER_ATOMIC);
      }
  },
  'madeup_reverse': {
    config:
      {
        "message0": "reverse",
        "inputsInline": true,
        "previousStatement": null,
        "nextStatement": null,
        "colour": STATEMENT_HUE,
        "tooltip": "",
        "helpUrl": "http://www.example.com/"
      },
    generator:
      function (block) {
        var code = 'reverse';
        return generateInMode(block, code, Blockly.Madeup.ORDER_ATOMIC);
      }
  },
  'madeup_coalesce': {
    config:
      {
        "message0": "coalesce %1",
        "args0": [
          { "type": "input_value", "name": "THRESHOLD", "check": "Boolean" },
        ],
        "inputsInline": true,
        "previousStatement": null,
        "nextStatement": null,
        "colour": STATEMENT_HUE,
        "tooltip": "",
        "helpUrl": "http://www.example.com/"
      },
    generator:
      function (block) {
        var value_threshold = Blockly.Madeup.valueToCode(block, 'THRESHOLD', Blockly.Madeup.ORDER_ATOMIC);
        var code = 'coalesce ' + value_threshold;
        return generateInMode(block, code, Blockly.Madeup.ORDER_ATOMIC);
      }
  },
  'madeup_dowel': {
    config:
      {
        "message0": "dowel maxBend %1",
        "args0": [
          { "type": "input_value", "align": "RIGHT", "name": "MAXBEND", "check": ["Real", "Integer"] },
        ],
        "inputsInline": false,
        "previousStatement": null,
        "nextStatement": null,
        "colour": STATEMENT_HUE,
        "tooltip": "",
        "helpUrl": "http://www.example.com/"
      },
    generator:
      function (block) {
        var value_maxBend = Blockly.Madeup.valueToCode(block, 'MAXBEND', Blockly.Madeup.ORDER_FUNCTION_CALL_ONLY_PARAMETER);
        var code = 'dowel maxBend:' + value_maxBend;
        return generateInMode(block, code, Blockly.Madeup.ORDER_ATOMIC);
      }
  },
  'madeup_echo': {
    config:
      {
        "message0": "echo %1",
        "args0": [
          { "type": "input_value", "align": "RIGHT", "name": "OBJECT", "check": ["Path", "Mesh"] },
        ],
        "inputsInline": true,
        "previousStatement": null,
        "nextStatement": null,
        "colour": STATEMENT_HUE,
        "tooltip": "",
        "helpUrl": "http://www.example.com/"
      },
    generator:
      function (block) {
        var value_message = Blockly.Madeup.valueToCode(block, 'OBJECT', Blockly.Madeup.ORDER_FUNCTION_CALL_ONLY_PARAMETER);
        var code = 'echo ' + value_message;
        return generateInMode(block, code, Blockly.Madeup.ORDER_ATOMIC);
      }
  },
  'madeup_surface': {
    config:
      {
        "message0": "surface columns %1 rows %2",
        "args0": [
          { "type": "input_value", "align": "RIGHT", "name": "COLUMNS", "check": "Integer" },
          { "type": "input_value", "align": "RIGHT", "name": "ROWS", "check": "Integer" },
        ],
        "inputsInline": false,
        "previousStatement": null,
        "nextStatement": null,
        "colour": STATEMENT_HUE,
        "tooltip": "",
        "helpUrl": "http://www.example.com/"
      },
    generator:
      function (block) {
        var value_columns = Blockly.Madeup.valueToCode(block, 'COLUMNS', Blockly.Madeup.ORDER_FUNCTION_CALL_FIRST_PARAMETER);
        var value_rows = Blockly.Madeup.valueToCode(block, 'ROWS', Blockly.Madeup.ORDER_FUNCTION_CALL_NOT_FIRST_PARAMETER);
        var code = 'surface ' + value_columns + ', ' + value_rows;
        return generateInMode(block, code, Blockly.Madeup.ORDER_ATOMIC);
      }
  },
  'madeup_where': {
    config:
      {
        "message0": "where",
        "inputsInline": true,
        "output": "Array",
        "colour": LITERAL_HUE,
        "tooltip": "",
        "helpUrl": "http://www.example.com/"
      },
    generator:
      function (block) {
        var code = 'where';
        return generateInMode(block, code, Blockly.Madeup.ORDER_ATOMIC);
      }
  },
  'madeup_repeat': {
    config: 
      {
        "message0": "repeat %1 %2",
        "args0": [
          { "type": "input_value", "name": "COUNT", "check": "Integer" },
          { "type": "input_statement", "name": "BODY" }
        ],
        "inputsInline": true,
        "previousStatement": null,
        "nextStatement": null,
        "colour": STATEMENT_HUE,
        "tooltip": "",
        "helpUrl": "http://www.example.com/"
      },
    generator:
      function(block) {
        var value_count = Blockly.Madeup.valueToCode(block, 'COUNT', Blockly.Madeup.ORDER_NONE);
        var statements_block = Blockly.Madeup.statementToCode(block, 'BODY');
        var code = 'repeat ' + value_count + '\n' + statements_block + 'end';
        return generateInMode(block, code, Blockly.Madeup.ORDER_ATOMIC);
      }
  },
  'madeup_repeat_around': {
    config:
      {
        "message0": "repeat %1 %2 around %3",
        "args0": [
          { "type": "input_value", "name": "COUNT", "check": "Integer" },
          { "type": "input_statement", "name": "SURROUNDER" },
          { "type": "input_statement", "name": "SURROUNDEE" }
        ],
        "inputsInline": true,
        "previousStatement": null,
        "nextStatement": null,
        "colour": STATEMENT_HUE,
        "tooltip": "",
        "helpUrl": "http://www.example.com/"
      },
    generator:
      function(block) {
        var value_count = Blockly.Madeup.valueToCode(block, 'COUNT', Blockly.Madeup.ORDER_NONE);
        var surrounder_block = Blockly.Madeup.statementToCode(block, 'SURROUNDER');
        var surroundee_block = Blockly.Madeup.statementToCode(block, 'SURROUNDEE');
        var code = 'repeat ' + value_count + '\n' + surrounder_block + '\naround\n' + surroundee_block + 'end';
        return generateInMode(block, code, Blockly.Madeup.ORDER_ATOMIC);
      }
  },
  'madeup_while': {
    config:
      {
        "message0": "while %1 %2",
        "args0": [
          { "type": "input_value", "name": "CONDITION", "check": "Boolean" },
          { "type": "input_statement", "name": "BODY" },
        ],
        "inputsInline": true,
        "previousStatement": null,
        "nextStatement": null,
        "colour": STATEMENT_HUE,
        "tooltip": "",
        "helpUrl": "http://www.example.com/"
      },
    generator:
      function(block) {
        var value_condition = Blockly.Madeup.valueToCode(block, 'CONDITION', Blockly.Madeup.ORDER_NONE);
        var statements_body = Blockly.Madeup.statementToCode(block, 'BODY');
        var code = 'while ' + value_condition + '\n' + statements_body + 'end';
        return generateInMode(block, code, Blockly.Madeup.ORDER_ATOMIC);
      }
  },
  'madeup_for_upper': {
    config: 
      {
        "message0": "for %1 %2 %3 %4",
        "args0": [
          { "type": "input_value", "name": "ITERATOR" },
          {
            "type": "field_dropdown",
            "name": "CLUSIVITY",
            "options": [
              [ "to", "TO" ],
              [ "through", "through" ],
            ]
          },
          { "type": "input_value", "name": "STOP", "check": "Integer" },
          { "type": "input_statement", "name": "BODY" },
        ],
        "inputsInline": true,
        "previousStatement": null,
        "nextStatement": null,
        "colour": STATEMENT_HUE,
        "tooltip": "",
        "helpUrl": "http://www.example.com/"
      },
    generator: 
      function(block) {
        var value_stop = Blockly.Madeup.valueToCode(block, 'STOP', Blockly.Madeup.ORDER_NONE);
        var iterator = Blockly.Madeup.valueToCode(block, 'ITERATOR', Blockly.Madeup.ORDER_NONE);
        var clusivity = block.getFieldValue('CLUSIVITY').toLowerCase();
        var statements_body = Blockly.Madeup.statementToCode(block, 'BODY');
        var code = 'for ' + iterator + ' ' + clusivity + ' ' + value_stop + '\n' + statements_body + 'end';
        return generateInMode(block, code, Blockly.Madeup.ORDER_ATOMIC);
      }
  },
  'madeup_for_upper_by': {
    config: 
      {
        "message0": "for %1 %2 %3 by %4 %5",
        "args0": [
          { "type": "input_value", "name": "ITERATOR" },
          {
            "type": "field_dropdown",
            "name": "CLUSIVITY",
            "options": [
              [ "to", "TO" ],
              [ "through", "through" ],
            ]
          },
          { "type": "input_value", "name": "STOP", "check": "Integer" },
          { "type": "input_value", "name": "BY", "check": "Integer" },
          { "type": "input_statement", "name": "BODY" },
        ],
        "inputsInline": true,
        "previousStatement": null,
        "nextStatement": null,
        "colour": STATEMENT_HUE,
        "tooltip": "",
        "helpUrl": "http://www.example.com/"
      },
    generator: 
      function(block) {
        var value_stop = Blockly.Madeup.valueToCode(block, 'STOP', Blockly.Madeup.ORDER_NONE);
        var iterator = Blockly.Madeup.valueToCode(block, 'ITERATOR', Blockly.Madeup.ORDER_NONE);
        var value_by = Blockly.Madeup.valueToCode(block, 'BY', Blockly.Madeup.ORDER_NONE);
        var clusivity = block.getFieldValue('CLUSIVITY').toLowerCase();
        var statements_body = Blockly.Madeup.statementToCode(block, 'BODY');
        var code = 'for ' + iterator + ' ' + clusivity + ' ' + value_stop + ' by ' + value_by + '\n' + statements_body + 'end';
        return generateInMode(block, code, Blockly.Madeup.ORDER_ATOMIC);
      }
  },
  'madeup_for_in': {
    config: 
      {
        "message0": "for %1 in %2 .. %3 %4",
        "args0": [
          { "type": "input_value", "name": "ITERATOR" },
          { "type": "input_value", "name": "START", "check": "Integer" },
          { "type": "input_value", "name": "STOP", "check": "Integer" },
          { "type": "input_statement", "name": "BODY" },
        ],
        "inputsInline": true,
        "previousStatement": null,
        "nextStatement": null,
        "colour": STATEMENT_HUE,
        "tooltip": "",
        "helpUrl": "http://www.example.com/"
      },
    generator: 
      function(block) {
        var value_start = Blockly.Madeup.valueToCode(block, 'START', Blockly.Madeup.ORDER_NONE);
        var value_stop = Blockly.Madeup.valueToCode(block, 'STOP', Blockly.Madeup.ORDER_NONE);
        var iterator = Blockly.Madeup.valueToCode(block, 'ITERATOR', Blockly.Madeup.ORDER_NONE);
        var statements_body = Blockly.Madeup.statementToCode(block, 'BODY');
        var code = 'for ' + iterator + ' in ' + value_start + '..' + value_stop + '\n' + statements_body + 'end';
        return generateInMode(block, code, Blockly.Madeup.ORDER_ATOMIC);
      }
  },
  'madeup_for_in_by': {
    config: 
      {
        "message0": "for %1 in %2 .. %3 by %4 %5",
        "args0": [
          { "type": "input_value", "name": "ITERATOR" },
          { "type": "input_value", "name": "START", "check": "Integer" },
          { "type": "input_value", "name": "STOP", "check": "Integer" },
          { "type": "input_value", "name": "BY", "check": "Integer" },
          { "type": "input_statement", "name": "BODY" },
        ],
        "inputsInline": true,
        "previousStatement": null,
        "nextStatement": null,
        "colour": STATEMENT_HUE,
        "tooltip": "",
        "helpUrl": "http://www.example.com/"
      },
    generator: 
      function(block) {
        var value_start = Blockly.Madeup.valueToCode(block, 'START', Blockly.Madeup.ORDER_NONE);
        var value_stop = Blockly.Madeup.valueToCode(block, 'STOP', Blockly.Madeup.ORDER_NONE);
        var iterator = Blockly.Madeup.valueToCode(block, 'ITERATOR', Blockly.Madeup.ORDER_NONE);
        var value_by = Blockly.Madeup.valueToCode(block, 'BY', Blockly.Madeup.ORDER_NONE);
        var statements_body = Blockly.Madeup.statementToCode(block, 'BODY');
        var code = 'for ' + iterator + ' ' + value_start + '..' + value_stop + ' by ' + value_by + '\n' + statements_body + 'end';
        return generateInMode(block, code, Blockly.Madeup.ORDER_ATOMIC);
      }
  },
  'madeup_array_by': {
    config:
      {
        "message0": "%1 by %2",
        "args0": [
          { "type": "input_value", "align": "RIGHT", "name": "N", "check": ["Integer"] },
          { "type": "input_value", "align": "RIGHT", "name": "ITEM" }
        ],
        "inputsInline": true,
        "output": "Array",
        "colour": EXPRESSION_HUE,
        "tooltip": "",
        "helpUrl": "http://www.example.com/"
      },
    generator:
      function(block) {
        var value_outer = Blockly.Madeup.valueToCode(block, 'N', Blockly.Madeup.ORDER_ARRAY_BY);
        var value_inner = Blockly.Madeup.valueToCode(block, 'ITEM', Blockly.Madeup.ORDER_ARRAY_BY);
        var code = value_outer + ' by ' + value_inner;
        return generateInMode(block, code, Blockly.Madeup.ORDER_ARRAY_BY);
      }
  },
  'madeup_array_of': {
    config:
      {
        "message0": "%1 of %2",
        "args0": [
          { "type": "input_value", "align": "RIGHT", "name": "N", "check": ["Integer"] },
          { "type": "input_value", "align": "RIGHT", "name": "ITEM" }
        ],
        "inputsInline": true,
        "output": "Array",
        "colour": EXPRESSION_HUE,
        "tooltip": "",
        "helpUrl": "http://www.example.com/"
      },
    generator:
      function(block) {
        var value_outer = Blockly.Madeup.valueToCode(block, 'N', Blockly.Madeup.ORDER_ARRAY_BY);
        var value_inner = Blockly.Madeup.valueToCode(block, 'ITEM', Blockly.Madeup.ORDER_ARRAY_BY);
        var code = value_outer + ' of ' + value_inner;
        return generateInMode(block, code, Blockly.Madeup.ORDER_ARRAY_OF);
      }
  },
  'madeup_cross': {
    config:
      {
        "message0": "%1 cross %2",
        "args0": [
          { "type": "input_value", "align": "RIGHT", "name": "A", "check": "Array" },
          { "type": "input_value", "align": "RIGHT", "name": "B", "check": "Array" }
        ],
        "inputsInline": true,
        "output": "Array",
        "colour": EXPRESSION_HUE,
        "tooltip": "",
        "helpUrl": "http://www.example.com/"
      },
    generator:
      function(block) {
        var value_a = Blockly.Madeup.valueToCode(block, 'A', Blockly.Madeup.ORDER_ATOMIC);
        var value_b = Blockly.Madeup.valueToCode(block, 'B', Blockly.Madeup.ORDER_ATOMIC);
        var code = 'cross ' + a + ', ' + b;
        return generateInMode(block, code, Blockly.Madeup.ORDER_ATOMIC);
      }
  },
  'madeup_dot': {
    config:
      {
        "message0": "%1 dot %2",
        "args0": [
          { "type": "input_value", "align": "RIGHT", "name": "A", "check": "Array" },
          { "type": "input_value", "align": "RIGHT", "name": "B", "check": "Array" }
        ],
        "inputsInline": true,
        "output": "Array",
        "colour": EXPRESSION_HUE,
        "tooltip": "",
        "helpUrl": "http://www.example.com/"
      },
    generator:
      function(block) {
        var value_a = Blockly.Madeup.valueToCode(block, 'A', Blockly.Madeup.ORDER_ATOMIC);
        var value_b = Blockly.Madeup.valueToCode(block, 'B', Blockly.Madeup.ORDER_ATOMIC);
        var code = 'dot ' + a + ', ' + b;
        return generateInMode(block, code, Blockly.Madeup.ORDER_ATOMIC);
      }
  },
  'madeup_size': {
    config:
      {
        "message0": "size %1",
        "args0": [
          { "type": "input_value", "align": "RIGHT", "name": "A", "check": ["Array", "String"] },
        ],
        "inputsInline": true,
        "output": "Integer",
        "colour": EXPRESSION_HUE,
        "tooltip": "",
        "helpUrl": "http://www.example.com/"
      },
    generator:
      function(block) {
        var value_a = Blockly.Madeup.valueToCode(block, 'A', Blockly.Madeup.ORDER_ATOMIC);
        var code = 'size ' + a;
        return generateInMode(block, code, Blockly.Madeup.ORDER_ATOMIC);
      }
  },
  'madeup_magnitude': {
    config:
      {
        "message0": "magnitude %1",
        "args0": [
          { "type": "input_value", "align": "RIGHT", "name": "A", "check": "Array" },
        ],
        "inputsInline": true,
        "output": "Real",
        "colour": EXPRESSION_HUE,
        "tooltip": "",
        "helpUrl": "http://www.example.com/"
      },
    generator:
      function(block) {
        var value_a = Blockly.Madeup.valueToCode(block, 'A', Blockly.Madeup.ORDER_ATOMIC);
        var code = 'magnitude ' + a;
        return generateInMode(block, code, Blockly.Madeup.ORDER_ATOMIC);
      }
  },
  'madeup_normalize': {
    config:
      {
        "message0": "normalize %1",
        "args0": [
          { "type": "input_value", "align": "RIGHT", "name": "A", "check": "Array" },
        ],
        "inputsInline": true,
        "output": "Array",
        "colour": EXPRESSION_HUE,
        "tooltip": "",
        "helpUrl": "http://www.example.com/"
      },
    generator:
      function(block) {
        var value_a = Blockly.Madeup.valueToCode(block, 'A', Blockly.Madeup.ORDER_ATOMIC);
        var code = 'normalize ' + a;
        return generateInMode(block, code, Blockly.Madeup.ORDER_ATOMIC);
      }
  },
  'madeup_subscript': {
    config:
      {
        "message0": "item %1 of %2",
        "args0": [
          { "type": "input_value", "align": "RIGHT", "name": "I", "check": "Integer" },
          { "type": "input_value", "align": "RIGHT", "name": "COLLECTION", "check": "Array" },
        ],
        "inputsInline": true,
        "output": null,
        "colour": EXPRESSION_HUE,
        "tooltip": "",
        "helpUrl": "http://www.example.com/"
      },
    generator:
      function(block) {
        var value_i = Blockly.Madeup.valueToCode(block, 'I', Blockly.Madeup.ORDER_ATOMIC);
        var value_collection = Blockly.Madeup.valueToCode(block, 'COLLECTION', Blockly.Madeup.ORDER_ATOMIC);
        var code = value_collection + '[' + value_i + ']';
        return generateInMode(block, code, Blockly.Madeup.ORDER_ATOMIC);
      }
  },
  'madeup_subscript_set': {
    config:
      {
        "message0": "set item %1 of %2 to %3",
        "args0": [
          { "type": "input_value", "align": "RIGHT", "name": "I", "check": "Integer" },
          { "type": "input_value", "align": "RIGHT", "name": "COLLECTION", "check": "Array" },
          { "type": "input_value", "align": "RIGHT", "name": "RHS" },
        ],
        "inputsInline": true,
        "previousStatement": null,
        "nextStatement": null,
        "colour": STATEMENT_HUE,
        "tooltip": "",
        "helpUrl": "http://www.example.com/"
      },
    generator:
      function(block) {
        var value_i = Blockly.Madeup.valueToCode(block, 'I', Blockly.Madeup.ORDER_ATOMIC);
        var value_collection = Blockly.Madeup.valueToCode(block, 'COLLECTION', Blockly.Madeup.ORDER_ATOMIC);
        var value_rhs = Blockly.Madeup.valueToCode(block, 'RHS', Blockly.Madeup.ORDER_ATOMIC);
        var code = value_collection + '[' + value_i + '] = ' + value_rhs;
        return generateInMode(block, code, Blockly.Madeup.ORDER_ATOMIC);
      }
  },
  'madeup_subrange': {
    config:
      {
        "message0": "items %1 through %2 of %3",
        "args0": [
          { "type": "input_value", "align": "RIGHT", "name": "I", "check": "Integer" },
          { "type": "input_value", "align": "RIGHT", "name": "J", "check": "Integer" },
          { "type": "input_value", "align": "RIGHT", "name": "COLLECTION", "check": "Array" },
        ],
        "inputsInline": true,
        "output": "Array",
        "colour": EXPRESSION_HUE,
        "tooltip": "",
        "helpUrl": "http://www.example.com/"
      },
    generator:
      function(block) {
        var value_i = Blockly.Madeup.valueToCode(block, 'I', Blockly.Madeup.ORDER_ATOMIC);
        var value_j = Blockly.Madeup.valueToCode(block, 'J', Blockly.Madeup.ORDER_ATOMIC);
        var value_collection = Blockly.Madeup.valueToCode(block, 'COLLECTION', Blockly.Madeup.ORDER_ATOMIC);
        var code = value_collection + '[' + value_i + '..' + value_j + ']';
        return generateInMode(block, code, Blockly.Madeup.ORDER_ATOMIC);
      }
  },
}
