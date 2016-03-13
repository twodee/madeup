var textEditor = ace.edit("textEditor");
textEditor.$blockScrolling = Infinity;
var Range = ace.require('ace/range').Range;

// Creates an undo manager implicitly.
textEditor.setSession(ace.createEditSession('', 'ace/mode/madeup'));

textEditor.setTheme("ace/theme/twilight");
textEditor.setHighlightSelectedWord(false);
textEditor.setHighlightActiveLine(false);

textEditor.commands.removeCommand('togglecomment');
textEditor.commands.removeCommand('gotoline');
