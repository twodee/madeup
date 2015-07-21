var textEditor = ace.edit("textEditor");
textEditor.$blockScrolling = Infinity;
var Range = ace.require('ace/range').Range;
textEditor.setTheme("ace/theme/twilight");
textEditor.getSession().setMode("ace/mode/madeup");
textEditor.setHighlightSelectedWord(false);
textEditor.setHighlightActiveLine(false);
