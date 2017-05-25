ace.define('ace/mode/madeup', ['require', 'exports', 'module' , 'ace/lib/oop', 'ace/mode/text', 'ace/tokenizer', 'ace/mode/madeup_highlight_rules'], function(require, exports, module) {

var oop = require("../lib/oop");
var TextMode = require("./text").Mode;
var Tokenizer = require("../tokenizer").Tokenizer;
var Range = ace.require('ace/range').Range;
var MadeupHighlightRules = require("./madeup_highlight_rules").MadeupHighlightRules;

var Mode = function() {
    this.HighlightRules = MadeupHighlightRules;
};
oop.inherits(Mode, TextMode);

(function() {
  this.lineCommentStart = "--";
    
  this.getNextLineIndent = function(state, line, tab) {
    var match = line.match(/^\s*/);
    var currentIndent = match[0];
    if (/^\s*(if|else|to|for|repeat|while|around)\b/.test(line)) {
      return currentIndent + '  ';
    } else {
      return currentIndent;
    }
  };

  // Do I need to fix the indent?
  this.checkOutdent = function(state, line, input) {
    return /^\s{2,}(en|els|aroun)\s*$/.test(line);
  };

  // Fix the indent.
  this.autoOutdent = function(state, doc, row) {
    var line = doc.getLine(row);
    var match = line.match(/^\s{2,}(end|else|around)\b/);
    if (match) {
      doc.replace(new Range(row, 0, row, 2), '');
    }
  };

//    // create worker for live syntax checking
//    this.createWorker = function(session) {
//        var worker = new WorkerClient(["ace"], "ace/mode/madeup_worker", "MadeupWorker");
//        worker.attachToDocument(session.getDocument());
//        worker.on("errors", function(e) {
//            session.setAnnotations(e.data);
//        });
//        return worker;
//    };
    
}).call(Mode.prototype);

exports.Mode = Mode;
});

ace.define('ace/mode/madeup_highlight_rules', ['require', 'exports', 'module' , 'ace/lib/oop', 'ace/mode/text_highlight_rules'], function(require, exports, module) {

var oop = require("../lib/oop");
var TextHighlightRules = require("./text_highlight_rules").TextHighlightRules;

var MadeupHighlightRules = function() {
  this.$rules = {
    "start" : [
      {
        token: "comment",
        regex: "---",
        next: "multiline_comment_allowed"
      },
      {
        token: "comment",
        regex: "--",
        next: "comment_allowed"
      },
      {
        token: "keyword.operator",
        regex: /[-+/%*]/,
        next: "start"
      },
      {
        token: "keyword",
        regex: /\b(?:repeat|return|not|of|around|while|end|for|end|else|if|through|and|or|to|in|by|then)\b/,
        next: "start"
      },
      {
        token: "range",
        regex: /\.\./,
        next: "start"
      },
      {
        token: "variable",
        regex: /(\.[a-zA-Z]|[a-zA-Z])(\w|\.\w)*/,
        next: "start"
      },
      {
        token: "string",
        regex: /"[^"]*"/,
        next: "start"
      },
      {
        token: "constant.numeric",
        regex: /-?\d+(?:\.\d+)?/,
        next: "start"
      }
    ],
    "comment_allowed" : [
      {
        token: "comment",
        regex: "$|^",
        next: "start"
      },
      {
        defaultToken: "comment"
      }
    ],
    "multiline_comment_allowed" : [
      {
        token: "comment",
        regex: "---",
        next: "start"
      },
      {
        defaultToken: "comment"
      }
    ]
  };
};

oop.inherits(MadeupHighlightRules, TextHighlightRules);

exports.MadeupHighlightRules = MadeupHighlightRules;

});
