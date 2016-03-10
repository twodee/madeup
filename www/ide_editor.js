var textEditor = ace.edit("textEditor");
textEditor.$blockScrolling = Infinity;
var Range = ace.require('ace/range').Range;

// Creates an undo manager implicitly.
textEditor.setSession(ace.createEditSession('', 'ace/mode/madeup'));

textEditor.setTheme("ace/theme/twilight");
textEditor.setHighlightSelectedWord(false);
textEditor.setHighlightActiveLine(false);

textEditor.commands.addCommand({
  'name' : 'solidify',
  'bindKey' : { 'win' : 'Ctrl-,', 'mac' : 'Ctrl-,' },
  'exec' : function(editor) {
    run(getSource(), GeometryMode.SURFACE);
  },
  'readOnly' : true
});

textEditor.commands.addCommand({
  'name' : 'pathify',
  'bindKey' : { 'win' : 'Ctrl-.', 'mac' : 'Ctrl-.' },
  'exec' : function(editor) {
    run(getSource(), GeometryMode.PATH);
  },
  'readOnly' : true
});

textEditor.commands.addCommand({
  'name' : 'save',
  'bindKey' : { 'win' : 'Ctrl-S', 'mac' : 'Ctrl-S' },
  'exec' : function(editor) {
    save();
  },
  'readOnly' : true
});

textEditor.commands.addCommand({
  'name' : 'fit',
  'bindKey' : { 'win' : 'Ctrl-/', 'mac' : 'Ctrl-/' },
  'exec' : function(editor) {
    fit();
  },
  'readOnly' : true
});
