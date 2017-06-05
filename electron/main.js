const {app, Menu, BrowserWindow, globalShortcut, dialog} = require('electron');
const path = require('path');
const url = require('url');
const LocalStorage = require('node-localstorage').LocalStorage;

let localStorage = new LocalStorage(require('os').homedir() + '/.madeup');

function recall(key, default_value) {
  let value = localStorage.getItem(key);
  return value === null ? default_value : value;
}

function recallInt(key, default_value) {
  let value = localStorage.getItem(key);
  return value === null ? default_value : parseInt(value);
}

function createWindow(mup) {
  // Restore settings or their defaults.
  let width = recallInt('window_width', 800);
  let height = recallInt('window_height', 600);
  let showTools = recallInt('show_tools', 0) == 1;

  var frame = new BrowserWindow({
    width: width,
    height: height,
    icon: path.join(__dirname, 'assets/icons/png/1024x1024.png')
  })

  frame.loadURL(url.format({
    pathname: path.join(__dirname, 'index.html'),
    protocol: 'file:',
    slashes: true
  }))

  if (showTools) {
    frame.webContents.openDevTools({mode: 'bottom'});
  }

  if (mup != undefined) {
    frame.webContents.on('did-finish-load', () => {
      frame.webContents.send('open', mup);
    });
  }

  frame.on('close', () => {
    let size = frame.getSize();
    localStorage.setItem('window_width', size[0]);
    localStorage.setItem('window_height', size[1]);
    localStorage.setItem('show_tools', frame.webContents.isDevToolsOpened() ? 1 : 0);
  });

  return frame;
}

app.on('ready', function() {
	const template = [
		{
			label: 'File',
			submenu: [
        {
          label: 'New',
          accelerator: 'CommandOrControl+N',
          click() {
            createWindow();
          },
        },
        {
          label: 'Open',
          accelerator: 'CommandOrControl+O',
          click() {
            dialog.showOpenDialog({
              title: 'Open...',
            }, function(path) {
              if (path) {
                // load(path[0]); 
                createWindow(path[0]);
              }
            });
          },
        },
        {
          label: 'Save',
          accelerator: 'CommandOrControl+S',
          click(item, focusedWindow) {
            focusedWindow.webContents.send('save');
          },
        },
        {
          label: 'Save As...',
          accelerator: 'Shift+CommandOrControl+S',
          click(item, focusedWindow) {
            focusedWindow.webContents.send('saveAs');
          },
        },
        {
          type: 'separator'
        },
        {
          role: 'close'
        },
			]
		},
		{
			label: 'Edit',
			submenu: [
				{role: 'undo'},
				{role: 'redo'},
				{type: 'separator'},
				{role: 'cut'},
				{role: 'copy'},
				{role: 'paste'},
				{role: 'pasteandmatchstyle'},
				{role: 'delete'},
				{role: 'selectall'}
			]
		},
		{
			label: 'View',
			submenu: [
				{role: 'reload'},
				{role: 'forcereload'},
				{role: 'toggledevtools'},
				{type: 'separator'},
				{role: 'resetzoom'},
				{role: 'zoomin'},
				{role: 'zoomout'},
				{type: 'separator'},
				{role: 'togglefullscreen'}
			]
		},
	];

  if (process.platform === 'darwin') {
    const name = app.getName();
    template.unshift({
      label: name,
      submenu: [
        {
          label: `About ${name}`,
          role: 'about',
        },
        { type: 'separator' },
        {
          label: 'Services',
          role: 'services',
          submenu: [],
        },
        { type: 'separator' },
        {
          label: `Hide ${name}`,
          accelerator: 'Command+H',
          role: 'hide',
        },
        {
          label: 'Hide Others',
          accelerator: 'Command+Alt+H',
          role: 'hideothers',
        },
        {
          label: 'Show All',
          role: 'unhide',
        },
        { type: 'separator' },
        {
          label: `Quit ${name}`,
          accelerator: 'Command+Q',
          click() { app.quit(); },
        },
      ],
    });
  }

	const menu = Menu.buildFromTemplate(template);
	Menu.setApplicationMenu(menu);

  var mups = null;
  if (process.argv[0].endsWith('lectron')) {
    var mups = process.argv.slice(2);
  } else {
    var mups = process.argv.slice(1);
  }

  if (mups.length > 0) {
    mups.forEach((mup) => createWindow(mup));
  } else {
    createWindow();
  }
});

app.on('window-all-closed', () => {
  app.quit();
});
