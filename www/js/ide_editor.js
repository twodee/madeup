var textEditor = ace.edit("textEditor");
textEditor.$blockScrolling = Infinity;
var Range = ace.require('ace/range').Range;

// Creates an undo manager implicitly.
textEditor.setSession(ace.createEditSession('', 'ace/mode/madeup'));

textEditor.setTheme("ace/theme/twilight");
textEditor.setHighlightSelectedWord(false);
textEditor.setHighlightActiveLine(false);
textEditor.setOption("tabSize", 2);
textEditor.setOption("useSoftTabs", true);

// https://github.com/ajaxorg/ace/blob/master/lib/ace/commands/default_commands.js
// https://ace.c9.io/demo/keyboard_shortcuts.html
// textEditor.commands.removeCommand(['togglecomment', 'gotoline', 'showSettingsMenu']);
textEditor.commands.bindKey('ctrl-/', null);
textEditor.commands.bindKey('ctrl-shift-?', null);
textEditor.commands.bindKey('ctrl-,', null);
textEditor.commands.bindKey('ctrl-\'', 'togglecomment');
