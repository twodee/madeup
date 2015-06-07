var text_editor = ace.edit("text_editor");
text_editor.$blockScrolling = Infinity;
var Range = ace.require('ace/range').Range;
text_editor.setTheme("ace/theme/twilight");
text_editor.getSession().setMode("ace/mode/madeup");
text_editor.setHighlightSelectedWord(false);
text_editor.setHighlightActiveLine(false);
