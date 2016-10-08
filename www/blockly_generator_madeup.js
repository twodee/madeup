/**
 * @license
 * Visual Blocks Language
 *
 * Copyright 2012 Google Inc.
 * https://developers.google.com/blockly/
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *   http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

/**
 * @fileoverview Helper functions for generating Python for blocks.
 * @author fraser@google.com (Neil Fraser)
 */
'use strict';

goog.require('Blockly.Generator');

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

